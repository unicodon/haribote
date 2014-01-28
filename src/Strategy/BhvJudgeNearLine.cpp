#include "BehaviorList.h"

#include <OPENR/OSyslog.h>


//ライン際の判定
void BhvJudgeNearLine::Execute(){

	double position_x = state.self.position.x.val;
	double position_y = state.self.position.y.val;
	double position_tt= state.self.position.tt.val;

		//味方ゴールライン際
		if(position_x<-2200 && (position_tt>90 || position_tt<-90)){
			bhvBallApproach.Execute();
		}

		//相手ゴールライン際
		else if(position_x> 2200 && (position_tt<90 || position_tt>-90)){
			bhvBallApproach.Execute();
		}
		
		//左ライン際
		else if(position_y>1300 && position_tt>0){
			bhvBallApproach.Execute();
		}
		
		//右ライン際
		else if(position_y>1300 && position_tt>0){
			bhvBallApproach.Execute();
		}

		//Directへ
		else{
			bhvBallAppDirect.Execute();
		}
}
