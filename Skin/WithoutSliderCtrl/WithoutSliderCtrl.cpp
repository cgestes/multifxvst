/**
*/


#include "stdafx.h"

#include <math.h>

#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "WithoutSliderCtrl.h"
#include ".\withoutsliderctrl.h"


/////////////////////////////////////////////////////////////////////////////
// CWithoutSliderCtrl


CWithoutSliderCtrl::CWithoutSliderCtrl()
{
  pos = 0;
	m_bDragging = false;
}

CWithoutSliderCtrl::~CWithoutSliderCtrl()
{
}

BEGIN_MESSAGE_MAP(CWithoutSliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(CWithoutSliderCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CWithoutSliderCtrl::PreSubclassWindow() 
{
	CSliderCtrl::PreSubclassWindow();

	SetRange(0, 100, FALSE);
	SetLineSize(5);
	SetPageSize(10);

}

BOOL CWithoutSliderCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	if(!CSliderCtrl::PreCreateWindow(cs)) return FALSE;

	SetRange(0, 100, FALSE);
	SetLineSize(5);
	SetPageSize(10);

	return TRUE;
}

BOOL CWithoutSliderCtrl::OnEraseBkgnd(CDC* pDC) 
{
  return TRUE; // don't draw background
}

void CWithoutSliderCtrl::OnPaint() 
{
	CPaintDC memDC(this); // device context for painting
	//CMemDC memDC(&dc);
	int nMin = GetRangeMin();
	int nMax = GetRangeMax();
  int scalepos = ConvertNOTIN400(pos); 
  CString buf;


   /*for (int i = 0;i <128;i++)
  {
    bool b = ConvertNOTIN400(ConvertIN400(i)) == i;
    CString buf;
    buf.Format("%d i = %d\n",b,i);
    OutputDebugString(buf);
  }*/

  buf.Format("%d",scalepos);
  CRect rc;
  GetClientRect(&rc);
  CBrush  br(RGB(0,0,0));
  memDC.FillRect(&rc,&br);
  memDC.SetBkMode(TRANSPARENT   );
  memDC.SetTextColor(RGB(250,10,10));
  memDC.DrawText(buf,&rc,DT_RIGHT | DT_VCENTER);
  /*	CString str;
	str.Format("bob");
	::OutputDebugString(str);*/
}


bool CWithoutSliderCtrl::SetKnobPos(float nPos)
{
	float nOldPos = pos;

	int nMin = GetRangeMin();
	int nMax = GetRangeMax();
	
	if(nPos < 0)
		nPos =0;
	else if(nPos > 400)
		nPos = 400;

	pos = nPos;

  int scalepos = ConvertNOTIN400(pos); 
  int scaleposold = ConvertNOTIN400(nOldPos); 

  DWORD wval = MAKEWPARAM(TB_THUMBPOSITION,scalepos);
  //DWORD w2val = MAKEWPARAM(TB_THUMBTRACK,scalepos);
  //GetParent()->SendMessage(WM_HSCROLL,w2val,LPARAM(this->GetSafeHwnd()));
  GetParent()->SendMessage(WM_HSCROLL,wval,LPARAM(this->GetSafeHwnd()));

  //UpdateWindow();
	/*CString str;
	str.Format(" Pos=%d OldPos=%d\r\n", nPos, nOldPos );
	::OutputDebugString(str);*/
	return(scalepos != scaleposold);



}


bool CWithoutSliderCtrl::UpdateKnobPos(int nDelta)
{
  if(!nDelta)return false;

  int nMin = GetRangeMin();
	int nMax = GetRangeMax();

  //pos += delta;

	//int nPos = GetPos();
  //nPos = 400 * (nMax - nMin) / nPos;
  //nPos = 
  //nDelta = nDelta * 400 / (nMax - nMin)  ;
	//nPos+=nDelta;
	
	bool b = SetKnobPos(pos + nDelta);

	return(b);

}


// mouse
void CWithoutSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_ptHit = point;

	if(!m_bDragging)
	{
		m_bDragging = true;
		SetCapture();
		SetFocus();
		RedrawWindow();
	}
	else
	{
		CSliderCtrl::OnLButtonDown(nFlags, point);
	}
}


void CWithoutSliderCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		if(UpdateKnobPos(m_ptHit.y-point.y))
		{
			RedrawWindow();
		}
    m_ptHit = point;
	}
	else
	{
		CSliderCtrl::OnMouseMove(nFlags, point);
	}
}

void CWithoutSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bDragging)
	{
		m_bDragging = false;
		::ReleaseCapture();
		UpdateKnobPos(m_ptHit.y-point.y);
		RedrawWindow();
	}
	else
	{
		CSliderCtrl::OnLButtonUp(nFlags, point);
	}
}

// keyboard

void CWithoutSliderCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int nMin = GetRangeMin();
	int nMax = GetRangeMax();

	switch(nChar)
	{
	/*case VK_LEFT:
		{
			SetKnobPos(90);
			RedrawWindow();
		}
		break;*/
	case VK_UP:
		{
			UpdateKnobPos(+4);
			RedrawWindow();
		}
		break;
	
	/*case VK_RIGHT:
		{
			SetKnobPos(270);
			RedrawWindow();
		}
		break;*/
	case VK_DOWN:
		{
			UpdateKnobPos(-4);
			RedrawWindow();
		}
		break;
	/*case VK_SPACE:
		{
			SetKnobPos(180);
			RedrawWindow();
		}
		break;*/

	case VK_HOME:
		{
			SetKnobPos((float)nMin);
			RedrawWindow();
		}
			break;
	case VK_END:
		{
			SetKnobPos((float)nMax);
			RedrawWindow();
		}
		break;

	default:
		CSliderCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}




 


float CWithoutSliderCtrl::ConvertIN400(int valnot400)
{
    int nMin = GetRangeMin();
	  int nMax = GetRangeMax();
    if(!(nMin-nMax))
      return 0;
    else
      return ((float(valnot400) - float(nMin))*  400.5f / float((nMax -nMin)));
}

int CWithoutSliderCtrl::ConvertNOTIN400(float val400) 
{
    int nMin = GetRangeMin();
	  int nMax = GetRangeMax();
    return int((val400* float((nMax - nMin)) / 399.5f) + float(nMin));
}

BOOL CWithoutSliderCtrl::OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
  // TODO : ajoutez ici votre code spécialisé et/ou l'appel de la classe de base

  if(message == TBM_GETPOS )
  {
    int nMin = GetRangeMin();
	  int nMax = GetRangeMax();
    BOOL b = CSliderCtrl::OnWndMsg(message, wParam, lParam, pResult);
    if(!b)//on fait que si il a fait qqch (normalement tjs)
      int((*((int *)pResult))) = ConvertNOTIN400(pos);
    return 1;

  }else if(message == TBM_SETPOS)
  {
    int nMin = GetRangeMin();
	  int nMax = GetRangeMax();

    BOOL b = CSliderCtrl::OnWndMsg(message, wParam, lParam, pResult);
    if(!b)//on fait que si il a fait qqch (normalement tjs)
    {
      float npos = ConvertIN400(lParam);
      if(npos != pos)
      {
        pos = npos;
        RedrawWindow();
      }
    }
    return 1;
  }//else


  return CSliderCtrl::OnWndMsg(message, wParam, lParam, pResult);
}
