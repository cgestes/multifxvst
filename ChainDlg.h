//------------------------------------------------------//
//- CopyLeft : CTAF
//-
//- Fenetre principal gérant les chaines de plug
//- TODO :  ****          
//-                                                   
//------------------------------------------------------//

#ifndef __Gratdlg__
#define __Gratdlg__


#pragma once

#include "resource.h"
#include "skin\hoverbutton.h"
#include "skin\skinlistctrl.h"
#include "skinslider/ZipSliderCtl.h"
#include "skin\hoverbutton.h"

// Boîte de dialogue CChainDlg
class CEffectWnd;
class CEffectTxTDlg;
class CStockEffetLst;
class CCVSTHost;
class multifxVST;
class CHoverButton;
class CMainDlg;
class CAppPointer;


class CChainDlg : public CDialog
{
	DECLARE_DYNAMIC(CChainDlg)

public:
  //## CONSTRUCTEUR ET INITIALISATION
	CChainDlg(CWnd* pParent = NULL);   // constructeur standard
  void SetAPP(CAppPointer * m_APP = NULL);
  virtual BOOL OnInitDialog();
  void InitialiseSkin();


  //MISE A JOUR DE CURRENT_CHAINE (async)
  void OnUpdate(int nb = 0);



  //fermeture des fenetre
  //void close();



  void LoadEffect(LPCSTR dllpath);
	virtual ~CChainDlg();




  // LOAD/SAVE
  BOOL SaveAll(LPCSTR Path);
  BOOL LoadAll(LPCSTR Path);
  BOOL SaveChaine(int chaine,LPCSTR Path);
  BOOL LoadChaine(int chaine,LPCSTR Path);
  BOOL SaveParamEffect(int chaine,int nbeffect,LPCSTR Path);
  BOOL LoadParamEffect(int chaine,int nbeffect,LPCSTR Path);

  // Données de boîte de dialogue
	enum { IDD = IDD_Chain };

public:



  //## MESSAGE MAP
  afx_msg void OnBnClickedBtnup();
  afx_msg void OnBnClickedBtndown();
  afx_msg void OnBnClickedBtnsave();
  afx_msg void OnBnClickedBtnload();
  afx_msg void OnBnClickedBtnsuppr();
  afx_msg void OnBnClickedBtnadd();
  afx_msg void OnBnClickedBtnsave2();
  afx_msg void OnBnClickedBtnload2();
  afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);



  //OBJET SKIN
  CHoverButton m_btnUP;
  CHoverButton m_btnDown;
  CHoverButton m_btnAdd;
  CHoverButton m_btnSuppr;
  CHoverButton m_btnSaveAll;
  CHoverButton m_btnLoadAll;
  CHoverButton m_btnSavechaine;
  CHoverButton m_btnLoadchaine;
  CSkinListCtrl m_listvst;
  //CSliderCtrl m_sld;//pas encore skiné :-(
  ::CFont font;


  //OBJET
  //int           m_slide;

  //CString       m_txtchaine;
  CAppPointer   * APP;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge DDX/DDV
	virtual void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
  ::CBitmap mybitmap;//fond
  BOOL LoadBitmap(UINT bitmapid);
  //int current_chaine;

public:
//  afx_msg void OnLvnOdstatechangedListvst(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnLvnItemchangedListvst(NMHDR *pNMHDR, LRESULT *pResult);
  afx_msg void OnNMDblclkListvst(NMHDR *pNMHDR, LRESULT *pResult);
//  afx_msg void OnBnClickedBtnsuppr4();
  afx_msg void OnBnClickedBtnclrchain();
  afx_msg void OnBnClickedBtncopieto();
  afx_msg void OnBnClickedBtnpasteto();
  int m_copied; //chaine copié
  CHoverButton m_btnclear;
  CHoverButton m_btncopy;
  CHoverButton m_btnpaste;
  int m_used;
//  afx_msg void OnStnClickedTxtinfoctaf();
  afx_msg void OnEffectsBrowse();
  afx_msg void OnEffectsShellplug();

  afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#endif