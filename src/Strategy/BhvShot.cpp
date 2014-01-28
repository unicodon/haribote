#include "StdAfx.h"
#include "BehaviorList.h"

#include <OPENR/OSyslog.h>
#include "../Motion/GaitMaker.h"

void BhvShot::Execute()
{
}


bool BhvShot::CalucShot(int &mode, double &x, double &y)
{
	double pos_tt = state.self.position.tt.val;
	double pos_x = state.self.position.x.val;
	double pos_y = state.self.position.y.val;
	if(pos_x>=2700.0) pos_x=2695.0;

	double goal_x=2700.0-pos_x;
	double goal_y=-pos_y;
	bool hold = false;
	double pi=3.14159;

	
	if(state.goalVisible){
		hold=false;
		if(state.goalDirection>20.0)		mode=HEADING_LEFT;
		else if(state.goalDirection>10.0)	mode=DIVING_LEFT45;
		else if(state.goalDirection>-10.0)	mode=SHOOT_FRONT;
		else if(state.goalDirection>-20.0)  mode=DIVING_RIGHT45;
		else								mode=HEADING_RIGHT;
	}else{
		double right=atan2(goal_y-400.0,goal_x);
		double left=atan2(goal_y+400.0,goal_x);
		left*=180.0/pi;
		right*=180.0/pi;

		if(pos_x > 1300){
			if(pos_y>500){
				if(pos_tt > 45)				hold = true;
				else if(pos_tt > 0)			mode = HEADING_RIGHT;
				else if(pos_tt > -45)		mode = DIVING_RIGHT45;
				else if(pos_tt > -60)		mode = SHOOT_FRONT;
				else if(pos_tt > -90)		mode = DIVING_LEFT45;
				else if(pos_tt > -170)		mode = HEADING_LEFT;
				else						hold = true;
			}else if(pos_y<-500){
				if (pos_tt > 170)			hold = true;
				else if(pos_tt > 90)		mode = HEADING_RIGHT;
				else if(pos_tt > 60)		mode = DIVING_RIGHT45;
				else if(pos_tt > 45)		mode = SHOOT_FRONT;
				else if(pos_tt >  0)		mode = DIVING_LEFT45;
				else if(pos_tt > -45)		mode = HEADING_LEFT;
				else						hold = true;
			}else{
				if(pos_tt > 130)			hold = true;
				else if(pos_tt > 40)		mode = HEADING_RIGHT;
				else if(pos_tt > 20)		mode = DIVING_RIGHT45;
				else if(pos_tt > -20)		mode = SHOOT_FRONT;
				else if(pos_tt > -40)		mode = DIVING_LEFT45;
				else if(pos_tt > -130)		mode = HEADING_LEFT;
				else						hold = true;
			}
		}else if(pos_x < -1500){
			if(pos_tt>left+90.0)			hold=true;	
			else if(pos_tt>left+50.0)		mode = HEADING_RIGHT;
			else if(pos_tt>left+25.0)		mode = DIVING_RIGHT45;
			else if(pos_tt>right-25.0)		mode = SHOOT_FRONT;
			else if(pos_tt>right-50.0)		mode = DIVING_LEFT45;
			else if(pos_tt>right-90.0)		mode = HEADING_LEFT;
			else							hold=true;

		}else{
			if(pos_tt>left+100.0)			hold=true;	
			else if(pos_tt>left+30.0)		mode = HEADING_RIGHT;
			else if(pos_tt>left+10.0)		mode = DIVING_RIGHT45;
			else if(pos_tt > right-10.0)	mode = SHOOT_FRONT;	
			else if(pos_tt > right-30.0)	mode = DIVING_LEFT45;
			else if(pos_tt >right-100.0)	mode = HEADING_LEFT;
			else							hold=true;
		}
	}

	double ball_x = 0; 
	double ball_y = 0;

	//胸に当たっているときはball_xの値を大きく
	//胸から遠い時はball_xの値を小さくしましょう

	switch(mode)
	{
		case HEADING_RIGHT:
			x = ball_x;
			y = ball_y;
			break;


		case HOOKE_RIGHT_45:
			x = ball_x;
			y = ball_y;
			break;

		case DIVING_RIGHT45:
			x = ball_x;
			y = ball_y;
			break;

		case MUNESHOT_00:
			x = ball_x;
			y = ball_y;
			break;

		case SHOOT_FRONT:
			x = ball_x;
			y = ball_y;
			break;
		
		case DIVING_LEFT45:
			x = ball_x;
			y = ball_y;
			break;
			
		case HOOKE_LEFT_45:
			x = ball_x;
			y = ball_y;
			break;

		case HEADING_LEFT:
			x = ball_x;
			y = ball_y;
			break;
	}
	//mode = DIVING_LEFT45;
	return hold;
	//return true;
}


bool BhvShot::HoldShot2()
{
	int pos_x =(int)(state.self.position.x.val);
	int pos_y =(int)(state.self.position.y.val);
	int pos_tt =(int)(state.self.position.tt.val);
	if(pos_x>=2700) pos_x=2600;

	double goal_x=2700.0-pos_x;
	double goal_y=-pos_y;
	double pi=3.14159;

	double goal_direc=atan2(goal_y,goal_x)*180.0/pi-pos_tt;
	if(goal_direc < -180)		goal_direc += 360;
	else if(goal_direc > 180)	goal_direc -= 360;



	static bool rotate_flag=false;
	int rotate_num=0;
	double dead_rec=22.0;

	if(rotate_flag==false){

		rotate_num=(int)(abs(goal_direc)/dead_rec);
		if(rotate_num==0) rotate_num=1; //0だと永遠に回り続けるので

		if(goal_direc>dead_rec){
			SetPresetWalk(BALL_ROTATE_LEFT,rotate_num, true);
			OSYSPRINT(("right,goal=%.1f,tt=%d,rot=%d\n",goal_direc,pos_tt,rotate_num));
		}else if(goal_direc>-dead_rec){
			SetPresetWalk(0);
			OSYSPRINT(("stay,goal=%.1f,tt=%d,rot=%d\n",goal_direc,pos_tt,rotate_num));
		}else{
			SetPresetWalk(BALL_ROTATE_RIGHT,rotate_num,true);
			OSYSPRINT(("left,goal=%.1f,tt=%d,rot=%d\n",goal_direc,pos_tt,rotate_num));
		}
		rotate_flag=true;
	}

	//OSYSPRINT(("direc=%.1f",goal_direc));

	if(GaitMaker::IsStop()){
		if((goal_direc<30)&&(goal_direc>-30)){
			if(goal_direc > 20)			SetMotion("GRABSHOOT_FAKE_LEFT40");
			else if(goal_direc > 15)	SetMotion("GRABSHOOT_FAKE_LEFT20");
			else if(goal_direc > 5)		SetMotion("GRABSHOOT_FAKE_LEFT");
			else if(goal_direc > -5)	SetMotion("MUNESHOT_00");
			else if(goal_direc > -15)	SetMotion("GRABSHOOT_FAKE_RIGHT");
			else if(goal_direc > -20)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
			else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
			OSYSPRINT(("shot,goal=%.1f,tt=%d\n",goal_direc,pos_tt));
		}else{
			rotate_flag=false;
			OSYSPRINT(("unshot,goal=%.1f,tt=%d\n",goal_direc,pos_tt));
			return false;
		}
	}else{
		return false;
	}

	rotate_flag=false;//シュートが終わったら初期化
	return true;
}

bool BhvShot::HoldShot(){
	static int cnt_rotate = 1;
	static bool rotate_flag = false;
	static int num_rotate = 1;

	double theta = state.self.position.tt.val;
	double goal_drct = state.goalDirection;

	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	if(pos_x > 3000)	pos_x = 2700;
	if(pos_x < -3000)	pos_x = -2700;
	if(pos_y > 2000)	pos_y = 2000;
	if(pos_y < -2000)	pos_y = -2000;

	double gx = 2700.0 - pos_x;
	double gy = -pos_y;

	double own_goal_drct;
	if(state.goalVisible) own_goal_drct = goal_drct;
	else{
		double glb_goal_drct = atan2(gy, gx)*180.0/3.141592;
		own_goal_drct = glb_goal_drct - theta;
		if(own_goal_drct < -180)		own_goal_drct += 360;
		else if(own_goal_drct > 180)	own_goal_drct -= 360;
	}

	if(!rotate_flag){
		if(own_goal_drct > 0){
			if(own_goal_drct < 10)		num_rotate = 0 +1;
			else if(own_goal_drct < 30)	num_rotate = 1 +1;
			else if(own_goal_drct < 50)	num_rotate = 2 +1;
			else{
				num_rotate = (int)( (own_goal_drct + 11.65)/49.5 + 0.5 +1 );
				if(num_rotate < 3)	num_rotate = 2 +1;
			}

			num_rotate = 10;

			if(num_rotate == 1)	SetPresetWalk(BALL_ROTATE1_LEFT, num_rotate, true);
			else				SetPresetWalk(BALL_ROTATE_LEFT, num_rotate, true);
		}
		else{
			if(own_goal_drct < -10)			num_rotate = 0 +1;
			else if(own_goal_drct < -30)	num_rotate = 1 +1;
			else if(own_goal_drct < -50)	num_rotate = 2 +1;
			else{
				num_rotate = (int)( (fabs(own_goal_drct) + 13.2)/47.9 + 0.5 +1 );
				if(num_rotate < 2)	num_rotate = 2 +1;
			}

			num_rotate = 10;

			if(num_rotate == 1)	SetPresetWalk(BALL_ROTATE1_RIGHT, num_rotate, true);
			else				SetPresetWalk(BALL_ROTATE_RIGHT, num_rotate, true);
		}

		rotate_flag = true;
		cnt_rotate = 0;
	}

	cnt_rotate++;
	static bool pre_shot = false;

if( fabs(goal_drct) > 35){//ゴールまでの方向が大きかったら，回転を続ける
		if(num_rotate != 0 && ( cnt_rotate < 5 || !GaitMaker::IsExpiring() ) ){
			SetFaceLED(FACE_SMILE, true);
			pre_shot = true;
			return false;//廻る
		}

		if(pre_shot){
			SetPresetWalk(BALL_HOLD);
			pre_shot = false;
			return false;
		}
	}

	/*if(state.goalVisibleOfOurs){
			rotate_flag = false;
			return true;
	}*/

	if(!state.goalVisibleOfOurs){//自軍ゴールが見えてなければ，シュート
		if( (fabs(goal_drct) < 50) && state.goalVisible ){
			//if(pos_x > 2000)			SetPresetWalk(WALK_FORWARD);
			if(goal_drct > 30)			SetMotion("GRABSHOOT_FAKE_LEFT40");
			else if(goal_drct > 15)		SetMotion("GRABSHOOT_FAKE_LEFT20");
			else if(goal_drct > 5)		SetMotion("GRABSHOOT_FAKE_LEFT");
			else if(goal_drct > -5)		SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
			else if(goal_drct > -15)	SetMotion("GRABSHOOT_FAKE_RIGHT");
			else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
			else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
		}
		else{
			if(own_goal_drct > 30)			SetMotion("GRABSHOOT_FAKE_LEFT40");
			else if(own_goal_drct > 15)		SetMotion("GRABSHOOT_FAKE_LEFT20");
			else if(own_goal_drct > 5)		SetMotion("GRABSHOOT_FAKE_LEFT");
			else if(own_goal_drct > -5)		SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
			else if(own_goal_drct > -15)	SetMotion("GRABSHOOT_FAKE_RIGHT");
			else if(own_goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
			else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
		}
	}


	rotate_flag = false;
	return true;




	int area = 0;
	
	if(pos_x > 1350){
		if(pos_y > 400) 		area = 1;
		else if(pos_y > -400)	area = 2;
		else					area = 3;
	}
	else if(pos_x > 0){
		if(pos_y > 400) 		area = 4;
		else if(pos_y > -400)	area = 5;
		else					area = 6;
	}
	else{
		if(pos_y > 0)			area = 7;
		else					area = 8;
	}
	
	switch(area){
		case 1:
			if(state.goalVisible && goal_drct > 30){
				//if (goal_drct < 45) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(state.goalVisible && goal_drct < -20){
				//if (goal_drct > -35) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta < -40){
				//if(theta > -95)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else					SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			
			else if(theta > 0){//-80){	
			    //if(theta < -25)			SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else					SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}

			else{
				if( (fabs(goal_drct) < 30) && state.goalVisible ){
					SetFaceLED(FACE_SMILE, true);
					if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_RIGHT");
					else if(goal_drct > -10)	SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
				}
				else{
					SetFaceLED(FACE_REDLOVELY, true);
					if(theta > 70)				SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > -10)		SetMotion("GRABSHOOT_FAKE_RIGHT40");//SHOT_8MM");//SHOOT_FRONT");
					else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_RIGHT");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT");
				}
				cnt_rotate = 1;
			}
			break;
			
		case 2:
			if(state.goalVisible && goal_drct > 40){
				//if (goal_drct < 55) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(state.goalVisible && goal_drct < -40){
				//if (goal_drct > -55) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta < -20){
				//if(theta > -55)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else					SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			
			else if(theta > 20){	
			    //if(theta < 55)			SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else					SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}

			else{
				if( (fabs(goal_drct) < 40) && state.goalVisible ){
					SetFaceLED(FACE_SMILE, true);
					if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(goal_drct > -10)	SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
				}
				else{
					SetFaceLED(FACE_REDLOVELY, true);
					if(theta > 70)				SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > -10){		//SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
						if(pos_y > 0)			SetMotion("GRABSHOOT_FAKE_RIGHT");
						else					SetMotion("GRABSHOOT_FAKE_LEFT");
					}
					else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else						SetMotion("GRABSHOOT_FAKE_LEFT40");
				}
				cnt_rotate = 1;
			}
			break;
		
		case 3:
			if(state.goalVisible && goal_drct > 20){
				//if (goal_drct < 35) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(state.goalVisible && goal_drct < -30){
				//if (goal_drct > -45) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta > 40){	
			    //if(theta < 95)			SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else					SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta < 0){
				//if(theta > 25)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else					SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}			

			else{
				if( (fabs(goal_drct) < 30) && state.goalVisible ){
					SetFaceLED(FACE_SMILE, true);
					if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_LEFT");
					else if(goal_drct > -10)	SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_LEFT");
					else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
				}
				else{
					SetFaceLED(FACE_REDLOVELY, true);
					if(theta > 70)				SetMotion("GRABSHOOT_FAKE_LEFT");
					else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(theta > -10)		SetMotion("GRABSHOOT_FAKE_LEFT40");//SHOT_8MM");//SHOOT_FRONT");
					else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else						SetMotion("GRABSHOOT_FAKE_LEFT");
				}
				cnt_rotate = 1;
			}
			break;
		
		case 4:
			if(state.goalVisible && goal_drct > 40){
				//if (goal_drct < 55) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(state.goalVisible && goal_drct < -30){
				//if (goal_drct > -45) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta < -60){
				//if(theta > -85)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else					SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			
			else if(theta > -20){	
			    //if(theta < -15)			SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else					SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}

			else{
				if( (fabs(goal_drct) < 40) && state.goalVisible ){
					SetFaceLED(FACE_SMILE, true);
					if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_LEFT");
					else if(goal_drct > -10)	SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT");
					else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
				}
				else{
					SetFaceLED(FACE_REDLOVELY, true);
					if(theta > 70)				SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > -10)		SetMotion("GRABSHOOT_FAKE_RIGHT40");//40SHOT_8MM");//SHOOT_FRONT");
					else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else						SetMotion("GRABSHOOT_FAKE_LEFT20");
				}
				cnt_rotate = 1;
			}
			break;
		
		case 5:
			if(state.goalVisible && goal_drct > 50){
				//if (goal_drct < 65) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(state.goalVisible && goal_drct < -50){
				//if (goal_drct > -65) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta < -20){
				//if(theta > -65)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else					SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			
			else if(theta > 20){	
			    //if(theta < 65)			SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else					SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}

			else{
				if( (fabs(goal_drct) < 50) && state.goalVisible ){
					SetFaceLED(FACE_SMILE, true);
					if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(goal_drct > -10){	//SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
						if(pos_y > 0)			SetMotion("GRABSHOOT_FAKE_RIGHT");
						else					SetMotion("GRABSHOOT_FAKE_LEFT");
					}
					else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
				}
				else{
					SetFaceLED(FACE_REDLOVELY, true);
					if(theta > 70)				SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > -10)		SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else						SetMotion("GRABSHOOT_FAKE_LEFT40");
				}
				cnt_rotate = 1;
			}
			break;
		
		case 6:
			if(state.goalVisible && goal_drct > 30){
				//if (goal_drct < 45) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(state.goalVisible && goal_drct < -40){
				//if (goal_drct > -55) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta < 20){
				//if(theta > 15)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else					SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			
			else if(theta > 60){	
			    //if(theta < 85)			SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else					SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}

			else{
				if( (fabs(goal_drct) < 40) && state.goalVisible ){
					SetFaceLED(FACE_SMILE, true);
					if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(goal_drct > -10)	SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT");
					else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
				}
				else{
					SetFaceLED(FACE_REDLOVELY, true);
					if(theta > 70)				SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_LEFT");
					else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(theta > -10)		SetMotion("GRABSHOOT_FAKE_LEFT20");//SHOT_8MM");//SHOOT_FRONT");
					else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else						SetMotion("GRABSHOOT_FAKE_LEFT40");
				}
				cnt_rotate = 1;
			}
			break;
		
		case 7:
			if(state.goalVisible && goal_drct > 30){
				//if (goal_drct < 45) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(state.goalVisible && goal_drct < -30){
				//if (goal_drct > -45) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta < -20){
				//if(theta > -45)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else					SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			
			else if(theta > 20){	
			    //if(theta < 35)			SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else					SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}

			else{
				if( (fabs(goal_drct) < 30) && state.goalVisible ){
					SetFaceLED(FACE_SMILE, true);
					if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_RIGHT");
					else if(goal_drct > -10)	SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT");
					else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
				}
				else{
					SetFaceLED(FACE_REDLOVELY, true);
					if(theta > 70)				SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > -10)		SetMotion("GRABSHOOT_FAKE_RIGHT");//SHOT_8MM");//SHOOT_FRONT");
					else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_RIGHT");
					else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else						SetMotion("GRABSHOOT_FAKE_LEFT40");
				}
				cnt_rotate = 1;
			}
			break;
		
		case 8:
			if(state.goalVisible && goal_drct > 30){
				//if (goal_drct < 45) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(state.goalVisible && goal_drct < -30){
				//if (goal_drct > -45) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			else if(theta < -20){
				//if(theta > -35)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_LEFT, true);
				else					SetPresetWalk(BALL_ROTATE_LEFT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}
			
			else if(theta > 20){	
			    //if(theta < 45)			SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				if(cnt_rotate < 1)		SetPresetWalk(BALL_ROTATE1_RIGHT, true);
				else					SetPresetWalk(BALL_ROTATE_RIGHT, true);
				if(GaitMaker::IsExpiring())cnt_rotate++;
				return false;
			}

			else{
				if( (fabs(goal_drct) < 30) && state.goalVisible ){
					SetFaceLED(FACE_SMILE, true);
					if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_LEFT");
					else if(goal_drct > -10)	SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else						SetMotion("GRABSHOOT_FAKE_RIGHT40");
				}
				else{
					SetFaceLED(FACE_REDLOVELY, true);
					if(theta > 70)				SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT40");
					else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_RIGHT20");
					else if(theta > -10)		SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
					else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_LEFT20");
					else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_LEFT40");
					else						SetMotion("GRABSHOOT_FAKE_LEFT40");
				}
				cnt_rotate = 1;
			}
			break;
			
	}
	return true;
	//ゴール真正面のときは，ドリブル突っ込み
	/*if(state.goalVisible  && state.self.position.x.val > 2000){
		SetPresetWalk(WALK_STABLE_FORWARD);
		return true;
	}*/

	/*
	//相手ゴール方面を向いていないときは，回転
	if(state.goalVisible && goal_drct > 50){
		if (goal_drct < 65) 	SetPresetWalk(BALL_ROTATE1_LEFT, true);
		else				    SetPresetWalk(BALL_ROTATE_LEFT, true);
	}
	else if(theta < -60){
		if(theta > -75)			SetPresetWalk(BALL_ROTATE1_LEFT, true);
		else					SetPresetWalk(BALL_ROTATE_LEFT, true);
		//SetHeadAngle(-60, 0, 45, -55, 100, true);
		return false;
	}
	if(state.goalVisible && goal_drct < -50){
		if (goal_drct > -65) 	SetPresetWalk(BALL_ROTATE1_RIGHT, true);
		else				    SetPresetWalk(BALL_ROTATE_RIGHT, true);
	}
	else if(theta > 60){	
	    if(theta < 75)                SetPresetWalk(BALL_ROTATE1_RIGHT, true);
		else			              SetPresetWalk(BALL_ROTATE_RIGHT, true);
		//SetHeadAngle(-60, 0, 45, -55, 100, true);60
		return false;
	}

	else{
		if( (fabs(goal_drct) < 70) && state.goalVisible ){
			SetFaceLED(FACE_SMILE, true);
			if(goal_drct > 80)			SetMotion("GRABSHOOT_FAKE_LEFT60");
			else if(goal_drct > 60)		SetMotion("GRABSHOOT_FAKE_LEFT60");
			else if(goal_drct > 30)		SetMotion("GRABSHOOT_FAKE_LEFT30");
			else if(goal_drct > 10)		SetMotion("GRABSHOOT_FAKE_LEFT");
			else if(goal_drct > -10)	SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
			else if(goal_drct > -30)	SetMotion("GRABSHOOT_FAKE_RIGHT");
			else if(goal_drct > -60)	SetMotion("GRABSHOOT_FAKE_RIGHT30");
			else if(goal_drct > -80)	SetMotion("GRABSHOOT_FAKE_RIGHT60");
			else						SetMotion("GRABSHOOT_FAKE_RIGHT60");
		}
		else{
			SetFaceLED(FACE_REDLOVELY, true);
			if(theta > 70)				SetMotion("GRABSHOOT_FAKE_RIGHT60");
			else if(theta > 50)			SetMotion("GRABSHOOT_FAKE_RIGHT60");
			else if(theta > 30)			SetMotion("GRABSHOOT_FAKE_RIGHT30");
			else if(theta > 10)			SetMotion("GRABSHOOT_FAKE_RIGHT");
			else if(theta > -10)		SetMotion("SHOOT_FRONT");//SHOT_8MM");//SHOOT_FRONT");
			else if(theta > -30)		SetMotion("GRABSHOOT_FAKE_LEFT");
			else if(theta > -50)		SetMotion("GRABSHOOT_FAKE_LEFT30");
			else if(theta > -70)		SetMotion("GRABSHOOT_FAKE_LEFT60");
			else						SetMotion("GRABSHOOT_FAKE_LEFT60");
		}
        return true;
	}*/
}

bool SelectShot(int &mode, double &x, double &y)
{
	double goal_drct = state.goalDirection;
	double theta = state.self.position.tt.val;
	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	bool direct = true;

	
	if(state.goalVisible)
	{
		if(pos_x > 0)
		{
			if(abs(pos_y) <= 400)  
			{
				if(pos_x > 1350)
				{
					if	(goal_drct >   70)		direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
					else if(goal_drct > 40)		mode = HEADING_LEFT; 
					else if(goal_drct > 20)		mode = DIVING_LEFT45;
					else if(goal_drct > -20)	mode = SHOOT_FRONT;
					else if(goal_drct > -40)	mode = DIVING_RIGHT45;
					else if(goal_drct > -70)	mode = HEADING_RIGHT;
					else						direct=SelectShot_UnVisible(mode,pos_x,pos_y,theta);
				}
				else
				{
					if	(goal_drct > 70)		direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
					else if(goal_drct > 50) 	mode = HEADING_LEFT;
					else if(goal_drct > 15)		mode = DIVING_LEFT45;
					else if(goal_drct > -15)	mode = SHOOT_FRONT;
					else if(goal_drct > -50)	mode = DIVING_RIGHT45;
					else if(goal_drct > -70)	mode = HEADING_RIGHT;
					else						direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
				}
			}
			else if(pos_y > 400)
			{
				if(pos_x >1350)
				{
					if	(goal_drct > 70)		direct =SelectShot_UnVisible(mode,pos_x,pos_y,theta);
					else if(goal_drct > 7)		mode = DIVING_LEFT45;
					else if(goal_drct > -8)		mode = SHOOT_FRONT;
					else if(goal_drct > -53)	mode = DIVING_RIGHT45;
					else if(goal_drct > -70)	mode = HEADING_RIGHT;
					else						direct =SelectShot_UnVisible(mode,pos_x,pos_y,theta);
				}
				else
				{
					if	(goal_drct > 70)		direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
					else if(goal_drct > 32) 	mode = HEADING_LEFT;
					else if(goal_drct > 7)		mode = DIVING_LEFT45;
					else if(goal_drct > -8)		mode = SHOOT_FRONT;
					else if(goal_drct > -58)	mode = DIVING_RIGHT45;
					else if(goal_drct > -70)	mode = HEADING_RIGHT;
					else						direct =SelectShot_UnVisible(mode,pos_x,pos_y,theta);
				}
			}
			else
			{
				if(pos_x >1350)
				{
					if	(goal_drct > 70)		direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
					else if(goal_drct > 53)		mode = HEADING_LEFT;
					else if(goal_drct > 8)		mode = DIVING_LEFT45;
					else if(goal_drct > -7) 	mode = SHOOT_FRONT;
					else if(goal_drct > -70)	mode = DIVING_RIGHT45;
					else						direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
				}
				else
				{
					if	(goal_drct > 70)		direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
					else if(goal_drct > 58)		mode = HEADING_LEFT;
					else if(goal_drct > 8)		mode = DIVING_LEFT45;
					else if(goal_drct > -7)		mode = SHOOT_FRONT;
					else if(goal_drct > -32)	mode = DIVING_RIGHT45;
					else if(goal_drct > -70)	mode = HEADING_RIGHT;
					else						direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
				}
			}
		}
		else
		{
			if(goal_drct > 70)					direct =SelectShot_UnVisible(mode,pos_x,pos_y,theta);
			else if(goal_drct > 50) 			mode = HEADING_LEFT;
			else if(goal_drct > 30)				mode = DIVING_LEFT45;
			else if(goal_drct > -30)			mode = SHOOT_FRONT;
			else if(goal_drct > -50)			mode = DIVING_RIGHT45;
			else if(goal_drct > -70)			mode = HEADING_RIGHT;
			else								direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);
		}
	}
	else direct = SelectShot_UnVisible(mode,pos_x,pos_y,theta);

	//FOR TEST BY TAKE
	//if(state.self.position.y.val > 0)	mode = DIVING_LEFT45;
	//else								mode = DIVING_RIGHT45;

	//胸に当たっているときはball_xの値を大きく
	//胸から遠い時はball_xの値を小さくしましょう

	double ball_x = state.ball.x-145;//185;
	double ball_y = state.ball.y;

	ball_x = 0;
	ball_y = 0;

	switch(mode)
	{
		case HEADING_LEFT:
			x = ball_x;
			y = ball_y;
			break;

		case HEADING_RIGHT:
			x = ball_x;
			y = ball_y;
			break;

		case SHOOT_FRONT:
			x = ball_x;
			y = ball_y;
			break;

		case DIVING_LEFT45:
			x = ball_x;
			y = ball_y;
			break;

		case DIVING_RIGHT45:
			x = ball_x;
			y = ball_y;
			break;
	}
	
	return direct;
}

bool SelectShot_UnVisible(int &mode, int &pos_x, int &pos_y, double &theta)
{
	bool hold = true;
	if(pos_x > -1350)
	{
		if(abs(pos_y) <= 400)
		{
			if(pos_x > 1350)
			{
				if(theta > 130)			hold = false;
				else if(theta > 40)		mode = HEADING_RIGHT;
				else if(theta > 30)		mode = DIVING_RIGHT45;
				else if(theta > -30)	mode = SHOOT_FRONT;
				else if(theta > -40)	mode = DIVING_LEFT45;
				else if(theta > -130)	mode = HEADING_LEFT;
				else					hold = false;
			}
			else
			{
				if(theta > 80)			hold = false;
				else if(theta > 45)		mode = HEADING_RIGHT;
				else if(theta > 25)		mode = DIVING_RIGHT45;
				else if(theta > -25)	mode = SHOOT_FRONT;
				else if(theta > -45)	mode = DIVING_LEFT45;
				else if(theta > -80)	mode = HEADING_LEFT;
				else					hold = false;
			}
		}
		else if(pos_y > 400)
		{
			if(pos_x >1350)
			{
				if(theta > 45)			hold = false;
				else if(theta > 0)		mode = HEADING_RIGHT;
				else if(theta > -45)	mode = DIVING_RIGHT45;
				else if(theta > -60)	mode = SHOOT_FRONT;
				else if(theta > -90)	mode = DIVING_LEFT45;
				else if(theta > -170)	mode = HEADING_LEFT;
				else					hold = false;
			}
			else
			{
				if(theta > 60)			hold = false;
				else if(theta > 20)		mode = HEADING_RIGHT;
				else if(theta > -30)	mode = DIVING_RIGHT45;
				else if(theta > -45)	mode = SHOOT_FRONT;
				else if(theta > -70)	mode = DIVING_LEFT45;
				else if(theta > -135)	mode = HEADING_LEFT;
				else					hold = false;
			}
		}
		else
		{
			if(pos_x >1350)
			{
				if (theta > 170)		hold = false;
				else if(theta > 90)		mode = HEADING_RIGHT;
				else if(theta > 60)		mode = DIVING_RIGHT45;
				else if(theta > 45)		mode = SHOOT_FRONT;
				else if(theta >  0)		mode = DIVING_LEFT45;
				else if(theta > -45	)	mode = HEADING_LEFT;
				else					hold = false;
			}
			else
			{
				if (theta > 135	)		hold = false;
				else if(theta > 70)		mode = HEADING_RIGHT;
				else if(theta > 45)		mode = DIVING_RIGHT45;
				else if(theta > 30)		mode = SHOOT_FRONT;
				else if(theta > -20)	mode = DIVING_LEFT45;
				else if(theta > -60)	mode = HEADING_LEFT;
				else					hold = false;	
			}
		}
	}
	else hold = false;
	//if(hold = false) mode = SHOOT_FRONT;
	return hold;//true;
}

int SelectShotSimple(){
	int shot_mode;

	double goal_drct = state.goalDirection;
	double ours_goal_drct = state.goalDirectionOfOurs;
	double theta = state.self.position.tt.val;
	int pos_x = (int)state.self.position.x.val;
	int pos_y = (int)state.self.position.y.val;
	if(pos_x > 2700)	pos_x = 2700;
	if(pos_x < -2700)	pos_x = -2700;
	if(pos_y > 2000)	pos_y = 2000;
	if(pos_y < -2000)	pos_y = -2000;

	double gx = 2700.0 - pos_x;
	double gy = -pos_y;

	double goal_drct_pos;//自己位置から求めたゴール方向
	if(state.goalVisible) goal_drct_pos = goal_drct;
	else{
		double glb_goal_drct = atan2(gy, gx)*180.0/3.141592;
		goal_drct_pos = glb_goal_drct - theta;
		while(fabs(goal_drct_pos) > 180){
			if(goal_drct_pos < -180)		goal_drct_pos += 360;
			else if(goal_drct_pos > 180)	goal_drct_pos -= 360;
		}
	}

	//for rUNSWift
	if(state.goalVisible){
		if(pos_x > 1350){
			if(abs(pos_y) <= 400){
				//if( fabs(goal_drct) <= 20)		shot_mode = SHOOT_FRONT;
				if(goal_drct > 40)				shot_mode = HEADING_LEFT;
				else if(goal_drct > 20)			shot_mode = DIVING_LEFT45;
				else if(goal_drct > -20)		shot_mode = SHOOT_FRONT;
				else if(goal_drct > -40)		shot_mode = DIVING_RIGHT45;
				else							shot_mode = HEADING_RIGHT;
			}
			else if(pos_y > 400){
				//if(goal_drct > 30)				shot_mode = HEADING_LEFT;
				if(goal_drct > 10)				shot_mode = DIVING_LEFT45;
				else if(goal_drct > -10)		shot_mode = SHOOT_FRONT;
				else if(goal_drct > -50)		shot_mode = DIVING_RIGHT45;
				else							shot_mode = HEADING_RIGHT;
			}
			else{
				if(goal_drct > 50)				shot_mode = HEADING_LEFT;
				else if(goal_drct > 10)			shot_mode = DIVING_LEFT45;
				else if(goal_drct > -10)		shot_mode = SHOOT_FRONT;
				else							shot_mode = DIVING_RIGHT45;
				//else							shot_mode = HEADING_RIGHT;
			}
		}
		else{
			if(abs(pos_y) <= 400){
				//if( fabs(goal_drct) <= 20)		shot_mode = SHOOT_FRONT;
				if(goal_drct > 50)				shot_mode = HEADING_LEFT;
				else if(goal_drct > 15)			shot_mode = DIVING_LEFT45;
				else if(goal_drct > -15)		shot_mode = SHOOT_FRONT;
				else if(goal_drct > -50)		shot_mode = DIVING_RIGHT45;
				else							shot_mode = HEADING_RIGHT;
			}
			else if(pos_y > 400){
				if(goal_drct > 30)				shot_mode = HEADING_LEFT;
				else if(goal_drct > 10)			shot_mode = DIVING_LEFT45;
				else if(goal_drct > -10)		shot_mode = SHOOT_FRONT;
				else if(goal_drct > -60)		shot_mode = DIVING_RIGHT45;
				else							shot_mode = HEADING_RIGHT;
			}
			else{
				if(goal_drct > 60)				shot_mode = HEADING_LEFT;
				else if(goal_drct > 10)			shot_mode = DIVING_LEFT45;
				else if(goal_drct > -10)		shot_mode = SHOOT_FRONT;
				else if(goal_drct > -30)		shot_mode = DIVING_RIGHT45;
				else							shot_mode = HEADING_RIGHT;
			}
		}
	}
	else if(state.goalVisibleOfOurs){
		if(ours_goal_drct > 0)			shot_mode = HEADING_RIGHT;//左に見えてる
		else if(ours_goal_drct < 0)		shot_mode = HEADING_LEFT;//右に見えてる
	}
	else{
		if( fabs(goal_drct_pos) > 160) shot_mode = SHOOT_FRONT;//BACK_SHOT;
		else if(pos_x > 1350){

			if(abs(pos_y) <= 400){
				//if( fabs(goal_drct) <= 20)		shot_mode = SHOOT_FRONT;
				if(goal_drct_pos > 40)				shot_mode = HEADING_LEFT;
				else if(goal_drct_pos > 20)			shot_mode = DIVING_LEFT45;
				else if(goal_drct_pos > -20)		shot_mode = SHOOT_FRONT;
				else if(goal_drct_pos > -40)		shot_mode = DIVING_RIGHT45;
				else								shot_mode = HEADING_RIGHT;
			}
			else if(pos_y > 400){
				//if(goal_drct > 30)				shot_mode = HEADING_LEFT;
				if(goal_drct_pos > 10)				shot_mode = DIVING_LEFT45;
				else if(goal_drct_pos > -10)		shot_mode = SHOOT_FRONT;
				else if(goal_drct_pos > -50)		shot_mode = DIVING_RIGHT45;
				else								shot_mode = HEADING_RIGHT;
			}
			else{
				if(goal_drct_pos > 50)				shot_mode = HEADING_LEFT;
				else if(goal_drct_pos > 10)			shot_mode = DIVING_LEFT45;
				else if(goal_drct_pos > -10)		shot_mode = SHOOT_FRONT;
				else								shot_mode = DIVING_RIGHT45;
				//else							shot_mode = HEADING_RIGHT;
			}
		}
		else{
			if(abs(pos_y) <= 400){
				//if( fabs(goal_drct) <= 20)		shot_mode = SHOOT_FRONT;
				if(goal_drct_pos > 50)				shot_mode = HEADING_LEFT;
				else if(goal_drct_pos > 15)			shot_mode = DIVING_LEFT45;
				else if(goal_drct_pos > -15)		shot_mode = SHOOT_FRONT;
				else if(goal_drct_pos > -50)		shot_mode = DIVING_RIGHT45;
				else								shot_mode = HEADING_RIGHT;
			}
			else if(pos_y > 400){
				if(goal_drct_pos > 30)				shot_mode = HEADING_LEFT;
				else if(goal_drct_pos > 10)			shot_mode = DIVING_LEFT45;
				else if(goal_drct_pos > -10)		shot_mode = SHOOT_FRONT;
				else if(goal_drct_pos > -60)		shot_mode = DIVING_RIGHT45;
				else								shot_mode = HEADING_RIGHT;
			}
			else{
				if(goal_drct_pos > 60)				shot_mode = HEADING_LEFT;
				else if(goal_drct_pos > 10)			shot_mode = DIVING_LEFT45;
				else if(goal_drct_pos > -10)		shot_mode = SHOOT_FRONT;
				else if(goal_drct_pos > -30)		shot_mode = DIVING_RIGHT45;
				else								shot_mode = HEADING_RIGHT;
			}
		}
	}

	//takeshita for u-pen
	/*
	if(state.goalVisible){
		if( fabs(goal_drct) < 20)		shot_mode = SHOOT_FRONT;
		else if(goal_drct > 0)			shot_mode = HEADING_LEFT;
		else							shot_mode = HEADING_RIGHT;
	}
	else if(state.goalVisibleOfOurs){
		if(ours_goal_drct > 0)			shot_mode = HEADING_RIGHT;//左に見えてる
		else if(ours_goal_drct < 0)		shot_mode = HEADING_LEFT;//右に見えてる
	}
	else if(pos_x > 0){
		if( fabs(goal_drct_pos) < 30)	shot_mode = SHOOT_FRONT;
		else if(goal_drct_pos > 0)		shot_mode = HEADING_LEFT;
		else							shot_mode = HEADING_RIGHT;
	}
	else{
		if( fabs(goal_drct_pos) < 40)	shot_mode = SHOOT_FRONT;
		else if(goal_drct_pos > 0)		shot_mode = HEADING_LEFT;
		else							shot_mode = HEADING_RIGHT;
	}*/

	return shot_mode;

}
