#ifndef DETECTBALL_H
#define DETECTBALL_H

#include "../config.h"
#include "../State/state.h"
#include "../Utility/Predictor.h"

struct circle_param{
	double param[3];
};

struct cfit {
	double x;
	double y;
	double r;
	double d;// åÎç∑
};

class DetectBall
{
public:
	DetectBall();
	virtual ~DetectBall();

	static void Execute();
	static void CutOffHorizon();

	static Predictor pred_x;
	static Predictor pred_y;


private:

	static int orangew;
	static int orangeh;

	static circle_param circle;

	static bool Judge();		

	static void SetTrackingTilt1(double radius);
	static void SetTrackingV(double radius);

	static double LeastSquare();
	static int DetectEdge();

	static double Dist2Ball(int r_pixel);

};

#endif
