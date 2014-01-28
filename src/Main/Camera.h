#pragma once

class CCamera
{
public:
	CCamera(void);
	~CCamera(void);

	GLfloat position[3];//視点
	GLfloat tilt,pan,roll;//向き

	GLdouble zNear, zFar;
	GLdouble fov;//視野角
	GLdouble aspect;//アスペクト比
public:
	void SetViewMatrix(bool bLookObject = false, dBodyID body = 0);
	void SetAspect(GLdouble new_aspect);

	void Move(GLdouble x, GLdouble y, GLdouble z);

	void AddTilt(GLdouble delta);
	void AddPan(GLdouble delta);
	void AddRoll(GLdouble delta);

	void Move(int dir, dBodyID target);
};
