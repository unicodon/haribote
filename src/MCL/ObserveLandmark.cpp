#include "StdAfx.h"
#include "ObserveLandmark.h"

MCLSample* ObserveLandmark::sample;
bool ObserveLandmark::sample_set = false;

ObserveLandmark::ObserveLandmark()
{
}

void
ObserveLandmark::SetSample(MCLSample* input)
{
	sample = input;
	sample_set = true;
}

void
ObserveLandmark::Observation(LandmarkInformation _input)
{
	if(!sample_set) return;

	//OSYSPRINT(("Observe Landmark ID = %d WIDTH = %d ANGLE = %f \n",_input.id,_input.width,_input.angle));
	const double coef = 22869.8132;
	int i = 0;
	static double dist[SAMPLE_NUMBER];
	int landmark_x,landmark_y;
	GetLandmarkXY(&landmark_x,&landmark_y,_input);
	
	int x_f,y_f;
	double lm_xr,lm_yr;
	int lm_ang;
	
	for(i=0;i<SAMPLE_NUMBER;i++)
	{
		x_f = landmark_x - sample->sample[i].m_x;//フィールド座標系でのランドマークの相対位置
		y_f = landmark_y - sample->sample[i].m_y;
		dist[i] = sqrt((double)(x_f*x_f + y_f*y_f));

		sample->sample[i].m_t &= MCLSample::m_nSinCosMask;
		unsigned int a = sample->sample[i].m_t;
		lm_xr = (landmark_x - sample->sample[i].m_x)*MCLSample::m_dCos[a] + (landmark_y - sample->sample[i].m_y)*MCLSample::m_dSin[a];
		lm_yr = -(landmark_x - sample->sample[i].m_x)*MCLSample::m_dSin[a] + (landmark_y - sample->sample[i].m_y)*MCLSample::m_dCos[a];
		lm_ang = (int)(atan2(lm_yr,lm_xr)*180.0/PI - _input.angle);

		if(lm_ang < angle_min || lm_ang > angle_max)	sample->sample[i].m_w *= 1e-10;
		else								sample->sample[i].m_w *= angle_likelyhood[lm_ang-angle_min];
	}
	
	//distance//////
	int w_index = (_input.width-width_step_min)/width_step_wid;
	if(w_index < width_step_num)
	{
		for(i=0;i<SAMPLE_NUMBER;i++)
		{
			int dist_index = (int)floor((dist[i]-dist_step_min)/(double)dist_step_wid);
			if(dist_index >= dist_step_num)	sample->sample[i].m_w *= pwd[w_index][dist_step_num-1];
			else if(dist_index > -1)		sample->sample[i].m_w *= pwd[w_index][dist_index];
			else							sample->sample[i].m_w *= 1e-10;
		}
	}
	else
	{
		for(i=0;i<SAMPLE_NUMBER;i++)
			if(dist[i] >= dist_step_min)
				sample->sample[i].m_w *= 1e-10;
	}
	double sum = 0.0;
	for(i=0;i<SAMPLE_NUMBER;i++)
		sum += sample->sample[i].m_w;

	const static double reset_th = pow(0.1,5);
	//const static double reset_th2 = pow(0.1,5)*5;
	double alpha = 1.0 - sum/reset_th;

	if(alpha <= 0.0)
	{
		for(i=0;i<SAMPLE_NUMBER;i++)	
			sample->sample[i].m_w /= sum;
	}
	else
	{
		if(state.self.position.x.dev > 300.0 ||
		   state.self.position.y.dev > 300.0 ||
		   state.self.position.tt.dev > 60.0)
		{
			alpha *= SAMPLE_NUMBER/2;
			SensorResetting((int)alpha,angle_like_sum,angle_range,angle_min,_input,sum);
		}
		else
		{
			sample->ExpansionResetting();
		}
	}
	
	/*
	alpha = 1.0 - sum/reset_th2;
	if(alpha > 0.0)
	{
		sample->ExpansionResetting();
	}*/
	state.self.position = sample->CalcAverageAndStdDev();
	//OSYSPRINT(("End ObserveLandmark::Observation()\n"));
}

void
ObserveLandmark::GetLandmarkXY(int *x,int *y,LandmarkInformation _input)
{
	switch(_input.id)
	{
		case HIGH_YELLOW_LOW_PINK:			*x = LANDMARK_X;			*y = -LANDMARK_Y;			break;
		case HIGH_SKYBLUE_LOW_PINK:			*x = -LANDMARK_X;			*y = -LANDMARK_Y;			break;
		case HIGH_PINK_LOW_YELLOW:			*x = LANDMARK_X;			*y = LANDMARK_Y;			break;
		case HIGH_PINK_LOW_SKYBLUE:			*x = -LANDMARK_X;			*y = LANDMARK_Y;			break;
	}
}

void
ObserveLandmark::SensorResetting(int reset_num, const int *likelyhood,int angle_range,int min,LandmarkInformation _input,double sum_weight)
{
	if(reset_num == 0) return;
	//OSYSPRINT(("SensorResetting()\n"));
	///////////////////////////////////
	static int rn = 0;
	static int prob_distribution[SAMPLE_NUMBER+1];
	double prob_sum = 0.0;
	static int old_x[SAMPLE_NUMBER];
	static int old_y[SAMPLE_NUMBER];
	static unsigned int old_t[SAMPLE_NUMBER];
	prob_distribution[0] = 0;

	//OSYSPRINT(("Initialize Sample\n"));
	for(int i=0;i<SAMPLE_NUMBER;i++)
	{
		prob_sum += sample->sample[i].m_w/sum_weight;
		prob_distribution[i+1] = (int)(prob_sum*MAX_WEIGHT);
		old_x[i] = sample->sample[i].m_x;
		old_y[i] = sample->sample[i].m_y;
		old_t[i] = sample->sample[i].m_t;
	}

	//OSYSPRINT(("Save Normal Sample\n"));
	const int x_goal_max = FIELD_MAX_X + 350;
	int normal_sample = SAMPLE_NUMBER - reset_num;
	for(int i=0;i<normal_sample;i++)
	{
		int index = search_pos(0,SAMPLE_NUMBER+1,USHORT_RAND15b,prob_distribution);

		int input_index = SAMPLE_NUMBER-1-i;

		sample->sample[input_index].m_x = old_x[index];
		sample->sample[input_index].m_y = old_y[index];
		sample->sample[input_index].m_t = old_t[index];

		sample->sample[i].m_w = 1.0/(double)SAMPLE_NUMBER;
	}
	//////////////////////////////////
	int dist;
	int landmark_x,landmark_y;
	GetLandmarkXY(&landmark_x,&landmark_y,_input);
	
	//OSYSPRINT(("Start Resseting\n"));
	if(_input.id == HIGH_PINK_LOW_SKYBLUE)
	{
		double step = 256.0/(double)reset_num;
		for(int i=0;i<reset_num;i++)
		{
			int w = (_input.width-width_step_min)/width_step_wid;
			if(w >= width_step_num)		w = width_step_num-1;
			int dist_array_pos = search_pos(0,dist_step_num+1,USHORT_RAND15b,pdw[w]);
			int dist_min = dist_step_min + dist_array_pos*dist_step_wid;
			dist = (int)(dist_min + USHORT_RAND15b%dist_step_wid);
			
			int x = (int)(landmark_x + dist*MCLSample::m_dCos[(int)(step*i)]);
			int y = (int)(landmark_y - dist*MCLSample::m_dSin[(int)(step*i)]);
			sample->sample[i].m_x = x;
			sample->sample[i].m_y = y;

			int view_angle = search_pos(0,angle_range+1,USHORT_RAND15b,likelyhood);
			view_angle += min;
			double robot_angle = atan2((double)(landmark_y - y),(double)(landmark_x - x))*180.0/PI - _input.angle + view_angle;
			while(robot_angle < 0.0)	robot_angle += 360.0;
			sample->sample[i].m_t = (unsigned int)(robot_angle*256.0/180.0);
			sample->sample[i].m_t &= MCLSample::m_nSinCosMask;
			sample->sample[i].m_w = 1.0/(double)SAMPLE_NUMBER;
		}
	}
	else if(_input.id == HIGH_PINK_LOW_YELLOW)
	{
		double step = 256.0/(double)reset_num;
		for(int i=0;i<reset_num;i++)
		{
			int w = (_input.width-width_step_min)/width_step_wid;
			if(w >= width_step_num)		w = width_step_num-1;
			int dist_array_pos = search_pos(0,dist_step_num+1,USHORT_RAND15b,pdw[w]);
			int dist_min = dist_step_min + dist_array_pos*dist_step_wid;
			dist = dist_min + USHORT_RAND15b%dist_step_wid;
			
			int x = (int)(landmark_x - dist*MCLSample::m_dCos[(int)(step*i)]);
			int y = (int)(landmark_y - dist*MCLSample::m_dSin[(int)(step*i)]);

			sample->sample[i].m_x = x;
			sample->sample[i].m_y = y;

			int view_angle = search_pos(0,angle_range+1,USHORT_RAND15b,likelyhood);
			view_angle += min;
			double robot_angle = atan2((double)(landmark_y - y),(double)(landmark_x - x))*180.0/PI - _input.angle + view_angle;
			while(robot_angle < 0.0)	robot_angle += 360.0;
			sample->sample[i].m_t = (unsigned int)(robot_angle*256.0/180.0);
			sample->sample[i].m_t &= MCLSample::m_nSinCosMask;
			sample->sample[i].m_w = 1.0/(double)SAMPLE_NUMBER;
		}
	}
	else if(_input.id == HIGH_SKYBLUE_LOW_PINK)
	{
		double step = 256.0/(double)reset_num;
		for(int i=0;i<reset_num;i++)
		{
			int w = (_input.width-width_step_min)/width_step_wid;
			if(w >= width_step_num)		w = width_step_num-1;
			int dist_array_pos = search_pos(0,dist_step_num+1,USHORT_RAND15b,pdw[w]);
			int dist_min = dist_step_min + dist_array_pos*dist_step_wid;
			dist = dist_min + USHORT_RAND15b%dist_step_wid;
			
			int x = (int)(landmark_x + dist*MCLSample::m_dCos[(int)(step*i)]);
			int y = (int)(landmark_y + dist*MCLSample::m_dSin[(int)(step*i)]);
			sample->sample[i].m_x = x;
			sample->sample[i].m_y = y;

			int view_angle = search_pos(0,angle_range+1,USHORT_RAND15b,likelyhood);
			view_angle += min;

			double robot_angle = atan2((double)(landmark_y - y),(double)(landmark_x - x))*180.0/PI - _input.angle + view_angle;
			while(robot_angle < 0.0)	robot_angle += 360.0;
			sample->sample[i].m_t = (unsigned int)(robot_angle*256.0/180.0);
			sample->sample[i].m_t &= MCLSample::m_nSinCosMask;
			sample->sample[i].m_w = 1.0/(double)SAMPLE_NUMBER;
		}
	}
	else if(_input.id == HIGH_YELLOW_LOW_PINK)
	{
		double step = 256.0/(double)reset_num;
		for(int i=0;i<reset_num;i++)
		{
			int w = (_input.width-width_step_min)/width_step_wid;
			if(w >= width_step_num)		w = width_step_num-1;
			int dist_array_pos = search_pos(0,dist_step_num+1,USHORT_RAND15b,pdw[w]);
			int dist_min = dist_step_min + dist_array_pos*dist_step_wid;
			dist = dist_min + USHORT_RAND15b%dist_step_wid;
			
			int x = (int)(landmark_x - dist*MCLSample::m_dCos[(int)(step*i)]);
			int y = (int)(landmark_y + dist*MCLSample::m_dSin[(int)(step*i)]);
			sample->sample[i].m_x = x;
			sample->sample[i].m_y = y;
			int view_angle = search_pos(0,angle_range+1,USHORT_RAND15b,likelyhood);
			view_angle += min;

			double robot_angle = atan2((double)(landmark_y - y),(double)(landmark_x - x))*180.0/PI - _input.angle + view_angle;
			while(robot_angle < 0.0)	robot_angle += 360.0;
			sample->sample[i].m_t = (unsigned int)(robot_angle*256.0/180.0);
			sample->sample[i].m_t &= MCLSample::m_nSinCosMask;
			sample->sample[i].m_w = 1.0/(double)SAMPLE_NUMBER;
		}
	}
	//OSYSPRINT(("End SensorResetting()\n"));
}

/*
int
ObserveLandmark::search_pos(int pos_min, int pos_max, int value,const int *prob_distribution)
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

