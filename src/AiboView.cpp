// AiboView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "HariboteServer.h"
#include "AiboView.h"
#include ".\aiboview.h"


// CAiboView

IMPLEMENT_DYNCREATE(CAiboView, CFrameWnd)

CAiboView::CAiboView()
{
}

CAiboView::~CAiboView()
{
}


BEGIN_MESSAGE_MAP(CAiboView, CFrameWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CAiboView ���b�Z�[�W �n���h��

BOOL CAiboView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	return CFrameWnd::PreCreateWindow(cs);
}

void CAiboView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CAiboView::OnClose()
{
	// TODO : �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B

	//Do Nothing;
	return;

	//CFrameWnd::OnClose();
}
