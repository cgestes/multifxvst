# Microsoft Developer Studio Project File - Name="multifxVST" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=multifxVST - Win32 Release Dyn
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "multifxVST.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "multifxVST.mak" CFG="multifxVST - Win32 Release Dyn"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "multifxVST - Win32 Release Dyn" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "multifxVST - Win32 Release Static" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "multifxVST - Win32 Debug Dyn" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "multifxVST - Win32 Debug Stat" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "multifxVST - Win32 Release Dyn"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\ReleaseC"
# PROP BASE Intermediate_Dir ".\ReleaseC"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\ReleaseC"
# PROP Intermediate_Dir ".\ReleaseC"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /I "E:\CTAF\Cpp\Guitaromatic" /Zi /W3 /Og /Ob2 /Oi /Ot /D "WIN32" /D "_RELEASE" /D "_WINDOWS" /D "_USRDLL" /D "_AFXDLL" /Yc"stdafx.h" /Fp"$(IntDir)/$(TargetName).pch" /Fo"$(IntDir)/" /Fd".\ReleaseC/" /TP /c /GA /GX 
# ADD CPP /nologo /MD /I "E:\CTAF\Cpp\Guitaromatic" /Zi /W3 /Og /Ob2 /Oi /Ot /D "WIN32" /D "_RELEASE" /D "_WINDOWS" /D "_USRDLL" /D "_AFXDLL" /Yc"stdafx.h" /Fp"$(IntDir)/$(TargetName).pch" /Fo"$(IntDir)/" /Fd".\ReleaseC/" /TP /c /GA /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /tlb".\Debug\multifxVST.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /tlb".\Debug\multifxVST.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_AFXDLL" /d "_DEBUG" 
# ADD RSC /l 1033 /d "_AFXDLL" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:"VstPlugins\multifxVST_rd.dll" /incremental:no /nodefaultlib:"LIBCMT.lib" /def:".\multifxVST.def" /pdb:"ReleaseC\$(ProjectName).pdb" /pdbtype:sept /subsystem:windows /opt:ref /opt:icf /implib:".\ReleaseC/MultifxVST.lib" 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:"VstPlugins\multifxVST_rd.dll" /incremental:no /nodefaultlib:"LIBCMT.lib" /def:".\multifxVST.def" /pdb:"ReleaseC\$(ProjectName).pdb" /pdbtype:sept /subsystem:windows /opt:ref /opt:icf /implib:".\ReleaseC/MultifxVST.lib" 

!ELSEIF  "$(CFG)" == "multifxVST - Win32 Release Static"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir ".\$(ConfigurationName)"
# PROP BASE Intermediate_Dir ".\$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ".\$(ConfigurationName)"
# PROP Intermediate_Dir ".\$(ConfigurationName)"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /Og /Ob2 /Oi /Ot /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_RELEASE" /Yc"stdafx.h" /Fp"$(IntDir)/$(TargetName).pch" /Fo"$(IntDir)/" /Fd".\$(ConfigurationName)/" /c /GA /GX 
# ADD CPP /nologo /MT /W3 /Og /Ob2 /Oi /Ot /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_RELEASE" /Yc"stdafx.h" /Fp"$(IntDir)/$(TargetName).pch" /Fo"$(IntDir)/" /Fd".\$(ConfigurationName)/" /c /GA /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /tlb".\Debug\multifxVST.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /tlb".\Debug\multifxVST.tlb" /win32 
# ADD BASE RSC /l 1033 /d "_RELEASE" /d "_AFXDLL" 
# ADD RSC /l 1033 /d "_RELEASE" /d "_AFXDLL" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:"VstPlugins\multifxVST_rs.dll" /incremental:no /def:".\multifxVST.def" /pdb:"ReleaseC\$(ProjectName).pdb" /pdbtype:sept /subsystem:windows /opt:ref /opt:noicf /release /implib:".\Release static/multifxVST.lib" 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:"VstPlugins\multifxVST_rs.dll" /incremental:no /def:".\multifxVST.def" /pdb:"ReleaseC\$(ProjectName).pdb" /pdbtype:sept /subsystem:windows /opt:ref /opt:noicf /release /implib:".\Release static/multifxVST.lib" 

!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\Debug"
# PROP BASE Intermediate_Dir ".\Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\Debug"
# PROP Intermediate_Dir ".\Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /ZI /W3 /Od /G5 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_AFXDLL" /YX /Fp"$(IntDir)/$(TargetName).pch" /Fo"$(IntDir)/" /Fd"$(IntDir)/vc70.pdb" /GZ /c /GX 
# ADD CPP /nologo /MDd /ZI /W3 /Od /G5 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_AFXDLL" /YX /Fp"$(IntDir)/$(TargetName).pch" /Fo"$(IntDir)/" /Fd"$(IntDir)/vc70.pdb" /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /tlb".\Debug\multifxVST.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /tlb".\Debug\multifxVST.tlb" /win32 
# ADD BASE RSC /l 1036 /d "_AFXDLL" /d "_DEBUG" 
# ADD RSC /l 1036 /d "_AFXDLL" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:"VstPlugins\multifxVST_dd.dll" /incremental:no /nodefaultlib:"LIBCMTD.lib" /def:".\multifxVST.def" /debug /pdb:".\Debug\$(ProjectName).pdb" /pdbtype:sept /subsystem:windows /implib:".\Debug/multifxVST.lib" 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:"VstPlugins\multifxVST_dd.dll" /incremental:no /nodefaultlib:"LIBCMTD.lib" /def:".\multifxVST.def" /debug /pdb:".\Debug\$(ProjectName).pdb" /pdbtype:sept /subsystem:windows /implib:".\Debug/multifxVST.lib" 

!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir ".\$(ConfigurationName)"
# PROP BASE Intermediate_Dir ".\$(ConfigurationName)"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir ".\$(ConfigurationName)"
# PROP Intermediate_Dir ".\$(ConfigurationName)"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /ZI /W3 /Od /G5 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /YX /Fp"$(IntDir)/$(TargetName).pch" /Fo"$(IntDir)/" /Fd"$(IntDir)/vc70.pdb" /GZ /c /GX 
# ADD CPP /nologo /MTd /ZI /W3 /Od /G5 /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /YX /Fp"$(IntDir)/$(TargetName).pch" /Fo"$(IntDir)/" /Fd"$(IntDir)/vc70.pdb" /GZ /c /GX 
# ADD BASE MTL /nologo /D"_DEBUG" /tlb".\Debug\multifxVST.tlb" /win32 
# ADD MTL /nologo /D"_DEBUG" /tlb".\Debug\multifxVST.tlb" /win32 
# ADD BASE RSC /l 1036 /d "_AFXDLL" /d "_DEBUG" 
# ADD RSC /l 1036 /d "_AFXDLL" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo 
# ADD BSC32 /nologo 
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:"VstPlugins\MultifxVST_ds.dll" /incremental:no /def:".\multifxVST.def" /debug /pdb:".\Debug\$(ProjectName).pdb" /pdbtype:sept /subsystem:windows /implib:".\Debug/MultifxVST.lib" 
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /out:"VstPlugins\MultifxVST_ds.dll" /incremental:no /def:".\multifxVST.def" /debug /pdb:".\Debug\$(ProjectName).pdb" /pdbtype:sept /subsystem:windows /implib:".\Debug/MultifxVST.lib" 

!ENDIF

# Begin Target

# Name "multifxVST - Win32 Release Dyn"
# Name "multifxVST - Win32 Release Static"
# Name "multifxVST - Win32 Debug Dyn"
# Name "multifxVST - Win32 Debug Stat"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ControleurLst.cpp
# End Source File
# Begin Source File

SOURCE=.\ControleurLst.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\stdafx.h
# End Source File
# Begin Source File

SOURCE=.\stockeffet.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\stockeffet.h
# End Source File
# End Group
# Begin Group "Fenetre VST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\ChainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ChainDlg.h
# End Source File
# Begin Source File

SOURCE=.\ControleurDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ControleurDlg.h
# End Source File
# Begin Source File

SOURCE=.\EffectTxTDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\EffectTxTDlg.h
# End Source File
# Begin Source File

SOURCE=.\EffectWnd.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\EffectWnd.h
# End Source File
# Begin Source File

SOURCE=.\MainDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MainDlg.h
# End Source File
# End Group
# Begin Group "Maintenance"

# PROP Default_Filter ""
# Begin Source File

SOURCE=".\End to End\multifxVST\INFO.txt"
# End Source File
# Begin Source File

SOURCE=".\End to End\multifxVST\TODO LIST.txt"
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter ".rc"
# Begin Source File

SOURCE=.\res\bmp10001.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\ColumnHeaderEnd.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\ColumnHeaderSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\ColumnHeaderStart.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\HorizontalScrollBarLeftArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\HorizontalScrollBarRightArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\HorizontalScrollBarSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\HorizontalScrollBarThumb.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\HoverButton.bmp
# End Source File
# Begin Source File

SOURCE=.\multifxVST.def
# End Source File
# Begin Source File

SOURCE=.\multifxVST.rc
# End Source File
# Begin Source File

SOURCE=.\skinslider\track_back.bmp
# End Source File
# Begin Source File

SOURCE=.\skinslider\track_tick.bmp
# End Source File
# Begin Source File

SOURCE=.\skinslider\track_ticksel.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\VerticleScrollbarBottom.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\VerticleScrollBarDownArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\VerticleScrollBarSpan.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\VerticleScrollBarThumb.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\VerticleScrollbarTop.bmp
# End Source File
# Begin Source File

SOURCE=.\Skin\ListCtrlSkin\VerticleScrollBarUpArrow.bmp
# End Source File
# End Group
# Begin Group "Skin"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Skin\HoverButton.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\Skin\HoverButton.h
# End Source File
# Begin Source File

SOURCE=.\Skin\SkinHeaderCtrl.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\Skin\SkinHeaderCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Skin\SkinHorizontalScrollbar.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\Skin\SkinHorizontalScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\Skin\SkinListCtrl.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\Skin\SkinListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\Skin\SkinVerticleScrollbar.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\Skin\SkinVerticleScrollbar.h
# End Source File
# Begin Source File

SOURCE=.\skinslider\ZipBitmap.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\skinslider\ZipBitmap.h
# End Source File
# Begin Source File

SOURCE=.\skinslider\ZipSliderCtl.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\skinslider\ZipSliderCtl.h
# End Source File
# End Group
# Begin Group "MultifxVST"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\multifxVST.cpp
# End Source File
# Begin Source File

SOURCE=.\multifxVST.h
# End Source File
# Begin Source File

SOURCE=.\multifxVSTeditor.cpp
# End Source File
# Begin Source File

SOURCE=.\multifxVSTeditor.h
# End Source File
# Begin Source File

SOURCE=.\multifxVSTmain.cpp
# End Source File
# Begin Source File

SOURCE=.\multifxVSTmain.h
# End Source File
# Begin Group "VST Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\AEffect.h
# End Source File
# Begin Source File

SOURCE=.\aeffectx.h
# End Source File
# Begin Source File

SOURCE=.\vstsdk2.3\vstsdk2.3\source\common\AEffEditor.hpp
# End Source File
# Begin Source File

SOURCE=.\aeffguieditor.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\aeffguieditor.h
# End Source File
# Begin Source File

SOURCE=.\AudioEffect.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\AudioEffect.hpp
# End Source File
# Begin Source File

SOURCE=.\audioeffectx.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\audioeffectx.h
# End Source File
# Begin Source File

SOURCE=.\vstcontrols.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\vstcontrols.h
# End Source File
# Begin Source File

SOURCE=.\vstgui.cpp
# End Source File
# Begin Source File

SOURCE=.\vstgui.h
# End Source File
# End Group
# Begin Group "VSTHost"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\CCVSTHost.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\CCVSTHost.h
# End Source File
# Begin Source File

SOURCE=.\vsthost\CVSTHost.cpp

!IF  "$(CFG)" == "multifxVST - Win32 Debug Dyn"

# ADD CPP /nologo /GZ /GX 
!ELSEIF  "$(CFG)" == "multifxVST - Win32 Debug Stat"

# ADD CPP /nologo /GZ /GX 
!ENDIF

# End Source File
# Begin Source File

SOURCE=.\vsthost\CVSTHost.h
# End Source File
# Begin Source File

SOURCE=.\vsthost\SmpEffect.cpp
# End Source File
# Begin Source File

SOURCE=.\vsthost\SmpEffect.h
# End Source File
# End Group
# End Group
# End Target
# End Project

