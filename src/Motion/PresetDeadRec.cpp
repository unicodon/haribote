#include "StdAfx.h"
#include "PresetDeadRec.h"

bool PresetDeadRec::isInitialized = false;
std::vector<Point3D> PresetDeadRec::deadRecData;
std::map<std::string,int> PresetDeadRec::walkNameMap;

int PresetDeadRec::ResolveWalkName(const char* walkname){
	std::string walkstr = walkname;
	if(walkNameMap.find(walkstr) != walkNameMap.end()){
		//モーション名が登録済みのとき
		int walkIndex = walkNameMap[walkstr];
		if(walkIndex < deadRecData.size()){
			return walkIndex;
		}
	}
	return -1;

}

//モーション名登録
void PresetDeadRec::RegisterWalkName(const char* walkname){
	static int index = 0;
	walkNameMap[walkname] = index;
	index++;//←わかりやすさ優先
}


PresetDeadRec::PresetDeadRec(){
	if(!isInitialized){
		Point3D point;
		
		/*
		temporaly written for experiment
		*/
		//forward
		RegisterWalkName("WALK_FORWARD");
		point.set(
			0.0,80.0,-40.0*DEGTO);
		deadRecData.push_back(point);
		
		//backward
		RegisterWalkName("WALK_BACKWARD");
		point.set(
			0.0,0.0,0.0*DEGTO);
		deadRecData.push_back(point);
		
		isInitialized = true;
	}
}


/*
参考:去年までのデータ

	m_p3dDeadRecData[STATE_StableWalkForward2].set(-7.2 , 115.8 , 0.3*DEGTO); //78   
	m_p3dDeadRecData[STATE_StableWalkBackward2].set(33.5 , -72.8 , -1.9*DEGTO); //79 

	m_p3dDeadRecData[STATE_StableTurnRight2].set(0.0 , 0.0 , -35.0*DEGTO);//80   
	m_p3dDeadRecData[STATE_StableTurnLeft2].set(0.0 , 0.0 , 38.5*DEGTO);//81    
	m_p3dDeadRecData[STATE_StableHalfTurnR2].set(0.0 , 0.0 , -17.8*DEGTO);//82   
	m_p3dDeadRecData[STATE_StableHalfTurnL2].set(0.0 , 0.0 , 18.9*DEGTO);//83   


	m_p3dDeadRecData[STATE_StableRightForward45].set(56.6 , 55.1 , 0.4*DEGTO);//84  
	m_p3dDeadRecData[STATE_StableLeftForward45].set(-55.4 , 54.1 , -0.2*DEGTO);//85  

	m_p3dDeadRecData[STATE_StableRollRightFast2].set(0.0 , 0.0 , -18.9*DEGTO);//86 
	m_p3dDeadRecData[STATE_StableRollLeftFast2].set(-32.7 , 17.4 , 22.5*DEGTO);//87 

	m_p3dDeadRecData[STATE_StableRightSide2].set(44.0 , 1.0 , -0.4*DEGTO);
	m_p3dDeadRecData[STATE_StableLeftSide2].set(-45.0 , 1.3 , 0.2*DEGTO);

	m_p3dDeadRecData[STATE_StableRollRight2].set(66.7 , 23.4 , 8.3*DEGTO);
	m_p3dDeadRecData[STATE_StableRollLeft2].set(-78.8 , 1.6 , -9.5*DEGTO);

	m_p3dDeadRecData[STATE_StableRightBackward45].set(41.2 , -41.1 , 0.5*DEGTO);//92 
	m_p3dDeadRecData[STATE_StableLeftBackward45].set(-34.3 , -46.7 , -0.4*DEGTO);//93  

	m_p3dDeadRecData[STATE_StableWalkForwardSlow].set(-0.3 , 41.3 , -0.1*DEGTO);

	m_p3dDeadRecData[STATE_StableDribRF2].set(70.8 , 19.5 , 8.7*DEGTO);//95  //
	m_p3dDeadRecData[STATE_StableDribLF2].set(-83.1 , -4.8 , -10.0*DEGTO);//96   //

	m_p3dDeadRecData[STATE_RightForwardTurnRight2].set(-234.2 , 50.4 , -12.8*DEGTO);       //右前 97
	m_p3dDeadRecData[STATE_LeftForwardTurnLeft2].set(225.3 , 57.6 , 13.1*DEGTO);	//左前 98

	m_p3dDeadRecData[STATE_RightForwardTurnRightMore].set(22.8 , 102.9 , -12.6*DEGTO);	//99
	m_p3dDeadRecData[STATE_LeftForwardTurnLeftMore].set(29.8 , 94.7 , 10.5*DEGTO);//  100
	m_p3dDeadRecData[STATE_StableDribFF3].set(-2.5 , 33.1 , -0.5*DEGTO);//101

	m_p3dDeadRecData[STATE_DribRightForwardTurnRight2].set(9.7 , 40.8 , -0.6*DEGTO);
	m_p3dDeadRecData[STATE_DribLeftForwardTurnLeft2].set(-15.5 , 37.2 , 0.8*DEGTO);
	m_p3dDeadRecData[STATE_DribRightForwardTurnRightMore].set(16.2 , 20.0 , -5.2*DEGTO);
	m_p3dDeadRecData[STATE_DribLeftForwardTurnLeftMore].set(-12.8 , 24.5 , 5.9*DEGTO);

///////////////////////////////////////////////////////////////////////////
	m_p3dDeadRecData[STATE_KeeperWalkForward].set(2.9 , 60.7 , 0.0*DEGTO);  //106        
	m_p3dDeadRecData[STATE_KeeperWalkBackward].set(2.1 , -38.0 , -0.3*DEGTO);     //107  
	m_p3dDeadRecData[STATE_KeeperRightTurn].set(0.0 , 0.0 , 0.0*DEGTO);  //108 speed=16 
	m_p3dDeadRecData[STATE_KeeperLeftTurn].set(0.0 , 0.0 , 0.0*DEGTO);  //109  speed=16
	m_p3dDeadRecData[STATE_KeeperRightSide].set(45.7 , 2.6 , 0.3*DEGTO);           //110 
	m_p3dDeadRecData[STATE_KeeperLeftSide].set(-43.4 , 4.6 , 0.3*DEGTO);            //111  

	m_p3dDeadRecData[STATE_StableWalkForwardMiddle].set(4.9 , 73.3 , -0.1*DEGTO);//112
	m_p3dDeadRecData[STATE_StableRightSideSlow].set(32.3 , 9.3 , 1.0*DEGTO);//113
	m_p3dDeadRecData[STATE_StableLeftSideSlow].set(-33.5 , 6.8 , 0.3*DEGTO);//114
	m_p3dDeadRecData[STATE_StableRollRightSmall].set(21.1 , 12.6 , 14.3*DEGTO);//115
	m_p3dDeadRecData[STATE_StableRollLeftSmall].set(-32.7 , 0.4 , -13.0*DEGTO);//116
	m_p3dDeadRecData[STATE_DribbleFF].set( 0.6 , 42.1 , -1.0*DEGTO);//117
	m_p3dDeadRecData[STATE_DribbleRF].set( 14.9 , 30.5 , -0.6*DEGTO);//118
	m_p3dDeadRecData[STATE_DribbleLF].set(-15.3 , 26.5 , 1.3*DEGTO);//119
	m_p3dDeadRecData[STATE_RightForwardRollRight].set(73.6 , 117.7 , 8.0*DEGTO);
	m_p3dDeadRecData[STATE_LeftForwardRollLet].set(-73.2 , 108.6 , -8.8*DEGTO);
	m_p3dDeadRecData[STATE_RightForwardSmallRollRight].set(60.5 , 65.5 , 5.0*DEGTO);
	m_p3dDeadRecData[STATE_LeftForwardSmallRollLet].set(-60.5 , 63.4 , -5.8*DEGTO);
*/
