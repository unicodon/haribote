#ifndef MYROBOT_H
#define MYROBOT_H

#include <math.h>
#include "../config.h"
#include "../Utility/MatrixUtil.h"

struct Triplet{
	double x;
	double y;
	double value;
};

struct TripletInt{
	int x;
	int y;
	int value;
};

//�����h�}�[�N
struct LandmarkInformation{
	LANDMARK_ID id;
	int width;
	double angle;//degree
};

//�S�[���֌W
struct GoalInformation{
	double directionLeft;//degree
	double directionRight;//degree
	COLOR_NAME_TAG goal_color_id;
};

//DeadRec�֌W
struct DeadRecInformation{
	double dx;//ahead of robot
	double dy;//beside of robot
	double dt;//degree
};

struct SensorValue{
	double jointValue[NUM_JOINTS];
	int jointPWMDuty[NUM_JOINTS];//�d��
	double jointAngularVel[NUM_JOINTS];//�p���x
	double jointAngularAcc[NUM_JOINTS];//�p�����x
	
	bool pawTouch[4];//����
	bool chinTouch;//�����Z���T�[

	bool headTouch;//���^�b�`�Z���T�[
	bool backTouch[3];//�w���^�b�`�Z���T�[

	double headNearPSD;//�����Z���T�[
	double headFarPSD;

	double chestPSD;

	double accelSensor[3];//�����x�Z���T�[
};

struct LEDValue{
	//head
	LED_COLOR_TAG head_color;
	bool wireless;

	//back
	bool back_front_white;
	bool back_front_color;
	bool back_middle_white;
	bool back_middle_color;
	bool back_rear_white;
	bool back_rear_color;

	//face
	int face_led_value;
};

struct ValueData
{
	double val;
	double dev;
	double max_err;//�K�v?
};

struct PositionData
{
	ValueData x;
	ValueData y;
	ValueData tt;
	int hit_wall_per_cent;
	double cov_xy;
	double cov_yt;
	double cov_tx;
};

//�����x���
struct Accelaration{
	double ave_x;
	double ave_y;
	double ave_z;//���ϒl

	double stdev_x;
	double stdev_y;
	double stdev_z;//�W���΍�

	double abs_value;//�����x�̑傫��
	double theta;
	double phi;//�����i�ɍ��W�j
};

struct BallInfo{
	double distance;
	double direction;
	double x;
	double y;
	bool visible;
	int losttime;
	int visibletime;
	void SetPosition(double r,double tt){
		distance = r;
		direction = tt;
		x = r*cos(tt*3.14/180);
		y = r*sin(tt*3.14/180);
	};
};


//for sample_viewer
struct LocalizationData{
	int x;
	int y;
	int theta;
	int weight;
};


//�摜�̐������Ȃ�
struct ImageHorizone{
	double tilt;
	double intercept;
	bool perpend;
};

//�������g�̏�Ԃ��L�q
struct MyRobot{
	//�Z���T�[�l
	SensorValue sensor;
	
	//LED�w��
	LEDValue led;

	//�����x���
	Accelaration accelaration;

	//�摜
	unsigned char image[IMAGE_WIDTH*IMAGE_HEIGHT*3];

	//�F���o�摜
	unsigned char CDTimage[IMAGE_WIDTH*IMAGE_HEIGHT];

	//�]�|���Ă邩�ǂ���
	bool isFallingDown;

	//�摜�̌X��
	ImageHorizone horizone;


	//�F���o���
	TripletInt cdt[NUM_COLORS];
	int cdt_histogram_hl[NUM_COLORS][IMAGE_WIDTH];//�����q�X�g�O����
	int cdt_histogram_vt[NUM_COLORS][IMAGE_HEIGHT];//����



	TripletInt ballpos_in_image;

	PositionData position;
};

#endif
