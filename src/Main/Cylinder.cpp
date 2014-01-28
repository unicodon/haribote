#include "StdAfx.h"
#include ".\cylinder.h"

CCylinder::CCylinder(void)
{
	dispList_init_flag[0] =
	dispList_init_flag[1] =	false;

	radius = 1.0;
	height = 1.0;
}

CCylinder::~CCylinder(void)
{
}

CCylinder::CCylinder(double rad, double hei)
{
	dispList_init_flag[0] =
	dispList_init_flag[1] =	false;

	radius = rad;
	height = hei;
}

GLuint CCylinder::MakeDisplayList_Polygon()
{
	GLuint dispList = glGenLists(1);

	const int nDiv = 16;
	float sins[nDiv];
	float coss[nDiv];

	//sin & cos init
	for(int i = 0; i < nDiv; i++){
		sins[i] = sin(2*M_PI * i / nDiv);
		coss[i] = cos(2*M_PI * i / nDiv);
	}

	float rad = radius;
	float hei = height;

	material.setMaterial();

	glNewList(dispList, GL_COMPILE);
	{		
		glBegin(GL_POLYGON);
		glNormal3f(0, -1, 0);
		for(int i = 0;i < nDiv;i++){
			float x = coss[i];
			float y = sins[i];
			glVertex3f(x*rad,-hei,y*rad);
		}
		glEnd();

		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		for(int i = 0;i < nDiv;i++){
			float x = coss[nDiv - i - 1];
			float y = sins[nDiv - i - 1];
			glVertex3f(x*rad,0,y*rad);
		}
		glEnd();

		for(int i = 0;i < nDiv;i++){
			float x = coss[i];
			float y = sins[i];
			float x2 = coss[(i+1)%nDiv];
			float y2 = sins[(i+1)%nDiv];
			glBegin(GL_POLYGON);
				glNormal3f(x    ,   0,	   y);
				glVertex3f(x*rad,-hei, y*rad);
				glVertex3f(x*rad,   0, y*rad);
				glNormal3f(x2   ,   0,    y2);
				glVertex3f(x2*rad,   0, y2*rad);
				glVertex3f(x2*rad,-hei, y2*rad);
			glEnd();
		}
	}
	glEndList();

	return dispList;
}

GLuint CCylinder::MakeDisplayList_Wire()
{
	GLuint dispList = glGenLists(1);
	const int nDiv = 16;
	float sins[nDiv];
	float coss[nDiv];

	//sin & cos init
	for(int i = 0; i < nDiv; i++){
		sins[i] = sin(2*M_PI * i / nDiv);
		coss[i] = cos(2*M_PI * i / nDiv);
	}

	float rad = radius;
	float hei = height;

	material.setMaterial();

	glNewList(dispList, GL_COMPILE);
	{		
		glBegin(GL_LINE_LOOP);
		glNormal3f(0, -1, 0);
		for(int i = 0;i < nDiv;i++){
			float x = coss[i];
			float y = sins[i];
			glVertex3f(x*rad,-hei,y*rad);
		}
		glEnd();

		glBegin(GL_LINE_LOOP);
		glNormal3f(0, 1, 0);
		for(int i = 0;i < nDiv;i++){
			float x = coss[i];
			float y = sins[i];
			glVertex3f(x*rad,0,y*rad);
		}
		glEnd();

		for(int i = 0;i < nDiv;i++){
			float x = coss[i];
			float y = sins[i];
			glBegin(GL_LINE_LOOP);
				glNormal3f(x,		0,     y);
				glVertex3f(x*rad,-hei, y*rad);
				glVertex3f(x*rad,   0, y*rad);
			glEnd();
		}
	}
	glEndList();

	return dispList;
}


void CCylinder::SetSize(double rad, double hei)
{
	dispList_init_flag[0] =
	dispList_init_flag[1] =	false;
	radius = rad;
	height = hei;
}

void CCylinder::InitDisplayList()
{
	int nIndex = s_bOffscreenRender ? 1 : 0;

	if (!dispList_init_flag[nIndex])
	{
		dispList_polygon[nIndex]	= MakeDisplayList_Polygon();
		dispList_wire[nIndex]		= MakeDisplayList_Wire();

		dispList_init_flag[nIndex] = true;
	}
}

void CCylinder::Draw(bool bWireFrame)
{
	int nIndex = s_bOffscreenRender ? 1 : 0;

	if(!dispList_init_flag[nIndex])
	{
		InitDisplayList();
	}
	
	glPushMatrix();
	glMultMatrixd(position);
	material.setMaterial();
	if(bWireFrame)
	{
		glCallList(dispList_wire[nIndex]);
	}
	else
	{
		glCallList(dispList_polygon[nIndex]);
	}
	glPopMatrix();
}

void CCylinder::DrawWire(double rad, double hei)
{
	const int nDiv = 16;
	static float sins[nDiv];
	static float coss[nDiv];

	static bool init_flag = false;
	static GLuint dispList;
	if(!init_flag)
	{
		//sin & cos init
		for(int i = 0; i < nDiv; i++){
			sins[i] = sin(2*M_PI * i / nDiv);
			coss[i] = cos(2*M_PI * i / nDiv);
		}

		dispList = glGenLists(1);

		glNewList(dispList, GL_COMPILE);
		{		
			glBegin(GL_LINE_LOOP);
			glNormal3f(0, -1, 0);
			for(int i = 0;i < nDiv;i++){
				float x = coss[i];
				float y = sins[i];
				glVertex3f(x,-1,y);
			}
			glEnd();

			glBegin(GL_LINE_LOOP);
			glNormal3f(0, 1, 0);
			for(int i = 0;i < nDiv;i++){
				float x = coss[i];
				float y = sins[i];
				glVertex3f(x,0,y);
			}
			glEnd();

			for(int i = 0;i < nDiv;i++){
				float x = coss[i];
				float y = sins[i];
				glBegin(GL_LINE_LOOP);
					glNormal3f(x, 0, y);
					glVertex3f(x,-1,y);
					glVertex3f(x,0,y);
				glEnd();
			}
		}
		glEndList();

		init_flag = true;
	}

	glPushMatrix();
	glScalef(rad, hei, rad);
	glCallList(dispList);
	glPopMatrix();
}