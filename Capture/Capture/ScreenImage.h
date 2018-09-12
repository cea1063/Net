// Author:  Ovidiu Cucu
// Website: http://www.codexpert.ro/

#pragma once

#include <atlimage.h>

class CScreenImage : public CImage
{
public:
    BOOL CaptureRect(const CRect& rect) throw();
    BOOL CaptureScreen() throw();
    BOOL CaptureWindow(HWND hWnd) throw();
	/*HBITMAP bitmap;
	HDC hDCScreen;
	HDC hDCMem;
	HBITMAP hBitmap;*/
#if 1
	BOOL saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal);
#else
	BOOL HDCToFile(const char* FilePath, HDC Context, RECT Area, UINT BitsPerPixel);
#endif
};
