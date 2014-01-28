#ifndef MOTIONSTRUCT_H
#define MOTIONSTRUCT_H

#include "../Utility/MySTL.h"

struct MotionPose{
	double intervaltime;
	jointangles angles;

	MotionPose(){
		intervaltime = 100;
		angles = jointangles();
	};

	MotionPose(double t, jointangles ang){
		intervaltime = t;
		angles = ang;
	};
};

struct MotionParam{
	int num_pose; 
	std::vector<MotionPose> pose;
//	MyVector<MotionPose> pose;
};


#endif
