#include "StdAfx.h"
#include ".\body.h"

bool CBody::bDrawArrow			= true;
bool CBody::s_bOffscreenRender	= false;
CBody::CBody(void)
{
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, position);
	glPopMatrix();
}

CBody::~CBody(void)
{
}
