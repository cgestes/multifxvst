//------------------------------------------------------//
//- CopyLeft : CTAF
//-
//- Class WinApp (app)    
//- TODO :            
//-                                                   
//------------------------------------------------------//

#ifndef __multifxVSTmain__
#define __multifxVSTmain__



#pragma once

//#include "stockeffet.h"

extern void* hInstance;

extern class CChainApp theApp;
//#include "vsthost/cvsthost.h"


#include "CCVSTHost.h"

//juste pour initialiser hInstance
class CChainApp : public CWinApp
{
public:
	CChainApp(){return;}
  

// Overrides
public:
	virtual BOOL InitInstance();

  CCVSTHost   host;
  virtual int ExitInstance();
};
#endif