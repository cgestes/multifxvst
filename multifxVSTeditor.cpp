//------------------------------------------------------------------------
//-
//- Project     : Use different Controls of VSTGUI
//- Filename    : multifxVSTeditor.cpp
//- Created by  : Yvan Grabit
//- Description :
//-
//- © 2000-1999, Steinberg Soft und Hardware GmbH, All Rights Reserved
//------------------------------------------------------------------------
#include "stdafx.h"

//#include "multifxVSTmain.h"

#ifndef __multifxVSTeditor__
#include "multifxVSTeditor.h"
#endif

#include "stockeffet.h"
#include "CCVSThost.h"


#ifndef __multifxVST__
#include "multifxVST.h"
#endif

#include "Chaindlg.h"
#include "MainDlg.h"
#include "controleurdlg.h"


#include <math.h>
#include <stdlib.h>	
#include <stdio.h>



enum
{
	// bitmaps
	kBackgroundBitmap = 10001,
  kKickBtn,
	kSliderHBgBitmap,
	kSliderHandleBitmap,
  kDigitBitmap,
	// others
	kBackgroundW = 420,
	kBackgroundH = 210
};

void stringConvert (float value, char* string);


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
multifxVSTEditor::multifxVSTEditor (AudioEffect *effect) 
	:	AEffGUIEditor (effect)
{
	frame = 0;
	oldTicks = 0;
  visible = false;
	rect.left   = 0;
	rect.top    = 0;
	rect.right  = kBackgroundW;
	rect.bottom = kBackgroundH;
  /*host = NULL;
  chaine_eff = NULL;*/

	// we decide in this plugin to open all bitmaps in the open function
}

//-----------------------------------------------------------------------------
multifxVSTEditor::~multifxVSTEditor ()
{
  CString buf;buf.Format("DESTROY :: multifxVSTEditor(%d) \n", this);  TRACE(buf);

	WinRect.top = WinRect.left = WinRect.bottom = WinRect.right =0;
}

void multifxVSTEditor::SetAPP(CAppPointer * m_cheff)
{
  APP = m_cheff;
}

long multifxVSTEditor::getRect(ERect **rect)
{
	*rect = &WinRect;
	return TRUE;
}
//-----------------------------------------------------------------------------
long multifxVSTEditor::open (void *ptr)
{
	//obligatoire avant le premier appel au MFC dans une dll lié dynamique
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// always call this !!!
	AEffGUIEditor::open (ptr);

	// get version
	int version = getVstGuiVersion ();
	int verMaj = (version & 0xFF00) >> 16;
	int verMin = (version & 0x00FF);
    
  //--dialog MFC---------------------------------------------
  frame = new CFrame (VSTGUI::CRect(0,0,100,100), ptr, this);
  //on récupere le handle de la fenetre contenant notre plug-ins (la fenetre de l'host)
  HWND vb= (HWND)frame->getSystemWindow();
  CWnd  dfk;
  dfk.Attach(vb);


  APP->pMainDlg = new CMainDlg(&dfk); 
  ASSERT(APP->pMainDlg);



  //on crée la fenetre principale
  BOOL b =APP->pMainDlg->Create((UINT)CMainDlg::IDD ,&dfk);
  ASSERT(b);
  dfk.Detach();


  //TRACE("NOUVELLE FENETRE\n");

  APP->pMainDlg->SetAPP(APP);

  APP->pMainDlg->Init();


  //dlg->SetWindowPos(NULL,WinRect.left,WinRect.top,WinRect.right,WinRect.bottom,0);
  ::CRect r;
  APP->pMainDlg->GetWindowRect(&r);

	WinRect.top = WinRect.left = 0;
	WinRect.bottom =r.Height();
	WinRect.right = r.Width();

  frame->setSize(r.Width(), r.Height());
  APP->pMainDlg->ShowWindow(SW_SHOW);

  /*APP->pMainDlg->ChildNotify(dlg,0);
  APP->pMainDlg->ChildNotify(dlg,1);*/


  
	// here we can call a initialize () function to initalize all controls values
	return true;
}

//-----------------------------------------------------------------------------
bool multifxVSTEditor::keysRequired ()
{
	if (frame && frame->getEditView ())
		return true;
	else
		return false;
}

//-----------------------------------------------------------------------------
long multifxVSTEditor::onKeyDown (VstKeyCode &keyCode)
{
	return 1;
}

//-----------------------------------------------------------------------------
long multifxVSTEditor::onKeyUp (VstKeyCode &keyCode)
{
	return 1; 
}

//-----------------------------------------------------------------------------
void multifxVSTEditor::close ()                               //AFFFF normalement plus besoin du if
{
	//pour ttes les fonctions générants des appels a la bibli mfc
  	AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CString buf;buf.Format("DEBCLOSE :: multifxVSTEditor(%d) \n", this);  TRACE(buf);
   
  if(APP->pMainDlg)
  {
     
     //if((long(APP->pMainDlg->m_hWnd) != 0) && (long(APP->pMainDlg->m_hWnd) != 0xfeeefeee))
     {
       //supprime les childs

     APP->pMainDlg->ShowWindow(SW_HIDE);
	   APP->pMainDlg->DestroyWindow();

     }
  }
  
	// don't forget to remove the frame !!
	if (frame)
		delete frame;


	// set to zero all pointer (security)
	cKickButton    = 0;
	frame          = 0;
  //APP            = 0;

  buf.Format("CLOSE :: multifxVSTEditor(%d) \n", this);  TRACE(buf);

}

//-----------------------------------------------------------------------------
void multifxVSTEditor::idle ()                                        //AFFAIREEEEEE
{

  APP->pMainDlg->EnterIdle();
	AEffGUIEditor::idle ();		// always call this to ensure update
}

void multifxVSTEditor::update()                                       
{
  AEffEditor::update();
  APP->pMainDlg->ChangeChaine(APP->current_chaine);
  APP->pMainDlg->SetEffect(-1);
}
//-----------------------------------------------------------------------------  AFAFAFAFAAF
void multifxVSTEditor::setParameter (long index, float value)
{
  //pour ttes les fonctions générants des appels a la bibli mfc
  AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// called from the Aeffect to update the control's value
	// test if the plug is opened
	if (!frame)
		return;

	switch (index)
	{
  case kSliderHTag:
    {
      //on notifie la fenetre que les params ont changées
      if(APP->pChain)
//       APP->pChain->SetNewNumChaine();

     break;
    }
	}
	
	// call this to be sure that the graphic will be updated
	postUpdate ();
}

//-----------------------------------------------------------------------------
void multifxVSTEditor::valueChanged (CDrawContext* context, CControl* control)
{
		control->update (context);	
}

//-----------------------------------------------------------------------------
void stringConvert (float value, char* string)
{
	 sprintf (string, "p %.2f", value);
}