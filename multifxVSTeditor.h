//------------------------------------------------------//
//- CopyLeft : CTAF
//-
//- gestion du VST (affichage dans l'host)     
//- TODO : un bel affichage            
//-                                                   
//------------------------------------------------------//

#ifndef __multifxVSTeditor__
#define __multifxVSTeditor__

#ifndef __vstgui__
#include "vstgui.h"
#endif


//class CChainDlg; 
//class CStockEffetLst;
//class CCVSTHost;
class CMainDlg;

class CAppPointer;


//class Test;

//-----------------------------------------------------------------------------
class multifxVSTEditor : public AEffGUIEditor, public CControlListener
{
public:
	multifxVSTEditor (AudioEffect *effect);
	virtual ~multifxVSTEditor ();

	/*void suspend ();
	void resume ();*/
	bool keysRequired ();
  void SetAPP(CAppPointer * m_cheff = NULL);
protected:
	virtual long open (void *ptr);
	virtual void idle ();
	void setParameter (long index, float value);
	virtual void close ();
  virtual void update();
	
	// VST 2.1
	virtual long onKeyDown (VstKeyCode &keyCode);
	virtual long onKeyUp (VstKeyCode &keyCode);
	virtual long getRect(ERect **rect);

private:
	void valueChanged (CDrawContext* context, CControl* control);

  bool visible;

	CHorizontalSlider *cHorizontalSlider;
	CHorizontalSlider *cHorizontalSlider2;
	CSpecialDigit     *cSpecialDigit;
	COnOffButton       *cKickButton;

	// others
  CAppPointer     * APP;
  
  /*CChainDlg  * dlg;
  CMainDlg        * maindlg;
  CStockEffetLst  * chaine_eff;
  CCVSTHost       * host;*/
  ERect             WinRect;
	long              oldTicks;
  int               UpdateType;
public:
  friend CMainDlg; //pour l'access au pointeur frame

};

#endif
