#include "StdAfx.h"
#include "BehaviorList.h"

#include <OPENR/OSyslog.h>
#include "../Motion/GaitMaker.h"
#include "../Motion/HeadController.h"

#define limit_hold_time 90 //90*0.033[s]

void BhvHoldShot::Execute(){
	turn_mode=MODE_DEFAULT;
	hold_timer=0;
	overshoot=0;
}


bool BhvHoldShot::HoldShot()
{
	hold_timer++;

    //‰ñ‚è‰ß‚¬‚É‚æ‚éovershoot[deg]‘½‚­‰ñ“]‚µ‚½Œã‚ÌƒS[ƒ‹•ûŒü
	int after_goal_direc=(int)(state.goalDirection+overshoot);

	if(state.enemyGoalVisible){
		if(after_goal_direc > 30)			SetMotion("GRABSHOOT_FAKE_LEFT40");
		else if(after_goal_direc > 10)		SetMotion("GRABSHOOT_FAKE_LEFT20");
		else if(after_goal_direc > 0)		SetMotion("GRABSHOOT_FAKE_LEFT");
		else if(after_goal_direc > -10)		SetMotion("GRABSHOOT_FAKE_RIGHT");
		else if(after_goal_direc > -30)		SetMotion("GRABSHOOT_FAKE_RIGHT20");
		else								SetMotion("GRABSHOOT_FAKE_RIGHT40");

		//OSYSPRINT(("goaldrc=%.1f,overshoot=%d,afterdrc=%d\n",state.goalDirection,overshoot,after_goal_direc));
		SetDefault();
		return true;
	}

	if(hold_timer>limit_hold_time){
		SetDefault();
		return true;

	}

	if(HeadController::hold_state!=HOLD_OK){
		SetDefault();
		return true;
	}

	switch(turn_mode)
	{
		case MODE_DEFAULT:
			if(state.goalDirection>0)	turn_mode=MODE_LEFT;
			else						turn_mode=MODE_RIGHT;
			break;

		case MODE_RIGHT:
			SetPresetWalk(BALL_ROTATE_RIGHT180,1, true);
			overshoot=40;
			break;

		case MODE_LEFT:
			SetPresetWalk(BALL_ROTATE_LEFT180,1, true);
			overshoot=-40;
			break;
	}

	return false;
}

void BhvHoldShot::SetDefault()
{
	SetPresetWalk(0,true);
	overshoot=0;
	turn_mode=MODE_DEFAULT;
	hold_timer=0;
}
