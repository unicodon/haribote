#include "StdAfx.h"
#include "DetectLine.h"

#include "../MCL/Observeline.h"

DetectLine::DetectLine()
{
}

void DetectLine::Execute()
{
	state.lineVisible = false;
	double a[2], b[2], ee[2], a_re, b_re, x_re[2], y_re[2];
	DetectHolizontalLine(a[0],b[0],ee[0]);
	DetectVerticalLine(a[1],b[1],ee[1]);
	int HorV;     //縦か横かのFrag
	double x[2] ,y[2];
	double ang[2] ,dis[2] ,r ,theta;
	if(ee[0] > 10.0 && ee[1] > 10.0) return;
	if(ee[0]<=ee[1])
	{
		double min_u;     //ユークリット距離を求める
		double min=IMAGE_WIDTH/2+IMAGE_HEIGHT/2;
		for(int u=0; u<=IMAGE_WIDTH; u++)
		{
			if(abs(u-IMAGE_WIDTH/2)+abs(a[0]*u+b[0]-IMAGE_HEIGHT/2)<min)
			{
				min=abs(u-IMAGE_WIDTH/2)+abs(a[0]*u+b[0]-IMAGE_HEIGHT/2);
				min_u=(double)u;
			}
		}
		//OSYSPRINT(("min_u = %4.2f v = %4.2f\n",min_u,a[0]*min_u+b[0]));
		x[0]=min_u;
		x[1]=min_u+1;
		y[0]=(double)(a[0]*x[0]+b[0]);
		y[1]=(double)(a[0]*x[1]+b[0]);
		HorV=0;
	}
	else
	{
		double min_v;
		double min=IMAGE_WIDTH/2+IMAGE_HEIGHT/2;
		for(int v=0; v<=IMAGE_HEIGHT; v++)
		{
			if(abs(v-IMAGE_HEIGHT/2)+abs(a[1]*v+b[1]-IMAGE_WIDTH/2)<min)
			{
				min=abs(v-IMAGE_HEIGHT/2)+abs(a[1]*v+b[1]-IMAGE_WIDTH/2);
				min_v=(double)v;
			}
		}
		//OSYSPRINT(("min_v = %4.2f u = %4.2f\n",min_v,a[1]*min_v+b[1]));
		y[0]=min_v;
		if(a[1]*(min_v+1)+b[1]>a[1]*(min_v-1)+b[1])  //u方向との整合性を取る
		{
			y[1]=min_v+1;
			//OSYSPRINT(("min_v+1 is selected\n"));
		}
		else// if(a[1]*(min_v+1)+b[1]<a[1]*(min_v-1)+b[1])
		{
			y[1]=min_v-1;
			//OSYSPRINT(("min_v-1 is selected\n"));
		}
		x[0]=(double)(a[1]*y[0]+b[1]);
		x[1]=(double)(a[1]*y[1]+b[1]);
		HorV=1;
	}
	for(int i=0; i<2; i++)
	{
		ang[i]=PixelToDirection(x[i],y[i]);
		dis[i]=PixelToDistance(x[i],y[i]);
		x_re[i]=(dis[i]*cos(ang[i]*PI/180.0));
		y_re[i]=(dis[i]*sin(ang[i]*PI/180.0));
		//OSYSPRINT(("number %d : x = %4.2f y = %4.2f\n",i,x_re[i],y_re[i]));
	}
	if(x_re[1]!=x_re[0])
	{
		theta=atan2((y_re[1]-y_re[0]),(x_re[1]-x_re[0]));
		a_re=tan(theta);       //double a_re=((y[1]-y[0])/(x[1]-x[0])); 左と同じ意味
		b_re=((x_re[1]*y_re[0]-x_re[0]*y_re[1])/(x_re[1]-x_re[0]));
		
		r=fabs(b_re)/sqrt(1+pow(a_re,2));
	}
	else
	{
		theta = atan2((y_re[1]-y_re[0]),(x_re[1]-x_re[0]));
		r = x_re[0];
	}

	theta  *= 180.0/PI;

	/*static int count = 0;
	if(count >= 1000)
	{
		if(count == 1000)
		{
			OSYSPRINT(("Start writting"));
		}
		FILE* file;
		file = fopen("MS/line.log","a");
		if(file && ee[HorV]<5.0 &&r<9999.0)    //例外処理
		{
			if(theta>90.0)
			{
				theta-=360.0;
			}
			double temp=theta+90.0;
			if(((-160.0<=theta)&&(theta<=-20.0))||(temp*a[HorV]<0.0))
			{
				stringstream ss;
				ss << r << "\t" << theta << "\n";
				fwrite(ss.str().c_str(),1,strlen(ss.str().c_str()),file);
			}
		}
		else
		{
			if(file == 0) OSYSPRINT(("File Open Failed\n"));
		}
		fclose(file);
	}
	count++;*/

	if(ee[HorV]<5.0 && r<9999.0) //例外処理
	{	
		if(theta>90.0)
		{
		theta-=360.0;
		}
		double temp=theta+90.0;
		if(((-160.0<=theta)&&(theta<=-20.0))||(temp*a[HorV]<0.0))
		{
			LineInformation info;
			info.r = (int)r;
			info.theta = (int)theta;
			if(HorV == 0)	info.type = HOLIZONTAL;
			else			info.type = VERTICAL;
			ObserveLine::Observation(info);

			state.lineinfo = info;
			state.lineVisible = true;


			//OSYSPRINT(("START DATA\n"));
			//OSYSPRINT(("ee[0]=%5.3f ee[1]=%5.3f" ,ee[0] ,ee[1]));
			//OSYSPRINT(("a[0]=%f b[0]=%f\n",a[0],b[0] ));
			//OSYSPRINT(("y[0]=%4d y[1]=%4d ", y[0], y[1]));
			//OSYSPRINT(("r=%5.1f ",r ));
			//OSYSPRINT(("theta=%6.1f\n",theta ));
		}
	}
	
}

void DetectLine::DetectHolizontalLine(double &a0, double &b0, double &ee)
{
	vector<Point> edge_point ;	
	enum LINE_MODE{Def,Green,White};
	
	for(int u=48; u<=168; u+=4 )
	{
		LINE_MODE MODE = Def;
		int w_sum, b_sum, e_start, e_end, e_v;
		for(int v = IMAGE_HEIGHT - 1 ; v >= 0 ; v--)
		{
			switch(MODE)
			{
				case Def:
					w_sum=0;
					b_sum=0;
					if (state.self.CDTimage[u+v*IMAGE_WIDTH] == FIELDGREEN)	MODE=Green;
					break;
				case Green:
					w_sum=0;
					b_sum=0;
					if (state.self.CDTimage[u+v*IMAGE_WIDTH] == WHITE)	 MODE=White;
					else if (state.self.CDTimage[u+v*IMAGE_WIDTH] != FIELDGREEN &&
							state.self.CDTimage[u+v*IMAGE_WIDTH] != SKYBLUE &&
							state.self.CDTimage[u+v*IMAGE_WIDTH] != 255 ) MODE=Def;
					break;
				case White :
					if (state.self.CDTimage[u+v*IMAGE_WIDTH] == FIELDGREEN)         
					{
						if(w_sum> 1 && w_sum>b_sum)
						{
							e_end=v+1;
							e_start=v+w_sum+1+b_sum;
					
							e_v=((e_start+e_end)/2);
							Point p;
							p.x=u;
							p.y=e_v;
							edge_point.push_back(p);
							v=0;
						}
						MODE=Green;
					}
					else if (state.self.CDTimage[u+v*IMAGE_WIDTH] != WHITE &&
							 state.self.CDTimage[u+v*IMAGE_WIDTH] != 255 &&
							 state.self.CDTimage[u+v*IMAGE_WIDTH] != SKYBLUE) MODE=Def;
					else if (state.self.CDTimage[u+v*IMAGE_WIDTH] == 255 ||
							state.self.CDTimage[u+v*IMAGE_WIDTH] == SKYBLUE) b_sum +=1;
					else w_sum +=1;
					break;
			}
		}
	}

/*	OSYSPRINT(("start data"));
	for(int i = 0;i < edge_point.size();i++)
	{
		OSYSPRINT(("x = %d y = %d\n",edge_point.at(i).x,edge_point.at(i).y));
	}
	OSYSPRINT(("end data"));
*/
    Method_LS(edge_point, a0, b0, ee);

}


void DetectLine::DetectVerticalLine(double &a0, double &b0, double &ee)
{
	vector<Point> edge_point ;	
	enum LINE_MODE{Def,Green,White};
	
	for(int v=IMAGE_HEIGHT-1; v>=0; v-=4 )
	{
		LINE_MODE MODE = Def;
		int w_sum, b_sum, e_start, e_end, e_u;
		for(int u = 48 ; u<=168 ; u++ )
		{
			switch(MODE)
			{
				case Def:
					w_sum=0;
					b_sum=0;
					if (state.self.CDTimage[u+v*IMAGE_WIDTH] == FIELDGREEN)	MODE=Green;
					break;
				case Green:
					w_sum=0;
					b_sum=0;
					if (state.self.CDTimage[u+v*IMAGE_WIDTH] == WHITE)	 MODE=White;
					else if (state.self.CDTimage[u+v*IMAGE_WIDTH] != FIELDGREEN &&
							state.self.CDTimage[u+v*IMAGE_WIDTH] != SKYBLUE &&
 							state.self.CDTimage[u+v*IMAGE_WIDTH] != 255 ) MODE=Def;
					break;
				case White :
					if (state.self.CDTimage[u+v*IMAGE_WIDTH] == FIELDGREEN)         
					{
						if(w_sum>1 && w_sum>b_sum)
						{
							e_end=u-1;
							e_start=u-w_sum-1-b_sum;

							e_u=((e_start+e_end)/2);
							Point p;
							p.x=v;
							p.y=e_u;
							edge_point.push_back(p);
							u=IMAGE_WIDTH;
						}
						MODE=Green;
					}
					else if (state.self.CDTimage[u+v*IMAGE_WIDTH] != WHITE &&
							 state.self.CDTimage[u+v*IMAGE_WIDTH] != 255 &&
							 state.self.CDTimage[u+v*IMAGE_WIDTH] != SKYBLUE) MODE=Def;
					else if (state.self.CDTimage[u+v*IMAGE_WIDTH] == 255 ||
							state.self.CDTimage[u+v*IMAGE_WIDTH] == SKYBLUE) b_sum +=1;
					else w_sum +=1;
					break;
			}
		}
	}

    Method_LS(edge_point, a0, b0, ee);

}

void DetectLine::Method_LS(vector<Point> edge_point, double &a0, double &b0, double &ee)
{
	int eg_point_num = edge_point.size();

	int sum_u = 0;
	int sum_v = 0;
	int sum_uv = 0;
	int sum_u2 = 0;
	int i;
	for(i=0;i<eg_point_num;i++)
	{
		sum_u += edge_point[i].x;
		sum_v += edge_point[i].y;
		sum_uv += edge_point[i].x*edge_point[i].y;
		sum_u2 += edge_point[i].x*edge_point[i].x;
	}
	a0 = (double)(eg_point_num*sum_uv - sum_u*sum_v)
				/(double)(eg_point_num*sum_u2 - sum_u*sum_u);
	b0 = ((double)sum_v - a0*sum_u)/eg_point_num;
	
	double sum_ee = 0.0;
	double tmp = 0;
	for(i=0;i<eg_point_num;i++)
	{
		tmp = edge_point[i].x*a0 + b0 - (double)edge_point[i].y;
		sum_ee += tmp*tmp;
	}
	ee=sum_ee/eg_point_num;
	if(eg_point_num < 12)
		ee=100.0;
}

