#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"

static unsigned int cnt_penalize;
static bool now_release;
static bool now_recover;
static bool now_penalize;

BhvUnpenalize::BhvUnpenalize() 
{
	cnt_penalize = 1000;
	now_penalize = false;
}

//ペナルティ状態のときに呼び出される
void BhvUnpenalize::Initialize() 
{
	cnt_penalize = 0;
	now_penalize = true;
}

void BhvUnpenalize::Execute(){

	//ボールが見えてる or ある程度探したら抜け出る
	if(state.ball.visible || cnt_penalize>60){
		now_penalize = false;
		cnt_penalize = 1000;
		return;
	}

	SetFaceLED(FACE_ONE, true);
	cnt_penalize++;

	//とりあえずその場で首振り
	SetPresetWalk(WALK_FORWARD, 10, true);
	SetHeadMode(HEAD_SWING_LOCALIZATION, true);

	return;
}
