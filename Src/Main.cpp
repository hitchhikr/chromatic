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
// Main.cpp: Main module
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Classes/WADock.h"
#include "Globals.h"
#include "Main.h"
#include "MsgCodeMax.h"
#include "ConstRes.h"
#include "IniFiles.h"
#include "Languages.h"
#include "APIDb.h"
#include "ImageLists.h"
#include "Debugger.h"
#include "Menus.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "MiscFunctions.h"
#include "ProjectsBuilder.h"
#include "AddIns.h"
#include "Toolbars.h"
#include "CmdMenus.h"
#include "Dlg_Splash.h"
#include "Dlg_Tips.h"
#include "Script_Engine/Scripts.h"

// -----------------------------------------------------------------------
// Constants
#define CHROMATIC_VERSION 1
#define CHROMATIC_REV_MAJ 3
#define CHROMATIC_REV_MIN 6

// -----------------------------------------------------------------------
// Variables
CStr StartupRetVal;
HACCEL hGlobAccelerators;
LPTOP_LEVEL_EXCEPTION_FILTER OldExceptFilter;

// -----------------------------------------------------------------------
// WinMain()
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    bool MultipleCheck = false;
    CStr TempToSave;
    CStr TempWindowsDir;
    CStr SplashStatus;
    CStr BufString;
    CStr TipsIniKey;
    long DbRead;

    // Save passed arguments
    PassedFile = lpCmdLine;

    //InstallExceptFilter();
    
    // --- Init global variables
    InitAppVars();

    // --- Multiple instances exclusions
    BroadCastMessage = RegisterWindowMessage("hitchhikr was here");
    MultipleCheck = false;
    // Check if multiple instances are allowed
    StartupRetVal = IniReadKey("Layout", "Multiple", MainIniFile);
    if(StartupRetVal.Len() != 0)
    {
        if(StartupRetVal.Get_Long() == 0) MultipleCheck = false;
        else MultipleCheck = true;
    }
    if(MultipleCheck == false)
    {
        BroadCastMutex = OpenMutex(MUTEX_ALL_ACCESS, 1, "ChromaticMutex");
        if(BroadCastMutex != 0)
        {
            // Save the filename
            TempWindowsDir = TempWindowsDir.String(MAX_PATH, 1);
            GetWindowsDirectory(TempWindowsDir.Get_String(), MAX_PATH);
            TempToSave = TempWindowsDir + (CStr) "\\TempOp.CF";
            MSaveFile(TempToSave.Get_String(), (long) PassedFile.Get_String(), PassedFile.Len());
            MiscSendBroadCastMsg(BroadCastMessage);
            // Already created so exit
            CloseHandle(BroadCastMutex);
        }
        else
        {
            // Own mutex
            BroadCastMutex = CreateMutex(0, -1, "ChromaticMutex");
            // Start initializations
            InitCodeMax(ApphInstance);
            if(hCodeMax != 0)
            {
                if((WidgetInit(ApphInstance,
                               LoadIcon(ApphInstance, MAKEINTRESOURCE(APP_ICON)),
                               CURSOR_BASE + CURSOR_SPLITV,
                               CURSOR_BASE + CURSOR_SPLITH) & INIT_NOEXCONTROLS) == 0)
                {
                    if(PassedFile.Len() == 0)
                    {
                        SplashStatus = IniReadKey("Layout", "ShowSplash", MainIniFile);
                        if(SplashStatus.Len() == 0) SplashStatus = "1";
                        if(SplashStatus == "1") hSplash = CreateSplashDialog(-1, -1, 500, 215, "DON'T PANIC", 0, 0, 0, &FrmSplashInitProc, &FrmSplashWinHook, 0, 0, SW_SHOW);
                    }
                    FrmSplashSetState("Checking directories structure...");
                    CheckDirectories();
                    FrmSplashSetState("Reading main configuration file...");
                    DirChangedNotify = ReadIniFile();
                    FrmSplashSetState("Reading languages definitions...");
                    ReadLangSpec();
                    FrmSplashSetState("Loading databases...");
                    FncBase = (long) MLoadFile(APIFncFile.Get_String(), &DbRead);
                    ConstBase = (long) MLoadFile(APIConstFile.Get_String(), &DbRead);
                    FrmSplashSetState("Initializing global tables...");
                    InitAppArrays();
                    FrmSplashSetState("Loading resources...");
                    InitImageList1();
                    InitImageList2();
                    InitImageList4();
                    InitImageList5();
                    Get_PSAPI();            // Obtain PSAPI
                    FrmSplashSetState("Creating interface context...");
                    hGlobAccelerators = LoadAccelerators(ApphInstance, MAKEINTRESOURCE(ACCEL_BASE));
                    InitMenuComments();
                    InitMenuToolTips();
                    CreateMDIForm();
                    FrmSplashSetState("Initialization done.");
                    ControlClose(hSplash);
                    if(NotifDirStructureError == 1) MiscMsgBox(hMDIform.hWnd, "Directory structure reconstructed.", MB_INFORMATION, Requesters);
                    if(NotifGrepError == 1) MiscMsgBox(hMDIform.hWnd, "Specified CGrep.ini file isn't usable.\rBuilt-in definitions will be used instead.", MB_INFORMATION, Requesters);
                    if(PassedFile.Len() != 0)
                    {
                        if(OpenUnknownFile(PassedFile, TRUE) == 0)
                        {
                            PassedFile = StringReplace(PassedFile, "\"", "", 1, -1, Binary_Compare);
                            OpenUnknownFile(PassedFile, TRUE);
                        }
                    }
                    else RestoreState(PrivateWorkSpaceFile);
                    SetForegroundWindow(hMDIform.hWnd);
                    // Display the tip of the day
                    TipsIniKey = IniReadKey("Layout", "ShowTips", MainIniFile);
                    if(TipsIniKey.Len() != 0)
                    {
                        if(_strcmpi(TipsIniKey.Get_String(), "1") == 0) DisplayTip(hMDIform.hWnd);
                    }
                    else
                    {
                        DisplayTip(hMDIform.hWnd);
                    }
                    AllocScriptEngine();
                    MiscWaitEvents(hMDIform.hClient, hGlobAccelerators, hMDIform.hWnd);
                    ReleaseScriptEngine();
                    // Stop running addins at the end
                    StopAddIns();
                    // Remove all image lists
                    if(GlobalImageList1 != 0) ImageListDestroy(GlobalImageList1);
                    if(GlobalImageList2 != 0) ImageListDestroy(GlobalImageList2);
                    if(GlobalImageList3 != 0) ImageListDestroy(GlobalImageList3);
                    if(GlobalImageList4 != 0) ImageListDestroy(GlobalImageList4);
                    if(FncBase != 0) FreeMem(FncBase);
                    if(ConstBase != 0) FreeMem(ConstBase);
                    Close_PSAPI();
                    WidgetUnInit(ApphInstance);
                    EraseResArrays();
                    UnInitCodeMax();
                    if(CurFontHandle != 0) DeleteObject(CurFontHandle);
                    CurFontHandle = 0;
                }
                else
                {
                    WidgetUnInit(ApphInstance);
                    UnInitCodeMax();
                    BufString = "Sing that song with me:";
                    BufString = BufString + "\r\r";
                    BufString = BufString + AppTitle;
                    BufString = BufString + " cannot be started\rMy system is outdated\rI will use Notepad instead";
                    MiscMsgBox(NULL, BufString, MB_ERROR, Requesters);
                    ShellExecute(0, "open", "notepad.exe", "", "", SW_SHOW);
                }
            }
            else
            {
                BufString = AppTitle;
                BufString = BufString + " can't find CMaxXX.dll.\rPlease, be sure that this file is located in the Dlls directory\ror in Windows System directory.";
                MiscMsgBox(NULL, BufString, MB_ERROR, Requesters);
            }
            // Release mutex before exiting
            if(BroadCastMutex != 0)
            {
                ReleaseMutex(BroadCastMutex);
                CloseHandle(BroadCastMutex);
            }
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Init global application static variables
void InitAppVars(void)
{
    int i = 0;
    char *LocAppPath = "";

    // Fake structure
    memset(&ChildStructBack, 0, sizeof(ChildStructBack));
    AppVersion = CHROMATIC_VERSION;
    AppRevMaj = CHROMATIC_REV_MAJ;
    AppRevMin = CHROMATIC_REV_MIN;
    AppTitle = "Chromatic";
    AppCredits = "Written by Franck Charlet";
    Requesters = AppTitle;
    Requesters = Requesters + " v";
    Requesters = Requesters + AppVersion;
    Requesters = Requesters + ".";
    Requesters = Requesters + AppRevMaj;
    Requesters = Requesters + ".";
    Requesters = Requesters + AppRevMin;
    VersionString = "Version ";
    VersionString = VersionString + AppVersion;
    VersionString = VersionString + ".";
    VersionString = VersionString + AppRevMaj;
    VersionString = VersionString + ".";
    VersionString = VersionString + AppRevMin;
    ApphInstance = GetModuleHandle(0);
    AppPath = AppPath.String(260, 1);
    LocAppPath = AppPath.Get_String();
    i = GetModuleFileName(0, LocAppPath, MAX_PATH);
    while(i--)
    {
        if(LocAppPath[i] == '\\')
        {
            LocAppPath[i] = 0;
            break;
        }
    }
    if(strcmp(AppPath.Right(1).Get_String(), "\\") == 0) AppPath = AppPath.Left(AppPath.Len() - 1);

    // Configuration files
    MainIniFile = AppPath + (CStr) "\\Config\\Main.ini";
    ToolbarsIniFile = AppPath + (CStr) "\\Config\\Toolbars.ini";
    ToolbarsTMPIniFile = AppPath + (CStr) "\\Config\\ToolbarsTmp.ini";
    ColorsIniFile = AppPath + (CStr) "\\Config\\Colors.ini";
    FtpAccountsIniFile = AppPath + (CStr) "\\Config\\FtpAccounts.ini";
    SkinsIniFile = AppPath + (CStr) "\\Config\\Skins.ini";
    LanguagesIniFile = AppPath + (CStr) "\\Config\\Languages.ini";
    DatesIniFile = AppPath + (CStr) "\\Config\\Dates.ini";
    ProjectsIniFile = AppPath + (CStr) "\\Config\\ProjectsType.ini";
    FiltersIniFile = AppPath + (CStr) "\\Config\\Filters.ini";
    PrivateWorkSpaceFile = AppPath + (CStr) "\\Config\\Chromatic.wst";

    Buttons_StaticEdge = WS_EX_STATICEDGE;
    int OsType = MiscGetOSClass();
    if(OsType & OSTYPE_WINDOWS_NT) WindowsNT = TRUE;
    if(OsType & OSTYPE_WINDOWS_2K) Windows2K = TRUE;
    if(OsType & OSTYPE_WINDOWS_XP)
    {
        Buttons_StaticEdge = 0;
        WindowsXP = TRUE;
    }
    if(OsType & OSTYPE_WINDOWS_7)
    {
        Windows7 = TRUE;
    }

    // Fill the AddIns services datas
    FillChromaticLib();
    // Get hwnd procs
    CurrentMDIChildProc = &MDIChildProc;
    CurrentCodeMaxProc = &CodeMaxProc;
    CurrentMDIChildInitProc = &MDIChildInitProc;
    CurrentMDIProc = &MDIProc;
}

// -----------------------------------------------------------------------
// Init global arrays
void InitAppArrays(void)
{
    //TabSearch.Erase();
    //TabWild.Erase();
    LastOpened.Erase();
    LastOpenedLine.Erase();
    LastOpenedCol.Erase();
    LastOpenedFirst.Erase();
    PrjIconsArray.Erase();
    PrjCursorsArray.Erase();
    PrjBitmapsArray.Erase();
    PrjStringsArray.Erase();
    PrjAcceleratorsArray.Erase();
    PrjMenusArray.Erase();
    PrjDialogsArray.Erase();
    PrjRawDatasArray.Erase();
    MDICreateHooks.Erase();
    MDIHooks.Erase();
    MDIClientHooks.Erase();
    ChildCreateHooks.Erase();
    ChildHooks.Erase();
    CodeMaxHooks.Erase();
    MDIHooksNbr.Erase();
    MDIClientHooksNbr.Erase();
    ChildCreateHooksNbr.Erase();
    ChildHooksNbr.Erase();
    CodeMaxHooksNbr.Erase();
    ChildshWnd.Erase();
    ChildsLVPos.Erase();

    // Search arrays
    FillVarCheckTable();
    FillFuncCheckTable();
    PrepareHexConvTab();
    //PrepareHexTab();
    PrepareRcTab();
    PrepareRcLanguages();
    PrepareAccelerators();
    InitToolBarsIcons();
}

// -----------------------------------------------------------------------
// Set exceptions handler
void InstallExceptFilter(void)
{
    SetErrorMode(SEM_NOOPENFILEERRORBOX | SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    OldExceptFilter = SetUnhandledExceptionFilter(&MainFilter);
}

// -----------------------------------------------------------------------
// Exception handler
long CALLBACK MainFilter(EXCEPTION_POINTERS *Filter)
{
    CStr ExceptType;
    CStr DumpFile;
    CStr BufString;

    BufString = "Latest news report from the graveyard:\r";
    BufString = BufString + "Chromatic got killed by something or someone.\r\r";
    BufString = BufString + "You better call your local police station right now.\r";
    BufString = BufString + "(Ask for agent Scruggs).\r\r";
    BufString = BufString + "Before that, do you want to try to save your current work ?";
    if(MiscMsgBox((HWND) 0, BufString, MB_ERROR + MB_YESNO, Requesters) == IDYES)
    {
        MCMD_SaveAll();
        MCMD_SaveProject();
    }
    DumpFile = "Exception 0";
    DumpFile = DumpFile + StringHexNumberComplement(Filter->ExceptionRecord->ExceptionCode, 8);
    DumpFile = DumpFile + "h (";
    switch(Filter->ExceptionRecord->ExceptionCode)
    {
        case EXCEPTION_ACCESS_VIOLATION:
            DumpFile = DumpFile + "EXCEPTION_ACCESS_VIOLATION";
            break;
        case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
            DumpFile = DumpFile + "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
            break;
        case EXCEPTION_BREAKPOINT:
            DumpFile = DumpFile + "EXCEPTION_BREAKPOINT";
            break;
        case EXCEPTION_DATATYPE_MISALIGNMENT:
            DumpFile = DumpFile + "EXCEPTION_DATATYPE_MISALIGNMENT";
            break;
        case EXCEPTION_FLT_DENORMAL_OPERAND:
            DumpFile = DumpFile + "EXCEPTION_FLT_DENORMAL_OPERAND";
            break;
        case EXCEPTION_FLT_DIVIDE_BY_ZERO:
            DumpFile = DumpFile + "EXCEPTION_FLT_DIVIDE_BY_ZERO";
            break;
        case EXCEPTION_FLT_INEXACT_RESULT:
            DumpFile = DumpFile + "EXCEPTION_FLT_INEXACT_RESULT";
            break;
        case EXCEPTION_FLT_INVALID_OPERATION:
            DumpFile = DumpFile + "EXCEPTION_FLT_INVALID_OPERATION";
            break;
        case EXCEPTION_FLT_OVERFLOW:
            DumpFile = DumpFile + "EXCEPTION_FLT_OVERFLOW";
            break;
        case EXCEPTION_FLT_STACK_CHECK:
            DumpFile = DumpFile + "EXCEPTION_FLT_STACK_CHECK";
            break;
        case EXCEPTION_FLT_UNDERFLOW:
            DumpFile = DumpFile + "EXCEPTION_FLT_UNDERFLOW";
            break;
        case EXCEPTION_ILLEGAL_INSTRUCTION:
            DumpFile = DumpFile + "EXCEPTION_ILLEGAL_In_StrUCTION";
            break;
        case EXCEPTION_IN_PAGE_ERROR:
            DumpFile = DumpFile + "EXCEPTION_IN_PAGE_ERROR";
            break;
        case EXCEPTION_INT_DIVIDE_BY_ZERO:
            DumpFile = DumpFile + "EXCEPTION_INT_DIVIDE_BY_ZERO";
            break;
        case EXCEPTION_INT_OVERFLOW:
            DumpFile = DumpFile + "EXCEPTION_INT_OVERFLOW";
            break;
        case EXCEPTION_INVALID_DISPOSITION:
            DumpFile = DumpFile + "EXCEPTION_INVALID_DISPOSITION";
            break;
        case EXCEPTION_NONCONTINUABLE_EXCEPTION:
            DumpFile = DumpFile + "EXCEPTION_NONCONTINUABLE_EXCEPTION";
            break;
        case EXCEPTION_PRIV_INSTRUCTION:
            DumpFile = DumpFile + "EXCEPTION_PRIV_In_StrUCTION";
            break;
        case EXCEPTION_SINGLE_STEP:
            DumpFile = DumpFile + "EXCEPTION_SINGLE_STEP";
            break;
        case EXCEPTION_STACK_OVERFLOW:
            DumpFile = DumpFile + "EXCEPTION_STACK_OVERFLOW";
            break;
    }
    DumpFile = DumpFile + ")";
    DumpFile = DumpFile + DumpFile.New_Line();
    DumpFile = DumpFile + DumpFile.New_Line();
    DumpFile = DumpFile + "Address: 0";
    DumpFile = DumpFile + StringHexNumberComplement((long) Filter->ExceptionRecord->ExceptionAddress, 8);
    DumpFile = DumpFile + "h";
    DumpFile = DumpFile + DumpFile.New_Line();
    if(Filter->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
    {
        if(Filter->ExceptionRecord->ExceptionInformation[0] == 0)
        {
            DumpFile = DumpFile + "(Access type: READ - Virtual address: 0";
            DumpFile = DumpFile + StringHexNumberComplement(Filter->ExceptionRecord->ExceptionInformation[1], 8);
            DumpFile = DumpFile + "h)";
            DumpFile = DumpFile + DumpFile.New_Line();
        }
        else
        {
            DumpFile = DumpFile + "(Access type: WRITE - Virtual address: 0";
            DumpFile = DumpFile + StringHexNumberComplement(Filter->ExceptionRecord->ExceptionInformation[1], 8);
            DumpFile = DumpFile + "h)";
            DumpFile = DumpFile + DumpFile.New_Line();
        }
    }
    DumpFile = DumpFile + DumpFile.New_Line();
    DumpFile = DumpFile + "-------------------------------";
    DumpFile = DumpFile + DumpFile.New_Line();
    DumpFile = DumpFile + "CPU state:";
    DumpFile = DumpFile + DumpFile.New_Line();
    DumpFile = DumpFile + DumpFile.New_Line();

    DumpFile = DumpFile + "EIP: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Eip, 8).Get_String() + (CStr) "h";
    DumpFile = DumpFile + DumpFile.New_Line();
    DumpFile = DumpFile + DumpFile.New_Line();

    DumpFile = DumpFile + "EAX: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Eax, 8).Get_String() + (CStr) "h  EBX: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Ebx, 8).Get_String() + (CStr) "h  ECX: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Ecx, 8).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "EDX: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Edx, 8).Get_String() + (CStr) "h  ESI: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Esi, 8).Get_String() + (CStr) "h  EDI: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Edi, 8).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "EBP: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Ebp, 8).Get_String() + (CStr) "h  ESP: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Esp, 8).Get_String() + (CStr) "h  EFL: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->EFlags, 8).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String() + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "CS: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->SegCs, 4).Get_String() + (CStr) "h DS: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->SegDs, 4).Get_String() + (CStr) "h ES: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->SegEs, 4).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "FS: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->SegFs, 4).Get_String() + (CStr) "h GS: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->SegGs, 4).Get_String() + (CStr) "h SS: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->SegSs, 4).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String() + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "DR0: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Dr0, 8).Get_String() + (CStr) "h DR1: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Dr1, 8).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "DR2: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Dr2, 8).Get_String() + (CStr) "h DR3: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Dr3, 8).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "DR6: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Dr6, 8).Get_String() + (CStr) "h DR7: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->Dr7, 8).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String() + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "-------------------------------" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "FPU state:" + (CStr) DumpFile.New_Line().Get_String() + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "CTRL: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->FloatSave.ControlWord, 4).Get_String() + (CStr) "h STATUS: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->FloatSave.StatusWord, 4).Get_String() + (CStr) "h TAG: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->FloatSave.TagWord, 4).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String() + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "Err. Off.: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->FloatSave.ErrorOffset, 8).Get_String() + (CStr) "h Err. Sel.: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->FloatSave.ErrorSelector, 4).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "Dat. Off.: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->FloatSave.DataOffset, 8).Get_String() + (CStr) "h Dat. Sel.: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->FloatSave.DataSelector, 4).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String() + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "CR0Npx: 0" + (CStr) StringHexNumberComplement(Filter->ContextRecord->FloatSave.Cr0NpxState, 8).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String() + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "ST0: 0" + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[0]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[1]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[2]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[3]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[4]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[5]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[6]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[7]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[8]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[9]).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "ST1: 0" + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[10]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[11]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[12]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[13]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[14]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[15]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[16]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[17]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[18]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[19]).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "ST2: 0" + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[20]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[21]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[22]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[23]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[24]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[25]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[26]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[27]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[28]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[29]).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "ST3: 0" + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[30]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[31]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[32]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[33]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[34]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[35]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[36]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[37]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[38]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[39]).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "ST4: 0" + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[40]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[41]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[42]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[43]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[44]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[45]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[46]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[47]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[48]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[49]).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "ST5: 0" + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[50]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[51]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[52]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[53]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[54]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[55]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[56]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[57]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[58]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[59]).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "ST6: 0" + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[60]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[61]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[62]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[63]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[64]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[65]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[66]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[67]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[68]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[69]).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    DumpFile = DumpFile + "ST7: 0" + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[70]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[71]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[72]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[73]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[74]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[75]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[76]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[77]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[78]).Get_String();
    DumpFile = DumpFile + (CStr) StringHex8(Filter->ContextRecord->FloatSave.RegisterArea[79]).Get_String() + (CStr) "h" + (CStr) DumpFile.New_Line().Get_String();
    BufString = AppPath;
    BufString = BufString + "\\Crash.Txt";
    MSaveFile(BufString.Get_String(), (long) DumpFile.Get_String(), DumpFile.Len());
    MiscMsgBox(NULL, (CStr) "Created dump file: " + (CStr) AppPath + (CStr) "\\Crash.Txt", MB_INFORMATION, Requesters);
    return(EXCEPTION_CONTINUE_SEARCH);
}

// -----------------------------------------------------------------------
// Check directories structure and reconstruct it if necessary
void CheckDirectories(void)
{
    NotifDirStructureError = 0;
    CheckStockDirectory((CStr) AppPath + (CStr) "\\AddIns");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Backup");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Config");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Config\\UserMenus");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\DataBases");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\DownLoads");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Examples");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Filters");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Help");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Tools");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Languages");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Languages\\Scripts");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Projects");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Skins");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Skins\\ExtraCode");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Snippets");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Templates");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Wizards");
    CheckStockDirectory((CStr) AppPath + (CStr) "\\Scripts");
}

// -----------------------------------------------------------------------
// Check a directory
void CheckStockDirectory(CStr DirectoryName)
{
    // Check it
    if(FileIsDirectory(DirectoryName) == 0)
    {
        // Create it
        CreateDirectory(DirectoryName.Get_String(),NULL);
        // Notify user that a reconstruction occured
        NotifDirStructureError = 1;
    }
}
