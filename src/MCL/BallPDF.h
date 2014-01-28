#ifndef BALLPDF_H
#define BALLPDF_H

#include "../State/MyRobot.h"

static const int NUM_SAMPLE = 1000;

struct BallParticle
{
	double bx;
	double by;
	double vx;
	double vy;
	double weight;
};

/*
struct BallObserveInfo
{
	double r;
	double phi;
};
*/

typedef BallInfo BallObserveInfo;

class BallPDF
{
public:
	BallParticle* sample;

	void Reset(BallObserveInfo info);
	void Update(BallObserveInfo info);


	void Move(double dx, double dy, double dtt, double dtime);
	void Progress(double dtime);

	double psum;

	BallParticle the_ball;

	BallPDF(void);
	~BallPDF(void);
};

#endif
