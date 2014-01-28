#include "BehaviorList.h"
//#include "../Motion/PresetWalkParam.h" //takeshita 05/0517
#include "../Motion/GaitMaker.h"
#include "../Motion/HeadController.h"
#include <math.h>

#include <OPENR/OSyslog.h>

bool BhvParaWalk::isInitialized = false;
WalkParam BhvParaWalk::walkparams[NUM_PARA_WALKPARAM];
static int theta_under[19];
static int dist_under[2];

#define DELTA_BALL_DISTANCE 80;//ボールまでの距離微調整用
#define DELTA_HOLD_DISTANCE 135;//210;//125;//ボール掴むときの距離微調整用

BhvParaWalk::BhvParaWalk(){
	if(!isInitialized){
		mode = PARA_BALL_APPROACH;
		for(int i=0;i<19;i++)	theta_under[i] = i*10 - 90;
		for(int i=0;i<2;i++)	dist_under[i] = i*50 + 50;

		//セットするパラメータ
		walkparams[WALK_PARA_SET].time = 270;
		walkparams[WALK_PARA_SET].gratio = 0.35;
		walkparams[WALK_PARA_SET].aratio = 0.25;
		walkparams[WALK_PARA_SET].legparam[0] = LegParam(15,95,0,0,0,0,0);
		walkparams[WALK_PARA_SET].legparam[1] = LegParam(15,95,0,0,0,0,0);
		walkparams[WALK_PARA_SET].legparam[2] = LegParam(-20,110,0,0,0,0,0);
		walkparams[WALK_PARA_SET].legparam[3] = LegParam(-20,110,0,0,0,0,0);
		walkparams[WALK_PARA_SET].move_x  = 0;
		walkparams[WALK_PARA_SET].move_y  = 0;
		walkparams[WALK_PARA_SET].move_tt = 0;
/*
		//WALK_SMALL5_RIGHT90
		walkparams[WALK_SMALL5_RIGHT90].time = 270;
		walkparams[WALK_SMALL5_RIGHT90].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT90].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT90].legparam[0] = LegParam(5,95,30,30,15,-90,-11);
		walkparams[WALK_SMALL5_RIGHT90].legparam[1] = LegParam(5,95,30,30,15,-74,-11);
		walkparams[WALK_SMALL5_RIGHT90].legparam[2] = LegParam(-5,110,0,30,5,-90,-7);
		walkparams[WALK_SMALL5_RIGHT90].legparam[3] = LegParam(-5,110,0,30,5,-74,-7);
		walkparams[WALK_SMALL5_RIGHT90].move_x  = 0;
		walkparams[WALK_SMALL5_RIGHT90].move_y  = -50/2;
		walkparams[WALK_SMALL5_RIGHT90].move_tt = 0;

		//WALK_SMALL5_RIGHT80
		walkparams[WALK_SMALL5_RIGHT80].time = 270;
		walkparams[WALK_SMALL5_RIGHT80].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT80].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT80].legparam[0] = LegParam(5,95,30,32,15,-78,-11);
		walkparams[WALK_SMALL5_RIGHT80].legparam[1] = LegParam(5,95,30,35,15,-68,-11);
		walkparams[WALK_SMALL5_RIGHT80].legparam[2] = LegParam(-5,110,0,30,5,-78,-7);
		walkparams[WALK_SMALL5_RIGHT80].legparam[3] = LegParam(-5,110,0,35,5,-68,-7);
		walkparams[WALK_SMALL5_RIGHT80].move_x  = 8.6/2;
		walkparams[WALK_SMALL5_RIGHT80].move_y  = -49.2/2;
		walkparams[WALK_SMALL5_RIGHT80].move_tt = 0;

		//WALK_SMALL5_RIGHT70
		walkparams[WALK_SMALL5_RIGHT70].time = 270;
		walkparams[WALK_SMALL5_RIGHT70].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT70].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT70].legparam[0] = LegParam(5,95,30,30,15,-69,-14);
		walkparams[WALK_SMALL5_RIGHT70].legparam[1] = LegParam(5,95,30,32,15,-63,-14);
		walkparams[WALK_SMALL5_RIGHT70].legparam[2] = LegParam(-5,110,0,30,5,-69,-10);
		walkparams[WALK_SMALL5_RIGHT70].legparam[3] = LegParam(-5,110,0,32,5,-63,-10);
		walkparams[WALK_SMALL5_RIGHT70].move_x  = 17.1/2;
		walkparams[WALK_SMALL5_RIGHT70].move_y  = -46.9/2;
		walkparams[WALK_SMALL5_RIGHT70].move_tt = 0;

		//WALK_SMALL5_RIGHT60
		walkparams[WALK_SMALL5_RIGHT60].time = 270;
		walkparams[WALK_SMALL5_RIGHT60].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT60].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT60].legparam[0] = LegParam(5,95,30,32,15,-58,-13);
		walkparams[WALK_SMALL5_RIGHT60].legparam[1] = LegParam(5,95,30,33,15,-56,-13);
		walkparams[WALK_SMALL5_RIGHT60].legparam[2] = LegParam(-5,110,0,32,5,-58,-9);
		walkparams[WALK_SMALL5_RIGHT60].legparam[3] = LegParam(-5,110,0,33,5,-56,-9);
		walkparams[WALK_SMALL5_RIGHT60].move_x  = 25/2;
		walkparams[WALK_SMALL5_RIGHT60].move_y  = -43.3/2;
		walkparams[WALK_SMALL5_RIGHT60].move_tt = 0;

		//WALK_SMALL5_RIGHT50
		walkparams[WALK_SMALL5_RIGHT50].time = 270;
		walkparams[WALK_SMALL5_RIGHT50].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT50].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT50].legparam[0] = LegParam(5,95,30,33,15,-47,-12);
		walkparams[WALK_SMALL5_RIGHT50].legparam[1] = LegParam(5,95,30,31,15,-48,-12);
		walkparams[WALK_SMALL5_RIGHT50].legparam[2] = LegParam(-5,110,0,33,5,-47,-8);
		walkparams[WALK_SMALL5_RIGHT50].legparam[3] = LegParam(-5,110,0,31,5,-48,-8);
		walkparams[WALK_SMALL5_RIGHT50].move_x  = 32.1/2;
		walkparams[WALK_SMALL5_RIGHT50].move_y  = -38.3/2;
		walkparams[WALK_SMALL5_RIGHT50].move_tt = 0;

		//WALK_SMALL5_RIGHT40
		walkparams[WALK_SMALL5_RIGHT40].time = 270;
		walkparams[WALK_SMALL5_RIGHT40].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT40].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT40].legparam[0] = LegParam(5,95,30,34,15,-43,-11);
		walkparams[WALK_SMALL5_RIGHT40].legparam[1] = LegParam(5,95,30,31,15,-38,-11);
		walkparams[WALK_SMALL5_RIGHT40].legparam[2] = LegParam(-5,110,0,34,5,-43,-7);
		walkparams[WALK_SMALL5_RIGHT40].legparam[3] = LegParam(-5,110,0,31,5,-38,-7);
		walkparams[WALK_SMALL5_RIGHT40].move_x  = 38.3/2;
		walkparams[WALK_SMALL5_RIGHT40].move_y  = -32.1/2;
		walkparams[WALK_SMALL5_RIGHT40].move_tt = 0;

		//WALK_SMALL5_RIGHT30
		walkparams[WALK_SMALL5_RIGHT30].time = 270;
		walkparams[WALK_SMALL5_RIGHT30].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT30].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT30].legparam[0] = LegParam(5,95,30,32,15,-32,-12);
		walkparams[WALK_SMALL5_RIGHT30].legparam[1] = LegParam(5,95,30,27,15,-29,-12);
		walkparams[WALK_SMALL5_RIGHT30].legparam[2] = LegParam(-5,110,0,32,5,-32,-8);
		walkparams[WALK_SMALL5_RIGHT30].legparam[3] = LegParam(-5,110,0,27,5,-29,-8);
		walkparams[WALK_SMALL5_RIGHT30].move_x  = 43.3/2;
		walkparams[WALK_SMALL5_RIGHT30].move_y  = -25/2;
		walkparams[WALK_SMALL5_RIGHT30].move_tt = 0;

		//WALK_SMALL5_RIGHT20
		walkparams[WALK_SMALL5_RIGHT20].time = 270;
		walkparams[WALK_SMALL5_RIGHT20].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT20].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT20].legparam[0] = LegParam(5,95,30,30,15,-25,-13);
		walkparams[WALK_SMALL5_RIGHT20].legparam[1] = LegParam(5,95,30,26,15,-26,-13);
		walkparams[WALK_SMALL5_RIGHT20].legparam[2] = LegParam(-5,110,0,30,5,-25,-9);
		walkparams[WALK_SMALL5_RIGHT20].legparam[3] = LegParam(-5,110,0,26,5,-26,-9);
		walkparams[WALK_SMALL5_RIGHT20].move_x  = 46.9/2;
		walkparams[WALK_SMALL5_RIGHT20].move_y  = -17.1/2;
		walkparams[WALK_SMALL5_RIGHT20].move_tt = 0;

		//WALK_SMALL5_RIGHT10
		walkparams[WALK_SMALL5_RIGHT10].time = 270;
		walkparams[WALK_SMALL5_RIGHT10].gratio = 0.45;
		walkparams[WALK_SMALL5_RIGHT10].aratio = 0.25;
		walkparams[WALK_SMALL5_RIGHT10].legparam[0] = LegParam(5,95,30,30,15,-17,-12);
		walkparams[WALK_SMALL5_RIGHT10].legparam[1] = LegParam(5,95,30,26,15,-17,-12);
		walkparams[WALK_SMALL5_RIGHT10].legparam[2] = LegParam(-5,110,0,30,5,-17,-8);
		walkparams[WALK_SMALL5_RIGHT10].legparam[3] = LegParam(-5,110,0,26,5,-17,-8);
		walkparams[WALK_SMALL5_RIGHT10].move_x  = 49.2/2;
		walkparams[WALK_SMALL5_RIGHT10].move_y  = -8.6/2;
		walkparams[WALK_SMALL5_RIGHT10].move_tt = 0;

		//WALK_SMALL5_FORWARD
		walkparams[WALK_SMALL5_FORWARD].time = 270;
		walkparams[WALK_SMALL5_FORWARD].gratio = 0.45;
		walkparams[WALK_SMALL5_FORWARD].aratio = 0.25;
		walkparams[WALK_SMALL5_FORWARD].legparam[0] = LegParam(5,95,30,28,15,2,-12);
		walkparams[WALK_SMALL5_FORWARD].legparam[1] = LegParam(5,95,30,28,15,2,-12);
		walkparams[WALK_SMALL5_FORWARD].legparam[2] = LegParam(-5,110,0,28,5,2,-8);
		walkparams[WALK_SMALL5_FORWARD].legparam[3] = LegParam(-5,110,0,28,5,2,-8);
		walkparams[WALK_SMALL5_FORWARD].move_x  = 50/2;
		walkparams[WALK_SMALL5_FORWARD].move_y  = 0;
		walkparams[WALK_SMALL5_FORWARD].move_tt = 0;

		//WALK_SMALL5_LEFT10
		walkparams[WALK_SMALL5_LEFT10].time = 270;
		walkparams[WALK_SMALL5_LEFT10].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT10].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT10].legparam[0] = LegParam(5,95,30,25,15,8,-12);
		walkparams[WALK_SMALL5_LEFT10].legparam[1] = LegParam(5,95,30,27,15,9,-12);
		walkparams[WALK_SMALL5_LEFT10].legparam[2] = LegParam(-5,110,0,25,5,8,-8);
		walkparams[WALK_SMALL5_LEFT10].legparam[3] = LegParam(-5,110,0,27,5,9,-8);
		walkparams[WALK_SMALL5_LEFT10].move_x  = 49.2/2;
		walkparams[WALK_SMALL5_LEFT10].move_y  = 8.6/2;
		walkparams[WALK_SMALL5_LEFT10].move_tt = 0;

		//WALK_SMALL5_LEFT20
		walkparams[WALK_SMALL5_LEFT20].time = 270;
		walkparams[WALK_SMALL5_LEFT20].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT20].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT20].legparam[0] = LegParam(5,95,30,24,15,24,-12);
		walkparams[WALK_SMALL5_LEFT20].legparam[1] = LegParam(5,95,30,29,15,22,-12);
		walkparams[WALK_SMALL5_LEFT20].legparam[2] = LegParam(-5,110,0,24,5,24,-8);
		walkparams[WALK_SMALL5_LEFT20].legparam[3] = LegParam(-5,110,0,29,5,22,-8);
		walkparams[WALK_SMALL5_LEFT20].move_x  = 46.9/2;
		walkparams[WALK_SMALL5_LEFT20].move_y  = 17.1/2;
		walkparams[WALK_SMALL5_LEFT20].move_tt = 0;

		//WALK_SMALL5_LEFT30
		walkparams[WALK_SMALL5_LEFT30].time = 270;
		walkparams[WALK_SMALL5_LEFT30].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT30].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT30].legparam[0] = LegParam(5,95,30,24,15,30,-13);
		walkparams[WALK_SMALL5_LEFT30].legparam[1] = LegParam(5,95,30,30,15,29,-13);
		walkparams[WALK_SMALL5_LEFT30].legparam[2] = LegParam(-5,110,0,24,5,30,-9);
		walkparams[WALK_SMALL5_LEFT30].legparam[3] = LegParam(-5,110,0,30,5,29,-9);
		walkparams[WALK_SMALL5_LEFT30].move_x  = 43.3/2;
		walkparams[WALK_SMALL5_LEFT30].move_y  = 25/2;
		walkparams[WALK_SMALL5_LEFT30].move_tt = 0;

		//WALK_SMALL5_LEFT40
		walkparams[WALK_SMALL5_LEFT40].time = 270;
		walkparams[WALK_SMALL5_LEFT40].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT40].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT40].legparam[0] = LegParam(5,95,30,25,15,48,-12);
		walkparams[WALK_SMALL5_LEFT40].legparam[1] = LegParam(5,95,30,32,15,46,-12);
		walkparams[WALK_SMALL5_LEFT40].legparam[2] = LegParam(-5,110,0,25,5,48,-8);
		walkparams[WALK_SMALL5_LEFT40].legparam[3] = LegParam(-5,110,0,32,5,46,-8);
		walkparams[WALK_SMALL5_LEFT40].move_x  = 38.3/2;
		walkparams[WALK_SMALL5_LEFT40].move_y  = 32.1/2;
		walkparams[WALK_SMALL5_LEFT40].move_tt = 0;

		//WALK_SMALL5_LEFT50
		walkparams[WALK_SMALL5_LEFT50].time = 270;
		walkparams[WALK_SMALL5_LEFT50].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT50].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT50].legparam[0] = LegParam(5,95,30,29,15,62,-12);
		walkparams[WALK_SMALL5_LEFT50].legparam[1] = LegParam(5,95,30,31,15,60,-12);
		walkparams[WALK_SMALL5_LEFT50].legparam[2] = LegParam(-5,110,0,29,5,62,-8);
		walkparams[WALK_SMALL5_LEFT50].legparam[3] = LegParam(-5,110,0,31,5,60,-8);
		walkparams[WALK_SMALL5_LEFT50].move_x  = 32.1/2;
		walkparams[WALK_SMALL5_LEFT50].move_y  = 38.3/2;
		walkparams[WALK_SMALL5_LEFT50].move_tt = 0;

		//WALK_SMALL5_LEFT60
		walkparams[WALK_SMALL5_LEFT60].time = 270;
		walkparams[WALK_SMALL5_LEFT60].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT60].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT60].legparam[0] = LegParam(5,95,30,31,15,73,-11);
		walkparams[WALK_SMALL5_LEFT60].legparam[1] = LegParam(5,95,30,31,15,72,-11);
		walkparams[WALK_SMALL5_LEFT60].legparam[2] = LegParam(-5,110,0,30,5,73,-7);
		walkparams[WALK_SMALL5_LEFT60].legparam[3] = LegParam(-5,110,0,30,5,72,-7);
		walkparams[WALK_SMALL5_LEFT60].move_x  = 25/2;
		walkparams[WALK_SMALL5_LEFT60].move_y  = 43.3/2;
		walkparams[WALK_SMALL5_LEFT60].move_tt = 0;

		//WALK_SMALL5_LEFT70
		walkparams[WALK_SMALL5_LEFT70].time = 270;
		walkparams[WALK_SMALL5_LEFT70].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT70].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT70].legparam[0] = LegParam(10,95,30,30,15,78,-12);
		walkparams[WALK_SMALL5_LEFT70].legparam[1] = LegParam(10,95,30,30,15,78,-12);
		walkparams[WALK_SMALL5_LEFT70].legparam[2] = LegParam(-5,110,0,30,5,78,-8);
		walkparams[WALK_SMALL5_LEFT70].legparam[3] = LegParam(-5,110,0,30,5,78,-8);
		walkparams[WALK_SMALL5_LEFT70].move_x  = 17.1/2;
		walkparams[WALK_SMALL5_LEFT70].move_y  = 46.9/2;
		walkparams[WALK_SMALL5_LEFT70].move_tt = 0;

		//WALK_SMALL5_LEFT80
		walkparams[WALK_SMALL5_LEFT80].time = 270;
		walkparams[WALK_SMALL5_LEFT80].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT80].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT80].legparam[0] = LegParam(5,95,30,34,15,81,-11);
		walkparams[WALK_SMALL5_LEFT80].legparam[1] = LegParam(5,95,30,36,15,100,-11);
		walkparams[WALK_SMALL5_LEFT80].legparam[2] = LegParam(-5,110,0,33,5,81,-7);
		walkparams[WALK_SMALL5_LEFT80].legparam[3] = LegParam(-5,110,0,36,5,100,-7);
		walkparams[WALK_SMALL5_LEFT80].move_x  = 8.6/2;
		walkparams[WALK_SMALL5_LEFT80].move_y  = 49.2/2;
		walkparams[WALK_SMALL5_LEFT80].move_tt = 0;

		//WALK_SMALL5_LEFT90
		walkparams[WALK_SMALL5_LEFT90].time = 270;
		walkparams[WALK_SMALL5_LEFT90].gratio = 0.45;
		walkparams[WALK_SMALL5_LEFT90].aratio = 0.25;
		walkparams[WALK_SMALL5_LEFT90].legparam[0] = LegParam(5,95,30,33,15,89,-11);
		walkparams[WALK_SMALL5_LEFT90].legparam[1] = LegParam(5,95,30,35,15,109,-11);
		walkparams[WALK_SMALL5_LEFT90].legparam[2] = LegParam(-5,110,0,33,5,89,-7);
		walkparams[WALK_SMALL5_LEFT90].legparam[3] = LegParam(-5,110,0,35,5,109,-7);
		walkparams[WALK_SMALL5_LEFT90].move_x  = 0;
		walkparams[WALK_SMALL5_LEFT90].move_y  = 50/2;
		walkparams[WALK_SMALL5_LEFT90].move_tt = 0;


		//WALK_SMALL10_RIGHT90
		walkparams[WALK_SMALL10_RIGHT90].time = 270;
		walkparams[WALK_SMALL10_RIGHT90].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT90].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT90].legparam[0] = LegParam(5,95,30,57,15,-96,-11);
		walkparams[WALK_SMALL10_RIGHT90].legparam[1] = LegParam(5,95,30,57,15,-78,-11);
		walkparams[WALK_SMALL10_RIGHT90].legparam[2] = LegParam(-5,110,0,55,5,-96,-7);
		walkparams[WALK_SMALL10_RIGHT90].legparam[3] = LegParam(-5,110,0,54,5,-78,-7);
		walkparams[WALK_SMALL10_RIGHT90].move_x  = 0;
		walkparams[WALK_SMALL10_RIGHT90].move_y  = -100/2;
		walkparams[WALK_SMALL10_RIGHT90].move_tt = 0;

		//WALK_SMALL10_RIGHT80
		walkparams[WALK_SMALL10_RIGHT80].time = 270;
		walkparams[WALK_SMALL10_RIGHT80].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT80].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT80].legparam[0] = LegParam(5,95,30,54,15,-78,-11);
		walkparams[WALK_SMALL10_RIGHT80].legparam[1] = LegParam(5,95,30,54,15,-72,-11);
		walkparams[WALK_SMALL10_RIGHT80].legparam[2] = LegParam(-5,110,0,54,5,-78,-7);
		walkparams[WALK_SMALL10_RIGHT80].legparam[3] = LegParam(-5,110,0,54,5,-72,-7);
		walkparams[WALK_SMALL10_RIGHT80].move_x  = 17.3/2;
		walkparams[WALK_SMALL10_RIGHT80].move_y  = -98/2;
		walkparams[WALK_SMALL10_RIGHT80].move_tt = 0;

		//WALK_SMALL10_RIGHT70
		walkparams[WALK_SMALL10_RIGHT70].time = 270;
		walkparams[WALK_SMALL10_RIGHT70].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT70].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT70].legparam[0] = LegParam(5,95,30,53,15,-71,-12);
		walkparams[WALK_SMALL10_RIGHT70].legparam[1] = LegParam(5,95,30,53,15,-66,-12);
		walkparams[WALK_SMALL10_RIGHT70].legparam[2] = LegParam(-5,110,0,53,5,-71,-8);
		walkparams[WALK_SMALL10_RIGHT70].legparam[3] = LegParam(-5,110,0,53,5,-66,-8);
		walkparams[WALK_SMALL10_RIGHT70].move_x  = 34.2/2;
		walkparams[WALK_SMALL10_RIGHT70].move_y  = -93.9/2;
		walkparams[WALK_SMALL10_RIGHT70].move_tt = 0;

		//WALK_SMALL10_RIGHT60
		walkparams[WALK_SMALL10_RIGHT60].time = 270;
		walkparams[WALK_SMALL10_RIGHT60].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT60].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT60].legparam[0] = LegParam(5,95,30,51,15,-61,-12);
		walkparams[WALK_SMALL10_RIGHT60].legparam[1] = LegParam(5,95,30,50,15,-60,-12);
		walkparams[WALK_SMALL10_RIGHT60].legparam[2] = LegParam(-5,110,0,51,5,-61,-8);
		walkparams[WALK_SMALL10_RIGHT60].legparam[3] = LegParam(-5,110,0,50,5,-60,-8);
		walkparams[WALK_SMALL10_RIGHT60].move_x  = 50/2;
		walkparams[WALK_SMALL10_RIGHT60].move_y  = -86.6/2;
		walkparams[WALK_SMALL10_RIGHT60].move_tt = 0;

		//WALK_SMALL10_RIGHT50
		walkparams[WALK_SMALL10_RIGHT50].time = 270;
		walkparams[WALK_SMALL10_RIGHT50].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT50].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT50].legparam[0] = LegParam(5,95,30,51,15,-52,-11);
		walkparams[WALK_SMALL10_RIGHT50].legparam[1] = LegParam(5,95,30,50,15,-50,-11);
		walkparams[WALK_SMALL10_RIGHT50].legparam[2] = LegParam(-5,110,0,51,5,-52,-7);
		walkparams[WALK_SMALL10_RIGHT50].legparam[3] = LegParam(-5,110,0,50,5,-50,-7);
		walkparams[WALK_SMALL10_RIGHT50].move_x  = 64.2/2;
		walkparams[WALK_SMALL10_RIGHT50].move_y  = -76.6/2;
		walkparams[WALK_SMALL10_RIGHT50].move_tt = 0;

		//WALK_SMALL10_RIGHT40
		walkparams[WALK_SMALL10_RIGHT40].time = 270;
		walkparams[WALK_SMALL10_RIGHT40].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT40].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT40].legparam[0] = LegParam(5,95,30,50,15,-46,-11);
		walkparams[WALK_SMALL10_RIGHT40].legparam[1] = LegParam(5,95,30,46,15,-41,-11);
		walkparams[WALK_SMALL10_RIGHT40].legparam[2] = LegParam(-5,110,0,50,5,-46,-7);
		walkparams[WALK_SMALL10_RIGHT40].legparam[3] = LegParam(-5,110,0,46,5,-41,-7);
		walkparams[WALK_SMALL10_RIGHT40].move_x  = 76.6/2;
		walkparams[WALK_SMALL10_RIGHT40].move_y  = -64.2/2;
		walkparams[WALK_SMALL10_RIGHT40].move_tt = 0;

		//WALK_SMALL10_RIGHT30
		walkparams[WALK_SMALL10_RIGHT30].time = 270;
		walkparams[WALK_SMALL10_RIGHT30].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT30].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT30].legparam[0] = LegParam(5,95,30,45,15,-34,-12);
		walkparams[WALK_SMALL10_RIGHT30].legparam[1] = LegParam(5,95,30,43,15,-31,-12);
		walkparams[WALK_SMALL10_RIGHT30].legparam[2] = LegParam(-5,110,0,45,5,-34,-8);
		walkparams[WALK_SMALL10_RIGHT30].legparam[3] = LegParam(-5,110,0,43,5,-31,-8);
		walkparams[WALK_SMALL10_RIGHT30].move_x  = 86.6/2;
		walkparams[WALK_SMALL10_RIGHT30].move_y  = -50/2;
		walkparams[WALK_SMALL10_RIGHT30].move_tt = 0;

		//WALK_SMALL10_RIGHT20
		walkparams[WALK_SMALL10_RIGHT20].time = 270;
		walkparams[WALK_SMALL10_RIGHT20].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT20].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT20].legparam[0] = LegParam(5,95,30,44,15,-24,-11);
		walkparams[WALK_SMALL10_RIGHT20].legparam[1] = LegParam(5,95,30,42,15,-22,-11);
		walkparams[WALK_SMALL10_RIGHT20].legparam[2] = LegParam(-5,110,0,44,5,-24,-7);
		walkparams[WALK_SMALL10_RIGHT20].legparam[3] = LegParam(-5,110,0,42,5,-22,-7);
		walkparams[WALK_SMALL10_RIGHT20].move_x  = 93.9/2;
		walkparams[WALK_SMALL10_RIGHT20].move_y  = -34.2/2;
		walkparams[WALK_SMALL10_RIGHT20].move_tt = 0;

		//WALK_SMALL10_RIGHT10
		walkparams[WALK_SMALL10_RIGHT10].time = 270;
		walkparams[WALK_SMALL10_RIGHT10].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT10].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT10].legparam[0] = LegParam(5,95,30,42,15,-15,-12);
		walkparams[WALK_SMALL10_RIGHT10].legparam[1] = LegParam(5,95,30,39,15,-14,-12);
		walkparams[WALK_SMALL10_RIGHT10].legparam[2] = LegParam(-5,110,0,42,5,-15,-8);
		walkparams[WALK_SMALL10_RIGHT10].legparam[3] = LegParam(-5,110,0,39,5,-14,-8);
		walkparams[WALK_SMALL10_RIGHT10].move_x  = 98.4/2;
		walkparams[WALK_SMALL10_RIGHT10].move_y  = -17.3/2;
		walkparams[WALK_SMALL10_RIGHT10].move_tt = 0;

		//WALK_SMALL10_FORWARD
		walkparams[WALK_SMALL10_FORWARD].time = 270;
		walkparams[WALK_SMALL10_FORWARD].gratio = 0.45;
		walkparams[WALK_SMALL10_FORWARD].aratio = 0.25;
		walkparams[WALK_SMALL10_FORWARD].legparam[0] = LegParam(5,95,30,38,15,5,-12);
		walkparams[WALK_SMALL10_FORWARD].legparam[1] = LegParam(5,95,30,39,15,4,-12);
		walkparams[WALK_SMALL10_FORWARD].legparam[2] = LegParam(-5,110,0,38,5,5,-8);
		walkparams[WALK_SMALL10_FORWARD].legparam[3] = LegParam(-5,110,0,39,5,4,-8);
		walkparams[WALK_SMALL10_FORWARD].move_x  = 100/2;
		walkparams[WALK_SMALL10_FORWARD].move_y  = 0;
		walkparams[WALK_SMALL10_FORWARD].move_tt = 0;

		//WALK_SMALL10_LEFT10
		walkparams[WALK_SMALL10_LEFT10].time = 270;
		walkparams[WALK_SMALL10_LEFT10].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT10].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT10].legparam[0] = LegParam(5,95,30,37,15,12,-13);
		walkparams[WALK_SMALL10_LEFT10].legparam[1] = LegParam(5,95,30,41,15,11,-13);
		walkparams[WALK_SMALL10_LEFT10].legparam[2] = LegParam(-5,110,0,37,5,12,-9);
		walkparams[WALK_SMALL10_LEFT10].legparam[3] = LegParam(-5,110,0,41,5,11,-9);
		walkparams[WALK_SMALL10_LEFT10].move_x  = 98.4/2;
		walkparams[WALK_SMALL10_LEFT10].move_y  = 17.3/2;
		walkparams[WALK_SMALL10_LEFT10].move_tt = 0;


		//WALK_SMALL10_LEFT20
		walkparams[WALK_SMALL10_LEFT20].time = 270;
		walkparams[WALK_SMALL10_LEFT20].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT20].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT20].legparam[0] = LegParam(5,95,30,40,15,23,-11);
		walkparams[WALK_SMALL10_LEFT20].legparam[1] = LegParam(5,95,30,45,15,22,-11);
		walkparams[WALK_SMALL10_LEFT20].legparam[2] = LegParam(-5,110,0,40,5,23,-7);
		walkparams[WALK_SMALL10_LEFT20].legparam[3] = LegParam(-5,110,0,45,5,22,-7);
		walkparams[WALK_SMALL10_LEFT20].move_x  = 93.9/2;
		walkparams[WALK_SMALL10_LEFT20].move_y  = 34.2/2;
		walkparams[WALK_SMALL10_LEFT20].move_tt = 0;

		//WALK_SMALL10_LEFT30
		walkparams[WALK_SMALL10_LEFT30].time = 270;
		walkparams[WALK_SMALL10_LEFT30].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT30].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT30].legparam[0] = LegParam(5,95,30,38,15,35,-13);
		walkparams[WALK_SMALL10_LEFT30].legparam[1] = LegParam(5,95,30,44,15,36,-13);
		walkparams[WALK_SMALL10_LEFT30].legparam[2] = LegParam(-5,110,0,38,5,35,-9);
		walkparams[WALK_SMALL10_LEFT30].legparam[3] = LegParam(-5,110,0,44,5,36,-9);
		walkparams[WALK_SMALL10_LEFT30].move_x  = 86.6/2;
		walkparams[WALK_SMALL10_LEFT30].move_y  = 50/2;
		walkparams[WALK_SMALL10_LEFT30].move_tt = 0;

		//WALK_SMALL10_LEFT40
		walkparams[WALK_SMALL10_LEFT40].time = 270;
		walkparams[WALK_SMALL10_LEFT40].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT40].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT40].legparam[0] = LegParam(5,95,30,41,15,51,-13);
		walkparams[WALK_SMALL10_LEFT40].legparam[1] = LegParam(5,95,30,48,15,49,-13);
		walkparams[WALK_SMALL10_LEFT40].legparam[2] = LegParam(-5,110,0,41,5,51,-9);
		walkparams[WALK_SMALL10_LEFT40].legparam[3] = LegParam(-5,110,0,48,5,49,-9);
		walkparams[WALK_SMALL10_LEFT40].move_x  = 76.6/2;
		walkparams[WALK_SMALL10_LEFT40].move_y  = 64.2/2;
		walkparams[WALK_SMALL10_LEFT40].move_tt = 0;

		//WALK_SMALL10_LEFT50
		walkparams[WALK_SMALL10_LEFT50].time = 270;
		walkparams[WALK_SMALL10_LEFT50].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT50].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT50].legparam[0] = LegParam(5,95,30,50,15,64,-11);
		walkparams[WALK_SMALL10_LEFT50].legparam[1] = LegParam(5,95,30,52,15,66,-11);
		walkparams[WALK_SMALL10_LEFT50].legparam[2] = LegParam(-5,110,0,50,5,64,-7);
		walkparams[WALK_SMALL10_LEFT50].legparam[3] = LegParam(-5,110,0,52,5,66,-7);
		walkparams[WALK_SMALL10_LEFT50].move_x  = 64.2/2;
		walkparams[WALK_SMALL10_LEFT50].move_y  = 76.6/2;
		walkparams[WALK_SMALL10_LEFT50].move_tt = 0;

		//WALK_SMALL10_LEFT60
		walkparams[WALK_SMALL10_LEFT60].time = 270;
		walkparams[WALK_SMALL10_LEFT60].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT60].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT60].legparam[0] = LegParam(5,95,30,60,15,75,-11);
		walkparams[WALK_SMALL10_LEFT60].legparam[1] = LegParam(5,95,30,63,15,79,-11);
		walkparams[WALK_SMALL10_LEFT60].legparam[2] = LegParam(-5,110,0,57,5,76,-7);
		walkparams[WALK_SMALL10_LEFT60].legparam[3] = LegParam(-5,110,0,61,5,80,-7);
		walkparams[WALK_SMALL10_LEFT60].move_x  = 50/2;
		walkparams[WALK_SMALL10_LEFT60].move_y  = 86.6/2;
		walkparams[WALK_SMALL10_LEFT60].move_tt = 0;

		//WALK_SMALL10_LEFT70
		walkparams[WALK_SMALL10_LEFT70].time = 270;
		walkparams[WALK_SMALL10_LEFT70].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT70].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT70].legparam[0] = LegParam(10,95,30,56,15,83,-11);
		walkparams[WALK_SMALL10_LEFT70].legparam[1] = LegParam(10,95,30,56,15,83,-11);
		walkparams[WALK_SMALL10_LEFT70].legparam[2] = LegParam(-5,110,0,56,5,83,-7);
		walkparams[WALK_SMALL10_LEFT70].legparam[3] = LegParam(-5,110,0,56,5,83,-7);
		walkparams[WALK_SMALL10_LEFT70].move_x  = 34.2/2;
		walkparams[WALK_SMALL10_LEFT70].move_y  = 93.9/2;
		walkparams[WALK_SMALL10_LEFT70].move_tt = 0;

		//WALK_SMALL10_LEFT80
		walkparams[WALK_SMALL10_LEFT80].time = 270;
		walkparams[WALK_SMALL10_LEFT80].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT80].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT80].legparam[0] = LegParam(5,95,30,70,15,80,-1);
		walkparams[WALK_SMALL10_LEFT80].legparam[1] = LegParam(5,95,30,70,15,90,-1);
		walkparams[WALK_SMALL10_LEFT80].legparam[2] = LegParam(-5,110,5,65,5,100,-5);
		walkparams[WALK_SMALL10_LEFT80].legparam[3] = LegParam(-5,110,5,65,5,100,-5);
		walkparams[WALK_SMALL10_LEFT80].move_x  = 17.3/2;
		walkparams[WALK_SMALL10_LEFT80].move_y  = 98.4/2;
		walkparams[WALK_SMALL10_LEFT80].move_tt = 0;

		//WALK_SMALL10_LEFT90
		walkparams[WALK_SMALL10_LEFT90].time = 270;
		walkparams[WALK_SMALL10_LEFT90].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT90].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT90].legparam[0] = LegParam(5,95,30,70,15,85,-1);
		walkparams[WALK_SMALL10_LEFT90].legparam[1] = LegParam(5,95,30,70,15,105,-1);
		walkparams[WALK_SMALL10_LEFT90].legparam[2] = LegParam(-5,110,5,65,5,110,-5);
		walkparams[WALK_SMALL10_LEFT90].legparam[3] = LegParam(-5,110,5,65,5,110,-5);
		walkparams[WALK_SMALL10_LEFT90].move_x  = 0;
		walkparams[WALK_SMALL10_LEFT90].move_y  = 100/2;
		walkparams[WALK_SMALL10_LEFT90].move_tt = 0;
		*/

	}
	isInitialized = true;
}

void BhvParaWalk::Execute(){
	static int cnt_flag = 0;
	if(state.ball.losttime > 100) return;

	if(mode == PARA_BALL_APPROACH) SetShotMode();//ボールを掴むかどうかをセット
	static bool ball_hold = true;//掴むかどうかのフラグ
    ball_hold = SetHoldOrDirect();

	//モーションをして，どれだけ経過したか．一定時間，モーションを出ないようにする．改良の必要アリ
	static int aft_motion = 0;
	static bool motion_flag = false;

	//if(mode != PARA_BALL_APPROACH) OSYSPRINT(("MODE = %d : ", mode));
	switch(mode){
		case PARA_BALL_APPROACH:
			cnt_flag =0;
			SetFaceLED(FACE_PARA_1, true);
			double x,y;
			double set_x;
			double set_y;
            //最後の2歩で接近するときになったか＆ボールを掴むかどうか
			if(ball_hold){
				set_x = state.ball.x-DELTA_HOLD_DISTANCE;
				set_y = state.ball.y;
				SetTargetXY(x, y, set_x, set_y, true);
				if(ParaWalkApproachXY()){
                    mode = PARA_LAST_APPROACH;//PARA_BALL_HOLD;
					SetHeadMode(HEAD_TRACK);
					//SetHeadAngle(-50, 0, 40, -35, 500, true);
				}
			}
			else{// if(!ball_hold){
				bhvShot.SetDirectShot(set_x, set_y);
				SetTargetXY(x, y, set_x, set_y, true);
				if(ParaWalkApproachXY()){//ParaWalkApproachXY()){
					mode = PARA_LAST_APPROACH;//PARA_DIRECT_SHOT;
					//SetHeadAngle(0, 0, 0, 0, 600, true);
					SetHeadMode(HEAD_TRACK);
				}
			}
			break;

		//最後の2歩を実行中
		case PARA_LAST_APPROACH:
			cnt_flag++;
			SetFaceLED(FACE_PARA_2, true);
			if( (cnt_flag > 3) && GaitMaker::IsExpiring() ){
				if(ball_hold){	
					mode = PARA_BALL_HOLD;
					//SetHeadAngle(-50, 0, 40, -25, 1000, true);
					SetHeadMode(HEAD_BALL_HOLDING, true);
					SetPresetWalk(SMALL_FORWARD_FORELEG);//, 5, true);
				}
				else{
					mode = PARA_DIRECT_SHOT;
					SetHeadMode(HEAD_TRACK);
				}
				cnt_flag = 50;
			}
			break;

		//ボールを掴むまで
		case PARA_BALL_HOLD:
			cnt_flag++;
			SetFaceLED(FACE_PARA_3, true);
			//チョコ歩きが終わり始めたらシュートへ
			if(HeadController::hold_state == HOLD_OK){
				mode = PARA_HOLD_SHOT;
				//SetHeadAngle(-65, 0, 50, -35, 200);
				cnt_flag = 100;
			}
			else if(HeadController::hold_state == HOLD_ING){
				SetPresetWalk(SMALL_FORWARD_FORELEG);
			}
			else{
				cnt_flag =0;
				mode = PARA_BALL_APPROACH;
				ReleaseCall();
			}
			/*if( (cnt_flag > 53) && GaitMaker::IsExpiring()){
				if(HeadController::hold_state == HOLD_NOT){
					cnt_flag =0;
					mode = PARA_BALL_APPROACH;
					ReleaseCall();
					return;
				}
				mode = PARA_HOLD_SHOT;
				//SetHeadAngle(-65, 0, 50, -35, 200);
				cnt_flag = 100;
			}*/
			break;

		//ボールを掴んだ後のシュート or クリア動作
		case PARA_HOLD_SHOT:
			cnt_flag++;
			//SetFaceLED(FACE_PARA_4, true);
			if(state.goalVisible)	SetFaceLED(FACE_LOVELY, true);
			else					SetFaceLED(FACE_SMILE, true);

			//確実に掴むため
			if(cnt_flag <= 105){
				/*if(!state.self.sensor.chinTouch){
					cnt_flag =0;
					mode = PARA_BALL_APPROACH;
					ReleaseCall();
				}*/
				//OSYSPRINT(("BALL HOLDING 2 : %d", cnt_flag ));
				//SetHeadMode(HEAD_BALL_HOLDING);
				//SetHeadAngle(-65, 0, 50, -35, 150);
				SetPresetWalk(BALL_HOLD);
			}
			//廻って廻って廻って廻ぁあ～るぅうぅ～
			else if(!bhvShot.HoldShot()) break;//bhvBallApproach.HoldShot()) return;
			else{
				motion_flag = true;
				cnt_flag =0;
				mode = PARA_BALL_APPROACH;
				ReleaseCall();
			}
			break;

		//ボールを直接シュート
		case PARA_DIRECT_SHOT:
			cnt_flag++;
			//SetFaceLED(FACE_PARA_4, true);
			if(state.goalVisible)	SetFaceLED(FACE_LOVELY, true);
			else					SetFaceLED(FACE_SMILE, true);
			//if( (cnt_flag > 10) && GaitMaker::IsExpiring()){//cnt_flag > 20){
				motion_flag = true;
                cnt_flag = 0;
				bhvShot.DirectShot();
				mode = PARA_BALL_APPROACH;
				ReleaseCall();
			//}
			break;
	}

	return;
}

bool BhvParaWalk::ParaWalkApproach(){
	/*
	static bool flag_dist = false;
	static bool flag_lastwalk = false;

	//ボールまでの距離・方向
	double ball_drct = state.ball.direction;
	double ball_dist = state.ball.distance - DELTA_BALL_DISTANCE;//ボールまでの距離を微調整

	//OSYSPRINT(("BALL DISTANCE = %.1f, %.1f\n", state.ball.distance, ball_dist));

	//ボールが後方にある場合
	if(fabs(ball_drct) > 90) return false;

	//ボールがどのエリア（どの方向）にあるのかのindex
	int index = (int)((ball_drct + 90.0)/10.0);
	if(index < 0)	index = 0;
	if(index > 17)	index = 17;

	//補間に用いる各種変数の算出
    double rate_t = (ball_drct - theta_under[index])/10.0;
	double rate_d = (ball_dist - dist_under[0])/50.0;
	if(rate_t > 1.0)	rate_t = 1.0;
	if(rate_t < 0.0)	rate_t = 0.0;
	if(rate_d > 1.0)	rate_d = 1.0;
	if(rate_d < 0.0)	rate_d = 0.0;

	double coe_a = (1.0-rate_t)*(1.0-rate_d);
	double coe_b = rate_t*(1.0-rate_d);
	double coe_c = rate_t*rate_d;
	double coe_d = (1.0-rate_t)*rate_d;

	//最後の接近をするためのflag立て．ボールまでの距離が5～10cm＆直前のチョコ歩行が終わりかけ
	static int cnt_flag;
	if( (rate_d < 1.0) && GaitMaker::IsExpiring()){
			flag_lastwalk = true;
	}
	else{
		//flag_dist = false;
		flag_lastwalk = false;
	}

	//各脚について，パラメータを補間
	for(int i=0;i<4;i++){
		walkparams[WALK_PARA_SET].legparam[i].x = 
			  coe_a*walkparams[index].legparam[i].x 
			+ coe_b*walkparams[index+1].legparam[i].x 
			+ coe_c*walkparams[index+19].legparam[i].x 
			+ coe_d*walkparams[index+20].legparam[i].x;

		walkparams[WALK_PARA_SET].legparam[i].y = 
			  coe_a*walkparams[index].legparam[i].y 
			+ coe_b*walkparams[index+1].legparam[i].y 
			+ coe_c*walkparams[index+19].legparam[i].y 
			+ coe_d*walkparams[index+20].legparam[i].y;

		walkparams[WALK_PARA_SET].legparam[i].z = 
			  coe_a*walkparams[index].legparam[i].z 
			+ coe_b*walkparams[index+1].legparam[i].z 
			+ coe_c*walkparams[index+19].legparam[i].z 
			+ coe_d*walkparams[index].legparam[i].z;

		walkparams[WALK_PARA_SET].legparam[i].w = 
			  coe_a*walkparams[index].legparam[i].w 
			+ coe_b*walkparams[index+1].legparam[i].w 
			+ coe_c*walkparams[index+19].legparam[i].w 
			+ coe_d*walkparams[index+20].legparam[i].w;

		walkparams[WALK_PARA_SET].legparam[i].h = 
			  coe_a*walkparams[index].legparam[i].h 
			+ coe_b*walkparams[index+1].legparam[i].h 
			+ coe_c*walkparams[index+19].legparam[i].h 
			+ coe_d*walkparams[index+20].legparam[i].h;

		walkparams[WALK_PARA_SET].legparam[i].theta = 
			  coe_a*walkparams[index].legparam[i].theta 
			+ coe_b*walkparams[index+1].legparam[i].theta 
			+ coe_c*walkparams[index+19].legparam[i].theta 
			+ coe_d*walkparams[index+20].legparam[i].theta;

		walkparams[WALK_PARA_SET].legparam[i].phi = 
			  coe_a*walkparams[index].legparam[i].phi 
			+ coe_b*walkparams[index+1].legparam[i].phi 
			+ coe_c*walkparams[index+19].legparam[i].phi 
			+ coe_d*walkparams[index+20].legparam[i].phi;
	}

	//デッドレコニング用
	walkparams[WALK_PARA_SET].move_x  = 
		  coe_a*walkparams[index].move_x 
		+ coe_b*walkparams[index+1].move_x 
		+ coe_c*walkparams[index+19].move_x 
		+ coe_d*walkparams[index+20].move_x;

	walkparams[WALK_PARA_SET].move_y  = 
		  coe_a*walkparams[index].move_y 
		+ coe_b*walkparams[index+1].move_y 
		+ coe_c*walkparams[index+19].move_y 
		+ coe_d*walkparams[index+20].move_y;

	walkparams[WALK_PARA_SET].move_tt = 
		  coe_a*walkparams[index].move_tt 
		+ coe_b*walkparams[index+1].move_tt 
		+ coe_c*walkparams[index+19].move_tt 
		+ coe_d*walkparams[index+20].move_tt;


	if(flag_lastwalk){
		GaitMaker::SetWalkParam(walkparams[WALK_PARA_SET], 2);
		//flag_dist = false;
		flag_lastwalk = false;
		ReserveCall();
		return true;
	}
	else GaitMaker::SetWalkParam(walkparams[WALK_PARA_SET], 1);

	//首関係
	static int ball_lost = 0;
	if(state.ball.losttime > 20)		SetHeadMode(HEAD_SWING_BALL_SEARCH);
	else if(state.ball.distance > 500)	SetHeadMode(HEAD_TRACK);
	else								SetHeadMode(HEAD_TRACK);//_NEAR);

	return false;//(flag_dist && flag_lastwalk);
	*/
}



bool BhvParaWalk::ParaWalkApproachXY(){
	static bool flag_dist = false;
	static bool flag_lastwalk = false;

	//目的地の距離・方向
	double set_x,set_y;
	SetTargetXY(set_x, set_y, 0, 0, false);
	double set_dist = sqrt(set_x*set_x + set_y*set_y);
	double set_drct = atan2(set_y, set_x)*180/PI;//目的地までの距離を微調整


	//目的地が後方にある場合
	if(fabs(set_drct) > 90) return false;

	//目的地がどのエリア（どの方向）にあるのかのindex
	int index = (int)((set_drct + 90.0)/10.0);
	if(index < 0)	index = 0;
	if(index > 17)	index = 17;

	//補間に用いる各種変数の算出
    double rate_t = (set_drct - theta_under[index])/10.0;
	double rate_d = (set_dist - dist_under[0])/50.0;
	if(rate_t > 1.0)	rate_t = 1.0;
	if(rate_t < 0.0)	rate_t = 0.0;
	if(rate_d > 1.0)	rate_d = 1.0;
	if(rate_d < 0.0)	rate_d = 0.0;

	double coe_a = (1.0-rate_t)*(1.0-rate_d);
	double coe_b = rate_t*(1.0-rate_d);
	double coe_c = rate_t*rate_d;
	double coe_d = (1.0-rate_t)*rate_d;

	//最後の接近をするためのflag立て．ボールまでの距離が5～10cm＆直前のチョコ歩行が終わりかけ
	static int cnt_flag;
	if( (rate_d < 1.0) && GaitMaker::IsExpiring()){
			flag_lastwalk = true;
	}
	else{
		flag_lastwalk = false;
	}

	//各脚について，パラメータを補間
	for(int i=0;i<4;i++){
		walkparams[WALK_PARA_SET].legparam[i].x = 
			  coe_a*walkparams[index].legparam[i].x 
			+ coe_b*walkparams[index+1].legparam[i].x 
			+ coe_c*walkparams[index+19].legparam[i].x 
			+ coe_d*walkparams[index+20].legparam[i].x;

		walkparams[WALK_PARA_SET].legparam[i].y = 
			  coe_a*walkparams[index].legparam[i].y 
			+ coe_b*walkparams[index+1].legparam[i].y 
			+ coe_c*walkparams[index+19].legparam[i].y 
			+ coe_d*walkparams[index+20].legparam[i].y;

		walkparams[WALK_PARA_SET].legparam[i].z = 
			  coe_a*walkparams[index].legparam[i].z 
			+ coe_b*walkparams[index+1].legparam[i].z 
			+ coe_c*walkparams[index+19].legparam[i].z 
			+ coe_d*walkparams[index].legparam[i].z;

		walkparams[WALK_PARA_SET].legparam[i].w = 
			  coe_a*walkparams[index].legparam[i].w 
			+ coe_b*walkparams[index+1].legparam[i].w 
			+ coe_c*walkparams[index+19].legparam[i].w 
			+ coe_d*walkparams[index+20].legparam[i].w;

		walkparams[WALK_PARA_SET].legparam[i].h = 
			  coe_a*walkparams[index].legparam[i].h 
			+ coe_b*walkparams[index+1].legparam[i].h 
			+ coe_c*walkparams[index+19].legparam[i].h 
			+ coe_d*walkparams[index+20].legparam[i].h;

		walkparams[WALK_PARA_SET].legparam[i].theta = 
			  coe_a*walkparams[index].legparam[i].theta 
			+ coe_b*walkparams[index+1].legparam[i].theta 
			+ coe_c*walkparams[index+19].legparam[i].theta 
			+ coe_d*walkparams[index+20].legparam[i].theta;

		walkparams[WALK_PARA_SET].legparam[i].phi = 
			  coe_a*walkparams[index].legparam[i].phi 
			+ coe_b*walkparams[index+1].legparam[i].phi 
			+ coe_c*walkparams[index+19].legparam[i].phi 
			+ coe_d*walkparams[index+20].legparam[i].phi;
	}

	//デッドレコニング用
	walkparams[WALK_PARA_SET].move_x  = 
		  coe_a*walkparams[index].move_x 
		+ coe_b*walkparams[index+1].move_x 
		+ coe_c*walkparams[index+19].move_x 
		+ coe_d*walkparams[index+20].move_x;

	walkparams[WALK_PARA_SET].move_y  = 
		  coe_a*walkparams[index].move_y 
		+ coe_b*walkparams[index+1].move_y 
		+ coe_c*walkparams[index+19].move_y 
		+ coe_d*walkparams[index+20].move_y;

	walkparams[WALK_PARA_SET].move_tt = 
		  coe_a*walkparams[index].move_tt 
		+ coe_b*walkparams[index+1].move_tt 
		+ coe_c*walkparams[index+19].move_tt 
		+ coe_d*walkparams[index+20].move_tt;


	if(flag_lastwalk){
		GaitMaker::SetWalkParam(walkparams[WALK_PARA_SET], 2);
		flag_lastwalk = false;
		ReserveCall();
		return true;
	}
	else GaitMaker::SetWalkParam(walkparams[WALK_PARA_SET], 1);

	//首関係
	SetHeadMode(HEAD_TRACK);
	//if(state.ball.losttime > 50)		SetHeadMode(HEAD_SWING_BALL_SEARCH);
	//else if(state.ball.distance > 500)	SetHeadMode(HEAD_TRACK);
	//else								SetHeadMode(HEAD_TRACK);//_NEAR);

	return false;//(flag_dist && flag_lastwalk);
}


bool BhvParaWalk::SetHoldOrDirect(bool hold_flag, bool overwrite){
	static bool return_flag = false;

	if(overwrite) return_flag = hold_flag;

    return return_flag;
}

void BhvParaWalk::SetShotMode(){
	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	int pos_t = (int)state.self.position.tt.val;

	if( state.goalVisible && fabs(state.goalDirection) < 40)	SetHoldOrDirect(false, true);
	else if(pos_x > 0)
	{
		if(abs(pos_y) <= 400)
		{
			if(pos_x > 1350)
			{
				if(abs(pos_t)<130)		SetHoldOrDirect(false,true);
				else					SetHoldOrDirect(true,true);
			}
			else
			{
				if(abs(pos_t)<80)		SetHoldOrDirect(false,true);
				else					SetHoldOrDirect(true,true);
			}
		}
		else if(pos_y > 400)
		{
			if(pos_x > 1350)
			{
				if(pos_t > 45)			SetHoldOrDirect(true,true);
				else if(pos_t > -170)	SetHoldOrDirect(false,true);
				else					SetHoldOrDirect(true,true);
			}
			else
			{
				if(pos_t > 60)			SetHoldOrDirect(true,true);
				else if(pos_t > -135)	SetHoldOrDirect(false,true);
				else					SetHoldOrDirect(true,true);
			}
		}
		else
		{
			if(pos_x >1350)
			{
				if(pos_t > 170)			SetHoldOrDirect(true,true);
				else if(pos_t > -45)	SetHoldOrDirect(false,true);
				else					SetHoldOrDirect(true,true);
			}
			else
			{
				if(pos_t > 135)			SetHoldOrDirect(true,true);
				else if(pos_t > -60)	SetHoldOrDirect(false,true);
				else					SetHoldOrDirect(true,true);
			}
		}
	}
	else SetHoldOrDirect(true,true);
	//else if( (pos_x > 0) && (abs(pos_t) < 60) )					SetHoldOrDirect(false, true);
	//else															SetHoldOrDirect(true, true);
	//if(abs(pos_t) > 90)	SetHoldOrDirect(true, true);
	//else				SetHoldOrDirect(false, true);
}

void BhvParaWalk::SetTargetXY(double &x, double &y, double target_x, double target_y, double flag_set){
	static double set_x;
	static double set_y;
	if(flag_set){
		set_x = target_x;
		set_y = target_y;
	}
	x = set_x;
	y = set_y;
}



