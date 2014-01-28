#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "../MCL/ObserveGoal.h"
#include "DetectGoal.h"
#include "../State/State.h"
#include "../Motion/MotionMaker.h"

void DetectGoal::Execute(){
#define GOAL_PIXEL_TH 100

	static int lostcounter = 100;
	lostcounter++;
	static int mygoal_lostcounter = 100;
	mygoal_lostcounter++;
	if(!MotionMaker::IsFinished())return;
	state.myGoalVisible = false;
	state.enemyGoalVisible = false;



	if(lostcounter > 10){
		state.goalVisible = false;
		double x = state.self.position.x.val;
		double y = state.self.position.y.val;
		double tt = state.self.position.tt.val;
		state.goalDirection = (atan2(-y,(2700 - x)) * 180 / 3.1416) - tt;
		if(state.goalDirection > 180)state.goalDirection -= 360;
		if(state.goalDirection < -180)state.goalDirection += 360;
	}
	else	state.goalVisible = true;

	if(mygoal_lostcounter > 10){
		state.goalVisibleOfOurs = false;
		double x = state.self.position.x.val;
		double y = state.self.position.y.val;
		double tt = state.self.position.tt.val;
		state.goalDirectionOfOurs = (atan2(-y,(-2700 - x)) * 180 / 3.1416) - tt;
		if(state.goalDirectionOfOurs > 180)state.goalDirectionOfOurs -= 360;
		if(state.goalDirectionOfOurs < -180)state.goalDirectionOfOurs += 360;
	}
	else	state.goalVisibleOfOurs = true;


	if(state.self.horizone.perpend)
		return;
	//傾きと切片
	double tilt = state.self.horizone.tilt;
	int intercept = (int)(-IMAGE_WIDTH/2*tilt + state.self.horizone.intercept + IMAGE_HEIGHT/2);

	int left = 0, right = 0;
	int top = (intercept < 20) ? 0 : intercept - 20;

	//YellowGoal
	if(state.self.cdt[YELLOW].value > GOAL_PIXEL_TH){
		int maxwidth = 0;
		int maxv = 0;
		int width = 0;
		int u = 0;
		int maxu = 0;
		int max_left = 0;
		int max_right = 0;

		for(int v = top;v < IMAGE_HEIGHT; v += 3){ 
			width = ScanLineYellow(tilt,v,u,left,right);
			if(width > maxwidth){
				maxwidth = width;
				maxv = v;
				maxu = u;
				max_left  = left;
				max_right = right;
			}
		}
		if(maxwidth > 15){
			int yellow = ScanVerticalLineYellow(tilt,maxu,maxv);
			if(yellow > 5){
				int v;
				GoalInformation gi;
				gi.goal_color_id = YELLOW;
				v = (int)(intercept + max_left * tilt);
				gi.directionLeft = PixelToDirection(max_left,v);
				v = (int)(intercept + max_right * tilt);
				gi.directionRight = PixelToDirection(max_right,v);

				//ゴールの方向セット
				if(state.game.team_color == TEAM_COLOR_BLUE){
					lostcounter = 0;
					state.goalDirection = PixelToDirection(maxu,maxv);
					state.enemyGoalVisible = true;
					state.enemyGoalInfo = gi;
				}else{
					mygoal_lostcounter = 0;
					state.goalDirectionOfOurs = PixelToDirection(maxu,maxv);
					state.myGoalVisible = true;
					state.myGoalInfo = gi;
				}

				ObserveGoal::Observation(gi);
				
//				OSYSPRINT(("YellowGoal left %.1f, right %.1f\n",gi.directionLeft, gi.directionRight));
			}

		}
	}

	//SkyBlue
	if(state.self.cdt[SKYBLUE].value > GOAL_PIXEL_TH){
		int maxwidth = 0;
		int maxv = 0;
		int width = 0;
		int u = 0;
		int maxu = 0;
		int max_left = 0;
		int max_right = 0;
		for(int v = top;v < IMAGE_HEIGHT; v += 3){ 
			width = ScanLineSkyblue(tilt,v,u,left,right);
			if(width > maxwidth){
				maxwidth = width;
				maxv = v;
				maxu = u;
				max_left  = left;
				max_right = right;
			}
		}
		if(maxwidth > 15){
			int skyblue = ScanVerticalLineSkyblue(tilt,maxu,maxv);
			if(skyblue > 5){
				int v;
				GoalInformation gi;
				gi.goal_color_id = SKYBLUE;
				v = (int)(intercept + max_left * tilt);
				gi.directionLeft = PixelToDirection(max_left,v);
				v = (int)(intercept + max_right * tilt);
				gi.directionRight = PixelToDirection(max_right,v);
				ObserveGoal::Observation(gi);

				//ゴールの方向セット
				if(state.game.team_color == TEAM_COLOR_RED){
					lostcounter = 0;
					state.goalDirection = PixelToDirection(maxu,maxv);
					state.enemyGoalVisible = true;
					state.enemyGoalInfo = gi;
				}else{
					mygoal_lostcounter = 0;
					state.goalDirectionOfOurs = PixelToDirection(maxu,maxv);
					state.myGoalVisible = true;
					state.myGoalInfo = gi;
				}

//				OSYSPRINT(("SkyblueGoal left %.1f, right %.1f\n",gi.directionLeft, gi.directionRight));
			}
		}
	}

}

int DetectGoal::ScanLineYellow(double tilt, int intercept, int &u, int &left, int &right){
	int mode = 0;
	const int gap = 4;
	int gapcount = 0;
	for(int x = 0;x < IMAGE_WIDTH;x++){
		int y = (int)(intercept + x*tilt);
		if(y < 0 || y >= IMAGE_HEIGHT)
			continue;

//		state.self.image[(x+y*IMAGE_WIDTH)*3] = 255;//test

		if(state.self.CDTimage[x+y*IMAGE_WIDTH] == YELLOW){
			if(mode == 0)
				gapcount++;

			if(mode == 1){
				right = x;
				gapcount = 0;
			}

			if(mode == 0 && gapcount > gap){
				left = x - gap;
				mode = 1;
				continue;
			}
		}else{
			if(mode == 0)
				gapcount = 0;
			if(mode == 1)
				gapcount++;

			if(mode == 1 && gapcount > gap){
				mode = 2;
			}
		}
	}

	int width = right - left + 1;
	u = (right+left)/2;
	return width;
}

int DetectGoal::ScanLineSkyblue(double tilt, int intercept, int &u, int &left, int &right){
	int mode = 0;
	const int gap = 4;
	int gapcount = 0;
	for(int x = 0;x < IMAGE_WIDTH;x++){
		int y = (int)(intercept + x*tilt);
		if(y < 0 || y >= IMAGE_HEIGHT)
			continue;

//		state.self.image[(x+y*IMAGE_WIDTH)*3] = 255;//test

		if(state.self.CDTimage[x+y*IMAGE_WIDTH] == SKYBLUE){
			if(mode == 0)
				gapcount++;

			if(mode == 1){
				right = x;
				gapcount = 0;
			}

			if(mode == 0 && gapcount > gap){
				left = x - gap;
				mode = 1;
				continue;
			}
		}else{
			if(mode == 0)
				gapcount = 0;
			if(mode == 1)
				gapcount++;

			if(mode == 1 && gapcount > gap){
				mode = 2;
			}
		}
	}

	int width = right - left + 1;
	u = (right+left)/2;
	return width;
}


int DetectGoal::ScanVerticalLineYellow(double tilt, int x, int y){
	int u0 = (int)(x + y*tilt);
	int sum = 0;
	int sum_pink = 0;
	int sum_white = 0;

	for(int v = 0;v < IMAGE_HEIGHT;v++){
		int u = (int)(-v * tilt + u0);
		if(u < 0 || u >= IMAGE_WIDTH || v < 0 || v > IMAGE_HEIGHT)
			continue;

//		state.self.image[(u+v*IMAGE_WIDTH)*3+2] = 255;//test

		if(state.self.CDTimage[u+v*IMAGE_WIDTH] == YELLOW)
			sum++;
		if(state.self.CDTimage[u+v*IMAGE_WIDTH] == PINK)
			sum_pink++;
		if(state.self.CDTimage[u+v*IMAGE_WIDTH] == WHITE && v > y)
			sum_white++;
	}

	if(sum_pink > sum || sum_pink > 3 || sum_white > 7)
		return 0;

	return sum;
}

int DetectGoal::ScanVerticalLineSkyblue(double tilt, int x, int y){
	int u0 = (int)(x + y*tilt);
	int sum = 0;
	int sum_pink = 0;
	int sum_white = 0;
	for(int v = 0;v < IMAGE_HEIGHT;v++){
		int u = (int)(-v * tilt + u0);
		if(u < 0 || u >= IMAGE_WIDTH || v < 0 || v > IMAGE_HEIGHT)
			continue;

//		state.self.image[(u+v*IMAGE_WIDTH)*3+2] = 255;//test

		if(state.self.CDTimage[u+v*IMAGE_WIDTH] == SKYBLUE)
			sum++;
		if(state.self.CDTimage[u+v*IMAGE_WIDTH] == PINK)
			sum_pink++;
		if(state.self.CDTimage[u+v*IMAGE_WIDTH] == WHITE && v > y)
			sum_white++;
	}

	if(sum_pink > sum || sum_pink > 3 || sum_white > 7)
		return 0;

	return sum;
}
