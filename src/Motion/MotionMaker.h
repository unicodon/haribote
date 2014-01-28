#ifndef MOTIONMAKER_H
#define MOTIONMAKER_H

#include <vector>

#include "../joint.h"
#include "PresetMotion.h"

/*
MotionMaker�̎g����

SetMotion("���[�V������")�C���邢��
SetMotion(�g�ݍ��ݍς݃��[�V�����̔ԍ�)
�Ń��[�V�������Z�b�g

StartMotion�Ń��[�V�������J�n

GetNextJoinValue()
�Ŏ��Ɏw�߂��ׂ��֐ߊp�f�[�^���擾���āC
AIBO�ɑ��M����D

���[�V�������I�����Ă邩�ǂ�����
IsFinished()
�Ŕ��肷��

FinishMotion()
�͋����I�ɏI����Ԃɂ���
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
	static double timer;//���̃|�[�Y�̂Ȃ��ł̎��Ԍo��[ms]
	static int poseIndex;//���Ԗڂ̃|�[�Y��
};

#endif
