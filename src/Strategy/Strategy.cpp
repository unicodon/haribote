#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "../SoccerDog.h"
#include "Strategy.h"
#include "../State/State.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/GaitMaker.h"
#include "../Motion/HeadController.h"
#include "../Motion/TailController.h"
#include "Behavior.h"
#include "BehaviorList.h"
#include "../Utility/StopWatch.h"

Behavior* Strategy::reservedBhv = NULL;
IsAlreadySet Strategy::isSet;


Strategy::Strategy(){
}

Strategy::~Strategy(){
}

void Strategy::SetReservedBehavior(Behavior* bhv)
{
	/*
	if(!bhv)
		OSYSPRINT(("Release\n"));
	else
		OSYSPRINT(("Res\n"));
	*/
	reservedBhv = bhv;
}


bool Strategy::Execute(){
/*	static int cn = 0;
	static int walk = 0;
	if(state.self.sensor.headTouch && GaitMaker::IsStop()){
		walk = (cn/3) % NUM_PRESET_WALKPARAM;
		GaitMaker::SetPresetWalk(walk,10);
		cn++;
	}
	return false;
*/

	//タッチセンサでの状態遷移
	TouchSensorOperation();

	isSet.isMotionSet = false;
	isSet.isWalkSet = false;
	isSet.isHeadSet = false;
	isSet.isTailSet = false;
	isSet.isFaceSet = false;

	//一番低レベルの反応

	//転倒から起きる
	if(state.self.isFallingDown && MotionMaker::IsFinished()){
		double phi = state.self.accelaration.phi;
		if(phi < -135){
			if(MotionMaker::SetMotion(GETUP_BACK))
				MotionMaker::StartMotion();
		}else if(phi < -45){
			if(MotionMaker::SetMotion(GETUP_RIGHT))
				MotionMaker::StartMotion();
		}else if(phi < 45){
			if(MotionMaker::SetMotion(GETUP_FRONT))
				MotionMaker::StartMotion();
		}else if(phi < 135){
			if(MotionMaker::SetMotion(GETUP_LEFT))
				MotionMaker::StartMotion();
		}else{
			if(MotionMaker::SetMotion(GETUP_BACK))
				MotionMaker::StartMotion();
		}

		state.self.led.face_led_value = FACE_GREENCONFUSE;
	}

	static int init_counter = 0;
	//ゲーム状態に応じて切り替え
	if(state.game.game_state != GAME_STATE_READY) bhvReady.Reset();
	if(state.game.game_state != GAME_STATE_PLAYING){
		switch(state.game.game_state){
			case GAME_STATE_INITIAL:
				GaitMaker::SetPresetWalk(0,0);
				HeadController::SetHeadMode(HEAD_STOP);
				init_counter++;
				if(init_counter < 30){
					state.self.led.face_led_value = FACE_FIVE;
				}else if(init_counter < 60){
					state.self.led.face_led_value = FACE_FOUR;
				}else if(init_counter < 90){
					state.self.led.face_led_value = FACE_THREE;
				}else if(init_counter < 120){
					state.self.led.face_led_value = FACE_TWO;
				}else if(init_counter < 150){
					state.self.led.face_led_value = FACE_ONE;
				}else if(init_counter < 180){
					init_counter = 0;
					state.self.led.face_led_value = FACE_FIVE;
				}

				break;
			case GAME_STATE_READY:
				//bhvGoalie.Reset();
				bhvGoalie2006.Reset();
				//bhvGoalie2007.Reset();
				bhvReady.Execute();
				break;
			case GAME_STATE_SET:
			
				//bhvGoalie.Reset();
				bhvGoalie2006.Reset();
				//bhvGoalie2007.Reset();
				bhvSet.Execute();
				
				break;
			case GAME_STATE_FINISHED:
				//SoccerDog::Suicide();

				GaitMaker::SetPresetWalk(0,0);
				HeadController::SetHeadMode(HEAD_STOP);
				state.self.led.face_led_value = FACE_FINISHED;
				break;
		}
		IndicateGameStateLED();

		//state.last_approach = bhvDrible.last_app;
		state.last_approach = bhvLastApproach.last_app;
		return true;

	}


	//ペナルティ状態
	if(state.game.penalty != PENALTY_NONE){
		//bhvGoalie.ResetForPenalize();
		bhvGoalie2006.ResetForPenalize();
		//bhvGoalie2007.ResetForPenalize();
		HeadController::SetHeadAngle(-5,0,45,-55,1000);
		TailController::SetTailMode(TAIL_STOP);
		GaitMaker::SetPresetWalk(0,0);
		state.self.led.face_led_value = FACE_USHIKUN;
		MotionMaker::FinishMotion();

		bhvUnpenalize.Initialize();

		IndicateGameStateLED();
		//state.last_approach = bhvDrible.last_app;
		state.last_approach = bhvLastApproach.last_app;
		return false;
	}

	//以下通常時
	//OSYSPRINT(("STRATEGY \n"));
	if(reservedBhv != NULL){
		reservedBhv->Execute();
		IndicateGameStateLED();
		//state.last_approach = bhvDrible.last_app;
		state.last_approach = bhvLastApproach.last_app;
		return false;
	}

	//以下Behavior呼び出し

	//if(state.ball.visible) bhvSearchBall.Initialize();
	
	static int ball_loss = 0;//ball.visibleが連続でfalseとなっているカウント数
	
	//モーション中はそれをやるれ
	if(!MotionMaker::IsFinished()){
		state.ball.losttime = 15;
		IndicateGameStateLED();
		//state.last_approach = bhvDrible.last_app;
		state.last_approach = bhvLastApproach.last_app;

		return false;
	}
	//if(1) bhvSample.Execute();
	else if(state.game.robot_id == 1){
		//bhvGoalie.Execute();
		bhvGoalie2006.Execute();
		//bhvGoalie2007.Execute();
	}else{
//		bhvGoBackCourt.Execute();	//自己位置推定がコート外に出ていないか，もしくは，AIBOがコートから出そうにないか
		bhvSupport.Execute();		//味方AIBOがボールそばにいる場
		//bhvSearchBall.Execute();	//ボールを見失った場合
//		bhvBallApproach.Execute();	//ボールに十分接近したとき
		bhvDPBallApproach.Execute();//ボールに接近するとき
		bhvUnpenalize.Execute();
	}

/*	if(!state.ball.visible){
		ball_loss++;
	}
	if(ball_loss > 50){
		ball_loss = 50;
		bhvBallApproach.Test();
	}
	else{
		ball_loss = 0;
		static bool flag_app = false;
		if(state.ball.distance < 400 || flag_app){
			flag_app = true;
			bhvBallApproach.Execute();
			if(state.ball.distance > 500) flag_app = false;
		}
		else{
			bhvDPBallApproach.Execute(); //takeshita 05/0421
		}
	}
*/
	//bhvLongShoot.Execute(); //takeshita 05/0421

	//以下デフォルト動作をば
	if(!isSet.isFaceSet)
		state.self.led.face_led_value = FACE_LOVELY;

	IndicateGameStateLED();

	//state.last_approach = bhvDrible.last_app;
	state.last_approach = bhvLastApproach.last_app;

	return false;
}

void Strategy::IndicateGameStateLED(){
	//ゲーム状態をLEDに反映
	if(state.game.game_state == GAME_STATE_INITIAL || state.game.game_state == GAME_STATE_READY){
		state.self.led.back_middle_color = ( state.game.kikkoff_team == state.game.team_color );
	}else{
		state.self.led.back_middle_color = 0;
	}

	if(state.game.team_color == TEAM_COLOR_RED){
		state.self.led.back_front_color = false;
		state.self.led.back_rear_color  = true;
		state.self.led.head_color = LED_RED;
	}else{
		state.self.led.back_front_color = true;
		state.self.led.back_rear_color  = false;
		state.self.led.head_color = LED_BLUE;
	}
	if(state.goalVisible)
		state.self.led.head_color = LED_GREEN;

}



void Strategy::TouchSensorOperation(){
	static int head_touch_counter = 0;
	static int back_touch_counter1 = 0;
	static int back_touch_counter2 = 0;
	static int back_touch_counter3 = 0;

	if(state.self.sensor.headTouch){
		if(head_touch_counter == 0)
			OnHeadTouch();
		
		head_touch_counter++;
		if(head_touch_counter >= 100)
			OnHeadTouch3sec();
	}else{
		if(head_touch_counter > 0 && head_touch_counter < 100)
			OnHeadRelease();
		head_touch_counter = 0;
	}

	if(state.self.sensor.backTouch[0]){
		if(back_touch_counter1 == 0)
			OnBackTouch(0);
		back_touch_counter1++;
		if(back_touch_counter1 > 30)
			OnBackTouch1sec();
	}else{
		back_touch_counter1= 0;
	}

	if(state.self.sensor.backTouch[1]){
		if(back_touch_counter2 == 0)
			OnBackTouch(1);
		back_touch_counter2++;
		if(back_touch_counter2 > 30)
			OnBackTouch1sec();
	}else{
		back_touch_counter2= 0;
	}

	if(state.self.sensor.backTouch[2]){
		if(back_touch_counter3 == 0)
			OnBackTouch(2);
		back_touch_counter3++;
		if(back_touch_counter3 > 30)
			OnBackTouch1sec();
	}else{
		back_touch_counter3 = 0;
	}

}

void Strategy::OnHeadTouch(){
	GAME_STATE gs = state.game.game_state;

	switch(gs){
		case GAME_STATE_INITIAL:
			state.game.game_state = GAME_STATE_READY;
			break;
		case GAME_STATE_READY:
			state.game.game_state = GAME_STATE_SET;
			break;
		case GAME_STATE_SET:
			state.game.game_state = GAME_STATE_PLAYING;
			break;
	}
}

void Strategy::OnHeadTouch3sec(){
	if(state.game.penalty != PENALTY_NONE){
		state.game.game_state = GAME_STATE_INITIAL;
		state.game.penalty = PENALTY_NONE;
	}
}

void Strategy::OnHeadRelease(){
	if(state.game.game_state == GAME_STATE_PLAYING && state.game.penalty != PENALTY_NONE){
		state.game.penalty = PENALTY_NONE;
	}
}

void Strategy::OnBackTouch(int i){
	if(state.game.game_state == GAME_STATE_INITIAL){
		switch(i){
			case 0://front
				state.game.team_color = TEAM_COLOR_BLUE;
				break;
			case 1:
				state.game.kikkoff_team = state.game.kikkoff_team == TEAM_COLOR_BLUE ? TEAM_COLOR_RED : TEAM_COLOR_BLUE;
				break;
			case 2:
				state.game.team_color = TEAM_COLOR_RED;
				break;
		}
	}
}

void Strategy::OnBackTouch1sec(){
	if(state.game.game_state == GAME_STATE_PLAYING && 
			state.game.penalty == PENALTY_NONE){
		state.game.penalty = PENALTY_LEAVING;
	}
}
