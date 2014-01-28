#pragma once


// CAiboView フレーム 

class CAiboView : public CFrameWnd
{
	DECLARE_DYNCREATE(CAiboView)
protected:
	CAiboView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CAiboView();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
};


