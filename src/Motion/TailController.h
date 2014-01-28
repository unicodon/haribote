#ifndef TAILCONTROLLER_H
#define TAILCONTROLLER_H

#include "../config.h"

/*
TailController�̎g�����@05/04/13 takeshita

SetTailAngle(tilt, pan, speed)
�ňړ����������p�x(tilt, pan)���w�肵�C
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
