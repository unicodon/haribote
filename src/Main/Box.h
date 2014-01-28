#pragma once
#include "PrimitiveBody.h"

class CBox :
	public CPrimitiveBody
{
private:
	double cx;
	double cy;
	double cz;
public:
	CBox(double cx_, double cy_, double cz_);
	CBox();
	~CBox(void);

	void SetSize(double cx_, double cy_, double cz_);


	void Draw(bool bWireFrame = false);
};
