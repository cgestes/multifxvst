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

//#include <string.h>

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
class CEffEditDlg;
class CStockEffetLst;
class CCVSTHost;
class CMainDlg;
class CGratomaticDlg;
class multifxVSTEditor;
class multifxVST;
class CControleurDlg;
class CControleurLst;

//pointeur global qui stoque les object pour chaque instance
class CAppPointer
{
public :
  CAppPointer(){
    chaine_eff     = 0;
    host           = 0;
    pGratomatic    = 0;
    pEffEditDlg    = 0;
    pEffParmDlg    = 0;
    pMainDlg       = 0;
    editor         = 0;
    effect         = 0;
    current_chaine = 0;
  }


  CStockEffetLst   * chaine_eff;
  CCVSTHost        * host;
  CGratomaticDlg   * pGratomatic;
  CControleurDlg   * pControleur;
  CControleurLst   * controleur;
  CEffectWnd       * pEffEditDlg;
  CEffEditDlg      * pEffParmDlg;
  CMainDlg         * pMainDlg;
  multifxVSTEditor * editor; 
  multifxVST       * effect; 

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
	/*virtual void setProgram (long program);
	virtual void setProgramName (char *name);
	virtual void getProgramName (char *name);
	virtual bool getProgramNameIndexed (long category, long index, char* text);*/

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

	//Program *programs;
	//float fSliderValue;
  
  /*CStockEffetLst * chaine_eff;
  CCVSTHost      * host;
  int            current_chaine;*/
  CAppPointer APP;
  void * dat; //pour les sauvegardes dans l'hote
};

#endif
