// MySliderCtl.cpp : implementation file
//

#include "stdafx.h"
#include "ZipSliderCtl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

HWND CZipSliderCtl::hwndTT = NULL;
TOOLINFO CZipSliderCtl::ti;

/////////////////////////////////////////////////////////////////////////////
// CZipSliderCtl

CZipSliderCtl::CZipSliderCtl()
{
	m_bDragging = false;
	m_ButDown = false;
	m_hCursor = NULL;
	m_bmTrans = NULL;
	m_bmTransDrag = NULL;
	m_bDisplayTT = false;
	m_bTrackTT = false;
}

CZipSliderCtl::~CZipSliderCtl()
{
	delete m_bmTrans;
	delete m_bmTransDrag;
	delete m_bmpBack;
}

BEGIN_MESSAGE_MAP(CZipSliderCtl, CSliderCtrl)
	//{{AFX_MSG_MAP(CZipSliderCtl)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CZipSliderCtl message handlers

void CZipSliderCtl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
	{	// Sort out ToolTip
		if (m_bDisplayTT)
			UpdateTTText();

		if (m_bTrackTT)
			UpdateTTPos();
	}

	CRect rcStartTick, rcEndTick;
	if (m_ButDown)
		GetTickRect(rcStartTick);

	CSliderCtrl::OnMouseMove(nFlags, point);

	if (m_ButDown)
	{
		GetTickRect(rcEndTick);
		CRect invalid;
		if (invalid.UnionRect(rcStartTick, rcEndTick))
			InvalidateRect(invalid, FALSE);
	}
}

void CZipSliderCtl::GetTickRect(CRect& rcTick) 
{
	int iMax,iMin;
	GetRange(iMin,iMax);
	double Range = iMax - iMin;
	double Percent = (GetPos() - iMin) / Range;

	rcTick = CArea;
	if (m_Vert)
	{
		int iTickHeight = m_bmTrans->GetHeight();
		double SlideHeight = rcTick.Height() - iMarginWidth * 2 - 1;
		rcTick.top += iMarginWidth + (int)(SlideHeight * Percent) - (iTickHeight / 2);
		rcTick.bottom = rcTick.top + m_bmTrans->GetHeight();
		rcTick.right = m_bmTrans->GetWidth();
	}
	else
	{
		int iTickWidth = m_bmTrans->GetWidth();
		double SlideWidth = rcTick.Width() - iMarginWidth * 2 - 1;
		rcTick.left += iMarginWidth + (int)(SlideWidth * Percent) - (iTickWidth / 2);
		rcTick.right = rcTick.left + m_bmTrans->GetWidth();
		rcTick.bottom = m_bmTrans->GetHeight();
	}
}

void CZipSliderCtl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect ClipArea;

	if (dc.GetClipBox(ClipArea))
	{
		CRect rcBack, rcTick;
		rcBack = CArea;
		GetTickRect(rcTick);

		if (m_bmpBack)
		{
			m_bmpBack->DrawBackGround(dc, ClipArea);
		}

		if (m_bDragging && m_bmTransDrag)
		{
			m_bmTransDrag->DrawTrans(dc, rcTick.left, rcTick.top);
		}
		else
		{
			if (m_bmTrans)
				m_bmTrans->DrawTrans(dc, rcTick.left, rcTick.top);
		}
	}
}

void CZipSliderCtl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bDragging)
		::SendMessage(hwndTT, TTM_TRACKACTIVATE, false, (LPARAM)(LPTOOLINFO) &ti);

	m_bDragging = m_ButDown = false;
	CSliderCtrl::OnLButtonUp(nFlags, point);
}

void CZipSliderCtl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_ButDown = true;

	CRect rcTick;
	GetTickRect(rcTick);
	if (rcTick.PtInRect(point))
	{
		if (m_bDisplayTT)
			CreateTT();

		m_bDragging = true;
	}

	InvalidateRect(rcTick, FALSE);
	CSliderCtrl::OnLButtonDown(nFlags, point);
	// Need to redraw tick as doesn't always get fully drawn...
	GetTickRect(rcTick);
	InvalidateRect(rcTick, FALSE);
}

void CZipSliderCtl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	Invalidate(FALSE);
}

void CZipSliderCtl::SetupTT(int Options)
{
	m_bTrackTT = (Options & BG_SHOW_TOOLTIP_TRACK) == BG_SHOW_TOOLTIP_TRACK;
	m_bDisplayTT = (Options & BG_SHOW_TOOLTIP) || m_bTrackTT;
}

void CZipSliderCtl::SetSkin(UINT uiBackGround, UINT uiTickImage, UINT uiTickDragImage, int Options)
{
	m_Vert = (GetStyle() & TBS_VERT) == TBS_VERT;

	SetupTT(Options);

	GetClientRect(&CArea);

	if (uiTickDragImage)
	{
		m_bmTransDrag = new CZipTransBitmap();
		m_bmTransDrag->CreateMask(uiTickDragImage);
	}

	m_bmTrans = new CZipTransBitmap();
	m_bmTrans->CreateMask(uiTickImage);

	m_bmpBack = new CZipBackGroundBitmap();
	m_bmpBack->CreateBackBitmap(CArea, uiBackGround, Options, m_bmTrans->GetWidth(), m_bmTrans->GetHeight());
}

void CZipSliderCtl::SetCursor(short nCursor)
{
	m_hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(nCursor));
}

BOOL CZipSliderCtl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	else
		return CSliderCtrl::OnSetCursor(pWnd, nHitTest, message);
}

void CZipSliderCtl::SetPos(int nPos)
{
	CRect rcStartTick, rcEndTick;
	GetTickRect(rcStartTick);

	CSliderCtrl::SetPos(nPos);

	GetTickRect(rcEndTick);
	CRect invalid;
	if (invalid.UnionRect(rcStartTick, rcEndTick))
		InvalidateRect(invalid, FALSE);
}

///////////////////////////////
// Tool tip functions
///////////////////////////////

void CZipSliderCtl::CreateTT()
{
	if (!hwndTT)
	{
		// CREATE A TOOLTIP WINDOW
		hwndTT = CreateWindowEx(WS_EX_TOPMOST, TOOLTIPS_CLASS, NULL, TTS_NOPREFIX | TTS_ALWAYSTIP,		
								CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
								NULL, NULL, NULL, NULL);

		// INITIALIZE MEMBERS OF THE TOOLINFO STRUCTURE
		ZeroMemory(&ti, sizeof(ti));
		ti.cbSize = sizeof(TOOLINFO);
		ti.uFlags = TTF_TRACK;
		ti.lpszText = GetTTString();

		// SEND AN ADDTOOL MESSAGE TO THE TOOLTIP CONTROL WINDOW
		::SendMessage(hwndTT, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &ti);
	}
	UpdateTTPos();
	::SendMessage(hwndTT, TTM_TRACKACTIVATE, true, (LPARAM)(LPTOOLINFO) &ti);
}

char* CZipSliderCtl::GetTTString()
{
	static char PosBuf[10];	// nice...
	itoa(GetPos(), PosBuf, 10);
	return PosBuf;
}

void CZipSliderCtl::UpdateTTText()
{
	ti.lpszText = GetTTString();
	::SendMessage(hwndTT, TTM_UPDATETIPTEXT, 0, (LPARAM)(LPTOOLINFO) &ti);
}

void CZipSliderCtl::UpdateTTPos()
{
	CRect TickRect;
	GetTickRect(TickRect);
	CPoint TTPos(TickRect.left, TickRect.bottom);
	ClientToScreen(&TTPos);
	::SendMessage(hwndTT, TTM_TRACKPOSITION, 0, (LPARAM)(DWORD) MAKELONG(TTPos.x, TTPos.y));
}
