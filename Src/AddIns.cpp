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
// AddIns.cpp: AddIns core handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Classes/WALib_WinSock.h"
#include "AddIns.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "Menus.h"
#include "MDI_Childs.h"
#include "MDI_Form.h"
#include "Languages.h"
#include "CmdMenus.h"
#include "ProjectsBuilder.h"
#include "ConstCodeMax.h"
#include "ConstRes.h"
#include "Main.h"
#include "Toolbars.h"
#include "Splitters.h"
#include "Statusbar.h"
#include "ImageLists.h"
#include "Dlg_Args.h"
#include "Dlg_Addins.h"
#include "Dlg_EnterValue.h"

// -----------------------------------------------------------------------
// Variables
CList <HMODULE> AddInsDLL;
CList <HMODULE> OldAddInsDLL;
CList <char *> AddInsFiles;
CList <int> LoadedAddIns;
CList <int> RunningAddIns;
CList <int> OldRunningAddIns;
CList <char *> OldAddInsFiles;
CList <char *> AddInsDescriptions;
CList <FARPROC> AddInsLoad;
CList <FARPROC> AddInsUnLoad;
CList <FARPROC> AddInsMenu;
CHROMATICLIB AddInLib;

CList <long> MDICreateHooks;
CList <DLGPROC> MDIHooks;
CList <DLGPROC> MDIClientHooks;
CList <long> ChildCreateHooks;
CList <DLGPROC> ChildHooks;
CList <DLGPROC> CodeMaxHooks;
CList <int> MDICreateHooksNbr;
CList <int> MDIHooksNbr;
CList <int> MDIClientHooksNbr;
CList <int> ChildCreateHooksNbr;
CList <int> ChildHooksNbr;
CList <int> CodeMaxHooksNbr;

//CList String_Garbage;

long HookNbr;

void (CALLBACK *CurCustChildInit)(HWND hwnd);
WNDPROC CurCustChildProc;
CStr CurCustChildTitle;

// -----------------------------------------------------------------------
// Functions declarations
void CALLBACK CustChildWinInitProc(HWND hWnd);
LRESULT CALLBACK CustChildWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------
// Fill the AddIns arrays with dll names and attributes
void FillAddInsArrays(void)
{
    int i = 0;
    long NbrAddin = 0;
    CStr AddInFileName;
    CStr AddInLoaded;
    CStr AddInRunning;
    HMODULE AddInLibMod = 0;
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
    CStr Auth;
    CStr Description;
    CStr BufString;

    AddInsFiles.Erase();
    AddInsDescriptions.Erase();
    LoadedAddIns.Erase();
    AddInsLoad.Erase();
    AddInsUnLoad.Erase();
    AddInsMenu.Erase();
    RunningAddIns.Erase();
    AddInsDLL.Erase();
    
    NbrAddin = 0;
    for(i = 0; i <= 999; i++)
    {
        AddInFileName = IniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInLoaded = IniReadKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInRunning = IniReadKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInFileName.Len() == 0) break;
        BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) AddInFileName;
        AddInLibMod = LoadLibrary(BufString.Get_String());
        if(AddInLibMod != 0)
        {
            AddInDesc = GetProcAddress(AddInLibMod, "AddInDescription");
            AddInAuth = GetProcAddress(AddInLibMod, "AddInAuthor");
            AddInLoad = GetProcAddress(AddInLibMod, "AddInLoad");
            AddInUnload = GetProcAddress(AddInLibMod, "AddInUnLoad");
            AddInMenu = GetProcAddress(AddInLibMod, "AddInMenu");
            // We need all procedures to run it correctly
            if(AddInAuth != 0 && AddInDesc != 0 && AddInLoad != 0 && AddInUnload != 0 && AddInMenu != 0)
            {
                // Register dll name
                AddInsDLL.Add(AddInLibMod);
                AddInsFiles.Add(AddInFileName.Get_String());
                AddInsDescriptions.Add(GetDLLDescription(AddInFileName, AddInDesc).Get_String());
                // Register loaded state
                if(_strcmpi(AddInLoaded.Get_String(), "1") == 0) LoadedAddIns.Add(1L);
                else LoadedAddIns.Add(0L);
                if(_strcmpi(AddInRunning.Get_String(), "1") == 0) RunningAddIns.Add(1L);
                else RunningAddIns.Add(0L);
                AddInsLoad.Add(AddInLoad);
                AddInsUnLoad.Add(AddInUnload);
                AddInsMenu.Add(AddInMenu);
                IniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                IniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                IniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                IniWriteKey("AddIns", "File" + (CStr) StringNumberComplement(NbrAddin, 3).Get_String(), AddInFileName, MainIniFile);
                IniWriteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(NbrAddin, 3).Get_String(), AddInLoaded, MainIniFile);
                IniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(NbrAddin, 3).Get_String(), AddInRunning, MainIniFile);
                NbrAddin++;
            }
            else
            {
                IniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                IniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                IniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            }
        }
        else
        {
            IniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            IniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            IniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
    }
}

// -----------------------------------------------------------------------
// Refresh the AddIns arrays with dll names and attributes
void ReFillAddInsArrays(void)
{
    int i = 0;
    int k = 0;
    long Revk = 0;
    long DllNewHandle = 0;
    CStr AddInFileName;
    CStr AddInLoaded;
    CStr AddInRunning;
    HMODULE AddInLibMod = 0;
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
    CStr Auth;
    CStr Description;
    long NbrAddins = 0;
    long HandlePos = 0;
    CStr BufString;

    AddInsFiles.Erase();
    AddInsDescriptions.Erase();
    LoadedAddIns.Erase();
    AddInsLoad.Erase();
    AddInsUnLoad.Erase();
    AddInsMenu.Erase();
    RunningAddIns.Erase();
    
    for(i = 0; i<= 999; i++)
    {
        AddInFileName = IniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInLoaded = IniReadKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInRunning = IniReadKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInFileName.Len() == 0) break;
        // Search if an addin had been added in the directory
        // before displaying the manager.
        DllNewHandle = i;
        HandlePos = SearchOldAddInName(AddInFileName);
        if(HandlePos == -1)
        {
            // Move entries down
            Revk = AddInsDLL.Amount();
            // Handle to be filled with library handle
            // We need to create a new empty entry in the handles table
            AddInsDLL.Add((HMODULE) 0L);
            // Now we can move down the entries (if required)
            // (From top to bottom)
            for(k = DllNewHandle; k < (AddInsDLL.Amount() - 1); k++)
            {
                AddInsDLL.Set(Revk, AddInsDLL.Get(Revk - 1)->Content);
                Revk--;
            }
            // Clear the cache entry (to force load of the dll)
            AddInsDLL.Set(DllNewHandle, 0);
        }
        else
        {
            // Restore old handle
            AddInsDLL.Set(DllNewHandle, OldAddInsDLL.Get(HandlePos)->Content);
        }
        AddInLibMod = AddInsDLL.Get(DllNewHandle)->Content;
        if(AddInLibMod == 0)
        {
            // Reload it
            BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) AddInFileName;
            AddInLibMod = LoadLibrary(BufString.Get_String());
            AddInsDLL.Set(DllNewHandle, AddInLibMod);
        }
        AddInDesc = GetProcAddress(AddInLibMod, "AddInDescription");
        AddInAuth = GetProcAddress(AddInLibMod, "AddInAuthor");
        AddInLoad = GetProcAddress(AddInLibMod, "AddInLoad");
        AddInUnload = GetProcAddress(AddInLibMod, "AddInUnLoad");
        AddInMenu = GetProcAddress(AddInLibMod, "AddInMenu");
        // We need all procedures to run it correctly
        if(AddInAuth != 0 && AddInDesc != 0 && AddInLoad != 0 && AddInUnload != 0 && AddInMenu != 0)
        {
            // Register dll name
            AddInsFiles.Add(AddInFileName.Get_String());
            AddInsDescriptions.Add(GetDLLDescription(AddInFileName, AddInDesc).Get_String());
            // Register loaded state
            if(_strcmpi(AddInLoaded.Get_String(), "1") == 0) LoadedAddIns.Add(1L);
            else LoadedAddIns.Add(0L);
            if(_strcmpi(AddInRunning.Get_String(), "1") == 0) RunningAddIns.Add(1L);
            else RunningAddIns.Add(0L);
            AddInsLoad.Add(AddInLoad);
            AddInsUnLoad.Add(AddInUnload);
            AddInsMenu.Add(AddInMenu);
            IniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            IniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            IniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            IniWriteKey("AddIns", "File" + (CStr) StringNumberComplement(NbrAddins, 3).Get_String(), AddInFileName, MainIniFile);
            IniWriteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(NbrAddins, 3).Get_String(), AddInLoaded, MainIniFile);
            IniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(NbrAddins, 3).Get_String(), AddInRunning, MainIniFile);
            NbrAddins++;
        }
        else
        {
            IniDeleteKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            IniDeleteKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            IniDeleteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
    }
    LoadedAddIns.Erase();
    RunningAddIns.Erase();
    for(i = 0; i<= 999; i++)
    {
        AddInFileName = IniReadKey("AddIns", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInLoaded = IniReadKey("AddIns", "Loaded" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        AddInRunning = IniReadKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(AddInFileName.Len() == 0) break;
        if(_strcmpi(AddInLoaded.Get_String(), "1") == 0) LoadedAddIns.Add(1L);
        else LoadedAddIns.Add(0L);
        if(_strcmpi(AddInRunning.Get_String(), "1") == 0) RunningAddIns.Add(1L);
        else RunningAddIns.Add(0L);
    }
}

// -----------------------------------------------------------------------
// Start AddIns at startup if necessary
void SetAddInsRunningState(void)
{
    int i = 0;

    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        // Must run now ?
        if(RunningAddIns.Get(i)->Content == 1)
        {
            if(AddInsLoad.Get(i)->Content != 0)
            {
                if(JumpToAddrArg(AddInsLoad.Get(i)->Content, (long) &AddInLib) == ADDIN_FINISHED)
                {
                    RunningAddIns.Set(i, 0);
                    // Unload it now
                    if(AddInsUnLoad.Get(i)->Content != 0)
                    {
                        JumpToAddr(AddInsUnLoad.Get(i)->Content);
                    }
                    // Stop it for good
                    IniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", MainIniFile);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Stop all the running AddIns
void StopAddIns(void)
{
    int i = 0;

    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        // Must run now ?
        if(RunningAddIns.Get(i)->Content == 1)
        {
            if(AddInsUnLoad.Get(i)->Content != 0)
            {
                JumpToAddr(AddInsUnLoad.Get(i)->Content);
            }
        }
        if(AddInsDLL.Get(i)->Content != 0)
        {
            FreeLibrary(AddInsDLL.Get(i)->Content);
            AddInsDLL.Set(i, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Shutdown an AddIn (Return old running state)
long ShutDownAddIn(long AddInNumber)
{
    long ReturnValue = 0;
    
    // Store old state
    ReturnValue = RunningAddIns.Get(AddInNumber)->Content;
    if(RunningAddIns.Get(AddInNumber)->Content == 1)
    {
        if(AddInsUnLoad.Get(AddInNumber)->Content != 0)
        {
            JumpToAddr(AddInsUnLoad.Get(AddInNumber)->Content);
        }
    }
    if(AddInsDLL.Get(AddInNumber)->Content != 0)
    {
        FreeLibrary(AddInsDLL.Get(AddInNumber)->Content);
        // Dlg_Filters.cpp for more infos
        #ifndef _DEBUG
            CloseHandle((HANDLE) AddInsDLL.Get(AddInNumber)->Content);
        #endif
        AddInsDLL.Set(AddInNumber, 0);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Restart an AddIn (Must use ShutDownAddIn before)
long RestartAddIn(long AddInNumber)
{
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
    long ReturnValue = 0;
    CStr BufString;

    BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) AddInsFiles.Get(AddInNumber)->Content;
    AddInsDLL.Set(AddInNumber, LoadLibrary(BufString.Get_String()));
    if(AddInsDLL.Get(AddInNumber)->Content != 0)
    {
        AddInDesc = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInDescription");
        AddInAuth = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInAuthor");
        AddInLoad = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInLoad");
        AddInUnload = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInUnLoad");
        AddInMenu = GetProcAddress(AddInsDLL.Get(AddInNumber)->Content, "AddInMenu");
        
        AddInsDescriptions.Set(AddInNumber, GetDLLDescription(AddInsFiles.Get(AddInNumber)->Content, AddInDesc).Get_String());
        AddInsLoad.Set(AddInNumber, AddInLoad);
        AddInsUnLoad.Set(AddInNumber, AddInUnload);
        AddInsMenu.Set(AddInNumber, AddInMenu);
        if(RunningAddIns.Get(AddInNumber)->Content == 1) RunAddIn(AddInNumber);
        ReturnValue = 1;
        IniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(AddInNumber, 3).Get_String(), RunningAddIns.Get(AddInNumber)->Content, MainIniFile);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Fill the AddIns menu with dll names and attributes
void FillAddInsMenu(void)
{
    long AddInSep = 0;
    int i = 0;

    // Remove them all
    while(DeleteMenu(hEditAddInsMenu, 1, MF_BYPOSITION) != 0)
    {
    }
    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        // Is it loaded by user ?
        if(LoadedAddIns.Get(i)->Content != 0)
        {
            if(AddInSep == 0)
            {
                MenuAddSeparator(hEditAddInsMenu);
                AddInSep = 1;
            }
            AppendMenu(hEditAddInsMenu, MF_STRING, MENU_ADDINS_IDBASE + MENU_ADDINS_FIRSTADDIN_ID + i, AddInsDescriptions.Get(i)->Content);
        }
    }
}

// -----------------------------------------------------------------------
// Retrieve the index of an AddIn name or -1
long SearchAddInName(CStr AddInName)
{
    long ReturnValue = -1;
    int i = 0;

    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        if(_strcmpi(AddInsFiles.Get(i)->Content, AddInName.Get_String()) == 0)
        {
            ReturnValue = i;
            break;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the index of an old AddIn name or -1
long SearchOldAddInName(CStr AddInName)
{
    long ReturnValue = -1;
    int i = 0;

    for(i = 0; i < OldAddInsFiles.Amount(); i++)
    {
        if(_strcmpi(OldAddInsFiles.Get(i)->Content, AddInName.Get_String()) == 0)
        {
            ReturnValue = i;
            break;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve description of an DLL (AddIn of Filter)
CStr GetDLLDescription(CStr AddInName, FARPROC DescriptionRoutine)
{
    long DescriptionLng = 0;
    long DescriptionLen = 0;
    CStr ReturnValue;
    CStr BufString;

    if(DescriptionRoutine == 0) goto CantGuessDescription;
    DescriptionLng = DescriptionRoutine();
    if(DescriptionLng == 0)
    {
CantGuessDescription:
        // Get filename if no description found
        DescriptionLen = AddInName.Len();
        BufString = BufString.String(DescriptionLen, 1);
        RtlCopyMemory(BufString.Get_String(), AddInName.Get_String(), DescriptionLen);
    }
    else
    {
        DescriptionLen = strlen((char *) DescriptionLng);
        BufString = BufString.String(DescriptionLen, 1);
        RtlCopyMemory(BufString.Get_String(), (void *) DescriptionLng, DescriptionLen);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve author name of a DLL (AddIn of Filter)
CStr GetDLLAuthor(FARPROC AuthorRoutine)
{
    long AuthLng = 0;
    long AuthLen = 0;
    CStr ReturnValue;
    CStr BufString;

    if(AuthorRoutine == 0) goto CantGuessAuthor;
    // Should return an author name string or 0
    AuthLng = AuthorRoutine();
    if(AuthLng == 0)
    {
CantGuessAuthor:
        // Get filename if no description found
        AuthLen = strlen("(The unknown soldier)");
        BufString = BufString.String(AuthLen, 1);
        RtlCopyMemory(BufString.Get_String(), (void *) "(The unknown soldier)", AuthLen);
    }
    else
    {
        AuthLen = strlen((char *) AuthLng);
        BufString = BufString.String(AuthLen, 1);
        RtlCopyMemory(BufString.Get_String(), (void *) AuthLng, AuthLen);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if a file is a suitable addin
long IsAddIn(CStr AddInName)
{
    HMODULE AddInLibMod = 0;
    FARPROC AddInDesc = 0;
    FARPROC AddInAuth = 0;
    FARPROC AddInLoad = 0;
    FARPROC AddInUnload = 0;
    FARPROC AddInMenu = 0;
    long ReturnValue = 0;

    AddInLibMod = LoadLibrary(AddInName.Get_String());
    if(AddInLibMod != 0)
    {
        AddInDesc = GetProcAddress(AddInLibMod, "AddInDescription");
        AddInAuth = GetProcAddress(AddInLibMod, "AddInAuthor");
        AddInLoad = GetProcAddress(AddInLibMod, "AddInLoad");
        AddInUnload = GetProcAddress(AddInLibMod, "AddInUnLoad");
        AddInMenu = GetProcAddress(AddInLibMod, "AddInMenu");
        // Must export all procedures
        if(AddInDesc != 0 && AddInAuth != 0 && AddInLoad != 0 && AddInUnload != 0 && AddInMenu != 0)
        {
            // It is an AddIn
            ReturnValue = 1;
        }
        FreeLibrary(AddInLibMod);
        CloseHandle((HANDLE) AddInLibMod);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if a file is a suitable support addin
long IsSpAddIn(CStr SpAddInName)
{
    HMODULE SpAddInLib = 0;
    FARPROC SpAddInDesc = 0;
    FARPROC SpAddInAuth = 0;
    FARPROC SpAddInLoad = 0;
    FARPROC SpAddInUnload = 0;
    FARPROC SpAddInMenu = 0;
    long ReturnValue = 0;
    
    SpAddInLib = LoadLibrary(SpAddInName.Get_String());
    if(SpAddInLib != 0)
    {
        SpAddInDesc = GetProcAddress(SpAddInLib, "SpAddInDescription");
        SpAddInAuth = GetProcAddress(SpAddInLib, "SpAddInAuthor");
        SpAddInLoad = GetProcAddress(SpAddInLib, "SpAddInLoad");
        SpAddInUnload = GetProcAddress(SpAddInLib, "SpAddInUnLoad");
        SpAddInMenu = GetProcAddress(SpAddInLib, "SpAddInAskResource");
        // Must export all procedures
        if(SpAddInDesc != 0 &&
           SpAddInAuth != 0 &&
           SpAddInLoad != 0 &&
           SpAddInUnload != 0 &&
           SpAddInMenu != 0)
        {
            // It is an AddIn
            ReturnValue = 1;
        }
        FreeLibrary(SpAddInLib);
        CloseHandle((HANDLE) SpAddInLib);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Running asked
long RunAddIn(long AddInToRun)
{
    long ReturnValue = 0;

    RunningAddIns.Set(AddInToRun, 0);
    // if addin is already running then will need to notify a possible menu
    // to perform the related command.
    // (auto update of an addin shouldn't run the menu command in case of a
    // persistant addin (which might be used for somekind of prefs).
    ReturnValue = ADDIN_PERSISTANT;
    if(AddInsLoad.Get(AddInToRun)->Content != 0)
    {
        switch(JumpToAddrArg(AddInsLoad.Get(AddInToRun)->Content, (long) &AddInLib))
        {
            case ADDIN_FINISHED:
                RunningAddIns.Set(AddInToRun, 0);
                // Now trigger the menu routine
                if(AddInsMenu.Get(AddInToRun)->Content != 0)
                {
                    RunningAddIns.Set(AddInToRun, 1);
                    switch(JumpToAddrArg(AddInsMenu.Get(AddInToRun)->Content, (long) &AddInLib))
                    {
                        case ADDIN_DIE:
                            if(AddInsUnLoad.Get(AddInToRun)->Content != 0)
                            {
                                JumpToAddr(AddInsUnLoad.Get(AddInToRun)->Content);
                            }
                            RunningAddIns.Set(AddInToRun, 0);
                            break;
                    }
                }
                ReturnValue = ADDIN_FINISHED;
                break;
            case ADDIN_PERSISTANT:
                RunningAddIns.Set(AddInToRun, 1);
                ReturnValue = ADDIN_PERSISTANT;
                break;
        }
    }
    // Store running state
    IniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(AddInToRun, 3).Get_String(),
                  RunningAddIns.Get(AddInToRun)->Content, MainIniFile);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Fill the ChromaticLib datas
void FillChromaticLib(void)
{
    AddInLib.ButtonGetImage = (long) &ButtonGetImage;
    AddInLib.ButtonGetState = (long) &ButtonGetState;
    AddInLib.ButtonSetIcon = (long) &ButtonSetIcon;
    AddInLib.CheckBoxGetState = (long) &CheckBoxGetState;
    AddInLib.CheckBoxSetState = (long) &CheckBoxSetState;
    AddInLib.ClientGetActiveChild = (long) &ClientGetActiveChild;
    AddInLib.ClientSetNextChild = (long) &ClientSetNextChild;
    AddInLib.ClientSetPreviousChild = (long) &ClientSetPreviousChild;
    AddInLib.ClientResize = (long) &ClientResize;
    AddInLib.ClientTileArrangeIconic = (long) &ClientTileArrangeIconic;
    AddInLib.ClientTileCascade = (long) &ClientTileCascade;
    AddInLib.ClientTileHorizontal = (long) &ClientTileHorizontal;
    AddInLib.ClientTileVertical = (long) &ClientTileVertical;
    AddInLib.ColorBoxGetColor = (long) &ColorBoxGetColor;
    AddInLib.ColorBoxSetColor = (long) &ColorBoxSetColor;
    AddInLib.GripBoxGetColor = (long) &GripBoxGetColor;
    AddInLib.GripBoxSetColor = (long) &GripBoxSetColor;
    AddInLib.GripBoxClose = (long) &GripBoxClose;
    AddInLib.GripBoxVisible = (long) &GripBoxVisible;
    AddInLib.GripBoxResize = (long) &GripBoxResize;
    AddInLib.ComboBoxAddItem = (long) &Wrapper_ComboBoxAddItem;
    AddInLib.ComboBoxCount = (long) &ComboBoxCount;
    AddInLib.ComboBoxDeleteItem = (long) &ComboBoxDeleteItem;
    AddInLib.ComboBoxGetCurrentItem = (long) &Wrapper_ComboBoxGetCurrentItem;
    AddInLib.ComboBoxGetIndex = (long) &ComboBoxGetIndex;
    AddInLib.ComboBoxGetItem = (long) &Wrapper_ComboBoxGetItem;
    AddInLib.ComboBoxGetItemData = (long) &ComboBoxGetItemData;
    AddInLib.ComboBoxItemExist = (long) &Wrapper_ComboBoxItemExist;
    AddInLib.ComboBoxReset = (long) &ComboBoxReset;
    AddInLib.ComboBoxSetIndex = (long) &ComboBoxSetIndex;
    AddInLib.ComboBoxSetMaxLen = (long) &ComboBoxSetMaxLen;
    AddInLib.ComboBoxSaveInIniFile = (long) &Wrapper_ComboBoxSaveInIniFile;
    AddInLib.ComboBoxFillFromIniFile = (long) &Wrapper_ComboBoxFillFromIniFile;
    AddInLib.ControlNextHook = (long) &ControlNextHook;
    AddInLib.ControlBound = (long) &ControlBound;
    AddInLib.ControlBringToBottom = (long) &ControlBringToBottom;
    AddInLib.ControlBringToTop = (long) &ControlBringToTop;
    AddInLib.ControlClientHeight = (long) &ControlClientHeight;
    AddInLib.ControlClientLeft = (long) &ControlClientLeft;
    AddInLib.ControlClientTop = (long) &ControlClientTop;
    AddInLib.ControlClientWidth = (long) &ControlClientWidth;
    AddInLib.ControlClose = (long) &ControlClose;
    AddInLib.ControlEnable = (long) &ControlEnable;
    AddInLib.ControlFreeze = (long) &ControlFreeze;
    AddInLib.ControlGetClassName = (long) &Wrapper_ControlGetClassName;
    AddInLib.ControlGetFont = (long) &ControlGetFont;
    AddInLib.ControlGetHotKey = (long) &ControlGetHotKey;
    AddInLib.ControlGetHwndFromPoint = (long) &ControlGetHwndFromPoint;
    AddInLib.ControlGetIcon = (long) &ControlGetIcon;
    AddInLib.ControlGetNotifiedCommand = (long) &ControlGetNotifiedCommand;
    AddInLib.ControlGetNotifiedhWnd = (long) &ControlGetNotifiedhWnd;
    AddInLib.ControlGetNotifiedID = (long) &ControlGetNotifiedID;
    AddInLib.ControlGetNotifiedMsg = (long) &ControlGetNotifiedMsg;
    AddInLib.ControlGetText = (long) &Wrapper_ControlGetText;
    AddInLib.ControlGetTextLen = (long) &ControlGetTextLen;
    AddInLib.ControlGetWindowState = (long) &ControlGetWindowState;
    AddInLib.ControlGetXMousePos = (long) &ControlGetXMousePos;
    AddInLib.ControlGetYMousePos = (long) &ControlGetYMousePos;
    AddInLib.ControlHeight = (long) &ControlHeight;
    AddInLib.ControlIsEnabled = (long) &ControlIsEnabled;
    AddInLib.ControlIsVisible = (long) &ControlIsVisible;
    AddInLib.ControlIsWindowChild = (long) &ControlIsWindowChild;
    AddInLib.ControlLeft = (long) &ControlLeft;
    AddInLib.ControlRefresh = (long) &ControlRefresh;
    AddInLib.ControlRefreshLocal = (long) &ControlRefreshLocal;
    AddInLib.ControlRemTopMost = (long) &ControlRemTopMost;
    AddInLib.ControlRepaint = (long) &ControlRepaint;
    AddInLib.ControlResize = (long) &ControlResize;
    AddInLib.ControlSendMessage = (long) &ControlSendMessage;
    AddInLib.ControlSetClassCursor = (long) &ControlSetClassCursor;
    AddInLib.ControlSetFont = (long) &ControlSetFont;
    AddInLib.ControlSetFontNoRedraw = (long) &ControlSetFontNoRedraw;
    AddInLib.ControlSetText = (long) &Wrapper_ControlSetText;
    AddInLib.ControlSetTopMost = (long) &ControlSetTopMost;
    AddInLib.ControlTop = (long) &ControlTop;
    AddInLib.ControlVisible = (long) &ControlVisible;
    AddInLib.ControlWidth = (long) &ControlWidth;
    AddInLib.CreateButton = (long) &Wrapper_CreateButton;
    AddInLib.CreateCheckBox = (long) &Wrapper_CreateCheckBox;
    AddInLib.CreateClient = (long) &CreateClient;
    AddInLib.CreateColorBox = (long) &CreateColorBox;
    AddInLib.CreateGripBox = (long) &CreateGripBox;
    AddInLib.CreateComboBox = (long) &Wrapper_CreateComboBox;
    AddInLib.CreateNonModalDialog = (long) &Wrapper_CreateNonModalDialog;
    AddInLib.CreateFrame = (long) &Wrapper_CreateFrame;
    AddInLib.CreateHexBox = (long) &Wrapper_CreateHexBox;
    AddInLib.CreateImageList = (long) &CreateImageList;
    AddInLib.CreateIPBox = (long) &CreateIPBox;
    AddInLib.CreateLabel = (long) &Wrapper_CreateLabel;
    AddInLib.CreateListBox = (long) &CreateListBox;
    AddInLib.CreateListView = (long) &CreateListView;
    AddInLib.CreateMDIChildDialog = (long) &Wrapper_CreateMDIChildDialog;
    AddInLib.CreateMDIDialog = (long) &Wrapper_CreateMDIDialog;
    AddInLib.CreateModalDialog = (long) &Wrapper_CreateModalDialog;
    AddInLib.CreatePager = (long) &CreatePager;
    AddInLib.CreatePictureBox = (long) &CreatePictureBox;
    AddInLib.CreateProgressBar = (long) &CreateProgressBar;
    AddInLib.CreateRadioButton = (long) &Wrapper_CreateRadioButton;
    AddInLib.CreateRebar = (long) &CreateRebar;
    AddInLib.CreateRichTextBox = (long) &Wrapper_CreateRichTextBox;
    AddInLib.CreateScrollBar = (long) &CreateScrollBar;
    AddInLib.CreateDumpBox = (long) &CreateDumpBox;
    AddInLib.CreateStockModalDialog = (long) &Wrapper_CreateStockModalDialog;
    AddInLib.DumpBoxScrollDown = (long) &DumpBoxScrollDown;
    AddInLib.DumpBoxScrollUp = (long) &DumpBoxScrollUp;
    AddInLib.DumpBoxGetVisibleLines = (long) &DumpBoxGetVisibleLines;
    AddInLib.DumpBoxGetVisibleColumns = (long) &DumpBoxGetVisibleColumns;
    AddInLib.DumpBoxResize = (long) &DumpBoxResize;
    AddInLib.CreateSplashDialog = (long) &Wrapper_CreateSplashDialog;
    AddInLib.CreateSplitter = (long) &CreateSplitter;
    AddInLib.CreateStatusBar = (long) &Wrapper_CreateStatusBar;
    AddInLib.CreateSysTab = (long) &CreateSysTab;
    AddInLib.CreateTextBox = (long) &Wrapper_CreateTextBox;
    AddInLib.CreateToolBar = (long) &CreateToolBar;
    AddInLib.CreateTrackBar = (long) &CreateTrackBar;
    AddInLib.CreateTreeView = (long) &CreateTreeView;
    AddInLib.CreateUpDown = (long) &CreateUpDown;
    AddInLib.CursorDisable = (long) &CursorDisable;
    AddInLib.CursorEnable = (long) &CursorEnable;
    AddInLib.CursorSetCross = (long) &CursorSetCross;
    AddInLib.CursorSetNo = (long) &CursorSetNo;
    AddInLib.CursorSetNormal = (long) &CursorSetNormal;
    AddInLib.CursorSetWait = (long) &CursorSetWait;
    AddInLib.DateGetDay = (long) &Wrapper_DateGetDay;
    AddInLib.DateGetFormat = (long) &Wrapper_DateGetFormat;
    AddInLib.DateGetHour = (long) &Wrapper_DateGetHour;
    AddInLib.DateGetMinutes = (long) &Wrapper_DateGetMinutes;
    AddInLib.DateGetMonth = (long) &Wrapper_DateGetMonth;
    AddInLib.DateGetNow = (long) &Wrapper_DateGetNow;
    AddInLib.DateGetSeconds = (long) &Wrapper_DateGetSeconds;
    AddInLib.DateGetTimeFormat = (long) &Wrapper_DateGetTimeFormat;
    AddInLib.DateGetYear = (long) &Wrapper_DateGetYear;
    AddInLib.DialogGetXUnit = (long) &DialogGetXUnit;
    AddInLib.DialogGetYUnit = (long) &DialogGetYUnit;
    AddInLib.DialogSetIcon = (long) &DialogSetIcon;
    AddInLib.DialogXPixelToUnit = (long) &DialogXPixelToUnit;
    AddInLib.DialogXUnitToPixel = (long) &DialogXUnitToPixel;
    AddInLib.DialogYPixelToUnit = (long) &DialogYPixelToUnit;
    AddInLib.DialogYUnitToPixel = (long) &DialogYUnitToPixel;
    AddInLib.FileClose = (long) &FileClose;
    AddInLib.FileCreateEmpty = (long) &Wrapper_FileCreateEmpty;
    AddInLib.FileExist = (long) &Wrapper_FileExist;
    AddInLib.FileGetAccessedTime = (long) &Wrapper_FileGetAccessedTime;
    AddInLib.FileGetCreationTime = (long) &Wrapper_FileGetCreationTime;
    AddInLib.FileGetDirectory = (long) &Wrapper_FileGetDirectory;
    AddInLib.FileGetExtension = (long) &Wrapper_FileGetExtension;
    AddInLib.FileGetFileName = (long) &Wrapper_FileGetFileName;
    AddInLib.FileGetSize = (long) &Wrapper_FileGetSize;
    AddInLib.FileGetWriteTime = (long) &Wrapper_FileGetWriteTime;
    AddInLib.FileIsDirectory = (long) &Wrapper_FileIsDirectory;
    AddInLib.FileOpenR = (long) &Wrapper_FileOpenR;
    AddInLib.FileOpenW = (long) &Wrapper_FileOpenW;
    AddInLib.FileOpenWAppend = (long) &Wrapper_FileOpenWAppend;
    AddInLib.FileRemoveExtension = (long) &Wrapper_FileRemoveExtension;
    AddInLib.FileReplaceExtension = (long) &Wrapper_FileReplaceExtension;
    AddInLib.FileIsReadOnly = (long) &Wrapper_FileIsReadOnly;
    AddInLib.FileIsUnix = (long) &Wrapper_FileIsUnix;
    AddInLib.FileWriteBuffer = (long) &FileWriteBuffer;
    AddInLib.FileWriteLine = (long) &Wrapper_FileWriteLine;
    AddInLib.FileLoadIntoMem = (long) &Wrapper_FileLoadIntoMem;
    AddInLib.FileSaveFromMem = (long) &Wrapper_FileSaveFromMem;
    AddInLib.FileDir = (long) &Wrapper_FileDir;
    AddInLib.FrameRefreshToolbar = (long) &FrameRefreshToolbar;
    AddInLib.FTPGetEntryDate = (long) &Wrapper_FTPGetEntryDate;
    AddInLib.FTPGetEntryFileAttributes = (long) &Wrapper_FTPGetEntryFileAttributes;
    AddInLib.FTPGetEntryFileName = (long) &Wrapper_FTPGetEntryFileName;
    AddInLib.FTPGetEntryFileSize = (long) &Wrapper_FTPGetEntryFileSize;
    AddInLib.FTPInitiatePort = (long) &FTPInitiatePort;
    AddInLib.FTPIsEntryDir = (long) &Wrapper_FTPIsEntryDir;
    AddInLib.GDIDrawHorzSep = (long) &GDIDrawHorzSep;
    AddInLib.GDIDrawLine = (long) &GDIDrawLine;
    AddInLib.GDIDrawLineXOR = (long) &GDIDrawLineXOR;
    AddInLib.GDIDrawPixel = (long) &GDIDrawPixel;
    AddInLib.GDIDrawVertSep = (long) &GDIDrawVertSep;
    AddInLib.GDIFillWindow = (long) &GDIFillWindow;
    AddInLib.GDIFillWindowFromBrush = (long) &GDIFillWindowFromBrush;
    AddInLib.GDIFillWindowFromBrushAndhDC = (long) &GDIFillWindowFromBrushAndhDC;
    AddInLib.GDICreateColorBrush = (long) &GDICreateColorBrush;
    AddInLib.GDIGetTextHeight = (long) &Wrapper_GDIGetTextHeight;
    AddInLib.GDIGetTextWidth = (long) &Wrapper_GDIGetTextWidth;
    AddInLib.GDIObtainFont = (long) &Wrapper_GDIObtainFont;
    AddInLib.GDIWriteClippedText = (long) &Wrapper_GDIWriteClippedText;
    AddInLib.GDIWriteText = (long) &Wrapper_GDIWriteText;
    AddInLib.GDIGetFontWidth = (long) &GDIGetFontWidth;
    AddInLib.GDIGetFontHeight = (long) &GDIGetFontHeight;
    AddInLib.GDICreateBackDC = (long) &GDICreateBackDC;
    AddInLib.GDIDestroyBackDC = (long) &GDIDestroyBackDC;
    AddInLib.GDIBlitBackDC = (long) &GDIBlitBackDC;
    AddInLib.GDIColorCalcLuminosity = (long) &GDIColorCalcLuminosity;
    AddInLib.GDIGetSerif = (long) &GDIGetSerif;
    AddInLib.GDIGetSerif10 = (long) &GDIGetSerif10;
    AddInLib.GDIGetCourierNew9 = (long) &GDIGetCourierNew9;
    AddInLib.ImageListAddIcon = (long) &ImageListAddIcon;
    AddInLib.ImageListDestroy = (long) &ImageListDestroy;
    AddInLib.ImageListGetIcon = (long) &ImageListGetIcon;
    AddInLib.ImageListGetIconsCount = (long) &ImageListGetIconsCount;
    AddInLib.IniDeleteKey = (long) &Wrapper_IniDeleteKey;
    AddInLib.IniReadKey = (long) &Wrapper_IniReadKey;
    AddInLib.IniReadBoolKey = (long) &Wrapper_IniReadBoolKey;
    AddInLib.IniWriteKey = (long) &Wrapper_IniWriteKey;
    AddInLib.IniWriteSection = (long) &Wrapper_IniWriteSection;
    AddInLib.IPBoxClearIP = (long) &IPBoxClearIP;
    AddInLib.IPBoxGetIP = (long) &IPBoxGetIP;
    AddInLib.IPBoxIsBlank = (long) &IPBoxIsBlank;
    AddInLib.IPBoxSetFocus = (long) &IPBoxSetFocus;
    AddInLib.IPBoxSetIP = (long) &IPBoxSetIP;
    AddInLib.IPBoxSetRange = (long) &IPBoxSetRange;
    AddInLib.LabelGetImage = (long) &LabelGetImage;
    AddInLib.ListBoxAddItem = (long) &Wrapper_ListBoxAddItem;
    AddInLib.ListBoxCount = (long) &ListBoxCount;
    AddInLib.ListBoxDeleteItem = (long) &ListBoxDeleteItem;
    AddInLib.ListBoxGetCurrentItem = (long) &Wrapper_ListBoxGetCurrentItem;
    AddInLib.ListBoxGetItem = (long) &Wrapper_ListBoxGetItem;
    AddInLib.ListBoxGetItemData = (long) &ListBoxGetItemData;
    AddInLib.ListBoxGetSelItemIndex = (long) &ListBoxGetSelItemIndex;
    AddInLib.ListBoxGetTopIndex = (long) &ListBoxGetTopIndex;
    AddInLib.ListBoxIsItemSelected = (long) &ListBoxIsItemSelected;
    AddInLib.ListBoxItemExist = (long) &Wrapper_ListBoxItemExist;
    AddInLib.ListBoxProcessDrag = (long) &ListBoxProcessDrag;
    AddInLib.ListBoxReplaceItem = (long) &Wrapper_ListBoxReplaceItem;
    AddInLib.ListBoxReplaceSelItem = (long) &Wrapper_ListBoxReplaceSelItem;
    AddInLib.ListBoxReset = (long) &ListBoxReset;
    AddInLib.ListBoxSelItemDown = (long) &ListBoxSelItemDown;
    AddInLib.ListBoxSelItemRemove = (long) &ListBoxSelItemRemove;
    AddInLib.ListBoxSelItemUp = (long) &ListBoxSelItemUp;
    AddInLib.ListBoxSetColumnsWidth = (long) &ListBoxSetColumnsWidth;
    AddInLib.ListBoxSetHorzScrollWidth = (long) &ListBoxSetHorzScrollWidth;
    AddInLib.ListBoxSetIndex = (long) &ListBoxSetIndex;
    AddInLib.ListBoxSetItemData = (long) &ListBoxSetItemData;
    AddInLib.ListBoxSetTopIndex = (long) &ListBoxSetTopIndex;
    AddInLib.ListBoxItemUnderCursor = (long) &ListBoxItemUnderCursor;
    AddInLib.ListViewAddCol = (long) &Wrapper_ListViewAddCol;
    AddInLib.ListViewAddItem = (long) &Wrapper_ListViewAddItem;
    AddInLib.ListViewBeginDrag = (long) &ListViewBeginDrag;
    AddInLib.ListViewCheckBoxItemDoubleClick = (long) &ListViewCheckBoxItemDoubleClick;
    AddInLib.ListViewClear = (long) &ListViewClear;
    AddInLib.ListViewDeleteItem = (long) &ListViewDeleteItem;
    AddInLib.ListViewDetachImageList = (long) &ListViewDetachImageList;
    AddInLib.ListViewEditValidated = (long) &ListViewEditValidated;
    AddInLib.ListViewEndDrag = (long) &ListViewEndDrag;
    AddInLib.ListViewFindItem = (long) &Wrapper_ListViewFindItem;
    AddInLib.ListViewFindSubItem = (long) &Wrapper_ListViewFindSubItem;
    AddInLib.ListViewGetBackColor = (long) &ListViewGetBackColor;
    AddInLib.ListViewGetColWidth = (long) &ListViewGetColWidth;
    AddInLib.ListViewGetCountPerPage = (long) &ListViewGetCountPerPage;
    AddInLib.ListViewGetEditControl = (long) &ListViewGetEditControl;
    AddInLib.ListViewGetEditResult = (long) &Wrapper_ListViewGetEditResult;
    AddInLib.ListViewGetExStyle = (long) &ListViewGetExStyle;
    AddInLib.ListViewGetFirstFocusItem = (long) &ListViewGetFirstFocusItem;
    AddInLib.ListViewGetHeaderLabel = (long) &Wrapper_ListViewGetHeaderLabel;
    AddInLib.ListViewGetHeaderPosition = (long) &ListViewGetHeaderPosition;
    AddInLib.ListViewGetHotCursor = (long) &ListViewGetHotCursor;
    AddInLib.ListViewGetImageList = (long) &ListViewGetImageList;
    AddInLib.ListViewGetItemCheckbox = (long) &ListViewGetItemCheckbox;
    AddInLib.ListViewGetItemIcon = (long) &ListViewGetItemIcon;
    AddInLib.ListViewGetItemSelState = (long) &ListViewGetItemSelState;
    AddInLib.ListViewGetItemText = (long) &Wrapper_ListViewGetItemText;
    AddInLib.ListViewGetItemUnderCursor = (long) &ListViewGetItemUnderCursor;
    AddInLib.ListViewGetNotifiedColumnIndex = (long) &ListViewGetNotifiedColumnIndex;
    AddInLib.ListViewGetSelItem = (long) &ListViewGetSelItem;
    AddInLib.ListViewGetSelItemText = (long) &Wrapper_ListViewGetSelItemText;
    AddInLib.ListViewGetSubItemImage = (long) &ListViewGetSubItemImage;
    AddInLib.ListViewGetSysHeader = (long) &ListViewGetSysHeader;
    AddInLib.ListViewGetTextBackColor = (long) &ListViewGetTextBackColor;
    AddInLib.ListViewGetTextColor = (long) &ListViewGetTextColor;
    AddInLib.ListViewGetToolTips = (long) &ListViewGetToolTips;
    AddInLib.ListViewGetTopIndex = (long) &ListViewGetTopIndex;
    AddInLib.ListViewIsCheckboxNotify = (long) &ListViewIsCheckboxNotify;
    AddInLib.ListViewItemCount = (long) &ListViewItemCount;
    AddInLib.ListViewMoveDrag = (long) &ListViewMoveDrag;
    AddInLib.ListViewMultiSelFirstItem = (long) &ListViewMultiSelFirstItem;
    AddInLib.ListViewPasteAutoEdit = (long) &ListViewPasteAutoEdit;
    AddInLib.ListViewReOrder = (long) &ListViewReOrder;
    AddInLib.ListViewSelItemCount = (long) &ListViewSelItemCount;
    AddInLib.ListViewSetBackColor = (long) &ListViewSetBackColor;
    AddInLib.ListViewSetColWidth = (long) &ListViewSetColWidth;
    AddInLib.ListViewSetEditModeOn = (long) &ListViewSetEditModeOn;
    AddInLib.ListViewSetHeaderPosition = (long) &ListViewSetHeaderPosition;
    AddInLib.ListViewSetItemCheckbox = (long) &ListViewSetItemCheckbox;
    AddInLib.ListViewSetItemFocus = (long) &ListViewSetItemFocus;
    AddInLib.ListViewSetItemSel = (long) &ListViewSetItemSel;
    AddInLib.ListViewSetItemHighlight = (long) &ListViewSetItemHighlight;
    AddInLib.ListViewSetScroll = (long) &ListViewSetScroll;
    AddInLib.ListViewSetSelItemText = (long) &Wrapper_ListViewSetSelItemText;
    AddInLib.ListViewSetSubItem = (long) &Wrapper_ListViewSetSubItem;
    AddInLib.ListViewSetSubItemImage = (long) &ListViewSetSubItemImage;
    AddInLib.ListViewSort = (long) &ListViewSort;
    AddInLib.MenuEnable = (long) &MenuEnable;
    AddInLib.MenuGetString = (long) &Wrapper_MenuGetString;
    AddInLib.MenuSetDefaultItem = (long) &MenuSetDefaultItem;
    AddInLib.MenuSetItemState = (long) &MenuSetItemState;
    AddInLib.MenuSetItemType = (long) &MenuSetItemType;
    AddInLib.MenuSetItemText = (long) &Wrapper_MenuSetItemText;
    AddInLib.MenuAddSeparator = (long) &MenuAddSeparator;
    AddInLib.MenuAddString = (long) &Wrapper_MenuAddString;
    AddInLib.PagerDisplaySetSize = (long) &PagerDisplaySetSize;
    AddInLib.PagerGetBackColor = (long) &PagerGetBackColor;
    AddInLib.PagerGetBorderSize = (long) &PagerGetBorderSize;
    AddInLib.PagerGetButtonSize = (long) &PagerGetButtonSize;
    AddInLib.PagerGetNotifiedChild = (long) &PagerGetNotifiedChild;
    AddInLib.PagerGetScrollPos = (long) &PagerGetScrollPos;
    AddInLib.PagerGetOrientation = (long) &PagerGetOrientation;
    AddInLib.PagerSetOrientation = (long) &PagerSetOrientation;
    AddInLib.PagerChangeOrientation = (long) &PagerChangeOrientation;
    AddInLib.PagerMaximizeX = (long) &PagerMaximizeX;
    AddInLib.PagerMaximizeY = (long) &PagerMaximizeY;
    AddInLib.PagerSetBackColor = (long) &PagerSetBackColor;
    AddInLib.PictureBoxChangeIcon = (long) &PictureBoxChangeIcon;
    AddInLib.PictureBoxChangeImage = (long) &PictureBoxChangeImage;
    AddInLib.ProgressBarSetRelPos = (long) &ProgressBarSetRelPos;
    AddInLib.ProgressBarSetRange = (long) &ProgressBarSetRange;
    AddInLib.RadioButtonGetState = (long) &RadioButtonGetState;
    AddInLib.RadioButtonSetState = (long) &RadioButtonSetState;
    AddInLib.RebarAddBand = (long) &Wrapper_RebarAddBand;
    AddInLib.RebarBandVisible = (long) &RebarBandVisible;
    AddInLib.RebarGetBackGroundColor = (long) &RebarGetBackGroundColor;
    AddInLib.RebarGetBandsCount = (long) &RebarGetBandsCount;
    AddInLib.RebarGetHeight = (long) &RebarGetHeight;
    AddInLib.RebarGetImageList = (long) &RebarGetImageList;
    AddInLib.RebarGetPalette = (long) &RebarGetPalette;
    AddInLib.RebarGetRowsCount = (long) &RebarGetRowsCount;
    AddInLib.RebarGetTextColor = (long) &RebarGetTextColor;
    AddInLib.RebarGetToolTips = (long) &RebarGetToolTips;
    AddInLib.RebarGetXSize = (long) &RebarGetXSize;
    AddInLib.RebarGetYSize = (long) &RebarGetYSize;
    AddInLib.RebarRemoveBand = (long) &RebarRemoveBand;
    AddInLib.RebarResize = (long) &RebarResize;
    AddInLib.RebarSetBackColor = (long) &RebarSetBackColor;
    AddInLib.RebarSetTextColor = (long) &RebarSetTextColor;
    AddInLib.RegistryDeleteKey = (long) &Wrapper_RegistryDeleteKey;
    AddInLib.RegistryGetKeyValue = (long) &Wrapper_RegistryGetKeyValue;
    AddInLib.RegistryUpdateKey = (long) &Wrapper_RegistryUpdateKey;
    AddInLib.ScreenHeight = (long) &ScreenHeight;
    AddInLib.ScreenWidth = (long) &ScreenWidth;
    AddInLib.ScreenRectToClient = (long) &ScreenRectToClient;
    AddInLib.ScrollBarGetMaxRange = (long) &ScrollBarGetMaxRange;
    AddInLib.ScrollBarGetMinRange = (long) &ScrollBarGetMinRange;
    AddInLib.ScrollBarGetPageRange = (long) &ScrollBarGetPageRange;
    AddInLib.ScrollBarSetPageRange = (long) &ScrollBarSetPageRange;
    AddInLib.ScrollBarShowHide = (long) &ScrollBarShowHide;
    AddInLib.ScrollBarSetMinMaxRange = (long) &ScrollBarSetMinMaxRange;
    AddInLib.MathGenerateRandomNumber = (long) &MathGenerateRandomNumber;
    AddInLib.SocketAcceptConnection = (long) &SocketAcceptConnection;
    AddInLib.SocketConnect = (long) &Wrapper_SocketConnect;
    AddInLib.SocketCreate = (long) &SocketCreate;
    AddInLib.SocketDestroy = (long) &SocketDestroy;
    AddInLib.SocketGetPort = (long) &SocketGetPort;
    AddInLib.SocketGetRevcLen = (long) &SocketGetRevcLen;
    AddInLib.SocketListen = (long) &SocketListen;
    AddInLib.SocketReceiveASCIIDatas = (long) &Wrapper_SocketReceiveASCIIDatas;
    AddInLib.SocketReceiveUnknownDatas = (long) &SocketReceiveUnknownDatas;
    AddInLib.SocketSendASCIIDatasCR = (long) &Wrapper_SocketSendASCIIDatasCR;
    AddInLib.SocketSendUnknownDatas = (long) &SocketSendUnknownDatas;
    AddInLib.StatusBarGetYSize = (long) &StatusBarGetYSize;
    AddInLib.StatusBarSetParts = (long) &StatusBarSetParts;
    AddInLib.StatusBarSetText = (long) &Wrapper_StatusBarSetText;
    AddInLib.StringCount = (long) &Wrapper_StringCount;
    AddInLib.StringReplace = (long) &Wrapper_StringReplace;
    AddInLib.StringGetSplitElement = (long) &Wrapper_StringGetSplitElement;
    AddInLib.StringGetSplitUBound = (long) &StringGetSplitUBound;
    AddInLib.StringHex16 = (long) &Wrapper_StringHex16;
    AddInLib.StringHex32 = (long) &Wrapper_StringHex32;
    AddInLib.StringHex8 = (long) &Wrapper_StringHex8;
    AddInLib.StringIsAlphaChar = (long) &Wrapper_StringIsAlphaChar;
    AddInLib.StringIsArobasChar = (long) &Wrapper_StringIsArobasChar;
    AddInLib.StringIsSpaceChar = (long) &Wrapper_StringIsSpaceChar;
    AddInLib.StringIsBlankChar = (long) &Wrapper_StringIsBlankChar;
    AddInLib.StringIsDecimal = (long) &Wrapper_StringIsDecimal;
    AddInLib.StringIsDigitChar = (long) &Wrapper_StringIsDigitChar;
    AddInLib.StringIsDotChar = (long) &Wrapper_StringIsDotChar;
    AddInLib.StringIsCommaChar = (long) &Wrapper_StringIsCommaChar;
    AddInLib.StringIsQuoteChar = (long) &Wrapper_StringIsQuoteChar;
    AddInLib.StringIsEOL = (long) &Wrapper_StringIsEOL;
    AddInLib.StringIsHexaChar = (long) &Wrapper_StringIsHexaChar;
    AddInLib.StringIsHexaDecimal = (long) &Wrapper_StringIsHexaDecimal;
    AddInLib.StringIsLabel = (long) &Wrapper_StringIsLabel;
    AddInLib.StringIsLabelChar = (long) &Wrapper_StringIsLabelChar;
    AddInLib.StringIsOpenBracketChar = (long) &Wrapper_StringIsOpenBracketChar;
    AddInLib.StringIsUnderLineChar = (long) &Wrapper_StringIsUnderLineChar;
    AddInLib.StringJoin = (long) &Wrapper_StringJoin;
    AddInLib.StringNumberComplement = (long) &Wrapper_StringNumberComplement;
    AddInLib.StringHexNumberComplement = (long) &Wrapper_StringHexNumberComplement;
    AddInLib.StringOctNumberComplement = (long) &Wrapper_StringOctNumberComplement;
    AddInLib.StringBinNumberComplement = (long) &Wrapper_StringBinNumberComplement;
    AddInLib.StringReleaseSplit = (long) &StringReleaseSplit;
    AddInLib.StringSplit = (long) &Wrapper_StringSplit;
    AddInLib.StringFree = (long) &StringFree;
    AddInLib.StringLen = (long) &StringLen;
    AddInLib.StringCat = (long) &Wrapper_StringCat;
    AddInLib.StringDecToString = (long) &Wrapper_StringDecToString;
    AddInLib.StringStringToDec = (long) &Wrapper_StringStringToDec;
    AddInLib.StringStringToHex = (long) &Wrapper_StringStringToHex;
    AddInLib.StringCountLines = (long) &StringCountLines;
    AddInLib.StringGetLinesOffsets = (long) &StringGetLinesOffsets;
    AddInLib.StringFilterNonPrintableChars = (long) &StringFilterNonPrintableChars;
    AddInLib.StringLeft = (long) &StringLeft;
    AddInLib.StringRight = (long) &StringRight;
    AddInLib.StringTrim = (long) &StringTrim;
    AddInLib.StringLTrim = (long) &StringLTrim;
    AddInLib.StringRTrim = (long) &StringRTrim;
    AddInLib.StringInStr = (long) &StringInStr;
    AddInLib.StringMid = (long) &StringMid;
    AddInLib.StringUpperCase = (long) &StringUpperCase;
    AddInLib.StringLowerCase = (long) &StringLowerCase;
    AddInLib.StringReverse = (long) &StringReverse;
    AddInLib.StringSpace = (long) &StringSpace;
    AddInLib.StringString = (long) &StringString;
    AddInLib.StringIsNumeric = (long) &StringIsNumeric;
    AddInLib.StringFillHexBytes = (long) &StringFillHexBytes;
    AddInLib.StringFillHexWords = (long) &StringFillHexWords;
    AddInLib.StringFillHexDWords = (long) &StringFillHexDWords;
    AddInLib.StringFileNameToLabel = (long) &StringFileNameToLabel;
    AddInLib.SysTabAddItem = (long) &Wrapper_SysTabAddItem;
    AddInLib.SysTabGetCurrentItem = (long) &SysTabGetCurrentItem;
    AddInLib.SysTabGetImagelist = (long) &SysTabGetImagelist;
    AddInLib.SysTabGetRowsCount = (long) &SysTabGetRowsCount;
    AddInLib.SysTabHighLightItem = (long) &SysTabHighLightItem;
    AddInLib.SysTabItemsCount = (long) &SysTabItemsCount;
    AddInLib.SysTabSetCurrentItem = (long) &SysTabSetCurrentItem;
    AddInLib.SysTabSetFocusItem = (long) &SysTabSetFocusItem;
    AddInLib.SysTabSetSeparators = (long) &SysTabSetSeparators;
    AddInLib.SysTabSetItemsSize = (long) &SysTabSetItemsSize;
    AddInLib.SysTabRemoveItem = (long) &SysTabRemoveItem;
    AddInLib.SysTabSetMinWidth = (long) &SysTabSetMinWidth;
    AddInLib.SysTabSetPadding = (long) &SysTabSetPadding;
    AddInLib.TextBoxGetLen = (long) &TextBoxGetLen;
    AddInLib.TextBoxAddText = (long) &Wrapper_TextBoxAddText;
    AddInLib.TextBoxCanPaste = (long) &TextBoxCanPaste;
    AddInLib.TextBoxCanUndo = (long) &TextBoxCanUndo;
    AddInLib.TextBoxGetBreakProcAddress = (long) &TextBoxGetBreakProcAddress;
    AddInLib.TextBoxGetCurrentOptions = (long) &TextBoxGetCurrentOptions;
    AddInLib.TextBoxGetFirstVisibleLine = (long) &TextBoxGetFirstVisibleLine;
    AddInLib.TextBoxGetMarginWidth = (long) &TextBoxGetMarginWidth;
    AddInLib.TextBoxGetMaxLen = (long) &TextBoxGetMaxLen;
    AddInLib.TextBoxGetPasswordChar = (long) &Wrapper_TextBoxGetPasswordChar;
    AddInLib.TextBoxSetPasswordChar = (long) &Wrapper_TextBoxSetPasswordChar;
    AddInLib.TextBoxGetScrollPos = (long) &TextBoxGetScrollPos;
    AddInLib.TextBoxGetSelection = (long) &TextBoxGetSelection;
    AddInLib.TextBoxGetTextHandle = (long) &TextBoxGetTextHandle;
    AddInLib.TextBoxIsModified = (long) &TextBoxIsModified;
    AddInLib.TextBoxLinesCount = (long) &TextBoxLinesCount;
    AddInLib.TextBoxRemoveSel = (long) &TextBoxRemoveSel;
    AddInLib.TextBoxSelText = (long) &TextBoxSelText;
    AddInLib.TextBoxSetCaretPos = (long) &TextBoxSetCaretPos;
    AddInLib.TextBoxSetMaxLen = (long) &TextBoxSetMaxLen;
    AddInLib.ToolBarAddButton = (long) &Wrapper_ToolBarAddButton;
    AddInLib.ToolBarAddSeparator = (long) &ToolBarAddSeparator;
    AddInLib.ToolBarDisplayPopupMenu = (long) &ToolBarDisplayPopupMenu;
    AddInLib.ToolBarDisplayToolTip = (long) &Wrapper_ToolBarDisplayToolTip;
    AddInLib.ToolBarGetButton = (long) &ToolBarGetButton;
    AddInLib.ToolBarGetXPadding = (long) &ToolBarGetXPadding;
    AddInLib.ToolBarGetYPadding = (long) &ToolBarGetYPadding;
    AddInLib.ToolBarGetButtonIndexXSize = (long) &ToolBarGetButtonIndexXSize;
    AddInLib.ToolBarGetButtonsCount = (long) &ToolBarGetButtonsCount;
    AddInLib.ToolBarGetButtonXPosition = (long) &ToolBarGetButtonXPosition;
    AddInLib.ToolBarGetButtonXSize = (long) &ToolBarGetButtonXSize;
    AddInLib.ToolBarGetButtonXYPos = (long) &ToolBarGetButtonXYPos;
    AddInLib.ToolBarGetButtonYSize = (long) &ToolBarGetButtonYSize;
    AddInLib.ToolBarGetImagelist = (long) &ToolBarGetImagelist;
    AddInLib.ToolBarGetNotifiedDropDownItem = (long) &ToolBarGetNotifiedDropDownItem;
    AddInLib.ToolBarGetNotifiedHotItem = (long) &ToolBarGetNotifiedHotItem;
    AddInLib.ToolBarGetNotifiedToolTip = (long) &ToolBarGetNotifiedToolTip;
    AddInLib.ToolBarGetRealPos = (long) &ToolBarGetRealPos;
    AddInLib.ToolBarGetRowsCount = (long) &ToolBarGetRowsCount;
    AddInLib.ToolBarGetXSize = (long) &ToolBarGetXSize;
    AddInLib.ToolBarGetXYSize = (long) &ToolBarGetXYSize;
    AddInLib.ToolBarGetYSize = (long) &ToolBarGetYSize;
    AddInLib.ToolBarIsButtonChecked = (long) &ToolBarIsButtonChecked;
    AddInLib.ToolBarIsButtonPressed = (long) &ToolBarIsButtonPressed;
    AddInLib.ToolBarResize = (long) &ToolBarResize;
    AddInLib.ToolBarSetButtonChecked = (long) &ToolBarSetButtonChecked;
    AddInLib.ToolBarSetButtonEnabled = (long) &ToolBarSetButtonEnabled;
    AddInLib.ToolBarSetButtonPressed = (long) &ToolBarSetButtonPressed;
    AddInLib.ToolBarRemoveButton = (long) &ToolBarRemoveButton;
    AddInLib.ToolBarSetButtonBitmap = (long) &ToolBarSetButtonBitmap;
    AddInLib.TrackBarGetPos = (long) &TrackBarGetPos;
    AddInLib.TrackBarSetPos = (long) &TrackBarSetPos;
    AddInLib.TrackBarSetRange = (long) &TrackBarSetRange;
    AddInLib.TrackBarSetTicks = (long) &TrackBarSetTicks;
    AddInLib.TrackBarGetToolTips = (long) &TrackBarGetToolTips;
    AddInLib.TreeViewAddItem = (long) &Wrapper_TreeViewAddItem;
    AddInLib.TreeViewGetBackColor = (long) &TreeViewGetBackColor;
    AddInLib.TreeViewGetChildItemsCount = (long) &TreeViewGetChildItemsCount;
    AddInLib.TreeViewGetDeletedItem = (long) &TreeViewGetDeletedItem;
    AddInLib.TreeViewGetEditControl = (long) &TreeViewGetEditControl;
    AddInLib.TreeViewGetExpandingItem = (long) &TreeViewGetExpandingItem;
    AddInLib.TreeViewGetExpandingState = (long) &TreeViewGetExpandingState;
    AddInLib.TreeViewGetFirstItemChild = (long) &TreeViewGetFirstItemChild;
    AddInLib.TreeViewGetImageList = (long) &TreeViewGetImageList;
    AddInLib.TreeViewGetIndent = (long) &TreeViewGetIndent;
    AddInLib.TreeViewGetInsertMarkColor = (long) &TreeViewGetInsertMarkColor;
    AddInLib.TreeViewGetItemCount = (long) &TreeViewGetItemCount;
    AddInLib.TreeViewGetItemFromPos = (long) &TreeViewGetItemFromPos;
    AddInLib.TreeViewGetItemHeight = (long) &TreeViewGetItemHeight;
    AddInLib.TreeViewGetItemLevel = (long) &TreeViewGetItemLevel;
    AddInLib.TreeViewGetItemParent = (long) &TreeViewGetItemParent;
    AddInLib.TreeViewGetItemText = (long) &Wrapper_TreeViewGetItemText;
    AddInLib.TreeViewGetLineColor = (long) &TreeViewGetLineColor;
    AddInLib.TreeViewGetNextItem = (long) &TreeViewGetNextItem;
    AddInLib.TreeViewGetPreviousItem = (long) &TreeViewGetPreviousItem;
    AddInLib.TreeViewGetRoot = (long) &TreeViewGetRoot;
    AddInLib.TreeViewGetScrollTime = (long) &TreeViewGetScrollTime;
    AddInLib.TreeViewGetSelChildItemPos = (long) &TreeViewGetSelChildItemPos;
    AddInLib.TreeViewGetSelectedItem = (long) &TreeViewGetSelectedItem;
    AddInLib.TreeViewGetTextColor = (long) &TreeViewGetTextColor;
    AddInLib.TreeViewGetToolTips = (long) &TreeViewGetToolTips;
    AddInLib.TreeViewGetVisibleCount = (long) &TreeViewGetVisibleCount;
    AddInLib.TreeViewRemoveItem = (long) &TreeViewRemoveItem;
    AddInLib.TreeViewSearchChildPartialText = (long) &Wrapper_TreeViewSearchChildPartialText;
    AddInLib.TreeViewSearchItemText = (long) &Wrapper_TreeViewSearchItemText;
    AddInLib.TreeViewSetBackColor = (long) &TreeViewSetBackColor;
    AddInLib.TreeViewSetIndent = (long) &TreeViewSetIndent;
    AddInLib.TreeViewSetItemExpandedState = (long) &TreeViewSetItemExpandedState;
    AddInLib.TreeViewGetItemExpandedState = (long) &TreeViewGetItemExpandedState;
    AddInLib.TreeViewSetSingleExpandState = (long) &TreeViewSetSingleExpandState;
    AddInLib.TreeViewSetItemIcon = (long) &TreeViewSetItemIcon;
    AddInLib.TreeViewSetItemText = (long) &Wrapper_TreeViewSetItemText;
    AddInLib.TreeViewSetSelectedItem = (long) &TreeViewSetSelectedItem;
    AddInLib.TreeViewSetToolTips = (long) &TreeViewSetToolTips;
    AddInLib.TreeViewGetItemTextRect = (long) &TreeViewGetItemTextRect;
    AddInLib.TreeViewGetItemRect = (long) &TreeViewGetItemRect;
    AddInLib.CreateDockingBox = (long) &Wrapper_CreateDockingBox;
    AddInLib.DockingBoxGetState = (long) &DockingBoxGetState;
    AddInLib.DockingBoxRemove = (long) &DockingBoxRemove;
    AddInLib.DockingBoxShow = (long) &Wrapper_DockingBoxShow;
    AddInLib.DockingBoxWasVisible = (long) &Wrapper_DockingBoxWasVisible;
    AddInLib.WinsockInit = (long) &WinsockInit;
    AddInLib.WinsockUnInit = (long) &WinsockUnInit;
    AddInLib.MiscClipBoardCopyText = (long) &Wrapper_MiscClipBoardCopyText;
    AddInLib.MiscClipBoardPasteText = (long) &Wrapper_MiscClipBoardPasteText;
    AddInLib.MiscClipBoardIsEmpty = (long) &MiscClipBoardIsEmpty;
    AddInLib.MiscDoEvents = (long) &MiscDoEvents;
    AddInLib.MiscMsgBox = (long) &Wrapper_MiscMsgBox;
    AddInLib.MiscSendBroadCastMsg = (long) &MiscSendBroadCastMsg;
    AddInLib.MiscWaitEvents = (long) &MiscWaitEvents;
    AddInLib.MiscObtainGUID = (long) &MiscObtainGUID;
    AddInLib.MiscAllocMem = (long) &MiscAllocMem;
    AddInLib.MiscFreeMem = (long) &MiscFreeMem;
    AddInLib.MiscCopyMemLoop = (long) &MiscCopyMemLoop;
    AddInLib.MiscCopyMemWithOffsets = (long) &MiscCopyMemWithOffsets;
    AddInLib.MiscShellAddFileToRecents = (long) &Wrapper_MiscShellAddFileToRecents;
    AddInLib.MiscGetOSClass = (long) &MiscGetOSClass;
    AddInLib.MiscObtainExtendedFunctions = (long) &MiscObtainExtendedFunctions;
    AddInLib.WAMMGetWindowLangGUID = (long) &WAMMGetWindowLangGUID;
    AddInLib.WAMMGetWindowLangInclude = (long) &WAMMGetWindowLangInclude;
    AddInLib.WAMMGetWindowLangDoubleSlash = (long) &WAMMGetWindowLangDoubleSlash;
    AddInLib.WAMMGetWindowLangComment = (long) &WAMMGetWindowLangComment;
    AddInLib.WAMMGetWindowLangMultiComment = (long) &WAMMGetWindowLangMultiComment;
    AddInLib.WAMMGetWindowLangMultiCommentEnd = (long) &WAMMGetWindowLangMultiCommentEnd;
    AddInLib.WAMMGetWindowLangProc = (long) &WAMMGetWindowLangProc;
    AddInLib.WAMMGetWindowLangEndp = (long) &WAMMGetWindowLangEndp;
    AddInLib.WAMMGetWindowLangProcInterlace = (long) &WAMMGetWindowLangProcInterlace;
    AddInLib.WAMMGetWindowLangProcOrphan = (long) &WAMMGetWindowLangProcOrphan;
    AddInLib.WAMMGetWindowLangProcOpenBlock = (long) &WAMMGetWindowLangProcOpenBlock;
    AddInLib.WAMMGetWindowLangProcCloseBlock = (long) &WAMMGetWindowLangProcCloseBlock;
    AddInLib.WAMMGetWindowLangFileName = (long) &WAMMGetWindowLangFileName;
    AddInLib.WAMMGetWindowLangIncludeName = (long) &WAMMGetWindowLangIncludeName;
    AddInLib.WAMMGetWindowLangIncludeNameFoot = (long) &WAMMGetWindowLangIncludeNameFoot;
    AddInLib.WAMMDecodeRelativePaths = (long) &WAMMDecodeRelativePaths;
    AddInLib.WAMMRemoveRelativePaths = (long) &WAMMRemoveRelativePaths;
    AddInLib.WAMMEncodeRelativePaths = (long) &WAMMEncodeRelativePaths;
    AddInLib.WAMMRunUserCommand = (long) &WAMMRunUserCommand;
    AddInLib.WAMMRunCommandScript = (long) &WAMMRunCommandScript;

    AddInLib.ComDlgChooseColor = (long) &Wrapper_ComDlgChooseColor;
    AddInLib.ComDlgGetOpenFileName = (long) &Wrapper_ComDlgGetOpenFileName;
    AddInLib.ComDlgGetSaveFileName = (long) &Wrapper_ComDlgGetSaveFileName;
    AddInLib.ComDlgChooseFont = (long) &Wrapper_ComDlgChooseFont;
    AddInLib.ComDlgBrowseForFolder = (long) &Wrapper_ComDlgBrowseForFolder;
    AddInLib.ComDlgParseMultiFilesSelection = (long) &Wrapper_ComDlgParseMultiFilesSelection;

    AddInLib.WAMMDisplayHelpFile = (long) &WAMMDisplayHelpFile;
    AddInLib.WAMMRunDOSCmd = (long) &WAMMRunDOSCmd;
    AddInLib.WAMMRunDOSCmdCapture = (long) &WAMMRunDOSCmdCapture;
    AddInLib.WAMMGetWindowPath = (long) &WAMMGetWindowPath;
    AddInLib.WAMMIsFileWorkSpace = (long) &WAMMIsFileWorkSpace;
    AddInLib.WAMMIsFileProject = (long) &WAMMIsFileProject;
    AddInLib.WAMMGetCurrentChildState = (long) &WAMMGetCurrentChildState;
    AddInLib.GotoFileLine = (long) &GotoFileLine;
    AddInLib.WAMMFocusOnFile = (long) &WAMMFocusOnFile;
    AddInLib.WAMMGetFileNameFromTreeViewItem = (long) &WAMMGetFileNameFromTreeViewItem;
    AddInLib.WAMMGetLabelFromTreeViewItem = (long) &WAMMGetLabelFromTreeViewItem;
    AddInLib.WAMMOpenUnknownFile = (long) &WAMMOpenUnknownFile;
    AddInLib.WAMMHexToDec = (long) &WAMMHexToDec;
    AddInLib.WAMMDecToHex = (long) &WAMMDecToHex;
    AddInLib.WAMMDecToBin = (long) &WAMMDecToBin;
    AddInLib.WAMMHexToBin = (long) &WAMMHexToBin;
    AddInLib.WAMMBinToDec = (long) &WAMMBinToDec;
    AddInLib.WAMMBinToHex = (long) &WAMMBinToHex;
    AddInLib.WAMMHexToFlt = (long) &WAMMHexToFlt;
    AddInLib.WAMMFltToHex = (long) &WAMMFltToHex;
    AddInLib.WAMMGetProceduresList = (long) &WAMMGetProceduresList;
    AddInLib.WAMMGetLineFromFile = (long) &WAMMGetLineFromFile;
    AddInLib.WAMMCreateNewWindow = (long) &WAMMCreateNewWindow;
    AddInLib.WAMMClearWindow = (long) &WAMMClearWindow;
    AddInLib.WAMMReloadWindow = (long) &WAMMReloadWindow;
    AddInLib.WAMMCloseWindow = (long) &WAMMCloseWindow;
    AddInLib.WAMMCloseAllWindows = (long) &WAMMCloseAllWindows;
    AddInLib.WAMMSaveAllWindows = (long) &WAMMSaveAllWindows;
    AddInLib.WAMMPrintWindow = (long) &WAMMPrintWindow;
    AddInLib.WAMMSaveWindow = (long) &WAMMSaveWindow;
    AddInLib.WAMMUndo = (long) &WAMMUndo;
    AddInLib.WAMMRedo = (long) &WAMMRedo;
    AddInLib.WAMMCut = (long) &WAMMCut;
    AddInLib.WAMMCopy = (long) &WAMMCopy;
    AddInLib.WAMMCopyFilePathName = (long) &WAMMCopyFilePathName;
    AddInLib.WAMMCopyEntireText = (long) &WAMMCopyEntireText;
    AddInLib.WAMMPaste = (long) &WAMMPaste;
    AddInLib.WAMMDeleteSel = (long) &WAMMDeleteSel;
    AddInLib.WAMMDeleteLine = (long) &WAMMDeleteLine;
    AddInLib.WAMMDeleteToStart = (long) &WAMMDeleteToStart;
    AddInLib.WAMMDeleteToEnd = (long) &WAMMDeleteToEnd;
    AddInLib.WAMMSelectLine = (long) &WAMMSelectLine;
    AddInLib.WAMMSelectAll = (long) &WAMMSelectAll;
    AddInLib.WAMMSelectProc = (long) &WAMMSelectProc;
    AddInLib.WAMMFindNext = (long) &WAMMFindNext;
    AddInLib.WAMMFindPrevious = (long) &WAMMFindPrevious;
    AddInLib.WAMMFindSelection = (long) &WAMMFindSelection;
    AddInLib.GotoTop = (long) &GotoTop;
    AddInLib.GotoBottom = (long) &GotoBottom;
    AddInLib.GotoLine = (long) &GotoLine;
    AddInLib.GotoMatchingBracket = (long) &GotoMatchingBracket;
    AddInLib.GotoPrevWord = (long) &GotoPrevWord;
    AddInLib.GotoNextWord = (long) &GotoNextWord;
    AddInLib.GotoPrevLetter = (long) &GotoPrevLetter;
    AddInLib.GotoNextLetter = (long) &GotoNextLetter;
    AddInLib.GotoPrevLine = (long) &GotoPrevLine;
    AddInLib.GotoNextLine = (long) &GotoNextLine;
    AddInLib.GotoPrevPage = (long) &GotoPrevPage;
    AddInLib.GotoNextPage = (long) &GotoNextPage;
    AddInLib.WAMMGetMiniChildStruct = (long) &WAMMGetMiniChildStruct;
    AddInLib.WAMMGetIniFilePathName = (long) &WAMMGetIniFilePathName;
    AddInLib.WAMMGetQuickDirectory = (long) &WAMMGetQuickDirectory;
    AddInLib.WAMMGetContext = (long) &WAMMGetContext;
    AddInLib.WAMMGetcurrentLine = (long) &WAMMGetcurrentLine;
    AddInLib.WAMMSelectCurrentWord = (long) &WAMMSelectCurrentWord;
    AddInLib.WAMMGetCurrentWord = (long) &WAMMGetCurrentWord;
    AddInLib.InsertText = (long) &InsertText;
    AddInLib.WAMMInsertLine = (long) &WAMMInsertLine;
    AddInLib.WAMMInsertLines = (long) &WAMMInsertLines;
    AddInLib.WAMMSetText = (long) &WAMMSetText;
    AddInLib.WAMMGetCaretRealXCoordinate = (long) &WAMMGetCaretRealXCoordinate;
    AddInLib.WAMMGetCaretRealYCoordinate = (long) &WAMMGetCaretRealYCoordinate;
    AddInLib.WAMMSetFocusInsideWindow = (long) &WAMMSetFocusInsideWindow;
    AddInLib.WAMMRestoreMDIFromSysTray = (long) &WAMMRestoreMDIFromSysTray;
    AddInLib.WAMMInstallHookProc = (long) &WAMMInstallHookProc;
    AddInLib.WAMMUnInstallHookProc = (long) &WAMMUnInstallHookProc;
    AddInLib.WAMMGetCurrentLineNumber = (long) &WAMMGetCurrentLineNumber;
    AddInLib.WAMMGetCurrentColNumber = (long) &WAMMGetCurrentColNumber;
    AddInLib.WAMMGetCurrentLineText = (long) &WAMMGetCurrentLineText;
    AddInLib.WAMMGetLineText = (long) &WAMMGetLineText;
    AddInLib.WAMMWriteToOutputBox = (long) &WAMMWriteToOutputBox;
    AddInLib.WAMMEnumSourceWindows = (long) &WAMMEnumSourceWindows;
    AddInLib.WAMMAddInKillZombie = (long) &WAMMAddInKillZombie;
    AddInLib.WAMMSpAddInIsAvailable = (long) &WAMMSpAddInIsAvailable;
    AddInLib.WAMMMiscInputBox = (long) &WAMMMiscInputBox;
    AddInLib.WAMMIsChildReadOnly = (long) &IsChildReadOnly;
    AddInLib.WAMMSetChildReadOnly = (long) &SetChildReadOnly;
    AddInLib.WAMMCreateCustomChildDialog = (long) &Wrapper_WAMMCreateCustomChildDialog;
    AddInLib.WAMMCreateCodeMax = (long) &WAMMCreateCodeMax;
}

// -----------------------------------------------------------------------
// Create a stock dialog
long CALLBACK Wrapper_CreateStockModalDialog(long TemplateNumber, long DLeft, long DTop, long DWidth, long DHeight,
                                               HWND hParent, char *DTitle, DLGPROC WinProc, long Centered)
{
    int RetValue = 0;

    // We need to freeze our internal timer
    FreezeTimer = 1;
    switch(TemplateNumber)
    {
        case MODALDLG_STOCK_WIZARD:
            RetValue = CreateStockModalDialog(TemplateNumber, DLeft, DTop, DWidth, DHeight, hParent, DTitle, WinProc, Centered, (long) MAKEINTRESOURCE(MBMP_BASE + MBMP_WIZARD));
            break;
        default:
            RetValue = CreateStockModalDialog(TemplateNumber, DLeft, DTop, DWidth, DHeight, hParent, DTitle, WinProc, Centered, 0);
            break;
    }
    // And restart it
    FreezeTimer = 0;
    return(RetValue);
}

// -----------------------------------------------------------------------
// Chromatic AddIns Extra Services
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Get GUID from window
char * CALLBACK WAMMGetWindowLangGUID(HWND hForm)
{
    CStr ReturnValue;

    if(hForm != 0) ReturnValue = GetCMLangGUID(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get Include from window
char * CALLBACK WAMMGetWindowLangInclude(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) ReturnValue = GetCMLangInclude(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get Double slash from window
char * CALLBACK WAMMGetWindowLangDoubleSlash(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) ReturnValue = GetCMLangDoubleSlash(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get comment from window 
char * CALLBACK WAMMGetWindowLangComment(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) ReturnValue = GetCMLangComment(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get multi-comment from window
char * CALLBACK WAMMGetWindowLangMultiComment(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) ReturnValue = GetCMLangMultiComment(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get multi-comment end from window
char * CALLBACK WAMMGetWindowLangMultiCommentEnd(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) ReturnValue = GetCMLangMultiCommentEnd(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get proc from window
char * CALLBACK WAMMGetWindowLangProc(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) ReturnValue = GetCMLangProc(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get endp from window
char * CALLBACK WAMMGetWindowLangEndp(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) ReturnValue = GetCMLangEndp(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get interlace from window
long CALLBACK WAMMGetWindowLangProcInterlace(HWND hForm)
{
    if(hForm != 0) return(GetCMLangProcInterlace(hForm));
    else return(0);
}

// -----------------------------------------------------------------------
// Get orphan from window
long CALLBACK WAMMGetWindowLangProcOrphan(HWND hForm)
{
    if(hForm != 0) return(GetCMLangProcOrphan(hForm));
    else return(0);
}

// -----------------------------------------------------------------------
// Get openblock from window
char * CALLBACK WAMMGetWindowLangProcOpenBlock(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) ReturnValue = GetCMLangProcOpenBlock(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get closeblock from window
char * CALLBACK WAMMGetWindowLangProcCloseBlock(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) return(StringCStrConvertToString(GetCMLangProcCloseBlock(hForm)));
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get closeblock from window
char * CALLBACK WAMMGetWindowLangFileName(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) return(StringCStrConvertToString(GetCMLangFileName(hForm)));
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get include name from window
char * CALLBACK WAMMGetWindowLangIncludeName(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) return(StringCStrConvertToString(GetCMLangIncludeNameByhWnd(hForm)));
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Get include name from window
char * CALLBACK WAMMGetWindowLangIncludeNameFoot(HWND hForm)
{
    CStr ReturnValue;
    
    if(hForm != 0) return(StringCStrConvertToString(GetCMLangIncludeNameFootByhWnd(hForm)));
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Decode relative paths
char * CALLBACK WAMMDecodeRelativePaths(char *PathName)
{
    CStr ReturnValue;
    
    ReturnValue = StringConvertToCStr(PathName);
    if(strlen(PathName) != 0) ReturnValue = ChangeRelativePaths(ReturnValue);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Remove relative paths
char * CALLBACK WAMMRemoveRelativePaths(char *PathName)
{
    CStr ReturnValue;
    
    ReturnValue = StringConvertToCStr(PathName);
    if(strlen(PathName) != 0) ReturnValue = RemoveRelativePaths(ReturnValue);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Encode relative paths
char * CALLBACK WAMMEncodeRelativePaths(char *PathName)
{
    CStr ReturnValue;

    ReturnValue = StringConvertToCStr(PathName);
    if(strlen(PathName) != 0) ReturnValue = ChangeAbsolutePaths(ReturnValue);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Run an user command
void CALLBACK WAMMRunUserCommand(long CmdNumber)
{
    RunUserCommand(CmdNumber);
}

// -----------------------------------------------------------------------
// Run a script command
void CALLBACK WAMMRunCommandScript(char *CommandScript, long SaveFiles)
{
    CStr TmpCom;
    
    FRMArgsEntered = 0;
    if(SaveFiles == 1) MCMD_SaveAll();
    TmpCom = TranslateUserCmd(StringConvertToCStr(CommandScript));
    if(FRMArgsEntered == 1)
    {
        FRMArgsEntered = 0;
        return;
    }
    TmpCom = StringReplace(TmpCom, "\t", " ", 1, -1, Binary_Compare);
    Exec(&TmpCom, StringReplace(TmpCom, "$", " ", 1, -1, Binary_Compare), 0);
}

// -----------------------------------------------------------------------
// Display a color selector
long CALLBACK Wrapper_ComDlgChooseColor(HWND hWnd, long *ColorToGet)
{
    long ReturnValue = 0;

    if(ComDlgChooseColor(hWnd, ColorToGet[0]) != 0)
    {
        ColorToGet[0] = MyColor.rgbResult;
        ReturnValue = 1;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Display an open file selector
char * CALLBACK Wrapper_ComDlgGetOpenFileName(HWND hWnd, char *FilesFilter, char *PrimaryDir, long MultiSelect)
{
    CStr LdFile;
    CStr ReturnValue;

    LdFile = ComDlgGetOpenFileName(hWnd, StringConvertToCStr(FilesFilter), StringConvertToCStr(PrimaryDir), MultiSelect, CurrentDir);
    if(LdFile.Len() != 0) ReturnValue.Set_String(LdFile.Get_String());
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Display a multi files selector
char * CALLBACK Wrapper_ComDlgParseMultiFilesSelection(char *SelectedFiles, long (CALLBACK *EnumProc)(char *, long),
                                                         long SearchDirection, long UserValue)
{
    return(StringCStrConvertToString(ComDlgParseMultiFilesSelection(StringConvertToCStr(SelectedFiles), EnumProc, SearchDirection, UserValue)));
}

// -----------------------------------------------------------------------
// Display a save file selector
char * CALLBACK Wrapper_ComDlgGetSaveFileName(HWND hWnd, char *FilesFilter, char *PrimaryDir)
{
    CStr LdFile;
    CStr ReturnValue;

    LdFile = ComDlgGetSaveFileName(hWnd, StringConvertToCStr(FilesFilter), StringConvertToCStr(PrimaryDir), CurrentDir);
    if(LdFile.Len() != 0) ReturnValue = LdFile;
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Choose a font
long CALLBACK Wrapper_ComDlgChooseFont(HWND hWnd, char *FntName, long FntSize,
                                         long FixedOnly, LPLOGFONT WALogFont,
                                         LPCHOOSEFONT WAMyFont)
{
    int i = 0;
    HDC FhDC = 0;
    CStr LocFntName;
    long ReturnValue = 0;

    LocFntName = StringConvertToCStr(FntName);
    FhDC = GetDC(hWnd);
    WALogFont->lfHeight = -((long) (FntSize * GetDeviceCaps(FhDC, LOGPIXELSY)) / 72);
    WALogFont->lfWidth = 0;
    WALogFont->lfEscapement = 0;
    WALogFont->lfOrientation = 0;
    WALogFont->lfWeight = FW_NORMAL;
    WALogFont->lfItalic = 0;
    WALogFont->lfUnderline = 0;
    WALogFont->lfStrikeOut = 0;
    WALogFont->lfCharSet = DEFAULT_CHARSET;
    WALogFont->lfOutPrecision = OUT_DEFAULT_PRECIS;
    WALogFont->lfClipPrecision = CLIP_DEFAULT_PRECIS;
    WALogFont->lfQuality = DEFAULT_QUALITY;
    WALogFont->lfPitchAndFamily = DEFAULT_PITCH;
    for(i = 0; i <= LF_FACESIZE - 1; i++)
    {
        if(i == LocFntName.Len()) break;
        WALogFont->lfFaceName[i] = LocFntName.Mid(i + 1, 1).Asc();
    }
    WALogFont->lfFaceName[i] = 0;
    
    hMemFont = calloc(sizeof(MyLogFont), 1);
    if(hMemFont)
    {
        RtlCopyMemory(hMemFont, WALogFont, sizeof(MyLogFont));
        WAMyFont->lStructSize = sizeof(MyFont);
        WAMyFont->hwndOwner = hWnd;
        WAMyFont->lpLogFont = (LPLOGFONT) hMemFont;
        WAMyFont->iPointSize = FntSize * 10;
        if(FixedOnly == 1) WAMyFont->Flags = CF_FIXEDPITCHONLY | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_LIMITSIZE;
        else WAMyFont->Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_LIMITSIZE;
        WAMyFont->rgbColors = 0;
        WAMyFont->lCustData = 0;
        WAMyFont->lpfnHook = 0;
        WAMyFont->lpTemplateName = "";
        WAMyFont->hInstance = 0;
        WAMyFont->lpszStyle = "";
        WAMyFont->nFontType = REGULAR_FONTTYPE;
        WAMyFont->nSizeMin = 8;
        WAMyFont->nSizeMax = 72;
        ReturnValue = ChooseFont(WAMyFont);
        RtlCopyMemory(WALogFont, (void *) hMemFont, sizeof(MyLogFont));
        free(hMemFont);
    }
    ReleaseDC(hWnd, FhDC);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Browse directories
char * CALLBACK Wrapper_ComDlgBrowseForFolder(HWND hParent, char *BrowseTitle)
{
    CStr ReturnValue;

    ReturnValue = ComDlgBrowseForFolder(hParent, StringConvertToCStr(BrowseTitle));
    if(ReturnValue.Len() == 0)
    {
        return(0);
    }
    else
    {
        return(StringCStrConvertToString(ReturnValue));
    }
}

// -----------------------------------------------------------------------
// Load a .hlp/.chm/.col file
long CALLBACK WAMMDisplayHelpFile(char *HelpFile, char *HelpWord)
{
    return(OpenContextHelpFile(StringConvertToCStr(HelpFile), StringConvertToCStr(HelpWord)));
}

// -----------------------------------------------------------------------
// Run a DOS command
long CALLBACK WAMMRunDOSCmd(char *CmdName, char *CmdArgs, char *CmdDir, char *CmdOutputFile)
{
    return(RunDOSCmd(StringConvertToCStr(CmdName), StringConvertToCStr(CmdArgs), StringConvertToCStr(CmdDir),
                     StringConvertToCStr(CmdOutputFile)));
}

// -----------------------------------------------------------------------
// Run a DOS command and capture output
void CALLBACK WAMMRunDOSCmdCapture(char *CmdName, char *CmdArgs, char *CmdDir)
{
    CaptureDOSOutput(StringConvertToCStr(CmdName), StringConvertToCStr(CmdArgs), StringConvertToCStr(CmdDir));
}

// -----------------------------------------------------------------------
// Focus on a CodeMax control
void CALLBACK WAMMSetFocusInsideWindow(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    SetFocus(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Retrieve the path of the current child window
char * CALLBACK WAMMGetWindowPath(void)
{
    return(StringCStrConvertToString(GetCurrentFileDir()));
}

// -----------------------------------------------------------------------
// Check if a given filename is a workspace
long CALLBACK WAMMIsFileWorkSpace(char *FileName)
{
    return(CheckWorkSpaceFile(StringConvertToCStr(FileName)));
}

// -----------------------------------------------------------------------
// Check if a given filename is a project
long CALLBACK WAMMIsFileProject(char *FileName)
{
    return(CheckProjectFile(StringConvertToCStr(FileName)));
}

// -----------------------------------------------------------------------
// Retrieve the current state of the current child window
long CALLBACK WAMMGetCurrentChildState(void)
{
    return(GetChildState());
}

// -----------------------------------------------------------------------
// Goto a line number of a given filename
HWND CALLBACK GotoFileLine(char *FileName, long LineNbr, long AddInRecents)
{
    return(GoToLine(StringConvertToCStr(FileName), LineNbr, AddInRecents));
}

// -----------------------------------------------------------------------
// Focus on a given file
void CALLBACK WAMMFocusOnFile(char *FileName, long AddInRecents)
{
    FocusFile(StringConvertToCStr(FileName), AddInRecents);
}

// -----------------------------------------------------------------------
// Retrieve a filename from a treeview item
char * CALLBACK WAMMGetFileNameFromTreeViewItem(HTREEITEM hItem)
{
    return(StringCStrConvertToString(GetFileNameFromTreeView(hItem)));
}

// -----------------------------------------------------------------------
// Retrieve a label from a treeview item
char * CALLBACK WAMMGetLabelFromTreeViewItem(HTREEITEM hItem)
{
    return(StringCStrConvertToString(GetLabelFromTreeView(hItem)));
}

// -----------------------------------------------------------------------
// Open an alien file
long CALLBACK WAMMOpenUnknownFile(char *FileName, int AddInRecent)
{
    return(OpenUnknownFile(StringConvertToCStr(FileName), AddInRecent));
}

// -----------------------------------------------------------------------
// Convert a hexadecimal number into decimal
char * CALLBACK WAMMHexToDec(char *Number)
{
    LPCHILDDAT ChildStruct = LoadStructure(CurrentForm);
    return(StringCStrConvertToString(HexToDec(ChildStruct->hChildCodeMax,
                                              StringConvertToCStr(Number))));
}

// -----------------------------------------------------------------------
// Convert a decimal number into hexadecimal
char * CALLBACK WAMMDecToHex(char *Number)
{
    LPCHILDDAT ChildStruct = LoadStructure(CurrentForm);
    return(StringCStrConvertToString(DecToHex(ChildStruct->hChildCodeMax,
                                              StringConvertToCStr(Number))));
}

// -----------------------------------------------------------------------
// Convert a decimal number into binary
char * CALLBACK WAMMDecToBin(char *Number)
{
    LPCHILDDAT ChildStruct = LoadStructure(CurrentForm);
    return(StringCStrConvertToString(DecToBin(ChildStruct->hChildCodeMax,
                                              StringConvertToCStr(Number))));
}

// -----------------------------------------------------------------------
// Convert a hexadecimal number into binary
char * CALLBACK WAMMHexToBin(char *Number)
{
    LPCHILDDAT ChildStruct = LoadStructure(CurrentForm);
    return(StringCStrConvertToString(HexToBin(ChildStruct->hChildCodeMax,
                                              StringConvertToCStr(Number))));
}

// -----------------------------------------------------------------------
// Convert a binary number into decimal
char * CALLBACK WAMMBinToDec(char *Number)
{
    LPCHILDDAT ChildStruct = LoadStructure(CurrentForm);
    return(StringCStrConvertToString(BinToDec(ChildStruct->hChildCodeMax,
                                              StringConvertToCStr(Number))));
}

// -----------------------------------------------------------------------
// Convert a binary number into hexadecimal
char * CALLBACK WAMMBinToHex(char *Number)
{
    LPCHILDDAT ChildStruct = LoadStructure(CurrentForm);
    return(StringCStrConvertToString(BinToHex(ChildStruct->hChildCodeMax,
                                              StringConvertToCStr(Number))));
}

// -----------------------------------------------------------------------
// Convert a hexadecimal number into a floating point
char * CALLBACK WAMMHexToFlt(char *Number)
{
    LPCHILDDAT ChildStruct = LoadStructure(CurrentForm);
    return(StringCStrConvertToString(HexToFlt(ChildStruct->hChildCodeMax,
                                              StringConvertToCStr(Number))));
}

// -----------------------------------------------------------------------
// Convert a floating point number into hexadecimal
char * CALLBACK WAMMFltToHex(char *Number)
{
    LPCHILDDAT ChildStruct = LoadStructure(CurrentForm);
    return(StringCStrConvertToString(FltToHex(ChildStruct->hChildCodeMax,
                                              StringConvertToCStr(Number))));
}

// -----------------------------------------------------------------------
// Retrieve the procedure list contained in a file
char * CALLBACK WAMMGetProceduresList(char *FileName)
{
    return(StringCStrConvertToString(GetProcedureList(StringConvertToCStr(FileName))));
}

// -----------------------------------------------------------------------
// Retrieve a text line from a file
char * CALLBACK WAMMGetLineFromFile(HANDLE hFile, long LineNumber)
{
    CStr ReturnValue;
    long FlSize = 0;

    if(hFile != 0)
    {
        FlSize = GetFileSize(hFile, NULL);
        ReturnValue = GetTxtLineFromFile((long) hFile, FlSize, LineNumber);
    }
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Create a new window
HWND CALLBACK WAMMCreateNewWindow(void)
{
    return(MCMD_New());
}

// -----------------------------------------------------------------------
// Clear a window
void CALLBACK WAMMClearWindow(HWND hForm)
{
    if(hForm != 0) ClearFile(hForm);
}

// -----------------------------------------------------------------------
// Reload a window
void CALLBACK WAMMReloadWindow(HWND hForm)
{
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(ChildStruct->FileLoaded == 1)
        {
            ClearFile(hForm);
            LoadFile(hForm);
        }
    }
}

// -----------------------------------------------------------------------
// Close a window
void CALLBACK WAMMCloseWindow(HWND hForm)
{
    if(NbForms != 0) if(hForm != 0) ControlClose(hForm);
    if(NbForms != 0) ClientSetPreviousChild(hMDIform.hClient);
}

// -----------------------------------------------------------------------
// Close all windows
void CALLBACK WAMMCloseAllWindows(void)
{
    MCMD_CloseAll();
}

// -----------------------------------------------------------------------
// Save all windows
void CALLBACK WAMMSaveAllWindows(void)
{
    MCMD_SaveAll();
}
    
// -----------------------------------------------------------------------
// Print a window
void CALLBACK WAMMPrintWindow(HWND hForm, long PrintSelection)
{
    if(NbForms != 0) if(hForm != 0) PrintFile(hForm, PrintSelection);
}

// -----------------------------------------------------------------------
// Save a window
char * CALLBACK WAMMSaveWindow(HWND hForm)
{
    CStr ReturnValue;

    if(NbForms != 0) if(hForm != 0) ReturnValue = SaveIt(hForm);
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Undo typing
void CALLBACK WAMMUndo(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Undo(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Redo typing
void CALLBACK WAMMRedo(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Redo(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Cut command
void CALLBACK WAMMCut(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Cut(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Copy command
void CALLBACK WAMMCopy(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Copy(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Copy filepathname command
long CALLBACK WAMMCopyFilePathName(HWND hForm)
{
    long ReturnValue = 0;

    if(NbForms == 0) return(0);
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(ChildStruct->FileLoaded == 0) return(ReturnValue);
        ReturnValue = MiscClipBoardCopyText(CMGetRealFile(ChildStruct->RFile));
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Copy entire text command
long CALLBACK WAMMCopyEntireText(HWND hForm)
{
    CStr EntText;
    long ReturnValue = 0;

    if(NbForms == 0) return(0);
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        EntText = EntText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
        CM_GetTextAll(ChildStruct->hChildCodeMax, EntText.Get_String());
        ReturnValue = MiscClipBoardCopyText(EntText);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Paste command
void CALLBACK WAMMPaste(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_Paste(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Delete selection command
void CALLBACK WAMMDeleteSel(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_DeleteSel(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Delete line command
void CALLBACK WAMMDeleteLine(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        LoadCurrentSel(ChildStruct->hChildCodeMax);
        CM_DeleteLine(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine);
    }
}

// -----------------------------------------------------------------------
// Delete to start of line command
void CALLBACK WAMMDeleteToStart(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDELETETOSTART, -1);
    }
}

// -----------------------------------------------------------------------
// Delete to end of line command
void CALLBACK WAMMDeleteToEnd(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDELETETOEND, 0);
    }
}

// -----------------------------------------------------------------------
// Select line command
void CALLBACK WAMMSelectLine(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SELECTLINE, 0);
    }
}

// -----------------------------------------------------------------------
// Select all command
void CALLBACK WAMMSelectAll(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SELECTALL, 0);
    }
}

// -----------------------------------------------------------------------
// Select a procedure in current window
long CALLBACK WAMMSelectProc(HWND hForm)
{
    if(NbForms == 0) if(hForm != 0) return(SelectProcedure(hForm));
    return(0);
}

// -----------------------------------------------------------------------
// Find next
void CALLBACK WAMMFindNext(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDNEXT, 0);
    }
}

// -----------------------------------------------------------------------
// find previous
void CALLBACK WAMMFindPrevious(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDPREV, 0);
    }
}

// -----------------------------------------------------------------------
// Find selection
void CALLBACK WAMMFindSelection(HWND hForm)
{
    CStr TxtToFind;

    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(GetCurrentLogicalSelLen(ChildStruct->hChildCodeMax) == 0)
        {
            TxtToFind = GetCurrentWord(ChildStruct->hChildCodeMax);
        }
        else
        {
            TxtToFind = GetCurrentLogicalSelText(ChildStruct->hChildCodeMax);
        }
        CM_SetFindText(ChildStruct->hChildCodeMax, TxtToFind.Get_String());
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDNEXT, 0);
    }
}

// -----------------------------------------------------------------------
// Goto top
void CALLBACK GotoTop(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, 0);
    }
}

// -----------------------------------------------------------------------
// Goto bottom
void CALLBACK GotoBottom(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, CM_GetLineCount(ChildStruct->hChildCodeMax) - 1);
    }
}

// -----------------------------------------------------------------------
// Goto line
void CALLBACK GotoLine(HWND hForm, long LineNumber)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, LineNumber);
    }
}

// -----------------------------------------------------------------------
// Goto matching bracket
void CALLBACK GotoMatchingBracket(HWND hForm)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOMATCHBRACE, 0);
    }
}

// -----------------------------------------------------------------------
// Goto previous word
void CALLBACK GotoPrevWord(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDLEFTEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto next word
void CALLBACK GotoNextWord(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDRIGHT, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDRIGHTEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto previous letter
void CALLBACK GotoPrevLetter(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_CHARLEFTEXTEND, 0);
        }
    }
} 

// -----------------------------------------------------------------------
// Goto next letter
void CALLBACK GotoNextLetter(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_CHARRIGHT, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_CHARRIGHTEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto previous line
void CALLBACK GotoPrevLine(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEUP, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEUPEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto next line
void CALLBACK GotoNextLine(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDOWN, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDOWNEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto previous page
void CALLBACK GotoPrevPage(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_PAGEUP, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_PAGEUPEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Goto next page
void CALLBACK GotoNextPage(HWND hForm, long Selection)
{
    if(NbForms == 0) return;
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
        if(Selection == 0)
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_PAGEDOWN, 0);
        }
        else
        {
            CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_PAGEDOWNEXTEND, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Get Mini child structure
void CALLBACK WAMMGetMiniChildStruct(HWND hForm, LPMINICHILDDAT MiniChildStruct)
{
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        MiniChildStruct->hChildCodeMax = ChildStruct->hChildCodeMax;
        MiniChildStruct->FileLoaded = ChildStruct->FileLoaded;
        if(ChildStruct->Language->Len() == 0)
        {
            MiniChildStruct->Language = "";
        }
        else
        {
            MiniChildStruct->Language = StringCStrConvertToString(ChildStruct->Language);
        }
        MiniChildStruct->ModifFlag = ChildStruct->ModifFlag;
        if(ChildStruct->ReadOnly == 1)
        {
            MiniChildStruct->ReadOnly = 1;
        }
        else
        {
            MiniChildStruct->ReadOnly = 0;
        }
        if(ChildStruct->Language->Len() == 0)
        {
            MiniChildStruct->FileName = "";
        }
        else
        {
            MiniChildStruct->FileName = StringCStrConvertToString(ChildStruct->RFile);
        }
    }
}

// -----------------------------------------------------------------------
// Retrieve the filename of an ini file
char * CALLBACK WAMMGetIniFilePathName(long FileNumber)
{
    CStr BufString;
    CStr ReturnValue;

    switch(FileNumber)
    {
        case INIFILE_MAIN:
            BufString = MainIniFile;
            break;
        case INIFILE_CURRENTUSERMENUS:
            BufString = UserMenusIniFile;
            break;
        case INIFILE_TOOLBARS:
            BufString = ToolbarsIniFile;
            break;
        case INIFILE_COLORS:
            BufString = ColorsIniFile;
            break;
        case INIFILE_FTPACCOUNTS:
            BufString = FtpAccountsIniFile;
            break;
        case INIFILE_SKINS:
            BufString = SkinsIniFile;
            break;
        case INIFILE_PROJECTSTYPES:
            BufString = ProjectsIniFile;
            break;
        case INIFILE_LANGUAGES:
            BufString = LanguagesIniFile;
            break;
        case INIFILE_DATES:
            BufString = DatesIniFile;
            break;
        case INIFILE_CGREP:
            BufString = cGrepIniFile;
            break;
        case INIFILE_FILTERS:
            BufString = FiltersIniFile;
            break;
        default:
            BufString = "";
            break;
    }
    ReturnValue = BufString;
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Retrieve a stock directory
char * CALLBACK WAMMGetQuickDirectory(long DirNumber)
{
    switch(DirNumber)
    {
        case DIR_ROOT:
            return(Dirs[DirNumber]);
        case DIR_BIN:
            return(Dirs[DirNumber]);
        case DIR_INCLUDE:
            return(Dirs[DirNumber]);
        case DIR_LIB:
            return(Dirs[DirNumber]);
        case DIR_HELP:
            return(Dirs[DirNumber]);
        case DIR_MAIN:
            return(Dirs[DirNumber]);
        case DIR_WIN:
            return(Dirs[DirNumber]);
        case DIR_WINSYS:
            return(Dirs[DirNumber]);
        case DIR_TEMP:
            return(Dirs[DirNumber]);
        case DIR_TEMPLATES:
            return(Dirs[DirNumber]);
        case DIR_PROJECTS:
            return(Dirs[DirNumber]);
        case DIR_MAINHELP:
            return(Dirs[DirNumber]);
        case DIR_SNIPPETS:
            return(Dirs[DirNumber]);
        case DIR_DATABASES:
            return(Dirs[DirNumber]);
        case DIR_SKINS:
            return(Dirs[DirNumber]);
        case DIR_LANGUAGES:
            return(Dirs[DirNumber]);
        case DIR_DOWNLOADS:
            return(Dirs[DirNumber]);
        case DIR_ADDINS:
            return(Dirs[DirNumber]);
        case DIR_CONFIG:
            return(Dirs[DirNumber]);
        case DIR_MENUS:
            return(Dirs[DirNumber]);
        case DIR_WIZARDS:
            return(Dirs[DirNumber]);
        case DIR_FILTERS:
            return(Dirs[DirNumber]);
        case DIR_EXTRACODE:
            return(Dirs[DirNumber]);
        case DIR_LANGSCRIPTS:
            return(Dirs[DirNumber]);
        case DIR_TOOLS:
            return(Dirs[DirNumber]);
        case DIR_SCRIPTS:
            return(Dirs[DirNumber]);
        default:
            return("");
    }
}

// -----------------------------------------------------------------------
// Fill main context
void CALLBACK WAMMGetContext(LPADDINCONTEXT VarsStruct)
{
    VarsStruct->Version = AppRevMin | (AppRevMaj << 8) | (AppVersion << 12);
    VarsStruct->NbrChilds = NbForms;
    VarsStruct->hInstance = ApphInstance;
    VarsStruct->ActiveProject = ProjectOn;

    VarsStruct->OSType = OSTYPE_WINDOWS_9X;
    if(WindowsNT) VarsStruct->OSType = OSTYPE_WINDOWS_NT;
    if(Windows2K) VarsStruct->OSType = OSTYPE_WINDOWS_2K;
    if(WindowsXP) VarsStruct->OSType = OSTYPE_WINDOWS_XP;
    if(Windows7) VarsStruct->OSType = OSTYPE_WINDOWS_7;

    if(FullScreenMode == 1) VarsStruct->FullScreenMode = 1;
    else VarsStruct->FullScreenMode = 0;

    if(SysTrayMode == 1) VarsStruct->SysTrayMode = 1;
    else VarsStruct->SysTrayMode = 0;

    VarsStruct->hMDI = hMDIform.hWnd;
    VarsStruct->hClient = hMDIform.hClient;
    VarsStruct->hAccelerators = hGlobAccelerators;
    
    if(NbForms == 0)
    {
        VarsStruct->hCurrentChild = 0;
        VarsStruct->hCurrentCodeMax = 0;
    }
    else
    {
        VarsStruct->hCurrentChild = CurrentForm;
        ChildStruct = LoadStructure(CurrentForm);
        VarsStruct->hCurrentCodeMax = ChildStruct->hChildCodeMax;
    }
    VarsStruct->hRebar = hRebar;
    VarsStruct->hTreeView = hTreeView;
    VarsStruct->hWindowsList = hWindowsListView;
    VarsStruct->hStatusBar = hStatusBar;
    VarsStruct->hOutputBox = hStatusBox;
    VarsStruct->hDebugBox = hDebugBox;
    VarsStruct->hOutputSysTab = hOutputSysTab;
    VarsStruct->hEmptyImageList = GlobalImageList3;
    VarsStruct->hMainImageList = GlobalImageList1;
    VarsStruct->hLittleImageList = GlobalImageList2;
    VarsStruct->hControlsImageList = GlobalImageList4;
    VarsStruct->hDialogEdImageList = GlobalImageList5;
    VarsStruct->MDIhWndProc = CurrentMDIProc;
    VarsStruct->ChildCreateProc = CurrentMDIChildInitProc;
    VarsStruct->ChildhWndProc = CurrentMDIChildProc;
    VarsStruct->CodeMaxhWndProc = CurrentCodeMaxProc;
    VarsStruct->hProjectToolbar = hProjectToolbar;
    VarsStruct->hProjectPager = hProjectPager;
    VarsStruct->hWindowsToolbar = hWindowsToolbar;
    VarsStruct->hWindowsPager = hWindowsPager;
    VarsStruct->hOutputBoxToolbar = hStatusToolBar;
    VarsStruct->hOutputBoxPager = hStatusPager;
    VarsStruct->ColdStart = FirstStart;
}

// -----------------------------------------------------------------------
// Fill main context
int CALLBACK WAMMGetContextElement(int Idx)
{
    switch(Idx)
    {
        case 0:
            return(0);

        case 1:
            return(AppRevMin | (AppRevMaj << 8) | (AppVersion << 12));

        case 2:
            return(NbForms);

        case 3:
            return(ProjectOn);

        case 4:
            return((int) ApphInstance);

        case 5:
            if(WindowsNT) return(OSTYPE_WINDOWS_NT);
            if(Windows2K) return(OSTYPE_WINDOWS_2K);
            if(WindowsXP) return(OSTYPE_WINDOWS_XP);
            if(Windows7) return(OSTYPE_WINDOWS_7);
            else return(OSTYPE_WINDOWS_9X);

        case 6:
            if(FullScreenMode == 1) return(1);
            else return(0);

        case 7:
            if(SysTrayMode == 1) return(1);
            else return(0);

        case 8:
            return((int) hMDIform.hWnd);

        case 9:
            return((int) hMDIform.hClient);

        case 10:
            return((int) hGlobAccelerators);
    
        case 11:
            return((int) hTreeView);

        case 12:
            return((int) hWindowsListView);

        case 13:
            return((int) hStatusBar);

        case 14:
            return((int) hStatusBox);

        case 15:
            return((int) hDebugBox);

        case 16:
            return((int) hOutputSysTab);

        case 17:
            if(NbForms == 0) {
                return(0);
            } else {
                return((int) CurrentForm);
            }
            break;

        case 18:
            if(NbForms == 0)
            {
                return(0);
            }
            else
            {
                ChildStruct = LoadStructure(CurrentForm);
                return((int) ChildStruct->hChildCodeMax);
            }
            break;

        case 19:
            return((int) hRebar);

        case 20:
            return((int) GlobalImageList3);

        case 21:
            return((int) GlobalImageList1);

        case 22:
            return((int) GlobalImageList2);

        case 23:
            return((int) GlobalImageList4);

        case 24:
            return((int) GlobalImageList5);

        case 25:
            return((int) CurrentMDIProc);

        case 26:
            return((int) CurrentMDIChildInitProc);

        case 27:
            return((int) CurrentMDIChildProc);

        case 28:
            return((int) CurrentCodeMaxProc);

        case 29:
            return((int) hProjectToolbar);

        case 30:
            return((int) hProjectPager);

        case 31:
            return((int) hWindowsToolbar);

        case 32:
            return((int) hWindowsPager);

        case 33:
            return((int) hStatusToolBar);

        case 34:
            return((int) hStatusPager);

        case 35:
            return(FirstStart);

        default:
            return(0);
    }
}

// -----------------------------------------------------------------------
// Get current line
char * CALLBACK WAMMGetcurrentLine(HWND hForm)
{
    CStr ReturnValue;
    
    if(NbForms == 0) return(StringCStrConvertToString(ReturnValue));
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        ReturnValue = RetrieveCurrentLine(ChildStruct->hChildCodeMax);
    }
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Select current word
char * CALLBACK WAMMSelectCurrentWord(HWND hForm)
{
    CStr ReturnValue;

    if(NbForms == 0) return(StringCStrConvertToString(ReturnValue));
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        ReturnValue = SelectCurrentWord(ChildStruct->hChildCodeMax);
    }
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Retrieve current word
char * CALLBACK WAMMGetCurrentWord(HWND hForm)
{
    CStr ReturnValue;
    
    if(NbForms == 0) return(StringCStrConvertToString(ReturnValue));
    if(hForm != 0)
    {
        ChildStruct = LoadStructure(hForm);
        ReturnValue = GetCurrentWord(ChildStruct->hChildCodeMax);
    }
    return(StringCStrConvertToString(ReturnValue));
}

// -----------------------------------------------------------------------
// Insert a text at current position
void CALLBACK InsertText(HWND hForm, char *Txt)
{
    if(NbForms == 0) return;
    if(hForm != 0) SmartInsert(hForm, StringConvertToCStr(Txt));
}

// -----------------------------------------------------------------------
// Insert a text at current position and jump to next line
void CALLBACK WAMMInsertLine(HWND hForm, char *Txt)
{
    if(NbForms == 0) return;
    if(hForm != 0) SmartInsertLine(hForm, StringConvertToCStr(Txt));
}

// -----------------------------------------------------------------------
// Insert a file at current position
void CALLBACK WAMMInsertLines(HWND hForm, long Txt)
{
    if(NbForms == 0) return;
    if(hForm != 0) SmartInsertFile(hForm, Txt);
}

// -----------------------------------------------------------------------
// Set the text in a window
void CALLBACK WAMMSetText(HWND hForm, char *Txt)
{
    if(NbForms == 0) return;
    if(hForm != 0) SendTextToChild(hForm, StringConvertToCStr(Txt));
}

// -----------------------------------------------------------------------
// Restore the MDI form
void CALLBACK WAMMRestoreMDIFromSysTray(void) {
    SysTrayMode = 0;
    RestoreMDI(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Hook chaining
long CALLBACK WAMMInstallHookProc(long HookAddress, long HookType)
{
    int i = 0;
    long FoundHook = 0;

    switch(HookType)
    {
        case HOOK_MDICREATE:
            for(i = 0; i < MDICreateHooks.Amount(); i++)
            {
                if(MDICreateHooks.Get(i)->Content == 0)
                {
                    if((MDICreateHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
                MDICreateHooks.Set(i, HookAddress);
                MDICreateHooksNbr.Set(i, HookNbr);
            }
            else
            {
                // Obtain a new hooking slot
                MDICreateHooks.Add(0L);
                MDICreateHooksNbr.Add(0L);
            }
            break;
        case HOOK_MDI:
            for(i = 0; i < MDIHooks.Amount(); i++)
            {
                if(MDIHooks.Get(i)->Content == 0)
                {
                    if((MDIHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
                MDIHooks.Set(i, (DLGPROC) HookAddress);
                MDIHooksNbr.Set(i, HookNbr);
            }
            else
            {
                // Obtain a new hooking slot
                MDIHooks.Add((DLGPROC) 0L);
                MDIHooksNbr.Add(0L);
            }
            break;
        case HOOK_MDICLIENT:
            for(i = 0; i < MDIClientHooks.Amount(); i++)
            {
                if(MDIClientHooks.Get(i)->Content == 0)
                {
                    if((MDIClientHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
                MDIClientHooks.Set(i, (DLGPROC) HookAddress);
                MDIClientHooksNbr.Set(i, HookNbr);
            }
            else
            {
                // Obtain a new hooking slot
                MDIClientHooks.Add((DLGPROC) 0L);
                MDIClientHooksNbr.Add(0L);
            }
            break;
        case HOOK_CHILDCREATE:
            for(i = 0; i < ChildCreateHooks.Amount(); i++)
            {
                if(ChildCreateHooks.Get(i)->Content == 0)
                {
                    if((ChildCreateHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
                ChildCreateHooks.Set(i, HookAddress);
                ChildCreateHooksNbr.Set(i, HookNbr);
            }
            else
            {
                ChildCreateHooks.Add(0L);
                ChildCreateHooksNbr.Add(0L);
            }
            break;
        case HOOK_CHILD:
            for(i = 0; i < ChildHooks.Amount(); i++)
            {
                if(ChildHooks.Get(i)->Content == 0)
                {
                    if((ChildHooks.Amount() - 1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
                ChildHooks.Set(i, (DLGPROC) HookAddress);
                ChildHooksNbr.Set(i, HookNbr);
            }
            else
            {
                ChildHooks.Add((DLGPROC) 0L);
                ChildHooksNbr.Add(0L);
            }
            break;
        case HOOK_CMAXXX:
            for(i = 0; i < CodeMaxHooks.Amount(); i++)
            {
                if(CodeMaxHooks.Get(i)->Content == 0)
                {
                    if((CodeMaxHooks.Amount() -1) == i) FoundHook = 1;
                    break;
                }
            }
            HookNbr++;
            if(FoundHook == 1)
            {
                CodeMaxHooks.Set(i, (DLGPROC) HookAddress);
                CodeMaxHooksNbr.Set(i, HookNbr);
            }
            else
            {
                CodeMaxHooks.Add((DLGPROC) 0L);
                CodeMaxHooksNbr.Add(0L);
            }
            break;
    }
    return(HookNbr);
}

void CALLBACK WAMMUnInstallHookProc(long HookAddress, long HookType, long HookNumber)
{
    int i = 0;
    long FoundHook = 0;

    switch(HookType)
    {
        case HOOK_MDICREATE:
            for(i = 0; i < MDICreateHooks.Amount(); i++)
            {
                if(MDICreateHooks.Get(i)->Content == HookAddress)
                {
                    // Good hook number ?
                    if(MDICreateHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) MDICreateHooks.Set(i, 0L);
            break;
        case HOOK_MDI:
            for(i = 0; i < MDIHooks.Amount(); i++)
            {
                if(MDIHooks.Get(i)->Content == (DLGPROC) HookAddress)
                {
                    // Good hook number ?
                    if(MDIHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) MDIHooks.Set(i, 0L);
            break;
        case HOOK_MDICLIENT:
            for(i = 0; i < MDIClientHooks.Amount(); i++)
            {
                if(MDIClientHooks.Get(i)->Content == (DLGPROC) HookAddress)
                {
                    // Good hook number ?
                    if(MDIClientHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) MDIClientHooks.Set(i, 0L);
            break;
        case HOOK_CHILDCREATE:
            for(i = 0; i < ChildCreateHooks.Amount(); i++)
            {
                if(ChildCreateHooks.Get(i)->Content == HookAddress)
                {
                    if(ChildCreateHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) ChildCreateHooks.Set(i, 0L);
            break;
        case HOOK_CHILD:
            for(i = 0; i < ChildHooks.Amount(); i++)
            {
                if(ChildHooks.Get(i)->Content == (DLGPROC) HookAddress)
                {
                    if(ChildHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) ChildHooks.Set(i, 0L);
            break;
        case HOOK_CMAXXX:
            for(i = 0; i < CodeMaxHooks.Amount(); i++)
            {
                if(CodeMaxHooks.Get(i)->Content == (DLGPROC) HookAddress)
                {
                    if(CodeMaxHooksNbr.Get(i)->Content == HookNumber)
                    {
                        FoundHook = 1;
                        break;
                    }
                }
            }
            if(FoundHook == 1) CodeMaxHooks.Set(i, 0L);
            break;
    }
}

// -----------------------------------------------------------------------
// Retrieve the current line of a window
long CALLBACK WAMMGetCurrentLineNumber(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    return(CodeMaxCurRange.posStart.nLine);
}

// -----------------------------------------------------------------------
// Retrieve the current col of a window
long CALLBACK WAMMGetCurrentColNumber(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    return(CodeMaxCurRange.posStart.nCol);
}

// -----------------------------------------------------------------------
// Retrieve the current line text of a window
char * CALLBACK WAMMGetCurrentLineText(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    return(StringCStrConvertToString(CMGetCurrentLine(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine)));
}

// -----------------------------------------------------------------------
// Retrieve the current line text of a window
char * CALLBACK WAMMGetLineText(HWND hWnd, long Line)
{
    ChildStruct = LoadStructure(hWnd);
    return(StringCStrConvertToString(CMGetCurrentLine(ChildStruct->hChildCodeMax, Line)));
}

// -----------------------------------------------------------------------
// Write a text into output box
void CALLBACK WAMMWriteToOutputBox(char *WAText)
{
    WriteToStatus(StringConvertToCStr(WAText));
}

// -----------------------------------------------------------------------
// Enumerate the childs windows
void CALLBACK WAMMEnumSourceWindows(long CallBack)
{
    if(CallBack != 0)
    {
        if(NbForms != 0) EnumChildWindows(hMDIform.hWnd, &EnumChilds, CallBack);
    }
}
                    
int CALLBACK EnumChilds(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(FALSE);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 0) goto NoEnumChildsProc;
        if(GetWindowLong(hWnd, GWL_USERDATA) == 0) goto NoEnumChildsProc;
        if(lParam != 0) return(JumpToAddrArg((FARPROC) lParam, (long) hWnd));
NoEnumChildsProc:
        return(TRUE);
    }
}

// -----------------------------------------------------------------------
// An AddIn requested a kill
void CALLBACK WAMMAddInKillZombie(long CallBack)
{
    int i = 0;
    long FoundAddInProc = 0;

    if(CallBack == 0) return;
    // Search Unload routine address
    for(i = 0; i < AddInsFiles.Amount(); i++)
    {
        if(AddInsUnLoad.Get(i)->Content == (FARPROC) CallBack)
        {
            FoundAddInProc = 1;
            break;
        }
    }
    if(FoundAddInProc == 0) return;
    if(AddInsUnLoad.Get(i)->Content != 0)
    {
        JumpToAddr(AddInsUnLoad.Get(i)->Content);
    }
    RunningAddIns.Set(i, 0L);
    IniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(i, 3).Get_String(), RunningAddIns.Get(i)->Content, MainIniFile);
    // Change icon
    if(FRMAddInsListView != 0) ListViewSetSubItemImage(FRMAddInsListView, ICON_ADDINR, i, 0);
}
                    
// -----------------------------------------------------------------------
// Check if a DLL is a support AddIn
long CALLBACK WAMMSpAddInIsAvailable(char *AlienDllName)
{
    CStr BufString;

    BufString = Dirs[DIR_ADDINS] + (CStr) "\\" + (CStr) StringConvertToCStr(AlienDllName);
    return(IsSpAddIn(BufString));
}

// -----------------------------------------------------------------------
// Chromatic standard inputbox
char * CALLBACK WAMMMiscInputBox(HWND hParent, char *NameTitle, long Icon, char *ValueToPass,
                                 long PromptLen, long PromptType, char *PromptIniKey)
{
    return(StringCStrConvertToString(MiscInputBox(hParent, StringConvertToCStr(NameTitle), Icon, StringConvertToCStr(ValueToPass), PromptLen, PromptType, StringConvertToCStr(PromptIniKey))));
}

// -----------------------------------------------------------------------
// WA Library wrappers for wide strings conversions
// -----------------------------------------------------------------------
    
long CALLBACK Wrapper_MenuSetItemText(HMENU hMenu, char *Wrapper_ItemText, long ItemID)
{
    return(MenuSetItemText(hMenu, StringConvertToCStr(Wrapper_ItemText), ItemID));
}

long CALLBACK Wrapper_MenuAddString(HMENU hMenu, char *Wrapper_ItemText, long ItemID, long Enabled)
{
    return(MenuAddString(hMenu, StringConvertToCStr(Wrapper_ItemText), ItemID, Enabled));
}

long CALLBACK Wrapper_MiscClipBoardCopyText(char *Wrapper_WATxt)
{
    return(MiscClipBoardCopyText(StringConvertToCStr(Wrapper_WATxt)));
}

char * CALLBACK Wrapper_MiscClipBoardPasteText(void)
{
    return(StringCStrConvertToString(MiscClipBoardPasteText()));
}

long CALLBACK Wrapper_ComboBoxAddItem(HWND Wrapper_WAhCB, char *Wrapper_WACBItemText, long Wrapper_WACBItemIndex)
{
    return(ComboBoxAddItem(Wrapper_WAhCB, StringConvertToCStr(Wrapper_WACBItemText), Wrapper_WACBItemIndex));
}

char * CALLBACK Wrapper_ComboBoxGetCurrentItem(HWND Wrapper_WAhCB)
{
    return(StringCStrConvertToString(ComboBoxGetCurrentItem(Wrapper_WAhCB)));
}

char * CALLBACK Wrapper_ComboBoxGetItem(HWND Wrapper_WAhCB, long Wrapper_WACBIndex)
{
    return(StringCStrConvertToString(ComboBoxGetItem(Wrapper_WAhCB, Wrapper_WACBIndex)));
}

long CALLBACK Wrapper_ComboBoxItemExist(HWND Wrapper_WAhCB, char *Wrapper_WACBItem)
{
    return(ComboBoxItemExist(Wrapper_WAhCB, StringConvertToCStr(Wrapper_WACBItem)));
}

void CALLBACK Wrapper_ComboBoxSaveInIniFile(HWND Wrapper_WAhCB, char *EntryToAdd, char *IniKey, char *IniFile)
{
    ComboBoxSaveInIniFile(Wrapper_WAhCB, StringConvertToCStr(EntryToAdd), StringConvertToCStr(IniKey), StringConvertToCStr(IniFile));
    return;
}

void CALLBACK Wrapper_ComboBoxFillFromIniFile(HWND Wrapper_WAhCB, char *IniKey, char *IniFile)
{
    ComboBoxFillFromIniFile(Wrapper_WAhCB, StringConvertToCStr(IniKey), StringConvertToCStr(IniFile));
    return;
}

char * CALLBACK Wrapper_ControlGetClassName(HWND Wrapper_WAhWnd)
{
    return(StringCStrConvertToString(ControlGetClassName(Wrapper_WAhWnd)));
}

char * CALLBACK Wrapper_ControlGetText(HWND Wrapper_WAhWnd)
{
    return(StringCStrConvertToString(ControlGetText(Wrapper_WAhWnd)));
}

long CALLBACK Wrapper_ControlSetText(HWND Wrapper_WAhWnd, char *WAATextToSet)
{
    return(ControlSetText(Wrapper_WAhWnd, StringConvertToCStr(WAATextToSet)));
}

HWND CALLBACK Wrapper_CreateButton(long Wrapper_WABLeft, long Wrapper_WABTop, long Wrapper_WABWidth, long Wrapper_WABHeight,
                                     HWND Wrapper_WAhParent, char *Wrapper_WABText, long Wrapper_WACtrlID, long Wrapper_WAImgType,
                                     HICON Wrapper_WAhImage, WNDPROC Wrapper_WAWinProc, long Wrapper_WAExtraStyle, long Wrapper_WAExtraExStyle)
{
    return(CreateButton(Wrapper_WABLeft, Wrapper_WABTop, Wrapper_WABWidth, Wrapper_WABHeight, Wrapper_WAhParent, StringConvertToCStr(Wrapper_WABText), Wrapper_WACtrlID, Wrapper_WAImgType, Wrapper_WAhImage, Wrapper_WAWinProc, Wrapper_WAExtraStyle, Wrapper_WAExtraExStyle | Buttons_StaticEdge));
}

long CALLBACK Wrapper_MiscMsgBox(HWND WAHParent, char *WAMBText, long WAMBType)
{
    return(MiscMsgBox(WAHParent, StringConvertToCStr(WAMBText), WAMBType, Requesters));
}

HWND CALLBACK Wrapper_CreateCheckBox(long WABLeft, long WABTop, long WABWidth, long WABHeight, HWND WAHParent, char *WABText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle, long WAUnderState)
{
    return(CreateCheckBox(WABLeft, WABTop, WABWidth, WABHeight, WAHParent, StringConvertToCStr(WABText), WACtrlID, WAWinProc, WAExtraStyle, WAUnderState));
}

HWND CALLBACK Wrapper_CreateComboBox(long WABLeft, long WABTop, long WABWidth, long WABHeight, HWND WAHParent, char *WABText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle)
{
    return(CreateComboBox(WABLeft, WABTop, WABWidth, WABHeight, WAHParent, StringConvertToCStr(WABText), WACtrlID, WAWinProc, WAExtraStyle));
}

HWND CALLBACK Wrapper_CreateNonModalDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WADTitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType)
{
    return(CreateNonModalDialog(WADLeft, WADTop, WADWidth, WADHeight, WAHParent, WAhMenu, WAhIcon, StringConvertToCStr(WADTitle), InitProc, WAWinProc, WAWExStyle, WAWStyle, WAShowType));
}

HWND CALLBACK Wrapper_CreateFrame(long WABLeft, long WABTop, long WABWidth, long WABHeight, HWND WAHParent, char *WABText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle)
{
    return(CreateFrame(WABLeft, WABTop, WABWidth, WABHeight, WAHParent, StringConvertToCStr(WABText), WACtrlID, WAWinProc, WAExtraStyle));
}

HWND CALLBACK Wrapper_CreateHexBox(long WAEDLeft, long WAEDTop, long WAEDWidth, long WAEDHeight, HWND WAHParent, char *WAEDText, long WACtrlID, long WAMaxDigits, long WAExtraStyle, long WAExtraExStyle)
{
    return(CreateHexBox(WAEDLeft, WAEDTop, WAEDWidth, WAEDHeight, WAHParent, StringConvertToCStr(WAEDText), WACtrlID, WAMaxDigits, WAExtraStyle, WAExtraExStyle));
}

HWND CALLBACK Wrapper_CreateLabel(long WALLeft, long WALTop, long WALWidth, long WALHeight, HWND WAHParent, char *WALText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle, HFONT WAhFont)
{
    return(CreateLabel(WALLeft, WALTop, WALWidth, WALHeight, WAHParent, StringConvertToCStr(WALText), WACtrlID, WAWinProc, WAExtraStyle, WAhFont));
}

HWND CALLBACK Wrapper_CreateMDIChildDialog(HWND WAHParent, char *WACTitle, HICON WAhIcon, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAZeroPos, char *WACustomClass)
{
    return(CreateMDIChildDialog(WAHParent, StringConvertToCStr(WACTitle), WAhIcon, InitProc, WAWinProc, WAExtraStyle, WAZeroPos, StringConvertToCStr(WACustomClass)));
}

HWND CALLBACK Wrapper_CreateMDIDialog(long WAMDILeft, long WAMDITop, long WAMDIWidth, long WAMDIHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WAMDITitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAShowType)
{
    return(CreateMDIDialog(WAMDILeft, WAMDITop, WAMDIWidth, WAMDIHeight, WAHParent, WAhMenu, WAhIcon, StringConvertToCStr(WAMDITitle), InitProc, WAWinProc, WAExtraStyle, WAShowType));
}

long CALLBACK Wrapper_CreateModalDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, HWND WAHParent, DLGPROC WAWinProc, long WAExtraStyle, long WACentered)
{
    return(CreateModalDialog(WADLeft, WADTop, WADWidth, WADHeight, WAHParent, WAWinProc, WAExtraStyle, WACentered));
}

HWND CALLBACK Wrapper_CreateRadioButton(long WABLeft, long WABTop, long WABWidth, long WABHeight, HWND WAHParent, char *WABText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle)
{
    return(CreateRadioButton(WABLeft, WABTop, WABWidth, WABHeight, WAHParent, StringConvertToCStr(WABText), WACtrlID, WAWinProc, WAExtraStyle));
}

HWND CALLBACK Wrapper_CreateRichTextBox(long WAEDLeft, long WAEDTop, long WAEDWidth, long WAEDHeight, HWND WAHParent, char *WAEDText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle, long WASetborder)
{
    return(CreateRichTextBox(WAEDLeft, WAEDTop, WAEDWidth, WAEDHeight, WAHParent, StringConvertToCStr(WAEDText), WACtrlID, WAWinProc, WAExtraStyle, WASetborder));
}

HWND CALLBACK Wrapper_CreateSplashDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, char *WADTitle, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType)
{
    return(CreateSplashDialog(WADLeft, WADTop, WADWidth, WADHeight, StringConvertToCStr(WADTitle), WAHParent, WAhMenu, WAhIcon, InitProc, WAWinProc, WAWExStyle, WAWStyle, WAShowType));
}

HWND CALLBACK Wrapper_CreateStatusBar(char *WASBText, long WARaiseType, HWND WAHParent, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle)
{
    return(CreateStatusBar(StringConvertToCStr(WASBText), WARaiseType, WAHParent, WACtrlID, WAWinProc, WAExtraStyle));
}

HWND CALLBACK Wrapper_CreateTextBox(long WAEDLeft, long WAEDTop, long WAEDWidth, long WAEDHeight, HWND WAHParent, char *WAEDText, long WACtrlID, WNDPROC WAWinProc, long WAExtraStyle, long WAExtraExStyle)
{
    return(CreateTextBox(WAEDLeft, WAEDTop, WAEDWidth, WAEDHeight, WAHParent, StringConvertToCStr(WAEDText), WACtrlID, WAWinProc, WAExtraStyle, WAExtraExStyle));
}

char * CALLBACK Wrapper_DateGetDay(void)
{
    return(StringCStrConvertToString(DateGetDay()));
}

char * CALLBACK Wrapper_DateGetFormat(char *Dateformat)
{
    return(StringCStrConvertToString(DateGetFormat(StringConvertToCStr(Dateformat))));
}

char * CALLBACK Wrapper_DateGetHour(void)
{
    return(StringCStrConvertToString(DateGetHour()));
}

char * CALLBACK Wrapper_DateGetMinutes(void)
{
    return(StringCStrConvertToString(DateGetMinutes()));
}

char * CALLBACK Wrapper_DateGetMonth(void)
{
    return(StringCStrConvertToString(DateGetMonth()));
}

char * CALLBACK Wrapper_DateGetNow(long WAReportSeconds)
{
    return(StringCStrConvertToString(DateGetNow(WAReportSeconds)));
}

char * CALLBACK Wrapper_DateGetSeconds(void)
{
    return(StringCStrConvertToString(DateGetSeconds()));
}

char * CALLBACK Wrapper_DateGetTimeFormat(char *Dateformat)
{
    return(StringCStrConvertToString(DateGetTimeFormat(StringConvertToCStr(Dateformat))));
}

char * CALLBACK Wrapper_DateGetYear(void)
{
    return(StringCStrConvertToString(DateGetYear()));
}

HANDLE CALLBACK Wrapper_FileCreateEmpty(char *FileName, long WASecurity)
{
    return(FileCreateEmpty(StringConvertToCStr(FileName), WASecurity));
}

long CALLBACK Wrapper_FileExist(char *FileToSearch)
{
    return(FileExist(StringConvertToCStr(FileToSearch)));
}

long CALLBACK Wrapper_FileGetAccessedTime(char *FileName, LPFILETIME FileAccessedTime)
{
    return(FileGetAccessedTime(StringConvertToCStr(FileName), FileAccessedTime));
}

long CALLBACK Wrapper_FileGetCreationTime(char *FileName, LPFILETIME FileCreationTime)
{
    return(FileGetCreationTime(StringConvertToCStr(FileName), FileCreationTime));
}

char * CALLBACK Wrapper_FileGetDirectory(char *FileName)
{
    return(StringCStrConvertToString(FileGetDirectory(StringConvertToCStr(FileName))));
}

char * CALLBACK Wrapper_FileGetExtension(char *FileName)
{
    return(StringCStrConvertToString(FileGetExtension(StringConvertToCStr(FileName))));
}

char * CALLBACK Wrapper_FileGetFileName(char *FileName)
{
    return(StringCStrConvertToString(FileGetFileName(StringConvertToCStr(FileName))));
}

long CALLBACK Wrapper_FileGetSize(char *FileName)
{
    return(FileGetSize(StringConvertToCStr(FileName)));
}

long CALLBACK Wrapper_FileGetWriteTime(char *FileName, LPFILETIME FileWriteTime)
{
    return(FileGetWriteTime(StringConvertToCStr(FileName), FileWriteTime));
}

long CALLBACK Wrapper_FileIsDirectory(char *FileToSearch)
{
    return(FileIsDirectory(StringConvertToCStr(FileToSearch)));
}

HANDLE CALLBACK Wrapper_FileOpenR(char *FileName) {
    return(FileOpenR(StringConvertToCStr(FileName)));
}

HANDLE CALLBACK Wrapper_FileOpenW(char *FileName, long FilePos)
{
    return(FileOpenW(StringConvertToCStr(FileName), FilePos));
}

HANDLE CALLBACK Wrapper_FileOpenWAppend(char *FileName)
{
    return(FileOpenWAppend(StringConvertToCStr(FileName)));
}

char * CALLBACK Wrapper_FileRemoveExtension(char *FileName)
{
    return(StringCStrConvertToString(FileRemoveExtension(StringConvertToCStr(FileName))));
}

char * CALLBACK Wrapper_FileReplaceExtension(char *FileName, char *WANewExtension)
{
    return(StringCStrConvertToString(FileReplaceExtension(StringConvertToCStr(FileName), StringConvertToCStr(WANewExtension))));
}

long CALLBACK Wrapper_FileIsReadOnly(char *FileToCheck)
{
    return(FileIsReadOnly(StringConvertToCStr(FileToCheck)));
}

long CALLBACK Wrapper_FileIsUnix(char *FileToCheck)
{
    return(FileIsUnix(StringConvertToCStr(FileToCheck)));
}

HGLOBAL CALLBACK Wrapper_FileLoadIntoMem(char *FileToLoad, long *BytesRd)
{
    return(FileLoadIntoMem(StringConvertToCStr(FileToLoad), BytesRd));
}

long CALLBACK Wrapper_FileSaveFromMem(char *FileToSave, long MemToSave, long LenToSave)
{
    return(FileSaveFromMem(StringConvertToCStr(FileToSave), MemToSave, LenToSave));
}

char * CALLBACK Wrapper_FileDir(char *WAWildCard, long FileType)
{
    return(StringCStrConvertToString(FileDir(&(CStr) StringConvertToCStr(WAWildCard), FileType)));
}

long CALLBACK Wrapper_FileWriteLine(HANDLE FileHandle, char *WALineToWrite)
{
    return(FileWriteLine(FileHandle, StringConvertToCStr(WALineToWrite)));
}

char * CALLBACK Wrapper_FTPGetEntryDate(char *WADirEntry)
{
    return(StringCStrConvertToString(FTPGetEntryDate(StringConvertToCStr(WADirEntry))));
}

char * CALLBACK Wrapper_FTPGetEntryFileAttributes(char *WADirEntry)
{
    return(StringCStrConvertToString(FTPGetEntryFileAttributes(StringConvertToCStr(WADirEntry))));
}

char * CALLBACK Wrapper_FTPGetEntryFileName(char *WADirEntry)
{
    return(StringCStrConvertToString(FTPGetEntryFileName(StringConvertToCStr(WADirEntry))));
}

char * CALLBACK Wrapper_FTPGetEntryFileSize(char *WADirEntry)
{
    return(StringCStrConvertToString(FTPGetEntryFileSize(StringConvertToCStr(WADirEntry))));
}

long CALLBACK Wrapper_FTPIsEntryDir(char *WADirEntry)
{
    return(FTPIsEntryDir(StringConvertToCStr(WADirEntry)));
}

long CALLBACK Wrapper_GDIGetTextHeight(HWND WAhWnd, HFONT WAhFont, char *WATxt)
{
    return(GDIGetTextHeight(WAhWnd, WAhFont, StringConvertToCStr(WATxt)));
}

long CALLBACK Wrapper_GDIGetTextWidth(HWND WAhWnd, HFONT WAhFont, char *WATxt)
{
    return(GDIGetTextWidth(WAhWnd, WAhFont, StringConvertToCStr(WATxt)));
}

HFONT CALLBACK Wrapper_GDIObtainFont(char *WAFontNameToObtain, long WAFontSizeToObtain, HWND WAhWnd, long WABold, long WAItalic)
{
    return(GDIObtainFont(StringConvertToCStr(WAFontNameToObtain), WAFontSizeToObtain, WAhWnd, WABold, WAItalic));
}

void CALLBACK Wrapper_GDIWriteClippedText(HDC WAhDC, long WAx, long WAy, long WAWidth, long WAHeight, char *WAText, long WATextColor, HFONT WAhTextFont, long WATextTransparent, long WABackGroundColor)
{
    GDIWriteClippedText(WAhDC, WAx, WAy, WAWidth, WAHeight, StringConvertToCStr(WAText), WATextColor, WAhTextFont, WATextTransparent, WABackGroundColor);
}

void CALLBACK Wrapper_GDIWriteText(HDC WAhDC, long WAx, long WAy, char *WAText, long WATextColor, HFONT WAhTextFont, long WATextTransparent, long WABackGroundColor)
{
    GDIWriteText(WAhDC, WAx, WAy, StringConvertToCStr(WAText), WATextColor, WAhTextFont, WATextTransparent, WABackGroundColor);
}

long CALLBACK Wrapper_IniDeleteKey(char *WASection, char *WAKey, char *File)
{
    return(IniDeleteKey(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(File)));
}

char * CALLBACK Wrapper_IniReadKey(char *WASection, char *WAKey, char *File)
{
    return(StringCStrConvertToString(IniReadKey(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(File))));
}

long CALLBACK Wrapper_IniReadBoolKey(char *WASection, char *WAKey, char *File)
{
    return(IniReadBoolKey(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(File)));
}

long CALLBACK Wrapper_IniWriteKey(char *WASection, char *WAKey, char *WAValue, char *File)
{
    return(IniWriteKey(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(WAValue), StringConvertToCStr(File)));
}

long CALLBACK Wrapper_IniWriteSection(char *WASection, char *WAKey, char *File)
{
    return(IniWriteSection(StringConvertToCStr(WASection), StringConvertToCStr(WAKey), StringConvertToCStr(File)));
}

long CALLBACK Wrapper_ListBoxAddItem(HWND WAhLB, char *WALBItemText, long WALBItemIndex)
{
    return(ListBoxAddItem(WAhLB, StringConvertToCStr(WALBItemText), WALBItemIndex));
}

char * CALLBACK Wrapper_ListBoxGetCurrentItem(HWND WAhLB)
{
    return(StringCStrConvertToString(ListBoxGetCurrentItem(WAhLB)));
}

char * CALLBACK Wrapper_ListBoxGetItem(HWND WAhLB, long WALBIndex)
{
    return(StringCStrConvertToString(ListBoxGetItem(WAhLB, WALBIndex)));
}

long CALLBACK Wrapper_ListBoxItemExist(HWND WAhListBox, char *WACBItem)
{
    return(ListBoxItemExist(WAhListBox, StringConvertToCStr(WACBItem)));
}

long CALLBACK Wrapper_ListBoxReplaceItem(HWND WAhLB, long WALBIndex, char *WALBNewItem)
{
    return(ListBoxReplaceItem(WAhLB, WALBIndex, StringConvertToCStr(WALBNewItem)));
}

long CALLBACK Wrapper_ListBoxReplaceSelItem(HWND WAhLB, char *WBLBNewItemText)
{
    return(ListBoxReplaceSelItem(WAhLB, StringConvertToCStr(WBLBNewItemText)));
}

long CALLBACK Wrapper_ListViewAddCol(HWND WAhListView, char *WALVColText, long WALVWidth, long WAColPosition)
{
    return(ListViewAddCol(WAhListView, StringConvertToCStr(WALVColText), WALVWidth, WAColPosition));
}

long CALLBACK Wrapper_ListViewAddItem(HWND WAhListView, char *WALVItemText, long WALVItemPos, long WALVImage)
{
    return(ListViewAddItem(WAhListView, StringConvertToCStr(WALVItemText), WALVItemPos, WALVImage));
}

long CALLBACK Wrapper_ListViewFindItem(HWND WAhListView, char *WALVItemText)
{
    return(ListViewFindItem(WAhListView, StringConvertToCStr(WALVItemText)));
}

long CALLBACK Wrapper_ListViewFindSubItem(HWND WAhListView, char *WATextToFind, long WASubItemNbr, long WASearchCase)
{
    return(ListViewFindSubItem(WAhListView, StringConvertToCStr(WATextToFind), WASubItemNbr, WASearchCase));
}

char * CALLBACK Wrapper_ListViewGetEditResult(long WAlParam)
{
    return(StringCStrConvertToString(ListViewGetEditResult(WAlParam)));
}

char * CALLBACK Wrapper_ListViewGetHeaderLabel(HWND WAhListView, long WALVColNumber)
{
    return(StringCStrConvertToString(ListViewGetHeaderLabel(WAhListView, WALVColNumber)));
}

char * CALLBACK Wrapper_ListViewGetItemText(HWND WAhListView, long WALVItemIndex, long WALVSubItemNumber)
{
    return(StringCStrConvertToString(ListViewGetItemText(WAhListView, WALVItemIndex, WALVSubItemNumber)));
}

char * CALLBACK Wrapper_ListViewGetSelItemText(HWND WAhListView, long WALVSubItemNumber)
{
    return(StringCStrConvertToString(ListViewGetSelItemText(WAhListView, WALVSubItemNumber)));
}

long CALLBACK Wrapper_ListViewSetSelItemText(HWND WAhListView, char *WALVItemText, long WALVSubItemNumber)
{
    return(ListViewSetSelItemText(WAhListView, StringConvertToCStr(WALVItemText), WALVSubItemNumber));
}

long CALLBACK Wrapper_ListViewSetSubItem(HWND WAhListView, char *WALVItemText, long WALVItemPos, long WALVSubItemPos)
{
    return(ListViewSetSubItem(WAhListView, StringConvertToCStr(WALVItemText), WALVItemPos, WALVSubItemPos));
}

char * CALLBACK Wrapper_MenuGetString(HMENU WAhMenu, long WAItemID)
{
    return(StringCStrConvertToString(MenuGetString(WAhMenu, WAItemID)));
}

long CALLBACK Wrapper_RebarAddBand(HWND WAhRebar, HWND WAhChild, char *WARBText, long WARBYSize, long WAExtraStyle, long WAMinSize)
{
    return(RebarAddBand(WAhRebar, WAhChild, StringConvertToCStr(WARBText), WARBYSize, WAExtraStyle, WAMinSize));
}

long CALLBACK Wrapper_RegistryDeleteKey(HKEY WAKeyRoot, char *WAKeyName, char *WASubKeyName, long WAKeyType)
{
    return(RegistryDeleteKey(WAKeyRoot, StringConvertToCStr(WAKeyName), StringConvertToCStr(WASubKeyName), WAKeyType));
}

char * CALLBACK Wrapper_RegistryGetKeyValue(HKEY WAKeyRoot, char *WAKeyName, char *WASubKeyRef)
{
    return(StringCStrConvertToString(RegistryGetKeyValue(WAKeyRoot, StringConvertToCStr(WAKeyName), StringConvertToCStr(WASubKeyRef))));
}

long CALLBACK Wrapper_RegistryUpdateKey(HKEY WAKeyRoot, char *WAKeyName, char *WASubKeyName, char *WASubKeyValue, long WAKeyType)
{
    return(RegistryUpdateKey(WAKeyRoot, StringConvertToCStr(WAKeyName), StringConvertToCStr(WASubKeyName), StringConvertToCStr(WASubKeyValue), WAKeyType));
}

void CALLBACK Wrapper_MiscShellAddFileToRecents(char *FileName)
{
    MiscShellAddFileToRecents(StringConvertToCStr(FileName));
}

long CALLBACK Wrapper_SocketConnect(SOCKET WAhSock, LPSOCKADDR_IN WASockStruct, long WAMessage, HWND WAhWnd, char *WAIP, long WAPort, long WATimeOut)
{
    return(SocketConnect(WAhSock, WASockStruct, WAMessage, WAhWnd, StringConvertToCStr(WAIP), WAPort, WATimeOut));
}

char * CALLBACK Wrapper_SocketReceiveASCIIDatas(SOCKET WAhSock)
{
    return(StringCStrConvertToString(SocketReceiveASCIIDatas(WAhSock)));
}

long CALLBACK Wrapper_SocketSendASCIIDatasCR(SOCKET WAhSock, char *WABuffer)
{
    return(SocketSendASCIIDatasCR(WAhSock, StringConvertToCStr(WABuffer)));
}

long CALLBACK Wrapper_StatusBarSetText(HWND WAhStatusBar, long WASBPart, long WARaiseType, char *WASBText, long WAOwnerDatas)
{
    return(StatusBarSetText(WAhStatusBar, WASBPart, WARaiseType, StringConvertToCStr(WASBText), WAOwnerDatas));
}

long CALLBACK Wrapper_StringCount(char *WAMaIn_String, char *StringToCount, long WACountStart, long WACompareType)
{
    return(StringCount(StringConvertToCStr(WAMaIn_String), StringConvertToCStr(StringToCount), WACountStart, WACompareType));
}

char * CALLBACK Wrapper_StringReplace(char *WAMaIn_String, char *StringToFind, char *StringToReplace, long WAReplaceStart, long WARepCount, long WACompareType)
{
    return(StringCStrConvertToString(StringReplace(StringConvertToCStr(WAMaIn_String), StringConvertToCStr(StringToFind), StringConvertToCStr(StringToReplace), WAReplaceStart, WARepCount, WACompareType)));
}

char * CALLBACK Wrapper_StringGetSplitElement(char *WAStrFromSplit, long *WASplitMem, long WAElement)
{
    return(StringCStrConvertToString(StringGetSplitElement(StringConvertToCStr(WAStrFromSplit), WASplitMem, WAElement)));
}

char * CALLBACK Wrapper_StringHex16(long WANumber)
{
    return(StringCStrConvertToString(StringHex16(WANumber)));
}

char * CALLBACK Wrapper_StringHex32(long WANumber)
{
    return(StringCStrConvertToString(StringHex32(WANumber)));
}

char * CALLBACK Wrapper_StringHex8(long WANumber)
{
    return(StringCStrConvertToString(StringHex8(WANumber)));
}

long CALLBACK Wrapper_StringIsAlphaChar(char *WAALetter)
{
    return(StringIsAlphaChar(StringConvertToCStr(WAALetter)));
}

long CALLBACK Wrapper_StringIsArobasChar(char *WAPLetter)
{
    return(StringIsArobasChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsSpaceChar(char *WAPLetter)
{
    return(StringIsSpaceChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsBlankChar(char *WABLetter)
{
    return(StringIsBlankChar(StringConvertToCStr(WABLetter)));
}

long CALLBACK Wrapper_StringIsDecimal(char *WADecText, long WAMaxLen)
{
    return(StringIsDecimal(StringConvertToCStr(WADecText), WAMaxLen));
}

long CALLBACK Wrapper_StringIsDigitChar(char *WADLetter)
{
    return(StringIsDigitChar(StringConvertToCStr(WADLetter)));
}

long CALLBACK Wrapper_StringIsDotChar(char *WAPLetter)
{
    return(StringIsDotChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsCommaChar(char *WAPLetter)
{
    return(StringIsCommaChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsQuoteChar(char *WAPLetter)
{
    return(StringIsQuoteChar(StringConvertToCStr(WAPLetter)));
}

long CALLBACK Wrapper_StringIsEOL(char *WAEOLetter)
{
    return(StringIsEOL(StringConvertToCStr(WAEOLetter)));
}

long CALLBACK Wrapper_StringIsHexaChar(char *WAALetter)
{
    return(StringIsHexaChar(StringConvertToCStr(WAALetter)));
}

long CALLBACK Wrapper_StringIsHexaDecimal(char *WAHexaText)
{
    return(StringIsHexaDecimal(StringConvertToCStr(WAHexaText)));
}

long CALLBACK Wrapper_StringIsLabel(char *WAAsmText, int Allow_Space)
{
    return(StringIsLabel(StringConvertToCStr(WAAsmText), Allow_Space));
}

long CALLBACK Wrapper_StringIsLabelChar(char *WAVCLetter)
{
    return(StringIsLabelChar(StringConvertToCStr(WAVCLetter)));
}

long CALLBACK Wrapper_StringIsOpenBracketChar(char *WAOLetter)
{
    return(StringIsOpenBracketChar(StringConvertToCStr(WAOLetter)));
}

long CALLBACK Wrapper_StringIsUnderLineChar(char *WAULetter)
{
    return(StringIsUnderLineChar(StringConvertToCStr(WAULetter)));
}

char * CALLBACK Wrapper_StringJoin(char *WAStrFromSplit, long *WASplitMem, char *WAJoIn_String, long WAFirstElement, long WAElementLimit)
{
    return(StringCStrConvertToString(StringJoin(StringConvertToCStr(WAStrFromSplit), WASplitMem, StringConvertToCStr(WAJoIn_String), WAFirstElement, WAElementLimit)));
}

char * CALLBACK Wrapper_StringNumberComplement(long WANbr, long WAComplement)
{
    return(StringCStrConvertToString(StringNumberComplement(WANbr, WAComplement)));
}

char * CALLBACK Wrapper_StringHexNumberComplement(long WANbr, long WAComplement)
{
    return(StringCStrConvertToString(StringHexNumberComplement(WANbr, WAComplement)));
}

char * CALLBACK Wrapper_StringOctNumberComplement(long WANbr, long WAComplement)
{
    return(StringCStrConvertToString(StringOctNumberComplement(WANbr, WAComplement)));
}

char * CALLBACK Wrapper_StringBinNumberComplement(long WANbr, long WAComplement)
{
    return(StringCStrConvertToString(StringBinNumberComplement(WANbr, WAComplement)));
}

long * CALLBACK Wrapper_StringSplit(char *WAStrToSplit, char *WAStrSplitter)
{
    return(StringSplit(StringConvertToCStr(WAStrToSplit), StringConvertToCStr(WAStrSplitter)));
}

char * CALLBACK Wrapper_StringCat(char *String1, char *String2)
{
    return(StringCStrConvertToString(StringCat(StringConvertToCStr(String1), StringConvertToCStr(String2))));
}

char * CALLBACK Wrapper_StringDecToString(long WADecimalNbr)
{
    return(StringCStrConvertToString(StringDecToString(WADecimalNbr)));
}

long CALLBACK Wrapper_StringStringToDec(char *WADecString)
{
    return(StringStringToDec(StringConvertToCStr(WADecString)));
}

long CALLBACK Wrapper_StringStringToHex(char *WAHexString)
{
    return(StringStringToHex(StringConvertToCStr(WAHexString)));
}

long CALLBACK Wrapper_SysTabAddItem(HWND WAhTab, char *WATabText, long WATabIndex, long WAImgIndex)
{
    return(SysTabAddItem(WAhTab, StringConvertToCStr(WATabText), WATabIndex, WAImgIndex));
}

void CALLBACK Wrapper_TextBoxAddText(HWND WAhTextBox, char *WATbText)
{
    TextBoxAddText(WAhTextBox, StringConvertToCStr(WATbText));
}

char * CALLBACK Wrapper_TextBoxGetPasswordChar(HWND WAhTextBox)
{
    return(StringCStrConvertToString(TextBoxGetPasswordChar(WAhTextBox)));
}

long CALLBACK Wrapper_TextBoxSetPasswordChar(HWND WAhTextBox, char *PassChar)
{
    return(TextBoxSetPasswordChar(WAhTextBox, StringConvertToCStr(PassChar)));
}

long CALLBACK Wrapper_ToolBarAddButton(HWND WAhToolbar, char *ButtonText, long WATBButtonID, long WAImgIndex, long ButtonStyle, long ButtonState, long WANoImage)
{
    return(ToolBarAddButton(WAhToolbar, StringConvertToCStr(ButtonText), WATBButtonID, WAImgIndex, ButtonStyle, ButtonState, WANoImage));
}

void CALLBACK Wrapper_ToolBarDisplayToolTip(char *WATextToShow, long WAlParam)
{
    ToolBarDisplayToolTip(StringConvertToCStr(WATextToShow), WAlParam);
}

HTREEITEM CALLBACK Wrapper_TreeViewAddItem(HWND WAhTreeView, char *WATVItemText, HTREEITEM WATVParent, HTREEITEM WATVChildPos, long WATVImage, long WATVSelImage, long WAExtraStyle, long WARefreshParentAfterAdd)
{
    return(TreeViewAddItem(WAhTreeView, StringConvertToCStr(WATVItemText), WATVParent, WATVChildPos, WATVImage, WATVSelImage, WAExtraStyle, WARefreshParentAfterAdd));
}

char * CALLBACK Wrapper_TreeViewGetItemText(HWND WAhTreeView, HTREEITEM WAhItem)
{
    return(StringCStrConvertToString(TreeViewGetItemText(WAhTreeView, WAhItem)));
}

long CALLBACK Wrapper_TreeViewSearchChildPartialText(HWND WAhTreeView, HTREEITEM WAhNode, char *WAItemText)
{
    return(TreeViewSearchChildPartialText(WAhTreeView, WAhNode, StringConvertToCStr(WAItemText)));
}

long CALLBACK Wrapper_TreeViewSearchItemText(HWND WAhTreeView, HTREEITEM WAhNode, char *WAItemText)
{
    return(TreeViewSearchItemText(WAhTreeView, WAhNode, StringConvertToCStr(WAItemText)));
}

long CALLBACK Wrapper_TreeViewSetItemText(HWND WAhTreeView, HTREEITEM WAhItem, char *WAItemText)
{
    return(TreeViewSetItemText(WAhTreeView, WAhItem, StringConvertToCStr(WAItemText)));
}

char * CALLBACK StringLeft(char *String, long Dat)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Left(Dat)));
}

char * CALLBACK StringRight(char *String, long Dat)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Right(Dat)));
}

char * CALLBACK StringTrim(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Trim()));
}

char * CALLBACK StringLTrim(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).L_Trim()));
}

char * CALLBACK StringRTrim(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).R_Trim()));
}

long CALLBACK StringInStr(char *String, long Start, char *WASearchString, long CompType)
{
    return(StringConvertToCStr(String).In_Str(Start, WASearchString, CompType));
}

char * CALLBACK StringMid(char *String, long Start, long Len)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Mid(Start, Len)));
}

char * CALLBACK StringUpperCase(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Upper_Case()));
}

char * CALLBACK StringLowerCase(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Lower_Case()));
}

char * CALLBACK StringReverse(char *String)
{
    return(StringCStrConvertToString(StringConvertToCStr(String).Str_Reverse()));
}

char * CALLBACK StringSpace(long Len)
{
    CStr BufString;
    return(StringCStrConvertToString(BufString.Space(Len)));
}

char * CALLBACK StringString(long Len, long Dat)
{
    CStr BufString;
    return(StringCStrConvertToString(BufString.String(Len, Dat)));
}

long CALLBACK StringIsNumeric(char *String)
{
    return(StringConvertToCStr(String).Is_Numeric());
}

// Future extensions
long CALLBACK MiscObtainExtendedFunctions(void)
{
    return(0);
}

HWND CALLBACK Wrapper_CreateDockingBox(char *Title, WNDPROC WindowProc)
{
    return(CreateDockingBox(StringConvertToCStr(Title), WindowProc, hMDIform.hDock, MainIniFile.Get_String()));
}

long CALLBACK Wrapper_DockingBoxWasVisible(char *DockName)
{
    return(DockingBoxWasVisible(StringConvertToCStr(DockName), MainIniFile));
}

void CALLBACK Wrapper_DockingBoxShow(HWND hDock, int DefaultXSize, int DefaultYSize, int DefaultState)
{
    DockingBoxShow(hDock, GetMDIWidth(), GetMDIHeight(), DefaultXSize, DefaultYSize, DefaultState);
}

// -----------------------------------------------------------------------
// Create a custom child Window
HWND CALLBACK Wrapper_WAMMCreateCustomChildDialog(char *Title, void (CALLBACK *CallBackInit)(HWND hwnd), WNDPROC CallBackProc)
{
    return(WAMMCreateCustomChildDialog(StringConvertToCStr(Title), CallBackInit, CallBackProc));
}

HWND WAMMCreateCustomChildDialog(CStr Title, void (CALLBACK *CallBackInit)(HWND hwnd), WNDPROC CallBackProc)
{
    HWND hWnd;

    CurCustChildInit = CallBackInit;
    CurCustChildProc = CallBackProc;
    CurCustChildTitle = Title;
    // Create a new child window
    if(NbForms != 0) if(ControlGetWindowState(CurrentForm) == SW_SHOWMAXIMIZED) goto ForceMaximize;
    // Save arguments
    switch(AutoMaximizeChilds)
    {
        case 0:
            hWnd = CreateMDIChildDialog(hMDIform.hClient, "",
                                        LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + ICON_SNAP)), 
                                        CustChildWinInitProc, CustChildWinProc, 0, 1, "");
            break;
        case 1:
ForceMaximize:
            hWnd = CreateMDIChildDialog(hMDIform.hClient, "",
                                        LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + ICON_SNAP)),
                                        CustChildWinInitProc, CustChildWinProc, WS_MAXIMIZE, 1, "");
            break;
    }
    DrawMenuBar(hMDIform.hWnd);
    return(hWnd);
}

// -----------------------------------------------------------------------
// Custom child window initialization
void CALLBACK CustChildWinInitProc(HWND hWnd)
{
    LPCHILDDAT NewChildMem;

    NewChildMem = (LPCHILDDAT) AllocMem(sizeof(CHILDDAT));
    NewChildMem->RFile = new CStr;
    NewChildMem->Language = new CStr;
    NewChildMem->oldAPIFnc = new CStr;
    NewChildMem->oldAPILineFnc = new CStr;
    NewChildMem->FileDateOpen = (LPFILETIME) AllocMem(sizeof(FILETIME));
    NewChildMem->hChildCodeMax = 0;
    NewChildMem->FileLoaded = 0;
    // (Necessary)
    NewChildMem->ReadOnly = 1;
    // Save user callback in child space
    NewChildMem->CurCallBackProc = CurCustChildProc;
    // Set title
    NewChildMem->RFile->Set_String(CurCustChildTitle.Get_String());
    // Set the child text
    ControlSetText(hWnd, CurCustChildTitle);
    CurrentForm = hWnd;
    // Now we can set the pointer to the structure
    SetWindowLong(hWnd, GWL_USERDATA, (long) NewChildMem);
    NbForms++;
    SendMessage(hMDIform.hClient, CHILD_MSG_CREATED, ICON_WINDOWLOCK, (LPARAM) hWnd);
    // Run user init now
    if(CurCustChildInit != NULL) CurCustChildInit(hWnd);
}

// -----------------------------------------------------------------------
// Child Winproc
LRESULT CALLBACK CustChildWinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CHILDACTIVATE:
            CurrentForm = hWnd;
            break;
        case WM_SIZE:
            CurrentForm = hWnd;
            break;
        case WM_MOUSEACTIVATE:
            if(ClientGetActiveChild(hMDIform.hClient) != hWnd)
            {
                CurrentForm = hWnd;
                SetFocus(CurrentForm);
                // Notify user in all cases
                ChildStruct = LoadStructure(hWnd);
                if(ChildStruct->CurCallBackProc != NULL)
                {
                    // If callback returns 1 then abort the process
                    if(ChildStruct->CurCallBackProc(hWnd, uMsg, wParam, lParam) == 1) return(0);
                }
                return(MA_ACTIVATE);
            }
            break;
        case WM_PAINT:
            CurrentForm = ClientGetActiveChild(hMDIform.hClient);
            break;
        case WM_CLOSE:
            ChildStruct = LoadStructure(hWnd);
            if(ChildStruct->CurCallBackProc != NULL)
            {
                // If callback returns 1 then abort the process
                if(ChildStruct->CurCallBackProc(hWnd, uMsg, wParam, lParam) == 1) return(0);
            }
            // Delete them manually
            delete ChildStruct->RFile;
            delete ChildStruct->Language;
            delete ChildStruct->oldAPIFnc;
            delete ChildStruct->oldAPILineFnc;
            FreeMem((long) ChildStruct->FileDateOpen);
            // Free the child structure
            FreeMem(GetWindowLong(hWnd, GWL_USERDATA));
            // Put a fictive structure for following messages
            // (Re-use global one since it never changes)
            SetWindowLong(hWnd, GWL_USERDATA, (long) &ChildStructBack);
            // (User callback called above)
            return(DefMDIChildProc(hWnd, uMsg, wParam, lParam));
        case WM_DESTROY:
            NbForms--;
            if(NbForms == 0) ClearStatusBarParts();
            // Notify the parent
            SendMessage(hMDIform.hClient, CHILD_MSG_DESTROYED, 0, (long) hWnd);
            return(0);
    }
    ChildStruct = LoadStructure(hWnd);
    if(ChildStruct->CurCallBackProc != NULL)
    {
        ChildStruct->CurCallBackProc(hWnd, uMsg, wParam, lParam);
    }
    return(DefMDIChildProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Attach a codemax control to a child window
HWND CALLBACK WAMMCreateCodeMax(long Left, long Top, long Width, long Height, HWND hWnd, void (CALLBACK *CodeMaxPropertiesHook)(HWND))
{
    HWND hCodeMax;
    
    hCodeMax = CreateCodeMax(Left, Top, Width, Height, hWnd, ApphInstance, 1);
    if(hCodeMax == NULL) return(NULL);
    ChildStruct = LoadStructure(hWnd);
    ChildStruct->hChildCodeMax = hCodeMax;
    ChildStruct->CodeMaxPropertiesHook = CodeMaxPropertiesHook;
    // Init basic behaviours / styles
    InitMinimumCodeMax(hCodeMax);
    SetCodeMaxColors(hCodeMax);
    SetCodeMaxFont(hCodeMax);
    CM_SetLanguage(hCodeMax, "X86 Assembler");
    DragAcceptFiles(hCodeMax, 0);
    // Run user props now
    CodeMaxPropertiesHook(hCodeMax);
    return(hCodeMax);
}
