#include "StdAfx.h"
#include "PresetMotion.h"

bool PresetMotion::isInitialized = false;
std::vector<MotionParam> PresetMotion::motions;
//MyVector<MotionParam> PresetMotion::motions;
std::map<std::string,int> PresetMotion::motionNameMap;

int PresetMotion::ResolveMotionName(const char* motionname){
	std::string motionstr = motionname;
	if(motionNameMap.find(motionstr) != motionNameMap.end()){
		//モーション名が登録済みのとき
		int motionIndex = motionNameMap[motionstr];
		if(motionIndex < motions.size()){
			return motionIndex;
		}
	}
	return -1;
}

//モーション名登録
void PresetMotion::RegisterMotionName(const char* motionname){
	static int index = 0;
	motionNameMap[motionname] = index;
	index++;//←わかりやすさ優先
}


PresetMotion::PresetMotion(){
}

void
PresetMotion::Init()
{
	if(!isInitialized){
		MotionParam p;

		//GETUP
		RegisterMotionName("GETUP");
		p.pose.push_back( MotionPose( 2000,jointangles(
			90, 90,90, 90, 90,90,  90, 90,90, 90, 90,90,	20,  30,  0, -60, 0, 0)));
		p.pose.push_back( MotionPose( 1000,jointangles(
			90, 0,110, 90, 0,110,  90, 0,110, 90, 0,110, 20,  -30,  0, 0, -60, 0)));
		p.pose.push_back( MotionPose( 1000,jointangles(
			-10, 0,110, -10, 0,110,-10, 0,110,-10, 0,110, 20,  30,  0, 0, -60, 0)));
		p.pose.push_back( MotionPose( 1000,jointangles(
			-5,13,106,   -5,13,106,  -46, 0, 92,  -46, 0,92,	-30,-40,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();


		//GETUP_FRONT
		RegisterMotionName("GETUP_FRONT");
		p.pose.push_back( MotionPose( 1000,jointangles(
			85, 80,-11, 85, 80,-11,  -82, 10, 60,   -82, 10, 60,	20,  0,  0, 0, 0, 0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			85, 60,  1,    85, 60,  1,  -80, 10, 67,   -80, 10, 67,	20,  0,  0, 0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			92, 26, 31,    92, 26, 31,  -77, 10, 89,   -77, 10, 89,	-8,  0,  0, 0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			71, 31, 56,    71, 31, 56,  -70, 13, 97,   -70, 13, 97,	-14,  0,  0, 0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-36, 33,125,   -36, 33,125,  -48, 32,125,   -48, 32,125,	-14,  0,  0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-5,13,106,   -5,13,106,  -46, 0, 92,  -46, 0,92,	-30,-40,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GETUP_RIGHT
		RegisterMotionName("GETUP_RIGHT");
		p.pose.push_back( MotionPose( 1000,jointangles(
			20,0,130,-27,0, 45,20,0,130,-27,0,45,10,-70,-20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			 115, 0, 10,  -57, 0,145,   115, 0,130,  -57, 0,145,	 20,-70,-20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			 115,85,130,  -50, 0,120,   115,85,130,  -50, 0,120,	-45,  0,  0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-110,85,130,  -70,25,138,  -110,85,130,  -70,25,138,	-30,  0,  0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			  -5,13,106,   -5,13,106,  -46, 0, 92,  -46, 0,92,	-30,-40,0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			  -5,13,106,   -5,13,106,  -46, 0, 92,  -46, 0,92,	-30,-40,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GETUP_LEFT
		RegisterMotionName("GETUP_LEFT");
		p.pose.push_back( MotionPose( 1000,jointangles(
			-27, 0, 45,    20, 0,130,  -27, 0, 45,    20, 0,130,	10,70,20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-57, 0,145,   115, 0, 10,  -57, 0,145,   115, 0,130,	20,70,20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-50, 0,120,   115,85,130,  -50, 0,120,   115,85,130,	-45, 0, 0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-70,25,138,  -110,85,130,  -70,25,138,  -110,85,130,	-30, 0, 0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			  -5,13,106,   -5,13,106,  -46, 0, 92,  -46, 0,92,	-30,-40,0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			  -5,13,106,   -5,13,106,  -46, 0, 92,  -46, 0,92,	-30,-40,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GETUP_BACK
		RegisterMotionName("GETUP_BACK");
		p.pose.push_back( MotionPose( 1000,jointangles(
			-58, 13, 90,  -58, 13, 90,    2, 27, -5,     2, 27, -5,	-70,  0,  0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-60,  7, 30,  -60,  7, 30,   82, 80, -5,    82, 80, -5,	-70,  0,  0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-60, -5, 64,  -60, -5, 64,   58, 40, 81,    58, 40, 81,	-52,  0,  0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-60,  4,125,  -60,  4,125,   21,  6,125,    21,  6,125,	-23,  0,  0, 0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-50,  5,105,  -50,  5,105,  -60,  5,105,   -60,  5,105,	-18,  0,  0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			  -5,13,106,   -5,13,106,  -46, 0, 92,  -46, 0,92,	-30,-40,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//HG
		RegisterMotionName("GET_GOAL_ME");
		p.pose.push_back( MotionPose( 500,jointangles(
		 90,0,105,90,0,105,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,0,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//HG_BACK
		RegisterMotionName("GET_GOAL_ME_HALF");
		p.pose.push_back( MotionPose( 500,jointangles(
		 -10,0,40,0,0,0,-120,0,110,0,0,30,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
		 0,0,40,0,0,0,0,0,110,0,0,30,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
		 0,90,0,0,90,0,0,90,0,0,90,0,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,90,0,110,90,0,110,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		 135,30,30,135,30,30,80,0,110,80,0,110,-80,0,30,-50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GUTSPOSE
		RegisterMotionName("GET_GOAL_ME");
		p.pose.push_back( MotionPose( 40,jointangles(
		-15,27,128,-15,27,128,-70,0,100,-70,0,100,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 64,jointangles(
		-15,27,128,-15,27,128,-70,0,100,-70,0,100,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 120,jointangles(
		-25,30,90,-25,30,90,0,0,20,0,0,20,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 250,jointangles(
		-25,30,90,-25,30,90,0,0,20,0,0,20,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
		20,50,70,20,50,70,90,0,20,90,0,20,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
		50,25,45,50,25,45,90,90,20,90,90,20,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
		80,-5,20,80,-5,20,-90,90,20,-90,90,20,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
		60,-10,35,60,-10,35,-90,30,0,-90,30,0,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
		10,-8,35,10,-8,35,-50,0,0,-50,0,0,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
		0,-8,35,0,-8,35,-50,0,50,-50,0,50,-80,0,0,0,0,0)));

		p.pose.push_back( MotionPose( 500,jointangles(
		-70,-8,120,0,-8,35,-50,0,100,-50,0,100,-50,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 600,jointangles(
		-70,-8,120,60,30,0,-50,0,100,-50,0,100,-50,0,0,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));

		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,30,50,-50,0,100,-50,0,100,-50,15,10,-30,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
		-70,-8,120,60,-8,50,-50,0,100,-50,0,100,-50,15,10,0,0,0)));



		/*
		p.pose.push_back( MotionPose( 300,jointangles(
		30,-8,0,-70,-8,120,-50,0,100,-50,0,100,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 600,jointangles(
		30,-8,0,-70,-8,120,-50,0,100,-50,0,100,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
		-70,-8,120,30,-8,0,-50,0,100,-50,0,100,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 600,jointangles(
		-70,-8,120,30,-8,0,-50,0,100,-50,0,100,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
		30,-8,0,-70,-8,120,-50,0,100,-50,0,100,-80,0,0,0,0,0)));
*/
//		p.pose.push_back( MotionPose( 600,jointangles(
//		30,-8,0,-70,-8,120,-50,0,100,-50,0,100,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 1000,jointangles(
		0,-8,120,0,-8,120,-50,0,100,-50,0,100,-80,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			30, 10, 0, 30,10, 0, -90,0, 120,-90,0,120,-15,30,0,0,0,0)));

		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GET_GOAL_ENEMY
		RegisterMotionName("GET_GOAL_ENEMY");
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,0,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,0,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,0,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,30,-15,30,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//FINISH_WIN
		RegisterMotionName("FINISH_WIN");
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,0,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,0,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,0,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			30, 10, 0, 30,10, 0, -90,0, 120,-90,0,120,-15,30,0,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//FINISH_DRAW
		RegisterMotionName("FINISH_DRAW");
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,0,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,0,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,0,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,30,-15,30,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//FINISH_LOSE
		RegisterMotionName("FINISH_LOSE");
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,0,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,0,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,0,10,100,-40,30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			130,10,60,100,10,0,0,10,100,-30,10,100,-40,-30,-15,30,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,10,0,100,10,0,-30,10,100,-30,10,100,-40,30,-15,30,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//LONGSHOOT_FRONT_100_150
		RegisterMotionName("LONGSHOOT_FRONT_100_150");
		p.pose.push_back( MotionPose( 100,jointangles(
			40,	0,	37,	40,	0,	37,	-40,	0,	127,	-40,	0,	127,	0,	0,	-20,	0,	0,	0)));
		p.pose.push_back( MotionPose( 70,jointangles(
			50,	0,	30,	50,	0,	30,	-30,	0,	127,	-30,	0,	127,	0,	0,	-20,	0,	0,	0)));
		p.pose.push_back( MotionPose( 70,jointangles(
			60,	0,	30,	60,	0,	30,	 10,	0,	127,	 10,	0,	127,	-90,	0,	-20,	0,	0,	0)));
		p.pose.push_back( MotionPose( 70,jointangles(
			60,	0,	30,	60,	0,	30,	 30,	0,	127,	 30,	0,	127,	-90,	0,	-20,	0,	0,	0)));
		p.pose.push_back( MotionPose( 800,jointangles(
			70,	0,	30,	70,	0,	30,	 30,	0,	127,	 30,	0,	127,	-90,	0,	-20,	0,	0,	0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FRONT
		RegisterMotionName("GRABSHOOT_FRONT");
		p.pose.push_back( MotionPose( 100,jointangles(
			90,-15,0,90,-15,0,-40,0,120,-40,0,120,-30,0,100,-20,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			90,-15,50,90,-15,50,-40,0,120,-40,0,120,-30,0,90,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			0,-15,60,0,-15,60,0,0,0,0,0,0,-30,0,90,-50,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
			0,-10,60,0,-10,60,0,0,0,0,0,0,-70,0,90,0,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			0,0,0,0,0,0,0,0,0,0,0,0,-70,0,-60,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			0,0,0,0,0,0,30,50,100,30,50,100,-70,0,-60,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			0,0,0,0,0,0,30,50,100,30,50,100,-50,0,-60,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			0,60,100,0,60,100,30,50,100,30,50,100,-50,0,-60,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			0,60,100,0,60,100,30,50,100,30,50,100,-50,0,-60,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();
		
		//GRABSHOOT_LEFT
		RegisterMotionName("GRABSHOOT_LEFT");
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,90,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,90,0,-20,-15,120,-40,35,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,0,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			100,40,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();
		
		//GRABSHOOT_RIGHT
		RegisterMotionName("GRABSHOOT_RIGHT");
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,90,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,90,0,100,-10,0,-40,30,120,-20,-15,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,90,0,100,0,0,-40,30,120,-20,-15,120,-30,-80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			100,90,0,100,40,0,-40,30,120,-20,-15,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,90,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			100,20,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();


		//GRABSHOOT_FAKE_LEFT
		RegisterMotionName("GRABSHOOT_FAKE_LEFT");
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,90,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,90,0,-20,-15,120,-40,35,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,0,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100/*120*/,jointangles(
			100,40,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_RIGHT
		RegisterMotionName("GRABSHOOT_FAKE_RIGHT");
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,90,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,90,0,100,-10,0,-40,30,120,-20,-15,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,90,0,100,0,0,-40,30,120,-20,-15,120,-30,-80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 100/*130*/,jointangles(
			100,90,0,100,40,0,-40,30,120,-20,-15,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_LEFT20
		RegisterMotionName("GRABSHOOT_FAKE_LEFT20");
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,90,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,90,0,-20,-15,120,-40,35,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,0,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,40,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		-37,0,110,-37,0,110,-27,0,110,-27,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_RIGHT20
		RegisterMotionName("GRABSHOOT_FAKE_RIGHT20");
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,90,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,90,0,100,-10,0,-40,30,120,-20,-15,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,90,0,100,0,0,-40,30,120,-20,-15,120,-30,-80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 135,jointangles(
		100,90,0,100,40,0,-40,30,120,-20,-15,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		-37,0,110,-37,0,110,-27,0,110,-27,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_LEFT30
		RegisterMotionName("GRABSHOOT_FAKE_LEFT30");
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,90,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,90,0,-20,-15,120,-40,35,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,0,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 180,jointangles(
		100,40,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		-37,0,110,-37,0,110,-27,0,110,-27,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_RIGHT30
		RegisterMotionName("GRABSHOOT_FAKE_RIGHT30");
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,90,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,90,0,100,-10,0,-40,30,120,-20,-15,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,90,0,100,0,0,-40,30,120,-20,-15,120,-30,-80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,90,0,100,40,0,-40,30,120,-20,-15,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		-37,0,110,-37,0,110,-27,0,110,-27,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_LEFT40
		RegisterMotionName("GRABSHOOT_FAKE_LEFT40");
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,90,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,90,0,-20,-15,120,-40,35,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,0,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 190,jointangles(
		100,40,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		-37,0,110,-37,0,110,-27,0,110,-27,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_RIGHT40
		RegisterMotionName("GRABSHOOT_FAKE_RIGHT40");
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,90,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,90,0,100,-10,0,-40,30,120,-20,-15,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,90,0,100,0,0,-40,30,120,-20,-15,120,-30,-80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 180,jointangles(
		100,90,0,100,40,0,-40,30,120,-20,-15,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		-37,0,110,-37,0,110,-27,0,110,-27,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_LEFT60
		RegisterMotionName("GRABSHOOT_FAKE_LEFT60");
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,90,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,90,0,-20,-15,120,-40,35,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,0,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 220,jointangles(
		100,40,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		-37,0,110,-37,0,110,-27,0,110,-27,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GRABSHOOT_FAKE_RIGHT60
		RegisterMotionName("GRABSHOOT_FAKE_RIGHT60");
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,90,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		100,90,0,100,-10,0,-40,30,120,-20,-15,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,90,0,100,0,0,-40,30,120,-20,-15,120,-30,-80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 215,jointangles(
		100,90,0,100,40,0,-40,30,120,-20,-15,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
		100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
		-37,0,110,-37,0,110,-27,0,110,-27,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();


		//SHOOT_FRONT
		RegisterMotionName("SHOOT_FRONT");
		p.pose.push_back( MotionPose( 100,jointangles(
		0,30,0,0,30,0,-30,30,127,-30,30,127,-10,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		0,30,0,0,30,0,-100,30,127,-100,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
		0,5,0,0,5,0,0,30,127,0,30,127,-10,0,-20,-20,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
		80,0,0,80,0,0,0,30,127,0,30,127,-70,0,-10,-30,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();


		//HEADING_LEFT
		RegisterMotionName("HEADING_LEFT");
		p.pose.push_back( MotionPose( 180,jointangles(
			0,0,127,-65,30,127,-70,30,70,0,0,70,-40,-90,50,-50,0,0)));
		p.pose.push_back( MotionPose( 120,jointangles(
			0,0,127,-65,30,127,-70,30,70,0,0,70,-80,-90,50,-50,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-33,25,127,-43,15,127,-35,15,70,-35,15,70,-80,0,50,-50,0,0)));
		p.pose.push_back( MotionPose( 90,jointangles(
			-55,30,127,-20,0,127,0,0,70,-70,30,70,-80,70,50,-50,0,0)));
		p.pose.push_back( MotionPose( 90,jointangles(
			-55,30,127,-20,0,127,0,0,70,-70,30,70,-80,70,50,-50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		/*p.pose.push_back( MotionPose( 180,jointangles(
			0,0,127,-45,30,127,-70,30,60,0,0,60,-80,-40,20,0,0,0)));
		p.pose.push_back( MotionPose( 180,jointangles(
			-45,30,127,0,0,127,0,0,60,-70,30,60,-80,70,20,0,0,0)));
		p.pose.push_back( MotionPose( 90,jointangles(
			-45,30,127,0,0,127,0,0,60,-70,30,60,-80,70,20,0,0,0)));*/
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();


		//HEADING_RIGHT
		RegisterMotionName("HEADING_RIGHT");
		p.pose.push_back( MotionPose( 180,jointangles(
			-65,30,127,0,0,127,0,0,70,-70,30,70,-40,90,50,-50,0,0)));
		p.pose.push_back( MotionPose( 120,jointangles(
			-65,30,127,0,0,127,0,0,70,-70,30,70,-80,90,50,-50,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-43,15,127,-33,25,127,-35,15,70,-35,15,70,-80,0,50,-50,0,0)));
		p.pose.push_back( MotionPose( 90,jointangles(
			-20,0,127,-55,30,127,-70,30,70,0,0,70,-80,-70,50,-50,0,0)));
		p.pose.push_back( MotionPose( 90,jointangles(
			-20,0,127,-55,30,127,-70,30,70,0,0,70,-80,-70,50,-50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		/*
		p.pose.push_back( MotionPose( 180,jointangles(
            -45,30,127,0,0,127,0,0,60,-70,30,60,-80,40,20,0,0,0)));
		p.pose.push_back( MotionPose( 180,jointangles(
            0,0,127,-45,30,127,-70,30,60,0,0,60,-80,-70,20,0,0,0)));
		p.pose.push_back( MotionPose( 90,jointangles(
            0,0,127,-45,30,127,-70,30,60,0,0,60,-80,-70,20,0,0,0)));*/
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//HEADING_2LEFT	2004年のヘディング	掴まない
		RegisterMotionName("HEADING_2LEFT");
		p.pose.push_back( MotionPose( 250,jointangles(
			-26,27,123,-16,36,88,-69,23,129,-65,23,129,-40,-40,20,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			-15,30,123,-28,62,90,-60,19,129,-40,40,107,-60,-60,20,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			-31,18,123,-34,64,40,-47,-1,131,-7,30,43,-60,-90,30,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-46,13,123,-44,60,40,-47,-1,131,-7,30,43,-50,-90,40,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-37,14,121,-44,73,80,-48,4,101,-17,21,43,-50,-30,50,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-47,47,120,-43,72,110,-41,4,88,-30,22,57,-60,0,50,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-35,58,108,-18,62,122,-29,4,50,-39,6,70,-80,30,40,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			-33,60,55,-16,28,135,-15,23,39,-57,6,91,-80,90,30,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			-33,60,55,8,29,125,-20,12,49,-57,6,91,-80,60,20,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();


		//HEADING_2RIGHT	2004年のヘディング	掴まない
		RegisterMotionName("HEADING_2RIGHT");
		p.pose.push_back( MotionPose( 250,jointangles(
            -16,36,88,-26,27,123,-65,23,129,-69,23,129,-40,40,20,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			-28,62,90,-15,30,123,-40,40,107,-60,19,129,-60,60,20,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			-34,64,40,-31,18,123,-7,30,43,-47,-1,131,-60,90,30,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-44,60,40,-46,13,123,-7,30,43,-47,-1,131,-50,90,40,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-44,73,80,-37,14,121,-17,21,43,-48,4,101,-50,30,50,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-43,72,110,-47,47,120,-30,22,57,-41,4,88,-60,0,50,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			-18,62,122,-35,58,108,-39,6,70,-29,4,50,-80,-30,40,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			-16,28,135,-33,60,55,-57,6,91,-15,23,39,-80,-90,30,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			8,29,125,-33,60,55,-57,6,91,-20,12,49,-80,-60,20,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		
		//SMASHHIT_LEFT 2004年	掴まない
		RegisterMotionName("SMASHHIT_LEFT");
		p.pose.push_back( MotionPose( 250,jointangles(
			-15,27,128,30,27,128,-110,0,127,-70,0,110,0,0,50,0,0,0)));
		p.pose.push_back( MotionPose( 250,jointangles(
			90,27,20,30,27,128,-110,0,127,-70,15,110,0,0,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			100,0,20,40,80,128,-110,0,127,-70,15,110,0,0,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			0,15,110,40,80,0,-110,0,120,-70,15,100,0,-90,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			0,5,110,40,-15,0,-70,0,120,40,0,40,0,-90,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			5,-5,110,80,-15,8,-65,0,120,40,0,60,0,-90,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			5,15,127,30,-15,98,-50,0,120,50,0,65,0,-90,50,0,0,0)));
		p.pose.push_back( MotionPose( 250,jointangles(
			-15,27,128,-15,27,128,-50,0,100,-50,0,100,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//SMASHHIT_RIGHT	2004年	掴まない
		RegisterMotionName("SMASHHIT_RIGHT");
		p.pose.push_back( MotionPose( 250,jointangles(
			30,27,128,-15,27,128,-70,0,110,-110,0,127,0,0,50,0,0,0)));
		p.pose.push_back( MotionPose( 250,jointangles(
			30,27,128,90,27,20,-70,15,110,-110,0,127,0,0,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			40,80,128,100,0,20,-70,15,110,-110,0,127,0,0,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			50,80,0,0,15,110,-70,15,100,-110,0,120,0,90,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			50,-15,0,0,5,110,40,0,40,-70,0,120,0,90,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			80,-15,28,5,-5,110,40,0,60,-65,0,120,0,90,50,0,0,0)));
		p.pose.push_back( MotionPose( 130,jointangles(
			30,-15,128,5,15,127,50,0,65,-50,0,120,0,90,50,0,0,0)));
		p.pose.push_back( MotionPose( 250,jointangles(
			-15,27,128,-15,27,127,-50,0,100,-50,0,100,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();


		//GOALIE_BLOCK
		RegisterMotionName("GOALIE_BLOCK");
		p.pose.push_back( MotionPose( 200,jointangles(
            -40,50,128,-40,50,128,40,0,100,40,0,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			-10,90,0,-10,90,0,40,30,100,40,30,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			90,90,0,90,90,0,40,30,100,40,30,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			90,0,0,90,0,0,40,0,100,40,0,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-15,0,128,-15,0,128,40,0,100,40,0,100,0,0,20,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GOALIE_BLOCK_RIGHT_WALK
		RegisterMotionName("GOALIE_BLOCK_RIGHTWALK");
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,-10,100,-24,-10,102,-42,20,101,-38,19,94,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-24,0,105,-23,1,102,-43,11,105,-44,12,108,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-24,16,103,-22,13,96,-42,1,105,-48,0,117,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,21,98,-20,20,90,-41,-4,102,-48,-5,116,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-22,21,95,-21,21,93,-43,-5,106,-46,-5,113,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,10,100,-24,11,103,-46,3,112,-44,4,109,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,-1,100,-25,-3,108,-46,15,111,-40,13,99,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-22,-9,97,-25,-10,105,-44,20,105,-36,19,89,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,-10,100,-24,-10,102,-42,20,101,-38,19,94,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-24,0,105,-23,1,102,-43,11,105,-44,12,108,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-24,16,103,-22,13,96,-42,1,105,-48,0,117,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,21,98,-20,20,90,-41,-4,102,-48,-5,116,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-22,21,95,-21,21,93,-43,-5,106,-46,-5,113,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,10,100,-24,11,103,-46,3,112,-44,4,109,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,-1,100,-25,-3,108,-46,15,111,-40,13,99,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-22,-9,97,-25,-10,105,-44,20,105,-36,19,89,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			-10,90,0,-10,90,0,40,30,100,40,30,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			90,90,0,90,90,0,40,30,100,40,30,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			90,0,0,90,0,0,40,0,100,40,0,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-15,0,128,-15,0,128,40,0,100,40,0,100,0,0,20,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//GOALIE_BLOCK_LEFT_WALK
		RegisterMotionName("GOALIE_BLOCK_LEFTWALK");
		p.pose.push_back( MotionPose( 35,jointangles(
			-24,-10,102,-23,-10,100,-38,19,94,-42,20,101,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,1,102,-24,0,105,-44,12,108,-43,11,105,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-22,13,96,-24,16,103,-48,0,117,-42,1,105,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-20,20,90,-23,21,98,-48,-5,116,-41,-4,102,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-21,21,93,-22,21,95,-46,-5,113,-43,-5,106,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-24,11,103,-23,10,100,-44,4,109,-46,3,112,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-25,-3,108,-23,-1,100,-40,13,99,-46,15,111,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-25,-10,105,-22,-9,97,-36,19,89,-44,20,105,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-24,-10,102,-23,-10,100,-38,19,94,-42,20,101,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-23,1,102,-24,0,105,-44,12,108,-43,11,105,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-22,13,96,-24,16,103,-48,0,117,-42,1,105,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-20,20,90,-23,21,98,-48,-5,116,-41,-4,102,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-21,21,93,-22,21,95,-46,-5,113,-43,-5,106,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-24,11,103,-23,10,100,-44,4,109,-46,3,112,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-25,-3,108,-23,-1,100,-40,13,99,-46,15,111,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 35,jointangles(
			-25,-10,105,-22,-9,97,-36,19,89,-44,20,105,0,0,0,0,0,0)));			
		p.pose.push_back( MotionPose( 150,jointangles(
			-10,90,0,-10,90,0,40,30,100,40,30,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			90,90,0,90,90,0,40,30,100,40,30,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			90,0,0,90,0,0,40,0,100,40,0,100,0,0,20,0,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
			-15,0,128,-15,0,128,40,0,100,40,0,100,0,0,20,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();
		
		//DIVING_LEFT45
		RegisterMotionName("DIVING_LEFT45");
		p.pose.push_back( MotionPose( 80,jointangles(
            -30,30,80,-30,30,80,-30,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
            0,30,127,-30,30,127,-100,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 70,jointangles(
            0,5,60,-30,30,60,0,30,127,-30,30,127,-50,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
            120,-15,60,120,30,60,0,30,127,0,30,127,-80,0,20,-50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
            120,-15,0,120,30,0,0,30,127,0,30,127,-80,0,20,-50,0,0)));
		/*
		p.pose.push_back( MotionPose( 30,jointangles(
            -55,0,84, -17,0,111, -58,0,111, 0,0,78, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -65,0,99, -12,0,96, -57,0,108, -1,0,83, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -54,0,125, -34,0,97, -37,0,110, -34,0,114, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -22,0,125, -43,0,68, 0,0,76, -59,0,114, 0,0,0,0,0,0)));//ここまDE
		p.pose.push_back( MotionPose( 30,jointangles(
            -17,0,111, -55,0,84, 0,0,78, -58,0,111, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -12,0, 96, -65,0,99,-1,0,83, -57,0,108,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -34,0, 97, -54,0,125,-34,0,114,-37,0,110,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -43,0, 68, -22,0,125,-59,0,114,0,0,76,0,0,0,0,0,0)));//ここまで歩行

		p.pose.push_back( MotionPose( 150,jointangles(
			0,0,0, 0,30,0, -30,30,127, -30,30,127, 0,20,-20,0,0,0)));
            //-30,30,80,-30,30,80,-30,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			0,0,0, 0,30,0, -70,30,127, -30,30,127, 0,20,-20,0,0,0)));
            //0,30,127,-30,30,127,-100,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			120,0,0, 0,30,0, 0,30,127, -30,30,127, 0,20,-20,0,0,0)));
            //0,5,60,-30,30,60,0,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			120,-15,0, 120,30,30, 0,-15,127, 0,30,127, -70,20,-20,0,0,0)));*/
            //120,-15,60,120,30,60,0,30,127,0,30,127,-80,0,20,-50,0,0)));
		//p.pose.push_back( MotionPose( 500,jointangles(
        //    120,-15,0,120,30,0,0,30,127,0,30,127,-80,0,20,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//DIVING_RIGHT45
		RegisterMotionName("DIVING_RIGHT45");
		p.pose.push_back( MotionPose( 80,jointangles(
            -30,30,80,-30,30,80,-30,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
            -30,30,127,0,30,127,-30,30,127,-100,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 70,jointangles(
            -30,30,60,0,5,60,-30,30,127,0,30,127,-50,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
            120,30,60,120,-15,60,0,30,127,0,30,127,-80,0,20,-50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
            120,30,0,120,-15,0,0,30,127,0,30,127,-80,0,20,-50,0,0)));
		/*
		p.pose.push_back( MotionPose( 30,jointangles(
            -55,0,84, -17,0,111, -58,0,111, 0,0,78, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -65,0,99, -12,0,96, -57,0,108, -1,0,83, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -54,0,125, -34,0,97, -37,0,110, -34,0,114, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -22,0,125, -43,0,68, 0,0,76, -59,0,114, 0,0,0,0,0,0)));//ここまでDE
		p.pose.push_back( MotionPose( 30,jointangles(
            -17,0,111, -55,0,84, 0,0,78, -58,0,111, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -12,0, 96, -65,0,99,-1,0,83, -57,0,108,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -34,0, 97, -54,0,125,-34,0,114,-37,0,110,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -43,0, 68, -22,0,125,-59,0,114,0,0,76,0,0,0,0,0,0)));//ここまで歩行

		p.pose.push_back( MotionPose( 150,jointangles(
			0,30,0, 0,0,0, -30,30,127, -30,30,127, 0,-20,-20,0,0,0)));
            //-30,30,80,-30,30,80,-30,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			0,30,0, 0,0,0, -30,30,127, -70,30,127, 0,-20,-20,0,0,0)));
            //0,30,127,-30,30,127,-100,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			0,30,0, 0,30,0, 0,30,127, -30,30,127, 0,-20,-20,0,0,0)));
            //0,5,60,-30,30,60,0,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			120,30,0, 120,-15,0, 0,30,127, 0,-15,127, -70,-20,-20,0,0,0)));*/

		/*
		p.pose.push_back( MotionPose( 100,jointangles(
            -30,30,80,-30,30,80,-30,30,127,-30,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
            -30,30,127,0,30,127,-30,30,127,-100,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
            -30,30,60,0,5,60,-30,30,127,0,30,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
            120,30,60,120,-15,60,0,30,127,0,30,127,-80,0,20,-50,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
            120,30,0,120,-15,0,0,30,127,0,30,127,-80,0,20,-50,0,0)));*/
		p.pose.push_back( MotionPose( 100,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//SHOT_8MM
		RegisterMotionName("SHOT_8MM");
		p.pose.push_back( MotionPose( 300,jointangles(
            15,-15,65,15,-15,65,-35,0,112,-35,0,112,-80,0,35,-55,0,0)));
		p.pose.push_back( MotionPose( 500,jointangles(
            15,-15,65,15,-15,65,-50,0,112,-50,0,112,-80,0,35,-55,0,0)));
		p.pose.push_back( MotionPose( 600,jointangles(
            10,-15,0,10,-15,0,-60,0,112,-60,0,112,0,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
            90,-8,0,90,-8,0,0,0,105,0,0,105,0,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
            90,-8,0,90,-8,0,-10,0,105,-10,0,105,-90,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
            60,-3,0,60,-3,0,-10,0,100,-10,0,100,-90,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();
		
		//HOLD_LEFT90
		RegisterMotionName("HOLD_LEFT90");
		p.pose.push_back( MotionPose( 500,jointangles(
            -6,19,94,-6,19,94,-35,0,112,-35,0,112,-60,0,45,-55,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			-6,19,94,-6,19,94,-35,0,112,-35,0,112,-60,0,45,-55,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
            100,-10,0,100,90,0,-40,0,120,-40,0,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
            100,-10,0,100,90,0,-20,-15,120,-40,35,120,-30,-50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,0,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 270,jointangles(
			100,40,0,100,90,0,-20,-15,120,-40,35,120,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			100,40,0,100,10,0,-40,35,120,-10,-15,60,-30,80,30,0,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();
		
		//HOLD_RIGHT90
		RegisterMotionName("HOLD_RIGHT90");
		p.pose.push_back( MotionPose( 500,jointangles(
            -6,19,94,-6,19,94,-35,0,112,-35,0,112,-60,0,45,-55,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			-6,19,94,-6,19,94,-35,0,112,-35,0,112,-60,0,45,-55,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,0,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,-10,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
            100,90,0,100,-10,0,-40,0,120,-40,0,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
            100,90,0,100,-10,0,-40,30,120,-20,-15,120,-30,50,60,-50,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			100,90,0,100,0,0,-40,30,120,-20,-15,120,-30,-80,30,-50,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			100,90,0,100,40,0,-40,30,120,-20,-15,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
			100,10,0,100,40,0,-10,-15,60,-40,35,120,-30,-80,30,0,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();
		
		
		//GRABLOOK
		RegisterMotionName("GRAB_LOOK");
		p.pose.push_back( MotionPose(200,jointangles(
            60,15,30,60,15,30,-35,0,112,-35,0,112,  -60, 0, 45,  0,0,0)));
		p.pose.push_back( MotionPose(200,jointangles(
            110,-5,0,110,-5,0,-35,0,112,-35,0,112,   0, 0,  0,  0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
            110,-5,0,110,-5,0,-35,0,112,-35,0,112,   0,- 0 ,30,  0,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
            110,-5,0,110,-5,0,-35,0,112,-35,0,112,  0,- 0 ,30,  0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
            110,-5,0,110,-5,0,-35,0,112,-35,0,112,  0,-85,30,    0,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
            110,-5,0,110,-5,0,-35,0,112,-35,0,112,  0,85,30,     0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
            110,-5,0,110,-5,0,-35,0,112,-35,0,112,  0, 0, 30,    0,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
            -6,19,94,-6,19,94,-35,0,112,-35,0,112,-60, 0, 45,  -55,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//AYAYA_RIGHT
		RegisterMotionName("AYAYA_RIGHT");
		p.pose.push_back( MotionPose( 100,jointangles(
			-120,0,110,90,50,0,90,55,0,-30,90,50,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			-120,0,110,90,50,0,90,55,0,-30,90,50,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
			-120,90,0,90,50,0,90,55,0,-30,90,50,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
			0,90,0,90,50,0,90,55,0,-30,90,50,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
			0,90,0,90,-10,0,90,93,0,-30,90,50,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 70,jointangles(
			0,90,60,90,-10,0,90,93,0,-30,90,50,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
			0,90,90,90,10,90,90,93,90,-30,90,90,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			-120,90,110,90,10,90,90,93,90,-30,90,90,0,0,14,-60,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//AYAYA_LEFT
		RegisterMotionName("AYAYA_LEFT");
		p.pose.push_back( MotionPose( 100,jointangles(
			90,50,0,-120,0,110,-30,90,50,90,55,0,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 300,jointangles(
			90,50,0,-120,0,110,-30,90,50,90,55,0,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
			90,50,0,-120,90,0,-30,90,50,90,55,0,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
			90,50,0,0,90,0,-30,90,50,90,55,0,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
			90,-10,0,0,90,0,-30,90,50,90,93,0,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 70,jointangles(
			90,-10,0,0,90,60,-30,90,50,90,93,0,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
			90,10,90,0,90,90,-30,90,90,90,93,90,0,0,14,-60,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			90,10,90,-120,90,110,-30,90,90,90,93,90,0,0,14,-60,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		/*
		//DIVING_LEFT45_OSAKA
		RegisterMotionName("DIVING_LEFT45_OSALKA");
		p.pose.push_back( MotionPose( 100,jointangles(
            -40, 45,100,-40,30,60,-70,30,100,-70,30,100,0,-80,0,  0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
             60, 45,50,-40,30,50,0,30,100,-70,30,100,-80,-80,0,  -50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
             60, -15,50,60,0,50,0,30,100,0,30,100,-80,0,0,  -50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//DIVING_RIGHT45_OSAKA
		RegisterMotionName("DIVING_LEFT45_OSALKA");
		p.pose.push_back( MotionPose( 100,jointangles(
            -40, 30,60,-40,45,100,-70,30,100,-70,30,100,0,80,0,  0,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
             -40, 30,50,60,45,50,-70,30,100,0,30,100,-80,80,0,  -50,0,0)));
		p.pose.push_back( MotionPose( 150,jointangles(
             60, 0,50,60,-15,50,0,30,100,0,30,100,-80,0,0,  -50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(//基本姿勢
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//BACK_SHOT
		RegisterMotionName("BACK_SHOT");

		p.pose.push_back( MotionPose( 30,jointangles(
            -55,0,84, -17,0,111, -58,0,111, 0,0,78, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -65,0,99, -12,0,96, -57,0,108, -1,0,83, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -54,0,125, -34,0,97, -37,0,110, -34,0,114, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -22,0,125, -43,0,68, 0,0,76, -59,0,114, 0,0,0,0,0,0)));//ここまでDE
		p.pose.push_back( MotionPose( 30,jointangles(
            -17,0,111, -55,0,84, 0,0,78, -58,0,111, 0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -12,0, 96, -65,0,99,-1,0,83, -57,0,108,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -34,0, 97, -54,0,125,-34,0,114,-37,0,110,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
            -43,0, 68, -22,0,125,-59,0,114,0,0,76,0,0,0,0,0,0)));//ここまで歩行

		p.pose.push_back( MotionPose( 300,jointangles(
            110,50,5,110,50,5,-40,0,120,-40,0,120,-20,0,100,  0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
            100,-15,0,100,-15,0,-40,0,120,-40,0,120,-30,0,100,  -30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
            90,-15,50,90,-15,50,-40,0,120,-40,0,120,-30,0,30,  -30,0,0)));
		p.pose.push_back( MotionPose( 160,jointangles(
			0,-15,70,0,-15,70,-30,0,120,-30,0,120,-30,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 160,jointangles(
			0,-15,70,0,-15,70,-30,0,120,-30,0,120,-30,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 240,jointangles(
			0,-15,70,0,-15,70,-30,0,120,-30,0,120,-80,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
			0,0,30,0,0,30,-30,0,100,-30,0,100,-80,0,-60,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			-20,10,20,-20,10,20,0,0,0,0,0,0,-80,0,-60,-50,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
			-20,30,30,-20,30,30,0,0,0,0,0,0,0,0,-60,-50,0,0)));
		p.pose.push_back( MotionPose( 400,jointangles(
			40,30,70,40,30,70,-50,10,100,-50,10,100,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			40,30,90,40,30,90,-50,20,100,-50,20,100,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			100,30,70,100,30,70,0,20,100,0,20,100,0,0,30,0,0,0)));
		p.pose.push_back( MotionPose( 600,jointangles(
			30,30,70,30,30,70,0,30,100,0,30,100,0,0,30,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();
		*/

		//MUNESHOT_00
        RegisterMotionName("MUNESHOT_00");
		p.pose.push_back( MotionPose( 100,jointangles(
	       -37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
        p.pose.push_back( MotionPose( 55,jointangles(
	       -37,0,80,-37,0,80,0,0,110,0,0,110,0,0,0,0,0,0)));
        p.pose.push_back( MotionPose( 45,jointangles(
	       -45,0,70,-45,0,70,15,0,80,15,0,80,-45,0,-25,0,0,0)));
        p.pose.push_back( MotionPose( 40,jointangles(
	       -50,0,60,-50,0,60,25,0,70,25,0,70,-35,0,-15,0,0,0)));
        p.pose.push_back( MotionPose( 60,jointangles(
	       -75,10,85,-75,10,85,50,0,40,50,0,40,-17,0,-7,0,0,0)));
        p.pose.push_back( MotionPose( 77,jointangles(
	       -90,25,127,-90,25,127,90,0,25,90,0,25,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 30,jointangles(
	       -100,30,127,-100,30,127,90,0,25,90,0,25,0,0,0,0,0,0)));
        p.num_pose = p.pose.size();
        motions.push_back(p);p.pose.clear();


		//HOOKE_LEFT_45
		RegisterMotionName("HOOKE_LEFT_45");
        p.pose.push_back( MotionPose( 65,jointangles(
	       -37,15,90,-37,25,90,-40,-15,90,-40,25,90,0,0,0,0,0,0)));
        p.pose.push_back( MotionPose( 60,jointangles(
	       -45,20,80,-45,20,80,-20,-15,80,-20,40,80,0,10,0,0,0,0)));
        p.pose.push_back( MotionPose( 100,jointangles(
	       0,3,60,-47,3,70,30,-8,70,20,13,70,0,20,0,0,0,0)));
        p.pose.push_back( MotionPose( 120,jointangles(
	       80,-15,45,-50,-15,65,80,10,60,50,-5,50,0,40,0,0,0,0)));
        p.pose.push_back( MotionPose( 50,jointangles(
	       90,-15,35,-70,-15,60,90,10,50,65,-10,40,0,45,0,0,0,0)));
        p.pose.push_back( MotionPose( 70,jointangles(
	       50,-10,65,-42,-10,93,52,0,70,42,0,70,0,45,0,0,0,0)));
        p.pose.push_back( MotionPose( 70,jointangles(
	       10,-5,86,-39,-5,101,22,0,90,8,0,90,0,45,0,0,0,0)));
        p.pose.push_back( MotionPose( 80,jointangles(
	       -37,0,110,-37,0,110,-26,0,110,-26,0,110,0,45,0,0,0,0)));
        p.num_pose = p.pose.size();
        motions.push_back(p);p.pose.clear();


		//HOOKE_RIGHT_45
		RegisterMotionName("HOOKE_RIGHT_45");
        p.pose.push_back( MotionPose( 65,jointangles(
	       -37,25,90,-37,15,90,-40,25,90,-40,-15,90,0,0,0,0,0,0)));
        p.pose.push_back( MotionPose( 60,jointangles(
	       -45,20,80,-45,20,80,-20,40,80,-20,-15,80,0,-10,0,0,0,0)));
        p.pose.push_back( MotionPose( 100,jointangles(
	       -47,3,70,0,3,60,20,13,70,30,-8,70,0,-20,0,0,0,0)));
        p.pose.push_back( MotionPose( 120,jointangles(
	       -50,-15,65,80,-15,45,50,-5,50,80,10,60,0,-40,0,0,0,0)));
        p.pose.push_back( MotionPose( 50,jointangles(
	       -70,-15,60,90,-15,35,65,-10,40,50,10,50,0,-45,0,0,0,0)));
        p.pose.push_back( MotionPose( 70,jointangles(
	       -42,-10,93,50,-10,65,42,0,70,52,0,70,0,-45,0,0,0,0)));
        p.pose.push_back( MotionPose( 70,jointangles(
	       -39,-5,101,10,-5,86,8,0,90,22,0,90,0,-45,0,0,0,0)));
        p.pose.push_back( MotionPose( 80,jointangles(
	       -37,0,110,-37,0,110,-26,0,110,-26,0,110,0,-45,0,0,0,0)));
        p.num_pose = p.pose.size();
        motions.push_back(p);p.pose.clear();

		//HG_FRONT
		RegisterMotionName("HG_FRONT");
		p.pose.push_back( MotionPose( 80,jointangles(
            -30,40,80,-30,40,80,-30,10,127,-30,10,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
            -30,40,127,-30,40,127,-30,10,127,-30,10,127,-10,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 70,jointangles(
            -30,-10,60,-30,-10,60,-30,10,127,-30,10,127,-50,0,-20,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
            120,-10,60,120,-10,60,0,20,127,0,20,127,-80,0,20,-50,0,0)));
		p.pose.push_back( MotionPose( 200,jointangles(
            120,-10,0,120,-10,0,0,20,127,0,20,127,-80,0,20,-50,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//EJYA9_FRONT
		RegisterMotionName("EJYA9_FRONT");
		p.pose.push_back( MotionPose( 80,jointangles(
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			0,30,0,0,30,0,-30,20,127,-30,20,127,-10,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 110,jointangles(
			0,30,0,0,30,0,-80,20,127,-80,20,127,-10,0,-10,0,0,0)));
		p.pose.push_back( MotionPose( 60,jointangles(
			20,5,0,20,5,0,0,20,127,0,20,127,-20,0,-10,-20,0,0)));
		p.pose.push_back( MotionPose( 160,jointangles(
			60,0,0,60,0,0,0,20,127,0,20,127,-70,0,10,-30,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			100,0,0,100,0,0,60,20,127,60,20,127,-80,0,15,-30,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//EJYA9_LEFT
		RegisterMotionName("EJYA9_LEFT");
		//p.pose.push_back( MotionPose( 50,jointangles(
		//	-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			-20,20,80,-20,20,80,-30,30,127,-30,10,127,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			0,20,60,-10,20,60,-60,30,127,-50,10,127,-10,10,15,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			20,15,30,0,15,30,0,30,127,-20,10,127,-20,-30,20,-20,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			100,0,0,60,0,0,30,30,127,0,10,127,-70,-35,20,-40,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			110,0,0,110,0,0,70,30,127,30,10,127,-80,-40,20,-40,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		//EJYA9_RIGHT
		RegisterMotionName("EJYA9_RIGHT");
		//p.pose.push_back( MotionPose( 50,jointangles(
		//	-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			-20,20,80,-20,20,80,-30,10,127,-30,30,127,0,0,0,0,0,0)));
		p.pose.push_back( MotionPose( 80,jointangles(
			-10,20,60,0,20,60,-50,10,127,-60,30,127,-10,-10,15,0,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			0,15,30,20,15,30,-20,10,127,0,30,127,-20,30,20,-20,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			60,0,0,100,0,0,0,10,127,30,30,127,-70,35,20,-40,0,0)));
		p.pose.push_back( MotionPose( 50,jointangles(
			110,0,0,110,0,0,30,10,127,70,30,127,-80,40,20,-40,0,0)));
		p.pose.push_back( MotionPose( 100,jointangles(
			-37,0,110,-37,0,110,-26,0,110,-26,0,110,0,0,0,0,0,0)));
		p.num_pose = p.pose.size();
		motions.push_back(p);p.pose.clear();

		isInitialized = true;
	}
}
