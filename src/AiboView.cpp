// AiboView.cpp : 実装ファイル
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


// CAiboView メッセージ ハンドラ

BOOL CAiboView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	return CFrameWnd::PreCreateWindow(cs);
}

void CAiboView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	CFrameWnd::OnLButtonDown(nFlags, point);
}

void CAiboView::OnClose()
{
	// TODO : ここにメッセージ ハンドラ コードを追加するか、既定の処理を呼び出します。

	//Do Nothing;
	return;

	//CFrameWnd::OnClose();
}
