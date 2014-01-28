#pragma once
#include "primitivebody.h"

class CCylinder :
	public CPrimitiveBody
{
private:
	double radius;
	double height;

	static void DrawWire(double rad, double hei);

	bool dispList_init_flag[2];
	GLuint dispList_polygon[2];
	GLuint dispList_wire[2];

	GLuint MakeDisplayList_Polygon();
	GLuint MakeDisplayList_Wire();

	void InitDisplayList();

public:
	CCylinder(void);
	CCylinder(double rad, double hei);
	~CCylinder(void);

	void SetSize(double rad, double hei);

	void Draw(bool bWireFrame = false);
};
