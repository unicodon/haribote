#ifndef MOTIONMAKER_H
#define MOTIONMAKER_H

#include <vector>

#include "../joint.h"
#include "PresetMotion.h"

/*
MotionMakerの使い方

SetMotion("モーション名")，あるいは
SetMotion(組み込み済みモーションの番号)
でモーションをセット

StartMotionでモーションを開始

GetNextJoinValue()
で次に指令すべき関節角データを取得して，
AIBOに送信する．

モーションが終了してるかどうかは
IsFinished()
で判定する

FinishMotion()
は強制的に終了状態にする
*/

class MotionMaker{
public:
	MotionMaker();
	virtual ~MotionMaker();

	static void FinishMotion();
	static bool IsFinished();

	static int SetMotion(int n);
	static int SetMotion(const char* motionname);
	static int SetMotion(MotionParam* motionparam);
	static int StartMotion();

	static void GetNextJointValue(jointangles &angles);

private:
	static MotionParam *mymotion;
	static PresetMotion presetMotions;
	static jointangles lastangles;
	static bool finished;
	static double timer;//そのポーズのなかでの時間経過[ms]
	static int poseIndex;//何番目のポーズか
};

#endif
