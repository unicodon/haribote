#include "StdAfx.h"
#include "CglDIB.h"

CglDIB::CglDIB() 
{
	BitmapInfo = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//デストラクタ
CglDIB::~CglDIB(){
	if(m_hRC){
		//解放
		wglDeleteContext(m_hRC);
	}
	//以前のビットマップとむすびつける
	SelectObject(m_hDC, hBitmapOld);
	//ビットマップオブジェクトを削除
	DeleteObject(hBitmap);
	//DCを開放
	DeleteDC(m_hDC);
	//ビットマップヘッダのメモリ開放
	if (BitmapInfo)
		delete BitmapInfo;
};

///////////////////////////////////////////////////////////////////////////////
//描画
BOOL CglDIB::Draw(HDC destDC,			//コピー先のデバイスコンテキスト
				  int destWidth,		//コピー先領域の範囲
				  int destHeight
				  ){
	//転送モード設定
	SetStretchBltMode(destDC, COLORONCOLOR);
	//転送（コピー）
	StretchDIBits(
		destDC,
		0,0,			//コピー先の座標
		destWidth,destHeight,
		0,0,			//ソースの座標
		width, height,
		data,
		(BITMAPINFO*)BitmapInfo,
		DIB_RGB_COLORS,
		SRCCOPY
	);
  return TRUE;
};

BOOL CglDIB::Copy2Buf(BYTE* dest_buf, int W, int H)
{
	if (data == NULL || BitmapInfo == NULL)
		return false;

	int size  = W * H;
	int size2 = BitmapInfo->biWidth * BitmapInfo->biHeight;

	int line_step = (((BitmapInfo->biWidth - 1) / 4 + 1) * 4)  * BitmapInfo->biBitCount / 8;

	if (size > size2) size = size2;
	BYTE* pDest = dest_buf;

	switch(	BitmapInfo->biBitCount)
	{
	case 32:
		for(int y = BitmapInfo->biHeight - 1; y >= 0; y--)
		{
			BYTE* p = (BYTE*)data + line_step * y;
			for (int x = 0;x < W; x++)
			{
				memcpy(pDest, p, 3);
				p		+= 4;
				pDest	+= 3;
			}
		}
		return true;
	case 24:
		for(int y = BitmapInfo->biHeight - 1; y >= 0; y--)
		{
			BYTE* p = (BYTE*)data + line_step * y;
			memcpy(pDest, p, 3*W);
			pDest	+= 3*W;
		}
		return true;
	default:
		return false;
	}
}

///////////////////////////////////////////////////////////////////////////////
//ビットマップメモリの作成
BOOL CglDIB::CreateGLDIB(int inputWidth,		//メモリのサイズ
						 int inputHeight,
						 int inputColor,		//メモリの色数
						 int inputDepth			//メモリのデプスバッファ
						 ){
	width = inputWidth;
	height = inputHeight;
	
	//ビットマップ情報
	BitmapInfo = new BITMAPINFOHEADER;
	int iSize = sizeof(BITMAPINFOHEADER) ;
	memset(BitmapInfo, 0, iSize);
	//
	BitmapInfo->biSize = iSize;
	BitmapInfo->biWidth = width;
	BitmapInfo->biHeight = height;
	BitmapInfo->biPlanes = 1;
	BitmapInfo->biBitCount = inputColor;
	BitmapInfo->biCompression = BI_RGB;
	//互換デバイスコンテキストの作成
	m_hDC = CreateCompatibleDC(NULL);
	//ビットマップメモリ作成
	hBitmap = CreateDIBSection(m_hDC,
		(BITMAPINFO*)BitmapInfo,
		DIB_RGB_COLORS,
		&data,
		NULL,
		0
		);
    //以前のビットマップメモリ情報を退避させる
	if(hBitmap){
		hBitmapOld = (HBITMAP)SelectObject(m_hDC,hBitmap);
	}
	//ピクセルフォーマット設定
	int pixelformat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),		//この構造体のサイズ
		1,									//OpenGLバージョン
		PFD_DRAW_TO_BITMAP |                //ビットマップ設定
		PFD_SUPPORT_OPENGL |                    
		PFD_SUPPORT_GDI,
		PFD_TYPE_RGBA,						//RGBAカラー
		inputColor,							//色数
		0, 0,								//RGBAのビットとシフト設定		
		0, 0,								//G
		0, 0,								//B
		0, 0,								//A
		0,									//アキュムレーションバッファ
		0, 0, 0, 0, 						//RGBAアキュムレーションバッファ
		inputDepth,									//Zバッファ	
		0,									//ステンシルバッファ
		0,									//使用しない
		PFD_MAIN_PLANE,						//レイヤータイプ
		0,									//予約
		0, 0, 0								//レイヤーマスクの設定・未使用
    };
    //ピクセルフォーマットの選定
    if((pixelformat = ChoosePixelFormat(m_hDC, &pfd))==0){
		return FALSE;
	}
    //ピクセルフォーマットの入力
    if(SetPixelFormat(m_hDC, pixelformat, &pfd)==FALSE){
		return FALSE;
	}
	//レンダリングコンテキストの作成
	if((m_hRC = wglCreateContext(m_hDC))==NULL){
		return FALSE;
	}
	return TRUE;
};
///////////////////////////////////////////////////////////////////////////////
HDC	CglDIB::GetDC(void){
	//ビットマップメモリクラスのコンテキストを返す
	return m_hDC;
}
///////////////////////////////////////////////////////////////////////////////
HBITMAP CglDIB::GetBitmap(void){
	//ビットマップメモリクラスのビットマップハンドルを返す
	return hBitmap;
}
