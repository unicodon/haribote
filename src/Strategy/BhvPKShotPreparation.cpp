#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/GaitMaker.h"
#include "../Vision/DetectBall.h"

#define PK


BhvPKShotPreparation::BhvPKShotPreparation() 
{
}

void BhvPKShotPreparation::Execute()
{
	ReserveCall();
	
	double theta = state.self.position.tt.val;
	
	if(state.goalVisible && goal_observation)
	{
		goal_direction = (goal_direction*goal_count + state.goalDirection) / (goal_count + 1.0);
		goal_count += 1.0;
		//OSYSPRINT(("GOAL: %f %f %f %d\n",state.goalDirection,goal_direction,goal_count,state.self.cdt[SKYBLUE].value));
	}
	if(mode == BALL_APPROACH_HEAD)
	{
		//Žñ•s—˜
		//OSYSPRINT(("HEAD LOOK\n"));
		head_start = true;
		goal_observation = true;
		//DetectBall::off = false;
		SetMotion("GRAB_LOOK");
		if(!MotionMaker::IsFinished()) mode = BALL_APPROACH_WALK;
	}
	else if(mode == BALL_APPROACH_WALK || mode == BALL_APPROACH_WALKING)
	{
		if(state.game.team_color == TEAM_COLOR_RED)
		{
			if(state.self.cdt[SKYBLUE].value > 7000 && fabs(state.self.sensor.jointValue[PAN]) < 10.0) goal_near = true;
		}
		if(state.game.team_color == TEAM_COLOR_BLUE)
		{
			if(state.self.cdt[YELLOW].value > 7000 && fabs(state.self.sensor.jointValue[PAN]) < 10.0) goal_near = true;
		}
		if(MotionMaker::IsFinished())
		{
			goal_observation = false;
			double goal_direction_localization = atan2(-state.self.position.y.val,2700.0-state.self.position.x.val)*180.0/PI - state.self.position.tt.val;

			if(!GaitMaker::IsNextWalkSet())
			{
				roll_count++;
				head_start = false;
			}
			if(false)//head_start && roll_count == 0 && ((goal_count > 2.0 && fabs(goal_direction) < 15.0) || (goal_count <= 2.0 && fabs(goal_direction_localization) < 15.0) || goal_near))
			{
				//mode = BALL_APPROACH_SHOT;
				//roll_count = 0;
			}
			else
			{
				SetHeadAngle(-60, 0, 45, -55, 300);
				static PRESETWALKINDEX walk;
				double shot_direction;
				static int max_roll_count = 0;//‚Â‚©‚ñ‚Å‰ñ‚é•à”(+1:””Ê‚ÌŽ–î‚É‚æ‚è)
				if(roll_count == 0)
				{
					if(goal_count > 2)
					{
						shot_direction = goal_direction;
					}
					else
					{
						double x = state.self.position.x.val;
						double y = state.self.position.y.val;
						double t = state.self.position.tt.val;
						shot_direction = atan2(-y,2700.0-x)*180.0/PI - t;
						while(shot_direction > 180.0) shot_direction -= 360.0;
						while(shot_direction < -180.0) shot_direction -= 360.0;
					}

					//OSYSPRINT(("shot direction: %f\n",shot_direction));

					if(100.0 < shot_direction)
					{
						max_roll_count = SetRollCount(shot_direction - 90.0);
						walk = BALL_ROTATE_LEFT;
						bhvPKShot.SetDirection(PK_SHOT_LEFT);
					}
					else if(80.0 <= shot_direction)
					{
						bhvPKShot.SetDirection(PK_SHOT_LEFT);
						mode = BALL_APPROACH_SHOT;
					}
					else if(10.0 < shot_direction)
					{
						max_roll_count = SetRollCount(shot_direction);
						walk = BALL_ROTATE_LEFT;
						bhvPKShot.SetDirection(PK_SHOT_CENTER);
					}
					else if( -10.0 <= shot_direction)
					{
						bhvPKShot.SetDirection(PK_SHOT_CENTER);
						mode = BALL_APPROACH_SHOT;
					}
					else if(-80.0 < shot_direction)
					{
						max_roll_count = SetRollCount(shot_direction);
						walk = BALL_ROTATE_RIGHT;
						bhvPKShot.SetDirection(PK_SHOT_CENTER);
					}
					else if(shot_direction <= -100.0)
					{
						bhvPKShot.SetDirection(PK_SHOT_RIGHT);
						mode = BALL_APPROACH_SHOT;
					}
					else
					{
						max_roll_count = SetRollCount(shot_direction + 90);
						walk = BALL_ROTATE_RIGHT;
						bhvPKShot.SetDirection(PK_SHOT_RIGHT);
					}
				}
				//if(mode == BALL_APPROACH_WALKING && GaitMaker::IsNextWalkSet()) mode = BALL_APPROACH_HEAD;
				if(roll_count >= max_roll_count)
				{
					mode = BALL_APPROACH_SHOT;
					//SetPresetWalk(WALK_STOP);
					//return;
					/*if(state.goalVisible || fabs(state.self.position.tt.val) < 20.0)
					{
						goal_count = 0.0;
						goal_direction = 0.0;
						roll_count = 0;
						if(state.goalVisible)	mode = BALL_APPROACH_SHOT;
						else					mode = BALL_APPROACH_HEAD;
					}
					if(roll_count >= 2)
					{
						mode = BALL_APPROACH_HEAD;
						goal_count = 0.0;
						goal_direction = 0.0;
						roll_count = 0;
					}*/
				}
				if(mode != BALL_APPROACH_SHOT) SetPresetWalk(walk);
			}
		}
	}
	if(mode == BALL_APPROACH_SHOT)
	{
		ReleaseCall();
		bhvPKShot.Execute();
		//DetectBall::off = true;
	}
}

#define MARGIN 10.0
int BhvPKShotPreparation::SetRollCount(double shot_direction)
{
	static double left_first_step = 8.55;
	static double right_first_step = -17.85;
	static double left_step = 45.75;
	static double right_step = -44.85;
	if(shot_direction > 0.0)
	{
		if(shot_direction + MARGIN >= left_first_step + left_step)
		{
			return 2;
		}
		else
		{
			shot_direction -= left_first_step;
			return ((int)(shot_direction/left_step)) + 2;
		}
	}
	else
	{
		if(shot_direction - MARGIN <= right_first_step + right_step)
		{
			return 2;
		}
		else
		{
			shot_direction -= right_first_step;
			return ((int)(shot_direction/right_step)) + 2;
		}
	}
}

void BhvPKShotPreparation::Initialize()
{
	roll_count = 0;
	mode = BALL_APPROACH_HEAD;
	goal_direction = 0.0;
	goal_count = 0.0;
	goal_observation = true;
	count_roll_end = 0;
	head_start = false;
	goal_near = false;
}
