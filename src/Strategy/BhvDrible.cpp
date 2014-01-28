#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"
#include "../Motion/HeadController.h"
#include "../Motion/MotionMaker.h"

static DPMapLocalApproach dpmla;
static DPMapGoalieApproach dpmga;

#define X_RANGE 15
#define Y_RANGE 15

BhvDrible::BhvDrible() 
{
	lastball.direction = 0;
	lastball.distance = 20000;

	shot_motion = 0;
	hold_flag = false;
	last_app = false;
	shot_x = 0;
	shot_y = 0;
}

void BhvDrible::LastApproach(){
	
	if(hold_flag){
		SetFaceLED(FACE_THREE,true);
	}else{
		SetFaceLED(FACE_FOUR,true);
	} 

	if(!state.ball.visible && lastball.distance < 250){
		if(hold_flag){
			SetHeadMode(HEAD_BALL_HOLDING,true);
			SetPresetWalk(SMALL_FORWARD_FORELEG,true);
		}else{
			//SetHeadMode(HEAD_SWING_BALL_SEARCH);
			SetMotion(shot_motion);
			ReleaseCall();
			//state.last_approach = false;
		}
		last_app = false;
		lastball.distance = 20000;
		return;
	}

	if((state.ball.losttime > 10 || state.ball.distance > 600 || fabs(state.ball.direction) > 60) && HeadController::hold_state == HOLD_NOT){
//		OSYSPRINT(("BallLost\n"));
		ReleaseCall();
		//state.last_approach = false;
		lastball.distance = 20000;
		SetPresetWalk(WALK_BACK);
		last_app = false;
	}

//	OSYSPRINT(("last_app %d\n",last_app));


	int walk = dpmga.get_action((int)(state.ball.x), (int)(state.ball.y - shot_y),0);//dpmga.get_action(state.ball.x - shot_x + 70, state.ball.y - shot_y,0);
	if(walk == WALK_STOP || (fabs(state.ball.x - shot_x + 150) < X_RANGE && (fabs(state.ball.y - shot_y) < Y_RANGE) )){
//		OSYSPRINT(("%.1f,%.1f,%.1f,%.1f\n",shot_x,state.ball.x,shot_y,state.ball.y));
		if(hold_flag){
			SetHeadMode(HEAD_BALL_HOLDING,true);
			SetPresetWalk(SMALL_FORWARD_FORELEG,true);
//			OSYSPRINT(("%d\n",HeadController::hold_state));
		}else{
			//OSYSPRINT(("ball%.1f\n",state.ball.x));
			//SetHeadMode(HEAD_SWING_BALL_SEARCH);
			SetMotion(shot_motion);
			ReleaseCall();
			//state.last_approach = false;
		}
		last_app = false;
		lastball.distance = 20000;
		return;
	}else{
//		if(walk == WALK_STOP)
//			walk = WALK_BACK;
		SetHeadMode(HEAD_TRACK);
		SetPresetWalk(walk);
	}
	SetHeadMode(HEAD_TRACK);
	if(!state.ball.visible){
		lastball.distance = 20000;
	}

}



void BhvDrible::Execute(){
	//Execute_Simple();
	//return;

	if(!MotionMaker::IsFinished()){
		lastball.distance = 20000;
		return;
	}

	if(last_app){
//		OSYSPRINT(("lastapp\n"));
		LastApproach();
		return;
	}

	//念のため，ボールまでの距離・方向に応じて抜け出す処置
	if(state.ball.distance >= 500 || fabs(state.ball.direction) > 50.0){
		last_app = false;
		ReleaseCall();		
	}

	SetFaceLED(FACE_ARROW,true);

	if(HeadController::hold_state == HOLD_ING && hold_flag){
//		OSYSPRINT(("HOLDING\n"));
		SetHeadMode(HEAD_BALL_HOLDING,true);
		SetPresetWalk(SMALL_FORWARD_FORELEG,true);
		return;
	}

	if(HeadController::hold_state == HOLD_OK && hold_flag){
		if(bhvShot.HoldShot2()){
			ReleaseCall();
			//state.last_approach = false;
			//SetHeadMode(HEAD_SWING_BALL_SEARCH);
			lastball.distance = 20000;
		}
		else
			SetHeadMode(HEAD_BALL_HOLDING,true);
		return;
	}

	if(state.ball.visible){
		lastball = state.ball;
		HeadController::SetHeadMode(HEAD_TRACK);

		int x = (int)(state.ball.x-50);
		int y = (int)state.ball.y;
		int walk;

		if(state.ball.distance > 400){
			walk = dpmla.get_action(x,y,0);
		}else{
			walk = dpmga.get_action(x,y,0);//平行移動のみ
		}
		if(walk == WALK_STOP){//シュートの種類選択（掴む掴まないも）
			//hold_flag = !SelectShot(shot_motion,shot_x,shot_y);
			hold_flag = bhvShot.CalucShot(shot_motion,shot_x,shot_y);
			if(hold_flag){
				shot_x = 30;
				shot_y = 0;
			}

			SetHeadMode(HEAD_BALL_HOLDING,true);
			SetPresetWalk(SMALL_FORWARD_FORELEG,true);

			last_app = true;
			LastApproach();
			ReserveCall();
			//state.last_approach = true;
			return;
		}
		double dir = state.ball.direction;
		if(dir < -55)
			walk = WALK_ROTATE_RIGHT180;
		if(dir > 55)
			walk = WALK_ROTATE_LEFT180;
		SetFaceLED(FACE_ONE);
		SetPresetWalk(walk);
	}else{
		if(state.ball.losttime > 60)
			ReleaseCall();
		SetHeadMode(HEAD_TRACK);

	}

//	OSYSPRINT(("%.1f\n",state.self.sensor.chestPSD / 1000));
//	bhvSearchBall.Execute();
}

void BhvDrible::Execute_Simple(){
	SetHeadMode(HEAD_TRACK);
	SetFaceLED(FACE_FIVE);

	double dist = state.ball.distance;
	double dir = state.ball.direction;
	int x = (int)state.ball.x;
	int y = (int)state.ball.y;

	if(dir < -50){
		SetPresetWalk(WALK_ROTATE_RIGHT180);
	}
	if(dir > 50){
		SetPresetWalk(WALK_ROTATE_LEFT180);
	}

	int rtt = (int)state.self.position.tt.val;


	int walk = dpmla.get_action(x+50,y,0);

	if(walk == 0){
		SetMotion(SelectShotSimple());
		return;
	}
	SetPresetWalk(walk);
}
