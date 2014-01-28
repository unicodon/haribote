#ifndef BHVSHOTCHOOSE_H
#define BHVSHOTCHOOSE_H

#include "../Motion/PresetMotion.h"
#include "Behavior.h"


class BhvShotChoose : public Behavior{
private:
	PRESETMOTIOININDEX mode;
	bool SetHoldArea();
	double pos_x;
	double pos_y;
	double pos_tt;
	bool hold;

public:
	void Execute();
	bool ShotChoose(int &mode, double &adj_dis);
	void AdjustPram(int &mode, double &adj_dis);
};

#endif
