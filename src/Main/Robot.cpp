#include "StdAfx.h"
#include <windows.h>
#include ".\robot.h"
#include "MotionEngine.h"
#include "../Motion/GaitMaker.h"
#include "../Motion/MotionMaker.h"
#include "../Motion/HeadController.h"

bool CRobot::bWireFrame = false;
bool CRobot::bUseGeomMesh = false;

#ifndef PI
#define PI 3.1416
#endif

bool bTorqueFlag = true;

const int PGAIN[NUM_JOINTS] = {
	//標準ゲイン
	0x1C,0x14,0x1C,
	0x1C,0x14,0x1C,
	0x1C,0x14,0x1C,
	0x1C,0x14,0x1C,

	0x0A,0x08,0x08,0x08,
	0x0A,0x0A
};

const int IGAIN[NUM_JOINTS] = {
	0x08,0x04,0x08,
	0x08,0x04,0x08,
	0x08,0x04,0x08,
	0x08,0x04,0x08,

	0x04,0x02,0x04,0x00,
	0x04,0x04
};

const int DGAIN[NUM_JOINTS] = {
	0x01,0x01,0x01,
	0x01,0x01,0x01,
	0x01,0x01,0x01,
	0x01,0x01,0x01,

	0x02,0x04,0x02,0x04,
	0x04,0x04
};

dReal massInfo[][10] = {
	{800,0,0,0,150,150,150,0,0,0},

	{250,0,0,0,1,5,5,0,0,0},
	{10,0,0,0,1,5,5,0,0,0},
	{10,0,0,0,1,1,1,0,0,0},
	{10,0,0,0,1,5,5,0,0,0},


	{10,0,0,0,1,1,1,0,0,0},
	{60,0,0,0,1,5,5,0,0,0},
	{80,0,0,0,1,5,5,0,0,0},

	{10,0,0,0,1,1,1,0,0,0},
	{60,0,0,0,1,5,5,0,0,0},
	{80,0,0,0,1,5,5,0,0,0},

	{10,0,0,0,1,1,1,0,0,0},
	{60,0,0,0,1,5,5,0,0,0},
	{80,0,0,0,1,5,5,0,0,0},

	{10,0,0,0,1,1,1,0,0,0},
	{60,0,0,0,1,5,5,0,0,0},
	{80,0,0,0,1,5,5,0,0,0},
};

dReal massCenter[NUM_BODY][3] = {
	{-30,0,20},

	{30,0,0},
	{20,0,0},
	{0,0,0},
	{0,0,0},

	{0,0,0},
	{0,0,-25},
	{0,0,-25},
	{0,0,0},
	{0,0,-25},
	{0,0,-25},
	{0,0,0},
	{0,0,-25},
	{0,0,-25},
	{0,0,0},
	{0,0,-25},
	{0,0,-25},
};


const char* draw_mesh_filename[] = {
	"Mesh\\draw_body_tail.x",

	"Mesh\\draw_head_ear_eye.x",
	"Mesh\\draw_chin.x",
	NULL,
	"Mesh\\draw_neck.x",

	"Mesh\\draw_rf1.x",
	"Mesh\\draw_rf2.x",
	"Mesh\\draw_rf3.x",
	"Mesh\\draw_lf1.x",
	"Mesh\\draw_lf2.x",
	"Mesh\\draw_lf3.x",
	"Mesh\\draw_rh1.x",
	"Mesh\\draw_rh2.x",
	"Mesh\\draw_rh3.x",
	"Mesh\\draw_lh1.x",
	"Mesh\\draw_lh2.x",
	"Mesh\\draw_lh3.x",

};

const char* geom_mesh_filename[] = {
	"Mesh\\geom_body.x",

	"Mesh\\geom_head.x",
	"Mesh\\draw_chin.x",
	NULL,
	NULL,

	NULL,
	"Mesh\\geom_rf2.x",
	"Mesh\\geom_rf3.x",
	NULL,
	"Mesh\\geom_lf2.x",
	"Mesh\\geom_lf3.x",
	NULL,
	"Mesh\\geom_rh2.x",
	"Mesh\\geom_rh3.x",
	NULL,
	"Mesh\\geom_lh2.x",
	"Mesh\\geom_lh3.x",

};

const double ini_body_pos[][3] = {
	{	0.0,			0.0,		0.0},
	
	{	0.0675,			0.0,		0.0800 + 0.0195},
	{	0.0675+0.0400,	0.0,		0.0800 + 0.0195 - 0.0175},
	{	0.0675,			0.0,		0.0195},
	{	0.0675,			0.0,		0.0195},

	{	0.0650,			-0.125/2,	0},
	{	0.0650,			-0.125/2,	0},
	{	0.0650+0.0078,	-0.1344/2,	-0.0695},
	{	0.0650,			0.1250/2,	0},
	{	0.0650,			0.1250/2,	0},
	{	0.0650+0.0078,	0.1344/2,	-0.0695},
	{	-0.0650,		-0.1250/2,	0},
	{	-0.0650,		-0.1250/2,	0},
	{	-0.0728,		-0.1344/2,	-0.0695},
	{	-0.0650,		0.1250/2,	0},
	{	-0.0650,		0.1250/2,	0},
	{	-0.0728,		0.1344/2,	-0.0695},
};

const double ini_joint_pos[][3] = {
	{	0.0650,			-0.125/2,	0},
	{	0.0650,			-0.125/2,	0},
	{	0.0650+0.0078,	-0.1344/2,	-0.0695},
	{	0.0650,			0.1250/2,	0},
	{	0.0650,			0.1250/2,	0},
	{	0.0650+0.0078,	0.1344/2,	-0.0695},
	{	-0.0650,		-0.1250/2,	0},
	{	-0.0650,		-0.1250/2,	0},
	{	-0.0728,		-0.1344/2,	-0.0695},
	{	-0.0650,		0.1250/2,	0},
	{	-0.0650,		0.1250/2,	0},
	{	-0.0728,		0.1344/2,	-0.0695},

	{	0.0675,			0.0,		0.0195},
	{	0.0675,			0.0,		0.0195},
	{	0.0675,			0.0,		0.0800 + 0.0195},
	{	0.0675+0.0400,	0.0,		0.0800 + 0.0195 - 0.0175},
};

dReal angle_limit[NUM_JOINT][2] = {
	{-115	*PI/180,	130	*PI/180},
	{-15	*PI/180,	93	*PI/180},
	{-25	*PI/180,	122	*PI/180},
	{-115	*PI/180,	130	*PI/180},
	{-15	*PI/180,	93	*PI/180},
	{-25	*PI/180,	122	*PI/180},
	{-135	*PI/180,	120	*PI/180},
	{-15	*PI/180,	93	*PI/180},
	{-25	*PI/180,	122	*PI/180},
	{-135	*PI/180,	120	*PI/180},
	{-15	*PI/180,	93	*PI/180},
	{-25	*PI/180,	122	*PI/180},

	{-75	*PI/180,	5	*PI/180},
	{-90	*PI/180,	90	*PI/180},
	{-15	*PI/180,	45	*PI/180},
	{(-55)	*PI/180,	3*PI/180},
};

dReal axis[NUM_JOINT][3] = {
	{0,1,0},	{1,0,0},	{0,1,0},
	{0,1,0},	{-1,0,0},	{0,1,0},
	{0,-1,0},	{1,0,0},	{0,-1,0},
	{0,-1,0},	{-1,0,0},	{0,-1,0},

	{0,1,0},
	{0,0,-1},
	{0,1,0},
	{0,1,0}
};

dReal testMotion2[5][NUM_JOINT + 1] = {
2.000,90,90,90,90,90,90,90,90,90,90,90,90,20,30,0,-60,
1.000,90,0,110,90,0,110,90,0,110,90,0,110,20,-30,0,0,
1.000,-10,0,110,-10,0,110,-10,0,110,-10,0,110,20,50,0,0,
2.0000,-5,13,106,-5,13,106,-46,0,92,-46,0,92,0,0,0,0,
2.000,-5,13,106,-5,13,106,-46,0,92,-46,0,92,0,0,0,0,
};

dReal testMotion[5][NUM_JOINT + 1 + 2] = {
0.200,-30,0,110,-30,0,110,-30,0,120,-30,0,120,-30,70,30,-35,-60,0,
0.240,-30,80,130,-30,-10,130,-30,60,120,-30,-10,120,-30,70,30,-50,-60,0,
0.180,-30,80,130,-30,-10,130,-30,60,120,-30,-10,120,-70,70,30,-50,-60,0,
0.120,-30,-10,130,-30,80,130,-30,-10,120,-30,60,120,-80,-90,30,-50,-60,0,
0.240,-30,-10,130,-30,80,130,-30,-10,120,-30,60,120,-80,-90,30,-50,-60,0,
};

#define SPHERE_SIZE 40

CRobot::CRobot(void)
{
	draw_mesh = 0;
	geom_mesh = 0;

	for(int i = 0;i < NUM_JOINT;i++)
	{
		jointAngles[i]		= 0.0f;
		jointRefAngles[i]	= 0.0f;
		torque[i]			= 0.0f;
	}

	motion.ReadLine("2000,-30,0,110,-30,0,110,-30,0,120,-30,0,120,-30,70,30,-35,-60,0");
	motion.ReadLine("1000,90,0,110,90,0,110,90,0,110,90,0,110,20,-30,0,0");
	motion.ReadLine("1000,-10,0,110,-10,0,110,-10,0,110,-10,0,110,20,50,0,0");
	motion.ReadLine("2000,-5,13,106,-5,13,106,-46,0,92,-46,0,92,0,0,0,0");
	motion.ReadLine("2000,-5,13,106,-5,13,106,-46,0,92,-46,0,92,0,0,0,0");

	GaitMaker::SetPresetWalk(0,0);

}

CRobot::~CRobot(void)
{
	soccerDog.DoStop();
	soccerDog.DoDestroy();

	if(draw_mesh)
		delete[] draw_mesh;
	if(geom_mesh)
		delete[] geom_mesh;
}

void CRobot::AddTorque(dReal interval)
{
	static double time = 0.008;

	static double jRefs[NUM_JOINT + 2] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	static dReal iAngle[NUM_JOINT + 2] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	static dReal dAngle[NUM_JOINT + 2] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

	dReal P,I,D;

	time += interval;
	while (time > 0.008)
	{
		time -= 0.008;

		if(!MotionMaker::IsFinished()){
			jointangles angles;
			MotionMaker::GetNextJointValue(angles);
			for (int i = 0;i < NUM_JOINT;i++)
			{
				jRefs[i] = angles.angle[i];
			}
		}else{
			GaitMaker::GetNextJointValue(jRefs);

			HeadController::GetNextJointValue(jRefs[TILT1], 
											  jRefs[PAN], 
											  jRefs[TILT2], 
											  jRefs[CHIN] );
		}
		jRefs[RFLEG3] -= 30;
		jRefs[LFLEG3] -= 30;
		jRefs[RHLEG3] -= 30;
		jRefs[LHLEG3] -= 30;


	}

	for(int i = 0;i < NUM_JOINT;i++)
	{
		jointRefAngles[i] = jRefs[i] * PI / 180;
	}

/*
	int numPose = 5;
	static int j = 0;
	int next = (j + 1) % numPose;
	static float dt = 0;

	float p =  dt / (testMotion[j][0]);
*/
	dReal delta;
//	motion.GetNextAngle(interval, jointRefAngles);

	const double TorqueRatio[NUM_JOINT] = 
	{
		0.06,
		0.03,
		0.04,

		0.06,
		0.03,
		0.04,

		0.06,
		0.03,
		0.04,

		0.06,
		0.03,
		0.04,

		0.15,
		0.10,
		0.20,
		0.01,
	};

	for (int i = 0; i < NUM_JOINT; i++)
	{
		jointAngles[i]  = dJointGetHingeAngle(joints[i]);

		P = jointAngles[i] - jointRefAngles[i];
		D = (jointAngles[i] - dAngle[i] ) / interval; dAngle[i] = jointAngles[i];

		iAngle[i] += P * interval;

		dReal vel = dJointGetHingeAngleRate(joints[i]);

		delta			= jointAngles[i] - jointRefAngles[i];

		torque[i]		= -P * (PGAIN[i]) - D * (DGAIN[i] + 0x0F) *0.06 - iAngle[i]*(IGAIN[i])*0.1;
		torque[i]		*= TorqueRatio[i] * 0.6;

		if ( fabs(P) < 2.0 * PI / 180) torque[i] = 0;
		const double limit = 0.9;
		if ( torque[i] >  limit) torque[i] =  limit;
		else
		if ( torque[i] < -limit) torque[i] = -limit;
	
		if (bTorqueFlag)
		{
			dJointAddHingeTorque(joints[i], torque[i]);
		}
	}
/*
	dt += interval;

	if(dt > testMotion[j][0]){
		dt -= testMotion[j][0];
		j = next;
	}
*/
}

void CRobot::ResetPosition()
{
	if (draw_mesh[B_BODY].body == NULL)return;
	const dReal destPos[3] = {0.0f, 0.0f, 2.0f};
	const dReal* bPos = dBodyGetPosition(draw_mesh[B_BODY].body);

	for(int i = 0; i < NUM_BODY;i++)
	{
		const dReal* pos = dBodyGetPosition(draw_mesh[i].body);
		dBodyDisable(draw_mesh[i].body);
		dBodySetPosition(draw_mesh[i].body,
				destPos[0] + (pos[0] - bPos[0]),
				destPos[1] + (pos[1] - bPos[1]),
				destPos[2] + (pos[2] - bPos[2])
			);
		dBodySetLinearVel(draw_mesh[i].body,0,0,0);
		dBodySetAngularVel(draw_mesh[i].body,0,0,0);
		dBodyEnable(draw_mesh[i].body);
	}

}

void CRobot::ResetPosition(dReal* destPos)
{
	if (draw_mesh[B_BODY].body == NULL)return;
//	const dReal destPos[3] = {0.0f, 0.0f, 0.30f};
	const dReal* bPos = dBodyGetPosition(draw_mesh[B_BODY].body);
	const dReal* bVel = dBodyGetLinearVel(draw_mesh[B_BODY].body);

	float p = 1000;
	float d = 1000;
	dBodyAddForce(draw_mesh[B_BODY].body,
		p*(destPos[0] - bPos[0]) - d*bVel[0],
		p*(destPos[1] - bPos[1]) - d*bVel[1],
		p*(destPos[2] - bPos[2]) - d*bVel[2]
		);

/*
	for (int i = 0; i < NUM_BODY;i++)
	{
		const dReal* pos = dBodyGetPosition(draw_mesh[i].body);
		dBodySetPosition( draw_mesh[i].body,
			destPos[0] + pos[0] - bPos[0],
			destPos[1] + pos[1] - bPos[1],
			destPos[2] + pos[2] - bPos[2]
			);

		dBodySetLinearVel(draw_mesh[i].body, 0,0,0);
		dBodySetAngularVel(draw_mesh[i].body, 0,0,0);
	}
*/
}

void CRobot::DrawShadow()
{
	for(int i = 0;i < NUM_BODY;i++){
		if(bUseGeomMesh)
		{
			if(geom_mesh[i].isValid)
				geom_mesh[i].DrawShadow();
		}
		else
		{
			if(draw_mesh[i].isValid)
				draw_mesh[i].DrawShadow();
		}
	}
}

void CRobot::Draw(void)
{

	for(int i = 0;i < NUM_BODY;i++){
		if(bUseGeomMesh)
		{
			if(geom_mesh[i].isValid)
				geom_mesh[i].Draw(CRobot::bWireFrame);
		}
		else
		{
			if(draw_mesh[i].isValid)
				draw_mesh[i].Draw(CRobot::bWireFrame);
		}
	}
}

void CRobot::Init(void)
{
	soccerDog.DoInit();
	soccerDog.DoStart();


	draw_mesh = new CMesh[NUM_BODY];
	geom_mesh = new CMesh[NUM_BODY];

	for (int i = 0;i < NUM_BODY;i++)
	{
		if(geom_mesh_filename[i])
		{
			geom_mesh[i].Load(geom_mesh_filename[i]);
		}
		geom_mesh[i].centerShift[0] = -massCenter[i][0] / 1000.0;
		geom_mesh[i].centerShift[1] = -massCenter[i][1] / 1000.0;
		geom_mesh[i].centerShift[2] = -massCenter[i][2] / 1000.0;
	}

	for (int i = 0;i < NUM_BODY;i++)
	{
		if(draw_mesh_filename[i])
		{
			draw_mesh[i].Load(draw_mesh_filename[i]);
		}
		draw_mesh[i].centerShift[0] = -massCenter[i][0] / 1000.0;
		draw_mesh[i].centerShift[1] = -massCenter[i][1] / 1000.0;
		draw_mesh[i].centerShift[2] = -massCenter[i][2] / 1000.0;
	}

	for (int i = 0;i < NUM_BODY;i++)
	{
		draw_mesh[i].body = geom_mesh[i].body = dBodyCreate(world);
		dBodySetAutoDisableFlag(draw_mesh[i].body, 0);
		dBodySetPosition(geom_mesh[i].body,
				-0.3000	+ ini_body_pos[i][0] + massCenter[i][0] / 1000.0,
				0.0		+ ini_body_pos[i][1] + massCenter[i][1] / 1000.0,
				0.1500	+ ini_body_pos[i][2] + massCenter[i][2]	/ 1000.0		
				);

		dMass mass;
		dMassSetSphereTotal(&(mass),massInfo[i][0]/1000.0,0.040);
		dBodySetMass (geom_mesh[i].body ,&(mass));

		//geom設定
		if(geom_mesh[i].isValid)
		{
			geom_mesh[i].trans = dCreateGeomTransform(space);

			char buf[256] = "i";
			sprintf(buf,"%d.txt", i);
			ASSERT(geom_mesh[i].triData != NULL);
			//trimesh読み込み済み
			dGeomTriMeshDataBuildSingle(geom_mesh[i].triData, 
				geom_mesh[i].trimeshData.vertices,
				3 * sizeof(dReal), 
				geom_mesh[i].trimeshData.nVertices,
				geom_mesh[i].trimeshData.indices,
				geom_mesh[i].trimeshData.nIndices,
				3 * sizeof(unsigned int)
				);

			geom_mesh[i].geom = dCreateTriMesh(0, geom_mesh[i].triData, 0, 0, 0);
			dGeomSetData(geom_mesh[i].geom, geom_mesh[i].triData);
			dGeomSetPosition(geom_mesh[i].geom, 
				-massCenter[i][0] / 1000.0,
				-massCenter[i][1] / 1000.0,
				-massCenter[i][2] / 1000.0);

			dGeomTransformSetGeom(geom_mesh[i].trans, geom_mesh[i].geom);

			dGeomSetBody(geom_mesh[i].trans, geom_mesh[i].body);
		}
		else
		{
//			geom_mesh[i].geom = dCreateSphere(space, SPHERE_SIZE);
//			dGeomSetPosition(geom_mesh[i].geom,300,0,200);
//			dGeomSetBody(geom_mesh[i].geom, geom_mesh[i].body);
		}
	}

	//joint
	for(int i = 0;i < NUM_JOINT;i++){
		joints[i] = dJointCreateHinge(world,0);
		dJointAttach(joints[i], 
					 draw_mesh[joint_attach[i][0]].body, 
					 draw_mesh[joint_attach[i][1]].body );

		//ジョイントパラメータをセット
		dJointSetHingeAnchor(joints[i],	
							 ini_joint_pos[i][0] + -0.3,
							 ini_joint_pos[i][1] + 0,
							 ini_joint_pos[i][2] + 0.15
							 );
				
		dJointSetHingeAxis  (joints[i],	axis[i][0], axis[i][1], axis[i][2]);
		dJointSetHingeParam (joints[i],	dParamLoStop,	angle_limit[i][0]);
		dJointSetHingeParam (joints[i],	dParamHiStop,	angle_limit[i][1]);
//		dJointSetHingeParam (joints[i],	dParamCFM,		1.0e0);
//		dJointSetHingeParam (joints[i],	dParamStopERP,	0.5);
//		dJointSetHingeParam (joints[i],	dParamStopCFM,	1.0e0);

	}

}

void CRobot::SetCameraView()
{
	if (!CBody::s_bOffscreenRender)
		return;

	::glViewport( 0, 0, IMAGE_WIDTH, IMAGE_HEIGHT );
	static const GLfloat mymat[16] = {
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	-1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
	};

	//投影行列設定（カメラの視野角とか）
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	float zNear = 0.0001;
	float zFar = 20.000;
	float fov = 30;
	float aspect = (float)IMAGE_WIDTH / IMAGE_HEIGHT;

	gluPerspective(fov, aspect, zNear, zFar);

	//モデルビュー行列：カメラ位置・姿勢の設定
	glMatrixMode(GL_MODELVIEW);
	dBodyID body = draw_mesh[B_HEAD].body;
	const dReal* bodyPos = dBodyGetPosition(body);
	const dReal* bodyRot = dBodyGetRotation(body);

	GLfloat viewMat[16] = {
		bodyRot[0], bodyRot[1], bodyRot[2], bodyRot[3], 
		bodyRot[4], bodyRot[5], bodyRot[6], bodyRot[7], 
		bodyRot[8], bodyRot[9], bodyRot[10], bodyRot[11],
		0,0,0,1
	};

	const GLfloat mymat2[16] = {
			0.0f,	1.0f,	0.0f,	0.0f,
		-1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	1.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
	};

	glLoadIdentity();

	glMultMatrixf(mymat);
	glMultMatrixf(mymat2);
	glTranslatef(-0.07006, 0.0, 0.0146);
	glMultMatrixf(viewMat);
	glTranslatef(-bodyPos[0],-bodyPos[1],-bodyPos[2]);

}


void CRobot::SetSensorData()
{
	const dReal* rot = dBodyGetRotation(this->draw_mesh[B_BODY].body);
	dReal z[3];
	z[0] = rot[8];
	z[1] = rot[9];
	z[2] = rot[10];

	dReal g[3] = {0,0,1};

	dReal c = z[0]*g[0] + z[1]*g[1] + z[2]*g[2];
	state.self.accelaration.theta = acos(c) * 180 / M_PI - 90;
	state.self.accelaration.phi   = atan2(z[1], z[0]) * 180 / M_PI;
	if ( state.self.accelaration.theta > 0)
		state.self.accelaration.phi *= -1;

	//ここまで加速度情報格納

	//転倒判定
	if(state.self.isFallingDown){
		if(state.self.accelaration.theta < -65)
			state.self.isFallingDown = false;
	}else{
		if(state.self.accelaration.theta > -35)
			state.self.isFallingDown = true;
	}

	//初期モーション起動
	if(!soccerDog.lastangle_init){
		soccerDog.lastangle_init = true;
	}
	
	//関節角設定
	for (int i = 0; i < NUM_JOINT; i++)
	{
		state.self.sensor.jointValue[i] = 
			dJointGetHingeAngle(joints[i]) / M_PI * 180;
			
	}
	state.self.sensor.jointValue[TAILTILT] = 0;
	state.self.sensor.jointValue[TAILPAN ] = 0;

	//result image
	soccerDog.ResultImage(imgbuf);

}
