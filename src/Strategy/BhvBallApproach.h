#ifndef BHVBALLAPPROACH_H
#define BHVBALLAPPROACH_H

#include "Behavior.h"

class BhvBallApproach : public Behavior{
public:
	void Execute();
	void Test();
	void BallRoll();
	bool HoldShot();
};

#endif
