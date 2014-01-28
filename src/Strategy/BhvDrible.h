#ifndef BHVDRIBLE_H
#define BHVDRIBLE_H

#include "Behavior.h"

class BhvDrible : public Behavior{
public:
	BhvDrible();
	void Execute();
	void Execute_Simple();
	bool last_app;

private:
	BallInfo lastball;
	void LastApproach();

	int shot_motion;
	bool hold_flag;
	//bool last_app;
	double shot_x;
	double shot_y;
};

#endif
