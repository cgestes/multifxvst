//------------------------------------------------------//
//- CopyLeft : CTAF
//-
//- Class CEffectWnd : affiche un plugins VST sous forme graphique
//- TODO :            
//-                                                   
//------------------------------------------------------//



/*****************************************************************************/
/* EffectWnd.h : header file                                                 */
/*****************************************************************************/

#if !defined(AFX_EFFECTWND_H__E21437F5_9BA1_11D5_8161_4000001054B2__INCLUDED_)
#define AFX_EFFECTWND_H__E21437F5_9BA1_11D5_8161_4000001054B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include "multifxVST.h"

/*****************************************************************************/
/* CEffectWnd window                                                         */
/*****************************************************************************/
struct ERect;
struct VstKeyCode;
class CRect;
class CCVSTHost;
class CAppPointer;
//class CChildFrame;
class CEffectWnd : public CWnd
{
	DECLARE_DYNCREATE(CEffectWnd)
// Construction
public:
	CEffectWnd(CAppPointer * m_app = NULL);

// Attributes
public:

// Operations
public:
    virtual void SetEffect(int nEff) { nEffect = nEff; }
    int  GetEffect() { return nEffect; }
    void SetupTitle();
    virtual LPCSTR GetEditType() { return "E"; }


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffectWnd)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
  int nEffect;
	virtual void Update();
    virtual bool OnSetParameterAutomated(long index, float value) { return false; }
	void SetEffSize(int cx, int cy);
	void SetEffSize(ERect *pRect);
	virtual ~CEffectWnd();
  void FuckEffect();//detach l'effet en cours
	// Generated message map functions
protected:
	void MakeVstKeyCode(UINT nChar, UINT nRepCnt, UINT nFlags, VstKeyCode &keyCode);
  BOOL GetEffectEditWndSize(::CRect &rcFrame, ::CRect &rcClient, ERect *pRect = NULL);
  virtual CWnd * CreateView() { return NULL; }
  virtual void PostNcDestroy();
  ::CRect rcEffFrame;
  ::CRect rcEffClient;
	//CWnd * pView;
	//CWnd * pMain;
	CAppPointer * APP;
	
	//{{AFX_MSG(CEffectWnd)
	afx_msg void OnClose();
	afx_msg void OnEffProgramName();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnEffEditParms();
	afx_msg void OnEffEdit();
	afx_msg void OnUpdateEffEdit(CCmdUI* pCmdUI);
	afx_msg void OnEffInfo();
	afx_msg void OnEffResize();
	afx_msg void OnEffChecksize();
	afx_msg void OnEffLoad();
	afx_msg void OnEffSave();
	afx_msg void OnEffSaveas();
	afx_msg void OnEffSelprogram();
	afx_msg void OnUpdateEffSelprogram(CCmdUI* pCmdUI);
	afx_msg void OnSelMidichn();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
    afx_msg void OnSetProgram(UINT nID);
	DECLARE_MESSAGE_MAP()
public:
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void EnterIdle();
  virtual BOOL DestroyWindow();
  afx_msg void OnDestroy();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFECTWND_H__E21437F5_9BA1_11D5_8161_4000001054B2__INCLUDED_)
