#include "StdAfx.h"
#include "BehaviorList.h"

BhvReady::BhvReady(){
	go_goal_mouth = true;
	cnt_goal_mouth = 0;
}

void BhvReady::Reset(){
	go_goal_mouth = true;
	cnt_goal_mouth = 0;
}

void BhvReady::Execute(){
	int ID = state.game.robot_id;
	int TeamColor = state.game.team_color;

	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	int pos_t = (int)state.self.position.tt.val;

	if(state.game.kikkoff_team == TeamColor){
		switch(ID){
			case 1:
			default:
				if(go_goal_mouth)
				{
					SetFaceLED(FACE_A_ALL);
					bhvMoveToPosDP.SetDestination(-2050,0,0);
					if( abs(pos_x + 1800) < 200 && abs(pos_y < 200) && abs(pos_t) < 20)
					{
						cnt_goal_mouth++;
						if(cnt_goal_mouth > 5)	go_goal_mouth = false;
					}
					else	cnt_goal_mouth = 0;
				}
				else{
                    bhvMoveToPosDP.SetDestination(-2550,0,0);
				}
				//bhvMoveToPosDP.SetDestination(-2600,0,0);//175);//0);
				break;
			case 2:
				bhvMoveToPosDP.SetDestination(-1650,-600,0);
				break;
			case 3:
				bhvMoveToPosDP.SetDestination(-700,500,0);
				break;
			case 4:
				bhvMoveToPosDP.SetDestination(-350,0,0);//(-500,-500,0);
				break;
		}
	}
	else{
		switch(ID){
			case 1:
			default:
				if(go_goal_mouth)
				{
					SetFaceLED(FACE_A_ALL);
					bhvMoveToPosDP.SetDestination(-2050,0,0);
					if( abs(pos_x + 1800) < 200 && abs(pos_y < 200) && abs(pos_t) < 20)
					{
						cnt_goal_mouth++;
						if(cnt_goal_mouth > 5)	go_goal_mouth = false;
					}
					else	cnt_goal_mouth = 0;
				}
				else{
                    bhvMoveToPosDP.SetDestination(-2550,0,0);
				}
				//bhvMoveToPosDP.SetDestination(-2600,0,0);//175);//0);
				break;
			case 2:
				bhvMoveToPosDP.SetDestination(-1650,-800,0);
				break;
			case 3:
				bhvMoveToPosDP.SetDestination(-1650,300,0);
				break;
			case 4:
				bhvMoveToPosDP.SetDestination(-1650,800,0);
				break;
		}
	}
	bhvMoveToPosDP.Execute();//ition.Execute();
	SetHeadMode(HEAD_SWING_LOCALIZATION);
	
	SetFaceLED(FACE_READY);
}
