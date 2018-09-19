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

   return bRet;
}

BOOL CScreenImage::CaptureRectSave(const CRect& rect, CString filename)
{
	// detach and destroy the old bitmap if any attached
	CImage::Destroy();

	TRACE("filename : %s\n", filename);

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
	saveBitmap(filename, hBitmap, hpal);
	return bRet;
}

BOOL CScreenImage::CheckBitmapFile(LPCSTR filename, CRect& rect)
{
	// detach and destroy the old bitmap if any attached
	CImage::Destroy();
	
	TCHAR currentDir[MAX_PATH];
	CString newfile;
	GetCurrentDirectory(MAX_PATH, currentDir);
	newfile.Format("%s\\temp.bmp", currentDir);

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
	if (!bRet)
	{
		TRACE("File to screen capture!!!\n");
		return bRet;
	}
	// attach bitmap handle to this object
	Attach(hBitmap);

	// restore the memory DC and perform cleanup
	::SelectObject(hDCMem, bitmap);

	::DeleteDC(hDCMem);
	::DeleteDC(hDCScreen);
	
	//Save new file
	HPALETTE hpal = NULL;
	saveBitmap(newfile, hBitmap, hpal);

	//Load HBITMAP with file name
	TRACE("Old filename : %s\n", filename);
	HBITMAP fileBMP = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	HBITMAP newfileBMP = (HBITMAP)LoadImage(NULL, newfile, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);


	bRet = CompareBitmaps(fileBMP, newfileBMP);
	if (bRet)
		TRACE("Two Bitmap is same\n");
	else
		TRACE("Two Bitmap is different\n");
	return bRet;
}

BOOL CScreenImage::CompareBitmaps(HBITMAP HBitmapLeft, HBITMAP HBitmapRight)
{
	if (HBitmapLeft == HBitmapRight)
	{
		return true;
	}

	if (NULL == HBitmapLeft || NULL == HBitmapRight)
	{
		return false;
	}

	bool bSame = false;

	HDC hdc = ::GetDC(NULL);
	BITMAPINFO BitmapInfoLeft = { 0 };
	BITMAPINFO BitmapInfoRight = { 0 };

	BitmapInfoLeft.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	BitmapInfoRight.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

	if (0 != GetDIBits(hdc, HBitmapLeft, 0, 0, NULL, &BitmapInfoLeft, DIB_RGB_COLORS) &&
		0 != GetDIBits(hdc, HBitmapRight, 0, 0, NULL, &BitmapInfoRight, DIB_RGB_COLORS))
	{
		// Compare the BITMAPINFOHEADERs of the two bitmaps

		if (0 == memcmp(&BitmapInfoLeft.bmiHeader, &BitmapInfoRight.bmiHeader,
			sizeof(BITMAPINFOHEADER)))
		{
			// The BITMAPINFOHEADERs are the same so now compare the actual bitmap bits

			BYTE *pLeftBits = new BYTE[BitmapInfoLeft.bmiHeader.biSizeImage];
			BYTE *pRightBits = new BYTE[BitmapInfoRight.bmiHeader.biSizeImage];
			BYTE *pByteLeft = NULL;
			BYTE *pByteRight = NULL;

			PBITMAPINFO pBitmapInfoLeft = &BitmapInfoLeft;
			PBITMAPINFO pBitmapInfoRight = &BitmapInfoRight;

			// calculate the size in BYTEs of the additional

			// memory needed for the bmiColor table

			int AdditionalMemory = 0;
			switch (BitmapInfoLeft.bmiHeader.biBitCount)
			{
			case 1:
				AdditionalMemory = 1 * sizeof(RGBQUAD);
				break;
			case 4:
				AdditionalMemory = 15 * sizeof(RGBQUAD);
				break;
			case 8:
				AdditionalMemory = 255 * sizeof(RGBQUAD);
				break;
			case 16:
			case 32:
				AdditionalMemory = 2 * sizeof(RGBQUAD);
			}

			if (AdditionalMemory)
			{
				// we have to allocate room for the bmiColor table that will be

				// attached to our BITMAPINFO variables

				pByteLeft = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
				if (pByteLeft)
				{
					memset(pByteLeft, 0, sizeof(BITMAPINFO) + AdditionalMemory);
					memcpy(pByteLeft, pBitmapInfoLeft, sizeof(BITMAPINFO));
					pBitmapInfoLeft = (PBITMAPINFO)pByteLeft;
				}

				pByteRight = new BYTE[sizeof(BITMAPINFO) + AdditionalMemory];
				if (pByteRight)
				{
					memset(pByteRight, 0, sizeof(BITMAPINFO) + AdditionalMemory);
					memcpy(pByteRight, pBitmapInfoRight, sizeof(BITMAPINFO));
					pBitmapInfoRight = (PBITMAPINFO)pByteRight;
				}
			}

			if (pLeftBits && pRightBits && pBitmapInfoLeft && pBitmapInfoRight)
			{
				// zero out the bitmap bit buffers

				memset(pLeftBits, 0, BitmapInfoLeft.bmiHeader.biSizeImage);
				memset(pRightBits, 0, BitmapInfoRight.bmiHeader.biSizeImage);

				// fill the bit buffers with the actual bitmap bits

				if (0 != GetDIBits(hdc, HBitmapLeft, 0,
					pBitmapInfoLeft->bmiHeader.biHeight, pLeftBits, pBitmapInfoLeft,
					DIB_RGB_COLORS) && 0 != GetDIBits(hdc, HBitmapRight, 0,
						pBitmapInfoRight->bmiHeader.biHeight, pRightBits, pBitmapInfoRight,
						DIB_RGB_COLORS))
				{
					// compare the actual bitmap bits of the two bitmaps

					bSame = 0 == memcmp(pLeftBits, pRightBits,
						pBitmapInfoLeft->bmiHeader.biSizeImage);
				}
			}

			// clean up

			delete[] pLeftBits;
			delete[] pRightBits;
			delete[] pByteLeft;
			delete[] pByteRight;
		}
	}

	::ReleaseDC(NULL, hdc);

	return bSame;
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

