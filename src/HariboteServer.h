// HariboteServer.h : HariboteServer �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂߂�O�ɁA'stdafx.h' ���܂߂Ă��������B
#endif

#include "resource.h"       // ���C�� �V���{��


// CHariboteServerApp:
// ���̃N���X�̎����ɂ��ẮAHariboteServer.cpp ���Q�Ƃ��Ă��������B
//

class CHariboteServerApp : public CWinApp
{
public:
	CHariboteServerApp();


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();

// ����

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CHariboteServerApp theApp;
