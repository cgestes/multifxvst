/*****************************************************************************/
/* EffEditWnd.cpp : implementation file                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "multifxVST.h"
#include "multifxVSTmain.h"
#include "vsthost/smpvsthost.h"
#include "CCVSThost.h"
#include "EffEditWnd.h"
#include ".\effeditwnd.h"


/*
#include "stdafx.h"
#include "vsthost.h"
#include "childfrm.h"
#include "EffEditWnd.h"*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#define GetApp()  ((CVsthostApp *)AfxGetApp())

/*===========================================================================*/
/* CEffEditDlg dialog                                                        */
/*===========================================================================*/

/*****************************************************************************/
/* CEffEditDlg : constructor                                                 */
/*****************************************************************************/
IMPLEMENT_DYNAMIC(CEffEditDlg, CDialog)

CEffEditDlg::CEffEditDlg(CWnd* pParent /*=NULL*/,CAppPointer * m_app)
	: CDialog(CEffEditDlg::IDD, pParent)
{
nEffect = -1;                           /* initialize effect to nothing      */
APP = m_app;
	//{{AFX_DATA_INIT(CEffEditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

/*****************************************************************************/
/* Create : creates the dialog                                               */
/*****************************************************************************/

BOOL CEffEditDlg::Create(CWnd* pParentWnd) 
{
return CDialog::Create(IDD, pParentWnd);
}

/*****************************************************************************/
/* DoDataExchange : data exchange between dialog and object                  */
/*****************************************************************************/

void CEffEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEffEditDlg)
	DDX_Control(pDX, IDC_PARMVAL, slParmval);
	DDX_Control(pDX, IDC_PARMTEXT, stParmtext);
	DDX_Control(pDX, IDC_PARMLIST, lbParms);
	//}}AFX_DATA_MAP
}
void CEffEditDlg::Update()
{
  SetEffect(nEffect);
}
/*****************************************************************************/
/* CEffEditDlg message map                                                   */
/*****************************************************************************/

BEGIN_MESSAGE_MAP(CEffEditDlg, CDialog)
	//{{AFX_MSG_MAP(CEffEditDlg)
	ON_LBN_SELCHANGE(IDC_PARMLIST, OnSelchangeParmlist)
	ON_WM_VSCROLL()
	//}}AFX_MSG_MAP
  ON_WM_CLOSE()
  ON_WM_DESTROY()
  ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/*****************************************************************************/
/* OnInitDialog : called upon WM_INITDIALOG                                  */
/*****************************************************************************/

BOOL CEffEditDlg::OnInitDialog() 
{
CDialog::OnInitDialog();

slParmval.SetRange(0, 65535);
SetDlgItemText(IDC_RVALUE, "");
	
return TRUE;  // return TRUE unless you set the focus to a control
              // EXCEPTION: OCX Property Pages should return FALSE
}

/*****************************************************************************/
/* SetEffect : initializes dialog to a specific effect                       */
/*****************************************************************************/

void CEffEditDlg::SetEffect(int nEffect)
{
CString sParm;
this->nEffect = nEffect;
CSmpEffect *pEffect = (CSmpEffect *) APP->host->GetAt(nEffect);
lbParms.ResetContent();
if (pEffect)
  {
    pEffect->SetParmWnd(this);
    lbParms.SetRedraw(FALSE);
    for (int i = 0; i < pEffect->pEffect->numParams; i++)
    {
      GetParamName(pEffect,i,sParm);
      int idx = lbParms.AddString(sParm);
      lbParms.SetItemData(idx, i);
    }
  lbParms.SetCurSel(0);
  lbParms.SetRedraw(TRUE);
  }

OnSelchangeParmlist();
}

/*****************************************************************************/
/* OnSelchangeParmlist : called when the parameter list selection changes    */
/*****************************************************************************/

void CEffEditDlg::OnSelchangeParmlist() 
{
  int i = (int)lbParms.GetItemData(lbParms.GetCurSel());
  CEffect *pEffect = APP->host->GetAt(nEffect);
  CString sVal;
  float fVal = GetParamVal(pEffect,i,sVal);

  SetDlgItemText(IDC_RVALUE, sVal);

  //on update la liste
  lbParms.DeleteString(i);
  GetParamName(pEffect,i,sVal);
  int idx = lbParms.InsertString(i,sVal);
  lbParms.SetItemData(idx, i);
  lbParms.SetCurSel(i);

  //on update l'affichage en bas a gauche
  SetParmDisp(pEffect,i);
  //SetDlgItemText(IDC_PARMTEXT,sVal);

  //on update l'affichage du slider
  slParmval.SetPos(65535 - (int)(fVal * 65535.));
}

/*****************************************************************************/
/* SetParmDisp : internal function for parameter display                     */
/*****************************************************************************/
void  CEffEditDlg::GetParamName(CEffect *pEffect, int nParm,CString & param)
{
  CString buf;
  CString szTxt;
  param = " -erreur- ";
  if(pEffect)
  {
    //CString sParm;
    param.Format("%02d: ", nParm);
    pEffect->EffGetParamName(nParm, szTxt.GetBufferSetLength(65));
    szTxt.ReleaseBuffer();
    param += szTxt;
    param += " = ";
    GetParamDisp(pEffect,nParm,buf);
    param += buf;
  }
}

float CEffEditDlg::GetParamVal(CEffect *pEffect, int nParm,CString & param)
{
float fVal = 0.;
if (pEffect)
  {
  fVal = pEffect->EffGetParameter(nParm);
  }

param.Format("%8f", fVal);
return fVal;

}

void CEffEditDlg::GetParamDisp(CEffect *pEffect, int nParm,CString & param)
{
CString szData;
param = "[0]";
float fVal;
if (pEffect)
  {
  pEffect->EffGetParamDisplay(nParm, szData.GetBufferSetLength(60));
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
    pEffect->EffGetParamLabel(nParm, szData.GetBufferSetLength(60));
    szData.ReleaseBuffer();
    param += CString(' ') + szData;
  }
  }
}

void CEffEditDlg::SetParmDisp(CEffect *pEffect, int nParm)
{
CString szData;
GetParamDisp(pEffect,nParm,szData);
stParmtext.SetWindowText(szData);
}

/*****************************************************************************/
/* OnVScroll : called when a vertical sb / slider value changes              */
/*****************************************************************************/

void CEffEditDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
if (pScrollBar->GetDlgCtrlID() == IDC_PARMVAL)
  {
  int i = (int)lbParms.GetItemData(lbParms.GetCurSel());
  float fNewVal = float(65535 - slParmval.GetPos()) / 65535.f;
  CEffect *pEffect = APP->host->GetAt(nEffect);
  pEffect->EffSetParameter(i, fNewVal);
  SetParmDisp(pEffect, i);
  }
	
CDialog::OnVScroll(nSBCode, nPos, pScrollBar);

if (nSBCode == SB_ENDSCROLL)            /* if scroll terminated              */
  OnSelchangeParmlist();                /* reset to real current value       */
}

/*****************************************************************************/
/* OnSetParameterAutomated : called when a parameter changes                 */
/*****************************************************************************/

bool CEffEditDlg::OnSetParameterAutomated(long index, float value)
{
if (index == lbParms.GetCurSel())       /* if it's the currently displayed   */
  OnSelchangeParmlist();                /* reget parameter settings          */
return true;
}

/*===========================================================================*/
/* CEffEditWnd class members                                                 */
/*===========================================================================*/

//IMPLEMENT_DYNCREATE(CEffEditWnd, CEffectWnd)

/*****************************************************************************/
/* CEffEditWnd : constructor                                                 */
/*****************************************************************************/

/*CEffEditWnd::CEffEditWnd()
{
}*/

/*****************************************************************************/
/* ~CEffEditWnd : destructor                                                 */
/*****************************************************************************/

/*CEffEditWnd::~CEffEditWnd()
{
}*/

/*****************************************************************************/
/* CEffEditWnd message map                                                   */
/*****************************************************************************/

/*BEGIN_MESSAGE_MAP(CEffEditWnd, CEffectWnd)
	//{{AFX_MSG_MAP(CEffEditWnd)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()*/

/*****************************************************************************/
/* CreateView : creates a view for the window                                */
/*****************************************************************************/

/*CWnd * CEffEditWnd::CreateView()
{
CEffEditDlg *pWnd = new CEffEditDlg;

if (pWnd)
  pWnd->Create(this);
return pWnd;
}*/

/*****************************************************************************/
/* SetEffect : sets new effect                                               */
/*****************************************************************************/

/*void CEffEditWnd::SetEffect(int nEff)
{
CEffectWnd::SetEffect(nEff);

CEffEditDlg *pEffDlg = (CEffEditDlg *)pView;
if (pEffDlg)
  {
  pEffDlg->SetEffect(nEffect);
  CRect rc;
  pEffDlg->GetWindowRect(&rc);
  SetEffSize(rc.Width(), rc.Height());
  }
}*/

/*****************************************************************************/
/* OnClose : called when the parameter edit window is closed                 */
/*****************************************************************************/

/*void CEffEditWnd::OnClose() 
{
// we need to skip CEffectWnd::OnClose() here since it would try to close an
// (eventually unopened) effect edit window... and would report the wrong
// window as closed to the main window.

pMain->ParmClosed();
CWnd::OnClose();
}*/

/*****************************************************************************/
/* OnSetParameterAutomated : called when one of the effect's parms changes   */
/*****************************************************************************/

/*bool CEffEditWnd::OnSetParameterAutomated(long index, float value)
{
CEffEditDlg *pEffDlg = (CEffEditDlg *)pView;
if (pEffDlg)
  return pEffDlg->OnSetParameterAutomated(index, value);
else
  return CEffectWnd::OnSetParameterAutomated(index, value);
}*/

/*****************************************************************************/
/* Update : makes sure that the dialog is up to date                         */
/*****************************************************************************/

/*void CEffEditWnd::Update()
{
SetupTitle();
CEffEditDlg *pEffDlg = (CEffEditDlg *)pView;
if (pEffDlg)
  pEffDlg->OnSelchangeParmlist();
}*/

void CEffEditDlg::close()
{
  CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nEffect);
  if (pEffect)
  {
    pEffect->SetParmWnd(NULL);
  }
  nEffect = -1;
}

void CEffEditDlg::PostNcDestroy()
{
  // TODO : ajoutez ici votre code spécialisé et/ou l'appel de la classe de base
/*  CSmpEffect *pEffect = (CSmpEffect *)host->GetAt(nEffect);
  if (pEffect)
  {
    pEffect->SetParmWnd(NULL);
  }*/

  CDialog::PostNcDestroy();
    m_hWnd = NULL;
    APP->pEffParmDlg = NULL;
  //CEffectStk  * eff = ;	SetParmWnd
  delete this;
  CString buf;buf.Format("PostNcDestroy :: CEffEditDlg(%d) \n", this);  TRACE(buf);

}

void CEffEditDlg::OnClose()
{
  // TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
  ShowWindow(FALSE);
  //CDialog::OnClose();
}

void CEffEditDlg::OnDestroy()
{

  close();
  CDialog::OnDestroy();

  // TODO : ajoutez ici le code de votre gestionnaire de messages
}


static CBrush brush(RGB(110,220,120));
HBRUSH CEffEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	 pDC->SetBkMode(TRANSPARENT);
	 return brush;

  // TODO :  Modifier ici les attributs du DC

  // TODO :  Retourner une autre brosse si la brosse par défaut n'est pas souhaitée
  return hbr;
}
