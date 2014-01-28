#include <OPENR/OSyslog.h>
#include "Strategy.h"
#include "../State/State.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/GaitMaker.h"
#include "../Motion/HeadController.h"
#include "../Motion/TailController.h"
#include "Behavior.h"
#include "BehaviorList.h"

Behavior* Strategy::reservedBhv = NULL;
IsAlreadySet Strategy::isSet;


Strategy::Strategy(){
}

Strategy::~Strategy(){
}

void Strategy::SetReservedBehavior(Behavior* bhv){
	reservedBhv = bhv;
}


bool Strategy::Execute(){
	GaitMaker::SetPresetWalk(0,0);
	HeadController::SetHeadMode(HEAD_STOP);

	static int counter = 0;
	static int imgcounter = 0;

	if(counter == 10){
		char buf[64];
		sprintf(buf,"%04d",imgcounter++);
		SaveYUVPPMImage(state.self.image, buf);
	}
	if(counter > 0)counter--;
	if(counter == 0 && state.self.sensor.headTouch){
		counter = 10;
	}

	if(counter > 0){
		state.self.led.face_led_value = FACE_BATSU;
		TailController::SetTailMode(TAIL_SWING);
	}else{
		state.self.led.face_led_value = FACE_SMILE;
		TailController::SetTailMode(TAIL_STOP);
	}

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
