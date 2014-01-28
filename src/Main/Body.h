#pragma once

#include "Material.h"
#include "DrawPrimitive.h"


class CBody
{
public:
	double position[16];
	dSpaceID space;
	dWorldID world;

	static bool bDrawArrow;
	static bool s_bOffscreenRender;

public:
	CBody(void);
	virtual ~CBody(void);

	virtual void Draw(bool bWireFrame = false) = 0;

};
