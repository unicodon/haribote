#ifndef DETECT_LINE_H
#define DETECT_LINE_H

#include <math.h>
#include <stdio.h>
#include <vector>
#include "VisionUtil.h"
#include "../config.h"
#include "../State/State.h"
#include <OPENR/OSyslog.h>
#include <sstream>
#include <fstream>

using namespace std;


struct Point {
	int x;
	int y;
} ;

class DetectLine{
public:
	//コンストラクタ
	DetectLine();
	//実行時に呼ばれる
	static void Execute(void);

private:
	//横の白線を検出
	static void DetectHolizontalLine(double &a0, double &b0, double &ee);
	//たての白線を検出
	static void DetectVerticalLine(double &a0, double &b0, double &ee);
	//最小二乗法で傾き，切片，誤差を求める
	static void Method_LS(vector<Point> edge_point, double &a0, double &b0, double &ee);

};

#endif

