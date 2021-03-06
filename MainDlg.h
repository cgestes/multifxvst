#pragma once
#include "resource.h"
#include "skin\hoverbutton.h"
#include "skin\WithoutSliderCtrl\WithoutSliderCtrl.h"


// Bo�te de dialogue CMainDlg
class CAppPointer;

enum {WD_CHAIN = 0,WD_CONTROLER,WD_EFFET,WD_EFFET2,WD_ABOUT};

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)
public:
	CMainDlg(CWnd* pParent = NULL);   // constructeur standard
  virtual ~CMainDlg(){};
// Donn�es de bo�te de dialogue
	enum { IDD = IDD_MAIN };

  void ChildNotify(CWnd * child,int sizex = -1,int sizey = -1);
  void SetAPP(CAppPointer * m_APP){APP = m_APP;};

  void KillAll();   //d�truit toutes les fenetres
  void CloseAll();  //ferme toutes les fenetres

  //0 = tt  //1 = eff  //2 = txt
  void KillEffect(int nb = 0);//d�truit les fenetres d'effet
  void SetEffect(int nbeffstk);
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
  CMenu  mnu1;

public:
  CWnd              * pActiv;//Attention ne sert que pour le size et n'est pas forcement exact

  virtual BOOL OnInitDialog();
  afx_msg void OnBnClickedButton1();
  afx_msg void OnBnClickedButton2();
  afx_msg void OnBnClickedButton3();
protected:
  virtual void PostNcDestroy();

  //initialise les skins
  void InitialiseSkin();
  //retourne le numero de la fenetre active (WD_CHAIN,WD_...)
  int GetActiveWindows();
public:
  void SaveBank(CString sName);
  void SavePreset(CString sName);
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
  void UpdateBouton(int btn);
  virtual BOOL DestroyWindow();
  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
  afx_msg void OnBnClickedBtneffup();
  afx_msg void OnBnClickedBtneffdown();
  afx_msg void OnBnClickedButton5();
  afx_msg void OnFxbSavebank();
  afx_msg void OnFxbLoadfxp();
  afx_msg void OnFxbSavefxp();
  afx_msg void OnFxbLoadbank();
  afx_msg void OnSize(UINT nType, int cx, int cy);
  afx_msg void OnBnDoubleclickedButton3();
  afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
  afx_msg void OnBnClickedBtnchmoins();
  afx_msg void OnBnClickedBtnchplus();
  afx_msg void OnBnDoubleclickedBtnchplus();
  afx_msg void OnBnDoubleclickedBtnchmoins();
  afx_msg void OnBnDoubleclickedBtneffup();
  afx_msg void OnBnDoubleclickedBtneffdown();
  afx_msg void OnBnHotItemChangeButton3(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnBnClickedBtneffecttxt();
  afx_msg void OnBnClickedBtnbypass();

  CHoverButton m_btnchplus;
  CHoverButton m_btnchmoins;
  CHoverButton m_btnpreset;
  CHoverButton m_btnbypass;
  CHoverButton m_btneffect2;
  CHoverButton m_btnabout;
  CHoverButton m_btnchaine;
  CHoverButton m_btncontroleur;
  CHoverButton m_btneff;
  CHoverButton m_btneffup;
  CHoverButton m_btneffdown;

  CString m_txteff;
  int nbeffstk;
  int nbeff;

  CWithoutSliderCtrl m_sld;
  int           m_sldchaine;

protected :
  ::CBitmap mybitmap;//fond
  BOOL LoadBitmap(UINT bitmapid);
public:

  afx_msg void OnBnClickedBtnabout();
};
