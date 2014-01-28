#ifndef BEHAVIOR_H
#define BEHAVIOR_H

#include "Strategy.h"
#include "../config.h"
#include "../Motion/PresetWalkParam.h"
#include "../Vision/DetectBall.h"
#include "../State/State.h"

class Behavior{
public:
	virtual ~Behavior(){};

	virtual void Execute() = 0;//純粋仮想関数

	void ReserveCall();
	void ReleaseCall();

	//各Controllerへのインターフェイス
	//Motion
	void SetMotion(int n, bool overwrite = false);
	void SetMotion(const char* motionname, bool overwrite = false);

	//Walk
	void SetPresetWalk(int walkIndex, int step = 1,bool overwrite = false);

	//Head
	void SetHeadAngle(double tilt1, double pan, double tilt2, double chin, int speed, bool overwrite = false);
	void SetHeadMode(HEAD_MODE h_mode, bool overwrite = false);

	//Tail
	void SetTailMode(TAIL_MODE t_mode, bool overwrite = false);

	//Face LED
	void SetFaceLED(int face, bool overwrite = false);

	//sound
	void PlaySound(SOUND_MODE mode, bool overwrite = false);
};

#endif
