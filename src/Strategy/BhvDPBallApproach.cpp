#include "StdAfx.h"
#include "BehaviorList.h"
#include <math.h>
#include "../Motion/GaitMaker.h"
#include "../Utility/StopWatch.h"

#include <OPENR/OSyslog.h>

#define DELTA_X  50

//歩行の実行時間 speed_time
const int speed_time = 12;//20;

const int x_elem = 30;//21;
const int y_elem = 20;//15;
const int theta_elem = 18;
const int r_elem = 10;//9;//10;
const int phi_elem = 18;//22;//15;
const int rtp_elem = r_elem*theta_elem*phi_elem;
const int full_elem = rtp_elem*x_elem*y_elem;

const int x_wid = 6000/x_elem;
const int y_wid = 4000/y_elem;
const int t_wid = 360/theta_elem;

static int x_under[x_elem+1];
static int y_under[y_elem+1];
static int theta_under[theta_elem+1];
static int r_under[r_elem+1];
static int phi_under[phi_elem+1];

static DPMapLocalApproach dpmla;
unsigned char* BhvDPBallApproach::m_map;
bool BhvDPBallApproach::load_map = false;
static bool ball_near = false;//最後のボール接近用のフラグ

const int conv[]
={	0,

	//2006_0429
	WALK_FORWARD,// 139.77 11.96 -0.30 300
	WALK_BACK,// -104.87 6.22 0.63 300
	WALK_LEFT15,// 132.11 27.13 -1.00 300
	WALK_LEFT30,// 122.00 67.67 0.00 300
	WALK_LEFT60,// 66.22 98.86 -0.10 300
	WALK_LEFT90,// 7.26 117.84 0.57 300
	WALK_LEFT120,// -40.23 100.42 0.70 300
	WALK_LEFT150,// -107.42 85.28 0.83 300
	WALK_RIGHT15,// 131.99 -45.85 -0.67 300
	WALK_RIGHT30,// 126.80 -77.61 1.00 300
	WALK_RIGHT60,// 75.90 -112.84 -0.43 300
	WALK_RIGHT90,// -9.65 -112.15 0.30 300
	WALK_RIGHT120,// -47.22 -104.50 -0.47 300
	WALK_RIGHT150,// -122.18 -52.76 0.20 300
	WALK_ROLL_LEFT10,// -4.17 50.77 -26.17 300
	WALK_ROLL_LEFT20,// 0.48 67.19 -18.25 300
	WALK_ROLL_LEFT40,// 6.84 72.00 -10.00 300
	WALK_ROLL_LEFT80,// 9.83 105.12 -4.00 300
	WALK_ROLL_RIGHT10,// -4.22 -69.45 31.75 300
	WALK_ROLL_RIGHT20,// 7.99 -66.86 21.00 300
	WALK_ROLL_RIGHT40,// -5.34 -74.05 9.75 300
	WALK_ROLL_RIGHT80,// -6.36 -100.41 7.50 300
	WALK_ROTATE_LEFT90,// 9.37 1.59 11.25 300
	WALK_ROTATE_LEFT180,// 0.12 26.70 22.00 300
	WALK_ROTATE_LEFT270,// 2.07 19.44 29.25 300
	WALK_ROTATE_RIGHT90,// 2.91 -9.04 -13.17 300
	WALK_ROTATE_RIGHT180,// -11.18 -20.27 -22.00 300
	WALK_ROTATE_RIGHT270,// 1.11 -28.17 -30.50 300
	WALK_TURN_LEFT20,// 76.44 18.89 20.50 300
	WALK_TURN_LEFT40,// 115.36 48.28 16.75 300
	WALK_TURN_LEFT80,// 137.97 26.95 10.00 300
	WALK_TURN_RIGHT20,// 74.42 -30.07 -17.00 300
	WALK_TURN_RIGHT40,// 118.59 -47.23 -16.17 300
	WALK_TURN_RIGHT80,// 137.12 -20.73 -10.50 300

	WALK_SMALL10_RIGHT90,// 0 -50 0 200
	WALK_SMALL10_RIGHT80,// 8.6 -49 0 200
	WALK_SMALL10_RIGHT70,// 17.1 -46.8 0 200
	WALK_SMALL10_RIGHT60,// 25 -43.3 0 200
	WALK_SMALL10_RIGHT50,// 32.1 -38.3 0 200
	WALK_SMALL10_RIGHT40,// 38.3 -32.1 0 200
	WALK_SMALL10_RIGHT30,// 43.3 -25 0 200
	WALK_SMALL10_RIGHT20,// 46.8 -17.1 0 200
	WALK_SMALL10_RIGHT10,// 49.2 -8.6 0 200
	WALK_SMALL10_FORWARD,// 50 0 0 200
	WALK_SMALL10_LEFT10,// 49.2 8.6 0 200
	WALK_SMALL10_LEFT20,// 46.8 17.1 0 200
	WALK_SMALL10_LEFT30,// 43.3 25 0 200
	WALK_SMALL10_LEFT40,// 38.3 32.1 0 200
	WALK_SMALL10_LEFT50,// 32.1 38.3 0 200
	WALK_SMALL10_LEFT60,// 25 43.3 0 200
	WALK_SMALL10_LEFT70,// 17.1 46.8 0 200
	WALK_SMALL10_LEFT80,// 8.6 49 0 200
	WALK_SMALL10_LEFT90,// 0 50 0 200

/*
//高速な歩行だけver.
	WALK_FORWARD,// 134.5 -2.75 -0.15 300
	WALK_BACK,// -103.5 1.75 -0.9 280
	WALK_LEFT15,// 119.0 29.5 -1.2 300
	WALK_LEFT30,// 101.75 70.0 0.3 300
	WALK_LEFT90,// 2.0 78.0 0 270
	WALK_LEFT120,// -49.5 87.5 -0.3 270
	WALK_LEFT150,// -103.0 52.0 0.3 270
	WALK_RIGHT15,// 115.75 -36.25 -0.15 290
	WALK_RIGHT30,// 104.5 -63.5 0 300
	WALK_RIGHT90,// 2.5 -77.75 0.45 270
	WALK_RIGHT150,// -84.25 -56.0 -0.15 270
	WALK_TURN_LEFT40,// 29.75 75.75 12.0 270
	WALK_TURN_LEFT80,// 79.0 67.5 7.2 280
	WALK_TURN_RIGHT40,// 35.5 -50.0 -9.0 260
	WALK_TURN_RIGHT80,// 84.5 -73.75 -7.5 290
	WALK_ROTATE_LEFT90,// -1.75 4.25 10.35 360
	WALK_ROTATE_LEFT180,// -4.75 -13.5 20.2 400
	WALK_ROTATE_RIGHT180,// -8.85 5.1 -20.55 370
	*/
};

const int used_action_num = sizeof(conv)/sizeof(conv[0]);

BhvDPBallApproach::BhvDPBallApproach(){

	//地図の読み込み
	if(!load_map){
		//状態変数の離散化
		int i;
		for(i=0;i<=x_elem;i++)		x_under[i] = x_wid*i -3000;
		for(i=0;i<=y_elem;i++)		y_under[i] = y_wid*i -2000;
		for(i=0;i<=theta_elem;i++)	theta_under[i] = 20*i -180;
		for(i=0;i<=phi_elem;i++)	phi_under[i] = 10*i -90;//110;
		
		r_under[0] = 150;//300;//150;
		r_under[1] = 200;//250;//400;//250;
		r_under[2] = 275;//350;//500;//350;
		r_under[3] = 350;//500;//600;//450;
		r_under[4] = 450;//700;//750;//650;
		r_under[5] = 600;//900;//900;//950;
		r_under[6] = 850;//1200;//1300;//1350;
		r_under[7] = 1200;//1600;//1800;//1850;
		r_under[8] = 1600;//2200;//2400;//2450;
		r_under[9] = 2200;//3000;//3150
		r_under[10] = 3000;

		m_map = new unsigned char [full_elem];
		ifstream ifs("./MS/forward.pol",ios::binary);
		ifs.read((char*)m_map,full_elem*sizeof(unsigned char));
		ifs.close();

		load_map = true;
	}
}


BhvDPBallApproach::~BhvDPBallApproach()
{
}

void BhvDPBallApproach::Execute(){
	//ボールが見えてなかったら，入らない
	if(!state.ball.visible && state.ball.losttime > 10)	return;

	int walk = 0;

	double pos_x = state.self.position.x.val;
	if(pos_x > 2700)	pos_x = 2700;
	if(pos_x < -2700)	pos_x = -2700;
	double pos_y = state.self.position.y.val;
	double pos_tt = state.self.position.tt.val;
	double ball_dist;
	double ball_drct;

	//ボールまでの距離・方向を微調整．前足の根元から，胸元へ.距離はDELTA＿Xだけ下駄をはかす
	double leg_ball_x = state.ball.x;
	double leg_ball_y = state.ball.y;
	double chest_ball_x = leg_ball_x - DELTA_X;
	double chest_ball_y = leg_ball_y;
	double chest_ball_dist = sqrt(chest_ball_x*chest_ball_x + chest_ball_y*chest_ball_y);
	double chest_ball_drct = atan2(chest_ball_y, chest_ball_x)*180/PI;
	ball_dist = state.ball.distance - DELTA_X;//chest_ball_dist;//state.ball.distance;//chest_ball_dist;
	ball_drct = chest_ball_drct;//state.ball.direction;//chest_ball_drct;

	//チョコ歩きで近づく範囲．それ以外は，SAMで接近
	if( ball_near && ball_dist <= 350 && fabs(ball_drct) < 45.0){
		//SetPresetWalk(0);
		//SetHeadMode(HEAD_TRACK);
		//SetFaceLED(FACE_SMILE);
		//bhvDrible.Execute();
		bhvLastApproach.Execute();
		return;
	}
	else{
		ball_near = false;
	}
	/*else if(ball_dist < 450){
		unsigned char pol = dpmla.get_action((int)leg_ball_x-30, (int)leg_ball_y, 0);
		SetPresetWalk(pol);
		SetHeadMode(HEAD_TRACK);
		return;
	}*/

	//ボールよりある程度遠いときは珠に首振ってローカライゼーションを行う
	static int cnt_swing = 0;
	if(ball_dist > 1250){
		cnt_swing++;
		if(cnt_swing > 130){
			SetHeadMode(HEAD_WATCHING);
			if(cnt_swing >= 145)	cnt_swing = 0;
		}
		else	SetHeadMode(HEAD_TRACK);
	}
	else{
		SetHeadMode(HEAD_TRACK);
		cnt_swing = 0;
	}
	SetFaceLED(FACE_DP);

//	bhvSearchBall.Initialize();

	/*if(fabs(state::self.head.joint.pan) < 75 && state::ball.position.r.val < 400.0 &&
		state::self.cdt[ORANGE].position_x < 40 || state::self.cdt[ORANGE].position_x > 136)
	{
		WalkController::MakeWalkingCommand(STATE_StopWalk,1,16);
		Strategy::m_nPrevBhv = m_nMyId;
		return;
	}*/

	/*
	if(ball_dist > r_under[r_elem]){//2000){
		double pan = state.self.sensor.jointValue[PAN];
		if(pan < -50){
			walk = WALK_HALFROTATE_RIGHT;//WALK_HALFROTATE_RIGHT;//
		}else if(pan < -35){
			walk = WALK_ROLL_RIGHT_R20;//WALK_HALFROTATE_RIGHT;//
		}else if(pan < -20){
			walk = WALK_ROLL_RIGHT_R40;
		}else if(pan < 20){
			walk = WALK_STABLE_FORWARD;
		}else if(pan < 35){
			walk = WALK_ROLL_LEFT_R40;
		}else if(pan < 50){
			walk = WALK_ROLL_LEFT_R20;//WALK_HALFROTATE_LEFT;//
		}else{
			walk = WALK_HALFROTATE_LEFT;//WALK_HALFROTATE_LEFT;//
		}

		SetPresetWalk(walk);
		return;
	}*/

	int x		= x_to_id((int)pos_x);//;state::self.position.x.val);
	int y		= y_to_id((int)pos_y);//;state::self.position.y.val);
	int theta	= theta_to_id((int)pos_tt);//;state::self.position.tt.val);
	int r		= r_to_id((int)ball_dist);//(int)state.ball.distance);//state::ball.position.r.val);
	int phi		= phi_to_id((int)ball_drct);//(int)state.ball.direction);//state::ball.position.tt.val);

	bool flag = false;
	if(x < 0)				x = 0;
	else if(x >= x_elem)	x = x_elem-1;
	if(y < 0)				y = 0;
	else if(y >= y_elem)	y = y_elem-1;
	if(theta < 0)					theta += theta_elem;
	else if(theta >= theta_elem)	theta -= theta_elem;
	if(r < 0)
	{
		r = 0;
		flag = true;
	}
	else if(r >= r_elem)		r = r_elem-1;
	if(phi < 0)					phi = 0;
	else if(phi >= phi_elem)	phi = phi_elem-1;

	unsigned char policy = get_action(x,y,r,phi,theta);

	//BhvDribleに入るフラグ立て
	static char cnt_ball_near = 0;
	if(policy == 0){
		cnt_ball_near++;
		if(cnt_ball_near >= 1){
			ball_near = true;
			//bhvDrible.Execute();
			bhvLastApproach.Execute();
			return;
		}
	}
	else{
		ball_near = false;
		cnt_ball_near = 0;
	}

	//takeshita wrote
	/*static int counter = 0;
	if(int(counter/20) == 0){
		cout << "Now Policy = " << (int)policy << "\n";
		counter = 0;
	}
	if(policy == 0){
		WalkController::MakeWalkingCommand(STATE_StopWalk,1,speed_time);//10);
		return;
	}
	counter++;*/
/*
	if(policy != 0 && flag)// ボールに近すぎるけど終端状態に近くない状態
	{
		WalkController::MakeWalkingCommand(STATE_StableWalkBackward2,1,10);
		Strategy::m_nPrevBhv = m_nMyId;
		return;	
	}
*/

	//jiji
	//ボールが横に見えてたら回転
	if(state.ball.visible){
		if(ball_drct < -90){
			SetPresetWalk(WALK_ROTATE_RIGHT360);
			return;
		}
		if(ball_drct > 90){
			SetPresetWalk(WALK_ROTATE_LEFT360);
			return;
		}
	}

	if(policy == 0)
	{
		int pan = (int)state.self.sensor.jointValue[PAN];//state.ball.direction;//::ball.position.tt.val;

		if(pan < -50)			walk = WALK_ROTATE_RIGHT180;//WALK_HALFROTATE_RIGHT;//WALK_HALFROTATE_RIGHT;//
		else if(pan < -35)		walk = WALK_ROLL_RIGHT20;//WALK_HALFROTATE_RIGHT;//
		else if(pan < -20)		walk = WALK_ROLL_RIGHT40;
		else if(pan < 20)		walk = SMALL_FORWARD;
		else if(pan < 35)		walk = WALK_ROLL_LEFT40;
		else if(pan < 50)		walk = WALK_ROLL_LEFT20;//WALK_HALFROTATE_LEFT;//
		else					walk = WALK_ROTATE_LEFT180;//WALK_HALFROTATE_LEFT;//WALK_HALFROTATE_LEFT;//
//		if(GaitMaker::IsExpiring())
			SetPresetWalk(walk);
		return;
	}

	if(policy >= used_action_num)
	{
		//if(state.self.sensor.jointValue[PAN] > 30.0) walk = WALK_HALFROTATE_LEFT;
		//else if(state.self.sensor.jointValue[PAN] < -30.0) walk = WALK_HALFROTATE_RIGHT;
		//else walk = WALK_STABLE_FORWARD;

		int pan = (int)state.self.sensor.jointValue[PAN];//state.ball.direction;//::ball.position.tt.val;

		if(pan < -50)			walk = WALK_ROTATE_RIGHT180;//WALK_HALFROTATE_RIGHT;//WALK_HALFROTATE_RIGHT;//
		else if(pan < -35)		walk = WALK_ROLL_RIGHT20;//WALK_HALFROTATE_RIGHT;//
		else if(pan < -20)		walk = WALK_ROLL_RIGHT40;
		else if(pan < 20)		walk = WALK_FORWARD;
		else if(pan < 35)		walk = WALK_ROLL_LEFT40;
		else if(pan < 50)		walk = WALK_ROLL_LEFT20;//WALK_HALFROTATE_LEFT;//
		else					walk = WALK_ROTATE_LEFT180;//WALK_HALFROTATE_LEFT;//WALK_HALFROTATE_LEFT;//
//		if(GaitMaker::IsExpiring())
			SetPresetWalk(walk);
		return;
	}

	walk = conv[policy];

	if(GaitMaker::IsExpiring())
		SetPresetWalk(walk);

	return;
}


unsigned char BhvDPBallApproach::get_action(int x,int y,int r,int phi,int theta)
{
	int index = x*y_elem*r_elem*phi_elem*theta_elem +
					   y*r_elem*phi_elem*theta_elem +
							  r*phi_elem*theta_elem +
									 phi*theta_elem + theta;
	return m_map[index];
}

int BhvDPBallApproach::x_to_id(int x){
	if(x < -3000)	return -1;
	return (x + 3000)/x_wid;
}

int BhvDPBallApproach::y_to_id(int y){ 
	if(y < -2000)	return -1;
	return (y + 2000)/y_wid;
}

int BhvDPBallApproach::theta_to_id(int theta){
	return (theta + 180)/t_wid;//20;
}

int BhvDPBallApproach::r_to_id(int r){
	for(int i = 0;i < r_elem + 1;i++){
		if(r < r_under[i])
			return i-1;
	}
	return -1;
}

int BhvDPBallApproach::phi_to_id(int phi){	
	if(phi < -90/*110*/)	return -1;
	return (phi+90/*110*/)/10;
}
