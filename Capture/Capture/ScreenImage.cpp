// Author:  Ovidiu Cucu
// Website: http://www.codexpert.ro/

#include "StdAfx.h"
#include "ScreenImage.h"

#include <vector>
#include <fstream>
#include <cstring>
#include <windows.h>

/****************************************************************************************
 Function:   CScreenImage::CaptureRect
 Purpose:    captures a screen rectangle
 Parameters: rect: screen rectangle to be captured
 Return:     non-zero value if successful
****************************************************************************************/
BOOL CScreenImage::CaptureRect(const CRect& rect)
{
   // detach and destroy the old bitmap if any attached
   CImage::Destroy();
   TCHAR currentDir[MAX_PATH];
   GetCurrentDirectory(MAX_PATH, currentDir);
   BMP_filename.Format("%s\\test.bmp",currentDir);
   TRACE("filename : %s\n", BMP_filename);
   
   // create a screen and a memory device context
   HDC hDCScreen = ::CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
   HDC hDCMem = ::CreateCompatibleDC(hDCScreen);
   // create a compatible bitmap and select it in the memory DC
   HBITMAP hBitmap =
      ::CreateCompatibleBitmap(hDCScreen, rect.Width(), rect.Height());
   HBITMAP bitmap = (HBITMAP)::SelectObject(hDCMem, hBitmap);

   // bit-blit from screen to memory device context
   // note: CAPTUREBLT flag is required to capture layered windows
   DWORD dwRop = SRCCOPY | CAPTUREBLT;
   BOOL bRet = ::BitBlt(hDCMem, 0, 0, rect.Width(), rect.Height(), 
                        hDCScreen, 
                        rect.left, rect.top, dwRop);

   // attach bitmap handle to this object
   Attach(hBitmap);
   
   // restore the memory DC and perform cleanup
   ::SelectObject(hDCMem, bitmap);
   
   ::DeleteDC(hDCMem);
   ::DeleteDC(hDCScreen);
   HPALETTE hpal = NULL;
   saveBitmap(BMP_filename, hBitmap, hpal);
   return bRet;
}


BOOL CScreenImage::saveBitmap(LPCSTR filename, HBITMAP bmp, HPALETTE pal)
{
	bool result = false;
	PICTDESC pd;

	pd.cbSizeofstruct = sizeof(PICTDESC);
	pd.picType = PICTYPE_BITMAP;
	pd.bmp.hbitmap = bmp;
	pd.bmp.hpal = pal;

	LPPICTURE picture;
	HRESULT res = OleCreatePictureIndirect(&pd, IID_IPicture, false,
		reinterpret_cast<void**>(&picture));

	if (!SUCCEEDED(res))
		return false;

	LPSTREAM stream;
	res = CreateStreamOnHGlobal(0, true, &stream);

	if (!SUCCEEDED(res))
	{
		picture->Release();
		return false;
	}

	LONG bytes_streamed;
	res = picture->SaveAsFile(stream, true, &bytes_streamed);

	HANDLE file = CreateFile(filename, GENERIC_WRITE, FILE_SHARE_READ, 0,
		CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (!SUCCEEDED(res) || !file)
	{
		stream->Release();
		picture->Release();
		return false;
	}

	HGLOBAL mem = 0;
	GetHGlobalFromStream(stream, &mem);
	LPVOID data = GlobalLock(mem);

	DWORD bytes_written;

	result = !!WriteFile(file, data, bytes_streamed, &bytes_written, 0);
	result &= (bytes_written == static_cast<DWORD>(bytes_streamed));

	GlobalUnlock(mem);
	CloseHandle(file);

	stream->Release();
	picture->Release();

	return result;
}


/****************************************************************************************
 Function:   CScreenImage::CaptureScreen
 Purpose:    captures the whole screen
 Parameters: none
 Return:     non-zero value if successful
****************************************************************************************/
BOOL CScreenImage::CaptureScreen()
{
   CRect rect(0, 0, ::GetSystemMetrics(SM_CXSCREEN), ::GetSystemMetrics(SM_CYSCREEN));
   return CaptureRect(rect);
}

/****************************************************************************************
 Function:   CScreenImage::CaptureWindow
 Purpose:    captures a window
 Parameters: hWnd - the window handle
 Return:     non-zero value if successful
****************************************************************************************/
BOOL CScreenImage::CaptureWindow(HWND hWnd)
{
   BOOL bRet = FALSE;
   if(::IsWindow(hWnd))
   {
      CRect rect;
      ::GetWindowRect(hWnd, rect);
      bRet = CaptureRect(rect);
   }
   return bRet;
}

