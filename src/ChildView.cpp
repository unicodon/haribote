// ChildView.cpp : CChildView クラスの実装
//

#include "stdafx.h"
#include "HariboteServer.h"
#include "ChildView.h"
#include ".\childview.h"
#include "Main/Mesh.h"
#include "Motion/GaitMaker.h"
#include "Motion/MotionMaker.h"
#include "Main/DIBUtil.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ENABLE_MISSILE 1
#define ENABLE_CAPTURE 0

extern CMaterial matr_white;
extern bool bTorqueFlag;

extern void doCMOSFilter(BYTE* oriimg);

////////////////////////////////////////////////////////////////////
//任意の平面へ影を射影する行列構築関数
void projectShadowMatrix(double *m,				//作成する行列のポインタ
						 double plane[4],		//射影する表面の平面方程式
						 float light[4]			//光源の同時座標値
						 ){
	float dot = plane[0]*light[0] + plane[1]*light[1] +
				plane[2]*light[2] + plane[3]*light[3];

	m[0]  = dot - light[0]*plane[0];
	m[4]  =     - light[0]*plane[1];
	m[8]  =     - light[0]*plane[2];
	m[12] =     - light[0]*plane[3];

	m[1]  =     - light[1]*plane[0];
	m[5]  = dot - light[1]*plane[1];
	m[9]  =     - light[1]*plane[2];
	m[13] =     - light[1]*plane[3];

	m[2]  =     - light[2]*plane[0];
	m[6]  =     - light[2]*plane[1];
	m[10] = dot - light[2]*plane[2];
	m[14] =     - light[2]*plane[3];

	m[3]  =     - light[3]*plane[0];
	m[7]  =     - light[3]*plane[1];
	m[11] =     - light[3]*plane[2];
	m[15] = dot - light[3]*plane[3];
}

GLdouble drop_shadow_mat[16];
GLdouble drop_shadow_mat_Light[4][16];


dWorldID CChildView::world;//ザ・ワールド
dSpaceID CChildView::space;//ルートスペース
dJointGroupID CChildView::contactgroup;

GLfloat Xrot = 0, Yrot = 0;

CField		CChildView::field;
CBall		CChildView::ball;
CRobot		CChildView::robot;
CMissile	CChildView::missile[MISSILE_NUM];

static int targetID = B_NECK_ROOT;

int		CChildView::nStepsPerFrame		= 20;
double	CChildView::dSecondPerStep		= 0.001;
int 	CChildView::nMiliSecondPerFrame	= 15;

double	CChildView::dTime				= 0.0;

bool bRec = false;

const char* ErrorStrings[] = {
	{"No Error"},				   // 0
	{"Unable to get a CD"},		 // 1
	{"ChoosePixelFormat failed"},   // 2
	{"SelectPixelFormat failed"},   // 3
	{"wglCreateContext failed"},	// 4
	{"wglMakeCurrect failed"},	  // 5
	{"wglDeleteCotext failed"},	 // 6
	{"SwapBuffer failed"},		  // 7
};

const int NUM_FIXED_CAM_POS = 3;
static int fixed_cam_index = 0; 
static float fixed_cam_pos[NUM_FIXED_CAM_POS][6] = 
{
	{-2.8000, 0.0, 0.0500,   0.0 , 0.0,  0.0},
	{ 2.8000, 0.0, 0.0500,   0.0 , 0.0,  0.0},
	{ 0.1, 0.0, 0.100,   0.0 , 0.0,  0.10}
};

void CChildView::ShowError(int e){
	AfxMessageBox(ErrorStrings[e]);
}

void CChildView::DoOneStep()
{
	dReal dt = dSecondPerStep;
	dSpaceCollide (space,0,&(nearCallback));
	robot.AddTorque(dt);
	ball.AddForce(dt);

#if ENABLE_MISSILE
	for (int i = 0; i < MISSILE_NUM;i++)
	{
		missile[i].AddForce(dt);
	}
#endif

	dWorldQuickStep (world,dt);
	dJointGroupEmpty (contactgroup);
	dTime += dt;
}

void CChildView::OnTimer(UINT nIDEvent)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	if(nIDEvent == 1){
		if (!bStop)
		{
			for(int i = 0;i < nStepsPerFrame;i ++){
				DoOneStep();
			}
		}

		Invalidate();
		
#if ENABLE_CAPTURE
		static int cnt = 0;
		static double lasttime = 0;
		const double interv = 0.0333;
//		const double interv = 0.02;
		if(dTime - lasttime > interv)
		{
			if(bRec)
				Capture();
			cnt = 0;
			lasttime += interv;
		}
#endif

	}
	CWnd::OnTimer(nIDEvent);
}

BOOL CChildView::InitializeODE()
{
	world = dWorldCreate();
	space = dHashSpaceCreate (0);
	contactgroup = dJointGroupCreate (0);
	dWorldSetGravity (world,0,0,-9.8);
	dWorldSetERP(world,0.3);
	dWorldSetCFM(world,1e-3);

	return TRUE;
}

void CChildView::nearCallback (void *data, dGeomID o1, dGeomID o2)
{
	if (dGeomIsSpace(o1) || dGeomIsSpace(o2))
	{
		dSpaceCollide2( o1, o2, data, &nearCallback);

		if( dGeomIsSpace(o1)) dSpaceCollide((dSpaceID)o1, data, &nearCallback);
		if( dGeomIsSpace(o2)) dSpaceCollide((dSpaceID)o2, data, &nearCallback);
	}
	else
	{
		if(dGeomGetSpace(o1) == robot.space && dGeomGetSpace(o2) == robot.space)
		{
			return ;
		}

		int i,n;

		const int N = 30;
		dContact contact[N];
		n = dCollide (o1,o2,N,&contact[0].geom,sizeof(dContact));
		if (n > 0) {
			for (i=0; i<n; i++) {
				contact[i].surface.mode = dContactApprox1 | dContactBounce;
				contact[i].surface.mu = 3.0e-1;
				contact[i].surface.mu2 = 3.0e-1;
				contact[i].surface.slip1 = 0.3;
				contact[i].surface.slip2 = 0.3;
				contact[i].surface.soft_erp = 0.4;
				contact[i].surface.soft_cfm = 1.0e-2;
				contact[i].surface.bounce = 0.2;
				dJointID c = dJointCreateContact (world,contactgroup,&contact[i]);
				dBodyID b1 = dGeomGetBody(o1);
				dBodyID b2 = dGeomGetBody(o2);
				dJointAttach (c,b1,b2);
			}
		}
	}
}

// CChildView

CChildView::CChildView() :m_hRC(0),m_pDC(0),bStop(false),bSmooth(true)
{
	bLookObject = true;
	TargetBody  = 0;
	current_cam = 0;

	PresetWalkParam::Init();

}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_TIMER()
	ON_COMMAND(ID_TOGGLE_WIREFRAME_ROBOT, OnToggleWireframeRobot)
	ON_COMMAND(ID_TOGGLEGEOMMESH, OnTogglegeommesh)
	ON_COMMAND(ID_STOP_GO, OnStopGo)
	ON_COMMAND(ID_OPEN_MOTION, OnOpenMotion)
END_MESSAGE_MAP()

void CChildView::ChangeRenderContext(bool bRenderOffscreen)
{
	if (bRenderOffscreen)
	{
		wglMakeCurrent(render.m_hDC, render.m_hRC);
		CBody::s_bOffscreenRender = true;
	}
	else
	{
		CDC *pDC = this->GetDC();
		wglMakeCurrent(pDC->GetSafeHdc(), m_hRC);
		this->ReleaseDC(pDC);
		CBody::s_bOffscreenRender = false;
	}
}

// CChildView メッセージ ハンドラ

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style |= WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CMainFrame* pParent = (CMainFrame*)GetParentFrame();
	const char text_gamestate[][64]	= {
		"INITIAL",
		"READY",
		"SET",
		"PLAYING",
		"FINISHED"
	};
	pParent->SetStatusText(text_gamestate[state.game.game_state]);

	static int debugcnt = 0;
	if (debugcnt++ > 10)
	{
		debugcnt = 0;

		CDC* pDC = pDebugWnd->GetDC();

		CRect rect;
		pDebugWnd->GetClientRect(rect);
		pDC->FillSolidRect(rect, RGB(0,0,0));
		pDC->SetTextColor(RGB(0,200,0));

		int dy = 20;
		int y  = 0 - dy;
		int x  = 0;
		CString str;

		str.Format("Theta: %f ", state.self.accelaration.theta);
		pDC->TextOut(x, y += dy, str);

		str.Format("Phi: %f ", state.self.accelaration.phi);
		pDC->TextOut(x, y += dy, str);

		pDC->TextOut(x, y += dy, "-torque-");

		str.Format("TILT1: %1.f ", robot.torque[TILT1]*1000);
		pDC->TextOut(x, y += dy, str);

		str.Format("PAN: %1.f ", robot.torque[PAN]*1000);
		pDC->TextOut(x, y += dy, str);

		str.Format("TILT2: %1.f ", robot.torque[TILT2]*1000);
		pDC->TextOut(x, y += dy, str);

		str.Format("RFLEG1: %1.f ", robot.torque[RFLEG1]*1000);
		pDC->TextOut(x, y += dy, str);

		str.Format("RFLEG2: %1.f ", robot.torque[RFLEG2]*1000);
		pDC->TextOut(x, y += dy, str);

		str.Format("RFLEG3: %1.f ", robot.torque[RFLEG3]*1000);
		pDC->TextOut(x, y += dy, str);

		pDC->TextOut(x, y += dy, "-joint angle-");

		str.Format("RFLEG1: %1.f ", dJointGetHingeAngle(robot.joints[RFLEG1])/PI*180);
		pDC->TextOut(x, y += dy, str);

		str.Format("RFLEG2: %1.f ", dJointGetHingeAngle(robot.joints[RFLEG2])/PI*180);
		pDC->TextOut(x, y += dy, str);

		str.Format("RFLEG3: %1.f ", dJointGetHingeAngle(robot.joints[RFLEG3])/PI*180 - 30);
		pDC->TextOut(x, y += dy, str);

		pDebugWnd->ReleaseDC(pDC);
	}


	CPaintDC dc(this); // 描画のデバイス コンテキスト
	
	// TODO : ここにメッセージ ハンドラ コードを追加します。
	
	// メッセージの描画のために CWnd::OnPaint() を呼び出さないでください。

	ChangeRenderContext(false);
	glShadeModel( bSmooth ? GL_SMOOTH : GL_FLAT);

	::glPushMatrix();

	if (targetID >= NUM_BODY){
		TargetBody = ball.bBall;
	}else{
		TargetBody = robot.draw_mesh[B_LF1].body;
	}

	if (current_cam == 0){
		camera[0].SetViewMatrix(bLookObject, robot.draw_mesh[B_HEAD].body);
#if ENABLE_MISSILE
//		camera[0].SetViewMatrix(bLookObject, missile[0].bMissile);
#endif
	}
	else if (current_cam == 1)
	{
		camera[1].SetViewMatrix(false, NULL);
	}
	else
	{

		static const GLfloat mymat[16] = {
			1.0f,	0.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	-1.0f,	0.0f,
			0.0f,	1.0f,	0.0f,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f,
		};

		//投影行列設定（カメラの視野角とか）
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		float zNear = 0.001;
		float zFar = 20.000;
		float fov = 30;

		gluPerspective(fov, camera[0].aspect, zNear, zFar);

		//モデルビュー行列：カメラ位置・姿勢の設定
		glMatrixMode(GL_MODELVIEW);
		dBodyID body = robot.draw_mesh[B_HEAD].body;
		const dReal* bodyPos = dBodyGetPosition(body);
		const dReal* bodyRot = dBodyGetRotation(body);

		GLfloat viewMat[16] = {
			bodyRot[0], bodyRot[1], bodyRot[2], bodyRot[3], 
			bodyRot[4], bodyRot[5], bodyRot[6], bodyRot[7], 
			bodyRot[8], bodyRot[9], bodyRot[10], bodyRot[11],
			0,0,0,1
		};

		const GLfloat mymat2[16] = {
				0.0f,	1.0f,	0.0f,	0.0f,
			-1.0f,	0.0f,	0.0f,	0.0f,
				0.0f,	0.0f,	1.0f,	0.0f,
				0.0f,	0.0f,	0.0f,	1.0f,
		};

		glLoadIdentity();

		glMultMatrixf(mymat);
		glMultMatrixf(mymat2);
		glTranslatef(-0.07006, 0.0, 0.0146);
		glMultMatrixf(viewMat);
		glTranslatef(-bodyPos[0],-bodyPos[1],-bodyPos[2]);

/*
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(camera[0].fov, camera[0].aspect, camera[0].zNear, camera[0].zFar);

		//モデルビュー行列：カメラ位置・姿勢の設定
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(
			fixed_cam_pos[fixed_cam_index][0],
			fixed_cam_pos[fixed_cam_index][1],
			fixed_cam_pos[fixed_cam_index][2],

			fixed_cam_pos[fixed_cam_index][3],
			fixed_cam_pos[fixed_cam_index][4],
			fixed_cam_pos[fixed_cam_index][5],
					0,0,1);
*/
	}

	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	RenderScene();
	::glFinish();
	::glPopMatrix();

	if( FALSE == ::SwapBuffers( m_pDC->GetSafeHdc())) ShowError(7);

	//Robot's vision
	ChangeRenderContext(true);

	glShadeModel( bSmooth ? GL_SMOOTH : GL_FLAT);
	::glPushMatrix();

	robot.SetCameraView();
	::glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	RenderScene();

	::glFinish();
	::glPopMatrix();

	//make image for robot
	if (!bStop)
	{
		static double lasttime = 0;
		while (dTime - lasttime > 0.032)
		{
			render.Copy2Buf( robot.imgbuf, IMAGE_WIDTH, IMAGE_HEIGHT);
			for (int i = 0; i < IMAGE_WIDTH*IMAGE_HEIGHT*3; i+=3)
			{
				RGB2YCbCr(	robot.imgbuf[i + 2], robot.imgbuf[i + 1], robot.imgbuf[i + 0], //B-G-R 
							robot.imgbuf[i + 0], robot.imgbuf[i + 2], robot.imgbuf[i + 1]); //Y-Cr-Cb
			}

//			doCMOSFilter(robot.imgbuf);

			robot.SetSensorData();
			lasttime += 0.032;
		}
	}
/*
	for (int i = 0; i < IMAGE_WIDTH*IMAGE_HEIGHT*3; i+=3)
	{
		BYTE R,G,B;
		YCbCr2RGB(	robot.imgbuf[i + 0], robot.imgbuf[i + 1], robot.imgbuf[i + 2],  
					R,G,B);
		::SetPixel(render.m_hDC, (i / 3) % IMAGE_WIDTH, (i/3) / IMAGE_WIDTH, RGB(R,G,B));
	}
*/

	HDC hDC = ::GetDC(pAiboViewWnd->m_hWnd);
	::BitBlt(hDC, 0,0, IMAGE_WIDTH, IMAGE_HEIGHT, render.m_hDC, 0, 0, SRCCOPY);

	::ReleaseDC(pAiboViewWnd->m_hWnd, hDC);


	ChangeRenderContext(false);
}

void CChildView::Capture()
{
	static int counter = 0;
	char buf[256];
	sprintf(buf, "BMP\\%06d.bmp", counter++);
	WriteWindowToDIB(buf, this);
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

//	return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

void CChildView::MakeWindow()
{
	{
		DWORD style = WS_POPUP | WS_CAPTION;
		CRect rect(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
		::AdjustWindowRect(rect, style, false);

		rect.MoveToXY(0,0);

		CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( CAiboView );
		pAiboViewWnd = reinterpret_cast<CAiboView*>(pRuntimeClass->CreateObject());
		ASSERT( pAiboViewWnd->IsKindOf( RUNTIME_CLASS( CAiboView ) ) );

		pAiboViewWnd->Create(NULL, "AiboView", style , rect, this);
		pAiboViewWnd->ShowWindow(SW_SHOW);
	}

	{
		DWORD style = WS_POPUP | WS_CAPTION;
		CRect rect(0, 0, IMAGE_WIDTH, 300);
		::AdjustWindowRect(rect, style, false);

		rect.MoveToXY(300,0);

		CRuntimeClass* pRuntimeClass = RUNTIME_CLASS( CDebugView );
		pDebugWnd = reinterpret_cast<CDebugView*>(pRuntimeClass->CreateObject());
		ASSERT( pDebugWnd->IsKindOf( RUNTIME_CLASS( CDebugView ) ) );

		pDebugWnd->Create(NULL, "internal state", style , rect, this);
		pDebugWnd->ShowWindow(SW_SHOW);
	}

}

BOOL CChildView::FinalizeODE()
{
	dJointGroupDestroy (contactgroup);
	  
	dSpaceDestroy (space);
	dWorldDestroy (world);

	dCloseODE();

	return TRUE;
}


BOOL CChildView::InitializeOpenGL()
{
	camera[1].position[0] = 0.0;
	camera[1].position[1] = -3.5;
	camera[1].position[2] = 1.5;

	m_pDC = new CClientDC(this);
	if( NULL == m_pDC ){
		ShowError(1);
		return FALSE;
	}
	if( !SetupPixelFormat() ) return FALSE;
	if( 0 == (m_hRC = ::wglCreateContext(m_pDC->GetSafeHdc()))){
		ShowError(4);
		return FALSE;
	}
	if( FALSE == ::wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC)){
		ShowError(5);
		return FALSE;
	}
	::glClearColor( 0.0f, 0.0f, 0.0f, 0.0f);
	::glClearDepth( 1.0f );
	::glClearStencil(0);
	::glEnable( GL_DEPTH_TEST );

	////////////////////
	//ライト設定
	float lightDiffuse0[] = {0.2f, 0.2f, 0.2f, 1.0f};	//拡散光データ
//	float lightSpecular0[] = {0.02f, 0.02f, 0.02f, 1.0f};	//鏡面光データ
	float lightSpecular0[] = {0.1f, 0.1f, 0.1f, 1.0f};	//鏡面光データ
	float lightAmbient0[] = {0.1f, 0.1f, 0.1f, 1.0f};	//環境光データ

	float m_nLightPos[4][4];
	
	//光源0
	m_nLightPos[0][0] =  1.5000;	//X
	m_nLightPos[0][1] =  1.0000;	//Y
	m_nLightPos[0][2] =  3.0000;	//Z
	m_nLightPos[0][3] =  1.0;		//種類

	m_nLightPos[1][0] =  1.5000;	//X
	m_nLightPos[1][1] = -1.0000;	//Y
	m_nLightPos[1][2] =  3.0000;	//Z
	m_nLightPos[1][3] =  1.0;		//種類

	m_nLightPos[2][0] = -1.5000;	//X
	m_nLightPos[2][1] =  1.0000;	//Y
	m_nLightPos[2][2] =  3.0000;	//Z
	m_nLightPos[2][3] =  1.0;		//種類

	m_nLightPos[3][0] = -1.5000;	//X
	m_nLightPos[3][1] = -1.0000;	//Y
	m_nLightPos[3][2] =  3.0000;	//Z
	m_nLightPos[3][3] =  1.0;		//種類

	//光源の定義
	glLightf (GL_LIGHT0, GL_SPOT_CUTOFF,  10.0);	
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT0, GL_POSITION, m_nLightPos[0]);	//光源位置

	glLightf (GL_LIGHT1, GL_SPOT_CUTOFF,  10.0);	
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT1, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT1, GL_POSITION, m_nLightPos[1]);	//光源位置

	glLightf (GL_LIGHT2, GL_SPOT_CUTOFF,  10.0);	
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT2, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT2, GL_POSITION, m_nLightPos[2]);	//光源位置

	glLightf (GL_LIGHT3, GL_SPOT_CUTOFF,  10.0);	
	glLightfv(GL_LIGHT3, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT3, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT3, GL_POSITION, m_nLightPos[3]);	//光源位置

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	//影の投影行列
	double plane[4] = {0.0, 0.0, 1.0, 0.0};
	projectShadowMatrix(
		drop_shadow_mat,
		plane,
		m_nLightPos[0]
		);
	for (int i = 0;i < 4; i++)
	{
		projectShadowMatrix(
			drop_shadow_mat_Light[i],
			plane,
			m_nLightPos[0]
			);
	}


	glColor3f(1,1,1);


	//フォント
    id1 = GenerateBitmapListForFont( "Arial" );

	wglMakeCurrent(render.m_hDC, render.m_hRC);
	::glClearColor( 0.0f, 0.0f, 0.0f, 0.0f);
	::glClearDepth( 1.0f );
	::glClearStencil(0);
	::glEnable( GL_DEPTH_TEST );

	////////////////////
	//ライト設定
	//光源の定義
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT0, GL_POSITION, m_nLightPos[0]);	//光源位置

	glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT1, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT1, GL_POSITION, m_nLightPos[1]);	//光源位置

	glLightfv(GL_LIGHT2, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT2, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT2, GL_POSITION, m_nLightPos[2]);	//光源位置

	glLightfv(GL_LIGHT3, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT3, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT3, GL_POSITION, m_nLightPos[3]);	//光源位置

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	::wglMakeCurrent( m_pDC->GetSafeHdc(), m_hRC);

	return TRUE;
}


BOOL CChildView::SetupPixelFormat()
{
	PIXELFORMATDESCRIPTOR pfd = 
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		24,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0,  0, 0, 0,
		24,
		1,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};
	int pixelformat;
	if( 0 == (pixelformat = ::ChoosePixelFormat( m_pDC->GetSafeHdc(), &pfd))){
		ShowError(2);
		return FALSE;
	}
	if( FALSE == ::SetPixelFormat(m_pDC->GetSafeHdc(), pixelformat, &pfd)){
		ShowError(3);
		return FALSE;
	}
	return TRUE;
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	MakeWindow();
	render.CreateGLDIB(IMAGE_WIDTH, IMAGE_HEIGHT, 24, 24);

	// TODO :  ここに特定な作成コードを追加してください。
	InitializeOpenGL();
	InitializeODE();

	field.space = space;
	field.world = world;
	field.Init();

	ball.space = dHashSpaceCreate(space);
	ball.world = world;
	ball.Init();

	robot.space = space;
	robot.world = world;
	robot.Init();

#if ENABLE_MISSILE

	for (int i = 0; i < MISSILE_NUM;i++){
		missile[i].space = dHashSpaceCreate(space);
		missile[i].world = world;
		missile[i].Init();
	}
#endif

	SetTimer(1, nMiliSecondPerFrame, NULL);

	sock.Create(20080);
	sock.buf = robot.imgbuf;
	sock.Listen(5);
	
	return 0;
}

void CChildView::OnDestroy()
{
	CWnd::OnDestroy();

	// TODO : ここにメッセージ ハンドラ コードを追加します。
	if( FALSE == ::wglMakeCurrent( 0, 0)) ShowError(2);
	if( FALSE == ::wglDeleteContext( m_hRC )) ShowError(6);
	if( m_pDC ) delete m_pDC;
}

void CChildView::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO : ここにメッセージ ハンドラ コードを追加します。
	if( 0>= cx || 0>=cy) return;

	camera[0].SetAspect((GLfloat)cx /(GLfloat)cy);
	camera[1].SetAspect((GLfloat)cx /(GLfloat)cy);

	::glViewport( 0, 0, cx, cy );

	screen_width = cx;
	screen_height = cy;

	return;
}

BOOL CChildView::RenderScene()
{
	float lightDiffuse0[] = {0.9f, 0.9f, 0.9f, 1.0f};	//拡散光データ
//	float lightSpecular0[] = {0.02f, 0.02f, 0.02f, 1.0f};	//鏡面光データ
	float lightSpecular0[] = {0.1f, 0.1f, 0.1f, 1.0f};	//鏡面光データ
	float lightAmbient0[] = {0.1f, 0.1f, 0.1f, 1.0f};	//環境光データ

	float m_nLightPos[4][4];
	float m_nLightDir[4][3];

	//光源0
	m_nLightPos[0][0] =  1.5000;	//X
	m_nLightPos[0][1] =  1.0000;	//Y
	m_nLightPos[0][2] =  3.0000;	//Z
	m_nLightPos[0][3] =  1.0;		//種類

	m_nLightDir[0][0] =  0.0;
	m_nLightDir[0][1] =  0.0;
	m_nLightDir[0][2] = -1.0;

	m_nLightPos[1][0] =  1.5000;	//X
	m_nLightPos[1][1] = -1.0000;	//Y
	m_nLightPos[1][2] =  3.0000;	//Z
	m_nLightPos[1][3] =  1.0;		//種類

	m_nLightDir[1][0] =  0.0;
	m_nLightDir[1][1] =  0.0;
	m_nLightDir[1][2] = -1.0;

	m_nLightPos[2][0] = -1.5000;	//X
	m_nLightPos[2][1] =  1.0000;	//Y
	m_nLightPos[2][2] =  3.0000;	//Z
	m_nLightPos[2][3] =  1.0;		//種類

	m_nLightDir[2][0] =  0.0;
	m_nLightDir[2][1] =  0.0;
	m_nLightDir[2][2] = -1.0;

	m_nLightPos[3][0] = -1.5000;	//X
	m_nLightPos[3][1] = -1.0000;	//Y
	m_nLightPos[3][2] =  3.0000;	//Z
	m_nLightPos[3][3] =  1.0;		//種類

	m_nLightDir[3][0] =  0.0;
	m_nLightDir[3][1] =  0.0;
	m_nLightDir[3][2] = -1.0;

	//光源の定義
	float expo		= 5.0;
	float cutoff	= 60.0;
	glLightf (GL_LIGHT0, GL_SPOT_CUTOFF,  cutoff);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, m_nLightDir[0]);
	glLightf (GL_LIGHT0, GL_SPOT_EXPONENT, expo);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT0, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT0, GL_POSITION, m_nLightPos[0]);	//光源位置

	glLightf (GL_LIGHT1, GL_SPOT_CUTOFF,  cutoff);	
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, m_nLightDir[1]);
	glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, expo);
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT1, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT1, GL_POSITION, m_nLightPos[1]);	//光源位置

	glLightf (GL_LIGHT2, GL_SPOT_CUTOFF,  cutoff);	
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, m_nLightDir[2]);
	glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, expo);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT2, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT2, GL_POSITION, m_nLightPos[2]);	//光源位置

	glLightf (GL_LIGHT3, GL_SPOT_CUTOFF,  cutoff);	
	glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, m_nLightDir[3]);
	glLightf (GL_LIGHT3, GL_SPOT_EXPONENT, expo);
	glLightfv(GL_LIGHT3, GL_DIFFUSE,  lightDiffuse0);	//拡散光
	glLightfv(GL_LIGHT3, GL_SPECULAR, lightSpecular0);	//鏡面光
	glLightfv(GL_LIGHT3, GL_AMBIENT,  lightAmbient0);	//環境光
	glLightfv(GL_LIGHT3, GL_POSITION, m_nLightPos[3]);	//光源位置

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);
	glEnable(GL_LIGHT3);

	//MainLight
	{
	float dir[3] = {0,0,-1};
	float pos[3] = {0,0,0.500};
	float dif[4] = {0.2,0.2,0.2,1.0};
	float spe[4] = {0.0,0.0,0.0,1.0};
	float amb[4] = {0.0,0.0,0.0,1.0};
	glLightf (GL_LIGHT4, GL_SPOT_CUTOFF,  180);	
	glLightfv(GL_LIGHT4, GL_SPOT_DIRECTION, dir);
	glLightf (GL_LIGHT4, GL_SPOT_EXPONENT, 0);
	glLightfv(GL_LIGHT4, GL_DIFFUSE,  dif);	//拡散光
	glLightfv(GL_LIGHT4, GL_SPECULAR, spe);	//鏡面光
	glLightfv(GL_LIGHT4, GL_AMBIENT,  amb);	//環境光
	glLightfv(GL_LIGHT4, GL_POSITION, pos);	//光源位置
	}

	glEnable(GL_LIGHT4);



	bool bWireBackup = CRobot::bWireFrame;
	if (CBody::s_bOffscreenRender) CRobot::bWireFrame = false;


	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glDisable(GL_DEPTH_TEST);
	field.DrawField(CRobot::bWireFrame);
	glEnable(GL_DEPTH_TEST);

	//shadow
	for (int i = 0; i < 1; i++)
	{
		memcpy(drop_shadow_mat, drop_shadow_mat_Light[i], sizeof(drop_shadow_mat));
		::glClear(GL_STENCIL_BUFFER_BIT);

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_STENCIL_TEST);
		
		glStencilFunc(GL_ALWAYS, 1, ~0);
		glStencilOp(GL_REPLACE,GL_REPLACE,GL_REPLACE);
		glColorMask(0,0,0,0);
		glDepthMask(0);

		ball.DrawShadow();
		robot.DrawShadow();

#if ENABLE_MISSILE
		for (int i = 0; i < MISSILE_NUM;i++)
			missile[i].DrawShadow();
#endif

		glColorMask(1,1,1,1);
		glDepthMask(1);
		glStencilOp(GL_KEEP,GL_KEEP ,GL_KEEP);
		glStencilFunc( GL_EQUAL, 1, ~0);

		glDisable(GL_LIGHTING);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glColor4f(0,0,0,0.4);
		glBegin(GL_QUADS);
			glVertex3f(-6.000,-4.000,0);
			glVertex3f( 6.000,-4.000,0);
			glVertex3f( 6.000, 4.000,0);
			glVertex3f(-6.000, 4.000,0);
		glEnd();
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);

		glDisable(GL_STENCIL_TEST);
		glEnable(GL_DEPTH_TEST);
	}

	robot.Draw();

	field.DrawLandmarks(CRobot::bWireFrame);
	field.DrawGoals(CRobot::bWireFrame);

#if ENABLE_MISSILE
	for (int i = 0; i < MISSILE_NUM;i++)
		missile[i].Draw();
#endif

	ball.Draw(CRobot::bWireFrame);


	CRobot::bWireFrame = bWireBackup;

	if (CBody::s_bOffscreenRender)
		return true;

	return true;


	//FPS取得
	static char fps_str[128] = "0.0 fps";
	static char pos_buf[256] = "";
	static char info_buf[256] = "";
	{
		static SYSTEMTIME lasttime;
		static SYSTEMTIME current;

		static double lastdTime;

		static bool init = false;
		if(!init){
			init = true;
			GetSystemTime(&lasttime);
			GetSystemTime(&current);
			lastdTime = 0;
		}

		static int counter = 0;

		counter++;

		if(counter >= 30){
			GetSystemTime(&current);
			int mili = current.wMilliseconds - lasttime.wMilliseconds;
			mili += (current.wSecond - lasttime.wSecond)*1000;
			mili += (current.wMinute - lasttime.wMinute)*1000*60;
			mili += (current.wHour - lasttime.wHour)*1000*60*60;

			double dt = dTime - lastdTime;

			lasttime = current;
			counter = 0;

			double fps = 30.0 / mili * 1000;

			double ratio =  (dt*1000) / (mili);

			sprintf(fps_str, "x %.2f", ratio);
//			sprintf(fps_str, "%.1f fps: x %.2f", fps, ratio);

			lastdTime = dTime;

			sprintf(pos_buf,"x:%.1f y:%.1f, dir:%.1f", 
				state.self.position.x.val,
				state.self.position.y.val,
				state.self.position.tt.val
				);
/*
			sprintf(info_buf, "%3.1f, %3.1f",
				state.self.accelaration.theta,
				state.self.accelaration.phi
			);
*/
		}
	}

	//文字列表示
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluOrtho2D(0.0,screen_width ,0, screen_height);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	//ここで書く
	glColor3f(0.0,1.0,0);
	glRasterPos2f(10,screen_height - 20);
	GLTextOut(fps_str);


	//robot pos
	glColor3f(0.0,1.0,0);
	glRasterPos2f(10,screen_height - 40);
	GLTextOut(pos_buf);

	glColor3f(0.0,1.0,0);
	glRasterPos2f(10,screen_height - 60);
	GLTextOut(info_buf);

	if (state.ball.visible || state.ball.losttime < 10)
	{
		glColor3f(0.0,1.0,0);
		glRasterPos2f(10,screen_height - 80);
//		GLTextOut("ball visible");
	}


	::glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	::glPopMatrix();

	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);




	return TRUE;
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。
	float step = 5.0;

	bool bCtrlDown  = GetKeyState ( VK_CONTROL ) < 0;
	bool bShiftDown = GetKeyState ( VK_SHIFT   ) < 0;

	switch (nChar) {
	case '1':
		MotionMaker::SetMotion(HG_BACK);
		MotionMaker::StartMotion();
		break;
	case '2':
		MotionMaker::SetMotion(GETUP_RIGHT);
		MotionMaker::StartMotion();
		break;
	case '3':
		MotionMaker::SetMotion(GRABSHOOT_FAKE_LEFT20);
		MotionMaker::StartMotion();
		break;
	case '4':
		MotionMaker::SetMotion(SMASHHIT_LEFT);
		MotionMaker::StartMotion();
		break;
	case '5':
		MotionMaker::SetMotion(GOALIE_BLOCK_RIGHTWALK);
		MotionMaker::StartMotion();
		break;
	case '6':
		GaitMaker::SetPresetWalk(WALK_FORWARD,10);
//		MotionMaker::SetMotion(DIVING_LEFT45_OSAKA);
//		MotionMaker::StartMotion();
		break;
	case '7':
		MotionMaker::SetMotion(MUNESHOT_00);
		MotionMaker::StartMotion();
		break;
	case '8':
		MotionMaker::SetMotion(GETUP);
		MotionMaker::StartMotion();
		break;
	case '9':
		MotionMaker::SetMotion(HG);
		MotionMaker::StartMotion();
		break;
	case '0':
		MotionMaker::SetMotion(GUTSPOSE);
		MotionMaker::StartMotion();
		break;

	case 'B':
		if (!bCtrlDown)
		{
#if ENABLE_MISSILE

			glPushMatrix();
			glLoadIdentity();
			const dReal* hpos = dBodyGetPosition(robot.draw_mesh[B_BODY].body);
			const dReal* hrot = dBodyGetRotation(robot.draw_mesh[B_BODY].body);
			dReal vel[3] = {0,0,0};
			dReal pos[3] = {0 + hpos[0],0 + hpos[1], 0.15 + hpos[2]};
			dReal rot[9] = {
				hrot[0], hrot[1], hrot[2],
				hrot[3], hrot[4], hrot[5],
				hrot[6], hrot[7], hrot[8]
			};
			dReal rot2[12];
			memcpy(rot2, hrot, 12*sizeof(dReal));
			static int mi = 0;
			missile[mi].Launch(
				pos,rot,vel
				);
			mi++;
			if ( mi >= MISSILE_NUM)
				mi = 0;
			glPopMatrix();
#endif
		}
		else
		{
			bTorqueFlag = !bTorqueFlag;
		}
		break;

	case 'T':
		{
		const dReal *pos;
		pos = dBodyGetPosition(ball.bBall);

		double dx = -2.700 - pos[0];
		double dy = -0.300 - pos[1];
		dBodySetLinearVel(ball.bBall, dx*4, dy*4, 2.7);
		}

		break;


	case 'O':
		if (bCtrlDown)
		{
			dBodyDisable(ball.bBall);
			dBodySetPosition(ball.bBall, 0,0,0.3);
			dBodySetLinearVel(ball.bBall, 0,0,0);
			dBodySetAngularVel(ball.bBall, 0,0,0);
			dBodyEnable(ball.bBall);
		}
		else
		{
			dBodyAddForce(ball.bBall, 1/0.1, 0, 0/0.1);
		}
		break;
	case 'L':
		if (bCtrlDown)
		{
	//		robot.ResetPosition();
		}
		else
		{
			dBodyAddForce(ball.bBall, 0, 1/0.1, 0/0.1);
		}
		break;
	case 'K':
		if(bCtrlDown)
			bRec = !bRec;
		else
			dBodyAddForce(ball.bBall, 0, -1/0.1, 0/0.1);
		break;
	case 'I':
		dBodyAddForce(ball.bBall, -1/0.1, 0, 0/0.1);
		break;

	case 'U':
		dBodyAddForce(ball.bBall, 0/0.1, 0, 5/0.1);
		break;


	case 'F':
		camera[0].position[2] *= 1.04;
//		camera[0].Move( 0.1000, 0.0, 0.0);
		break;
	case 'D':
		camera[0].Move(-0.01000, 0.0, 0.0);
		break;

	case 'R':
		if (bCtrlDown){
			dReal destpos[3] = {0,0,0.8};
			robot.ResetPosition(destpos);
		}
		else
			camera[0].Move(0.0, -0.01000, 0.0);
		break;
	case 'E':
		camera[0].Move(0.0,  0.01000, 0.0);
		break;

	case 'V':
		camera[0].Move(0.0,  0.0, 0.01000);
		break;
	case 'C':
		camera[0].Move(0.0,  0.0,-0.01000);
		break;

	case 'A':
		ball.bDrawTail = !ball.bDrawTail;
		break;

	case 'S':
		OnStopGo();
		break;

	case 'Z':
		bSmooth = !bSmooth;
		break;

	case 'W':
		CRobot::bWireFrame = !CRobot::bWireFrame;
		break;

	case 'X':
		state.game.game_state = GAME_STATE_READY;
		break;

	case 'N':
		state.game.game_state = GAME_STATE_SET;
		break;

	case 'M':
		state.game.game_state = GAME_STATE_PLAYING;
		break;

	case 'P':
		if (current_cam == 0)
			current_cam = 1;
		else if ( current_cam == 1)
			current_cam = 2;
		else 
			current_cam = 0;

//		bLookObject = !bLookObject;
//		targetID = (targetID+1) % (NUM_BODY + 1);
		break;

	case 'Q':
		{
			static int cnt = 0;
			char buf[256];
			sprintf(buf, "%04d", cnt++);
			WritePPM(buf, robot.imgbuf, IMAGE_WIDTH, IMAGE_HEIGHT);
		}
		break;

	case VK_UP:
		if(current_cam == 0){
			if(bLookObject)
				camera[0].Move(0, TargetBody);
			else
				camera[0].AddTilt(1.0);
		}
		else if ( current_cam == 1)
		{
			if (bCtrlDown)
				camera[1].fov /= 1.01;
			else
				camera[1].AddTilt(1.0);
		}

		break;
	case VK_DOWN:
		if(current_cam == 0){
			if(bLookObject)
				camera[0].Move(1, TargetBody);
			else
				camera[0].AddTilt(-1.0);
		}
		else if ( current_cam == 1)
		{
			if (bCtrlDown)
				camera[1].fov *= 1.01;
			else
				camera[1].AddTilt(-1.0);
		}
		break;
	case VK_LEFT:
		if(current_cam == 0){
			if(bLookObject)
				camera[0].Move(2, TargetBody);
			else
				camera[0].AddPan(1.0);
		}
		else if ( current_cam == 1)
		{
			if (bCtrlDown)
				;
			else
				camera[1].AddPan(1.0);
		}
		else
		{
			fixed_cam_index = (fixed_cam_index - 1 + NUM_FIXED_CAM_POS) % NUM_FIXED_CAM_POS;
		}
		break;
	case VK_RIGHT:
		if(current_cam == 0){
			if(bLookObject)
				camera[0].Move(3, TargetBody);
			else
				camera[0].AddPan(-1.0);
		}
		else if ( current_cam == 1)
		{
			if (bCtrlDown)
				;
			else
				camera[1].AddPan(-1.0);
		}
		else
		{
			fixed_cam_index = (fixed_cam_index + 1) % NUM_FIXED_CAM_POS;
		}
		break;
	case VK_HOME:
		Xrot = 0;
		Yrot = 0;
		break;
	}



//	Invalidate();

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

GLuint CChildView::GenerateBitmapListForFont( char * fontname )
{
	GLuint id;
	if( 0 == m_pDC || (GLuint)0 == (id = ::glGenLists(256)))
		return 0;

	LOGFONT logfont;
	logfont.lfHeight		= 20;
	logfont.lfWidth		 = 0;
	logfont.lfEscapement	= 0;
	logfont.lfOrientation   = logfont.lfEscapement;
	logfont.lfWeight		= FW_NORMAL;
	logfont.lfItalic		= FALSE;
	logfont.lfUnderline	 = FALSE;
	logfont.lfStrikeOut	 = FALSE;
	logfont.lfCharSet	   = ANSI_CHARSET;
	logfont.lfOutPrecision  = OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
	logfont.lfQuality	   = DEFAULT_QUALITY;
	logfont.lfPitchAndFamily= FF_DONTCARE|DEFAULT_PITCH;

	strcpy( logfont.lfFaceName, fontname );

	CFont newfont;
	BOOL result;

	result = newfont.CreateFontIndirect( &logfont );
	CFont* oldfont = m_pDC->SelectObject( &newfont );
	ASSERT( 0 != oldfont );

	if( FALSE == result || FALSE == ::wglUseFontBitmaps( m_pDC->m_hDC, 0, 256, id))
	{
		::glDeleteLists( id, 256 );
		id = 0;
	}

	m_pDC->SelectObject( oldfont );
	return id;
}


void CChildView::GLTextOut(const char* const textstring )
{
	if (0 == textstring ) return;

	GLsizei size = strlen( textstring );
	::glListBase( id1 );
	glColor3f(1,1,1);
	matr_white.setMaterial();
	::glCallLists( size, GL_UNSIGNED_BYTE, (const GLvoid*)textstring );
}


void CChildView::OnToggleWireframeRobot()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	CRobot::bWireFrame = !CRobot::bWireFrame;
}

void CChildView::OnTogglegeommesh()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	CRobot::bUseGeomMesh = !CRobot::bUseGeomMesh;
}

void CChildView::OnStopGo()
{
	// TODO : ここにコマンド ハンドラ コードを追加します。
	bStop = !bStop;
}


void CChildView::OnOpenMotion()
{
	static char dirbuf[260];
	::GetCurrentDirectory(256,dirbuf);
	// TODO : ここにコマンド ハンドラ コードを追加します。
	CFileDialog dlg(true, "csv", NULL, 0, "CSVファイル|*.csv");
	if (dlg.DoModal() == IDOK)
	{
		CStdioFile file;
		if (file.Open(dlg.GetPathName(), CFile::modeRead))
		{
			robot.motion.Clear();
			CString str;
			while (file.ReadString(str))
			{
				robot.motion.ReadLine(str);
			}
			robot.motion.AddTail();
			file.Close();
		}
	}
	::SetCurrentDirectory(dirbuf);

}
