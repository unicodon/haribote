#include "StdAfx.h"
#include "HeadController.h"
#include "../State/State.h"
#include "MotionMaker.h"
#include "PresetWalkParam.h"
#include <OPENR/OSyslog.h>

//#include "../Utility/Logger.h"

bool HeadController::finished = true;
bool HeadController::start    = true;//新たな指令角を与えられたばかりかどうか
bool HeadController::automode = false;
HEAD_MODE HeadController::mode	  = HEAD_STOP;
HeadParam HeadController::setHead  = {0.0, 0.0, 0.0, 0.0};
HeadParam HeadController::lastHead = {0.0, 0.0, 0.0, 0.0};
double HeadController::frame_num = 8;
static double c_x = 0.0;
static double c_y = 0.0;

HEAD_MODE HeadController::last_mode = HEAD_STOP;
bool HeadController::start_heading = false;


double HeadController::tracking_tilt1 = 90;
double HeadController::tracking_v = IMAGE_HEIGHT/2;
double HeadController::radius = 0.0;

HOLD_STATE HeadController::hold_state = HOLD_NOT;//掴み状態

//PID制御用
static double e_pa      = 0.0;
static double e_pa_buf1 = 0.0;
static double e_pa_buf2 = 0.0;
static double e_t2      = 0.0;
static double e_t2_buf1 = 0.0;
static double e_t2_buf2 = 0.0;
static double phi_pa = 0.0;//カメラからボールまでの角度
static double phi_t2 = 0.0;//同上
static double ball_d = 0.0;//カメラからボールまでの距離
int counter = 0;

HeadController::HeadController(){
}

HeadController::~HeadController(){
}

void HeadController::SetTrackingTilt1(double t1){
	tracking_tilt1 = t1;
}

void HeadController::SetRadius(double r){
	radius = r;
}

void HeadController::SetTrackingV(double v){
	tracking_v = v;
}


void HeadController::GetNextJointValue(double &tilt1, double &pan, double &tilt2, double &chin){
	//OSYSPRINT(("HeadController::GetNextJointValue() called\n"));
	static double del_t1 = 0.0;
	static double del_pa = 0.0;
	static double del_t2 = 0.0;
	static double del_ch = 0.0;
	static double pre_t1 = 0.0;
	static double pre_pa = 0.0;
	static double pre_t2 = 0.0;
	static double pre_ch = 0.0;
	static double pre_sp = 0.0;
	static int timer = 0;

	if(state.ball.losttime > 10){
        c_x = 0.0;
		c_y = 0.0;
	}

	if(automode){
		//pre_** のリセット
/*		pre_t1 = state.self.sensor.jointValue[TILT1];
		pre_pa = state.self.sensor.jointValue[PAN];
		pre_t2 = state.self.sensor.jointValue[TILT2];
		pre_ch = state.self.sensor.jointValue[CHIN];
*/
		switch(mode){
			case HEAD_STOP:
			default:
				SetHeadAngle(0.0, 0.0, 0.0, 0.0, 1000);
				return;
			case HEAD_SWING_LOCALIZATION:
				SwingingForLocalization(tilt1,pan,tilt2,chin);
				return;
			case HEAD_SWING_BALL_SEARCH:
				SwingingForBallSearch(tilt1,pan,tilt2,chin);
				return;
			case HEAD_SWING_BALL_SEARCH_FAST:
				SwingingForBallSearchFast(tilt1,pan,tilt2,chin);
				return;
			case HEAD_SWING_RIGHT:
				SwingingRight(tilt1,pan,tilt2,chin);
				return;
			case HEAD_SWING_LEFT:
				SwingingLeft(tilt1,pan,tilt2,chin);
				return;
			case HEAD_SWING_EIGHT:
				SwingingEight(tilt1,pan,tilt2,chin);
				return;
			case HEAD_WATCH_GROUND:
				WatchingGround(tilt1,pan,tilt2,chin);
				return;
			case HEAD_TRACK:
				if(!state.ball.visible && state.ball.losttime > 30){
					SwingingForBallSearch(tilt1,pan,tilt2,chin);
				}else{
					Tracking(tilt1, pan, tilt2, chin, false);
				}
				return;
			case HEAD_TRACK_KP:
				if(!state.ball.visible && state.ball.losttime > 30){
					SwingingForBallSearchKP(tilt1,pan,tilt2,chin);
				}else{
					Tracking(tilt1, pan, tilt2, chin, false);
				}
				return;
			case HEAD_TRACK_NEAR:
				if(!state.ball.visible && state.ball.losttime > 30){
					SwingingForBallSearch(tilt1,pan,tilt2,chin);
				}else{
					Tracking(tilt1, pan, tilt2, chin, true);
				}
				return;
			case HEAD_HOLD:
				SetHeadAngle(-60.0, 0.0, 45.0, -55.0, 500);
				return;
			case HEAD_USHIKUN:
				UshiKuning(tilt1, pan, tilt2, chin);
				return;
			case HEAD_WATCHING:
				Watching(tilt1, pan, tilt2, chin);
				return;
			case HEAD_BALL_HOLDING:
				BallHolding(tilt1, pan, tilt2, chin);
				return;
			case HEAD_HEADING_RIGHT:
			case HEAD_HEADING_LEFT:
				SwingingHeading(tilt1, pan, tilt2, chin);
				return;
		}
	}
	
	if(start){
		del_t1 = (setHead.t1_val - pre_t1)/frame_num;//lastHead.t1_val)/frame_num;//
		del_pa = (setHead.pa_val - pre_pa)/frame_num;//lastHead.pa_val)/frame_num;//
		del_t2 = (setHead.t2_val - pre_t2)/frame_num;//lastHead.t2_val)/frame_num;//
		del_ch = (setHead.ch_val - pre_ch)/frame_num;///frame_num;//lastHead.ch_val)/frame_num;//
		timer = 0;
		start = false;
	}

	timer++;

	pre_t1 = del_t1 + pre_t1;//timer*del_t1 + lastHead.t1_val;//
	pre_pa = del_pa + pre_pa;//timer*del_pa + lastHead.pa_val;//
	pre_t2 = del_t2 + pre_t2;//timer*del_t2 + lastHead.t2_val;//
	pre_ch = del_ch + pre_ch;//timer*del_ch + lastHead.ch_val;//

	if(pre_t1 >   0)	pre_t1 =   0.0;
	if(pre_t1 < -80)	pre_t1 = -80.0;
	if(pre_pa >  93)	pre_pa =  93.0;
	if(pre_pa < -93)	pre_pa = -93.0;
	if(pre_t2 >  50)	pre_t2 =  50.0;
	if(pre_t2 < -20)	pre_t2 = -20.0;
	if(pre_ch >  -3)	pre_ch =  -3.0;
	if(pre_ch < -55)	pre_ch = -55.0;

	tilt1 = pre_t1;
	pan   = pre_pa;
	tilt2 = pre_t2;
	chin  = pre_ch;

	if(timer > frame_num){
		finished = true;
		del_t1 = 0.0;
		del_pa = 0.0;
		del_t2 = 0.0;
		del_ch = 0.0;
	}
}


void HeadController::SetHeadAngle(double t1, double pa, double t2, double ch, int speed){
	static double pre_t1 = 666;
	static double pre_pa = 666;
	static double pre_t2 = 666;
	static double pre_ch = 666;
	static double pre_sp = 666;

	automode = false;

	if(finished || (t1!=pre_t1) || (pa!=pre_pa) || (t2!=pre_t2) || (ch!=pre_ch) || (speed!=pre_sp)){
        start    = true;
		finished = false;
		//automode = false;

		pre_t1 = t1;
		pre_pa = pa;
		pre_t2 = t2;
		pre_ch = ch;
		pre_sp = speed;

		setHead.t1_val = t1;
		setHead.pa_val = pa;
		setHead.t2_val = t2;
		setHead.ch_val = ch;
		frame_num = speed/8;//8msec = 1frameだから．
	}
	
// by Jiji
		lastHead.t1_val = state.self.sensor.jointValue[TILT1];
		lastHead.pa_val = state.self.sensor.jointValue[PAN];
		lastHead.t2_val = state.self.sensor.jointValue[TILT2];
		lastHead.ch_val = state.self.sensor.jointValue[CHIN];
//end
}

void HeadController::Tracking(double &tilt1, double &pan, double &tilt2, double &chin, bool near){
	//ごちゃごちゃになるけど間に合わせ by sakamoto
	//書き直したほうがよい
	//OSYSPRINT(("HeadController::Tracking() called\n"));	
	if(state.ball.losttime > BALL_TRACKING_FRAME)
	{
		e_pa_buf1 = 0.0;
		e_pa_buf2 = 0.0;
		e_t2_buf1 = 0.0;
		e_t2_buf2 = 0.0;

		SwingingForBallSearch(tilt1 ,pan,tilt2,chin);
	}
	static int frame_count = 0;

	static double pre_t1 = 0.0;
	static double pre_t2 = 0.0;
	static double pre_pa = 0.0;
	static double pre_ch = 0.0;
	static double del_t1 = 0.0;
	static double del_t2 = 0.0;
	static double del_pa = 0.0;
	static double del_ch = 0.0;

	static bool set = false;

	frame_num = NUM_FRAMES;
	if(!set)
	{
		//OSYSPRINT(("Set Tracking\n"));
		double set_t1 = -5;
		double set_pa;
		double set_t2;
		double set_ch = -5;
		static double log_x = IMAGE_WIDTH / 2;
		static double log_y = tracking_v;
//		static double log_y = IMAGE_HEIGHT / 2;

		static double x_cent;
		static double y_cent;

		x_cent = state.self.ballpos_in_image.x;
		y_cent = state.self.ballpos_in_image.y;

//		if(near)	set_t1 = -30;
//		else		set_t1 = -5;
		//tracking = true;
		//jiji
		//set_t1 = tracking_tilt1;

		//*
		if(state.ball.visible)//If Ball Visible
		{
			x_cent = state.self.ballpos_in_image.x + c_x*0.2;
			y_cent = state.self.ballpos_in_image.y + c_y*0.2;
			c_x = state.self.ballpos_in_image.x + c_x*0.2 - IMAGE_WIDTH / 2.0;
			c_y = state.self.ballpos_in_image.y + c_y*0.2 - tracking_v;
			log_x = x_cent;
			log_y = y_cent;
		}
		else if(state.ball.losttime <= BALL_TRACKING_FRAME)//in BALL_TRACKING_FRAME after ball lost
		{
			x_cent = log_x + c_x*0.66;
			y_cent = log_y + c_y*0.66;
			c_x = c_x*0.33;
			c_y = c_y*0.33;
			log_x = x_cent;
			log_y = y_cent;
		}
		else//Ball INVISIBLE
		{
			x_cent = state.self.ballpos_in_image.x;
			y_cent = state.self.ballpos_in_image.y;
			c_x = 0.0;
			c_y = 0.0;
			log_x = x_cent;
			log_y = y_cent;
		}
		//*/
		//OSYSPRINT(("Now Head Angle is tilt1:%f tilt2:%f\n",state.self.sensor.jointValue[TILT1],state.self.sensor.jointValue[TILT2]));

		/*
		lastHead.t1_val = state.self.sensor.jointValue[TILT1];
		lastHead.pa_val = state.self.sensor.jointValue[PAN];
		lastHead.t2_val = state.self.sensor.jointValue[TILT2];
		lastHead.ch_val = state.self.sensor.jointValue[CHIN];
		*/
		pre_t1 = state.self.sensor.jointValue[TILT1];
		pre_pa = state.self.sensor.jointValue[PAN];
		pre_t2 = state.self.sensor.jointValue[TILT2];
		pre_ch = state.self.sensor.jointValue[CHIN];

		/*
		if(-15.0<pre_pa && pre_pa<15.0){
			tracking_v = IMAGE_HEIGHT/2;
		}
		*/

		double neck_coef = 1 - 0.4/10000*(state.self.cdt[ORANGE].value+1);
		if(neck_coef < 0.2)neck_coef = 0.2;
		//OSYSPRINT(("cdt = %d\n",state.self.cdt[ORANGE].value));
		//横画角56.9deg，縦画角45.2deg
		//set_pa = pre_pa + (-1.0*56.9*(x_cent - (double)IMAGE_WIDTH/2.0)/(double)IMAGE_WIDTH )*neck_coef;
//		set_t2 = pre_t1 + pre_t2 + (-1.0*45.2*(y_cent - (double)IMAGE_HEIGHT/2.0)/(double)IMAGE_HEIGHT);
//		set_t2 = pre_t1 - tracking_tilt1 + pre_t2 + (-1.0*45.2*(y_cent - (double)tracking_v)/(double)IMAGE_HEIGHT)*neck_coef;
//		OSYSPRINT(("pre_t1 = %f, tracking_tilt1 = %f\n",pre_t1,tracking_tilt1));
		//OSYSPRINT(("TrackingData : %d,%d,%f,%f,%f,%f\n",state.self.ballpos_in_image.x,state.self.ballpos_in_image.y,set_pa,set_t2,c_x,c_y));

		//okuzumi wrote
		static double Kp = neck_coef;//0.6;
		static double Ki = 0.0;//0.01;
		static double Kd = 0.0;//0.01;
		
		ball_d  = 8000/radius;		//ボールの半径×カメラからの距離＝約8000（計測による）
		
		phi_pa  = (-1.0*56.9*(x_cent - (double)IMAGE_WIDTH/2.0)/(double)IMAGE_WIDTH)*3.14/180;	//カメラからの角度を算出
		e_pa    = atan(ball_d*sin(phi_pa)/(ball_d*cos(phi_pa)+81.6))*180/3.14;					//首の付け根からの角度に変換
		set_pa  = pre_pa + Kp*e_pa + Ki*e_pa_buf2 + Kd*(e_pa - e_pa_buf1);						//PID制御
		e_pa_buf2 += e_pa;																		//積分値を更新
		e_pa_buf1 = e_pa;																		//微分値用のバッファ
		
		phi_t2  = (-1.0*45.2*(y_cent - (double)tracking_v)/(double)IMAGE_HEIGHT)*3.14/180;
		e_t2    = atan(ball_d*sin(phi_t2)/(ball_d*cos(phi_t2)+81.6))*180/3.14;
		set_t2  = pre_t2 + Kp*e_t2 + Ki*e_t2_buf2 + Kd*(e_t2 - e_t2_buf1);
		e_t2_buf2 += e_t2;
		e_t2_buf1 = e_t2;

		set_t1 = tracking_tilt1;

		set = true;
		frame_count = 0;

		//値の範囲外だとセットしなおし&積分値を巻き戻す
		//                              ↑いらね
		if(set_pa > 88.0)
		{
			//c_x += (set_pa - 88.0)*(double)IMAGE_WIDTH/56.9;
			set_pa = 88.0;
		}
		if(set_pa < -88.0)
		{
			//c_x += (set_pa + 88.0)*(double)IMAGE_WIDTH/56.9;
			set_pa = -88.0;
		}
		if(set_t2 + set_t1 > 0)
		{
			//c_y += (set_t2 - 45.0)*(double)IMAGE_HEIGHT/45.2;
			set_t2 = 0.0 - set_t1;
		}
		if(set_t2 < -20)
		{
			set_t2 = -20.0;
		}

		//OSYSPRINT(("e = %.1f, p2 = %.1f, s2 = %.1f, p1 = %.1f, s1 = %.1f, c = %.1f\n",e_t2,pre_t2,set_t2,pre_t1,set_t1,neck_coef));

//		if(set_t2 < -20)
//		{
//			//c_y += (set_t2 + 45.0)*(double)IMAGE_HEIGHT/45.2;
//			set_t1 = set_t2 - (-20.0);
//			set_t2 = -20.0; 
//		}

		del_pa = (set_pa - pre_pa)/frame_num;
		del_t1 = (set_t1 - pre_t1)/frame_num;
		del_t2 = (set_t2 - pre_t2)/frame_num;
		del_ch = (set_ch - pre_ch)/frame_num;
	}
	
	//jiji modified
	tilt1 = del_t1 + pre_t1;
//	tilt1 = tracking_tilt1;
	pan   = del_pa + pre_pa;
	tilt2 = del_t2 + pre_t2;
	chin  = del_ch + pre_ch;

	//OSYSPRINT(("Set Angle: %f,%f,%f\n",pan,tilt2,tilt1));
	//OSYSPRINT(("pre_t2 = %f, set_t2 = %f, e_t2 = %f\n",pre_t2,set_t2,e_t2));
/*	
	static Logger logger("./MS/TILTLOG.TXT");
	logger.Register();
	char strbuf[256];
	sprintf(strbuf,"%.1f,%.1f,%.1f\n",ball_d,tilt2,pan);
	logger.WriteString(strbuf);
*/
//	counter++;
//	if(counter == 1000){
//		logger.Flush();
//	}
//	OSYSPRINT(("count = %d\n",counter));


	pre_t1 += del_t1;
	pre_pa += del_pa;
	pre_t2 += del_t2;
	pre_ch += del_ch;

	frame_count++;

	if(frame_count == (int)frame_num)
	{
		del_t1 = 0;
		del_t2 = 0;
		del_pa = 0;
		del_ch = 0;
		set = false;
	}
}

void HeadController::SetHeadMode(HEAD_MODE h_mode){
	if(h_mode == HEAD_HEADING_LEFT || h_mode == HEAD_HEADING_RIGHT){
		if(!start_heading){
			last_mode = mode;
			start_heading = true;
		}
		hold_state = HOLD_NOT;
	}else if(h_mode == HEAD_BALL_HOLDING){
		if(!start_heading){
			if(hold_state == HOLD_NOT){
//				hold_state = HOLD_ING;
				if(mode != HEAD_BALL_HOLDING)
					last_mode = mode;
				start_heading = true;
			}
		}
	}else{
		if(start_heading)
			return;
		hold_state = HOLD_NOT;
	}
	if(h_mode < NUM_HEAD_MODE){
		mode = h_mode;
		automode = true;
		finished = true;
	}
}

void HeadController::SwingingForLocalization(double& tilt1, double& pan, double& tilt2, double& chin){
        static int current = 0;
		static int timer = 0;
		const int StayHeadAngLocalization[6][3] ={
					{ 0, -90,15},
					{ 0, 0, 15},
					{ 0, 90,15},
					{ 0, 90, -20},
					{ 0, 0 , -20},
					{ 0, -90,-20}
				};
		const int StayHeadTimeLocalization[6] = {
			300,300,100,300,300,100
		};

		int next = (current+1)%6;
		int interval = 10;
		interval = StayHeadTimeLocalization[current];
		
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAngLocalization[current][0] + ratio*StayHeadAngLocalization[next][0];
		pan   = (1 - ratio) * StayHeadAngLocalization[current][1] + ratio*StayHeadAngLocalization[next][1];
		tilt2 = (1 - ratio) * StayHeadAngLocalization[current][2] + ratio*StayHeadAngLocalization[next][2];
		chin  = 0;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 6)
				current = 0;
		}
}

void HeadController::SwingingForBallSearch(double& tilt1, double& pan, double& tilt2, double& chin){
		//OSYSPRINT(("HEAD_SWING_FOR_BALLSEARCH\n"));
        static int current = 0;
		static int timer = 0;
		const int StayHeadAngBallSearch[6][3] ={
					{ -2, -80,   5},
					{ -2,   0,   5},
					{ -2,  80,   5},
					{  0,  60, -25},
					{-20,   0, -20},
					{  0, -80, -25}
		/*			{ -2, -90,   5},
					{ -2,   0,   5},
					{ -2,  90,   5},
					{  0,  90, -25},
					{-20,   0, -20},
					{  0, -90, -25}*/
				};
		const int StayHeadTimeBallSearch[6] = {
			300,300,30,200,200,30//500,500,50,300,300,50
		};

		int next = (current+1)%6;
		int interval = 10;
		interval = StayHeadTimeBallSearch[current];
		
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAngBallSearch[current][0] + ratio*StayHeadAngBallSearch[next][0];
		pan   = (1 - ratio) * StayHeadAngBallSearch[current][1] + ratio*StayHeadAngBallSearch[next][1];
		tilt2 = (1 - ratio) * StayHeadAngBallSearch[current][2] + ratio*StayHeadAngBallSearch[next][2];
		chin  = 0;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 6)
				current = 0;
		}
}

void HeadController::SwingingForBallSearchFast(double& tilt1, double& pan, double& tilt2, double& chin){
		//OSYSPRINT(("HEAD_SWING_FOR_BALLSEARCH\n"));
        static int current = 0;
		static int timer = 0;
		const int StayHeadAngBallSearch[6][3] ={
					{ -2, -90,   5},
					{ -2,   0,   5},
					{ -2,  90,   5},
					{  0,  90, -25},
					{-20,   0, -20},
					{  0, -90, -25}
				};
		const int StayHeadTimeBallSearch[6] = {
			250,250,25,150,150,25
		};

		int next = (current+1)%6;
		int interval = 10;
		interval = StayHeadTimeBallSearch[current];
		
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAngBallSearch[current][0] + ratio*StayHeadAngBallSearch[next][0];
		pan   = (1 - ratio) * StayHeadAngBallSearch[current][1] + ratio*StayHeadAngBallSearch[next][1];
		tilt2 = (1 - ratio) * StayHeadAngBallSearch[current][2] + ratio*StayHeadAngBallSearch[next][2];
		chin  = 0;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 6)
				current = 0;
		}
}

void HeadController::SwingingForBallSearchKP(double& tilt1, double& pan, double& tilt2, double& chin){
        static int current = 0;
		static int timer = 0;
		const int StayHeadAngBallSearch[6][3] ={
					{ -5, -90, 5},
					{ -2, 0,   5},
					{ -5, 90,  5},
					{ -15, 90,-30},
					{ -25, 0 , -30},
					{ -15, -90,-30}
				};
		const int StayHeadTimeBallSearch[6] = {
			300,300,40,220,220,40
		};

		int next = (current+1)%6;
		int interval = 10;
		interval = StayHeadTimeBallSearch[current];
		
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAngBallSearch[current][0] + ratio*StayHeadAngBallSearch[next][0];
		pan   = (1 - ratio) * StayHeadAngBallSearch[current][1] + ratio*StayHeadAngBallSearch[next][1];
		tilt2 = (1 - ratio) * StayHeadAngBallSearch[current][2] + ratio*StayHeadAngBallSearch[next][2];
		chin  = 0;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 6)
				current = 0;
		}
}


void HeadController::UshiKuning(double& tilt1, double& pan, double& tilt2, double& chin){
        static int current = 0;
		static int timer = 0;
		const int StayHeadAng[2][3] ={
					{ 0, -10,45},
					{ 0,  10, 45},
				};
		const int StayHeadTime[2] = {
			60,60
		};

		int next = (current+1)%2;
		int interval = StayHeadTime[current];
		
		double ratio = (double)timer / interval;

		tilt1 = (1 - ratio) * StayHeadAng[current][0] + ratio*StayHeadAng[next][0];
		pan   = (1 - ratio) * StayHeadAng[current][1] + ratio*StayHeadAng[next][1];
		tilt2 = (1 - ratio) * StayHeadAng[current][2] + ratio*StayHeadAng[next][2];
		chin  = -55;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 2)
				current = 0;
		}
}

void HeadController::Watching(double& tilt1, double& pan, double& tilt2, double& chin){
    static int current = 0;
	static int timer = 0;
	static bool set = false;
	static int track_t1 = 0;
	static int track_pa = 0;
	static int track_t2 = 0;

	if(!set){
        track_t1 = (int)state.self.sensor.jointValue[TILT1];
		track_pa = (int)state.self.sensor.jointValue[PAN];
		track_t2 = (int)state.self.sensor.jointValue[TILT2];
		set = true;
	}

	int StayHeadAng[3][3] ={
				{ track_t1, track_pa,track_t2},
				{ 0, -80, 0},
				{ 0, 80, 0}
			};
	const int StayHeadTime[3] = {
		150,300,150
	};

	int next = (current+1)%3;
	int interval = StayHeadTime[current];
		
	double ratio = (double)timer / interval;

	tilt1 = (1 - ratio) * StayHeadAng[current][0] + ratio*StayHeadAng[next][0];
	pan   = (1 - ratio) * StayHeadAng[current][1] + ratio*StayHeadAng[next][1];
	tilt2 = (1 - ratio) * StayHeadAng[current][2] + ratio*StayHeadAng[next][2];
	chin  = 0;

	timer += 8;
	if(timer > interval){
		timer -= interval;
		current++;
		if(current >= 3){
			current = 0;
			set = false;
		}
	}
}


void HeadController::BallHolding(double& tilt1, double& pan, double& tilt2, double& chin){
    static int current = 0;
	static int timer = 0;
	static int chestcounter = 0;

	/*
	static int c = 0;
	c++;
	if(c++ >= 4){
		OSYSPRINT(("cur %d, time %d, PSD %.1f, cnt %d, chin %d\n",current, timer , state.self.sensor.chestPSD/100,chestcounter,
			state.self.sensor.chinTouch));
		c = 0;
	}
	*/

	if(hold_state == HOLD_NOT){
		hold_state = HOLD_ING;
		current = 0;
		timer = 0;
		chestcounter = 0;
		PresetWalkParam::walkparams[SMALL_FORWARD_FORELEG].legparam[0].z = 25;
		PresetWalkParam::walkparams[SMALL_FORWARD_FORELEG].legparam[1].z = 25;
	}

//	OSYSPRINT(("hold_state %d, current %d\n",hold_state,current));

	const int StayHeadAng[5][4] ={
				{ -10,   0,   10,0},
				{ -10,   0,   10,0},
				{ -80,   0,   50,-50},
				{ -75,   0,   50,-50},
				{ -58,   0,   50,-50},
			};
	const int StayHeadTime[5] = {
		400,500,500,500,100
	};

	int next = (current+1)%5;
	int interval = StayHeadTime[current];
	
	if(hold_state == HOLD_OK){
		tilt1 = StayHeadAng[4][0];
		pan   = StayHeadAng[4][1];
		tilt2 = StayHeadAng[4][2];
		chin  = StayHeadAng[4][3];
	}else{
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAng[current][0] + ratio*StayHeadAng[next][0];
		pan   = (1 - ratio) * StayHeadAng[current][1] + ratio*StayHeadAng[next][1];
		tilt2 = (1 - ratio) * StayHeadAng[current][2] + ratio*StayHeadAng[next][2];
		chin  = (1 - ratio) * StayHeadAng[current][3] + ratio*StayHeadAng[next][3];
	}

	timer += 8;


	if(current >= 1){
		PresetWalkParam::walkparams[SMALL_FORWARD_FORELEG].legparam[0].z = 15;
		PresetWalkParam::walkparams[SMALL_FORWARD_FORELEG].legparam[1].z = 15;

		if(state.ball.visible && state.ball.distance < 400){
			hold_state = HOLD_NOT;
			current = 0;
			mode = last_mode;
			start_heading = false;
			timer = 0;
			chestcounter = 0;
		}
	}

	if(current >= 2){
		if(state.self.sensor.chestPSD / 100 < 1005){
			chestcounter++;
			if(chestcounter >= 1){
				hold_state = HOLD_NOT;
				current = 0;
				mode = last_mode;
				start_heading = false;
				timer = 0;
				chestcounter = 0;
			}
		}else{
			chestcounter = 0;
		}
	}else{
		chestcounter = 0;
	}


	if(timer > interval){
		timer -= interval;
		current++;

		if(current >= 3){
			current = 0;
//			mode = last_mode;
			start_heading = false;
			timer = 0;
			hold_state = HOLD_OK;
		}
	}
/*
    static int current = 0;
	static int timer = 0;
	static bool reset = true;
	int now_t1;
	int now_t2;
	int now_ch;

	const int StayHeadAng[4][4] ={
				{ -40, 0, 50, -5},
				{ -55, 0, 50, -20},
				{ -70, 0, 50, -55},
				{ -70, 0, 50, -55},
			};

	const int StayHeadTime[4] = {
		200,200,100,2000
	};

	if(reset){
		current = 0;
		timer = 0;
		now_t1 = (int)state.self.sensor.jointValue[TILT1];
		now_t2 = (int)state.self.sensor.jointValue[TILT2];
		now_ch = (int)state.self.sensor.jointValue[CHIN];
		reset = false;
	}

	int next = (current+1)%4;
	int interval = StayHeadTime[current];
	
	double ratio = (double)timer / interval;

	if(current == 0){
		tilt1 = (1 - ratio) * now_t1 + ratio*StayHeadAng[current][0];
		pan   = 0;//(1 - ratio) * StayHeadAng[current][1] + ratio*StayHeadAng[next][1];
		tilt2 = (1 - ratio) * now_t2 + ratio*StayHeadAng[current][2];
		chin  = (1 - ratio) * now_ch + ratio*StayHeadAng[current][3];
	}
	else{
		tilt1 = (1 - ratio) * StayHeadAng[current-1][0] + ratio*StayHeadAng[next-1][0];
		pan   = 0;//(1 - ratio) * StayHeadAng[current][1] + ratio*StayHeadAng[next][1];
		tilt2 = (1 - ratio) * StayHeadAng[current-1][2] + ratio*StayHeadAng[next-1][2];
		chin  = (1 - ratio) * StayHeadAng[current-1][3] + ratio*StayHeadAng[next-1][3];
	}

	timer += 8;
	if(timer > interval){
		timer -= interval;
		current++;
		if(current >= 3)
			reset = true;
	}
*/
}

void HeadController::SwingingLeft(double& tilt1, double& pan, double& tilt2, double& chin){
		//OSYSPRINT(("HEAD_SWING_LEFT\n"));
        static int current = 0;
		static int timer = 0;
/*
		const int StayHeadAngSwingingLeft[6][3] ={
					{  0, -90, -20},
					{-20,   0, -20},
					{  0,  90,  -5},
					{  0,  90,  10},
					{  0,   0,   5},
					{  0, -90,   5}
				};
		const int StayHeadTimeSwingingLeft[6] = {
			350,350,75,180,270,150//200,200,50,120,180,100
		};
*/
		const int StayHeadAngSwingingLeft[6][3] ={
					{  0, -30, -20},
					{-20,   0, -20},
					{  0,  30,  -5},
					{  0,  30,  10},
					{  0,   0,   5},
					{  0, -30,   5}
				};
		const int StayHeadTimeSwingingLeft[6] = {
			150,150,75,60,90,150//200,200,50,120,180,100
		};

		int next = (current+1)%6;
		int interval = StayHeadTimeSwingingLeft[current];
		
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAngSwingingLeft[current][0] + ratio*StayHeadAngSwingingLeft[next][0];
		pan   = (1 - ratio) * StayHeadAngSwingingLeft[current][1] + ratio*StayHeadAngSwingingLeft[next][1];
		tilt2 = (1 - ratio) * StayHeadAngSwingingLeft[current][2] + ratio*StayHeadAngSwingingLeft[next][2];
		chin  = 0;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 6)
				current = 0;
		}
}

void HeadController::SwingingRight(double& tilt1, double& pan, double& tilt2, double& chin){
		//OSYSPRINT(("HEAD_SWING_RIGHT\n"));
        static int current = 0;
		static int timer = 0;
/*
		const int StayHeadAngSwingingRight[6][3] ={
					{  0,  90, -20},
					{-20,   0, -20},
					{  0, -90,  -5},
					{  0, -90,  10},
					{  0,   0,   5},
					{  0,  90,   5}
				};
		const int StayHeadTimeSwingingRight[6] = {
			350,350,75,180,270,150//200,200,50,120,180,100
		};
*/
		const int StayHeadAngSwingingRight[6][3] ={
					{  0,  30, -20},
					{-20,   0, -20},
					{  0, -30,  -5},
					{  0, -30,  10},
					{  0,   0,   5},
					{  0,  30,   5}
				};
		const int StayHeadTimeSwingingRight[6] = {
			150,150,75,60,90,150//200,200,50,120,180,100
		};

		int next = (current+1)%6;
		int interval = StayHeadTimeSwingingRight[current];
		
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAngSwingingRight[current][0] + ratio*StayHeadAngSwingingRight[next][0];
		pan   = (1 - ratio) * StayHeadAngSwingingRight[current][1] + ratio*StayHeadAngSwingingRight[next][1];
		tilt2 = (1 - ratio) * StayHeadAngSwingingRight[current][2] + ratio*StayHeadAngSwingingRight[next][2];
		chin  = 0;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 6)
				current = 0;
		}
}

void HeadController::SwingingEight(double& tilt1, double& pan, double& tilt2, double& chin){
        static int current = 0;
		static int timer = 0;
		const int StayHeadAngSwingingEight[8][3] ={
					{   0,   0,   0},
					{   0,  90, -20},
					{ -20,   0, -20},
					{   0, -90, -20},
					{   0,   0,   0},
					{   0,  90,  15},
					{   0,   0,  15},
					{   0, -90,  15}
				};
		const int StayHeadTimeSwingingEight[8] = {
			230,220,220,230,250,300,300,250
		};

		int next = (current+1)%8;
		int interval = StayHeadTimeSwingingEight[current];
		
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAngSwingingEight[current][0] + ratio*StayHeadAngSwingingEight[next][0];
		pan   = (1 - ratio) * StayHeadAngSwingingEight[current][1] + ratio*StayHeadAngSwingingEight[next][1];
		tilt2 = (1 - ratio) * StayHeadAngSwingingEight[current][2] + ratio*StayHeadAngSwingingEight[next][2];
		chin  = 0;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 8)
				current = 0;
		}
}

void HeadController::WatchingGround(double& tilt1, double& pan, double& tilt2, double& chin){
        static int current = 0;
		static int timer = 0;
		const int StayHeadAngWatchingGround[2][3] ={
					{-20,   0, -20},
					{-20,   0, -20},
				};
		const int StayHeadTimeWatchingGround[2] = {
			200,200,
		};

		int next = (current+1)%2;
		int interval = StayHeadTimeWatchingGround[current];
		
		double ratio = (double)timer / interval;
		tilt1 = (1 - ratio) * StayHeadAngWatchingGround[current][0] + ratio*StayHeadAngWatchingGround[next][0];
		pan   = (1 - ratio) * StayHeadAngWatchingGround[current][1] + ratio*StayHeadAngWatchingGround[next][1];
		tilt2 = (1 - ratio) * StayHeadAngWatchingGround[current][2] + ratio*StayHeadAngWatchingGround[next][2];
		chin  = 0;

		timer += 8;
		if(timer > interval){
			timer -= interval;
			current++;
			if(current >= 2)
				current = 0;
		}
}

void HeadController::SwingingHeading(double& tilt1, double& pan, double& tilt2, double& chin){
    static int current = 0;
	static int timer = 0;
	const int StayHeadAngHeadingLeft[5][3] ={
				{ -30,   0,   20},
				{ -30,  50,   20},
				{ -90,  50,   20},
				{ -90, -50,   20},
				{ -30,   0,   20},
			};
	const int StayHeadAngHeadingRight[5][3] ={
				{ -30,   0,   20},
				{ -30,  50,   20},
				{ -90,  50,   20},
				{ -90, -50,   20},
				{ -30,   0,   20},
			};
	const int StayHeadTimeHeading[5] = {
		150,150,300,200,100
	};

	int next = (current+1)%8;
	int interval = StayHeadTimeHeading[current];
	
	double ratio = (double)timer / interval;
	if(mode = HEAD_HEADING_RIGHT){
		tilt1 = (1 - ratio) * StayHeadAngHeadingRight[current][0] + ratio*StayHeadAngHeadingRight[next][0];
		pan   = (1 - ratio) * StayHeadAngHeadingRight[current][1] + ratio*StayHeadAngHeadingRight[next][1];
		tilt2 = (1 - ratio) * StayHeadAngHeadingRight[current][2] + ratio*StayHeadAngHeadingRight[next][2];
		chin  = 0;
	}else{
		tilt1 = (1 - ratio) * StayHeadAngHeadingLeft[current][0] + ratio*StayHeadAngHeadingLeft[next][0];
		pan   = (1 - ratio) * StayHeadAngHeadingLeft[current][1] + ratio*StayHeadAngHeadingLeft[next][1];
		tilt2 = (1 - ratio) * StayHeadAngHeadingLeft[current][2] + ratio*StayHeadAngHeadingLeft[next][2];
		chin  = 0;
	}

	timer += 8;
	if(timer > interval){
		timer -= interval;
		current++;
		if(current >= 5){
			current = 0;
			mode = last_mode;
			start_heading = false;
			timer = 0;
		}
	}
}
