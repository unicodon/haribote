// HariboteServer.h : HariboteServer アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含める前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"       // メイン シンボル


// CHariboteServerApp:
// このクラスの実装については、HariboteServer.cpp を参照してください。
//

class CHariboteServerApp : public CWinApp
{
public:
	CHariboteServerApp();


// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHariboteServerApp theApp;
