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
/*****************************************************************************/
/* CEffectTxTDlg dialog                                                        */
/*****************************************************************************/
class CCVSTHost;
class CAppPointer;
class CEffectTxTDlg : public CDialog
{
  DECLARE_DYNAMIC(CEffectTxTDlg)
// Construction
public:
  CEffectTxTDlg(CWnd* pParent = NULL,CAppPointer * m_app = NULL);
  virtual ~CEffectTxTDlg(){CString buf;buf.Format("DESTROY :: CEffectTxTDlg(%d) \n", this);TRACE(buf);}
  virtual BOOL Create(CWnd *pParentWnd=NULL);

  bool OnSetParameterAutomated(long index, float value);
	int nEffect;

// Dialog Data
	//{{AFX_DATA(CEffectTxTDlg)
	enum { IDD = IDD_PARMS };
	CSliderCtrl	slParmval;
	CStatic	stParmtext;
	//}}AFX_DATA

  //void SetMain(CChildFrame *pFrm) { pMain = pFrm; }
	void SetEffect(int nEffect);
  void Update();

	protected:
  DECLARE_MESSAGE_MAP()
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  virtual void PostNcDestroy();
	void SetParmDisp(CEffect *pEffect, int nParm);
  void OnSelchangeParmlist();
  CAppPointer    * APP;
public:
  afx_msg void OnClose();
  float GetParamVal(CEffect *pEffect, int nParm,CString & param);
  void  GetParamDisp(CEffect *pEffect, int nParm,CString & param);
  void  GetParamName(CEffect *pEffect, int nParm,CString & param);
  void  close();
  afx_msg void OnDestroy();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  CSkinListCtrl m_lstparms;
  ::CFont font;
  afx_msg void OnLvnItemchangedListparms(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMClickListparms(NMHDR *pNMHDR, LRESULT *pResult);
};

#endif // !defined(AFX_EffectTxTDlg_H__45274173_9C71_11D5_8162_4000001054B2__INCLUDED_)
