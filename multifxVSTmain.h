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

extern class CGratOmaTicApp theApp;
//#include "vsthost/cvsthost.h"


#include "CCVSTHost.h"

//juste pour initialiser hInstance
class CGratOmaTicApp : public CWinApp
{
public:
	CGratOmaTicApp(){return;}
  

// Overrides
public:
	virtual BOOL InitInstance();

  CCVSTHost   host;
  virtual int ExitInstance();
};
#endif