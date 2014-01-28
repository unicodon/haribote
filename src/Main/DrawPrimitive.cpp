#include "StdAfx.h"
#include "DrawPrimitive.h"

void DrawCylinder(float radius, float length, bool bDrawWire)
{
	const int nDiv = 24;

	double sins[nDiv];
	double coss[nDiv];
	for (int i = 0;i < nDiv; i++)
	{
		sins[i] = sin(2 * M_PI * i/ nDiv); 
		coss[i] = cos(2 * M_PI * i/ nDiv); 
	}

	GLfloat x;
	GLfloat y;
	GLfloat z;

	if (bDrawWire)
	{
		for (int i = 0; i < nDiv /2; i++)
		{
			glBegin(GL_LINE_LOOP);
			z = coss[i];
			for (int j = 0; j < nDiv; j++)
			{
				x = coss[j] * sins[i];
				y = sins[j] * sins[i];
				
				glNormal3f(x, y, z);
				glVertex3f(x * radius, y * radius, z * radius);
			}
			glEnd();
		}

		for (int j = 0; j < nDiv; j++)
		{
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < nDiv /2 + 1; i++)
			{
			z = coss[i];
			x = coss[j] * sins[i];
			y = sins[j] * sins[i];
				
				glNormal3f(x, y, z);
				glVertex3f(x * radius, y * radius, z * radius);
			}
			glEnd();
		}
	}
	else
	{
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j < nDiv + 1; j++)
		{
				int k = ( j + 1 ) % nDiv;

				z = 0.5;
				x = coss[k];
				y = sins[k];
				glNormal3f(x, y, 0);
				glVertex3f(x * radius, y * radius, z * length);					

				z = -0.5;
				x = coss[j];
				y = sins[j];
				glNormal3f(x, y, 0);
				glVertex3f(x * radius, y * radius, z * length);					
		}
		glEnd();
	}
}


void DrawSphere(float radius, bool bDrawWire)
{
	const int nDiv = 24;

	double sins[nDiv];
	double coss[nDiv];
	for (int i = 0;i < nDiv; i++)
	{
		sins[i] = sin(2 * M_PI * i/ nDiv); 
		coss[i] = cos(2 * M_PI * i/ nDiv); 
	}

	GLfloat x;
	GLfloat y;
	GLfloat z;

	if (bDrawWire)
	{
		for (int i = 0; i < nDiv /2; i++)
		{
			glBegin(GL_LINE_LOOP);
			z = coss[i];
			for (int j = 0; j < nDiv; j++)
			{
				x = coss[j] * sins[i];
				y = sins[j] * sins[i];
				
				glNormal3f(x, y, z);
				glVertex3f(x * radius, y * radius, z * radius);
			}
			glEnd();
		}

		for (int j = 0; j < nDiv; j++)
		{
			glBegin(GL_LINE_STRIP);
			for (int i = 0; i < nDiv /2 + 1; i++)
			{
			z = coss[i];
			x = coss[j] * sins[i];
			y = sins[j] * sins[i];
				
				glNormal3f(x, y, z);
				glVertex3f(x * radius, y * radius, z * radius);
			}
			glEnd();
		}
	}
	else
	{
		for (int j = 0; j < nDiv; j++)
		{
			glBegin(GL_QUAD_STRIP);
			for (int i = 0; i < nDiv /2 + 1; i++)
			{
				int k = ( j + 1 ) % nDiv;

					z = coss[i];
					x = coss[k] * sins[i];
					y = sins[k] * sins[i];
					glNormal3f(x, y, z);
					glVertex3f(x * radius, y * radius, z * radius);					

					z = coss[i];
					x = coss[j] * sins[i];
					y = sins[j] * sins[i];
					glNormal3f(x, y, z);
					glVertex3f(x * radius, y * radius, z * radius);					
			}
			glEnd();
		}
	}
}

void DrawCube(float length, bool bDrawWire)
{
	DrawBox(length, length, length, bDrawWire);
}

void DrawBoxRev(float cx, float cy, float cz, bool bDrawWire)
{
	cx /= 2;
	cy /= 2;
	cz /= 2;

	glBegin(GL_QUADS);
		glNormal3f(   0,   0,  -1);
		glVertex3f(  cx,  cy,  cz);
		glVertex3f(  cx, -cy,  cz);
		glVertex3f( -cx, -cy,  cz);
		glVertex3f( -cx,  cy,  cz);

		glNormal3f(   0,   0,   1);
		glVertex3f(  cx,  cy, -cz);
		glVertex3f( -cx,  cy, -cz);
		glVertex3f( -cx, -cy, -cz);
		glVertex3f(  cx, -cy, -cz);

		glNormal3f(  -1,   0,   0);
		glVertex3f(  cx,  cy,  cz);
		glVertex3f(  cx,  cy, -cz);
		glVertex3f(  cx, -cy, -cz);
		glVertex3f(  cx, -cy,  cz);

		glNormal3f(   1,   0,   0);
		glVertex3f( -cx,  cy,  cz);
		glVertex3f( -cx, -cy,  cz);
		glVertex3f( -cx, -cy, -cz);
		glVertex3f( -cx,  cy, -cz);
		
		glNormal3f(   0,  -1,   0);
		glVertex3f(  cx,  cy,  cz);
		glVertex3f( -cx,  cy,  cz);
		glVertex3f( -cx,  cy, -cz);
		glVertex3f(  cx,  cy, -cz);

		glNormal3f(   0,   1,   0);
		glVertex3f(  cx, -cy,  cz);
		glVertex3f(  cx, -cy, -cz);
		glVertex3f( -cx, -cy, -cz);
		glVertex3f( -cx, -cy,  cz);

	glEnd();
}


void DrawBox(float cx, float cy, float cz, bool bDrawWire)
{
	cx /= 2;
	cy /= 2;
	cz /= 2;

	glBegin(GL_QUADS);
		glNormal3f(   0,   0,   1);
		glVertex3f(  cx,  cy,  cz);
		glVertex3f( -cx,  cy,  cz);
		glVertex3f( -cx, -cy,  cz);
		glVertex3f(  cx, -cy,  cz);

		glNormal3f(   0,   0,  -1);
		glVertex3f(  cx,  cy, -cz);
		glVertex3f(  cx, -cy, -cz);
		glVertex3f( -cx, -cy, -cz);
		glVertex3f( -cx,  cy, -cz);

		glNormal3f(   1,   0,   0);
		glVertex3f(  cx,  cy,  cz);
		glVertex3f(  cx, -cy,  cz);
		glVertex3f(  cx, -cy, -cz);
		glVertex3f(  cx,  cy, -cz);

		glNormal3f(  -1,   0,   0);
		glVertex3f( -cx,  cy,  cz);
		glVertex3f( -cx,  cy, -cz);
		glVertex3f( -cx, -cy, -cz);
		glVertex3f( -cx, -cy,  cz);
		
		glNormal3f(   0,   1,   0);
		glVertex3f(  cx,  cy,  cz);
		glVertex3f(  cx,  cy, -cz);
		glVertex3f( -cx,  cy, -cz);
		glVertex3f( -cx,  cy,  cz);

		glNormal3f(   0,  -1,   0);
		glVertex3f(  cx, -cy,  cz);
		glVertex3f( -cx, -cy,  cz);
		glVertex3f( -cx, -cy, -cz);
		glVertex3f(  cx, -cy, -cz);


	glEnd();
}

void DrawCone(float radius, float height, bool bDrawWire)
{
}

void DrawOctahedron(float length, bool bDrawWire)
{
	GLfloat x;
	GLfloat y;
	GLfloat z;

	float s3 = sqrt(1 / 3.0);
	float  l = length;

	glBegin(GL_TRIANGLES);
		glNormal3f(  s3,  s3,  s3);
		glVertex3f(   0,   0,   l);
		glVertex3f(   l,   0,   0);
		glVertex3f(   0,   l,   0);

		glNormal3f(  s3, -s3,  s3);
		glVertex3f(   0,   0,   l);
		glVertex3f(   0,  -l,   0);
		glVertex3f(   l,   0,   0);

		glNormal3f( -s3, -s3,  s3);
		glVertex3f(   0,   0,   l);
		glVertex3f(  -l,   0,   0);
		glVertex3f(   0,  -l,   0);

		glNormal3f( -s3,  s3,  s3);
		glVertex3f(   0,   0,   l);
		glVertex3f(   0,   l,   0);
		glVertex3f(  -l,   0,   0);

		glNormal3f(  s3,  s3, -s3);
		glVertex3f(   0,   0,  -l);
		glVertex3f(   0,   l,   0);
		glVertex3f(   l,   0,   0);

		glNormal3f(  s3, -s3, -s3);
		glVertex3f(   0,   0,  -l);
		glVertex3f(   l,   0,   0);
		glVertex3f(   0,  -l,   0);

		glNormal3f( -s3, -s3, -s3);
		glVertex3f(   0,   0,  -l);
		glVertex3f(   0,  -l,   0);
		glVertex3f(  -l,   0,   0);

		glNormal3f( -s3,  s3, -s3);
		glVertex3f(   0,   0,  -l);
		glVertex3f(  -l,   0,   0);
		glVertex3f(   0,   l,   0);

	glEnd();
}
