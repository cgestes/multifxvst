// ControleurDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "multifxVST.h"
#include "ControleurDlg.h"
#include ".\controleurdlg.h"
#include "controleurlst.h"

#include "multifxvst.h"
// Boîte de dialogue CControleurDlg

IMPLEMENT_DYNAMIC(CControleurDlg, CDialog)
CControleurDlg::CControleurDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CControleurDlg::IDD, pParent)
{
}

CControleurDlg::~CControleurDlg()
{

}

void CControleurDlg::Update()
{
   // APP->controleur->ViewControleur(m_lstcontroleur,nbsel);
}

void CControleurDlg::SetAPP(CAppPointer * m_APP)
{
  APP = m_APP;
}

void CControleurDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LSTCONTROLER, m_lstcontroleur);
}

BEGIN_MESSAGE_MAP(CControleurDlg, CDialog)
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// Gestionnaires de messages CControleurDlg
void CControleurDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
  APP->pControleur  = NULL;
	delete this;
  //CString buf;buf.Format("PostNcDestroy :: CControleurDlg(%d) \n", this);  TRACE(buf);
}

BOOL CControleurDlg::OnInitDialog()
{
  CDialog::OnInitDialog();
    //INITIALISE LA LISTE
  m_lstcontroleur.Init();
	/*m_lstcontroleur.g_MyClrFgHi = RGB(35,12,200);
	m_lstcontroleur.g_MyClrBgHi = RGB(20,242,0);
  m_lstcontroleur.g_MyClrBg   = RGB(130,120,240);*/
	/*m_listvst.SetBkColor(RGB(60,45,20));
	m_listvst.SetTextColor(RGB(52,242,22));*/

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 12;
  //lf.lfWidth  = 10;
	strcpy(lf.lfFaceName, "Verdana");
	font.CreateFontIndirect(&lf);
	m_lstcontroleur.SetFont(&font, TRUE);

  m_lstcontroleur.InsertColumn(0, "Midi Controleur", LVCFMT_LEFT, 60);
	m_lstcontroleur.InsertColumn(1, "Effect", LVCFMT_LEFT, 60);
  m_lstcontroleur.InsertColumn(2, "Parameters", LVCFMT_LEFT, 60);

  /*m_lstcontroleur.InsertColumn(6, "Min", LVCFMT_LEFT, 60);
	m_lstcontroleur.InsertColumn(7, "Max", LVCFMT_LEFT, 60);
	m_lstcontroleur.InsertColumn(9, "Controleur value", LVCFMT_LEFT, 60);*/



	ListView_SetExtendedListViewStyle(m_lstcontroleur.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

static CBrush brush(RGB(50,67,90));
static CBrush brush2(RGB(132,121,247));
HBRUSH CControleurDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetBkMode(TRANSPARENT);
  pDC->SetBkColor(RGB(132,121,247));
  if(pWnd == this || pWnd->GetDlgCtrlID() == IDC_STATIC || pWnd->GetDlgCtrlID() == IDC_CKSIMPLE)
  {
    return brush;
  }
	return brush2;
}


