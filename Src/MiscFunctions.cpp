// -----------------------------------------------------------------------
// Chromatic
// Integrated Development Environment
//
// Copyright (C) 2001-2010 Franck Charlet.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL FRANCK CHARLET OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
// -----------------------------------------------------------------------
// MiscFunctions.cpp: Misc functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "Splitters.h"
#include "Statusbar.h"
#include "Debugger.h"
#include "Languages.h"
#include "MsgCodeMax.h"
#include "ConstMenus.h"
#include "ConstCodeMax.h"
#include "ConstMisc.h"
#include "CmdMenus.h"
#include "Menus.h"
#include "ProjectsBuilder.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Toolbars.h"
#include "Dlg_Args.h"
#include "Dlg_Results.h"
#include "Script_Engine/Scripts.h"

// -----------------------------------------------------------------------
// Variables
CStr GlobalRetVal;
int CursorDisabled;

// -----------------------------------------------------------------------
// Change relative paths to absolute
CStr ChangeRelativePaths(CStr Path, CStr SubsidDir)
{
    CStr CurrentFilePath;
    CStr ReturnValue;

    Path = StringReplace(Path, "[ROOTDIR]", Dirs[DIR_ROOT], 1, -1, Text_Compare);
    CurrentFilePath = GetCurrentFileDir();
    // Take care of any currently opened file if SubsidDir isn't empty (projects loading)
    if((CurrentFilePath.Len() != 0) && (SubsidDir == ""))
    {
        Path = StringReplace(Path, "[SOURCEDIR]", CurrentFilePath, 1, -1, Text_Compare);
    }
    else
    {
        if (SubsidDir != "")
        {
            Path = StringReplace(Path, "[SOURCEDIR]", SubsidDir, 1, -1, Text_Compare);
        }
    }
    Path = StringReplace(Path, "[BINDIR]", Dirs[DIR_BIN], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[INCLUDEDIR]", Dirs[DIR_INCLUDE], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[LIBDIR]", Dirs[DIR_LIB], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[HELPDIR]", Dirs[DIR_HELP], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[TOOLSDIR]", Dirs[DIR_TOOLS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[WINDIR]", Dirs[DIR_WIN], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[WINSYSDIR]", Dirs[DIR_WINSYS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[TEMPDIR]", Dirs[DIR_TEMP], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[TEMPLATESDIR]", Dirs[DIR_TEMPLATES], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[PROJECTSDIR]", Dirs[DIR_PROJECTS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[MAINHELPDIR]", Dirs[DIR_MAINHELP], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[SNIPPETSDIR]", Dirs[DIR_SNIPPETS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[DATABASESDIR]", Dirs[DIR_DATABASES], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[SKINSDIR]", Dirs[DIR_SKINS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[LANGSCRIPTSDIR]", Dirs[DIR_LANGSCRIPTS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[LANGUAGESDIR]", Dirs[DIR_LANGUAGES], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[DOWNLOADSDIR]", Dirs[DIR_DOWNLOADS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[ADDINSDIR]", Dirs[DIR_ADDINS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[CONFIGDIR]", Dirs[DIR_CONFIG], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[MENUSDIR]", Dirs[DIR_MENUS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[WIZARDSDIR]", Dirs[DIR_WIZARDS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[FILTERSDIR]", Dirs[DIR_FILTERS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[EXTRACODEDIR]", Dirs[DIR_EXTRACODE], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[SCRIPTSDIR]", Dirs[DIR_SCRIPTS], 1, -1, Text_Compare);
    Path = StringReplace(Path, "[MAINDIR]", Dirs[DIR_MAIN], 1, -1, Text_Compare);
    ReturnValue = Path;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Remove relative paths
CStr RemoveRelativePaths(CStr Path)
{
    CStr BufString;
    CStr ReturnValue;
    
    BufString = Dirs[DIR_INCLUDE] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_BIN] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_LIB] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_HELP] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_ROOT] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_WINSYS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_TEMP] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_WIN] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_SNIPPETS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_MAINHELP] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_PROJECTS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_TOOLS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_DATABASES] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_SKINS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_LANGSCRIPTS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_LANGUAGES] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_DOWNLOADS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_ADDINS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_CONFIG] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_MENUS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_WIZARDS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_SCRIPTS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_FILTERS] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_EXTRACODE] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    BufString = Dirs[DIR_MAIN] + (CStr) "\\";
    Path = StringReplace(Path, BufString, "", 1, -1, Text_Compare);
    ReturnValue = Path;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Change absolute paths to relative
CStr ChangeAbsolutePaths(CStr Path, long AvoidSource)
{
    CStr CurrentFileD;
    CStr BufString;
    CStr ReturnValue;

    if(strlen(Dirs[DIR_INCLUDE]))
    {
        BufString = Dirs[DIR_INCLUDE] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[INCLUDEDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_BIN]))
    {
        BufString = Dirs[DIR_BIN] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[BINDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_LIB]))
    {
        BufString = Dirs[DIR_LIB] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[LIBDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_HELP]))
    {
        BufString = Dirs[DIR_HELP] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[HELPDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_ROOT]))
    {
        BufString = Dirs[DIR_ROOT] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[ROOTDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_WINSYS]))
    {
        BufString = Dirs[DIR_WINSYS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[WINSYSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_TEMP]))
    {
        BufString = Dirs[DIR_TEMP] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[TEMPDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_WIN]))
    {
        BufString = Dirs[DIR_WIN] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[WINDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_DATABASES]))
    {
        BufString = Dirs[DIR_DATABASES] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[DATABASESDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_SKINS]))
    {
        BufString = Dirs[DIR_SKINS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[SKINSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_LANGSCRIPTS]))
    {
        BufString = Dirs[DIR_LANGSCRIPTS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[LANGSCRIPTSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_LANGUAGES]))
    {
        BufString = Dirs[DIR_LANGUAGES] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[LANGUAGESDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_DOWNLOADS]))
    {
        BufString = Dirs[DIR_DOWNLOADS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[DOWNLOADSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_ADDINS]))
    {
        BufString = Dirs[DIR_ADDINS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[ADDINSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_MENUS]))
    {
        BufString = Dirs[DIR_MENUS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[MENUSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_WIZARDS]))
    {
        BufString = Dirs[DIR_WIZARDS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[WIZARDSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_SCRIPTS]))
    {
        BufString = Dirs[DIR_SCRIPTS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[SCRIPTSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_FILTERS]))
    {
        BufString = Dirs[DIR_FILTERS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[FILTERSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_EXTRACODE]))
    {
        BufString = Dirs[DIR_EXTRACODE] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[EXTRACODEDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_CONFIG]))
    {
        BufString = Dirs[DIR_CONFIG] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[CONFIGDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_SNIPPETS]))
    {
        BufString = Dirs[DIR_SNIPPETS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[SNIPPETSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_MAINHELP]))
    {
        BufString = Dirs[DIR_MAINHELP] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[MAINHELPDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_TEMPLATES]))
    {
        BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[TEMPLATESDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_PROJECTS]))
    {
        BufString = Dirs[DIR_PROJECTS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[PROJECTSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_TOOLS]))
    {
        BufString = Dirs[DIR_TOOLS] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[TOOLSDIR]\\", 1, -1, Text_Compare);
    }
    if(strlen(Dirs[DIR_MAIN]))
    {
        BufString = Dirs[DIR_MAIN] + (CStr) "\\";
        Path = StringReplace(Path, BufString, "[MAINDIR]\\", 1, -1, Text_Compare);
    }
    CurrentFileD = GetCurrentFileDir();
    if(CurrentFileD.Len() != 0 && (AvoidSource == 0))
    {
        if(strcmp(CurrentFileD.Right(1).Get_String(), "\\") != 0) CurrentFileD = CurrentFileD + "\\";
        Path = StringReplace(Path, CurrentFileD, "[SOURCEDIR]\\", 1, -1, Text_Compare);
    }
    ReturnValue = Path;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the working directory
void SetCurDir(HWND hWnd)
{
    CStr TmpCurDir;

    TmpCurDir = ComDlgBrowseForFolder(hWnd, "Select the working directory...");
    if(TmpCurDir.Len() != 0)
    {
        if(strcmp(TmpCurDir.Right(1).Get_String(), "\\") == 0) TmpCurDir = TmpCurDir.Left(TmpCurDir.Len() - 1);
        CurrentDir = TmpCurDir;
        ChDrive(CurrentDir);
        ChDir(CurrentDir);
        IniWriteKey("Dirs", "WORKDIR", CurrentDir, MainIniFile);
    }
}

// -----------------------------------------------------------------------
// Run an user command already decoded
void RunUserCommandRec(CStr CommandRec)
{
    CStr TmpCom;
    
    TmpCom = TranslateUserCmd(CommandRec);
    if(FRMArgsEntered == 1)
    {
        FRMArgsEntered = 0;
        return;
    }
    TmpCom = StringReplace(TmpCom, "\t", " ", 1, -1, Binary_Compare);
    Exec(TmpCom, StringReplace(TmpCom, "$", " ", 1, -1, Binary_Compare), 0);
}

// -----------------------------------------------------------------------
// Execute a shell command
void Exec(CStr CmdName, CStr CmdFakeName, long IsInHelp)
{
    CStr LocalCmdName;
    long *CmdParsed = 0;
    CStr LangComm;
    CStr TempReArgs;
    CStr TempPrep;
    long UserCmdNbr = 0;
    CStr UserCmdErr;
    long CurrentPosPrep = 0;
    long *ArrayPrep = 0;
    CStr PrepLine;
    CStr Args;
    CStr VideStr;
    int i = 0;
    int Script_Return_Code;
    long RunWithsnap = 0;
    long RunPlugIn = 0;
    long RunCatch = 0;
    long RunHtml = 0;
    long RunDocu = 0;
    long RunBuiltin = 0;
    long RunScript = 0;
    HMODULE PlugDllHandle = 0;
    FARPROC PlugAddr = 0;
    CStr TrimedCommand;
    long DBlockBeg = 0;
    long DBlockEnd = 0;
    CStr CurSourceText;
    CStr CurSourceTextBlock;
    CStr BufString;
    CStr LUA_Args;
    
    LocalCmdName = StringReplace(CmdName, "\t", " ", 1, -1, Binary_Compare);
    LocalCmdName = LocalCmdName.Trim();
    if(LocalCmdName.Len() > 6)
    {
        BufString = LocalCmdName.Left(5).Upper_Case();
        if(BufString == "SNAP ")
        {
            LocalCmdName = LocalCmdName.Mid(6);
            RunWithsnap = 1;
        }
        else if(BufString == "PLUG ")
        {
            LocalCmdName = LocalCmdName.Mid(6);
            RunPlugIn = 1;
        }
        else if(BufString == "CAPT ")
        {
            LocalCmdName = LocalCmdName.Mid(6);
            RunCatch = 1;
        }
        else if(BufString == "HTML ")
        {
            LocalCmdName = LocalCmdName.Mid(6);
            RunHtml = 1;
        }
        else if(BufString == "DOCU ")
        {
            LocalCmdName = LocalCmdName.Mid(6);
            RunDocu = 1;
        }
        else if(BufString == "PREP ")
        {
            LocalCmdName = LocalCmdName.Mid(6);
            RunBuiltin = 1;
        }
        else if(BufString == "LUAS ")
        {
            LocalCmdName = LocalCmdName.Mid(6);
            RunScript = 1;
        }
    }
    if(LocalCmdName.Len() == 0)
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Unable to process empty command.");
        return;
    }
    if(LocalCmdName == "NULL")
    {
        return;
    }
    CmdParsed = StringSplit(LocalCmdName, " ");
    Args = "";
    if(StringGetSplitUBound(CmdParsed) == -1)
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Unable to process empty command.");
        StringReleaseSplit(CmdParsed);
        return;
    }
    LUA_Args = "";
    if(RunBuiltin == 0)
    {
        if(StringGetSplitUBound(CmdParsed) >= 1)
        {
ParseSpaces:
            TempReArgs = StringReplace(StringGetSplitElement(LocalCmdName, CmdParsed, 0), BufString.Chr(2), " ", 1, -1, Binary_Compare);
            LUA_Args = LocalCmdName;
            // Check if it's a correct filename
            if(FileExist(StringReplace(TempReArgs, "\"", "", 1, -1, Binary_Compare)) == 1) goto ReAssembleArg;
            // Check if it's a directory
            if(FileIsDirectory(StringReplace(TempReArgs, "\"", "", 1, -1, Binary_Compare)) == 0)
            {
                // Check if element can be found
                if(StringGetSplitUBound(CmdParsed) <= 0) goto ReAssembleArg;
                // Discard and re-assemble
                StringReleaseSplit(CmdParsed);
                // Remove the first space from the passed string
                LocalCmdName = StringReplace(LocalCmdName, " ", BufString.Chr(2), 1, 1, Binary_Compare);
                CmdParsed = StringSplit(LocalCmdName, " ");
                goto ParseSpaces;
            }
ReAssembleArg:
            LUA_Args = StringReplace(LUA_Args, " ", BufString.Chr(2), 1, -1, Binary_Compare);
            
            Args = StringJoin(LocalCmdName, CmdParsed, " ", 1, -1).Trim();
            Args = StringReplace(Args, BufString.Chr(2), " ", 1, -1, Binary_Compare);
            LocalCmdName = StringReplace(LocalCmdName, BufString.Chr(2), " ", 1, -1, Binary_Compare);
        }
    }
    UserCmdErr = "";
    // Open an help file
    if(IsInHelp == 1)
    {
        UserCmdErr = "";
        TrimedCommand = StringGetSplitElement(LocalCmdName, CmdParsed, 0).Trim();
        TrimedCommand = StringReplace(TrimedCommand, "\"", "", 1, -1, Binary_Compare);
        BufString = TrimedCommand.Right(4).Lower_Case();
        if(BufString == ".chm" || BufString == ".col")
        {
            DisplayChmVB(TrimedCommand, Args);
        }
        else
        {
            Args = Args.Trim();
            if(Args.Len() != 0)
            {
                UserCmdNbr = WinHelp(hMDIform.hWnd, TrimedCommand.Get_String(), HELP_PARTIALKEY, (long) Args.Get_String());
            }
            else
            {
                UserCmdNbr = WinHelp(hMDIform.hWnd, TrimedCommand.Get_String(), HELP_INDEX, (long) VideStr.Get_String());
            }
            if(UserCmdNbr == 0) UserCmdErr = "Error while opening help file.";
        }
    }
    else
    {
        if(RunPlugIn == 1)
        {
            TrimedCommand = StringGetSplitElement(LocalCmdName, CmdParsed, 0).Trim();
            PlugDllHandle = LoadLibrary(StringReplace(TrimedCommand, "\"", "", 1, -1, Binary_Compare).Get_String());
            if(PlugDllHandle == 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't load Plug-In (LoadLibrary() failed).");
                goto NoRunSnap;
            }
            PlugAddr = GetProcAddress(PlugDllHandle, "QePlugIn");
            if(PlugAddr == 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Not a QEditor Plug-In.");
                FreeLibrary(PlugDllHandle);
                goto NoRunSnap;
            }
            if(NbForms == 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't run a QEditor Plug-In without any opened documents.");
                FreeLibrary(PlugDllHandle);
                goto NoRunSnap;
            }
            // Run the Quick Editor function
            ChildStruct = LoadStructure(CurrentForm);
            QEPlug((QEPROC) PlugAddr, ApphInstance, CurrentForm, ChildStruct->hChildCodeMax, 0, hStatusBar);
            FreeLibrary(PlugDllHandle);
            goto NoRunSnap;
        }
        if(RunWithsnap == 1)
        {
            TrimedCommand = StringGetSplitElement(LocalCmdName, CmdParsed, 0).Trim();
            if(WaitForDebugOutput(StringReplace(TrimedCommand, "\"", "", 1, -1, Binary_Compare), Args) == 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't create process.");
            }
            goto NoRunSnap;
        }
        if(RunCatch == 1)
        {
            TrimedCommand = StringGetSplitElement(LocalCmdName, CmdParsed, 0).Trim();
            CaptureDOSOutput(TrimedCommand, Args, GetCurrentFileDir());
            goto NoRunSnap;
        }
        if(RunDocu == 1)
        {
            TrimedCommand = StringGetSplitElement(LocalCmdName, CmdParsed, 0).Trim();
            OpenUnknownFile(StringReplace(TrimedCommand, "\"", "", 1, -1, Binary_Compare), TRUE);
            goto NoRunSnap;
        }
        if(RunBuiltin == 1)
        {
            if(NbForms != 0)
            {
                // Command to search
                TrimedCommand = StringGetSplitElement(LocalCmdName, CmdParsed, 0).Trim();
                ChildStruct = LoadStructure(CurrentForm);
                LangComm = GetCMLangComment(CurrentForm);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    CurSourceText = CurSourceText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
                    CM_GetTextAll(ChildStruct->hChildCodeMax, CurSourceText.Get_String());
                    if(CurSourceText.Len() != 0)
                    {
                        CurrentPosPrep = 1;
CheckPrepcommand:       DBlockBeg = CurSourceText.In_Str(CurrentPosPrep, "BUILDBLOCK", Text_Compare);
                        if(DBlockBeg != 0)
                        {
                            DBlockBeg = DBlockBeg + 11;
                            DBlockEnd = CurSourceText.In_Str(DBlockBeg, "BUILDBLOCKEND", Text_Compare);
                            // Move pointer
                            CurrentPosPrep = DBlockEnd + 14;
                            if(DBlockEnd != 0)
                            {
                                CurSourceTextBlock = CurSourceText.Mid(DBlockBeg, DBlockEnd - DBlockBeg);
                                TempPrep = StringReplace(CurSourceTextBlock, "\t", " ", 1, -1, Binary_Compare);
                                ArrayPrep = StringSplit(TempPrep, "\r\n");
                                if(StringGetSplitElement(TempPrep, ArrayPrep, 0).Len() != 0)
                                {
                                    // Check the block name
                                    PrepLine = StringGetSplitElement(TempPrep, ArrayPrep, 0).Trim();
                                    if(_strcmpi(PrepLine.Left(LangComm.Len()).Get_String(), LangComm.Get_String()) == 0) PrepLine = PrepLine.Mid(2);
                                    PrepLine = PrepLine.Trim();
                                    if(_strcmpi(PrepLine.Get_String(), TrimedCommand.Get_String()) == 0)
                                    {
                                        for(i = 1; i <= (long) StringGetSplitUBound(ArrayPrep) - 1; i++)
                                        {
                                            PrepLine = StringGetSplitElement(TempPrep, ArrayPrep, i).Trim();
                                            if(_strcmpi(PrepLine.Left(LangComm.Len()).Get_String(), LangComm.Get_String()) == 0) PrepLine = PrepLine.Mid(LangComm.Len() + 1);
                                            PrepLine = PrepLine.Trim();
                                            RunUserCommandRec(PrepLine);
                                        }
                                        StringReleaseSplit(ArrayPrep);
                                        goto NoRunSnap;
                                    }
                                }
                                StringReleaseSplit(ArrayPrep);
                                goto CheckPrepcommand;
                            }
                            else
                            {
                                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't find end of " + (CStr) TrimedCommand + (CStr) " directives block statement");
                            }
                        }
                        else
                        {
                            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't find " + (CStr) TrimedCommand + (CStr) " directives block");
                        }
                    }
                }
            }
            goto NoRunSnap;
        }
        if(RunScript == 1)
        {
            TrimedCommand = StringGetSplitElement(LocalCmdName, CmdParsed, 0).Trim();
            Script_Return_Code = RunScriptFile(TrimedCommand, LUA_Args.Get_String());
            if(Script_Return_Code) WriteToStatus("Script " + LocalCmdName + (CStr) " returned user code: " + (CStr) Script_Return_Code);
            goto NoRunSnap;
        }
        TrimedCommand = StringGetSplitElement(LocalCmdName, CmdParsed, 0).Trim();
        UserCmdNbr = (long) ShellExecute(hMDIform.hWnd, "open", StringReplace(TrimedCommand, "\"", "", 1, -1, Binary_Compare).Get_String(), Args.Get_String(), GetCurrentFileDir().Get_String(), SW_SHOW);
        switch(UserCmdNbr)
        {
            case 0:
                UserCmdErr = "The operating system is out of memory or resources.";
                break;
            case ERROR_FILE_NOT_FOUND:
                UserCmdErr = "The specified file was not found.";
                break;
            case ERROR_PATH_NOT_FOUND:
                UserCmdErr = "The specified path was not found.";
                break;
            case ERROR_BAD_FORMAT:
                UserCmdErr = "The .EXE file is invalid (non-Win32 .EXE or error in .EXE image).";
                break;
            case SE_ERR_ACCESSDENIED:
                UserCmdErr = "The operating system denied access to the specified file.";
                break;
            case SE_ERR_ASSOCINCOMPLETE:
                UserCmdErr = "The filename association is incomplete or invalid.";
                break;
            case SE_ERR_DDEBUSY:
                UserCmdErr = "The DDE transaction could not be completed because other DDE transactions were being processed.";
                break;
            case SE_ERR_DDEFAIL:
                UserCmdErr = "The DDE transaction failed.";
                break;
            case SE_ERR_DDETIMEOUT:
                UserCmdErr = "The DDE transaction could not be completed because the request timed out.";
                break;
            case SE_ERR_DLLNOTFOUND:
                UserCmdErr = "The specified dynamic-link library was not found.";
                break;
            case SE_ERR_NOASSOC:
                UserCmdErr = "There is no application associated with the given filename extension.";
                break;
            case SE_ERR_OOM:
                UserCmdErr = "There was not enough memory to complete the operation.";
                break;
            case SE_ERR_SHARE:
                UserCmdErr = "A sharing violation occurred.";
                break;
        }
    }
    if(UserCmdErr.Len() != 0)
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Unable to process user command.");
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Command: " + (CStr) CmdFakeName.Get_String());
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Error: " + (CStr) UserCmdNbr);
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Description: " + (CStr) UserCmdErr);
    }
NoRunSnap:
    StringReleaseSplit(CmdParsed);
    return;
}

// -----------------------------------------------------------------------
// Run an user command
void RunUserCommand(long CommandNumber)
{
    long InHelpFile = 0;
    CStr TmpCom;
    CStr BufString;

    if((long) (CommandNumber - (MENU_USER_IDBASE + 1)) > MenusCmdsHelp.Amount()) return;
    // Save the files before running any external command
    InHelpFile = MenusCmdsHelp.Get(CommandNumber - (MENU_USER_IDBASE + 1))->Content;
    if(MenusCmds.Amount() > 6)
    {
        BufString = MenusCmds.Get(CommandNumber - (MENU_USER_IDBASE + 1))->Content;
        if(_strcmpi(BufString.Left(5).Get_String(), "HTML ") == 0) goto NoSaveHtml;
    }
    if(InHelpFile == 0) if(AutoSave == 1) MCMD_SaveAll();
NoSaveHtml:
    TmpCom = TranslateUserCmd(MenusCmds.Get(CommandNumber - (MENU_USER_IDBASE + 1))->Content);
    if(FRMArgsEntered == 1)
    {
        FRMArgsEntered = 0;
        return;
    }
    TmpCom = StringReplace(TmpCom, "\t", " ", 1, -1, Binary_Compare);
    Exec(TmpCom, StringReplace(TmpCom, "$", " ", 1, -1, Binary_Compare), InHelpFile);
}

// -----------------------------------------------------------------------
// Translate an user command
CStr TranslateUserCmd(CStr cmd)
{
    CStr ReturnValue;

    UserArgs = 1;
    cmd = ChangeRelativePaths(cmd);
    if(NbForms != 0)
    {
        ChildStruct = LoadStructure(CurrentForm);
        cmd = TranslateCommands(cmd, CMGetRealFile(ChildStruct->RFile).Get_String());
        // %6 = Current word of current source
        if(cmd.In_Str(1, "%6") != 0) cmd = StringReplace(cmd, "%6", GetCurrentWord(ChildStruct->hChildCodeMax), 1, -1, Text_Compare);
    }
    else
    {
        cmd = TranslateCommands(cmd, "");
        // Clear it if no forms
        if(cmd.In_Str(1, "%6") != 0) cmd = StringReplace(cmd, "%6", "", 1, -1, Text_Compare);
    }
    ReturnValue = cmd.Trim();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a general command
CStr TranslateCommands(CStr Comd, CStr Cm)
{
    CStr RetFnc;
    CStr ProducedPrj;
    CStr CDir;
    CStr Arg8;
    long PosOpenFileDlg = 0;
    CStr StartupDir;
    CStr DefaultFilters;
    CStr BufString;
    CStr ReturnValue;
    CStr Select_File;

    BufString = Comd;
    BufString = StringReplace(BufString, "\t", " ", 1, -1, Text_Compare);
    // %1 = Filename with directory / without extension
    BufString = StringReplace(BufString, "%1", FileRemoveExtension(Cm), 1, -1, Text_Compare);
    // %2 = Filename with directory / with extension
    BufString = StringReplace(BufString, "%2", Cm, 1, -1, Text_Compare);
    // %3 = Filename without directory / without extension
    BufString = StringReplace(BufString, "%3", FileRemoveExtension(FileGetFileName(Cm)), 1, -1, Text_Compare);
    // %4 = Filename without directory / with extension
    BufString = StringReplace(BufString, "%4", FileGetFileName(Cm), 1, -1, Text_Compare);
    BufString = StringReplace(BufString, "%7", FileGetExtension(FileGetFileName(Cm)), 1, -1, Text_Compare);
    // %5 = Directory without filename
    CDir = FileGetDirectory(Cm);
    if(strcmp(CDir.Right(1).Get_String(), "\\") == 0) CDir = CDir.Left(CDir.Len() - 1);
    if(BufString.In_Str(1, "%5") != 0) BufString = StringReplace(BufString, "%5", CDir, 1, -1, Text_Compare);
    CommandToEx = StringReplace(BufString, "$", " ", 1, -1, Binary_Compare);
    // %? = ask user for required argument
    ArgRequired = 1;
    if(BufString.In_Str(1, "%?") != 0) BufString = StringReplace(BufString, "%?", Argument(), 1, -1, Text_Compare);
    ArgRequired = 0;
    // %# = prompt user with a file selection dialog
    PosOpenFileDlg = BufString.In_Str(1, "%#");
    if(PosOpenFileDlg != 0)
    {
        StartupDir = GetCommandArg(&BufString, PosOpenFileDlg + 2);
        DefaultFilters = GetCommandArg(&BufString, PosOpenFileDlg + 2);
        Select_File = AffSelFile(&StartupDir, &DefaultFilters);
        if(Select_File.Len() == 0) return("NULL");
        BufString = StringReplace(BufString, "%#", Select_File, 1, -1, Text_Compare);
    }
    // %8 = prompt user with a file selection dialog and return %3
    PosOpenFileDlg = BufString.In_Str(1, "%8");
    if(PosOpenFileDlg != 0)
    {
        StartupDir = GetCommandArg(&BufString, PosOpenFileDlg + 2);
        DefaultFilters = GetCommandArg(&BufString, PosOpenFileDlg + 2);
        Select_File = AffSelFile(&StartupDir, &DefaultFilters);
        if(Select_File.Len() == 0) return("NULL");
        BufString = StringReplace(BufString, "%8", FileRemoveExtension(FileGetFileName(Select_File)), 1, -1, Text_Compare);
    }
    // %9 = prompt user with a file selection dialog and return %4
    PosOpenFileDlg = BufString.In_Str(1, "%9");
    if(PosOpenFileDlg != 0)
    {
        StartupDir = GetCommandArg(&BufString, PosOpenFileDlg + 2);
        DefaultFilters = GetCommandArg(&BufString, PosOpenFileDlg + 2);
        Select_File = AffSelFile(&StartupDir, &DefaultFilters);
        if(Select_File.Len() == 0) return("NULL");
        BufString = StringReplace(BufString, "%9", FileGetFileName(Select_File), 1, -1, Text_Compare);
    }
    // %C = prompt user with a color dialog (result as decimal)
    if(BufString.In_Str(1, "%C") != 0) BufString = StringReplace(BufString, "%C", AffSelColor(), 1, -1, Text_Compare);
    // %C = prompt user with a color dialog (result as hexadecimal)
    if(BufString.In_Str(1, "%D") != 0) BufString = StringReplace(BufString, "%D", AffSelColorHex(), 1, -1, Text_Compare);
    // %% = prompt user with a save file selection dialog
    PosOpenFileDlg = BufString.In_Str(1, "%&");
    if(PosOpenFileDlg != 0)
    {
        StartupDir = GetCommandArg(&BufString, PosOpenFileDlg + 2);
        DefaultFilters = GetCommandArg(&BufString, PosOpenFileDlg + 2);
        Select_File = AffSelFileSave(&StartupDir, &DefaultFilters);
        if(Select_File.Len() == 0) return("NULL");
        BufString = StringReplace(BufString, "%&", Select_File, 1, -1, Text_Compare);
    }
    // %* = prompt user with a directory selection dialog
    if(BufString.In_Str(1, "%*") != 0)
    {
        Select_File = ComDlgBrowseForFolder(hMDIform.hWnd, "Select a directory...");
        BufString = StringReplace(BufString, "%*", Select_File, 1, -1, Text_Compare);
        if(Select_File.Len() == 0) return("NULL");
    }
    // %@ = prompt with an optional argument dialog
    if(BufString.In_Str(1, "%@") != 0) BufString = StringReplace(BufString, "%@", Argument(), 1, -1, Text_Compare);
    // %N = create a new child window
    if(BufString.In_Str(1, "%N") != 0) BufString = StringReplace(BufString, "%N", AffNewChild(), 1, -1, Text_Compare);
    // %% = prompt with an required argument dialog
    if(BufString.In_Str(1, "%%") != 0) BufString = StringReplace(BufString, "%%", "%", 1, -1, Text_Compare);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve an optional argument and remove it from a command
// Argument must be enclosed inside <>
CStr GetCommandArg(CStr *ComLine, long InitPos)
{
    CStr ReturnValue;
    CStr CurrentChar;
    CStr InitialDirectory;
    CStr BufString;
    long LineLen = 0;

    InitialDirectory = "";
    // Pass blank chars
    while(StringIsBlankChar(ComLine->Mid(InitPos, 1)))
    {
        InitPos++;
    }
    BufString = ComLine->Mid(InitPos, 1);
    if(BufString == "<")
    {
        LineLen = ComLine->Len();
        // Remove < char from the line
        ComLine->Get_String()[InitPos - 1] = 2;
        InitPos++;
        if(InitPos <= LineLen)
        {
            CurrentChar = ComLine->Mid(InitPos, 1);
            ComLine->Get_String()[InitPos - 1] = 2;
            while(CurrentChar != ">")
            {
                InitPos++;
                if(InitPos > LineLen) break;
                InitialDirectory = InitialDirectory + CurrentChar;
                CurrentChar = ComLine->Mid(InitPos, 1);
                ComLine->Get_String()[InitPos - 1] = 2;
            }
        }
        ReturnValue.Set_String(InitialDirectory.Get_String());
        ComLine->Set_String(StringReplace(ComLine, ComLine->Chr(2), "", 1, -1, Text_Compare).Get_String());
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Change the current drive
int ChDrive(CStr DriveStr)
{
    int ReturnValue = 1;
    CStr ChDriveStrTmp;

    ChDriveStrTmp = ChDriveStrTmp.String(MAX_PATH + 1, 1);
    GetCurrentDirectory(MAX_PATH, ChDriveStrTmp.Get_String());

	strncpy(ChDriveStrTmp.Get_String(), DriveStr.Left(1).Get_String(), 1);

    if(SetCurrentDirectory(ChDriveStrTmp.Get_String()) == 0)
    {
        ChDriveStrTmp.Get_String()[1] = ':';
        ChDriveStrTmp.Get_String()[2] = '\\';
        ChDriveStrTmp.Get_String()[3] = '\0';
        ReturnValue = SetCurrentDirectory(ChDriveStrTmp.Get_String());
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Change the current directory
int ChDir(CStr DirStr)
{
    return(SetCurrentDirectory(DirStr.Get_String()));
}

// -----------------------------------------------------------------------
// Retrieve the current directory
CStr CurDir(CStr Drive)
{
    CStr CurDriveStrTmp;
    CStr ReturnValue;
    
    CurDriveStrTmp = CurDriveStrTmp.String(MAX_PATH + 1,1);
    GetCurrentDirectory(MAX_PATH,CurDriveStrTmp.Get_String());
    ReturnValue = CurDriveStrTmp;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Select a file to open
CStr AffSelFile(CStr *SpecifiedDir, CStr *SpecifiedFilters)
{
    CStr ReturnValue;
    CStr OpFilters;
    CStr BufString;

    OpFilters = "All files (*.*)|*.*";
    if(SpecifiedFilters != 0) OpFilters = StringReplace(SpecifiedFilters, ":", "|", 1, -1, Text_Compare);
    if(SpecifiedDir != 0)
    {
        BufString = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, SpecifiedDir, 0, CurrentDir);
    }
    else
    {
        if(GetUseFileDir() == 1)
        {
            if(NbForms != 0)
            {
                ChildStruct = LoadStructure(CurrentForm);
                BufString = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, FileGetDirectory(ChildStruct->RFile), 0, CurrentDir);
            }
            else
            {
                BufString = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastUserLoadDir, 0, CurrentDir);
            }
        }
        else
        {
            BufString = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastUserLoadDir, 0, CurrentDir);
        }
    }
    ReturnValue = BufString;
    if(BufString.Len() == 0) return(ReturnValue);
    LastUserLoadDir = FileGetDirectory(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Select a file to save
CStr AffSelFileSave(CStr *SpecifiedDir, CStr *SpecifiedFilters)
{
    CStr ReturnValue;
    CStr OpFilters;
    CStr BufString;

    OpFilters = "All files (*.*)|*.*";
    if(SpecifiedFilters != 0) OpFilters = StringReplace(SpecifiedFilters, ":", "|", 1, -1, Text_Compare);
    if(SpecifiedDir != 0)
    {
        BufString = ComDlgGetSaveFileName(hMDIform.hWnd, OpFilters, SpecifiedDir, CurrentDir);
    }
    else
    {
        if(GetUseFileDir() == 1)
        {
            if(NbForms != 0)
            {
                ChildStruct = LoadStructure(CurrentForm);
                BufString = ComDlgGetSaveFileName(hMDIform.hWnd, OpFilters, FileGetDirectory(ChildStruct->RFile), CurrentDir);
            }
            else
            {
                BufString = ComDlgGetSaveFileName(hMDIform.hWnd, OpFilters, LastUserSaveDir, CurrentDir);
            }
        }
        else
        {
            BufString = ComDlgGetSaveFileName(hMDIform.hWnd, OpFilters, LastUserSaveDir, CurrentDir);
        }
    }
    ReturnValue = BufString;
    if(BufString.Len() == 0) return(ReturnValue);
    LastUserSaveDir = FileGetDirectory(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Select a fast color and return it as decimal
CStr AffSelColor(void)
{
    CStr ReturnValue;
    CStr BufString;

    if(ComDlgChooseColor(hMDIform.hWnd, 0) != 0) BufString = MyColor.rgbResult;
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Select a fast color and return it as hexadecimal
CStr AffSelColorHex(void)
{
    CStr ReturnValue;
    CStr BufString;

    if(ComDlgChooseColor(hMDIform.hWnd, 0) != 0) BufString = BufString.Hex_To_String(MyColor.rgbResult);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Create a new child window
CStr AffNewChild(void)
{
    CStr ReturnValue;

    MCMD_New();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Ask user for an argument
CStr Argument(void)
{
    CStr ReturnValue;

    CreateModalDialog(-1, -1, 277, 120, hMDIform.hWnd, &FRMArgsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    ReturnValue = EnteredArg;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Display a MSDN file
void DisplayChmVB(CStr ChmFile, CStr ChmWord)
{
    DisplayChm(ChmFile.Get_String(), ChmWord.Get_String());
}

// -----------------------------------------------------------------------
// Run a DOS command
long RunDOSCmd(CStr DOSCmdName, CStr DOSCmdArgs, CStr DOSCmdDir, CStr OutputFile)
{
    long ReturnValue = 0;
    HANDLE hTempDosFile = 0;
    SECURITY_ATTRIBUTES SecAt;
    CStr BufStringCmdDir;
    CStr BufStringCmdName;
    CStr BufString;

    SecAt.nLength = sizeof(SecAt);
    SecAt.lpSecurityDescriptor = 0;
    SecAt.bInheritHandle = 1;
    memset(&MyProcStartup, 0, sizeof(MyProcStartup));
    memset(&MyProcInfos, 0, sizeof(MyProcInfos));
    MyProcStartup.cb = sizeof(MyProcStartup);
    MyProcStartup.lpDesktop = 0;
    MyProcStartup.lpReserved = 0;
    MyProcStartup.lpTitle = "";
    MyProcStartup.dwFlags = STARTF_USESHOWWINDOW;
    MyProcStartup.wShowWindow = SW_HIDE;
    MyProcStartup.hStdInput = 0;
    MyProcStartup.hStdError = 0;
    MyProcStartup.hStdOutput = 0;
    if(OutputFile.Len() != 0)
    {
        hTempDosFile = FileCreateEmpty(OutputFile, USE_SECURITY_DESCRIPTOR);
        MyProcStartup.dwFlags = MyProcStartup.dwFlags | STARTF_USESTDHANDLES;
        MyProcStartup.hStdInput = 0;
        MyProcStartup.hStdError = hTempDosFile;
        MyProcStartup.hStdOutput = hTempDosFile;
    }
    BufStringCmdDir = DOSCmdDir;
    if(BufStringCmdDir.Len() == 0) BufStringCmdDir = FileGetDirectory(DOSCmdName);
    if(BufStringCmdDir.Len() == 0) BufStringCmdDir = CurDir();
    if(strcmp(BufStringCmdDir.Right(1).Get_String(), "\\") == 0) BufStringCmdDir = BufStringCmdDir.Mid(1, BufStringCmdDir.Len() - 1);
    // Create the process
    CursorSetWait();
    BufStringCmdName = StringReplace(DOSCmdName, "\"", "", 1, -1, Binary_Compare);
    if(OutputFile.Len() == 0)
    {
        ChDir(DOSCmdDir);
        BufString = CorrectFileName(BufStringCmdName).Get_String() + (CStr) " " + (CStr) DOSCmdArgs.Get_String();
        CreateProcess(BufStringCmdName.Get_String(), BufString.Get_String(), &SecAt, &SecAt, 0, 0, NULL, BufStringCmdDir.Get_String(), &MyProcStartup, &MyProcInfos);
    }
    else
    {
        BufString = CorrectFileName(BufStringCmdName).Get_String() + (CStr) " " + (CStr) DOSCmdArgs.Get_String();
        CreateProcess(BufStringCmdName.Get_String(), BufString.Get_String(), &SecAt, &SecAt, 1, 0, NULL, BufStringCmdDir.Get_String(), &MyProcStartup, &MyProcInfos);
    }
    if(MyProcInfos.hProcess != 0)
    {
        // --- Display captured text in status window
        WaitForSingleObject(MyProcInfos.hProcess, -1);
        CloseHandle(MyProcInfos.hThread);
        CloseHandle(MyProcInfos.hProcess);
        if(hTempDosFile != 0) FileClose(hTempDosFile);
        ReturnValue = 1;
    }
    else
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Unable to run command : '" + (CStr) DOSCmdName.Get_String() + (CStr) "'");
        if(hTempDosFile != 0) FileClose(hTempDosFile);
        DeleteFile(OutputFile.Get_String());
        ReturnValue = 0;
    }
    CursorSetNormal();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Run a DOS command and trap the output
void CaptureDOSOutput(CStr DOSCmdName, CStr DOSCmdArgs, CStr DOSCmdDir)
{
    CStr StrCapture;
    unsigned long OutSizeRead = 0;
    long *ArrayCapture = 0;
    SECURITY_ATTRIBUTES SecAt;
    int i = 0;
    CStr BufStringCmdDir;
    CStr BufStringCmdName;
    CStr BufString;

    memset(&MyProcStartup, 0, sizeof(MyProcStartup));
    memset(&MyProcInfos, 0, sizeof(MyProcInfos));
    SecAt.nLength = sizeof(SecAt);
    SecAt.lpSecurityDescriptor = 0;
    SecAt.bInheritHandle = -1;
    if(CreatePipe(&PipeIn, &PipeOut, &SecAt, 65535 * 2) == 0)
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Unable to trap command.");
        return;
    }
    MyProcStartup.cb = sizeof(MyProcStartup);
    MyProcStartup.lpDesktop = 0;
    MyProcStartup.lpReserved = 0;
    MyProcStartup.lpTitle = "";
    MyProcStartup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    MyProcStartup.hStdInput = 0;
    MyProcStartup.hStdOutput = PipeOut;
    MyProcStartup.hStdError = PipeOut;
    MyProcStartup.wShowWindow = SW_HIDE;
    BufStringCmdDir = DOSCmdDir;
    if(BufStringCmdDir.Len() == 0) BufStringCmdDir = FileGetDirectory(DOSCmdName);
    if(BufStringCmdDir.Len() == 0) BufStringCmdDir = CurDir();
    if(strcmp(BufStringCmdDir.Right(1).Get_String(), "\\") == 0) BufStringCmdDir = BufStringCmdDir.Mid(1, BufStringCmdDir.Len() - 1);
    // Create the process
    WriteComment("Executing process...");
    CursorSetWait();
    BufStringCmdName = StringReplace(DOSCmdName, "\"", "", 1, 2, Binary_Compare);
    BufStringCmdDir = StringReplace(BufStringCmdDir, "\"", "", 1, 2, Binary_Compare);
    BufString = CorrectFileName(BufStringCmdName).Get_String() + (CStr) " " + (CStr) DOSCmdArgs.Get_String();
    CreateProcess(BufStringCmdName.Get_String(), BufString.Get_String(), &SecAt, &SecAt, 1, 0, NULL, BufStringCmdDir.Get_String(), &MyProcStartup, &MyProcInfos);
    if(MyProcInfos.hProcess != 0)
    {
        // --- Display captured text in status window
        WaitForSingleObject(MyProcInfos.hProcess, -1);
        CloseHandle(PipeOut);
        StrCapture = StrCapture.String(65535, 1);
        if(ReadFile(PipeIn, StrCapture.Get_String(), 65535, &OutSizeRead, 0) != 0)
        {
            StrCapture.Get_String()[OutSizeRead] = 0;
            if(AutoClearOut == 1) ClearStatus();
            ArrayCapture = StringSplit(StrCapture, "\n");
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "*** Captured from command: " + (CStr) DOSCmdName.Get_String() + (CStr) " " + (CStr) DOSCmdArgs.Get_String());
            for(i = 0; i <= (long) StringGetSplitUBound(ArrayCapture); i++)
            {
                WriteToStatus(StringReplace(StringReplace(StringGetSplitElement(StrCapture, ArrayCapture, i), "\r", "", 1, -1, Binary_Compare), BufString.Chr(0xFF), " ", 1, -1, Binary_Compare));
            }
        }
        StringReleaseSplit(ArrayCapture);
        CloseHandle(PipeIn);
        CloseHandle(MyProcInfos.hThread);
        CloseHandle(MyProcInfos.hProcess);
        CursorSetNormal();
        WriteComment("");
        return;
    }
    else
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Unable to run command : '" + (CStr) DOSCmdName.Get_String() + (CStr) "'");
        CloseHandle(PipeOut);
        CloseHandle(PipeIn);
    }
    CursorSetNormal();
    WriteComment("");
}

// -----------------------------------------------------------------------
// Retrieve the directory of the current file
CStr GetCurrentFileDir(void)
{
    CStr ReturnValue;
    CStr TmpDir;
    int i = 0;

    if(NbForms == 0) return(ReturnValue);
    ChildStruct = LoadStructure(CurrentForm);
    TmpDir = ChildStruct->RFile;
    for(i = TmpDir.Len(); i >= 1; i--)
    {
        if(strcmp(TmpDir.Mid(i, 1).Get_String(), "\\") == 0)
        {
            ReturnValue.Set_String(TmpDir.Left(i).Get_String());
            break;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Prepare hex array for db conversion
/*void PrepareHexTab(void) {
    TabHex[0] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[1] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[2] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[3] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[4] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[5] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[6] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[7] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[8] = "\tdb\t000h,000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[9] = "\tdb\t000h,000h,000h,000h,000h,000h,000h\r\n";
    TabHex[10] = "\tdb\t000h,000h,000h,000h,000h,000h\r\n";
    TabHex[11] = "\tdb\t000h,000h,000h,000h,000h\r\n";
    TabHex[12] = "\tdb\t000h,000h,000h,000h\r\n";
    TabHex[13] = "\tdb\t000h,000h,000h\r\n";
    TabHex[14] = "\tdb\t000h,000h\r\n";
    TabHex[15] = "\tdb\t000h\r\n";
    TabHex[16] = "\tdb\t\r\n";
}*/

// -----------------------------------------------------------------------
// Prepare hex array for rcdata resources
void PrepareRcTab(void)
{
    TabRc[0] = "\t0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000\r\n";
    TabRc[1] = "\t0x0000 0x0000 0x0000 0x0000 0x0000 0x0000 0x0000\r\n";
    TabRc[2] = "\t0x0000 0x0000 0x0000 0x0000 0x0000 0x0000\r\n";
    TabRc[3] = "\t0x0000 0x0000 0x0000 0x0000 0x0000\r\n";
    TabRc[4] = "\t0x0000 0x0000 0x0000 0x0000\r\n";
    TabRc[5] = "\t0x0000 0x0000 0x0000\r\n";
    TabRc[6] = "\t0x0000 0x0000\r\n";
    TabRc[7] = "\t0x0000\r\n";
    TabRc[8] = "\t\r\n";

    TabRcComma[0] = "\t0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,\r\n";
    TabRcComma[1] = "\t0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,\r\n";
    TabRcComma[2] = "\t0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,\r\n";
    TabRcComma[3] = "\t0x0000,0x0000,0x0000,0x0000,0x0000,\r\n";
    TabRcComma[4] = "\t0x0000,0x0000,0x0000,0x0000,\r\n";
    TabRcComma[5] = "\t0x0000,0x0000,0x0000,\r\n";
    TabRcComma[6] = "\t0x0000,0x0000,\r\n";
    TabRcComma[7] = "\t0x0000,\r\n";
    TabRcComma[8] = "\t\r\n";

    TabRcCommaEnd[0] = "\t0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000\r\n";
    TabRcCommaEnd[1] = "\t0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000\r\n";
    TabRcCommaEnd[2] = "\t0x0000,0x0000,0x0000,0x0000,0x0000,0x0000\r\n";
    TabRcCommaEnd[3] = "\t0x0000,0x0000,0x0000,0x0000,0x0000\r\n";
    TabRcCommaEnd[4] = "\t0x0000,0x0000,0x0000,0x0000\r\n";
    TabRcCommaEnd[5] = "\t0x0000,0x0000,0x0000\r\n";
    TabRcCommaEnd[6] = "\t0x0000,0x0000\r\n";
    TabRcCommaEnd[7] = "\t0x0000\r\n";
    TabRcCommaEnd[8] = "\t\r\n";
}

// -----------------------------------------------------------------------
// Prepare languages array for languages resources
void PrepareRcLanguages(void)
{
    TabLangRc[0] = "Neutral";
    TabLangRc[1] = "Arabic";
    TabLangRc[2] = "Bulgarian";
    TabLangRc[3] = "Catalan";
    TabLangRc[4] = "Chinese";
    TabLangRc[5] = "Czech";
    TabLangRc[6] = "Danish";
    TabLangRc[7] = "German";
    TabLangRc[8] = "Greek";
    TabLangRc[9] = "English";
    TabLangRc[10] = "Spanish";
    TabLangRc[11] = "Finnish";
    TabLangRc[12] = "French";
    TabLangRc[13] = "Hebrew";
    TabLangRc[14] = "Hungarian";
    TabLangRc[15] = "Icelandic";
    TabLangRc[16] = "Italian";
    TabLangRc[17] = "Japanese";
    TabLangRc[18] = "Korean";
    TabLangRc[19] = "Dutch";
    TabLangRc[20] = "Norwegian";
    TabLangRc[21] = "Polish";
    TabLangRc[22] = "Portuguese";
    TabLangRc[23] = "";
    TabLangRc[24] = "Romanian";
    TabLangRc[25] = "Russian";
    TabLangRc[26] = "Croatian";
    TabLangRc[27] = "Slovak";
    TabLangRc[28] = "Albanian";
    TabLangRc[29] = "Swedish";
    TabLangRc[30] = "Thai";
    TabLangRc[31] = "Turkish";
    TabLangRc[32] = "Urdu";
    TabLangRc[33] = "Indonesian";
    TabLangRc[34] = "Ukrainian";
    TabLangRc[35] = "Belarusian";
    TabLangRc[36] = "Slovenian";
    TabLangRc[37] = "Estonian";
    TabLangRc[38] = "Latvian";
    TabLangRc[39] = "Lithuanian";
    TabLangRc[40] = "";
    TabLangRc[41] = "Farsi";
    TabLangRc[42] = "Vietnamese";
    TabLangRc[43] = "Armenian";
    TabLangRc[44] = "Azeri (latin)";
    TabLangRc[45] = "Basque";
    TabLangRc[46] = "";
    TabLangRc[47] = "Macedonian";
    TabLangRc[48] = "";
    TabLangRc[49] = "";
    TabLangRc[50] = "";
    TabLangRc[51] = "";
    TabLangRc[52] = "";
    TabLangRc[53] = "";
    TabLangRc[54] = "Afrikaans";
    TabLangRc[55] = "Georgians";
    TabLangRc[56] = "Faeroese";
    TabLangRc[57] = "Hindi";
    TabLangRc[58] = "";
    TabLangRc[59] = "";
    TabLangRc[60] = "";
    TabLangRc[61] = "";
    TabLangRc[62] = "Malay";
    TabLangRc[63] = "Kazakh";
    TabLangRc[64] = "";
    TabLangRc[65] = "Swahili";
    TabLangRc[66] = "";
    TabLangRc[67] = "Uzbek";
    TabLangRc[68] = "Tatar";
    TabLangRc[69] = "";
    TabLangRc[70] = "";
    TabLangRc[71] = "";
    TabLangRc[72] = "";
    TabLangRc[73] = "Tamil";
    TabLangRc[74] = "";
    TabLangRc[75] = "";
    TabLangRc[76] = "";
    TabLangRc[77] = "";
    TabLangRc[78] = "Marathi";
    TabLangRc[79] = "Sanskrit";
    TabLangRc[80] = "";
    TabLangRc[81] = "";
    TabLangRc[82] = "";
    TabLangRc[83] = "";
    TabLangRc[84] = "";
    TabLangRc[85] = "";
    TabLangRc[86] = "";
    TabLangRc[87] = "Konkani";
}

// -----------------------------------------------------------------------
// Prepare builtin accelerators table
void PrepareAccelerators(void)
{
    TabAccelerators[0] = "VK_LBUTTON";
    TabAccelerators[1] = "VK_RBUTTON";
    TabAccelerators[2] = "VK_CANCEL";
    TabAccelerators[3] = "VK_MBUTTON";
    TabAccelerators[4] = "VK_BACK";
    TabAccelerators[5] = "VK_TAB";
    TabAccelerators[6] = "VK_CLEAR";
    TabAccelerators[7] = "VK_RETURN";
    TabAccelerators[8] = "VK_SHIFT";
    TabAccelerators[9] = "VK_CONTROL";
    TabAccelerators[10] = "VK_MENU";
    TabAccelerators[11] = "VK_PAUSE";
    TabAccelerators[12] = "VK_CAPITAL";
    TabAccelerators[13] = "VK_ESCAPE";
    TabAccelerators[14] = "VK_SPACE";
    TabAccelerators[15] = "VK_PRIOR";
    TabAccelerators[16] = "VK_NEXT";
    TabAccelerators[17] = "VK_END";
    TabAccelerators[18] = "VK_HOME";
    TabAccelerators[19] = "VK_LEFT";
    TabAccelerators[20] = "VK_UP";
    TabAccelerators[21] = "VK_RIGHT";
    TabAccelerators[22] = "VK_DOWN";
    TabAccelerators[23] = "VK_SELECT";
    TabAccelerators[24] = "VK_PRINT";
    TabAccelerators[25] = "VK_EXECUTE";
    TabAccelerators[26] = "VK_SNAPSHOT";
    TabAccelerators[27] = "VK_INSERT";
    TabAccelerators[28] = "VK_DELETE";
    TabAccelerators[29] = "VK_HELP";
    TabAccelerators[30] = "VK_LWIN";
    TabAccelerators[31] = "VK_RWIN";
    TabAccelerators[32] = "VK_APPS";
    TabAccelerators[33] = "VK_NUMPAD0";
    TabAccelerators[34] = "VK_NUMPAD1";
    TabAccelerators[35] = "VK_NUMPAD2";
    TabAccelerators[36] = "VK_NUMPAD3";
    TabAccelerators[37] = "VK_NUMPAD4";
    TabAccelerators[38] = "VK_NUMPAD5";
    TabAccelerators[39] = "VK_NUMPAD6";
    TabAccelerators[40] = "VK_NUMPAD7";
    TabAccelerators[41] = "VK_NUMPAD8";
    TabAccelerators[42] = "VK_NUMPAD9";
    TabAccelerators[43] = "VK_MULTIPLY";
    TabAccelerators[44] = "VK_ADD";
    TabAccelerators[45] = "VK_SEPARATOR";
    TabAccelerators[46] = "VK_SUBTRACT";
    TabAccelerators[47] = "VK_DECIMAL";
    TabAccelerators[48] = "VK_DIVIDE";
    TabAccelerators[49] = "VK_F1";
    TabAccelerators[50] = "VK_F2";
    TabAccelerators[51] = "VK_F3";
    TabAccelerators[52] = "VK_F4";
    TabAccelerators[53] = "VK_F5";
    TabAccelerators[54] = "VK_F6";
    TabAccelerators[55] = "VK_F7";
    TabAccelerators[56] = "VK_F8";
    TabAccelerators[57] = "VK_F9";
    TabAccelerators[58] = "VK_F10";
    TabAccelerators[59] = "VK_F11";
    TabAccelerators[60] = "VK_F12";
    TabAccelerators[61] = "VK_F13";
    TabAccelerators[62] = "VK_F14";
    TabAccelerators[63] = "VK_F15";
    TabAccelerators[64] = "VK_F16";
    TabAccelerators[65] = "VK_F17";
    TabAccelerators[66] = "VK_F18";
    TabAccelerators[67] = "VK_F19";
    TabAccelerators[68] = "VK_F20";
    TabAccelerators[69] = "VK_F21";
    TabAccelerators[70] = "VK_F22";
    TabAccelerators[71] = "VK_F23";
    TabAccelerators[72] = "VK_F24";
    TabAccelerators[73] = "VK_NUMLOCK";
    TabAccelerators[74] = "VK_SCROLL";
    TabAccelerators[75] = "VK_PROCESSKEY";
    TabAccelerators[76] = "VK_ATTN";
    TabAccelerators[77] = "VK_CRSEL";
    TabAccelerators[78] = "VK_EXSEL";
    TabAccelerators[79] = "VK_EREOF";
    TabAccelerators[80] = "VK_PLAY";
    TabAccelerators[81] = "VK_ZOOM";
    TabAccelerators[82] = "VK_NONAME";
    TabAccelerators[83] = "VK_PA1";
    TabAccelerators[84] = "VK_OEM_CLEAR";
}

// -----------------------------------------------------------------------
// Prepare hex array for hex to dec conversion
void PrepareHexConvTab(void)
{
    TabConvHexCoher[0] = 0;
    TabConvHexCoher[1] = 1;
    TabConvHexCoher[2] = 2;
    TabConvHexCoher[3] = 3;
    TabConvHexCoher[4] = 4;
    TabConvHexCoher[5] = 5;
    TabConvHexCoher[6] = 6;
    TabConvHexCoher[7] = 7;
    TabConvHexCoher[8] = 8;
    TabConvHexCoher[9] = 9;
    TabConvHexCoher[10] = 0;
    TabConvHexCoher[11] = 0;
    TabConvHexCoher[12] = 0;
    TabConvHexCoher[13] = 0;
    TabConvHexCoher[14] = 0;
    TabConvHexCoher[15] = 0;
    TabConvHexCoher[16] = 0;
    TabConvHexCoher[17] = 10;
    TabConvHexCoher[18] = 11;
    TabConvHexCoher[19] = 12;
    TabConvHexCoher[20] = 13;
    TabConvHexCoher[21] = 14;
    TabConvHexCoher[22] = 15;
}

// -----------------------------------------------------------------------
// Check for project file validity
long CheckWorkSpaceFile(CStr FileName)
{
    long ReturnValue = 0;
    HANDLE WorkFileHandle = 0;
    unsigned long WorkFileRead = 0;
    CStr WorkHeader;
    CStr BufString;

    WorkFileHandle = FileOpenR(FileName);
    if(WorkFileHandle != INVALID_HANDLE_VALUE)
    {
        WorkHeader = WorkHeader.String(15,1);
        ReadFile(WorkFileHandle, WorkHeader.Get_String(), 15, &WorkFileRead, 0);
        FileClose(WorkFileHandle);
    }
    BufString = "[" + (CStr) AppTitle + (CStr) "workspc]";
    if(_strcmpi(WorkHeader.Get_String(), BufString.Get_String()) == 0) ReturnValue = 1;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve current child state
long GetChildState(void)
{
    if(NbForms == 0) return(0);
    return(ControlGetWindowState(CurrentForm));
}

// -----------------------------------------------------------------------
// Remove a quoted string
CStr RemoveQuotesVB(CStr Str)
{
    CStr ReturnValue;
    long PosQuote = 0;
    long PosQuoteEnd = 0;
    CStr BufString;

    BufString = Str;
    PosQuote = 0;
    PosQuoteEnd = 0;
    PosQuote = BufString.In_Str(1, "'");
    if(PosQuote != 0)
    {
        PosQuoteEnd = BufString.In_Str(PosQuote + 1, "'");
        if(PosQuoteEnd != 0) if(PosQuoteEnd != PosQuote) BufString = BufString.Mid(1, PosQuote - 1).Get_String() + (CStr) BufString.Mid(PosQuoteEnd + 1).Get_String();
    }
    else
    {
        PosQuote = BufString.In_Str(1, "\"");
        if(PosQuote != 0)
        {
            PosQuoteEnd = BufString.In_Str(PosQuote + 1, "\"");
            if(PosQuoteEnd != 0) if(PosQuoteEnd != PosQuote) BufString = BufString.Mid(1, PosQuote - 1).Get_String() + (CStr) BufString.Mid(PosQuoteEnd + 1).Get_String();
        }
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Select a procedure
// Core routine !
long SelectProcedure(HWND hForm)
{
    long ReturnValue = 0;
    long CurLinePos = 0;
    long CurLinePosBack = 0;
    CStr CurLangProc1;
    CStr CurLangProc2;
    CM_RANGE ProcRange;
    CStr CurLine;
    long ProcPos = 0;
    long EndpPos = 0;
    long PosCommentLine = 0;
    int i = 0;
    int j = 0;
    CStr CurLangComment;
    CStr CurLangProc;
    long *CurLangProcArray = 0;
    long *CurLangEndpArray = 0;
    CStr OpenBalance;
    CStr CloseBalance;
    CStr SelToCheck;
    long CurrentBalancePos = 0;
    long OpenBalanceNumber = 0;
    long CloseBalanceNumber = 0;
    long BalanceMaxLines = 0;
    long BalanceOrphanState = 0;
    long BalanceInterlaceState = 0;
    CStr BufString;

    ChildStruct = LoadStructure(hForm);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    CurLinePos = GetCurrentLineNumber(ChildStruct->hChildCodeMax);
    CurLangComment = GetCMLangComment(hForm);
    CurLangProc = GetCMLangProc(hForm);
    CurLangProc1 = CurLangProc;
    CurLangProcArray = StringSplit(CurLangProc1, " ");
    // Allow single keyword on a line or not
    BalanceOrphanState = GetCMLangProcOrphan(hForm);
    BalanceInterlaceState = GetCMLangProcInterlace(hForm);
    // Default but...
    LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, 0);
    // Very default in case we can't retrieve any definition
    if(LastProcTypeKeyword.Len() == 0) LastProcTypeKeyword = "proc";
    if(StringGetSplitUBound(CurLangProcArray) == -1) goto FreeSplits;
    CurLangProc = GetCMLangEndp(hForm);
    CurLangProc2 = CurLangProc;
    CurLangEndpArray = StringSplit(CurLangProc2, " ");
    if(StringGetSplitUBound(CurLangEndpArray) == -1) goto FreeSplits;
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) goto FreeSplits;
    ProcPos = -1;
RechckProc:
    CurLinePosBack = CurLinePos;
    EndpPos = -1;
    // Check text to the end
    for(i = CurLinePos; i <= (long) CM_GetLineCount(ChildStruct->hChildCodeMax) - 1; i++)
    {
        CurLinePos = i + 1;
        CurLine = CMGetCurrentLine(ChildStruct->hChildCodeMax, i).Trim();
        PosCommentLine = CurLine.In_Str(1, CurLangComment);
        if(PosCommentLine != 0)
        {
            if(PosCommentLine != 1) CurLine = CurLine.Left(PosCommentLine - 1);
            else CurLine = "";
        }
        if(CurLine.Len() != 0)
        {
            CurLine = RemoveQuotesVB(StringReplace(CurLine, "\t", " ", 1, -1, Binary_Compare)).Trim();
            // Check if we found a proc entry point before endpoint
            for(j = 0; j <= (long) StringGetSplitUBound(CurLangProcArray); j++)
            {
                // xxx proc xxx
                BufString = " " + (CStr) StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String() + (CStr) " ";
                if(CurLine.In_Str(1, BufString, Text_Compare) != 0)
                {
                    if(ProcPos != -1)
                    {
                        if(BalanceInterlaceState == 1)
                        {
                            LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, j);
                            goto FuturBalancing;
                        }
                    }
                    ProcPos = i;
                    goto RechckProc;
                }
                // proc xxx (tasm)
                BufString = StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String() + (CStr) " ";
                if(CurLine.In_Str(1, BufString, Text_Compare) == 1)
                {
                    if(ProcPos != -1)
                    {
                        if(BalanceInterlaceState == 1)
                        {
                            LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, j);
                            goto FuturBalancing;
                        }
                    }
                    ProcPos = i;
                    goto RechckProc;
                }
                // Orphans
                if(BalanceOrphanState == 1)
                {
                    BufString = StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String();
                    if(CurLine.In_Str(1, BufString, Text_Compare) == 1)
                    {
                        if(ProcPos != -1)
                        {
                            if(BalanceInterlaceState == 1)
                            {
                                LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, j);
                                goto FuturBalancing;
                            }
                        }
                        ProcPos = i;
                        if(ProcPos != 0) if(_strcmpi(CurLine.Get_String(), StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String()) == 0) ProcPos--;
                        goto RechckProc;
                    }
                }
                // xxx proc
                BufString = StringGetSplitElement(CurLangProc1, CurLangProcArray, j);
                if((CurLine.Len() - BufString.Len()) != 0)
                {
                    BufString = " " + (CStr) StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String();
                    if(CurLine.In_Str(1, BufString, Text_Compare) == (CurLine.Len() - StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Len())) {
                        if(ProcPos != -1)
                        {
                            if(BalanceInterlaceState == 1)
                            {
                                LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, j);
                                goto FuturBalancing;
                            }
                        }
                        ProcPos = i;
                        goto RechckProc;
                    }
                }
            }
FuturBalancing:
            // Put first one
            for(j = 0; j <= (long) StringGetSplitUBound(CurLangEndpArray); j++)
            {
                // xxx endp xxx
                BufString = " " + (CStr) StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Get_String() + (CStr) " ";
                if(CurLine.In_Str(1, BufString, Text_Compare) != 0)
                {
                    EndpPos = i;
                    goto FoundEndp;
                }
                // xxx endp
                if((CurLine.Len() - StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Len()) != 0)
                {
                    BufString = " " + (CStr) StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Get_String();
                    if(CurLine.In_Str(1, BufString, Text_Compare) == (CurLine.Len() - 
                       StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Len()))
                    {
                        EndpPos = i;
                        goto FoundEndp;
                    }
                }
                // endp xxx
                BufString = StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Get_String() + (CStr) " ";
                if(CurLine.In_Str(1, BufString, Text_Compare) == 1)
                {
                    EndpPos = i;
                    goto FoundEndp;
                }
                // endp
                if(_strcmpi(CurLine.Get_String(), StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Get_String()) == 0)
                {
                    EndpPos = i;
                    goto FoundEndp;
                }
            }
        }
    }
FoundEndp:
    if(EndpPos == -1)
    {
        MiscMsgBox(hMDIform.hWnd, "Can't find end of procedure.", MB_ERROR, Requesters);
        if(ControlIsVisible(ChildStruct->hChildCodeMax) != 0) SetFocus(ChildStruct->hChildCodeMax);
        goto FreeSplits;
    }
    // Check if we were already inside a procedure
    if(ProcPos == -1)
    {
        for(i = CurLinePosBack; i >= 0; i--)
        {
            CurLinePosBack = i - 1;
            CurLine = CMGetCurrentLine(ChildStruct->hChildCodeMax, i).Trim();
            PosCommentLine = CurLine.In_Str(1, CurLangComment);
            if(PosCommentLine != 0)
            {
                if(PosCommentLine != 1) CurLine = CurLine.Left(PosCommentLine - 1);
                else CurLine = "";
            }
            if(CurLine.Len() != 0)
            {
                if(_strcmpi(CurLine.Get_String(), "NULL") != 0)
                {
                    CurLine = RemoveQuotesVB(StringReplace(CurLine, "\t", " ", 1, -1, Binary_Compare)).Trim();
                    for(j = 0; j <= (long) StringGetSplitUBound(CurLangEndpArray); j++)
                    {
                        // Check if we found a proc end point before endpoint
                        // xxx endp xxx
                        BufString = " " + (CStr) StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Get_String() + (CStr) " ";
                        if(CurLine.In_Str(1, BufString, Text_Compare) != 0) goto FoundEndpEnd;
                        // endp xxx (tasm)
                        BufString = StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Get_String() + (CStr) " ";
                        if(CurLine.In_Str(1, BufString, Text_Compare) == 1) goto FoundEndpEnd;
                        // endp crlf (tasm)
                        BufString = StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Get_String() + (CStr) "\r\n";
                        if(CurLine.In_Str(1, BufString, Text_Compare) == 1) goto FoundEndpEnd;
                        // xxx endp
                        if((CurLine.Len() - StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Len()) != 0)
                        {
                            BufString = " " + (CStr) StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Get_String();
                            if(CurLine.In_Str(1, BufString, Text_Compare) == (CurLine.Len() - StringGetSplitElement(CurLangProc2, CurLangEndpArray, j).Len())) goto FoundEndpEnd;
                        }
                    }

                    for(j = 0; j <= (long) StringGetSplitUBound(CurLangProcArray); j++)
                    {
                        // xxx proc xxx
                        BufString = " " + (CStr) StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String() + (CStr) " ";
                        if(CurLine.In_Str(1, BufString, Text_Compare) != 0)
                        {
                            ProcPos = i;
                            LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String();
                            goto FoundEndpEnd;
                        }
                        // xxx proc
                        if((CurLine.Len() - StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Len()) != 0)
                        {
                            BufString = " " + (CStr) StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String();
                            if(CurLine.In_Str(1, BufString, Text_Compare) == (CurLine.Len() -
                               StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Len()))
                            {
                                ProcPos = i;
                                LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, j);
                                goto FoundEndpEnd;
                            }
                        }
                        // proc xxx
                        if(CurLine.In_Str(1, StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String() + (CStr) " ", Text_Compare) == 1)
                        {
                            ProcPos = i;
                            LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, j);
                            goto FoundEndpEnd;
                        }
                        if(BalanceOrphanState == 1)
                        {
                            // Orphans
                            if(CurLine.In_Str(1, StringGetSplitElement(CurLangProc1, CurLangProcArray, j), Text_Compare) == 1)
                            {
                                ProcPos = i;
                                LastProcTypeKeyword = StringGetSplitElement(CurLangProc1, CurLangProcArray, j);
                                if(ProcPos != 0) if(_strcmpi(CurLine.Get_String(), StringGetSplitElement(CurLangProc1, CurLangProcArray, j).Get_String()) == 0) ProcPos--;
                                goto FoundEndpEnd;
                            }
                        }
                    }
                }
            }
        }
FoundEndpEnd:
        if(ProcPos == -1)
        {
            MiscMsgBox(hMDIform.hWnd, "Can't find procedure entry point.", MB_ERROR, Requesters);
            if(ControlIsVisible(ChildStruct->hChildCodeMax) != 0) SetFocus(ChildStruct->hChildCodeMax);
            goto FreeSplits;
        }
    }
CheckOpenCloseBalance:
    ProcRange.posStart.nCol = 0;
    ProcRange.posEnd.nCol = 0;
    ProcRange.posEnd.nLine = EndpPos + 1;
    ProcRange.posStart.nLine = ProcPos;
    // Check balance
    if(BalanceInterlaceState == 1)
    {
        ChildStruct = LoadStructure(hForm);
        SelToCheck = SelToCheck.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &ProcRange, 1), 1);
        BalanceMaxLines = CM_GetLineCount(ChildStruct->hChildCodeMax);
        CM_GetText(ChildStruct->hChildCodeMax, SelToCheck.Get_String(), &ProcRange);
        if(SelToCheck.Len() == 0) goto StopBalanceCheck;
        OpenBalance = GetCMLangProcOpenBlock(hForm);
        CloseBalance = GetCMLangProcCloseBlock(hForm);
        if(OpenBalance.Len() != 0)
        {
            if(CloseBalance.Len() != 0)
            {
                // Search balances
                RemoveSingleLineComment(SelToCheck.Get_String(), SelToCheck.Len(), GetCMLangComment(hForm).Get_String());
                RemoveQuotes(SelToCheck.Get_String(), SelToCheck.Len());
                TabToSpace(SelToCheck.Get_String(), SelToCheck.Len());
                CurrentBalancePos = 1;
                OpenBalanceNumber = 0;
CheckOpenBalance:
                CurrentBalancePos = SelToCheck.In_Str(CurrentBalancePos, OpenBalance, Text_Compare);
                if(CurrentBalancePos == 0) goto StopCheckOpenBalance;
                CurrentBalancePos = CurrentBalancePos + OpenBalance.Len();
                OpenBalanceNumber++;
                goto CheckOpenBalance;
StopCheckOpenBalance:
                CurrentBalancePos = 1;
                CloseBalanceNumber = 0;
CheckCloseBalance:
                CurrentBalancePos = SelToCheck.In_Str(CurrentBalancePos, CloseBalance, Text_Compare);
                if(CurrentBalancePos == 0) goto StopCheckCloseBalance;
                CurrentBalancePos = CurrentBalancePos + CloseBalance.Len();
                CloseBalanceNumber++;
                goto CheckCloseBalance;
StopCheckCloseBalance:
                if(OpenBalanceNumber != CloseBalanceNumber)
                {
                    EndpPos++;
                    if((long) EndpPos > (long) BalanceMaxLines)
                    {
StopBalanceCheck:       MiscMsgBox(hMDIform.hWnd, "Unmatched block in procedure.", MB_ERROR, Requesters);
                        if(ControlIsVisible(ChildStruct->hChildCodeMax) != 0) SetFocus(ChildStruct->hChildCodeMax);
                        goto FreeSplits;
                    }
                    goto CheckOpenCloseBalance;
                }
            }
        }
    }
    if(CM_SetSel(ChildStruct->hChildCodeMax, &ProcRange, 1) != CME_SUCCESS)
    {
        ProcRange.posEnd.nLine = ProcRange.posEnd.nLine - 1;
        ProcRange.posEnd.nCol = 2000;
        // Last chance...
        if(CM_SetSel(ChildStruct->hChildCodeMax, &ProcRange, 1) != CME_SUCCESS) goto FreeSplits;
    }
    ReturnValue = 1;
FreeSplits:
    StringReleaseSplit(CurLangEndpArray);
    StringReleaseSplit(CurLangProcArray);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the procedure name of a proc/endp first line
CStr GetProcName(CStr ProcBlock, CStr ProcKeyWord)
{
    CStr ProcChar;
    long ProcPos = 0;
    CStr ExprCharToTest;
    long BracketBalance = 0;
    CStr TempProcName;
    int i = 0;
    CStr ReturnValue;
    CStr BufString;

    ProcBlock = StringReplace(ProcBlock, "\t", "", 1, -1, Binary_Compare).Trim();
    ProcPos = ProcBlock.In_Str(1, ProcKeyWord, Text_Compare);
    // proc xxx
    if(ProcPos == 1)
    {
        ProcPos = ProcPos + ProcKeyWord.Len() + 1;
        // Remove tabs
        ProcChar = ProcBlock.Mid(ProcPos, 1);
        while((strcmp(ProcChar.Get_String(), " ") != 0) && (strcmp(ProcChar.Get_String(), "(") != 0) &&
              (strcmp(ProcChar.Get_String(), ";") != 0))
        {
            BufString = BufString + ProcChar;
            ProcPos++;
            ProcChar = ProcBlock.Mid(ProcPos, 1);
            // End of line
            if(ProcChar.Len() == 0) break;
        }
    }
    else
    {
        if(ProcPos != 0)
        {
            BufString = StringReplace(ProcBlock.Left(ProcPos - 1), "\t", "", 1, -1, Binary_Compare).Trim();
            // Trim possible brackets
            BracketBalance = 0;
            ProcPos--;
            if(ProcPos == 1)
            {
                ReturnValue = BufString;
                return(ReturnValue);
            }
            for(i = ProcPos; i >= 1; i--)
            {
                if(StringIsCloseBracketChar(BufString.Mid(i, 1)) == 0) ProcPos--;
                else break;
            }
            if(ProcPos == 1)
            {
                ReturnValue = BufString;
                return(ReturnValue);
            }
            for(i = ProcPos; i >= 1; i--)
            {
                ExprCharToTest = BufString.Mid(i, 1);
                if(StringIsOpenBracketChar(ExprCharToTest)) BracketBalance--;
                if(StringIsCloseBracketChar(ExprCharToTest)) BracketBalance++;
                ProcPos--;
                if(BracketBalance == 0) break;
            }
            if(ProcPos == 1)
            {
                ReturnValue = BufString;
                return(ReturnValue);
            }
            for(i = ProcPos; i >= 1; i--)
            {
                if(StringIsLabelChar(BufString.Mid(i, 1)) == 0) ProcPos--;
                else break;
            }
            if(ProcPos == 1)
            {
                ReturnValue = BufString;
                return(ReturnValue);
            }
            for(i = ProcPos; i >= 1; i--)
            {
                if(StringIsLabelChar(BufString.Mid(i, 1)))
                {
                    TempProcName = BufString.Mid(i, 1).Get_String() + (CStr) TempProcName;
                    ProcPos--;
                }
                else
                {
                    break;
                }
            }
            TempProcName = TempProcName.Trim();
            if(TempProcName.Len() != 0) BufString = TempProcName;
        }
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Open a file referenced at current line
long OpenFileUnderCursor(long FileToInclude, long ExecFile)
{
    long ReturnValue = 0;
    CStr CollapsedInclude;
    long PosCollapsedInclude = 0;
    CStr BegCollapsedInclude;
    CStr EndCollapsedInclude;
    CStr FileLine;
    CStr FileFromDirectory;
    CStr CollapsedLine;
    CStr CollapsedDir;
    CStr OpFilters;
    long CurPos = 0;
    long PosLeft = 0;
    long PosLeft2 = 0;
    long PosRight2 = 0;
    long CurPos2 = 0;
    long NbrChars = 0;
    CStr FileRight;
    CStr FileLeft;
    CStr CharF;
    CStr CharF1;
    CStr CharF2;
    CStr TrimedFileLine;
    CStr TrimedFileUnderCursor;
    int i = 0;
    long DidRight = 0;
    long DidLeft = 0;
    CStr CompleteLine;
    CStr BufString;

    if(NbForms == 0) return(ReturnValue);
    ChildStruct = LoadStructure(CurrentForm);
    FileLine = RetrieveCurrentLine(ChildStruct->hChildCodeMax);
    // Check for any select text
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    // Work only for a single line
    if(CodeMaxCurRange.posStart.nLine == CodeMaxCurRange.posEnd.nLine)
    {
        // Work only for a multicolumns
        if(CodeMaxCurRange.posStart.nCol != CodeMaxCurRange.posEnd.nCol)
        {
            FileLine = GetCurrentLogicalSelText(ChildStruct->hChildCodeMax);
        }
    }
    TrimedFileLine = FileLine.Trim();
    if(TrimedFileLine.Len() == 0) return(ReturnValue);
    CurPos = GetCurrentColNumber(ChildStruct->hChildCodeMax) + 1;
    FileRight = "";
    FileLeft = "";
    CurPos2 = CurPos;
    FileLeft = FileLeft.Space(FileLine.Len());
    PosLeft = FileLeft.Len();
    PosLeft2 = CurPos2 - 1;
    PosRight2 = CurPos;
ReCheckFile:
    DidRight = 0;
    DidLeft = 0;
    // Invalid chars : \ / : * ? " < > |
    if(CurPos <= (FileLine.Len() + 1))
    {
        for(i = CurPos; i <= FileLine.Len(); i++)
        {
            CharF = FileLine.Mid(PosRight2, 1);
            NbrChars++;
            PosRight2++;
            CurPos++;
            if(CharF == "/") break;
            if(CharF == "*") break;
            if(CharF == "?") break;
            if(CharF == "\"") break;
            if(CharF == "<") break;
            if(CharF == ">") break;
            if(CharF == "|") break;
            if(CharF == "}") break;
            CharF1 = CharF;
            if(CharF == " ") break;
            if(CharF == "\t") break;
            FileRight = FileRight + CharF;
            DidRight = 1;
        }
    }
    if(CurPos2 >= 1)
    {
        for(i = (CurPos2 - 1); i >= 1; i--)
        {
            CharF = FileLine.Mid(PosLeft2, 1);
            PosLeft2--;
            NbrChars++;
            CurPos2--;
            if(CharF == "/") break;
            if(CharF == "*") break;
            if(CharF == "?") break;
            if(CharF == "\"") break;
            if(CharF == "<") break;
            if(CharF == ">") break;
            if(CharF == "|") break;
            CharF2 = CharF;
            PosLeft--;
            if(CharF == " ") goto GoTest;
            if(CharF == "\t") goto GoTest;
            if(PosLeft < 0) break;
            FileLeft.Get_String()[PosLeft] = CharF.Get_String()[0];
            DidLeft = 1;
        }
    }
    // Needed for no carriage return ends the line
    if(NbrChars > FileLine.Len()) goto NoGo;
    // BOL and EOL reached
    if(DidLeft == 0 && DidRight == 0) goto NoGo;
GoTest:
    BufString = FileLeft.L_Trim().Get_String() + (CStr) FileRight;
    BufString = BufString.Trim();
    if(TestUnderFile(BufString) == 1) goto GoForITLastChance;
    if(strcmp(CharF1.Get_String(), " ") == 0) FileRight = FileRight + " ";
    if(FileLeft.Len() != 0)
    {
        if(FileLeft.Len() >= PosLeft)
        {
            if(strcmp(CharF2.Get_String(), " ") == 0) FileLeft.Get_String()[PosLeft] = ' ';
        }
    }
    CharF = "";
    CharF2 = "";
    goto ReCheckFile;
NoGo:
    if(FileToInclude == 1) goto GoForITLastChance;
Includeterminus:
    // Perform a test in current file directory if anything else fails
    MiscMsgBox(hMDIform.hWnd, "Can't find specified file.", MB_ERROR, Requesters);
    if(ControlIsVisible(ChildStruct->hChildCodeMax) != 0) SetFocus(ChildStruct->hChildCodeMax);
    return(ReturnValue);
GoForITLastChance:
    // No file found ? Check for a collapsed procedure
    if(FileToInclude == 1) goto ForceInInclude;
    TrimedFileUnderCursor = FileUnderCursor.Trim();
    if(TrimedFileUnderCursor.Len() == 0)
    {
ForceInInclude:
        CollapsedInclude = GetCMLangInclude(CurrentForm);
        if(CollapsedInclude.Len() == 0)
        {
            MiscMsgBox(hMDIform.hWnd, "No include definition found for current file.", MB_ERROR, Requesters);
            FileUnderCursor = "";
            return(0);
        }
        BegCollapsedInclude = CollapsedInclude;
        PosCollapsedInclude = BegCollapsedInclude.In_Str(1, "%1", Text_Compare);
        if(PosCollapsedInclude == 0)
        {
            MiscMsgBox(hMDIform.hWnd, "Error in include definition.", MB_ERROR, Requesters);
            FileUnderCursor = "";
            return(0);
        }
        BegCollapsedInclude = BegCollapsedInclude.Left(PosCollapsedInclude - 1);
        EndCollapsedInclude = CollapsedInclude.Mid(PosCollapsedInclude + 2);
        if(FileLine.In_Str(1, BegCollapsedInclude, Text_Compare) != 0)
        {
            CollapsedLine = StringReplace(FileLine, "\t", " ", 1, -1, Binary_Compare).Trim();
            CollapsedLine = StringReplace(RemoveComments(CollapsedLine), BegCollapsedInclude, "", 1, 1, Text_Compare).Trim();
            FileUnderCursor = CollapsedLine;
            FileUnderCursor = StringReplace(FileUnderCursor, "\r\n", "", 1, -1, Binary_Compare);
            FileUnderCursor = FileUnderCursor.Mid(1, FileUnderCursor.Len() - EndCollapsedInclude.Len());
            if(FileExist(FileUnderCursor) == 0)
            {
                CollapsedDir = FileGetDirectory(CMGetRealFile(ChildStruct->RFile));
                if(strcmp(CollapsedDir.Right(1).Get_String(), "\\") == 0) CollapsedDir = CollapsedDir.Left(CollapsedDir.Len() - 1);
                FileUnderCursor = CollapsedDir + (CStr) "\\" + (CStr) FileUnderCursor;
            }
        }
        else
        {
            FileUnderCursor = "";
            return(0);
        }
    }
    if(FileUnderCursor.Trim().Len() != 0)
    {
        if(FileToInclude == 1)
        {
            if(FileExist(FileUnderCursor) != 0)
            {
                MCMD_DeleteLine();
                if(IncludeFile(CurrentForm, FileUnderCursor) == 0) goto Includeterminus;
                return(1);
            }
            else
            {
                goto Includeterminus;
            }
        }
        else
        {
            if(FileIsDirectory(FileUnderCursor) == 1)
            {
                OpFilters = "All files (*.*)|*.*";
                FileFromDirectory = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, FileUnderCursor, 0, CurrentDir);
                if(FileFromDirectory.Len() == 0) return(0);
                if(ExecFile == 0) OpenFileNorm(FileFromDirectory, 0, 0, 0, 1);
                else ShellExecute(hMDIform.hWnd, "open", FileFromDirectory.Get_String(), "", FileGetDirectory(FileFromDirectory).Get_String(), SW_SHOW);
            }
            else
            {
                if(ExecFile == 0)
                {
                    // Check if there's a directory in filename
                    OpenFileNorm(FileUnderCursor, 0, 0, 0, 1);
                } else ShellExecute(hMDIform.hWnd, "open", FileUnderCursor.Get_String(), "", FileGetDirectory(FileUnderCursor).Get_String(), SW_SHOW);
            }
            return(1);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Check for a file
long TestUnderFile(CStr FileName)
{
    CStr FDirec;
    CStr FDirec2;
    CStr RepDir;
    CStr Extensions;
    LONG *MainSplit = NULL;
    LONG *ExtSplit = NULL;
    int i;

    FileUnderCursor = FileName;
    if(FileExist(FileName) != 0) goto FileOk;
    ChildStruct = LoadStructure(CurrentForm);
    FDirec = FileGetDirectory(CMGetRealFile(ChildStruct->RFile));
    if(strcmp(FDirec.Right(1).Get_String(), "\\") != 0) FDirec = FDirec + "\\";
    RepDir = FDirec;
    if(strcmp(FileName.Left(1).Get_String(), "\\") == 0) if(strcmp(FDirec.Right(1).Get_String(), "\\") == 0) FDirec = FDirec.Left(FDirec.Len() - 1);
    FDirec = FDirec + FileName;
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;

    FDirec2 = FDirec;

    // use the internal extensions to search for the file
    Extensions = IniReadKey("ExtrasExt", "AddInclude", MainIniFile);
    MainSplit = StringSplit(Extensions, "|");
    if(MainSplit)
    {
        // Get the extensions part
        Extensions = StringGetSplitElement(Extensions, MainSplit, 1);
        ExtSplit = StringSplit(Extensions, ";");
        if(ExtSplit)
        {
            for(i = 0; i <= StringGetSplitUBound(ExtSplit); i++)
            {
                FDirec = FDirec2 + StringGetSplitElement(Extensions, ExtSplit, i).Mid(2);
                FileUnderCursor = FDirec;
                if(FileExist(FDirec) != 0) goto FileOk;
            }
        }
    }
    if(MainSplit) StringReleaseSplit(MainSplit);
    if(ExtSplit) StringReleaseSplit(ExtSplit);

    // Then search registered directories
    FDirec = Dirs[DIR_ROOT] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_BIN] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_INCLUDE] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_LIB] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_HELP] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_MAIN] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_WIN] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_WINSYS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_TEMP] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_MAINHELP] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_SNIPPETS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_DATABASES] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_SKINS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_LANGSCRIPTS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_LANGUAGES] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_DOWNLOADS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_CONFIG] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_MENUS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_WIZARDS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_FILTERS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_EXTRACODE] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;
    FDirec = Dirs[DIR_SCRIPTS] + (CStr) "\\" + (CStr) FileName.Get_String();
    FileUnderCursor = FDirec;
    if(FileExist(FDirec) != 0) goto FileOk;

    return(0);
FileOk:
    if(MainSplit) StringReleaseSplit(MainSplit);
    if(ExtSplit) StringReleaseSplit(ExtSplit);
    return(1);
}

// -----------------------------------------------------------------------
// Remove a commented lines
CStr RemoveComments(CStr Str)
{
    CStr ReturnValue;
    long *CLines = 0;
    CStr LineToCheck;
    long InComment = 0;
    CStr CommentChar;
    CStr TempLines;
    long PosSngComment = 0;
    CStr CurLangComment;
    CStr BufString;
    int i = 0;

    TempLines = "";
    CurLangComment = GetCMLangComment(CurrentForm);
    CLines = StringSplit(Str, "\r\n");
    if(StringGetSplitUBound(CLines) != -1)
    {
        InComment = 0;
        for(i = 0; i <= (long) StringGetSplitUBound(CLines); i++)
        {
            LineToCheck = StringReplace(StringGetSplitElement(Str, CLines, i), "\t", " ", 1, -1, Binary_Compare).Trim();
            if(LineToCheck.Len() != 0)
            {
                if(InComment == 1)
                {
                    if(_strcmpi(LineToCheck.Left(1).Get_String(), CommentChar.Get_String()) == 0)
                    {
                        // Comment mode OFF
                        InComment = 0;
                        goto NoStoreLine;
                    }
                }
                else
                {
                    if(_strcmpi(LineToCheck.Left(7).Get_String(), "COMMENT") == 0)
                    {
                        // Pass comment word
                        LineToCheck = LineToCheck.Mid(8).Trim();
                        // Get char
                        CommentChar = LineToCheck.Left(1);
                        // Check if end comment is on same line
                        if(LineToCheck.In_Str(2, CommentChar) != 0)
                        {
                            InComment = 0;
                            // No comment mode and no line store
                            goto NoStoreLine;
                        }
                        // Comment mode ON
                        InComment = 1;
                        goto NoStoreLine;
                    }
                    TempLines = TempLines + LineToCheck + (CStr) "\r\n";
                }
            }
NoStoreLine:;
        }
    }
    StringReleaseSplit(CLines);
    CLines = StringSplit(TempLines, "\r\n");
    if(StringGetSplitUBound(CLines) != -1)
    {
        for(i = 0; i <= (long) StringGetSplitUBound(CLines); i++)
        {
            if(StringGetSplitElement(TempLines, CLines, i).Len() != 0)
            {
                LineToCheck = RemoveQuotesVB(StringGetSplitElement(TempLines, CLines, i));
                PosSngComment = LineToCheck.In_Str(1, CurLangComment);
                if(PosSngComment > 1)
                {
                    // Store the line left to the comment
                    BufString = BufString + (CStr) LineToCheck.Left(PosSngComment - 1).Trim().Get_String() + (CStr) "\r\n";
                }
                else
                {
                    if(PosSngComment == 0) BufString = BufString + (CStr) LineToCheck.Trim().Get_String() + (CStr) "\r\n";
                }
            }
        }
    }
    StringReleaseSplit(CLines);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Go to referenced line
// Returns handle of the child window or 0
HWND GoToLine(CStr FileName, long LineNumber, long FNoAddRecent)
{
    long fFile = 0;
    HWND hwndRegs = 0;
    HWND ReturnValue = 0;
    CStr ExtraCurrentDir;
    CStr ExtraSlash;
    CStr BufString;
    OFSTRUCT File_Dat;
    
    if(FileName.Len() == 0) return(0);
    BufString = FileName;
    fFile = 0;
ReCheckFileName:
    BufString = StringReplace(BufString, "/", "\\", 1, -1, Binary_Compare);
    ExtraSlash = "";
    if(_strcmpi(BufString.Left(1).Get_String(), "\\") == 0) ExtraSlash = "\\";
    BufString = StringReplace(BufString, "\\\\", "\\", 1, -1, Binary_Compare);
    BufString = ExtraSlash + (CStr) BufString;
    
    // Remove the relative path
    OpenFile(BufString.Get_String(), &File_Dat, OF_EXIST);
    
    SearchOpenedFile(hMDIform.hWnd, BufString);
    // Opened file ?
    if(FoundOpened == 1)
    {
Undecorated_Found:
        if(ControlIsVisible(FoundOpenedhWnd) == 0) ControlVisible(FoundOpenedhWnd, 1);
        SetFocus(FoundOpenedhWnd);
        ChildStruct = LoadStructure(FoundOpenedhWnd);
        // Check if the line is empty
        while(CMGetCurrentLine(ChildStruct->hChildCodeMax, LineNumber) == "")
        {
            LineNumber++;
            if(LineNumber >= CM_GetLineCount(ChildStruct->hChildCodeMax))
            {
                LineNumber = CM_GetLineCount(ChildStruct->hChildCodeMax) - 1;
                break;
            }
        }
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, LineNumber);
        
        SetFocus(ChildStruct->hChildCodeMax);
        ReturnValue = FoundOpenedhWnd;
    }
    else
    {
        SearchOpenedFile(hMDIform.hWnd, File_Dat.szPathName);
        if(FoundOpened == 1) goto Undecorated_Found;
        if(NbForms != 0)
        {
            // Get the directory of the current file (if possible)
            ChildStruct = LoadStructure(CurrentForm);
            // Check if a file has been loaded
            if(ChildStruct->FileLoaded)
            {
                ExtraCurrentDir = FileGetDirectory(CMGetRealFile(ChildStruct->RFile)).Get_String() + (CStr) BufString;
                // Check if file exists
                if(FileExist(ExtraCurrentDir))
                {
                    BufString = ExtraCurrentDir;
                    goto ReCheckFileName;
                }
            }
        }
        hwndRegs = OpenFileNorm(File_Dat.szPathName, 0, 1, 0, FNoAddRecent);
        if(LoadResultGlob == 1)
        {
            ChildStruct = LoadStructure(hwndRegs);
            while(CMGetCurrentLine(ChildStruct->hChildCodeMax, LineNumber) == "")
            {
                LineNumber++;
                if(LineNumber >= CM_GetLineCount(ChildStruct->hChildCodeMax))
                {
                    LineNumber = CM_GetLineCount(ChildStruct->hChildCodeMax) - 1;
                    break;
                }
            }
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, LineNumber);

            SetFocus(ChildStruct->hChildCodeMax);
            ReturnValue = hwndRegs;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Go to referenced file
HWND FocusFile(CStr FileName, long FNoAddRecent)
{
    long fFile = 0;
    HWND hwndRegs = 0;

    fFile = 0;
    SearchOpenedFile(hMDIform.hWnd, FileName);
    // Opened file ?
    if(FoundOpened == 1)
    {
        if(ControlIsVisible(FoundOpenedhWnd) == 0) ControlVisible(FoundOpenedhWnd, 1);
        SetFocus(FoundOpenedhWnd);
        ChildStruct = LoadStructure(FoundOpenedhWnd);
        SetFocus(FoundOpenedhWnd);
        SetFocus(ChildStruct->hChildCodeMax);
        return(FoundOpenedhWnd);
    }
    else
    {
        hwndRegs = OpenFileNorm(FileName, 0, 1, 0, FNoAddRecent);
        if(LoadResultGlob == 1)
        {
            ChildStruct = LoadStructure(hwndRegs);
            SetFocus(FoundOpenedhWnd);
            SetFocus(ChildStruct->hChildCodeMax);
            return(hwndRegs);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Retrieve the filename associated with a treeview entry
CStr GetFileNameFromTreeView(HTREEITEM hItem)
{
    CStr ReturnValue;
    CStr BufString;
    long PosRefFile = 0;

    BufString = TreeViewGetItemText(hTreeView, hItem);
    if(BufString.Len() != 0)
    {
        PosRefFile = BufString.In_Str(1, "(");
        if(PosRefFile != 0)
        {
            PosRefFile++;
            BufString = BufString.Mid(PosRefFile);
            BufString = BufString.Mid(1, BufString.Len() - 1);
        }
        else
        {
            BufString = "";
        }
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the label associated with a treeview entry
CStr GetLabelFromTreeView(HTREEITEM hItem)
{
    CStr ReturnValue;
    CStr BufString;
    long PosRefFile = 0;

    BufString = TreeViewGetItemText(hTreeView, hItem);
    if(BufString.Len() != 0)
    {
        PosRefFile = BufString.In_Str(1, "(");
        if(PosRefFile != 0)
        {
            PosRefFile--;
            BufString = BufString.Left(PosRefFile).Trim();
        }
        else
        {
            BufString = "";
        }
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Remove a file from the results listview
void RemFile(CStr FileKey)
{
    ProcLines--;
    ListViewDeleteItem(FrmResultsListView, ProcLines);
}

// -----------------------------------------------------------------------
// Add a file to the results listview
void AddFile(CStr FileKey, long j, CStr Info, CStr Comment)
{
    CStr jStr;

    jStr = j + 1;
    ListViewAddItem(FrmResultsListView, FileKey, ProcLines, 0);
    ListViewSetSubItem(FrmResultsListView, jStr, ProcLines, 1);
    ListViewSetSubItem(FrmResultsListView, Info, ProcLines, 2);
    ListViewSetSubItem(FrmResultsListView, StringReplace(Comment, "\t", " ", 1, -1, Binary_Compare).Trim(), ProcLines, 3);
    ProcLines++;
}

// -----------------------------------------------------------------------
// Load a file in memory to search
void SearchOpenFile(CStr FileToSearch)
{
    CurrentFileOpen = FileToSearch;
    CurrentFilePointer = (long) MLoadFile(CurrentFileOpen.Get_String(), &CurrentFileSize);
    CurrentFileLines = CountFileLines((char *) CurrentFilePointer, CurrentFileSize);
}

// -----------------------------------------------------------------------
// Retrieve a file to search from CodeMax
void SearchOpenFileCM(HWND hWnd)
{
    CurrentFileSize = CM_GetTextLengthAll(hWnd, 1);
    CurrentFilePointer = AllocMem(CurrentFileSize);
    CM_GetTextAllLng(hWnd, CurrentFilePointer);
    CurrentFileLines = CountFileLines((char *) CurrentFilePointer, CurrentFileSize);
}

// -----------------------------------------------------------------------
// Search routines

// -----------------------------------------------------------------------
// Search a keyword in a text
// Return found line
long SearchInText(long Txt, long TxtLen, CStr KeyW, long SearchType, long StartPo)
{
    long ReturnValue = 0;

    ReturnValue = Str_InString(StartPo, (char *) Txt, KeyW.Lower_Case().Get_String());
    // Don't check if no keyword
    CurrentRetriPosition = ReturnValue;
    if(ReturnValue != 0) ReturnValue = (long) GetCurrentLine((char *) Txt, TxtLen, ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve a line from a text
CStr SearchGetTextLine(long Txt, long TxtLen, long LineNumber)
{
    char *TxtLineAddr = 0;
    long TxtLineAddrLen = 0;
    long TxtLineLenAll = 0;
    CStr ReturnValue;
    CStr BufString;

    TxtLineAddr = GetFileLineAddress((char *) Txt, TxtLen, LineNumber);
    // Get line len
    TxtLineLenAll = strlen(TxtLineAddr);
    TxtLineAddrLen = MGetStrLenCR(TxtLineAddr, TxtLineLenAll);
    BufString = BufString.String(TxtLineAddrLen, 1);
    RtlCopyMemory(BufString.Get_String(), (void *) TxtLineAddr, TxtLineAddrLen);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve a line from a text with string passed
CStr SearchGetTextLineStr(CStr Txt, long TxtLen, long LineNumber)
{
    long TxtLineAddr = 0;
    long TxtLineAddrLen = 0;
    long TxtLineLenAll = 0;
    CStr ReturnValue;
    CStr BufString;
   
    TxtLineAddr = (long) GetFileLineAddress(Txt.Get_String(), TxtLen, LineNumber);
    // Get line len
    TxtLineLenAll = TxtLineAddr - (long) Txt.Get_String();
    TxtLineAddrLen = MGetStrLenCR((char *) TxtLineAddr, TxtLineLenAll);
    BufString = BufString.String(TxtLineAddrLen, 1);
    RtlCopyMemory(BufString.Get_String(), (void *) TxtLineAddr, TxtLineAddrLen);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve a line
CStr SearchGetLines(long LineNumber)
{
    long TxtLineLenAll = 0;
    CStr ReturnValue;
    CStr BufString;

    CurrentFileCLine = GetFileLineAddress((char *) CurrentFilePointer, CurrentFileSize, LineNumber);
    // Get line len
    TxtLineLenAll = strlen(CurrentFileCLine);
    CurrentFileLineLen = MGetStrLenCR(CurrentFileCLine, TxtLineLenAll);
    BufString = BufString.String(CurrentFileLineLen, 1);
    RtlCopyMemory(BufString.Get_String(), (void *) CurrentFileCLine, CurrentFileLineLen);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Search a keyword from CodeMax
// Return found line
long SearchInFileCM(HWND hWnd, CStr KeyW, long SearchType, long StartPo, CStr CommentC, long RawMode)
{
    long ReturnValue = 0;

    CurrentFileSizeBis = CM_GetTextLengthAll(hWnd, 1);
    CurrentFilePointerBis = AllocMem(CurrentFileSizeBis);
    CM_GetTextAllLng(hWnd, CurrentFilePointerBis);
    // lower case all if vbText_Compare
    LoCase((char *) CurrentFilePointerBis);
    CurrentFileSearchKey = KeyW.Lower_Case().Get_String();
    // Not raw ?
    if(RawMode == 0)
    {
        RemoveCommentBlocksFromFileByChild(CurrentFilePointerBis, CurrentFileSizeBis, hWnd);
        RemoveSingleLineComment((char *) CurrentFilePointerBis, CurrentFileSizeBis, CommentC.Get_String());
    }
    ReturnValue = Str_InString(StartPo, (char *) CurrentFilePointerBis, CurrentFileSearchKey.Get_String());
    CurrentRetriPosition = ReturnValue;
    // Don't check if no keyword
    if(ReturnValue != 0) ReturnValue = (long) GetCurrentLine((char *) CurrentFilePointerBis, CurrentFileSizeBis, ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Search a keyword from CodeMax (continued from previous function)
// Return found line
long ContinueSearchInFileCM(HWND hWnd, CStr KeyW, long SearchType, long StartPo, CStr CommentC)
{
    long ReturnValue = 0;

    CurrentFileSearchKey = KeyW.Lower_Case().Get_String();
    ReturnValue = Str_InString(StartPo, (char *) CurrentFilePointerBis, CurrentFileSearchKey.Get_String());
    CurrentRetriPosition = ReturnValue;
    if(ReturnValue != 0) ReturnValue = (long) GetCurrentLine((char *) CurrentFilePointerBis, CurrentFileSizeBis, ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Search a keyword in a file
// Return found line
long SearchInFile(CStr FileName, CStr KeyW, long SearchType, long StartPo, long RawMode)
{
    long ReturnValue = 0;
  
    CurrentFileOpenBis = FileName;
    CurrentFilePointerBis = (long) MLoadFile(CurrentFileOpenBis.Get_String(), &CurrentFileSizeBis);
    // lower case all if vbText_Compare
    LoCase((char *) CurrentFilePointerBis);
    CurrentFileSearchKey = KeyW.Lower_Case().Get_String();
    // Not raw ?
    if(RawMode == 0)
    {
        RemoveCommentBlocksFromFileByFile(CurrentFilePointerBis, CurrentFileSizeBis, CurrentFileOpenBis);
        RemoveSingleLineComment((char *) CurrentFilePointerBis, CurrentFileSizeBis, GetCMLangCommentByFile(FileName).Get_String());
    }
    ReturnValue = Str_InString(StartPo, (char *) CurrentFilePointerBis, CurrentFileSearchKey.Get_String());
    CurrentRetriPosition = ReturnValue;
    // Don't check if no keyword
    if(ReturnValue != 0) ReturnValue = (long) GetCurrentLine((char *) CurrentFilePointerBis, CurrentFileSizeBis, ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Search a keyword in a file (continued from previous function)
// Return found line
long ContinueSearchInFile(CStr FileName, CStr KeyW, long SearchType, long StartPo)
{
    long ReturnValue = 0;

    CurrentFileSearchKey = KeyW.Lower_Case().Get_String();
    ReturnValue = Str_InString(StartPo, (char *) CurrentFilePointerBis, CurrentFileSearchKey.Get_String());
    CurrentRetriPosition = ReturnValue;
    if(ReturnValue != 0) ReturnValue = (long) GetCurrentLine((char *) CurrentFilePointerBis, CurrentFileSizeBis, ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Free the search file memory
void SearchCloseFile(void)
{
    if(CurrentFilePointerBis != 0)
    {
        FreeMem(CurrentFilePointerBis);
        CurrentFilePointerBis = 0;
    }
    if(CurrentFilePointer != 0)
    {
        FreeMem(CurrentFilePointer);
        CurrentFilePointer = 0;
    }
}

// -----------------------------------------------------------------------
// Fill the variables arrays
void FillVarCheckTable(void)
{
    CStr GrepSep;
    CStr GrepLine;
    long *GrepLineArray = 0;
    int i = 0;

    if(cGrepIniFile.Len() != 0)
    {
        GrepSep = IniReadKey("ListSep", "Separator", cGrepIniFile);
        if(GrepSep.Len() == 0) goto ErrInGrep;
        TabCheckVars.Erase();
        TabCheckVarsPos.Erase();
        TabCheckVarsComment.Erase();
        for(i = 0; i <= 999; i++)
        {
            GrepLine = IniReadKey("VarResults", "Var" + (CStr) StringNumberComplement(i, 3).Get_String(), cGrepIniFile);
            if(GrepLine.Len() == 0) break;
            GrepLineArray = StringSplit(GrepLine, GrepSep);
            if(StringGetSplitUBound(GrepLineArray) != 2) goto ErrInGrepParse;
            FillVarEntry(StringGetSplitElement(GrepLine, GrepLineArray, 0), StringGetSplitElement(GrepLine, GrepLineArray, 1).Get_Long(), StringGetSplitElement(GrepLine, GrepLineArray, 2));
            StringReleaseSplit(GrepLineArray);
        }
    }
    else
    {
ErrInGrepParse:
        StringReleaseSplit(GrepLineArray);
        NotifGrepError = 1;
ErrInGrep:
        FillVarEntry(",", POS_BEFORE, "USED");
        FillVarEntry(",", POS_AFTER, "USED");
        FillVarEntry(".", POS_AFTER, "USED");
        FillVarEntry(".", POS_BEFORE, "USED");
        FillVarEntry("[", POS_BEFORE, "USED");
        FillVarEntry("]", POS_AFTER, "USED");
        FillVarEntry(":", POS_BEFORE, "USED");
        FillVarEntry(":", POS_AFTER, "USED");
        FillVarEntry("cs:", POS_BEFORE, "USED");
        FillVarEntry("ds:", POS_BEFORE, "USED");
        FillVarEntry("ss:", POS_BEFORE, "USED");
        FillVarEntry("es:", POS_BEFORE, "USED");
        FillVarEntry("fs:", POS_BEFORE, "USED");
        FillVarEntry("gs:", POS_BEFORE, "USED");
        FillVarEntry("==", POS_BEFORE, "USED");
        FillVarEntry("offset", POS_BEFORE, "USED");
        FillVarEntry("addr", POS_BEFORE, "USED");
        FillVarEntry("local", POS_BEFORE, "DECLARATION");
        FillVarEntry("public", POS_BEFORE, "DECLARATION");
        FillVarEntry("db", POS_AFTER, "DECLARATION");
        FillVarEntry("dw", POS_AFTER, "DECLARATION");
        FillVarEntry("dd", POS_AFTER, "DECLARATION");
        FillVarEntry("df", POS_AFTER, "DECLARATION");
        FillVarEntry("dq", POS_AFTER, "DECLARATION");
        FillVarEntry("dt", POS_AFTER, "DECLARATION");
        FillVarEntry("db", POS_BEFORE, "USED");
        FillVarEntry("dw", POS_BEFORE, "USED");
        FillVarEntry("dd", POS_BEFORE, "USED");
        FillVarEntry("df", POS_BEFORE, "USED");
        FillVarEntry("dq", POS_BEFORE, "USED");
        FillVarEntry("dt", POS_BEFORE, "USED");
        FillVarEntry("push", POS_BEFORE, "USED");
        FillVarEntry("pop", POS_BEFORE, "USED");
        FillVarEntry("dec", POS_BEFORE, "USED");
        FillVarEntry("div", POS_BEFORE, "USED");
        FillVarEntry("mul", POS_BEFORE, "USED");
        FillVarEntry("inc", POS_BEFORE, "USED");
        FillVarEntry("lgdt", POS_BEFORE, "USED");
        FillVarEntry("lidt", POS_BEFORE, "USED");
        FillVarEntry("lldt", POS_BEFORE, "USED");
        FillVarEntry("inc", POS_BEFORE, "USED");
        FillVarEntry("lmsw", POS_BEFORE, "USED");
        FillVarEntry("ltr", POS_BEFORE, "USED");
        FillVarEntry("neg", POS_BEFORE, "USED");
        FillVarEntry("not", POS_BEFORE, "USED");
        FillVarEntry("setae", POS_BEFORE, "USED");
        FillVarEntry("setnb", POS_BEFORE, "USED");
        FillVarEntry("setb", POS_BEFORE, "USED");
        FillVarEntry("setnae", POS_BEFORE, "USED");
        FillVarEntry("setbe", POS_BEFORE, "USED");
        FillVarEntry("setna", POS_BEFORE, "USED");
        FillVarEntry("setc", POS_BEFORE, "USED");
        FillVarEntry("sete", POS_BEFORE, "USED");
        FillVarEntry("setz", POS_BEFORE, "USED");
        FillVarEntry("setg", POS_BEFORE, "USED");
        FillVarEntry("setnle", POS_BEFORE, "USED");
        FillVarEntry("setge", POS_BEFORE, "USED");
        FillVarEntry("setnl", POS_BEFORE, "USED");
        FillVarEntry("setl", POS_BEFORE, "USED");
        FillVarEntry("setnge", POS_BEFORE, "USED");
        FillVarEntry("setle", POS_BEFORE, "USED");
        FillVarEntry("setng", POS_BEFORE, "USED");
        FillVarEntry("setbe", POS_BEFORE, "USED");
        FillVarEntry("setno", POS_BEFORE, "USED");
        FillVarEntry("setnz", POS_BEFORE, "USED");
        FillVarEntry("seto", POS_BEFORE, "USED");
        FillVarEntry("setpo", POS_BEFORE, "USED");
        FillVarEntry("setgdt", POS_BEFORE, "USED");
        FillVarEntry("setp", POS_BEFORE, "USED");
        FillVarEntry("setpe", POS_BEFORE, "USED");
        FillVarEntry("setnp", POS_BEFORE, "USED");
        FillVarEntry("sets", POS_BEFORE, "USED");
        FillVarEntry("sgdt", POS_BEFORE, "USED");
        FillVarEntry("sidt", POS_BEFORE, "USED");
        FillVarEntry("sldt", POS_BEFORE, "USED");
        FillVarEntry("smsw", POS_BEFORE, "USED");
        FillVarEntry("str", POS_BEFORE, "USED");
        FillVarEntry("verr", POS_BEFORE, "USED");
    }
}

// -----------------------------------------------------------------------
// Fill an entry in variables arrays
void FillVarEntry(CStr KeyWord, long Position, CStr Comment)
{
    TabCheckVars.Add(KeyWord.Get_String());
    TabCheckVarsPos.Add(Position);
    TabCheckVarsComment.Add(Comment.Get_String());
}

// -----------------------------------------------------------------------
// Fill the functions arrays
void FillFuncCheckTable(void)
{
    CStr GrepSep;
    CStr GrepLine;
    long *GrepLineArray = 0;
    int i = 0;

    if(cGrepIniFile.Len() != 0)
    {
        GrepSep = IniReadKey("ListSep", "Separator", cGrepIniFile);
        if(GrepSep.Len() == 0) goto ErrInGrep;
        TabCheckFunc.Erase();
        TabCheckFuncPos.Erase();
        TabCheckFuncComment.Erase();

        for(i = 0; i <= 999; i++)
        {
            GrepLine = IniReadKey("FncResults", "Fnc" + (CStr) StringNumberComplement(i, 3).Get_String(), cGrepIniFile);
            if(GrepLine.Len() == 0) break;
            GrepLineArray = StringSplit(GrepLine, GrepSep);
            if(StringGetSplitUBound(GrepLineArray) != 2) goto ErrInGrepParse;
            FillFuncEntry(StringGetSplitElement(GrepLine, GrepLineArray, 0), StringGetSplitElement(GrepLine, GrepLineArray, 1).Get_Long(), StringGetSplitElement(GrepLine, GrepLineArray, 2));
            StringReleaseSplit(GrepLineArray);
        }
    }
    else
    {
ErrInGrepParse:
        StringReleaseSplit(GrepLineArray);
        NotifGrepError = 1;
ErrInGrep:
        // Default cgrep definition
        FillFuncEntry("call", POS_BEFORE, "USED");
        FillFuncEntry("invoke", POS_BEFORE, "USED");
        FillFuncEntry("procedure", POS_BEFORE, "DECLARATION");
        FillFuncEntry("function", POS_BEFORE, "DECLARATION");
        FillFuncEntry("void", POS_BEFORE, "DECLARATION");
        FillFuncEntry("proto", POS_AFTER, "DECLARATION");
        FillFuncEntry("procdesc", POS_AFTER, "DECLARATION");
        FillFuncEntry("proc", POS_AFTER, "ENTRYPOINT");
        FillFuncEntry("proc", POS_BEFORE, "ENTRYPOINT");
        FillFuncEntry("endp", POS_AFTER, "ENDPOINT");
        FillFuncEntry("endp", POS_BEFORE, "ENDPOINT");
        FillFuncEntry("extrn", POS_BEFORE, "EXTERN REF.");
    }
}

// -----------------------------------------------------------------------
// Fill an entry in functions arrays
void FillFuncEntry(CStr KeyWord, long Position, CStr Comment)
{
    TabCheckFunc.Add(KeyWord.Get_String());
    TabCheckFuncPos.Add(Position);
    TabCheckFuncComment.Add(Comment.Get_String());
}

// -----------------------------------------------------------------------
// Search for a variable declaration
long SearchVarEntry(HWND hWnd, CStr KeyWord)
{
    VarProcFound = 0;
    VarProcToSearch = KeyWord.Lower_Case();
    VarProcToSearchReal = KeyWord;
    VarProcFoundhwnd = 0;
    if(KeyWord.Len() == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ".") == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ",") == 0) return(VarProcFound);
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchVarEntry, 0);
    return(VarProcFound);
}

int CALLBACK EnumSearchVarEntry(HWND hWnd, long lParam)
{
    CStr BufString;

    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        VarProcFound = 0;
        VarProcFoundLine = 0;
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    VarProcFoundPos = 1;
                    VarProcText = VarProcText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
                    CM_GetTextAll(ChildStruct->hChildCodeMax, VarProcText.Get_String());
                    RemoveCommentBlocksFromFileByChildStr(VarProcText, VarProcText.Len(), ChildStruct->hChildCodeMax);
                    RemoveSingleLineComment(VarProcText.Get_String(), VarProcText.Len(), GetCMLangComment(hWnd).Get_String());
                    VarProcTextStrip = VarProcText.String(VarProcText.Len(), 1);
                    RemoveCommChar(VarProcText.Get_String(), VarProcTextStrip.Get_String(), VarProcText.Len());
                    TabToSpace(VarProcTextStrip.Get_String(), VarProcTextStrip.Len());
                    VarProcText = VarProcTextStrip.String(VarProcTextStrip.Len(), 1);
                    MarkSpaces(VarProcTextStrip.Get_String(), VarProcText.Get_String(), VarProcTextStrip.Len());
                    LoCase(VarProcText.Get_String());
SearchVarEntryLoop:
                    VarProcFoundPos = Str_InString(VarProcFoundPos, VarProcText.Get_String(), VarProcToSearch.Get_String());
                    if(VarProcFoundPos != 0)
                    {
                        VarProcFoundLine = (long) GetCurrentLine(VarProcText.Get_String(), VarProcText.Len(), VarProcFoundPos);
                        VarProcFoundLineTxt = SearchGetTextLineStr(VarProcText, VarProcText.Len(), VarProcFoundLine - 1);
                        VarProcFoundLineTxt = StringReplace(VarProcFoundLineTxt, VarProcFoundLineTxt.Chr(2).Get_String(), "", 1, -1, Binary_Compare);
                        if(VarProcFoundLineTxt.In_Str(1, "LOCAL" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "DB", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "DW", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "DD", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "DF", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "DQ", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "DT", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "QWORD", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "long", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "WORD", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "BYTE", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "BOOLEAN", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "BOOL", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "WCHAR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "CHAR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "LPSTR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "LPCSTR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "HANDLE", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "HDWP", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "HFILE", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "TCHAR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "OLECHAR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "LPOLESTR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "LPCOLESTR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "FLOAT", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "USHORT", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "ULONG", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "DOUBLE", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "EQU", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":DB", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":DW", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":DD", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":DF", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":DQ", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":DT", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":QWORD", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":long", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":WORD", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":BYTE", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":BOOLEAN", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":BOOL", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":WCHAR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":CHAR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":LPSTR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":LPCSTR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":HANDLE", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":HDWP", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":HFILE", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":TCHAR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":OLECHAR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":LPOLESTR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":LPCOLESTR", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":FLOAT", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":USHORT", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":ULONG", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":DOUBLE", Text_Compare) != 0) goto FoundVarEntry;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ":EQU", Text_Compare) != 0) goto FoundVarEntry;
                        BufString = VarProcToSearch + (CStr) ":";
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch, Text_Compare) == 1) if(_strcmpi(VarProcFoundLineTxt.Left(VarProcToSearch.Len() + 1).Get_String(), BufString.Get_String()) == 0) goto FoundVarEntry;
                        VarProcFoundPos++;
                        goto SearchVarEntryLoop;
FoundVarEntry:          // Mark as found and exit
                        VarProcFound = 1;
                        VarProcFoundhwnd = hWnd;
                        return(0);
                    }
                }
            }
        }
        return(1);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Search for a variable next use
long SearchVarNextUse(HWND hWnd, CStr KeyWord, long FirstLine)
{
    VarProcFound = 0;
    VarProcToSearch = KeyWord.Lower_Case();
    VarProcToSearchReal = KeyWord;
    VarProcFoundhwnd = 0;
    if(KeyWord.Len() == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ".") == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ",") == 0) return(VarProcFound);
    ChildStruct = LoadStructure(CurrentForm);
    VarProcText = VarProcText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
    CM_GetTextAll(ChildStruct->hChildCodeMax, VarProcText.Get_String());
    VarProcFoundPos = GetFileLineOffset(VarProcText.Get_String(), VarProcText.Len(), FirstLine + 1);
    VarProcText = "";
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchVarNextUse, 0);
    return(VarProcFound);
}

int CALLBACK EnumSearchVarNextUse(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        VarProcFound = 0;
        VarProcFoundLine = 0;
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    VarProcText = VarProcText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
                    CM_GetTextAll(ChildStruct->hChildCodeMax, VarProcText.Get_String());
                    RemoveCommentBlocksFromFileByChildStr(VarProcText, VarProcText.Len(), ChildStruct->hChildCodeMax);
                    RemoveSingleLineComment(VarProcText.Get_String(), VarProcText.Len(), GetCMLangComment(hWnd).Get_String());
                    VarProcTextStrip = VarProcTextStrip.String(VarProcText.Len(), 1);
                    RemoveCommChar(VarProcText.Get_String(), VarProcTextStrip.Get_String(), VarProcText.Len());
                    TabToSpace(VarProcTextStrip.Get_String(), VarProcTextStrip.Len());
                    VarProcText = VarProcText.String(VarProcTextStrip.Len(), 1);
                    MarkSpaces(VarProcTextStrip.Get_String(), VarProcText.Get_String(), VarProcTextStrip.Len());
                    LoCase(VarProcText.Get_String());
SearchVarUseLoop:   VarProcFoundPos = Str_InString(VarProcFoundPos, VarProcText.Get_String(), VarProcToSearch.Get_String());
                    if(VarProcFoundPos != 0)
                    {
                        VarProcFoundLine = (long) GetCurrentLine(VarProcText.Get_String(), VarProcText.Len(), VarProcFoundPos);
                        VarProcFoundLineTxt = SearchGetTextLineStr(VarProcText, VarProcText.Len(), VarProcFoundLine - 1);
                        // Check if variable is the first word of the line (it's a declaration then)
                        VarProcFoundLineTxt = StringReplace(VarProcFoundLineTxt, VarProcFoundLineTxt.Chr(2), "", 1, -1, Binary_Compare);
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch, Text_Compare) != 1)
                        {
                            if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ",", Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "addr" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "offset" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "," + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "+" + (CStr) VarProcToSearch + (CStr) "]", Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "[" + (CStr) VarProcToSearch + (CStr) "]", Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "." + (CStr) VarProcToSearch + (CStr) ",", Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) ".", Text_Compare) != 0) goto FoundVarUse;
                        }
                        VarProcFoundPos++;
                        goto SearchVarUseLoop;
FoundVarUse:            // Mark as found and exit
                        VarProcFound = 1;
                        VarProcFoundhwnd = hWnd;
                        return(0);
                    }
                    VarProcFoundPos = 1;
                }
            }
        }
        return(1);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Search for a procedure prototype
long SearchProcProto(HWND hWnd, CStr KeyWord)
{
    VarProcFound = 0;
    VarProcToSearch = KeyWord.Lower_Case();
    VarProcToSearchReal = KeyWord;
    VarProcFoundhwnd = 0;
    if(KeyWord.Len() == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ".") == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ",") == 0) return(VarProcFound);
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchProcProto, 0);
    return(VarProcFound);
}

int CALLBACK EnumSearchProcProto(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        VarProcFound = 0;
        VarProcFoundLine = 0;
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    VarProcFoundPos = 1;
                    VarProcText = VarProcText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
                    CM_GetTextAll(ChildStruct->hChildCodeMax, VarProcText.Get_String());
                    RemoveCommentBlocksFromFileByChildStr(VarProcText, VarProcText.Len(), ChildStruct->hChildCodeMax);
                    RemoveSingleLineComment(VarProcText.Get_String(), VarProcText.Len(), GetCMLangComment(hWnd).Get_String());
                    VarProcTextStrip = VarProcTextStrip.String(VarProcText.Len(), 1);
                    RemoveCommChar(VarProcText.Get_String(), VarProcTextStrip.Get_String(), VarProcText.Len());
                    TabToSpace(VarProcTextStrip.Get_String(), VarProcTextStrip.Len());
                    VarProcText = VarProcText.String(VarProcTextStrip.Len(), 1);
                    MarkSpaces(VarProcTextStrip.Get_String(), VarProcText.Get_String(), VarProcTextStrip.Len());
                    LoCase(VarProcText.Get_String());
SearchProcProtoLoop:
                    VarProcFoundPos = Str_InString(VarProcFoundPos, VarProcText.Get_String(), VarProcToSearch.Get_String());
                    if(VarProcFoundPos != 0)
                    {
                        VarProcFoundLine = (long) GetCurrentLine(VarProcText.Get_String(), VarProcText.Len(), VarProcFoundPos);
                        VarProcFoundLineTxt = SearchGetTextLineStr(VarProcText, VarProcText.Len(), VarProcFoundLine - 1);
                        VarProcFoundLineTxt = StringReplace(VarProcFoundLineTxt, VarProcFoundLineTxt.Chr(2), "", 1, -1, Binary_Compare);
                        if(VarProcFoundLineTxt.In_Str(1, "EXTRN" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundProcProto;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "PROTO", Text_Compare) != 0) goto FoundProcProto;
                        VarProcFoundPos++;
                        goto SearchProcProtoLoop;
FoundProcProto:         // Mark as found and exit
                        VarProcFound = 1;
                        VarProcFoundhwnd = hWnd;
                        return(0);
                    }
                }
            }
        }
        return(1);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Search for a procedure declaration
long SearchProcDeclare(HWND hWnd, CStr KeyWord)
{
    VarProcFound = 0;
    VarProcToSearch = KeyWord.Lower_Case();
    VarProcToSearchReal = KeyWord;
    VarProcFoundhwnd = 0;
    if(KeyWord.Len() == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ".") == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ",") == 0) return(VarProcFound);
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchProcDeclare, 0);
    return(VarProcFound);
}

int CALLBACK EnumSearchProcDeclare(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        VarProcFound = 0;
        VarProcFoundLine = 0;
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    VarProcFoundPos = 1;
                    VarProcText = VarProcText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
                    CM_GetTextAll(ChildStruct->hChildCodeMax, VarProcText.Get_String());
                    RemoveCommentBlocksFromFileByChildStr(VarProcText, VarProcText.Len(), ChildStruct->hChildCodeMax);
                    RemoveSingleLineComment(VarProcText.Get_String(), VarProcText.Len(), GetCMLangComment(hWnd).Get_String());
                    VarProcTextStrip = VarProcTextStrip.String(VarProcText.Len(), 1);
                    RemoveCommChar(VarProcText.Get_String(), VarProcTextStrip.Get_String(), VarProcText.Len());
                    TabToSpace(VarProcTextStrip.Get_String(), VarProcTextStrip.Len());
                    VarProcText = VarProcText.String(VarProcTextStrip.Len(), 1);
                    MarkSpaces(VarProcTextStrip.Get_String(), VarProcText.Get_String(), VarProcTextStrip.Len());
                    LoCase(VarProcText.Get_String());
SearchProcDeclareLoop:
                    VarProcFoundPos = Str_InString(VarProcFoundPos, VarProcText.Get_String(), VarProcToSearch.Get_String());
                    if(VarProcFoundPos != 0)
                    {
                        VarProcFoundLine = (long) GetCurrentLine(VarProcText.Get_String(), VarProcText.Len(), VarProcFoundPos);
                        VarProcFoundLineTxt = SearchGetTextLineStr(VarProcText, VarProcText.Len(), VarProcFoundLine - 1);
                        VarProcFoundLineTxt = StringReplace(VarProcFoundLineTxt, VarProcFoundLineTxt.Chr(2), "", 1, -1, Binary_Compare);
                        if(VarProcFoundLineTxt.In_Str(1, "PROC" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundProcDeclare;
                        if(VarProcFoundLineTxt.In_Str(1, VarProcToSearch + (CStr) "PROC", Text_Compare) != 0) goto FoundProcDeclare;
                        VarProcFoundPos++;
                        goto SearchProcDeclareLoop;
FoundProcDeclare:       // Mark as found and exit
                        VarProcFound = 1;
                        VarProcFoundhwnd = hWnd;
                        return(0);
                    }
                }
            }
        }
        return(1);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Search for a procedure next use
long SearchProcNextUse(HWND hWnd, CStr KeyWord, long FirstLine)
{
    VarProcFound = 0;
    VarProcToSearch = KeyWord.Lower_Case();
    VarProcToSearchReal = KeyWord;
    VarProcFoundhwnd = 0;
    if(KeyWord.Len() == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ".") == 0) return(VarProcFound);
    if(strcmp(KeyWord.Get_String(), ",") == 0) return(VarProcFound);
    ChildStruct = LoadStructure(CurrentForm);
    VarProcText = VarProcText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
    CM_GetTextAll(ChildStruct->hChildCodeMax, VarProcText.Get_String());
    VarProcFoundPos = GetFileLineOffset(VarProcText.Get_String(), VarProcText.Len(), FirstLine + 1);
    VarProcText = "";
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchProcNextUse, 0);
    return(VarProcFound);
}

int CALLBACK EnumSearchProcNextUse(HWND hWnd, long lParam)
{
    CStr BufString;

    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        VarProcFound = 0;
        VarProcFoundLine = 0;
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    VarProcText = VarProcText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
                    CM_GetTextAll(ChildStruct->hChildCodeMax, VarProcText.Get_String());
                    RemoveCommentBlocksFromFileByChildStr(VarProcText, VarProcText.Len(), ChildStruct->hChildCodeMax);
                    RemoveSingleLineComment(VarProcText.Get_String(), VarProcText.Len(), GetCMLangComment(hWnd).Get_String());
                    VarProcTextStrip = VarProcTextStrip.String(VarProcText.Len(), 1);
                    RemoveCommChar(VarProcText.Get_String(), VarProcTextStrip.Get_String(), VarProcText.Len());
                    TabToSpace(VarProcTextStrip.Get_String(), VarProcTextStrip.Len());
                    VarProcText = VarProcText.String(VarProcTextStrip.Len(), 1);
                    MarkSpaces(VarProcTextStrip.Get_String(), VarProcText.Get_String(), VarProcTextStrip.Len());
                    LoCase(VarProcText.Get_String());
SearchVarUseLoop:   VarProcFoundPos = Str_InString(VarProcFoundPos, VarProcText.Get_String(), VarProcToSearch.Get_String());
                    if(VarProcFoundPos != 0)
                    {
                        VarProcFoundLine = (long) GetCurrentLine(VarProcText.Get_String(), VarProcText.Len(), VarProcFoundPos);
                        VarProcFoundLineTxt = SearchGetTextLineStr(VarProcText, VarProcText.Len(), VarProcFoundLine - 1);
                        // Check if variable is the first word of the line (it's declaration then)
                        BufString = StringReplace(VarProcFoundLineTxt, VarProcFoundLineTxt.Chr(2), "", 1, -1, Binary_Compare);
                        if(BufString.In_Str(1, VarProcToSearch, Text_Compare) != 1)
                        {
                            VarProcFoundLineTxt = BufString;
                            if(VarProcFoundLineTxt.In_Str(1, "CALL" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "INVOKE" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "JMP" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundVarUse;
                            if(VarProcFoundLineTxt.In_Str(1, "ADDR" + (CStr) VarProcToSearch, Text_Compare) != 0) goto FoundVarUse;
                        }
                        VarProcFoundPos++;
                        goto SearchVarUseLoop;
FoundVarUse:            // Mark as found and exit
                        VarProcFound = 1;
                        VarProcFoundhwnd = hWnd;
                        return(0);
                    }
                    VarProcFoundPos = 1;
                }
            }
        }
        return(1);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Remove leading/trailing chr(2)
CStr RemoveChr2(CStr Txt)
{
    CStr ReturnValue;
    int i = 0;
    long LeadPos = 0;
    long TrailPos = 0;

    LeadPos = 1;
    TrailPos = Txt.Len() - 1;
    for(i = 1; i <= Txt.Len(); i++)
    {
        if(Txt.Get_String()[i-1] != 2) break;
        LeadPos++;
    }
    for(i = Txt.Len() - 1; i >= 1; i--)
    {
        if(Txt.Get_String()[i-1] != 2) break;
        TrailPos--;
    }
    if((TrailPos - LeadPos) > 0) ReturnValue = Txt.Mid(LeadPos, TrailPos);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Search for a procedure/variable keyword in a line
long ProcSearch(CStr Ln, CStr KeyWord, CStr BoundedKeyWord, long Position)
{
    long ReturnValue = 0;
    long PosKeyWord = 0;

    switch(Position)
    {
        case POS_BEFORE:
            PosKeyWord = Ln.In_Str(1, KeyWord.Get_String() + (CStr) BoundedKeyWord.Get_String(), Text_Compare);
            break;
        case POS_AFTER:
            PosKeyWord = Ln.In_Str(1, BoundedKeyWord.Get_String() + (CStr) KeyWord.Get_String(), Text_Compare);
            break;
    }
    if(PosKeyWord != 0) ReturnValue = 1;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check for a file type and open it (if possible)
// (This function is recursive if the passed argument is a directory)
long OpenUnknownFile(CStr FileName, int AddInRecent)
{
    CStr SelUnknownFiles;
    CStr NameToOpen;
    int SkipPrjLoadRecent;

    // Open passed file
    if(FileExist(FileName) == 1)
    {
        if(FileIsDirectory(FileName) == 0)
        {
            if(FileName.Len() != 0) {
                if(CheckProjectFile(FileName) == 0)
                {
                    if(CheckWorkSpaceFile(FileName) == 0)
                    {
                        if(AddInRecent)
                        {
                            if(OpenFileNorm(FileName, 0, 0, 0, FALSE) != 0) LastLoadDir = FileGetDirectory(FileName);
                        }
                        else
                        {
                            if(OpenFileNorm(FileName, 0, 0, 0, TRUE) != 0) LastLoadDir = FileGetDirectory(FileName);
                        }
                        return(TRUE);
                    }
                    else
                    {
                        OpenWorkSpc(FileName);
                        return(TRUE);
                    }
                }
                else
                {
                    SkipPrjLoadRecent = RealOpenPrj(FileName);
                    if(SkipPrjLoadRecent != -1) AddRecentPrj(FileName, SkipPrjLoadRecent, 0);
                    return(TRUE);
                }
            }
        }
        else
        {
            SelUnknownFiles = ComDlgGetOpenFileName(GetActiveWindow(),"All files (*.*)|*.*", FileName, 1, CurrentDir);
            if(ComDlgParseMultiFilesSelection(SelUnknownFiles, &EnumUnknownFiles, MULTIFILESENUM_FORWARD, AddInRecent).Len() != 0)
            {
                return(TRUE);
            }
        }
    }
    return(FALSE);
}

long CALLBACK EnumUnknownFiles(char *FileToAdd, long UserValue)
{
    OpenUnknownFile(FileToAdd, UserValue);
    return(TRUE);
}

// -----------------------------------------------------------------------
// Fill a combobox with ftp accounts
void FillFTPCombo(HWND hGlobComboBox)
{
    for(int i = 0; i <= 999; i++)
    {
        GlobalRetVal = IniReadKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), FtpAccountsIniFile);
        if(GlobalRetVal.Len() == 0) break;
        ComboBoxAddItem(hGlobComboBox, GlobalRetVal, -1);
    }
}

// -----------------------------------------------------------------------
// Conversions

// -----------------------------------------------------------------------
// Check the sanity of an ASCII number with a given base against a format descriptor
CStr Check_Number_Sanity(CStr Number, CStr Format,
                         int Base)
{
    int i;
    int Prefix_Len;
    int Suffix_Len;
    CStr BufString;
    CStr BufString_Up;
    CStr Header;
    CStr Local_Format;
    int j;

    /* Nothing was specified */
    if(Format.Len() == 0)
    {
        WriteToStatus("Number format not supplied for this language.");
        return "";
    }
    j = 0;
    for(;;)
    {
        while(Format[j] == ' ') j++;
        Local_Format = "";
        while(Format[j] != ' ' &&
              Format[j])
        {
            Local_Format += Format[j];
            j++;
        }

        Prefix_Len = Local_Format.In_Str(1, "%1");
        /* Format is fucked up */
        if(Prefix_Len == 0)
        {
            if(!Format[j])
            {
                WriteToStatus("Bad number format for this language.");
                return "";
            }
        }
        else
        {
            Header = StringReplace(Local_Format, "%1", "", 1, -1, Binary_Compare);
            Suffix_Len = Header.Len() - (Prefix_Len - 1);
            Suffix_Len = Number.Len() - Suffix_Len - (Prefix_Len - 1);

            BufString = Local_Format;
            int test = Number.Len() - Suffix_Len;
            BufString = StringReplace(BufString, "%1", Number.Mid(Prefix_Len, Suffix_Len), 1, -1, Binary_Compare);
            if(BufString.Str_Comp(BufString, Number, Text_Compare) == 0)
            {
                BufString_Up = Number.Mid(Prefix_Len, Suffix_Len).Upper_Case();
                for(i = 0; i < BufString_Up.Len(); i++)
                {
                    switch(Base)
                    {
                        case 2:
                            if(BufString_Up[i] < '0') if(!Format[j]) return "";
                            if(BufString_Up[i] > '1') if(!Format[j]) return "";
                            break;

                        case 10:
                            if(BufString_Up[i] < '0') if(!Format[j]) return "";
                            if(BufString_Up[i] > '9') if(!Format[j]) return "";
                            break;

                        case 16:
                            if(BufString_Up[i] < '0') if(!Format[j]) return "";
                            if(BufString_Up[i] > '9')
                            {
                                if(BufString_Up[i] < 'A') if(!Format[j]) return "";
                                if(BufString_Up[i] > 'F') if(!Format[j]) return "";
                            }
                            break;
                    }
                }
                return BufString_Up;
            }
            else
            {
                if(!Format[j]) return "";
            }
        }
    }
}

// -----------------------------------------------------------------------
// Convert a hexadecimal number into decimal
// In: current word (usually)
// Out: word converted ("" if none)
CStr HexToDec(HWND hWnd, CStr Number)
{
    int i = 0;
    long DestNumber = 0;
    CStr Char;
    CStr NumberToConvert;

    NumberToConvert = Check_Number_Sanity(Number.Trim(),
                                          GetCMLangHexType(GetParent(hWnd), -1),
                                          16);
    if(NumberToConvert.Len() == 0) return "";

    for(i = 1; i <= NumberToConvert.Len(); i++)
    {
        DestNumber <<= 4;
        Char = NumberToConvert.Mid(i, 1);
        DestNumber += TabConvHexCoher[(Char.Asc() - 0x30)];
    }
    return(DestNumber);
}

// -----------------------------------------------------------------------
// Convert a decimal number into hexadecimal
// In: current word (usually)
// Out: word converted ("" if none)
CStr DecToHex(HWND hWnd, CStr Number)
{
    int i = 0;
    long DestNumber = 0;
    CStr NumberToConvert;

    NumberToConvert = Check_Number_Sanity(Number.Trim(),
                                          "%1",
                                          10);
    if(NumberToConvert.Len() == 0) return "";

    DestNumber = AsciiToDw(NumberToConvert.Get_String());
    return(NumberToConvert.Hex_To_String(DestNumber));
}

// -----------------------------------------------------------------------
// Convert a decimal number into binary
// In: current word (usually)
// Out: word converted ("" if none)
CStr DecToBin(HWND hWnd, CStr Number)
{
    int i = 0;
    int mask;
    long DestNumber = 0;
    CStr Char;
    CStr NumberToConvert;

    NumberToConvert = Check_Number_Sanity(Number.Trim(),
                                          "%1",
                                          10);
    if(NumberToConvert.Len() == 0) return "";

    DestNumber = AsciiToDw(NumberToConvert.Get_String());
    mask = 1;
    NumberToConvert = "";
    for(i = 0; i <= 31; i++)
    {
        if((DestNumber & mask) != 0)
        {
            NumberToConvert = "1" + (CStr) NumberToConvert;
        }
        else
        {
            NumberToConvert = "0" + (CStr) NumberToConvert;
        }
        mask = mask << 1;
    }
    for(i = 1; i <= NumberToConvert.Len(); i++)
    {
        if(strcmp(NumberToConvert.Mid(i, 1).Get_String(), "0") != 0) break;
    }
    return(NumberToConvert.Mid(i));
}

// -----------------------------------------------------------------------
// Convert a hexadecimal number into binary
// In: current word (usually)
// Out: word converted ("" if none)
CStr HexToBin(HWND hWnd, CStr Number)
{
    CStr NumberToConvert;

    NumberToConvert = HexToDec(hWnd, Number);
    if(NumberToConvert.Len() == 0) return "";
    return(DecToBin(hWnd, NumberToConvert));
}

// -----------------------------------------------------------------------
// Convert a binary number into decimal
// In: current word (usually)
// Out: word converted ("" if none)
CStr BinToDec(HWND hWnd, CStr Number)
{
    int i = 0;
    unsigned int DestNumber = 0;
    CStr Char;
    CStr NumberToConvert;

    NumberToConvert = Check_Number_Sanity(Number.Trim(),
                                          GetCMLangBinType(GetParent(hWnd), -1),
                                          2);
    if(NumberToConvert.Len() == 0) return "";

    for(i = 1; i <= NumberToConvert.Len(); i++)
    {
        Char = NumberToConvert.Mid(i, 1).Upper_Case();
        OutputDebugString(Char.Get_String());
        DestNumber = (DestNumber << 1) | Char.Get_Long();
    }
    return(DestNumber);
}

// -----------------------------------------------------------------------
// Convert a binary number into hexadecimal
// In: current word (usually)
// Out: word converted ("" if none)
CStr BinToHex(HWND hWnd, CStr Number)
{
    int i = 0;
    unsigned int DestNumber = 0;
    CStr Char;
    CStr NumberToConvert;

    NumberToConvert = Check_Number_Sanity(Number.Trim(),
                                          GetCMLangBinType(GetParent(hWnd), -1),
                                          2);
    if(NumberToConvert.Len() == 0) return "";

    for(i = 1; i <= NumberToConvert.Len(); i++)
    {
        Char = NumberToConvert.Mid(i, 1).Upper_Case();
        DestNumber = (DestNumber << 1) | Char.Get_Long();
    }
    return(DecToHex(hWnd, DestNumber));
}

// -----------------------------------------------------------------------
// Convert a hexadecimal integer number into a floating point 
// In: current word (usually)
// Out: word converted ("" if none)
CStr HexToFlt(HWND hWnd, CStr Number)
{

    int i = 0;
    long DestNumber = 0;
    CStr Char;
    CStr NumberToConvert;
    float DestFlt;

    NumberToConvert = Check_Number_Sanity(Number.Trim(),
                                          GetCMLangHexType(GetParent(hWnd), -1),
                                          16);
    if(NumberToConvert.Len() == 0) return "";

    for(i = 1; i <= NumberToConvert.Len(); i++)
    {
        DestNumber <<= 4;
        Char = NumberToConvert.Mid(i, 1);
        DestNumber += TabConvHexCoher[(Char.Asc() - 0x30)];
    }
    _asm
    {
        fld dword ptr [DestNumber]
        fstp dword ptr [DestFlt]
    }
    NumberToConvert = DestFlt;
    if(NumberToConvert.In_Str(1, ".") == 0) NumberToConvert += ".0";
    return(NumberToConvert);
}

// -----------------------------------------------------------------------
// Convert a floating point number into hexadecimal
// In: current word (usually)
// Out: word converted ("" if none)
CStr FltToHex(HWND hWnd, CStr Number)
{
    CStr BufString;
    float fNbr;
    DWORD dwNbrOut;
    char *pEnd;

    fNbr = (float) strtod(Number.Get_String(), &pEnd);
    if(strlen(pEnd) > 1) return "";
    if(strlen(pEnd) == 1)
    {
        if(pEnd[0] != 'f' &&
           pEnd[0] != 'F')
        {
           return "";
        }
    }
    _asm
    {
        mov eax,[fNbr]
        mov [dwNbrOut],eax
    }
    BufString = BufString.Hex_To_String(dwNbrOut);
    return(BufString);
}

// -----------------------------------------------------------------------
// Construct the list of procedures contained in modules files
CStr GetModulesProc(void)
{
    CStr ReturnValue;
    HTREEITEM TreeViewChildEntryProc = 0;
    CStr TVEntryFName;
    CStr BufString;

    // Add user modules to header
    TreeViewChildEntryProc = TreeViewGetFirstItemChild(hTreeView, hTreeViewModules);
    while(TreeViewChildEntryProc != 0)
    {
        TVEntryFName = GetFileNameFromTreeView(TreeViewChildEntryProc);
        BufString = BufString + GetProcedureList(TVEntryFName);
        TreeViewChildEntryProc = TreeViewGetNextItem(hTreeView, TreeViewChildEntryProc);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return the position of a procedure entry point/end point in a text
CStr FindProcedure(CStr FileName, long hFile, CStr hFileCase,
                   long FileLen, long StartPos,
                   LPPOINT ProcPos, CStr CurLangProc, CStr CurLangEndProc)
{
    long *CurLangProcArray = 0;
    long *CurLangProcEndpArray = 0;
    BYTE *hFilePtr = 0;
    int i = 0;
    int j = 0;
    int EntryPointPos = 0;
    int EndPointPos = 0;
    int EntryPointPosReal = 0;
    int EndPointPosReal = 0;
    CStr LastLangProc;
    int k = 0;
    CStr FoundProcName;
    long InProcFound = 0;
    long ProcBalance = 0;
    long Locali = 0;
    CStr ReturnValue;

    ProcPos->x = -1;
    ProcPos->y = -1;

    // Not valid ?
    if(CurLangProc.Len() == 0) return(ReturnValue);
    if(CurLangEndProc.Len() == 0) return(ReturnValue);
    // Split'em
    CurLangProcArray = StringSplit(CurLangProc, " ");
    CurLangProcEndpArray = StringSplit(CurLangEndProc, " ");

    hFilePtr = (BYTE *) hFile;
    i = StartPos;
    if(i == 0) i = 1;
    // Loop till the end of the file
    while(i < FileLen)
    {
        if(InProcFound == 0)
        {
            // Search any specified entry point
            for(j = 0; j <= StringGetSplitUBound(CurLangProcArray); j++)
            {
                // xxx proc xxx
                EntryPointPos = Str_InString(i, (char *) hFile, StringGetSplitElement(CurLangProc, CurLangProcArray, j).Upper_Case().Get_String());
                if(EntryPointPos != 0)
                {
                    i = EntryPointPos;
                    LastLangProc = StringGetSplitElement(CurLangProc, CurLangProcArray, j);
                    if(CheckProcKeywordValidity((char *) hFile, EntryPointPos, LastLangProc) == 1) goto FoundNextProc;
                }
            }
        }
        else
        {
            // Search entry point
            EntryPointPosReal = 0;
            Locali = i;
SearchRealEntryPoint:
            for(j = 0; j <= StringGetSplitUBound(CurLangProcArray); j++)
            {
                // xxx proc xxx
                EndPointPos = Str_InString(Locali, (char *) hFile, StringGetSplitElement(CurLangProc, CurLangProcArray, j).Upper_Case().Get_String());
                if(EndPointPos != 0)
                {
                    if(CheckProcKeywordValidity((char *) hFile, EndPointPos, StringGetSplitElement(CurLangProc, CurLangProcArray, j)) == 1)
                    {
                        EntryPointPosReal = EndPointPos;
                        break;
                    }
                    else
                    {
                        Locali = EndPointPos + StringGetSplitElement(CurLangProc, CurLangProcArray, j).Len();
                        goto SearchRealEntryPoint;
                    }
                }
            }
            // Add keyword length if found
            // (Or start search at same point if nothing found previously)
            // Search end point (once we found a valid entry point for a function).
            EndPointPosReal = 0;
            Locali = i;
SearchRealEndPoint:
            for(j = 0; j <= StringGetSplitUBound(CurLangProcEndpArray); j++)
            {
                // xxx proc xxx
                k = Str_InString(Locali, (char *) hFile, StringGetSplitElement(CurLangEndProc, CurLangProcEndpArray, j).Upper_Case().Get_String());
                if(k != 0)
                {
                    if(CheckProcKeywordValidity((char *) hFile, k, StringGetSplitElement(CurLangProc, CurLangProcArray, j)) == 1)
                    {
                        EndPointPosReal = k;
                        break;
                    }
                    else
                    {
                        // Wrong keyword
                        Locali = k + StringGetSplitElement(CurLangEndProc, CurLangProcEndpArray, j).Len();
                        goto SearchRealEndPoint;
                    }
                }
            }
            // Error in file structure: report it and exit
            if(EntryPointPosReal == 0 && EndPointPosReal == 0)
            {
                WriteToStatus("Unexpected end of file ('" + (CStr) FileName + (CStr) "')");
                StringReleaseSplit(CurLangProcEndpArray);
                StringReleaseSplit(CurLangProcArray);
                return(ReturnValue);
            }
            // If the found entry point is lower than the end point
            // then make it the new reference.
            // (They can't be equal)
            if(EntryPointPosReal != 0)
            {   // Entry not found (possible end of file)
                if(EntryPointPosReal < EndPointPosReal)
                {
                    EndPointPosReal = EntryPointPosReal;
                    ProcBalance++;
                }
                else
                {
                    ProcBalance--;
                }
            }
            else
            {
                ProcBalance--;
            }
            k = EndPointPosReal;
            // Correct balance ? Switch mode
            if(ProcBalance == 0)
            {
                ProcPos->y = EndPointPosReal;           // Store endpoint
                // Release the splits
                StringReleaseSplit(CurLangProcEndpArray);
                StringReleaseSplit(CurLangProcArray);
                ReturnValue = FoundProcName;
                return(ReturnValue);
            }
        }
        // Nothing found in file or wrong type
        if(k != 0) i = k + 1;       // Add keyword length
        else i++;
        goto NoNextProc;
FoundNextProc:  // Valid ?
        //GetTxtLineFromFile(hPFileCase, PFileLen, (long) GetCurrentLine((char *) hPFile, PFileLen, i) - 1)
        FoundProcName = GetProcedureName(hFileCase, i, LastLangProc, ProcPos);
        if(FoundProcName.Len() != 0)
        {
            ProcBalance = 1;        // Reset counter
            InProcFound = 1;
        }
        i = i + StringGetSplitElement(CurLangProc, CurLangProcArray, j).Len();
NoNextProc:;
    }
    // Release the splits
    StringReleaseSplit(CurLangProcEndpArray);
    StringReleaseSplit(CurLangProcArray);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check the validity
long CheckProcKeywordValidity(char *FileToCheck, long PosInFile, CStr KeyWord)
{
    if(PosInFile == 1)
    {
        // proc xxx
        if(FileToCheck[PosInFile + KeyWord.Len() - 1] == 32) return(1);
    }
    else
    {
        // xxx proc xxx
        if(FileToCheck[PosInFile - 2] == 10) goto ForceProcFound;
        if(FileToCheck[PosInFile - 2] == 32)
        {
ForceProcFound:             
            if(FileToCheck[PosInFile + KeyWord.Len() - 1] == 32) return(1);
            if(FileToCheck[PosInFile + KeyWord.Len() - 1] == 13) return(1);
            // Handle Unix texts style
            if(FileToCheck[PosInFile + KeyWord.Len() - 1] == 10) return(1);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Retrieve the procedure name of a proc/endp first line
CStr GetProcedureName(CStr ProcBlock, long Pos, CStr ProcKeyWord, LPPOINT PointPos)
{
    CStr ProcChar;
    long ProcPos = 0;
    CStr ExprCharToTest;
    long BracketBalance = 0;
    CStr TempProcName;
    int i = 0;
    int BracketToCheck = 0;
    CStr ReturnValue;
    CStr BufString;

    ProcPos = Pos - 1; // Pos should be on first char of entry point keyword
    // proc xxx
    if(ProcPos == 1)
    {
    }
    else
    {
        if(ProcPos != 0)
        {
            BufString = ProcBlock.Left(ProcPos);
            // Trim possible brackets
            BracketBalance = 0;
            if(ProcPos == 1)
            {
                ReturnValue = BufString;
                PointPos->x = ProcPos;
                return(ReturnValue);
            }
            for(i = ProcPos; i >= 1; i--)
            {
                if(StringIsCloseBracketChar(BufString.Mid(i, 1)) == 0)
                {
                    if(!isspace(BufString.Mid(i, 1).Get_String()[0] & 0xff))
                    {
                        BracketToCheck++;
                        break;
                    }
                    ProcPos--;
                }
                else
                {
                    break;
                }
            }
            if(ProcPos == 1)
            {
                ReturnValue = BufString;
                PointPos->x = ProcPos;
                return(ReturnValue);
            }
            if(BracketToCheck == 0)
            {
                for(i = ProcPos; i >= 1; i--)
                {
                    ExprCharToTest = BufString.Mid(i, 1);
                    if(StringIsOpenBracketChar(ExprCharToTest)) BracketBalance--;
                    if(StringIsCloseBracketChar(ExprCharToTest)) BracketBalance++;
                    ProcPos--;
                    if(BracketBalance == 0) break;
                }
                if(ProcPos == 1)
                {
                    ReturnValue = BufString;
                    PointPos->x = ProcPos;
                    return(ReturnValue);
                }
            }
            for(i = ProcPos; i >= 1; i--)
            {
                if(StringIsLabelChar(BufString.Mid(i, 1)) == 0) ProcPos--;
                else break;
            }
            if(ProcPos == 1)
            {
                ReturnValue = BufString;
                PointPos->x = ProcPos;
                return(ReturnValue);
            }
            for(i = ProcPos; i >= 1; i--)
            {
                if(StringIsLabelChar(BufString.Mid(i, 1)))
                {
                    TempProcName = BufString.Mid(i, 1).Get_String() + (CStr) TempProcName;
                    ProcPos--;
                }
                else
                {
                    break;
                }
            }
            TempProcName = TempProcName.Trim();
            if(TempProcName.Len() != 0) BufString = TempProcName;
            // Update position
            PointPos->x = ProcPos;
        }
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Construct the list of procedures contained in a file
CStr GetProcedureList(CStr FileName)
{
    CStr ReturnValue;
    CStr CurLangProc;
    CStr CurLangEndProc;
    CStr BufString;
    CStr FoundProcName;
    CStr FlexFile;
    POINT PosInFile;
    long hPFile;
    char *hPFileCase;
    long BytesRead = 0;
    long PFileLen = 0;

    // Entry/End keywords
    CurLangProc = GetCMLangProcByFile(FileName);
    CurLangEndProc = GetCMLangProcCloseByFile(FileName);
    // Load the file to be checked
    hPFile = (long) MLoadFile(FileName.Get_String(), &BytesRead);
    if(hPFile != 0)
    {
        // Take the length of the file
        PFileLen = strlen((char *) hPFile);
        // Prepare it
        TabToSpace((char *) hPFile, PFileLen);
        RemoveCommentBlocksFromFileByFile(hPFile, PFileLen, FileName);
        RemoveSingleLineComment((char *) hPFile, PFileLen, GetCMLangCommentByFile(FileName).Get_String());
        RemoveQuotes((char *) hPFile, PFileLen);
        // Make a copy for case preservation
        hPFileCase = (char *) AllocMem(PFileLen);
        RtlCopyMemory(hPFileCase, (void *) hPFile, PFileLen);
        // Convert case now
        UpCase((char *) hPFile);
        FlexFile = hPFileCase;
        PosInFile.y = 1;
        for(;;)
        {
            FoundProcName = FindProcedure(FileName, hPFile, FlexFile, PFileLen, PosInFile.y, &PosInFile, CurLangProc, CurLangEndProc);
            if(FoundProcName.Len() == 0) break;
            BufString = BufString + FoundProcName + (CStr) BufString.Chr(2).Get_String();
        }
        FreeMem((long) hPFileCase);
        FreeMem(hPFile);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Open an help file
long OpenContextHelpFile(CStr HelpFileName, CStr HelpKeyWord)
{
    long ReturnValue = 0;
    CStr TmpHelpFile;
    CStr BufString;

    if(HelpFileName.Len() != 0)
    {
        TmpHelpFile = HelpFileName;
        TmpHelpFile = ChangeRelativePaths(TmpHelpFile);
        MSDNWord = HelpKeyWord;
        BufString = FileGetExtension(TmpHelpFile).Upper_Case();
        if(BufString == "CHM" || BufString == "COL")
        {
            ReturnValue = -1;
            DisplayChmVB(TmpHelpFile, MSDNWord);
        }
        else
        {
            ReturnValue = WinHelp(hMDIform.hWnd, TmpHelpFile.Get_String(), HELP_PARTIALKEY, (long) HelpKeyWord.Get_String());
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve a text line from a file 
CStr GetTxtLineFromFile(long hFile, long FileLength, long LineNumber)
{
    CStr ReturnValue;
    char *TxtLineAddr = 0;
    long TxtLineAddrLen = 0;
    long TxtLineLenAll = 0;
    CStr BufString;

    TxtLineAddr = GetFileLineAddress((char *) hFile, FileLength, LineNumber);
    TxtLineLenAll = strlen(TxtLineAddr);
    TxtLineAddrLen = MGetStrLenCR(TxtLineAddr, TxtLineLenAll);
    BufString = BufString.String(TxtLineAddrLen + 1, 1);
    RtlCopyMemory(BufString.Get_String(), (void *) TxtLineAddr, TxtLineAddrLen);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return TRUE if a given file is in the project
int IsFileInProject(char *FileName)
{
    HTREEITEM TreeViewChildEntry;
    
    if(ProjectOn == 1)
    {
        // Includes
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewIncludes);
        while(TreeViewChildEntry != 0)
        {
            if(_strcmpi(GetFileNameFromTreeView(TreeViewChildEntry).Get_String(), FileName) == 0) return(TRUE);
            TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        }
        // Modules
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewModules);
        while(TreeViewChildEntry != 0)
        {
            if(_strcmpi(GetFileNameFromTreeView(TreeViewChildEntry).Get_String(), FileName) == 0) return(TRUE);
            TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        }
        // Resources
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewResources);
        while(TreeViewChildEntry != 0)
        {
            // Avoid these folders
            if(TreeViewChildEntry == hTreeViewIcons) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewCursors) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewBitmaps) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewStrings) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewAccelerators) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewMenus) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewDialogs) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewRawdatas) goto AvoidTreeFolders;
            else
            {
                if(_strcmpi(GetFileNameFromTreeView(TreeViewChildEntry).Get_String(), FileName) == 0) return(TRUE);
            }
AvoidTreeFolders:
            TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        }
    }
    return(FALSE);
}

// -----------------------------------------------------------------------
// Construct the list of opened files or in project
void GetAllFiles(void)
{
    HTREEITEM TreeViewChildEntry = 0;
    
    FilesList.Erase();
    FilesListhWnd.Erase();
    if(ProjectOn == 1)
    {
        // Includes
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewIncludes);
        while(TreeViewChildEntry != 0)
        {
            FilesList.Add(GetFileNameFromTreeView(TreeViewChildEntry).Get_String());
            FilesListhWnd.Add((HWND) 0L);
            TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        }
        // Modules
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewModules);
        while(TreeViewChildEntry != 0)
        {
            FilesList.Add(GetFileNameFromTreeView(TreeViewChildEntry).Get_String());
            FilesListhWnd.Add((HWND) 0L);
            TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        }
        // Resources
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewResources);
        while(TreeViewChildEntry != 0)
        {
            // Avoid these folders
            if(TreeViewChildEntry == hTreeViewIcons) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewCursors) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewBitmaps) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewStrings) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewAccelerators) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewMenus) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewDialogs) goto AvoidTreeFolders;
            else if(TreeViewChildEntry == hTreeViewRawdatas) goto AvoidTreeFolders;
            else
            {
                FilesList.Add(GetFileNameFromTreeView(TreeViewChildEntry).Get_String());
                FilesListhWnd.Add((HWND) 0L);
            }
AvoidTreeFolders:
            TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        }
    }
    SearchAllFiles(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Modify fileslist array according to opened files
void SearchAllFiles(HWND hWnd)
{
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumFillFilesProc, 0);
}
                    
int CALLBACK EnumFillFilesProc(HWND hWnd, long lParam)
{
    int j = 0;
    long FoundF = 0;
    CStr ChildFile;

    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 0) goto NoSearchFileProc;
        if(GetWindowLong(hWnd, GWL_USERDATA) == 0) goto NoSearchFileProc;
        ChildStruct = LoadStructure(hWnd);
        ChildFile = ChildStruct->RFile;
        FoundF = 0;
        for(j = 0; j < FilesList.Amount(); j++)
        {
            if(strlen(FilesList.Get(j)->Content) != 0)
            {
                if(_strcmpi(ChildFile.Get_String(), FilesList.Get(j)->Content) == 0)
                {
                    // Mark it as opened (0 otherwise)
                    FilesListhWnd.Set(j, ChildStruct->hChildCodeMax);
                    FoundF = 1;
                    break;
                }
            }
        }
        // Add it if not found in array
        if(FoundF == 0)
        {
            FilesList.Add(ChildFile.Get_String());
            FilesListhWnd.Add(ChildStruct->hChildCodeMax);
        }
NoSearchFileProc:
        return(1);
    }
}

// -----------------------------------------------------------------------
// Search for first available index in a treeview childs
long SearchNextTreeViewEntry(HWND hTree, HTREEITEM hChild, CStr Prefix)
{
    int i = 0;
    long CurToAdd = 0;
    long EntryCount = 0;
    CStr BufString;
    
    EntryCount = TreeViewGetChildItemsCount(hTree, hChild);
    if(EntryCount != 0)
    {
        CurToAdd = -1;
        for(i = 0; i <= (long) EntryCount; i++)
        {
            CurToAdd++;
            BufString = (CStr) Prefix.Get_String() + (CStr) CurToAdd;
            if(TreeViewSearchChildPartialText(hTree, hChild, BufString) == -1) break;
        }
        EntryCount = CurToAdd;
    }
    return(EntryCount);
}

// -----------------------------------------------------------------------
// Retrieve "Use file Dir" state
long GetUseFileDir(void)
{
    CStr KeyValue;
    long ReturnValue = 0;

    if(NbForms != 0)
    {
        KeyValue = IniReadKey("Layout", "UseFileDir", MainIniFile);
        if(strlen(KeyValue.Get_String()) != 0)
        {
            if(_strcmpi(KeyValue.Get_String(), "1") == 0)
            {
                ChildStruct = LoadStructure(CurrentForm);
                if(ChildStruct->FileLoaded == 1) ReturnValue = 1;
            }
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Save Application state into a given workspace file
void SaveState(CStr WorkSpaceName)
{
    WINDOWPLACEMENT Mdi_Datas;

    if(IniReadBoolKey("Layout", "RememberState", MainIniFile))
    {
        // Delete the file first
        DeleteFile(WorkSpaceName.Get_String());
        IniWriteKey(AppTitle.Upper_Case().Get_String() + (CStr) "WORKSPC", "Version", AppVersion + (CStr) AppRevMaj + (CStr) AppRevMin, WorkSpaceName);
        WorkSpaceFileName = WorkSpaceName;
        SaveAllChildsInWorkSpace(hMDIform.hClient);
        // Include project file into the list
        if(ProjectOn == 1) IniWriteKey("Files", "File" + (CStr) StringNumberComplement(WorkSpaceNumber, 3).Get_String(), ProjectFName, WorkSpaceName);
    }
    Mdi_Datas.length = sizeof(WINDOWPLACEMENT);
    GetWindowPlacement(hMDIform.hWnd, &Mdi_Datas);
    IniWriteKey("Layout", "MDI_Flags", Mdi_Datas.flags, MainIniFile);
    IniWriteKey("Layout", "MDI_MinPosX", Mdi_Datas.ptMinPosition.x, MainIniFile);
    IniWriteKey("Layout", "MDI_MinPosY", Mdi_Datas.ptMinPosition.y, MainIniFile);
    IniWriteKey("Layout", "MDI_MaxPosX", Mdi_Datas.ptMaxPosition.x, MainIniFile);
    IniWriteKey("Layout", "MDI_MaxPosY", Mdi_Datas.ptMaxPosition.y, MainIniFile);
    IniWriteKey("Layout", "MDI_PosL", Mdi_Datas.rcNormalPosition.left, MainIniFile);
    IniWriteKey("Layout", "MDI_PosT", Mdi_Datas.rcNormalPosition.top, MainIniFile);
    IniWriteKey("Layout", "MDI_PosR", Mdi_Datas.rcNormalPosition.right, MainIniFile);
    IniWriteKey("Layout", "MDI_PosB", Mdi_Datas.rcNormalPosition.bottom, MainIniFile);
    IniWriteKey("Layout", "MDI_Show", Mdi_Datas.showCmd, MainIniFile);
}

// -----------------------------------------------------------------------
// Restore Application state into a given workspace file
void RestoreState(CStr WorkSpaceName)
{
    CStr ResState;

    ResState = IniReadKey("Layout", "RememberState", MainIniFile);
    if(ResState.Len() != 0)
    {
        if(_strcmpi(ResState.Get_String(), "1") == 0)
        {
            OpenWorkSpc(WorkSpaceName);
            return;
        }
    }
    ResState = IniReadKey("Layout", "ShowProjects", MainIniFile);
    if(ResState.Len() != 0)
    {
        if(_strcmpi(ResState.Get_String(), "1") == 0)
        {
            MCMD_CreateProject();
        }
    }
}

// -----------------------------------------------------------------------
// Add an icon into systray
void ControlSetSysToTray(HINSTANCE hInstance, HWND hWnd, long IDTray, long IconID, CStr ToolTipText)
{
    MEMORYSTATUS MemStat;
    CStr BalloonText;

    if(Windows2K == 1)
    {
        SysTrayStruct.cbSize = sizeof(SysTrayStruct);
        SysTrayStruct.hWnd = hWnd;
        SysTrayStruct.uID = IDTray;
        SysTrayStruct.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP | NIF_INFO;
        SysTrayStruct.uTimeout = 10;
        strncpy(SysTrayStruct.szInfoTitle, ToolTipText.Get_String(), 64);
        MemStat.dwLength = sizeof(MemStat);
        GlobalMemoryStatus(&MemStat);
        BalloonText = "Double click to bring to front\r\nRight click for options\r\n\r\n";
        BalloonText = BalloonText + (CStr) "Memory usage: " + (CStr) MemStat.dwMemoryLoad + (CStr) "%\r";
        BalloonText = BalloonText + (CStr) "Free physical memory: " + (CStr) MemStat.dwAvailPhys + (CStr) "\r";
		strncpy(SysTrayStruct.szInfo, BalloonText.Get_String(), 256);
        SysTrayStruct.dwInfoFlags = NIIF_INFO;
        SysTrayStruct.dwState = NIS_SHAREDICON;
        SysTrayStruct.dwStateMask = NIS_SHAREDICON;
        SysTrayStruct.uCallbackMessage = WM_SHELLNOTIFY;
        SysTrayStruct.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IconID));
		strncpy(SysTrayStruct.szTip, ToolTipText.Get_String(), 128);
        ShowWindow(hWnd, SW_HIDE);
        Shell_NotifyIcon(NIM_ADD, &SysTrayStruct);
    }
    else
    {
        // Use it as old structure
        SysTrayStruct.cbSize = 88; //sizeof(SysTrayStruct);
        SysTrayStruct.hWnd = hWnd;
        SysTrayStruct.uID = IDTray;
        SysTrayStruct.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
        SysTrayStruct.uCallbackMessage = WM_SHELLNOTIFY;
        SysTrayStruct.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IconID));
		strncpy(SysTrayStruct.szTip, ToolTipText.Get_String(), 64);
        ShowWindow(hWnd, SW_HIDE);
        Shell_NotifyIcon(NIM_ADD, &SysTrayStruct);
    }
    SysTrayMode = 1;
}

// -----------------------------------------------------------------------
// Remove a block of comments
void RemoveCommentBlocksFromFileByFile(long hFile, long FileLen, CStr FileName)
{
    if(_strcmpi(GetCMLangLangNameByFile(FileName).Get_String(), "x86 assembler") == 0)
    {
        RemoveCommentBlocks((char *) hFile, FileLen);
    }
    else
    {
        RemoveCommentBlocksEx((char *) hFile,
                              FileLen,
                              GetCMLangMultiCommentByFile(FileName).Trim().Get_String(),
                              GetCMLangMultiCommentEndByFile(FileName).Trim().Get_String());
    }
}

// -----------------------------------------------------------------------
// Remove a block of comments
void RemoveCommentBlocksFromFile(long hFile, long FileLen, HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    if(_strcmpi(GetCMLangNameByChild(ChildStruct->hChildCodeMax).Get_String(), "x86 assembler") == 0)
    {
        RemoveCommentBlocks((char *) hFile, FileLen);
    }
    else
    {
        RemoveCommentBlocksEx((char *) hFile,
                              FileLen,
                              GetCMLangMultiComment(hWnd).Trim().Get_String(),
                              GetCMLangMultiCommentEnd(hWnd).Trim().Get_String());
    }
}

// -----------------------------------------------------------------------
// Remove a block of comments (by child)
void RemoveCommentBlocksFromFileByChild(long hFile, long FileLen, HWND hWnd)
{
    if(_strcmpi(GetCMLangNameByChild(hWnd).Get_String(), "x86 assembler") == 0)
    {
        RemoveCommentBlocks((char *) hFile, FileLen);
    }
    else
    {
        RemoveCommentBlocksEx((char *) hFile,
                              FileLen,
                              GetCMLangMultiCommentByChild(hWnd).Trim().Get_String(),
                              GetCMLangMultiCommentEndByChild(hWnd).Trim().Get_String());
    }
}

// -----------------------------------------------------------------------
// Remove a block of comments (by child)
void RemoveCommentBlocksFromFileByChildStr(CStr FileString, long FileLen, HWND hWnd)
{
    if(_strcmpi(GetCMLangNameByChild(hWnd).Get_String(), "x86 assembler") == 0)
    {
        RemoveCommentBlocks(FileString.Get_String(), FileLen);
    }
    else
    {
        RemoveCommentBlocksEx(FileString.Get_String(),
                              FileLen,
                              GetCMLangMultiCommentByChild(hWnd).Trim().Get_String(),
                              GetCMLangMultiCommentEndByChild(hWnd).Trim().Get_String());
    }
}

// -----------------------------------------------------------------------
// Add a date prefix in status box lines
CStr PutStatusDatePrefix(void)
{
    CStr ReturnValue;
    CStr BufString;
    
    if(OutputDates == 1) BufString = "[" + (CStr) DateGetNow(DATE_NO_SECONDS).Get_String() + (CStr) "] ";
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Run an external program and add it in recent programs list if necessary
void RunExtProg(CStr ProgToRun)
{
    if(ProgToRun.Len() != 0)
    {
        Exec(ProgToRun, ProgToRun, 0);
        LastExeDir = FileGetDirectory(ProgToRun);
        if(FileExist(ProgToRun) == 0)
        {
            AddRecentRunProg(ProgToRun, 0, 0);
        }
        else
        {
            AddRecentRunProg(ProgToRun, 1, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Handle filenames with spaces
CStr CorrectFileName(CStr FileName)
{
    CStr ReturnValue;
    CStr BufString;

    BufString = FileName;
    if(BufString.In_Str(1, " ") != 0) BufString = "\"" + (CStr) BufString + (CStr) "\"";
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Jump to an arbitrary address (to be removed)
long JumpToAddr(FARPROC Address)
{
    long ReturnValue = 0;
    _asm
    {
        mov     eax,Address
        test    eax,eax
        je      NoJumpToAddr
        call    eax
        mov     ReturnValue,eax
NoJumpToAddr:
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Jump to an arbitrary address (to be removed)
long JumpToAddrArg(FARPROC Address, long Arg)
{
    long ReturnValue = 0;
    _asm
    {
        mov     eax,Address
        test    eax,eax
        jz      NoJumpToAddrArg
        push    Arg
        call    eax
        mov     ReturnValue,eax
NoJumpToAddrArg:
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the real height of the MDI
int GetMDIWidth(void)
{
//    return(ControlWidth(hMDIform.hWnd));
    return(GetSystemMetrics(SM_CXMAXIMIZED));
}

// -----------------------------------------------------------------------
// Retrieve the real height of the MDI
int GetMDIHeight(void)
{
    return(GetSystemMetrics(SM_CYMAXIMIZED));
}

// -----------------------------------------------------------------------
// Display the mouse pointer
void DisplayCursor(void)
{
    ShowCursor(TRUE);
    CursorDisabled = 0;
}

// -----------------------------------------------------------------------
// Hide the mouse pointer
void HideCursor(void)
{
    ShowCursor(FALSE);
    CursorDisabled = 1;
}

// -----------------------------------------------------------------------
// Turn a string into upper case
extern "C"
void CALLBACK UpCase(char *String)
{
    for(int i = 0; String[i] != '\0'; i++)
    {
        String[i] = toupper(String[i]);
    }
}

// -----------------------------------------------------------------------
// Turn a string into lower case
extern "C"
void CALLBACK LoCase(char *String)
{
    for(int i = 0; String[i] != '\0'; i++)
    {
        String[i] = tolower(String[i]);
    }
}

// -----------------------------------------------------------------------
// Fill a memory block
extern "C"
void CALLBACK FillMemDat(BYTE *Mem, int Length, int Data)
{
    memset(Mem, Data, Length);
}

// -----------------------------------------------------------------------
// Count the number of lines in a file
extern "C"
int CALLBACK CountFileLines(char *String, int MaxSize)
{
    int Lines = 0;

    if(String != NULL)
    {
        for(int i = 0; i < MaxSize; i++)
        {
            if(String[i] == '\0') break;
            if(String[i] == '\r') continue;
            if(String[i] == '\n') Lines++;
        }
        Lines++;
    }
    return(Lines);
}

// -----------------------------------------------------------------------
// Retrieve the address of a given line
extern "C"
char * CALLBACK GetFileLineAddress(char *String, int MaxSize, int LineNumber)
{
    char Char;

    if(String != NULL)
    {
        if(LineNumber)
        {
            for(int i = 0; i < MaxSize; i++)
            {
                Char = *String++;
                if(Char == '\0') break;
                if(Char == '\r') continue;
                if(Char == '\n')
                {
                    LineNumber--;
                    if(LineNumber == 0) break;
                }                
            }
        }
    }
    return(String);
}

// -----------------------------------------------------------------------
// Retrieve the offset of a given line
extern "C" 
int CALLBACK GetFileLineOffset(char *String, int MaxSize, int LineNumber)
{
    if(String == NULL) return(0);
    return((int) GetFileLineAddress(String, MaxSize, LineNumber) - (int) String);
}

// -----------------------------------------------------------------------
// Convert space chars to newline
void CALLBACK SpaceToNewLine(char *String, int MaxSize)
{
    if(String != NULL)
    {
        for(int i = 0; i < MaxSize; i++)
        {
            if(String[i] == '\0') break;
            if(String[i] == ' ') String[i] = '\n';
        }
    }
}

// -----------------------------------------------------------------------
// Convert tabs chars to space
extern "C"
void CALLBACK ReplaceTabs(char *String, int MaxSize)
{
    if(String != NULL)
    {
        for(int i = 0; i < MaxSize; i++)
        {
            if(String[i] == '\0') break;
            if(String[i] == '\t') String[i] = ' ';
        }
    }
}

// -----------------------------------------------------------------------
// Convert space chars to newline
extern "C"
int CALLBACK RemCRLF(char *String, int MaxSize)
{
    int Counter = 0;
    int Len = 0;

    if(String != NULL)
    {
        for(int i = 0; i < MaxSize; i++)
        {
            if(String[i] == '\0') break;
            if(String[i] == '\r') continue;
            Counter++;
            if(String[i] == '\n')
            {
                Counter--;
                if(Counter > Len)
                {
                    Len = Counter;
                }
                Counter = 0;
                String[i - 1] = '\0';
                if(String[i - 2] == '\r')
                {
                    String[i - 2] = '\0';
                }
            }
        }
    }
    return(Len);
}

// -----------------------------------------------------------------------
// Memory copy with S/D offsets
void CALLBACK CopyMem2(char *Source,
                       char *Dest,
                       int Len,
                       int Src_Offset,
                       int Dest_Offset)
{
    memcpy((void *) (Dest + Dest_Offset),
           (void *) (Source + Src_Offset), 
           Len);
}

// -----------------------------------------------------------------------
// Memory copy with S/D offsets
void CALLBACK CopyMem(char *Source,
                      char *Dest,
                      int Len,
                      int Dest_Offset,
                      int RepeatCount)
{
    for(int i = 0; i < RepeatCount; i++)
    {
        CopyMem2(Source, Dest, Len, 0, Dest_Offset);
        Dest_Offset += Len;
    }
}

// -----------------------------------------------------------------------
// Replace space chars in a text by tabs
void CALLBACK SpaceToTab(char *String, int MaxSize)
{
    if(String != NULL)
    {
        for(int i = 0; i < MaxSize; i++)
        {
            if(String[i] == '\0') break;
            if(String[i] == ' ') String[i] = '\t';
        }
    }
}

// -----------------------------------------------------------------------
// Replace tab chars in a text by spaces
void TabToSpace(char *String, int MaxSize)
{
    if(String != NULL)
    {
        for(int i = 0; i < MaxSize; i++)
        {
            if(String[i] == '\0') break;
            if(String[i] == '\t') String[i] = ' ';
        }
    }
}

// -----------------------------------------------------------------------
// Toggle string chars case
void CALLBACK ToggleCase(char *String, int Len)
{
    if(String != NULL)
    {
        for(int i = 0; i < Len; i++)
        {
            if(isupper(String[i])) String[i] = tolower(String[i]);
            else if(islower(String[i])) String[i] = toupper(String[i]);
        }
    }
}

// -----------------------------------------------------------------------
// Convert decimal string into dword value
int CALLBACK AsciiToDw(char *String)
{
    return(atol(String));
}

// -----------------------------------------------------------------------
// Display a .chm file
HWND CALLBACK DisplayChm(char *FileName, char *KeyWord)
{
    HH_AKLINK MyhLink;
    
    memset((void *) &MyhLink, 0, sizeof(HH_AKLINK));
    MyhLink.cbStruct = sizeof(HH_AKLINK);
    MyhLink.pszKeywords = KeyWord;
    MyhLink.fIndexOnFail = TRUE;
    return(HtmlHelp(0, FileName, HH_KEYWORD_LOOKUP, (DWORD) &MyhLink));
}

// -----------------------------------------------------------------------
// Run a Quick Editor plugin
void CALLBACK  QEPlug(QEPROC PlugAddr,
                      HINSTANCE hInstance,
                      HWND hWnd,
                      HWND hText,
                      HWND hToolBar,
                      HWND hStatusBar)
{
    if(PlugAddr)
    {
        PlugAddr(hInstance, hWnd, hText, hToolBar, hStatusBar);
    }
}

// -----------------------------------------------------------------------
// Open a file and load it into memory
// (Return memory block or 0)
BYTE * CALLBACK MLoadFile(char *FileName, long *Bytes)
{
    int Size;
	FILE *FHandle;
	BYTE *MemBlock = NULL;
    *Bytes = 0;

    FHandle = fopen(FileName, "rb");
    if(FHandle)
    {
        fseek(FHandle, 0, SEEK_END);
        Size = ftell(FHandle);
        fseek(FHandle, 0, SEEK_SET);
        MemBlock = (BYTE *) calloc(Size + 4, 1);
        if(MemBlock)
        {
			 *Bytes = fread( MemBlock, sizeof( char ), Size, FHandle );
        }
        fclose(FHandle);
    }
    return(MemBlock);
}
/*
// -----------------------------------------------------------------------
// Open a file and load it into memory
// (Return memory block or 0)
BYTE *MLoadFile(char *FileName, long *Bytes)
{
    int Size;
    BYTE *MemBlock = NULL;
    *Bytes = 0;
    FILE *FHandle = fopen(FileName, "rb");
    if(FHandle)
    {
        fseek(FHandle, 0, SEEK_END);
        Size = ftell(FHandle);
        fseek(FHandle, 0, SEEK_SET);
        MemBlock = (BYTE *) calloc(Size + 4, 1);
        if(MemBlock)
        {
            *Bytes = fread(MemBlock, sizeof(char), Size, FHandle);
        }
        fclose(FHandle);
    }
    return(MemBlock);
}
*/