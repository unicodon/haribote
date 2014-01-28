#pragma once
#include "primitivebody.h"

class CBall :
	public CPrimitiveBody
{
public:
	float bDrawTail;
	double radius;

	dGeomID gBall;
	dGeomID transBall;
	dBodyID bBall;

	bool dispList_init_flag[2];
	GLuint dispList_polygon[2];
	GLuint dispList_wire[2];
	GLuint dispList_shadow[2];

	GLuint MakeDisplayList_Polygon();
	GLuint MakeDisplayList_Wire();
	GLuint MakeDisplayList_Shadow();

	void InitDisplayList();

public:
	CBall(void);
	~CBall(void);

	void AddForce(dReal dt);

	void Draw(bool bWireFrame = false);
	void DrawShadow();
	void Init();
};
