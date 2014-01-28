// MySock.cpp : 実装ファイル
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

// CMySock メンバ関数

void CMySock::OnAccept(int nErrorCode)
{

	Sleep(100);
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	CMySock* pSock = new CMySock();
	pSock->buf = buf;
	Accept(*pSock);
	v_pSock.push_back(pSock);

	CAsyncSocket::OnAccept(nErrorCode);
}

void CMySock::OnClose(int nErrorCode)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CAsyncSocket::OnClose(nErrorCode);
}

void CMySock::OnConnect(int nErrorCode)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CAsyncSocket::OnConnect(nErrorCode);
}

void CMySock::OnReceive(int nErrorCode)
{
	Sleep(500);
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。
	if (buf != NULL)
		Send(buf, 208*160*3);

	CAsyncSocket::OnReceive(nErrorCode);
}

void CMySock::OnSend(int nErrorCode)
{
	// TODO : ここに特定なコードを追加するか、もしくは基本クラスを呼び出してください。

	CAsyncSocket::OnSend(nErrorCode);
}
