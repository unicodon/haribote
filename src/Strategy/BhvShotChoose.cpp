#include "StdAfx.h"
#include "BehaviorList.h"

#include <OPENR/OSyslog.h>

void BhvShotChoose::Execute()
{
}


bool BhvShotChoose::ShotChoose(int &mode, double &adj_dis)
{
	pos_tt = state.self.position.tt.val;
	pos_x = state.self.position.x.val;
	pos_y = state.self.position.y.val;
	if(pos_x>=2700.0) pos_x=2695.0;

	double goal_x=2700.0-pos_x;
	double goal_y=-pos_y;

	double pi=3.14159;
	double right=atan2(goal_y-400.0,goal_x);
	double left=atan2(goal_y+400.0,goal_x);
	left*=180.0/pi;
	right*=180.0/pi;

	hold = false;

	int g_dircLApp=bhvLastApproach.goalinfolastapp.drct;

	if(bhvLastApproach.goalinfolastapp.visible){
		if(g_dircLApp>30.0)			mode=HEADING_LEFT;  
		else if(g_dircLApp>20.0)	mode=DIVING_LEFT45;  
		else if(g_dircLApp>10.0)	mode=EJYA9_LEFT; 
		else if(g_dircLApp>-10.0)	mode=EJYA9_FRONT; 
		else if(g_dircLApp>-20.0)	mode=EJYA9_RIGHT; 
		else if(g_dircLApp>-30.0)	mode=DIVING_RIGHT45; 
		else						mode=HEADING_RIGHT; 
		
		return false;
	}else if(SetHoldArea()){
		hold=true;

	}else if(pos_x < -1350){
		if(pos_tt>left+180.0)			hold=true;	
		else if(pos_tt>left+50.0)		mode = HEADING_RIGHT;
		else if(pos_tt>left+40.0)		mode = DIVING_RIGHT45;
		else if(pos_tt>left+20.0)		mode = EJYA9_RIGHT;
		else if(pos_tt>right-20.0)		mode = EJYA9_FRONT;
		else if(pos_tt>right-40.0)		mode = EJYA9_LEFT;
		else if(pos_tt>right-50.0)		mode = DIVING_LEFT45;
		else if(pos_tt>right-180.0)		mode = HEADING_LEFT;
		else							hold=true;

	}else{
		if(pos_tt>left+180.0)			hold=true;	
		else if(pos_tt>left+50.0)		mode = HEADING_RIGHT;
		else if(pos_tt>left+40.0)		mode = DIVING_RIGHT45;
		else if(pos_tt>left+10.0)		mode = EJYA9_RIGHT;
		else if(pos_tt > right-10.0)	mode = EJYA9_FRONT;
		else if(pos_tt > right-40.0)	mode = EJYA9_LEFT;
		else if(pos_tt > right-50.0)	mode = DIVING_LEFT45;
		else if(pos_tt > right-180.0)	mode = HEADING_LEFT;
		else							hold=true;
	}

	//mode = HEADING_RIGHT;
	AdjustPram(mode,adj_dis);
	return hold;	//return true;

}

bool BhvShotChoose::SetHoldArea(){
	//if(pos_x>-2700)	return true;
	//else			return false;
	return true;
}

void BhvShotChoose::AdjustPram(int &mode, double &adj_dis)
{
	//ボール距離がadj_dis[mm]になった時シュートを打ちます
	switch(mode)
	{
		case HEADING_RIGHT:
			adj_dis = 180;	break;

		case HOOKE_RIGHT_45:
			adj_dis = 180;	break;

		case DIVING_RIGHT45:
			adj_dis = 180;	break;

		case EJYA9_RIGHT:
			adj_dis = 200;	break;

		case MUNESHOT_00:
			adj_dis = 180;	break;

		case HG_FRONT:
			adj_dis = 180;	break;
		
		case EJYA9_FRONT:
			adj_dis = 190;	break;

		case SHOOT_FRONT:
			adj_dis = 180;	break;

		case EJYA9_LEFT:
			adj_dis = 195;	break;

		case DIVING_LEFT45:
			adj_dis = 180;	break;

		case HOOKE_LEFT_45:
			adj_dis = 180;	break;

		case HEADING_LEFT:
			adj_dis = 180;	break;
	}

}

