#ifndef JOINT_H
#define JOINT_H

#include "config.h"

extern const char* const JOINT_LOCATOR[];

extern const char* const SENSOR_LOCATOR[];

//関節角データ構造体　単位は[deg]推奨．
struct jointangles{
	//関節角データの配列
	double angle[NUM_JOINTS];

	//デフォルトコンストラクタ
	jointangles(){
		angle[0] = 0;
		angle[1] = 0;
		angle[2] = 90;

		angle[3] = 0;
		angle[4] = 0;
		angle[5] = 90;

		angle[6] = 0;
		angle[7] = 0;
		angle[8] = 90;

		angle[9] = 0;
		angle[10] = 0;
		angle[11] = 90;

		angle[12] = 0;
		angle[13] = 0;
		angle[14] = 0;
		angle[15] = 0;

		angle[16] = 0;
		angle[17] = 0;
	};

	//値を与えて初期化用
	jointangles(double j0,double j1,double j2,double j3,double j4,double j5,
				double j6,double j7,double j8,double j9,double j10,double j11,
				double j12,double j13,double j14,double j15,double j16,double j17){
		angle[0] = j0;
		angle[1] = j1;
		angle[2] = j2;

		angle[3] = j3;
		angle[4] = j4;
		angle[5] = j5;

		angle[6] = j6;
		angle[7] = j7;
		angle[8] = j8;

		angle[9] = j9;
		angle[10] = j10;
		angle[11] = j11;

		angle[12] = j12;
		angle[13] = j13;
		angle[14] = j14;
		angle[15] = j15;

		angle[16] = j16;
		angle[17] = j17;
	};
};

struct LegParam{
	double x;
	double y;
	double z;
	double w;
	double h;
	double theta;
	double phi;
	LegParam(double X,double Y,double Z,double W,double H,double THETA,double PHI){
		x = X;
		y = Y;
		z = Z;
		w = W;
		h = H;
		theta = THETA;
		phi = PHI;
	};
	LegParam(){
		x = 0;
		y = 100;
		z = 0;
		w = 0;
		h = 0;
		theta = 0;
		phi = 0;
	};
};

struct WalkParam{
	LegParam legparam[4];
	double time;
	double gratio;
	double aratio;

	double move_x;
	double move_y;
	double move_tt;
	
	WalkParam(LegParam l1,LegParam l2,LegParam l3,LegParam l4,double t,double r, double x_, double y_, double tt_){
		legparam[0] = l1;
		legparam[1] = l2;
		legparam[2] = l3;
		legparam[3] = l4;
		time = t;
		gratio = r;

		move_x = x_;
		move_y = y_;
		move_tt = tt_;
	};

	WalkParam(){
		legparam[0] = LegParam();
		legparam[1] = LegParam();
		legparam[2] = LegParam();
		legparam[3] = LegParam();
		time = 1000;
		gratio = 0.5;
		move_x = 0;
		move_y = 0;
		move_tt = 0;
	};
};

#endif


