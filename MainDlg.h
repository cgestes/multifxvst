#pragma once
#include "resource.h"
#include "skin\hoverbutton.h"
#include "skin\hoverbutton.h"

// Boîte de dialogue CMainDlg
class CAppPointer;

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)
public:
	CMainDlg(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CMainDlg();
// Données de boîte de dialogue
	enum { IDD = IDD_MAIN };

  void ChildNotify(CWnd * child,bool bAlwaisResize = false);
  void SetAPP(CAppPointer * m_APP){APP = m_APP;};

  void KillAll();   //détruit toutes les fenetres
  void CloseAll();  //ferme toutes les fenetres

  //0 = tt  //1 = eff  //2 = txt
  void KillEffect(int nb = 0);//détruit les fenetres d'effet
  void SetEffect(int nb);
  void Init();
  void OpenEffect(int chaine,int nbeffectstk);
  void OpenEffectTxT(int chaine,int nbeffectstk);
  void EnterIdle();//dispatch l'idle a la fentre fille plugins
  void OnUpdate();
  void ChangeChaine(int chaine,BOOL paramAutom = TRUE);
  void FillPopup(CMenu* pPopupMenu, int nEffect);
  void OnSetProgram(UINT nID);
  afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	DECLARE_MESSAGE_MAP()

  //pour connaitre sa taille actuel et d'origine
  ::CRect   WinRect,WinRectInit;

  CAppPointer       * APP;
  CWnd              * pActiv;//Attention ne sert que pour le size et n'est pas forcement exact
  CMenu  mnu1;

public:
  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  afx_msg void OnBnClickedButton3();
protected:
  virtual void PostNcDestroy();
public:
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  virtual BOOL DestroyWindow();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnBnClickedBtneffup();
  afx_msg void OnBnClickedBtneffdown();
  CHoverButton m_btnchaine;
  CHoverButton m_btncontroleur;
  CHoverButton m_btneff;
  CHoverButton m_btneffup;
  CHoverButton m_btneffdown;
  CString m_txteff;
  int nbeff;
  int nbef;
  afx_msg void OnBnClickedButton5();
  afx_msg void OnFxbSavebank();
  afx_msg void OnFxbLoadfxp();
  afx_msg void OnFxbSavefxp();
  afx_msg void OnFxbLoadbank();
  void SaveBank(CString sName);
  void SavePreset(CString sName);
  afx_msg void OnSize(UINT nType, int cx, int cy);
//  afx_msg void OnNMReleasedcaptureSldchaine(NMHDR *pNMHDR, LRESULT *pResult);
  CString       m_txtchaine;
  CSliderCtrl m_sld;//pas encore skiné :-(
  int           m_sldchaine;
  afx_msg void OnBnDoubleclickedButton3();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//  afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnBnClickedBtnchmoins();
  afx_msg void OnBnClickedBtnchplus();
  CHoverButton m_btnchplus;
  CHoverButton m_btnchmoins;
  CHoverButton m_btnpreset;
  CHoverButton m_btnbypass;
  afx_msg void OnBnDoubleclickedBtnchplus();
  afx_msg void OnBnDoubleclickedBtnchmoins();
  afx_msg void OnBnDoubleclickedBtneffup();
  afx_msg void OnBnDoubleclickedBtneffdown();
  afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
  afx_msg void OnBnDoubleclickedButton2();
};
