#ifndef HEADCONTROLLER_H
#define HEADCONTROLLER_H

/*
TailController�̎g�����@05/04/13 takeshita

SetHeadAngle(tilt, pan, tilt2, chin, speed)
�ňړ����������p�x(tilt1, pan, tilt2, chin)���w�肵�C
�����܂łɉ�msec�ňړ����邩(speed)��ݒ肷��D

GetNextJoinValue()
�Ŏ��Ɏw�߂��ׂ��֐ߊp�f�[�^���擾���āC
AIBO�ɑ��M����D

���[�V�������I�����Ă邩�ǂ�����
finished
�̒l�Ŕ��肷��

Off()
�͋����I�ɏI����Ԃɂ���
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
	//���̊֐ߊp�w�ߒl��ǂݏo��
	static void GetNextJointValue(double &tilt1, double &pan, double &tilt2, double &chin);
	//�ڕW�֐ߊp���w��Dspeed[ms]��ɖڕW���B
	static void SetHeadAngle(double tilt1, double pan, double tilt2, double chin, int speed);
	//�������[�h
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
	//�������[�h�p
	//�{�[���ǔ�
	static void Tracking(double &tilt1, double &pan, double &tilt2, double &chin, bool near);
	//�����������
	static void SwingingForLocalization(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingForBallSearch(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingForBallSearchFast(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingLeft(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingRight(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingEight(double &tilt1, double &pan, double &tilt2, double &chin);
	static void WatchingGround(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingForBallSearchKP(double &tilt1, double &pan, double &tilt2, double &chin);
	static void SwingingHeading(double &tilt1, double &pan, double &tilt2, double &chin);


	//�E�V���[�[��I�I
	static void UshiKuning(double &tilt1, double &pan, double &tilt2, double &chin);
	//DP�Ń{�[���ڋߒ��C�f�������͂����n��
	static void Watching(double &tilt1, double &pan, double &tilt2, double &chin);
	//�{�[����߂܂���
	static void BallHolding(double &tilt1, double &pan, double &tilt2, double &chin);
	static HeadParam setHead;
	static HeadParam lastHead;
	static double frame_num;
	static double tracking_tilt1;//�g���b�L���O�̂Ƃ���Tilt1
	static double tracking_v;//�g���b�L���O�̂Ƃ��̃I�����W�̏d�S�̖ڕW�̉摜���ł̐��������̈ʒu
	static double radius;//ball���a
};

#endif
