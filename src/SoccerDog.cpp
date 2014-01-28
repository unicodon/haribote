#include "StdAfx.h"

#include <OPENR/OSyslog.h>
#include "joint.h"
#include "SoccerDog.h"
#include <OPENR/OPrimitiveControl.h>
#include "State/State.h"
#include "Soccerdog.h"
#include "Vision/DetectBall.h"
#include "Vision/DetectLandmark.h"
#include "Vision/DetectGoal.h"
#include "Vision/DetectLine.h"

#include "Utility/StopWatch.h"
#include "Utility/MtRand.h"

#include "Motion/GaitMaker.h"
#include "Motion/MotionMaker.h"
#include "Motion/HeadController.h"
#include "Motion/TailController.h"


//重要なインスタンスっす
State state;

bool SoccerDog::death = false;

SoccerDog::SoccerDog(){
	whitebalance = ocamparamWB_INDOOR_MODE;
//	whitebalance = ocamparamWB_OUTDOOR_MODE;
//	whitebalance = ocamparamWB_FL_MODE;

	cameragain = ocamparamGAIN_HIGH;
//	cameragain = ocamparamGAIN_MID;
//	cameragain = ocamparamGAIN_LOW;

//	shutterspeed = ocamparamSHUTTER_FAST;
	shutterspeed = ocamparamSHUTTER_MID;
//	shutterspeed = ocamparamSHUTTER_SLOW;

	gamecontroller_beacon_losttime = 100;

	now_goal_motion = false;
	now_finish_motion = false;
	highgain = false;
};

SoccerDog::~SoccerDog(){
};

void SoccerDog::LoadConfig(){
	FILE* file;

	OSYSPRINT(("---Load Config---\n"));

	file = fopen("./MS/CONFIG.TXT","r");
	if(file){
		const int maxlen = 256;
		char buf[maxlen];
		while(fgets(buf,maxlen-1,file)){
			if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
			if(strcmp(buf,"[team number]") == 0){
				if(fgets(buf,maxlen-1,file))
					state.game.team_number = atoi(buf);
				else break;
				OSYSPRINT(("Team number = %d\n",state.game.team_number));
				continue;
			}
			if(strcmp(buf,"[robot id]") == 0){
				if(fgets(buf,maxlen-1,file))
					state.game.robot_id = atoi(buf);
				else break;
				OSYSPRINT(("Robot ID = %d\n",state.game.robot_id));
				continue;
			}
			if(strcmp(buf,"[team color]") == 0){
				if(fgets(buf,maxlen-1,file)){
					if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
					if(strcmp(buf,"blue") == 0){
						state.game.team_color = TEAM_COLOR_BLUE;
						OSYSPRINT(("Team color is blue\n"));
					}else{
						state.game.team_color = TEAM_COLOR_RED;
						OSYSPRINT(("Team color is red\n"));
					}
				}else break;
				continue;
			}
			if(strcmp(buf,"[whitebalance]") == 0){
				if(fgets(buf,maxlen-1,file)){
					if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
					if(strcmp(buf,"fl") == 0){
						whitebalance = ocamparamWB_FL_MODE;
						OSYSPRINT(("whitebalance is fl mode\n"));
					}else if(strcmp(buf,"outdoor") == 0){
						whitebalance = ocamparamWB_OUTDOOR_MODE;
						OSYSPRINT(("whitebalance is outdoor mode\n"));
					}else{
						whitebalance = ocamparamWB_INDOOR_MODE;
						OSYSPRINT(("whitebalance is indoor mode\n"));
					}
				}else break;
				continue;
			}
			if(strcmp(buf,"[shutterspeed]") == 0){
				if(fgets(buf,maxlen-1,file)){
					if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
					if(strcmp(buf,"slow") == 0){
						shutterspeed = ocamparamSHUTTER_SLOW;
						OSYSPRINT(("shutterspeed is slow\n"));
					}else if(strcmp(buf,"mid") == 0){
						shutterspeed = ocamparamSHUTTER_MID;
						OSYSPRINT(("shutterspeed is mid\n"));
					}else{
						shutterspeed = ocamparamSHUTTER_FAST;
						OSYSPRINT(("shutterspeed is fast\n"));
					}
				}else break;
				continue;
			}
			if(strcmp(buf,"[cameragain]") == 0){
				if(fgets(buf,maxlen-1,file)){
					if(buf[strlen(buf)-1] == '\n') buf[strlen(buf)-1] = '\0';
					if(strcmp(buf,"low") == 0){
						cameragain = ocamparamGAIN_LOW;
						OSYSPRINT(("cameragain is low\n"));
					}else if(strcmp(buf,"mid") == 0){
						cameragain = ocamparamGAIN_MID;
						OSYSPRINT(("cameragain is mid\n"));
					}else{
						cameragain = ocamparamGAIN_HIGH;
						OSYSPRINT(("cameragain is high\n"));
					}
				}else break;
				continue;
			}
		}
		fclose(file);
	}
}

void SoccerDog::DoInit(){
	//おまじない

	InitState();
	gain_set = false;
	lastangle_init = false;
	
	PresetWalkParam::Init();
	PresetMotion::Init();

	GaitMaker::SetPresetWalk(WALK_STOP,0);

}

double dt = 0;
double dp = 0;
bool test_start = false;

void SoccerDog::DoStart(){

	//MCL
	//MCLSample* sample = new MCLSample();//for sample_viewer
	sample = new MCLSample();//for sample_viewer
	DeadRec::SetSample(sample);
	ObserveLandmark::SetSample(sample);
	ObserveGoal::SetSample(sample);
	ObserveLine::SetSample(sample);
	
	MtRand::init_genrand(time(NULL));
}

void SoccerDog::DoStop(){
}


void SoccerDog::DoDestroy(){
    
	delete sample;//for sample_viewer
}

/*
void SoccerDog::ReadyJoint(const OReadyEvent& event)
{
	//OSYSPRINT(("Ready Joint\n"));
	if(death)return;

	if(gain_set == false && lastangle_init){
		MotionMaker::SetMotion(GETUP);
		MotionMaker::StartMotion();

		static int counter = 0;
		if(counter++ > 1){
			SetJointGain();
			gain_set = true;
		}
	}

	SetJointValue(FindFreeRegion(jointRegions));
	return;
}
*/


void SoccerDog::ResultImage(BYTE* image)
{
	if(death)return;

	//移動量反映
	GaitMaker::UpdateMoveData();

	memcpy(state.self.image, image, 3*IMAGE_WIDTH*IMAGE_HEIGHT);

	//色抽出
	DetectColor();

	double tilt = Inverter::CalculateTilt();
	Inverter::CalculateHorizone(tilt,state.self.horizone.tilt,state.self.horizone.intercept,state.self.horizone.perpend);

	//Shrink(ORANGE);
	//Expand(ORANGE);

	if(state.game.robot_id == 1){
		DetectBall::CutOffHorizon();
		DetectBall::Execute();
	}else{
		DetectBall::CutOffHorizon();
		DetectBall::Execute();
	}

	//３回に２回Detect系を呼び出します
	static unsigned int cnt_image=0;
	cnt_image++;
	if(!(cnt_image%3==0))	DetectLandmark::Execute();
	if(!(cnt_image%3==1))	DetectLine::Execute();
	if(!(cnt_image%3==2))	DetectGoal::Execute();

	if(now_goal_motion){
		if(MotionMaker::IsFinished()){
			now_goal_motion = false;
		}
	}else if(now_finish_motion){
		if(MotionMaker::IsFinished()){
			now_finish_motion = false;
		}
	}else{
		Strategy::Execute();
	}
}

/*
void SoccerDog::SetJointValue(RCRegion* rgn){
	//StopWatch::SetZero(777);
    OCommandVectorData* cmdVecData = (OCommandVectorData*)rgn->Base();
    
    static jointangles lastangle(0,0,90,0,0,90,0,0,90,0,0,90,-10,0,10,0,0,0);

	//HeadController::SetHeadAngle(-60, 0, 45, -55, 10000);
	//ocommandMAX_FRAMESからNUM_FRAMESへ（16フレームから4フレーム）
	jointangles *angles = new jointangles[NUM_FRAMES];
    for (int i = 0; i < NUM_FRAMES; i++) {
		if(!MotionMaker::IsFinished()){
			//モーション
			if(!highgain){
				SetJointGainHigh();
				highgain = true;
			}

			MotionMaker::GetNextJointValue(angles[i]);
		}else{
			//歩行
			if(highgain){
				SetJointGain();
				highgain = false;
			}
			double j[NUM_JOINTS];
			GaitMaker::GetNextJointValue(j);
			for(int n = 0;n < 12;n++){
				angles[i].angle[n] = j[n];
			}

			HeadController::GetNextJointValue(angles[i].angle[TILT1], 
											  angles[i].angle[PAN], 
											  angles[i].angle[TILT2], 
											  angles[i].angle[CHIN] );

			TailController::GetNextJointValue(angles[i].angle[TAILTILT], 
										angles[i].angle[TAILPAN] );

			//angles[i].angle[16] = angles[i].angle[17] = 0;
		}
		lastangle = angles[i];
	}

	for(int idx = 0;idx < NUM_JOINTS;idx++){
	    OCommandInfo* info = cmdVecData->GetInfo(idx);
	    info->Set(odataJOINT_COMMAND2, jointID[idx], NUM_FRAMES);

	    OCommandData* data = cmdVecData->GetData(idx);
	    OJointCommandValue2* jval = (OJointCommandValue2*)data->value;

		for (int i = 0; i < NUM_FRAMES; i++) {
		    jval[i].value = oradians(angles[i].angle[idx]);
		}
    }
    delete[] angles;

	subject[sbjMoveJoint]->SetData(rgn);
    subject[sbjMoveJoint]->NotifyObservers();	
}
*/

void SoccerDog::InitState(void)
{
	OSYSPRINT(("InitState!\n"));

	//ボールの位置情報
	state.self.ballpos_in_image.x		= 0;
	state.self.ballpos_in_image.y		= 0;
	state.self.ballpos_in_image.value	= 0;
	state.ball.visible				= false;//ボールの可視・不可視判定 takeshita 05/04/06
	state.ball.losttime = 1000;
	state.ball.visibletime = 0;

	state.virtual_ball.x = 2000;
	state.virtual_ball.y = 0;
	state.virtual_ball.direction = 0;
	state.virtual_ball.distance = 2000;
	state.virtual_ball.visible = false;
	state.virtual_ball.losttime = 1000;

	state.ballpdf.Reset(state.ball);

	state.myGoalVisible = false;
	state.enemyGoalVisible = false;



	//加速度情報
	state.self.accelaration.ave_x = 0;
	state.self.accelaration.ave_y = 0;
	state.self.accelaration.ave_z = -9.8e6;

	state.self.accelaration.stdev_x = 0;
	state.self.accelaration.stdev_y = 0;
	state.self.accelaration.stdev_z = 0;

	state.self.accelaration.abs_value = 9.8e6;
	state.self.accelaration.theta = -90;
	state.self.accelaration.phi = 0;

	state.goalDirection = 0.0;//ゴールの方向
	state.goalVisible = false;
	state.goalDirectionOfOurs = 180.0;//ゴールの方向
	state.goalVisibleOfOurs = false;

	//転倒してるか
	state.self.isFallingDown = false;
	
	state.ball.SetPosition(2000,0);

	//GameState
	/*要更新2005/4/2坂本*/
	state.game.team_number = 0;//
	state.game.robot_id = 4;//jiji修正
	state.game.game_state = GAME_STATE_INITIAL;
	state.game.first_half = FIRST_HALF;
	state.game.team_color = TEAM_COLOR_BLUE;
	state.game.point_me = 0;
	state.game.point_enemy = 0;
	state.game.penalty = PENALTY_NONE;
	state.game.communication_data = new RoboCupGameControlData();

	//チームメート
	for(int i = 0;i < 5;i++)
	{
		state.team_mate[i].connected = false;
		state.team_mate[i].lastTime = 0;
	}

	for(int i = 0;i < 19;i++){
		state.field.free_direction[i] = SPACE_UNKNOWN;
	}

	LoadConfig();
	LoadCDT();

	//	InitColorTable();	
}


void SoccerDog::GetGoalMotion(bool point_is_mine){
	if(now_goal_motion)
		return;

	if(point_is_mine){
		MotionMaker::SetMotion(HG);
		MotionMaker::StartMotion();
		state.self.led.face_led_value = 0xFFF;
	}else{
		MotionMaker::SetMotion(GET_GOAL_ENEMY);
		MotionMaker::StartMotion();
		state.self.led.face_led_value = 0x33;
	}

	now_goal_motion = true;

}

void SoccerDog::FinishHalf(int def){
	if(now_finish_motion)
		return;

	if(def > 0){//win
		MotionMaker::SetMotion(HG_BACK);
		MotionMaker::StartMotion();
		state.self.led.face_led_value = 0xFFF;
	}else if(def < 0){
		MotionMaker::SetMotion(GET_GOAL_ENEMY);
		MotionMaker::StartMotion();
		state.self.led.face_led_value = 0x33;
	}else{
		MotionMaker::SetMotion(GET_GOAL_ENEMY);
		MotionMaker::StartMotion();
		state.self.led.face_led_value = 0x33;
	}
	now_finish_motion = true;;
}
