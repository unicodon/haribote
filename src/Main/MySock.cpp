// MySock.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MySock.h"
#include ".\mysock.h"

#include <vector>
using namespace std;

static vector<CMySock*> v_pSock;

void CMySock::DeleteSocks()
{
	int size = v_pSock.size();
	for(int i = 0; i < size; i++)
	{
		delete v_pSock[i];
	}
	v_pSock.clear();
}

// CMySock
CMySock::~CMySock()
{
}

// CMySock
CMySock::CMySock()
{
	buf = NULL;
}

// CMySock �����o�֐�

void CMySock::OnAccept(int nErrorCode)
{

	Sleep(100);
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	CMySock* pSock = new CMySock();
	pSock->buf = buf;
	Accept(*pSock);
	v_pSock.push_back(pSock);

	CAsyncSocket::OnAccept(nErrorCode);
}

void CMySock::OnClose(int nErrorCode)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CAsyncSocket::OnClose(nErrorCode);
}

void CMySock::OnConnect(int nErrorCode)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySock::OnReceive(int nErrorCode)
{
	Sleep(500);
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	if (buf != NULL)
		Send(buf, 208*160*3);

	CAsyncSocket::OnReceive(nErrorCode);
}

void CMySock::OnSend(int nErrorCode)
{
	// TODO : �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	CAsyncSocket::OnSend(nErrorCode);
}
