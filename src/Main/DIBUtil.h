#ifndef DIB_UTIL_H
#define DIB_UTIL_H

//BOOL WriteWindow2Mem(BYTE* buf, int W, int H, HDC hDC);
BOOL WriteWindowToDIB( LPTSTR szFile, CWnd* wnd);
void WritePPM(const char* fileName, BYTE* buf, int _W, int _H);

void RGB2YCbCr(BYTE R, BYTE G, BYTE B, BYTE &Y, BYTE &Cb, BYTE &Cr);
void YCbCr2RGB(BYTE Y, BYTE Cb, BYTE Cr,BYTE &R, BYTE &G, BYTE &B);

#endif //DIB_UTIL_H
