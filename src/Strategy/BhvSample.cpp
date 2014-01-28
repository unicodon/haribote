#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "../Motion/GaitMaker.h"
#include "BehaviorList.h"
#include "../Motion/HeadController.h"


BhvSample::BhvSample()
{
}

BhvSample::~BhvSample()
{
}

void BhvSample::Execute(){
	//回転シュートの行き過ぎ量計測のタメのコード
	static bool swc=true;
	static TURN_MODE turn_mode=MODE_DEFAULT;
	static int overshoot=0;

    //タイムラグによりovershoot[deg]多く回転した後のゴール方向
	int after_goal_direc=(int)(state.goalDirection+overshoot);

	if(swc){
		if(state.enemyGoalVisible){
			if(after_goal_direc > 30)			SetMotion("GRABSHOOT_FAKE_LEFT40");
			else if(after_goal_direc > 10)		SetMotion("GRABSHOOT_FAKE_LEFT20");
			else if(after_goal_direc > 0)		SetMotion("GRABSHOOT_FAKE_LEFT");
			else if(after_goal_direc > -10)		SetMotion("GRABSHOOT_FAKE_RIGHT");
			else if(after_goal_direc > -30)		SetMotion("GRABSHOOT_FAKE_RIGHT20");
			else								SetMotion("GRABSHOOT_FAKE_RIGHT40");

			//OSYSPRINT(("goaldrc=%.1f,overshoot=%d,afterdrc=%d\n",state.goalDirection,overshoot,after_goal_direc));
			SetPresetWalk(0);
			swc=false;
			SetHeadMode(HEAD_SWING_LOCALIZATION,true);
			return;
		}

		SetHeadMode(HEAD_BALL_HOLDING);
		switch(turn_mode)
		{
			case MODE_DEFAULT:
				if(state.goalDirection>0)	turn_mode=MODE_LEFT;
				else						turn_mode=MODE_RIGHT;
				break;

			case MODE_RIGHT:
				SetPresetWalk(BALL_ROTATE_RIGHT180,1, true);
				overshoot=30;
				break;

			case MODE_LEFT:
				SetPresetWalk(BALL_ROTATE_LEFT180,1, true);
				overshoot=-30;
				break;
		}
	}
	
	if(state.self.sensor.backTouch[2]){
		SetHeadMode(HEAD_BALL_HOLDING);
		swc=true;
		turn_mode=MODE_DEFAULT;
		//SoundController::SetSound(SOUND_SHOT);
		//SoundController::Play();
	}

	return;


//回転シュート決めうちvar.テスト
/*	static int swith=1;
	static int counts=0;
	int rotate_num=0;
	double dead_rec=22.0;

	int posx =(int)(state.self.position.x.val);
	int posy =(int)(state.self.position.y.val);
	int post =(int)(state.self.position.tt.val);
	
	rotate_num=(int)(abs(post)/dead_rec);

	counts++;
	if(counts==20){
		//SetHeadMode(HEAD_BALL_HOLDING);
		if(post>20){
			SetPresetWalk(BALL_ROTATE_RIGHT,rotate_num, true);
			OSYSPRINT(("right,tt=%d,rot=%d\n",post,rotate_num));
		}else if(post>-20){
			SetPresetWalk(0);
			OSYSPRINT(("stay,tt=%d,rot=%d\n",post,rotate_num));
		}else{
			SetPresetWalk(BALL_ROTATE_LEFT,rotate_num,true);
			OSYSPRINT(("left,tt=%d,rot=%d\n",post,rotate_num));
		}
	}
	if(counts>20&&GaitMaker::IsStop()&&swith==1){
		SetMotion("GRABSHOOT_FAKE_LEFT");
		OSYSPRINT(("shot,tt=%d,counts=%d\n",post,counts));
		swith=0;
	}

	SetHeadMode(HEAD_SWING_LOCALIZATION);

	if(state.self.sensor.backTouch[2]){
			swith=1;
			counts=0;
			SoundController::SetSound(SOUND_SHOT);
			SoundController::Play();
	}
	return;*/
/*
	//ボールホールドで歩いてみる
	SetHeadMode(HEAD_HOLD);
	int walk;
	if(
		state.field.free_direction[4] == SPACE_FREE &&
		state.field.free_direction[5] == SPACE_FREE &&
		state.field.free_direction[6] == SPACE_FREE 
	){
		walk = WALK_STABLE_FORWARD;
	}else if(
		state.field.free_direction[8] == SPACE_FREE &&
		state.field.free_direction[9] == SPACE_FREE 
	){
		walk = WALK_STABLE_SIDERIGHT;
	}else if(
		state.field.free_direction[1] == SPACE_FREE &&
		state.field.free_direction[2] == SPACE_FREE
	){
		walk = WALK_STABLE_SIDELRFT;
	}else{
		walk = WALK_HALFROTATE_RIGHT;
	}
	SetPresetWalk(walk);
	SetTailMode(TAIL_SWING);
*/
/*
	//障害物でストップ
	SetHeadAngle(-10,0,0,0,200);

	int walk;
	if(
		state.field.free_direction[4] == SPACE_FREE &&
		state.field.free_direction[5] == SPACE_FREE &&
		state.field.free_direction[6] == SPACE_FREE
	){
		SetTailMode(TAIL_STOP);
		walk = WALK_STABLE_FORWARD;
	}else{
		SetTailMode(TAIL_SWING);
		walk = WALK_STOP;
	}
	SetPresetWalk(walk);
	return;
*/


/*
	//ボール追いかけ
	if(state.ball.visible){
		SetHeadMode(HEAD_TRACK);
		int walk = 0;
		double pan = state.self.sensor.jointValue[PAN];
		if(pan < -50){
			walk = WALK_HALFROTATE_RIGHT;
		}else if(pan < -35){
			walk = WALK_ROLL_RIGHT_R20;
		}else if(pan < -20){
			walk = WALK_ROLL_RIGHT_R40;
		}else if(pan < 20){
			walk = WALK_STABLE_FORWARD;
		}else if(pan < 35){
			walk = WALK_ROLL_LEFT_R40;
		}else if(pan < 50){
			walk = WALK_ROLL_LEFT_R20;
		}else{
			walk = WALK_HALFROTATE_LEFT;
		}

		SetTailMode(TAIL_SWING);

		SetPresetWalk(walk);
	}else{
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetTailMode(TAIL_STOP);
	}
*/

	static bool st_flag = false;
	if(!st_flag){
		/*
		Logger lg(/MS/test.txt, true);
		lg.Register();
		st_flag = true;*/
		
	}
/*

	//ボールトラック
	if(state.ball.visible){
		SetHeadMode(HEAD_TRACK);
		SetTailMode(TAIL_STOP);
		SetPresetWalk(0);
	}else{
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetTailMode(TAIL_STOP);
		SetPresetWalk(0);
	}

	static bool write = false;
	static vector<double> r_dist;
	static vector<double> r_drct;

	int size = r_dist.size();

	//OSYSPRINT(("Now size = %d\n", size));
	static int cnt = 0;
	cnt++;
	if(cnt == 15){
		//OSYSPRINT(("Now Ball Dist. = %.1f\n", state.ball.distance));
		cnt = 0;
	}

	if(size < 100){//cnt < 100){
		SetFaceLED(FACE_A_ALL);
		//OSYSPRINT(("PUSH!!!"));

		double dist = state.ball.distance;
		double drct = state.ball.direction;
		r_dist.push_back(dist);
		r_drct.push_back(drct);
		return;
	}
	else if(!write){
		FILE *fp;
		fp = fopen("./MS/dist.txt", "a+");
		if(!fp)return;
		double dist_total = 0;
		double drct_total = 0;
		//int size = r_dist.size();
		for(int i=0;i<100;i++){
			//OSYSPRINT(("%.1f, ", r_dist[i]));
			fprintf(fp,"%.1f, %.1f\n",r_dist[i],r_drct[i]);
			dist_total += r_dist[i];
			drct_total += r_drct[i];
		}
		fprintf(fp, "\n");
		fprintf(fp,"%.1f,%.1f",dist_total/100.0,drct_total/100.0);

		fprintf(fp, "\n");

		fclose(fp);
		write = true;
		SetFaceLED(FACE_FIVE);
	}
	else{
		SetFaceLED(FACE_FIVE);
	}

	if(state.self.sensor.backTouch[0]){
		//r_dist.clear();
		size = r_dist.size();
		for(int i=0;i<size;i++)	r_dist.pop_back();
		for(int i=0;i<size;i++)	r_drct.pop_back();
		//cnt = 0;
		write = false;
	}*/
//	state.self.sensor.headNearPSD
	//ボールアプローチ takeshita 0421
	/*
	static bool ball_hold = false;
	static bool ball_appr = false;
	static bool apr_flag  = false;
	static bool ball_near = false;
	static int hold_test = 0;
	static int appr_test = 0;
	
	if((state.ball.visible || ball_hold || ball_appr) || ball_near){
		static int walk = 0;
		if((state.ball.distance < 400) || ball_hold || ball_appr || apr_flag){
			if(ball_hold){
				if(hold_test > 50){
					ball_hold = false;
					ball_near = false;
					hold_test = 0;
					return;
				}
				if(hold_test > 10)	SetHeadAngle(-60, 0, 45, -55, 300);
				else				SetHeadAngle(-75, 0, 40, -55, 200);
				hold_test++;

				walk = BALL_HOLD;
			}
			else if((state.ball.distance < 130 || ball_appr) && apr_flag){
				ball_appr = true;
				if(appr_test > 30){
					ball_hold = true;
					ball_appr = false;
					apr_flag  = false;
					appr_test = 0;
					return;
				}
				//if(appr_test > 6)	SetHeadAngle(-75, 0, 45, -25, 150);
				//else 				SetHeadAngle(0, 0, 45, 0, 150);//(0, 0, 25, -20, 100);
				if(appr_test < 18){
					walk = SMALL_FORWARD;//BALL_PREAPPROACH;
					SetHeadAngle(-40, 0, 45, -5, 200);
				}
				else{
					walk = BALL_APPROACH;
					SetHeadAngle(-75, 0, 45, -25, 360);
				}
				appr_test++;
			} 
			else{
				double b_direc = state.self.sensor.jointValue[PAN];//state.ball.direction;
				apr_flag = false;
				if(b_direc > 30)			walk = WALK_HALFROTATE_LEFT;
				else if(b_direc > 15)		walk = WALK_FOURTHROTATE_LEFT;
				else if(b_direc < -30)		walk = WALK_HALFROTATE_RIGHT;
				else if(b_direc < -15)		walk = WALK_FOURTHROTATE_RIGHT;
				else{
					walk = SMALL_FORWARD;
					apr_flag = true;
				}
				SetHeadMode(HEAD_TRACK2);
			}
			if(state.ball.distance > 550)	ball_near = false;
			else							ball_near = true;
		}
		else{
			SetHeadMode(HEAD_TRACK);
			double pan = state.self.sensor.jointValue[PAN];
			if(pan < -50){
				walk = WALK_HALFROTATE_RIGHT;
			}else if(pan < -35){
				walk = WALK_ROLL_RIGHT_R20;
			}else if(pan < -20){
				walk = WALK_ROLL_RIGHT_R40;
			}else if(pan < 20){
				walk = WALK_STABLE_FORWARD;
			}else if(pan < 35){
				walk = WALK_ROLL_LEFT_R40;
			}else if(pan < 50){
				walk = WALK_ROLL_LEFT_R20;
			}else{
				walk = WALK_HALFROTATE_LEFT;
			}

			SetTailMode(TAIL_SWING);
		}
		//walk = 0;
		SetPresetWalk(walk);
	}
	else{
		static int count = 0;
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetTailMode(TAIL_STOP);

		count++;
		if(count > 30){
			SetPresetWalk(WALK_HALFROTATE_LEFT);
			if(count == 60) count = 0;
		}
        else	SetPresetWalk(WALK_STOP);
	}*/
}
