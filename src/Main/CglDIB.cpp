#include "StdAfx.h"
#include "CglDIB.h"

CglDIB::CglDIB() 
{
	BitmapInfo = NULL;
}

///////////////////////////////////////////////////////////////////////////////
//�f�X�g���N�^
CglDIB::~CglDIB(){
	if(m_hRC){
		//���
		wglDeleteContext(m_hRC);
	}
	//�ȑO�̃r�b�g�}�b�v�Ƃނ��т���
	SelectObject(m_hDC, hBitmapOld);
	//�r�b�g�}�b�v�I�u�W�F�N�g���폜
	DeleteObject(hBitmap);
	//DC���J��
	DeleteDC(m_hDC);
	//�r�b�g�}�b�v�w�b�_�̃������J��
	if (BitmapInfo)
		delete BitmapInfo;
};

///////////////////////////////////////////////////////////////////////////////
//�`��
BOOL CglDIB::Draw(HDC destDC,			//�R�s�[��̃f�o�C�X�R���e�L�X�g
				  int destWidth,		//�R�s�[��̈�͈̔�
				  int destHeight
				  ){
	//�]�����[�h�ݒ�
	SetStretchBltMode(destDC, COLORONCOLOR);
	//�]���i�R�s�[�j
	StretchDIBits(
		destDC,
		0,0,			//�R�s�[��̍��W
		destWidth,destHeight,
		0,0,			//�\�[�X�̍��W
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
//�r�b�g�}�b�v�������̍쐬
BOOL CglDIB::CreateGLDIB(int inputWidth,		//�������̃T�C�Y
						 int inputHeight,
						 int inputColor,		//�������̐F��
						 int inputDepth			//�������̃f�v�X�o�b�t�@
						 ){
	width = inputWidth;
	height = inputHeight;
	
	//�r�b�g�}�b�v���
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
	//�݊��f�o�C�X�R���e�L�X�g�̍쐬
	m_hDC = CreateCompatibleDC(NULL);
	//�r�b�g�}�b�v�������쐬
	hBitmap = CreateDIBSection(m_hDC,
		(BITMAPINFO*)BitmapInfo,
		DIB_RGB_COLORS,
		&data,
		NULL,
		0
		);
    //�ȑO�̃r�b�g�}�b�v����������ޔ�������
	if(hBitmap){
		hBitmapOld = (HBITMAP)SelectObject(m_hDC,hBitmap);
	}
	//�s�N�Z���t�H�[�}�b�g�ݒ�
	int pixelformat;
	static PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),		//���̍\���̂̃T�C�Y
		1,									//OpenGL�o�[�W����
		PFD_DRAW_TO_BITMAP |                //�r�b�g�}�b�v�ݒ�
		PFD_SUPPORT_OPENGL |                    
		PFD_SUPPORT_GDI,
		PFD_TYPE_RGBA,						//RGBA�J���[
		inputColor,							//�F��
		0, 0,								//RGBA�̃r�b�g�ƃV�t�g�ݒ�		
		0, 0,								//G
		0, 0,								//B
		0, 0,								//A
		0,									//�A�L�������[�V�����o�b�t�@
		0, 0, 0, 0, 						//RGBA�A�L�������[�V�����o�b�t�@
		inputDepth,									//Z�o�b�t�@	
		0,									//�X�e���V���o�b�t�@
		0,									//�g�p���Ȃ�
		PFD_MAIN_PLANE,						//���C���[�^�C�v
		0,									//�\��
		0, 0, 0								//���C���[�}�X�N�̐ݒ�E���g�p
    };
    //�s�N�Z���t�H�[�}�b�g�̑I��
    if((pixelformat = ChoosePixelFormat(m_hDC, &pfd))==0){
		return FALSE;
	}
    //�s�N�Z���t�H�[�}�b�g�̓���
    if(SetPixelFormat(m_hDC, pixelformat, &pfd)==FALSE){
		return FALSE;
	}
	//�����_�����O�R���e�L�X�g�̍쐬
	if((m_hRC = wglCreateContext(m_hDC))==NULL){
		return FALSE;
	}
	return TRUE;
};
///////////////////////////////////////////////////////////////////////////////
HDC	CglDIB::GetDC(void){
	//�r�b�g�}�b�v�������N���X�̃R���e�L�X�g��Ԃ�
	return m_hDC;
}
///////////////////////////////////////////////////////////////////////////////
HBITMAP CglDIB::GetBitmap(void){
	//�r�b�g�}�b�v�������N���X�̃r�b�g�}�b�v�n���h����Ԃ�
	return hBitmap;
}
