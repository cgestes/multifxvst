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
  , m_channel(0)
  , m_controleur(0)
  , m_action(0)
{
}

CControleurDlg::~CControleurDlg()
{
}

void CControleurDlg::SetAPP(CAppPointer * m_APP)
{
  APP = m_APP;
}
void CControleurDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Control(pDX, IDC_LSTCONTROLER, m_lstcontroleur);
  DDX_CBIndex(pDX, IDC_CBCHANNEL, m_channel);
  DDX_Text(pDX, IDC_TXTCONTROLEUR_NOTE, m_controleur);
  DDV_MinMaxLong(pDX, m_controleur, 0, 127);
  DDX_CBIndex(pDX, IDC_CBACTION, m_action);
}


BEGIN_MESSAGE_MAP(CControleurDlg, CDialog)
  ON_WM_CTLCOLOR()
  ON_BN_CLICKED(IDC_BTNVALIDATE, OnBnClickedBtnvalidate)
  ON_CBN_SELCHANGE(IDC_CBACTION, OnCbnSelchangeCbaction)
//  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LSTCONTROLER, OnLvnItemchangedLstcontroler)
ON_NOTIFY(NM_CLICK, IDC_LSTCONTROLER, OnNMClickLstcontroler)
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
	m_lstcontroleur.g_MyClrFgHi = RGB(35,12,200);
	m_lstcontroleur.g_MyClrBgHi = RGB(20,242,0);
  m_lstcontroleur.g_MyClrBg   = RGB(130,120,240);
	/*m_listvst.SetBkColor(RGB(60,45,20));
	m_listvst.SetTextColor(RGB(52,242,22));*/

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 12;
  //lf.lfWidth  = 10;
	strcpy(lf.lfFaceName, "Verdana");
	font.CreateFontIndirect(&lf);
	m_lstcontroleur.SetFont(&font, TRUE);

  m_lstcontroleur.InsertColumn(0, "#", LVCFMT_LEFT, 20);
	m_lstcontroleur.InsertColumn(1, "Midi channel", LVCFMT_LEFT, 58);
  m_lstcontroleur.InsertColumn(2, "Controleur/Note", LVCFMT_LEFT, 80);
	m_lstcontroleur.InsertColumn(3, "Action", LVCFMT_LEFT, 100);
	m_lstcontroleur.InsertColumn(4, "Value1", LVCFMT_LEFT, 60);
	m_lstcontroleur.InsertColumn(5, "Invert", LVCFMT_LEFT, 60);
  m_lstcontroleur.InsertColumn(6, "Min", LVCFMT_LEFT, 60);
	m_lstcontroleur.InsertColumn(7, "Max", LVCFMT_LEFT, 60);
	m_lstcontroleur.InsertColumn(9, "Controleur value", LVCFMT_LEFT, 60);


	ListView_SetExtendedListViewStyle(m_lstcontroleur.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
	

  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

static CBrush brush(RGB(230,220,12));
HBRUSH CControleurDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO :  Modifier ici les attributs du DC
	pDC->SetBkMode(TRANSPARENT);
  pDC->SetBkColor(RGB(230,220,12));
	return brush;

  // TODO :  Retourner une autre brosse si la brosse par défaut n'est pas souhaitée
  return hbr;
}

void CControleurDlg::OnBnClickedBtnvalidate()
{
  int n = m_lstcontroleur.GetCurSel();
  CControleurStk * param;
  if(n<0)
  {
    APP->controleur->ViewControleur(m_lstcontroleur);
    return ;
  }
  
  UpdateData();
   param = APP->controleur->Get(n);
   
   param->midi_channel = m_channel;
   param->midi_controleur = m_controleur;
   param->action = m_action ;

  APP->controleur->ViewControleur(m_lstcontroleur);
}

void CControleurDlg::OnCbnSelchangeCbaction()
{
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}


void CControleurDlg::OnNMClickLstcontroler(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
  int n = m_lstcontroleur.GetCurSel();
  CControleurStk * param;
  if(n<0)return ;
  
  UpdateData();
   param = APP->controleur->Get(n);
   m_channel = param->midi_channel;
   m_controleur = param->midi_controleur;
   m_action     = param->action;
  UpdateData(FALSE);
  *pResult = 0;
}
