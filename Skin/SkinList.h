// SkinList.h : main header file for the SKINLIST application
//

#if !defined(AFX_SKINLIST_H__BA521C63_8E8D_4B9F_846A_050C3348E57D__INCLUDED_)
#define AFX_SKINLIST_H__BA521C63_8E8D_4B9F_846A_050C3348E57D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "../resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CSkinListApp:
// See SkinList.cpp for the implementation of this class
//

class CSkinListApp : public CWinApp
{
public:
	CSkinListApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkinListApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSkinListApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKINLIST_H__BA521C63_8E8D_4B9F_846A_050C3348E57D__INCLUDED_)
