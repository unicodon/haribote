#include "StdAfx.h"
#include ".\motionengine.h"

static const char c_defPoseStr[] = 
	"2000,-5,13,106,-5,13,106,-46,0,92,-46,0,92,0,0,0,0";

MotionEngine::MotionEngine(void)
{
	current = 0;
	time = 0;
	stop = false;
	poses.clear();
}

MotionEngine::~MotionEngine(void)
{
}

void MotionEngine::Clear()
{
	current = 0;
	time = 0;
	poses.clear();
	stop = false;
}

void MotionEngine::AddTail()
{
	poses[poses.size() - 1].timeInterval = 1.0;
	ReadLine(c_defPoseStr);
}

void MotionEngine::Start()
{
	stop = false;
}

void MotionEngine::GetNextAngle(float dtime, dReal* angles)
{
	if(stop)return;

	int numPose = poses.size();
	if (numPose <= 0) return;

	int next = (current + 1) % numPose;
	float p =  time / (poses[current].timeInterval);

	for (int i = 0; i < NUM_JOINT; i++)
	{
		angles[i] = 
			poses[current].jointAngles[i] * (1-p) + 
			poses[next].jointAngles[i]	  * p;
	}

	time += dtime;
	while (time > poses[current].timeInterval){
		time -= poses[current].timeInterval;
		current = next;
		next = (current + 1) % numPose;
		if(next == 0) stop = true;
	}
}


void MotionEngine::ReadLine(CString str)
{
	static char buf[256];

	if (str.GetLength() > 254) return;

	strcpy(buf, str);

	char* token;

	Pose p = {
		1.0f,

		0.0f,
		0.0f,
		0.0f,
		0.0f,

		0.0f,
		0.0f,
		0.0f,
		0.0f,
	
		0.0f,
		0.0f,
		0.0f,
		0.0f,
	
		0.0f,
		0.0f,
		0.0f,
		0.0f
	};
	
	token = strtok(buf, " ,\t\n\r");
	if (!token) goto final;
	p.timeInterval = atof(token) / 1000.0;

	for (int i = 0; i < NUM_JOINT; i++)
	{
		token = strtok(NULL, " ,\t\n\r");
		if (!token) goto final;
		p.jointAngles[i] = atof(token) / 180 * 3.1416;
	}

final:
	poses.push_back(p);
}