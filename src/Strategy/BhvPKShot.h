#ifndef BHV_PK_SHOT_
#define BHV_PK_SHOT_

#include "Behavior.h"

enum ShotDirection
{
	PK_SHOT_LEFT,
	PK_SHOT_CENTER,
	PK_SHOT_RIGHT
};

class BhvPKShot: public Behavior
{
public:
	BhvPKShot();
	void Execute();
	void SetDirection(ShotDirection input);
private:
	ShotDirection shotDirection;
};

#endif //BHV_PK_SHOT_PREPARATION_
