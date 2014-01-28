//�{�[���ڋ߂�BvhLastApproch����BhvDrible�ɖ߂��Ƃ���
//BhvDPBallApproach.cpp��BvhLastApproch::Execute();����BhvDrible:::Execute();�ւ̕ύX��2�J��
//Strategy.cpp��state.last_approach = bhvLastApproach.last_app����
//state.last_approach = bhvDrible.last_app;�ւ̕ύX��4�J����Y�ꂸ�ɕύX���Ă������� by kase
//
#ifndef BHVLASTAPPROACH_H
#define BHVLASTAPPROACH_H

#include "Behavior.h"

struct GaolInfoLastApp{
	bool visible;
	int drct;
};

class BhvLastApproach : public Behavior{
public:
	GaolInfoLastApp goalinfolastapp;
	BhvLastApproach();
	void Execute();
	bool last_app;
	void ReserveLastApp();	//BhvLastApproach::LastApproach���Ăяo���ݒ�
	void ReleseLastApp();	//BhvLastApproach::LastApproach�𔲂��鎞�̏�����
	void ReleseExe();		//BhvLastApproach::Execute�𔲂��鎞�̏�����
private:	
	void LastApproach();
	int shot_motion;
	bool hold_flag;
	double adjust_ball_dis;
	//BallInfo lastball;
	//double shot_x;
	//double shot_y;
};

#endif
