//------------------------------------------------------//
//- CopyLeft : CTAF
//-
//- Class CCVSTHost : gere des plugins VST
//- derivé pour le initcritical dans le constructeur de CSmpEffect
//- TODO : **           
//-                                                   
//------------------------------------------------------//

#ifndef __CVSTHost__
#define __CVSTHost__

#include "vsthost/cvsthost.h"

class CSmpEffect;
class CAppPointer;

class CCVSTHost :  public CVSTHost
{
public:
  CCVSTHost();
  virtual ~CCVSTHost();
  virtual CEffect * CreateEffect();

  void IncreaseUse(int nEffect);

  int GetUse(int nEffect);

  //return vrai si il à supprimer le plug
  bool DecreaseUse(int nEffect,bool killing_plug = false,CAppPointer * m_app = NULL);

  int LoadPlugin(const char *lpszName,CAppPointer * APP);

  //VST COMMUNICATION
  virtual VstTimeInfo *OnGetTime(int nEffect,long filter);
  virtual long OnTempoAt(int nEffect,long pos);
  virtual long OnGetHostVendorVersion(int nEffect);
  virtual bool OnGetHostProductString(int nEffect,char *text);
  virtual long OnHostVendorSpecific(int nEffect, long lArg1, long lArg2, void* ptrArg, float floatArg);
  virtual bool OnIoChanged(int nEffect);
  virtual long OnGetOutputLatency(int nEffect);
  virtual long OnGetInputLatency(int nEffect);
//0684942445

  virtual bool OnSetParameterAutomated(int nEffect, long index, float value);
  
  CAppPointer * SetAPP(CAppPointer * m_diff_from_APP = NULL);
protected:
  CAppPointer * APP; //pointeur sur une des APP
};

#endif