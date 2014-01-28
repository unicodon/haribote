#pragma once

class CCamera
{
public:
	CCamera(void);
	~CCamera(void);

	GLfloat position[3];//���_
	GLfloat tilt,pan,roll;//����

	GLdouble zNear, zFar;
	GLdouble fov;//����p
	GLdouble aspect;//�A�X�y�N�g��
public:
	void SetViewMatrix(bool bLookObject = false, dBodyID body = 0);
	void SetAspect(GLdouble new_aspect);

	void Move(GLdouble x, GLdouble y, GLdouble z);

	void AddTilt(GLdouble delta);
	void AddPan(GLdouble delta);
	void AddRoll(GLdouble delta);

	void Move(int dir, dBodyID target);
};
