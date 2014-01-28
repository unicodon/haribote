#include "StdAfx.h"
#include <math.h>
#include <stdio.h>
#include <OPENR/OPrimitiveControl.h>
#include <OPENR/OSyslog.h>
#include "VisionUtil.h"
#include "../State/State.h"
#include "../Motion/Inverter.h"

#define PAN_COEF 0.5637 // = tan(画面中央から右端までの角度)
#define TILT_COEF 0.4170 //


static byte cdt[128*128*128];

void YUV2RGB(byte Y,byte U,byte V,byte &R,byte &G,byte &B){
	int r,g,b;
	r = Y + U - 128;
	b = Y + V - 128;
	g = (int)((Y - 0.299*r - 0.114*b)/0.587);

	if(r < 0)R = 0;else if(r > 255)R = 255;else R = r;
	if(g < 0)G = 0;else if(g > 255)G = 255;else G = g;
	if(b < 0)B = 0;else if(b > 255)B = 255;else B = b;
}

void RGB2HSV(byte R,byte G,byte B,int &H,byte &S,byte &V){
	int cmax,cmin;
	double c;
	double h;
	if ( R >= G)   cmax = R; else cmax = G;
	if ( B > cmax) cmax = B;
	if ( R <= G)   cmin = R; else cmin = G;
	if ( B < cmin) cmin = B;
	V = cmax;
	c = cmax - cmin;
	if (cmax == 0) S = 0; else S = (byte)((255*c)/cmax);
	h = 0;
	if (S != 0){
		if (R == cmax){
			h = (G - B)/c;
		}else{
			if (G == cmax){
				h = 2 + (B - R)/c;
			}else{
				if (B == cmax) h = 4 + ( R - G)/c;
			}
		}
		H = (int)(h * 60);
		if (H < 0) H = H + 360;
	}

};

byte HSV2Reliability(int H,byte S,byte V, const ColorClusterParam param){

	double h_s = param.cov[HVAL];
	double s_s = param.cov[SVAL];
	double v_s = param.cov[VVAL];
	double h_e = param.ave[HVAL];
	double s_e = param.ave[SVAL];
	double v_e = param.ave[VVAL];
	double coef = param.coef;


	if(!(h_s && v_s && s_s))return 0;
	double dh = fabs(H - h_e);
	if(dh > 180) dh = 360 - dh;
	dh /= h_s;
	double ds = (S - s_e)/(double)s_s;
	double dv = (V - v_e)/(double)v_s;
	double r = sqrt(dh*dh+ds*ds+dv*dv);
	double value;

	value = 1 / (1 + exp( (r - 1)/coef))*(1+exp(-1/coef));//シグモイド
	value *= 256;

	/*	if(r >= 1){
		value = 0.5*(exp((1-r)/coef));
	}else{
		value = 0.5*(2 - exp((r-1)/coef));
	}
*/

	int ret = (int)value;
	if(ret > 255)ret = 255;
	if(ret < 0)ret = 0;

	return (byte)ret;
}

byte YUV2Reliability(byte Y,byte U,byte V,const ColorClusterParam param){
	byte R,G,B;
	int H;byte S;byte VAL;
	YUV2RGB(Y,U,V,R,G,B);
	RGB2HSV(R,G,B,H,S,VAL);
	return HSV2Reliability(H,S,VAL,param);
}


void MakeColorReliabilityTable(byte* table ,const ColorClusterParam param){
	if(!table){
		printf("Invalid Table!!\n");
		return;
	}
	for(int Y = 0;Y < 128;Y++){
		for(int U = 0;U < 128;U++){
			for(int V = 0;V < 128;V++){
				table[Y+U*128+V*128*128] = YUV2Reliability(Y*2,U*2,V*2,param);
			}
		}
	}
}

void ReviseImage(byte* image, longword whitebalance, longword shutterspeed){
	const int Yx = 108;
	const int Yy = 80;
	const int Ux = 108;
	const int Uy = 80;
	const int Vx = 103;
	const int Vy = 80;

	const int offset_array[9][3] = {
		//INDOOR_MODE
		{30,130,150},//Slow
		{30,120,150},//Mid
		{40,120,160},//Fast

		//OUTDOOR_MODE
		{30,120,160},
		{30,130,140},
		{30,130,150},

		//FL_MODE
		{30,120,160},
		{40,120,170},
		{30,110,190}
	};

	int offset_index = 0;
	switch(whitebalance){
		case ocamparamWB_INDOOR_MODE:
		default:
			offset_index = 0;
			break;
		case ocamparamWB_OUTDOOR_MODE:
			offset_index = 3;
			break;
		case ocamparamWB_FL_MODE:
			offset_index = 6;
			break;
	}

	switch(shutterspeed){
		case ocamparamSHUTTER_SLOW:
		default:
			break;
		case ocamparamSHUTTER_MID:
			offset_index += 1;
			break;
		case ocamparamSHUTTER_FAST:
			offset_index += 2;
			break;
	}

	int offset[3];
	memcpy(offset, offset_array[offset_index], 3*sizeof(int));

	static int coef[IMAGE_WIDTH*IMAGE_HEIGHT*3];//とりあえず1024倍してみよう
	static int flag = 1;
	if(flag){
		for(int x = 0;x < IMAGE_WIDTH;x++){
			for(int y = 0;y < IMAGE_HEIGHT;y++){
				double r;
				double d;
				r = sqrt((double)(x - Yx)*(x - Yx) + (y - Yy)*(y - Yy))/200;
				d = 1 - 3/(150-r);
				coef[(x+y*IMAGE_WIDTH)*3 + 0] = (int)(pow((r*r+1),1.5)*1024);
				r = sqrt((double)(x - Ux)*(x - Ux) + (y - Uy)*(y - Uy))/200;
				coef[(x+y*IMAGE_WIDTH)*3 + 1] = (int)(pow((r*r+1),1.5)*d*1024);
				r = sqrt((double)(x - Vx)*(x - Vx) + (y - Vy)*(y - Vy))/200;
				coef[(x+y*IMAGE_WIDTH)*3 + 2] = (int)(pow((r*r+1),1.5)*d*1024);
			}
		}
		flag = 0;
	}

	int tmp;
	int pos = 0;
	for(int i = 0;i < IMAGE_WIDTH*IMAGE_HEIGHT;i++)
	{
		tmp = offset[0] + (((image[pos + 0] - offset[0])*coef[pos + 0]) >> 10);if(tmp < 0)tmp = 0;if(tmp > 255)tmp = 255;
		image[pos + 0] = (BYTE)tmp;
		tmp = offset[1] + (((image[pos + 1] - offset[1])*coef[pos + 1]) >> 10);if(tmp < 0)tmp = 0;if(tmp > 255)tmp = 255;
		image[pos + 1] = (BYTE)tmp;
		tmp = offset[2] + (((image[pos + 2] - offset[2])*coef[pos + 2]) >> 10);if(tmp < 0)tmp = 0;if(tmp > 255)tmp = 255;
		image[pos + 2] = (BYTE)tmp;
		pos += 3;
	}
}

int SaveYUVPPMImage(const BYTE* image, const char* name){
	FILE* file;
	char buf[1024];
	sprintf(buf,"./MS/%s.PPM",name);
	printf(buf);
	file = fopen(buf,"wb");
	if(file){
		printf("open\n");
		char head[] = "P6\n208 160\n255\n";
		fwrite(head,1,strlen(head),file);

		fwrite(image,1,3*IMAGE_WIDTH*IMAGE_HEIGHT,file);
		fclose(file);
		return 1;
	}
	printf("save failed\n");
	return 0;
}

int SaveRGBPPMImage(const BYTE* image, const char* name){
	BYTE* buf = new BYTE[3*IMAGE_WIDTH*IMAGE_HEIGHT];
	for(int i = 0;i < IMAGE_WIDTH*IMAGE_HEIGHT*3;i+=3){
		YUV2RGB(image[i+0],image[i+1],image[i+2],buf[i+0],buf[i+1],buf[i+2]);
	}
	char str[1024];
	sprintf(str,"RGB%s",name);
	int res = SaveYUVPPMImage(buf,str);
	delete[] buf;

	return res;
}


static byte CDTColor[NUM_COLORS+1][3] = {
	{240,80,0},{0,200,50},{0,230,230},{250,250,100},{250,120,250},{0,0,255},{255,0,0},{255,255,255},{0,0,0}
};

int SaveCDTPPMImage(const char* name){
	BYTE* buf = new BYTE[3*IMAGE_WIDTH*IMAGE_HEIGHT];
	byte color;
	for(int i = 0;i < IMAGE_WIDTH*IMAGE_HEIGHT*3;i+=3){
		color = state.self.CDTimage[i/3];
		if(color >= NUM_COLORS)
			color = NUM_COLORS;
		memcpy(buf+i,CDTColor[color],3);
	}
	char str[1024];
	sprintf(str,"CDT%s",name);
	int res = SaveYUVPPMImage(buf,str);
	delete[] buf;

	return res;
}


void LoadCDT(){
	static bool init_flag = false;
	if(init_flag) return;

	memset(cdt,255,128*128*128);

	FILE *f = fopen("./MS/color.dat","rb");
	if(f){

		unsigned long ref;	
		unsigned long buf;

		fread(&ref,sizeof(unsigned long),1,f);

		for(unsigned long i=0;i<ref;i++)
		{
			fread((char *)&buf,sizeof(unsigned long),1,f);
			cdt[(0x00ffffff&buf)] = (buf>>24);
		}

		OSYSPRINT(("Color Table Loaded!!\n"));

		fclose(f);
	}

	for(int i = 0;i < NUM_COLORS; i++){
		state.self.cdt[i].value = 0;
		state.self.cdt[i].x = 0;
		state.self.cdt[i].y = 0;
	}
	init_flag = true;
}

void DetectColor(){
	int size = IMAGE_WIDTH*IMAGE_HEIGHT;

	int x[NUM_COLORS];
	int y[NUM_COLORS];
	int sum[NUM_COLORS];

	memset(x,0,sizeof(int)*NUM_COLORS);
	memset(y,0,sizeof(int)*NUM_COLORS);
	memset(sum,0,sizeof(int)*NUM_COLORS);

	int pos = 0;
	for(int i = 0;i < size; i++){

		byte color = cdt[
            (state.self.image[pos    ] & 0xFE) << 13  |
            (state.self.image[pos + 1] & 0xFE) << 6  |
            (state.self.image[pos + 2] >> 1) ];

		pos += 3;
		
		/*/北九州スペシャル　ボールが近いときはピンクをオレンジに
		if(state.ball.distance < 250 && color == PINK){
			state.self.CDTimage[i] = ORANGE;
		}else{
			state.self.CDTimage[i] = color;
		}
		*/

		state.self.CDTimage[i] = color;

		if(color < NUM_COLORS){
			int u = i % IMAGE_WIDTH;
			int v = i / IMAGE_WIDTH;
			x[color] += u;
			y[color] += v;
			sum[color]++;
		}

	}

	for(int i = 0;i < NUM_COLORS;i++){
		state.self.cdt[i].value = sum[i];
		if(sum[i] > 0){
			state.self.cdt[i].x = x[i] / sum[i];
			state.self.cdt[i].y = y[i] / sum[i];
		}else{
			state.self.cdt[i].x = 0;
			state.self.cdt[i].y = 0;
		}
	}

}

void PixelToAngle(double x,double y,double &pan,double &tilt)
{
	pan = atan((x - IMAGE_WIDTH/2) / 200);
	tilt = atan((y - IMAGE_HEIGHT/2) / 200);
}





///加瀬ソース(FOCUS=200, d1=140)※PixelToDirectionは不完全////////////////////////

/*double PixelToDirection(double x, double y)
{
	double ret;
	double focus=200;
	double d1=140;
	double d2=80;
	double d3=14.6;
	double d4=80.06;

	double tilt0=Inverter::CalculateTilt();
	double tilt1=state.self.sensor.jointValue[TILT1]*3.14/180;
	//OSYSPRINT(("tilt1=%4f\n", tilt1));
	double tilt2=3.14/2-state.self.sensor.jointValue[TILT2]*3.14/180;//90-tilt2
	//OSYSPRINT(("tilt2=%4f\n", tilt2));
	double pan=state.self.sensor.jointValue[PAN]*3.14/180;
	//OSYSPRINT(("pan=%4f\n", pan));

	double c0=cos(tilt0);
	double s0=sin(tilt0);
	double c1=cos(tilt1);
	double s1=sin(tilt1);
	double c2=cos(tilt2);
	double s2=sin(tilt2);
	double cp=cos(pan);
	double sp=sin(pan);

	double t11=cp;
	double t12=-c2*sp;
	double t13=-s2*sp;
	double t14=-c2*d3*sp-d4*s2*sp;

	double t21=(c0*c1 + s0*s1)*sp;
	double t22=c2*cp*(c0*c1 + s0*s1) - (c1*s0 - c0*s1)*s2;
	double t23=c2*(c1*s0-c0*s1) + cp*(c0*c1+s0*s1)*s2;
	double t24=d2*(c1*s0-c0*s1) + d3*(c2*cp*(c0*c1 + s0*s1) - (c1*s0 - c0*s1)*s2)
		+ d4*(c2*(c1*s0 - c0*s1) + cp*(c0*c1 + s0*s1)*s2);

	double t31=(-c1*s0 + c0*s1)*sp;//4*4行列の三行目
	double t32=c2*cp*(-c1*s0 + c0*s1) - s2*(c0*c1 + s0*s1);
	double t33=c2*(c0*c1+s0*s1)+cp*s2*(-c1*s0+c0*s1);
	double t34=d1 + d2*(c0*c1 + s0*s1) + d4*(c2*(c0*c1 + s0*s1) + cp*s2*(-c1*s0 + c0*s1)) 
		+ d3*(c2*cp*(-c1*s0 + c0*s1) - s2*(c0*c1 + s0*s1));

	double v = (y - IMAGE_HEIGHT/2);
	double u = (x - IMAGE_HEIGHT/2);
	if(v==0&&u==0)
	{
		ret=110000;
		return ret;
	}else{
		double z=-t34/(u*t31/focus+v*t32/focus+t33);
		double xw=(t11*u+t12*v)*z/focus+t13*z+t14;
		double yw=(t21*u+t22*v)*z/focus+t23*z+t24;

		ret =atan2(-xw,yw)*180/3.14;
		//if((pan>0)&&(xw>0)) ret+=180;
		//if((pan<0)&&(xw<0)) ret-=180;
		return ret;

	}

}*/

double PixelToDistance(double x, double y)
{
	double ret;
	double focus=200;
	double d1=140;
	double d2=80;
	double d3=14.6;
	double d4=80.06;

	double tilt0=Inverter::CalculateTilt();
	double tilt1=state.self.sensor.jointValue[TILT1]*3.14/180;
	double tilt2=3.14/2-state.self.sensor.jointValue[TILT2]*3.14/180;//90-tilt2
	double pan=state.self.sensor.jointValue[PAN]*3.14/180;

	double c0=cos(tilt0);
	double s0=sin(tilt0);
	double c1=cos(tilt1);
	double s1=sin(tilt1);
	double c2=cos(tilt2);
	double s2=sin(tilt2);
	double cp=cos(pan);
	double sp=sin(pan);

	double t11=cp;
	double t12=-c2*sp;
	double t13=-s2*sp;
	double t14=-c2*d3*sp-d4*s2*sp;

	double t21=(c0*c1 + s0*s1)*sp;
	double t22=c2*cp*(c0*c1 + s0*s1) - (c1*s0 - c0*s1)*s2;
	double t23=c2*(c1*s0-c0*s1) + cp*(c0*c1+s0*s1)*s2;
	double t24=d2*(c1*s0-c0*s1) + d3*(c2*cp*(c0*c1 + s0*s1) - (c1*s0 - c0*s1)*s2)
		+ d4*(c2*(c1*s0 - c0*s1) + cp*(c0*c1 + s0*s1)*s2);

	double t31=(-c1*s0 + c0*s1)*sp;//4*4行列の三行目
	double t32=c2*cp*(-c1*s0 + c0*s1) - s2*(c0*c1 + s0*s1);
	double t33=c2*(c0*c1+s0*s1)+cp*s2*(-c1*s0+c0*s1);
	double t34=d1 + d2*(c0*c1 + s0*s1) + d4*(c2*(c0*c1 + s0*s1) + cp*s2*(-c1*s0 + c0*s1)) 
		+ d3*(c2*cp*(-c1*s0 + c0*s1) - s2*(c0*c1 + s0*s1));

	double v = (y - IMAGE_HEIGHT/2);
	double u = (x - IMAGE_HEIGHT/2);
	if(v==0&&u==0)
	{
		ret=110000;
		return ret;
	}else{
		double z=-t34/(u*t31/focus+v*t32/focus+t33);
		double xw=(t11*u+t12*v)*z/focus+t13*z+t14;
		double yw=(t21*u+t22*v)*z/focus+t23*z+t24;

		ret=sqrt(xw*xw+yw*yw);
		//if(ret > 100000||ret<0)ret = 100000;
		if(ret > 100000)ret = 100000;
		return ret;
	}
}

////実川ソース(FOCUS=200, HEIGHT=220)///////////////////////////////////////////
double PixelToDirection(double x, double y)
{
	double focus =200;
	double u = (x - IMAGE_WIDTH/2);
	double v = (y - IMAGE_HEIGHT/2);

	double theta = atan(state.self.horizone.tilt);

	double w = u*cos(theta)+v*sin(theta);

	double pan0 = -atan(w/focus) + state.self.sensor.jointValue[PAN]*3.14/180;
	double tilt = state.self.sensor.jointValue[TILT1]*3.14/180;

	return atan2(sin(pan0), cos(pan0)*cos(tilt))*180/3.14;
}
/*double PixelToDistance(double x, double y)
{
	double focus =200;
	double u = (x - IMAGE_WIDTH/2);
	double v = (y - IMAGE_HEIGHT/2);

	double theta = atan(state.self.horizone.tilt);

	if((y + 20.0) - (state.self.horizone.tilt*x + state.self.horizone.intercept + IMAGE_HEIGHT/2) <= 0.0) return 10000.0;//if over holizone

	double w = -u*sin(theta)+v*cos(theta);

	double tantan = 0;

	double tilt2 = atan(w/focus) - state.self.sensor.jointValue[TILT2]*3.14/180;
	double tilt = -state.self.sensor.jointValue[TILT1]*3.14/180 + Inverter::CalculateTilt();
	double pan = state.self.sensor.jointValue[PAN]*3.14/180;

	double a = sin(tilt2);
	double b2 = (a / cos(tilt))*(a / cos(tilt));

	double x2 = cos(tilt2)*cos(tilt2) + a*a*tan(tilt)*tan(tilt) - 2*cos(tilt2)*a*tan(tilt)*cos(pan);

	double y2 = (1 + b2 - x2)*(1 + b2 - x2);

	tantan = sqrt(4*b2/y2 - 1);

	const double height = 220;

	double ret = height*tantan;

	if(ret > 100000)ret = 100000;

	//if(v == 0) Inverter::CalculateCameraPosition().PrintMatrix();

	return ret;
}*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void Expand(BYTE c)
{
	int w = IMAGE_WIDTH;
	int h = IMAGE_HEIGHT;
	static BYTE buf[IMAGE_WIDTH*IMAGE_HEIGHT];
	BYTE *img = state.self.CDTimage;
	memcpy(buf,img,w*h);
	memset(img,255,w*h);

	int p = 0;
	for(int y = 0;y < h;y++){
		p = w*y-1;
		for(int x = 0;x < w;x++){
			p++;
			if(buf[p] == c){
				img[p] = c;
				continue;
			}
			if(x > 0 && buf[p-1] == c){
				img[p] = c;
				continue;
			}
			if(x < w-1 && buf[p+1] == c){
				img[p] = c;
				continue;
			}
			if(y > 0 && buf[p-w] == c){
				img[p] = c;
				continue;
			}
			if(y < h-1 && buf[p+w] == c){
				img[p] = c;
				continue;
			}
			img[p] = buf[p];
		}
	}
	int sum = 0;
	int xsum = 0;
	int ysum = 0;
	for(int y = 0;y < h;y++){
		p = w*y;
		for(int x = 0;x < w;x++){
			if(img[p] == c){
				sum++;
				xsum+= x;
				ysum+= y;
			}
			p++;
		}
	}
	if(sum > 0){
		xsum /= sum;
		ysum /= sum;
	}
	state.self.cdt[c].value = sum;
	state.self.cdt[c].x = xsum;
	state.self.cdt[c].y = ysum;
}

void Shrink(BYTE c)
{
	int w = IMAGE_WIDTH;
	int h = IMAGE_HEIGHT;
	static BYTE buf[IMAGE_WIDTH*IMAGE_HEIGHT];
	BYTE *img = state.self.CDTimage;
	memcpy(buf,img,w*h);

	int p = 0;
	for(int y = 0;y < h;y++){
		p = y*w-1;
		for(int x = 0;x < w;x++){
			p++;
			if(buf[p] != c){
				img[p] = buf[p];
				continue;
			}
			if(x > 0 && buf[p-1] != c){
				img[p] = 255;
				continue;
			}
			if(x < w-1 && buf[p+1] != c){
				img[p] = 255;
				continue;
			}
			if(y > 0 && buf[p-w] != c){
				img[p] = 255;
				continue;
			}
			if(y < h-1 && buf[p+w] != c){
				img[p] = 255;
				continue;
			}
		}
	}

	int sum = 0;
	int xsum = 0;
	int ysum = 0;

	for(int y = 0;y < h;y++){
		p = w*y;
		for(int x = 0;x < w;x++){
			if(img[p] == c){
				sum++;
				xsum+=x;
				ysum+=y;
			}
			p++;
		}
	}
	if(sum > 0){
		xsum /= sum;
		ysum /= sum;
	}
	state.self.cdt[c].value = sum;
	state.self.cdt[c].x = xsum;
	state.self.cdt[c].y = ysum;
}

