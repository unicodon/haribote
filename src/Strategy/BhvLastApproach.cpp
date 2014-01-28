#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"
#include "../Motion/HeadController.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/GaitMaker.h"

static DPMapLocalApproach dpmla;
static DPMapGoalieApproach dpmga;

#define ADJ_X 100 //ADJ_X��傫������Ɖ���������LastApproach()�ɓ���܂�

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

	if(!hold_flag){//�͂܂Ȃ���
		
		//LastApproach���ɃS�[�������������V���[�g�I�����X�V
		if(state.enemyGoalVisible){
			hold_flag = bhvShotChoose.ShotChoose(shot_motion,adjust_ball_dis);
		}
		
		//�{�[��������臒l���������V���[�g
		if(state.ball.distance < adjust_ball_dis){
			ReleseExe();
			SetMotion(shot_motion);
			return;
		}
		//BhvLastApproach()�𔲂��o������
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
		
	}else{//�͂ނƂ�
		
		if(HeadController::hold_state == HOLD_OK){
			if(bhvHoldShot.HoldShot())	ReleseExe();
			else						SetHeadMode(HEAD_BALL_HOLDING);
			return;
		}
		
		//LastApproach���ɃS�[������������,�͂܂Ȃ��V���[�g�ɕύX�����(hold_flag=false�ɂȂ�)
		if(state.enemyGoalVisible){
			hold_flag = bhvShotChoose.ShotChoose(shot_motion,adjust_ball_dis);
		}
	
		//�ȉ�BhvLastApproach()�𔲂��o������
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

	//�S�[�����̍X�V(���̏��BhvShotChoose()�ŗ��p�����)
	if(state.enemyGoalVisible){
		goalinfolastapp.visible=true;
		goalinfolastapp.drct=(int)(state.goalDirection);
	}

	if(last_app){
		LastApproach();
		return;
	}

	HeadController::SetHeadMode(HEAD_TRACK);

	//Execute()�𔲂��o������
	if(state.ball.distance >= 700 || fabs(state.ball.direction) > 50.0 || state.ball.losttime > 60){
		ReleseExe();
		return;
	}

	if(state.ball.visible || state.ball.losttime<10){
		int x = (int)(state.ball.x-ADJ_X);
		int y = (int)(state.ball.y);
		int walk = dpmga.get_action(x,y,0);
		if(walk == WALK_STOP){
			hold_flag = bhvShotChoose.ShotChoose(shot_motion,adjust_ball_dis);//�V���[�g�̎�ޑI��
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

