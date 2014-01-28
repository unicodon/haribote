#pragma once

struct CMaterial
{
public:
	CMaterial(void);
	~CMaterial(void);

	void SetDiffuse(float* val);
	void SetSpecular(float* val);
	void SetAmbient(float* val);
	void SetShininess(float val);

	void SetValues(float* dif, float* spec, float* amb, float shin);

	void setMaterial();


	float diffuse[4];
	float specular[4];
	float ambient[4];
	float shininess;
};
