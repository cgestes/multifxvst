#pragma once

#include "resource.h"
// Boîte de dialogue CAboutDlg

class CAboutDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutDlg)

public:
	CAboutDlg(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CAboutDlg();
  void SetAPP(CAppPointer * theAPP){APP = theAPP;}

// Données de boîte de dialogue
	enum { IDD = IDD_ABOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//  afx_msg void OnNcDestroy();
protected:
  CAppPointer * APP;
  virtual void PostNcDestroy();
public:
  afx_msg void OnStnClickedStatic1();
  afx_msg void OnStnClickedStatic2();
};
