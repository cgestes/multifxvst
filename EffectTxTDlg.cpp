/*****************************************************************************/
/* EffectTxTDlg.cpp : implementation file                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "multifxVST.h"
#include "multifxVSTmain.h"
#include "vsthost/smpvsthost.h"
#include "CCVSThost.h"
#include "EffectTxTDlg.h"
#include ".\EffectTxTDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

IMPLEMENT_DYNAMIC(CEffectTxTDlg, CDialog)

CEffectTxTDlg::CEffectTxTDlg(CWnd* pParent /*=NULL*/,CAppPointer * m_app)
	: CDialog(CEffectTxTDlg::IDD, pParent)
{
nEffect = -1;                           /* initialize effect to nothing      */
APP = m_app;
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

}

void CEffectTxTDlg::Update()
{
  SetEffect(nEffect);
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
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTPARMS, OnLvnItemchangedListparms)
  ON_NOTIFY(NM_CLICK, IDC_LISTPARMS, OnNMClickListparms)
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

void CEffectTxTDlg::SetEffect(int nEffect)
{
CString sParm;
this->nEffect = nEffect;
CSmpEffect *pEffect = (CSmpEffect *) APP->host->GetAt(nEffect);
m_lstparms.SetRedraw(FALSE);
m_lstparms.DeleteAllItems();
if (pEffect)
  {
    pEffect->SetParmWnd(this);

    for (int i = 0; i < pEffect->pEffect->numParams; i++)
    {
      GetParamName(pEffect,i,sParm);
      int idx =   m_lstparms.InsertItem(i,sParm);
      GetParamVal(pEffect,i,sParm);
      m_lstparms.SetItemText(idx,2,sParm);
      GetParamDisp(pEffect,i,sParm);
      m_lstparms.SetItemText(idx,1,sParm);

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
void  CEffectTxTDlg::GetParamName(CEffect *pEffect, int nParm,CString & param)
{
  CString buf;
  CString szTxt;
  param = " -erreur- ";
  if(pEffect)
  {
    //CString sParm;
    param.Format("%02d: ", nParm);
    pEffect->EffGetParamName(nParm, szTxt.GetBuffer(65));
    szTxt.ReleaseBuffer();
    param += szTxt;
    /*param += " = ";
    GetParamDisp(pEffect,nParm,buf);
    param += buf;*/
  }
}

float CEffectTxTDlg::GetParamVal(CEffect *pEffect, int nParm,CString & param)
{
float fVal = 0.;
if (pEffect)
  {
  fVal = pEffect->EffGetParameter(nParm);
  }

param.Format("%8f", fVal);
return fVal;

}

void CEffectTxTDlg::GetParamDisp(CEffect *pEffect, int nParm,CString & param)
{
CString szData;
param = "[0]";
float fVal;
if (pEffect)
  {
  pEffect->EffGetParamDisplay(nParm, szData.GetBuffer(60));
  szData.ReleaseBuffer();

  if(szData.GetLength() == 0)
  {
    fVal = pEffect->EffGetParameter(nParm);
    szData.Format("[%8f]",fVal);
    param = szData;
  }
  else
  {
    param = szData;
    pEffect->EffGetParamLabel(nParm, szData.GetBuffer(60));
    szData.ReleaseBuffer();
    param += CString(' ') + szData;
  }
  }
}

void CEffectTxTDlg::SetParmDisp(CEffect *pEffect, int nParm)
{
  CString szData;
  GetParamDisp(pEffect,nParm,szData);
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
      CEffect *pEffect = APP->host->GetAt(nEffect);
      pEffect->EffSetParameter(i, fNewVal);
      SetParmDisp(pEffect, i);
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
if (index == m_lstparms.GetCurSel())       /* if it's the currently displayed   */
  OnSelchangeParmlist();                /* reget parameter settings          */
return true;
}

void CEffectTxTDlg::close()
{
  CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nEffect);
  if (pEffect)
  {
    pEffect->SetParmWnd(NULL);
  }
  nEffect = -1;
}

void CEffectTxTDlg::PostNcDestroy()
{
  CDialog::PostNcDestroy();
  m_hWnd = NULL;
  APP->pEffParmDlg = NULL;
  //CEffectStk  * eff = ;	SetParmWnd
  delete this;
  CString buf;buf.Format("PostNcDestroy :: CEffectTxTDlg(%d) \n", this);  TRACE(buf);

}

void CEffectTxTDlg::OnClose()
{
  // TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
  ShowWindow(FALSE);
  //CDialog::OnClose();
}

void CEffectTxTDlg::OnDestroy()
{

  close();
  CDialog::OnDestroy();

  // TODO : ajoutez ici le code de votre gestionnaire de messages
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
  CEffect *pEffect = APP->host->GetAt(nEffect);
  CString sVal;
  float fVal = GetParamVal(pEffect,i,sVal);

  SetDlgItemText(IDC_RVALUE, sVal);

  //on update la liste
  //m_lstparms.DeleteItem(i);

  m_lstparms.SetRedraw(FALSE);

  GetParamName(pEffect,i,sVal);
  m_lstparms.SetItemText(sel,0,sVal);
  GetParamVal(pEffect,i,sVal);
  m_lstparms.SetItemText(sel,2,sVal);
  GetParamDisp(pEffect,i,sVal);
  m_lstparms.SetItemText(sel,1,sVal);

  m_lstparms.SetItemData(sel, i);

  //m_lstparms.SetCurSel(sel);
  m_lstparms.SetRedraw(TRUE);
  //on update l'affichage en bas a gauche
  SetParmDisp(pEffect,i);
  //SetDlgItemText(IDC_PARMTEXT,sVal);

  //on update l'affichage du slider
  slParmval.SetPos(65535 - (int)(fVal * 65535.));
}
void CEffectTxTDlg::OnLvnItemchangedListparms(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);


  *pResult = 0;
}

void CEffectTxTDlg::OnNMClickListparms(NMHDR *pNMHDR, LRESULT *pResult)
{
  OnSelchangeParmlist();
  *pResult = 0;
}
