// Author:  Ovidiu Cucu
// Website: http://www.codexpert.ro/

#pragma once

#include <atlimage.h>

class CScreenImage : public CImage
{
public:
    BOOL CaptureRect(const CRect& rect) throw();
	BOOL CaptureRectSave(const CRect& rect, CString filename) throw();
    BOOL CaptureScreen() throw();
    BOOL CaptureWindow(HWND hWnd) throw();
	BOOL saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal);
	BOOL CheckBitmapFile(LPCSTR filename, CRect& rect);
};
