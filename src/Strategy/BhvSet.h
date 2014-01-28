#ifndef BHVSET_H
#define BHVSET_H

#include "Behavior.h"

enum SET_MODE
{
	SET_MODE_LOCALIZATION,
	SET_MODE_BALL_SEARCH,
	SET_MODE_LOST_BALL_SEARCH,
	SET_MODE_TRACKING
};

class BhvSet : public Behavior{
public:
	BhvSet();
	void Execute();
private:
	SET_MODE mode;
	int count;
};

#endif
