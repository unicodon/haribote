#include "StdAfx.h"
#include <math.h>
#include <stdio.h>
#include <OPENR/OSyslog.h>
#include <string>
#include "GaitMaker.h"

#include "../Utility/MatrixUtil.h"
#include "PresetWalkParam.h"
#include "../MCL/DeadRec.h"
#include "../Vision/DetectBall.h"
#include "../Utility/StopWatch.h"


Inverter inverter;


double GaitMaker::angleData[NUM_GAIT_POINT][12];
double GaitMaker::nextAngleData[NUM_GAIT_POINT][12];
bool GaitMaker::next_flag = false;
double GaitMaker::cycletime = 32.0;
double GaitMaker::nextcycletime = 32.0;
int GaitMaker::nextStepLimit = 0;

bool GaitMaker::isStop = true;
bool GaitMaker::next_isStop = false;

PresetWalkParam GaitMaker::presetWalkParam;
int GaitMaker::stepLimit = 1;
double GaitMaker::phase = 0;
double GaitMaker::gratio = 0.24;
double GaitMaker::aratio = 0.24;
WalkParam GaitMaker::myparam;

Movement GaitMaker::movedata = {0,0,0};
Movement GaitMaker::next_movedata = {0,0,0};
Movement GaitMaker::d_movedata = {0,0,0};

unsigned char walk_index;//takeshita
static LegDegParam SetLegParam[NUM_PRESET_WALKPARAM];


GaitMaker::GaitMaker(){
	SetPresetWalk(WALK_STOP,10);
}


GaitMaker::~GaitMaker(){
}

void GaitMaker::Initialized()
{
	OSYSPRINT(("NOW WALK IS SET!!!!!!!!"));

	static bool set_flag = false;
	if(!set_flag)
	{
		for(int index=0;index<NUM_PRESET_WALKPARAM;index++)
		{
			MakeLegParamForPreset(index, SetLegParam[index], presetWalkParam.walkparams[index]);
		}
		set_flag = true;
	}
}

bool GaitMaker::IsStop(){
	return isStop;
}

bool GaitMaker::IsNextWalkSet(){
	return next_flag;
}

//takeshita 0618
bool GaitMaker::IsExpiring(){
	//OSYSPRINT(("Flag = %d, STEP = %d, PHASE = %.1f, CYCLETIME = %d\n",
	//	((stepLimit - phase)*cycletime < 2*NUM_FRAMES*8), stepLimit, phase, (int)cycletime));
	return ((stepLimit - phase)*cycletime < 2*NUM_FRAMES*8) ;
}

int GaitMaker::SetWalkParamForPreset(WalkParam param){
	//OSYSPRINT((" ????????????? , phase=%.1f, Steplimit=%d\n", phase, stepLimit));
	myparam = param;
	if(isStop){
		for(int i = 0;i < NUM_GAIT_POINT;i++){
			//if(!MakeLocus(&(angleData[i][0]), (double)i/NUM_GAIT_POINT)){
			//if(!MakeLocus2(&(angleData[i][0]), i)){
			if(!MakeLocus3(walk_index, &(angleData[i][0]), i)){
				//OSYSPRINT(("SetWalkParam Failed\n"));
				return 0;
			}
		}
		next_isStop = true;
		next_flag = false;
		cycletime = myparam.time;
		movedata.x = myparam.move_x / cycletime * 8.0;
		movedata.y = myparam.move_y / cycletime * 8.0;
		movedata.tt = myparam.move_tt / cycletime * 8.0;

	}else{
		for(int i = 0;i < NUM_GAIT_POINT;i++){
			//if(!MakeLocus(&(nextAngleData[i][0]), (double)i/NUM_GAIT_POINT)){
			//if(!MakeLocus2(&(nextAngleData[i][0]), i)){
			if(!MakeLocus3(walk_index, &(nextAngleData[i][0]), i)){
				//OSYSPRINT(("SetWalkParam Failed\n"));
				return 0;
			}
		}
		nextcycletime = myparam.time;
		next_flag = true;
		next_isStop = false;
		next_movedata.x = myparam.move_x / nextcycletime * 8.0;
		next_movedata.y = myparam.move_y / nextcycletime * 8.0;
		next_movedata.tt = myparam.move_tt / nextcycletime * 8.0;
//		OSYSPRINT(("Add Next WalkParam \n"));
	}
	return 1;
}

int GaitMaker::SetWalkParam(WalkParam param, int step){
	myparam = param;
	walk_index = 1;

	if(isStop){
//		OSYSPRINT(("Now Stop\n"));
		for(int i = 0;i < NUM_GAIT_POINT;i++){
			//if(!MakeLocus(&(angleData[i][0]), (double)i/NUM_GAIT_POINT)){
			if(!MakeLocus2(&(angleData[i][0]), i)){
				//OSYSPRINT(("SetWalkParam Failed\n"));
				return 0;
			}
		}

		phase = 0;
		cycletime = myparam.time;
		stepLimit = step;
		movedata.x = myparam.move_x / cycletime * 8.0;
		movedata.y = myparam.move_y / cycletime * 8.0;
		movedata.tt = myparam.move_tt / cycletime * 8.0;
		next_isStop = true;
		next_flag = false;
		isStop = false;
	}else{
//		OSYSPRINT(("Now Not Stop\n"));
		for(int i = 0;i < NUM_GAIT_POINT;i++){
			//if(!MakeLocus(&(nextAngleData[i][0]), (double)i/NUM_GAIT_POINT)){
			if(!MakeLocus2(&(nextAngleData[i][0]), i)){
				//OSYSPRINT(("SetWalkParam Failed\n"));
				return 0;
			}
		}
		nextcycletime = myparam.time;
		phase = phase - (int)phase;
		stepLimit = 1;
		nextStepLimit = step;
		next_flag = true;
		next_movedata.x = myparam.move_x / nextcycletime * 8.0;
		next_movedata.y = myparam.move_y / nextcycletime * 8.0;
		next_movedata.tt = myparam.move_tt / nextcycletime * 8.0;
		next_isStop = false;
//		OSYSPRINT(("Add Next WalkParam \n"));
	}

	return 1;
}

int GaitMaker::SetParaWalkParam(WalkParam param, int step){
	myparam = param;

	for(int i = 0;i < NUM_GAIT_POINT;i++){
		//if(!MakeLocus(&(nextAngleData[i][0]), (double)i/NUM_GAIT_POINT)){
		if(!MakeLocus2(&(nextAngleData[i][0]), i)){
			return 0;
		}
	}
	phase = 0;
	cycletime = myparam.time;
	stepLimit = step;
	movedata.x = myparam.move_x / cycletime * 8.0;
	movedata.y = myparam.move_y / cycletime * 8.0;
	movedata.tt = myparam.move_tt / cycletime * 8.0;
	next_isStop = true;
	next_flag = false;
	isStop = false;

	return 1;
}



int GaitMaker::SetPresetWalk(int walkIndex, int step){//プリセットの歩行をセットする

	walk_index = (unsigned char) walkIndex;//takeshita
	if(step < 0)step = 0;
	if(walkIndex >= 0 && walkIndex < NUM_PRESET_WALKPARAM)
	{
		myparam = presetWalkParam.walkparams[walkIndex];
		//int test_num = SetWalkParamForPreset(myparam);

		if(SetWalkParamForPreset(myparam) == 0)
		{
			//OSYSPRINT(("1 time = %.1f, set_time = %.1f\n", time/1000.0, time_set/1000.0));
			return 0;
		}
		if(isStop){
			phase = 0;
			if(walkIndex == WALK_STOP){
				isStop = true;
				stepLimit = 0;
			}else{
				isStop = false;
				stepLimit = step;
//				OSYSPRINT(("is stop\n"));
			}
			next_flag = false;
		}else{
			if(walkIndex == WALK_STOP){
				next_isStop = true;
				nextStepLimit = 0;
				stepLimit = 1;
				phase = phase - (int)phase;
			}else{
				next_isStop = false;
				phase = phase - (int)phase;
				stepLimit = 1;
				nextStepLimit = step;
//				OSYSPRINT(("not stop\n"));
			}
			next_flag = true;
		}
		//OSYSPRINT(("2 time = %.1f, set_time = %.1f\n", time/1000.0, time_set/1000.0));
		return 1;
	}else{
		//OSYSPRINT(("3 time = %.1f, set_time = %.1f\n", time/1000.0, time_set/1000.0));
		return 0;
	}

}

/*
//何歩あるくか指定
void GaitMaker::SetStepLimit(int n){
	if(isStop){
		phase = 0;
		stepLimit = n;
	}
	else
		nextStepLimit = n;
}
*/

void GaitMaker::ResetPhase(){
	phase = 0.0;
}

void GaitMaker::GetNextJointValue(double *j){

	static bool initialized = false;
	if(!initialized)
	{
		Initialized();
		initialized = true;
	}

//	static unsigned int cnt_=0;
//	cnt_++;
//	if(cnt_%10 == 0)
//		for(int i=0;i<4;i++)
//			OSYSPRINT(("i = %d, time=%.1f: %.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\n", i,cycletime,myparam.legparam[i].x, myparam.legparam[i].y,myparam.legparam[i].z,myparam.legparam[i].h,myparam.legparam[i].w,myparam.legparam[i].phi,myparam.legparam[i].theta));

//	OSYSPRINT(("phase = %.1f, isStop = %d, stepLimit = %d\n", phase, (int)isStop, (int)next_flag, stepLimit));

	if(!isStop && (stepLimit == 0 || phase <= stepLimit)){
		phase += 8.0 / cycletime;
	}else{
		//OSYSPRINT((" !!!!!!!!!! , phase=%.1f, cycletime=%.1f\n", phase, cycletime));
		if(isStop)
			phase = 0;
		else
			phase = phase - (int)phase;

		if(next_flag){
			next_flag = false;
			movedata = next_movedata;
			cycletime = nextcycletime;
			stepLimit = nextStepLimit;
			memcpy(angleData,nextAngleData,sizeof(double)*NUM_GAIT_POINT*12);
			if(next_isStop)
				isStop = true;
			else
				isStop = false;
		}else{
			SetPresetWalk(WALK_STOP,1);
			phase = 0.0;
			isStop = true;
		}
	}

	double r = phase*NUM_GAIT_POINT - (int)(phase*NUM_GAIT_POINT);
	int index1 = (int)(phase*NUM_GAIT_POINT) % NUM_GAIT_POINT;
	int index2 = (index1+1) % NUM_GAIT_POINT;

	//OSYSPRINT(("index1 = %d, index2 = %d\n",index1,index2));
	//DeadRecogning用
	//
	if(!isStop){
		if(next_flag){
			double p = phase - (int)phase;
			d_movedata.x  += (1-p)*movedata.x  + p*next_movedata.x; 
			d_movedata.y  += (1-p)*movedata.y  + p*next_movedata.y; 
			d_movedata.tt += (1-p)*movedata.tt + p*next_movedata.tt; 
		}else{
			d_movedata.x  += movedata.x; 
			d_movedata.y  += movedata.y; 
			d_movedata.tt += movedata.tt;
		}
	}

	//

	for(int i = 0;i < 12;i++){
//old
//		j[i] = (1-r)*angleData[index1][i] + r*angleData[index2][i];

		if(next_flag){
			double p = phase - (int)phase;
			j[i] = (1-p)*((1-r)*angleData[index1][i] + r*angleData[index2][i]) +
					p*((1-r)*nextAngleData[index1][i] + r*nextAngleData[index2][i]);
		}else{
			j[i] = (1-r)*angleData[index1][i] + r*angleData[index2][i];
		}
	}
}

void GaitMaker::UpdateMoveData(){
	UpdatePosition(d_movedata.x, d_movedata.y, d_movedata.tt);

	d_movedata.x  = 0;
	d_movedata.y  = 0;
	d_movedata.tt = 0;
}

bool GaitMaker::MakeLocus(double* j,double phase_)
{

	double t = phase_ - (int)phase_;

	gratio = myparam.gratio;
	aratio = myparam.aratio;
	if(aratio == 0){
		aratio = (1 - gratio)/3;
	}

	double theta[4];
	double phi[4];
	double w[4];
	double h[4];

	for(int i = 0;i < 4;i++){
		theta[i] = myparam.legparam[i].theta * PI/180;
		phi[i]   = myparam.legparam[i].phi * PI/180;
		w[i]     = myparam.legparam[i].w;
		h[i]     = myparam.legparam[i].h;
	}


	double phase[4] = {0.0, 0.5, 0.5, 0.0};


#define locusfunction getUnitRectLocus


	Mat44 rotz_0(	cos(phi[0]),	-sin(phi[0]),	0,	0,	
					sin(phi[0]),	cos(phi[0]),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);
	Mat44 rotz_1(	cos(phi[1]),	-sin(phi[1]),	0,	0,	
					sin(phi[1]),	cos(phi[1]),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);
	Mat44 rotz_2(	cos(phi[2]),	-sin(phi[2]),	0,	0,	
					sin(phi[2]),	cos(phi[2]),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);
	Mat44 rotz_3(	cos(phi[3]),	-sin(phi[3]),	0,	0,	
					sin(phi[3]),	cos(phi[3]),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);


	Mat44 roty_0(	cos(-theta[0]),	0,-sin(-theta[0]),	0,	
					0,	1,	0,	0,
					sin(-theta[0]),	0, cos(-theta[0]),	0,
					0,	0,	0,	1	);
	Mat44 roty_1(	cos(theta[1]),	0,-sin(theta[1]),	0,	
					0,	1,	0,	0,
					sin(theta[1]),	0, cos(theta[1]),	0,
					0,	0,	0,	1	);
	Mat44 roty_2(	cos(-theta[2]),	0,-sin(-theta[2]),	0,	
					0,	1,	0,	0,
					sin(-theta[2]),	0, cos(-theta[2]),	0,
					0,	0,	0,	1	);
	Mat44 roty_3(	cos(theta[3]),	0,-sin(theta[3]),	0,	
					0,	1,	0,	0,
					sin(theta[3]),	0, cos(theta[3]),	0,
					0,	0,	0,	1	);


	//ここから軌道生成
	Inverter inv;
	double x,y,z,
		rfj1,rfj2,rfj3,
		lfj1,lfj2,lfj3,
		rrj1,rrj2,rrj3,
		lrj1,lrj2,lrj3;
 
	double px,py;

//	OSYSPRINT(("WalkParam %1.f,%1.f,%1.f,%1.f,\n",w[0],h[0],theta[0]*180/3.14,phi[0]*180/3.14));
//	OSYSPRINT(("%1.f,%1.f,%1.f\n",myparam.legparam[0].x,myparam.legparam[0].y,myparam.legparam[0].z));

//f
	locusfunction(t+phase[0],px,py);
	px *= w[0] /2; py *= h[0] /2;
	Vec4 vf(px,py,0,1);
	x = (roty_0*rotz_0*vf).vec[0] + myparam.legparam[0].x;
	y = (roty_0*rotz_0*vf).vec[1] + myparam.legparam[0].y;
	z = (roty_0*rotz_0*vf).vec[2] + myparam.legparam[0].z;
//	OSYSPRINT(("Leg1 %.1f,%.1f,%.1f\n",x,y,z));
	if(!inv.Pos2AngleFront(x,y,z,rfj1,rfj2,rfj3))return false;

	locusfunction(t+phase[1],px,py);
	px *= w[1] /2; py *= h[1] /2;
	vf.vec[0] = px;vf.vec[1] = py;
	x = (roty_1*rotz_1*vf).vec[0] + myparam.legparam[1].x;
	y = (roty_1*rotz_1*vf).vec[1] + myparam.legparam[1].y;
	z = (roty_1*rotz_1*vf).vec[2] + myparam.legparam[1].z;
//	OSYSPRINT(("Leg2 %.1f,%.1f,%.1f\n",x,y,z));
	if(!inv.Pos2AngleFront(x,y,z,lfj1,lfj2,lfj3))return false;

//r
	locusfunction(t+phase[2],px,py);
	px *= w[2] /2; py *= h[2] /2;
	vf.vec[0] = px;vf.vec[1] = py;
	x = (roty_2*rotz_2*vf).vec[0] + myparam.legparam[2].x;
	y = (roty_2*rotz_2*vf).vec[1] + myparam.legparam[2].y;
	z = (roty_2*rotz_2*vf).vec[2] + myparam.legparam[2].z;
//	OSYSPRINT(("Leg3 %.1f,%.1f,%.1f\n",x,y,z));
	if(!inv.Pos2AngleRear(-x,y,z,rrj1,rrj2,rrj3))return false;

	locusfunction(t+phase[3],px,py);
	px *= w[3] /2; py *= h[3] /2;
	vf.vec[0] = px;vf.vec[1] = py;
	x = (roty_3*rotz_3*vf).vec[0] + myparam.legparam[3].x;
	y = (roty_3*rotz_3*vf).vec[1] + myparam.legparam[3].y;
	z = (roty_3*rotz_3*vf).vec[2] + myparam.legparam[3].z;
//	OSYSPRINT(("Leg4 %.1f,%.1f,%.1f\n",x,y,z));
	if(!inv.Pos2AngleRear(-x,y,z,lrj1,lrj2,lrj3))return false;

	j[0] = rfj1;
	j[1] = rfj2;
	j[2] = rfj3;

	j[3] = lfj1;
	j[4] = lfj2;
	j[5] = lfj3;

	j[6] = rrj1;
	j[7] = rrj2;
	j[8] = rrj3;
	
	j[9] =  lrj1;
	j[10] = lrj2;
	j[11] = lrj3;

	return true;
}

void GaitMaker::getUnitLocus(double t, double& x, double& y)
{
	t = t - (int)t;

	const double P[8][2] = {
		{-0.8,	-1},
		{-0,		-1},
		{0.8,	-1},
		{1,	-0.6},
		{0.6, 0.6},
		{0.4,1},
		{-1,0.2},
		{-1,-0.8}
	};

	int n = ((int)(t*8))%8;
	double r = t - n/8.0;
	x = (1-r)*P[n][0] + r*P[(n+1)%8][0];
	y = (1-r)*P[n][1] + r*P[(n+1)%8][1];

}

void GaitMaker::getUnitRectLocus(double t, double& y, double& x)
{
	t += gratio/2;
	t = t - (int)t;
	t = 1-t;//いろいろ補正

	double liftdownratio = (1- gratio - aratio);

	double a = aratio;
	double b = liftdownratio / 2;
	if(t < gratio){//接地
		x = 1;
		y = -1 + 2 * t / gratio;
	}else if(t - gratio < b){//上げ下ろし
		y = 1;
		x = 1 - 2* (t - gratio) / b;
	}else if(t - gratio - b < a){
		x = -1;
		y = 1 - 2* (t - gratio - b)/a;
	}else{
		y = -1;
		x = -1 + 2* (t - gratio - b - a)/b;
	}
}

/*
void GaitMaker::getUnitRectLocus(double t, double& y, double& x)
{
	t += gratio/2;
	t = t - (int)t;
	t = 1-t;//いろいろ補正

	double liftdownratio = (1- gratio)*2/3;

	double a = 1 - gratio - liftdownratio;
	double b = liftdownratio / 2;
	if(t < gratio){//接地
		x = 1;
		y = -1 + 2 * t / gratio;
	}else if(t - gratio < b){//上げ下ろし
		y = 1;
		x = 1 - 2* (t - gratio) / b;
	}else if(t - gratio - b < a){
		x = -1;
		y = 1 - 2* (t - gratio - b)/a;
	}else{
		y = -1;
		x = -1 + 2* (t - gratio - b - a)/b;
	}
}

*/


bool GaitMaker::MakeLocus2(double *j,int phase_)//軌道を線形補間
{
	//基本姿勢
	static int dp_m_rf_x = 60;
	static int dp_m_rf_y = 70;
	static int dp_m_rf_z = 15;
	static int dp_m_rf_w = 0;
	static int dp_m_rf_h = 0;
	static int dp_m_rf_t = 0;
	static int dp_m_rf_p = 0;

	static int dp_m_lf_x = 60;
	static int dp_m_lf_y = 70;
	static int dp_m_lf_z = 15;
	static int dp_m_lf_w = 0;
	static int dp_m_lf_h = 0;
	static int dp_m_lf_t = 0;
	static int dp_m_lf_p = 0;

	static int dp_m_rh_x = 0;
	static int dp_m_rh_y = 120;
	static int dp_m_rh_z = 0;
	static int dp_m_rh_w = 0;
	static int dp_m_rh_h = 0;
	static int dp_m_rh_t = 0;
	static int dp_m_rh_p = 0;

	static int dp_m_lh_x = 0;
	static int dp_m_lh_y = 120;
	static int dp_m_lh_z = 0;
	static int dp_m_lh_w = 0;
	static int dp_m_lh_h = 0;
	static int dp_m_lh_t = 0;
	static int dp_m_lh_p = 0;

	static double dp_m_gratio = 0.24;
	static double dp_m_aratio = 0.24;
	static int dp_m_time = 300;
	static int dp_m_num = 8;
	//ここまで基本姿勢

	double m_gratio = myparam.gratio;
	double m_aratio = myparam.aratio;

	double totaltime = myparam.time;
	int num = NUM_GAIT_POINT;//myparam.num;

	double rf_theta = -myparam.legparam[0].theta*PI/180;
	double lf_theta = -myparam.legparam[1].theta*PI/180;
	double rh_theta = myparam.legparam[2].theta*PI/180;
	double lh_theta = myparam.legparam[3].theta*PI/180;

	double rf_phi = myparam.legparam[0].phi*PI/180;
	double lf_phi = myparam.legparam[1].phi*PI/180;
	double rh_phi = myparam.legparam[2].phi*PI/180;
	double lh_phi = myparam.legparam[3].phi*PI/180;

	//double t_ = phase_ - (int)phase_;

	if(m_aratio == 0){
		m_aratio = (1 - m_gratio)/3;
	}

	double theta[4];
	double phi[4];
	double w[4];
	double h[4];

	for(int i = 0;i < 4;i++){
		theta[i] = myparam.legparam[i].theta * PI/180;
		phi[i]   = myparam.legparam[i].phi * PI/180;
		w[i]     = myparam.legparam[i].w;
		h[i]     = myparam.legparam[i].h;
	}


	double phase[4] = {0.0, 0.5, 0.5, 0.0};

#define locusfunction getUnitRectLocus


	Mat44 rotz_rf(	cos(rf_phi),	-sin(rf_phi),	0,	0,	
					sin(rf_phi),	cos(rf_phi),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);

	Mat44 rotz_lf(	cos(lf_phi),	-sin(lf_phi),	0,	0,	
					sin(lf_phi),	cos(lf_phi),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);

	Mat44 rotz_rh(	cos(rh_phi),	-sin(rh_phi),	0,	0,	
					sin(rh_phi),	cos(rh_phi),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);

	Mat44 rotz_lh(	cos(lh_phi),	-sin(lh_phi),	0,	0,	
					sin(lh_phi),	cos(lh_phi),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);


	Mat44 roty_rf(	cos(rf_theta),	0,-sin(rf_theta),	0,	
					0,	1,	0,	0,
					sin(rf_theta),	0, cos(rf_theta),	0,
					0,	0,	0,	1	);

	Mat44 roty_lf(	cos(-lf_theta),	0,-sin(-lf_theta),	0,	
					0,	1,	0,	0,
					sin(-lf_theta),	0, cos(-lf_theta),	0,
					0,	0,	0,	1	);

	Mat44 roty_rh(	cos(-rh_theta),	0,-sin(-rh_theta),	0,	
					0,	1,	0,	0,
					sin(-rh_theta),	0, cos(-rh_theta),	0,
					0,	0,	0,	1	);

	Mat44 roty_lh(	cos(lh_theta),	0,-sin(lh_theta),	0,	
					0,	1,	0,	0,
					sin(lh_theta),	0, cos(lh_theta),	0,
					0,	0,	0,	1	);


	//ここから軌道生成
	Inverter inv;
	double x,y,z,rfj1,rfj2,rfj3,rhj1,rhj2,rhj3,lfj1,lfj2,lfj3,lhj1,lhj2,lhj3;
	static double dp_rfj1,dp_rfj2,dp_rfj3,dp_rhj1,dp_rhj2,dp_rhj3,dp_lfj1,dp_lfj2,dp_lfj3,dp_lhj1,dp_lhj2,dp_lhj3;
	//vector<double> set_rfj1,set_rfj3,set_rhj1,set_rhj2,set_rhj3,set_lfj1,set_lfj2,set_lfj3,set_lhj1,set_lhj2,set_lhj3;
	static double set_rfj1[NUM_GAIT_POINT],set_rfj2[NUM_GAIT_POINT],set_rfj3[NUM_GAIT_POINT],set_rhj1[NUM_GAIT_POINT],set_rhj2[NUM_GAIT_POINT],set_rhj3[NUM_GAIT_POINT],set_lfj1[NUM_GAIT_POINT],set_lfj2[NUM_GAIT_POINT],set_lfj3[NUM_GAIT_POINT],set_lhj1[NUM_GAIT_POINT],set_lhj2[NUM_GAIT_POINT],set_lhj3[NUM_GAIT_POINT];

	static bool dp_flag = false;

	double t;
	double px,py;
	int ret;
	//規定値
	if(!dp_flag){
		locusfunction(0.0+phase[0],px,py);
		px *= dp_m_rf_w/2; py *= dp_m_rf_h/2;
		Vec4 vf(px,py,0,1);
		x = (roty_rf*rotz_rf*vf).vec[0] + dp_m_rf_x;
		y = (roty_rf*rotz_rf*vf).vec[1] + dp_m_rf_y;
		z = (roty_rf*rotz_rf*vf).vec[2] + dp_m_rf_z;
		ret = inv.Pos2AngleFront(x,y,z,dp_rfj1,dp_rfj2,dp_rfj3);

		locusfunction(0.0+phase[1],px,py);
		px *= dp_m_lf_w/2; py *= dp_m_lf_h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_lf*rotz_lf*vf).vec[0] + dp_m_lf_x;
		y = (roty_lf*rotz_lf*vf).vec[1] + dp_m_lf_y;
		z = (roty_lf*rotz_lf*vf).vec[2] + dp_m_lf_z;
		ret = inv.Pos2AngleFront(x,y,z,dp_lfj1,dp_lfj2,dp_lfj3);

		locusfunction(0.0+phase[2],px,py);
		px *= dp_m_rh_w/2; py *= dp_m_rh_h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_rh*rotz_rh*vf).vec[0] + dp_m_rh_x;
		y = (roty_rh*rotz_rh*vf).vec[1] + dp_m_rh_y;
		z = (roty_rh*rotz_rh*vf).vec[2] + dp_m_rh_z;
		ret = inv.Pos2AngleRear(-x,y,z,dp_rhj1,dp_rhj2,dp_rhj3);

		locusfunction(0.0+phase[3],px,py);
		px *= dp_m_lh_w/2; py *= dp_m_lh_h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_lh*rotz_lh*vf).vec[0] + dp_m_lh_x;
		y = (roty_lh*rotz_lh*vf).vec[1] + dp_m_lh_y;
		z = (roty_lh*rotz_lh*vf).vec[2] + dp_m_lh_z;
		ret = inv.Pos2AngleRear(-x,y,z,dp_lhj1,dp_lhj2,dp_lhj3);

		dp_flag = true;
	}

	//WalkStop のときの処理
	if(walk_index == 0){
		j[0] = dp_rfj1;
		j[1] = dp_rfj2;
		j[2] = dp_rfj3;

		j[3] = dp_lfj1;
		j[4] = dp_lfj2;
		j[5] = dp_lfj3;

		j[6] = dp_rhj1;
		j[7] = dp_rhj2;
		j[8] = dp_rhj3;
		
		j[9] =  dp_lhj1;
		j[10] = dp_lhj2;
		j[11] = dp_lhj3;
		
		return true;
	}

	//軌道がはみ出たときver.
	for(int i = 0;i < num;i ++){
		t = 1.0*i/num;
//f
		locusfunction(t+phase[0],px,py);
		px *= myparam.legparam[0].w/2; py *= myparam.legparam[0].h/2;
		Vec4 vf(px,py,0,1);
		x = (roty_rf*rotz_rf*vf).vec[0] + myparam.legparam[0].x;
		y = (roty_rf*rotz_rf*vf).vec[1] + myparam.legparam[0].y;
		z = (roty_rf*rotz_rf*vf).vec[2] + myparam.legparam[0].z;
		ret = inv.Pos2AngleFront(x,y,z,rfj1,rfj2,rfj3);
		if(ret==0){
			set_rfj1[i]=777;//.push_back(777);
			set_rfj2[i]=777;//.push_back(777);
			set_rfj3[i]=777;//.push_back(777);
		}
		else{
			set_rfj1[i]=rfj1;//.push_back(rfj1);
			set_rfj2[i]=rfj2;//.push_back(rfj2);
			set_rfj3[i]=rfj3;//.push_back(rfj3);
		}

		
		locusfunction(t+phase[1],px,py);
		px *= myparam.legparam[1].w/2; py *= myparam.legparam[1].h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_lf*rotz_lf*vf).vec[0] + myparam.legparam[1].x;
		y = (roty_lf*rotz_lf*vf).vec[1] + myparam.legparam[1].y;
		z = (roty_lf*rotz_lf*vf).vec[2] + myparam.legparam[1].z;
		ret = inv.Pos2AngleFront(x,y,z,lfj1,lfj2,lfj3);
		if(ret==0){
			set_lfj1[i]=777;//.push_back(777);
			set_lfj2[i]=777;//.push_back(777);
			set_lfj3[i]=777;//.push_back(777);
		}
		else{
			set_lfj1[i]=lfj1;//.push_back(lfj1);
			set_lfj2[i]=lfj2;//.push_back(lfj2);
			set_lfj3[i]=lfj3;//.push_back(lfj3);
		}

//r
		locusfunction(t+phase[2],px,py);
		px *= myparam.legparam[2].w/2; py *= myparam.legparam[2].h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_rh*rotz_rh*vf).vec[0] + myparam.legparam[2].x;
		y = (roty_rh*rotz_rh*vf).vec[1] + myparam.legparam[2].y;
		z = (roty_rh*rotz_rh*vf).vec[2] + myparam.legparam[2].z;
		ret = inv.Pos2AngleRear(-x,y,z,rhj1,rhj2,rhj3);
		if(ret==0){
			set_rhj1[i]=777;//.push_back(777);
			set_rhj2[i]=777;//.push_back(777);
			set_rhj3[i]=777;//.push_back(777);
		}
		else{
			set_rhj1[i]=rhj1;//.push_back(rhj1);
			set_rhj2[i]=rhj2;//.push_back(rhj2);
			set_rhj3[i]=rhj3;//.push_back(rhj3);
		}


		locusfunction(t+phase[3],px,py);
		px *= myparam.legparam[3].w/2; py *= myparam.legparam[3].h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_lh*rotz_lh*vf).vec[0] + myparam.legparam[3].x;
		y = (roty_lh*rotz_lh*vf).vec[1] + myparam.legparam[3].y;
		z = (roty_lh*rotz_lh*vf).vec[2] + myparam.legparam[3].z; 
		ret = inv.Pos2AngleRear(-x,y,z,lhj1,lhj2,lhj3);
		if(ret==0){
			set_lhj1[i]=777;//.push_back(777);
			set_lhj2[i]=777;//.push_back(777);
			set_lhj3[i]=777;//.push_back(777);
		}
		else{
			set_lhj1[i]=lhj1;//.push_back(lhj1);
			set_lhj2[i]=lhj2;//.push_back(lhj2);
			set_lhj3[i]=lhj3;//.push_back(lhj3);
		}

		//tmp.Format("%4.1f, %4d, %4d, %4d,  %4d, %4d, %4d,  %4d, %4d, %4d,  %4d, %4d, %4d\n",totaltime/num,(int)rfj1,(int)rfj2,(int)rfj3,(int)lfj1,(int)lfj2,(int)lfj3,(int)rhj1,(int)rhj2,(int)rhj3,(int)lhj1,(int)lhj2,(int)lhj3);
		//str += tmp;
	}

	//角度値の線形補間　set_**j[0]が777のときの処理がまだてきとー
	for(int i=0;i<num;i++){
		t = 1.0*i/num;
		if(i==0){
			if(set_rfj1[0] == 777) set_rfj1[0] = dp_rfj1;
			if(set_rfj2[0] == 777) set_rfj2[0] = dp_rfj2;
			if(set_rfj3[0] == 777) set_rfj3[0] = dp_rfj3;
			if(set_lfj1[0] == 777) set_lfj1[0] = dp_lfj1;
			if(set_lfj2[0] == 777) set_lfj2[0] = dp_lfj2;
			if(set_lfj3[0] == 777) set_lfj3[0] = dp_lfj3;

			if(set_rhj1[0] == 777) set_rhj1[0] = dp_rhj1;
			if(set_rhj2[0] == 777) set_rhj2[0] = dp_rhj2;
			if(set_rhj3[0] == 777) set_rhj3[0] = dp_rhj3;
			if(set_lhj1[0] == 777) set_lhj1[0] = dp_lhj1;
			if(set_lhj2[0] == 777) set_lhj2[0] = dp_lhj2;
			if(set_lhj3[0] == 777) set_lhj3[0] = dp_lhj3;
		}

		//rfj
		if(set_rfj1[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rfj1[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rfj1[i] = dp_rfj1;
				break;
			}
			else{
				if(i == num-1)	set_rfj1[i] = (set_rfj1[0]-set_rfj1[i-1])/2.0;
				else			set_rfj1[i] = (set_rfj1[j]-set_rfj1[i-1])/(j-i+1);
			}
		}
        if(set_rfj2[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rfj2[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rfj2[i] = dp_rfj2;
				break;
			}
			else{
				if(i == num-1)	set_rfj2[i] = (set_rfj2[0]-set_rfj2[i-1])/2.0;
				else			set_rfj2[i] = (set_rfj2[j]-set_rfj2[i-1])/(j-i+1);
			}
		}
		if(set_rfj3[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rfj3[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rfj3[i] = dp_rfj3;
				break;
			}
			else{
				if(i == num-1)	set_rfj3[i] = (set_rfj3[0]-set_rfj3[i-1])/2.0;
				else			set_rfj3[i] = (set_rfj3[j]-set_rfj3[i-1])/(j-i+1);
			}
		}

		//lfj
		if(set_lfj1[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lfj1[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lfj1[i] = dp_lfj1;
				break;
			}
			else{
				if(i == num-1)	set_lfj1[i] = (set_lfj1[0]-set_lfj1[i-1])/2.0;
				else			set_lfj1[i] = (set_lfj1[j]-set_lfj1[i-1])/(j-i+1);
			}
		}
        if(set_lfj2[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lfj2[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lfj2[i] = dp_lfj2;
				break;
			}
			else{
				if(i == num-1)	set_lfj2[i] = (set_lfj2[0]-set_lfj2[i-1])/2.0;
				else			set_lfj2[i] = (set_lfj2[j]-set_lfj2[i-1])/(j-i+1);
			}
		}
		if(set_lfj3[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lfj3[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lfj3[i] = dp_lfj3;
				break;
			}
			else{
				if(i == num-1)	set_lfj3[i] = (set_lfj3[0]-set_lfj3[i-1])/2.0;
				else			set_lfj3[i] = (set_lfj3[j]-set_lfj3[i-1])/(j-i+1);
			}
		}

		//rhj
		if(set_rhj1[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rhj1[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rhj1[i] = dp_rhj1;
				break;
			}
			else{
				if(i == num-1)	set_rhj1[i] = (set_rhj1[0]-set_rhj1[i-1])/2.0;
				else			set_rhj1[i] = (set_rhj1[j]-set_rhj1[i-1])/(j-i+1);
			}
		}
        if(set_rhj2[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rhj2[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rhj2[i] = dp_rhj2;
				break;
			}
			else{
				if(i == num-1)	set_rhj2[i] = (set_rhj2[0]-set_rhj2[i-1])/2.0;
				else			set_rhj2[i] = (set_rhj2[j]-set_rhj2[i-1])/(j-i+1);
			}
		}
		if(set_rhj3[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rhj3[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rhj3[i] = dp_rhj3;
				break;
			}
			else{
				if(i == num-1)	set_rhj3[i] = (set_rhj3[0]-set_rhj3[i-1])/2.0;
				else			set_rhj3[i] = (set_rhj3[j]-set_rhj3[i-1])/(j-i+1);
			}
		}

		//lhj
		if(set_lhj1[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lhj1[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lhj1[i] = dp_lhj1;
				break;
			}
			else{
				if(i == num-1)	set_lhj1[i] = (set_lhj1[0]-set_lhj1[i-1])/2.0;
				else			set_lhj1[i] = (set_lhj1[j]-set_lhj1[i-1])/(j-i+1);
			}
		}
        if(set_lhj2[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lhj2[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lhj2[i] = dp_lhj2;
				break;
			}
			else{
				if(i == num-1)	set_lhj2[i] = (set_lhj2[0]-set_lhj2[i-1])/2.0;
				else			set_lhj2[i] = (set_lhj2[j]-set_lhj2[i-1])/(j-i+1);
			}
		}
		if(set_lhj3[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lhj3[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lhj3[i] = dp_lhj3;
				break;
			}
			else{
				if(i == num-1)	set_lhj3[i] = (set_lhj3[0]-set_lhj3[i-1])/2.0;
				else			set_lhj3[i] = (set_lhj3[j]-set_lhj3[i-1])/(j-i+1);
			}
		}

	}

	int t_num = phase_;//(int)(t_*num);
//	if(t_num < 0)		t_num = 0;
//	if(t_num >= num)	t_num = num-1;

	j[0] = set_rfj1[t_num];
	j[1] = set_rfj2[t_num];
	j[2] = set_rfj3[t_num];

	j[3] = set_lfj1[t_num];
	j[4] = set_lfj2[t_num];
	j[5] = set_lfj3[t_num];

	j[6] = set_rhj1[t_num];
	j[7] = set_rhj2[t_num];
	j[8] = set_rhj3[t_num];
	
	j[9] =  set_lhj1[t_num];
	j[10] = set_lhj2[t_num];
	j[11] = set_lhj3[t_num];

/*
	for(int i=0;i<num;i++){
		OSYSPRINT(("p_num=%d, %.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\n",
			i, set_rfj1[i],set_rfj2[i],set_rfj3[i], set_lfj1[i],set_lfj2[i],set_lfj3[i], set_rhj1[i],set_rhj2[i],set_rhj3[3], set_lhj1[i],set_lhj2[i],set_lhj3[3]));
	}
	OSYSPRINT(("set_j=$d,  %.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\n", t_num,j[0],j[1],j[2],j[3],j[4],j[5],j[6],j[7],j[8],j[9],j[10],j[11]));//
*/

	static unsigned int cnt=0;
	cnt++;
	if(false)//cnt%10==0)
	{
		OSYSPRINT(("MakeLocus2 TIME = %.1f\n", time_/1000.0));
	}

	return true;
}



void GaitMaker::UpdatePosition(double x, double y, double tt){
	//MCLのデッドレコニング
	DeadRecInformation info;
	info.dx = x;
	info.dy = y;
	info.dt = tt;
	DeadRec::UpdatePosition(info);

	//Predictorのアップデート
	int size = DetectBall::pred_x.data.size();
	double theta = tt*PI/180;
	for(int i = 0;i < size; i++){
		double x_ = DetectBall::pred_x.data[i];
		double y_ = DetectBall::pred_y.data[i];
		x_ -= x;
		y_ -= y;
		DetectBall::pred_x.data[i] = x_*cos(-theta) - y_*sin(-theta);
		DetectBall::pred_y.data[i] = x_*sin(-theta) + y_*cos(-theta);
	}

	state.goalDirection -= tt;
}


void GaitMaker::MakeLegParamForPreset(int index, LegDegParam &pLegParam, WalkParam myparam)
{
	//基本姿勢
	static int dp_m_rf_x = 60;
	static int dp_m_rf_y = 70;
	static int dp_m_rf_z = 15;
	static int dp_m_rf_w = 0;
	static int dp_m_rf_h = 0;
	static int dp_m_rf_t = 0;
	static int dp_m_rf_p = 0;

	static int dp_m_lf_x = 60;
	static int dp_m_lf_y = 70;
	static int dp_m_lf_z = 15;
	static int dp_m_lf_w = 0;
	static int dp_m_lf_h = 0;
	static int dp_m_lf_t = 0;
	static int dp_m_lf_p = 0;

	static int dp_m_rh_x = 0;
	static int dp_m_rh_y = 120;
	static int dp_m_rh_z = 0;
	static int dp_m_rh_w = 0;
	static int dp_m_rh_h = 0;
	static int dp_m_rh_t = 0;
	static int dp_m_rh_p = 0;

	static int dp_m_lh_x = 0;
	static int dp_m_lh_y = 120;
	static int dp_m_lh_z = 0;
	static int dp_m_lh_w = 0;
	static int dp_m_lh_h = 0;
	static int dp_m_lh_t = 0;
	static int dp_m_lh_p = 0;

	static double dp_m_gratio = 0.24;
	static double dp_m_aratio = 0.24;
	static int dp_m_time = 300;
	static int dp_m_num = 8;
	//ここまで基本姿勢

	double m_gratio = myparam.gratio;
	double m_aratio = myparam.aratio;

	double totaltime = myparam.time;
	int num = NUM_GAIT_POINT;//myparam.num;

	double rf_theta = -myparam.legparam[0].theta*PI/180;
	double lf_theta = -myparam.legparam[1].theta*PI/180;
	double rh_theta = myparam.legparam[2].theta*PI/180;
	double lh_theta = myparam.legparam[3].theta*PI/180;

	double rf_phi = myparam.legparam[0].phi*PI/180;
	double lf_phi = myparam.legparam[1].phi*PI/180;
	double rh_phi = myparam.legparam[2].phi*PI/180;
	double lh_phi = myparam.legparam[3].phi*PI/180;

	//double t_ = phase_ - (int)phase_;

	if(m_aratio == 0){
		m_aratio = (1 - m_gratio)/3;
	}

	double theta[4];
	double phi[4];
	double w[4];
	double h[4];

	for(int i = 0;i < 4;i++){
		theta[i] = myparam.legparam[i].theta * PI/180;
		phi[i]   = myparam.legparam[i].phi * PI/180;
		w[i]     = myparam.legparam[i].w;
		h[i]     = myparam.legparam[i].h;
	}


	double phase[4] = {0.0, 0.5, 0.5, 0.0};

#define locusfunction getUnitRectLocus


	Mat44 rotz_rf(	cos(rf_phi),	-sin(rf_phi),	0,	0,	
					sin(rf_phi),	cos(rf_phi),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);

	Mat44 rotz_lf(	cos(lf_phi),	-sin(lf_phi),	0,	0,	
					sin(lf_phi),	cos(lf_phi),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);

	Mat44 rotz_rh(	cos(rh_phi),	-sin(rh_phi),	0,	0,	
					sin(rh_phi),	cos(rh_phi),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);

	Mat44 rotz_lh(	cos(lh_phi),	-sin(lh_phi),	0,	0,	
					sin(lh_phi),	cos(lh_phi),	0,	0,
					0,	0,	1,	0,
					0,	0,	0,	1	);


	Mat44 roty_rf(	cos(rf_theta),	0,-sin(rf_theta),	0,	
					0,	1,	0,	0,
					sin(rf_theta),	0, cos(rf_theta),	0,
					0,	0,	0,	1	);

	Mat44 roty_lf(	cos(-lf_theta),	0,-sin(-lf_theta),	0,	
					0,	1,	0,	0,
					sin(-lf_theta),	0, cos(-lf_theta),	0,
					0,	0,	0,	1	);

	Mat44 roty_rh(	cos(-rh_theta),	0,-sin(-rh_theta),	0,	
					0,	1,	0,	0,
					sin(-rh_theta),	0, cos(-rh_theta),	0,
					0,	0,	0,	1	);

	Mat44 roty_lh(	cos(lh_theta),	0,-sin(lh_theta),	0,	
					0,	1,	0,	0,
					sin(lh_theta),	0, cos(lh_theta),	0,
					0,	0,	0,	1	);


	//ここから軌道生成
	Inverter inv;
	double x,y,z,rfj1,rfj2,rfj3,rhj1,rhj2,rhj3,lfj1,lfj2,lfj3,lhj1,lhj2,lhj3;
	static double dp_rfj1,dp_rfj2,dp_rfj3,dp_rhj1,dp_rhj2,dp_rhj3,dp_lfj1,dp_lfj2,dp_lfj3,dp_lhj1,dp_lhj2,dp_lhj3;
	//vector<double> set_rfj1,set_rfj3,set_rhj1,set_rhj2,set_rhj3,set_lfj1,set_lfj2,set_lfj3,set_lhj1,set_lhj2,set_lhj3;
	static double set_rfj1[NUM_GAIT_POINT],set_rfj2[NUM_GAIT_POINT],set_rfj3[NUM_GAIT_POINT],set_rhj1[NUM_GAIT_POINT],set_rhj2[NUM_GAIT_POINT],set_rhj3[NUM_GAIT_POINT],set_lfj1[NUM_GAIT_POINT],set_lfj2[NUM_GAIT_POINT],set_lfj3[NUM_GAIT_POINT],set_lhj1[NUM_GAIT_POINT],set_lhj2[NUM_GAIT_POINT],set_lhj3[NUM_GAIT_POINT];

	static bool dp_flag = false;

	double t;
	double px,py;
	int ret;
	//規定値
	if(!dp_flag){
		locusfunction(0.0+phase[0],px,py);
		px *= dp_m_rf_w/2; py *= dp_m_rf_h/2;
		Vec4 vf(px,py,0,1);
		x = (roty_rf*rotz_rf*vf).vec[0] + dp_m_rf_x;
		y = (roty_rf*rotz_rf*vf).vec[1] + dp_m_rf_y;
		z = (roty_rf*rotz_rf*vf).vec[2] + dp_m_rf_z;
		ret = inv.Pos2AngleFront(x,y,z,dp_rfj1,dp_rfj2,dp_rfj3);

		locusfunction(0.0+phase[1],px,py);
		px *= dp_m_lf_w/2; py *= dp_m_lf_h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_lf*rotz_lf*vf).vec[0] + dp_m_lf_x;
		y = (roty_lf*rotz_lf*vf).vec[1] + dp_m_lf_y;
		z = (roty_lf*rotz_lf*vf).vec[2] + dp_m_lf_z;
		ret = inv.Pos2AngleFront(x,y,z,dp_lfj1,dp_lfj2,dp_lfj3);

		locusfunction(0.0+phase[2],px,py);
		px *= dp_m_rh_w/2; py *= dp_m_rh_h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_rh*rotz_rh*vf).vec[0] + dp_m_rh_x;
		y = (roty_rh*rotz_rh*vf).vec[1] + dp_m_rh_y;
		z = (roty_rh*rotz_rh*vf).vec[2] + dp_m_rh_z;
		ret = inv.Pos2AngleRear(-x,y,z,dp_rhj1,dp_rhj2,dp_rhj3);

		locusfunction(0.0+phase[3],px,py);
		px *= dp_m_lh_w/2; py *= dp_m_lh_h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_lh*rotz_lh*vf).vec[0] + dp_m_lh_x;
		y = (roty_lh*rotz_lh*vf).vec[1] + dp_m_lh_y;
		z = (roty_lh*rotz_lh*vf).vec[2] + dp_m_lh_z;
		ret = inv.Pos2AngleRear(-x,y,z,dp_lhj1,dp_lhj2,dp_lhj3);

		dp_flag = true;
	}

	//WalkStop のときの処理
	if(index == 0){
		for(int i=0;i<NUM_GAIT_POINT;i++)
		{
            pLegParam.rfj1[i] = dp_rfj1;
			pLegParam.rfj2[i] = dp_rfj2;
			pLegParam.rfj3[i] = dp_rfj3;

			pLegParam.lfj1[i] = dp_lfj1;
			pLegParam.lfj2[i] = dp_lfj2;
			pLegParam.lfj3[i] = dp_lfj3;

			pLegParam.rhj1[i] = dp_rhj1;
			pLegParam.rhj2[i] = dp_rhj2;
			pLegParam.rhj3[i] = dp_rhj3;

			pLegParam.lhj1[i] = dp_lhj1;
			pLegParam.lhj2[i] = dp_lhj2;
			pLegParam.lhj3[i] = dp_lhj3;
		}
		return;
	}

	//軌道がはみ出たときver.
	for(int i = 0;i < num;i ++){
		t = 1.0*i/num;
//f
		locusfunction(t+phase[0],px,py);
		px *= myparam.legparam[0].w/2; py *= myparam.legparam[0].h/2;
		Vec4 vf(px,py,0,1);
		x = (roty_rf*rotz_rf*vf).vec[0] + myparam.legparam[0].x;
		y = (roty_rf*rotz_rf*vf).vec[1] + myparam.legparam[0].y;
		z = (roty_rf*rotz_rf*vf).vec[2] + myparam.legparam[0].z;
		ret = inv.Pos2AngleFront(x,y,z,rfj1,rfj2,rfj3);
		if(ret==0){
			set_rfj1[i]=777;//.push_back(777);
			set_rfj2[i]=777;//.push_back(777);
			set_rfj3[i]=777;//.push_back(777);
		}
		else{
			set_rfj1[i]=rfj1;//.push_back(rfj1);
			set_rfj2[i]=rfj2;//.push_back(rfj2);
			set_rfj3[i]=rfj3;//.push_back(rfj3);
		}

		
		locusfunction(t+phase[1],px,py);
		px *= myparam.legparam[1].w/2; py *= myparam.legparam[1].h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_lf*rotz_lf*vf).vec[0] + myparam.legparam[1].x;
		y = (roty_lf*rotz_lf*vf).vec[1] + myparam.legparam[1].y;
		z = (roty_lf*rotz_lf*vf).vec[2] + myparam.legparam[1].z;
		ret = inv.Pos2AngleFront(x,y,z,lfj1,lfj2,lfj3);
		if(ret==0){
			set_lfj1[i]=777;//.push_back(777);
			set_lfj2[i]=777;//.push_back(777);
			set_lfj3[i]=777;//.push_back(777);
		}
		else{
			set_lfj1[i]=lfj1;//.push_back(lfj1);
			set_lfj2[i]=lfj2;//.push_back(lfj2);
			set_lfj3[i]=lfj3;//.push_back(lfj3);
		}

//r
		locusfunction(t+phase[2],px,py);
		px *= myparam.legparam[2].w/2; py *= myparam.legparam[2].h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_rh*rotz_rh*vf).vec[0] + myparam.legparam[2].x;
		y = (roty_rh*rotz_rh*vf).vec[1] + myparam.legparam[2].y;
		z = (roty_rh*rotz_rh*vf).vec[2] + myparam.legparam[2].z;
		ret = inv.Pos2AngleRear(-x,y,z,rhj1,rhj2,rhj3);
		if(ret==0){
			set_rhj1[i]=777;//.push_back(777);
			set_rhj2[i]=777;//.push_back(777);
			set_rhj3[i]=777;//.push_back(777);
		}
		else{
			set_rhj1[i]=rhj1;//.push_back(rhj1);
			set_rhj2[i]=rhj2;//.push_back(rhj2);
			set_rhj3[i]=rhj3;//.push_back(rhj3);
		}


		locusfunction(t+phase[3],px,py);
		px *= myparam.legparam[3].w/2; py *= myparam.legparam[3].h/2;
		vf.vec[0] = px;vf.vec[1] = py;
		x = (roty_lh*rotz_lh*vf).vec[0] + myparam.legparam[3].x;
		y = (roty_lh*rotz_lh*vf).vec[1] + myparam.legparam[3].y;
		z = (roty_lh*rotz_lh*vf).vec[2] + myparam.legparam[3].z; 
		ret = inv.Pos2AngleRear(-x,y,z,lhj1,lhj2,lhj3);
		if(ret==0){
			set_lhj1[i]=777;//.push_back(777);
			set_lhj2[i]=777;//.push_back(777);
			set_lhj3[i]=777;//.push_back(777);
		}
		else{
			set_lhj1[i]=lhj1;//.push_back(lhj1);
			set_lhj2[i]=lhj2;//.push_back(lhj2);
			set_lhj3[i]=lhj3;//.push_back(lhj3);
		}

		//tmp.Format("%4.1f, %4d, %4d, %4d,  %4d, %4d, %4d,  %4d, %4d, %4d,  %4d, %4d, %4d\n",totaltime/num,(int)rfj1,(int)rfj2,(int)rfj3,(int)lfj1,(int)lfj2,(int)lfj3,(int)rhj1,(int)rhj2,(int)rhj3,(int)lhj1,(int)lhj2,(int)lhj3);
		//str += tmp;
	}

	//角度値の線形補間　set_**j[0]が777のときの処理がまだてきとー
	for(int i=0;i<num;i++){
		t = 1.0*i/num;
		if(i==0){
			if(set_rfj1[0] == 777) set_rfj1[0] = dp_rfj1;
			if(set_rfj2[0] == 777) set_rfj2[0] = dp_rfj2;
			if(set_rfj3[0] == 777) set_rfj3[0] = dp_rfj3;
			if(set_lfj1[0] == 777) set_lfj1[0] = dp_lfj1;
			if(set_lfj2[0] == 777) set_lfj2[0] = dp_lfj2;
			if(set_lfj3[0] == 777) set_lfj3[0] = dp_lfj3;

			if(set_rhj1[0] == 777) set_rhj1[0] = dp_rhj1;
			if(set_rhj2[0] == 777) set_rhj2[0] = dp_rhj2;
			if(set_rhj3[0] == 777) set_rhj3[0] = dp_rhj3;
			if(set_lhj1[0] == 777) set_lhj1[0] = dp_lhj1;
			if(set_lhj2[0] == 777) set_lhj2[0] = dp_lhj2;
			if(set_lhj3[0] == 777) set_lhj3[0] = dp_lhj3;
		}

		//rfj
		if(set_rfj1[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rfj1[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rfj1[i] = dp_rfj1;
				break;
			}
			else{
				if(i == num-1)	set_rfj1[i] = (set_rfj1[0]-set_rfj1[i-1])/2.0;
				else			set_rfj1[i] = (set_rfj1[j]-set_rfj1[i-1])/(j-i+1);
			}
		}
        if(set_rfj2[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rfj2[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rfj2[i] = dp_rfj2;
				break;
			}
			else{
				if(i == num-1)	set_rfj2[i] = (set_rfj2[0]-set_rfj2[i-1])/2.0;
				else			set_rfj2[i] = (set_rfj2[j]-set_rfj2[i-1])/(j-i+1);
			}
		}
		if(set_rfj3[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rfj3[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rfj3[i] = dp_rfj3;
				break;
			}
			else{
				if(i == num-1)	set_rfj3[i] = (set_rfj3[0]-set_rfj3[i-1])/2.0;
				else			set_rfj3[i] = (set_rfj3[j]-set_rfj3[i-1])/(j-i+1);
			}
		}

		//lfj
		if(set_lfj1[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lfj1[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lfj1[i] = dp_lfj1;
				break;
			}
			else{
				if(i == num-1)	set_lfj1[i] = (set_lfj1[0]-set_lfj1[i-1])/2.0;
				else			set_lfj1[i] = (set_lfj1[j]-set_lfj1[i-1])/(j-i+1);
			}
		}
        if(set_lfj2[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lfj2[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lfj2[i] = dp_lfj2;
				break;
			}
			else{
				if(i == num-1)	set_lfj2[i] = (set_lfj2[0]-set_lfj2[i-1])/2.0;
				else			set_lfj2[i] = (set_lfj2[j]-set_lfj2[i-1])/(j-i+1);
			}
		}
		if(set_lfj3[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lfj3[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lfj3[i] = dp_lfj3;
				break;
			}
			else{
				if(i == num-1)	set_lfj3[i] = (set_lfj3[0]-set_lfj3[i-1])/2.0;
				else			set_lfj3[i] = (set_lfj3[j]-set_lfj3[i-1])/(j-i+1);
			}
		}

		//rhj
		if(set_rhj1[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rhj1[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rhj1[i] = dp_rhj1;
				break;
			}
			else{
				if(i == num-1)	set_rhj1[i] = (set_rhj1[0]-set_rhj1[i-1])/2.0;
				else			set_rhj1[i] = (set_rhj1[j]-set_rhj1[i-1])/(j-i+1);
			}
		}
        if(set_rhj2[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rhj2[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rhj2[i] = dp_rhj2;
				break;
			}
			else{
				if(i == num-1)	set_rhj2[i] = (set_rhj2[0]-set_rhj2[i-1])/2.0;
				else			set_rhj2[i] = (set_rhj2[j]-set_rhj2[i-1])/(j-i+1);
			}
		}
		if(set_rhj3[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_rhj3[j] != 777) break;
				j++;
			}
			if(j==num){
				set_rhj3[i] = dp_rhj3;
				break;
			}
			else{
				if(i == num-1)	set_rhj3[i] = (set_rhj3[0]-set_rhj3[i-1])/2.0;
				else			set_rhj3[i] = (set_rhj3[j]-set_rhj3[i-1])/(j-i+1);
			}
		}

		//lhj
		if(set_lhj1[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lhj1[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lhj1[i] = dp_lhj1;
				break;
			}
			else{
				if(i == num-1)	set_lhj1[i] = (set_lhj1[0]-set_lhj1[i-1])/2.0;
				else			set_lhj1[i] = (set_lhj1[j]-set_lhj1[i-1])/(j-i+1);
			}
		}
        if(set_lhj2[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lhj2[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lhj2[i] = dp_lhj2;
				break;
			}
			else{
				if(i == num-1)	set_lhj2[i] = (set_lhj2[0]-set_lhj2[i-1])/2.0;
				else			set_lhj2[i] = (set_lhj2[j]-set_lhj2[i-1])/(j-i+1);
			}
		}
		if(set_lhj3[i] == 777){
			int j=i+1;
			while(j<num){
				if(set_lhj3[j] != 777) break;
				j++;
			}
			if(j==num){
				set_lhj3[i] = dp_lhj3;
				break;
			}
			else{
				if(i == num-1)	set_lhj3[i] = (set_lhj3[0]-set_lhj3[i-1])/2.0;
				else			set_lhj3[i] = (set_lhj3[j]-set_lhj3[i-1])/(j-i+1);
			}
		}

	}

	for(int i=0;i<NUM_GAIT_POINT;i++)
	{
        pLegParam.rfj1[i] = set_rfj1[i];
		pLegParam.rfj2[i] = set_rfj2[i];
		pLegParam.rfj3[i] = set_rfj3[i];

		pLegParam.lfj1[i] = set_lfj1[i];
		pLegParam.lfj2[i] = set_lfj2[i];
		pLegParam.lfj3[i] = set_lfj3[i];

		pLegParam.rhj1[i] = set_rhj1[i];
		pLegParam.rhj2[i] = set_rhj2[i];
		pLegParam.rhj3[i] = set_rhj3[i];

		pLegParam.lhj1[i] = set_lhj1[i];
		pLegParam.lhj2[i] = set_lhj2[i];
		pLegParam.lhj3[i] = set_lhj3[i];
	}

	//OSYSPRINT(("index = %d, rfj1[0] = %.1f, rfj1[1] = %.1f, rfj1[2] = %.1f, rfj1[3] = %.1f\n",
	//	index, pLegParam.rfj1[0], pLegParam.rfj1[1], pLegParam.rfj1[2], pLegParam.rfj1[3] ));
    
}

bool GaitMaker::MakeLocus3(unsigned char walk_index, double *j,int phase_)
{

	int t_num = phase_;

	j[0] = SetLegParam[walk_index].rfj1[t_num];
	j[1] = SetLegParam[walk_index].rfj2[t_num];
	j[2] = SetLegParam[walk_index].rfj3[t_num];

	j[3] = SetLegParam[walk_index].lfj1[t_num];
	j[4] = SetLegParam[walk_index].lfj2[t_num];
	j[5] = SetLegParam[walk_index].lfj3[t_num];

	j[6] = SetLegParam[walk_index].rhj1[t_num];
	j[7] = SetLegParam[walk_index].rhj2[t_num];
	j[8] = SetLegParam[walk_index].rhj3[t_num];
	
	j[9] =  SetLegParam[walk_index].lhj1[t_num];
	j[10] = SetLegParam[walk_index].lhj2[t_num];
	j[11] = SetLegParam[walk_index].lhj3[t_num];

	return true;
}
