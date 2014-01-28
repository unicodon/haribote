#include "StdAfx.h"
#include "TailController.h"
#include "../State/State.h"
#include "MotionMaker.h"
#include <OPENR/OSyslog.h>
#include <math.h>

bool TailController::finished = true;
bool TailController::start    = true;//êVÇΩÇ»éwóﬂäpÇó^Ç¶ÇÁÇÍÇΩÇŒÇ©ÇËÇ©Ç«Ç§Ç©
bool TailController::off      = false;//îˆÇÃìÆçÏÇé~ÇﬂÇÈÇ©
TailParam TailController::setTail  = {0.0, 0.0};
TailParam TailController::lastTail = {0.0, 0.0};
double TailController::frame_num = 8;
bool TailController::auto_mode = true;
TAIL_MODE TailController::tail_mode = TAIL_STOP;


TailController::TailController(){
}

TailController::~TailController(){
}

void TailController::GetNextJointValue(double &t_t, double &t_p){

//	OPowerStatus power;
//	OPENR::GetPowerStatus(&power);
	//OSYSPRINT(("POWER = %3d %\n",power.remainingCapacity));
/*
	const int cycle = 250*100/power.remainingCapacity;//[ms]
	if(auto_mode){
		switch(tail_mode){
			case TAIL_STOP:
				t_t = 5.0;
				t_p = 0.0;
				return;
			case TAIL_SWING:
				static int timer = 0;
				timer += 8;
				t_t = 5.0;
				t_p = 45*cos(PI*timer/cycle);
				if(timer > cycle)
					timer -= cycle;				
				return;
			case TAIL_ANGRY:
				t_t = 60.0;
				t_p = 0.0;
				return;
		}
	}

*/
	static double del_t = 0.0;
	static double del_p = 0.0;
	static double pre_t = 0.0;
	static double pre_p = 0.0;
	static int timer = 0;

	if(start){
		del_t = (setTail.t_val - pre_t)/frame_num;//lastTail.t1_val)/frame_num;//
		del_p = (setTail.p_val - pre_p)/frame_num;//lastTail.pa_val)/frame_num;//
		timer = 0;
		start = false;
	}
	
	timer++;

	pre_t = del_t + pre_t;//timer*del_t1 + lastTail.t1_val;//
	pre_p = del_p + pre_p;//timer*del_pa + lastTail.pa_val;//
	
	if(pre_t > 60)		pre_t = 60.0;
	if(pre_t < 5)		pre_t = 5.0;
	if(pre_p > 45)		pre_p = 45.0;
	if(pre_p < -45)		pre_p = -45.0;
	
	t_t = pre_t;
	t_p = pre_p;

	if(timer > frame_num){
		finished = true;
		del_t = 0.0;
		del_p = 0.0;
	}
}

//êUÇËÇèIóπ
void TailController::Off(){
	off = true;
	tail_mode = TAIL_STOP;
}

int TailController::SetTailAngle(double tail_t, double tail_p, int speed){
	static double pre_t = 666;
	static double pre_p = 666;
	static double pre_s = 666;

	if((tail_t!=pre_t) || (tail_p!=pre_p) || (speed!=pre_s)){
        start    = true;
		finished = false;
		off      = false;

		pre_t = tail_t;
		pre_p = tail_p;
		pre_s = speed;

		setTail.t_val = tail_t;
		setTail.p_val = tail_p;
		frame_num = speed/8;//8msec = 1frameÇæÇ©ÇÁÅD
	}
	
// by Jiji
	lastTail.t_val = state.self.sensor.jointValue[TAILTILT];
	lastTail.p_val = state.self.sensor.jointValue[TAILPAN];
//end
	auto_mode = false;
	return 0;
}

void TailController::SetTailMode(TAIL_MODE mode){
	if(mode < NUM_TAIL_MODE){
		tail_mode = mode;
		auto_mode = true;
	}
}
