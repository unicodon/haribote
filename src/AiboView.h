#pragma once


// CAiboView �t���[�� 

class CAiboView : public CFrameWnd
{
	DECLARE_DYNCREATE(CAiboView)
protected:
	CAiboView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
	virtual ~CAiboView();

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
};


