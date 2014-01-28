#ifndef OBSERVE_LANDMARK_H_
#define OBSERVE_LANDMARK_H_
#include "../Utility/MatrixUtil2.h"
#include "../config.h"
#include "LandmarkData.h"
#include "MCLSample.h"



/******Move to config.h
********************************/


/*******************
end of moved section *******************/

class ObserveLandmark
{
public:
	ObserveLandmark();
	~ObserveLandmark(){}
	static void SetSample(MCLSample* _input);
	static void Observation(LandmarkInformation _input);
private:
	static bool sample_set;
	static void GetLandmarkXY(int *x,int *y,LandmarkInformation _input);
//	static int SearchPosition(int pos_min, int pos_max, int value,const int *prob_distribution);
	static MCLSample* sample;
	static void SensorResetting(int reset_num, const int *likelyhood,int angle_range,int min,LandmarkInformation _input,double sum_weight);
};

#endif //OBSERVE_LANDMARK_H_
