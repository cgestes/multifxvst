// ChainDlg.cpp : fichier d'implémentation
//

#include "stdafx.h"
//#include "multifxVST.h"
#include "multifxVSTmain.h"
#include "ChainDlg.h"
#include "CCVSThost.h"
#include "stockeffet.h"
#include "multifxVSTEditor.h"

//#include "vsthost/CVSThost.h"
#include "vsthost/SmpEffect.h"
#include "effectwnd.h"
#include "EffectTxTDlg.h"
#include "multifxVST.h"
#include "maindlg.h"
#include ".\chaindlg.h"
#include "Controleurlst.h"

extern class CChainApp theApp;

// Boîte de dialogue CChainDlg

IMPLEMENT_DYNAMIC(CChainDlg, CDialog);

CChainDlg::CChainDlg( CWnd* pParent)
:CDialog(CChainDlg::IDD, pParent)
, m_copied(-1)
, m_used(0)
{

  APP = NULL;
  TRACE("CREATE::CChainDlg\n");
}

// Load a bitmap
BOOL CChainDlg::LoadBitmap(UINT bitmapid)
{
	mybitmap.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(bitmapid), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	BITMAP	bitmapbits;
	mybitmap.GetBitmap(&bitmapbits);
	return TRUE;
}

void CChainDlg::OnUpdate(int nb)
{
  APP->chaine_eff->ViewChaine(APP->current_chaine,m_listvst,nb);
  UpdateData();
    m_used = APP->chaine_eff->nb_effect_used;
  UpdateData(FALSE);
}

void CChainDlg::SetAPP(CAppPointer * m_APP)
{
  APP = m_APP;
}


CChainDlg::~CChainDlg()
{
	//AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CString buf;buf.Format("DESTROY :: CChainDlg(%d) \n", this);  TRACE(buf);
}



void CChainDlg::DoDataExchange(CDataExchange* pDX)
{
  CDialog::DoDataExchange(pDX);
  //DDX_Slider(pDX, IDC_SLDCHAINE, m_slide);

  DDX_Control(pDX, IDC_BTNUP, m_btnUP);
  DDX_Control(pDX, IDC_BTNDOWN, m_btnDown);
  DDX_Control(pDX, IDC_BTNADD, m_btnAdd);
  DDX_Control(pDX, IDC_BTNSUPPR, m_btnSuppr);
  DDX_Control(pDX, IDC_BTNSAVE, m_btnSaveAll);
  DDX_Control(pDX, IDC_BTNLOAD, m_btnLoadAll);
  DDX_Control(pDX, IDC_BTNSAVE2, m_btnSavechaine);
  DDX_Control(pDX, IDC_BTNLOAD2, m_btnLoadchaine);
  DDX_Control(pDX, IDC_LISTVST, m_listvst);
  DDX_Text(pDX, IDC_TXTCOPIED, m_copied);
  DDX_Control(pDX, IDC_BTNCLRCHAIN, m_btnclear);
  DDX_Control(pDX, IDC_BTNCOPIETO, m_btncopy);
  DDX_Control(pDX, IDC_BTNPASTETO, m_btnpaste);
  DDX_Text(pDX, IDC_TXTUSED, m_used);
}

#define ON_WM_LBUTTONDOWN_MOD() {WM_LBUTTONDOWN,0,0,0,AfxSig_vwp,(AFX_PMSG)(AFX_PMSGW)(static_cast< void(AFX_MSG_CALL CWnd::*)(UINT, ::CPoint)>(OnLButtonDown))},
#define ON_WM_LBUTTONUP_MOD() {WM_LBUTTONUP,0,0,0,AfxSig_vwp,(AFX_PMSG)(AFX_PMSGW)(static_cast< void(AFX_MSG_CALL CWnd::*)(UINT, ::CPoint)>(OnLButtonUp))},
#define ON_WM_MOUSEMOVE_MOD() {WM_MOUSEMOVE,0,0,0,AfxSig_vwp,(AFX_PMSG)(AFX_PMSGW)(static_cast< void(AFX_MSG_CALL CWnd::*)(UINT, ::CPoint)>(OnMouseMove))},


BEGIN_MESSAGE_MAP(CChainDlg, CDialog)
  ON_BN_CLICKED(IDC_BTNADD, OnBnClickedBtnadd)
  ON_BN_CLICKED(IDC_BTNUP, OnBnClickedBtnup)
  ON_BN_CLICKED(IDC_BTNDOWN, OnBnClickedBtndown)
  ON_BN_CLICKED(IDC_BTNSAVE, OnBnClickedBtnsave)
  ON_BN_CLICKED(IDC_BTNLOAD, OnBnClickedBtnload)
  ON_BN_CLICKED(IDC_BTNSUPPR, OnBnClickedBtnsuppr)
  ON_BN_CLICKED(IDC_BTNSAVE2, OnBnClickedBtnsave2)
  ON_BN_CLICKED(IDC_BTNLOAD2, OnBnClickedBtnload2)
  ON_WM_CTLCOLOR()
/*
	ON_WM_LBUTTONDOWN_MOD()
  ON_WM_LBUTTONUP_MOD()
	ON_WM_MOUSEMOVE_MOD()*/
//  ON_NOTIFY(LVN_ODSTATECHANGED, IDC_LISTVST, OnLvnOdstatechangedListvst)
  ON_NOTIFY(LVN_ITEMCHANGED, IDC_LISTVST, OnLvnItemchangedListvst)
  ON_NOTIFY(NM_DBLCLK, IDC_LISTVST, OnNMDblclkListvst)
//  ON_BN_CLICKED(IDC_BTNSUPPR4, OnBnClickedBtnsuppr4)
ON_BN_CLICKED(IDC_BTNCLRCHAIN, OnBnClickedBtnclrchain)
ON_BN_CLICKED(IDC_BTNCOPIETO, OnBnClickedBtncopieto)
ON_BN_CLICKED(IDC_BTNPASTETO, OnBnClickedBtnpasteto)
//ON_STN_CLICKED(IDC_TXTINFOCTAF, OnStnClickedTxtinfoctaf)
ON_COMMAND(ID_EFFECTS_BROWSE, OnEffectsBrowse)
ON_COMMAND(ID_EFFECTS_SHELLPLUG, OnEffectsShellplug)
ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// Gestionnaires de messages CChainDlg

void CChainDlg::PostNcDestroy()
{
	CDialog::PostNcDestroy();
  APP->pChain  = NULL;
	delete this;
  CString buf;buf.Format("PostNcDestroy :: CChainDlg(%d) \n", this);  TRACE(buf);
}

//ajoute un plug-ins a la fin de current_chaine
void CChainDlg::OnBnClickedBtnadd()
{
  ::CRect r;
  GetDlgItem(IDC_BTNADD)->GetWindowRect(&r);


    if (APP->mnu == NULL)
         return;

    CMenu  * mnu = APP->mnu->GetSubMenu(0);
    if(mnu)
      mnu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, r.left , r.bottom , this);
}



//charge un plug-ins
void CChainDlg::LoadEffect(LPCSTR dllpath)
{
  AFX_MANAGE_STATE(AfxGetStaticModuleState());
  int nb  = APP->chaine_eff->add_eff(APP->current_chaine,dllpath);
  if(nb == -1)
  {
    AfxMessageBox("Impossible d'ajouter le plug-ins");
    return;
  }

  OnUpdate(nb);
}







//A


BOOL CChainDlg::OnInitDialog()
{
  CDialog::OnInitDialog();

  CString buf;
  buf.Format("INITDIALOG :: CChainDlg(%d) \n", this);
  TRACE(buf);
  
  InitialiseSkin();

  return TRUE;  // return TRUE unless you set the focus to a control
}

void CChainDlg::InitialiseSkin()
{
  LoadBitmap(IDB_CHAIN);
 //INITIALISATION DES BOUTONS
  m_btnUP.LoadBitmap(IDB_CHAIN_UP,false,431,18);
  CString text=_T("Fait monter l'effet sélectionné");
  m_btnUP.SetToolTipText(&text);

  m_btnDown.LoadBitmap(IDB_CHAIN_DOWN,false,431,41);
  text=_T("Fait descendre l'effet sélectionné");
  m_btnDown.SetToolTipText(&text);

  m_btnAdd.LoadBitmap(IDB_CHAIN_ADD,false,431,64);
  text=_T("Ajoute un effet à la chaine");
  m_btnAdd.SetToolTipText(&text);

  m_btnSuppr.LoadBitmap(IDB_CHAIN_DEL,false,431,87);
  text=_T("Supprime un effet de la chaine");
  m_btnSuppr.SetToolTipText(&text);

  m_btnSaveAll.LoadBitmap(IDB_CHAIN_SAVEALL,false,449,203);
  text=_T("Sauvegarde toutes les chaines");
  m_btnSaveAll.SetToolTipText(&text);

  m_btnLoadAll.LoadBitmap(IDB_CHAIN_LOADALL,false,449,180);
  text=_T("Charge toutes les chaines");
  m_btnLoadAll.SetToolTipText(&text);

  m_btnSavechaine.LoadBitmap(IDB_CHAIN_SAVE,false,449,157);
  text=_T("Sauvegarde une chaine");
  m_btnSavechaine.SetToolTipText(&text);

  m_btnLoadchaine.LoadBitmap(IDB_CHAIN_LOAD,false,449,134);
  text=_T("Charge une chaine");
  m_btnLoadchaine.SetToolTipText(&text);

  m_btnpaste.LoadBitmap(IDB_CHAIN_PASTE,false,449,279);
  text=_T("Colle une chaine");
  m_btnpaste.SetToolTipText(&text);

  m_btnclear.LoadBitmap(IDB_CHAIN_CLEAR,false,449,233);
  text=_T("Efface une chaine");
  m_btnclear.SetToolTipText(&text);

  m_btncopy.LoadBitmap(IDB_CHAIN_COPY,false,449,256);
  text=_T("Copie une chaine");
  m_btncopy.SetToolTipText(&text);

  //INITIALISE LA LISTE
  m_listvst.Init();
	/*m_listvst.g_MyClrFgHi = RGB(35,12,200);
	m_listvst.g_MyClrBgHi = RGB(20,242,0);
  m_listvst.g_MyClrBg   = RGB(130,120,240);*/
	/*m_listvst.SetBkColor(RGB(60,45,20));
	m_listvst.SetTextColor(RGB(52,242,22));*/

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = 12;
  //lf.lfWidth  = 10;
	strcpy(lf.lfFaceName, "Verdana");
	font.CreateFontIndirect(&lf);
	m_listvst.SetFont(&font, TRUE);

	m_listvst.InsertColumn(0, "Name", LVCFMT_LEFT, 150);
	m_listvst.InsertColumn(1, "NBeffect", LVCFMT_LEFT, 60);
  m_listvst.InsertColumn(2, "NBeffectBis", LVCFMT_LEFT, 60);
	m_listvst.InsertColumn(3, "NBStkeffect", LVCFMT_LEFT, 60);
	m_listvst.InsertColumn(4, "Vide", LVCFMT_LEFT, 60);

	ListView_SetExtendedListViewStyle(m_listvst.m_hWnd, LVS_EX_FULLROWSELECT  | LVS_EX_HEADERDRAGDROP);
	
}


//monte un effet dans current_chaine de 1 cran
void CChainDlg::OnBnClickedBtnup()
{
  int n = m_listvst.GetCurSel();
  if(n<0)return ;
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
  APP->chaine_eff->Up_eff(APP->current_chaine,n);
    OnUpdate(n-1);
}


//descend un effet dans current_chaine de 1 cran
void CChainDlg::OnBnClickedBtndown()
{
  int n = m_listvst.GetCurSel();
  if(n<0)return ;
  APP->chaine_eff->Down_eff(APP->current_chaine,n);
    OnUpdate(n+1);
}

void CChainDlg::OnBnClickedBtnsave()
{
  CString sStartAt;
  sStartAt = AfxGetApp()->GetProfileString("Load", "PathLoadAll");
  if (sStartAt.GetLength())
    sStartAt += "\\*.ctaf.all";

  CFileDialog dlg(FALSE, "ctaf.all", sStartAt,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "Preset Complet (*.ctaf.all)|*.ctaf.all|All Files (*.*)|*.*||");

  if (dlg.DoModal() == IDOK)
  {
    sStartAt = dlg.GetPathName();

    while(sStartAt.Find(".ctaf.all",sStartAt.GetLength() - 9) != -1)
      sStartAt = sStartAt.Mid(0,sStartAt.GetLength() - 9);

    sStartAt += ".ctaf.all";
    SaveAll(sStartAt);
    sStartAt = sStartAt.Left(sStartAt.ReverseFind('\\'));
    AfxGetApp()->WriteProfileString("Load", "PathLoadAll", sStartAt);
  }
}

BOOL CChainDlg::SaveAll(LPCSTR Path)
{
  TRY{
  CFile f(Path,CFile::modeWrite | CFile::modeCreate); 
  CArchive ar(&f,CArchive::store);

    APP->chaine_eff->SaveParamsToMem(APP->current_chaine);
    APP->chaine_eff->save(ar);

  ar.Close();
  f.Close();
  return TRUE;
  }CATCH(CFileException , ex)
  {
    return FALSE;
    TRACE("###erreur de sauvegarde (save)");
  }END_CATCH
}

BOOL CChainDlg::LoadAll(LPCSTR Path)
{
  TRY{
  CFile f(Path,CFile::modeRead); 
  CArchive ar(&f,CArchive::load);

  //on supprime les fenetres
  APP->pMainDlg->KillEffect();
  APP->pMainDlg->SetEffect(-1);
  APP->parameter->DeleteAllItems();

  if(APP->chaine_eff->m_processing)
    APP->chaine_eff->suspend(APP->current_chaine);

  APP->chaine_eff->RemoveAll();


  APP->chaine_eff->load(ar);
  APP->chaine_eff->LoadParamsFromMem(0);
  APP->effect->setParameterAutomated(0,NBChaine2float(0));
  //ChangeChaine(0); /* reactive current_chaine */

  OnUpdate();

  if(APP->chaine_eff->m_processing)
    APP->chaine_eff->resume(0);

  ar.Close();
  f.Close();
  return TRUE;
  }CATCH(CFileException , ex)
  {
    TRACE("###erreur de sauvegarde (load)");
        AfxMessageBox("Erreur de chargement");
    return FALSE;
  }END_CATCH

}

BOOL CChainDlg::SaveChaine(int chaine,LPCSTR Path)
{
  TRY{
  CFile f(Path,CFile::modeWrite | CFile::modeCreate); 
  CArchive ar(&f,CArchive::store);
    APP->chaine_eff->SaveParamsToMem(APP->current_chaine);
    APP->chaine_eff->save_chaine(chaine,ar);
  ar.Close();
  f.Close();
  return TRUE;
  }CATCH(CFileException , ex)
  {
    return FALSE;
    TRACE("###erreur de sauvegarde (save)");
  }END_CATCH
}

BOOL CChainDlg::LoadChaine(int chaine,LPCSTR Path)
{
  TRY{
  CFile f(Path,CFile::modeRead); 
  CArchive ar(&f,CArchive::load);
  
  //on supprime les fenetres
  //close();
  APP->pMainDlg->KillEffect();
  APP->pMainDlg->SetEffect(-1);
  APP->parameter->DeleteAllItems();

  if(APP->chaine_eff->m_processing)
    APP->chaine_eff->suspend(APP->current_chaine);

  APP->chaine_eff->RemoveAt(APP->current_chaine);


  APP->chaine_eff->load_chaine(chaine,ar);
  APP->chaine_eff->LoadParamsFromMem(chaine);
 // ChangeChaine(chaine); /* réactive current_chaine */
  //APP->effect->setParameterAutomated(0,NBChaine2float(chaine));


  OnUpdate();

  if(APP->chaine_eff->m_processing)
    APP->chaine_eff->resume(APP->current_chaine);

  ar.Close();
  f.Close();
  return TRUE;
  }CATCH(CFileException , ex)
  {
    TRACE("###erreur de sauvegarde (load)");
    AfxMessageBox("Erreur de chargement");
    return FALSE;
  }END_CATCH


}

BOOL CChainDlg::SaveParamEffect(int chaine,int nbeffect,LPCSTR Path)
{
  return FALSE;
}

BOOL CChainDlg::LoadParamEffect(int chaine,int nbeffect,LPCSTR Path)
{
  return FALSE;
}

void CChainDlg::OnBnClickedBtnload()
{
CString sStartAt;
sStartAt = AfxGetApp()->GetProfileString("Load", "PathLoadAll");
if (sStartAt.GetLength())
  sStartAt += "\\*.ctaf.all";

CFileDialog dlg(TRUE, "ctaf.all", sStartAt,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "Preset Complet (*.ctaf.all)|*.ctaf.all|All Files (*.*)|*.*||");

if (dlg.DoModal() == IDOK)
  {
  sStartAt = dlg.GetPathName();
  sStartAt = sStartAt.Left(sStartAt.ReverseFind('\\'));
  AfxGetApp()->WriteProfileString("Load", "PathLoadAll", sStartAt);
  LoadAll(dlg.GetPathName());
  }
}

void CChainDlg::OnBnClickedBtnsuppr()
{
  int nb = m_listvst.GetCurSel();
  if(nb < 0)return;
  int nbafft = nb -1;
  if(nbafft < 0) nbafft = 0;

  APP->pMainDlg->KillEffect();
  APP->pMainDlg->SetEffect(-1);

  APP->chaine_eff->suppr_eff(APP->current_chaine,nb);
  OnUpdate(nbafft);

}

void CChainDlg::OnBnClickedBtnsave2()
{
CString sStartAt;
sStartAt = AfxGetApp()->GetProfileString("Load", "PathLoadChaine");
if (sStartAt.GetLength())
  sStartAt += "\\*.ctaf.cha";

CFileDialog dlg(FALSE, "ctaf.cha", sStartAt,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "Preset Complet (*.ctaf.cha)|*.ctaf.cha|All Files (*.*)|*.*||");

if (dlg.DoModal() == IDOK)
  {
  sStartAt = dlg.GetPathName();

  while(sStartAt.Find(".ctaf.cha",sStartAt.GetLength() - 9) != -1)
  {
    sStartAt = sStartAt.Mid(0,sStartAt.GetLength() - 9);
  }

  sStartAt += ".ctaf.cha";


  SaveChaine(APP->current_chaine,sStartAt);

  sStartAt = sStartAt.Left(sStartAt.ReverseFind('\\'));
  AfxGetApp()->WriteProfileString("Load", "PathLoadChaine", sStartAt);

  }
}

/*void CChainDlg::SetNewNumChaine(int newchaine)
{
  if(APP->current_chaine != newchaine)
     this->newchaine = newchaine;
};*/
    
//void CChainDlg::OnUpdateCh()
//{
  //APP->ChangeChaine(APP->current_chaine,FALSE);
//}


void CChainDlg::OnBnClickedBtnload2()
{
CString sStartAt;
sStartAt = AfxGetApp()->GetProfileString("Load", "PathLoadChaine");
if (sStartAt.GetLength())
  sStartAt += "\\*.ctaf.cha";

CFileDialog dlg(TRUE, "ctaf.cha", sStartAt,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "Preset Complet (*.ctaf.cha)|*.ctaf.cha|All Files (*.*)|*.*||");

if (dlg.DoModal() == IDOK)
  {
  sStartAt = dlg.GetPathName();
  sStartAt = sStartAt.Left(sStartAt.ReverseFind('\\'));
  AfxGetApp()->WriteProfileString("Load", "PathLoadChaine", sStartAt);
  LoadChaine(APP->current_chaine,dlg.GetPathName());
  }
}

//static CBrush brush(RGB(230,220,12));
static CBrush brush(RGB(148,174,189));
HBRUSH CChainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	//if((pWnd == this) || (pWnd->GetDlgCtrlID() == IDC_SLDCHAINE) ||(pWnd->GetDlgCtrlID() == IDC_TXTCHAINE))
	{
	pDC->SetBkMode(TRANSPARENT);

	 return brush;
	}
	return hbr;
}

void CChainDlg::OnLvnItemchangedListvst(NMHDR *pNMHDR, LRESULT *pResult)
{
  LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
  //LVIS_SELECTED
  if(pNMLV->uNewState & LVIS_SELECTED)
  {
    int n = pNMLV->iItem;
    APP->pMainDlg->SetEffect(n);
  }
  *pResult = 0;
}

void CChainDlg::OnNMDblclkListvst(NMHDR *pNMHDR, LRESULT *pResult)
{
  int n = m_listvst.GetCurSel();
  if(n<0)return ;
  APP->pMainDlg->OpenEffect(APP->current_chaine,n);
  // TODO : ajoutez ici le code de votre gestionnaire de notification de contrôle
  *pResult = 0;
}

void CChainDlg::OnBnClickedBtnclrchain()
{
  APP->chaine_eff->RemoveAt(APP->current_chaine);
  OnUpdate();
}

void CChainDlg::OnBnClickedBtncopieto()
{
  if(APP->chaine_eff->get_count(APP->current_chaine) <= 0)
    return;

  UpdateData();
    //ts se fait dans paste
    m_copied = APP->current_chaine;
  UpdateData(FALSE);
}

void CChainDlg::OnBnClickedBtnpasteto()
{
  if((m_copied != APP->current_chaine)&&(m_copied != -1))
  {
    APP->chaine_eff->copie_chaine(m_copied,APP->current_chaine);
    OnUpdate();
  }
}

void CChainDlg::OnEffectsBrowse()
{

CString sStartAt;
sStartAt = AfxGetApp()->GetProfileString("Load", "Path");
if (sStartAt.GetLength())
  sStartAt += "\\*.dll";

CFileDialog dlg(TRUE, NULL, sStartAt,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "VST Plug-ins (*.dll)|*.dll|All Files (*.*)|*.*||");

if (dlg.DoModal() == IDOK)
  {
  sStartAt = dlg.GetPathName();
  sStartAt = sStartAt.Left(sStartAt.ReverseFind('\\'));
  AfxGetApp()->WriteProfileString("Load", "Path", sStartAt);
  LoadEffect(dlg.GetPathName());
  }

}

void CChainDlg::OnEffectsShellplug()
{
CString sStartAt;
sStartAt = AfxGetApp()->GetProfileString("Load", "Path");
if (sStartAt.GetLength())
  sStartAt += "\\*.dll";

CFileDialog dlg(TRUE, NULL, sStartAt,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
     "VST Plug-ins (*.dll)|*.dll|All Files (*.*)|*.*||");

if (dlg.DoModal() == IDOK)
  {
  sStartAt = dlg.GetPathName();
  sStartAt = sStartAt.Left(sStartAt.ReverseFind('\\'));
  AfxGetApp()->WriteProfileString("Load", "Path", sStartAt);
  LoadEffect(dlg.GetPathName());
  }
}

BOOL CChainDlg::OnEraseBkgnd(CDC* pDC)
{
  // TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels
	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(pDC);
  
  //CRect r;
	//GetClientRect(&r);



  ::CBitmap * pOldBitmap;
	pOldBitmap = pMemDC -> SelectObject(&mybitmap);
	
  BITMAP	bitmapbits;
	mybitmap.GetBitmap(&bitmapbits);

	//CPoint point(0,0);	
	//pDC->SetStretchBltMode(HALFTONE);
  pDC->BitBlt(0,0,bitmapbits.bmWidth/* getWidth()*/,bitmapbits.bmHeight,pMemDC,0,0,SRCCOPY);


	// clean up
	pMemDC -> SelectObject(pOldBitmap);
	delete pMemDC;

  /*if(WinRect.Width() > bitmapbits.bmWidth)
  {
    pDC->FillSolidRect(bitmapbits.bmWidth,0,WinRect.Width()-bitmapbits.bmWidth,27,RGB(255,153,0));
    pDC->FillSolidRect(bitmapbits.bmWidth,27,WinRect.Width()-bitmapbits.bmWidth,18,RGB(239,205,69));
    pDC->FillSolidRect(bitmapbits.bmWidth,45,WinRect.Width()-bitmapbits.bmWidth,19,RGB(113,153,175));
  }
  pDC->FillSolidRect(0,OFFSETY,WinRect.Width(),WinRect.Height()-OFFSETY,RGB(204,204,255));*/
  return TRUE;

}
