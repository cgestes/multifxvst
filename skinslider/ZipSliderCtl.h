#if !defined(AFX_MYSLIDERCTL_H__8B85C506_15A8_4386_B319_2D678DB72506__INCLUDED_)
#define AFX_MYSLIDERCTL_H__8B85C506_15A8_4386_B319_2D678DB72506__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MySliderCtl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CZipSliderCtl window
#include "ZipBitmap.h"


class CZipSliderCtl : public CSliderCtrl
{
// Construction
public:
	CZipSliderCtl();

	HDC GetBackgroundDC() {return m_bmpBack->GetDC();}
  ::CRect& GetClientArea() {return CArea;}
	bool IsDragging() {return m_bDragging;}

// Attributes
private:
	CZipTransBitmap *m_bmTrans;
	CZipTransBitmap *m_bmTransDrag;
	CZipBackGroundBitmap *m_bmpBack;
	HCURSOR m_hCursor;

	bool m_ButDown;
	bool m_bDragging;

	bool m_Vert;

	bool m_bDisplayTT;
	bool m_bTrackTT;
	static HWND hwndTT;
	static TOOLINFO ti;

  ::CRect CArea;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CZipSliderCtl)
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetSkin(UINT uiBackGround, UINT uiTickImage, UINT uiTickDragImage = 0, int Options = 0);
	void SetCursor(short nCursor);
	void SetupTT(int Options);

	virtual ~CZipSliderCtl();

	void SetPos(int nPos);

	// Generated message map functions
protected:
  void GetTickRect(::CRect& rcTick);
	void CreateTT();
	void UpdateTTText();
	void UpdateTTPos();
	virtual char* GetTTString();

	//{{AFX_MSG(CZipSliderCtl)
  afx_msg void OnMouseMove(UINT nFlags, ::CPoint point);
	afx_msg void OnPaint();
  afx_msg void OnLButtonUp(UINT nFlags, ::CPoint point);
  afx_msg void OnLButtonDown(UINT nFlags, ::CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYSLIDERCTL_H__8B85C506_15A8_4386_B319_2D678DB72506__INCLUDED_)
