#ifndef INVERTER_H
#define INVERTER_H

#include "../Utility/MatrixUtil.h"

class Inverter  
{
public:
	Inverter();
	virtual ~Inverter();

	int Pos2AngleFront(double x, double y, double z, double &j1, double &j2, double &j3);
	int Pos2AngleRear(double x, double y, double z, double &j1, double &j2, double &j3);

	static Mat44 CalculateCameraPosition();
	static double CalculateTilt();
	static void CalculateHorizone(double bodytilt, double& m, double &a, bool& perp);

private:
	int pos2angle(double x,double y,double z,double &j1, double &j2, double &j3, double l1, double l2);
};

#endif
