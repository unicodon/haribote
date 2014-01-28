#include "StdAfx.h"
#include "ObserveLine.h"

#include <math.h>
#include <stdio.h>
#include <vector>
#include "../config.h"
#include "../State/State.h"
#include <OPENR/OSyslog.h>
#include <sstream>
#include <fstream>

#define LINE_VIEW_THRESHOLD 1000

MCLSample* ObserveLine::sample;
bool ObserveLine::sample_set = false;
LineStruct ObserveLine::lines[NUM_OF_LINE] =
{
	//Â‚©‚ç‚Ý‚½‚Æ‚«‚Ìâ‘ÎÀ•W
	//Field Lines
	{X,2712		,-1812	,1812},			//{X,2712 ,	-2000	,2000},
	{X,-2712	,-1812	,1812},			//{X,-2712,	-2000	,2000},
	{X,0		,-1812	,1812},			//{X,0	  ,	-2000	,2000},
	{Y,1812		-2700	,2712},			//{Y,2012 ,	-2700	,2700},
	{Y,-1812	,-2700	,2712},			//{Y,-2012,	-2700	,2700},
	//Goal Area Lines
	{X,2062		,-650	,650},			//{X,1062	,-650	,650},
	{X,-2062	,-650	,650},			//{X,-1062	,-650	,650},
	{Y,650		,2062	,2712},			//{Y,637	,2050	,2700},
	{Y,-650		,2062	,2712},			//{Y,-637	,2050	,2700},
	{Y,650		,-2712	,-2062},		//{Y,637	,-2700	,-2050},
	{Y,-650		,-2712	,-2062}			//{Y,-637	,-2700	,-2050}
};
SlatePoint ObserveLine::slatePoint[2*NUM_OF_LINE];

ObserveLine::ObserveLine()
{
}

void
ObserveLine::SetSample(MCLSample* input)
{
	sample = input;
	sample_set = true;
}

void
ObserveLine::Observation(LineInformation _input)
{
	if(!sample_set) return;

	if(_input.r >= 1200.0) return;

	//std::stringstream ss;
	//ss << "Line: type = " << _input.type << " r = " << _input.r << " theta = " << _input.theta << " pan = " << state.self.sensor.jointValue[PAN] << "\n";
	//OSYSPRINT((ss.str().c_str()));

	for(int i = 0;i < NUM_OF_LINE;i++)
	{
		int slate_number = i*2;
		//direction
		slatePoint[slate_number].direction = lines[i].direction;
		slatePoint[slate_number+1].direction = lines[i].direction;
		//Position
		slatePoint[slate_number].constant = lines[i].constant + _input.r;
		slatePoint[slate_number+1].constant = lines[i].constant - _input.r;

		//Val of Position
		slatePoint[slate_number].val_constant =max(_input.r/6,50);
		slatePoint[slate_number+1].val_constant =max(_input.r/6,50);

		//theta
		if(lines[i].direction == Y)
		{
			slatePoint[slate_number].theta = -_input.theta - 180;
			slatePoint[slate_number+1].theta = -_input.theta;
		}
		else//line[i].direction == X
		{
			slatePoint[slate_number].theta = -(_input.theta - 90);
			slatePoint[slate_number+1].theta = -(_input.theta + 90);
		}

		while(slatePoint[slate_number].theta < 0) slatePoint[slate_number].theta += 360;
		while(slatePoint[slate_number].theta >= 360.0) slatePoint[slate_number].theta -= 360;
		while(slatePoint[slate_number+1].theta < 0) slatePoint[slate_number+1].theta += 360;
		while(slatePoint[slate_number+1].theta >= 360.0) slatePoint[slate_number+1].theta -= 360;

		//val_of_theta
		slatePoint[slate_number].val_theta = 10;
		slatePoint[slate_number+1].val_theta = 10;
	}
	
	int r_different;
	int theta_different;
	int particle_theta;

	for(int i = 0;i < SAMPLE_NUMBER;i++)
	{
		bool in_area = false;
		for(int j = 0;j < 2*NUM_OF_LINE;j++)
		{
			if(slatePoint[j].direction == X)
			{
				if(sample->sample[i].m_y - lines[j/2].end > LINE_VIEW_THRESHOLD || lines[j/2].start - sample->sample[i].m_y > LINE_VIEW_THRESHOLD)
				{
					continue;
				}
				r_different = abs(slatePoint[j].constant - sample->sample[i].m_x);
			}
			else
			{
				if(sample->sample[i].m_x - lines[j/2].end > LINE_VIEW_THRESHOLD || lines[j/2].start - sample->sample[i].m_x > LINE_VIEW_THRESHOLD)
				{
					continue;
				}
				r_different = abs(slatePoint[j].constant - sample->sample[i].m_y);
			}
			particle_theta = (int)(sample->sample[i].m_t*180.0/256.0);

			theta_different = min(abs(particle_theta - slatePoint[j].theta),360-abs(particle_theta - slatePoint[j].theta));

			if(r_different <= slatePoint[j].val_constant && theta_different <= slatePoint[j].val_theta)
			{
				in_area = true;
				break;
			}
		}
		if(!in_area) sample->sample[i].m_w *= 0.01;
	}
	double sum = 0.0;
	for(int i=0;i<SAMPLE_NUMBER;i++)
		sum += sample->sample[i].m_w;

	if(sum < 0.02)
		sample->ExpansionResetting();
	else
	{
		for(int i=0;i<SAMPLE_NUMBER;i++)	
			sample->sample[i].m_w /= sum;
	}
	state.self.position = sample->CalcAverageAndStdDev();
	//OSYSPRINT(("x = %4.2f y = %4.2f theta = %4.2f\n",state.self.position.x.val,state.self.position.y.val,state.self.position.tt.val));
}
