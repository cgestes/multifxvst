/*****************************************************************************/
/* SmpVSTHost.cpp: CSmpVSTHost / CSmpEffect implementation                   */
/*****************************************************************************/

#include "stdafx.h"
#include "SmpEffect.h"
#include "../EffectWnd.h"
#include "../EffectTxTDlg.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CParamStk::CParamStk()
{
  tabval = NULL;
  chunk = NULL;
  ischunk = false;
  length = 0;
}


CParamStk::~CParamStk()
{
  if(tabval)delete tabval;
  if(chunk)delete chunk;
  TRACE("DESTROY::CParamStk\n");
}

CParamStk::CParamStk(CParamStk & eff)
{
  tabval = NULL;
  chunk = NULL;
  ischunk = eff.ischunk;
  length = eff.length;


  if(eff.ischunk)
  {
    if(eff.chunk)
    {
      chunk = new char[eff.length];
      memcpy(chunk,eff.chunk,eff.length);
    }
  }
  else
  {
    if(eff.tabval)
    {
      tabval = new float[(int)eff.tabval[0]+1];

      for(int i =0; i<=eff.tabval[0];i++)
        tabval[i] = eff.tabval[i];
    }  
  }
}
void CParamStk::loadFromMem(CEffect * ceff)
{
  if(ceff->pEffect->flags & effFlagsProgramChunks)
  {
    if(chunk)
    {
      ceff->EffSetChunk(chunk,length,true);
    }
  }
  else
  {
    if(tabval)
    {
    int k,l = (int)tabval[0];
    for( k = 0; k < l;k++)
      ceff->EffSetParameter(k,tabval[k+1]);
    }
  }
}


void CParamStk::saveToMem(CEffect * ceff)
{
  if(ceff->pEffect->flags & effFlagsProgramChunks)
  {
        if(tabval)
          delete tabval;

        tabval = NULL;

        void * chunke = NULL;
        long lg = ceff->EffGetChunk(&chunke,true);
        if(lg != length)
        {
          if( chunk )
            delete chunk;
          chunk = NULL;
        }

        if(!chunk)
          chunk = new char [lg];

        ASSERT(chunk);
        memcpy(chunk,chunke,lg);
        length = lg;
        ischunk = true;

  }
  else
  {
        int k,l = ceff->pEffect->numParams;

        //par sécurité on kill les chunk si yen a
        if(chunk)
          delete chunk;

        chunk = NULL;
        length = 0;
        ischunk = false;


        //si c'est pas le bon tableau on kill
        if(tabval && tabval[0] != l)
        {delete [] tabval;
        tabval = NULL;
        }

        //si pas de tableau
        if(!tabval)
        {tabval = new float[l+1];
        tabval[0] = (float)l;
        }

        //on sauvegarde l'effet
        for( k = 0; k < l;k++)
          tabval[k+1] = ceff->EffGetParameter(k);
  }
}

CSmpEffect::CSmpEffect(CVSTHost *pHost)
  : CEffect(pHost)
{
InitializeCriticalSection(&cs);
effectuse = 0;
pEditWnd = NULL;
pParmWnd = NULL;
wChnMask = 0xffff;
sChunkFile = _T("");
}

/*****************************************************************************/
/* ~CSmpEffect : destructor                                                  */
/*****************************************************************************/

CSmpEffect::~CSmpEffect()
{
  DeleteCriticalSection(&cs);
}

/*****************************************************************************/
/* Load : loads a plugin                                                     */
/*****************************************************************************/

bool CSmpEffect::Load(const char *name)
{
if (!CEffect::Load(name))               /* do basic stuff                    */
{
  CString buf;
  buf.Format("Ce plug-ins n'a pas put etre chargé : \n%s",name);
  AfxMessageBox(buf);
  return false;
}

//if(!(pEffect->flags & kPlugCategShell))
  DefParam.saveToMem(this);

return true;
}

/*****************************************************************************/
/* Unload : removes an effect from memory                                    */
/*****************************************************************************/

bool CSmpEffect::Unload()
{
bool bRC = false;

__try
  {
  bRC = CEffect::Unload();
  }
__except (EvalException(GetExceptionCode()))
  {
       // no code here; isn't executed
  }

return bRC;
}

/*****************************************************************************/
/* OnSizeEditorWindow : called to resize the editor window                   */
/*****************************************************************************/

void CSmpEffect::OnSizeEditorWindow(long width, long height)
{
if (pEditWnd)
  pEditWnd->SetEffSize((int)width, (int)height);
}

/*****************************************************************************/
/* OnUpdateDisplay() _ called when the display is updated                    */
/*****************************************************************************/

bool CSmpEffect::OnUpdateDisplay()
{
if (pEditWnd)
  pEditWnd->Update();
return CEffect::OnUpdateDisplay();
}

/*****************************************************************************/
/* EvalException : handle exceptions in called effect                        */
/*****************************************************************************/

int CSmpEffect::EvalException(int n_except)
{
TRACE0("Ouch! Ouch! Effect just died and wants to take us with it! OUCH!\n");

//((CChainApp *)AfxGetApp())->FullStop();
return EXCEPTION_CONTINUE_SEARCH;       /* go to next handler                */
}

/*****************************************************************************/
/* EffDispatch : called for any effect method to be sent                     */
/*****************************************************************************/

long CSmpEffect::EffDispatch(long opCode, long index, long value, void *ptr, float opt)
{
#if defined(_DEBUG) || defined(_DEBUGFILE)
switch (opCode)
  {
  case effOpen :
    TRACE0("effDispatch: effOpen\n");
    break;
  case effClose :
    TRACE0("effDispatch: effClose\n");
    break;
  case effSetProgram :
    TRACE0("effDispatch: effSetProgram\n");
    break;
  case effGetProgram :
    TRACE0("effDispatch: effGetProgram\n");
    break;
  case effSetProgramName :
    TRACE0("effDispatch: effSetProgramName\n");
    break;
  case effGetProgramName :
    TRACE0("effDispatch: effGetProgramName\n");
    break;
  case effGetParamLabel :
    TRACE0("effDispatch: effGetParamLabel\n");
    break;
  case effGetParamDisplay :
    TRACE0("effDispatch: effGetParamDisplay\n");
    break;
  case effGetParamName :
    TRACE0("effDispatch: effGetParamName\n");
    break;
  case effGetVu :
    TRACE0("effDispatch: effGetVu\n");
    break;
  case effSetSampleRate :
    TRACE0("effDispatch: effSetSampleRate\n");
    break;
  case effSetBlockSize :
    TRACE0("effDispatch: effSetBlockSize\n");
    break;
  case effMainsChanged :
    TRACE0("effDispatch: effMainsChanged\n");
    break;
  case effEditGetRect :
    TRACE0("effDispatch: effEditGetRect\n");
    break;
  case effEditOpen :
    TRACE0("effDispatch: effEditOpen\n");
    break;
  case effEditClose :
    TRACE0("effDispatch: effEditClose\n");
    break;
  case effEditDraw :
    TRACE0("effDispatch: effEditDraw\n");
    break;
  case effEditMouse :
    TRACE0("effDispatch: effEditMouse\n");
    break;
  case effEditKey :
    TRACE0("effDispatch: effEditKey\n");
    break;
  case effEditIdle :
    //TRACE0("effDispatch: effEditIdle\n");
    break;
  case effEditTop :
    TRACE0("effDispatch: effEditTop\n");
    break;
  case effEditSleep :
    TRACE0("effDispatch: effEditSleep\n");
    break;
  case effIdentify :
    TRACE0("effDispatch: effIdentify\n");
    break;
  case effGetChunk :
    TRACE0("effDispatch: effGetChunk\n");
    break;
  case effSetChunk :
    TRACE0("effDispatch: effSetChunk\n");
    break;
  case effProcessEvents :
    TRACE0("effDispatch: effProcessEvents\n");
    break;
  case effCanBeAutomated :
    TRACE0("effDispatch: effCanBeAutomated\n");
    break;
  case effString2Parameter :
    TRACE0("effDispatch: effString2Parameter\n");
    break;
  case effGetNumProgramCategories :
    TRACE0("effDispatch: effGetNumProgramCategories\n");
    break;
  case effGetProgramNameIndexed :
    TRACE0("effDispatch: effGetProgramNameIndexed\n");
    break;
  case effCopyProgram :
    TRACE0("effDispatch: effCopyProgram\n");
    break;
  case effConnectInput :
    TRACE0("effDispatch: effConnectInput\n");
    break;
  case effConnectOutput :
    TRACE0("effDispatch: effConnectOutput\n");
    break;
  case effGetInputProperties :
    TRACE0("effDispatch: effGetInputProperties\n");
    break;
  case effGetOutputProperties :
    TRACE0("effDispatch: effGetOutputProperties\n");
    break;
  case effGetPlugCategory :
    TRACE0("effDispatch: effGetPlugCategory\n");
    break;
  case effGetCurrentPosition :
    TRACE0("effDispatch: effGetCurrentPosition\n");
    break;
  case effGetDestinationBuffer :
    TRACE0("effDispatch: effGetDestinationBuffer\n");
    break;
  case effOfflineNotify :
    TRACE0("effDispatch: effOfflineNotify\n");
    break;
  case effOfflinePrepare :
    TRACE0("effDispatch: effOfflinePrepare\n");
    break;
  case effOfflineRun :
    TRACE0("effDispatch: effOfflineRun\n");
    break;
  case effProcessVarIo :
    TRACE0("effDispatch: effProcessVarIo\n");
    break;
  case effSetSpeakerArrangement :
    TRACE0("effDispatch: effSetSpeakerArrangement\n");
    break;
  case effSetBlockSizeAndSampleRate :
    TRACE0("effDispatch: effSetBlockSizeAndSampleRate\n");
    break;
  case effSetBypass :
    TRACE0("effDispatch: effSetBypass\n");
    break;
  case effGetEffectName :
    TRACE0("effDispatch: effGetEffectName\n");
    break;
  case effGetErrorText :
    TRACE0("effDispatch: effGetErrorText\n");
    break;
  case effGetVendorString :
    TRACE0("effDispatch: effGetVendorString\n");
    break;
  case effGetProductString :
    TRACE0("effDispatch: effGetProductString\n");
    break;
  case effGetVendorVersion :
    TRACE0("effDispatch: effGetVendorVersion\n");
    break;
  case effVendorSpecific :
    TRACE0("effDispatch: effVendorSpecific\n");
    break;
  case effCanDo :
    TRACE0("effDispatch: effCanDo\n");
    break;
  case effGetTailSize :
    TRACE0("effDispatch: effGetTailSize\n");
    break;
  case effIdle :
    TRACE0("effDispatch: effIdle\n");
    break;
  case effGetIcon :
    TRACE0("effDispatch: effGetIcon\n");
    break;
  case effSetViewPosition :
    TRACE0("effDispatch: effSetViewPosition\n");
    break;
  case effGetParameterProperties :
    TRACE0("effDispatch: effGetParameterProperties\n");
    break;
  case effKeysRequired :
    TRACE0("effDispatch: effKeysRequired\n");
    break;
  case effGetVstVersion :
    TRACE0("effDispatch: effGetVstVersion\n");
    break;
#if defined(VST_2_1_EXTENSIONS)
    //---from here VST 2.1 extension opcodes---------------------------------------------------------
  case effEditKeyDown :
    TRACE0("effDispatch: effEditKeyDown\n");
    break;
  case effEditKeyUp :
    TRACE0("effDispatch: effEditKeyUp\n");
    break;
  case effSetEditKnobMode :
    TRACE0("effDispatch: effSetEditKnobMode\n");
    break;
  case effGetMidiProgramName :
    TRACE0("effDispatch: effGetMidiProgramName\n");
    break;
  case effGetCurrentMidiProgram :
    TRACE0("effDispatch: effGetCurrentMidiProgram\n");
    break;
  case effGetMidiProgramCategory :
    TRACE0("effDispatch: effGetMidiProgramCategory\n");
    break;
  case effHasMidiProgramsChanged :
    TRACE0("effDispatch: effHasMidiProgramsChanged\n");
    break;
  case effGetMidiKeyName :
    TRACE0("effDispatch: effGetMidiKeyName\n");
    break;
  case effBeginSetProgram :
    TRACE0("effDispatch: effBeginSetProgram\n");
    break;
  case effEndSetProgram :
    TRACE0("effDispatch: effEndSetProgram\n");
    break;
#endif
#if defined(VST_2_3_EXTENSIONS)
    //---from here VST 2.3 extension opcodes---------------------------------------------------------
  case effGetSpeakerArrangement :
    TRACE0("effDispatch: effGetSpeakerArrangement\n");
    break;
  case effShellGetNextPlugin :
    TRACE0("effDispatch: effShellGetNextPlugin\n");
    break;
  case effStartProcess :
    TRACE0("effDispatch: effStartProcess\n");
    break;
  case effStopProcess :
    TRACE0("effDispatch: effStopProcess\n");
    break;
  case effSetTotalSampleToProcess :
    TRACE0("effDispatch: effSetTotalSampleToProcess\n");
    break;
  case effSetPanLaw :
    TRACE0("effDispatch: effSetPanLaw\n");
    break;
  case effBeginLoadBank :
    TRACE0("effDispatch: effBeginLoadBank\n");
    break;
  case effBeginLoadProgram :
    TRACE0("effDispatch: effBeginLoadProgram\n");
    break;
#endif
  }
#endif

long lRC = 0;
//__try
  {
  lRC = CEffect::EffDispatch(opCode, index, value, ptr, opt);
  }
//__except (EvalException(GetExceptionCode()))
  {
       // no code here; isn't executed
  }

return lRC;
}

/*****************************************************************************/
/* OnSetParameterAutomated : called when an effect reports a parm change     */
/*****************************************************************************/

bool CSmpEffect::OnSetParameterAutomated(long index, float value)
{
//CTAF INCONCISNENCE (that was pParmWnd)
if (pParmWnd)                           /* pass it on.                       */
  return pParmWnd->OnSetParameterAutomated(index, value);
return false;
}

/*****************************************************************************/
/* EnterCritical : enter critical section                                    */
/*****************************************************************************/

void CSmpEffect::EnterCritical()
{
EnterCriticalSection(&cs);              /* inhibit others!                   */
}

/*****************************************************************************/
/* LeaveCritical : leave critical section                                    */
/*****************************************************************************/

void CSmpEffect::LeaveCritical()
{
LeaveCriticalSection(&cs);              /* reallow others                    */
}
