#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"
#include "../Motion/HeadController.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/GaitMaker.h"

static DPMapLocalApproach dpmla;
static DPMapGoalieApproach dpmga;

#define ADJ_X 100 //ADJ_Xを大きくすると遠い距離でLastApproach()に入ります

BhvLastApproach::BhvLastApproach() 
{
	goalinfolastapp.visible=false;
	goalinfolastapp.drct=0;
	shot_motion = 0;
	hold_flag = false;
	last_app = false;
	adjust_ball_dis =170;
	//shot_x = 0;
	//shot_y = 0;
	//lastball.direction = 0;
	//lastball.distance = 20000;	
}

void BhvLastApproach::LastApproach(){
	SetFaceLED(FACE_SLEEP, true);

	if(!hold_flag){//掴まない時
		
		//LastApproach中にゴールが見えた時シュート選択を更新
		if(state.enemyGoalVisible){
			hold_flag = bhvShotChoose.ShotChoose(shot_motion,adjust_ball_dis);
		}
		
		//ボール距離が閾値をわったらシュート
		if(state.ball.distance < adjust_ball_dis){
			ReleseExe();
			SetMotion(shot_motion);
			return;
		}
		//BhvLastApproach()を抜け出す処理
		if(state.ball.losttime > 20 || fabs(state.ball.direction) > 40 || state.ball.distance > 400){
			ReleseExe();
			return;
		}

		SetHeadMode(HEAD_TRACK);

		if(state.ball.direction>30)			SetPresetWalk(SMALL_LEFT60_FORELEG);
		else if(state.ball.direction>10)	SetPresetWalk(SMALL_LEFT45_FORELEG);
		else if(state.ball.direction>-10)	SetPresetWalk(SMALL_FORWARD_FORELEG);
		else if(state.ball.direction>-30)	SetPresetWalk(SMALL_RIGHT45_FORELEG);
		else								SetPresetWalk(SMALL_RIGHT60_FORELEG);
		
	}else{//掴むとき
		
		if(HeadController::hold_state == HOLD_OK){
			if(bhvHoldShot.HoldShot())	ReleseExe();
			else						SetHeadMode(HEAD_BALL_HOLDING);
			return;
		}
		
		//LastApproach中にゴールが見えた時,掴まないシュートに変更される(hold_flag=falseになる)
		if(state.enemyGoalVisible){
			hold_flag = bhvShotChoose.ShotChoose(shot_motion,adjust_ball_dis);
		}
	
		//以下BhvLastApproach()を抜け出す処理
		if(state.ball.losttime > 20 && HeadController::hold_state == HOLD_NOT){
			ReleseExe();
			return;
		}
		if(fabs(state.ball.direction) > 40 && HeadController::hold_state == HOLD_ING){
			ReleseExe();
			return;
		}
		if(state.ball.distance > 400 && HeadController::hold_state == HOLD_ING){
			ReleseExe();
			return;
		}

		SetHeadMode(HEAD_BALL_HOLDING);

		if(state.ball.direction>30)			SetPresetWalk(SMALL_LEFT60_FORELEG);
		else if(state.ball.direction>10)	SetPresetWalk(SMALL_LEFT45_FORELEG);
		else if(state.ball.direction>-10)	SetPresetWalk(SMALL_FORWARD_FORELEG);
		else if(state.ball.direction>-30)	SetPresetWalk(SMALL_RIGHT45_FORELEG);
		else								SetPresetWalk(SMALL_RIGHT60_FORELEG);

		
	}
}


void BhvLastApproach::Execute(){
	SetFaceLED(FACE_ARROW,true);

	if(!MotionMaker::IsFinished()){
		return;
	}

	//ゴール情報の更新(この情報がBhvShotChoose()で利用される)
	if(state.enemyGoalVisible){
		goalinfolastapp.visible=true;
		goalinfolastapp.drct=(int)(state.goalDirection);
	}

	if(last_app){
		LastApproach();
		return;
	}

	HeadController::SetHeadMode(HEAD_TRACK);

	//Execute()を抜け出す条件
	if(state.ball.distance >= 700 || fabs(state.ball.direction) > 50.0 || state.ball.losttime > 60){
		ReleseExe();
		return;
	}

	if(state.ball.visible || state.ball.losttime<10){
		int x = (int)(state.ball.x-ADJ_X);
		int y = (int)(state.ball.y);
		int walk = dpmga.get_action(x,y,0);
		if(walk == WALK_STOP){
			hold_flag = bhvShotChoose.ShotChoose(shot_motion,adjust_ball_dis);//シュートの種類選択
			ReserveLastApp();
			return;
		}
		SetPresetWalk(walk);
	}
}

void BhvLastApproach::ReserveLastApp()
{
	last_app = true;
	ReserveCall();
	LastApproach();
}

void BhvLastApproach::ReleseLastApp()
{
	last_app = false;
	goalinfolastapp.visible=false;
	//SetHeadMode(HEAD_TRACK,true);
}

void BhvLastApproach::ReleseExe()
{
	last_app = false;
	goalinfolastapp.visible=false;
	ReleaseCall();
	//SetHeadMode(HEAD_TRACK,true);
}

