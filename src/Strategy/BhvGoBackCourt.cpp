#include "StdAfx.h"
#include "BehaviorList.h"

void BhvGoBackCourt::Execute(){
	static int court_out = 0;

	if(state.ball.visible){
		court_out = 0;
		return;
	}


	double pos_x = state.self.position.x.val;
	double pos_y = state.self.position.y.val;
	
	//自己位置おかしい場合
	if(	pos_x >  3000.0 ||
		pos_x < -3000.0 ||
		pos_y >  2000.0 ||
		pos_y < -2000.0
		)
		court_out++;
	else court_out = 0;

	if(court_out > 10){
		SetFaceLED(FACE_BATSU);
		if(court_out > 50){
			SetPresetWalk(WALK_BACK);//WALK_STABLE_ROTATERIGHT);//WALK_FOURTHROTATE_RIGHT);
		}
		SetHeadMode(HEAD_SWING_LOCALIZATION);
		SetPresetWalk(WALK_STOP);
            
		return;
		//bhvMoveToPosition.SetDestination(0, 0, 0);//AIBOのRobotIDで場合わけをする
	}

	//コートから出そうな場合
	/*
	int walk = 0;
	if( 
		(state.field.free_direction[6] == SPACE_NONE ||
		state.field.free_direction[7] == SPACE_NONE ||
		state.field.free_direction[8] == SPACE_NONE ||
		state.field.free_direction[9] == SPACE_NONE ||
		state.field.free_direction[10] == SPACE_NONE ||
		state.field.free_direction[11] == SPACE_NONE ||
		state.field.free_direction[12] == SPACE_NONE ) &&
		!state.ball.visible
	){
		if(pos_y < 0)		walk = WALK_STABLE_ROTATELEFT;
		else if(pos_y >= 0)	walk = WALK_STABLE_ROTATERIGHT;
		SetPresetWalk(walk);
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetFaceLED(FACE_LOST_FIELD);
		return;
	}
	*/

	//自軍ゴールに入りそうな場合

	return;
}
