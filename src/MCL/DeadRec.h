#ifndef DEAD_REC_H_DEFINED_
#define DEAD_REC_H_DEFINED_

#include "../config.h"
#include "../State/State.h"
#include "MCLSample.h"


class DeadRec
{
public:
	DeadRec();

	~DeadRec(){}
	static void SetSample(MCLSample* sample);
	static void UpdatePosition(const DeadRecInformation info);
private:
	static bool sample_set;
	static void UpdateSample();
	static MCLSample* sample;
	static bool GetLocomotion(double &x,double &y,double &t);
//	static int search_pos(int pos_min, int pos_max, int value,int *prob_distribution);
};

#endif  //DEAD_REC_H_DEFINED_
