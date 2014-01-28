#ifndef BHVREADY_H
#define BHVREADY_H

#include "Behavior.h"

class BhvReady : public Behavior{
private:
public:
	bool go_goal_mouth;
	int cnt_goal_mouth;
	BhvReady();
	void Execute();
	void Reset();
};

#endif
