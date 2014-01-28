#ifndef GAITMAKER_H
#define GAITMAKER_H

#include "../config.h"
#include "../joint.h"
#include "../State/state.h"
#include "Inverter.h"
#include "PresetWalkParam.h"

#include <vector>

#define NUM_GAIT_POINT 8

struct LegDegParam
{
	double rfj1[NUM_GAIT_POINT];
	double rfj2[NUM_GAIT_POINT];
	double rfj3[NUM_GAIT_POINT];

	double lfj1[NUM_GAIT_POINT];
	double lfj2[NUM_GAIT_POINT];
	double lfj3[NUM_GAIT_POINT];

	double rhj1[NUM_GAIT_POINT];
	double rhj2[NUM_GAIT_POINT];
	double rhj3[NUM_GAIT_POINT];
	
	double lhj1[NUM_GAIT_POINT];
	double lhj2[NUM_GAIT_POINT];
	double lhj3[NUM_GAIT_POINT];
};

struct Movement{
	double x;
	double y;
	double tt;
};

class GaitMaker
{
public:
	GaitMaker();
	virtual ~GaitMaker();
	static void GaitMaker::GetNextJointValue(double *j);
	static bool GaitMaker::MakeLocus(double *j,double phase_);
	static bool GaitMaker::MakeLocus2(double *j,int phase_);

//	static LegDegParam SetLegParam[NUM_PRESET_WALKPARAM];
	static void MakeLegParamForPreset(int index, LegDegParam &pLegParam, WalkParam myparam);
	static bool GaitMaker::MakeLocus3(unsigned char walk_index, double *j,int phase_);
	static void GaitMaker::Initialized();

//	static void SetStepLimit(int n);
	static void ResetPhase();
	static int SetPresetWalk(int walkIndex, int step);
	static int SetWalkParam(WalkParam param, int step);
	static int SetParaWalkParam(WalkParam param, int step);//ParaWalk最後の2歩セット用
	static int SetWalkParamForPreset(WalkParam param);

	static void UpdateMoveData();

	static bool IsStop();
	static bool IsNextWalkSet();
	static bool IsExpiring();

private:
	static Movement d_movedata;
	static bool isStop;
	static bool next_isStop;
	static bool next_flag;

	static void UpdatePosition(double x, double y, double tt);//歩いた後のデッドレコニング処理
	static double angleData[NUM_GAIT_POINT][12];
	static double nextAngleData[NUM_GAIT_POINT][12];
	static Movement next_movedata;
	static Movement movedata;
	static double cycletime;
	static double nextcycletime;
	static int nextStepLimit;
	static PresetWalkParam presetWalkParam;
	static int stepLimit;
	static double phase;
	static double gratio;//接地してる時間の割合
	static double aratio;//足を前に持っていく時間の割合
	static Inverter inverter;
	static void getUnitLocus(double t, double& x, double& y);
	static void getUnitRectLocus(double t, double& y, double& x);
	static WalkParam myparam;
};

#endif
