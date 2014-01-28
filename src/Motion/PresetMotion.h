#ifndef PRESETMOTION_H
#define PRESETMOTION_H

#include <vector>
#include <string>
#include <map>

#include "../joint.h"
#include "MotionStruct.h"
#include "../Utility/MySTL.h"


enum PRESETMOTIOININDEX{
	GETUP,
	GETUP_FRONT,
	GETUP_RIGHT,
	GETUP_LEFT,
	GETUP_BACK,
	HG,
	HG_BACK,
	GUTSPOSE,
	GET_GOAL_ENEMY,
	FINISH_WIN,
	FINISH_DRAW,
	FINISH_LOSE,
	LONGSHOOT_FRONT_100_150,
	GRABSHOOT_FRONT,//HASE
	GRABSHOOT_LEFT,//HASE
	GRABSHOOT_RIGHT,//HASE
	GRABSHOOT_FAKE_LEFT,//HASE
	GRABSHOOT_FAKE_RIGHT,//HASE
	GRABSHOOT_FAKE_LEFT20,//HASE
	GRABSHOOT_FAKE_RIGHT20,//HASE
	GRABSHOOT_FAKE_LEFT30,//HASE
	GRABSHOOT_FAKE_RIGHT30,//HASE
	GRABSHOOT_FAKE_LEFT40,//HASE
	GRABSHOOT_FAKE_RIGHT40,//HASE
	GRABSHOOT_FAKE_LEFT60,//HASE
	GRABSHOOT_FAKE_RIGHT60,//HASE
	SHOOT_FRONT,//KASE
	HEADING_LEFT,//KASE
	HEADING_RIGHT,//KASE
	HEADING_2LEFT,//2004
	HEADING_2RIGHT,//2004
	SMASHHIT_LEFT,//2004
	SMASHHIT_RIGHT,//2004
	GOALIE_BLOCK,//キーパー用で，手を広げてるだけー
	GOALIE_BLOCK_RIGHTWALK,//キーパー用で，右に歩いて手をひろげる
	GOALIE_BLOCK_LEFTWALK,//キーパー用で，左に歩いて手をひろげる
	DIVING_LEFT45,//KASE
	DIVING_RIGHT45,//KASE
	SHOT_8MM,
	HOLD_LEFT90,
	HOLD_RIGHT90,
	GRABLOOK,
	AYAYA_RIGHT,
	AYAYA_LEFT,
	/*	
	DIVING_LEFT45_OSAKA,
	DIVING_RIGHT45_OSAKA,
	BACK_SHOT, //hase
	*/
	MUNESHOT_00,
	HOOKE_LEFT_45,
	HOOKE_RIGHT_45,
	HG_FRONT,//HASE0526
	EJYA9_FRONT,
	EJYA9_LEFT,
	EJYA9_RIGHT,
	NUM_PRESET_MOTIONS
};

class PresetMotion{
public:
	static std::vector<MotionParam> motions;
//	static MyVector<MotionParam> motions;
	PresetMotion();
	static int ResolveMotionName(const char* motionname);

	static void Init();

private:
	static void RegisterMotionName(const char* motionname);
	static std::map<std::string,int> motionNameMap;
	static bool isInitialized;
};

#endif
