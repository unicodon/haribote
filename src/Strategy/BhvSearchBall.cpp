#include "StdAfx.h"
#include "BehaviorList.h"
#include "../config.h"
#include "../Motion/GaitMaker.h"
#include "../Motion/PresetWalkParam.h"
#include <OPENR/OSyslog.h>

#define BACK_NUM 2
#define ROLL_NUM 12
#define FORWARD_NUM 3
#define SWING_NUM 2
#define STAY_NUM 40

BhvSearchBall::BhvSearchBall(){
	Initialize();
}


void BhvSearchBall::Execute()
{
	static PRESETWALKINDEX walk;
	//ƒ{[ƒ‹‚ªŒ©‚¦‚Ä‚¢‚½‚ç‚±‚±‚Å‚Í‰½‚à‚µ‚È‚¢
	static unsigned int cnt_ball_visible = 0;

	if(state.ball.visible){
		cnt_ball_visible++;
		if(cnt_ball_visible>=10) Initialize();
		return;
	}else if(state.ball.losttime < 10){
		return;
	}else{
		cnt_ball_visible = 0;
	}
	//OSYSPRINT(("Mode is %d isStop %d nextStop %d nextFlag %d\n",mode,GaitMaker::isStop,GaitMaker::next_isStop,GaitMaker::IsNextWalkSet()));

	
	bhvLeavingField.Execute();
	SetFaceLED(FACE_GREENWAVE);
	switch(mode)
	{
		case SEARCH_BALL_STAY:
			SetHeadMode(HEAD_SWING_BALL_SEARCH_FAST);
			SetPresetWalk(WALK_STOP);

			if(count_stay > STAY_NUM){
				mode = SEARCH_BALL_ROLL;
				count_stay=0;
			}else{
				count_stay++;
			}

			return;

		case SEARCH_BALL_SWING:
			SetHeadMode(HEAD_SWING_BALL_SEARCH_FAST);
			if(swing_count==0){
				swing_count++;
				if(state.self.position.y.val > 0.0){
					SetPresetWalk(WALK_ROTATE_RIGHT360,2);
					swing_swith=false;
				}else{
					SetPresetWalk(WALK_ROTATE_LEFT360,2);
					swing_swith=true;		
				}
			}else if(swing_count > SWING_NUM){
				mode = SEARCH_BALL_BACK;
				swing_count=0;
			}else if(GaitMaker::IsStop()){
				swing_count++;
				if(swing_swith){
					SetPresetWalk(WALK_ROTATE_RIGHT360,4);
					swing_swith=false;
				}else{
					SetPresetWalk(WALK_ROTATE_LEFT360,4);
					swing_swith=true;
				}
			}
			return;

		case SEARCH_BALL_ROLL:
			//OSYSPRINT(("rollcnt=%d walk=%d\n",roll_count,walk));
			if(roll_count == 0)
			{
				roll_count++;
				if(state.self.position.y.val > 0.0){
					walk = WALK_ROTATE_RIGHT360;
					SetPresetWalk(WALK_ROTATE_RIGHT360,1);
					SetHeadMode(HEAD_SWING_BALL_SEARCH);
				}else{
					walk = WALK_ROTATE_LEFT360;
					SetPresetWalk(WALK_ROTATE_LEFT360,1);
					SetHeadMode(HEAD_SWING_BALL_SEARCH);
				}
			}else if(roll_count > ROLL_NUM){
				mode = SEARCH_BALL_END;
				roll_count = 0;
			}else if(GaitMaker::IsStop()){
				SetPresetWalk(walk);
				roll_count++;
			}
			return;

		case SEARCH_BALL_BACK:
//			OSYSPRINT(("SEARCH_BALL_BACK\n"));
			SetHeadMode(HEAD_SWING_BALL_SEARCH_FAST);
			if(back_count >= BACK_NUM){
				mode = SEARCH_BALL_FORWARD;
				back_count = 0;
			}else if(GaitMaker::IsStop()){
				back_count++;
				SetPresetWalk(WALK_BACK);
			}
			return;

		case SEARCH_BALL_FORWARD:
			SetHeadMode(HEAD_SWING_EIGHT);
			if(forward_count >= FORWARD_NUM)
			{
				mode = SEARCH_BALL_ROLL;
				forward_count = 0;
			}else if(GaitMaker::IsStop()){
				forward_count++;
				SetPresetWalk(WALK_FORWARD);
				
			}
			return;

		case SEARCH_BALL_END:
			//OSYSPRINT(("SEARCH_BALL_END\n"));
			SetHeadMode(HEAD_SWING_BALL_SEARCH_FAST);
			if(GaitMaker::IsStop()){
				mode = SEARCH_BALL_NEXT_BHV;
			}
			return;

		case SEARCH_BALL_NEXT_BHV:
			bhvGoToMyArea.Execute();
//			OSYSPRINT(("GOTOMYAREA\n"));
			return;;
		default:
			return;
	}
	//OSYSPRINT(("Mode is %d isStop %d nextStop %d nextFlag %d\n",mode,GaitMaker::isStop,GaitMaker::next_isStop,GaitMaker::IsNextWalkSet()));
}

void BhvSearchBall::Initialize()
{
	bhvGoToMyArea.Initialize();
	mode = SEARCH_BALL_STAY;
	count_stay = 0;
	roll_count = 0;
	back_count = 0;
	forward_count = 0;
	swing_count = 0;
	swing_swith = true;
}
