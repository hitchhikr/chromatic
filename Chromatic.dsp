# Microsoft Developer Studio Project File - Name="Chromatic" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Chromatic - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Chromatic.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Chromatic.mak" CFG="Chromatic - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Chromatic - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Chromatic - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Chromatic - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "TmpRelease"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "TmpRelease"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GF /GA /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib wsock32.lib Debug\Projects\ChromaticSup\ChromaticSup.lib comctl32.lib libs\dbghelp.lib libs\libdisasm.lib libs\lua514.lib libs\htmlhelp.lib /nologo /subsystem:windows /pdb:none /machine:I386

!ELSEIF  "$(CFG)" == "Chromatic - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "TmpDebug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "TmpDebug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /FD /GZ /GF /GA /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib wsock32.lib Debug\Projects\ChromaticSup\ChromaticSup.lib comctl32.lib libs\dbghelp.lib libs\lua514.lib libs\libdisasm.lib libs\htmlhelp.lib /nologo /subsystem:windows /pdb:"TmpDebug/Chromatic.pdb" /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Chromatic - Win32 Release"
# Name "Chromatic - Win32 Debug"
# Begin Group "Main"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\AddIns.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\APIDb.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\CmdMenus.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Crypt.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Debugger.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\FilesExtensions.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Globals.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ImageLists.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\IniFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Languages.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Main.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MDI_Childs.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MDI_Form.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Menus.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MiscFunctions.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\MsgCodeMax.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\ProjectsBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Splitters.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\StatusBar.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Toolbars.cpp
# End Source File
# End Group
# Begin Group "Main headers"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\AddIns.h
# End Source File
# Begin Source File

SOURCE=.\Src\APIDb.h
# End Source File
# Begin Source File

SOURCE=.\Src\CmdMenus.h
# End Source File
# Begin Source File

SOURCE=.\Src\ConstCodeMax.h
# End Source File
# Begin Source File

SOURCE=.\Src\ConstMenus.h
# End Source File
# Begin Source File

SOURCE=.\Src\ConstMisc.h
# End Source File
# Begin Source File

SOURCE=.\Src\ConstRes.h
# End Source File
# Begin Source File

SOURCE=.\Src\Crypt.h
# End Source File
# Begin Source File

SOURCE=.\Src\Debugger.h
# End Source File
# Begin Source File

SOURCE=.\Src\FilesExtensions.h
# End Source File
# Begin Source File

SOURCE=.\Src\Globals.h
# End Source File
# Begin Source File

SOURCE=.\Src\ImageLists.h
# End Source File
# Begin Source File

SOURCE=.\Src\IniFiles.h
# End Source File
# Begin Source File

SOURCE=.\Src\Languages.h
# End Source File
# Begin Source File

SOURCE=.\Src\Main.h
# End Source File
# Begin Source File

SOURCE=.\Src\MDI_Childs.h
# End Source File
# Begin Source File

SOURCE=.\Src\MDI_Form.h
# End Source File
# Begin Source File

SOURCE=.\Src\Menus.h
# End Source File
# Begin Source File

SOURCE=.\Src\MiscFunctions.h
# End Source File
# Begin Source File

SOURCE=.\Src\MsgCodeMax.h
# End Source File
# Begin Source File

SOURCE=.\Src\ProjectsBuilder.h
# End Source File
# Begin Source File

SOURCE=.\Src\Splitters.h
# End Source File
# Begin Source File

SOURCE=.\Src\StatusBar.h
# End Source File
# Begin Source File

SOURCE=.\Src\Toolbars.h
# End Source File
# End Group
# Begin Group "Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Classes\CList.h
# End Source File
# Begin Source File

SOURCE=.\Src\Classes\CStr.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Classes\CStr.h
# End Source File
# End Group
# Begin Group "Dialogs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Dlg_About.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_About.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Accelerators.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Accelerators.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_AddIns.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_AddIns.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_API.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_API.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_APIList.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_APIList.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Args.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Args.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_constants.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Constants.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_CreateProject.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_CreateProject.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_DllDef.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_DllDef.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_EnterValue.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_EnterValue.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Favorites.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Favorites.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Filters.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Filters.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_FTP.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_FTP.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_ImageView.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_ImageView.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Menus.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Menus.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_MissingFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_MissingFiles.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Perms.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Perms.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_PrjProps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_PrjProps.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_ProcVarSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_ProcVarSearch.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Properties.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Properties.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Registers.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Registers.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Registry.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_ResProps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_ResProps.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Results.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Results.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_SearchInFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_SearchInFiles.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_SelFiles.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_SelFiles.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Splash.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Strings.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Strings.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Tips.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Tips.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Toolbars.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_Toolbars.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_UserDialogs.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_UserDialogs.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_UserDialogsObjects.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_UserDialogsObjects.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_UserDialogsProps.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_UserDialogsProps.h
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_UserMenusEd.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Dlg_UserMenusEd.h
# End Source File
# End Group
# Begin Group "WALib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Classes\ChromaticSupDLL.h
# End Source File
# Begin Source File

SOURCE=.\Src\Classes\WADock.h
# End Source File
# Begin Source File

SOURCE=.\Src\Classes\WALib.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Classes\WALib.h
# End Source File
# Begin Source File

SOURCE=.\Src\Classes\WALib_WinSock.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Classes\WALib_WinSock.h
# End Source File
# End Group
# Begin Group "Script Engine"

# PROP Default_Filter ""
# Begin Group "Script Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\AddInContextDat.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\AddInContextDat.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\BackDC.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\BackDC.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\Button.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\Button.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\CommonDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\CommonDialog.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\DockingBox.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\DockingBox.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\FileTime.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\FileTime.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\GripBoxDat.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\GripBoxDat.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\ImageList.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\ImageList.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\MiniChildDat.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\MiniChildDat.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\Now.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\Now.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\Rect.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\Rect.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\Screen.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\Screen.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\SplitterDat.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Classes\SplitterDat.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Script_Engine\Constants.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Constants.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Objects_Handler.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Objects_Handler.h
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Scripts.cpp
# End Source File
# Begin Source File

SOURCE=.\Src\Script_Engine\Scripts.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\Src\Chromatic.RES
# End Source File
# End Target
# End Project
