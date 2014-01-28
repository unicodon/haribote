#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"
#include "../Motion/PresetMotion.h"
#include "../Motion/GaitMaker.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/HeadController.h"

#define WAIT_DIST 1700
#define CHASE_DISTANCE 900
#define QUIT_CHASE_DISTANCE 1200
#define BALL_CLEAR_DISTANCE 180
#define GOALPIXEL_TH 20000


extern DPMapLocalApproach dpmla;
extern DPMapGoalieApproach dpmga;

static MoveToPosPotential m2pPot;



BhvGoalie2006::BhvGoalie2006()
{
	mode = RETURN_GOAL_MOUTH;
	savingflag = false;
	savingmotion = GOALIE_BLOCK_LEFTWALK;
	gmcnt = 0;
}

void BhvGoalie2006::Reset()
{
	mode = GOALIE_ADJUST_POSITION;//RETURN_GOALIE_POSITION;
	savingflag = false;
	savingmotion = GOALIE_BLOCK_LEFTWALK;
	gmcnt = 0;
}

void BhvGoalie2006::ResetForPenalize()
{
	mode = RETURN_GOAL_MOUTH;
	savingflag = false;
	savingmotion = GOALIE_BLOCK_LEFTWALK;
	gmcnt = 0;
}


void BhvGoalie2006::Execute()
{
	switch(mode)
	{
		case GO_BACK:
			GoBack();
			break;

		case RETURN_GOALIE_POSITION:
			SetFaceLED(FACE_ONE);
			SetTailMode(TAIL_STOP);
			ReturnToGoaliePosition();
			break;

		case GOALIE_ADJUST_POSITION:
			SetFaceLED(FACE_TWO);
			SetTailMode(TAIL_ANGRY);
			AdjustPosition();
			break;

		case GOALIE_WAIT:
			SetFaceLED(FACE_THREE);
			SetTailMode(TAIL_SWING);
			Wait();
			break;

		case GOALIE_CHASE:
			SetFaceLED(FACE_FOUR);
//			OSYSPRINT(("%.1f\n",state.ball.distance));
			ChaseBall();
			SetTailMode(TAIL_ANGRY);
			break;

		case GOALIE_CLEAR:
			SetFaceLED(FACE_FIVE);
			Clear();
			SetTailMode(TAIL_STOP);
			break;

		case RETURN_GOAL_MOUTH:
			SetFaceLED(FACE_A_ALL);
			ReturnGoalMouth();
			break;
	}
}

void BhvGoalie2006::ReturnGoalMouth()
{
	bool ball_in_area = CheckBallPosition();

	if(Saving())
	{
		mode = GOALIE_WAIT;
		return;
	}

	if(state.ball.visible && 
	   state.ball.distance < CHASE_DISTANCE && 
	   ball_in_area)
	{
		SetHeadMode(HEAD_TRACK);
		mode = GOALIE_CHASE;
		return;
	}

	int walk;

	static int cnt_track = 0;
	if(state.ball.visible || state.ball.losttime < 20)
	{
		cnt_track++;
		if(cnt_track < 100 || state.ball.distance < 1000)
		{
			SetHeadMode(HEAD_TRACK);
		}
		else if(cnt_track < 120)
		{
			SetHeadMode(HEAD_WATCHING);//HEAD_SWING_LOCALIZATION);
		}
		else						cnt_track = 0;
	}
	else
	{
		if(cnt_track > 100)	cnt_track++;
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
	}

	walk = m2pPot.GetWalk();
	SetPresetWalk(walk);

	if(InSpot(-2400, 0))
	{
		mode = RETURN_GOALIE_POSITION;
	}
	return;

	//ゴールの中へ戻る前に戻る場所の指定
	int dest_x = -2300;
	int dest_y = 0;
	int dest_t = 0;//0;//-180;

	if(InSpot(dest_x, dest_y))
	{
		mode = RETURN_GOALIE_POSITION;
	}

	bhvMoveToPosDP.SetDestination(dest_x, dest_y, dest_t);
	bhvMoveToPosDP.Execute();
}

void BhvGoalie2006::ReturnToGoaliePosition()
{
	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	int pos_t = (int)state.self.position.tt.val;

	if(Saving())
	{
		mode = GOALIE_WAIT;
		return;
	}

	bool ball_in_area = CheckBallPosition();
	if((state.ball.visible || state.ball.losttime < 10) && 
	   state.ball.distance < CHASE_DISTANCE && 
	   ball_in_area)
	{
		SetHeadMode(HEAD_TRACK);
		mode = GOALIE_CHASE;
		return;
	}

	static int cnt_track = 0;
	if(state.ball.visible || state.ball.losttime < 20)
	{
		cnt_track++;
		if(cnt_track < 100 || state.ball.distance < 1000)
		{
			SetHeadMode(HEAD_TRACK);
		}
		else if(cnt_track < 120)
		{
			SetHeadMode(HEAD_WATCHING);//HEAD_SWING_LOCALIZATION);
		}
		else						cnt_track = 0;
	}
	else
	{
		if(cnt_track > 100)	cnt_track++;
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
	}

	int dest_x = -2550;
	int dest_y = 0;
	int dest_t = 0;

	static int cnt_in_spot;
	if(InSpotTheta(dest_x, dest_y, dest_t))
	{
		cnt_in_spot++;
		if(cnt_in_spot > 3)
		{
            mode = GOALIE_ADJUST_POSITION;
            return;
		}
	}
	else if( pos_x < -3000 || abs(pos_y) > 350)//変なところに居たらゴールに戻る
	{
		mode = RETURN_GOAL_MOUTH;
		cnt_in_spot = 0;
		return;
	}
	else
	{
		cnt_in_spot = 0;
	}

	bhvMoveToPosDP.SetDestination(dest_x, dest_y, dest_t);
	bhvMoveToPosDP.Execute();

}

void BhvGoalie2006::Wait()
{
	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	int pos_t = (int)state.self.position.tt.val;

	static bool moving = false;

	//SetHeadMode(HEAD_TRACK_KP);

	static int cnt_track = 0;
	if(state.ball.visible || state.ball.losttime < 20)
	{
		cnt_track++;
		cnt_track = 0;
		if(cnt_track < 150 || state.ball.distance < 1000)
		{
			SetHeadMode(HEAD_TRACK);
		}
		else if(cnt_track < 165)
		{
			SetHeadMode(HEAD_WATCHING);//HEAD_SWING_LOCALIZATION);
		}
		else						cnt_track = 0;
	}
	else
	{
		if(cnt_track > 100)	cnt_track++;
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
	}

	


	if(state.ball.visible){
		bool ball_in_area = CheckBallPosition();
		if(!moving){
			if(state.ball.distance < CHASE_DISTANCE && ball_in_area)
			{
				mode = GOALIE_CHASE;
				return;
			}
		}else{
			if(state.ball.distance < 300 && ball_in_area){
				mode = GOALIE_CHASE;
				return;
			}
		}
	}
	
	if( state.ball.losttime > 100||//100 ||
		pos_x > -2100 || 
		abs(pos_y) > 300 ||
		abs(pos_t) > 120)
	{
		mode = GOALIE_ADJUST_POSITION;
	}

	moving = false;

	static int wait_counter = 0;
	if(state.ball.visible || state.ball.losttime < 10)
	{
		wait_counter = 0;

		//test
		double ex,ey,xx,xv,yx,yv,py,t;
		int nx,ny;
		ex = DetectBall::pred_x.MakePredictor(12);
		ey = DetectBall::pred_y.MakePredictor(12);
		nx = DetectBall::pred_x.data.size();
		ny = DetectBall::pred_y.data.size();
		if(state.ball.visibletime > 10 && ny > 10 && nx > 10 && ex < 200 && ey < 200){
			xx = DetectBall::pred_x.x - 100;
			xv = DetectBall::pred_x.v;
			yx = DetectBall::pred_y.x;
			yv = DetectBall::pred_y.v;

			if(xv < -8 && xx > 0 && xx < 900)
			{
				t = -xx / xv;
				int x = (int)(xx + t*xv);
				int y = (int)(yx + t*yv);
				if(t < 17 && abs(y) < 300 && abs(y) > 30){
					if(y < 0)
						SetMotion("AYAYA_LEFT");
					else
						SetMotion("AYAYA_RIGHT");
					return;
				}
			}

			if(xv < -5 && xx > 0 && xx < 1700){
				t = -xx / xv;

				int mini = WALK_GOALIE_STOP;
				int minv = 10000;
				double v;
				for(int j = 0;j < t+30;j+=1){
					int x = (int)(xx + j*xv);
					int y = (int)(yx + j*yv);
					int movetime = dpmga.get_cost(x,y,0);
//						OSYSPRINT(("time:%d, x:%d, y:%d\n",movetime,x,y));
					v = j * 33 - (movetime + 100);
					if( v < 0) v *= -0.25;
					if(minv > v){
						minv = (int)v;
						mini = dpmga.get_action(x,y,0);
					}
				}
		//		if(GaitMaker::IsStop())
				SetPresetWalk(mini);
				if(mini != WALK_GOALIE_STOP)
					moving = true;
				else moving = false;
				return;
			}
		}
		moving = false;

		if(!Saving())
		{
            int ball_x = pos_x + (int)state.ball.x;
			int	ball_y = pos_y + (int)state.ball.y;

			int dest_x = -2550;
			int dest_y = ball_y;//ball_y/2;
			if(dest_y > 300)	dest_y = 250;
			if(dest_y < -300)	dest_y = -250;
			int dest_t = (int)atan2((double)(ball_y - dest_y), (double)(ball_x - dest_y));

			if( abs(pos_x - dest_x) < 150 &&
				abs(pos_y - dest_y) < 150 &&
				abs(pos_y) < 300)
			{
				if(GaitMaker::IsExpiring())
				{
					if(state.ball.visible || state.ball.losttime <10)	SetPresetWalk(WALK_STOP);
					else												SetPresetWalk(WALK_GOALIE_STOP);
				}
			}
			else
			{
				bhvMoveToPosDP.SetDestination(dest_x, dest_y, dest_t);
				bhvMoveToPosDP.Execute();
			}
		}
		return;
	}
	else{

		if(GaitMaker::IsExpiring())
		{
			if(state.ball.visible || state.ball.losttime <10)	SetPresetWalk(WALK_STOP);
			else					SetPresetWalk(WALK_GOALIE_STOP);
		}
	}
}

void BhvGoalie2006::ChaseBall(){
	if(Saving())
	{
		mode = GOALIE_WAIT;
		return;
	}
	if(state.ball.visible || state.ball.losttime < 10){
		SetHeadMode(HEAD_TRACK);

		bool ball_in_area = CheckBallPosition();
		if(state.ball.distance > QUIT_CHASE_DISTANCE || !ball_in_area)//あきらめ
			mode = GOALIE_ADJUST_POSITION;//mode = RETURN_GOAL_MOUTH;
//			mode = GOALIE_WAIT;

		if(state.ball.distance < BALL_CLEAR_DISTANCE)
		{
			Clear();
		}

		double dist = state.ball.distance;
		double dir = state.ball.direction;
		int x = (int)state.ball.x;
		int y = (int)state.ball.y;

		if(dir < -50){
			SetPresetWalk(WALK_ROTATE_RIGHT360);//270);
		}
		if(dir > 50){
			SetPresetWalk(WALK_ROTATE_LEFT360);//270);//);
		}

		int rtt = (int)state.self.position.tt.val;


		int walk = dpmla.get_action(x, y, 0);//dpmla.get_action(x+50,y,0);


		if(walk == 0){
			mode = GOALIE_CLEAR;
			SetPresetWalk(SMALL_FORWARD);
			return;
		}
		SetPresetWalk(walk);
	}
	else
	{
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		if(GaitMaker::IsExpiring())
		{
			if(state.ball.visible || state.ball.losttime <10)	SetPresetWalk(WALK_STOP);
			else					SetPresetWalk(WALK_GOALIE_STOP);
		}
		if(state.ball.losttime > 50)//120)
			mode = GOALIE_ADJUST_POSITION;//RETURN_GOAL_MOUTH;
	}
}

void BhvGoalie2006::Clear(){
	if(state.self.position.tt.val < -80){
		SetMotion(HEADING_LEFT);
	}else if(state.self.position.tt.val > 80){
		SetMotion(HEADING_RIGHT);
	}else{
		SetMotion(HG_FRONT);//(SHOOT_FRONT);
	}
	mode = GOALIE_CHASE;
}

bool BhvGoalie2006::Saving()
{
	double ex,ey,xx,xv,yx,yv,py,t;
	int nx,ny;
	ex = DetectBall::pred_x.MakePredictor(12);
	ey = DetectBall::pred_y.MakePredictor(12);
	nx = DetectBall::pred_x.data.size();
	ny = DetectBall::pred_y.data.size();

	if( state.ball.visible && 
		state.ball.visibletime > 10 &&
		ny > 10 && 
		nx > 10 && 
		ex < 200 && 
		ey < 200)
	{
		xx = DetectBall::pred_x.x - 100;
		xv = DetectBall::pred_x.v;
		yx = DetectBall::pred_y.x;
		yv = DetectBall::pred_y.v;

		if(xv < -8 && xx > 0 && xx < 900)
		{
			t = -xx / xv;
			int x = (int)(xx + t*xv);
			int y = (int)(yx + t*yv);
			if(t < 17 && abs(y) < 300 && abs(y) > 30){
				if(y < 0)
					SetMotion("AYAYA_LEFT");
				else
					SetMotion("AYAYA_RIGHT");
				return true;
			}
		}
	}
	return false;
}

void BhvGoalie2006::SaveMotion(){
	static bool startmotion = false; 
	if(!startmotion && GaitMaker::IsStop()){
//		savingflag = false;
//		return;

		startmotion = true;
		SetMotion("GOALIE_BLOCK");
	}
	if(startmotion && MotionMaker::IsFinished()){
		startmotion = false;
		savingflag = false;
	}
}

void BhvGoalie2006::SetSaveMotion(double y, double t){
	double ay = fabs(y);
	if(ay < 120 || ay > 500){
		return;
	}

	int step = (int)((ay - 150)/ 70);
	if(step <= 0){
		if(t < 20){
			savingflag = true;
			if(GaitMaker::IsExpiring())
			{
				if(state.ball.visible || state.ball.losttime <10)	SetPresetWalk(WALK_STOP);
				else					SetPresetWalk(WALK_GOALIE_STOP);
			}
		}
		return;
	}

	if(t - step*10 < 17){
		if(y > 0){
			SetPresetWalk(WALK_LEFT90,step);
		}else{
			SetPresetWalk(WALK_RIGHT90,step);
		}
		savingflag = true;
	}
}

//一旦ゴールに戻って，ボールが見えていないときの行動
void BhvGoalie2006::AdjustPosition()
{
	if(Saving())
	{
		mode = GOALIE_WAIT;
		return;
	}

	static int cnt_track = 0;
	if(state.ball.visible || state.ball.losttime < 20)
	{
		cnt_track++;
		cnt_track = 0;
		if(cnt_track < 150 || state.ball.distance < 1200)
		{
			SetHeadMode(HEAD_TRACK);
		}
		else if(cnt_track < 165)
		{
			SetHeadMode(HEAD_WATCHING);//HEAD_SWING_LOCALIZATION);
		}
		else						cnt_track = 0;
	}
	else
	{
		if(cnt_track > 100)	cnt_track++;
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
	}

	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	int pos_t = (int)state.self.position.tt.val;

	static unsigned int cnt_r2gm = 0;
	static unsigned int cnt_r2gp = 0;
	static unsigned int cnt_wait = 0;
	static unsigned int cnt_chas = 0;

	bool ball_in_area = CheckBallPosition();
	if((state.ball.visible || state.ball.losttime < 10) && 
	   state.ball.distance < CHASE_DISTANCE && 
	   ball_in_area)
	{
		cnt_chas++;
		if(cnt_chas > 3)
		{
			SetHeadMode(HEAD_TRACK);
			mode = GOALIE_CHASE;
			return;
		}
	}

	//ゴールエリアから出てたら，ゴールへ戻る感じ．
	if(pos_x > -1500 || pos_x < -3000 || abs(pos_y) > 400)//(pos_x > -2100 || pos_x < -3000 || abs(pos_y) > 700)
	{
		cnt_r2gm++;
		if(cnt_r2gm > 15)
			mode = RETURN_GOAL_MOUTH;
	}
	else if(pos_x > -2400 || abs(pos_y > 250) || abs(pos_t) > 30)
	{
		cnt_r2gp++;
		if(cnt_r2gp > 15)
            mode = RETURN_GOALIE_POSITION;
	}
	else if(state.ball.visible && state.ball.distance < WAIT_DIST)
	{
		cnt_wait++;
		if(cnt_wait > 15)
            mode = GOALIE_WAIT;
	}
	else
	{
		cnt_r2gm = 0;
		cnt_r2gp = 0;
		cnt_wait = 0;
	}

	if(GaitMaker::IsExpiring())
	{
		if(state.ball.visible || state.ball.losttime <10)	SetPresetWalk(WALK_STOP);
		else					SetPresetWalk(WALK_GOALIE_STOP);
	}

	return;
}

void BhvGoalie2006::GoBack(){

}

bool BhvGoalie2006::CheckBallPosition(){
	if(!state.ball.visible && state.ball.losttime > 10) return false;

	int ball_x = (int)(state.self.position.x.val + state.ball.x);
	int ball_y = (int)(state.self.position.y.val + state.ball.y);

	//キーパーが飛び出る範囲の設定
	if(mode != GOALIE_CHASE)
	{
		if(ball_x < -1350 && abs(ball_y) < 1600)	return true;
	}
	else
	{
		if(ball_x < -1000 && abs(ball_y) < 1200)	return true;
	}

	return false;
}

bool BhvGoalie2006::InSpot(int x, int y){
	return (
		 abs((int)state.self.position.x.val - x) < 350 &&
		 abs((int)state.self.position.y.val - y) < 350
		 //&& abs((int)state.self.position.tt.val - theta) < 20
		 );
}

bool BhvGoalie2006::InSpotTheta(int x, int y, int theta){
	return (
		 abs((int)state.self.position.x.val - x) < 150 &&
		 abs((int)state.self.position.y.val - y) < 200 &&
		 abs((int)state.self.position.tt.val - theta) < 20
		 );
}
