#include "StdAfx.h"
#include "BehaviorList.h"
#include "../Motion/PresetWalkParam.h" //takeshita 05/0517
#include <math.h>

#include <OPENR/OSyslog.h>

//PresetWalkParam BhvMoveToPosition::presetWalkParam;

BhvMoveToPosition::BhvMoveToPosition(){
	dest_x = 0;
	dest_y = 0;
	dest_tt = 0;
}

void BhvMoveToPosition::Execute(){
	int walk = 0;
	static bool final_state = false;
	
	if(final_state){
		static int count_fs = 0;
		SetPresetWalk(0);
		if(count_fs == 100){
			final_state = false;
			count_fs = 0;
		}
		count_fs++;
		return;
	}

/*	static int count = 0;
	if(count == 20){
		OSYSPRINT(("x = %.1f, y = %.1f, theta = %.1f \n", 
		state.self.position.x.val, state.self.position.y.val, state.self.position.tt.val ));
		count = 0;
	}
	count++;
*/
/*
	static int cnt_swing = 0;
	cnt_swing++;
	if(state.ball.visible){
		cnt_swing++;
		if(cnt_swing > 100){
			SetHeadMode(HEAD_WATCHING);//HEAD_SWING_LOCALIZATION);
			if(cnt_swing == 130) cnt_swing = 0;
		}
		else SetHeadMode(HEAD_TRACK);
	}
	else{
		SetHeadMode(HEAD_SWING_LOCALIZATION);
		cnt_swing = 0;
	}
*/

	static double k_x = 1.0;
	static double k_y = 1.0;
	static double k_tt = 50.0;

	double pos_x = state.self.position.x.val;
	double pos_y = state.self.position.y.val;
	double pos_tt = state.self.position.tt.val;

	//終端条件
	if( fabs(pos_x-dest_x) < 200.0 &&
		fabs(pos_y-dest_y) < 200.0 &&
		fabs(pos_tt-dest_tt) < 20.0)
	{
		final_state = true;
		SetPresetWalk(0);
		return;
	}

	double min_pt = 1000000000.0;
	double tmp_pt = 1000000000.0;

	static double counter = 0;

	for(int i=0;i<NUM_PRESET_WALKPARAM;i++){
//		if(counter == 100)		OSYSPRINT(("Walk[%d].move_x = %f\n", i, PresetWalkParam::walkparams[i].move_x));
		double del_x  = dest_x - (pos_x + cos(pos_tt*3.14/180)*PresetWalkParam::walkparams[i].move_x - sin(pos_tt*3.14/180)*PresetWalkParam::walkparams[i].move_y);
		double del_y  = dest_y - (pos_y + sin(pos_tt*3.14/180)*PresetWalkParam::walkparams[i].move_x + cos(pos_tt*3.14/180)*PresetWalkParam::walkparams[i].move_y);
		double del_tt = dest_tt - (pos_tt + PresetWalkParam::walkparams[i].move_tt);

		double pt_x  = k_x*(del_x*del_x);
		double pt_y  = k_y*(del_y*del_y);
		double pt_tt = k_tt*(del_tt*del_tt);

		tmp_pt = pt_x + pt_y + pt_tt;
//		if(counter == 100)		OSYSPRINT(("Walk[%d].pt_x = %.1f , del_x = %.1f \n", i, pt_x, del_x));
//		if(counter == 100)		OSYSPRINT(("Walk[%d] tmp_pt = %.1f\n", i, tmp_pt));
		if(tmp_pt < min_pt){
			min_pt = tmp_pt;
			walk = i;
		}
	}
/*	
	if(counter > 200){
        OSYSPRINT(("Now Walk = %d\n", walk));
		counter = 0;
	}
	counter++;
*/
	SetPresetWalk(walk);

	return;


	double tx  = dest_x  - state.self.position.x.val;
	double ty  = dest_y  - state.self.position.y.val;
	double tt  = -state.self.position.tt.val;

	double dx = tx*cos(tt*3.14/180) - ty*sin(tt*3.14/180);	//AIBOのx軸方向への移動量
	double dy = tx*sin(tt*3.14/180) + ty*cos(tt*3.14/180);	//AIBOのy軸方向への移動量
	double dtt = dest_tt - state.self.position.tt.val;		//AIBOの回転量

	if(dtt < -180.0)	dtt += 360.0;
	if(dtt >= 180.0)	dtt -= 360.0;
	
	//目標値に十分接近しているとき
	if( (fabs(dx) < 200.0) && (fabs(dy) < 200.0) ){
		if(dtt < -130)		walk = WALK_ROTATE_RIGHT360;//270;//180;
		else if(dtt < -80)	walk = WALK_ROTATE_RIGHT360;//270;//180;
		else if(dtt < -20)	walk = WALK_ROTATE_RIGHT360;//270;//180;
		else if(dtt < 20){	walk = WALK_STOP; final_state = true;}				//終端状態
		else if(dtt < 80 )	walk = WALK_ROTATE_LEFT360;//270;//180;
		else if(dtt < 130)	walk = WALK_ROTATE_LEFT360;//270;//180;
		else				walk = WALK_ROTATE_LEFT360;//270;//180;
	}
	else{//目標値にまだまだ接近
		double goto_tt = atan2(dy, dx)*180.0/PI;
		if(goto_tt < -180.0)	goto_tt += 360.0;
		if(goto_tt >= 180.0)	goto_tt -= 360.0;

		if(goto_tt < -165)		walk = WALK_BACK;
		else if(goto_tt < -135)	walk = WALK_RIGHT150;
		else if(goto_tt < -105) walk = WALK_RIGHT120;
		else if(goto_tt < -75)	walk = WALK_RIGHT90;
        else if(goto_tt < -45)	walk = WALK_RIGHT60;
		else if(goto_tt < -15)	walk = WALK_RIGHT30;
		else if(goto_tt < 15)	walk = WALK_FORWARD;
		else if(goto_tt < 45)	walk = WALK_LEFT30;
		else if(goto_tt < 75)	walk = WALK_LEFT60;
		else if(goto_tt < 105)	walk = WALK_LEFT90;
		else if(goto_tt < 135)	walk = WALK_LEFT120;
		else if(goto_tt < 165)	walk = WALK_LEFT150;
		else					walk = WALK_BACK;
	}
    
	SetPresetWalk(walk);

	SetTailMode(TAIL_SWING);	
}

void BhvMoveToPosition::SetDestination(double x, double y, double tt){
	dest_x = x;
	dest_y = y;
	dest_tt = tt;
}
