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
  , m_action(0)
  , nbsel(0)
  , m_txtdesc1(_T(""))
  , m_txtdesc2(_T(""))
  , m_cksimple(FALSE)
  , m_txtdesc3(_T(""))
  , m_controleur()
  , m_value1(0)
  , inited(0)
  , m_controleurnb(0)
{
}


static const char * const tabdesc[7][5] = 
//controleur SET
{{"Midi Controler","static value:","value = <controler_value>","value = <static value>","Use static value"},
{"Midi Controler" ,"increment:"   ,"value = value + <increment>\n","value = value - <increment>","Decrement"},
{"Midi Controler" ,"limit:"       ,"toggle value between 0 and 127","controler_value  < limit =>    value = 0\ncontroler_value >= limit =>    value = 127","Use limit"},
{"Midi Note"      ,"static value:","value = note_velocity","value = <static value>","Use static value"},
{"Midi Note"      ,"increment:"   ,"value = value + <increment>\n","value = value - <increment>","Decrement"},
 {"Midi Note"      ,""            ,"toggle value between 0 and 127","NoteOn => value = 127\nNoteOff => value = 0","Use NoteOn/NoteOff"},
{""          ,""      ,"value = <program_change_value>","value =127 - <program_change_value>","Invert"}};

static int  const tabvisible[7][2] =
{{0x1001,0x1011},
{0x1111,0x1111},
{0x1001,0x1011},
{0x1001,0x1011},
{0x1111,0x1111},
{0x1001,0x1001},
{0x0001,0x0001}};

CControleurDlg::~CControleurDlg()
{

}

void CControleurDlg::Update()
{
    APP->controleur->ViewControleur(m_lstcontroleur,nbsel);
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
  DDX_CBIndex(pDX, IDC_CBACTION, m_action);
  DDX_Text(pDX, IDC_TXTDESC1, m_txtdesc1);
  DDX_Text(pDX, IDC_DESC2, m_txtdesc2);
  DDX_Check(pDX, IDC_CKSIMPLE, m_cksimple);
  DDX_Text(pDX, IDC_TXTDESC3, m_txtdesc3);
  DDX_Control(pDX, IDC_CBNOTE, m_cbnote);
  DDX_CBIndex(pDX, IDC_CBNOTE, m_controleur);
  DDX_Text(pDX, IDC_TXTINCREMENT, m_value1);
  DDV_MinMaxLong(pDX, m_value1, 0, 127);
  DDX_Control(pDX, IDC_CB_CONTROLEUR, m_cbcontroleur);
  DDX_CBIndex(pDX, IDC_CB_CONTROLEUR, m_controleurnb);
}

BEGIN_MESSAGE_MAP(CControleurDlg, CDialog)
  ON_WM_CTLCOLOR()
  ON_BN_CLICKED(IDC_BTNVALIDATE, OnBnClickedBtnvalidate)
  ON_CBN_SELCHANGE(IDC_CBACTION, OnCbnSelchangeCbaction)
  ON_NOTIFY(NM_CLICK, IDC_LSTCONTROLER, OnNMClickLstcontroler)
  ON_BN_CLICKED(IDC_CKSIMPLE, OnBnClickedCksimple)
  ON_BN_CLICKED(IDC_BTNLOADMIDI2, OnBnClickedBtnloadmidi2)
  ON_BN_CLICKED(IDC_BTNSAVE3, OnBnClickedBtnsave3)
  ON_CBN_DROPDOWN(IDC_CB_CONTROLEUR, OnCbnDropdownCbControleur)
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
  initialied = false;
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
  inited = true;
  OnCbnSelchangeCbaction();
  inited = false;
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

static CBrush brush(RGB(230,220,12));
static CBrush brush2(RGB(230,185,70));
HBRUSH CControleurDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO :  Modifier ici les attributs du DC
	pDC->SetBkMode(TRANSPARENT);
  pDC->SetBkColor(RGB(230,185,70));
  if(pWnd == this || pWnd->GetDlgCtrlID() == IDC_STATIC || pWnd->GetDlgCtrlID() == IDC_CKSIMPLE)
  {
    return brush;
  }
	return brush2;

  // TODO :  Retourner une autre brosse si la brosse par défaut n'est pas souhaitée
  return hbr;
}

void CControleurDlg::OnBnClickedBtnvalidate()
{
  int n = m_lstcontroleur.GetCurSel();
  CControleurStk * param;
  if(n<0)
  {
    return ;
  }
  
  UpdateData();
   param = APP->controleur->Get(n);
   param->midi_channel = m_channel;
   param->midi_controleur = m_controleur;
   param->invert = m_cksimple;
   param->action = m_action ;
   param->value1 = m_value1;

  APP->controleur->ViewControleur(m_lstcontroleur,n);
}

void CControleurDlg::OnCbnSelchangeCbaction()
{



  bool note =  m_action > 2;
  UpdateData();
  if (m_action<0)return;

  m_txtdesc1 = tabdesc[m_action][0];
  m_txtdesc2 = tabdesc[m_action][1];
  if (m_cksimple)
    m_txtdesc3 = tabdesc[m_action][3];
  else
    m_txtdesc3 = tabdesc[m_action][2];

  GetDlgItem(IDC_CKSIMPLE)->SetWindowText(tabdesc[m_action][4]);

  UpdateData(FALSE);

  int val = tabvisible[m_action][m_cksimple];
  if( val & 0x1000)
  {
    GetDlgItem(IDC_CBNOTE)->ShowWindow(SW_SHOW);
    GetDlgItem(IDC_TXTDESC1)->ShowWindow(SW_SHOW);
  }
  else
  {
    GetDlgItem(IDC_CBNOTE)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_TXTDESC1)->ShowWindow(SW_HIDE);
  }
  
  
  if( val & 0x0001)
    GetDlgItem(IDC_CKSIMPLE)->ShowWindow(SW_SHOW);
  else
    GetDlgItem(IDC_CKSIMPLE)->ShowWindow(SW_HIDE);
 
  if( val & 0x0010)
  {  GetDlgItem(IDC_TXTINCREMENT)->ShowWindow(SW_SHOW);
     GetDlgItem(IDC_DESC2)->ShowWindow(SW_SHOW);
  }
  else
  {
    GetDlgItem(IDC_TXTINCREMENT)->ShowWindow(SW_HIDE);
    GetDlgItem(IDC_DESC2)->ShowWindow(SW_HIDE);
  }

  int nb = m_cbnote.GetCurSel();
  if( (!initialied || note) && m_action <= 2)
  {
    initialied = true;
    m_cbnote.SetRedraw(FALSE);
    m_cbnote.ResetContent();
    for(int i = 0;i<128;i++)
    {
      m_cbnote.AddString(tabmidi[i][0]);
    }
    m_cbnote.SetRedraw(TRUE);
    m_cbnote.SetCurSel(nb);
    
  }else if((!initialied || !note) && m_action > 2)
  {  initialied = true;
    m_cbnote.SetRedraw(FALSE);
    m_cbnote.ResetContent();
    for(int i = 0;i<128;i++)
    {
      m_cbnote.AddString(tabmidi[i][1]);
    }
    m_cbnote.SetRedraw(TRUE);
    m_cbnote.SetCurSel(nb);
    
  }

}


void CControleurDlg::OnNMClickLstcontroler(NMHDR *pNMHDR, LRESULT *pResult)
{
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
  int n = m_lstcontroleur.GetCurSel();
  CControleurStk * param;
  if(n<0)return ;
  
  UpdateData();
   nbsel = n;
   param = APP->controleur->Get(n);
   m_channel = param->midi_channel;
   m_controleur = param->midi_controleur;
   m_action     = param->action;
   m_value1 = param->value1;
   m_cksimple = param->invert;
  UpdateData(FALSE);

  OnCbnSelchangeCbaction();
  *pResult = 0;
}

void CControleurDlg::OnBnClickedCksimple()
{
  OnCbnSelchangeCbaction();
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}


void CControleurDlg::OnBnClickedBtnloadmidi2()
{
  CControleurStk clf;
  int nb = APP->controleur->Add(clf);
  APP->controleur->ViewControleur(m_lstcontroleur,nb);
}

void CControleurDlg::OnBnClickedBtnsave3()
{
  int nb = m_lstcontroleur.GetCurSel();
  if(nb < 0)return;
  int nbafft = nb -1;
  if(nbafft < 0) nbafft = 0;
  APP->controleur->Suppr(nb);
  APP->controleur->ViewControleur(m_lstcontroleur,nbafft);
}

void CControleurDlg::OnCbnDropdownCbControleur()
{
  if(!inited)
  { inited = true;
    CString buf;
    m_cbcontroleur.ResetContent();
    m_cbcontroleur.SetRedraw(FALSE);
    int i,j = APP->parameter->GetCount();

    for(i= 0; i < j;i++)
    {
      buf.Format("%d",i+1);
      m_cbcontroleur.AddString(buf);
    }
    m_cbcontroleur.SetRedraw();
    m_cbcontroleur.SetCurSel(0);
  }
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}
