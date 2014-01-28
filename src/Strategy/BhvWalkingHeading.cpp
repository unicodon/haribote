#include "StdAfx.h"
#include "BhvWalkingHeading.h"
#include "../Motion/HeadController.h"

#include <OPENR/OSyslog.h>

const int BhvWalkingHeading::headingPosition[NUM_POSITION][4] =
{
	{-90,0,-20,200},
	{-90,0,-20,200},
	{-90,0,50,200},
	{-90,0,50,200}
};

void
BhvWalkingHeading::Execute()
{
	//tilt1.pan,tilt2,time


	if(finish) return;
	ReserveCall();
	SetPresetWalk(WALK_FORWARD);
	if(HeadController::finished && !first_time)//ç≈èâÇÃàÍâÒÇÕñ≥éãÇ∑ÇÈ
	{
		count++;
	}
	if(first_time) first_time = false;

	if(count == NUM_POSITION)
	{
		finish = true;
		ReleaseCall();
	}
	else
	{
		SetHeadAngle(headingPosition[count][0],headingPosition[count][1],
			headingPosition[count][2],
			0.0,headingPosition[count][3]);
	}
}

void
BhvWalkingHeading::Initialize()
{
	count = 0;
	finish = false;
	first_time = true;
}

bool
BhvWalkingHeading::IsFinished()
{
	return finish;
}
