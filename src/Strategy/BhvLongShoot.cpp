#include "StdAfx.h"
#include <OPENR/OSyslog.h>
#include "BehaviorList.h"

void BhvLongShoot::Execute(){
	/*static bool shootflag = 0;
	static int approachcounter = 0;
	if(shootflag){
		approachcounter++;

		SetHeadMode(HEAD_TRACK);
		SetTailMode(TAIL_STOP);
		SetFaceLED(FACE_READY);

		if(approachcounter > 0){
			SetPresetWalk(WALK_STOP);
		}else{
			SetPresetWalk(SMALL_FORWARD);
		}

		if(approachcounter > 300){
//			SetMotion("LONGSHOOT_FRONT_100_150");
			ReleaseCall();
			shootflag = false;
		}
		return;
	}

	if(state.ball.visible){
		if(state.ball.distance > 400){
			bhvBallApproach.Execute();
			return;
		}
		SetHeadMode(HEAD_TRACK);
		SetTailMode(TAIL_STOP);
		SetFaceLED(FACE_READY);
		if(state.ball.direction < -8){
			SetPresetWalk(WALK_FOURTHROTATE_RIGHT);
		}else if(state.ball.direction > 8){
			SetPresetWalk(WALK_FOURTHROTATE_LEFT);
		}else if(state.ball.distance < 100){
			SetPresetWalk(WALK_STABLE_BACKWARD);
		}else if(state.ball.distance >= 100){
			SetPresetWalk(SMALL_FORWARD);
			if(state.self.sensor.chestPSD < 110000){
				//シュート！！
				shootflag = true;
				approachcounter = 0;

				ReserveCall();
				SetPresetWalk(SMALL_FORWARD);


				SetFaceLED(FACE_READY);
				return;
			}
		}
	}else{
		bhvBallApproach.Execute();
		return;
	}
	*/
}
