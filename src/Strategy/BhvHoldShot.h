#ifndef BHVHOLDSHOT_H
#define BHVHOLDSHOT_H

#include "../Motion/PresetMotion.h"
#include "Behavior.h"

enum TURN_MODE
{
	MODE_DEFAULT,
	MODE_RIGHT,
	MODE_LEFT
};


class BhvHoldShot : public Behavior{
private:
	TURN_MODE turn_mode;
	unsigned int hold_timer;
	int overshoot;				//âÒÇËÇ∑Ç¨ÇÃÇΩÇﬂÇÃí≤êÆíl
public:
	void Execute();
	bool HoldShot();
	void SetDefault();
};

#endif
