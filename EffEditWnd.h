/*****************************************************************************/
/* EffEditWnd.h : header file                                                */
/*****************************************************************************/

#if !defined(AFX_EFFEDITWND_H__45274173_9C71_11D5_8162_4000001054B2__INCLUDED_)
#define AFX_EFFEDITWND_H__45274173_9C71_11D5_8162_4000001054B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "EffectWnd.h"
#include "resource.h"
/*****************************************************************************/
/* CEffEditDlg dialog                                                        */
/*****************************************************************************/

/*class CChildFrame;
class CEffEditWnd;*/
class CCVSTHost;
class CAppPointer;
class CEffEditDlg : public CDialog
{
  DECLARE_DYNAMIC(CEffEditDlg)
friend class CEffEditWnd;
// Construction
public:
  virtual ~CEffEditDlg(){CString buf;buf.Format("DESTROY :: CEffEditDlg(%d) \n", this);  TRACE(buf);}
	bool OnSetParameterAutomated(long index, float value);
	CEffEditDlg(CWnd* pParent = NULL,CAppPointer * m_app = NULL);   // standard constructor
	virtual BOOL Create(CWnd *pParentWnd=NULL);
	int nEffect;

// Dialog Data
	//{{AFX_DATA(CEffEditDlg)
	enum { IDD = IDD_PARMS };
	CSliderCtrl	slParmval;
	CStatic	stParmtext;
	CListBox	lbParms;
	//}}AFX_DATA

  //void SetMain(CChildFrame *pFrm) { pMain = pFrm; }
	void SetEffect(int nEffect);
  void Update();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffEditDlg)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetParmDisp(CEffect *pEffect, int nParm);
//	CChildFrame * pMain;

  CAppPointer    * APP;
	// Generated message map functions
	//{{AFX_MSG(CEffEditDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeParmlist();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
  virtual void PostNcDestroy();
public:
  afx_msg void OnClose();
  float GetParamVal(CEffect *pEffect, int nParm,CString & param);
  void  GetParamDisp(CEffect *pEffect, int nParm,CString & param);
  void  GetParamName(CEffect *pEffect, int nParm,CString & param);
  void  close();
  afx_msg void OnDestroy();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
/*****************************************************************************/
/* CEffEditWnd window                                                        */
/*****************************************************************************/
/*
class CChildFrame;
class CEffEditWnd : public CEffectWnd
{
	DECLARE_DYNCREATE(CEffEditWnd)
// Construction
public:
	CEffEditWnd();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEffEditWnd)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void Update();
	virtual bool OnSetParameterAutomated(long index, float value);
	virtual void SetEffect(int nEff);
	virtual ~CEffEditWnd();

	// Generated message map functions
protected:
	virtual CWnd * CreateView();
//    virtual HICON RetrieveIcon() { return AfxGetApp()->LoadIcon(IDI_EFFPARM); }
    virtual LPCSTR GetEditType() { return "P"; }
    virtual BOOL NeedView() { return TRUE; }
	//{{AFX_MSG(CEffEditWnd)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
*/
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EFFEDITWND_H__45274173_9C71_11D5_8162_4000001054B2__INCLUDED_)
