//------------------------------------------------------//
//- CopyLeft : CTAF
//-
//- gestion du VST (communication avec l'host)     
//- TODO : transmettre les params aux chaines plugins             
//-                                                   
//------------------------------------------------------//


#ifndef __multifxVST__
#define __multifxVST__

#ifndef __audioeffectx__
#include "audioeffectx.h"
#endif

enum
{
  kSliderHTag = 0,
  kNumParams,
	kOnOffTag,
  kKickTag,
  kDigitTag,
};

class CStockEffetLst;
class CCVSTHost;
class CEffectWnd;
class CEffectTxTDlg;
class CStockEffetLst;
class CCVSTHost;
class CMainDlg;
class CChainDlg;
class multifxVSTEditor;
class multifxVST;
class CControleurDlg;
class CControleurLst;
class CParameterLst;
//pointeur global qui stoque les object pour chaque instance
class CAppPointer
{
public :
  CAppPointer(){
    chaine_eff     = 0;
    host           = 0;
    pChain         = 0;
    pEffEditDlg    = 0;
    pEffParmDlg    = 0;
    pMainDlg       = 0;
    editor         = 0;
    effect         = 0;
    current_chaine = 0;
    parameter      = 0;
    mnu            = 0;
  }


  CStockEffetLst   * chaine_eff;//chaines d'effet
  CCVSTHost        * host;      //host VST virtuel (pr effet fils)
  CChainDlg        * pChain;    //dlg avec les listes de chaines
  CControleurDlg   * pControleur;//dlg azvec les controleurs midis
  CControleurLst   * controleur;  //liste des controleurs midi
  CEffectWnd       * pEffEditDlg; //fenetre d'effet
  CEffectTxTDlg    * pEffParmDlg; //fenetre d'effet générique
  CMainDlg         * pMainDlg;    //fenetre principale
  multifxVSTEditor * editor;      //fenetre VST contenant la fenetre principale
  multifxVST       * effect;      //Notre effet 
  CParameterLst    * parameter;   //automatisation des parametres
  CMenu            * mnu;
  int current_chaine;

};


//---------------------------------------------------------
class multifxVST : public AudioEffectX
{
public:
	multifxVST (audioMasterCallback audioMaster);
	~multifxVST ();

	virtual long vendorSpecific (long lArg1, long lArg2, void* ptrArg, float floatArg);
	virtual void process (float **inputs, float **outputs, long sampleFrames);
	virtual void processReplacing (float **inputs, float **outputs, long sampleFrames);
	virtual void setBlockSize (long blockSize);
	virtual void setParameter (long index, float value);
	virtual float getParameter (long index);
	virtual void getParameterLabel(long index, char *label);
	virtual void getParameterDisplay(long index, char *text);
	virtual void getParameterName(long index, char *text);
  virtual bool  needIdle(){return 1;};
  virtual long fxIdle(){return 0;};
	void mIdle () {masterIdle ();}

	virtual void suspend ();
	virtual void resume ();
  virtual void setSampleRate(float sampleRate);
	virtual bool keysRequired ();
  virtual bool getProductString(char *text){strcpy(text,"MultifxVST"); return TRUE;}
  virtual bool getVendorString (char* text){strcpy(text,"CTAF Audio - Cédric GESTES");return TRUE;}
  virtual bool getEffectName (char* name){strcpy(name,"MultifxVST");return TRUE;}
  virtual void close();

  virtual void open();
  void UpdatehostAPP();
  virtual void setParameterAutomated(long index ,float value);
  /*virtual int getInputLatency();
  virtual int getOutputLatency();*/
  //virtual void setParameterAutomated (long index, float value);
	virtual bool string2parameter (long index, char* text);

  virtual long getChunk(void **data,bool isPreset);
  virtual long setChunk(void *data,long byteSize,bool isPreset);

protected:
  CAppPointer APP;
  void * dat; //pour les sauvegardes dans l'hote
};

#endif
