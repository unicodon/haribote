///////////////////////////////////////////////////////////////////////////////
//OpenGL�pDIB�N���X

#ifdef _CGLDIB
#else
#define _CGLDIB

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>

class CglDIB{
protected:
	BITMAPINFOHEADER*	BitmapInfo;			//�r�b�g�}�b�v�w�b�_
	HBITMAP				hBitmap;
	HBITMAP				hBitmapOld;			//�ȑO�̃r�b�g�}�b�v�n���h��
	void*				data;				//�r�b�g�}�b�v�f�[�^
	int					width;				//�������̃T�C�Y
	int					height;				
	int					color_bit;			//OpenGL�@�F��
	int					depth_bit;			//OpenGL�@�f�v�X�o�b�t�@
public:
	CglDIB();
	~CglDIB();								//�f�X�g���N�^
	BOOL	CreateGLDIB(int,int,int,int);	//�������̍쐬
	BOOL	Draw(HDC,int,int);				//�`��
	HDC		GetDC(void);					//�R���e�L�X�g�n���h����Ԃ�
	HBITMAP GetBitmap(void);				//�r�b�g�}�b�v�n���h����Ԃ�

	HGLRC	m_hRC;
	HDC		m_hDC;
	BOOL    Copy2Buf(BYTE* dest_buf, int W, int H);
};

#endif
