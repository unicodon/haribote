#include "StdAfx.h"
#include "MotionMaker.h"
#include "../State/State.h"

MotionParam* MotionMaker::mymotion = NULL;
PresetMotion MotionMaker::presetMotions;
jointangles MotionMaker::lastangles;
bool MotionMaker::finished = true;
double MotionMaker::timer = 0;//���̃|�[�Y�̂Ȃ��ł̎��Ԍo��[ms]
int MotionMaker::poseIndex = 0;//���Ԗڂ̃|�[�Y��


MotionMaker::MotionMaker(){
	mymotion = NULL;
	finished = true;
	timer = 0;
}

MotionMaker::~MotionMaker(){
}

//���[�V�����������I��
void MotionMaker::FinishMotion(){
	finished = true;
	lastangles = jointangles();
}

//���[�V�������I����Ă邩�ǂ���
bool MotionMaker::IsFinished(){
	return finished;
}

//�V�������[�V�������Z�b�g(�Œ胂�[�V����)
int MotionMaker::SetMotion(int n){
	if(!finished)
		return 0;
	if(n >= NUM_PRESET_MOTIONS)
		return 0;
	mymotion = &(PresetMotion::motions[n]);
	return 1;
}

//�V�������[�V�������Z�b�g(�t�@�C������ǂݍ��ނ���)
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

//�����ō����
int MotionMaker::SetMotion(MotionParam* motionparam){
	if(finished){
		mymotion = motionparam;
		return 1;
	}else{
		return 0;
	}
}


//���[�V�����������߂�
int MotionMaker::StartMotion(){
	if(!finished)
		return 0;
	memcpy(lastangles.angle, state.self.sensor.jointValue,sizeof(double)*NUM_JOINTS);
	finished = false;
	timer = 0;
	poseIndex = 0;
	return 1;
}

//���̊֐ߊp�f�[�^���v�Z���ĕԂ�
void MotionMaker::GetNextJointValue(jointangles &angles){
	if(finished){
		angles = lastangles;
		return;
	}

	timer += 8.0;//8.0[ms] = 1�t���[���̎���
	if(timer > mymotion->pose[poseIndex].intervaltime){
		if(poseIndex >= mymotion->num_pose - 1){//���[�V�����̍Ō�܂ōs������
			lastangles = mymotion->pose[poseIndex].angles;
			finished = true;
			return;
		}

		timer -= mymotion->pose[poseIndex].intervaltime;
		lastangles = mymotion->pose[poseIndex].angles;
		poseIndex++;
	}

	//lastangles�Ǝ��̃|�[�Y�Ƃ̓��}�����߂�
	MotionPose* pNextPose = &(mymotion->pose[poseIndex]);
	double r = timer / pNextPose->intervaltime;//������
	for(int i = 0;i < NUM_JOINTS;i++){
		angles.angle[i] = (1-r)*lastangles.angle[i] + r*pNextPose->angles.angle[i];
	}

}

