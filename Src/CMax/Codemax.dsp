# Microsoft Developer Studio Project File - Name="codemax" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=codemax - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Codemax.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Codemax.mak" CFG="codemax - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "codemax - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "codemax - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "codemax - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W4 /GX /O2 /D "NDEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /o "NUL" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "_MBCS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib version.lib /nologo /subsystem:windows /dll /pdb:none /machine:I386 /out:"..\..\Debug\cmaxXX.dll"

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W4 /Gm /GX /ZI /Od /D "_DEBUG" /D "_WINDOWS" /D "WIN32" /D "_MBCS" /FD /c
# SUBTRACT CPP /Fr /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /o "NUL" /win32
# SUBTRACT MTL /mktyplib203
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_MBCS"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comctl32.lib version.lib /nologo /subsystem:windows /dll /pdb:none /debug /machine:I386 /out:"..\..\Debug\cmaxXX.dll"

!ENDIF 

# Begin Target

# Name "codemax - Win32 Release"
# Name "codemax - Win32 Debug"
# Begin Group "Includes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BUFFER.H
# End Source File
# Begin Source File

SOURCE=.\CEDIT.H
# End Source File
# Begin Source File

SOURCE=.\CODEMAX.H
# End Source File
# Begin Source File

SOURCE=.\DRAGDROP.H
# End Source File
# Begin Source File

SOURCE=.\EDITDLGS.H
# End Source File
# Begin Source File

SOURCE=.\EDITSEL.H
# End Source File
# Begin Source File

SOURCE=.\EDITVIEW.H
# End Source File
# Begin Source File

SOURCE=.\HKEYCTRL.H
# End Source File
# Begin Source File

SOURCE=.\MISC.H
# End Source File
# Begin Source File

SOURCE=.\PRECOMP.H
# End Source File
# Begin Source File

SOURCE=.\regexp.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=.\resource2.h
# End Source File
# Begin Source File

SOURCE=.\UNDO.H
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\codemax.rc
# End Source File
# Begin Source File

SOURCE=.\CODEMAX.RC2
# End Source File
# Begin Source File

SOURCE=.\hsplit.cur
# End Source File
# Begin Source File

SOURCE=.\hvsplit.cur
# End Source File
# Begin Source File

SOURCE=.\leftmarg.cur
# End Source File
# Begin Source File

SOURCE=.\vsplit.cur
# End Source File
# End Group
# Begin Group "Source"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\bookmark.cpp
# End Source File
# Begin Source File

SOURCE=.\BreakPoint.cpp
# End Source File
# Begin Source File

SOURCE=.\buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\cedit.cpp
# End Source File
# Begin Source File

SOURCE=.\ceditmsg.cpp
# End Source File
# Begin Source File

SOURCE=.\cmdarry.cpp
# End Source File
# Begin Source File

SOURCE=.\dragdrop.cpp
# End Source File
# Begin Source File

SOURCE=.\editcmd1.cpp
# End Source File
# Begin Source File

SOURCE=.\editdlgs.cpp
# End Source File
# Begin Source File

SOURCE=.\editsel.cpp
# End Source File
# Begin Source File

SOURCE=.\editview.cpp
# End Source File
# Begin Source File

SOURCE=.\exports.cpp
# End Source File
# Begin Source File

SOURCE=.\findrepl.cpp
# End Source File
# Begin Source File

SOURCE=.\hkeyctrl.cpp
# End Source File
# Begin Source File

SOURCE=.\hotkey.cpp
# End Source File
# Begin Source File

SOURCE=.\language.cpp
# End Source File
# Begin Source File

SOURCE=.\line.cpp
# End Source File
# Begin Source File

SOURCE=.\misc.cpp
# End Source File
# Begin Source File

SOURCE=.\msgpump.cpp
# End Source File
# Begin Source File

SOURCE=.\print.cpp
# End Source File
# Begin Source File

SOURCE=.\props.cpp
# End Source File
# Begin Source File

SOURCE=.\regexp.cpp

!IF  "$(CFG)" == "codemax - Win32 Release"

# ADD CPP /W1

!ELSEIF  "$(CFG)" == "codemax - Win32 Debug"

# ADD CPP /W4 /GX

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\splitter.cpp
# End Source File
# Begin Source File

SOURCE=.\syntax.cpp
# End Source File
# Begin Source File

SOURCE=.\undo.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\codemax.def
# End Source File
# End Target
# End Project
