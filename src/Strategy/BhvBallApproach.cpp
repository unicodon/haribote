#include "StdAfx.h"
#include "BehaviorList.h"

#include <OPENR/OSyslog.h>

void BhvBallApproach::Execute(){

	/*
	static bool ball_hold = false;//ボールを掴んだか
	static bool ball_appr = false;//ボール掴み動作へ入ったか
	static bool apr_flag  = false;//ボールを（おそらく）真正面に捕らえて前進歩行中か
	static bool ball_near = false;//ボールへちょこちょこ歩きで接近中か
	static bool drct_flag = false;
	static int hold_test = 0;
	static int appr_test = 0;
	static int param_test = 30;
	static int motion_cnt= 0;
	static int aft_motion = 0;//モーションをして，どれだけ経過したか．一定時間，モーションを出ないようにする
	static bool motion_flag = false;
	int walk = 0;

	if(
		state.ball.distance < 200.0 && 
		state.game.robot_id >= 3 && 
		state.self.position.x.val > 1000 && 
		fabs(state.self.position.tt.val) < 90 &&
		!ball_hold){
		bhvBallAppDirect.Execute();
		return;
	}

	//OSYSPRINT(("APPROACH\n"));


	if(motion_flag){
		aft_motion++;
		if(aft_motion > 60){
			aft_motion = 0;
			motion_flag = false;
			//ReleaseCall();
		}
		if(!state.ball.visible) SetHeadMode(HEAD_SWING_BALL_SEARCH);
		return;
	}

	if((state.ball.distance < 200.0) || ball_hold || ball_appr  || ball_near || apr_flag){
		bhvSearchBall.Initialize();
		if(ball_hold){
			//OSYSPRINT(("Now Grab\n"));
			hold_test++;
			
			if(hold_test < 10){//確実に掴むため
				SetHeadAngle(-75, 0, 40, -55, 200);
				walk = BALL_HOLD;
			}
			else{
				bool flag_shot = bhvBallApproach.HoldShot();
				if(flag_shot){
					motion_flag = true;
                    ball_hold = false;
					hold_test = 0;
					ReleaseCall();
					return;
				}
				else	return;
			}
		}

		//13cm以下に近付いたら以下の操作を実行
		else if((state.ball.distance < 130 || ball_appr) && apr_flag){
			//OSYSPRINT(("Grabing\n"));
			appr_test++;

			if(drct_flag){
				motion_cnt = 0;
				static int count_direct = 0;
				count_direct++;
				if(count_direct < param_test -10){
					SetPresetWalk(SMALL_FORWARD_FORELEG);
                    SetHeadMode(HEAD_TRACK);
				}
				else{//(count_direct > param_test-10){
                    drct_flag = false;
					apr_flag  = false;
					ball_near = false;
					motion_flag = true;
					count_direct = 0;
					//ReleaseCall();
					bhvBallAppDirect.AfterApproach();//Execute();
				}
		
				return;
			}

			if(appr_test == 1){
				ball_appr = true;
				//ReserveCall();
			}

			if(appr_test < param_test){
				walk = SMALL_FORWARD_FORELEG;//SMALL_FORWARD;//BALL_PREAPPROACH;
				SetHeadAngle(-40, 0, 45, -15, 200, true);
			}
			else if(appr_test < param_test+10){//20){
				walk = SMALL_FORWARD_FORELEG;//BALL_APPROACH;
				SetHeadAngle(-75, 0, 45, -35, 100, true);
			}
			else{
				ball_hold = true;
				ball_appr = false;
				apr_flag  = false;
				ball_near = false;
				appr_test = 0;
				SetHeadAngle(-75, 0, 40, -55, 200, true);
				SetPresetWalk(BALL_HOLD);
				return;
			}
		} 

		//13cm～30cmの間の動作
		else{
			//OSYSPRINT(("Approaching\n"));
			double b_direc = state.self.sensor.jointValue[PAN];//state.ball.direction;
			static int s_forward = 0;
			apr_flag = false;
			drct_flag = false;
			motion_cnt++;
			//ReserveCall();

			if(motion_cnt > 100){
				if(	state.self.position.x.val < -1000 || 
					fabs(state.self.position.y.val) > 1500 ||
					fabs(state.self.position.tt.val) > 90)
				{
					motion_cnt = 0;
				}//自陣にいるときは，掴む
				else drct_flag = true;
				//if(motion_cnt < 100)	walk = SMALL_FORWARD_FORELEG;
				//else{
                  //  motion_cnt = 0;
					//ball_near = false;
					//motion_flag = true;
					//ReleaseCall();
				//	bhvBallAppDirect.Execute();
				//}
				//return;
			}
			
			if(b_direc > 40){
				walk = WALK_STABLE_SIDELEFT;//WALK_HALFROTATE_LEFT;
				s_forward =0;
			}
			else if(b_direc > 30){//20){
				walk = WALK_STABLE_SIDELEFT;//WALK_HALFROTATE_LEFT;//FOURTHROTATE_LEFT;
				s_forward =0;
			}
			else if(b_direc < -40){
				walk = WALK_STABLE_SIDERIGHT;//WALK_HALFROTATE_RIGHT;
				s_forward =0;
			}
			else if(b_direc < -30){//-20){
				walk = WALK_STABLE_SIDERIGHT;//WALK_FOURTHROTATE_RIGHT;
				s_forward =0;
			}
			else{
				walk = SMALL_FORWARD_FORELEG;//SMALL_FORWARD;
				s_forward++;
				if(s_forward > 10){
					apr_flag = true;
					//motion_cnt = 0;
					ReserveCall();
				}
			}
			SetHeadMode(HEAD_TRACK_NEAR);
		}

		if(state.ball.distance > 400)	ball_near = false;
		else							ball_near = true;
	}

	//ボールまでの距離が30cm以上の時以下の操作を実行
	else{
		//bhvDPBallApproach.Execute();
		return;
	}
	SetPresetWalk(walk);
	return;*/
}

void BhvBallApproach::Test(){
}

void BhvBallApproach::BallRoll(){
}

bool BhvBallApproach::HoldShot(){
	double theta = state.self.position.tt.val;

	double goal_dirc = state.goalDirection;

	//ゴール真正面のときは，ドリブル突っ込み
	/*if(state.goalVisible  && state.self.position.x.val > 2000){
		SetPresetWalk(WALK_STABLE_FORWARD);
		return true;
	}*/

	if(theta < -80){// || (state.goalVisible && goal_dirc < 50)){
		SetHeadAngle(-60, 0, 45, -55, 300, true);
		SetPresetWalk(BALL_ROTATE_LEFT, true);
		return false;
	}
	else if(theta > 80){// || (state.goalVisible && goal_dirc > -50)){
		SetHeadAngle(-60, 0, 45, -55, 300, true);
		SetPresetWalk(BALL_ROTATE_RIGHT, true);
		return false;
	}
	else{
		//if(state.goalVisible && (fabs(goal_dirc) < 20))	SetMotion("GRABSHOOT_FRONT");
		if((fabs(goal_dirc) < 90) && state.goalVisible){
			//if(state.self.position.y.val > 0){
				if(goal_dirc > 70)			SetMotion("GRABSHOOT_LEFT");
				else if(goal_dirc > 50)		SetMotion("GRABSHOOT_FAKE_LEFT60");
				else if(goal_dirc > 30)		SetMotion("GRABSHOOT_FAKE_LEFT30");
				else if(goal_dirc > 10)		SetMotion("GRABSHOOT_FAKE_LEFT");
				else if(goal_dirc > -10)	SetMotion("GRABSHOOT_FRONT");
				else if(goal_dirc > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT");
				else if(goal_dirc > -50)	SetMotion("GRABSHOOT_FAKE_RIGHT30");
				else if(goal_dirc > -70)	SetMotion("GRABSHOOT_FAKE_RIGHT60");
				else						SetMotion("GRABSHOOT_FAKE_RIGHT");
			//}
			//else						SetMotion("GRABSHOOT_FRONT");
		}
		//else if(state.self.position.x.val < -1000 && fabs(theta) < 30) SetMotion("GRABSHOOT_FRONT");
		else{// if(state.self.position.x.val > -1000){
			//if(state.self.position.y.val < 0){
				if(theta > 70)				SetMotion("GRABSHOOT_RIGHT");
				else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT60");
				else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_RIGHT30");
				else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_RIGHT");
				else if(theta > -10)		SetMotion("GRABSHOOT_FRONT");
				else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_LEFT");
				else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_LEFT30");
				else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_LEFT60");
				else						SetMotion("GRABSHOOT_FAKE_LEFT");
			//}
			/*if(theta < -30)				SetMotion("GRABSHOOT_FAKE_LEFT");
			else if(theta > 30) 		SetMotion("GRABSHOOT_FAKE_RIGHT");
			else if(theta < -50)		SetMotion("GRABSHOOT_LEFT");
			else if(theta > 50)			SetMotion("GRABSHOOT_RIGHT");
			else if(goal_dirc < -50)	SetMotion("GRABSHOOT_RIGHT");
			else if(goal_dirc > 50)		SetMotion("GRABSHOOT_LEFT");
			else						SetMotion("GRABSHOOT_FRONT");*/
		}
        return true;
	}
}
