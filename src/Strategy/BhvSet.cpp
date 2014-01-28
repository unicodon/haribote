#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"

BhvSet::BhvSet() 
{
}

void BhvSet::Execute(){
	static bool firstTime =true;
	if(firstTime)
	{
		mode = SET_MODE_LOCALIZATION;
		count = 0;
		firstTime = false;
	}
	SetPresetWalk(0);
	SetTailMode(TAIL_STOP);	
	SetFaceLED(FACE_SET);

	if(mode == SET_MODE_LOCALIZATION && count > 150)//Localization for 5 seconds
	{
		mode = SET_MODE_BALL_SEARCH;
	}
	else if(mode == SET_MODE_BALL_SEARCH && state.ball.visible)
	{
		mode = SET_MODE_TRACKING;
	}
	else if(mode == SET_MODE_TRACKING && state.ball.losttime > BALL_TRACKING_FRAME)
	{
		mode = SET_MODE_LOST_BALL_SEARCH;
	}
	else if(mode == SET_MODE_LOST_BALL_SEARCH && state.ball.visible)
	{
		mode = SET_MODE_TRACKING;
	}
	else if(mode == SET_MODE_LOST_BALL_SEARCH && state.ball.losttime > 100)
	{
		mode = SET_MODE_LOCALIZATION;
		count = 0;
	}

	if(mode == SET_MODE_LOCALIZATION)
	{
		count++;
	}

	switch(mode)
	{
		case SET_MODE_LOCALIZATION:			SetHeadMode(HEAD_SWING_LOCALIZATION);	break;
		case SET_MODE_BALL_SEARCH:			SetHeadMode(HEAD_SWING_BALL_SEARCH);	break;
		case SET_MODE_TRACKING:
			SetHeadMode(HEAD_TRACK);break;
		case SET_MODE_LOST_BALL_SEARCH:		SetHeadMode(HEAD_SWING_BALL_SEARCH);	break;
		default:							SetHeadMode(HEAD_SWING_BALL_SEARCH);	break;
	}
	//OSYSPRINT(("CHIN TOUCH is %d\n",state.self.sensor.chinTouch));
//	static int count_test = 0;
//	count_test++;
//	if(count_test%10)
//		OSYSPRINT(("Ball Distance = %.1f, Ball Direction = %.1f\n", state.ball.distance, state.ball.direction));
}
