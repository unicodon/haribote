#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"
#include "../Motion/PresetMotion.h"
#include "../Motion/GaitMaker.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/HeadController.h"

#define CHASE_DISTANCE 600
#define QUIT_CHASE_DISTANCE 1000
#define CLEAR_DISTANCE (180+50)

extern DPMapLocalApproach dpmla;
extern DPMapGoalieApproach dpmga;

static MoveToPosPotential m2pPot;


//ポテンシャル場計算用
struct PosPoint
{
	double x;
	double y;
	double mass; //正が引力
};

struct DirPoint
{
	double x;
	double y;
	double mass;
	double dir;	//正が引力
};

PosPoint PosPointsForWait[] = 
{
	{	-2500,	   0,  600}, //ゴール真ん中

	{	-2700,	 500,  -50}, //ゴール壁
	{	-2700,	-500,  -50}, //ゴール壁
	{	-2900,	 500, -100}, //ゴール壁
	{	-2900,	-500, -100}, //ゴール壁
	{	-3000,	   0, -100}, //ゴール壁後方．ﾀｹｽﾀ追加 06/06/06

	{	-1700,	 200,  -50}, //センター付近
	{	-1700,	-200,  -50}, //センター付近
};

const int nNumPosPointsForWait = 
	sizeof(PosPointsForWait) / sizeof(PosPoint);

DirPoint DirPointsForWait[] = 
{
	{	-2600,	    0,  20,    0},
	{	    0,	 1200,  10, -170},
	{	    0,	-1200,  10,  170},
	{	-2000,	 1200,  10,  -90},
	{	-2000,	-1200,  10,   90},
};

const int nNumDirPointsForWait = 
	sizeof(DirPointsForWait) / sizeof(DirPoint);

PosPoint PosPointBall = 
{
	0,		0,		2000	
};

DirPoint DirPointBall = 
{
	0,		0,		30,		0
};

void MoveToPosPotential::GetDest(int walk, double &dest_x, double &dest_y, double &dest_t)
{
	if (walk < 0 || walk >= NUM_PRESET_WALKPARAM)
		return;

	double du = PresetWalkParam::walkparams[walk].move_x ;
	double dv = PresetWalkParam::walkparams[walk].move_y ;
	double dt = state.self.position.tt.val / 180 *PI;

	double dx = du * cos(dt) - dv * sin(dt);
	double dy = du * sin(dt) + dv * cos(dt);

	dest_x = state.self.position.x.val  + dx ;
	dest_y = state.self.position.y.val  + dy ;
	dest_t = state.self.position.tt.val + 
		PresetWalkParam::walkparams[walk].move_tt ;
}

double MoveToPosPotential::GetValue(double x, double y, double tt)
{
	double value = 0;
	for (int i = 0; i < nNumPosPointsForWait; i++)
	{
		value += PosPointsForWait[i].mass / (
				(x - PosPointsForWait[i].x) * (x - PosPointsForWait[i].x) + 
				(y - PosPointsForWait[i].y) * (y - PosPointsForWait[i].y) + 2500
			);
	}

    //ボールはとりあえず無視
	/*
	if (state.ball.visible && 
		sqrt( (x - 2000)*(x - 2000) + y*y ) < 1000
		)
	{
		value += PosPointBall.mass / (
				(x - PosPointBall.x) * (x - PosPointBall.x) + 
				(y - PosPointBall.y) * (y - PosPointBall.y) + 2500
			);
	}*/


	return value;
}

int MoveToPosPotential::GetWalk()
{
	double	x  = state.self.position.x.val;
	double	y  = state.self.position.y.val;
	double	tt = state.self.position.tt.val;

	//まず方角チェック
	double dt = 0;
	if (false)//state.ball.visible)
	{
		dt = state.ball.direction;
	}
	else
	{
		double dirx = 0;
		double diry = 0;
		double dir  = 0;
		for (int i = 0; i < nNumDirPointsForWait; i++)
		{
			double weight = DirPointsForWait[i].mass /
				(
					(x - DirPointsForWait[i].x) * (x - DirPointsForWait[i].x) + 
					(y - DirPointsForWait[i].y) * (y - DirPointsForWait[i].y) + 1
				);
			dirx += weight * cos(DirPointsForWait[i].dir * PI / 180);
			diry += weight * sin(DirPointsForWait[i].dir * PI / 180);
		}
		dir	= atan2(diry, dirx) * 180 / PI;
		dt	= dir - tt;
	}

	if (dt < -30)
	{
		return WALK_ROTATE_RIGHT360;
	}
	else if (dt > 30)
	{
		return WALK_ROTATE_LEFT360;
	}

	//次にポテンシャル法で移動
	if (state.ball.visible)
	{
		PosPointBall.x =  state.ball.distance * cos((state.ball.direction + tt) * PI / 180)
						+ state.self.position.x.val;
		PosPointBall.y =  state.ball.distance * sin((state.ball.direction + tt) * PI / 180)
						+ state.self.position.y.val;
	}
	
	int		best_walk	= 0;
	double	max_value	= GetValue(x, y, tt);
	double	value		= 0;

	for(int walk = 1; walk < NUM_PRESET_WALKPARAM; walk++)
	{
		GetDest(walk, x, y, tt);
		value = GetValue(x, y, tt);

		if (value > max_value)
		{
			max_value = value;
			best_walk = walk;
		}
	}
//	OSYSPRINT(("val = %f\n", max_value));
//	if (max_value > 600 / (2500) )
//		return WALK_STOP;
	return best_walk;
}




BhvGoalie2007::BhvGoalie2007()
{
	dest_x = 	-1800.0;
	dest_y =		0.0;
	dest_t =		0.0;

	Reset();
}

void BhvGoalie2007::Reset()
{
	mode = GOALIE2007_WAIT;
	savingflag = false;
	savingmotion = GOALIE_BLOCK_LEFTWALK;
}

void BhvGoalie2007::ResetForPenalize()
{
	Reset();
}


void BhvGoalie2007::Execute()
{
	if (savingflag)
	{
		SaveMotion();
		return;
	}

	if (state.self.position.x.val > -1000 && !state.ball.visible &&
 		(mode == GOALIE2007_WAIT || mode == GOALIE2007_CHASE_BALL)
		)
	{
		mode = GOALIE2007_RETURN;
	}

	switch(mode)
	{
		case GOALIE2007_WAIT:
			Wait();
			break;
		case GOALIE2007_CHASE_BALL:
			ChaseBall();
			break;
		case GOALIE2007_CLEAR:
			Clear();
			break;
		case GOALIE2007_RETURN:
			Return();
			break;
		case GOALIE2007_SAVING:
			Saving();
			break;
	}

	switch(mode)
	{
		case GOALIE2007_CHASE_BALL:
			SetFaceLED(FACE_THREE);
			SetTailMode(TAIL_SWING);
			break;
		case GOALIE2007_CLEAR:
			SetFaceLED(FACE_FOUR);
			SetTailMode(TAIL_ANGRY);
			break;
		case GOALIE2007_RETURN:
			SetFaceLED(FACE_READY);
			SetTailMode(TAIL_STOP);
			break;
		case GOALIE2007_SAVING:
			SetFaceLED(FACE_FIVE);
			SetTailMode(TAIL_ANGRY);
			break;
	}

}


void BhvGoalie2007::Wait()
{
	bool ball_in_area = CheckBallPosition();

	if (state.ball.visible || state.ball.losttime < 10)
	{
		if (state.ball.distance < CHASE_DISTANCE && ball_in_area)
		{
			mode = GOALIE2007_CHASE_BALL;
			return;
		}
		SetHeadMode(HEAD_TRACK);
		SetFaceLED(FACE_TWO);
		SetTailMode(TAIL_SWING);
	}
	else
	{
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetFaceLED(FACE_ONE);
		SetTailMode(TAIL_STOP);
	}

	if (GaitMaker::IsExpiring())
	{
		int walk = m2pPot.GetWalk();
		SetPresetWalk(walk);
	}

	CheckSaving();
}

void BhvGoalie2007::CheckSaving()
{
	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	int pos_t = (int)state.self.position.tt.val;

	static bool moving = false;

	moving = false;

	static int wait_counter = 0;
	if(state.ball.visible || state.ball.losttime < 10){
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

				int mini = WALK_STOP;
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
				if(mini != WALK_STOP)
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

			int dest_x = -2600;
			int dest_y = ball_y/2;
			if(dest_y > 250)	dest_y = 250;
			if(dest_y < -250)	dest_y = -250;
			int dest_t = (int)atan2((double)(ball_y - dest_y), (double)(ball_x - dest_y));
			bhvMoveToPosDP.SetDestination(dest_x, dest_y, dest_t);
			bhvMoveToPosDP.Execute();
		}
	}
}

bool BhvGoalie2007::CheckBallPosition(){
	if(!state.ball.visible && state.ball.losttime > 10) return false;

	int ball_x = (int)(state.self.position.x.val + state.ball.x);
	int ball_y = (int)(state.self.position.y.val + state.ball.y);

	//キーパーが飛び出る範囲の設定
	if(    ball_x < -1500 && abs(ball_y) < 1200)
	{
			return true;
	}

	return false;
}

void BhvGoalie2007::ChaseBall()
{
	if(state.ball.visible || state.ball.losttime < 10){
		SetHeadMode(HEAD_TRACK);

		bool ball_in_area = CheckBallPosition();

		if(state.ball.distance > QUIT_CHASE_DISTANCE || !ball_in_area)//あきらめ
		{
			mode = GOALIE2007_WAIT;
			return;
		}


		if(state.ball.distance < CLEAR_DISTANCE){
			mode = GOALIE2007_CLEAR;
			Clear();
			return;
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
			mode = GOALIE2007_CLEAR;
			SetPresetWalk(SMALL_FORWARD);
			return;
		}
		SetPresetWalk(walk);

		if (state.ball.distance > 300)
		{
			CheckSaving();
		}

	}else{
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetPresetWalk(WALK_STOP);
		if(state.ball.losttime > 50)//120)
		{
			mode = GOALIE2007_WAIT;
			return;
		}
	}
}

void BhvGoalie2007::Return()
{
	static bool set = false;

	if (state.ball.visible)
	{
		mode = GOALIE2007_WAIT;
		set  = false;
		return;
	}


	if (!set)
	{
		if (state.self.position.y.val > 0)
		{
			//コート左側
			dest_x = -2200;
			dest_y = 600;
			dest_t = -90;
		}
		else
		{
			//コート右側
			dest_x = -2200;
			dest_y = -600;
			dest_t = 90;
		}
		set = true;
	}

	if (state.self.position.x.val < -2100)
	{
		set = false;
		mode = GOALIE2007_WAIT;
		return;
	}

	bhvMoveToPosDP.SetDestination(dest_x, dest_y, dest_t);
	bhvMoveToPosDP.Execute();
}

void BhvGoalie2007::Clear()
{
	if(state.self.position.tt.val < -80)
	{
		SetMotion(HEADING_LEFT);
	}
	else if(state.self.position.tt.val > 80)
	{
		SetMotion(HEADING_RIGHT);
	}
	else
	{
		SetMotion(SHOOT_FRONT);
	}
	mode = GOALIE2007_CHASE_BALL;
}

bool BhvGoalie2007::Saving(){
	double ex,ey,xx,xv,yx,yv,py,t;
	int nx,ny;
	ex = DetectBall::pred_x.MakePredictor(12);
	ey = DetectBall::pred_y.MakePredictor(12);
	nx = DetectBall::pred_x.data.size();
	ny = DetectBall::pred_y.data.size();
	if(state.ball.visible && state.ball.visibletime > 10 &&
		ny > 10 && nx > 10 && ex < 200 && ey < 200)
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

void BhvGoalie2007::SaveMotion(){
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

void BhvGoalie2007::SetSaveMotion(double y, double t){
	double ay = fabs(y);
	if(ay < 120 || ay > 500){
		return;
	}

	int step = (int)((ay - 150)/ 70);
	if(step <= 0)
	{
		if(t < 20)
		{
			savingflag = true;
			SetPresetWalk(WALK_STOP);
		}
		return;
	}

	if(t - step*10 < 17)
	{
		if(y > 0)
		{
			SetPresetWalk(WALK_LEFT90,step);
		}
		else
		{
			SetPresetWalk(WALK_RIGHT90,step);
		}
		savingflag = true;
	}
}
