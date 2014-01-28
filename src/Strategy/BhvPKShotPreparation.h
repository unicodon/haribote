#ifndef BHV_PK_SHOT_PREPARATION_
#define BHV_PK_SHOT_PREPARATION_

#include "Behavior.h"

enum BALL_APPROACH
{
	BALL_APPROACH_HEAD,
	BALL_APPROACH_WALK,
	BALL_APPROACH_WALKING,
	BALL_APPROACH_SHOT
};

class BhvPKShotPreparation : public Behavior
{
public:
	BhvPKShotPreparation();
	void Execute();
	void Initialize();
private:
	int SetRollCount(double shot_direction);

	BALL_APPROACH mode;
	double goal_direction;
	double goal_count;
	bool goal_observation;
	int count_roll_end;
	bool head_start;
	bool goal_near;
	int roll_count;
};

#endif //BHV_PK_SHOT_PREPARATION_
