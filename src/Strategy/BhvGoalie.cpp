#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"
#include "../Motion/PresetMotion.h"
#include "../Motion/GaitMaker.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/HeadController.h"

/*
#define WAIT_DIST 2000
#define CHASE_DISTANCE 400
#define QUIT_CHASE_DISTANCE 600
#define CLEAR_DISTANCE 180
#define GOALPIXEL_TH 20000

*/
#define WAIT_DIST 1700
#define CHASE_DISTANCE 900
#define QUIT_CHASE_DISTANCE 1200
#define CLEAR_DISTANCE 180
#define GOALPIXEL_TH 20000

DPMapLocalApproach dpmla;
DPMapGoalieApproach dpmga;

BhvGoalie::BhvGoalie(){
	mode = RETURN_GOALMOUTH;
	savingflag = false;
	savingmotion = GOALIE_BLOCK_LEFTWALK;
	gmcnt = 0;
}

void BhvGoalie::Reset(){
	mode = RETURN_GOALAREA;
	savingflag = false;
	savingmotion = GOALIE_BLOCK_LEFTWALK;
	gmcnt = 0;
}

void BhvGoalie::ResetForPenalize(){
	mode = RETURN_GOALMOUTH;
	savingflag = false;
	savingmotion = GOALIE_BLOCK_LEFTWALK;
	gmcnt = 0;
}


void BhvGoalie::Execute(){
//	bhvDrible.Execute();
//	SetHeadMode(HEAD_TRACK);
//	return;

	if(savingflag){
		SaveMotion();
		return;
	}
/*
	if(GaitMaker::IsStop()){
		int x,y;
		scanf("%d",&x);
		scanf("%d",&y);
		int walk = dpmla.get_action(x,y,0);
		printf("%d\n",walk);
		SetPresetWalk(walk);
	}
	*/
/*
	SetHeadMode(HEAD_TRACK_KP);
	Saving();
	return;
*/
	if(mode != RETURN_GOALMOUTH)
		gmcnt = 0;

	switch(mode){
		case GO_BACK:
			GoBack();
			break;
		case RETURN_GOALAREA:
			SetFaceLED(FACE_ONE);
			SetTailMode(TAIL_STOP);
			ReturnToGoalArea();
			break;
		case ADJUST_POSITION:
			SetFaceLED(FACE_TWO);
			SetTailMode(TAIL_ANGRY);
			AdjustPosition();
			break;
		case WAIT:
			SetFaceLED(FACE_THREE);
			SetTailMode(TAIL_SWING);
			Wait();
			break;
		case CHASE:
			SetFaceLED(FACE_FOUR);
//			OSYSPRINT(("%.1f\n",state.ball.distance));
			ChaseBall();
			SetTailMode(TAIL_ANGRY);
			break;
		case CLEAR:
			SetFaceLED(FACE_FIVE);
			Clear();
			SetTailMode(TAIL_STOP);
			break;
		case RETURN_GOALMOUTH:
			ReturnGoalMouth();
			break;
	}
}

void BhvGoalie::ReturnGoalMouth(){
	if(gmcnt++ > 200){
		mode = RETURN_GOALAREA;
	}

	if(state.ball.visible && state.ball.distance < CHASE_DISTANCE){
		SetHeadMode(HEAD_TRACK);
		mode = CHASE;
		return;
	}

	int walk;
	SetHeadAngle(0,0,0,0,100);
	if(!state.myGoalVisible){
//		SetHeadMode(HEAD_SWING_BALL_SEARCH);
//		if(state.self.position.tt.val > 0){
			walk = WALK_ROTATE_LEFT360;//270;//180;
//		}else{
//			walk = WALK_ROTATE_RIGHT90;
//		}
	}else{
		walk = WALK_FORWARD;
//		gmcnt = 0;
		double width = state.myGoalInfo.directionLeft - state.myGoalInfo.directionRight;
		double dir = (state.myGoalInfo.directionLeft + state.myGoalInfo.directionRight)/2;
		//OSYSPRINT(("witdh %1.f, dir %1.f\n",width,dir));
//		SetHeadAngle(0,dir,0,0,100);
//		if(width < 6){
//			mode = RETURN_GOALAREA;
//			return;
//		}
		int goalcolor = state.game.team_color == TEAM_COLOR_RED ? YELLOW : SKYBLUE;
		if(state.self.cdt[goalcolor].value > GOALPIXEL_TH){
			mode = ADJUST_POSITION;
			return;
		}
/*
		if(dir < -30)
			walk = WALK_ROTATE_RIGHT90;
		else if (dir < 30)
			walk = WALK_FORWARD;
		else
			walk = WALK_ROTATE_LEFT90;
	*/
	}
//	if(GaitMaker::IsStop())
		SetPresetWalk(walk);
}

void BhvGoalie::ReturnToGoalArea(){
	double x = state.self.position.x.val;
	double y = state.self.position.y.val;
	double tt = state.self.position.tt.val;

//	MoveToPosition();

	static int movemode = 0;

	SetHeadMode(HEAD_SWING_BALL_SEARCH);
	static bool rotateflag = false;

	if((state.ball.visible || state.ball.losttime < 10) && state.ball.distance < CHASE_DISTANCE){
		mode = CHASE;
		rotateflag = false;
		movemode = 0;
		return;
	}
	int goalcolor = state.game.team_color == TEAM_COLOR_RED ? YELLOW : SKYBLUE;
	if(state.self.cdt[goalcolor].value > GOALPIXEL_TH){
		mode = ADJUST_POSITION;
		rotateflag = false;
		movemode = 0;
		return;
	}

	static int stopcounter = 0;
	if(GaitMaker::IsStop()){
		stopcounter++;
	}else{
		stopcounter = 0;
	}
	
	if(stopcounter > 300){
		if(movemode == 4){
			movemode = 0;
			rotateflag = false;
			mode = GO_BACK;
		}else if(movemode == 3)
			movemode = 4;

	}

	switch(movemode){
		case 0:
			if(rotateflag && GaitMaker::IsStop()){
				rotateflag = false;
				movemode = 1;
				return;
			}
			if(!rotateflag){
				SetPresetWalk(WALK_ROTATE_RIGHT360,7);//270,7);//180,7);
				rotateflag = true;
			}
			return;
		case 1:
			bhvMoveToPosDP.SetDestination(-1800,0,-180);
			break;
		case 2:
			bhvMoveToPosDP.SetDestination(-2800,0,-180);
			break;
		case 3:
			bhvMoveToPosDP.SetDestination(-3800,0,-180);
			break;
		case 4:
			bhvMoveToPosDP.SetDestination(-1800, 0, 0);
			break;
	}
	if(movemode <= 4){
		bhvMoveToPosDP.Execute();
	}else{
		if(!state.goalVisible){
			SetPresetWalk(WALK_ROTATE_RIGHT360);//270);//180);
		}else{
			if(state.goalDirection < -20){
				if(GaitMaker::IsStop())
					SetPresetWalk(WALK_ROTATE_RIGHT360);//270);//180);
			}else if(state.goalDirection < 20){
				if(GaitMaker::IsStop())
					SetPresetWalk(WALK_FORWARD);
			}else{
				if(GaitMaker::IsStop())
					SetPresetWalk(WALK_ROTATE_LEFT360);//270);//);
			}
		}
	}
	if(GaitMaker::IsStop()){
		if(movemode == 1){
			movemode = 2;
		}else if(movemode == 2){
			movemode = 3;
//			mode = ADJUST_POSITION;
			return;
		}
	}

/*	if(x < -2450 && x > -3300 && y > - 230 && y < 230 && tt > -25 && tt < 25){
		mode = WAIT;
		return;
	}
*/
}

void BhvGoalie::Wait(){
	double x = state.self.position.x.val;
	double y = state.self.position.y.val;
	double tt = state.self.position.tt.val;

	static bool moving = false;

	SetHeadMode(HEAD_TRACK_KP);
//	Saving();

	if(state.ball.losttime > 100)
		mode = RETURN_GOALMOUTH;
//		mode = ADJUST_POSITION;

	if(state.ball.visible){
		if(!moving){
			if(state.ball.distance < CHASE_DISTANCE){
				mode = CHASE;
				return;
			}
		}else{
			if(state.ball.distance < 300){
				mode = CHASE;
				return;
			}
		}
	}

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

			if(xv < -8 && xx > 0 && xx < 900){
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


		if(state.ball.direction < -20){
			SetPresetWalk(WALK_ROTATE_RIGHT360);//270);//180);
		}else if(state.ball.direction < 20){

			SetPresetWalk(WALK_STOP);
		}else{
			SetPresetWalk(WALK_ROTATE_LEFT360);//270);//);
		}
/*
		static int seecount = 0;
		if(seecount++ > 150){
			if(seecount > 250){
				seecount = 0;
			}
			SetHeadMode(HEAD_SWING_LOCALIZATION);
		}else{
			SetHeadMode(HEAD_TRACK);
		}
*/

//			SetPresetWalk(WALK_STOP);
	}else{
/*		wait_counter++;
		if(wait_counter > 300){
			static int turn_counter = 0;
			if(turn_counter++ > 35){
				wait_counter = 0;
				turn_counter = 0;
			}

			SetPresetWalk(WALK_ROTATE_RIGHT180);

		}else{
			SetPresetWalk(WALK_STOP);
		}
		*/
//		SetHeadMode(HEAD_SWING_BALL_SEARCH);
/*		static int wcnt = 0;
		if(GaitMaker::IsStop()){
			wcnt++;
			int goalcolor = state.game.team_color == TEAM_COLOR_RED ? YELLOW : SKYBLUE;
			if(wcnt > 5 || state.self.cdt[goalcolor].value > GOALPIXEL_TH){
				SetPresetWalk(WALK_ROTATE_LEFT90);
				wcnt = 0;
			}
		}else{
			if(wcnt != 0){
				SetPresetWalk(WALK_STOP);
				wcnt = 0;
			}
		}
*/
		SetPresetWalk(WALK_STOP);
	}
//	if(state.ball.distance > 800 && !(x < -2350 && x > -3300 && y > - 400 && y < 400)){
//		mode = RETURN_GOALAREA;
//	}

}

void BhvGoalie::ChaseBall(){
	if(state.ball.visible || state.ball.losttime < 10){
		SetHeadMode(HEAD_TRACK);

		if(state.ball.distance > QUIT_CHASE_DISTANCE)//あきらめ
			mode = RETURN_GOALMOUTH;
//			mode = WAIT;

		if(state.ball.distance < CLEAR_DISTANCE){
			Clear();
		}

		/*
		if(state.ball.direction < -20){
			SetPresetWalk(WALK_ROTATE_RIGHT90);
			return;
		}else if(state.ball.direction > 20){
			SetPresetWalk(WALK_ROTATE_LEFT90);
			return;
		}else{
			if(state.ball.distance > 350){
				SetPresetWalk(WALK_FORWARD);
			}else{
				SetPresetWalk(SMALL_FORWARD);
			}
			return;
		}
		*/

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


		int walk = dpmla.get_action(x+50,y,0);


//		OSYSPRINT(("walk: %d\n",walk));

/*		double tt = 0;
		int walk;

		double X,Y,TT;
		int mini = SMALL_FORWARD;
		double minv = (x*x + y*y)*2;
		int i = 0;
		for(i = WALK_FORWARD; i < WALK_TURN_LEFT20;i++){
			double v = 0;
			X  = x  - PresetWalkParam::walkparams[i].move_x*cos(tt*PI/180) + PresetWalkParam::walkparams[i].move_y*sin(tt*PI/180);
			Y  = y  - PresetWalkParam::walkparams[i].move_x*sin(tt*PI/180) - PresetWalkParam::walkparams[i].move_y*cos(tt*PI/180);
			TT = PresetWalkParam::walkparams[i].move_tt;

			v = X*X + Y*Y + fabs(TT)*10; 
			if(minv > v){
				minv = v;
				mini = i;
			}
		}
*/

		if(walk == 0){
			mode = CLEAR;
			SetPresetWalk(SMALL_FORWARD);
			return;
		}
		SetPresetWalk(walk);

/*
		if(state.ball.distance < 350 && fabs(state.ball.direction) < 20){
			SetPresetWalk(SMALL_FORWARD);
		}else{
			if(state.ball.direction < -30){
				SetPresetWalk(WALK_ROTATE_RIGHT90);
				return;
			}else if(state.ball.direction > 30){
				SetPresetWalk(WALK_ROTATE_LEFT90);
				return;
			}else{
				SetPresetWalk(walk);
				return;
			}
		}
*/
	}else{
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetPresetWalk(WALK_STOP);
		if(state.ball.losttime > 120)
			mode = RETURN_GOALMOUTH;
	}
}

void BhvGoalie::Clear(){
//	OSYSPRINT(("clear: theta = %1.f\n",state.self.position.tt.val));
	if(state.self.position.tt.val < -80){
		SetMotion(HEADING_LEFT);
	}else if(state.self.position.tt.val > 80){
		SetMotion(HEADING_RIGHT);
	}else{
		SetMotion(SHOOT_FRONT);
	}
	mode = CHASE;
}

void BhvGoalie::Saving(){
//	OSYSPRINT(("losttime %d\n",state.ball.losttime));
//	if(state.ball.distance < CHASE_DISTANCE)
//		mode = CHASE;

//	SetHeadMode(HEAD_TRACK_KP);
//	SetPresetWalk(WALK_STOP);
	double ex,ey,xx,xv,yx,yv,py,t;
	int nx,ny;
	ex = DetectBall::pred_x.MakePredictor(20);
	ey = DetectBall::pred_y.MakePredictor(20);
	nx = DetectBall::pred_x.data.size();
	ny = DetectBall::pred_y.data.size();
	if(state.ball.visibletime > 15 && ny > 15 && nx > 15 && ex < 200 && ey < 200){
		xx = DetectBall::pred_x.x - 100;
		xv = DetectBall::pred_x.v;
		yx = DetectBall::pred_y.x;
		yv = DetectBall::pred_y.v;

		if(xv < -20 && xx > 0 && xx < 1200){
			t = -xx / xv;
			py = yx + yv*t;
//			OSYSPRINT(("%1.f,%1.f,%1.f,%1.f,%1.f,%1.f\n",xx,xv,yx,yv,t,py));
			SetSaveMotion(py,t);
			return;

		}
	}
}

/*void BhvGoalie::MoveToPosition(){
	int walk = 0;
	static bool final_state = false;

	double dest_x = -2650;
	double dest_y = 0;
	double dest_tt = 0;
	
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
		if(dtt < -130)		walk = WALK_ROTATE_RIGHT180;
		else if(dtt < -80)	walk = WALK_ROTATE_RIGHT90;
		else if(dtt < -20)	walk = WALK_ROTATE_RIGHT90;
		else if(dtt < 20){	walk = WALK_STOP; final_state = true;}				//終端状態
		else if(dtt < 80 )	walk = WALK_ROTATE_LEFT90;
		else if(dtt < 130)	walk = WALK_ROTATE_LEFT90;
		else				walk = WALK_ROTATE_LEFT180;
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
    
	SetPresetWalk(walk, true);

	SetTailMode(TAIL_SWING);	
}
*/
void BhvGoalie::SaveMotion(){
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

void BhvGoalie::SetSaveMotion(double y, double t){
	double ay = fabs(y);
	if(ay < 120 || ay > 500){
		return;
	}

	int step = (int)((ay - 150)/ 70);
	if(step <= 0){
		if(t < 20){
			savingflag = true;
			SetPresetWalk(WALK_STOP);
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

void BhvGoalie::AdjustPosition(){
	if(state.ball.visible && state.ball.distance < WAIT_DIST){
		mode = WAIT;
	}

	SetHeadMode(HEAD_SWING_BALL_SEARCH);

	if(state.myGoalVisible){
		if(state.myGoalInfo.directionRight < 100){
			SetPresetWalk(WALK_ROTATE_RIGHT360);//270);//180);
			return;
		}
		if(state.myGoalInfo.directionLeft > -100){
			SetPresetWalk(WALK_ROTATE_LEFT360);//270);//180);
			return;
		}


	}

	if(state.goalVisible){
		int gtheta = (int)state.goalDirection;
		if(gtheta < -20){
			SetPresetWalk(WALK_ROTATE_RIGHT360);//270);//180);
			return;
		}else if(gtheta > 20){
			SetPresetWalk(WALK_ROTATE_LEFT360);//270);//180);
			return;
		}
	}

	if(state.goalVisibleOfOurs){
		int gtheta = (int)state.goalDirectionOfOurs;
//		OSYSPRINT(("mygoal, %d\n",gtheta));
		if(gtheta < -90){
			SetPresetWalk(WALK_ROTATE_LEFT360);//270);//180);
			return;
		}else if(gtheta > 90){
			SetPresetWalk(WALK_ROTATE_RIGHT360);//270);//180);
			return;
		}
	}

	if(state.lineVisible){
		int theta,x,y,walk;
		double pan = state.self.sensor.jointValue[PAN];
		if(pan < -80){
//			theta = state.lineinfo.theta + 180 + 90*(1 - (int)state.lineinfo.type);
			y = - state.lineinfo.r + 700;
			SetPresetWalk(dpmga.get_action(150,y,0),1);
		}
		if(pan > -10 && pan < 10){
//			theta = state.lineinfo.theta + 90 + 90*(1 - (int)state.lineinfo.type);
			x = state.lineinfo.r - 1000;
			SetPresetWalk(dpmga.get_action(x+150,0,0),1);
		}
		if(pan > 80){
//			theta = state.lineinfo.theta + 0 + 90*(1 - (int)state.lineinfo.type);
			y = state.lineinfo.r - 700;
			SetPresetWalk(dpmga.get_action(150,y,0),1);
		}
	}
	SetPresetWalk(WALK_STOP);
	return;
/*

	const double look_dir[6] = {
		0,0,-90,-90,90,90
	};
	const double look_speed[6] = {
		300,100,300,100,300,100
	};
	static int dir_index = 0;
	static bool looking = false;

	if(dir_index >= 6)dir_index -= 6;
	if(!looking){
		SetHeadAngle(0,look_dir[dir_index],-20,0,look_speed[dir_index]);
		looking = true;
	}

	if(HeadController::finished){
		if(state.lineVisible && dir_index % 2){
			int theta,x,y,walk;
			switch(dir_index){
				case 1:
					theta = state.lineinfo.theta + 90 + 90*(1 - (int)state.lineinfo.type);
					x = state.lineinfo.r - 500;
					SetPresetWalk(dpmla.get_action(x,0,-theta),3);
					break;
				case 3:
					theta = state.lineinfo.theta + 180 + 90*(1 - (int)state.lineinfo.type);
					y = - state.lineinfo.r + 500;
					SetPresetWalk(dpmla.get_action(0,y,-theta),3);
					break;
				case 5:
					theta = state.lineinfo.theta + 0 + 90*(1 - (int)state.lineinfo.type);
					y = state.lineinfo.r - 500;
					SetPresetWalk(dpmla.get_action(0,y,-theta),3);
					break;
			}
			OSYSPRINT(("%d r:%d, tt:%d, type:%d\n",dir_index, state.lineinfo.r,state.lineinfo.theta,state.lineinfo.type));
		}
		dir_index++;
		looking = false;
	}

//	SetPresetWalk(WALK_STOP);
	if(state.ball.visible && state.ball.distance < 1300)
		mode = WAIT;
	return;


	SetHeadMode(HEAD_TRACK);
	if((state.ball.visible || state.ball.losttime < 10) && state.ball.distance < CHASE_DISTANCE){
		mode = CHASE;
		return;
	}
	double tt = state.self.position.tt.val;
	if(tt < -25){
		SetPresetWalk(WALK_ROTATE_LEFT90);
	}if(tt < 25){
		int goalcolor = state.game.team_color == TEAM_COLOR_RED ? YELLOW : SKYBLUE;
		if(state.self.cdt[goalcolor].value > GOALPIXEL_TH){
		}else{
			mode = WAIT;
			return;
		}
	}else{
		SetPresetWalk(WALK_ROTATE_RIGHT90);
	}
*/
}

void BhvGoalie::GoBack(){
	static bool starting = false;
	if(GaitMaker::IsStop()){
		if(starting){
			SetPresetWalk(WALK_BACK,15);
			starting = true;
		}else{
			starting = false;
			mode = RETURN_GOALAREA;
		}
	}
}
