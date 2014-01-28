#include "StdAfx.h"
#include "BehaviorList.h"
#include "BhvDPBallApproach.h"
#include "../Motion/GaitMaker.h"

#include <OPENR/OSyslog.h>

static BhvDPBallApproach dpba;
unsigned char* BhvSupport::stt_map;
bool BhvSupport::load_sttmap = false;

BhvSupport::BhvSupport(){
	if(!load_sttmap){
		stt_map = new unsigned char [full_elem];
		ifstream ifs("./MS/STV256.STT",ios::binary);
		ifs.read((char*)stt_map,full_elem*sizeof(unsigned char));
		ifs.close();

		//for(int i=0;i<full_elem/100;i++)	OSYSPRINT(("%d, ", (int) stt_map[i]));

		load_sttmap = true;
	}
}

BhvSupport::~BhvSupport(){
}

void BhvSupport::Execute(){
	static double BALL_DISTANCE = 2500.0;

	int ID = state.game.robot_id;

//	�������{�[�������Ă��ă{�[���ɋ߂��Ƃ� �L�[�p�[�͖���
	if( //state.ball.visible &&
		( //(state.team_mate[1].connected && state.team_mate[1].data.penalty == PENALTY_NONE && state.team_mate[1].data.ball_visible && state.team_mate[1].data.ball_distance < 750) ||
		  (state.team_mate[2].connected && state.team_mate[2].data.penalty == PENALTY_NONE && state.team_mate[2].data.ball_visible && state.team_mate[2].data.ball_distance < BALL_DISTANCE) ||
		  (state.team_mate[3].connected && state.team_mate[3].data.penalty == PENALTY_NONE && state.team_mate[3].data.ball_visible && state.team_mate[3].data.ball_distance < BALL_DISTANCE) ||
		  (state.team_mate[4].connected && state.team_mate[4].data.penalty == PENALTY_NONE && state.team_mate[4].data.ball_visible && state.team_mate[4].data.ball_distance < BALL_DISTANCE) )
	)
	{
		//�ǂ�RobotID��AIBO���{�[���֐ڋ߂���̂� �� �����̖�������
		int hold_id;
		//int hold_id = GetHoldID();//
        GetHoldID(hold_id, my_role); 
		//OSYSPRINT(("HOLD_ID = %d, MY_ROLE = %d\n", hold_id, (int)my_role));
		
		if(hold_id == 0 || hold_id == ID) return;
		//if(hold_id != ID)	SetFaceLED(FACE_SMILE);
		switch(my_role)
		{
			case ROLE_FW:
				SetFaceLED(FACE_FOUR);

			case ROLE_MF:
				SetFaceLED(FACE_THREE);
			
			case ROLE_DF:
				SetFaceLED(FACE_TWO);
		}

		//�{�[���������ĂȂ�������C�܂��̓t�B�[���h����o�Ȃ��悤�ɂ���
		if(!state.ball.visible) bhvLeavingField.Execute();
		
		//RobotID���ƂɃT�|�[�g�֍s���ʒu�̏ꍇ�킯
		int dest_x,dest_y,dest_t;//�T�|�[�g�֍s���ʒu
		GetSupportPosition(ID, my_role, hold_id, dest_x, dest_y, dest_t);

		//���s���s
		if(InSpot(dest_x, dest_y, dest_t)){
			if(state.ball.visible){
				if(GaitMaker::IsExpiring())	SetPresetWalk(WALK_STOP);
				SetHeadMode(HEAD_TRACK);
			}
			else{
				if(GaitMaker::IsExpiring())	SetPresetWalk(WALK_ROTATE_LEFT360);//270);
				static unsigned int cnt_lost = 0;
				if(cnt_lost < 100)	SetHeadMode(HEAD_SWING_LEFT);
				else				SetHeadMode(HEAD_SWING_LOCALIZATION);

				if(cnt_lost > 150) cnt_lost = 0;
				cnt_lost++;
			}
		}
		else{
            bhvMoveToPosDP.SetDestination(dest_x, dest_y, dest_t);
			bhvMoveToPosDP.Execute();
		}
		return;
	}
	else{//�N���{�[�������ĂȂ�������C�{�[���T��
		bhvSearchBall.Execute();
	}
	return;
}


bool BhvSupport::InSpot(int x, int y, int theta){
	return (abs((int)state.self.position.x.val - x) < 300
		 && abs((int)state.self.position.y.val - y) < 300
		 //&& abs((int)state.self.position.tt.val - theta) < 20 �Ƃ͎g������ʖ�
		 );
}


unsigned char BhvSupport::get_stt(int pos_x, int pos_y, int pos_t, int pos_r, int pos_p){
	int x		= dpba.x_to_id(pos_x);
	int y		= dpba.y_to_id(pos_y);
	int theta	= dpba.theta_to_id(pos_t);
	int r		= dpba.r_to_id(pos_r + 300);//�O���[�o��DP�̏I�[��Ԃɂ���ꍇ�̏�ԉ��l���v�Z���邽�߂ɉ���
	int phi		= dpba.phi_to_id(pos_p);

	if(x < 0)						x = 0;					else if(x >= x_elem)			x = x_elem-1;
	if(y < 0)						y = 0;					else if(y >= y_elem)			y = y_elem-1;
	if(theta < 0)					theta += theta_elem;	else if(theta >= theta_elem)	theta -= theta_elem;
	if(r < 0)						r = 0;					else if(r >= r_elem)			r = r_elem-1;
	if(phi < 0)						phi = 0;				else if(phi >= phi_elem)		phi = phi_elem-1;

	int index = x*y_elem*r_elem*phi_elem*theta_elem +
						y*r_elem*phi_elem*theta_elem +
								r*phi_elem*theta_elem +
										phi*theta_elem + theta;

	unsigned char stt_value = stt_map[index];

	/*static int cnt = 0;
	cnt++;
	if(cnt%30 == 0)
		OSYSPRINT(("stt = %d, x = %d, y = %d, t = %d, r = %d, p = %d\n", (int)stt_value, x, y ,theta, r, phi));
	*/
	return stt_value;
}


void BhvSupport::GetHoldID(int &hold_id, SUPPORT_ROLE &my_role){
	static int cnt = 0;
	int ID = state.game.robot_id;
	static int pre_hold_id = 0;
	static int cnt_hold_id = 0;

	//�e���{�b�g�̏�ԉ��l���v�Z
	//int hold_id = 0;
	unsigned char min_stt = 255;
	unsigned char stt_ID[4] = {255,255,255,255};
	bool all_connect = false;//���̃t�B�[���h�v���C���[2��Ƃ��t�B�[���h�ɂ��邩���R�l�N�g���Ă��邩
	int cnt_connect = 0;
	for(int i=2;i<5;i++){
		if(i!=ID 
			&& state.team_mate[i].connected 
			//&& state.team_mate[i].data.ball_visible 
			&& state.team_mate[i].data.penalty == PENALTY_NONE)
		{
			cnt_connect++;

			//�{�[���������Ă��閡�����{�b�g�̏ꍇ
			if(state.team_mate[i].data.ball_visible){
				int pos_x = (int)state.team_mate[i].data.x_val;
				int pos_y = (int)state.team_mate[i].data.y_val;
				int pos_t = (int)state.team_mate[i].data.t_val;
				int pos_r = (int)state.team_mate[i].data.ball_distance;
				int pos_p = (int)state.team_mate[i].data.ball_direction;

				unsigned char now_stt = get_stt(pos_x, pos_y, pos_t, pos_r, pos_p);
				stt_ID[i] = now_stt;

				if(now_stt <= min_stt){
					min_stt = now_stt;
					hold_id = i;
				}
			}
			//�{�[���������Ă��Ȃ��������{�b�g�̏ꍇ
			else{
                stt_ID[i] = 255;
			}
		}
	}
	if(cnt_connect==2) all_connect = true;

	unsigned char my_stt;
	if(state.ball.visible){
		int my_x, my_y, my_t, my_r, my_p;
		if(state.team_mate[ID].connected && state.team_mate[ID].data.ball_visible){//���̃��{�b�g�ƒʐM�̓�������邽��
			my_x = (int)state.team_mate[ID].data.x_val;
			my_y = (int)state.team_mate[ID].data.y_val;
			my_t = (int)state.team_mate[ID].data.t_val;
			my_r = (int)state.team_mate[ID].data.ball_distance;
			my_p = (int)state.team_mate[ID].data.ball_direction;
		}
		else{
			my_x = (int)state.self.position.x.val;
			my_y = (int)state.self.position.y.val;
			my_t = (int)state.self.position.tt.val;
			my_r = (int)state.ball.distance;
			my_p = (int)state.ball.direction;
		}
		my_stt = get_stt(my_x, my_y, my_t, my_r, my_p);//get_stt((int)state.self.position.x.val, (int)state.self.position.y.val, (int)state.self.position.tt.val, (int)state.ball.distance, (int)state.ball.direction);
	}
	else					my_stt = 255;

	//�����̏�ԉ��l�ƁC�������{�b�g�̍ŏ���ԉ��l�������������ꍇ�̏���
	if(my_stt == min_stt)
	{
		if(ID == 4)	hold_id = ID;
		if(ID == 3)
		{
			if(hold_id == 4) hold_id = 4;
			if(hold_id == 2) hold_id = ID;
		}
	}
	else if(my_stt < min_stt)	hold_id = ID;

	//�`�[�����C�g���C�{�[���������ĂȂ��Ă��Ō�̐ڋߒ����ǂ����D�Ō�̐ڋߒ��Ȃ�C������hold_id
	bool last_app = false;
	for(int i=2;i<5;i++)
	{
		if(i!=ID 
			&& state.team_mate[i].connected 
			&& state.team_mate[i].data.penalty == PENALTY_NONE 
			&& state.team_mate[i].data.last_approach){
			hold_id = i;
			//OSYSPRINT(("NOW ID=%d IS LAST APPROACHING\n", hold_id));
		}
	}

	//�����̖�������
	if(hold_id == ID)		my_role = ROLE_FW;
	else if(!all_connect)	my_role = ROLE_DF;
	else{
		for(int i=2;i<5;i++){
			if(	   i!=ID
				&& i!=hold_id
				&& state.team_mate[i].connected 
				&& state.team_mate[i].data.penalty == PENALTY_NONE
			){
				if(my_stt < stt_ID[i])	my_role = ROLE_MF;
				else					my_role = ROLE_DF;
			}
		}
	}

	//5��A������ID��hold_id��������T�|�[�g��
	if(hold_id == pre_hold_id){
		cnt_hold_id++;
		if(cnt_hold_id < 5)	{
			hold_id = 0;
		}
		else{
			return;
		}
	}
	else{
		pre_hold_id = hold_id;
		cnt_hold_id = 0;
		hold_id = 0;//return 0;
	}

}


void BhvSupport::GetSupportPosition(int ID, SUPPORT_ROLE my_role, int hold_id, int &dest_x, int &dest_y, int &dest_t){
	//�{�[���̃t�B�[���h���W
	int ball_x,ball_y;
	if(state.ball.visible){
		ball_x = (int)(state.self.position.x.val + state.ball.x);
		ball_y = (int)(state.self.position.y.val + state.ball.y);
	}
	else{
		ball_x = (int)(state.team_mate[hold_id].data.x_val 
			+ state.team_mate[hold_id].data.ball_distance 
			* cos(state.team_mate[hold_id].data.ball_direction + state.team_mate[hold_id].data.t_val));
		ball_y = (int)(state.team_mate[hold_id].data.y_val 
			+ state.team_mate[hold_id].data.ball_distance 
			* sin(state.team_mate[hold_id].data.ball_direction + state.team_mate[hold_id].data.t_val));
	}

	if(ball_x > 2700)	ball_x = 2700;
	if(ball_x < -2700)	ball_x = -2700;
	if(ball_y > 1800)	ball_y = 1800;
	if(ball_y < -1800)	ball_y = -1800;

	//ID�ɂ��Ȃ��ꍇ�킯�i��ԉ��l����DF,MF�̖�����I�o�j�DMF��DF���U�����Ȃ��悤�ɂ���K�v����I
	/*switch(my_role){
		case ROLE_FW:
			break;

		case ROLE_MF:
			GetMFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
		
		case ROLE_DF:
			GetDFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
	}
	return;*/

	//ID����ɂ����ꍇ�킯�i��ԉ��l��FW�����߂邾���Ɏg�p�DMF,DF��ID����I�o�j
    switch(ID){
		case 1:
			break;

		case 2:
			if(hold_id == 1){//DF
				GetDFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
			}
			else if(hold_id == 3){//DF
				GetDFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
			}
			else{//DF
				GetDFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
			}
			break;

		case 3://all_connect
			if(hold_id == 1){//MF
				GetMFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
			}
			else if(hold_id == 2){//DF
				GetDFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
			}
			else if(hold_id == 4){//��{MF�DID2���t�B�[���h�ɂ��Ȃ�������DF
				if(state.team_mate[2].connected && state.team_mate[2].data.penalty != PENALTY_NONE){//DF
					GetDFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
				}
				else {//MF
					GetMFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
				}
			}
			else{
				GetMFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
			}
			break;

		case 4:
			if(hold_id == 1){//FW
				GetFWPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
			}
			else if(hold_id == 2){//��{MF�DID3���t�B�[���h�ɂ��Ȃ�������DF
				if(state.team_mate[3].connected && state.team_mate[3].data.penalty != PENALTY_NONE){//DF
					GetDFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
				}
				else{//MF
					GetMFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
				}
			}
			else if(hold_id == 3){//��{MF�DID2���t�B�[���h�ɂ��Ȃ�������DF
				if(state.team_mate[2].connected && state.team_mate[2].data.penalty != PENALTY_NONE){//DF
					GetDFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
				}
				else{//MF
					GetMFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
				}
			}
			else{
				GetMFPosition(dest_x, dest_y, dest_t, ball_x, ball_y);
			}
			break;
	}
}


void BhvSupport::GetFWPosition(int &dest_x, int &dest_y, int &dest_t, int ball_x, int ball_y){
	dest_x = 1000;
	dest_y = 0;
	dest_t = 0;    
}


void BhvSupport::GetMFPosition(int &dest_x, int &dest_y, int &dest_t, int ball_x, int ball_y){
	//�{�[���Ƌt�T�C�h�ŏ������
	dest_x = ball_x - 1500;
	dest_y = -(ball_y/2);

	if(dest_x > 2000)	dest_x = 2000;
	if(dest_x < -2000)	dest_x = -2000;
	if(dest_y > 1500)	dest_y = 1500;
	if(dest_y < -1500)	dest_y = -1500;
	dest_t = (int)(atan2(double(ball_y - dest_y), double(ball_x - dest_x)));
}


void BhvSupport::GetDFPosition(int &dest_x, int &dest_y, int &dest_t, int ball_x, int ball_y){
	//�{�[���ƃS�[���̊ԁi���ꂾ�ƃL�[�p�[���{�[�������Ȃ����ꂠ��j�H
	dest_x = -1500;//-1350 + ball_x/2;//(ball_x - 2700)/2;
	dest_y = ball_y/2;

	if(dest_x > 2000)	dest_x = 2000;
	if(dest_x < -2000)	dest_x = -2000;
	if(dest_y > 1500)	dest_y = 1500;
	if(dest_y < -1500)	dest_y = -1500;
	dest_t = (int)(atan2(double(ball_y - dest_y), double(ball_x - dest_x)));
}
