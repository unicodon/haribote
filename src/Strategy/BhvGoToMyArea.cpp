#include "StdAfx.h"
#include "BehaviorList.h"
#include "../State/State.h"
#include "../Motion/GaitMaker.h"

static unsigned int cnt_mf_stay = 0;

void BhvGoToMyArea::Execute(){
	//AIBOのロボットIDで守るエリアを分割
	int walk = WALK_ROTATE_LEFT360;
	static int id = state.game.robot_id;
	double goal_x,goal_y,goal_t;

	GetPosition(id, goal_x, goal_y, goal_t);

	if(mode == GO_TO_AREA_GO && InSpot(goal_x,goal_y,goal_t))	mode = GO_TO_AREA_STAY;
	if(mode == GO_TO_AREA_STAY && !InArea(goal_x,goal_y))		mode = GO_TO_AREA_GO;
	
	if(mode == GO_TO_AREA_GO)
	{
		SetHeadMode(HEAD_SWING_BALL_SEARCH);
		bhvMoveToPosDP.SetDestination(goal_x,goal_y,goal_t);
		bhvMoveToPosDP.Execute();
	}
	else if(mode == GO_TO_AREA_STAY)
	{
		//SetHeadMode(HEAD_SWING_BALL_SEARCH);
		SetHeadMode(HEAD_SWING_LEFT);
		switch(id){
			case 1:
				SetPresetWalk(WALK_STOP);
				break;
			
			case 2:
				walk = WALK_ROTATE_LEFT360;//270;//SetPresetWalk(WALK_ROTATE_LEFT90);
				cnt_mf_stay++;

				if(cnt_mf_stay < 100)		mode_mf = GO_TO_AREA_MIDDLE;
				else if(cnt_mf_stay < 200)	mode_mf = GO_TO_AREA_LOW;
				else if(cnt_mf_stay < 300)	mode_mf = GO_TO_AREA_HIGH;
				else{
					mode_mf = GO_TO_AREA_MIDDLE;
					cnt_mf_stay = 0;
				}
				break;
		
			case 3:
				cnt_mf_stay++;
				if(mode_mf == GO_TO_AREA_HIGH)	walk = WALK_ROTATE_LEFT360;//270;//SetPresetWalk(WALK_ROTATE_LEFT90);//LEFT10);
				else							walk = WALK_ROTATE_LEFT360;//270;//SetPresetWalk(WALK_ROTATE_LEFT90);

				if(cnt_mf_stay < 100)		mode_mf = GO_TO_AREA_MIDDLE;
				else if(cnt_mf_stay < 200)	mode_mf = GO_TO_AREA_LOW;
				//else if(cnt_mf_stay < 300)	mode_mf = GO_TO_AREA_MIDDLE;
				else if(cnt_mf_stay < 300)	mode_mf = GO_TO_AREA_HIGH;
				else{
					mode_mf = GO_TO_AREA_MIDDLE;
					cnt_mf_stay = 0;
				}
				break;

			case 4:
				cnt_mf_stay++;
				walk = WALK_ROTATE_LEFT360;//270;//SetPresetWalk(WALK_ROTATE_LEFT90);//WALK_ROLL_RIGHT_R40);
				if(cnt_mf_stay < 100)		mode_mf = GO_TO_AREA_MIDDLE;
				else if(cnt_mf_stay < 200)	mode_mf = GO_TO_AREA_LOW;
				//else if(cnt_mf_stay < 300)	mode_mf = GO_TO_AREA_MIDDLE;
				else if(cnt_mf_stay < 300)	mode_mf = GO_TO_AREA_HIGH;
				else{
					mode_mf = GO_TO_AREA_MIDDLE;
					cnt_mf_stay = 0;
				}
				break;
		}
		//if(GaitMaker::IsExpiring()) SetPresetWalk(walk);
		SetPresetWalk(walk);
	}
	return;
}

bool
BhvGoToMyArea::InArea(double x,double y)
{
	return fabs(state.self.position.x.val - x) < 300//500.0
		&& fabs(state.self.position.y.val - y) < 300;//500.0;
}

bool
BhvGoToMyArea::InSpot(double x,double y,double t)
{
	return fabs(state.self.position.x.val - x) < 200//250.0
		&& fabs(state.self.position.y.val - y) < 200;//250.0;
		//&& fabs(state.self.position.tt.val - t) < 20.0; //θは使っちゃ駄目
}

void
BhvGoToMyArea::Initialize()
{
	mode = GO_TO_AREA_GO;
	mode_mf = GO_TO_AREA_MIDDLE;
	cnt_mf_stay = 0;
}

void BhvGoToMyArea::GetPosition(int id, double &goal_x, double &goal_y, double &goal_t)
{
	
	if(id == 1)//Keeper
	{
		goal_x = -2700;
		goal_y = 0;
		goal_t = 0;
	}
	else if(id == 2)//DF
	{
		if(state.team_mate[3].connected && state.team_mate[3].data.penalty == PENALTY_NONE){//更新中＆プレイ中
			if(mode_mf == GO_TO_AREA_MIDDLE){
				goal_x = -1000.0;
				goal_y = -500;//800.0;
				goal_t = 120;
			}
			else if(mode_mf == GO_TO_AREA_LOW){
				goal_x = -1400.0;
				goal_y = -500;//800.0;
				goal_t = 150;
			}
			else{
				goal_x = -700.0;
				goal_y = -500;//800.0;
				goal_t = 90;
			}
			/*goal_x = -1350.0;
			goal_y = -600.0;
			goal_t = 30;*/
		}
		else{
			if(mode_mf == GO_TO_AREA_MIDDLE){
			goal_x = -1500.0;
			goal_y = 0.0;
			goal_t = 170;
			}
			else if(mode_mf == GO_TO_AREA_LOW){
				goal_x = -1500.0;
				goal_y = -500;//800.0;
				goal_t = 150;
			}
			else{
				goal_x = -1500.0;
				goal_y = 500;//800.0;
				goal_t = -150;
			}
			/*goal_x = -1350;
			goal_y = -300;//700;//-1000;
			goal_t = 60;//50;*/
		}
	}
	else if(id == 3)//FW⇒MFに変更？takesita 0705
	{
		if(state.team_mate[2].connected && state.team_mate[2].data.penalty == PENALTY_NONE){
			if(mode_mf == GO_TO_AREA_MIDDLE){
				goal_x = -900.0;
				goal_y = 500;//800.0;
				goal_t = -120;
			}
			else if(mode_mf == GO_TO_AREA_LOW){
				goal_x = -1200.0;
				goal_y = 500;//800.0;
				goal_t = -150;
			}
			else{
				goal_x = -600.0;
				goal_y = 500;//800.0;
				goal_t = -90;
			}
		}
		else{
			if(mode_mf == GO_TO_AREA_MIDDLE){
			goal_x = -700.0;
			goal_y = 0.0;
			goal_t = 170;
			}
			else if(mode_mf == GO_TO_AREA_LOW){
				goal_x = -700.0;
				goal_y = 500;//800.0;
				goal_t = -150;
			}
			else{
				goal_x = -700.0;
				goal_y = -500;//800.0;
				goal_t = 150;
			}
			/*goal_x = -900.0;
			goal_y = 600.0;
			goal_t = 0;//-60;*/
		}

	}
	else if(id == 4)//MF⇒FWに変更？takesita 0705
	{
		if(mode_mf == GO_TO_AREA_MIDDLE){
			goal_x = 500;//700.0;
			goal_y = 0.0;
			goal_t = 0;
		}
		else if(mode_mf == GO_TO_AREA_LOW){
			goal_x = 700;//1000.0;
			goal_y = -800.0;
			goal_t = 0;
		}
		else{
			goal_x = 700;//1000.0;
			goal_y = 800.0;
			goal_t = 0;
		}
	}
}

