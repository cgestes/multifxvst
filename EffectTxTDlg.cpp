/*****************************************************************************/
/* EffectTxTDlg.cpp : implementation file                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "multifxVST.h"
#include "multifxVSTmain.h"
#include "vsthost/SmpEffect.h"
#include "CCVSThost.h"
#include "EffectTxTDlg.h"
#include "stockeffet.h"
#include "controleurlst.h"
#include ".\effecttxtdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CEffectTxTDlg, CDialog)

CEffectTxTDlg::CEffectTxTDlg(CWnd* pParent /*=NULL*/,CAppPointer * m_app)
	: CDialog(CEffectTxTDlg::IDD, pParent)
  , m_controleurnb(0)
{
nEffect = -1;                           /* initialize effect to nothing      */
APP = m_app;
inited = 0;
}

/*****************************************************************************/
/* Create : creates the dialog                                               */
/*****************************************************************************/
BOOL CEffectTxTDlg::Create(CWnd* pParentWnd) 
{
return CDialog::Create(IDD, pParentWnd);
}

/*****************************************************************************/
/* DoDataExchange : data exchange between dialog and object                  */
/*****************************************************************************/
void CEffectTxTDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //{{AFX_DATA_MAP(CEffectTxTDlg)
  DDX_Control(pDX, IDC_PARMVAL, slParmval);
  DDX_Control(pDX, IDC_PARMTEXT, stParmtext);
  DDX_Control(pDX, IDC_LISTPARMS, m_lstparms);
  //}}AFX_DATA_MAP

  DDX_Control(pDX, IDC_COMBO1, m_cbcontroleurnb);
  DDX_CBIndex(pDX, IDC_COMBO1, m_controleurnb);
}

void CEffectTxTDlg::Update(int nbsel)
{
 /* CString sParm;
  m_lstparms.SetRedraw(FALSE);

  if(nbsel == -1)
    nbsel = m_lstparms.GetCurSel();

for (int idx = 0; idx < eff->pEffect->numParams; idx++)
    {
      int i = m_lstparms.GetItemData(idx);
      GetParamVal(i,sParm);
      m_lstparms.SetItemText(idx,2,sParm);
      GetParamDisp(i,sParm);
      m_lstparms.SetItemText(idx,1,sParm);

      sParm.Format("%d",effstk->Get_Controleur(i));
      m_lstparms.SetItemText(idx,3,sParm);
      //UpdateData();
      //m_controleurnb = effstk->Get_Controleur(i);
      //UpdateData(FALSE);
      m_lstparms.SetItemData(idx, i);
    }
  m_lstparms.SetCurSel(nbsel);

m_lstparms.SetRedraw(TRUE);
//OnSelchangeParmlist();
*/
}

/*****************************************************************************/
/* CEffectTxTDlg message map                                                   */
/*****************************************************************************/
BEGIN_MESSAGE_MAP(CEffectTxTDlg, CDialog)
	//{{AFX_MSG_MAP(CEffectTxTDlg)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
  ON_WM_CLOSE()
  ON_WM_DESTROY()
  ON_WM_CTLCOLOR()
  ON_NOTIFY(NM_CLICK, IDC_LISTPARMS, OnNMClickListparms)
  ON_CBN_DROPDOWN(IDC_COMBO1, OnCbnDropdownCombo1)
  ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
  ON_CBN_CLOSEUP(IDC_COMBO1, OnCbnCloseupCombo1)
END_MESSAGE_MAP()

/*****************************************************************************/
/* OnInitDialog : called upon WM_INITDIALOG                                  */
/*****************************************************************************/

BOOL CEffectTxTDlg::OnInitDialog() 
{
CDialog::OnInitDialog();

  m_lstparms.Init();
	m_lstparms.g_MyClrFgHi = RGB(35,12,200);
	m_lstparms.g_MyClrBgHi = RGB(20,242,0);
  m_lstparms.g_MyClrBg   = RGB(130,120,240);

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 12;
  //lf.lfWidth  = 10;
	strcpy(lf.lfFaceName, "Verdana");
	font.CreateFontIndirect(&lf);
	m_lstparms.SetFont(&font, TRUE);

	m_lstparms.InsertColumn(0, "Name", LVCFMT_LEFT, 150);
  m_lstparms.InsertColumn(1, "Display", LVCFMT_LEFT, 160);
	m_lstparms.InsertColumn(2, "Value", LVCFMT_LEFT, 100);
  m_lstparms.InsertColumn(3, "Controleur #", LVCFMT_LEFT, 60);

	ListView_SetExtendedListViewStyle(m_lstparms.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
	
  slParmval.SetRange(0, 65535);
  SetDlgItemText(IDC_RVALUE, "");
	
return TRUE;  // return TRUE unless you set the focus to a control
              // EXCEPTION: OCX Property Pages should return FALSE
}

/*****************************************************************************/
/* SetEffect : initializes dialog to a specific effect                       */
/*****************************************************************************/

void CEffectTxTDlg::SetEffect(int nEffectStk)
{
  int nEffect = APP->chaine_eff->get_effect(APP->current_chaine,nEffectStk);
  effstk = APP->chaine_eff->get(APP->current_chaine,nEffectStk);
CString sParm;
this->nEffect = nEffect;
this->nEffectStk = nEffectStk;
eff = (CSmpEffect *) APP->host->GetAt(nEffect);
m_lstparms.SetRedraw(FALSE);
m_lstparms.DeleteAllItems();
if (eff)
  {
    eff->SetParmWnd(this);

    for (int i = 0; i < eff->pEffect->numParams; i++)
    {
      GetParamName(i,sParm);
      int idx =   m_lstparms.InsertItem(i,sParm);
      GetParamVal(i,sParm);
      m_lstparms.SetItemText(idx,2,sParm);
      GetParamDisp(i,sParm);
      m_lstparms.SetItemText(idx,1,sParm);

      sParm.Format("%d",effstk->Get_Controleur(i));
      m_lstparms.SetItemText(idx,3,sParm);
      UpdateData();
      m_controleurnb = effstk->Get_Controleur(i);
      UpdateData(FALSE);
      m_lstparms.SetItemData(idx, i);
    }
  m_lstparms.SetCurSel(0);

  }
m_lstparms.SetRedraw(TRUE);
OnSelchangeParmlist();
}



/*****************************************************************************/
/* SetParmDisp : internal function for parameter display                     */
/*****************************************************************************/
void  CEffectTxTDlg::GetParamName( int nParm,CString & param)
{
  CString buf;
  CString szTxt;
  param = " -erreur- ";
  if(eff)
  {
    //CString sParm;
    param.Format("%02d: ", nParm);
    eff->EffGetParamName(nParm, szTxt.GetBuffer(65));
    szTxt.ReleaseBuffer();
    param += szTxt;
    /*param += " = ";
    GetParamDisp(pEffect,nParm,buf);
    param += buf;*/
  }
}

float CEffectTxTDlg::GetParamVal( int nParm,CString & param)
{
float fVal = 0.;
if (eff)
  {
  fVal = eff->EffGetParameter(nParm);
  }

param.Format("%8f", fVal);
return fVal;

}

void CEffectTxTDlg::GetParamDisp( int nParm,CString & param)
{
CString szData;
param = "[0]";
float fVal;
if (eff)
  {
  eff->EffGetParamDisplay(nParm, szData.GetBuffer(60));
  szData.ReleaseBuffer();

  if(szData.GetLength() == 0)
  {
    fVal = eff->EffGetParameter(nParm);
    szData.Format("[%8f]",fVal);
    param = szData;
  }
  else
  {
    param = szData;
    szData = "";
    eff->EffGetParamLabel(nParm, szData.GetBuffer(60));
    szData.ReleaseBuffer();
    param += CString(' ') + szData;
  }
  }
}

void CEffectTxTDlg::SetParmDisp( int nParm)
{
  CString szData;
  GetParamDisp(nParm,szData);
  stParmtext.SetWindowText(szData);
}

/*****************************************************************************/
/* OnVScroll : called when a vertical sb / slider value changes              */
/*****************************************************************************/

void CEffectTxTDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
if (pScrollBar->GetDlgCtrlID() == IDC_PARMVAL)
  {
    int sel = m_lstparms.GetCurSel();
    if(sel >= 0)
    {
      int i = (int)m_lstparms.GetItemData(sel);
      float fNewVal = float(65535 - slParmval.GetPos()) / 65535.f;
      eff->EffSetParameter(i, fNewVal);

      int ind = effstk->Get_Controleur(i);
      if(ind != -1)
      {
        APP->parameter->setParameter(ind-1,fNewVal);
        APP->effect->setParameterAutomated(ind-1+kNumParams,fNewVal);
      }
      SetParmDisp(i);
    }
  }
	
CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

if (nSBCode == SB_ENDSCROLL)            /* if scroll terminated              */
  OnSelchangeParmlist();                /* reset to real current value       */
}





/*****************************************************************************/
/* OnSetParameterAutomated : called when a parameter changes                 */
/*****************************************************************************/

bool CEffectTxTDlg::OnSetParameterAutomated(long index, float value)
{
  int sel = m_lstparms.GetCurSel();
  bool selected = false;

  if (index == sel)       
    selected = true;

  sel = (int ) index;
  if(sel < 0)return false;
  int i = (int)m_lstparms.GetItemData(sel);
  CString sVal;
  float fVal = GetParamVal(i,sVal);

  SetDlgItemText(IDC_RVALUE, sVal);

  //on update la liste
  //m_lstparms.DeleteItem(i);

  m_lstparms.SetRedraw(FALSE);
  /*GetParamName(i,sVal);
  m_lstparms.SetItemText(sel,0,sVal);*/
  GetParamVal(i,sVal);
  m_lstparms.SetItemText(sel,2,sVal);
  GetParamDisp(i,sVal);
  m_lstparms.SetItemText(sel,1,sVal);
  sVal.Format("%d",effstk->Get_Controleur(i));
  m_lstparms.SetItemText(sel,3,sVal);
  m_lstparms.SetItemData(sel, i);
  /*UpdateData();
  m_controleurnb = effstk->Get_Controleur(i);
  UpdateData(FALSE);*/
  //m_lstparms.SetCurSel(sel);
  m_lstparms.SetRedraw(TRUE);

  if(selected)
  {
    //on update l'affichage en bas a gauche
    SetParmDisp(i);
    //SetDlgItemText(IDC_PARMTEXT,sVal);

    //on update l'affichage du slider
    slParmval.SetPos(65535 - (int)(fVal * 65535.));
  }

return true;
}

void CEffectTxTDlg::close()
{
  if (eff)
  {
    eff->SetParmWnd(NULL);
    eff = NULL;
  }
  nEffect = -1;
}

void CEffectTxTDlg::PostNcDestroy()
{
  CDialog::PostNcDestroy();
  m_hWnd = NULL;
  APP->pEffParmDlg = NULL;
  delete this;
  CString buf;buf.Format("PostNcDestroy :: CEffectTxTDlg(%d) \n", this);  TRACE(buf);
}

void CEffectTxTDlg::OnClose()
{
  ShowWindow(FALSE);
}

void CEffectTxTDlg::OnDestroy()
{
  close();
  CDialog::OnDestroy();
}


static CBrush brush(RGB(110,220,120));
HBRUSH CEffectTxTDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	 pDC->SetBkMode(TRANSPARENT);
	 return brush;

  // TODO :  Modifier ici les attributs du DC

  // TODO :  Retourner une autre brosse si la brosse par défaut n'est pas souhaitée
  return hbr;
}
void CEffectTxTDlg::OnSelchangeParmlist()
{
  int sel = m_lstparms.GetCurSel();
  if(sel < 0)return;
  int i = (int)m_lstparms.GetItemData(sel);
  CString sVal;
  float fVal = GetParamVal(i,sVal);

  SetDlgItemText(IDC_RVALUE, sVal);

  //on update la liste
  //m_lstparms.DeleteItem(i);

  m_lstparms.SetRedraw(FALSE);

  /*GetParamName(i,sVal);
  m_lstparms.SetItemText(sel,0,sVal);*/
  GetParamVal(i,sVal);
  m_lstparms.SetItemText(sel,2,sVal);
  GetParamDisp(i,sVal);
  m_lstparms.SetItemText(sel,1,sVal);
  
  sVal.Format("%d",effstk->Get_Controleur(i));
  m_lstparms.SetItemText(sel,3,sVal);

  m_lstparms.SetItemData(sel, i);

  UpdateData();
  m_controleurnb = effstk->Get_Controleur(i);
  UpdateData(FALSE);
  //m_lstparms.SetCurSel(sel);
  m_lstparms.SetRedraw(TRUE);
  //on update l'affichage en bas a gauche
  SetParmDisp(i);
  //SetDlgItemText(IDC_PARMTEXT,sVal);

  //on update l'affichage du slider
  slParmval.SetPos(65535 - (int)(fVal * 65535.));
}


void CEffectTxTDlg::OnNMClickListparms(NMHDR *pNMHDR, LRESULT *pResult)
{
  OnSelchangeParmlist();
  *pResult = 0;
}

void CEffectTxTDlg::OnCbnDropdownCombo1()
{

  if(!inited)
  { inited = true;
    CString buf;
    m_cbcontroleurnb.ResetContent();
    m_cbcontroleurnb.SetRedraw(FALSE);
    int i,j = APP->parameter->GetCount();
    m_cbcontroleurnb.AddString(buf);
    for(i= 0; i < j;i++)
    {
      buf.Format("%d",i+1);
      m_cbcontroleurnb.AddString(buf);
    }
    m_cbcontroleurnb.SetRedraw();
    m_cbcontroleurnb.SetCurSel(0);
  }

}

void CEffectTxTDlg::OnCbnSelchangeCombo1()
{ 
  int nbsel = m_lstparms.GetCurSel();
  if(nbsel < 0)return;
  UpdateData();
  int m_value;
  if(m_controleurnb == 0)
    m_value = -1;
  else
    m_value = m_controleurnb;

  int nbparam = m_lstparms.GetItemData(nbsel);
  int ancval = effstk->Get_Controleur(nbparam);
  effstk->Set_Controleur(nbparam,m_value,eff);
  //on supprime l'ancienne valeur
  if(ancval != -1)
  {
    APP->parameter->ParamSupprParam(ancval-1,nEffectStk,nbparam);
  }
  //on ajoute la nouvelle valeur
  APP->parameter->ParamAddParam(m_value-1,nEffectStk,nbparam);
  
  CString buf;
  buf.Format("%d",m_value);
  m_lstparms.SetItemText(nbsel,3,buf);
}

void CEffectTxTDlg::OnCbnCloseupCombo1()
{
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}
