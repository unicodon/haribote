#include "StdAfx.h"
#include "DeadRec.h"

MCLSample* DeadRec::sample;
bool DeadRec::sample_set = false;

DeadRec::DeadRec()
{
}

void
DeadRec::SetSample(MCLSample* input)
{
	sample = input;
	int t;
	srand(time(NULL));
	sample_set = true;
}

void
DeadRec::UpdatePosition(const DeadRecInformation info)
{
	if(!sample_set) return;

//	OSYSPRINT(("DeadRec::UpdatePosition() %f %f %f\n",info.dx,info.dy,info.dt));
	double x,y,t;
	x = info.dx;y = info.dy;t = info.dt*DEGTO;
	
	int max_ex = abs((int)(x*0.1))+1;
	int max_ey = abs((int)(y*0.1))+1;
	int max_et = abs((int)(t*0.25))+2;
	int max_ex2 = max_ex*2+1;
	int max_ey2 = max_ey*2+1;
	int max_et2 = max_et*2+1;

	double ex,ey,et;
	unsigned int a;
	int tmp_x,tmp_y;


//	double prob_distribution[SAMPLE_NUMBER+1];
	static int prob_distribution[SAMPLE_NUMBER+1];
	double prob_sum = 0.0;
	static int old_x[SAMPLE_NUMBER];
	static int old_y[SAMPLE_NUMBER];
	static unsigned int old_t[SAMPLE_NUMBER];
	prob_distribution[0] = 0;

	for(int i=0;i<SAMPLE_NUMBER;i++)
	{
		prob_sum += sample->sample[i].m_w;
		prob_distribution[i+1] = (int)(prob_sum*MAX_WEIGHT);
		old_x[i] = sample->sample[i].m_x;
		old_y[i] = sample->sample[i].m_y;
		old_t[i] = sample->sample[i].m_t;
	}
	
	for(int i=0;i<SAMPLE_NUMBER;i++)
	{
		int index = search_pos(0,SAMPLE_NUMBER+1,USHORT_RAND15b,prob_distribution);

		a = old_t[index]&MCLSample::m_nSinCosMask;

		ex = x + (USHORT_RAND15b%max_ex2-max_ex);
		ey = y + (USHORT_RAND15b%max_ey2-max_ey);
		et = t + (USHORT_RAND15b%max_et2-max_et);

		tmp_x = old_x[index] + (int)(ex*(MCLSample::m_dCos[a]) - ey*(MCLSample::m_dSin[a]));
		tmp_y = old_y[index] + (int)(ex*(MCLSample::m_dSin[a]) + ey*(MCLSample::m_dCos[a]));

		int ax = (int)abs(tmp_x);
		int ay = (int)abs(tmp_y);
		bool flag_normal = true;

		if(ax > FIELD_MAX_X + FIELD_REMAIN_X + ROBOT_LENGTH)		flag_normal = false;
		if(ay > FIELD_MAX_Y + FIELD_REMAIN_Y + ROBOT_LENGTH)		flag_normal = false;
		if(flag_normal)
		{
			sample->sample[i].m_x = tmp_x;
			sample->sample[i].m_y = tmp_y;
			sample->sample[i].m_t = old_t[index] + (int)et + 512;
			sample->sample[i].m_t &= MCLSample::m_nSinCosMask;
		}
		else
		{
			sample->sample[i].m_x = tmp_x;
			sample->sample[i].m_y = tmp_y;
			sample->sample[i].m_t = old_t[index] + (int)et;
			sample->sample[i].m_t &= MCLSample::m_nSinCosMask;
		}
		sample->sample[i].m_w = 1.0 / (double)SAMPLE_NUMBER;
	}
	state.self.position = sample->CalcAverageAndStdDev();
}


bool
DeadRec::GetLocomotion(double &x,double &y,double &t)
{
	OSYSDEBUG(("DeadRec::GetLocomotion() is currently unused\n"));
	/*
	static int prev_integ_times[NUM_OF_WALK];
	static bool flag = false;
	bool ret = false;
	ans.set(0.0,0.0,0.0);

	if(!flag)
	{
		flag = true;
		for(int i=0;i<NUM_OF_WALK;i++)
			prev_integ_times[i] = 0;
	}

	int step;
	for(int i=0;i<NUM_OF_WALK;i++)
	{
		step = state::self.walk.integ_times[i] - prev_integ_times[i];
		if(step != 0)
		{
			ans += m_p3dDeadRecData[i]*step;

			ret = true;
			prev_integ_times[i] += step;
		}
	}
		
	x = ans.get(0,0);	y = ans.get(1,0);	t = ans.get(2,0)*360.0/512.0;
	
	return ret;
	*/
	return false;
}

/*
int
DeadRec::search_pos(int pos_min, int pos_max, int value,int *prob_distribution)
{
	if(pos_max == pos_min+1)	return pos_min;

	int ans;
	int center = (pos_min+pos_max)>>1;
	if(prob_distribution[center] > value)
		ans = search_pos(pos_min,center,value,prob_distribution);
	else
		ans = search_pos(center,pos_max,value,prob_distribution);

	return ans;
}
*/
