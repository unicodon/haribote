#include "BehaviorList.h"

#include <OPENR/OSyslog.h>


//���C���ۂ̔���
void BhvJudgeNearLine::Execute(){

	double position_x = state.self.position.x.val;
	double position_y = state.self.position.y.val;
	double position_tt= state.self.position.tt.val;

		//�����S�[�����C����
		if(position_x<-2200 && (position_tt>90 || position_tt<-90)){
			bhvBallApproach.Execute();
		}

		//����S�[�����C����
		else if(position_x> 2200 && (position_tt<90 || position_tt>-90)){
			bhvBallApproach.Execute();
		}
		
		//�����C����
		else if(position_y>1300 && position_tt>0){
			bhvBallApproach.Execute();
		}
		
		//�E���C����
		else if(position_y>1300 && position_tt>0){
			bhvBallApproach.Execute();
		}

		//Direct��
		else{
			bhvBallAppDirect.Execute();
		}
}
