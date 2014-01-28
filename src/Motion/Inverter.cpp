#include "StdAfx.h"
#include "Inverter.h"

#include <math.h>
#include "../config.h"
#include "../State/State.h"


static const double	fl1 = 70.1;
static const double 	fl2 = 62.0;//掌底
//static const double 	fl2 = 76.9;//肉球
static const double 	rl1 = 70.1;
//static const double 	rl2 = 79.4;//肉球
static const double 	rl2 = 61.9;//つま先

static const double 	f_j1_offset = 7.37;
static const double 	f_j3_offset = -30+13.2;//掌底
//static const double 	f_j3_offset = -8.9;//肉球
static const double 	r_j1_offset = 7.37;
static const double 	r_j3_offset = -(30+8.22);//つま先
//static const double 	r_j3_offset = -15.5;

static const double	f_j1_min = -120;
static const double	f_j1_max = 135;
static const double	f_j2_min = -15;
static const double	f_j2_max = 93;
static const double	f_j3_min = -30;
static const double	f_j3_max = 130;

static const double	r_j1_min = -120;
static const double	r_j1_max = 135;
static const double	r_j2_min = -15;
static const double	r_j2_max = 93;
static const double	r_j3_min = -30;
static const double	r_j3_max = 130;

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

Inverter::Inverter()
{

}

Inverter::~Inverter()
{

}

int Inverter::pos2angle(double x, double y, double z, double &j1, double &j2, double &j3, double l1, double l2)
{
	double r = sqrt(x*x + y*y + z*z);
	if(r > l1+l2)
		return 0;

	j2 = atan2(z,y);//joint 2

	j3 = PI - acos((l1*l1 + l2*l2 - r*r)/( 2*l1*l2 ));//joint 3

	double t1 = acos( (r*r + l1*l1 - l2*l2)/(2*r*l1) );
	double t2 = atan2(x,sqrt(y*y + z*z));
	double t3 = -t1 + t2;

	double x1 = l1*sin(t3);
	double y1 = l1*cos(t3);

	j1 = atan2(x1,y1*cos(j2));

	return 1;
}

int Inverter::Pos2AngleFront(double x, double y, double z, double &j1, double &j2, double &j3){
	if(!pos2angle(x,y,z,j1,j2,j3,fl1,fl2))return 0;
	j1 *= 180/PI;
	j2 *= 180/PI;
	j3 *= 180/PI;

	j1 -= f_j1_offset;
	j3 -= -f_j1_offset + f_j3_offset;

	if(j1 < f_j1_min || j1 > f_j1_max)
		return 0;
	if(j2 < f_j2_min || j2 > f_j2_max)
		return 0;
	if(j3 < f_j3_min || j3 > f_j3_max)
		return 0;

	return 1;
}

int Inverter::Pos2AngleRear(double x, double y, double z, double &j1, double &j2, double &j3){
	if(!pos2angle(x,y,z,j1,j2,j3,rl1,rl2))return 0;
	j1 *= 180/PI;
	j2 *= 180/PI;
	j3 *= 180/PI;

	j1 -= r_j1_offset;
	j3 -= -r_j1_offset + r_j3_offset;

	if(j1 < r_j1_min || j1 > r_j1_max)
		return 0;
	if(j2 < r_j2_min || j2 > r_j2_max)
		return 0;
	if(j3 < r_j3_min || j3 > r_j3_max)
		return 0;

	return 1;
}

double Inverter::CalculateTilt(){
/*
	//脚座標：x 前後方向外側 y 下側 z 左右方向外側
	double frx,fry,frz,
		   flx,fly,flz,
		   hrx,hry,hrz,
		   hlx,hly,hlz;

	double j1,j2,j3;

	//Foreleg Right
	j1 = (state.self.sensor.jointValue[RFLEG1]+f_j1_offset)/180*PI;
	j2 = state.self.sensor.jointValue[RFLEG2]/180*PI;
	j3 = (state.self.sensor.jointValue[RFLEG3]+f_j3_offset)/180*PI;
	frx = fl1*sin(j1)+fl2*sin(j1+j3);
	fry = fl1*cos(j1)+fl2*cos(j1+j3);
	frz = fry*sin(j2);
	fry *= cos(j2);
	if(fry < 60){//胸が接地
		frx = frz = 0;
		fry = 60;
	}

	//Foreleg Left
	j1 = (state.self.sensor.jointValue[LFLEG1]+f_j1_offset)/180*PI;
	j2 = state.self.sensor.jointValue[LFLEG2]/180*PI;
	j3 = (state.self.sensor.jointValue[LFLEG3]+f_j3_offset)/180*PI;
	flx = fl1*sin(j1)+fl2*sin(j1+j3);
	fly = fl1*cos(j1)+fl2*cos(j1+j3);
	flz = fly*sin(j2);
	fly *= cos(j2);
	if(fly < 60){
		flx = flz = 0;
		fly = 60;
	}

	//Hindleg Right
	j1 = (state.self.sensor.jointValue[RHLEG1]+r_j1_offset)/180*PI;
	j2 = state.self.sensor.jointValue[RHLEG2]/180*PI;
	j3 = (state.self.sensor.jointValue[RHLEG3]+r_j3_offset)/180*PI;
	hrx = rl1*sin(j1)+rl2*sin(j1+j3);
	hry = rl1*cos(j1)+rl2*cos(j1+j3);
	hrz = hry*sin(j2);
	hry *= cos(j2);
	if(hry < 60){
		hrx = hrz = 0;
		hry = 60;
	}

	//Hindleg Left
	j1 = (state.self.sensor.jointValue[LHLEG1]+r_j1_offset)/180*PI;
	j2 = state.self.sensor.jointValue[LHLEG2]/180*PI;
	j3 = (state.self.sensor.jointValue[LHLEG3]+r_j3_offset)/180*PI;
	hlx = rl1*sin(j1)+rl2*sin(j1+j3);
	hly = rl1*cos(j1)+rl2*cos(j1+j3);
	hlz = hly*sin(j2);
	hly *= cos(j2);
	if(hly < 60){
		hlx = hlz = 0;
		hly = 60;
	}

	double f_m = (fly - fry) / (135+frz+flz);
	double f_y = fly - f_m * (65+fly);
	double h_m = (hly - hry) / (135+hrz+hlz);
	double h_y = hly - h_m * (65+hly);

	double f_x = (frx+flx)/2;
	double h_x = (hrx+hlx)/2;
	
	
	double t = -atan((f_y - h_y)/(130+f_x-h_x));
	return t;
	*/
	//return -atan((-10)/130);//両足間で10mm高さが違う
	return -atan2((-15),130.0);

}

//カメラ姿勢計算
Mat44 Inverter::CalculateCameraPosition(){
	double m = 0;
	double t = -atan((double)(95-110)/130);
	double h = 95;

	const double neck = 80.0;
	const double nose_x = 81.06;
	const double nose_z = -14.6;
	double tilt1,pan,tilt2;

	tilt1 = state.self.sensor.jointValue[TILT1]*PI/180;
	pan = state.self.sensor.jointValue[PAN]*PI/180;
	tilt2 = state.self.sensor.jointValue[TILT2]*PI/180;


	Mat44 NeckBase(cos(t),0,-sin(t),0,
		0,1,0,0,
		sin(t),0,cos(t),h,
		0,0,0,1);

	Mat44 BaseXRotate(1,0,0,0,
		0,cos(m),-sin(m),0,
		0,sin(m),cos(m),0,
		0,0,0,1);

	Mat44 T12(1.0,  0,  0,2.5,
			     0,1.0,  0,   0,
			     0,  0,1.0,19.5,
				 0,  0,  0, 1.0);
	Mat44 T23(cos(tilt1),0,-sin(tilt1),     0,
					  0,1,		   0,	  0,
			  sin(tilt1),0, cos(tilt1),0,
					  0,0,		   0,	  1);
	Mat44 T34(cos(pan),-sin(pan),0,    0,
			  sin(pan), cos(pan),0,	   0,
					 0,		   0,1, neck,
					 0,		   0,0,	   1);
	Mat44 T45(cos(tilt2),	0,	 -sin(tilt2),nose_x,
			  0		,		1,	0,   0,
			  sin(tilt2),	0, cos(tilt2),   nose_z,
			  0,		0,		   0,	1);

	return NeckBase*BaseXRotate*T12*T23*T34*T45;
}

void Inverter::CalculateHorizone(double bodytilt, double& m, double &A, bool& perp){
	double t1 = (bodytilt+state.self.sensor.jointValue[TILT1]) * PI / 180;
	double t2 = state.self.sensor.jointValue[TILT2] * PI / 180;
	double p = state.self.sensor.jointValue[PAN] * PI / 180;

	if(t1 < -89*PI/180){
		perp = true;
		m = 0;
		if(p > 5){
			A =  - 200/tan(p);
		}else if(p < -5){
			A = 200/tan(-p);
		}else{
			A = 0;
			m = 1;
		}
		return;
	}
	perp = false;
	
	Mat44 tilt1(
		1.0,0.0,0.0,0.0,
		0.0,cos(-t1),-sin(-t1),0.0,
		0.0,sin(-t1),cos(-t1),0.0,
		0.0,0.0,0.0,1.0
	);

	Mat44 pan(
		cos(p),0.0,sin(p),0.0,
		0.0,1.0,0.0,0.0,
		-sin(p),0.0,cos(p),0.0,
		0.0,0.0,0.0,1.0
	);
		
	Mat44 tilt2(
		1.0,0.0,0.0,0.0,
		0.0,cos(-t2),-sin(-t2),0.0,
		0.0,sin(-t2),cos(-t2),0.0,
		0.0,0.0,0.0,1.0
	);
	
	Mat44 mat = tilt2*pan*tilt1;
//	mat.PrintMatrix();
	
	double a = mat.matrix[0][0];
	double c = mat.matrix[0][2];
	double d = mat.matrix[1][0];
	double f = mat.matrix[1][2];
	double g = mat.matrix[2][0];
	double i = mat.matrix[2][2];
			
	if(fabs(d) < 0.01 && fabs(f) < 0.01){
		//Y=0
		m = 0;A = 0;
		return;
	}

	if(fabs(a) < 0.01 && fabs(c) < 0.01){
		//X=0
		perp = true;
		A = 0;
	}

	double del = a*f - c*d;
	
	double P = (f*g - d*i)/del;
	double Q = (-c*g + a*i)/del;
	
	m =  -(P/Q);
	A = 200/Q;
}
