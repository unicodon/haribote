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

//ランドマーク
struct LandmarkInformation{
	LANDMARK_ID id;
	int width;
	double angle;//degree
};

//ゴール関係
struct GoalInformation{
	double directionLeft;//degree
	double directionRight;//degree
	COLOR_NAME_TAG goal_color_id;
};

//DeadRec関係
struct DeadRecInformation{
	double dx;//ahead of robot
	double dy;//beside of robot
	double dt;//degree
};

struct SensorValue{
	double jointValue[NUM_JOINTS];
	int jointPWMDuty[NUM_JOINTS];//電圧
	double jointAngularVel[NUM_JOINTS];//角速度
	double jointAngularAcc[NUM_JOINTS];//角加速度
	
	bool pawTouch[4];//足先
	bool chinTouch;//あごセンサー

	bool headTouch;//頭タッチセンサー
	bool backTouch[3];//背中タッチセンサー

	double headNearPSD;//距離センサー
	double headFarPSD;

	double chestPSD;

	double accelSensor[3];//加速度センサー
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
	double max_err;//必要?
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

//加速度情報
struct Accelaration{
	double ave_x;
	double ave_y;
	double ave_z;//平均値

	double stdev_x;
	double stdev_y;
	double stdev_z;//標準偏差

	double abs_value;//加速度の大きさ
	double theta;
	double phi;//方向（極座標）
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


//画像の水平線など
struct ImageHorizone{
	double tilt;
	double intercept;
	bool perpend;
};

//自分自身の状態を記述
struct MyRobot{
	//センサー値
	SensorValue sensor;
	
	//LED指令
	LEDValue led;

	//加速度情報
	Accelaration accelaration;

	//画像
	unsigned char image[IMAGE_WIDTH*IMAGE_HEIGHT*3];

	//色抽出画像
	unsigned char CDTimage[IMAGE_WIDTH*IMAGE_HEIGHT];

	//転倒してるかどうか
	bool isFallingDown;

	//画像の傾き
	ImageHorizone horizone;


	//色抽出情報
	TripletInt cdt[NUM_COLORS];
	int cdt_histogram_hl[NUM_COLORS][IMAGE_WIDTH];//水平ヒストグラム
	int cdt_histogram_vt[NUM_COLORS][IMAGE_HEIGHT];//垂直



	TripletInt ballpos_in_image;

	PositionData position;
};

#endif
