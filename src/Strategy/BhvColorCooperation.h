#ifndef BHV_COLOR_COOPERATION
#define BHV_COLOR_COOPERATION

#include "Behavior.h"

class BhvColorCooperation : public Behavior
{
	void Execute(void);
protected:
	void Target(void);
	double Radian(double deg);

	double target_x;
	double target_y;
	double ball_x;
	double ball_y;
	bool Reached;
};

#endif
