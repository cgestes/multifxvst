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
//#define _WIN32_DCOM 

BOOL CChainApp::InitInstance()
	{
 	  CWinApp::InitInstance();
    //Enable3dControls();

    HRESULT result = 0;;
    result=CoInitializeEx(NULL,COINIT_MULTITHREADED   | COINIT_SPEED_OVER_MEMORY);
    if(result != S_OK)
    {//RPC_E_CHANGED_MODE 
      TRACE("Ole init error\n");
    }

    //AfxEnableControlContainer();
    //CoInitialize(NULL);//ole init
    //OleInitialize(NULL);
    TRACE("CChainApp::InitInstance()\n");
	  hInstance = m_hInstance;

    
	  return TRUE;//continue running
}

int CChainApp::ExitInstance()
{
  //on uninitialse OLE
  CoUninitialize();

  TRACE("CChainApp::ExitInstance()\n");
  return CWinApp::ExitInstance();
}
