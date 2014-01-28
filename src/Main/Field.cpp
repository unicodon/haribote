#include "StdAfx.h"
#include ".\field.h"
#include "PPMReader.h"

static float dif_white[4]	= {0.9,0.9,0.9,1.0};
static float dif_green[4]	= {0.1,0.5,0.1,1.0};
static float dif_pink[4]	= {0.8,0.4,0.5,1.0};
static float dif_yellow[4]	= {0.6,0.6,0.1,1.0};
static float dif_skyblue[4] = {0.1,0.5,0.7,1.0};
static float spec_all[4]    = {0.0,0.0,0.0,1.0};

static float dif_red[4] =  {1.0,0.2,0.2,1.0};
static float dif_blue[4] = {0.2,0.2,1.0,1.0};

static float shininess  = 0.0;

GLfloat dif_shadow[4] = {0.0, 0.0, 0.0, 1.0};

CMaterial matr_white;
CMaterial matr_skyblue;
CMaterial matr_yellow;
CMaterial matr_red;
CMaterial matr_blue;


CMaterial matr_shadow;

extern GLdouble drop_shadow_mat[16];

CField::CField(void) 
{
	dispList_init_flag[0] =
	dispList_init_flag[1] =	false;
}

CField::~CField(void)
{
}

void CField::Init()
{
	space = dHashSpaceCreate(space);

	const GLfloat mymat[16] = {
		1.0f,	0.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	-1.0f,	0.0f,
		0.0f,	1.0f,	0.0f,	0.0f,
		0.0f,	0.0f,	0.0f,	1.0f,
	};

	//material
	matr_skyblue.SetDiffuse(dif_skyblue);
	matr_skyblue.SetAmbient(dif_skyblue);
	matr_skyblue.SetSpecular(spec_all);
	matr_skyblue.SetShininess(shininess);

	matr_yellow.SetDiffuse(dif_yellow);
	matr_yellow.SetAmbient(dif_yellow);
	matr_yellow.SetSpecular(spec_all);
	matr_yellow.SetShininess(shininess);

	matr_red.SetDiffuse(dif_red);
	matr_red.SetAmbient(dif_red);
	matr_red.SetSpecular(spec_all);
	matr_red.SetShininess(shininess);

	matr_blue.SetDiffuse(dif_blue);
	matr_blue.SetAmbient(dif_blue);
	matr_blue.SetSpecular(spec_all);
	matr_blue.SetShininess(shininess);

	matr_shadow.SetDiffuse(dif_shadow);
	matr_shadow.SetAmbient(dif_shadow);
	matr_shadow.SetSpecular(spec_all);
	matr_shadow.SetShininess(shininess);

	//ground
	ground.SetSize(6.000,4.000,0.100);
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, ground.position);
	glPopMatrix();
//	ground.material.SetDiffuse(dif_green);
//	ground.material.SetAmbient(dif_green);

	//landmark
	//sp
	landmark1sp.SetSize(0.050,0.200);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(1.350, 1.950,0);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark1sp.position);
	glPopMatrix();
	landmark1sp.material.SetDiffuse(dif_white);
	landmark1sp.material.SetAmbient(dif_white);

	landmark2sp.SetSize(0.050,0.100);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(1.350, 1.950,0.200);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark2sp.position);
	glPopMatrix();
	landmark2sp.material.SetDiffuse(dif_pink);
	landmark2sp.material.SetAmbient(dif_pink);

	landmark3sp.SetSize(0.050,0.100);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(1.350, 1.950,0.300);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark3sp.position);
	glPopMatrix();
	landmark3sp.material.SetDiffuse(dif_skyblue);
	landmark3sp.material.SetAmbient(dif_skyblue);

	//yp
	landmark1yp.SetSize(0.050,0.200);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1.350, 1.950,0);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark1yp.position);
	glPopMatrix();
	landmark1yp.material.SetDiffuse(dif_white);
	landmark1yp.material.SetAmbient(dif_white);

	landmark2yp.SetSize(0.050,0.100);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1.350, 1.950,0.200);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark2yp.position);
	glPopMatrix();
	landmark2yp.material.SetDiffuse(dif_pink);
	landmark2yp.material.SetAmbient(dif_pink);

	landmark3yp.SetSize(0.050,0.100);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1.350, 1.950,0.300);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark3yp.position);
	glPopMatrix();
	landmark3yp.material.SetDiffuse(dif_yellow);
	landmark3yp.material.SetAmbient(dif_yellow);

	//landmark
	//ps
	landmark1ps.SetSize(0.050,0.200);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(1.350, -1.950,0);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark1ps.position);
	glPopMatrix();
	landmark1ps.material.SetDiffuse(dif_white);
	landmark1ps.material.SetAmbient(dif_white);

	landmark2ps.SetSize(0.050,0.100);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(1.350, -1.950,0.200);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark2ps.position);
	glPopMatrix();
	landmark2ps.material.SetDiffuse(dif_skyblue);
	landmark2ps.material.SetAmbient(dif_skyblue);

	landmark3ps.SetSize(0.050,0.100);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(1.350, -1.950,0.300);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark3ps.position);
	glPopMatrix();
	landmark3ps.material.SetDiffuse(dif_pink);
	landmark3ps.material.SetAmbient(dif_pink);

	//py
	landmark1py.SetSize(0.050,0.200);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1.350, -1.950,0);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark1py.position);
	glPopMatrix();
	landmark1py.material.SetDiffuse(dif_white);
	landmark1py.material.SetAmbient(dif_white);

	landmark2py.SetSize(0.050,0.100);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1.350, -1.950,0.200);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark2py.position);
	glPopMatrix();
	landmark2py.material.SetDiffuse(dif_yellow);
	landmark2py.material.SetAmbient(dif_yellow);

	landmark3py.SetSize(0.050,0.100);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-1.350, -1.950,0.300);
	glMultMatrixf(mymat);
	glGetDoublev(GL_MODELVIEW_MATRIX, landmark3py.position);
	glPopMatrix();
	landmark3py.material.SetDiffuse(dif_pink);
	landmark3py.material.SetAmbient(dif_pink);


	//以下ODE関係
	//フィールドの物体配置
//	gGround = dCreateBox (space,9.00,6.00,4.0);
//	dGeomSetPosition(gGround,0,0,2.0);
	static dGeomID ground[6];
	for (int i = 0;i < 6; i++)
	{
		ground[i] = dCreateBox (space,	9.00,	6.00,	4.0);
	}
	dGeomSetPosition(ground[0],		0,		0,		-2.0	);
	dGeomSetPosition(ground[1],		0,		0,		 2.0 + 4.0);
	dGeomSetPosition(ground[2],		0,		6.0,	 0		);
	dGeomSetPosition(ground[3],		0,		-6.0,	 0		);
	dGeomSetPosition(ground[4],		9.0,	0,		 0		);
	dGeomSetPosition(ground[5],		-9.0,	0,		 0		);



	//ゴール正面板
	gSgoal1 = dCreateBox(space, 0.008, 0.800, 0.300);
	dGeomSetPosition(gSgoal1, (300+2700+4)/1000.0, 0, 0.150);

	//ゴール側面板
	gSgoal2 = dCreateBox(space, 0.300, 0.008, 0.300);
	dGeomSetPosition(gSgoal2, (2700+150)/1000.0, (400+4)/1000.0, 0.150);
	gSgoal3 = dCreateBox(space, 0.300, 0.008, 0.300);
	dGeomSetPosition(gSgoal3, (2700+150)/1000.0, (-400-4)/1000.0, 0.150);
	
	//ゴール正面板
	gYgoal1 = dCreateBox(space, 0.008, 0.800, 0.300);
	dGeomSetPosition(gYgoal1, -(300+2700+4)/1000.0, 0, 0.150);

	//ゴール側面板
	gYgoal2 = dCreateBox(space, 0.300, 0.008, 0.300);
	dGeomSetPosition(gYgoal2, -(2700+150)/1000.0, (400+4)/1000.0, 0.150);
	gYgoal3 = dCreateBox(space, 0.300, 0.008, 0.300);
	dGeomSetPosition(gYgoal3, -(2700+150)/1000.0, (-400-4)/1000.0, 0.150);

	//ランドマーク
	gLandmarkYP = dCreateCCylinder(space, 0.050, 0.400);
	dGeomSetPosition( gLandmarkYP, -1.350, 1.950, 0.200);

	gLandmarkPY = dCreateCCylinder(space, 0.050, 0.400);
	dGeomSetPosition( gLandmarkPY, -1.350, -1.950, 0.200);

	gLandmarkSP = dCreateCCylinder(space, 0.050, 0.400);
	dGeomSetPosition( gLandmarkSP, 1.350, 1.950, 0.200);

	gLandmarkPS = dCreateCCylinder(space, 0.050, 0.400);
	dGeomSetPosition( gLandmarkPS, 1.350, -1.950, 0.200);
}

void CField::DrawGoals(bool bWireFrame)
{
	if(bWireFrame)
	{
		{
			//goal
			const float goal_x = 2.700;
			const float goal_z = 0.400;
			const float goal_y = 0.300;
			const float goal_d = 0.300;
			const float goal_w = 0.006;//板の厚さ

			//skyblue
			matr_skyblue.setMaterial();
			glBegin(GL_LINE_LOOP);
				glNormal3f( 1.0f,	0.0f,	0.0f);
				glVertex3f( (goal_x + goal_d + goal_w) ,			-(goal_z + goal_w) , 0);
				glVertex3f( (goal_x + goal_d + goal_w) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d + goal_w) ,			 (goal_z + goal_w) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	1.0f,	0.0f);
				glVertex3f( (goal_x + goal_d + goal_w) ,			 (goal_z + goal_w) , 0);
				glVertex3f( (goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x			         ) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x			         ) ,			 (goal_z + goal_w) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	-1.0f,	0.0f);
				glVertex3f( (goal_x + goal_d + goal_w) ,			 -(goal_z + goal_w) , 0);
				glVertex3f( (goal_x + goal_d + goal_w) ,	 -(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x			         ) ,	 -(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x			         ) ,			 -(goal_z + goal_w) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( (goal_x + goal_d + goal_w) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d		 ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d		 ) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( (goal_x 			      ) ,	(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d +	goal_w) ,	(goal_z + goal_w ) , goal_y);
				glVertex3f( (goal_x + goal_d +	goal_w) ,	(goal_z ) , goal_y);
				glVertex3f( (goal_x 			      )	,	(goal_z ) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( (goal_x 			      ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d +	goal_w) ,	-(goal_z + goal_w ) , goal_y);
				glVertex3f( (goal_x + goal_d +	goal_w) ,	-(goal_z ) , goal_y);
				glVertex3f( (goal_x 			      )	,	-(goal_z ) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( -1.0f,	0.0f,	0.0f);
				glVertex3f( (goal_x 			      ) ,	(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x 			      ) ,	(goal_z + goal_w) ,		0);
				glVertex3f( (goal_x 			      )	,	(goal_z ) ,		0);
				glVertex3f( (goal_x 			      )	,	(goal_z ) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( -1.0f,	0.0f,	0.0f);
				glVertex3f( (goal_x 			      ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x 			      ) ,	-(goal_z + goal_w) ,		0);
				glVertex3f( (goal_x 			      )	,	-(goal_z ) ,		0);
				glVertex3f( (goal_x 			      )	,	-(goal_z ) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( -1.0f,	0.0f,	0.0f);
				glVertex3f( (goal_x + goal_d) ,		-(goal_z ) , 0);
				glVertex3f( (goal_x + goal_d) ,	-(goal_z ) , goal_y);
				glVertex3f( (goal_x + goal_d) ,	 (goal_z ) , goal_y);
				glVertex3f( (goal_x + goal_d) ,		 (goal_z ) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	-1.0f,	0.0f);
				glVertex3f( (goal_x + goal_d) ,		 (goal_z) , 0);
				glVertex3f( (goal_x + goal_d) ,	 (goal_z) , goal_y);
				glVertex3f( (goal_x			) ,	 (goal_z) , goal_y);
				glVertex3f( (goal_x			) ,		 (goal_z) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	1.0f,	0.0f);
				glVertex3f( (goal_x + goal_d) ,		 -(goal_z) , 0);
				glVertex3f( (goal_x + goal_d) ,	 -(goal_z) , goal_y);
				glVertex3f( (goal_x			) ,	 -(goal_z) , goal_y);
				glVertex3f( (goal_x			) ,		 -(goal_z) , 0);
			glEnd();

			//yellow
			matr_yellow.setMaterial();
			glBegin(GL_LINE_LOOP);
				glNormal3f( -1.0f,	0.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d + goal_w) ,			-(goal_z + goal_w) , 0);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d + goal_w) ,			 (goal_z + goal_w) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	1.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d + goal_w) ,			 (goal_z + goal_w) , 0);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x		          ) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x		          ) ,			 (goal_z + goal_w) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	-1.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d + goal_w) ,			 -(goal_z + goal_w) , 0);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	 -(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x			      ) ,	 -(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x			      ) ,			 -(goal_z + goal_w) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d		 ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d		 ) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( -(goal_x 			      ) ,	(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d +	goal_w) ,	(goal_z + goal_w ) , goal_y);
				glVertex3f( -(goal_x + goal_d +	goal_w) ,	(goal_z ) , goal_y);
				glVertex3f( -(goal_x 			      )	,	(goal_z ) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( -(goal_x  			      ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d +	goal_w) ,	-(goal_z + goal_w ) , goal_y);
				glVertex3f( -(goal_x + goal_d +	goal_w) ,	-(goal_z ) , goal_y);
				glVertex3f( -(goal_x 			      )	,	-(goal_z ) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 1.0f,	0.0f,	0.0f);
				glVertex3f( -(goal_x 			      ) ,	(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x 			      ) ,	(goal_z + goal_w) ,		0);
				glVertex3f( -(goal_x 			      )	,	(goal_z ) ,		0);
				glVertex3f( -(goal_x 			      )	,	(goal_z ) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 1.0f,	0.0f,	0.0f);
				glVertex3f( -(goal_x 			      ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x 			      ) ,	-(goal_z + goal_w) ,		0);
				glVertex3f( -(goal_x 			      )	,	-(goal_z ) ,		0);
				glVertex3f( -(goal_x 			      )	,	-(goal_z ) , goal_y);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 1.0f,	0.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d) ,		-(goal_z ) , 0);
				glVertex3f( -(goal_x + goal_d) ,	-(goal_z ) , goal_y);
				glVertex3f( -(goal_x + goal_d) ,	 (goal_z ) , goal_y);
				glVertex3f( -(goal_x + goal_d) ,		 (goal_z ) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	-1.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d) ,		 (goal_z) , 0);
				glVertex3f( -(goal_x + goal_d) ,	 (goal_z) , goal_y);
				glVertex3f( -(goal_x			) ,	 (goal_z) , goal_y);
				glVertex3f( -(goal_x			) ,		 (goal_z) , 0);
			glEnd();

			glBegin(GL_LINE_LOOP);
				glNormal3f( 0.0f,	1.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d) ,		 -(goal_z) , 0);
				glVertex3f( -(goal_x + goal_d) ,	 -(goal_z) , goal_y);
				glVertex3f( -(goal_x			) ,	 -(goal_z) , goal_y);
				glVertex3f( -(goal_x			) ,		 -(goal_z) , 0);
			glEnd();
		}
	}
	else
	{

		{
			//goal
			const float goal_x = 2.700;
			const float goal_z = 0.400;
			const float goal_y = 0.300;
			const float goal_d = 0.300;
			const float goal_w = 0.008;//板の厚さ

			//white
			matr_white.setMaterial();
			glBegin(GL_QUADS);

				glNormal3f( 1.0f,	0.0f,	0.0f);
				glVertex3f( (goal_x + goal_d + goal_w) ,			-(goal_z + goal_w) , 0);
				glVertex3f( (goal_x + goal_d + goal_w) ,			 (goal_z + goal_w) , 0);
				glVertex3f( (goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d + goal_w) ,	-(goal_z + goal_w) , goal_y);

				glNormal3f( -1.0f,	0.0f,	0.0f);
				glVertex3f( (goal_x 			      ) ,	(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x 			      ) ,	(goal_z + goal_w) ,		0);
				glVertex3f( (goal_x 			      )	,	(goal_z ) ,		0);
				glVertex3f( (goal_x 			      )	,	(goal_z ) , goal_y);
				
				glNormal3f( -1.0f,	0.0f,	0.0f);
				glVertex3f( (goal_x 			      ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x 			      )	,	-(goal_z ) , goal_y);
				glVertex3f( (goal_x 			      )	,	-(goal_z ) ,		0);
				glVertex3f( (goal_x 			      ) ,	-(goal_z + goal_w) ,		0);

				glNormal3f( 0.0f,	1.0f,	0.0f);
				glVertex3f( (goal_x + goal_d + goal_w) ,			 (goal_z + goal_w) , 0);
				glVertex3f( (goal_x			         ) ,			 (goal_z + goal_w) , 0);
				glVertex3f( (goal_x			         ) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);

				glNormal3f( 0.0f,	-1.0f,	0.0f);
				glVertex3f( (goal_x + goal_d + goal_w) ,			 -(goal_z + goal_w) , 0);
				glVertex3f( (goal_x + goal_d + goal_w) ,	 -(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x			         ) ,	 -(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x			         ) ,			 -(goal_z + goal_w) , 0);

				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( (goal_x + goal_d + goal_w) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d		 ) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d		 ) ,	-(goal_z + goal_w) , goal_y);

				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( (goal_x 			      ) ,	(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x 			      )	,	(goal_z ) , goal_y);
				glVertex3f( (goal_x + goal_d +	goal_w) ,	(goal_z ) , goal_y);
				glVertex3f( (goal_x + goal_d +	goal_w) ,	(goal_z + goal_w ) , goal_y);

				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( (goal_x 			      ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( (goal_x + goal_d +	goal_w) ,	-(goal_z + goal_w ) , goal_y);
				glVertex3f( (goal_x + goal_d +	goal_w) ,	-(goal_z ) , goal_y);
				glVertex3f( (goal_x 			      )	,	-(goal_z ) , goal_y);


				glNormal3f( -1.0f,	0.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d + goal_w) ,			-(goal_z + goal_w) , 0);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d + goal_w) ,			 (goal_z + goal_w) , 0);

				glNormal3f( 1.0f,	0.0f,	0.0f);
				glVertex3f( -(goal_x 			      ) ,	(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x 			      )	,	(goal_z ) , goal_y);
				glVertex3f( -(goal_x 			      )	,	(goal_z ) ,		0);
				glVertex3f( -(goal_x 			      ) ,	(goal_z + goal_w) ,		0);

				glNormal3f( 1.0f,	0.0f,	0.0f);
				glVertex3f( -(goal_x 			      ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x 			      ) ,	-(goal_z + goal_w) ,		0);
				glVertex3f( -(goal_x 			      )	,	-(goal_z ) ,		0);
				glVertex3f( -(goal_x 			      )	,	-(goal_z ) , goal_y);

				glNormal3f( 0.0f,	1.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d + goal_w) ,			 (goal_z + goal_w) , 0);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x		          ) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x		          ) ,			 (goal_z + goal_w) , 0);

				glNormal3f( 0.0f,	-1.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d + goal_w) ,			 -(goal_z + goal_w) , 0);
				glVertex3f( -(goal_x			      ) ,			 -(goal_z + goal_w) , 0);
				glVertex3f( -(goal_x			      ) ,	 -(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	 -(goal_z + goal_w) , goal_y);

				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d		 ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d		 ) ,	 (goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d + goal_w) ,	 (goal_z + goal_w) , goal_y);

				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( -(goal_x 			      ) ,	(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x + goal_d +	goal_w) ,	(goal_z + goal_w ) , goal_y);
				glVertex3f( -(goal_x + goal_d +	goal_w) ,	(goal_z ) , goal_y);
				glVertex3f( -(goal_x 			      )	,	(goal_z ) , goal_y);

				glNormal3f( 0.0f,	0.0f,	1.0f);
				glVertex3f( -(goal_x  			      ) ,	-(goal_z + goal_w) , goal_y);
				glVertex3f( -(goal_x 			      )	,	-(goal_z ) , goal_y);
				glVertex3f( -(goal_x + goal_d +	goal_w) ,	-(goal_z ) , goal_y);
				glVertex3f( -(goal_x + goal_d +	goal_w) ,	-(goal_z + goal_w ) , goal_y);
			glEnd();


			//skyblue
			matr_skyblue.setMaterial();
			glBegin(GL_QUADS);

				glNormal3f( -1.0f,	0.0f,	0.0f);
				glVertex3f( (goal_x + goal_d) ,		-(goal_z ) , 0);
				glVertex3f( (goal_x + goal_d) ,	-(goal_z ) , goal_y);
				glVertex3f( (goal_x + goal_d) ,	 (goal_z ) , goal_y);
				glVertex3f( (goal_x + goal_d) ,		 (goal_z ) , 0);

				glNormal3f( 0.0f,	-1.0f,	0.0f);
				glVertex3f( (goal_x + goal_d) ,		 (goal_z) , 0);
				glVertex3f( (goal_x + goal_d) ,	 (goal_z) , goal_y);
				glVertex3f( (goal_x			) ,	 (goal_z) , goal_y);
				glVertex3f( (goal_x			) ,		 (goal_z) , 0);

				glNormal3f( 0.0f,	1.0f,	0.0f);
				glVertex3f( (goal_x + goal_d) ,		 -(goal_z) , 0);
				glVertex3f( (goal_x			) ,		 -(goal_z) , 0);
				glVertex3f( (goal_x			) ,	 -(goal_z) , goal_y);
				glVertex3f( (goal_x + goal_d) ,	 -(goal_z) , goal_y);
			glEnd();

			//yellow
			matr_yellow.setMaterial();
			glBegin(GL_QUADS);
				glNormal3f( 1.0f,	0.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d) ,		-(goal_z ) , 0);
				glVertex3f( -(goal_x + goal_d) ,		 (goal_z ) , 0);
				glVertex3f( -(goal_x + goal_d) ,	 (goal_z ) , goal_y);
				glVertex3f( -(goal_x + goal_d) ,	-(goal_z ) , goal_y);

				glNormal3f( 0.0f,	-1.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d) ,		 (goal_z) , 0);
				glVertex3f( -(goal_x			) ,		 (goal_z) , 0);
				glVertex3f( -(goal_x			) ,	 (goal_z) , goal_y);
				glVertex3f( -(goal_x + goal_d) ,	 (goal_z) , goal_y);

				glNormal3f( 0.0f,	1.0f,	0.0f);
				glVertex3f( -(goal_x + goal_d) ,		 -(goal_z) , 0);
				glVertex3f( -(goal_x + goal_d) ,	 -(goal_z) , goal_y);
				glVertex3f( -(goal_x			) ,	 -(goal_z) , goal_y);
				glVertex3f( -(goal_x			) ,		 -(goal_z) , 0);

			glEnd();
		}
	}
}

void CField::DrawLandmarks(bool bWireFrame)
{
	//landmark
	landmark1sp.Draw(bWireFrame);
	landmark2sp.Draw(bWireFrame);
	landmark3sp.Draw(bWireFrame);

	landmark1yp.Draw(bWireFrame);
	landmark2yp.Draw(bWireFrame);
	landmark3yp.Draw(bWireFrame);

	landmark1ps.Draw(bWireFrame);
	landmark2ps.Draw(bWireFrame);
	landmark3ps.Draw(bWireFrame);

	landmark1py.Draw(bWireFrame);
	landmark2py.Draw(bWireFrame);
	landmark3py.Draw(bWireFrame);
}


void CField::InitDisplayList()
{
	int nIndex = CBody::s_bOffscreenRender ? 1 : 0;

	if (!dispList_init_flag[nIndex])
	{
		dispList_polygon[nIndex]	= MakeDisplayList_Polygon();
		dispList_wire[nIndex]		= MakeDisplayList_Wire();

		dispList_init_flag[nIndex] = true;
	}
}

GLuint CField::MakeDisplayList_Polygon()
{
	GLuint dispList = glGenLists(1);

	return dispList;
}

GLuint CField::MakeDisplayList_Wire()
{
	GLuint dispList = glGenLists(1);

	return dispList;
}

void CField::DrawField(bool bWireFrame)
{
	if(bWireFrame)
	{
		//ground
		//	ground.Draw();
		matr_white.setMaterial();
		glEnable(GL_TEXTURE_2D);

		const int nDiv = 16;

		glBegin(GL_LINE_LOOP);
			glNormal3f(0,0,1);
			glVertex3f(-3.000,-2.000,0);
			glVertex3f(-3.000, 2.000,0);
			glVertex3f( 3.000, 2.000,0);
			glVertex3f( 3.000,-2.000,0);
		glEnd();

		for(int i = 0;i < nDiv;i++){
			glBegin(GL_LINES);
				glVertex3f(-3.000 + i * 6.000 / nDiv, -2.000, 0);
				glVertex3f(-3.000 + i * 6.000 / nDiv,  2.000, 0);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(-3.000, -2.000 + i * 4.000 / nDiv, 0);
				glVertex3f( 3.000, -2.000 + i * 4.000 / nDiv, 0);
			glEnd();
		}

		glDisable(GL_TEXTURE_2D);

		//line
		matr_white.setMaterial();
		glNormal3f(0,0,1);
		glBegin(GL_LINE_LOOP);
			glVertex3f(-2.700, -1.800, 0);
			glVertex3f( 2.700, -1.800, 0);
			glVertex3f( 2.700,  1.800, 0);
			glVertex3f(-2.700,  1.800, 0);
		glEnd();
		glBegin(GL_LINE_LOOP);
			glVertex3f(-2.725, -1.825, 0);
			glVertex3f( 2.725, -1.825, 0);
			glVertex3f( 2.725,  1.825, 0);
			glVertex3f(-2.725,  1.825, 0);
		glEnd();
		glBegin(GL_LINES);
			glVertex3f(-0.0125, -1.800, 0);
			glVertex3f(-0.0125,  1.800, 0);
			glVertex3f( 0.0125, -1.800, 0);
			glVertex3f( 0.0125,  1.800, 0);
		glEnd();

		const int nDiv2 = 16;
		static float sins[nDiv2];
		static float coss[nDiv2];

		static bool init_flag = false;
		if(!init_flag)
		{
			//sin & cos init
			for(int i = 0; i < nDiv2; i++){
				sins[i] = sin(2*M_PI * i / nDiv);
				coss[i] = cos(2*M_PI * i / nDiv);
			}
			init_flag = true;
		}

		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < nDiv2;i++)
		{
			glVertex3f(coss[i]*0.180, sins[i]*0.180, 0);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		for(int i = 0; i < nDiv2;i++)
		{
			glVertex3f(coss[i]*0.1925, sins[i]*0.1925, 0);
		}
		glEnd();
	}
	else
	{
		//texture

		int nIndex = CBody::s_bOffscreenRender ? 1 : 0;

		static bool texture_init_flag[2] = {false, false};
		static GLuint field_texture[2];
		static GLuint kim_texture[2];
		static Texture field_texdata[2];
		static Texture kim_texdata[2];


		if (!texture_init_flag[nIndex])
		{

			PPMReader::ReadFile("texture\\ground.ppm", field_texdata[nIndex]);
			glGenTextures(1, &field_texture[nIndex]);
			glBindTexture(GL_TEXTURE_2D, field_texture[nIndex]);

			PPMReader::ReadFile("texture\\araibo.ppm", kim_texdata[nIndex]);
			glGenTextures(1, &kim_texture[nIndex]);
			glBindTexture(GL_TEXTURE_2D, kim_texture[nIndex]);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			texture_init_flag[nIndex] = true;
		}


		//ground

		//	ground.Draw();

		//Box
		
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		matr_white.setMaterial();
		glPushMatrix();
		glTranslatef(0.0 , 0.0, 2.0);
		DrawBoxRev(9.0, 6.0, 4.0);

		glPopMatrix();

		glDisable(GL_DEPTH_TEST);
		


		matr_white.setMaterial();
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, field_texture[nIndex]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
			field_texdata[nIndex].width, 
			field_texdata[nIndex].height, 
			0, GL_RGB, GL_UNSIGNED_BYTE, 
			field_texdata[nIndex].data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		const double texture_size = 0.500;
		const double texture_repeat_x = 6.0 / texture_size;
		const double texture_repeat_y = 4.0 / texture_size;
		const int nDivx = 6.0 / texture_size;
		const int nDivy = 4.0 / texture_size;


		glNormal3f(0,0,1);
		glBegin(GL_QUADS);
		for(int x = 0; x < nDivx; x++){
			for(int y = 0;y < nDivy; y++){

				glTexCoord2f(0.0,	0.0);
				glVertex3f(-3.000 + (x + 0) * texture_size, -2.000 + (y + 0) * texture_size, 0);

				glTexCoord2f(1.0,	0.0);
				glVertex3f(-3.000 + (x + 1) * texture_size, -2.000 + (y + 0) * texture_size, 0);

				glTexCoord2f(1.0,	1.0);
				glVertex3f(-3.000 + (x + 1) * texture_size, -2.000 + (y + 1) * texture_size, 0);

				glTexCoord2f(0.0, 1.0);
				glVertex3f(-3.000 + (x + 0) * texture_size, -2.000 + (y + 1) * texture_size, 0);

			}
		}
		glEnd();
		glDisable(GL_TEXTURE_2D);

		//lines
		glNormal3f(0,0,1);
		glDisable(GL_LIGHTING);
		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_QUADS);
		{
			glVertex3f( -2.725, -1.800, 0);
			glVertex3f( -2.725, -1.825, 0);
			glVertex3f(  2.725, -1.825, 0);
			glVertex3f(  2.725, -1.800, 0);

			glVertex3f( -2.725,  1.800, 0);
			glVertex3f(  2.725,  1.800, 0);
			glVertex3f(  2.725,  1.825, 0);
			glVertex3f( -2.725,  1.825, 0);

			glVertex3f( -2.700,  1.825, 0);
			glVertex3f( -2.725,  1.825, 0);
			glVertex3f( -2.725, -1.825, 0);
			glVertex3f( -2.700, -1.825, 0);

			glVertex3f(  2.700,  1.825, 0);
			glVertex3f(  2.700, -1.825, 0);
			glVertex3f(  2.725, -1.825, 0);
			glVertex3f(  2.725,  1.825, 0);

			//goal line
			glVertex3f( -2.050, -0.675, 0);
			glVertex3f( -2.050, -0.650, 0);
			glVertex3f( -2.725, -0.650, 0);
			glVertex3f( -2.725, -0.675, 0);

			glVertex3f( -2.050,  0.675, 0);
			glVertex3f( -2.725,  0.675, 0);
			glVertex3f( -2.725,  0.650, 0);
			glVertex3f( -2.050,  0.650, 0);

			glVertex3f( -2.050,  0.675, 0);
			glVertex3f( -2.075,  0.675, 0);
			glVertex3f( -2.075, -0.675, 0);
			glVertex3f( -2.050, -0.675, 0);

			glVertex3f(  2.050, -0.675, 0);
			glVertex3f(  2.725, -0.675, 0);
			glVertex3f(  2.725, -0.650, 0);
			glVertex3f(  2.050, -0.650, 0);

			glVertex3f(  2.050,  0.675, 0);
			glVertex3f(  2.050,  0.650, 0);
			glVertex3f(  2.725,  0.650, 0);
			glVertex3f(  2.725,  0.675, 0);

			glVertex3f(  2.050,  0.675, 0);
			glVertex3f(  2.050, -0.675, 0);
			glVertex3f(  2.075, -0.675, 0);
			glVertex3f(  2.075,  0.675, 0);

			//center line
			glVertex3f(  0.0125,  1.825, 0);
			glVertex3f( -0.0125,  1.825, 0);
			glVertex3f( -0.0125, -1.825, 0);
			glVertex3f(  0.0125, -1.825, 0);

			//center circle
			const int nDiv = 32;
			static bool init = false;
			static double sins[nDiv];
			static double coss[nDiv];
			if (init == false)
			{
				for (int i = 0; i < nDiv;i++)
				{
					sins[i] = sin(i * 2*M_PI / nDiv);
					coss[i] = cos(i * 2*M_PI / nDiv);
				}
			}

			for (int i = 0;i < nDiv; i++)
			{
				const double in  = 0.180 - 0.0125;
				const double out = 0.180 + 0.0125;
				int j = (i + 1) % nDiv;
				glVertex3f( in*coss[i],  in*sins[i], 0);
				glVertex3f(out*coss[i], out*sins[i], 0);
				glVertex3f(out*coss[j], out*sins[j], 0);
				glVertex3f( in*coss[j],  in*sins[j], 0);
			}
		}
		glEnd();
		glEnable(GL_LIGHTING);


		if (nIndex == 0)
		{

			glEnable(GL_DEPTH_TEST);


			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, kim_texture[nIndex]);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
				kim_texdata[nIndex].width, 
				kim_texdata[nIndex].height, 
				0, GL_RGB, GL_UNSIGNED_BYTE, 
				kim_texdata[nIndex].data);

			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glBegin(GL_QUADS);
			{

				float z = 0.500;
				float w = 0.800 / 2;
				for ( int i = 0;i < 5; i++)
				{

					float q = 1.5;
					float x = (-2 + i) * q;

					glNormal3f(0,-1,0);
					glTexCoord2f(0.0,	1.0);
					glVertex3f(-w + x, 2.9, z-w);			

					glTexCoord2f(1.0,	1.0);
					glVertex3f( w + x, 2.9, z-w);			

					glTexCoord2f(1.0,	0.0);
					glVertex3f( w + x, 2.9, z+w);			

					glTexCoord2f(0.0,	0.0);
					glVertex3f(-w + x, 2.9, z+w);			
				}

			}
			glEnd();
			glDisable(GL_TEXTURE_2D);
		}



	}


}
