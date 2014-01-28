#pragma once

#include <vector>
#include "inc.h"

struct Pose
{
	dReal timeInterval;
	dReal jointAngles[NUM_JOINT];
};



class MotionEngine
{
private:
	std::vector<Pose> poses;
	int current;
	float time;
	bool stop;

public:
	MotionEngine(void);
	virtual ~MotionEngine(void);

	void Clear();
	void ReadLine(CString str);
	void AddTail();

	void Start();

	void GetNextAngle(float dtime, dReal* angles);
};
