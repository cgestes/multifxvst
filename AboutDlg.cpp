// AboutDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "multifxVST.h"
#include "AboutDlg.h"
#include ".\aboutdlg.h"


// Boîte de dialogue CAboutDlg

IMPLEMENT_DYNAMIC(CAboutDlg, CDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
  ON_WM_CTLCOLOR()
//  ON_WM_NCDESTROY()
ON_STN_CLICKED(IDC_STATIC1, OnStnClickedStatic1)
ON_STN_CLICKED(IDC_STATIC2, OnStnClickedStatic2)
END_MESSAGE_MAP()


// Gestionnaires de messages CAboutDlg

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  static CBrush brush(RGB(216,135,57));
  pDC->SetBkMode(TRANSPARENT);
  return brush;
}

//void CAboutDlg::OnNcDestroy()
//{
//  CDialog::OnNcDestroy();
//
//  // TODO : ajoutez ici le code de votre gestionnaire de messages
//}

void CAboutDlg::PostNcDestroy()
{
  // TODO : ajoutez ici votre code spécialisé et/ou l'appel de la classe de base
	CDialog::PostNcDestroy();
  APP->pAboutDlg  = NULL;
	delete this;

}

void CAboutDlg::OnStnClickedStatic1()
{
  ShellExecute(NULL,"open","http://www.ctaf.free.fr","","",SW_NORMAL);
}

void CAboutDlg::OnStnClickedStatic2()
{
  ShellExecute(NULL,"open","http://www.ctaf.free.fr","","",SW_NORMAL);
}
