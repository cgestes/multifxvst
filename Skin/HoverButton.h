#if !defined(AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_)
#define AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HoverButton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CHoverButton by Niek Albers
// Thanks to some people for the tooltip.
// A cool CBitmapButton derived class with 3 states,
// Up/Down/Hover.
class CHoverButton : public CBitmapButton
{
	DECLARE_DYNAMIC(CHoverButton);

	// Construction
public:
	CHoverButton();
	void SetToolTipText(CString* spText, BOOL bActivate = TRUE);
	void SetToolTipText(int nId, BOOL bActivate = TRUE);
  void SetValue(bool val){m_value = val;RedrawWindow();}
  bool GetValue(){return m_value ;}
// Attributes
protected:
	void ActivateTooltip(BOOL bActivate = TRUE);
	BOOL m_bHover;						// indicates if mouse is over the button
	CSize m_ButtonSize;					// width and height of the button
	::CBitmap mybitmap;
	BOOL m_bTracking;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHoverButton)
	protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL LoadBitmap(UINT bitmapid,bool OnOff = false,int x = 0 ,int y = 0);
  void SetBitmapDisp(int m_hoverOn,int m_hoverOff,int m_clickOn,int m_clickOff,int m_normalOn,int m_normalOff);
	virtual ~CHoverButton();

	// Generated message map functions
protected:
	CToolTipCtrl m_ToolTip;
  bool OnOff;
  bool m_value;
	void InitToolTip();
	//{{AFX_MSG(CHoverButton)
	afx_msg void OnMouseMove(UINT nFlags, ::CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	//}}AFX_MSG

  int m_hoverOn,m_hoverOff,m_clickOn,m_clickOff,m_normalOn,m_normalOff;
	DECLARE_MESSAGE_MAP()
public:
//  afx_msg void OnLButtonDown(UINT nFlags, ::CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, ::CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HOVERBUTTON_H__16C6D980_BD45_11D3_BDA3_00104B133581__INCLUDED_)
