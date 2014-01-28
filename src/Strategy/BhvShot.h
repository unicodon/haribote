#ifndef BHVSHOT_H
#define BHVSHOT_H

#include "../Motion/PresetMotion.h"
#include "Behavior.h"

bool CalucShot(int &mode, double &x, double &y);
bool SelectShot(int &mode, double &x, double &y);
int	SelectShotSimple();
bool SelectShot_UnVisible(int &mode, int &pos_x, int &pos_y, double &theta);
class BhvShot : public Behavior{
private:
	PRESETMOTIOININDEX mode;
public:
	void Execute();
	bool HoldShot();
	bool HoldShot2();
	bool CalucShot(int &mode, double &x, double &y);
};

#endif
