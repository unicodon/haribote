#ifndef TAILCONTROLLER_H
#define TAILCONTROLLER_H

#include "../config.h"

/*
TailControllerの使い方　05/04/13 takeshita

SetTailAngle(tilt, pan, speed)
で移動させたい角度(tilt, pan)を指定し，
そこまでに何msecで移動するか(speed)を設定する．

GetNextJoinValue()
で次に指令すべき関節角データを取得して，
AIBOに送信する．

モーションが終了してるかどうかは
finished
の値で判定する

Off()
は強制的に終了状態にする
*/

struct TailParam{
    double t_val;
	double p_val;
};

class TailController{
public:
	TailController();
	virtual ~TailController();
	static void SetTailMode(TAIL_MODE mode);
	static void GetNextJointValue(double &t_t, double &t_p);
	static int SetTailAngle(double tail_t, double tail_p, int speed);
	static void Off();
	static bool finished;
	static bool start;
	static bool off;

private:
	static bool auto_mode;
	static TailParam setTail;
	static TailParam lastTail;
	static double frame_num;
	static TAIL_MODE tail_mode;
};

#endif
