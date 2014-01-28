#include "StdAfx.h"
#include "BehaviorList.h"
#include "BhvLeavingField.h"
#include "../config.h"
#include "../Motion/PresetWalkParam.h"
#include <OPENR/OSyslog.h>

#define Value 100 //�������l


void BhvLeavingField::Execute(){

	
	double x = state.self.cdt[FIELDGREEN].x;
	double y = state.self.cdt[FIELDGREEN].y;
	int fieldgreen = state.self.cdt[FIELDGREEN].value;
	
	//�������l���������t�B�[���h�������Ă����牽�����Ȃ�
	if(fieldgreen > Value){
		return;
	}
	//�t�B�[���h�������Ȃ��Ȃ�����ȉ��̍s��
	else
	{
		//���ς���
		SetFaceLED(FACE_A_ALL);	
		SetPresetWalk(WALK_BACK);
		return;
	}
}
