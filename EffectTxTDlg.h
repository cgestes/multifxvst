/*****************************************************************************/
/* EffectTxTDlg.h : header file                                                */
/*****************************************************************************/

#if !defined(AFX_EffectTxTDlg_H__45274173_9C71_11D5_8162_4000001054B2__INCLUDED_)
#define AFX_EffectTxTDlg_H__45274173_9C71_11D5_8162_4000001054B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "EffectWnd.h"
#include "resource.h"
#include "skin\skinlistctrl.h"
#include "afxwin.h"
/*****************************************************************************/
/* CEffectTxTDlg dialog                                                        */
/*****************************************************************************/
class CCVSTHost;
class CAppPointer;
class CEffectStk;
class CEffectTxTDlg : public CDialog
{
  DECLARE_DYNAMIC(CEffectTxTDlg)
// Construction
public:
  CEffectTxTDlg(CWnd* pParent = NULL,CAppPointer * m_app = NULL);
  virtual ~CEffectTxTDlg(){CString buf;buf.Format("DESTROY :: CEffectTxTDlg(%d) \n", this);TRACE(buf);}
  virtual BOOL Create(CWnd *pParentWnd=NULL);

  bool OnSetParameterAutomated(long index, float value);
	int nEffect,nEffectStk;
  CSmpEffect * eff;
  CEffectStk * effstk;
// Dialog Data
	//{{AFX_DATA(CEffectTxTDlg)
	enum { IDD = IDD_PARMS };
	CSliderCtrl	slParmval;
	CStatic	stParmtext;
	//}}AFX_DATA

  //void SetMain(CChildFrame *pFrm) { pMain = pFrm; }
	void SetEffect(int nEffectStk);
  void Update(int nbsel = -1);

	protected:
  DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  virtual void PostNcDestroy();
	void SetParmDisp( int nParm);
  void OnSelchangeParmlist();
  CAppPointer    * APP;
public:
  afx_msg void OnClose();
  float GetParamVal( int nParm,CString & param);
  void  GetParamDisp( int nParm,CString & param);
  void  GetParamName( int nParm,CString & param);
  void  close();
  afx_msg void OnDestroy();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  CSkinListCtrl m_lstparms;
  ::CFont font;
  afx_msg void OnNMClickListparms(NMHDR *pNMHDR, LRESULT *pResult);
  CComboBox m_cbcontroleurnb;
  afx_msg void OnCbnDropdownCombo1();
  BOOL inited;
  int m_controleurnb;
  afx_msg void OnCbnSelchangeCombo1();
  afx_msg void OnCbnCloseupCombo1();
};

#endif // !defined(AFX_EffectTxTDlg_H__45274173_9C71_11D5_8162_4000001054B2__INCLUDED_)
