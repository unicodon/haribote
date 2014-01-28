#ifndef OBSERVE_LINE_H_
#define OBSERVE_LINE_H_

#include "LineData.h"
#include "MCLSample.h"

struct SlatePoint
{
	LineDirection direction;
	int constant;
	int val_constant;
	int theta;
	int val_theta;
};


#define NUM_OF_LINE 11



class ObserveLine
{
public:
	ObserveLine();
	~ObserveLine(){}
	static void SetSample(MCLSample* _input);
	static void Observation(LineInformation _input);
private:
	static MCLSample* sample;
	static bool sample_set;
	static SlatePoint slatePoint[];
	static LineStruct lines[];
};

#endif //OBSERVE_LINE_H_
