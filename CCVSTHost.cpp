#include "stdafx.h"

#include "CCVSThost.h"
#include "multifxVST.h"
#include "stockeffet.h"
#include "vsthost/SmpEffect.h"



CEffect * CCVSTHost::CreateEffect()
{
  return new CSmpEffect(this);
}

CCVSTHost::CCVSTHost()/*:CVSTHost()*/
{
  int a = 0;
  APP = NULL;
  return;
}

//on recherche un pointeur APP valide
CAppPointer * CCVSTHost::SetAPP(CAppPointer * m_diff_from_APP)
{
  int i,j = GetSize();
  CSmpEffect * eff;
  for(i=0;i<j;i++)
  {
    eff = (CSmpEffect *)GetAt(i);
    if(eff && eff->APP)
    {
      if(m_diff_from_APP)
      {
        if(eff->APP != m_diff_from_APP)
        {
          APP = eff->APP;
          break;
        }
      }else
      {
        APP = eff->APP;
        break;
      }
    }
  }
  return APP;
}

CCVSTHost::~CCVSTHost()
{
  CString buf;buf.Format("DESTROY :: CCVSTHost(%d) \n", this);  TRACE(buf);
  int b = 0;//histoire de rien faire!
  return;
}

void CCVSTHost::IncreaseUse(int nEffect)
{
  CSmpEffect * eff = (CSmpEffect *)GetAt(nEffect);
  if(eff)
    eff->effectuse++;
}

int CCVSTHost::LoadPlugin(const char *sName,CAppPointer * APP)
{
CSmpEffect *pEffect = (CSmpEffect *)CreateEffect();      /* allocate an effect                */
pEffect->APP = APP;

int nIndex;                             /* look for empty slot in array      */
for (nIndex = GetSize() - 1; nIndex >= 0; nIndex--)
  if (!GetAt(nIndex))
    break;
if (nIndex < 0)                         /* if no empty slot available        */
  {
  if (naEffects + 1 > nmaEffects)       /* if no more space in effects array */
    {                                   /* re-allocate effects array         */
    void **anew = new void *[nmaEffects + 5];
    if (anew)
      {
      for (int i = 0; i < naEffects; i++)
        anew[i] = aEffects[i];
      if (aEffects)
        delete[] aEffects;
      aEffects = anew;
      nmaEffects += 5;
      }
    else
      {
      delete pEffect;                   /* upon error delete the object      */
      return -1;                        /* and return with error             */
      }
    }
  nIndex = naEffects++;                 /* remember position                 */
  aEffects[nIndex] = pEffect;           /* add to effect array               */
  }
else                                    /* otherwise                         */
  aEffects[nIndex] = pEffect;           /* put into free slot                */

//on fait le load au final,sinon peut yavoir des couillons dans le potager
//le plug demande des truc avant qu'il ne soit dans le tableau dc son index est inconnu
if (!pEffect->Load(sName))              /* try to load the thing             */
  {
  aEffects[nIndex] = NULL;
  delete pEffect;                       /* upon error delete the object      */
  return -1;                            /* and regretfully return error      */
  }


pEffect->EffOpen();                     /* open the effect                   */
pEffect->EffSetSampleRate(fSampleRate); /* adjust its sample rate            */
pEffect->EffSetBlockSize(lBlockSize);   /* and block size                    */
pEffect->EffMainsChanged(true);         /* then force resume.                */

 return nIndex;
}

int CCVSTHost::GetUse(int nEffect)
{
  CSmpEffect * eff = (CSmpEffect *)GetAt(nEffect);
  if(eff)
    return eff->effectuse;
  else
    return 0;
}

//return true si ca supprime l'effet
bool CCVSTHost::DecreaseUse(int nEffect,bool killing_plug,CAppPointer * m_app)
{
  CSmpEffect * eff = (CSmpEffect *)GetAt(nEffect);
  if(eff)
    eff->effectuse--;

  //si l'effect n'est plus utilisé on le supprime
  if(eff->effectuse < 1)
  {
    RemoveAt(nEffect);
    return true;
  }else
  {
    //si cette effet est associé a cette instance de APP on essaye d'en chopper une autre!
    if(killing_plug && m_app && (eff->APP == m_app))
      eff->APP = SetAPP(m_app);
  }
  return false;
}

bool CCVSTHost::OnSetParameterAutomated(int nEffect, long index, float value)
{
CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
if (pEffect)
  pEffect->OnSetParameterAutomated(index, value);
return CVSTHost::OnSetParameterAutomated(nEffect, index, value);
  return 0;
}

//faut demander a l'host et retourner cette valeur
VstTimeInfo *CCVSTHost::OnGetTime(int nEffect,long filter)
{
  CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
  if(!pEffect)
  {
    SetAPP();
    if(APP)
      return APP->effect->getTimeInfo(filter);
    else
    {
      TRACE("ERREUR OnGetTime\n");
      return 0;
    }
  }
  else
    return   pEffect->APP->effect->getTimeInfo(filter);

}

long CCVSTHost::OnTempoAt(int nEffect,long pos)
{
  CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
  if(!pEffect)
  {
    SetAPP();
    if(APP)
      return APP->effect->tempoAt(pos);
    else
    {
      TRACE("ERREUR TEMPOAT\n");
      return 0;
    }
  }
  else
  return   pEffect->APP->effect->tempoAt(pos);
}


long CCVSTHost::OnGetHostVendorVersion(int nEffect)
{
  CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
  if(!pEffect)
  {
    SetAPP();
    if(APP)
      return APP->effect->getHostVendorVersion();
    else
    {
      TRACE("ERREUR OnGetHostVendorVersion\n");
      return 0;
    }
  }
  else
  return   pEffect->APP->effect->getHostVendorVersion();
}

long CCVSTHost::OnHostVendorSpecific(int nEffect, long lArg1, long lArg2, void* ptrArg, float floatArg)
{
  CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
  if(!pEffect)
  {
    SetAPP();
    if(APP)
      return APP->effect->hostVendorSpecific( lArg1,  lArg2,  ptrArg,  floatArg);
    else
    {
      TRACE("ERREUR OnHostVendorSpecific\n");
      return 0;
    }
  }
  return pEffect->APP->effect->hostVendorSpecific( lArg1,  lArg2,  ptrArg,  floatArg);
}

bool CCVSTHost::OnGetHostProductString(int nEffect,char *text)
{
  CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
  if(!pEffect)
  {
    SetAPP();
    if(APP)
      return APP->effect->getProductString(text);
    else
    {
      TRACE("ERREUR OnGetHostProductString\n");
      return 0;
    }
  }
  else
  return pEffect->APP->effect->getProductString(text);
}


bool CCVSTHost::OnIoChanged(int nEffect)
{
  long InitDelay = 0;
  CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
  if(!pEffect)
  {
    SetAPP();
    if(APP)
    {
      InitDelay = APP->chaine_eff->CalculDelay(APP->current_chaine);
      APP->effect->setInitialDelay(InitDelay);
      APP->effect->ioChanged();
      return 1;
    }
    else
    {
      TRACE("ERREUR OnGetHostProductString\n");
      return 0;
    }
  }
  else
  {
      InitDelay = pEffect->APP->chaine_eff->CalculDelay(pEffect->APP->current_chaine);
      pEffect->APP->effect->setInitialDelay(InitDelay);
      pEffect->APP->effect->ioChanged();
      return 1;
  }
}

long  CCVSTHost::OnGetOutputLatency(int nEffect)
{
 CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
  if(!pEffect)
  {
    SetAPP();
    if(APP)
      return APP->effect->getOutputLatency();
    else
    {
      TRACE("ERREUR OnHostVendorSpecific\n");
      return 0;
    }
  }
  return pEffect->APP->effect->getOutputLatency();

}

long CCVSTHost::OnGetInputLatency(int nEffect)
{
 CSmpEffect *pEffect = (CSmpEffect *)GetAt(nEffect);
  if(!pEffect)
  {
    SetAPP();
    if(APP)
      return APP->effect->getInputLatency();
    else
    {
      TRACE("ERREUR OnHostVendorSpecific\n");
      return 0;
    }
  }
  return pEffect->APP->effect->getInputLatency();

}
