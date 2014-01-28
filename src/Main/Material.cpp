#include "StdAfx.h"
#include ".\material.h"

CMaterial::CMaterial(void)
{
	float val[4] = {0.5f,0.5f,0.5f,1.0f};
	SetSpecular(val);
	SetDiffuse(val);
	SetAmbient(val);
	SetShininess(20.0f);
}

CMaterial::~CMaterial(void)
{
}

void CMaterial::SetDiffuse(float* val)
{
	memcpy(diffuse, val, 4*sizeof(float));
}

void CMaterial::SetSpecular(float* val)
{
	memcpy(specular, val, 4*sizeof(float));
}

void CMaterial::SetAmbient(float* val)
{
	memcpy(ambient, val, 4*sizeof(float));
}

void CMaterial::SetShininess(float val)
{
	shininess = val;
}

void CMaterial::SetValues(float* dif, float* spec, float* amb, float shin)
{
	SetDiffuse(dif);
	SetSpecular(spec);
	SetAmbient(amb);
	SetShininess(shin);
}

void CMaterial::setMaterial()
{
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

