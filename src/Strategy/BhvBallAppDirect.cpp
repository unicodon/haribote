#include "StdAfx.h"
#include "BehaviorList.h"

#include <OPENR/OSyslog.h>

void BhvBallAppDirect::Execute(){
	/*
	static int aft_motion = 0;//モーションをして，どれだけ経過したか．一定時間，モーションを出ないようにする
	static bool motion_flag = false;

	if(motion_flag){
		aft_motion++;
		if(aft_motion > 60){
			aft_motion = 0;
			motion_flag = false;
		}
		if(!state.ball.visible) SetHeadMode(HEAD_SWING_BALL_SEARCH);
		return;
	}

	static int s_forward = 0;
	if(state.ball.losttime > 40 || s_forward > 40){
		s_forward = 0;
		return;
	}

	double goal_dirc = state.goalDirection;
	double theta = state.self.position.tt.val;

	double b_direc = state.self.sensor.jointValue[PAN];//state.ball.direction;

	int walk = 0;
	bhvSearchBall.Initialize();
	
	if(b_direc > 40){
		walk = WALK_STABLE_SIDELEFT;//WALK_HALFROTATE_LEFT;
		s_forward = 0;
	}
	else if(b_direc > 30){//20){
		walk = WALK_STABLE_SIDELEFT;//WALK_FOURTHROTATE_LEFT;
		s_forward = 0;
	}
	else if(b_direc < -40){
		walk = WALK_STABLE_SIDERIGHT;//WALK_HALFROTATE_RIGHT;
		s_forward = 0;
	}
	else if(b_direc < -30){//-20){
		walk = WALK_STABLE_SIDERIGHT;//WALK_FOURTHROTATE_RIGHT;
		s_forward = 0;
	}
	else{
		//walk = SMALL_FORWARD_FORELEG;//SMALL_FORWARD;
		s_forward++;
		if(s_forward > 30){
			//SetHeadMode(HEAD_TRACK);
			SetFaceLED(FACE_GREENWAVE);
			s_forward = 0;


			motion_flag = true;
			if(state.goalVisible && fabs(goal_dirc) < 90){
				if(goal_dirc > 30) 			SetMotion("HEADING_LEFT");
				//else if(goal_dirc > 30)		SetMotion("SMASHHIT_LEFT");
				else if(goal_dirc > -30)	SetMotion("SHOOT_FRONT");
				//else if(goal_dirc > -60)	SetMotion("SMASHHIT_RIGHT");
				else						SetMotion("HEADING_RIGHT");
//				state.ball.visible = false;
				return;
			}
			else{
				double theta = state.self.position.tt.val;

				if(state.self.position.x.val > 2000){
					if(state.self.position.y.val > 0 && theta > 0)		SetMotion("HEADING_RIGHT");
					else if(state.self.position.y.val > 0 && theta < 0)	SetMotion("HEADING_LEFT");
				}
				if(state.self.position.y.val > 0){
					if(theta > 120)			SetMotion("HEADING_LEFT");
					else if(theta > 30)		SetMotion("HEADING_RIGHT");
					//else if(theta > 30)		SetMotion("SMASHHIT_RIGHT");
					else if(theta > -60)	SetMotion("SHOOT_FRONT");
					//else if(theta > -80)	SetMotion("SMASHHIT_LEFT");
					else if(theta > -120)	SetMotion("HEADING_LEFT");
					else					SetMotion("HEADING_LEFT");//RIGHT");
				 }
				else{
					if(theta > 120)			SetMotion("HEADING_RIGHT");
					else if(theta > 60)		SetMotion("HEADING_RIGHT");//LEFT");
					//else if(theta > 30)		SetMotion("SMASHHIT_RIGHT");
					else if(theta > -30)	SetMotion("SHOOT_FRONT");
					//else if(theta > -80)	SetMotion("SMASHHIT_LEFT");
					else if(theta > -120)	SetMotion("HEADING_LEFT");//RIGHT");
					else					SetMotion("HEADING_RIGHT");//LEFT");
				}
			}
			state.ball.visible = false;
			return;
		}
		walk = SMALL_FORWARD_FORELEG;//SMALL_FORWARD;
	}
	SetFaceLED(0x8C3);
	SetHeadMode(HEAD_TRACK_NEAR);
	SetPresetWalk(walk);*/
}

void BhvBallAppDirect::AfterApproach(){
	double goal_dirc = state.goalDirection;
	double theta = state.self.position.tt.val;

	if(state.goalVisible && fabs(goal_dirc) < 80){
		if(goal_dirc > 40) 			SetMotion("HEADING_LEFT");
		//else if(goal_dirc > 30)		SetMotion("SMASHHIT_LEFT");
		else if(goal_dirc > -40)	SetMotion("SHOOT_FRONT");
		//else if(goal_dirc > -60)	SetMotion("SMASHHIT_RIGHT");
		else						SetMotion("HEADING_RIGHT");
		return;
	}
	else{
		double theta = state.self.position.tt.val;

		if(state.self.position.x.val > 2000){
			if(state.self.position.y.val > 0)	SetMotion("HEADING_RIGHT");
			else								SetMotion("HEADING_LEFT");
			return;
		}
		if(state.self.position.y.val > 0){
			if(theta > 120)			SetMotion("HEADING_LEFT");
			else if(theta > 30)		SetMotion("HEADING_RIGHT");
			//else if(theta > 30)		SetMotion("SMASHHIT_RIGHT");
			else if(theta > -60)	SetMotion("SHOOT_FRONT");
			//else if(theta > -60)	SetMotion("SMASHHIT_LEFT");
			else if(theta > -120)	SetMotion("HEADING_LEFT");
			else					SetMotion("HEADING_LEFT");//RIGHT");
			return;
			}
		else{
			if(theta > 120)			SetMotion("HEADING_RIGHT");
			else if(theta > 60)		SetMotion("HEADING_RIGHT");//LEFT");
			//else if(theta > 30)		SetMotion("SMASHHIT_RIGHT");
			else if(theta > -30)	SetMotion("SHOOT_FRONT");
			//else if(theta > -80)	SetMotion("SMASHHIT_LEFT");
			else if(theta > -120)	SetMotion("HEADING_LEFT");//RIGHT");
			else					SetMotion("HEADING_RIGHT");//LEFT");
			return;
		}
	}

//	if(theta < -70)				SetMotion("HEADING_2LEFT");
//	else if(theta > 70)			SetMotion("HEADING_2RIGHT");
//	if(state.self.position.tt.val < -50)		SetMotion("HEADING_2LEFT");
//	else if(state.self.position.tt.val > 50)	SetMotion("HEADING_2RIGHT");
//	else if(goal_dirc < -40)			SetMotion("HEADING_2RIGHT");
//	else if(goal_dirc > 40)		SetMotion("HEADING_2LEFT");
//	else						SetMotion("SHOOT_FRONT");
	return;
}
