#pragma once

#include "resource.h"
#include "skin\skinlistctrl.h"


// Boîte de dialogue CControleurDlg
class CAppPointer;
class CControleurDlg : public CDialog
{
	DECLARE_DYNAMIC(CControleurDlg)
  void SetAPP(CAppPointer * m_APP);
public:
	CControleurDlg(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CControleurDlg();

// Données de boîte de dialogue
	enum { IDD = IDD_CONTROLER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
  virtual void PostNcDestroy();
  CAppPointer * APP;
public:
  ::CFont font;
  virtual BOOL OnInitDialog();
  CSkinListCtrl m_lstcontroleur;
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  afx_msg void OnBnClickedBtnvalidate();
  afx_msg void OnCbnSelchangeCbaction();
//  afx_msg void OnLvnItemchangedLstcontroler(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMClickLstcontroler(NMHDR *pNMHDR, LRESULT *pResult);
  int m_channel;
  long m_controleur;
  int m_action;
};
