//
//

#include "stdafx.h"
#include "multifxVST.h"
#include "MainDlg.h"
#include "multifxVSTEditor.h"
#include "stockeffet.h"
#include "CCVSThost.h"
#include "ChainDlg.h"
#include "vsthost/SmpEffect.h"
#include "effectwnd.h"
#include "EffectTxTDlg.h"
#include "Chaindlg.h"
#include "controleurdlg.h"
#include "aboutdlg.h"
#include ".\maindlg.h"

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
  , m_txteff(_T(""))
{
    APP            = 0;
    pActiv         = NULL;
    nbeffstk       = -1;
    nbeff           = -1;
}

void CMainDlg::ChangeChaine(int chaine,BOOL paramAutom)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  if(!VCH(chaine))return ;

  //on affiche la nouvelle chaine
  APP->chaine_eff->ViewChaine(APP->current_chaine,APP->pChain->m_listvst,0);

  UpdateData();
  //update le splider
  m_sldchaine = APP->current_chaine;
  //update le txt
//  m_txtchaine.Format("%d", m_sldchaine);
  UpdateData(FALSE);
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  DDX_Slider(pDX, IDC_SLDCHAINE, m_sldchaine);
  DDX_Control(pDX, IDC_SLDCHAINE, m_sld);
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
  DDX_Control(pDX, IDC_BTNABOUT, m_btnabout);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
  ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
  ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
  ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
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
  ON_BN_CLICKED(IDC_BTNCHMOINS, OnBnClickedBtnchmoins)
  ON_BN_CLICKED(IDC_BTNCHPLUS, OnBnClickedBtnchplus)
  ON_BN_DOUBLECLICKED(IDC_BTNCHPLUS, OnBnDoubleclickedBtnchplus)
  ON_BN_DOUBLECLICKED(IDC_BTNCHMOINS, OnBnDoubleclickedBtnchmoins)
  ON_BN_DOUBLECLICKED(IDC_BTNEFFUP, OnBnDoubleclickedBtneffup)
  ON_BN_DOUBLECLICKED(IDC_BTNEFFDOWN, OnBnDoubleclickedBtneffdown)
  ON_WM_SHOWWINDOW()
  ON_BN_CLICKED(IDC_BTNEFFECTTXT, OnBnClickedBtneffecttxt)
  ON_BN_CLICKED(IDC_BTNBYPASS, OnBnClickedBtnbypass)
  ON_BN_CLICKED(IDC_BTNABOUT, OnBnClickedBtnabout)
END_MESSAGE_MAP()


void CMainDlg::OnSetProgram(UINT nID)
{
CEffect *pEffect = APP->host->GetAt(nbeff);
if (pEffect)
  {
    pEffect->EffSetProgram(nID - IDM_EFF_PROGRAM_0);

    //rafraichi les plug-ins (affichage)
    OnUpdate();
  }
  //pas oublier le update
}

//place la fenetre a sa place,redimentionne decoupe cisaille tronconne etc...
static const OFFSETY =64;
void CMainDlg::ChildNotify(CWnd * child)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  //bool Alwaisresize = true/*(pActiv == child)*/  ;
  
  int posx=0,posy=OFFSETY;

  CloseAll();

  if(!child)return;

  ::CRect r;
  child->GetClientRect(&r);

  WinRect.bottom = WinRect.top  + OFFSETY + r.Height(); 
  WinRect.right  = WinRect.left + r.Width();

  if(WinRect.Width() < WinRectInit.Width())
    WinRect.right = WinRect.left  + WinRectInit.Width(); 
  APP->editor->frame->setSize(WinRect.Width(),WinRect.Height());
  SetWindowPos(NULL,0,0,WinRect.Width(),WinRect.Height(),SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW);
  
  posx =  (WinRect.Width() - r.Width()) >> 1;
  posy =  OFFSETY + (((WinRect.Height()- OFFSETY) - r.Height()) >> 1 );

  //positionne la fenetre enfant
  child->SetWindowPos(NULL,posx,posy ,0,0,SWP_NOSIZE|SWP_NOOWNERZORDER|SWP_NOZORDER|SWP_NOREDRAW);

  child->ShowWindow(SW_SHOW);
 
  //pointeur sur la fenetre active
  pActiv = child;

  UpdateBouton(GetActiveWindows());
}


//Implementation VST, idle pour l'affichage du plug-ins
void CMainDlg::EnterIdle()
{
  //IDLE DE L'host VST pour laffichage du plug enfant
  if(APP->pEffEditDlg)
    APP->pEffEditDlg->EnterIdle();
}

void CMainDlg::SetEffect(int nbeffstk)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  //if(nb == nbeff)return;
  UpdateData();
  
  if((nbeffstk < 0) || (APP->chaine_eff->get_count(APP->current_chaine)<=0))
  {
    this->nbeffstk = -1;//un nb peu correspondr a un effet a t = 0 , et a un autre a t = +!!
    nbeff  = -1;
    m_txteff = "";
  }
  else
  {
    this->nbeffstk = nbeffstk;//un nb peu correspondr a un effet a t = 0 , et a un autre a t = +!!
    nbeff  = APP->chaine_eff->get_effect(APP->current_chaine,nbeffstk);
    m_txteff.Format("%d:%s",nbeffstk+1,APP->chaine_eff->Get_Name(APP->current_chaine,nbeffstk));
  }
  UpdateData(FALSE);
}

void CMainDlg::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

  //On crée la fenetre chain
  APP->pChain = new CChainDlg(this);
  ASSERT(APP->pChain);
  BOOL b = APP->pChain->Create((UINT)CChainDlg::IDD ,this);
  ASSERT(b);

  //On crée la fenetre controleur
  APP->pControleur = new CControleurDlg(this);
  ASSERT(APP->pControleur);
  b = APP->pControleur->Create((UINT)CControleurDlg::IDD,this);
  ASSERT(b);

  //On crée la fenetre about
  APP->pAboutDlg = new CAboutDlg(this);
  ASSERT(APP->pAboutDlg);
  b = APP->pAboutDlg->Create((UINT)CAboutDlg::IDD,this);
  ASSERT(b);

  APP->pControleur->SetAPP(APP);
  APP->pChain->SetAPP(APP);
  APP->pAboutDlg->SetAPP(APP);

  ChangeChaine(float2NBChaine( APP->effect->getParameter(0)),FALSE);

  //APP->pControleur->Update();

  APP->pChain->ShowWindow(SW_SHOW);

  ChildNotify(APP->pChain);
}

//initialise les skins
void CMainDlg::InitialiseSkin()
{
  LoadBitmap(IDB_MAIN);

  m_btnchaine.LoadBitmap(IDB_BTN_CHAIN,true);
  CString text=_T("Gestion des chaines");
  m_btnchaine.SetToolTipText(&text);


  m_btncontroleur.LoadBitmap(IDB_BTN_CONTROLER,true);
  text=_T("Gestion des controleurs");
  m_btncontroleur.SetToolTipText(&text);


  m_btneff.LoadBitmap(IDB_BTN_EFF,true);
  text=_T("Affiche l'effet");
  m_btneff.SetToolTipText(&text);


  m_btneffect2.LoadBitmap(IDB_BTN_EFF2,true);
  text=_T("Affiche l'effet en mode texte");
  m_btneffect2.SetToolTipText(&text);


  m_btnabout.LoadBitmap(IDB_BTN_ABOUT,true);
  text=_T("Affiche la fenetre à propos");
  m_btnabout.SetToolTipText(&text);


  m_btnbypass.LoadBitmap(IDB_BYPASS,true,7,27);
  text=_T("Bypass un effet");
  m_btnbypass.SetToolTipText(&text);

  m_btneffup.LoadBitmap(IDB_LEFT_ARROW_EFFECT,false,112,28);
  text=_T("Sélectionne l'effet suivant dans la chaine actuelle");
  m_btneffup.SetToolTipText(&text);

  m_btneffdown.LoadBitmap(IDB_RIGHT_ARROW_EFFECT,false,272,28);
  text=_T("Sélectionne l'effet précédent dans la chaine actuelle");
  m_btneffdown.SetToolTipText(&text);

  m_btnchplus.LoadBitmap(IDB_CHAIN_RIGHT,false,143,6);
  text=_T("Sélectionne la chaine suivante");
  m_btnchplus.SetToolTipText(&text);

  m_btnchmoins.LoadBitmap(IDB_CHAIN_LEFT,false,104,6);
  text=_T("Sélectionne la chaine précédente");
  m_btnchmoins.SetToolTipText(&text);

  m_btnpreset.LoadBitmap(IDB_PRESET);
  text=_T("Configuration de l'effet");
  m_btnpreset.SetToolTipText(&text);


  //on initialise le slider de chaine
  m_sld.SetRange(0,127);
  m_sld.SetPageSize(10);
  m_sld.SetLineSize(1);
  //m_sld.SetTic(20);
  m_sld.SetTicFreq(10);
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

  if(APP->pAboutDlg)
    APP->pAboutDlg->ShowWindow(SW_HIDE);

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

  if(APP->pAboutDlg)
  {
    APP->pAboutDlg->ShowWindow(SW_HIDE);
    APP->pAboutDlg->DestroyWindow();
    APP->pAboutDlg = NULL;
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

  //on initialise les skins
  InitialiseSkin();

  //on récupere la taille des fenetres
	GetWindowRect(&WinRect);
	GetWindowRect(&WinRectInit);
  return TRUE;  // return TRUE unless you set the focus to a control
  // EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

int CMainDlg::GetActiveWindows()
{

  int nbbtn = -1;
  if(pActiv == APP->pAboutDlg)
    nbbtn = WD_ABOUT;
  else if(pActiv == APP->pChain)
    nbbtn = WD_CHAIN;
  else if(pActiv == APP->pControleur)
    nbbtn = WD_CONTROLER;
  else if(pActiv == APP->pEffEditDlg)
    nbbtn = WD_EFFET;
  else if(pActiv == APP->pEffParmDlg)
    nbbtn = WD_EFFET2;

  return nbbtn;
}
//n
void CMainDlg::OpenEffect(int chaine,int nbeffectstk)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if(nbeffectstk < 0)
  {
    UpdateBouton(GetActiveWindows());
    return;
  }

  if(APP->pEffEditDlg)
  {
    //c'est deja cette effect...on réactive la fenetre et tayo
    if(APP->chaine_eff->get_effect(chaine , nbeffectstk)== APP->pEffEditDlg->nEffect )
    {
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
    pWnd->CreateEx( WS_EX_TOPMOST, strMyClass,"Effect", WS_VISIBLE | WS_CHILD ,::CRect(0, 0, 0, 0),this,0);
 
    if (pWnd)
      {
      pWnd->SetEffect(nEffect);
      APP->pEffEditDlg = pWnd;

      ::CRect r;
      pWnd->GetWindowRect(&r);

      ((CSmpEffect *)pEffect)->SetEditWnd(pWnd);
      lResult = APP->host->EffEditOpen(nEffect, pWnd->GetSafeHwnd());
      long lResult = APP->host->EffEditGetRect(nEffect, &prc);
      pWnd->SetEffSize(prc);

      ChildNotify(pWnd);
      }
  }
  else  //pas d'interface graphique!
  {
    OpenEffectTxT(chaine,nbeffectstk);
  }
}

void CMainDlg::OpenEffectTxT(int chaine,int nbeffectstk)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

  if(nbeffectstk < 0)
  {
    UpdateBouton(GetActiveWindows());
    return;
  }


  
  if(APP->pEffParmDlg)
  {
    if(APP->chaine_eff->get_effect(APP->current_chaine ,nbeffectstk) == APP->pEffParmDlg->nEffect )
    {
      //mm effet on réactive et tayo
      ChildNotify(APP->pEffParmDlg);
      return;
    }
  }
  KillEffect(2);

  {
  int nEffect = APP->chaine_eff->get_effect(APP->current_chaine,nbeffectstk);
  CEffect *pEffect = APP->host->GetAt(nEffect);


  CEffectTxTDlg  *pWnd = (CEffectTxTDlg *) new CEffectTxTDlg(this,APP);
  if(!pWnd->Create(this))
  {
    TRACE("ERREUR EFFEDITDLG->CREATEINDIRECT (OnbtnE2)");
    return;
  }

  if (pWnd)
    {
    pWnd->SetEffect(nbeffectstk);
    APP->pEffParmDlg = pWnd;
    ::CRect r;
    pWnd->GetWindowRect(&r);
    //place la fentre au bon endroit
    ChildNotify(pWnd);
    }
  }
  
}

void CMainDlg::OnBnClickedButton1()
{
  ChildNotify(APP->pChain);
}

void CMainDlg::OnBnClickedButton2()
{
  ChildNotify(APP->pControleur);
}

void CMainDlg::OnBnClickedButton3()
{
  OpenEffect(APP->current_chaine,nbeffstk);  
}

void CMainDlg::OnBnClickedBtneffecttxt()
{
  OpenEffectTxT(APP->current_chaine,nbeffstk); 
}

void CMainDlg::PostNcDestroy()
{
  CDialog::PostNcDestroy();
  APP->pMainDlg = NULL;
  delete this;
}

static CBrush brush(RGB(110,220,120));
static CBrush brush2(RGB(221,221,221));
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

//on detruit la fenetre => on detruit les fenetres child
BOOL CMainDlg::DestroyWindow()
{
  KillAll(); 
  return CDialog::DestroyWindow();
}

// Load a bitmap
BOOL CMainDlg::LoadBitmap(UINT bitmapid)
{
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP	bitmapbits;
	mybitmap.GetBitmap(&bitmapbits);
	return TRUE;
}

BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
{
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC);
  
  ::CBitmap * pOldBitmap;
	pOldBitmap = pMemDC -> SelectObject(&mybitmap);
	
  BITMAP	bitmapbits;
	mybitmap.GetBitmap(&bitmapbits);

	//pDC->SetStretchBltMode(HALFTONE);
  pDC->BitBlt(0,0,bitmapbits.bmWidth,bitmapbits.bmHeight,pMemDC,0,0,SRCCOPY);

	// clean up
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;

  //dessine la fin de la fenetre si elle est plus grande que la bitmap
  if(WinRect.Width() > bitmapbits.bmWidth)
  {
    pDC->FillSolidRect(bitmapbits.bmWidth,0,WinRect.Width()-bitmapbits.bmWidth,27,RGB(247,150,16));
    pDC->FillSolidRect(bitmapbits.bmWidth,27,WinRect.Width()-bitmapbits.bmWidth,18,RGB(231,199,74));
    pDC->FillSolidRect(bitmapbits.bmWidth,45,WinRect.Width()-bitmapbits.bmWidth,19,RGB(107,150,173));
  }

  pDC->FillSolidRect(0,OFFSETY,WinRect.Width(),WinRect.Height()-OFFSETY,RGB(175,10,14));
  return TRUE;
}

//on change d'effet
void CMainDlg::OnBnClickedBtneffup()
{
  if(nbeffstk < 1)return;
  nbeffstk--;

  //met a jour la list d'effet (la selection)
  //la selection => met a jour le text dans main
  APP->pChain->m_listvst.EnsureVisible(nbeffstk,TRUE);
  APP->pChain->m_listvst.SetItemState(nbeffstk,LVIS_SELECTED,LVIS_SELECTED);
  
  if(APP->pEffEditDlg == pActiv || APP->pEffParmDlg == pActiv)
    OpenEffect(APP->current_chaine,nbeffstk);

  //else on fait rien car,tt est deja fait!
}

void CMainDlg::OnBnClickedBtneffdown()
{
  if(nbeffstk >= APP->chaine_eff->get_count(APP->current_chaine)-1)return;
  nbeffstk++;

  //met a jour la list d'effet (la selection)
  //la selection => met a jour le text dans main
  APP->pChain->m_listvst.EnsureVisible(nbeffstk,TRUE);
  APP->pChain->m_listvst.SetItemState(nbeffstk,LVIS_SELECTED,LVIS_SELECTED);
  //SetEffect(nbeff);

  if(APP->pEffEditDlg == pActiv || APP->pEffParmDlg == pActiv)
    OpenEffect(APP->current_chaine,nbeffstk);
}

// on remplit un popup avec chaque program d'un effet
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

// initialise le popup program
void CMainDlg::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
  CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);

  if ((pPopupMenu->GetMenuItemCount() > 0) &&
    (pPopupMenu->GetMenuItemID(pPopupMenu->GetMenuItemCount()-1) == IDM_EFF_PROGRAM_0))
  if(nbeff>=0)
    FillPopup(pPopupMenu,nbeff);
}


//ajoute un effet VST
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
}

void CMainDlg::SaveBank(CString sName)
{
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbeff);
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
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbeff);
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
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbeff);
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
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbeff);
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

  ChangeChaine(APP->current_chaine);
  SetEffect(0);

  if(pActiv == APP->pControleur ||pActiv == APP->pAboutDlg )
  {

  }
  //pas d'effete a ouvrir donc on remet la fenetre main
  else if((APP->chaine_eff->get_count(APP->current_chaine) == 0) && (pActiv != (CWnd *)APP->pChain) )
  {
      APP->pMainDlg->ChildNotify(APP->pChain);
  }
  else
  {
    if(APP->pMainDlg->pActiv == (CWnd *)APP->pEffEditDlg)
    { 
      OpenEffect(APP->current_chaine,0);
    }
    else if (pActiv == (CWnd *)APP->pEffParmDlg)
    {
      OpenEffectTxT(APP->current_chaine,0);
    }
  }

/*
  if(pActiv ==  APP->pEffEditDlg)
     APP->pEffEditDlg->Update();
  else if(pActiv ==  APP->pEffParmDlg)
     APP->pEffParmDlg->Update();
  else if(pActiv == APP->pControleur)
     APP->pControleur->Update();  */
}

void CMainDlg::OnFxbLoadfxp()
{
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbeff);
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
CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nbeff);
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

//chaine précedente
void CMainDlg::OnBnClickedBtnchmoins()
{
  if(APP->current_chaine > 0)
     APP->effect->setParameterAutomated(0,NBChaine2float(APP->current_chaine-1));
}

//chaine suivante
void CMainDlg::OnBnClickedBtnchplus()
{
  if(APP->current_chaine < 127)
   APP->effect->setParameterAutomated(0,NBChaine2float(APP->current_chaine+1));
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


//met a jour les boutons de chaines
void CMainDlg::UpdateBouton(int btn)
{
  switch(btn)
  {
  case WD_ABOUT:
    m_btnabout.SetValue(true);
    m_btncontroleur.SetValue(false);
    m_btnchaine.SetValue(false);
    m_btneffect2.SetValue(false);
    m_btneff.SetValue(false);
    break;
  case WD_CHAIN:
    m_btnchaine.SetValue(true);
    m_btncontroleur.SetValue(false);
    m_btneffect2.SetValue(false);
    m_btneff.SetValue(false);
    m_btnabout.SetValue(false);
    break;
  case WD_CONTROLER:
    m_btncontroleur.SetValue(true);
    m_btnchaine.SetValue(false);
    m_btneffect2.SetValue(false);
    m_btneff.SetValue(false);
    m_btnabout.SetValue(false);
    break;
  case WD_EFFET:
    m_btneff.SetValue(true);
    m_btncontroleur.SetValue(false);
    m_btnchaine.SetValue(false);
    m_btneffect2.SetValue(false);
    m_btnabout.SetValue(false);
    break;
  case WD_EFFET2:
    m_btneffect2.SetValue(true);
    m_btncontroleur.SetValue(false);
    m_btnchaine.SetValue(false);
    m_btneff.SetValue(false);
    m_btnabout.SetValue(false);
    break;
  default:
    m_btneffect2.SetValue(false);
    m_btncontroleur.SetValue(false);
    m_btnchaine.SetValue(false);
    m_btneff.SetValue(false);
    m_btnabout.SetValue(false);
  }

}

void CMainDlg::OnBnClickedBtnbypass()
{
  bool bypass = APP->chaine_eff->GetByPass(APP->current_chaine,nbeffstk);
  APP->chaine_eff->SetByPass(APP->current_chaine,nbeffstk,!bypass);
}

//void CMainDlg::OnBnClickedBtneffecttxt2()
//{
//  //ViewWindow(
//}

void CMainDlg::OnBnClickedBtnabout()
{
  ChildNotify(APP->pAboutDlg);
  
}
