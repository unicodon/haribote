#include "StdAfx.h"
#include "PresetWalkParam.h"

bool PresetWalkParam::isInitialized = false;
WalkParam PresetWalkParam::walkparams[NUM_PRESET_WALKPARAM];

PresetWalkParam::PresetWalkParam(){
}

void 
PresetWalkParam::Init()
{
	if(!isInitialized){
		//WALK_STOP は必ず一番最初！！！ ID=0にする！！！
		//WALK_STOP
		walkparams[WALK_STOP].time = 300;
		walkparams[WALK_STOP].gratio = 0.24;
		walkparams[WALK_STOP].aratio = 0.24;
		walkparams[WALK_STOP].legparam[0] = LegParam(60,70,15,0,0,0,0);//(60,90,15,0,0,0,0);
		walkparams[WALK_STOP].legparam[1] = LegParam(60,70,15,0,0,0,0);//(60,90,15,0,0,0,0);
		walkparams[WALK_STOP].legparam[2] = LegParam(0,120,0,0,0,0,0);//(-5,110,10,0,0,0,0);
		walkparams[WALK_STOP].legparam[3] = LegParam(0,120,0,0,0,0,0);//(-5,110,10,0,0,0,0);
		walkparams[WALK_STOP].move_x  = 0;
		walkparams[WALK_STOP].move_y  = 0;
		walkparams[WALK_STOP].move_tt = 0;

		//SMALL_WALK
		walkparams[SMALL_FORWARD].time = 200;
		walkparams[SMALL_FORWARD].gratio = 0.45;
		walkparams[SMALL_FORWARD].aratio = 0.30;
		walkparams[SMALL_FORWARD].legparam[0] = LegParam(5,95,30,30,10,0,-13);
		walkparams[SMALL_FORWARD].legparam[1] = LegParam(5,90,30,27,10,0,-13);
		walkparams[SMALL_FORWARD].legparam[2] = LegParam(-5,110,0,30,5,0,-9);
		walkparams[SMALL_FORWARD].legparam[3] = LegParam(-5,110,0,27,5,0,-9);
		walkparams[SMALL_FORWARD].move_x  = 17.39;
		walkparams[SMALL_FORWARD].move_y  = 2.43;
		walkparams[SMALL_FORWARD].move_tt = 0.60;

		//SMALL_FORWARD_FORELEG
		walkparams[SMALL_FORWARD_FORELEG].time = 200;
		walkparams[SMALL_FORWARD_FORELEG].gratio = 0.45;
		walkparams[SMALL_FORWARD_FORELEG].aratio = 0.30;
		walkparams[SMALL_FORWARD_FORELEG].legparam[0] = LegParam(75,80,15,50,13,0,-12);
		walkparams[SMALL_FORWARD_FORELEG].legparam[1] = LegParam(75,80,15,50,13,0,-12);
		walkparams[SMALL_FORWARD_FORELEG].legparam[2] = LegParam(20,115,15,45,3,0,-17);
		walkparams[SMALL_FORWARD_FORELEG].legparam[3] = LegParam(20,115,15,45,3,0,-17);
		walkparams[SMALL_FORWARD_FORELEG].move_x  = 30.75;
		walkparams[SMALL_FORWARD_FORELEG].move_y  = 2.75;
		walkparams[SMALL_FORWARD_FORELEG].move_tt = 0.00;

		//SMALL_LEFT45_FORELEG
		walkparams[SMALL_LEFT45_FORELEG].time = 200;
		walkparams[SMALL_LEFT45_FORELEG].gratio = 0.45;
		walkparams[SMALL_LEFT45_FORELEG].aratio = 0.30;
		walkparams[SMALL_LEFT45_FORELEG].legparam[0] = LegParam(75,80,15,50,13,25,-12);
		walkparams[SMALL_LEFT45_FORELEG].legparam[1] = LegParam(75,80,15,50,13,20,-12);
		walkparams[SMALL_LEFT45_FORELEG].legparam[2] = LegParam(20,115,15,45,3,35,-17);
		walkparams[SMALL_LEFT45_FORELEG].legparam[3] = LegParam(20,115,15,45,3,35,-17);
		walkparams[SMALL_LEFT45_FORELEG].move_x  = 29.85;
		walkparams[SMALL_LEFT45_FORELEG].move_y  = 20.85;
		walkparams[SMALL_LEFT45_FORELEG].move_tt = 0.50;
		
		//SMALL_LEFT60_FORELEG
		walkparams[SMALL_LEFT60_FORELEG].time = 200;
		walkparams[SMALL_LEFT60_FORELEG].gratio = 0.45;
		walkparams[SMALL_LEFT60_FORELEG].aratio = 0.30;
		walkparams[SMALL_LEFT60_FORELEG].legparam[0] = LegParam(75,80,15,50,13,40,-12);
		walkparams[SMALL_LEFT60_FORELEG].legparam[1] = LegParam(75,80,15,50,13,45,-12);
		walkparams[SMALL_LEFT60_FORELEG].legparam[2] = LegParam(20,115,15,50,3,75,-17);
		walkparams[SMALL_LEFT60_FORELEG].legparam[3] = LegParam(20,115,15,50,3,75,-17);
		walkparams[SMALL_LEFT60_FORELEG].move_x  = 19.65;
		walkparams[SMALL_LEFT60_FORELEG].move_y  = 27.05;
		walkparams[SMALL_LEFT60_FORELEG].move_tt = 0.30;
		
		//SMALL_RIGHT45_FORELEG
		walkparams[SMALL_RIGHT45_FORELEG].time = 200;
		walkparams[SMALL_RIGHT45_FORELEG].gratio = 0.45;
		walkparams[SMALL_RIGHT45_FORELEG].aratio = 0.30;
		walkparams[SMALL_RIGHT45_FORELEG].legparam[0] = LegParam(75,80,15,50,13,-30,-12);
		walkparams[SMALL_RIGHT45_FORELEG].legparam[1] = LegParam(75,80,15,50,13,-30,-12);
		walkparams[SMALL_RIGHT45_FORELEG].legparam[2] = LegParam(20,115,15,45,3,-38,-17);
		walkparams[SMALL_RIGHT45_FORELEG].legparam[3] = LegParam(20,115,15,45,3,-38,-17);
		walkparams[SMALL_RIGHT45_FORELEG].move_x  = 22.11;
		walkparams[SMALL_RIGHT45_FORELEG].move_y  = -25.49;
		walkparams[SMALL_RIGHT45_FORELEG].move_tt = -0.30;
		
		//SMALL_RIGHT60_FORELEG
		walkparams[SMALL_RIGHT60_FORELEG].time = 200;
		walkparams[SMALL_RIGHT60_FORELEG].gratio = 0.45;
		walkparams[SMALL_RIGHT60_FORELEG].aratio = 0.30;
		walkparams[SMALL_RIGHT60_FORELEG].legparam[0] = LegParam(75,80,15,50,13,-45,-12);
		walkparams[SMALL_RIGHT60_FORELEG].legparam[1] = LegParam(75,80,15,50,13,-45,-12);
		walkparams[SMALL_RIGHT60_FORELEG].legparam[2] = LegParam(20,115,15,45,3,-70,-17);
		walkparams[SMALL_RIGHT60_FORELEG].legparam[3] = LegParam(20,115,15,45,3,-70,-17);
		walkparams[SMALL_RIGHT60_FORELEG].move_x  = 16.80;
		walkparams[SMALL_RIGHT60_FORELEG].move_y  = -32.15;
		walkparams[SMALL_RIGHT60_FORELEG].move_tt = -0.70;
		
		//BALL_HOLD
		walkparams[BALL_HOLD].time = 100;
		walkparams[BALL_HOLD].gratio = 0.45;
		walkparams[BALL_HOLD].aratio = 0.30;
		walkparams[BALL_HOLD].legparam[0] = LegParam(60,85,30,0,0,0,-11);
		walkparams[BALL_HOLD].legparam[1] = LegParam(60,85,30,0,0,0,-11);
		walkparams[BALL_HOLD].legparam[2] = LegParam(-5,110,0,0,0,0,-7);
		walkparams[BALL_HOLD].legparam[3] = LegParam(-5,110,0,0,0,0,-7);
		walkparams[BALL_HOLD].move_x  = 0;
		walkparams[BALL_HOLD].move_y  = 0;
		walkparams[BALL_HOLD].move_tt = 0;

		//BALL_FORWARD
		walkparams[BALL_FORWARD].time = 200;
		walkparams[BALL_FORWARD].gratio = 0.33;
		walkparams[BALL_FORWARD].aratio = 0.26;
		walkparams[BALL_FORWARD].legparam[0] = LegParam(70,90,10,50,10,0,-12);
		walkparams[BALL_FORWARD].legparam[1] = LegParam(70,90,10,50,10,0,-12);
		walkparams[BALL_FORWARD].legparam[2] = LegParam(15,105,0,50,10,0,-17);
		walkparams[BALL_FORWARD].legparam[3] = LegParam(15,105,0,50,10,0,-17);
		walkparams[BALL_HOLD].move_x  = 39.02;
		walkparams[BALL_HOLD].move_y  = 0.89;
		walkparams[BALL_HOLD].move_tt = 0.20;
		
		//BALL_LEFT90
		walkparams[BALL_LEFT90].time = 200;
		walkparams[BALL_LEFT90].gratio = 0.33;
		walkparams[BALL_LEFT90].aratio = 0.26;
		walkparams[BALL_LEFT90].legparam[0] = LegParam(65,90,10,50,10,90,-12);
		walkparams[BALL_LEFT90].legparam[1] = LegParam(65,90,10,50,10,78,-12);
		walkparams[BALL_LEFT90].legparam[2] = LegParam(15,105,0,50,10,90,-12);
		walkparams[BALL_LEFT90].legparam[3] = LegParam(15,105,0,50,10,90,-12);
		walkparams[BALL_HOLD].move_x  = -4.98;
		walkparams[BALL_HOLD].move_y  = 50.36;
		walkparams[BALL_HOLD].move_tt = -0.50;
		
		//BALL_RIGHT90
		walkparams[BALL_RIGHT90].time = 200;
		walkparams[BALL_RIGHT90].gratio = 0.33;
		walkparams[BALL_RIGHT90].aratio = 0.26;
		walkparams[BALL_RIGHT90].legparam[0] = LegParam(65,90,10,50,10,-84,-12);
		walkparams[BALL_RIGHT90].legparam[1] = LegParam(65,90,10,50,10,-90,-12);
		walkparams[BALL_RIGHT90].legparam[2] = LegParam(15,105,0,50,10,-90,-12);
		walkparams[BALL_RIGHT90].legparam[3] = LegParam(15,105,0,50,10,-90,-12);
		walkparams[BALL_HOLD].move_x  = -9.40;
		walkparams[BALL_HOLD].move_y  = -51.43;
		walkparams[BALL_HOLD].move_tt = 0.10;
		
		//BALL_ROTATE_LEFT
		walkparams[BALL_ROTATE_LEFT].time = 300;
		walkparams[BALL_ROTATE_LEFT].gratio = 0.40;
		walkparams[BALL_ROTATE_LEFT].aratio = 0.25;
		walkparams[BALL_ROTATE_LEFT].legparam[0] = LegParam(65,90,15,30,10,60,-7);
		walkparams[BALL_ROTATE_LEFT].legparam[1] = LegParam(65,90,15,30,10,125,-7);
		walkparams[BALL_ROTATE_LEFT].legparam[2] = LegParam(-5,110,15,50,10,-60,-7);
		walkparams[BALL_ROTATE_LEFT].legparam[3] = LegParam(-5,110,15,50,10,-120,-7);
		walkparams[BALL_ROTATE_LEFT].move_x  = 0.49;
		walkparams[BALL_ROTATE_LEFT].move_y  = 53.98;
		walkparams[BALL_ROTATE_LEFT].move_tt = 21.25;

		//BALL_ROTATE_LEFT180
		walkparams[BALL_ROTATE_LEFT180].time = 200;
		walkparams[BALL_ROTATE_LEFT180].gratio = 0.40;
		walkparams[BALL_ROTATE_LEFT180].aratio = 0.25;
		walkparams[BALL_ROTATE_LEFT180].legparam[0] = LegParam(65,90,15,45,10,60,-10);
		walkparams[BALL_ROTATE_LEFT180].legparam[1] = LegParam(65,90,15,45,10,125,-10);
		walkparams[BALL_ROTATE_LEFT180].legparam[2] = LegParam(-5,105,0,50,10,-60,-10);
		walkparams[BALL_ROTATE_LEFT180].legparam[3] = LegParam(-5,105,0,50,10,-120,-10);
		walkparams[BALL_ROTATE_LEFT180].move_x  = -3.57;
		walkparams[BALL_ROTATE_LEFT180].move_y  = 10.16;
		walkparams[BALL_ROTATE_LEFT180].move_tt = 17.50;
/*
		walkparams[BALL_ROTATE_LEFT].time = 250;
		walkparams[BALL_ROTATE_LEFT].gratio = 0.40;
		walkparams[BALL_ROTATE_LEFT].aratio = 0.25;
		walkparams[BALL_ROTATE_LEFT].legparam[0] = LegParam(60,90,15,86,10,60,-18);
		walkparams[BALL_ROTATE_LEFT].legparam[1] = LegParam(60,90,15,90,10,120,-12);
		walkparams[BALL_ROTATE_LEFT].legparam[2] = LegParam(-5,110,10,86,13,-60,-12);
		walkparams[BALL_ROTATE_LEFT].legparam[3] = LegParam(-5,110,10,86,13,-120,-12);
                walkparams[BALL_ROTATE_LEFT].move_x  = -46.04;
		walkparams[BALL_ROTATE_LEFT].move_y  = 90.39;
		walkparams[BALL_ROTATE_LEFT].move_tt = 49.50;
		*/

//BALL_ROTATE_RIGHT
		walkparams[BALL_ROTATE_RIGHT].time = 300;
		walkparams[BALL_ROTATE_RIGHT].gratio = 0.40;
		walkparams[BALL_ROTATE_RIGHT].aratio = 0.25;
		walkparams[BALL_ROTATE_RIGHT].legparam[0] = LegParam(65,95,15,30,10,-120,-10);
		walkparams[BALL_ROTATE_RIGHT].legparam[1] = LegParam(65,95,15,30,10,-60,-10);
		walkparams[BALL_ROTATE_RIGHT].legparam[2] = LegParam(-5,110,15,50,10,120,-10);
		walkparams[BALL_ROTATE_RIGHT].legparam[3] = LegParam(-5,110,15,50,10,60,-10);
		walkparams[BALL_ROTATE_RIGHT].move_x  = 0.63;
		walkparams[BALL_ROTATE_RIGHT].move_y  = -39.19;
		walkparams[BALL_ROTATE_RIGHT].move_tt = -20.50;
		
//BALL_ROTATE_RIGHT180
		walkparams[BALL_ROTATE_RIGHT180].time = 200;
		walkparams[BALL_ROTATE_RIGHT180].gratio = 0.40;
		walkparams[BALL_ROTATE_RIGHT180].aratio = 0.25;
		walkparams[BALL_ROTATE_RIGHT180].legparam[0] = LegParam(65,90,15,45,10,-120,-10);
		walkparams[BALL_ROTATE_RIGHT180].legparam[1] = LegParam(65,90,15,45,10,-60,-10);
		walkparams[BALL_ROTATE_RIGHT180].legparam[2] = LegParam(-5,105,0,50,10,120,-10);
		walkparams[BALL_ROTATE_RIGHT180].legparam[3] = LegParam(-5,105,0,50,10,60,-10);
		walkparams[BALL_ROTATE_RIGHT180].move_x  = -11.07;
		walkparams[BALL_ROTATE_RIGHT180].move_y  = -13.49;
		walkparams[BALL_ROTATE_RIGHT180].move_tt = -16.00;
/*
		walkparams[BALL_ROTATE_RIGHT].time = 250;
		walkparams[BALL_ROTATE_RIGHT].gratio = 0.40;
		walkparams[BALL_ROTATE_RIGHT].aratio = 0.25;
		walkparams[BALL_ROTATE_RIGHT].legparam[0] = LegParam(60,90,15,89,10,-121,-13);
		walkparams[BALL_ROTATE_RIGHT].legparam[1] = LegParam(60,90,15,86,10,-60,-16);
		walkparams[BALL_ROTATE_RIGHT].legparam[2] = LegParam(-5,110,10,86,13,120,-11);
		walkparams[BALL_ROTATE_RIGHT].legparam[3] = LegParam(-5,110,10,86,13,60,-11);
		walkparams[BALL_ROTATE_RIGHT].move_x  = -24.08;
		walkparams[BALL_ROTATE_RIGHT].move_y  = -83.80;
		walkparams[BALL_ROTATE_RIGHT].move_tt = -47.90;
		*/

//BALL_ROTATE1_LEFT
		walkparams[BALL_ROTATE1_LEFT].time = 250;
		walkparams[BALL_ROTATE1_LEFT].gratio = 0.40;
		walkparams[BALL_ROTATE1_LEFT].aratio = 0.25;
		walkparams[BALL_ROTATE1_LEFT].legparam[0] = LegParam(60,90,15,86,10,60,-18);
		walkparams[BALL_ROTATE1_LEFT].legparam[1] = LegParam(60,90,15,90,10,120,-12);
		walkparams[BALL_ROTATE1_LEFT].legparam[2] = LegParam(-5,110,10,86,13,-60,-12);
		walkparams[BALL_ROTATE1_LEFT].legparam[3] = LegParam(-5,110,10,86,13,-120,-12);
		walkparams[BALL_ROTATE1_LEFT].move_x  = -25.54;
		walkparams[BALL_ROTATE1_LEFT].move_y  = 21.46;
		walkparams[BALL_ROTATE1_LEFT].move_tt = 11.65;

//BALL_ROTATE1_RIGHT
		walkparams[BALL_ROTATE1_RIGHT].time = 250;
		walkparams[BALL_ROTATE1_RIGHT].gratio = 0.40;
		walkparams[BALL_ROTATE1_RIGHT].aratio = 0.25;
		walkparams[BALL_ROTATE1_RIGHT].legparam[0] = LegParam(60,90,15,89,10,-121,-13);
		walkparams[BALL_ROTATE1_RIGHT].legparam[1] = LegParam(60,90,15,86,10,-60,-16);
		walkparams[BALL_ROTATE1_RIGHT].legparam[2] = LegParam(-5,110,10,86,13,120,-11);
		walkparams[BALL_ROTATE1_RIGHT].legparam[3] = LegParam(-5,110,10,86,13,60,-11);
		walkparams[BALL_ROTATE1_RIGHT].move_x  = -9.66;
		walkparams[BALL_ROTATE1_RIGHT].move_y  = -24.98;
		walkparams[BALL_ROTATE1_RIGHT].move_tt = -13.20;

//ここから歩行調整
		//WALK_FORWARD
		walkparams[WALK_FORWARD].time = 300;
		walkparams[WALK_FORWARD].gratio = 0.35;
		walkparams[WALK_FORWARD].aratio = 0.27;
		walkparams[WALK_FORWARD].legparam[0] = LegParam(5,95,30,80,10,0,-10);
		walkparams[WALK_FORWARD].legparam[1] = LegParam(5,95,30,80,10,0,-10);
		walkparams[WALK_FORWARD].legparam[2] = LegParam(-5,110,0,80,5,0,-10);
		walkparams[WALK_FORWARD].legparam[3] = LegParam(-5,110,0,80,5,0,-10);
		walkparams[WALK_FORWARD].move_x  = 139.77 ;
		walkparams[WALK_FORWARD].move_y  = 11.96 ;
		walkparams[WALK_FORWARD].move_tt = -0.30 ;

//WALK_BACK
		walkparams[WALK_BACK].time = 300;
		walkparams[WALK_BACK].gratio = 0.33;
		walkparams[WALK_BACK].aratio = 0.26;
		walkparams[WALK_BACK].legparam[0] = LegParam(25,95,30,80,10,180,-6);
		walkparams[WALK_BACK].legparam[1] = LegParam(25,95,30,80,10,180,-6);
		walkparams[WALK_BACK].legparam[2] = LegParam(0,110,0,50,10,180,-6);
		walkparams[WALK_BACK].legparam[3] = LegParam(0,110,0,50,10,180,-6);
		walkparams[WALK_BACK].move_x  = -104.87 ;
		walkparams[WALK_BACK].move_y  = 6.22 ;
		walkparams[WALK_BACK].move_tt = 0.63 ;

//WALK_LEFT15
		walkparams[WALK_LEFT15].time = 300;
		walkparams[WALK_LEFT15].gratio = 0.45;
		walkparams[WALK_LEFT15].aratio = 0.25;
		walkparams[WALK_LEFT15].legparam[0] = LegParam(10,95,30,80,15,15,-11);
		walkparams[WALK_LEFT15].legparam[1] = LegParam(10,95,30,80,15,15,-11);
		walkparams[WALK_LEFT15].legparam[2] = LegParam(-5,110,0,80,5,18,-7);
		walkparams[WALK_LEFT15].legparam[3] = LegParam(-5,110,0,80,5,18,-7);
		walkparams[WALK_LEFT15].move_x  = 132.11 ;
		walkparams[WALK_LEFT15].move_y  = 27.13 ;
		walkparams[WALK_LEFT15].move_tt = -1.00 ;

//WALK_LEFT30
		walkparams[WALK_LEFT30].time = 300;
		walkparams[WALK_LEFT30].gratio = 0.45;
		walkparams[WALK_LEFT30].aratio = 0.25;
		walkparams[WALK_LEFT30].legparam[0] = LegParam(10,95,30,85,15,30,-11);
		walkparams[WALK_LEFT30].legparam[1] = LegParam(10,95,30,85,15,30,-11);
		walkparams[WALK_LEFT30].legparam[2] = LegParam(-5,110,0,85,5,30,-7);
		walkparams[WALK_LEFT30].legparam[3] = LegParam(-5,110,0,85,5,30,-7);
		walkparams[WALK_LEFT30].move_x  = 122.00;
		walkparams[WALK_LEFT30].move_y  = 67.67;
		walkparams[WALK_LEFT30].move_tt = 0.00;

//WALK_LEFT60
		walkparams[WALK_LEFT60].time = 300;
		walkparams[WALK_LEFT60].gratio = 0.33;
		walkparams[WALK_LEFT60].aratio = 0.26;
		walkparams[WALK_LEFT60].legparam[0] = LegParam(20,100,15,70,10,65,-12);
		walkparams[WALK_LEFT60].legparam[1] = LegParam(20,100,15,70,10,65,-12);
		walkparams[WALK_LEFT60].legparam[2] = LegParam(15,110,15,70,10,65,-12);
		walkparams[WALK_LEFT60].legparam[3] = LegParam(15,110,15,70,10,65,-12);
		walkparams[WALK_LEFT60].move_x  = 66.22;
		walkparams[WALK_LEFT60].move_y  = 98.86;
		walkparams[WALK_LEFT60].move_tt = -0.10;

//WALK_LEFT90
		walkparams[WALK_LEFT90].time = 300;
		walkparams[WALK_LEFT90].gratio = 0.33;
		walkparams[WALK_LEFT90].aratio = 0.26;
		walkparams[WALK_LEFT90].legparam[0] = LegParam(35,100,10,70,10,90,-12);
		walkparams[WALK_LEFT90].legparam[1] = LegParam(35,100,10,70,10,80,-12);
		walkparams[WALK_LEFT90].legparam[2] = LegParam(15,110,10,75,10,90,-12);
		walkparams[WALK_LEFT90].legparam[3] = LegParam(15,110,10,75,10,90,-12);
		walkparams[WALK_LEFT90].move_x  = 7.26;
		walkparams[WALK_LEFT90].move_y  = 117.84;
		walkparams[WALK_LEFT90].move_tt = 0.57;

//WALK_LEFT120
		walkparams[WALK_LEFT120].time = 300;
		walkparams[WALK_LEFT120].gratio = 0.33;
		walkparams[WALK_LEFT120].aratio = 0.26;
		walkparams[WALK_LEFT120].legparam[0] = LegParam(35,100,10,70,10,116,-12);
		walkparams[WALK_LEFT120].legparam[1] = LegParam(35,100,10,70,10,116,-12);
		walkparams[WALK_LEFT120].legparam[2] = LegParam(15,110,10,70,10,96,-12);
		walkparams[WALK_LEFT120].legparam[3] = LegParam(15,110,10,70,10,113,-12);
		walkparams[WALK_LEFT120].move_x  = -40.23;
		walkparams[WALK_LEFT120].move_y  = 100.42;
		walkparams[WALK_LEFT120].move_tt = 0.70;

//WALK_LEFT150
		walkparams[WALK_LEFT150].time = 300;
		walkparams[WALK_LEFT150].gratio = 0.33;
		walkparams[WALK_LEFT150].aratio = 0.26;
		walkparams[WALK_LEFT150].legparam[0] = LegParam(35,95,10,75,5,145,-3);
		walkparams[WALK_LEFT150].legparam[1] = LegParam(35,85,10,75,5,145,-3);
		walkparams[WALK_LEFT150].legparam[2] = LegParam(15,105,15,85,5,145,-5);
		walkparams[WALK_LEFT150].legparam[3] = LegParam(15,105,15,85,5,145,-5);
		walkparams[WALK_LEFT150].move_x  = -107.42;
		walkparams[WALK_LEFT150].move_y  = 85.28;
		walkparams[WALK_LEFT150].move_tt = 0.83;

//WALK_RIGHT15
		walkparams[WALK_RIGHT15].time = 300;
		walkparams[WALK_RIGHT15].gratio = 0.45;
		walkparams[WALK_RIGHT15].aratio = 0.25;
		walkparams[WALK_RIGHT15].legparam[0] = LegParam(5,95,30,90,10,-14,-11);
		walkparams[WALK_RIGHT15].legparam[1] = LegParam(5,95,30,90,10,-10,-11);
		walkparams[WALK_RIGHT15].legparam[2] = LegParam(-5,110,0,90,5,-10,-7);
		walkparams[WALK_RIGHT15].legparam[3] = LegParam(-5,110,0,90,5,-10,-7);
		walkparams[WALK_RIGHT15].move_x  = 131.99;
		walkparams[WALK_RIGHT15].move_y  = -45.85;
		walkparams[WALK_RIGHT15].move_tt = -0.67;

//WALK_RIGHT30
		walkparams[WALK_RIGHT30].time = 300;
		walkparams[WALK_RIGHT30].gratio = 0.45;
		walkparams[WALK_RIGHT30].aratio = 0.25;
		walkparams[WALK_RIGHT30].legparam[0] = LegParam(5,95,30,90,15,-26,-11);
		walkparams[WALK_RIGHT30].legparam[1] = LegParam(5,95,30,90,15,-26,-11);
		walkparams[WALK_RIGHT30].legparam[2] = LegParam(-5,110,0,90,5,-26,-7);
		walkparams[WALK_RIGHT30].legparam[3] = LegParam(-5,110,0,90,5,-26,-7);
		walkparams[WALK_RIGHT30].move_x  = 126.80;
		walkparams[WALK_RIGHT30].move_y  = -77.61;
		walkparams[WALK_RIGHT30].move_tt = 1.00;

//WALK_RIGHT60
		walkparams[WALK_RIGHT60].time = 300;
		walkparams[WALK_RIGHT60].gratio = 0.33;
		walkparams[WALK_RIGHT60].aratio = 0.26;
		walkparams[WALK_RIGHT60].legparam[0] = LegParam(20,100,15,80,13,-50,-12);
		walkparams[WALK_RIGHT60].legparam[1] = LegParam(20,100,15,80,10,-55,-12);
		walkparams[WALK_RIGHT60].legparam[2] = LegParam(15,110,15,80,10,-55,-12);
		walkparams[WALK_RIGHT60].legparam[3] = LegParam(15,110,15,80,10,-60,-12);
		walkparams[WALK_RIGHT60].move_x  = 75.90;
		walkparams[WALK_RIGHT60].move_y  = -112.84;
		walkparams[WALK_RIGHT60].move_tt = -0.43;

//WALK_RIGHT90
		walkparams[WALK_RIGHT90].time = 300;
		walkparams[WALK_RIGHT90].gratio = 0.33;
		walkparams[WALK_RIGHT90].aratio = 0.26;
		walkparams[WALK_RIGHT90].legparam[0] = LegParam(35,100,10,70,10,-85,-12);
		walkparams[WALK_RIGHT90].legparam[1] = LegParam(35,100,10,70,10,-90,-12);
		walkparams[WALK_RIGHT90].legparam[2] = LegParam(15,110,10,70,10,-90,-12);
		walkparams[WALK_RIGHT90].legparam[3] = LegParam(15,110,10,70,10,-90,-12);
		walkparams[WALK_RIGHT90].move_x  = -9.65;
		walkparams[WALK_RIGHT90].move_y  = -112.15;
		walkparams[WALK_RIGHT90].move_tt = 0.30;

//WALK_RIGHT120
		walkparams[WALK_RIGHT120].time = 300;
		walkparams[WALK_RIGHT120].gratio = 0.33;
		walkparams[WALK_RIGHT120].aratio = 0.26;
		walkparams[WALK_RIGHT120].legparam[0] = LegParam(35,100,10,70,10,-103,-12);
		walkparams[WALK_RIGHT120].legparam[1] = LegParam(35,100,10,70,10,-105,-12);
		walkparams[WALK_RIGHT120].legparam[2] = LegParam(15,110,15,70,10,-110,-12);
		walkparams[WALK_RIGHT120].legparam[3] = LegParam(15,110,15,70,10,-90,-12);
		walkparams[WALK_RIGHT120].move_x  = -47.22;
		walkparams[WALK_RIGHT120].move_y  = -104.50;
		walkparams[WALK_RIGHT120].move_tt = -0.47;

//WALK_RIGHT150
		walkparams[WALK_RIGHT150].time = 300;
		walkparams[WALK_RIGHT150].gratio = 0.33;
		walkparams[WALK_RIGHT150].aratio = 0.26;
		walkparams[WALK_RIGHT150].legparam[0] = LegParam(35,95,10,75,5,-145,-3);
		walkparams[WALK_RIGHT150].legparam[1] = LegParam(35,85,10,75,5,-150,-3);
		walkparams[WALK_RIGHT150].legparam[2] = LegParam(15,105,15,85,5,-160,-5);
		walkparams[WALK_RIGHT150].legparam[3] = LegParam(15,105,15,85,5,-160,-5);
		walkparams[WALK_RIGHT150].move_x  = -122.18;
		walkparams[WALK_RIGHT150].move_y  = -52.76;
		walkparams[WALK_RIGHT150].move_tt = 0.20;


//WALK_ROLL_LEFT10
		walkparams[WALK_ROLL_LEFT10].time = 300;
		walkparams[WALK_ROLL_LEFT10].gratio = 0.40;
		walkparams[WALK_ROLL_LEFT10].aratio = 0.25;
		walkparams[WALK_ROLL_LEFT10].legparam[0] = LegParam(25,90,10,60,5,125,-10);
		walkparams[WALK_ROLL_LEFT10].legparam[1] = LegParam(25,90,10,60,5,78,-10);
		walkparams[WALK_ROLL_LEFT10].legparam[2] = LegParam(-10,110,10,80,7,130,-12);
		walkparams[WALK_ROLL_LEFT10].legparam[3] = LegParam(-10,110,10,80,7,70,-12);
		walkparams[WALK_ROLL_LEFT10].move_x  = -4.17;
		walkparams[WALK_ROLL_LEFT10].move_y  = 50.77;
		walkparams[WALK_ROLL_LEFT10].move_tt = -26.17;

//WALK_ROLL_LEFT20
		walkparams[WALK_ROLL_LEFT20].time = 300;
		walkparams[WALK_ROLL_LEFT20].gratio = 0.40;
		walkparams[WALK_ROLL_LEFT20].aratio = 0.25;
		walkparams[WALK_ROLL_LEFT20].legparam[0] = LegParam(15,90,10,60,5,88,-10);
		walkparams[WALK_ROLL_LEFT20].legparam[1] = LegParam(15,90,10,60,5,96,-10);
		walkparams[WALK_ROLL_LEFT20].legparam[2] = LegParam(-10,110,10,80,9,130,-10);
		walkparams[WALK_ROLL_LEFT20].legparam[3] = LegParam(-10,110,10,80,9,80,-10);
		walkparams[WALK_ROLL_LEFT20].move_x  = 0.48;
		walkparams[WALK_ROLL_LEFT20].move_y  = 67.19;
		walkparams[WALK_ROLL_LEFT20].move_tt = -18.25;


//WALK_ROLL_LEFT40
		walkparams[WALK_ROLL_LEFT40].time = 300;
		walkparams[WALK_ROLL_LEFT40].gratio = 0.40;
		walkparams[WALK_ROLL_LEFT40].aratio = 0.25;
		walkparams[WALK_ROLL_LEFT40].legparam[0] = LegParam(15,90,15,60,5,85,-10);
		walkparams[WALK_ROLL_LEFT40].legparam[1] = LegParam(15,90,15,60,5,70,-10);
		walkparams[WALK_ROLL_LEFT40].legparam[2] = LegParam(-10,110,10,80,7,115,-10);
		walkparams[WALK_ROLL_LEFT40].legparam[3] = LegParam(-10,110,10,80,7,110,-10);
		walkparams[WALK_ROLL_LEFT40].move_x  = 6.84;
		walkparams[WALK_ROLL_LEFT40].move_y  = 72.00;
		walkparams[WALK_ROLL_LEFT40].move_tt = -10.00;

//WALK_ROLL_LEFT80
		walkparams[WALK_ROLL_LEFT80].time = 300;
		walkparams[WALK_ROLL_LEFT80].gratio = 0.33;
		walkparams[WALK_ROLL_LEFT80].aratio = 0.26;
		walkparams[WALK_ROLL_LEFT80].legparam[0] = LegParam(35,100,10,70,10,100,-12);
		walkparams[WALK_ROLL_LEFT80].legparam[1] = LegParam(35,100,10,70,10,85,-12);
		walkparams[WALK_ROLL_LEFT80].legparam[2] = LegParam(15,110,10,70,10,90,-12);
		walkparams[WALK_ROLL_LEFT80].legparam[3] = LegParam(15,110,10,70,10,80,-12);
		walkparams[WALK_ROLL_LEFT80].move_x  = 9.83;
		walkparams[WALK_ROLL_LEFT80].move_y  = 105.12;
		walkparams[WALK_ROLL_LEFT80].move_tt = -4.00;

//WALK_ROLL_RIGHT10
		walkparams[WALK_ROLL_RIGHT10].time = 300;
		walkparams[WALK_ROLL_RIGHT10].gratio = 0.40;
		walkparams[WALK_ROLL_RIGHT10].aratio = 0.25;
		walkparams[WALK_ROLL_RIGHT10].legparam[0] = LegParam(25,90,10,60,5,-70,-10);
		walkparams[WALK_ROLL_RIGHT10].legparam[1] = LegParam(25,90,10,60,5,-125,-10);
		walkparams[WALK_ROLL_RIGHT10].legparam[2] = LegParam(-10,110,10,80,7,-70,-10);
		walkparams[WALK_ROLL_RIGHT10].legparam[3] = LegParam(-10,110,10,80,7,-130,-10);
		walkparams[WALK_ROLL_RIGHT10].move_x  = -4.22;
		walkparams[WALK_ROLL_RIGHT10].move_y  = -69.45;
		walkparams[WALK_ROLL_RIGHT10].move_tt = 31.75;

//WALK_ROLL_RIGHT20
		walkparams[WALK_ROLL_RIGHT20].time = 300;
		walkparams[WALK_ROLL_RIGHT20].gratio = 0.40;
		walkparams[WALK_ROLL_RIGHT20].aratio = 0.25;
		walkparams[WALK_ROLL_RIGHT20].legparam[0] = LegParam(15,90,10,60,5,-90,-10);
		walkparams[WALK_ROLL_RIGHT20].legparam[1] = LegParam(15,90,10,60,5,-88,-10);
		walkparams[WALK_ROLL_RIGHT20].legparam[2] = LegParam(-10,110,10,80,7,-80,-12);
		walkparams[WALK_ROLL_RIGHT20].legparam[3] = LegParam(-10,110,10,80,7,-130,-12);
		walkparams[WALK_ROLL_RIGHT20].move_x  = 7.99;
		walkparams[WALK_ROLL_RIGHT20].move_y  = -66.86;
		walkparams[WALK_ROLL_RIGHT20].move_tt = 21.00;


//WALK_ROLL_RIGHT40
		walkparams[WALK_ROLL_RIGHT40].time = 300;
		walkparams[WALK_ROLL_RIGHT40].gratio = 0.40;
		walkparams[WALK_ROLL_RIGHT40].aratio = 0.25;
		walkparams[WALK_ROLL_RIGHT40].legparam[0] = LegParam(15,90,15,60,5,-70,-10);
		walkparams[WALK_ROLL_RIGHT40].legparam[1] = LegParam(15,90,15,60,5,-85,-10);
		walkparams[WALK_ROLL_RIGHT40].legparam[2] = LegParam(-10,110,10,80,7,-110,-12);
		walkparams[WALK_ROLL_RIGHT40].legparam[3] = LegParam(-10,110,10,80,7,-115,-12);
		walkparams[WALK_ROLL_RIGHT40].move_x  = -5.34;
		walkparams[WALK_ROLL_RIGHT40].move_y  = -74.05;
		walkparams[WALK_ROLL_RIGHT40].move_tt = 9.75;

//WALK_ROLL_RIGHT80
		walkparams[WALK_ROLL_RIGHT80].time = 300;
		walkparams[WALK_ROLL_RIGHT80].gratio = 0.33;
		walkparams[WALK_ROLL_RIGHT80].aratio = 0.26;
		walkparams[WALK_ROLL_RIGHT80].legparam[0] = LegParam(35,100,10,70,10,-83,-12);
		walkparams[WALK_ROLL_RIGHT80].legparam[1] = LegParam(35,100,10,70,10,-100,-12);
		walkparams[WALK_ROLL_RIGHT80].legparam[2] = LegParam(15,110,10,70,10,-80,-12);
		walkparams[WALK_ROLL_RIGHT80].legparam[3] = LegParam(15,110,10,70,10,-90,-12);
		walkparams[WALK_ROLL_RIGHT80].move_x  = -6.36;
		walkparams[WALK_ROLL_RIGHT80].move_y  = -100.41;
		walkparams[WALK_ROLL_RIGHT80].move_tt = 7.50;

//WALK_ROTATE_LEFT90
		walkparams[WALK_ROTATE_LEFT90].time = 300;
		walkparams[WALK_ROTATE_LEFT90].gratio = 0.40;
		walkparams[WALK_ROTATE_LEFT90].aratio = 0.25;
		walkparams[WALK_ROTATE_LEFT90].legparam[0] = LegParam(35,90,15,10,10,60,-10);
		walkparams[WALK_ROTATE_LEFT90].legparam[1] = LegParam(35,90,15,10,10,125,-10);
		walkparams[WALK_ROTATE_LEFT90].legparam[2] = LegParam(-5,110,10,30,10,-60,-10);
		walkparams[WALK_ROTATE_LEFT90].legparam[3] = LegParam(-5,110,10,30,10,-120,-10);
		walkparams[WALK_ROTATE_LEFT90].move_x  = 9.37;
		walkparams[WALK_ROTATE_LEFT90].move_y  = 1.59;
		walkparams[WALK_ROTATE_LEFT90].move_tt = 11.25;

//WALK_ROTATE_LEFT180
		walkparams[WALK_ROTATE_LEFT180].time = 300;
		walkparams[WALK_ROTATE_LEFT180].gratio = 0.40;
		walkparams[WALK_ROTATE_LEFT180].aratio = 0.25;
		walkparams[WALK_ROTATE_LEFT180].legparam[0] = LegParam(35,90,15,30,10,60,-10);
		walkparams[WALK_ROTATE_LEFT180].legparam[1] = LegParam(35,90,15,30,10,125,-10);
		walkparams[WALK_ROTATE_LEFT180].legparam[2] = LegParam(-5,110,10,30,10,-60,-10);
		walkparams[WALK_ROTATE_LEFT180].legparam[3] = LegParam(-5,110,10,30,10,-120,-10);
		walkparams[WALK_ROTATE_LEFT180].move_x  = 0.12;
		walkparams[WALK_ROTATE_LEFT180].move_y  = 26.70;
		walkparams[WALK_ROTATE_LEFT180].move_tt = 22.00;

//WALK_ROTATE_LEFT270
		walkparams[WALK_ROTATE_LEFT270].time = 300;
		walkparams[WALK_ROTATE_LEFT270].gratio = 0.40;
		walkparams[WALK_ROTATE_LEFT270].aratio = 0.25;
		walkparams[WALK_ROTATE_LEFT270].legparam[0] = LegParam(35,90,15,30,10,60,-7);
		walkparams[WALK_ROTATE_LEFT270].legparam[1] = LegParam(35,90,15,30,10,125,-7);
		walkparams[WALK_ROTATE_LEFT270].legparam[2] = LegParam(-5,110,15,50,10,-60,-7);
		walkparams[WALK_ROTATE_LEFT270].legparam[3] = LegParam(-5,110,15,50,10,-120,-7);
		walkparams[WALK_ROTATE_LEFT270].move_x  = 2.07;
		walkparams[WALK_ROTATE_LEFT270].move_y  = 19.44;
		walkparams[WALK_ROTATE_LEFT270].move_tt = 29.25;

//WALK_ROTATE_RIGHT90
		walkparams[WALK_ROTATE_RIGHT90].time = 300;
		walkparams[WALK_ROTATE_RIGHT90].gratio = 0.40;
		walkparams[WALK_ROTATE_RIGHT90].aratio = 0.25;
		walkparams[WALK_ROTATE_RIGHT90].legparam[0] = LegParam(35,95,15,15,10,-120,-10);
		walkparams[WALK_ROTATE_RIGHT90].legparam[1] = LegParam(35,90,15,15,10,-60,-10);
		walkparams[WALK_ROTATE_RIGHT90].legparam[2] = LegParam(-5,110,10,25,10,120,-10);
		walkparams[WALK_ROTATE_RIGHT90].legparam[3] = LegParam(-5,110,10,25,10,60,-10);
		walkparams[WALK_ROTATE_RIGHT90].move_x  = 2.91;
		walkparams[WALK_ROTATE_RIGHT90].move_y  = -9.04;
		walkparams[WALK_ROTATE_RIGHT90].move_tt = -13.17;

//WALK_ROTATE_RIGHT180
		walkparams[WALK_ROTATE_RIGHT180].time = 300;
		walkparams[WALK_ROTATE_RIGHT180].gratio = 0.40;
		walkparams[WALK_ROTATE_RIGHT180].aratio = 0.25;
		walkparams[WALK_ROTATE_RIGHT180].legparam[0] = LegParam(35,95,15,30,10,-120,-10);
		walkparams[WALK_ROTATE_RIGHT180].legparam[1] = LegParam(35,95,15,30,10,-60,-10);
		walkparams[WALK_ROTATE_RIGHT180].legparam[2] = LegParam(-5,110,15,30,10,120,-10);
		walkparams[WALK_ROTATE_RIGHT180].legparam[3] = LegParam(-5,110,15,30,10,60,-10);
		walkparams[WALK_ROTATE_RIGHT180].move_x  = -11.18;
		walkparams[WALK_ROTATE_RIGHT180].move_y  = -20.27;
		walkparams[WALK_ROTATE_RIGHT180].move_tt = -22.00;

//WALK_ROTATE_RIGHT270
		walkparams[WALK_ROTATE_RIGHT270].time = 300;
		walkparams[WALK_ROTATE_RIGHT270].gratio = 0.40;
		walkparams[WALK_ROTATE_RIGHT270].aratio = 0.25;
		walkparams[WALK_ROTATE_RIGHT270].legparam[0] = LegParam(35,95,15,30,10,-120,-10);
		walkparams[WALK_ROTATE_RIGHT270].legparam[1] = LegParam(35,95,15,30,10,-60,-10);
		walkparams[WALK_ROTATE_RIGHT270].legparam[2] = LegParam(-5,110,15,50,10,120,-10);
		walkparams[WALK_ROTATE_RIGHT270].legparam[3] = LegParam(-5,110,15,50,10,60,-10);
		walkparams[WALK_ROTATE_RIGHT270].move_x  = 1.11;
		walkparams[WALK_ROTATE_RIGHT270].move_y  = -28.17;
		walkparams[WALK_ROTATE_RIGHT270].move_tt = -30.50;

//WALK_TURN_LEFT20
		walkparams[WALK_TURN_LEFT20].time = 300;
		walkparams[WALK_TURN_LEFT20].gratio = 0.45;
		walkparams[WALK_TURN_LEFT20].aratio = 0.25;
		walkparams[WALK_TURN_LEFT20].legparam[0] = LegParam(5,95,30,80,5,21,-12);
		walkparams[WALK_TURN_LEFT20].legparam[1] = LegParam(5,95,30,40,5,21,-12);
		walkparams[WALK_TURN_LEFT20].legparam[2] = LegParam(-5,110,0,80,5,-24,-11);
		walkparams[WALK_TURN_LEFT20].legparam[3] = LegParam(-5,110,0,40,5,-9,-11);
		walkparams[WALK_TURN_LEFT20].move_x  = 76.44;
		walkparams[WALK_TURN_LEFT20].move_y  = 18.89;
		walkparams[WALK_TURN_LEFT20].move_tt = 20.50;

//WALK_TURN_LEFT40
		walkparams[WALK_TURN_LEFT40].time = 300;
		walkparams[WALK_TURN_LEFT40].gratio = 0.45;
		walkparams[WALK_TURN_LEFT40].aratio = 0.25;
		walkparams[WALK_TURN_LEFT40].legparam[0] = LegParam(15,95,15,90,16,16,-12);
		walkparams[WALK_TURN_LEFT40].legparam[1] = LegParam(15,95,15,70,21,21,-12);
		walkparams[WALK_TURN_LEFT40].legparam[2] = LegParam(-5,110,0,90,5,-9,-10);
		walkparams[WALK_TURN_LEFT40].legparam[3] = LegParam(-5,110,0,75,5,-6,-10);
		walkparams[WALK_TURN_LEFT40].move_x  = 115.36;
		walkparams[WALK_TURN_LEFT40].move_y  = 48.28;
		walkparams[WALK_TURN_LEFT40].move_tt = 16.75;

//WALK_TURN_LEFT80
		walkparams[WALK_TURN_LEFT80].time = 300;
		walkparams[WALK_TURN_LEFT80].gratio = 0.45;
		walkparams[WALK_TURN_LEFT80].aratio = 0.25;
		walkparams[WALK_TURN_LEFT80].legparam[0] = LegParam(5,95,30,85,15,13,-14);
		walkparams[WALK_TURN_LEFT80].legparam[1] = LegParam(5,95,30,85,15,13,-14);
		walkparams[WALK_TURN_LEFT80].legparam[2] = LegParam(-5,110,0,85,5,-5,-10);
		walkparams[WALK_TURN_LEFT80].legparam[3] = LegParam(-5,110,0,85,5,-4,-10);
		walkparams[WALK_TURN_LEFT80].move_x  = 137.97;
		walkparams[WALK_TURN_LEFT80].move_y  = 26.95;
		walkparams[WALK_TURN_LEFT80].move_tt = 10.00;

//WALK_TURN_RIGHT20
		walkparams[WALK_TURN_RIGHT20].time = 300;
		walkparams[WALK_TURN_RIGHT20].gratio = 0.45;
		walkparams[WALK_TURN_RIGHT20].aratio = 0.25;
		walkparams[WALK_TURN_RIGHT20].legparam[0] = LegParam(15,90,0,40,5,-20,-14);
		walkparams[WALK_TURN_RIGHT20].legparam[1] = LegParam(25,95,10,80,5,-20,-14);
		walkparams[WALK_TURN_RIGHT20].legparam[2] = LegParam(-5,110,0,40,5,9,-10);
		walkparams[WALK_TURN_RIGHT20].legparam[3] = LegParam(-5,110,10,80,9,24,-10);
		walkparams[WALK_TURN_RIGHT20].move_x  = 74.42;
		walkparams[WALK_TURN_RIGHT20].move_y  = -30.07;
		walkparams[WALK_TURN_RIGHT20].move_tt = -17.00;

//WALK_TURN_RIGHT40
		walkparams[WALK_TURN_RIGHT40].time = 300;
		walkparams[WALK_TURN_RIGHT40].gratio = 0.45;
		walkparams[WALK_TURN_RIGHT40].aratio = 0.25;
		walkparams[WALK_TURN_RIGHT40].legparam[0] = LegParam(15,95,15,70,15,-25,-14);
		walkparams[WALK_TURN_RIGHT40].legparam[1] = LegParam(15,95,15,90,15,-15,-14);
		walkparams[WALK_TURN_RIGHT40].legparam[2] = LegParam(-5,110,0,75,5,6,-10);
		walkparams[WALK_TURN_RIGHT40].legparam[3] = LegParam(-5,110,0,90,5,9,-10);
		walkparams[WALK_TURN_RIGHT40].move_x  = 118.59;
		walkparams[WALK_TURN_RIGHT40].move_y  = -47.23;
		walkparams[WALK_TURN_RIGHT40].move_tt = -16.17;

//WALK_TURN_RIGHT80
		walkparams[WALK_TURN_RIGHT80].time = 300;
		walkparams[WALK_TURN_RIGHT80].gratio = 0.45;
		walkparams[WALK_TURN_RIGHT80].aratio = 0.25;
		walkparams[WALK_TURN_RIGHT80].legparam[0] = LegParam(5,95,30,85,15,-3,-14);
		walkparams[WALK_TURN_RIGHT80].legparam[1] = LegParam(5,95,30,87,15,-4,-14);
		walkparams[WALK_TURN_RIGHT80].legparam[2] = LegParam(-5,110,0,85,5,12,-10);
		walkparams[WALK_TURN_RIGHT80].legparam[3] = LegParam(-5,110,0,87,5,14,-10);
		walkparams[WALK_TURN_RIGHT80].move_x  = 137.12;
		walkparams[WALK_TURN_RIGHT80].move_y  = -20.73;
		walkparams[WALK_TURN_RIGHT80].move_tt = -10.50;

//============================================================SMALL_WALK============================================================
//以前のVer.
		//WALK_SMALL10_RIGHT90
		walkparams[WALK_SMALL10_RIGHT90].time = 270;
		walkparams[WALK_SMALL10_RIGHT90].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT90].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT90].legparam[0] = LegParam(15,95,30,57,15,-86,-11);
		walkparams[WALK_SMALL10_RIGHT90].legparam[1] = LegParam(15,95,30,57,15,-85,-11);
		walkparams[WALK_SMALL10_RIGHT90].legparam[2] = LegParam(-5,110,0,55,5,-86,-7);
		walkparams[WALK_SMALL10_RIGHT90].legparam[3] = LegParam(-5,110,0,54,5,-85,-7);
		walkparams[WALK_SMALL10_RIGHT90].move_x  = 0;
		walkparams[WALK_SMALL10_RIGHT90].move_y  = -100/2;
		walkparams[WALK_SMALL10_RIGHT90].move_tt = 0;

		//WALK_SMALL10_RIGHT80
		walkparams[WALK_SMALL10_RIGHT80].time = 270;
		walkparams[WALK_SMALL10_RIGHT80].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT80].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT80].legparam[0] = LegParam(15,95,30,54,15,-78,-11);
		walkparams[WALK_SMALL10_RIGHT80].legparam[1] = LegParam(15,95,30,54,15,-72,-11);
		walkparams[WALK_SMALL10_RIGHT80].legparam[2] = LegParam(-5,110,0,54,5,-78,-7);
		walkparams[WALK_SMALL10_RIGHT80].legparam[3] = LegParam(-5,110,0,54,5,-72,-7);
		walkparams[WALK_SMALL10_RIGHT80].move_x  = 17.3/2;
		walkparams[WALK_SMALL10_RIGHT80].move_y  = -98/2;
		walkparams[WALK_SMALL10_RIGHT80].move_tt = 0;

		//WALK_SMALL10_RIGHT70
		walkparams[WALK_SMALL10_RIGHT70].time = 270;
		walkparams[WALK_SMALL10_RIGHT70].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT70].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT70].legparam[0] = LegParam(15,95,30,53,15,-71,-12);
		walkparams[WALK_SMALL10_RIGHT70].legparam[1] = LegParam(15,95,30,53,15,-66,-12);
		walkparams[WALK_SMALL10_RIGHT70].legparam[2] = LegParam(-5,110,0,53,5,-71,-8);
		walkparams[WALK_SMALL10_RIGHT70].legparam[3] = LegParam(-5,110,0,53,5,-66,-8);
		walkparams[WALK_SMALL10_RIGHT70].move_x  = 34.2/2;
		walkparams[WALK_SMALL10_RIGHT70].move_y  = -93.9/2;
		walkparams[WALK_SMALL10_RIGHT70].move_tt = 0;

		//WALK_SMALL10_RIGHT60
		walkparams[WALK_SMALL10_RIGHT60].time = 270;
		walkparams[WALK_SMALL10_RIGHT60].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT60].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT60].legparam[0] = LegParam(15,95,30,51,15,-62,-12);
		walkparams[WALK_SMALL10_RIGHT60].legparam[1] = LegParam(15,95,30,50,15,-60,-12);
		walkparams[WALK_SMALL10_RIGHT60].legparam[2] = LegParam(-5,110,0,51,5,-62,-8);
		walkparams[WALK_SMALL10_RIGHT60].legparam[3] = LegParam(-5,110,0,50,5,-60,-8);
		walkparams[WALK_SMALL10_RIGHT60].move_x  = 50/2;
		walkparams[WALK_SMALL10_RIGHT60].move_y  = -86.6/2;
		walkparams[WALK_SMALL10_RIGHT60].move_tt = 0;

		//WALK_SMALL10_RIGHT50
		walkparams[WALK_SMALL10_RIGHT50].time = 270;
		walkparams[WALK_SMALL10_RIGHT50].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT50].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT50].legparam[0] = LegParam(15,95,30,51,15,-53,-11);
		walkparams[WALK_SMALL10_RIGHT50].legparam[1] = LegParam(15,95,30,50,15,-50,-11);
		walkparams[WALK_SMALL10_RIGHT50].legparam[2] = LegParam(-5,110,0,51,5,-53,-7);
		walkparams[WALK_SMALL10_RIGHT50].legparam[3] = LegParam(-5,110,0,50,5,-50,-7);
		walkparams[WALK_SMALL10_RIGHT50].move_x  = 64.2/2;
		walkparams[WALK_SMALL10_RIGHT50].move_y  = -76.6/2;
		walkparams[WALK_SMALL10_RIGHT50].move_tt = 0;

		//WALK_SMALL10_RIGHT40
		walkparams[WALK_SMALL10_RIGHT40].time = 270;
		walkparams[WALK_SMALL10_RIGHT40].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT40].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT40].legparam[0] = LegParam(15,95,30,50,15,-45,-11);
		walkparams[WALK_SMALL10_RIGHT40].legparam[1] = LegParam(15,95,30,46,15,-42,-11);
		walkparams[WALK_SMALL10_RIGHT40].legparam[2] = LegParam(-5,110,0,50,5,-45,-7);
		walkparams[WALK_SMALL10_RIGHT40].legparam[3] = LegParam(-5,110,0,46,5,-42,-7);
		walkparams[WALK_SMALL10_RIGHT40].move_x  = 76.6/2;
		walkparams[WALK_SMALL10_RIGHT40].move_y  = -64.2/2;
		walkparams[WALK_SMALL10_RIGHT40].move_tt = 0;

		//WALK_SMALL10_RIGHT30
		walkparams[WALK_SMALL10_RIGHT30].time = 270;
		walkparams[WALK_SMALL10_RIGHT30].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT30].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT30].legparam[0] = LegParam(15,95,30,45,15,-34,-12);
		walkparams[WALK_SMALL10_RIGHT30].legparam[1] = LegParam(15,95,30,43,15,-31,-12);
		walkparams[WALK_SMALL10_RIGHT30].legparam[2] = LegParam(-5,110,0,45,5,-34,-8);
		walkparams[WALK_SMALL10_RIGHT30].legparam[3] = LegParam(-5,110,0,43,5,-31,-8);
		walkparams[WALK_SMALL10_RIGHT30].move_x  = 86.6/2;
		walkparams[WALK_SMALL10_RIGHT30].move_y  = -50/2;
		walkparams[WALK_SMALL10_RIGHT30].move_tt = 0;

		//WALK_SMALL10_RIGHT20
		walkparams[WALK_SMALL10_RIGHT20].time = 270;
		walkparams[WALK_SMALL10_RIGHT20].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT20].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT20].legparam[0] = LegParam(15,95,30,44,15,-20,-11);
		walkparams[WALK_SMALL10_RIGHT20].legparam[1] = LegParam(15,95,30,42,15,-18,-11);
		walkparams[WALK_SMALL10_RIGHT20].legparam[2] = LegParam(-5,110,0,44,5,-20,-7);
		walkparams[WALK_SMALL10_RIGHT20].legparam[3] = LegParam(-5,110,0,42,5,-18,-7);
		walkparams[WALK_SMALL10_RIGHT20].move_x  = 93.9/2;
		walkparams[WALK_SMALL10_RIGHT20].move_y  = -34.2/2;
		walkparams[WALK_SMALL10_RIGHT20].move_tt = 0;

		//WALK_SMALL10_RIGHT10
		walkparams[WALK_SMALL10_RIGHT10].time = 270;
		walkparams[WALK_SMALL10_RIGHT10].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT10].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT10].legparam[0] = LegParam(15,95,30,42,15,-10,-12);
		walkparams[WALK_SMALL10_RIGHT10].legparam[1] = LegParam(15,95,30,39,15,-9,-12);
		walkparams[WALK_SMALL10_RIGHT10].legparam[2] = LegParam(-5,110,0,42,5,-10,-8);
		walkparams[WALK_SMALL10_RIGHT10].legparam[3] = LegParam(-5,110,0,39,5,-9,-8);
		walkparams[WALK_SMALL10_RIGHT10].move_x  = 98.4/2;
		walkparams[WALK_SMALL10_RIGHT10].move_y  = -17.3/2;
		walkparams[WALK_SMALL10_RIGHT10].move_tt = 0;

		//WALK_SMALL10_FORWARD
		walkparams[WALK_SMALL10_FORWARD].time = 270;
		walkparams[WALK_SMALL10_FORWARD].gratio = 0.45;
		walkparams[WALK_SMALL10_FORWARD].aratio = 0.25;
		walkparams[WALK_SMALL10_FORWARD].legparam[0] = LegParam(15,95,30,38,15,5,-12);
		walkparams[WALK_SMALL10_FORWARD].legparam[1] = LegParam(15,95,30,39,15,4,-12);
		walkparams[WALK_SMALL10_FORWARD].legparam[2] = LegParam(-5,110,0,38,5,5,-8);
		walkparams[WALK_SMALL10_FORWARD].legparam[3] = LegParam(-5,110,0,39,5,4,-8);
		walkparams[WALK_SMALL10_FORWARD].move_x  = 100/2;
		walkparams[WALK_SMALL10_FORWARD].move_y  = 0;
		walkparams[WALK_SMALL10_FORWARD].move_tt = 0;

		//WALK_SMALL10_LEFT10
		walkparams[WALK_SMALL10_LEFT10].time = 270;
		walkparams[WALK_SMALL10_LEFT10].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT10].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT10].legparam[0] = LegParam(15,95,30,37,15,12,-13);
		walkparams[WALK_SMALL10_LEFT10].legparam[1] = LegParam(15,95,30,41,15,11,-13);
		walkparams[WALK_SMALL10_LEFT10].legparam[2] = LegParam(-5,110,0,37,5,12,-9);
		walkparams[WALK_SMALL10_LEFT10].legparam[3] = LegParam(-5,110,0,41,5,11,-9);
		walkparams[WALK_SMALL10_LEFT10].move_x  = 98.4/2;
		walkparams[WALK_SMALL10_LEFT10].move_y  = 17.3/2;
		walkparams[WALK_SMALL10_LEFT10].move_tt = 0;


		//WALK_SMALL10_LEFT20
		walkparams[WALK_SMALL10_LEFT20].time = 270;
		walkparams[WALK_SMALL10_LEFT20].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT20].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT20].legparam[0] = LegParam(15,95,30,40,15,28,-11);
		walkparams[WALK_SMALL10_LEFT20].legparam[1] = LegParam(15,95,30,45,15,27,-11);
		walkparams[WALK_SMALL10_LEFT20].legparam[2] = LegParam(-5,110,0,40,5,28,-7);
		walkparams[WALK_SMALL10_LEFT20].legparam[3] = LegParam(-5,110,0,45,5,27,-7);
		walkparams[WALK_SMALL10_LEFT20].move_x  = 93.9/2;
		walkparams[WALK_SMALL10_LEFT20].move_y  = 34.2/2;
		walkparams[WALK_SMALL10_LEFT20].move_tt = 0;

		//WALK_SMALL10_LEFT30
		walkparams[WALK_SMALL10_LEFT30].time = 270;
		walkparams[WALK_SMALL10_LEFT30].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT30].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT30].legparam[0] = LegParam(15,95,30,38,15,38,-13);
		walkparams[WALK_SMALL10_LEFT30].legparam[1] = LegParam(15,95,30,44,15,39,-13);
		walkparams[WALK_SMALL10_LEFT30].legparam[2] = LegParam(-5,110,0,38,5,38,-9);
		walkparams[WALK_SMALL10_LEFT30].legparam[3] = LegParam(-5,110,0,44,5,39,-9);
		walkparams[WALK_SMALL10_LEFT30].move_x  = 86.6/2;
		walkparams[WALK_SMALL10_LEFT30].move_y  = 50/2;
		walkparams[WALK_SMALL10_LEFT30].move_tt = 0;

		//WALK_SMALL10_LEFT40
		walkparams[WALK_SMALL10_LEFT40].time = 270;
		walkparams[WALK_SMALL10_LEFT40].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT40].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT40].legparam[0] = LegParam(15,95,30,41,15,54,-13);
		walkparams[WALK_SMALL10_LEFT40].legparam[1] = LegParam(15,95,30,48,15,52,-13);
		walkparams[WALK_SMALL10_LEFT40].legparam[2] = LegParam(-5,110,0,41,5,54,-9);
		walkparams[WALK_SMALL10_LEFT40].legparam[3] = LegParam(-5,110,0,48,5,52,-9);
		walkparams[WALK_SMALL10_LEFT40].move_x  = 76.6/2;
		walkparams[WALK_SMALL10_LEFT40].move_y  = 64.2/2;
		walkparams[WALK_SMALL10_LEFT40].move_tt = 0;

		//WALK_SMALL10_LEFT50
		walkparams[WALK_SMALL10_LEFT50].time = 270;
		walkparams[WALK_SMALL10_LEFT50].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT50].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT50].legparam[0] = LegParam(15,95,30,50,15,64,-11);
		walkparams[WALK_SMALL10_LEFT50].legparam[1] = LegParam(15,95,30,52,15,66,-11);
		walkparams[WALK_SMALL10_LEFT50].legparam[2] = LegParam(-5,110,0,50,5,64,-7);
		walkparams[WALK_SMALL10_LEFT50].legparam[3] = LegParam(-5,110,0,52,5,66,-7);
		walkparams[WALK_SMALL10_LEFT50].move_x  = 64.2/2;
		walkparams[WALK_SMALL10_LEFT50].move_y  = 76.6/2;
		walkparams[WALK_SMALL10_LEFT50].move_tt = 0;

		//WALK_SMALL10_LEFT60
		walkparams[WALK_SMALL10_LEFT60].time = 270;
		walkparams[WALK_SMALL10_LEFT60].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT60].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT60].legparam[0] = LegParam(15,95,30,60,15,75,-11);
		walkparams[WALK_SMALL10_LEFT60].legparam[1] = LegParam(15,95,30,63,15,90,-11);
		walkparams[WALK_SMALL10_LEFT60].legparam[2] = LegParam(-5,110,0,57,5,76,-7);
		walkparams[WALK_SMALL10_LEFT60].legparam[3] = LegParam(-5,110,0,61,5,80,-7);
		walkparams[WALK_SMALL10_LEFT60].move_x  = 50/2;
		walkparams[WALK_SMALL10_LEFT60].move_y  = 86.6/2;
		walkparams[WALK_SMALL10_LEFT60].move_tt = 0;

		//WALK_SMALL10_LEFT70
		walkparams[WALK_SMALL10_LEFT70].time = 270;
		walkparams[WALK_SMALL10_LEFT70].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT70].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT70].legparam[0] = LegParam(15,95,30,56,15,84,-11);
		walkparams[WALK_SMALL10_LEFT70].legparam[1] = LegParam(15,95,30,56,15,90,-11);
		walkparams[WALK_SMALL10_LEFT70].legparam[2] = LegParam(-5,110,0,56,5,84,-7);
		walkparams[WALK_SMALL10_LEFT70].legparam[3] = LegParam(-5,110,0,56,5,90,-7);
		walkparams[WALK_SMALL10_LEFT70].move_x  = 34.2/2;
		walkparams[WALK_SMALL10_LEFT70].move_y  = 93.9/2;
		walkparams[WALK_SMALL10_LEFT70].move_tt = 0;

		//WALK_SMALL10_LEFT80
		walkparams[WALK_SMALL10_LEFT80].time = 270;
		walkparams[WALK_SMALL10_LEFT80].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT80].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT80].legparam[0] = LegParam(15,95,30,70,15,80,-1);
		walkparams[WALK_SMALL10_LEFT80].legparam[1] = LegParam(15,95,30,70,15,90,-1);
		walkparams[WALK_SMALL10_LEFT80].legparam[2] = LegParam(-5,110,5,65,5,100,-5);
		walkparams[WALK_SMALL10_LEFT80].legparam[3] = LegParam(-5,110,5,65,5,100,-5);
		walkparams[WALK_SMALL10_LEFT80].move_x  = 17.3/2;
		walkparams[WALK_SMALL10_LEFT80].move_y  = 98.4/2;
		walkparams[WALK_SMALL10_LEFT80].move_tt = 0;

		//WALK_SMALL10_LEFT90
		walkparams[WALK_SMALL10_LEFT90].time = 270;
		walkparams[WALK_SMALL10_LEFT90].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT90].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT90].legparam[0] = LegParam(15,95,30,70,15,85,-1);
		walkparams[WALK_SMALL10_LEFT90].legparam[1] = LegParam(15,95,30,70,15,105,-1);
		walkparams[WALK_SMALL10_LEFT90].legparam[2] = LegParam(-5,110,5,65,5,105,-5);
		walkparams[WALK_SMALL10_LEFT90].legparam[3] = LegParam(-5,110,5,65,5,110,-5);
		walkparams[WALK_SMALL10_LEFT90].move_x  = 0;
		walkparams[WALK_SMALL10_LEFT90].move_y  = 100/2;
		walkparams[WALK_SMALL10_LEFT90].move_tt = 0;

		//WALK_ROTATE_LEFT360
		walkparams[WALK_ROTATE_LEFT360].time = 300;
		walkparams[WALK_ROTATE_LEFT360].gratio = 0.40;
		walkparams[WALK_ROTATE_LEFT360].aratio = 0.25;
		walkparams[WALK_ROTATE_LEFT360].legparam[0] = LegParam(35,90,15,40,10,60,-7);
		walkparams[WALK_ROTATE_LEFT360].legparam[1] = LegParam(35,90,15,40,10,125,-7);
		walkparams[WALK_ROTATE_LEFT360].legparam[2] = LegParam(-5,110,15,65,10,-60,-7);
		walkparams[WALK_ROTATE_LEFT360].legparam[3] = LegParam(-5,110,15,65,10,-120,-7);
		walkparams[WALK_ROTATE_LEFT360].move_x  = 2.07;
		walkparams[WALK_ROTATE_LEFT360].move_y  = 19.44;
		walkparams[WALK_ROTATE_LEFT360].move_tt = 37.0;

		//WALK_ROTATE_RIGHT360
		walkparams[WALK_ROTATE_RIGHT360].time = 300;
		walkparams[WALK_ROTATE_RIGHT360].gratio = 0.40;
		walkparams[WALK_ROTATE_RIGHT360].aratio = 0.25;
		walkparams[WALK_ROTATE_RIGHT360].legparam[0] = LegParam(35,95,15,35,10,-120,-10);
		walkparams[WALK_ROTATE_RIGHT360].legparam[1] = LegParam(35,95,15,35,10,-60,-10);
		walkparams[WALK_ROTATE_RIGHT360].legparam[2] = LegParam(-5,110,15,65,10,120,-10);
		walkparams[WALK_ROTATE_RIGHT360].legparam[3] = LegParam(-5,110,15,65,10,60,-10);
		walkparams[WALK_ROTATE_RIGHT360].move_x  = 1.11;
		walkparams[WALK_ROTATE_RIGHT360].move_y  = -28.17;
		walkparams[WALK_ROTATE_RIGHT360].move_tt = -37.0;

		//WALK_ROTATE_LEFT520
		walkparams[WALK_ROTATE_LEFT520].time = 300;
		walkparams[WALK_ROTATE_LEFT520].gratio = 0.40;
		walkparams[WALK_ROTATE_LEFT520].aratio = 0.25;
		walkparams[WALK_ROTATE_LEFT520].legparam[0] = LegParam(35,90,15,60,10,60,-7);
		walkparams[WALK_ROTATE_LEFT520].legparam[1] = LegParam(35,90,15,60,10,125,-7);
		walkparams[WALK_ROTATE_LEFT520].legparam[2] = LegParam(-5,110,15,80,10,-60,-7);
		walkparams[WALK_ROTATE_LEFT520].legparam[3] = LegParam(-5,110,15,80,10,-120,-7);
		walkparams[WALK_ROTATE_LEFT520].move_x  = 2.07;
		walkparams[WALK_ROTATE_LEFT520].move_y  = 19.44;
		walkparams[WALK_ROTATE_LEFT520].move_tt = 53.0;

		//WALK_ROTATE_RIGHT520
		walkparams[WALK_ROTATE_RIGHT520].time = 300;
		walkparams[WALK_ROTATE_RIGHT520].gratio = 0.40;
		walkparams[WALK_ROTATE_RIGHT520].aratio = 0.25;
		walkparams[WALK_ROTATE_RIGHT520].legparam[0] = LegParam(35,95,15,60,10,-120,-10);
		walkparams[WALK_ROTATE_RIGHT520].legparam[1] = LegParam(35,95,15,60,10,-60,-10);
		walkparams[WALK_ROTATE_RIGHT520].legparam[2] = LegParam(-5,110,15,80,10,120,-10);
		walkparams[WALK_ROTATE_RIGHT520].legparam[3] = LegParam(-5,110,15,80,10,60,-10);
		walkparams[WALK_ROTATE_RIGHT520].move_x  = 1.11;
		walkparams[WALK_ROTATE_RIGHT520].move_y  = -28.17;
		walkparams[WALK_ROTATE_RIGHT520].move_tt = -52.0;

/*0429
//WALK_SMALL10_RIGHT90
		walkparams[WALK_SMALL10_RIGHT90].time = 300;
		walkparams[WALK_SMALL10_RIGHT90].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT90].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT90].legparam[0] = LegParam(5,100,30,55,15,-94,-12);
		walkparams[WALK_SMALL10_RIGHT90].legparam[1] = LegParam(5,100,30,55,15,-82,-12);
		walkparams[WALK_SMALL10_RIGHT90].legparam[2] = LegParam(-5,110,10,45,5,-94,-8);
		walkparams[WALK_SMALL10_RIGHT90].legparam[3] = LegParam(-5,110,0,45,5,-82,-8);
		walkparams[WALK_SMALL10_RIGHT90].move_x  = 0;
		walkparams[WALK_SMALL10_RIGHT90].move_y  = -100/2;
		walkparams[WALK_SMALL10_RIGHT90].move_tt = 0;

//WALK_SMALL10_RIGHT80
		walkparams[WALK_SMALL10_RIGHT80].time = 300;
		walkparams[WALK_SMALL10_RIGHT80].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT80].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT80].legparam[0] = LegParam(5,100,30,47,15,-88,-12);
		walkparams[WALK_SMALL10_RIGHT80].legparam[1] = LegParam(5,100,30,47,15,-78,-12);
		walkparams[WALK_SMALL10_RIGHT80].legparam[2] = LegParam(-5,110,10,47,5,-88,-8);
		walkparams[WALK_SMALL10_RIGHT80].legparam[3] = LegParam(-5,110,10,47,5,-78,-8);
		walkparams[WALK_SMALL10_RIGHT80].move_x  = 17.3/2;
		walkparams[WALK_SMALL10_RIGHT80].move_y  = -98/2;
		walkparams[WALK_SMALL10_RIGHT80].move_tt = 0;

//WALK_SMALL10_RIGHT70
		walkparams[WALK_SMALL10_RIGHT70].time = 300;
		walkparams[WALK_SMALL10_RIGHT70].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT70].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT70].legparam[0] = LegParam(5,100,30,45,15,-76,-12);
		walkparams[WALK_SMALL10_RIGHT70].legparam[1] = LegParam(5,100,30,45,15,-71,-12);
		walkparams[WALK_SMALL10_RIGHT70].legparam[2] = LegParam(-5,110,10,45,5,-76,-8);
		walkparams[WALK_SMALL10_RIGHT70].legparam[3] = LegParam(-5,110,10,45,5,-71,-8);
		walkparams[WALK_SMALL10_RIGHT70].move_x  = 34.2/2;
		walkparams[WALK_SMALL10_RIGHT70].move_y  = -93.9/2;
		walkparams[WALK_SMALL10_RIGHT70].move_tt = 0;

//WALK_SMALL10_RIGHT60
		walkparams[WALK_SMALL10_RIGHT60].time = 300;
		walkparams[WALK_SMALL10_RIGHT60].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT60].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT60].legparam[0] = LegParam(5,100,30,37,15,-60,-12);
		walkparams[WALK_SMALL10_RIGHT60].legparam[1] = LegParam(5,100,30,33,15,-59,-12);
		walkparams[WALK_SMALL10_RIGHT60].legparam[2] = LegParam(-5,110,10,37,5,-60,-8);
		walkparams[WALK_SMALL10_RIGHT60].legparam[3] = LegParam(-5,110,10,37,5,-59,-8);
		walkparams[WALK_SMALL10_RIGHT60].move_x  = 50/2;
		walkparams[WALK_SMALL10_RIGHT60].move_y  = -86.6/2;
		walkparams[WALK_SMALL10_RIGHT60].move_tt = 0;

//WALK_SMALL10_RIGHT50
		walkparams[WALK_SMALL10_RIGHT50].time = 300;
		walkparams[WALK_SMALL10_RIGHT50].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT50].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT50].legparam[0] = LegParam(5,100,30,39,15,-43,-12);
		walkparams[WALK_SMALL10_RIGHT50].legparam[1] = LegParam(5,100,30,38,15,-41,-12);
		walkparams[WALK_SMALL10_RIGHT50].legparam[2] = LegParam(-5,110,0,39,5,-43,-8);
		walkparams[WALK_SMALL10_RIGHT50].legparam[3] = LegParam(-5,110,0,38,5,-41,-8);
		walkparams[WALK_SMALL10_RIGHT50].move_x  = 64.2/2;
		walkparams[WALK_SMALL10_RIGHT50].move_y  = -76.6/2;
		walkparams[WALK_SMALL10_RIGHT50].move_tt = 0;

//WALK_SMALL10_RIGHT40
		walkparams[WALK_SMALL10_RIGHT40].time = 300;
		walkparams[WALK_SMALL10_RIGHT40].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT40].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT40].legparam[0] = LegParam(5,100,30,38,15,-33,-12);
		walkparams[WALK_SMALL10_RIGHT40].legparam[1] = LegParam(5,100,30,34,15,-28,-12);
		walkparams[WALK_SMALL10_RIGHT40].legparam[2] = LegParam(-5,110,0,38,5,-33,-8);
		walkparams[WALK_SMALL10_RIGHT40].legparam[3] = LegParam(-5,110,0,34,5,-28,-8);
		walkparams[WALK_SMALL10_RIGHT40].move_x  = 76.6/2;
		walkparams[WALK_SMALL10_RIGHT40].move_y  = -64.2/2;
		walkparams[WALK_SMALL10_RIGHT40].move_tt = 0;

//WALK_SMALL10_RIGHT30
		walkparams[WALK_SMALL10_RIGHT30].time = 300;
		walkparams[WALK_SMALL10_RIGHT30].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT30].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT30].legparam[0] = LegParam(5,100,30,37,15,-25,-12);
		walkparams[WALK_SMALL10_RIGHT30].legparam[1] = LegParam(5,100,30,35,15,-22,-12);
		walkparams[WALK_SMALL10_RIGHT30].legparam[2] = LegParam(-5,110,0,37,5,-25,-8);
		walkparams[WALK_SMALL10_RIGHT30].legparam[3] = LegParam(-5,110,0,35,5,-22,-8);
		walkparams[WALK_SMALL10_RIGHT30].move_x  = 86.6/2;
		walkparams[WALK_SMALL10_RIGHT30].move_y  = -50/2;
		walkparams[WALK_SMALL10_RIGHT30].move_tt = 0;

//WALK_SMALL10_RIGHT20
		walkparams[WALK_SMALL10_RIGHT20].time = 300;
		walkparams[WALK_SMALL10_RIGHT20].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT20].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT20].legparam[0] = LegParam(5,100,30,36,15,-12,-12);
		walkparams[WALK_SMALL10_RIGHT20].legparam[1] = LegParam(5,100,30,34,15,-12,-12);
		walkparams[WALK_SMALL10_RIGHT20].legparam[2] = LegParam(-5,110,0,36,5,-12,-8);
		walkparams[WALK_SMALL10_RIGHT20].legparam[3] = LegParam(-5,110,0,34,5,-12,-8);
		walkparams[WALK_SMALL10_RIGHT20].move_x  = 93.9/2;
		walkparams[WALK_SMALL10_RIGHT20].move_y  = -34.2/2;
		walkparams[WALK_SMALL10_RIGHT20].move_tt = 0;

//WALK_SMALL10_RIGHT10
		walkparams[WALK_SMALL10_RIGHT10].time = 300;
		walkparams[WALK_SMALL10_RIGHT10].gratio = 0.45;
		walkparams[WALK_SMALL10_RIGHT10].aratio = 0.25;
		walkparams[WALK_SMALL10_RIGHT10].legparam[0] = LegParam(5,100,30,38,15,0,-12);
		walkparams[WALK_SMALL10_RIGHT10].legparam[1] = LegParam(5,100,30,34,15,0,-12);
		walkparams[WALK_SMALL10_RIGHT10].legparam[2] = LegParam(-5,110,0,38,5,0,-8);
		walkparams[WALK_SMALL10_RIGHT10].legparam[3] = LegParam(-5,110,0,34,5,0,-8);
		walkparams[WALK_SMALL10_RIGHT10].move_x  = 98.4/2;
		walkparams[WALK_SMALL10_RIGHT10].move_y  = -17.3/2;
		walkparams[WALK_SMALL10_RIGHT10].move_tt = 0;

//WALK_SMALL10_FORWARD
		walkparams[WALK_SMALL10_FORWARD].time = 300;
		walkparams[WALK_SMALL10_FORWARD].gratio = 0.45;
		walkparams[WALK_SMALL10_FORWARD].aratio = 0.25;
		walkparams[WALK_SMALL10_FORWARD].legparam[0] = LegParam(5,100,30,41,15,5,-12);
		walkparams[WALK_SMALL10_FORWARD].legparam[1] = LegParam(5,100,30,31,15,5,-12);
		walkparams[WALK_SMALL10_FORWARD].legparam[2] = LegParam(-5,110,0,41,5,5,-8);
		walkparams[WALK_SMALL10_FORWARD].legparam[3] = LegParam(-5,110,0,31,5,5,-8);
		walkparams[WALK_SMALL10_FORWARD].move_x  = 100/2;
		walkparams[WALK_SMALL10_FORWARD].move_y  = 0;
		walkparams[WALK_SMALL10_FORWARD].move_tt = 0;

//WALK_SMALL10_LEFT10
		walkparams[WALK_SMALL10_LEFT10].time = 300;
		walkparams[WALK_SMALL10_LEFT10].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT10].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT10].legparam[0] = LegParam(5,100,30,40,15,33,-12);
		walkparams[WALK_SMALL10_LEFT10].legparam[1] = LegParam(5,100,30,36,15,15,-12);
		walkparams[WALK_SMALL10_LEFT10].legparam[2] = LegParam(-5,110,0,36,5,16,-8);
		walkparams[WALK_SMALL10_LEFT10].legparam[3] = LegParam(-5,110,0,36,5,15,-8);
		walkparams[WALK_SMALL10_LEFT10].move_x  = 98.4/2;
		walkparams[WALK_SMALL10_LEFT10].move_y  = 17.3/2;
		walkparams[WALK_SMALL10_LEFT10].move_tt = 0;

//WALK_SMALL10_LEFT20
		walkparams[WALK_SMALL10_LEFT20].time = 300;
		walkparams[WALK_SMALL10_LEFT20].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT20].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT20].legparam[0] = LegParam(5,100,30,45,15,35,-12);
		walkparams[WALK_SMALL10_LEFT20].legparam[1] = LegParam(5,100,30,36,15,22,-12);
		walkparams[WALK_SMALL10_LEFT20].legparam[2] = LegParam(-5,110,0,38,5,23,-8);
		walkparams[WALK_SMALL10_LEFT20].legparam[3] = LegParam(-5,110,0,36,5,22,-8);
		walkparams[WALK_SMALL10_LEFT20].move_x  = 93.9/2;
		walkparams[WALK_SMALL10_LEFT20].move_y  = 34.2/2;
		walkparams[WALK_SMALL10_LEFT20].move_tt = 0;

//WALK_SMALL10_LEFT30
		walkparams[WALK_SMALL10_LEFT30].time = 300;
		walkparams[WALK_SMALL10_LEFT30].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT30].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT30].legparam[0] = LegParam(5,100,30,45,15,45,-12);
		walkparams[WALK_SMALL10_LEFT30].legparam[1] = LegParam(5,100,30,39,15,40,-12);
		walkparams[WALK_SMALL10_LEFT30].legparam[2] = LegParam(-5,110,0,39,5,40,-8);
		walkparams[WALK_SMALL10_LEFT30].legparam[3] = LegParam(-5,110,0,39,5,40,-9);
		walkparams[WALK_SMALL10_LEFT30].move_x  = 86.6/2;
		walkparams[WALK_SMALL10_LEFT30].move_y  = 50/2;
		walkparams[WALK_SMALL10_LEFT30].move_tt = 0;

//WALK_SMALL10_LEFT40
		walkparams[WALK_SMALL10_LEFT40].time = 300;
		walkparams[WALK_SMALL10_LEFT40].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT40].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT40].legparam[0] = LegParam(5,100,30,55,15,55,-12);
		walkparams[WALK_SMALL10_LEFT40].legparam[1] = LegParam(5,100,30,41,15,52,-12);
		walkparams[WALK_SMALL10_LEFT40].legparam[2] = LegParam(-5,110,0,41,5,52,-8);
		walkparams[WALK_SMALL10_LEFT40].legparam[3] = LegParam(-5,110,0,41,5,52,-8);
		walkparams[WALK_SMALL10_LEFT40].move_x  = 76.6/2;
		walkparams[WALK_SMALL10_LEFT40].move_y  = 64.2/2;
		walkparams[WALK_SMALL10_LEFT40].move_tt = 0;

//WALK_SMALL10_LEFT50
		walkparams[WALK_SMALL10_LEFT50].time = 300;
		walkparams[WALK_SMALL10_LEFT50].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT50].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT50].legparam[0] = LegParam(5,100,30,50,15,63,-12);
		walkparams[WALK_SMALL10_LEFT50].legparam[1] = LegParam(5,100,30,40,15,58,-12);
		walkparams[WALK_SMALL10_LEFT50].legparam[2] = LegParam(-5,110,0,40,5,58,-8);
		walkparams[WALK_SMALL10_LEFT50].legparam[3] = LegParam(-5,110,0,40,5,58,-8);
		walkparams[WALK_SMALL10_LEFT50].move_x  = 64.2/2;
		walkparams[WALK_SMALL10_LEFT50].move_y  = 76.6/2;
		walkparams[WALK_SMALL10_LEFT50].move_tt = 0;

//WALK_SMALL10_LEFT60
		walkparams[WALK_SMALL10_LEFT60].time = 300;
		walkparams[WALK_SMALL10_LEFT60].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT60].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT60].legparam[0] = LegParam(5,100,30,45,15,70,-12);
		walkparams[WALK_SMALL10_LEFT60].legparam[1] = LegParam(5,100,30,40,15,95,-12);
		walkparams[WALK_SMALL10_LEFT60].legparam[2] = LegParam(-5,110,10,33,5,83,-8);
		walkparams[WALK_SMALL10_LEFT60].legparam[3] = LegParam(-5,110,0,33,5,83,-8);
		walkparams[WALK_SMALL10_LEFT60].move_x  = 50/2;
		walkparams[WALK_SMALL10_LEFT60].move_y  = 86.6/2;
		walkparams[WALK_SMALL10_LEFT60].move_tt = 0;

//WALK_SMALL10_LEFT70
		walkparams[WALK_SMALL10_LEFT70].time = 300;
		walkparams[WALK_SMALL10_LEFT70].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT70].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT70].legparam[0] = LegParam(10,100,30,45,15,70,-12);
		walkparams[WALK_SMALL10_LEFT70].legparam[1] = LegParam(10,100,30,40,15,95,-12);
		walkparams[WALK_SMALL10_LEFT70].legparam[2] = LegParam(-5,110,10,33,5,83,-8);
		walkparams[WALK_SMALL10_LEFT70].legparam[3] = LegParam(-5,110,10,33,5,83,-8);
		walkparams[WALK_SMALL10_LEFT70].move_x  = 34.2/2;
		walkparams[WALK_SMALL10_LEFT70].move_y  = 93.9/2;
		walkparams[WALK_SMALL10_LEFT70].move_tt = 0;

//WALK_SMALL10_LEFT80
		walkparams[WALK_SMALL10_LEFT80].time = 300;
		walkparams[WALK_SMALL10_LEFT80].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT80].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT80].legparam[0] = LegParam(5,100,30,45,15,72,-12);
		walkparams[WALK_SMALL10_LEFT80].legparam[1] = LegParam(5,100,30,43,15,107,-12);
		walkparams[WALK_SMALL10_LEFT80].legparam[2] = LegParam(-5,110,10,30,5,92,-8);
		walkparams[WALK_SMALL10_LEFT80].legparam[3] = LegParam(-5,110,10,30,5,92,-8);
		walkparams[WALK_SMALL10_LEFT80].move_x  = 17.3/2;
		walkparams[WALK_SMALL10_LEFT80].move_y  = 98.4/2;
		walkparams[WALK_SMALL10_LEFT80].move_tt = 0;

//WALK_SMALL10_LEFT90
		walkparams[WALK_SMALL10_LEFT90].time = 300;
		walkparams[WALK_SMALL10_LEFT90].gratio = 0.45;
		walkparams[WALK_SMALL10_LEFT90].aratio = 0.25;
		walkparams[WALK_SMALL10_LEFT90].legparam[0] = LegParam(5,100,30,45,15,80,-12);
		walkparams[WALK_SMALL10_LEFT90].legparam[1] = LegParam(5,100,30,43,15,115,-12);
		walkparams[WALK_SMALL10_LEFT90].legparam[2] = LegParam(-5,110,10,30,5,100,-8);
		walkparams[WALK_SMALL10_LEFT90].legparam[3] = LegParam(-5,110,10,30,5,100,-8);
		walkparams[WALK_SMALL10_LEFT90].move_x  = 0;
		walkparams[WALK_SMALL10_LEFT90].move_y  = 100/2;
		walkparams[WALK_SMALL10_LEFT90].move_tt = 0;
*/
//============================================================SMALL_WALK ここまで============================================================
		//WALK_ROTATE_LEFT_SEARCHBALL
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].time = 250;
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].gratio = 0.4;
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].aratio = 0.25;
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].legparam[0] = LegParam(5,90,15,50,10,60,-12);
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].legparam[1] = LegParam(5,90,15,50,10,120,-12);
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].legparam[2] = LegParam(-5,110,10,50,13,300,-16);
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].legparam[3] = LegParam(-5,110,10,50,13,240,-16);
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].move_x  = 0;
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].move_y  = 100/2;
		walkparams[WALK_ROTATE_LEFT_SEARCHBALL].move_tt = 0;

		//WALK_ROTATE_RIGHT_SEARCHBALL
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].time = 250;
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].gratio = 0.4;
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].aratio = 0.25;
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].legparam[0] = LegParam(5,90,15,50,10,240,-12);
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].legparam[1] = LegParam(5,90,15,50,10,300,-12);
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].legparam[2] = LegParam(-5,110,10,50,13,120,-16);
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].legparam[3] = LegParam(-5,110,10,50,13,60,-16);
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].move_x  = 0;
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].move_y  = 100/2;
		walkparams[WALK_ROTATE_RIGHT_SEARCHBALL].move_tt = 0;

		//WALK_GOALIE_STOP
		walkparams[WALK_GOALIE_STOP].time = 300;
		walkparams[WALK_GOALIE_STOP].gratio = 0.24;
		walkparams[WALK_GOALIE_STOP].aratio = 0.24;
		walkparams[WALK_GOALIE_STOP].legparam[0] = LegParam(40,5,125,0,0,0,0);//LegParam(40,30,120,0,0,0,0);
		walkparams[WALK_GOALIE_STOP].legparam[1] = LegParam(40,5,125,0,0,0,0);//LegParam(40,30,120,0,0,0,0);
		walkparams[WALK_GOALIE_STOP].legparam[2] = LegParam(-110,0,0,0,0,0,0);
		walkparams[WALK_GOALIE_STOP].legparam[3] = LegParam(-110,0,0,0,0,0,0);
		walkparams[WALK_GOALIE_STOP].move_x  = 0;
		walkparams[WALK_GOALIE_STOP].move_y  = 0;
		walkparams[WALK_GOALIE_STOP].move_tt = 0;

		

		isInitialized = true;
	}
}

