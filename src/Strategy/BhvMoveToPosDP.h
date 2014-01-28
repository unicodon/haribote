#ifndef BHVMOVETOPOSDP_H
#define BHVMOVETOPOSDP_H

#include <iostream>
#include <vector>
#include <fstream>
using namespace std;

#include "Behavior.h"
#include "../Motion/PresetWalkParam.h"

typedef int valuetype;
typedef unsigned char actiontype;

class DPMap{
public:
	DPMap();

	static unsigned char get_action(int x, int y, int theta);
	static unsigned char GetActionToGoToXYTheta(int x, int y, int theta);

private:
	static int x_to_id(int x);
	static int y_to_id(int y);
	static int theta_to_id(int theta);

	static bool loadflag;
	static unsigned char *m_map;
};

class DPMapLocalApproach{
public:
	DPMapLocalApproach();

	unsigned char get_action(int x, int y, int theta);
	int get_cost(int x, int y, int theta);

private:
	int State2ID(int dist, int btt, int rtt);

	static bool loadflag;
	static valuetype *value_map;
	static actiontype *policy_map;
};

class DPMapGoalieApproach{
public:
	DPMapGoalieApproach();

	unsigned char get_action(int x, int y, int theta);
	int get_cost(int x, int y, int theta);

private:
	int State2ID(int dist, int btt, int rtt);

	static bool loadflag;
	static valuetype *value_map;
	static actiontype *policy_map;
};


class BhvMoveToPosDP : public Behavior{
private:
	double dest_x;
	double dest_y;
	double dest_tt;
	double x_width;
	double y_width;
	double tt_width;
	//static PresetWalkParam presetWalkParam;
	void GetPositionDelta(int &x, int &y, int &theta);
public:
	BhvMoveToPosDP();

	void Execute();
	void SetDestination(double x, double y, double tt, double x_w = 0, double y_w = 0, double tt_w = 0);
};

#endif
