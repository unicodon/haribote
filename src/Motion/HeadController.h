#ifndef HEADCONTROLLER_H
#define HEADCONTROLLER_H

/*
TailControllerの使い方　05/04/13 takeshita

SetHeadAngle(tilt, pan, tilt2, chin, speed)
で移動させたい角度(tilt1, pan, tilt2, chin)を指定し，
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

#include "../config.h"

struct HeadParam{
    double t1_val;
    double pa_val;
	double t2_val;
	double ch_val;
};

class HeadController{
public:
	HeadController();
	virtual ~HeadController();
	//次の関節角指令値を読み出す
	static void GetNextJointValue(double &tilt1, double &pan, double &tilt2, double &chin);
	//目標関節角を指定．speed[ms]後に目標到達
	static void SetHeadAngle(double tilt1, double pan, double tilt2, double chin, int speed);
	//自動モード
	static void SetHeadMode(HEAD_MODE h_mode);
	static bool finished;
	static bool automode;
	static HEAD_MODE mode;
	static HEAD_MODE last_mode;
	static bool start_heading;
	static bool start;
	static void SetTrackingTilt1(double t1);
	static void SetRadius(double r);
	static void SetTrackingV(double v);
	static HOLD_STATE hold_state;

private:
	//自動モード用
	//ボール追尾
	static void Tracking(double &tilt1, double &pan, double &tilt2, double &chin, bool near);
	//あたりを見回す
	static void SwingingForLocalization(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingForBallSearch(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingForBallSearchFast(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingLeft(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingRight(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingEight(double &tilt1, double &pan, double &tilt2, double &chin);
	static void WatchingGround(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingForBallSearchKP(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingHeading(double &tilt1, double &pan, double &tilt2, double &chin);


	//ウシくーーん！！
	static void UshiKuning(double &tilt1, double &pan, double &tilt2, double &chin);
	//DPでボール接近中，素早く周囲を見渡す
	static void Watching(double &tilt1, double &pan, double &tilt2, double &chin);
	//ボールを捕まえる
	static void BallHolding(double &tilt1, double &pan, double &tilt2, double &chin);
	static HeadParam setHead;
	static HeadParam lastHead;
	static double frame_num;
	static double tracking_tilt1;//トラッキングのときのTilt1
	static double tracking_v;//トラッキングのときのオレンジの重心の目標の画像中での垂直方向の位置
	static double radius;//ball半径
};

#endif
