///////////////////////////////////////////////////////////////////////////////
//OpenGL用DIBクラス

#ifdef _CGLDIB
#else
#define _CGLDIB

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

class CglDIB{
protected:
	BITMAPINFOHEADER*	BitmapInfo;			//ビットマップヘッダ
	HBITMAP				hBitmap;
	HBITMAP				hBitmapOld;			//以前のビットマップハンドル
	void*				data;				//ビットマップデータ
	int					width;				//メモリのサイズ
	int					height;				
	int					color_bit;			//OpenGL　色数
	int					depth_bit;			//OpenGL　デプスバッファ
public:
	CglDIB();
	~CglDIB();								//デストラクタ
	BOOL	CreateGLDIB(int,int,int,int);	//メモリの作成
	BOOL	Draw(HDC,int,int);				//描画
	HDC		GetDC(void);					//コンテキストハンドルを返す
	HBITMAP GetBitmap(void);				//ビットマップハンドルを返す

	HGLRC	m_hRC;
	HDC		m_hDC;
	BOOL    Copy2Buf(BYTE* dest_buf, int W, int H);
};

#endif
