#ifndef OBSERVE_GOAL_H_
#define OBSERVE_GOAL_H_

#include "GoalData.h"
#include "MCLSample.h"



class ObserveGoal
{
public:
	ObserveGoal();
	~ObserveGoal(){}
	static void SetSample(MCLSample* input);
	static void Observation(GoalInformation _input);
private:
	static MCLSample* sample;
	static bool sample_set;
	static void DirectionOfGoal(COLOR_NAME_TAG color,double direction0,double direction1);

};


#endif //OBSERVE_GOAL_H_
