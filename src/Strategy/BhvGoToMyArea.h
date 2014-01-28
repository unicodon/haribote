#ifndef BHVGOTOMYAREA_H
#define BHVGOTOMYAREA_H

#include "Behavior.h"

enum GO_TO_AREA_MODE
{
	GO_TO_AREA_GO,
	GO_TO_AREA_STAY
};

enum GO_TO_AREA_MF{
	GO_TO_AREA_MIDDLE,
	GO_TO_AREA_LOW,
	GO_TO_AREA_HIGH
};

class BhvGoToMyArea : public Behavior{
public:
	void Execute();
	void Initialize();
private:
	bool InArea(double x,double y);
	bool InSpot(double x,double y,double t);
	void GetPosition(int id, double &goal_x, double &goal_y, double &goal_t);
	GO_TO_AREA_MODE mode;
	GO_TO_AREA_MF mode_mf;
};

#endif
