#pragma once
#include "Mesh.h"
#include "inc.h"
#include "MotionEngine.h"
#include "../config.h"

#include "../SoccerDog.h"

const joint_attach[NUM_JOINT][2] = 
{
	{B_BODY,	B_RF1},
	{B_RF1,		B_RF2},
	{B_RF2,		B_RF3},

	{B_BODY,	B_LF1},
	{B_LF1,		B_LF2},
	{B_LF2,		B_LF3},

	{B_BODY,	B_RH1},
	{B_RH1,		B_RH2},
	{B_RH2,		B_RH3},

	{B_BODY,	B_LH1},
	{B_LH1,		B_LH2},
	{B_LH2,		B_LH3},

	{B_BODY,	B_NECK_ROOT},
	{B_NECK_ROOT,	B_NECK},
	{B_NECK,	B_HEAD},
	{B_HEAD,	B_CHIN},
};

class CRobot
{
private:


public:
	void ResetPosition(dReal* destPos);
	void ResetPosition();
	void SetSensorData();

	SoccerDog soccerDog;

	BYTE imgbuf[IMAGE_WIDTH*IMAGE_HEIGHT*3];

	CMesh *draw_mesh;
	CMesh *geom_mesh;

	static bool bWireFrame;
	static bool bUseGeomMesh;

	dReal jointAngles[NUM_JOINT];
	dReal jointRefAngles[NUM_JOINT];
	dReal torque[NUM_JOINT];
	dJointID joints[NUM_JOINT];
	dGeomID	transGeom[NUM_BODY];

	dSpaceID space;
	dWorldID world;

	CRobot(void);
	~CRobot(void);
	void Draw(void);
	void DrawShadow();
	void Init(void);
	void InitGL();

	void AddTorque(dReal interval);

	void SetCameraView();

	MotionEngine motion;
};
