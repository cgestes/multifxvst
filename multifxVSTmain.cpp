#include "stdafx.h"

#ifndef __multifxVST__
#include "multifxVST.h"
#endif

#include "stockeffet.h"
#include "Chaindlg.h"



#include "multifxVSTmain.h"
bool oome = false;


//------------------------------------------------------------------------
// Prototype of the export function main
//------------------------------------------------------------------------
/*AEffect **/long main (audioMasterCallback audioMaster);
//------------------------------------------------------------------------
/*AEffect **/long main (audioMasterCallback audioMaster)
{
  TRACE("main\n"); 
	// Get VST Version
	if (!audioMaster (0, audioMasterVersion, 0, 0, 0, 0))
		return 0;  // old version

	// Create the AudioEffect
	multifxVST* effect = new multifxVST (audioMaster);
	if (!effect)
		return 0;
  TRACE("main2\n"); 
	// Check if no problem in constructor of AGain
	if (oome)
	{
		delete effect;
		return 0;
	}
	return (long)effect->getAeffect ();
}


CChainApp theApp;
void* hInstance =  NULL;

BOOL CChainApp::InitInstance()
	{
 	  CWinApp::InitInstance();
    //Enable3dControls();

    /*AfxEnableControlContainer();
    CoInitialize(NULL);//ole init*/
    TRACE("CChainApp::InitInstance()\n");
	  hInstance = m_hInstance;

	  return TRUE;//continue running
}

int CChainApp::ExitInstance()
{
  TRACE("CChainApp::ExitInstance()\n");
  return CWinApp::ExitInstance();
}
