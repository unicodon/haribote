#ifndef SOCCERDOG_H
#define SOCCERDOG_H

#include <list>
#include "config.h"
#include "Strategy/Strategy.h"

#include "RoboCupGameControlData.h"

#include "MCL/DeadRec.h"
#include "MCL/ObserveLandmark.h"
#include "MCL/ObserveGoal.h"
#include "MCL/ObserveLine.h"

class SoccerDog {
	friend class CRobot;
public:
	SoccerDog();
	virtual ~SoccerDog();

	void DoInit	  ();
	void DoStart  ();
	void DoStop	  ();
	void DoDestroy();

	void ResultImage(BYTE* image);

/*
	void ReadyJoint(const OReadyEvent& event);

	void ResultSensor(const ONotifyEvent& event);
*/
private:
	MCLSample* sample;//for sample_viewer
	bool now_goal_motion;
	bool now_finish_motion;
	static bool death;//自爆用
	int gamecontroller_beacon_losttime;
	bool gain_set;
	bool lastangle_init;//最初の関節角データをセンサーから読み込んだか
	longword shutterspeed;
	longword whitebalance;
	longword cameragain;

	static const size_t NUM_COMMAND_VECTOR = 2;

	bool highgain;
	void LoadConfig();//チームナンバーとかチームカラーとか読み込み

	void SetJointGain(void);
	void SetJointGainHigh(void);

	void InitColorTable(void);
	void InitState(void);
	void SetSensorData(longword frameIndex);

	char robot_ip[4][128];
//	int  robot_port[4];
	int UDPSendPort[4];
	int UDPRecvPort;
	int GameControllerPort;

	bool ReadIPandPortConfig();
    
	//得点＆失点後のモーション用
	void GetGoalMotion(bool point_is_mine);

	//終了モーション
	void FinishHalf(int point_def);
};

#endif
