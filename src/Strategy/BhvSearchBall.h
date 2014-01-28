#ifndef BHV_ONLY_SEARCH_BALL_H
#define BHV_ONLY_SEARCH_BALL_H

#include "Behavior.h"

enum SEARCH_BALL_MODE
{
	SEARCH_BALL_STAY,
	SEARCH_BALL_BACK,
	SEARCH_BALL_SWING,
	SEARCH_BALL_ROLL,
	SEARCH_BALL_FORWARD,
	SEARCH_BALL_END,
	SEARCH_BALL_NEXT_BHV
};

class BhvSearchBall : public Behavior{
public:
	BhvSearchBall();
	void Execute();
	void Initialize();
	SEARCH_BALL_MODE mode;
private:
	int count_stay;
	int back_count;
	int roll_count;
	int forward_count;
	int swing_count;
	bool swing_swith;
};

#endif
