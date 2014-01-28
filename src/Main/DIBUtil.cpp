#include "StdAfx.h"
#include "DIBUtil.h"

void RGB2YCbCr(BYTE R, BYTE G, BYTE B, BYTE &Y, BYTE &Cb, BYTE &Cr)
{
	int y, cb, cr;
	y  =  0.29900 * R + 0.58700 * G + 0.11400 * B;
	cb = -0.29900 * R - 0.58700 * G + 0.88600 * B + 0x80;
	cr =  0.70100 * R - 0.58700 * G - 0.11400 * B + 0x80;

	if (y < 0) y = 0; if (y > 255) y = 255; Y = y;
	if (cb < 0) cb = 0; if (cb > 255) cb = 255; Cb = cb;
	if (cr < 0) cr = 0; if (cr > 255) cr = 255; Cr = cr;
}


void YCbCr2RGB(BYTE Y, BYTE Cb, BYTE Cr,BYTE &R, BYTE &G, BYTE &B)
{
	int r,g,b;
	Cb -= 0x80;
	Cr -= 0x80;
	b =  Y + Cb;
	r =  Y + Cr;
	g = (Y - 0.29900 * r - 0.11400 * b) / 0.58700;

	if (r < 0) r = 0; if (r > 255) r = 255; R = r;
	if (g < 0) g = 0; if (g > 255) g = 255; G = g;
	if (b < 0) b = 0; if (b > 255) b = 255; B = b;
}


// DDBToDIB		- Creates a DIB from a DDB
// bitmap		- Device dependent bitmap
// dwCompression	- Type of compression - see BITMAPINFOHEADER
// pPal			- Logical palette
HANDLE DDBToDIB( CBitmap& bitmap, DWORD dwCompression, CPalette* pPal )
{
	BITMAP			bm;
	BITMAPINFOHEADER	bi;
	LPBITMAPINFOHEADER 	lpbi;
	DWORD			dwLen;
	HANDLE			hDIB;
	HANDLE			handle;
	HDC 			hDC;
	HPALETTE		hPal;


	ASSERT( bitmap.GetSafeHandle() );

	// The function has no arg for bitfields
	if( dwCompression == BI_BITFIELDS )
		return NULL;

	// If a palette has not been supplied use defaul palette
	hPal = (HPALETTE) pPal->GetSafeHandle();
	if (hPal==NULL)
		hPal = (HPALETTE) GetStockObject(DEFAULT_PALETTE);

	// Get bitmap information
	bitmap.GetObject(sizeof(bm),(LPSTR)&bm);

	// Initialize the bitmapinfoheader
	bi.biSize		= sizeof(BITMAPINFOHEADER);
	bi.biWidth		= bm.bmWidth;
	bi.biHeight 		= bm.bmHeight;
	bi.biPlanes 		= 1;
	bi.biBitCount		= bm.bmPlanes * bm.bmBitsPixel;
	bi.biCompression	= dwCompression;
	bi.biSizeImage		= 0;
	bi.biXPelsPerMeter	= 0;
	bi.biYPelsPerMeter	= 0;
	bi.biClrUsed		= 0;
	bi.biClrImportant	= 0;

	// Compute the size of the  infoheader and the color table
	int nColors = (1 << bi.biBitCount);
	if( nColors > 256 )
		nColors = 0;
	dwLen  = bi.biSize + nColors * sizeof(RGBQUAD);

	// We need a device context to get the DIB from
	hDC = GetDC(NULL);
	hPal = SelectPalette(hDC,hPal,FALSE);
	RealizePalette(hDC);

	// Allocate enough memory to hold bitmapinfoheader and color table
	hDIB = GlobalAlloc(GMEM_FIXED,dwLen);

	if (!hDIB){
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	*lpbi = bi;

	// Call GetDIBits with a NULL lpBits param, so the device driver 
	// will calculate the biSizeImage field 
	GetDIBits(hDC, (HBITMAP)bitmap.GetSafeHandle(), 0L, (DWORD)bi.biHeight,
			(LPBYTE)NULL, (LPBITMAPINFO)lpbi, (DWORD)DIB_RGB_COLORS);

	bi = *lpbi;

	// If the driver did not fill in the biSizeImage field, then compute it
	// Each scan line of the image is aligned on a DWORD (32bit) boundary
	if (bi.biSizeImage == 0){
		bi.biSizeImage = ((((bi.biWidth * bi.biBitCount) + 31) & ~31) / 8)
						* bi.biHeight;

		// If a compression scheme is used the result may infact be larger
		// Increase the size to account for this.
		if (dwCompression != BI_RGB)
			bi.biSizeImage = (bi.biSizeImage * 3) / 2;
	}

	// Realloc the buffer so that it can hold all the bits
	dwLen += bi.biSizeImage;
	if (handle = GlobalReAlloc(hDIB, dwLen, GMEM_MOVEABLE))
		hDIB = handle;
	else{
		GlobalFree(hDIB);

		// Reselect the original palette
		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	// Get the bitmap bits
	lpbi = (LPBITMAPINFOHEADER)hDIB;

	// FINALLY get the DIB
	BOOL bGotBits = GetDIBits( hDC, (HBITMAP)bitmap.GetSafeHandle(),
				0L,				// Start scan line
				(DWORD)bi.biHeight,		// # of scan lines
				(LPBYTE)lpbi 			// address for bitmap bits
				+ (bi.biSize + nColors * sizeof(RGBQUAD)),
				(LPBITMAPINFO)lpbi,		// address of bitmapinfo
				(DWORD)DIB_RGB_COLORS);		// Use RGB for color table

	if( !bGotBits )
	{
		GlobalFree(hDIB);

		SelectPalette(hDC,hPal,FALSE);
		ReleaseDC(NULL,hDC);
		return NULL;
	}

	SelectPalette(hDC,hPal,FALSE);
	ReleaseDC(NULL,hDC);
	return hDIB;
}

BOOL WriteDIB2Mem(HANDLE hDIB, int W, int H, BYTE* buf)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	int nColors = 1 << lpbi->biBitCount;

	// Fill in the fields of the file header 
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= GlobalSize (hDIB) + sizeof( hdr );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

	BYTE* pCurrent	= reinterpret_cast<BYTE*>(lpbi + lpbi->biSize);
	size_t nSize	= lpbi->biWidth * lpbi->biHeight;

	if (lpbi->biBitCount == 24 )
	{
		for (int i = 0;i < nSize; i++)
		{
			buf[3*i + 0] = *(pCurrent + 0);
			buf[3*i + 1] = *(pCurrent + 1);
			buf[3*i + 2] = *(pCurrent + 2);

			pCurrent += 3;
		}
		return true;
	}
	else if (lpbi->biBitCount == 32)
	{
		for (int i = 0;i < nSize; i++)
		{
			buf[3*i + 0] = *(pCurrent + 0);
			buf[3*i + 1] = *(pCurrent + 1);
			buf[3*i + 2] = *(pCurrent + 2);

			pCurrent += 4;
		}
		return true;
	}
	else
	{
		return false;
	}

	return TRUE;
}

// WriteDIB		- Writes a DIB to file
// Returns		- TRUE on success
// szFile		- Name of file to write to
// hDIB			- Handle of the DIB
BOOL WriteDIB( LPTSTR szFile, HANDLE hDIB)
{
	BITMAPFILEHEADER	hdr;
	LPBITMAPINFOHEADER	lpbi;

	if (!hDIB)
		return FALSE;

	CFile file;
	if( !file.Open( szFile, CFile::modeWrite|CFile::modeCreate) )
		return FALSE;

	lpbi = (LPBITMAPINFOHEADER)hDIB;

	int nColors = 1 << lpbi->biBitCount;

	// Fill in the fields of the file header 
	hdr.bfType		= ((WORD) ('M' << 8) | 'B');	// is always "BM"
	hdr.bfSize		= GlobalSize (hDIB) + sizeof( hdr );
	hdr.bfReserved1 	= 0;
	hdr.bfReserved2 	= 0;
	hdr.bfOffBits		= (DWORD) (sizeof( hdr ) + lpbi->biSize +
						nColors * sizeof(RGBQUAD));

	// Write the file header 
	file.Write( &hdr, sizeof(hdr) );

	// Write the DIB header and the bits 
	file.Write( lpbi, GlobalSize(hDIB) );

	return TRUE;
}

BOOL WriteWindowToDIB( LPTSTR szFile, CWnd* wnd)
{
	CDC *pDC = wnd->GetDC();

	CBitmap 	bitmap;
	CDC 		memDC;
	CRect		rect;

	int W = 640;
	int H = 480;

	memDC.CreateCompatibleDC(pDC);

	wnd->GetWindowRect(rect);

//	W = rect.Width() - 10;
//	H = rect.Height() - 10;

	bitmap.CreateCompatibleBitmap(pDC, W,H );

	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	int x = rect.Width()  - W; x /= 2;
	int y = rect.Height() - H; y /= 2;

	memDC.BitBlt(0, 0, W,H, pDC, x, y, SRCCOPY);

	// Create logical palette if device support a palette
	CPalette pal;
	if( pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries =
			GetSystemPaletteEntries( *pDC, 0, 255, pLP->palPalEntry );

		// Create the palette
		pal.CreatePalette( pLP );

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// Convert the bitmap to a DIB
	HANDLE hDIB = DDBToDIB( bitmap, BI_RGB, &pal );

	if( hDIB == NULL )
		return FALSE;

	// Write it to file
	WriteDIB( szFile, hDIB );

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree( hDIB );

	wnd->ReleaseDC(pDC);
	return TRUE;
}

BOOL WriteWindow2Mem(BYTE* buf, int W, int H, HDC hDC)
{
	CBitmap 	bitmap;
	CDC 		memDC;
	CRect		rect;

	CDC dc;
	dc.Attach(hDC);
	CDC* pDC = &dc;

	bitmap.CreateCompatibleBitmap(pDC, W,H );

	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	memDC.BitBlt(0, 0, W,H, pDC, 0, 0, SRCCOPY);

	// Create logical palette if device support a palette
	CPalette pal;
	if( pDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE )
	{
		UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * 256);
		LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
		pLP->palVersion = 0x300;

		pLP->palNumEntries =
			::GetSystemPaletteEntries( *pDC, 0, 255, pLP->palPalEntry );

		// Create the palette
		pal.CreatePalette( pLP );

		delete[] pLP;
	}

	memDC.SelectObject(pOldBitmap);

	// Convert the bitmap to a DIB
	HANDLE hDIB = DDBToDIB( bitmap, BI_RGB, &pal );

	if( hDIB == NULL )
		return FALSE;

	// Write it to file
	WriteDIB2Mem(hDIB, W, H, buf);

	// Free the memory allocated by DDBToDIB for the DIB
	GlobalFree( hDIB );

	dc.Detach();

	return TRUE;
}

void WritePPM(const char* fileName, BYTE* buf, int _W, int _H)
{
	CString filename = fileName;
	filename = filename + ".ppm";
	CFile file;
	if(!file.Open(filename,CFile::modeWrite | CFile::modeCreate))return;

	file.Write("P6\n",strlen("P6\n"));
	char buff[256];
	sprintf(buff,"%d %d\n",_W,_H);
	file.Write(buff,strlen(buff));
	file.Write("255\n",strlen("255\n"));

	file.Write(buf,_W*_H*3);
	file.Close();
}