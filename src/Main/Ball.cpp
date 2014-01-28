#include "StdAfx.h"
#include ".\ball.h"

extern CMaterial matr_red;
extern CMaterial matr_blue;

extern GLdouble drop_shadow_mat[16];

extern GLfloat dif_shadow[4];



CBall::CBall(void)
{
	dispList_init_flag[0] =
	dispList_init_flag[1] =	false;
	bDrawTail = false;
}

CBall::~CBall(void)
{
}

void CBall::AddForce(dReal dt)
{
	dReal coef = 0.015f;
	const dReal* vel;
	vel = dBodyGetLinearVel(bBall);
	dBodyAddForce( bBall,
		-vel[0]*coef,
		-vel[1]*coef,
		-vel[2]*coef
		);
}

void CBall::InitDisplayList()
{

	int nIndex = s_bOffscreenRender ? 1 : 0;

	if (!dispList_init_flag[nIndex])
	{
		dispList_polygon[nIndex]	= MakeDisplayList_Polygon();
		dispList_wire[nIndex]		= MakeDisplayList_Wire();
		dispList_shadow[nIndex]	= MakeDisplayList_Shadow();

		dispList_init_flag[nIndex] = true;
	}
}

GLuint CBall::MakeDisplayList_Polygon()
{
	GLuint dispList = glGenLists(1);
	glNewList(dispList, GL_COMPILE);
		material.setMaterial();
		DrawSphere(radius, false);
	glEndList();

	return dispList;
}

GLuint CBall::MakeDisplayList_Wire()
{
	GLuint dispList = glGenLists(1);
	glNewList(dispList, GL_COMPILE);
		material.setMaterial();
		DrawSphere(radius, true);
	glEndList();

	return dispList;
}

GLuint CBall::MakeDisplayList_Shadow()
{
	GLuint dispList = glGenLists(1);
	glNewList(dispList, GL_COMPILE);
		glDisable(GL_LIGHTING);
		glColor4fv(dif_shadow);		
		DrawSphere(radius, false);
		glEnable(GL_LIGHTING);
	glEndList();

	return dispList;
}

void CBall::Init()
{
	static float dif_orange[4] = {0.75,0.4,0.2,1.0};
	static float amb_orange[4] = {0.5,0.2,0.1,1.0};
	static float spec[4] = {1.0,1.0,1.0,1.0};

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.000,  0.000,  1.100);
	glGetDoublev(GL_MODELVIEW_MATRIX, position);
	glPopMatrix();

	material.SetDiffuse(dif_orange);
	material.SetAmbient(amb_orange);
	material.SetSpecular(spec);
	material.SetShininess(300);

	radius = 0.040;

	//bodyê›íË
	bBall = dBodyCreate(world);
	dBodySetAutoDisableFlag(bBall,0);
	dBodySetPosition(bBall, position[12+0], position[12+1], position[12+2]);

	dMass mass;
	dMassSetSphereTotal(&(mass),0.010,0.040);
	dBodySetMass (bBall ,&(mass));

	//geomê›íË
	transBall = dCreateGeomTransform(space);
	gBall = dCreateSphere (0,0.040);
	dGeomSetPosition(gBall, 0 ,0, 0.002);//èdêSÇ™3mmÇ∏ÇÍÇƒÇÈ
	dGeomTransformSetGeom(transBall, gBall);

	dGeomSetBody(transBall, bBall);

}

void CBall::DrawShadow()
{
	int nIndex = s_bOffscreenRender ? 1 : 0;

	if(!dispList_init_flag[nIndex])
	{
		InitDisplayList();
	}

	const dReal* pos = dGeomGetPosition(transBall);
	const dReal* gpos = dGeomGetPosition(gBall);
	const dReal* rot = dGeomGetRotation(transBall);
	
	position[ 0] = rot[0];
	position[ 1] = rot[4];
	position[ 2] = rot[8];
	position[ 3] = rot[3];
	position[ 4] = rot[1];
	position[ 5] = rot[5];
	position[ 6] = rot[9];
	position[ 7] = rot[7];
	position[ 8] = rot[2];
	position[ 9] = rot[6];
	position[10] = rot[10];
	position[11] = rot[11];
	position[12] = pos[0];
	position[13] = pos[1];
	position[14] = pos[2];
	position[15] = 1;

	glPushMatrix();
	glMultMatrixd(drop_shadow_mat);
	glMultMatrixd(position);
		glCallList(dispList_shadow[nIndex]);
	glPopMatrix();

}

void CBall::Draw(bool bWireFrame)
{
	int nIndex = s_bOffscreenRender ? 1 : 0;

	if(!dispList_init_flag[nIndex])
	{
		InitDisplayList();
	}

	const dReal* pos = dGeomGetPosition(transBall);
	const dReal* gpos = dGeomGetPosition(gBall);
	const dReal* rot = dGeomGetRotation(transBall);
	
	position[ 0] = rot[0];
	position[ 1] = rot[4];
	position[ 2] = rot[8];
	position[ 3] = rot[3];
	position[ 4] = rot[1];
	position[ 5] = rot[5];
	position[ 6] = rot[9];
	position[ 7] = rot[7];
	position[ 8] = rot[2];
	position[ 9] = rot[6];
	position[10] = rot[10];
	position[11] = rot[11];
	position[12] = pos[0];
	position[13] = pos[1];
	position[14] = pos[2];
	position[15] = 1;

	const int log_size		= 300;
	static int max_log		= 0;
	static int current_log	= 0;
	static double pos_log[log_size][16];
	if(bWireFrame)
	{
		glPushMatrix();
		glMultMatrixd(position);
		glTranslatef(gpos[0], gpos[1], gpos[2]);
		material.setMaterial();
		glCallList(dispList_wire[nIndex]);
		glPopMatrix();

		max_log = current_log = 0;
	}
	else
	{
		if (!s_bOffscreenRender)
		{

			static float dif[4]  = {0.75,0.4,0.2,0.0};
			static float amb[4]  = {0.5,0.2,0.1,0.0};
			static float spec[4] = {1.0,1.0,1.0,0.0};
			if (bDrawTail)
			{

				glDepthMask(0);

				glEnable(GL_BLEND);
				glDisable(GL_LIGHTING);

				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dif);
				glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, amb);
				glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, spec);
				glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 300);

				glBlendFunc(GL_SRC_ALPHA, GL_ONE);

				for (int i = 0;i < max_log; i++)
				{
					int j;
					if (max_log == log_size)
						j = (current_log + i) % log_size;
					else
						j = i;
					glColor4f(0.75,0.4,0.2, 0.5*(double)i / log_size);
					glPushMatrix();
					glMultMatrixd(pos_log[j]);
					glCallList(dispList_polygon[nIndex]);
					glPopMatrix();
				}

				glDisable(GL_BLEND);
				glEnable(GL_LIGHTING);
				glDepthMask(1);

				for (int i = 0;i < 16; i++)	pos_log[current_log][i] = position[i];
				current_log++;
				current_log = current_log % log_size;
				if (max_log < log_size)max_log++;
			}
			else
			{
				max_log = 0;
				current_log = 0;
			}
		}

		glPushMatrix();
		glMultMatrixd(position);
		material.setMaterial();
		glTranslatef(gpos[0], gpos[1], gpos[2]);
		glCallList(dispList_polygon[nIndex]);
		glPopMatrix();


	}

	glPushMatrix();
	glMultMatrixd(position);
	if(bWireFrame)
	{
		matr_blue.setMaterial();
		DrawSphere(0.005);
	}
	else
	{
	}
	glPopMatrix();
}