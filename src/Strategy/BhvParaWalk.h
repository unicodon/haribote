#ifndef BHVPARAWALK_H
#define BHVPARAWALK_H

#include "Behavior.h"
#include "../Motion/PresetWalkParam.h"
#include "../joint.h"

enum PARAWALKINDEX{
	/*WALK_SMALL5_RIGHT90,
	WALK_SMALL5_RIGHT80,
	WALK_SMALL5_RIGHT70,
	WALK_SMALL5_RIGHT60,
	WALK_SMALL5_RIGHT50,
	WALK_SMALL5_RIGHT40,
	WALK_SMALL5_RIGHT30,
	WALK_SMALL5_RIGHT20,
	WALK_SMALL5_RIGHT10,

	WALK_SMALL5_FORWARD,
	
	WALK_SMALL5_LEFT10,
	WALK_SMALL5_LEFT20,
	WALK_SMALL5_LEFT30,
	WALK_SMALL5_LEFT40,
	WALK_SMALL5_LEFT50,
	WALK_SMALL5_LEFT60,
	WALK_SMALL5_LEFT70,
	WALK_SMALL5_LEFT80,
	WALK_SMALL5_LEFT90,
	
	WALK_SMALL10_RIGHT90,
	WALK_SMALL10_RIGHT80,
	WALK_SMALL10_RIGHT70,
	WALK_SMALL10_RIGHT60,
	WALK_SMALL10_RIGHT50,
	WALK_SMALL10_RIGHT40,
	WALK_SMALL10_RIGHT30,
	WALK_SMALL10_RIGHT20,
	WALK_SMALL10_RIGHT10,

	WALK_SMALL10_FORWARD,

	WALK_SMALL10_LEFT10,
	WALK_SMALL10_LEFT20,
	WALK_SMALL10_LEFT30,
	WALK_SMALL10_LEFT40,
	WALK_SMALL10_LEFT50,
	WALK_SMALL10_LEFT60,
	WALK_SMALL10_LEFT70,
	WALK_SMALL10_LEFT80,
	WALK_SMALL10_LEFT90,
*/
	WALK_PARA_SET,//セットする歩行データ

	NUM_PARA_WALKPARAM	
};

enum PARA_WALK_MODE
{
	PARA_BALL_APPROACH,
	PARA_LAST_APPROACH,
	PARA_BALL_HOLD,
	PARA_HOLD_SHOT,
	PARA_DIRECT_SHOT
};

class BhvParaWalk : public Behavior{
private:
	PARA_WALK_MODE mode;
	static bool isInitialized;
	static WalkParam walkparams[NUM_PARA_WALKPARAM];

public:
	void Execute();
	bool ParaWalkApproach();
	bool ParaWalkApproachXY();
	bool SetHoldOrDirect(bool hold_flag=false, bool overwrite=false);//ボールを掴むかどうかのフラグを立てる
	void SetShotMode();//ボールを掴むかどうかの場合わけ
	void SetTargetXY(double &x, double &y, double target_x, double target_y, double flag_set);//チョコチョコ歩きで行く目的地の設定
	BhvParaWalk();
};

#endif
