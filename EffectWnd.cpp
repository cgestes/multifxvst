/*****************************************************************************/
/* EffectWnd.cpp : implementation file                                       */
/*****************************************************************************/

#include "stdafx.h"
//#include "multifxVST.h" //deja inclus ds effect...
#include "multifxVST.h"
#include "multifxVSTmain.h"
//#include "childfrm.h"
#include "CCVSThost.h"
#include "MAinDlg.h"

#include "vsthost/smpvsthost.h"


#include "EffectWnd.h"
#include ".\effectwnd.h"


//#include "prognamedlg.h"
//#include "vsthost/cvsthost.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern class CGratOmaTicApp theApp; 
#define GetApp()  (&theApp)/*((CGratOmaTicApp *)AfxGetApp())*/

/*===========================================================================*/
/* CEffectWnd class members                                                  */
/*===========================================================================*/

IMPLEMENT_DYNCREATE(CEffectWnd, CDialog)

/*****************************************************************************/
/* CEffectWnd : constructor                                                  */
/*****************************************************************************/
void CEffectWnd::PostNcDestroy()
{

	// TODO : ajoutez ici votre code spécialisé et/ou l'appel de la classe de base
	CWnd::PostNcDestroy();
  m_hWnd = NULL;
  APP->pEffEditDlg = NULL;
	delete this;
  CString buf;buf.Format("PostNcDestroy :: CEffectWnd(%d) \n", this);  TRACE(buf);
}

CEffectWnd::CEffectWnd(CAppPointer * m_host)
{
  TRACE("CEffectWnd () \n");
  APP = m_host;
  //host = m_host;
  /*pMain = NULL;
  pView = NULL;*/
  nEffect = -1;
  rcEffFrame.SetRectEmpty();
  rcEffClient.SetRectEmpty();
}

/*****************************************************************************/
/* ~CEffectWnd : destructor                                                  */
/*****************************************************************************/

CEffectWnd::~CEffectWnd()
{
  CString buf;buf.Format("DESTROY :: CEffectWnd(%d) \n", this);  TRACE(buf);
}

/*****************************************************************************/
/* CEffectWnd message map                                                    */
/*****************************************************************************/

BEGIN_MESSAGE_MAP(CEffectWnd, CWnd)
	//{{AFX_MSG_MAP(CEffectWnd)
	ON_WM_CLOSE()
	//ON_COMMAND(IDM_EFF_PROGRAM_NAME, OnEffProgramName)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_SIZING()
	//ON_COMMAND(IDM_EFF_EDIT_PARMS, OnEffEditParms)
	//ON_COMMAND(IDM_EFF_EDIT, OnEffEdit)
	//ON_UPDATE_COMMAND_UI(IDM_EFF_EDIT, OnUpdateEffEdit)
	//ON_COMMAND(IDM_EFF_INFO, OnEffInfo)
	//ON_COMMAND(IDM_EFF_RESIZE, OnEffResize)
	//ON_COMMAND(IDM_EFF_CHECKSIZE, OnEffChecksize)
	//ON_COMMAND(IDM_EFF_LOAD, OnEffLoad)
	//ON_COMMAND(IDM_EFF_SAVE, OnEffSave)
	//ON_COMMAND(IDM_EFF_SAVEAS, OnEffSaveas)
	//ON_COMMAND(IDM_EFF_SELPROGRAM, OnEffSelprogram)
	//ON_UPDATE_COMMAND_UI(IDM_EFF_SELPROGRAM, OnUpdateEffSelprogram)
	//ON_COMMAND(IDM_SEL_MIDICHN, OnSelMidichn)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_SYSKEYDOWN()
	ON_WM_SYSKEYUP()
	//}}AFX_MSG_MAP
    //ON_COMMAND_RANGE(IDM_EFF_PROGRAM_0, IDM_EFF_PROGRAM_0+999, OnSetProgram)
    ON_WM_ERASEBKGND()
    ON_WM_ENTERIDLE()
    ON_WM_DESTROY()
END_MESSAGE_MAP()

/*****************************************************************************/
/* OnClose : called when the editor window is closed                         */
/*****************************************************************************/
void CEffectWnd::FuckEffect()
{
  ASSERT(nEffect >= 0);
  CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nEffect);
  ASSERT(pEffect);
  if (pEffect)
  {
    pEffect->SetEditWnd(NULL);
    if(pEffect->bEditOpen)
    {
      //CTAFTEST
      //pEffect->EnterCritical();               /* make sure we're not processing                                          */
      pEffect->EffEditClose(); //ca yavait
      //pEffect->LeaveCritical();               /* re-enable processing                                               */
    }
  }

  CString buf;buf.Format("FuckEffect :: CEffectWnd(%d) \n", this);  TRACE(buf);

  //CVSTHost &Host = theApp.host;
  //Host.EffEditClose(nEffect);


  //pEffect->EffEditOpen((void *)3211);
  //pEffect->EffEditClose();                /* tell effect edit window's closed  */

  //pEffect->SetEditWnd(NULL);
  //pMain->EditClosed();                    /* tell main window it's gone        */
 

  //nEffect = -1;
 /* rcEffFrame.SetRectEmpty();
  rcEffClient.SetRectEmpty();*/
}
void CEffectWnd::OnClose() 
{
  //FuckEffect();
  ShowWindow(FALSE);
//CWnd::OnClose();
}

/*****************************************************************************/
/* OnSetProgram : called to change to another program                        */
/*****************************************************************************/

void CEffectWnd::OnSetProgram(UINT nID)
{
//pMain->OnSetProgram(nID);
}

/*****************************************************************************/
/* OnEffProgramName : called to change the program name                      */
/*****************************************************************************/

void CEffectWnd::OnEffProgramName() 
{
CEffect *pEffect = APP->host->GetAt(nEffect);
if (pEffect)
  {
/*  CProgNameDlg dlg(this);
  char szTxt[65] = "";
  pEffect->EffGetProgramName(szTxt);
  dlg.sPgName = szTxt;
  if (dlg.DoModal() == IDOK)
    {
    strcpy(szTxt, dlg.sPgName.Left(64));
    pEffect->EffSetProgramName(szTxt);
    SetupTitle();
    }*/;
  }
}

/*****************************************************************************/
/* SetupTitle : sets up the window title                                     */
/*****************************************************************************/

void CEffectWnd::SetupTitle()
{
CEffect *pEffect = APP->host->GetAt(nEffect);
CString sName;
if (pEffect)
  {
  sName.Format("%d %s: ", nEffect, GetEditType());
  char szBuf[256] = "";
                                        /* if V2 plugin                      */
  if (pEffect->EffGetProductString(szBuf))
    sName += szBuf;                     /* use plugin info                   */
  else                                  /* if V1 plugin                      */
    {
    CString sFile(pEffect->sName);
    int nrbsl = sFile.ReverseFind('\\');
    sName += sFile.Mid(nrbsl + 1);      /* use plugin file name              */
    }

  CString sProg;
  pEffect->EffGetProgramName(szBuf);
  sProg.Format(" Program %d: %s", pEffect->EffGetProgram(), szBuf);
  sName += sProg;
  }
SetWindowText(sName);
//SetTitle(sName);
//OnUpdateFrameTitle(TRUE);
}

/*****************************************************************************/
/* OnCreate : called when the window is created                              */
/*****************************************************************************/

int CEffectWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
if (CWnd::OnCreate(lpCreateStruct) == -1)
  return -1;
	
/*if (NeedView())
  {
  pView = CreateView();
  if (!pView)
    {
    TRACE0("Failed to create view window\n");
    return -1;
    }
  }

SetIcon(RetrieveIcon(), TRUE);*/
return 0;
}

/*****************************************************************************/
/* OnSetFocus : called when the window receives the input focus              */
/*****************************************************************************/

void CEffectWnd::OnSetFocus(CWnd* pOldWnd) 
{
CWnd::OnSetFocus(pOldWnd);
/*if (pView)
  pView->SetFocus();*/
}

/*****************************************************************************/
/* GetEffectEditWndSize : calculates the effect window's size                */
/*****************************************************************************/

BOOL CEffectWnd::GetEffectEditWndSize(CRect &rcFrame, CRect &rcClient, ERect *pRect)
{
if (/*(!pView) && */(!pRect))
  {
  CEffect *pEffect = APP->host->GetAt(nEffect);
  if (!pEffect)
    return FALSE;
  pEffect->EffEditGetRect(&pRect);
  }
if (!pRect)
  return FALSE;

rcFrame.SetRect(pRect->left, pRect->top, pRect->right, pRect->bottom);
/*rcFrame.bottom += /*::GetSystemMetrics(SM_CYCAPTION) +
             /*2 * ::GetSystemMetrics(SM_CYBORDER) +*/
             /*4*::GetSystemMetrics(SM_CYBORDER);*/
/*rcFrame.right += /*2 * ::GetSystemMetrics(SM_CXFRAME)*/ /*+
            4 * ::GetSystemMetrics(SM_CXBORDER);*/

rcClient.left = rcClient.top = 0;
rcClient.right = pRect->right - pRect->left;
rcClient.bottom = pRect->bottom - pRect->top;

return TRUE;
}

/*****************************************************************************/
/* SetEffSize : sets the effect size                                         */
/*****************************************************************************/

void CEffectWnd::SetEffSize(ERect *pRect)
{
GetEffectEditWndSize(rcEffFrame, rcEffClient, pRect);
SetWindowPos(&wndTop, 0, 0, rcEffFrame.Width(), rcEffFrame.Height(),
             SWP_NOACTIVATE | SWP_NOMOVE |
                 SWP_NOOWNERZORDER | SWP_NOZORDER);
}

void CEffectWnd::SetEffSize(int cx, int cy)
{
CRect rcW;
GetWindowRect(&rcW);

ERect rc;
rc.left = (short)rcW.left;
rc.top = (short)rcW.top;
rc.right = rc.left + cx;
rc.bottom = rc.top + cy;
SetEffSize(&rc);
}

/*****************************************************************************/
/* OnSize : called when the WM_SIZE message comes in                         */
/*****************************************************************************/

void CEffectWnd::OnSize(UINT nType, int cx, int cy) 
{
  CWnd::OnSize(nType, cx, cy);
  if((cx != 0)&&(cy != 0))
    APP->pMainDlg->ChildNotify(this,true);
}

/*****************************************************************************/
/* OnEffChecksize : called to check that the window is not too big now       */
/*****************************************************************************/

void CEffectWnd::OnEffChecksize() 
{
//if (!pView)                             /* if not using our own view         */
  GetEffectEditWndSize(rcEffFrame,      /* get current effect edit size idea */
                       rcEffClient);

CRect rcFrame;
GetWindowRect(&rcFrame);

int cx = rcFrame.Width(), cy = rcFrame.Height();
int cxmax = rcEffFrame.Width();
int cymax = rcEffFrame.Height();
bool bReset = false;
if (cx > rcEffFrame.Width())
  {
  cx = cxmax;
  bReset = true;
  }
if (cy > cymax)
  {
  cy = cymax;
  bReset = true;
  }

if (bReset)
  SetWindowPos(&wndTop, 0, 0, cx, cy,
               SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOZORDER);
	
}


/*****************************************************************************/
/* OnSizing : called when the window is being resized                        */
/*****************************************************************************/

void CEffectWnd::OnSizing(UINT fwSide, LPRECT pRect) 
{
CWnd::OnSizing(fwSide, pRect);

//if (!pView)                             /* if not using our own view         */
  GetEffectEditWndSize(rcEffFrame,      /* get current effect edit size idea */
                       rcEffClient);

if (pRect->right - pRect->left >
    rcEffFrame.right - rcEffFrame.left)
  pRect->right = pRect->left + rcEffFrame.right - rcEffFrame.left;

if (pRect->bottom - pRect->top >
    rcEffFrame.bottom - rcEffFrame.top)
  pRect->bottom = pRect->top + rcEffFrame.bottom - rcEffFrame.top;
	
}



/*****************************************************************************/
/* Update : make sure screen is up-to-date                                   */
/*****************************************************************************/

void CEffectWnd::Update()
{
SetupTitle();
Invalidate(FALSE);
}

/*****************************************************************************/
/* MakeVstKeyCode : converts from Windows to VST                             */
/*****************************************************************************/

void CEffectWnd::MakeVstKeyCode(UINT nChar, UINT nRepCnt, UINT nFlags, VstKeyCode &keyCode)
{
#if defined(VST_2_1_EXTENSIONS)

static struct
  {
  UINT vkWin;
  unsigned char vstVirt;
  } VKeys[] =
  {
    { VK_BACK,      VKEY_BACK      },
    { VK_TAB,       VKEY_TAB       },
    { VK_CLEAR,     VKEY_CLEAR     },
    { VK_RETURN,    VKEY_RETURN    },
    { VK_PAUSE,     VKEY_PAUSE     },
    { VK_ESCAPE,    VKEY_ESCAPE    },
    { VK_SPACE,     VKEY_SPACE     },
//  { VK_NEXT,      VKEY_NEXT      },
    { VK_END,       VKEY_END       },
    { VK_HOME,      VKEY_HOME      },
    { VK_LEFT,      VKEY_LEFT      },
    { VK_UP,        VKEY_UP        },
    { VK_RIGHT,     VKEY_RIGHT     },
    { VK_DOWN,      VKEY_DOWN      },
    { VK_PRIOR,     VKEY_PAGEUP    },
    { VK_NEXT,      VKEY_PAGEDOWN  },
    { VK_SELECT,    VKEY_SELECT    },
    { VK_PRINT,     VKEY_PRINT     },
    { VK_EXECUTE,   VKEY_ENTER     },
    { VK_SNAPSHOT,  VKEY_SNAPSHOT  },
    { VK_INSERT,    VKEY_INSERT    },
    { VK_DELETE,    VKEY_DELETE    },
    { VK_HELP,      VKEY_HELP      },
    { VK_NUMPAD0,   VKEY_NUMPAD0   },
    { VK_NUMPAD1,   VKEY_NUMPAD1   },
    { VK_NUMPAD2,   VKEY_NUMPAD2   },
    { VK_NUMPAD3,   VKEY_NUMPAD3   },
    { VK_NUMPAD4,   VKEY_NUMPAD4   },
    { VK_NUMPAD5,   VKEY_NUMPAD5   },
    { VK_NUMPAD6,   VKEY_NUMPAD6   },
    { VK_NUMPAD7,   VKEY_NUMPAD7   },
    { VK_NUMPAD8,   VKEY_NUMPAD8   },
    { VK_NUMPAD9,   VKEY_NUMPAD9   },
    { VK_MULTIPLY,  VKEY_MULTIPLY  },
    { VK_ADD,       VKEY_ADD,      },
    { VK_SEPARATOR, VKEY_SEPARATOR },
    { VK_SUBTRACT,  VKEY_SUBTRACT  },
    { VK_DECIMAL,   VKEY_DECIMAL   },
    { VK_DIVIDE,    VKEY_DIVIDE    },
    { VK_F1,        VKEY_F1        },
    { VK_F2,        VKEY_F2        },
    { VK_F3,        VKEY_F3        },
    { VK_F4,        VKEY_F4        },
    { VK_F5,        VKEY_F5        },
    { VK_F6,        VKEY_F6        },
    { VK_F7,        VKEY_F7        },
    { VK_F8,        VKEY_F8        },
    { VK_F9,        VKEY_F9        },
    { VK_F10,       VKEY_F10       },
    { VK_F11,       VKEY_F11       },
    { VK_F12,       VKEY_F12       },
    { VK_NUMLOCK,   VKEY_NUMLOCK   },
    { VK_SCROLL,    VKEY_SCROLL    },
    { VK_SHIFT,     VKEY_SHIFT     },
    { VK_CONTROL,   VKEY_CONTROL   },
    { VK_MENU,      VKEY_ALT       },
//  { VK_EQUALS,    VKEY_EQUALS    },

  };

if ((nChar >= 'A') && (nChar <= 'Z'))
  keyCode.character = nChar + ('a' - 'A');
else
  keyCode.character = nChar;
keyCode.virt = 0;
for (int i = 0; i < (sizeof(VKeys)/sizeof(VKeys[0])); i++)
  if (nChar == VKeys[i].vkWin)
    {
    keyCode.virt = VKeys[i].vstVirt;
    break;
    }
keyCode.modifier = 0;
if (GetKeyState(VK_SHIFT) & 0x8000)
  keyCode.modifier |= MODIFIER_SHIFT;
if (GetKeyState(VK_CONTROL) & 0x8000)
  keyCode.modifier |= MODIFIER_CONTROL;
if (GetKeyState(VK_MENU) & 0x8000)
  keyCode.modifier |= MODIFIER_ALTERNATE;
#endif
}

/*****************************************************************************/
/* OnKeyDown : called when a key is pressed on the effect window             */
/*****************************************************************************/

void CEffectWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#if defined(VST_2_1_EXTENSIONS)
VstKeyCode keyCode;
MakeVstKeyCode(nChar, nRepCnt, nFlags, keyCode);
if (APP->host->EffKeyDown(nEffect, keyCode) != 1)
#endif

  CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

/*****************************************************************************/
/* OnKeyUp : called when a key is released on the effect window              */
/*****************************************************************************/

void CEffectWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#if defined(VST_2_1_EXTENSIONS)
VstKeyCode keyCode;
MakeVstKeyCode(nChar, nRepCnt, nFlags, keyCode);
if (APP->host->EffKeyUp(nEffect, keyCode) != 1)
#endif

  CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

/*****************************************************************************/
/* OnSysKeyDown : called when a system key is pressed                        */
/*****************************************************************************/

void CEffectWnd::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#if defined(VST_2_1_EXTENSIONS)
VstKeyCode keyCode;
MakeVstKeyCode(nChar, nRepCnt, nFlags, keyCode);
if (APP->host->EffKeyDown(nEffect, keyCode) != 1)
#endif

  CWnd::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

/*****************************************************************************/
/* OnSysKeyUp : called when a system key is released                         */
/*****************************************************************************/

void CEffectWnd::OnSysKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
#if defined(VST_2_1_EXTENSIONS)
VstKeyCode keyCode;
MakeVstKeyCode(nChar, nRepCnt, nFlags, keyCode);
if (APP->host->EffKeyUp(nEffect, keyCode) != 1)
#endif
	
  CWnd::OnSysKeyUp(nChar, nRepCnt, nFlags);
}

BOOL CEffectWnd::OnEraseBkgnd(CDC* pDC)
{
  // TODO : ajoutez ici le code de votre gestionnaire de messages et/ou les paramètres par défaut des appels

  return FALSE;//de toutes facon il connait pas les erase le plug
  return CWnd::OnEraseBkgnd(pDC);

  //return TRUE;
}

void CEffectWnd::EnterIdle()
{

  if(nEffect == -1)return;

  CSmpEffect *pEffect = (CSmpEffect *)APP->host->GetAt(nEffect);
  //ASSERT(pEffect); failed dans classic flanger
  if(pEffect)
    pEffect->EffEditIdle();

  // TODO : ajoutez ici le code de votre gestionnaire de messages
}

BOOL CEffectWnd::DestroyWindow()
{
  // TODO : ajoutez ici votre code spécialisé et/ou l'appel de la classe de base

  
  return CWnd::DestroyWindow();
}

void CEffectWnd::OnDestroy()
{
  FuckEffect();
  CWnd::OnDestroy();

  // TODO : ajoutez ici le code de votre gestionnaire de messages
}
