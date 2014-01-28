#pragma once

// CMySock コマンド ターゲット

class CMySock : public CAsyncSocket
{
public:
	CMySock();
	virtual ~CMySock();

	virtual void OnAccept(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	virtual void OnConnect(int nErrorCode);
	virtual void OnReceive(int nErrorCode);
	virtual void OnSend(int nErrorCode);

	const		 BYTE* buf;

	void DeleteSocks();
};


