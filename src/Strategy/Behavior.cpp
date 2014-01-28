#include "StdAfx.h"
#include "Behavior.h"

#include "Strategy.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/GaitMaker.h"
#include "../Motion/HeadController.h"
#include "../Motion/TailController.h"
#include "../Motion/PresetWalkParam.h"


//次は直接私のBehaviorを呼んでねーってやつ
void Behavior::ReserveCall(){
	Strategy::SetReservedBehavior(this);
}

//↑のを解除
void Behavior::ReleaseCall(){
	Strategy::SetReservedBehavior(0);
}


//以下Motion系モジュールのラッパ
void Behavior::SetMotion(int n, bool overwrite){
	if(overwrite || !Strategy::isSet.isMotionSet){
		if(MotionMaker::SetMotion(n)){
			Strategy::isSet.isMotionSet = true;
			MotionMaker::StartMotion();
			//SoundController::SetSound(SOUND_SHOT);
			//SoundController::Play();
		}
	}
}

void Behavior::SetMotion(const char* motionname, bool overwrite){
	if(overwrite || !Strategy::isSet.isMotionSet){
		if(MotionMaker::SetMotion(motionname)){
			Strategy::isSet.isMotionSet = true;
			MotionMaker::StartMotion();
			//SoundController::SetSound(SOUND_SHOT);
			//SoundController::Play();
		}
	}
}

void Behavior::SetPresetWalk(int walkIndex, int step, bool overwrite){
	if(overwrite || !Strategy::isSet.isWalkSet){
		if(walkIndex == WALK_STOP)
			step = 0;
		if(GaitMaker::SetPresetWalk(walkIndex,step)){
			Strategy::isSet.isWalkSet = true;
		}
	}
}
void Behavior::SetHeadAngle(double tilt1, double pan, double tilt2, double chin, int speed, bool overwrite){
	if(overwrite || !Strategy::isSet.isHeadSet){
		HeadController::SetHeadAngle(tilt1,pan,tilt2,chin,speed);
		Strategy::isSet.isHeadSet = true;
	}
}

void Behavior::SetHeadMode(HEAD_MODE h_mode, bool overwrite){
	if(overwrite || !Strategy::isSet.isHeadSet){
		HeadController::SetHeadMode(h_mode);
//		if(h_mode == HEAD_TRACK || h_mode == HEAD_TRACK_NEAR)
//		{
//			SetFaceLED(FACE_ARROW,true);
//		}
		Strategy::isSet.isHeadSet = true;
	}
}

void Behavior::SetTailMode(TAIL_MODE t_mode, bool overwrite){
	if(overwrite || !Strategy::isSet.isTailSet){
		TailController::SetTailMode(t_mode);
		Strategy::isSet.isTailSet = true;
	}
}

void Behavior::SetFaceLED(int face, bool overwrite){
	if(overwrite || !Strategy::isSet.isFaceSet){
		state.self.led.face_led_value = face;
		Strategy::isSet.isFaceSet = true;
	}
}

void Behavior::PlaySound(SOUND_MODE mode, bool overwrite){
}


