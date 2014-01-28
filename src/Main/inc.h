#ifndef __INC_H_
#define __INC_H_

#define NUM_BODY 17
#define NUM_JOINT 16

enum BODY_TAG
{
	B_BODY = 0,

	B_HEAD,
	B_CHIN,
	B_NECK_ROOT,
	B_NECK,

	B_RF1,
	B_RF2,
	B_RF3,
	B_LF1,
	B_LF2,
	B_LF3,
	B_RH1,
	B_RH2,
	B_RH3,
	B_LH1,
	B_LH2,
	B_LH3,
};

enum JOINT_TAG
{
	J_RF1 = 0,
	J_RF2,
	J_RF3,
	J_LF1,
	J_LF2,
	J_LF3,
	J_RH1,
	J_RH2,
	J_RH3,
	J_LH1,
	J_LH2,
	J_LH3,
	J_TILT1,
	J_PAN,
	J_TILT2,
	J_CHIN
};


#endif
