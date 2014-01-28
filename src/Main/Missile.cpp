#include "StdAfx.h"
#include ".\missile.h"

extern CMaterial matr_red;
extern CMaterial matr_blue;

extern GLdouble drop_shadow_mat[16];

extern GLfloat dif_shadow[4];



CMissile::CMissile(void)
{
	dispList_init_flag[0] =
	dispList_init_flag[1] =	false;
	bDrawTail = false;
}

CMissile::~CMissile(void)
{
}

void CMissile::AddForce(dReal dt)
{
	livetime -= dt;
	if (livetime < 0)
	{
		livetime = 0;
		bLive = false;
		dBodyDisable(bMissile);
		dGeomDisable(gMissile);
	}

	if (bLive == false)
		return;

	dReal coef = 0.015f;
	const dReal* vel;
	vel = dBodyGetLinearVel(bMissile);
	dBodyAddForceAtRelPos( bMissile,
		-vel[0]*coef,
		-vel[1]*coef,
		-vel[2]*coef,
		0,0,-0.4
		);

	dMass mass;
	dBodyGetMass(bMissile, &mass);

	dBodyAddRelForceAtRelPos( bMissile,
		0, 0, mass.mass * 12.0,
		0,0, 0.2
		);
	

	dBodyAddForceAtRelPos( bMissile,
		0, 0, mass.mass * 0.2,
		0,0,0
		);

}

void CMissile::InitDisplayList()
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

GLuint CMissile::MakeDisplayList_Polygon()
{
	GLuint dispList = glGenLists(1);
	glNewList(dispList, GL_COMPILE);
		material.setMaterial();
		glPushMatrix();
		DrawCylinder(0.01,0.1, false);
		glTranslated(0,0,0.05);
		DrawSphere(0.01, false);
		glPopMatrix();
	glEndList();

	return dispList;
}

GLuint CMissile::MakeDisplayList_Wire()
{
	GLuint dispList = glGenLists(1);
	glNewList(dispList, GL_COMPILE);
		material.setMaterial();
		DrawSphere(0.02, false);
	glEndList();

	return dispList;
}

GLuint CMissile::MakeDisplayList_Shadow()
{
	GLuint dispList = glGenLists(1);
	glNewList(dispList, GL_COMPILE);
		material.setMaterial();
		glPushMatrix();
		DrawCylinder(0.01,0.1, false);
		glTranslated(0,0,0.05);
		DrawSphere(0.01, false);
		glPopMatrix();
	glEndList();

	return dispList;
}

void CMissile::Launch(dReal* pos, dReal* rot, dReal* vel)
{
	bLive = true;
	livetime = 3.0;
	dBodyEnable(bMissile);
	dGeomEnable(gMissile);

	dBodySetForce(bMissile,0,0,0);
	dBodySetLinearVel(bMissile,0,0,0);
	dBodySetAngularVel(bMissile,0,0,0);
	dBodySetPosition(bMissile, pos[0], pos[1], pos[2]);
	dBodySetRotation(bMissile, rot);
}

void CMissile::Init()
{
	bLive = false;
	livetime = 0;

	static float dif_orange[4] = {0.7,0.7,0.7,1.0};
	static float amb_orange[4] = {0.5,0.5,0.5,1.0};
	static float spec[4] = {1.0,1.0,1.0,1.0};

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(0.000,  0.000,  2.000);
	glGetDoublev(GL_MODELVIEW_MATRIX, position);
	glPopMatrix();

	material.SetDiffuse(dif_orange);
	material.SetAmbient(amb_orange);
	material.SetSpecular(spec);
	material.SetShininess(300);

	radius = 0.040;

	//bodyÝ’è
	bMissile = dBodyCreate(world);
	dBodySetAutoDisableFlag(bMissile,0);
	dBodySetPosition(bMissile, position[12+0], position[12+1], position[12+2]);

	dMass mass;
	dMassSetSphereTotal(&(mass),3.010,0.040);
	dBodySetMass (bMissile ,&(mass));

	//geomÝ’è
	gMissile = dCreateCCylinder (space,0.01, 0.2);
	dGeomSetPosition(gMissile, 0 ,0, 0);//dS‚ª3mm‚¸‚ê‚Ä‚é

	dGeomSetBody(gMissile, bMissile);

	dBodyDisable(bMissile);
	dGeomDisable(gMissile);

}

void CMissile::DrawShadow()
{
	if(!bLive)
		return;

	int nIndex = s_bOffscreenRender ? 1 : 0;

	if(!dispList_init_flag[nIndex])
	{
		InitDisplayList();
	}

	const dReal* pos = dGeomGetPosition(gMissile);
	const dReal* rot = dGeomGetRotation(gMissile);
	
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

void CMissile::Draw(bool bWireFrame)
{
	if (!bLive)
	{
		return;
	}

	int nIndex = s_bOffscreenRender ? 1 : 0;

	if(!dispList_init_flag[nIndex])
	{
		InitDisplayList();
	}

	const dReal* pos = dGeomGetPosition(gMissile);
	const dReal* rot = dGeomGetRotation(gMissile);
	
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

	const int log_size		= 30;
	static int max_log		= 0;
	static int current_log	= 0;
	static double pos_log[log_size][16];
	if(bWireFrame)
	{
		glPushMatrix();
		glMultMatrixd(position);
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
			if (1)
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
					glColor4f(0.5,0.5,0.5, 0.5*(double)i / log_size);
					glPushMatrix();
					glMultMatrixd(pos_log[j]);

					glTranslated(0,0,-0.05);
					glScaled(0.8,0.8,1);
					glCallList(dispList_wire[nIndex]);

					glTranslated(0,0,-0.02);
					glScaled(0.8,0.8,1.2);
					glCallList(dispList_wire[nIndex]);

					glTranslated(0,0,-0.02);
					glScaled(0.8,0.8,1.2);
					glCallList(dispList_wire[nIndex]);
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