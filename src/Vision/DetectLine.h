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
	//�R���X�g���N�^
	DetectLine();
	//���s���ɌĂ΂��
	static void Execute(void);

private:
	//���̔��������o
	static void DetectHolizontalLine(double &a0, double &b0, double &ee);
	//���Ă̔��������o
	static void DetectVerticalLine(double &a0, double &b0, double &ee);
	//�ŏ����@�ŌX���C�ؕЁC�덷�����߂�
	static void Method_LS(vector<Point> edge_point, double &a0, double &b0, double &ee);

};

#endif

