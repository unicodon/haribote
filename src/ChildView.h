// ChildView.h : CChildView �N���X�̃C���^�[�t�F�C�X
//


#pragma once

#include "Main/Camera.h"
#include "Main/Field.h"
#include "Main/Ball.h"
#include "Main/Missile.h"
#include "Main/Robot.h"
#include "Main/CglDIB.h"

#include "Main/MySock.h"

#include "AiboView.h"
#include "DebugView.h"

// CChildView �E�B���h�E

#define MISSILE_NUM 1

class CChildView : public CWnd
{
//�O���N���X�̃C���X�^���X
private:
	CMySock sock;

	void ChangeRenderContext(bool bRenderOffscreen);

	void MakeWindow();
	CglDIB render;

	CAiboView	*pAiboViewWnd;
	CDebugView	*pDebugWnd;

	CCamera camera[2];
	int		current_cam;

	static CField field;
	static CBall ball;
	static CRobot robot;

	static CMissile missile[MISSILE_NUM];

	static dWorldID world;//�U�E���[���h
	static dSpaceID space;//���[�g�X�y�[�X
	static dJointGroupID contactgroup;

	static void nearCallback(void *data, dGeomID o1, dGeomID o2);
	static void DoOneStep();

	//������`��
	GLuint GenerateBitmapListForFont( char* fontname );
	void GLTextOut(const char* const textstring );

	GLuint id1;

	bool bStop;

	void SetupLighting();

// �R���X�g���N�V����
public:
	CChildView();

// ����
public:
	static int nStepsPerFrame;
	static double dSecondPerStep;
	static int nMiliSecondPerFrame;

	static double dTime;

// ����
public:

// �I�[�o�[���C�h
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	virtual ~CChildView();

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

	HGLRC m_hRC;
	CDC* m_pDC;

public:
	BOOL RenderScene( void );
	BOOL RenderScene2();

private:
	BOOL SetupPixelFormat( void );
	BOOL InitializeOpenGL( void );
	BOOL InitializeODE( void );
	BOOL FinalizeODE( void );

	void ShowError(int e);

	int screen_width;
	int screen_height;

	void Capture();

	bool	bLookObject;
	dBodyID TargetBody;

	bool	bSmooth;

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnToggleWireframeRobot();
	afx_msg void OnTogglegeommesh();
	afx_msg void OnStopGo();
	afx_msg void OnOpenMotion();
};

