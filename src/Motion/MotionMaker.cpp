#include "StdAfx.h"
#include "MotionMaker.h"
#include "../State/State.h"

MotionParam* MotionMaker::mymotion = NULL;
PresetMotion MotionMaker::presetMotions;
jointangles MotionMaker::lastangles;
bool MotionMaker::finished = true;
double MotionMaker::timer = 0;//そのポーズのなかでの時間経過[ms]
int MotionMaker::poseIndex = 0;//何番目のポーズか


MotionMaker::MotionMaker(){
	mymotion = NULL;
	finished = true;
	timer = 0;
}

MotionMaker::~MotionMaker(){
}

//モーションを強制終了
void MotionMaker::FinishMotion(){
	finished = true;
	lastangles = jointangles();
}

//モーションが終わってるかどうか
bool MotionMaker::IsFinished(){
	return finished;
}

//新しいモーションをセット(固定モーション)
int MotionMaker::SetMotion(int n){
	if(!finished)
		return 0;
	if(n >= NUM_PRESET_MOTIONS)
		return 0;
	mymotion = &(PresetMotion::motions[n]);
	return 1;
}

//新しいモーションをセット(ファイルから読み込むもの)
int MotionMaker::SetMotion(const char* motionname){
	if(!finished)
		return 0;
	int motionIndex = PresetMotion::ResolveMotionName(motionname);
	if(motionIndex >= 0){
		mymotion = &(PresetMotion::motions[motionIndex]);		
		return 1;
	}else{
		return 0;
	}
}

//自分で作るやつ
int MotionMaker::SetMotion(MotionParam* motionparam){
	if(finished){
		mymotion = motionparam;
		return 1;
	}else{
		return 0;
	}
}


//モーションを巻き戻し
int MotionMaker::StartMotion(){
	if(!finished)
		return 0;
	memcpy(lastangles.angle, state.self.sensor.jointValue,sizeof(double)*NUM_JOINTS);
	finished = false;
	timer = 0;
	poseIndex = 0;
	return 1;
}

//次の関節角データを計算して返す
void MotionMaker::GetNextJointValue(jointangles &angles){
	if(finished){
		angles = lastangles;
		return;
	}

	timer += 8.0;//8.0[ms] = 1フレームの時間
	if(timer > mymotion->pose[poseIndex].intervaltime){
		if(poseIndex >= mymotion->num_pose - 1){//モーションの最後まで行ったら
			lastangles = mymotion->pose[poseIndex].angles;
			finished = true;
			return;
		}

		timer -= mymotion->pose[poseIndex].intervaltime;
		lastangles = mymotion->pose[poseIndex].angles;
		poseIndex++;
	}

	//lastanglesと次のポーズとの内挿を求める
	MotionPose* pNextPose = &(mymotion->pose[poseIndex]);
	double r = timer / pNextPose->intervaltime;//内分比
	for(int i = 0;i < NUM_JOINTS;i++){
		angles.angle[i] = (1-r)*lastangles.angle[i] + r*pNextPose->angles.angle[i];
	}

}

