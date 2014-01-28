#include "StdAfx.h"
#include ".\box.h"

CBox::CBox(void)
{
	cx = 1.0; cy = 1.0; cz = 1.0;
}

CBox::~CBox(void)
{
}

CBox::CBox(double cx_, double cy_, double cz_){
	cx = cx_; cy = cy_; cz = cz_;
}

void CBox::SetSize(double cx_, double cy_, double cz_){
	cx = cx_; cy = cy_; cz = cz_;
}

void CBox::Draw(bool bWireFrame)
{
	glPushMatrix();
	glMultMatrixd(position);
	material.setMaterial();
	DrawBox(cx,cy,cz);
	glPopMatrix();
}