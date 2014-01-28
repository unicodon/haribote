#ifndef MCL_SAMPLE_H
#define MCL_SAMPLE_H

#include "../config.h"
#include "../Utility/MatrixUtil.h"
#include "../State/State.h"

#include "../Utility/MtRand.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <OPENR/OSyslog.h>

#define SAMPLE_NUMBER 1000
#define SAMPLE_MAX_X FIELD_MAX_X+FIELD_REMAIN_X
#define SAMPLE_MAX_Y FIELD_MAX_Y+FIELD_REMAIN_Y

#define MAX_WEIGHT 0x7FFF

struct PositionSample
{
	int m_x;
	int m_y;
	unsigned int m_t;
	double m_w;
};


inline int search_pos(int pos_min, int pos_max, const int value, const int *prob_distribution)
{
	int center;
	while(pos_max > pos_min+1){
		center = (pos_min+pos_max) / 2;
		if(prob_distribution[center] > value)
			pos_max = center;
		else
			pos_min = center;
	}
	return pos_min;
}

#define USHORT_RAND15b (MCLSample::m_nRand[MCLSample::rn++])


class MCLSample{
public :
	//for sample_viewer
	void storage_data(void);

	//基本データ
	PositionSample sample[SAMPLE_NUMBER];

	static unsigned int m_nTmid;
	static unsigned int m_nTwid;

	MCLSample();
	~MCLSample();
	
	static unsigned short m_nRand[0x10000];
	static bool init_flag;
	static void InitRand();
	static unsigned short rn;

//	static const int m_nRand[0x1000];
//	static const int m_nRandomMask;
	static const double m_dSin[0x200];
	static const double m_dCos[0x200];
	static const short m_nSinCosMask;
	
	PositionData CalcAverageAndStdDev(void);
	void PrintSample(void);
	void InputStartPoint(int x,int y,int t);
	void ExpansionResetting();
	
private:
	void make_random_pos(void);
	void clip(void);
};

#endif //MCLSample
