//------------------------------------------------------------------------
//-
//- Project     : Use different Controls of VSTGUI
//- Filename    : multifxVST.cpp
//- Created by  : Yvan Grabit
//- Description :
//-
//- © 2000-1999, Steinberg Soft und Hardware GmbH, All Rights Reserved
//------------------------------------------------------------------------
#include "stdafx.h"


#ifndef __multifxVST__
#include "multifxVST.h"
#endif


#ifndef __multifxVSTeditor__
#include "multifxVSTeditor.h"
#endif

/*#include <stdio.h>
#include <string.h>*/

#include "controleurdlg.h"

#include "multifxVSTmain.h"
// global

extern bool oome;

#include "stockeffet.h"
#include "CCVSThost.h"
#include "ChainDlg.h"
#include "multifxVSTEditor.h"
#include "effectwnd.h"
#include "EffectTxTDlg.h"
#include "Chaindlg.h"
#include "maindlg.h"
#include "controleurlst.h"


//-----------------------------------------------------------------------------
multifxVST::multifxVST (audioMasterCallback audioMaster) :
  AudioEffectX (audioMaster, 0, kNumParams+32)
{
  TRACE("multifxVST::multifxVST \n");
  dat = NULL;

  APP.effect     = this;
  APP.current_chaine = 0;
  APP.parameter  = new CParameterLst;
  APP.controleur = new CControleurLst;
  APP.chaine_eff = new CStockEffetLst;
  APP.host       = &theApp.host;
  
  APP.parameter->Init(32);
  APP.controleur->Init(32);

  APP.controleur->Set_APP(&APP);
  APP.parameter->Set_APP(&APP);




  APP.chaine_eff->Set(&APP);

  //setOfflineQualities();
  //programsAreChunks();
  //setInitialDelay();
  //setRealtimeQualities();
  //VST DEF
	setNumInputs (2);
	setNumOutputs (2);
	hasVu (false);
	canMono (true);
	canProcessReplacing ();
  programsAreChunks();
  //canParameterBeAutomated(
	setUniqueID (CCONST ('C', 'T', 'A', 'F'));
  //needIdle();
	// init programs
//	programs = NULL;
  //numPrograms = 0;
 
	editor = new multifxVSTEditor (this);
  ASSERT(editor);
  ((multifxVSTEditor *)editor)->SetAPP(&APP);

  APP.editor = (multifxVSTEditor *)editor;
}


long multifxVST::getChunk(void **data,bool isPreset)
{
  CMemFile f;
  CArchive ar(&f,CArchive::store);
    APP.chaine_eff->SaveParamsToMem(APP.current_chaine);
    APP.chaine_eff->save(ar);

  ar.Close();

  long len =(long) f.GetLength();

  dat = f.Detach();

  *data = dat;


  f.Close();


  return len;
}

long multifxVST::setChunk(void *data,long byteSize,bool isPreset)
{
  CMemFile f(0);
  f.Attach((BYTE *)data,byteSize);
  CArchive ar(&f,CArchive::load);
  APP.chaine_eff->load(ar);
  APP.chaine_eff->LoadParamsFromMem(APP.current_chaine);

  ar.Close();
  f.Close();
  return 1;
}



//-----------------------------------------------------------------------------
multifxVST::~multifxVST ()
{
  //detruit la liste des effets qui reduit le nombre de lien des plug-ins
  //si les plug-iins n'ont plus de lien,ca les détruits
  if(APP.chaine_eff)
  {
    delete APP.chaine_eff;
    APP.chaine_eff = NULL;
  }
  if(APP.controleur)
  {
    delete APP.controleur;
    APP.controleur = NULL;
  }
  if(APP.parameter)
  {
    delete APP.parameter;
    APP.controleur = NULL;
  }
  if(dat)
    delete dat;

  CString buf;buf.Format("DESTROY :: multifxVST(%d) \n", this);  TRACE(buf);

}
void multifxVST::setParameterAutomated(long index ,float value)
{
  AudioEffect::setParameterAutomated(index,value);
}
//------------------------------------------------------------------------
long multifxVST::vendorSpecific (long lArg1, long lArg2, void* ptrArg, float floatArg)
{
	if (editor && lArg1 == 'stCA' && lArg2 == 'Whee')
		return editor->onWheel (floatArg) == true ? 1 : 0;
	else
		return AudioEffectX::vendorSpecific (lArg1, lArg2, ptrArg, floatArg);
}

//-----------------------------------------------------------------------------
void multifxVST::resume ()
{
  APP.chaine_eff->resume(APP.current_chaine);

	/*if (editor)
		((multifxVSTEditor*)editor)->resume ();*/
}

//-----------------------------------------------------------------------------
void multifxVST::suspend ()
{
  /*AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CChainApp * theApp =(CChainApp *) AfxGetApp();*/
  APP.chaine_eff->suspend(APP.current_chaine);

	/*if (editor)
		((multifxVSTEditor*)editor)->suspend ();*/
}

//-----------------------------------------------------------------------------
bool multifxVST::keysRequired ()
{
	if (editor)
		return ((multifxVSTEditor*)editor)->keysRequired ();
	else
		return false;
}

//-----------------------------------------------------------------------------
bool multifxVST::string2parameter (long index, char* text)
{
	bool v = false;
  int val;
	
	switch (index)
	{
//		case kSliderVTag :
		case kSliderHTag :
//		case kKnobTag :
			if (text)
			{
				sscanf (text, "%d", &val);
        float fSliderValue = float(val);
				if (fSliderValue < 0.f)
					fSliderValue = 0.f;
				else if (fSliderValue > 1.f)
					fSliderValue = 1.f;

        APP.current_chaine = float2NBChaine(fSliderValue);

				//Program* ap = programs + curProgram;
				//ap->setValue (fSliderValue);
			}
			v = true;
		break;

		default:
			v = false;
	}

	return v;
}

//-----------------------------------------------------------------------------
void multifxVST::setBlockSize (long size)
{
  /*AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CChainApp * theApp =(CChainApp *) AfxGetApp();*/
  //theApp->chaine_eff.setBlockSize(theApp->current_chaine,size);

  //AF                  AF             AF       chaine_eff->set..
	//host->SetBlockSize(size);
  APP.chaine_eff->SetBlockSize(size);
  AudioEffect::setBlockSize (size);

}

void multifxVST::setSampleRate(float sampleRate)
{
  /*AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CChainApp * theApp =(CChainApp *) AfxGetApp();*/
  //theApp->chaine_eff.setBlockSize(theApp->current_chaine,size);
	APP.chaine_eff->SetSampleRate(sampleRate);
  AudioEffect::setSampleRate (sampleRate);

}

//-----------------------------------------------------------------------------
void multifxVST::setParameter (long index, float value)
{
	if (index == kSliderHTag)
	{
        BOOL b =APP.chaine_eff->m_processing;
        //on arrete de les plugins
        if(b)
          APP.chaine_eff->suspend(APP.current_chaine);

        //on sauvegarde les parametres de l'anciens chaine (de cq plug)
        APP.chaine_eff->SaveParamsToMem(APP.current_chaine);

        APP.current_chaine = float2NBChaine(value);

        //on charge les params de l'autre chaine
        APP.chaine_eff->LoadParamsFromMem(APP.current_chaine);

        //on lance les autres
        if(b)
          APP.chaine_eff->resume(APP.current_chaine);
	}else if((index >= kNumParams) && (index < kNumParams + APP.parameter->GetCount()))
  {
    APP.parameter->setParameter(index-kNumParams,value);
  
  }

  //on met l'affichage a jours
	if (editor)
		((AEffGUIEditor*)editor)->setParameter (index, value);
}

//-----------------------------------------------------------------------------
float multifxVST::getParameter (long index)
{
	float v = 1.0;

	if (index == kSliderHTag )
		v = NBChaine2float(APP.current_chaine);
  else
  {
    v = APP.parameter->getParameter(index-kNumParams);
    //APP.parameter->setParameter(index-kNumParams,value);
  }

	return v;
}

//-----------------------------------------------------------------------------
void multifxVST::getParameterDisplay (long index, char *text)
{
	if (index == kSliderHTag )
		sprintf (text, "%d",APP.current_chaine);
  else
		sprintf (text, "%f",getParameter(index));

}

//-----------------------------------------------------------------------------
void multifxVST::getParameterLabel (long index, char *text)
{
	switch (index)
	{
	case kSliderHTag :
    strcpy (text, "");
		break;

	default :
		strcpy (text, "");
	}
  		//strcpy (text, "-");
}

//-----------------------------------------------------------------------------
void multifxVST::getParameterName (long index, char *text)
{
	if (index == kSliderHTag)
		strcpy (text, "Chain number");
  else
		sprintf (text, "Parameter %d",index - kNumParams +1);
	
  	//	strcpy (text, "-");
}

//-----------------------------------------------------------------------------
void multifxVST::process (float **inputs, float **outputs, long sampleFrames)
{
  /*AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CChainApp * theApp =(CChainApp *) AfxGetApp();*/
  
  APP.chaine_eff->process(APP.current_chaine,inputs,outputs,sampleFrames);
	//outputs = outputs;
}

//-----------------------------------------------------------------------------
void multifxVST::processReplacing (float **inputs, float **outputs, long sampleFrames)
{
  /*AFX_MANAGE_STATE(AfxGetStaticModuleState());
  CChainApp * theApp =(CChainApp *) AfxGetApp();*/
  APP.chaine_eff->processReplace (APP.current_chaine,inputs,outputs,sampleFrames);
}

void multifxVST::close()
{

    CString buf;buf.Format("DEBCLOSE :: multifxVST(%d) \n", this);  TRACE(buf);
    AudioEffectX::close();
    buf.Format("CLOSE :: multifxVST(%d) \n", this);  TRACE(buf);

};



void multifxVST::open()
{

}

