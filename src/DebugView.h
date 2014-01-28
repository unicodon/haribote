#pragma once


// CDebugView フレーム 

class CDebugView : public CFrameWnd
{
	DECLARE_DYNCREATE(CDebugView)
protected:
	CDebugView();           // 動的生成で使用される protected コンストラクタ
	virtual ~CDebugView();

protected:
	DECLARE_MESSAGE_MAP()
};


