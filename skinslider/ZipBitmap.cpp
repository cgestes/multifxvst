// ZipBitmap.cpp : implementation file
//

#include "stdafx.h"
#include "ZipBitmap.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


COLORREF rgbWhite = RGB(255,255,255);

/////////////////////////////////////////////////////////////////////////////
// CZipBitmap
CZipBitmap::CZipBitmap()
{
    m_iWidth = 0;
    m_iHeight = 0;
}

CZipBitmap::~CZipBitmap()
{
}

void CZipBitmap::GetMetrics()
{
    // Get the width and height
    BITMAP bm;
    GetObject(sizeof(bm), &bm);
    m_iWidth = bm.bmWidth;
    m_iHeight = bm.bmHeight;
}

int CZipBitmap::GetWidth()
{
    if (!m_iWidth)
        GetMetrics();

    return m_iWidth;
}

int CZipBitmap::GetHeight()
{
    if (!m_iHeight)
        GetMetrics();

    return m_iHeight;
}

CZipTransBitmap::CZipTransBitmap()
{
    m_hbmMask = NULL;
}

CZipTransBitmap::~CZipTransBitmap()
{
	if (m_hbmMask)
		::DeleteObject(m_hbmMask);
}

void CZipTransBitmap::CreateMask(UINT uiTickImage, COLORREF colTrans)
{
	LoadBitmap(uiTickImage);
    // Create a monochrome bitmap for the mask
    m_hbmMask = ::CreateBitmap(GetWidth(), GetHeight(), 1, 1, NULL);

    // Create memory DCs to work with
    HDC hdcMask = ::CreateCompatibleDC(NULL);
    HBITMAP hbmOldMask = (HBITMAP)::SelectObject(hdcMask, m_hbmMask);

    HDC hdcImage = ::CreateCompatibleDC(NULL);
    HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcImage, m_hObject);

	if (colTrans == -1)
		colTrans = ::GetPixel(hdcImage, 0, 0);	// Set the transparency color to be the top-left pixel

    // Make the mask
    ::SetBkColor(hdcImage,colTrans);
    ::BitBlt(hdcMask, 0, 0, GetWidth(), GetHeight(), hdcImage, 0, 0, SRCCOPY);

	// Apply the mask to the image
    ::BitBlt(hdcImage, 0, 0, GetWidth(), GetHeight(), hdcMask, 0, 0, SRCINVERT);

    // Tidy up
    ::SelectObject(hdcMask, hbmOldMask);
    ::SelectObject(hdcImage, hbmOldImage);
    ::DeleteDC(hdcMask);
    ::DeleteDC(hdcImage);
}

void CZipTransBitmap::DrawTrans(HDC ScreenDC, int x, int y)
{
    int dx = GetWidth();
    int dy = GetHeight();

    // Create a memory DC to which to draw
    HDC hdcOffScr = ::CreateCompatibleDC(NULL);

    // AND the destination with the mask
    HBITMAP hbmOldImage = (HBITMAP)::SelectObject(hdcOffScr, m_hbmMask);
    ::BitBlt(ScreenDC, x, y, dx, dy, hdcOffScr, 0, 0, SRCAND);

    // XOR the destination with the masked tick image
    ::SelectObject(hdcOffScr, m_hObject);
    ::BitBlt(ScreenDC, x, y, dx, dy, hdcOffScr, 0, 0, SRCINVERT);

    // Tidy up
    ::SelectObject(hdcOffScr, hbmOldImage);
    ::DeleteDC(hdcOffScr);
}

CZipBackGroundBitmap::CZipBackGroundBitmap()
{
	hDC = NULL;
}

CZipBackGroundBitmap::~CZipBackGroundBitmap()
{
	if (hDC)
	{
		::SelectObject(hDC, hbmold);
		::DeleteDC(hDC);
	}
}

void CZipBackGroundBitmap::CreateBackBitmap(CRect& Area, int uiBackGround, int Options, int TickWidth, int TickHeight)
{
	// Clear background
	hDC = ::CreateCompatibleDC(NULL);
	CreateBitmap(Area.Width(), Area.Height(), 1, ::GetDeviceCaps(hDC, BITSPIXEL), NULL);
	hbmold = (HBITMAP)::SelectObject(hDC, (HBITMAP)(m_hObject));

	HBRUSH hBR = CreateSolidBrush(GetSysColor(COLOR_3DFACE));
	::FillRect(hDC, Area, hBR);
	::DeleteObject(hBR);

	// Copy Background picture
	if (uiBackGround)
	{
		CZipBitmap Pic;
		Pic.LoadBitmap(uiBackGround);
		HDC hdcMem = ::CreateCompatibleDC(NULL);
		HBITMAP hbmold = (HBITMAP)::SelectObject(hdcMem, (HBITMAP)(Pic.m_hObject));

		int Left, Top;
		Left = Top = 0;
		if (Options & BG_CENTER)
		{
			Left = (GetWidth() - Pic.GetWidth()) / 2;
			Top = (GetHeight() - Pic.GetHeight()) / 2;
		}
		else if (Options & BG_CENTER_ONTICK)
		{
			Left = (GetWidth() - Pic.GetWidth()) / 2;
			Top = (TickHeight - Pic.GetHeight()) / 2;
		}
		else if (Options & BG_CENTER_ONTICK_VERT)
		{
			Left = (TickWidth - Pic.GetWidth()) / 2;
			Top = (GetHeight() - Pic.GetHeight()) / 2;
		}
		if (Options & BG_STARTATICK)
		{
			Left = iMarginWidth - TickWidth / 2;
		}
		if (Options & BG_STARTATICK_VERT)
		{
			Top = iMarginWidth - TickHeight / 2;
		}

		// Blt the bits
		if (Options & BG_STRETCH_HOR)
		{
			int SpreadWidth = Area.Width();
			if (Options & BG_STARTATICK)
				SpreadWidth -= Left * 2;
			::StretchBlt(hDC, Left, Top, SpreadWidth, Pic.GetHeight(), hdcMem, 0, 0, Pic.GetWidth(), Pic.GetHeight(), SRCCOPY);
		}
		else if (Options & BG_STRETCH_VERT)
		{
			int SpreadHeight = Area.Height();
			if (Options & BG_STARTATICK_VERT)
				SpreadHeight -= Top * 2;
			::StretchBlt(hDC, Left, Top, Pic.GetWidth(), SpreadHeight, hdcMem, 0, 0, Pic.GetWidth(), Pic.GetHeight(), SRCCOPY);
		}
		else
			::BitBlt(hDC, Left, Top, Pic.GetWidth(), Pic.GetHeight(), hdcMem, 0, 0, SRCCOPY);

		::SelectObject(hdcMem, hbmold);
		::DeleteDC(hdcMem);
	}
}

void CZipBackGroundBitmap::DrawBackGround(HDC ScreenDC, const CRect& Area)
{
	::BitBlt(ScreenDC, Area.left, Area.top, Area.Width(), Area.Height(), hDC, Area.left, Area.top, SRCCOPY);
}

