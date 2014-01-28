#include "StdAfx.h"
#include "BehaviorList.h"
#include "BhvLeavingField.h"
#include "../config.h"
#include "../Motion/PresetWalkParam.h"
#include <OPENR/OSyslog.h>

#define Value 100 //しきい値


void BhvLeavingField::Execute(){

	
	double x = state.self.cdt[FIELDGREEN].x;
	double y = state.self.cdt[FIELDGREEN].y;
	int fieldgreen = state.self.cdt[FIELDGREEN].value;
	
	//しきい値よりも多くフィールドが見えていたら何もしない
	if(fieldgreen > Value){
		return;
	}
	//フィールドが見えなくなったら以下の行動
	else
	{
		//顔を変える
		SetFaceLED(FACE_A_ALL);	
		SetPresetWalk(WALK_BACK);
		return;
	}
}
