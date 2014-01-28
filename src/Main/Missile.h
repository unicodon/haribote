#pragma once
#include "primitivebody.h"

class CMissile :
	public CPrimitiveBody
{
public:
	float bDrawTail;
	double radius;

	dGeomID gMissile;
	dBodyID bMissile;

	bool dispList_init_flag[2];
	GLuint dispList_polygon[2];
	GLuint dispList_wire[2];
	GLuint dispList_shadow[2];

	GLuint MakeDisplayList_Polygon();
	GLuint MakeDisplayList_Wire();
	GLuint MakeDisplayList_Shadow();

	void InitDisplayList();

	void Launch(dReal* pos, dReal* rot, dReal* vel);

	double	livetime;
	bool	bLive;

public:
	CMissile(void);
	~CMissile(void);

	void AddForce(dReal dt);

	void Draw(bool bWireFrame = false);
	void DrawShadow();
	void Init();
};
