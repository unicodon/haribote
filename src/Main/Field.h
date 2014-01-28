#pragma once
#include "Box.h"
#include "Cylinder.h"

class CField
{
private:
	dGeomID gGround;
	dGeomID gYgoal1;
	dGeomID gYgoal2;
	dGeomID gYgoal3;
	dGeomID gSgoal1;
	dGeomID gSgoal2;
	dGeomID gSgoal3;
	dGeomID gLandmarkYP;
	dGeomID gLandmarkPY;
	dGeomID gLandmarkSP;
	dGeomID gLandmarkPS;

	CBox ground;

	CBox Ygoal[3];
	CBox Sgoal[3];

	CCylinder landmark1ps;
	CCylinder landmark2ps;
	CCylinder landmark3ps;

	CCylinder landmark1py;
	CCylinder landmark2py;
	CCylinder landmark3py;

	CCylinder landmark1sp;
	CCylinder landmark2sp;
	CCylinder landmark3sp;

	CCylinder landmark1yp;
	CCylinder landmark2yp;
	CCylinder landmark3yp;

	bool dispList_init_flag[2];

	GLuint dispList_polygon[2];
	GLuint dispList_wire[2];

	GLuint MakeDisplayList_Polygon();
	GLuint MakeDisplayList_Wire();

	void InitDisplayList();

public:
	CField(void);
	~CField(void);

	void Init();
	void InitGL();

	void DrawField(bool bWireFrame = false);
	void DrawGoals(bool bWireFrame = false);
	void DrawLandmarks(bool bWireFrame = false);

	dSpaceID space;
	dWorldID world;

};
