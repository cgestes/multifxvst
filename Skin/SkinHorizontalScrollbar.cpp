// SkinHorizontalScrollbar.cpp : implementation file
//

#include "stdafx.h"
#include "SkinHorizontalScrollbar.h"
#include "../resource.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkinHorizontalScrollbar
#define T_ARROW    19
#define T_CARREBAS 11
#define T_CURSOR   7
#define LARGEUR    9

CSkinHorizontalScrollbar::CSkinHorizontalScrollbar()
{
	nThumbLeft = T_ARROW-1;
	dbThumbRemainder = 0.00f;

	bMouseDown = false;
	bMouseDownArrowLeft = false;
	bMouseDownArrowRight = false;
	bDragging = false;
	pList = NULL;
}

CSkinHorizontalScrollbar::~CSkinHorizontalScrollbar()
{
}


BEGIN_MESSAGE_MAP(CSkinHorizontalScrollbar, CStatic)
	//{{AFX_MSG_MAP(CSkinHorizontalScrollbar)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkinHorizontalScrollbar message handlers

void CSkinHorizontalScrollbar::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	Draw();
}

void CSkinHorizontalScrollbar::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetCapture();
	CRect clientRect;
	GetClientRect(&clientRect);
	
	int nWidth = clientRect.Width()-T_ARROW;

	CRect rectLeftArrow(0,0,T_ARROW,LARGEUR);
	CRect rectRightArrow(nWidth,0,nWidth+T_ARROW,LARGEUR);
	CRect rectThumb(nThumbLeft,0,nThumbLeft+T_CURSOR,LARGEUR);
	
	if(rectThumb.PtInRect(point))
	{
		bMouseDown = true;
	}


	if(rectRightArrow.PtInRect(point))
	{
		bMouseDownArrowRight = true;
		SetTimer(2,250,NULL);
	}

	if(rectLeftArrow.PtInRect(point))
	{
		bMouseDownArrowLeft = true;
		SetTimer(2,250,NULL);
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CSkinHorizontalScrollbar::OnLButtonUp(UINT nFlags, CPoint point) 
{
	UpdateThumbPosition();
	KillTimer(1);
	ReleaseCapture();
	

	bool bInChannel = true;
	
	CRect clientRect;
	GetClientRect(&clientRect);
	
	int nWidth = clientRect.Width()-T_ARROW;

	CRect rectLeftArrow(0,0,T_ARROW,LARGEUR);
	CRect rectRightArrow(nWidth,0,nWidth+T_ARROW,LARGEUR);
	CRect rectThumb(nThumbLeft,0,nThumbLeft+T_CURSOR,LARGEUR);

	if(rectLeftArrow.PtInRect(point))
	{
		ScrollLeft();	
		bInChannel = false;
	}



	
	if(rectRightArrow.PtInRect(point))
	{
		ScrollRight();	
		bInChannel = false;
	}

	if(rectThumb.PtInRect(point))
	{
		bInChannel = false;
	}

	if(bInChannel == true && !bMouseDown)
	{
		if(point.x > nThumbLeft)
		{
			PageRight();
		}
		else
		{
			PageLeft();
		}
	}

	//reset all variables
	bMouseDown = false;
	bDragging = false;
	bMouseDownArrowLeft = false;
	bMouseDownArrowRight = false;
	CStatic::OnLButtonUp(nFlags, point);
}

void CSkinHorizontalScrollbar::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect clientRect;
	GetClientRect(&clientRect);

	if(bMouseDown)
		bDragging = true;

	if(bDragging)
	{	
		nThumbLeft = point.x-(T_CURSOR >> 1); //-13 so mouse is in middle of thumb

		double nMax = pList->GetScrollLimit(SB_HORZ);
		int nPos = pList->GetScrollPos(SB_HORZ);

		double nWidth = clientRect.Width()-(((T_ARROW - 1)<<1)+(T_CURSOR -1));
		double nVar = nMax;
		dbThumbInterval = nWidth/nVar;

		//figure out how many times to scroll total from top
		//then minus the current position from it
		int nScrollTimes = (int)((nThumbLeft-(T_ARROW - 1))/dbThumbInterval)-nPos;
		
		CSize size;
		size.cx = nScrollTimes;
		size.cy = 0;
		
		pList->Scroll(size);
		
		LimitThumbPosition();
		
		Draw();
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CSkinHorizontalScrollbar::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == 1)
	{
		if(bMouseDownArrowRight)
		{
			ScrollRight();
		}
		
		if(bMouseDownArrowLeft)
		{
			ScrollLeft();
		}
	}
	else if(nIDEvent == 2)
	{
		if(bMouseDownArrowRight)
		{
			KillTimer(2);
			SetTimer(1, 50, NULL);
		}
		
		if(bMouseDownArrowLeft)
		{
			KillTimer(2);
			SetTimer(1, 50, NULL);
		}
	}
	CStatic::OnTimer(nIDEvent);
}

void CSkinHorizontalScrollbar::ScrollLeft()
{
	pList->SendMessage(WM_HSCROLL, MAKELONG(SB_LINELEFT,0),NULL);
	UpdateThumbPosition();
}

void CSkinHorizontalScrollbar::ScrollRight()
{
	pList->SendMessage(WM_HSCROLL, MAKELONG(SB_LINERIGHT,0),NULL);
	UpdateThumbPosition();
}

void CSkinHorizontalScrollbar::UpdateThumbPosition()
{
	CRect clientRect;
	GetClientRect(&clientRect);

	double nPos = pList->GetScrollPos(SB_HORZ);
	double nMax = pList->GetScrollLimit(SB_HORZ);
	double nWidth = clientRect.Width()-(((T_ARROW - 1)<<1)+(T_CURSOR -1)); 
	double nVar = nMax;

	dbThumbInterval = nWidth/nVar;

	double nNewdbValue = dbThumbInterval * (nPos);
	int nNewValue = (int)nNewdbValue;
	double nExtra = nNewdbValue - nNewValue;
	dbThumbRemainder = nExtra;
	
	nThumbLeft = (T_ARROW - 1)+nNewValue;

	LimitThumbPosition();
	
	Draw();
}

void CSkinHorizontalScrollbar::PageRight()
{
	pList->SendMessage(WM_HSCROLL, MAKELONG(SB_PAGEDOWN,0),NULL);
	UpdateThumbPosition();
}

void CSkinHorizontalScrollbar::PageLeft()
{
	pList->SendMessage(WM_HSCROLL, MAKELONG(SB_PAGEUP,0),NULL);
	UpdateThumbPosition();
}

void CSkinHorizontalScrollbar::Draw()
{
	CClientDC dc(this);
	CRect clientRect;
	GetClientRect(&clientRect);
	CMemDC memDC(&dc, &clientRect);
	memDC.FillSolidRect(&clientRect,  RGB(76,85,118));

	CDC bitmapDC;
	bitmapDC.CreateCompatibleDC(&dc);

	//draw left arrow of scrollbar
	CBitmap bitmap;
	bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_LEFTARROW);
	CBitmap* pOldBitmap = bitmapDC.SelectObject(&bitmap);
	memDC.BitBlt(clientRect.left,clientRect.top,T_ARROW,LARGEUR,&bitmapDC,0,0,SRCCOPY);
	bitmapDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	pOldBitmap = NULL;

	
	bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_SPAN);

	pOldBitmap = bitmapDC.SelectObject(&bitmap);
	

	int nWidth = clientRect.Width() - T_ARROW ;

	memDC.StretchBlt(clientRect.left+T_ARROW, clientRect.top, nWidth - T_ARROW,LARGEUR,&bitmapDC, 0,0, 1, LARGEUR, SRCCOPY);

	bitmapDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	pOldBitmap = NULL;
	
	bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_RIGHTARROW);

	pOldBitmap = bitmapDC.SelectObject(&bitmap);
	memDC.BitBlt(nWidth,clientRect.top,T_ARROW,LARGEUR,&bitmapDC,0,0,SRCCOPY);

	bitmapDC.SelectObject(pOldBitmap);
	bitmap.DeleteObject();
	pOldBitmap = NULL;

	//If there is nothing to scroll then don't
	//show the thumb control otherwise show it
	if(pList->GetScrollLimit(SB_HORZ) != 0)
	{
		bitmap.LoadBitmap(IDB_HORIZONTAL_SCROLLBAR_THUMB);
		
		pOldBitmap = bitmapDC.SelectObject(&bitmap);
		memDC.BitBlt(clientRect.left+nThumbLeft,clientRect.top,T_CURSOR,LARGEUR,&bitmapDC,0,0,SRCCOPY);
		
		bitmapDC.SelectObject(pOldBitmap);
		bitmap.DeleteObject();
		pOldBitmap = NULL;
	}
}

void CSkinHorizontalScrollbar::LimitThumbPosition()
{
	CRect clientRect;
	GetClientRect(&clientRect);

	if(nThumbLeft+T_CURSOR > (clientRect.Width()-T_ARROW))
	{
		nThumbLeft = clientRect.Width()-T_ARROW-T_CURSOR+1;
	}

	if(nThumbLeft < (clientRect.left+T_ARROW - 1 ))
	{
		nThumbLeft = clientRect.left+T_ARROW - 1;
	}
}
