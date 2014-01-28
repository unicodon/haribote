#pragma once
#include "body.h"
#include "Material.h"

class CPrimitiveBody :
	public CBody
{
public:
	CPrimitiveBody(void);
	~CPrimitiveBody(void);

	CMaterial material;
};
