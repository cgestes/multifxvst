// MainDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "multifxVST.h"
#include "MainDlg.h"
#include "multifxVSTEditor.h"

#include ".\maindlg.h"

//#include "multifxVSTmain.h"
#include "stockeffet.h"
#include "CCVSThost.h"
#include "ChainDlg.h"
#include "multifxVSTEditor.h"
#include "vsthost/SmpEffect.h"
#include "effectwnd.h"
#include "EffectTxTDlg.h"
#include "Chaindlg.h"
// Boîte de dialogue CMainDlg
#include "controleurdlg.h"

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
  , m_txteff(_T(""))
{
    m_txtchaine = "0";
    APP            = 0;
    pActiv         = NULL;
    nbeff          = -1;
    nbef           = -1;
}
void CMainDlg::ChangeChaine(int chaine,BOOL paramAutom)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if(!VCH(chaine))return ;

  //APP->pMainDlg->KillEffect();
  //ChildNotify(APP->pChain);

  //on affiche la nouvelle chaine
  APP->chaine_eff->ViewChaine(APP->current_chaine,APP->pChain->m_listvst,0);
 

  UpdateData();
  //update le splider
  m_sldchaine = APP->current_chaine;
  //update le txt
  m_txtchaine.Format("%d", m_sldchaine);
  UpdateData(FALSE);
}

CMainDlg::~CMainDlg()
{
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Slider(pDX, IDC_SLDCHAINE, m_sldchaine);
  DDX_Control(pDX, IDC_SLDCHAINE, m_sld);
  DDX_Text(pDX, IDC_TXTCHAINE, m_txtchaine);

  DDX_Control(pDX, IDC_BUTTON1, m_btnchaine);
  DDX_Control(pDX, IDC_BUTTON2, m_btncontroleur);
  DDX_Control(pDX, IDC_BUTTON3, m_btneff);
  DDX_Control(pDX, IDC_BTNEFFUP, m_btneffup);
  DDX_Control(pDX, IDC_BTNEFFDOWN, m_btneffdown);
  DDX_Text(pDX, IDC_TXTEFF, m_txteff);
  DDX_Control(pDX, IDC_BTNCHPLUS, m_btnchplus);
  DDX_Control(pDX, IDC_BTNCHMOINS, m_btnchmoins);
  DDX_Control(pDX, IDC_BUTTON5, m_btnpreset);
  DDX_Control(pDX, IDC_BTNBYPASS, m_btnbypass);
  DDX_Control(pDX, IDC_BTNEFFECTTXT, m_btneffect2);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
  ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
//  ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLDCHAINE, OnNMReleasedcaptureSldchaine)
  ON_WM_CTLCOLOR()
  ON_WM_ERASEBKGND()
  ON_BN_CLICKED(IDC_BTNEFFUP, OnBnClickedBtneffup)
  ON_BN_CLICKED(IDC_BTNEFFDOWN, OnBnClickedBtneffdown)
  ON_WM_INITMENUPOPUP()
  ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
  ON_COMMAND_RANGE(IDM_EFF_PROGRAM_0, IDM_EFF_PROGRAM_0+999, OnSetProgram)
  ON_COMMAND(ID_FXB_LOADFXP, OnFxbLoadfxp)
  ON_COMMAND(ID_FXB_SAVEFXP, OnFxbSavefxp)
  ON_COMMAND(ID_FXB_LOADBANK, OnFxbLoadbank)
  ON_COMMAND(ID_FXB_SAVEBANK, OnFxbSavebank)
  ON_WM_SIZE()
  ON_BN_DOUBLECLICKED(IDC_BUTTON3, OnBnDoubleclickedButton3)
  ON_WM_HSCROLL()
//  ON_WM_VSCROLL()
ON_BN_CLICKED(IDC_BTNCHMOINS, OnBnClickedBtnchmoins)
ON_BN_CLICKED(IDC_BTNCHPLUS, OnBnClickedBtnchplus)
ON_BN_DOUBLECLICKED(IDC_BTNCHPLUS, OnBnDoubleclickedBtnchplus)
ON_BN_DOUBLECLICKED(IDC_BTNCHMOINS, OnBnDoubleclickedBtnchmoins)
ON_BN_DOUBLECLICKED(IDC_BTNEFFUP, OnBnDoubleclickedBtneffup)
ON_BN_DOUBLECLICKED(IDC_BTNEFFDOWN, OnBnDoubleclickedBtneffdown)
ON_WM_SHOWWINDOW()
ON_BN_DOUBLECLICKED(IDC_BUTTON2, OnBnDoubleclickedButton2)

ON_BN_CLICKED(IDC_BTNEFFECTTXT, OnBnClickedBtneffecttxt)
//ON_COMMAND(ID_EFFECTS_SHELLPLUG, OnEffectsShellplug)
END_MESSAGE_MAP()


void CMainDlg::OnSetProgram(UINT nID)
{
CEffect *pEffect = APP->host->GetAt(nbef);
if (pEffect)
  {
  pEffect->EffSetProgram(nID - IDM_EFF_PROGRAM_0);

  //rafraichi les plug-ins (affichage)
  OnUpdate();
  }

  //pas oublier le update
}

//void CMainDlg::OnNMReleasedcaptureSldchaine(NMHDR *pNMHDR, LRESULT *pResult)
//{
///*
//  UpdateData();
//  if(APP->current_chaine == m_sldchaine)return;
//  APP->effect->setParameterAutomated(0,NBChaine2float(m_sldchaine));
//*/
//  //ChangeChaine(m_sldchaine);
//
//  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
//  *pResult = 0;
//}

//place la fenetre a sa place,redimentionne decoupe cisaille tronconne etc...
static const OFFSETY = 40;
void CMainDlg::ChildNotify(CWnd * child,bool bAlwaisResize)
{

  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  bool Alwaisresize = (pActiv == child)  ;
  

  int posx=0,posy=OFFSETY;
  CloseAll();

  if(!child)return;

  /*ERect *rect;

  APP->editor->getRect(&rect);

  WinRectInit.bottom = rect->bottom;
  WinRectInit.top = rect->top;
  WinRectInit.left = rect->left;
  WinRectInit.right = rect->right;*/

  ::CRect r;
  child->GetClientRect(&r);
  if(Alwaisresize)
  {
      WinRect.bottom = WinRect.top  + OFFSETY + r.Height(); 
      WinRect.right  = WinRect.left + r.Width();
      
      /*if(WinRect.Height() < WinRectInit.Height())
        WinRect.bottom = WinRect.top  + OFFSETY + WinRectInit.Height();*/ 
      
      if(WinRect.Width() < WinRectInit.Width())
        WinRect.right = WinRect.left  + WinRectInit.Width(); 

  }
  else
  {
    if(r.Height() > (WinRect.Height()-OFFSETY))
      WinRect.bottom = WinRect.top + OFFSETY + r.Height(); 

    if(r.Width() > WinRect.Width())
      WinRect.right = WinRect.left + r.Width();


  }

  posx =  (WinRect.Width() - r.Width()) >> 1;
  posy =  OFFSETY + (((WinRect.Height()- OFFSETY) - r.Height()) >> 1 );

  //if(WinRect.Width < r.Width())Win
  APP->editor->frame->setSize(WinRect.Width(),WinRect.Height());
  SetWindowPos(NULL,0,0,WinRect.Width(),WinRect.Height(),SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW);

  //positionne la fenetre enfant
  child->SetWindowPos(NULL,posx,posy ,0,0,SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW);
  //child->UpdateWindow();
  child->ShowWindow(SW_SHOW);
 

  //pointeur sur la fenetre active
  if(!bAlwaisResize) //en fait pActiv windows ne sert que pour le size
   pActiv = child;


}

void CMainDlg::EnterIdle()
{
  //IDLE DE L'host VST pour laffichage du plug enfant
  if(APP->pEffEditDlg)
    APP->pEffEditDlg->EnterIdle();
  //if(pEditWnd)pEditWnd->EnterIdle();
  return;
}

void CMainDlg::SetEffect(int nb)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  //if(nb == nbeff)return;
  UpdateData();
  
  if(nb < 0)
  {
    nbeff = -1;//un nb peu correspondr a un effet a t = 0 , et a un autre a t = +!!
    nbef  = -1;
    m_txteff = "";
  }
  else
  {
    nbeff = nb;//un nb peu correspondr a un effet a t = 0 , et a un autre a t = +!!
    nbef  = APP->chaine_eff->get_effect(APP->current_chaine,nb);
    m_txteff.Format("%d:%s",nb+1,APP->chaine_eff->Get_Name(APP->current_chaine,nb));
  }

  UpdateData(FALSE);

}

void CMainDlg::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  //crée la fenetre du listing des effets
  APP->pChain = new CChainDlg(this);
  ASSERT(APP->pChain);
  BOOL b = APP->pChain->Create((UINT)CChainDlg::IDD ,this);
  ASSERT(b);

  APP->pControleur = new CControleurDlg(this);
  ASSERT(APP->pControleur);
  b = APP->pControleur->Create((UINT)CControleurDlg::IDD,this);
  ASSERT(b);

  APP->pControleur->SetAPP(APP);

  APP->pChain->SetAPP(APP);
  APP->pMainDlg->ChangeChaine(float2NBChaine( APP->effect->getParameter(0)),FALSE);
  APP->pControleur->Update();


  APP->pChain->ShowWindow(SW_SHOW);
  ChildNotify(APP->pChain);
}

void CMainDlg::CloseAll()
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if(APP->pChain)
    APP->pChain->ShowWindow(SW_HIDE);

  if(APP->pControleur)
    APP->pControleur->ShowWindow(SW_HIDE);

  if(APP->pEffParmDlg)
    APP->pEffParmDlg->ShowWindow(SW_HIDE);

  if(APP->pEffEditDlg)
    APP->pEffEditDlg->ShowWindow(SW_HIDE);

  pActiv = NULL;
}

void CMainDlg::KillAll()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if(APP->pEffParmDlg)
  {
    APP->pEffParmDlg->ShowWindow(SW_HIDE);
    APP->pEffParmDlg->DestroyWindow();
    APP->pEffParmDlg = NULL;
  }
  if(APP->pEffEditDlg)
  {
    APP->pEffEditDlg->ShowWindow(SW_HIDE);
    APP->pEffEditDlg->DestroyWindow();
    APP->pEffEditDlg = NULL;
  }
  if(APP->pChain)
  {
    APP->pChain->ShowWindow(SW_HIDE);
    APP->pChain->DestroyWindow();
    APP->pChain = NULL;
  }

  if(APP->pControleur)
  {
    APP->pControleur->ShowWindow(SW_HIDE);
    APP->pControleur->DestroyWindow();
    APP->pControleur = NULL;
  }

  pActiv = NULL;
}

//0 =tt    1 = eff    2 = txt
void CMainDlg::KillEffect(int nb)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if(APP->pEffParmDlg && ((nb == 2)||(nb==0)))
  {
    if(pActiv == APP->pEffParmDlg)
      pActiv = NULL;

    APP->pEffParmDlg->ShowWindow(SW_HIDE);
    APP->pEffParmDlg->DestroyWindow();
    APP->pEffParmDlg = NULL;
  }
  if(APP->pEffEditDlg && ((nb == 1)||(nb==0)))
  {
    if(pActiv == APP->pEffEditDlg)
      pActiv = NULL;

    APP->pEffEditDlg->ShowWindow(SW_HIDE);
    APP->pEffEditDlg->DestroyWindow();
    APP->pEffEditDlg = NULL;
  }
}


BOOL CMainDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  m_btnchaine.LoadBitmap(IDB_HOVERBUTTON);
  CString text=_T("Gestion des chaines");
  m_btnchaine.SetToolTipText(&text);

  m_btncontroleur.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Gestion des controleurs");
  m_btncontroleur.SetToolTipText(&text);

  m_btneff.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Affiche l'effet");
  m_btneff.SetToolTipText(&text);

  m_btneffect2.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Affiche l'effet en mode texte");
  m_btneffect2.SetToolTipText(&text);

  m_btnbypass.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Bypass un effet");
  m_btnbypass.SetToolTipText(&text);

  m_btneffup.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Sélectionne l'effet suivant dans la chaine actuelle");
  m_btneffup.SetToolTipText(&text);

  m_btneffdown.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Sélectionne l'effet précédent dans la chaine actuelle");
  m_btneffdown.SetToolTipText(&text);

  m_btnchplus.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Sélectionne la chaine suivante");
  m_btnchplus.SetToolTipText(&text);

  m_btnchmoins.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Sélectionne la chaine précédente");
  m_btnchmoins.SetToolTipText(&text);

  m_btnpreset.LoadBitmap(IDB_HOVERBUTTON);
  text=_T("Configuration de l'effet");
  m_btnpreset.SetToolTipText(&text);


  m_sld.SetRange(0,127);

  m_sld.SetPageSize(10);
  m_sld.SetLineSize(1);
  //m_sld.SetTic(20);
  m_sld.SetTicFreq(10);

  //on récupere la taille des fenetres
	GetWindowRect(&WinRect);
	GetWindowRect(&WinRectInit);
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

//n
void CMainDlg::OpenEffect(int chaine,int nbeffectstk)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  //int nedit = m_listvst.GetCurSel();
  //m_listvst.GetSele
  if(nbeffectstk < 0)return;

  if(APP->pEffEditDlg)
  {
    //c'est deja cette effect...on réactive la fenetre et tayo
    if(APP->chaine_eff->get_effect(chaine , nbeffectstk)== APP->pEffEditDlg->nEffect )
    {
      /*CloseAll();
      APP->pEffEditDlg->ShowWindow(SW_SHOW);
      APP->pEffEditDlg->UpdateWindow();*/
      ChildNotify(APP->pEffEditDlg);
      return;
    }
  }
  //on ferme les anciennes fenetres
  KillEffect(1);

  int nEffect = APP->chaine_eff->get_effect(chaine,nbeffectstk);

  CEffect *pEffect = APP->host->GetAt(nEffect);

  ERect *prc = NULL;
  long lResult = APP->host->EffEditGetRect(nEffect, &prc);


  if (prc){
    CEffectWnd *pWnd = (CEffectWnd *) new CEffectWnd(APP);
    CString strMyClass;
    // my own window class  DA RULEZ :!
    try{   strMyClass = AfxRegisterWndClass(NULL,::LoadCursor(NULL, IDC_ARROW),
          (HBRUSH) ::GetStockObject(WHITE_BRUSH),::LoadIcon(NULL, IDI_APPLICATION));
    }catch (CResourceException* pEx)
    {  AfxMessageBox(_T("Couldn't register class! (Already registered?)"));
          pEx->Delete();
    }

    //pWnd->Create( NULL,"Effect",WS_CAPTION | WS_VISIBLE ,CRect(0, 0, 20, 20),this,1234);
    pWnd->CreateEx( WS_EX_TOPMOST, strMyClass,"Effect", /*WS_CAPTION |*/ /*WS_BORDER|*/ WS_VISIBLE | WS_CHILD/* | WS_SYSMENU*/  ,::CRect(0, 0, 0, 0),this,0);

    //pWnd->CreateEx( WS_EX_TOPMOST,strMyClass,"Effect",WS_CAPTION | WS_VISIBLE  ,CRect(0, 0, 20, 20),GetDesktopWindow(),1234);
 
    if (pWnd)
      {
      //pWnd->ShowWindow(SW_SHOW);
      pWnd->SetEffect(nEffect);
//      pWnd->SetMain(this);
      APP->pEffEditDlg = pWnd;

      ::CRect r;
      
      pWnd->GetWindowRect(&r);



      ((CSmpEffect *)pEffect)->SetEditWnd(pWnd);
      lResult = APP->host->EffEditOpen(nEffect, pWnd->GetSafeHwnd());
      long lResult = APP->host->EffEditGetRect(nEffect, &prc);
      pWnd->SetEffSize(prc);
      pWnd->SetupTitle();

      ChildNotify(pWnd);

      //pWnd->ShowWindow(SW_SHOW);
     // APP->pEffEditDlg->Update();
      }
  }//if (prc)
  else
  {
    OpenEffectTxT(chaine,nbeffectstk);
  }

}

void CMainDlg::OpenEffectTxT(int chaine,int nbeffectstk)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if(nbeffectstk < 0)return;


  if(APP->pEffParmDlg)
  {
    if(APP->chaine_eff->get_effect(APP->current_chaine ,nbeffectstk) == APP->pEffParmDlg->nEffect )
    {
      /*CloseAll();
      APP->pEffParmDlg->ShowWindow(SW_SHOW);
      APP->pEffParmDlg->UpdateWindow();*/
      ChildNotify(APP->pEffParmDlg);
      return;
    }
  }
  //close();
  KillEffect(2);
//else
  {
  int nEffect = APP->chaine_eff->get_effect(APP->current_chaine,nbeffectstk);

  CEffect *pEffect = APP->host->GetAt(nEffect);

  {

    CEffectTxTDlg  *pWnd = (CEffectTxTDlg *) new CEffectTxTDlg(this,APP);
    if(!pWnd->Create(this))
    {TRACE("ERREUR EFFEDITDLG->CREATEINDIRECT (OnbtnE2)");
    return;
    }

    if (pWnd)
      {
      pWnd->SetEffect(/*nEffect,*/nbeffectstk);

      APP->pEffParmDlg = pWnd;
      
      ::CRect r;
      pWnd->GetWindowRect(&r);

      //place la fentre au bon endroit
      ChildNotify(pWnd);

      //pWnd->ShowWindow(SW_SHOW);
     //pWnd->Update();// NEEDDD
      }
    }
  }
}

void CMainDlg::OnBnClickedButton1()
{
  APP->pMainDlg->ChildNotify(APP->pChain);
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}

void CMainDlg::OnBnClickedButton2()
{
  APP->pMainDlg->ChildNotify(APP->pControleur);
  //APP->pMainDlg->ChildNotify(APP->pEffEditDlg,1);
  //APP->pMainDlg->ChildNotify(APP->pEffEditDlg,2);
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}

void CMainDlg::OnBnClickedButton3()
{
    OpenEffect(APP->current_chaine,nbeff);  
}

void CMainDlg::OnBnClickedBtneffecttxt()
{
  OpenEffectTxT(APP->current_chaine,nbeff); 
}

void CMainDlg::PostNcDestroy()
{
  CDialog::PostNcDestroy();
  APP->pMainDlg = NULL;
  delete this;
}


static CBrush brush(RGB(110,220,120));
static CBrush brush2(RGB(255,128,128));
HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	pDC->SetBkMode(TRANSPARENT);
  if(pWnd->GetDlgCtrlID() == IDC_TXTEFF)
	{
	 return brush2;
	}
  else
    return brush;

  return hbr;
}

BOOL CMainDlg::DestroyWindow()
{
  KillAll();  //  :-)
  return CDialog::DestroyWindow();
}

BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
{

  pDC->FillSolidRect(0,0,WinRect.Width(),OFFSETY>>1,RGB(110,220,120));
  pDC->FillSolidRect(0,OFFSETY>>1,WinRect.Width(),OFFSETY>>1,RGB(255,128,128));
  pDC->FillSolidRect(0,OFFSETY,WinRect.Width(),WinRect.Height()-OFFSETY,RGB(230,220,12));
  return TRUE;

}


void CMainDlg::OnBnClickedBtneffup()
{
  if(nbeff < 1)return;
  nbeff--;

  //met a jour la list d'effet (la selection)
  //la selection => met a jour le text dans main
  APP->pChain->m_listvst.EnsureVisible(nbeff,TRUE);
  APP->pChain->m_listvst.SetItemState(nbeff,LVIS_SELECTED,LVIS_SELECTED);
  
  //SetEffect(nbeff);

  if(APP->pEffEditDlg == pActiv || APP->pEffParmDlg == pActiv)
    OpenEffect(APP->current_chaine,nbeff);


  //else on fait rien car,tt est deja fait!


}

void CMainDlg::OnBnClickedBtneffdown()
{
  if(nbeff >= APP->chaine_eff->get_count(APP->current_chaine)-1)return;
  nbeff++;

  //met a jour la list d'effet (la selection)
  //la selection => met a jour le text dans main
  APP->pChain->m_listvst.EnsureVisible(nbeff,TRUE);
  APP->pChain->m_listvst.SetItemState(nbeff,LVIS_SELECTED,LVIS_SELECTED);
  //SetEffect(nbeff);

  if(APP->pEffEditDlg == pActiv || APP->pEffParmDlg == pActiv)
    OpenEffect(APP->current_chaine,nbeff);



}

void CMainDlg::FillPopup(CMenu* pPopupMenu, int nEffect)
{

int i, j;
CEffect *pEffect = APP->host->GetAt(nEffect);
                                        /* remove all old items              */
for (i = pPopupMenu->GetMenuItemCount() - 1; i >= 2; i--)
  {
  if ((pPopupMenu->GetSubMenu(i)) ||
      (pPopupMenu->GetMenuItemID(i) ==
           IDM_EFF_PROGRAM_0))
    pPopupMenu->RemoveMenu(i, MF_BYPOSITION);
  }

if ((pEffect) &&
    (pEffect->pEffect->numPrograms > 1))
  {
  long lProg = pEffect->EffGetProgram();
  bool bProgSet = false;

  int nClass = (pEffect->pEffect->flags & effFlagsIsSynth) ?
      kPlugCategSynth :
      kPlugCategUnknown;
  for (i = 0; i < pEffect->pEffect->numPrograms; i += 16)
    {
    CMenu popup;
    popup.CreatePopupMenu();
    for (j = i; (j < i + 16) && (j < pEffect->pEffect->numPrograms); j++)
      {
      CString sProg;
      char szPgName[256] = "";
      if (!pEffect->EffGetProgramNameIndexed(nClass, j, szPgName))
        {
        bProgSet = true;
        pEffect->EffSetProgram(j);
        pEffect->EffGetProgramName(szPgName);
        if (!*szPgName)
         sprintf(szPgName, "Program %d", j);
        }
      sProg.Format("%d. %s", j, szPgName);
      popup.AppendMenu(MF_STRING, IDM_EFF_PROGRAM_0 + j, sProg);
      }
    CString sSub;
    sSub.Format("Programs %d-%d", i, j-1);
    pPopupMenu->AppendMenu(MF_POPUP | MF_STRING,
                           (UINT)popup.Detach(),
                           sSub);
    }
  if (bProgSet)
    pEffect->EffSetProgram(lProg);
  pPopupMenu->CheckMenuItem(IDM_EFF_PROGRAM_0 + lProg,
                            MF_CHECKED | MF_BYCOMMAND);
  }
}

void CMainDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
  CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

  if ((pPopupMenu->GetMenuItemCount() > 0) &&
    (pPopupMenu->GetMenuItemID(pPopupMenu->GetMenuItemCount()-1) == IDM_EFF_PROGRAM_0))
  if(nbef>=0)
    FillPopup(pPopupMenu,nbef);

}

void CMainDlg::OnBnClickedButton5()
{

  ::CRect r;
  GetDlgItem(IDC_BUTTON5)->GetWindowRect(&r);


    if (mnu1.GetSafeHmenu() != NULL)
          mnu1.DestroyMenu();

    mnu1.LoadMenu(IDR_MNUVST);
    CMenu  * mnu = mnu1.GetSubMenu(0);
    if(mnu)
      mnu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, r.left , r.bottom , this);


  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}
void CMainDlg::SaveBank(CString sName)
{
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbef);
if (!pEffect)
  return;

CFxBank b;

if (pEffect->pEffect->flags & effFlagsProgramChunks)
  {
  void * pChunk;
  int lSize = pEffect->EffGetChunk(&pChunk);
  if (lSize)
    b.SetSize_Chunk(lSize);
  if (b.IsLoaded())
    b.SetChunk(pChunk);
  }
else
  {
    b.SetSize(pEffect->pEffect->numPrograms, pEffect->pEffect->numParams);
  if (b.IsLoaded())
    {
    int i, j;
    int nParms = b.GetNumParams();

    int cProg = pEffect->EffGetProgram();
    for (i = 0; i < b.GetNumPrograms(); i++)
      {
      pEffect->EffSetProgram(i);
      char szName[128];
      pEffect->EffGetProgramName(szName);
      b.SetProgramName(i, szName);
      for (j = 0; j < nParms; j++)
        b.SetProgParm(i, j, pEffect->EffGetParameter(j));
      }
    pEffect->EffSetProgram(cProg);
    }
  }
if (!b.IsLoaded())
  {
  MessageBox("Impossible d'allouer de la mémoire!", "Oulala!", MB_ICONERROR);
  return;
  }

b.SetFxID(pEffect->pEffect->uniqueID);
b.SetFxVersion(pEffect->pEffect->version);
if (b.SaveBank((char *)(LPCSTR)sName))
  {
    pEffect->SetChunkFile(sName);

  OnUpdate();

  }
else
  MessageBox("Impossible de sauvegarder la bank", NULL, MB_ICONERROR);


}
void CMainDlg::SavePreset(CString sName)
{
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbef);
if (!pEffect)
  return;

CFxBank b;

if (pEffect->pEffect->flags & effFlagsProgramChunks)
  {
  void * pChunk;
  int lSize = pEffect->EffGetChunk(&pChunk,true);
  if (lSize)
    b.SetSize_ChunkPreset(lSize);
  if (b.IsLoaded())
    b.SetChunk(pChunk);
  }
else
  {
    b.SetSize_Preset(pEffect->pEffect->numParams);
  if (b.IsLoaded())
    {
    int  j;
    int nParms = b.GetNumParams();

    for (j = 0; j < nParms; j++)
      b.SetProgParm(0, j, pEffect->EffGetParameter(j));

    }
  }
  if(b.IsLoaded())
  {
  char buffer[28]; 
  memset(buffer,0,28);
  pEffect->EffGetProgramName(buffer);
  b.SetProgramName(0,buffer);
  }

if (!b.IsLoaded())
  {
  MessageBox("Impossible d'allouer de la mémoire!", "Oulala!", MB_ICONERROR);
  return;
  }

b.SetFxID(pEffect->pEffect->uniqueID);
b.SetFxVersion(pEffect->pEffect->version);
if (b.SaveBank((char *)(LPCSTR)sName))
  {

    pEffect->SetChunkFilePreset(sName);


  OnUpdate();

  }
else
  MessageBox("Impossible de sauvegarder la bank", NULL, MB_ICONERROR);

}

void CMainDlg::OnFxbSavebank()
{
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbef);
if (!pEffect)
  return;

CFileDialog dlg(FALSE,"fxb",pEffect->GetChunkFile(),OFN_CREATEPROMPT | OFN_ENABLESIZING |
                    OFN_NOCHANGEDIR | OFN_NOREADONLYRETURN |
                    OFN_OVERWRITEPROMPT,"Effect Bank Files (.fxb)|*.fxb|All Files|*.*||");
if (dlg.DoModal() == IDOK)
  SaveBank(dlg.GetPathName());
}

void CMainDlg::OnFxbLoadbank()
{
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbef);
if(!pEffect)return;
CFileDialog dlg(TRUE,"fxb", pEffect->GetChunkFile(),OFN_ENABLESIZING | OFN_NOCHANGEDIR,
                "Effect Bank Files (.fxb)|*.fxb|All Files|*.*||");
if (dlg.DoModal() != IDOK)
  return;

CFxBank b((char *)(LPCSTR)dlg.GetPathName());
if (b.IsLoaded())
  {
  if (!pEffect)
    return;
  if (pEffect->pEffect->uniqueID != b.GetFxID())
    {
    MessageBox("L'id de la banque ne correspond pas à celui du plug-ins", "Erreur de chargement", MB_ICONERROR);
    return;
    }
  if (b.IsChunk())
    {
    if (!(pEffect->pEffect->flags & effFlagsProgramChunks))
      {
      MessageBox("La banque est de type chunk,le plug-ins n'accepte pas ca!"
                 ,"Erreur de chargement", MB_ICONERROR);
      return;
      }
    pEffect->EffSetChunk(b.GetChunk(),b.GetChunkSize(), b.IsPreset());
    }
  else
    {
    int cProg = pEffect->EffGetProgram();
    int i, j;
    int nParms = b.GetNumParams();
    for (i = 0; i < b.GetNumPrograms(); i++)
      {
      pEffect->EffSetProgram(i);
      pEffect->EffSetProgramName(b.GetProgramName(i));
      for (j = 0; j < nParms; j++)
        pEffect->EffSetParameter(j, b.GetProgParm(i, j));
      
      }
    pEffect->EffSetProgram(cProg);
    }
  pEffect->SetChunkFile(dlg.GetPathName());
  OnUpdate();
  }
else
  MessageBox("Impossible de charger la banque", "Erreur de chargement", MB_ICONERROR);
}

void CMainDlg::OnUpdate()
{
  if(!pActiv) return;

  if(pActiv ==  APP->pEffEditDlg)
     APP->pEffEditDlg->Update();
  else if(pActiv ==  APP->pEffParmDlg)
     APP->pEffParmDlg->Update();
  else if(pActiv == APP->pControleur)
     APP->pControleur->Update();

  //case APP->pChain    : APP->pChain->Up
  
}

void CMainDlg::OnFxbLoadfxp()
{
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbef);
if(!pEffect)return;
CFileDialog dlg(TRUE,"fxp", pEffect->GetChunkFilePreset(),OFN_ENABLESIZING | OFN_NOCHANGEDIR,
                "Effect Preset Files (.fxp)|*.fxp|All Files|*.*||");
if (dlg.DoModal() != IDOK)
  return;

CFxBank b((char *)(LPCSTR)dlg.GetPathName());
if (b.IsLoaded())
  {
  if (!pEffect)
    return;
  if (pEffect->pEffect->uniqueID != b.GetFxID())
    {
    MessageBox("L'id de la banque ne correspond pas à celui du plug-ins", "Erreur de chargement", MB_ICONERROR);
    return;
    }
  if (b.IsChunk())
    {
    if (!(pEffect->pEffect->flags & effFlagsProgramChunks))
      {
      MessageBox("La banque est de type chunk,le plug-ins n'accepte pas ca!"
                 ,"Erreur de chargement", MB_ICONERROR);
      return;
      }
    pEffect->EffSetChunk(b.GetChunk(),b.GetChunkSize(), b.IsPreset());
    }
  else
    {
    int i = 0, j;
    int nParms = b.GetNumParams();
    //pEffect->EffSetProgram(i);
    //pEffect->EffSetProgramName(b.GetProgramName(i));
    for (j = 0; j < nParms; j++)
      pEffect->EffSetParameter(j, b.GetProgParm(i, j));
    
    }

  pEffect->EffSetProgramName(b.GetProgramName(0));
  pEffect->SetChunkFilePreset(dlg.GetPathName());
  OnUpdate();
  }
else
  MessageBox("Impossible de charger la banque", "Erreur de chargement", MB_ICONERROR);
}

void CMainDlg::OnFxbSavefxp()
{
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbef);
if (!pEffect)
  return;

CFileDialog dlg(FALSE,"fxp",pEffect->GetChunkFilePreset(),OFN_CREATEPROMPT | OFN_ENABLESIZING |
                    OFN_NOCHANGEDIR | OFN_NOREADONLYRETURN |
                    OFN_OVERWRITEPROMPT,"Effect Bank Files (.fxp)|*.fxp|All Files|*.*||");
if (dlg.DoModal() == IDOK)
  SavePreset(dlg.GetPathName());
}



void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
  CDialog::OnSize(nType, cx, cy);
  if(APP)
    APP->editor->frame->setSize(cx,cy);
  Invalidate();
  UpdateWindow();
}

void CMainDlg::OnBnDoubleclickedButton3()
{
  
  //OpenEffectTxT(APP->current_chaine,nbeff);  
}

void CMainDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
  if((pScrollBar == (CScrollBar*)&m_sld)
    && (nSBCode != TB_THUMBTRACK))
  {
    UpdateData();
    if(APP->current_chaine != m_sldchaine)
      APP->effect->setParameterAutomated(0,NBChaine2float(m_sldchaine));
  }
  CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMainDlg::OnBnClickedBtnchmoins()
{

  if(APP->current_chaine > 0)
  {
     //APP->current_chaine--;
     APP->effect->setParameterAutomated(0,NBChaine2float(APP->current_chaine-1));
  }
}

void CMainDlg::OnBnClickedBtnchplus()
{

  if(APP->current_chaine < 127)
  {
   //APP->current_chaine++;
   APP->effect->setParameterAutomated(0,NBChaine2float(APP->current_chaine+1));
  }

}

void CMainDlg::OnBnDoubleclickedBtnchplus()
{
  OnBnClickedBtnchplus();
}

void CMainDlg::OnBnDoubleclickedBtnchmoins()
{
  OnBnClickedBtnchmoins();
}

void CMainDlg::OnBnDoubleclickedBtneffup()
{
  OnBnClickedBtneffup();

}

void CMainDlg::OnBnDoubleclickedBtneffdown()
{
  OnBnClickedBtneffdown();
}





void CMainDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
  CDialog::OnShowWindow(bShow, nStatus);

  // TODO : ajoutez ici le code de votre gestionnaire de messages
}

void CMainDlg::OnBnDoubleclickedButton2()
{
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
}




//void CMainDlg::OnEffectsShellplug()
//{
//  // TODO : ajoutez ici le code de votre gestionnaire de commande
//}
