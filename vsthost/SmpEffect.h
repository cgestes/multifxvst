/*****************************************************************************/
/* SmpEffect.h: Interface for classes CSmpVSTHost / CSmpEffect              */
/*****************************************************************************/

#if !defined(AFX_SMPVSTHOST_H__EA642421_A9AF_11D5_B6BD_004F4E003207__INCLUDED_)
#define AFX_SMPVSTHOST_H__EA642421_A9AF_11D5_B6BD_004F4E003207__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CVSTHost.h"

/*****************************************************************************/
/* CSmpEffect class declaration                                              */
/*****************************************************************************/

class CEffectTxTDlg;
class CEffectWnd;
class CAppPointer;
class CParamStk
{
public :

	CParamStk();
  ~CParamStk();
	CParamStk(CParamStk & eff);

  void loadFromMem(CEffect *ceff);
  void saveToMem(CEffect *ceff);


	/*int effect_nb;       //numero de l'effet dans CVSTHost
  int effectbisnb;     //numero d'instance de l'effet (si plusieurs fois le mm effet dans une chaine)
	CString effect_name; //path et nom de la dll
	CString bankname;    //path et nom de la bank d'effet utilis?
  */
  float  * tabval;
  char   * chunk;
  char   * tabcontroleur;
  bool   ischunk;
  long   length;
  //int    nbcontroleur;
};

class CSmpEffect : public CEffect  
{
public:
  friend class CCVSTHost;
	CSmpEffect(CVSTHost *pHost);
	virtual ~CSmpEffect();

public:
	void LeaveCritical();
	void EnterCritical();
	virtual bool Load(const char *name);
    virtual bool Unload();
	virtual long EffDispatch(long opCode, long index=0, long value=0, void *ptr=0, float opt=0.);
	virtual void OnSizeEditorWindow(long width, long height);
    void SetEditWnd(CEffectWnd  *pEWnd = 0) { pEditWnd = pEWnd; }
    void SetParmWnd(CEffectTxTDlg *pPWnd = 0) { pParmWnd = pPWnd; }
    unsigned short GetChnMask() { return wChnMask; }
    void SetChnMask(unsigned short mask) { wChnMask = mask; }
    bool OnSetParameterAutomated(long index, float value);
    virtual bool OnUpdateDisplay();
    void SetChunkFile(CString sFile) { sChunkFile = sFile; }
    void SetChunkFilePreset(CString sFile) { sChunkFilePreset = sFile; }
    CString GetChunkFile() { return sChunkFile; }
    CString GetChunkFilePreset() { return sChunkFilePreset; }
   CAppPointer * APP; //c'est pas un vrai pointeur APP, ne pas s'en servir

   CEffectWnd * pEditWnd;
   CEffectTxTDlg * pParmWnd;
   CParamStk DefParam;
protected:
	unsigned short wChnMask;
	//int nAllocatedOutbufs;
	//int nAllocatedInbufs;
  int effectuse;
	static int EvalException(int n_except);
    CRITICAL_SECTION cs;

	//float ** outBufs;
	//float ** inBufs;
	/*CEffectWnd * pEditWnd;
	CEffectTxTDlg * pParmWnd;*/
   CString sChunkFile;
   CString sChunkFilePreset;
};

/*****************************************************************************/
/* CSmpVSTHost class declaration                                             */
/*****************************************************************************/


#endif // !defined(AFX_SMPVSTHOST_H__EA642421_A9AF_11D5_B6BD_004F4E003207__INCLUDED_)
