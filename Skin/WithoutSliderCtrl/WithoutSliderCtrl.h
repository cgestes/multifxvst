/*
CWithoutSliderCtrl
range : 0 => 400 (car le plus adapté a la souris)
faudrait faire un produit en croix mais ....
*/

#ifndef _CWithoutSliderCtrl_
#define _CWithoutSliderCtrl_

class CWithoutSliderCtrl : public CSliderCtrl
{

public:
	
	CWithoutSliderCtrl();
	virtual ~CWithoutSliderCtrl();

	//{{AFX_VIRTUAL(CWithoutSliderCtrl)
	protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

protected:
	virtual bool SetKnobPos(float nPos);
	virtual bool UpdateKnobPos(int nDeltaPos);

	//{{AFX_MSG(CWithoutSliderCtrl)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
  afx_msg void OnLButtonDown(UINT nFlags, ::CPoint point);
  afx_msg void OnMouseMove(UINT nFlags, ::CPoint point);
  afx_msg void OnLButtonUp(UINT nFlags, ::CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

protected:
  float ConvertIN400(int valnot400);
  int ConvertNOTIN400(float val400);
	bool	m_bDragging;
  float pos;
	POINT		m_ptHit;
  virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};


#endif


