#include "StdAfx.h"
#include "BhvColorCooperation.h"

void
BhvColorCooperation::Execute(void)
{
	bool team_mate_visible = false;
	bool wireless_lan = false;
	if(state.game.team_number == TEAM_COLOR_RED)
	{
		if(state.self.cdt[RED].value > 50)
		{
			team_mate_visible = true;
		}
	}
	else
	{
		if(state.self.cdt[DARKBLUE].value > 50)
		{
			team_mate_visible = true;
		}
	}
	for(int i = 0;i < 5;i++)
	{
		if(state.team_mate[i].connected) wireless_lan = true;
	}
	if(!state.ball.visible || !team_mate_visible || wireless_lan)
	{
		return;
	}
	SetHeadMode(HEAD_TRACK);
	Target();
	double x = state.self.position.x.val;
	double y = state.self.position.y.val;
	double ballt_ = state.ball.direction;
	double ballr_ = state.ball.distance;

	static int walkcom = 0;
	double r = sqrt((x - target_x)*(x - target_x) + (y - target_y)*(y - target_y));
	int t_ = (int)(atan2((target_y - y),(target_x - x))*180.0/3.1415 - state.self.position.tt.val);
	while(t_ > 180) t_ -=360;
	while(t_ <-180) t_ +=360;


	
	if(ballr_ < 500.0){
		if(abs(t_) < 30)
			SetPresetWalk(WALK_LEFT90);
		else
			SetPresetWalk(WALK_RIGHT90);
	}


	if(!Reached){
		if(ballt_>30){
				SetPresetWalk(WALK_ROTATE_LEFT180);
		}else if(ballt_<-30){
				SetPresetWalk(WALK_ROTATE_RIGHT180);
		}else if(r > 100.0){
			if(t_ > 165)		walkcom = WALK_BACK;
			else if(t_ > 135)	walkcom = WALK_LEFT150;
			else if(t_ > 105)	walkcom = WALK_LEFT120;
			else if(t_ >  75)	walkcom = WALK_LEFT90;
			else if(t_ >  45)	walkcom = WALK_LEFT60;
			else if(t_ >  15)	walkcom = WALK_LEFT30;
			else if(t_ > -15)	walkcom = WALK_FORWARD;
			else if(t_ > -45)	walkcom = WALK_RIGHT30;
			else if(t_ > -75)	walkcom = WALK_RIGHT60;
			else if(t_ >-105)	walkcom = WALK_RIGHT90;
			else if(t_ >-135)	walkcom = WALK_RIGHT120;
			else if(t_ >-165)	walkcom = WALK_RIGHT150;
			else				walkcom = WALK_BACK;

		//	if(!state.self.walk.ing){
				SetPresetWalk(walkcom);
		//	}
		}else {
			Reached = true;
			SetPresetWalk(WALK_STOP);
		}
	}else {
		if(ballt_>30){
		//	if(!state.self.walk.ing)
				SetPresetWalk(WALK_ROTATE_LEFT180);
		}else if(ballt_>10){
		//	if(!state.self.walk.ing)
				SetPresetWalk(WALK_LEFT90);
		}else if(ballt_<-10){
		//	if(!state.self.walk.ing)
				SetPresetWalk(WALK_RIGHT90);
		}else if(ballt_<-30){
		//	if(!state.self.walk.ing)
				SetPresetWalk(WALK_ROTATE_RIGHT180);
		}else{
			SetPresetWalk(WALK_STOP);
		}
	}

	return;
}

void
BhvColorCooperation::Target(void){
	double tx = state.self.position.x.val;
	double ty = state.self.position.y.val;
	double tt = state.self.position.tt.val;
	double br = state.ball.distance;
	double bt = state.ball.direction;
	double ball_tt = tt+bt;
	if(ball_tt > 180.0) ball_tt -=360.0;
	if(ball_tt <-180.0) ball_tt +=360.0;

	ball_y = ty+br*sin(Radian(ball_tt));//coordinates of ball
	ball_x = tx+br*cos(Radian(ball_tt));

	double distance = (tx+2100.0)/3.0;
	if(distance<500.0)distance=500.0;

	target_x = ball_x-distance*cos(atan2(ball_y,(ball_x+2100.0)));
	target_y = ball_y-distance*sin(atan2(ball_y,(ball_x+2100.0)));
	if(target_x<-1850.0)target_x = -1850.0;
}

double
BhvColorCooperation::Radian(double deg){
	return deg*PI/180.0;
}

