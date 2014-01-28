#ifndef BHV_WALKING_HEADING_H_
#define BHV_WALKING_HEADING_H_

#include "Behavior.h"

#define NUM_POSITION 4

class BhvWalkingHeading : public Behavior
{
public:
	void Execute();
	void Initialize();
	bool IsFinished();
private:
	int count;
	bool finish;
	bool first_time;
	static const int headingPosition[NUM_POSITION][4];
};

#endif
