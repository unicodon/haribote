#ifndef BHVMOVETOPOSITION_H
#define BHVMOVETOPOSITION_H

#include "Behavior.h"
#include "../Motion/PresetWalkParam.h"

class BhvMoveToPosition : public Behavior{
private:
	double dest_x;
	double dest_y;
	double dest_tt;
	//static PresetWalkParam presetWalkParam;
public:
	BhvMoveToPosition();

	void SetDestination(double x, double y, double tt);
	void Execute();
};

#endif
