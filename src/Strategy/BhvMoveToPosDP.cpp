#include "StdAfx.h"
#include "BehaviorList.h"
#include "../Motion/PresetWalkParam.h" //takeshita 05/0517
#include "../Motion/GaitMaker.h"
#include <math.h>

#include <OPENR/OSyslog.h>

//PresetWalkParam BhvMoveToPosition::presetWalkParam;


#define DIM 3
#define DIST_NUM 59// 39 //ball distance 
#define BTT_NUM 36 //Ball theta
#define RTT_NUM 24 //Robot theta
#define BTT_NUM2 36 //Ball theta

#define MAPSIZE (DIST_NUM*BTT_NUM*RTT_NUM)
#define MAPSIZE2 (DIST_NUM*BTT_NUM2*RTT_NUM)

/*
static int dist_min[DIST_NUM+1];
static int btt_min[BTT_NUM+1];
static int rtt_min[RTT_NUM+1];
*/


const int m2p_x_elem = 120;//60;//21;
const int m2p_y_elem = 120;//60;//15;
const int m2p_theta_elem = 36;//18;
const int m2p_full_elem = m2p_x_elem*m2p_y_elem*m2p_theta_elem;

const int m2p_x_wid = 12000/m2p_x_elem;
const int m2p_y_wid = 12000/m2p_y_elem;
const int m2p_t_wid = 360/m2p_theta_elem;

static int m2p_x_under[m2p_x_elem+1];
static int m2p_y_under[m2p_y_elem+1];
static int m2p_theta_under[m2p_theta_elem+1];

const int loconv[]
={
	0,
	//竹下Ver.
	WALK_STOP,// 0.0 0.0 0.0 300
	WALK_FORWARD,// 139.77 11.96 -0.30 300
	WALK_BACK,// -104.87 6.22 0.63 300
	WALK_LEFT15,// 132.11 27.13 -1.00 300
	WALK_LEFT30,// 122.00 67.67 0.00 300
	WALK_LEFT60,// 66.22 98.86 -0.10 300
	WALK_LEFT90,// 7.26 117.84 0.57 300
	WALK_LEFT120,// -40.23 100.42 0.70 300
	WALK_LEFT150,// -107.42 85.28 0.83 300
	WALK_RIGHT15,// 131.99 -45.85 -0.67 300
	WALK_RIGHT30,// 126.80 -77.61 1.00 300
	WALK_RIGHT60,// 75.90 -112.84 -0.43 300
	WALK_RIGHT90,// -9.65 -112.15 0.30 300
	WALK_RIGHT120,// -47.22 -104.50 -0.47 300
	WALK_RIGHT150,// -122.18 -52.76 0.20 300
	WALK_ROLL_LEFT10,// -4.17 50.77 -26.17 300
	WALK_ROLL_LEFT20,// 0.48 67.19 -18.25 300
	WALK_ROLL_LEFT40,// 6.84 72.00 -10.00 300
	WALK_ROLL_LEFT80,// 9.83 105.12 -4.00 300
	WALK_ROLL_RIGHT10,// -4.22 -69.45 31.75 300
	WALK_ROLL_RIGHT20,// 7.99 -66.86 21.00 300
	WALK_ROLL_RIGHT40,// -5.34 -74.05 9.75 300
	WALK_ROLL_RIGHT80,// -6.36 -100.41 7.50 300
	WALK_TURN_LEFT20,// 76.44 18.89 20.50 300
	WALK_TURN_LEFT40,// 115.36 48.28 16.75 300
	WALK_TURN_LEFT80,// 137.97 26.95 10.00 300
	WALK_TURN_RIGHT20,// 74.42 -30.07 -17.00 300
	WALK_TURN_RIGHT40,// 118.59 -47.23 -16.17 300
	WALK_TURN_RIGHT80,// 137.12 -20.73 -10.50 300
	WALK_SMALL10_RIGHT90,// 0 -50 0 200
	WALK_SMALL10_RIGHT80,// 8.6 -49 0 200
	WALK_SMALL10_RIGHT70,// 17.1 -46.8 0 200
	WALK_SMALL10_RIGHT60,// 25 -43.3 0 200
	WALK_SMALL10_RIGHT50,// 32.1 -38.3 0 200
	WALK_SMALL10_RIGHT40,// 38.3 -32.1 0 200
	WALK_SMALL10_RIGHT30,// 43.3 -25 0 200
	WALK_SMALL10_RIGHT20,// 46.8 -17.1 0 200
	WALK_SMALL10_RIGHT10,// 49.2 -8.6 0 200
	WALK_SMALL10_FORWARD,// 50 0 0 200
	WALK_SMALL10_LEFT10,// 49.2 8.6 0 200
	WALK_SMALL10_LEFT20,// 46.8 17.1 0 200
	WALK_SMALL10_LEFT30,// 43.3 25 0 200
	WALK_SMALL10_LEFT40,// 38.3 32.1 0 200
	WALK_SMALL10_LEFT50,// 32.1 38.3 0 200
	WALK_SMALL10_LEFT60,// 25 43.3 0 200
	WALK_SMALL10_LEFT70,// 17.1 46.8 0 200
	WALK_SMALL10_LEFT80,// 8.6 49 0 200
	WALK_SMALL10_LEFT90,// 0 50 0 200



/* 実川Ver.
	SMALL_FORWARD,// 17.39 2.43 0.60 200
	WALK_FORWARD,// 130.68 -11.17 0.60 300
	WALK_BACK,// -104.00 -0.50 -0.00 280
	WALK_LEFT15,// 119.96 36.71 -0.60 300
	WALK_LEFT30,// 110.81 51.36 -1.05 300
	WALK_LEFT60,// 47.51 65.71 -1.50 280
	WALK_LEFT90,// -1.76 84.69 0.55 280
	WALK_LEFT120,// -61.35 84.63 2.10 270
	WALK_LEFT150,// -93.89 56.66 0.75 270
	WALK_RIGHT15,// 120.76 -33.36 0.30 290
	WALK_RIGHT30,// 110.21 -63.45 0.95 300
	WALK_RIGHT60,// 46.16 -77.82 2.30 300
	WALK_RIGHT90,// 6.93 -79.27 0.90 270
	WALK_RIGHT120,// -33.91 -76.61 0.15 270
	WALK_RIGHT150,// -80.06 -50.63 2.15 270
	WALK_TURN_LEFT20,// 58.10 16.59 12.10 250
	WALK_TURN_LEFT40,// 93.31 38.47 10.25 270
	WALK_TURN_LEFT80,// 117.75 -2.35 9.25 280
	WALK_TURN_RIGHT20,// 60.62 -24.85 -12.30 250
	WALK_TURN_RIGHT40,// 72.61 -21.68 -8.50 260
	WALK_TURN_RIGHT80,// 117.68 -0.80 -8.25 290
	WALK_ROLL_LEFT40,// 3.22 54.71 -10.00 290
	WALK_ROLL_RIGHT40,// 3.94 -46.95 6.50 280
	WALK_ROTATE_LEFT90,// 1.61 10.60 12.70 360
	WALK_ROTATE_RIGHT90,// 1.25 -10.32 -11.80 290
*/
};

const int gconv[]
={
	0,
//竹下Ver.
	WALK_STOP,// 0.0 0.0 0.0 300
	WALK_FORWARD,// 139.77 11.96 -0.30 300
	WALK_BACK,// -104.87 6.22 0.63 300
	WALK_LEFT15,// 132.11 27.13 -1.00 300
	WALK_LEFT30,// 122.00 67.67 0.00 300
	WALK_LEFT60,// 66.22 98.86 -0.10 300
	WALK_LEFT90,// 7.26 117.84 0.57 300
	WALK_LEFT120,// -40.23 100.42 0.70 300
	WALK_LEFT150,// -107.42 85.28 0.83 300
	WALK_RIGHT15,// 131.99 -45.85 -0.67 300
	WALK_RIGHT30,// 126.80 -77.61 1.00 300
	WALK_RIGHT60,// 75.90 -112.84 -0.43 300
	WALK_RIGHT90,// -9.65 -112.15 0.30 300
	WALK_RIGHT120,// -47.22 -104.50 -0.47 300
	WALK_RIGHT150,// -122.18 -52.76 0.20 300
	WALK_ROLL_LEFT10,// -4.17 50.77 -26.17 300
	WALK_ROLL_LEFT20,// 0.48 67.19 -18.25 300
	WALK_ROLL_LEFT40,// 6.84 72.00 -10.00 300
	WALK_ROLL_LEFT80,// 9.83 105.12 -4.00 300
	WALK_ROLL_RIGHT10,// -4.22 -69.45 31.75 300
	WALK_ROLL_RIGHT20,// 7.99 -66.86 21.00 300
	WALK_ROLL_RIGHT40,// -5.34 -74.05 9.75 300
	WALK_ROLL_RIGHT80,// -6.36 -100.41 7.50 300
	WALK_TURN_LEFT20,// 76.44 18.89 20.50 300
	WALK_TURN_LEFT40,// 115.36 48.28 16.75 300
	WALK_TURN_LEFT80,// 137.97 26.95 10.00 300
	WALK_TURN_RIGHT20,// 74.42 -30.07 -17.00 300
	WALK_TURN_RIGHT40,// 118.59 -47.23 -16.17 300
	WALK_TURN_RIGHT80,// 137.12 -20.73 -10.50 300
	WALK_SMALL10_RIGHT90,// 0 -50 0 200
	WALK_SMALL10_RIGHT80,// 8.6 -49 0 200
	WALK_SMALL10_RIGHT70,// 17.1 -46.8 0 200
	WALK_SMALL10_RIGHT60,// 25 -43.3 0 200
	WALK_SMALL10_RIGHT50,// 32.1 -38.3 0 200
	WALK_SMALL10_RIGHT40,// 38.3 -32.1 0 200
	WALK_SMALL10_RIGHT30,// 43.3 -25 0 200
	WALK_SMALL10_RIGHT20,// 46.8 -17.1 0 200
	WALK_SMALL10_RIGHT10,// 49.2 -8.6 0 200
	WALK_SMALL10_FORWARD,// 50 0 0 200
	WALK_SMALL10_LEFT10,// 49.2 8.6 0 200
	WALK_SMALL10_LEFT20,// 46.8 17.1 0 200
	WALK_SMALL10_LEFT30,// 43.3 25 0 200
	WALK_SMALL10_LEFT40,// 38.3 32.1 0 200
	WALK_SMALL10_LEFT50,// 32.1 38.3 0 200
	WALK_SMALL10_LEFT60,// 25 43.3 0 200
	WALK_SMALL10_LEFT70,// 17.1 46.8 0 200
	WALK_SMALL10_LEFT80,// 8.6 49 0 200
	WALK_SMALL10_LEFT90,// 0 50 0 200


/* 実川Ver.
	SMALL_FORWARD,// 17.39 2.43 0.60 200
	WALK_FORWARD,// 130.68 -11.17 0.60 300
	WALK_BACK,// -104.00 -0.50 -0.00 280
	WALK_LEFT15,// 119.96 36.71 -0.60 300
	WALK_LEFT30,// 110.81 51.36 -1.05 300
	WALK_LEFT60,// 47.51 65.71 -1.50 280
	WALK_LEFT90,// -1.76 84.69 0.55 280
	WALK_LEFT120,// -61.35 84.63 2.10 270
	WALK_LEFT150,// -93.89 56.66 0.75 270
	WALK_RIGHT15,// 120.76 -33.36 0.30 290
	WALK_RIGHT30,// 110.21 -63.45 0.95 300
	WALK_RIGHT60,// 46.16 -77.82 2.30 300
	WALK_RIGHT90,// 6.93 -79.27 0.90 270
	WALK_RIGHT120,// -33.91 -76.61 0.15 270
	WALK_RIGHT150,// -80.06 -50.63 2.15 270
	WALK_TURN_LEFT20,// 58.10 16.59 12.10 250
	WALK_TURN_LEFT40,// 93.31 38.47 10.25 270
	WALK_TURN_LEFT80,// 117.75 -2.35 9.25 280
	WALK_TURN_RIGHT20,// 60.62 -24.85 -12.30 250
	WALK_TURN_RIGHT40,// 72.61 -21.68 -8.50 260
	WALK_TURN_RIGHT80,// 117.68 -0.80 -8.25 290
	WALK_ROLL_LEFT40,// 3.22 54.71 -10.00 290
	WALK_ROLL_RIGHT40,// 3.94 -46.95 6.50 280
*/
};

const int conv[]
={	0,

	WALK_FORWARD,// 139.77 11.96 -0.30 300
	WALK_BACK,// -104.87 6.22 0.63 300
	WALK_LEFT15,// 132.11 27.13 -1.00 300
	WALK_LEFT30,// 122.00 67.67 0.00 300
	WALK_LEFT60,// 66.22 98.86 -0.10 300
	WALK_LEFT90,// 7.26 117.84 0.57 300
	WALK_LEFT120,// -40.23 100.42 0.70 300
	WALK_LEFT150,// -107.42 85.28 0.83 300
	WALK_RIGHT15,// 131.99 -45.85 -0.67 300
	WALK_RIGHT30,// 126.80 -77.61 1.00 300
	WALK_RIGHT60,// 75.90 -112.84 -0.43 300
	WALK_RIGHT90,// -9.65 -112.15 0.30 300
	WALK_RIGHT120,// -47.22 -104.50 -0.47 300
	WALK_RIGHT150,// -122.18 -52.76 0.20 300
	WALK_ROLL_LEFT10,// -4.17 50.77 -26.17 300
	WALK_ROLL_LEFT20,// 0.48 67.19 -18.25 300
	WALK_ROLL_LEFT40,// 6.84 72.00 -10.00 300
	WALK_ROLL_LEFT80,// 9.83 105.12 -4.00 300
	WALK_ROLL_RIGHT10,// -4.22 -69.45 31.75 300
	WALK_ROLL_RIGHT20,// 7.99 -66.86 21.00 300
	WALK_ROLL_RIGHT40,// -5.34 -74.05 9.75 300
	WALK_ROLL_RIGHT80,// -6.36 -100.41 7.50 300
	WALK_ROTATE_LEFT180,// 0.12 26.70 22.00 300
	WALK_ROTATE_LEFT270,// 2.07 19.44 29.25 300
	WALK_ROTATE_RIGHT180,// -11.18 -20.27 -22.00 300
	WALK_ROTATE_RIGHT270,// 1.11 -28.17 -30.50 300
	WALK_TURN_LEFT20,// 76.44 18.89 20.50 300
	WALK_TURN_LEFT40,// 115.36 48.28 16.75 300
	WALK_TURN_LEFT80,// 137.97 26.95 10.00 300
	WALK_TURN_RIGHT20,// 74.42 -30.07 -17.00 300
	WALK_TURN_RIGHT40,// 118.59 -47.23 -16.17 300
	WALK_TURN_RIGHT80,// 137.12 -20.73 -10.50 300
/*
//高速な歩行だけver.
	WALK_FORWARD,// 134.5 -2.75 -0.15 300
	WALK_BACK,// -103.5 1.75 -0.9 280
	WALK_LEFT15,// 119.0 29.5 -1.2 300
	WALK_LEFT30,// 101.75 70.0 0.3 300
	WALK_LEFT90,// 2.0 78.0 0 270
	WALK_LEFT120,// -49.5 87.5 -0.3 270
	WALK_LEFT150,// -103.0 52.0 0.3 270
	WALK_RIGHT15,// 115.75 -36.25 -0.15 290
	WALK_RIGHT30,// 104.5 -63.5 0 300
	WALK_RIGHT90,// 2.5 -77.75 0.45 270
	WALK_RIGHT150,// -84.25 -56.0 -0.15 270
	WALK_TURN_LEFT40,// 29.75 75.75 12.0 270
	WALK_TURN_LEFT80,// 79.0 67.5 7.2 280
	WALK_TURN_RIGHT40,// 35.5 -50.0 -9.0 260
	WALK_TURN_RIGHT80,// 84.5 -73.75 -7.5 290
	WALK_ROTATE_LEFT90,// -1.75 4.25 10.35 360
	WALK_ROTATE_LEFT180,// -4.75 -13.5 20.2 400
	WALK_ROTATE_RIGHT180,// -8.85 5.1 -20.55 370
*/
/*
WALK_FORWARD,// 134.5 -2.75 -0.15 300
WALK_BACK,// -103.5 1.75 -0.9 280
WALK_LEFT15,// 119.0 29.5 -1.2 300
WALK_LEFT30,// 101.75 70.0 0.3 300
WALK_LEFT60,// 40.25 54.25 -2.25 280
WALK_LEFT90,// 2.0 78.0 0 270
WALK_LEFT120,// -49.5 87.5 -0.3 270
WALK_LEFT150,// -103.0 52.0 0.3 270
WALK_RIGHT15,// 115.75 -36.25 -0.15 290
WALK_RIGHT30,// 104.5 -63.5 0 300
WALK_RIGHT60,// 58.33 -81.5 0.3 300
WALK_RIGHT90,// 2.5 -77.75 0.45 270
WALK_RIGHT120,// -44.0 -74.25 0.45 270
WALK_RIGHT150,// -84.25 -56.0 -0.15 270
WALK_TURN_LEFT20,// 13.75 44.25 12.05 250
WALK_TURN_LEFT40,// 29.75 75.75 12.0 270
WALK_TURN_LEFT80,// 79.0 67.5 7.2 280
WALK_TURN_RIGHT20,// 3.75 -50.5 -12.9 250
WALK_TURN_RIGHT40,// 35.5 -50.0 -9.0 260
WALK_TURN_RIGHT80,// 84.5 -73.75 -7.5 290
WALK_ROLL_LEFT20,// 32.0 18.25 -10.5 240
WALK_ROLL_LEFT40,// 33.0 40.25 -8.7 290
WALK_ROLL_LEFT80,// 20.0 51.0 -4.65 300
WALK_ROLL_RIGHT20,// 32.5 -19.75 10.95 260
WALK_ROLL_RIGHT40,// 21.75 -42.0 6.6 270
WALK_ROLL_RIGHT80,// 32.25 -39.0 7.65 300
WALK_ROTATE_LEFT90,// -1.75 4.25 10.35 360
WALK_ROTATE_LEFT180,// -4.75 -13.5 20.2 400
WALK_ROTATE_RIGHT90,// -1.75 -5.5 -10.2 290
WALK_ROTATE_RIGHT180,// -8.85 5.1 -20.55 370
*/
/*
WALK_LEFT_15,// 111.1 56.7 -1.83 300
WALK_LEFT_30,// 104.7 64 -0.4 300
WALK_LEFT_60,// 63.8 65.3 -1 300
WALK_LEFT_60_rotate,// 82.3 53.8 6.7 300
WALK_LEFTBACKWARD_20,// -87.9 42.5 3.6 300
WALK_LEFTBACKWARD_30,// -94.4 49.5 -0.67 270
WALK_LEFTBACKWARD_60,// -50.1 79.7 -0.33 270
WALK_RIGHT_15,// 123.6 -6.8 -0.33 300
WALK_RIGHT_30,// 109.5 -63.4 0.17 300
WALK_RIGHT_30_ROTATE,// 77.3 -94.9 3.3 300
WALK_RIGHT_60,// 45.9 -81.6 -0.5 300
WALK_RIGHTBACKWARD_30,// -92.2 -44.4 -0.33 300
WALK_RIGHTBACKWARD_45_ROTATE,// -87.4 -42.7 5.17 300
WALK_RIGHTBACKWARD_60,// -62.5 -74.7 -0.67 270
WALK_ROLL_LEFT_R20,// 76.5 45.9 17 280
WALK_ROLL_LEFT_R40,// 100.1 32.3 12.2 280
WALK_ROLL_LEFT_R80,// 110.4 20.1 7.97 280
WALK_ROLL_RIGHT_R20,// 36 -47 -19.07 280
WALK_ROLL_RIGHT_R30,// 52.4 -58.5 -18.27 300
WALK_ROLL_RIGHT_R40,// 98 -52.6 -10.53 280
WALK_ROLL_RIGHT_R80,// 108.9 -24.8 -5.93 280
WALK_LEFT_SLIDE,// 88.8 34.2 -8.73 300
WALK_RIGHT_SLIDE,// 74.5 -69.8 10 300
WALK_STABLE_FORWARD,// 132.8 27.2 -0.5 300
WALK_STABLE_FORWARD2,// 122.9 28.5 -0.8 300
WALK_STABLE_FORWARD3,// 117.7 16.2 -0.67 280
WALK_STABLE_BACKWARD,// -100.5 -3.3 0.43 280
WALK_STABLE_BACKWARD2,// -115.2 3.9 2.6 300
WALK_STABLE_SIDELEFT,// -1.4 78.8 2.33 280
WALK_STABLE_SIDERIGHT,// -12.3 -60.9 1.17 270
WALK_STABLE_ROTATELEFT,// 3.4 26.7 37.5 300
WALK_STABLE_ROTATERIGHT,// 1.7 18.2 -38.8 300
WALK_HALFROTATE_LEFT,// 8.3 0.8 17.93 300
WALK_HALFROTATE_RIGHT,// -1.7 -5.4 -20.27 300
WALK_FOURTHROTATE_LEFT,// 6.1 2.5 9.77 200
WALK_FOURTHROTATE_RIGHT,// 2.8 -5.4 -10.17 200
WALK_ROTATE_LEFT,// 18.7 47.7 -10 300
WALK_ROTATE_RIGHT,// 3.5 -45.2 10.3 300
*/

};

const int used_action_num = sizeof(conv)/sizeof(conv[0]);

unsigned char* DPMap::m_map;
bool DPMap::loadflag = false;

valuetype *DPMapLocalApproach::value_map;
actiontype *DPMapLocalApproach::policy_map;
bool DPMapLocalApproach::loadflag = false;

valuetype *DPMapGoalieApproach::value_map;
actiontype *DPMapGoalieApproach::policy_map;
bool DPMapGoalieApproach::loadflag = false;


DPMap::DPMap(){
	if(!loadflag){
		int i;
		for(i=0;i<=m2p_x_elem;i++)		m2p_x_under[i] = m2p_x_wid*i -6000;
		for(i=0;i<=m2p_y_elem;i++)		m2p_y_under[i] = m2p_y_wid*i -6000;
		for(i=0;i<=m2p_theta_elem;i++)	m2p_theta_under[i] = m2p_t_wid*i -180;

		m_map = new unsigned char [m2p_full_elem];
		ifstream ifs("./MS/move2pos.pol",ios::binary);
		ifs.read((char*)m_map,m2p_full_elem*sizeof(unsigned char));
		ifs.close();

		loadflag = true;
	}
}


int DPMapLocalApproach::State2ID(int dist, int btt, int rtt){
	//dist
	int d = (dist-100)/25;//50;
	if(d < 0){
		return -1;
	}
	if( d >= DIST_NUM){
		d = DIST_NUM-1;
	}

	//btt;
	int bt = btt;
	while(bt < -180){bt += 360;}
	while(bt >= 180){bt -= 360;}
	bt = (bt + 180)/10;
	if(bt < 0 || bt >= BTT_NUM)
		return -1;

	//rtt;
	int rt = rtt;
	while(rt < -180){rt += 360;}
	while(rt >= 180){rt -= 360;}
	rt = (rt + 180)/15;

	return d + bt*DIST_NUM + rt*DIST_NUM*BTT_NUM;
}

unsigned char DPMapLocalApproach::get_action(int x, int y, int theta){
	int dist = (int)sqrt((double)(x*x+y*y));
	int btt = (int)(atan2((double)y,(double)x)*180/PI);
	int rtt = theta;
	int id = State2ID(dist,btt,rtt);

	if(id < 0 || id > MAPSIZE)
		return 0;

	unsigned char act = policy_map[id];

	return loconv[act];
}

int DPMapLocalApproach::get_cost(int x, int y, int theta){
	int dist = (int)sqrt((double)(x*x+y*y));
	int btt = (int)(atan2((double)y,(double)x)*180/PI);
	int rtt = theta;
	int id = State2ID(dist,btt,rtt);

	if(id < 0 || id > MAPSIZE)
		return -1;

	return value_map[id];
}



DPMapLocalApproach::DPMapLocalApproach(){
	if(!loadflag){
//		InitMins();

		policy_map = new actiontype[MAPSIZE];
		ifstream ifs("./MS/SAM.POL",ios::binary);
		ifs.read((char*)policy_map,MAPSIZE*sizeof(actiontype));
		ifs.close();

		value_map = new valuetype[MAPSIZE];
		ifstream ifs2("./MS/STATEVAL.SV",ios::binary);
		ifs2.read((char*)value_map,MAPSIZE*sizeof(valuetype));
		ifs2.close();

		loadflag = true;
	}
}

int DPMapGoalieApproach::State2ID(int dist, int btt, int rtt){
	//dist
	int d = (dist-100)/25;//50;
	if(d < 0){
		return -1;
	}
	if( d >= DIST_NUM){
		d = DIST_NUM-1;
	}

	//btt;
	int bt = btt;
	while(bt < -180){bt += 360;}
	while(bt >= 180){bt -= 360;}
	bt = (bt + 180)/10;
	if(bt < 0 || bt >= BTT_NUM2)
		return -1;

	//rtt;
	int rt = rtt;
	while(rt < -180){rt += 360;}
	while(rt >= 180){rt -= 360;}
	rt = (rt + 180)/15;

	return d + bt*DIST_NUM + rt*DIST_NUM*BTT_NUM2;
}

unsigned char DPMapGoalieApproach::get_action(int x, int y, int theta){
	int dist = (int)sqrt((double)(x*x+y*y));
	int btt = (int)(atan2((double)y,(double)x)*180/PI);
	int rtt = theta;
	int id = State2ID(dist,btt,rtt);

	if(id < 0 || id > MAPSIZE2)
		return 0;

	unsigned char act = policy_map[id];

	return gconv[act];
}

int DPMapGoalieApproach::get_cost(int x, int y, int theta){
	int dist = (int)sqrt((double)(x*x+y*y));
	int btt = (int)(atan2((double)y,(double)x)*180/PI);
	int rtt = theta;
	int id = State2ID(dist,btt,rtt);

	if(id < 0 || id > MAPSIZE2)
		return -1;

	return value_map[id];
}



DPMapGoalieApproach::DPMapGoalieApproach(){
	if(!loadflag){
//		InitMins();

		policy_map = new actiontype[MAPSIZE2];
		ifstream ifs("./MS/GSAM.POL",ios::binary);
		ifs.read((char*)policy_map,MAPSIZE2*sizeof(actiontype));
		ifs.close();

		value_map = new valuetype[MAPSIZE2];
		ifstream ifs2("./MS/GSTTVAL.SV",ios::binary);
		ifs2.read((char*)value_map,MAPSIZE2*sizeof(valuetype));
		ifs2.close();

		loadflag = true;
	}
}

unsigned char DPMap::get_action(int x, int y, int theta)
{
	int x_id = x_to_id(x);
	int y_id = y_to_id(y);
	int theta_id = theta_to_id(theta);
	int index = x_id*m2p_y_elem*m2p_theta_elem +
					   y_id*m2p_theta_elem +
									+ theta_id;
	return m_map[index];
}

int DPMap::x_to_id(int x){
	if(x < -6000)	return -1;
	else	return (int)((x + 6000)/m2p_x_wid);
}

int DPMap::y_to_id(int y){ 
	if(y < -6000)	return -1;
	else	return (int)((y + 6000)/m2p_y_wid);
}

int DPMap::theta_to_id(int theta){
	if(theta > 180)		theta -= 360;
	if(theta < -180)	theta += 360;
	return (int)((theta + 180)/m2p_t_wid);
}

unsigned char DPMap::GetActionToGoToXYTheta(int x, int y, int theta){
	int dx = (int)(-x*cos(-theta*PI/180) + y*sin(-theta*PI/180));
	int dy = (int)(-x*sin(-theta*PI/180) - y*cos(-theta*PI/180));
	int dt = -theta;
	get_action(dx,dy,dt);
	return 0;
}

static DPMap dpmap;

BhvMoveToPosDP::BhvMoveToPosDP(){
	dest_x = 0;
	dest_y = 0;
	dest_tt = 0;

//	x_width = X_WIDTH;
//	y_width = Y_WIDTH;
//	tt_width = TT_WIDTH;

		//状態変数の離散化
/*	int i;
	for(i=0;i<=x_elem;i++)		x_under[i] = x_wid*i -6000;
	for(i=0;i<=y_elem;i++)		y_under[i] = y_wid*i -6000;
	for(i=0;i<=theta_elem;i++)	theta_under[i] = 20*i -180;

	m_map = new unsigned char [full_elem];
	ifstream ifs("./MS/move2pos.pol",ios::binary);
	ifs.read((char*)m_map,full_elem*sizeof(unsigned char));
	ifs.close();
*/
}

void BhvMoveToPosDP::Execute(){
	//前回の指令値
	static double last_dest_x = 0;
	static double last_dest_y = 0;
	static double last_dest_tt = 0;

	int walk = 0;
	static bool final_state = false;

	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	int pos_tt = (int)state.self.position.tt.val;

	//SetHeadMode(HEAD_SWING_LOCALIZATION);
	static int count_fs = 0;
	static unsigned int cnt_fs = 0;

	if(dest_x != last_dest_x || dest_y != last_dest_y || dest_tt != last_dest_tt)
	{
		count_fs = 0;
		final_state = false;
	}
	
	//終端状態になった場合．ある程度時間が経過したら，抜ける．
	if(final_state)
	{
		if(cnt_fs > 30)
		{
			final_state = false;
			cnt_fs = 0;
		}
		else
		{
			SetPresetWalk(0);
		}
	}
	/*if(final_state)
	{
		SetPresetWalk(0);
		if(count_fs == 150)
		{
			final_state = false;
			count_fs = 0;
		}
		count_fs++;
		return;
	}*/

	last_dest_x = dest_x;
	last_dest_y = dest_y;
	last_dest_tt = dest_tt;



	GetPositionDelta(pos_x, pos_y, pos_tt);

	//NOTICE!!
	//Modified by Jiji on 2005/07/03
	//get_actionは
	unsigned char policy = DPMap::get_action(pos_x, pos_y, pos_tt);	

	if(policy == 0 || 
		(abs(pos_x - dest_x) < 100 &&
		 abs(pos_y - dest_y) < 100 &&
		 abs(pos_tt - dest_tt) < 10)
		 )
	{
		cnt_fs = 0;
		final_state = true;
		/*int pan = (int)state.self.sensor.jointValue[PAN];//state.ball.direction;//::ball.position.tt.val;

		if(pan < -50)			walk = WALK_HALFROTATE_RIGHT;//WALK_HALFROTATE_RIGHT;//
		else if(pan < -35)		walk = WALK_ROLL_RIGHT_R20;//WALK_HALFROTATE_RIGHT;//
		else if(pan < -20)		walk = WALK_ROLL_RIGHT_R40;
		else if(pan < 20)		walk = WALK_STABLE_FORWARD;
		else if(pan < 35)		walk = WALK_ROLL_LEFT_R40;
		else if(pan < 50)		walk = WALK_ROLL_LEFT_R20;//WALK_HALFROTATE_LEFT;//
		else					walk = WALK_HALFROTATE_LEFT;//WALK_HALFROTATE_LEFT;//
		*/
		SetPresetWalk(0);
		return;
	}
	else
	{
		cnt_fs++;
	}

	if(policy >= used_action_num){
		int pan = (int)state.self.sensor.jointValue[PAN];//state.ball.direction;//::ball.position.tt.val;

		if(pan < -50)			walk = WALK_ROTATE_RIGHT180;//WALK_HALFROTATE_RIGHT;//WALK_HALFROTATE_RIGHT;//
		else if(pan < -35)		walk = WALK_ROLL_RIGHT20;//WALK_HALFROTATE_RIGHT;//
		else if(pan < -20)		walk = WALK_ROLL_RIGHT40;
		else if(pan < 20)		walk = WALK_FORWARD;
		else if(pan < 35)		walk = WALK_ROLL_LEFT40;
		else if(pan < 50)		walk = WALK_ROLL_LEFT20;//WALK_HALFROTATE_LEFT;//
		else					walk = WALK_ROTATE_LEFT180;//WALK_HALFROTATE_LEFT;//WALK_HALFROTATE_LEFT;//
		
		SetPresetWalk(walk);
		return;
	}

	walk = conv[policy];
	if(GaitMaker::IsExpiring())	SetPresetWalk(walk);
	SetTailMode(TAIL_SWING);	

	return;
}

//目的地の指定
void BhvMoveToPosDP::SetDestination(double x, double y, double tt, double x_w, double y_w, double tt_w){
	dest_x = x;
	dest_y = y;
	dest_tt = tt;
	if(dest_tt > 180)	dest_tt -= 360;
	if(dest_tt < -180)	dest_tt += 360;
}

void BhvMoveToPosDP::GetPositionDelta(int &x, int &y, int &theta){
	int del_x = x - (int)dest_x;
	int del_y = y - (int)dest_y;
	int del_theta = (int)(theta - dest_tt);

	if(del_theta > 180)		del_theta -= 360;
	if(del_theta < -180)	del_theta += 360;
	theta = del_theta;

	x = (int)(cos(dest_tt*3.14/180)*del_x + sin(dest_tt*3.14/180)*del_y);
	y = (int)(-sin(dest_tt*3.14/180)*del_x + cos(dest_tt*3.14/180)*del_y);
}
