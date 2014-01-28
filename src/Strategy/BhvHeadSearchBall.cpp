#include "StdAfx.h"
#include "BehaviorList.h"

#include <OPENR/OSyslog.h>

void BhvHeadSearchBall::Execute()
{
	if(false)//state.ball.visible)
	{
		//OSYSPRINT(("Ball Tracking visible\n"));
		SetHeadMode(HEAD_TRACK);
		SetTailMode(TAIL_STOP);
		SetPresetWalk(0);
	}
	else if(false)//state.ball.losttime <= BALL_TRACKING_FRAME)
	{
		//OSYSPRINT(("Ball Tracking invisible\n"));
		SetHeadMode(HEAD_TRACK);
		SetTailMode(TAIL_STOP);
		SetPresetWalk(0);
	}
	else
	{
		//OSYSPRINT(("Search Ball\n"));
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetTailMode(TAIL_STOP);
		SetPresetWalk(0);
	}
}
