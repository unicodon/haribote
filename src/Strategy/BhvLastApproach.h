//ボール接近をBvhLastApprochからBhvDribleに戻すときは
//BhvDPBallApproach.cppでBvhLastApproch::Execute();からBhvDrible:::Execute();への変更を2カ所
//Strategy.cppでstate.last_approach = bhvLastApproach.last_appから
//state.last_approach = bhvDrible.last_app;への変更を4カ所を忘れずに変更してください by kase
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
	void ReserveLastApp();	//BhvLastApproach::LastApproachを呼び出す設定
	void ReleseLastApp();	//BhvLastApproach::LastApproachを抜ける時の初期化
	void ReleseExe();		//BhvLastApproach::Executeを抜ける時の初期化
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
