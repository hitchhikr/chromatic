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
// Debugger.cpp: Unfinished source level debugger
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Classes/WADock.h"
#include "Debugger.h"
#include "MiscFunctions.h"
#include "Globals.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Menus.h"
#include "Main.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Splitters.h"
#include "AddIns.h"
#include "libdisasm/libdis.h"

// -----------------------------------------------------------------------
// Constants
#define BP_STICKY 1
#define BP_VOLATILE 2
#define BP_BRANCH 3

#define DEBUG_ERR_SYMBTABLE 1
#define DEBUG_ERR_NODEBUG 2

#define STEP_INTO 0
#define STEP_OVER 1
#define STEP_OUT 2

#define REG_EAX 0
#define REG_EBX 1
#define REG_ECX 2
#define REG_EDX 3
#define REG_ESI 4
#define REG_EDI 5
#define REG_EIP 6
#define REG_ESP 7
#define REG_EBP 8
#define REG_EFL 9
#define REG_CS 10
#define REG_DS 11
#define REG_ES 12
#define REG_FS 13
#define REG_GS 14
#define REG_SS 15
#define REG_ST0 16
#define REG_ST1 17
#define REG_ST2 18
#define REG_ST3 19
#define REG_ST4 20
#define REG_ST5 21
#define REG_ST6 22
#define REG_ST7 23
#define REG_FPUCTRL 24
#define REG_FPUSTAT 25
#define REG_FPUTAGS 26
#define REG_FPUCS 27
#define REG_FPUEIP 28
#define REG_FPUDS 29
#define REG_FPUEDO 30
#define REG_MM0 31
#define REG_MM1 32
#define REG_MM2 33
#define REG_MM3 34
#define REG_MM4 35
#define REG_MM5 36
#define REG_MM6 37
#define REG_MM7 38
#define REG_MM00 39
#define REG_MM01 40
#define REG_MM10 41
#define REG_MM11 42
#define REG_MM20 43
#define REG_MM21 44
#define REG_MM30 45
#define REG_MM31 46
#define REG_MM40 47
#define REG_MM41 48
#define REG_MM50 49
#define REG_MM51 50
#define REG_MM60 51
#define REG_MM61 52
#define REG_MM70 53
#define REG_MM71 54
#define REG_XMM0 55
#define REG_XMM1 56
#define REG_XMM2 57
#define REG_XMM3 58
#define REG_XMM4 59
#define REG_XMM5 60
#define REG_XMM6 61
#define REG_XMM7 62
#define REG_XMM00 63
#define REG_XMM01 64
#define REG_XMM02 65
#define REG_XMM03 66
#define REG_XMM10 67
#define REG_XMM11 68
#define REG_XMM12 69
#define REG_XMM13 70
#define REG_XMM20 71
#define REG_XMM21 72
#define REG_XMM22 73
#define REG_XMM23 74
#define REG_XMM30 75
#define REG_XMM31 76
#define REG_XMM32 77
#define REG_XMM33 78
#define REG_XMM40 79
#define REG_XMM41 80
#define REG_XMM42 81
#define REG_XMM43 82
#define REG_XMM50 83
#define REG_XMM51 84
#define REG_XMM52 85
#define REG_XMM53 86
#define REG_XMM60 87
#define REG_XMM61 88
#define REG_XMM62 89
#define REG_XMM63 90
#define REG_XMM70 91
#define REG_XMM71 92
#define REG_XMM72 93
#define REG_XMM73 94
#define REG_MXCSR 95
#define REG_NBR 96

long Reg_Icons[] =
{
    ICON_REGEAX,
    ICON_REGEBX,
    ICON_REGECX,
    ICON_REGEDX,
    ICON_REGESI,
    ICON_REGEDI,
    ICON_REGEIP,
    ICON_REGESP,
    ICON_REGEBP,
    ICON_REGEFL,
    ICON_REGCS,
    ICON_REGDS,
    ICON_REGES,
    ICON_REGFS,
    ICON_REGGS,
    ICON_REGSS,
    ICON_REGST0,
    ICON_REGST1,
    ICON_REGST2,
    ICON_REGST3,
    ICON_REGST4,
    ICON_REGST5,
    ICON_REGST6,
    ICON_REGST7,
    ICON_REGFPUCTRL,
    ICON_REGFPUSTAT,
    ICON_REGFPUTAGS,
    ICON_REGCS,
    ICON_REGEIP,
    ICON_REGDS,
    ICON_REGFPUEDO,
    ICON_REGMM0,
    ICON_REGMM1,
    ICON_REGMM2,
    ICON_REGMM3,
    ICON_REGMM4,
    ICON_REGMM5,
    ICON_REGMM6,
    ICON_REGMM7,
    ICON_REGMM0, -1,
    ICON_REGMM1, -1,
    ICON_REGMM2, -1,
    ICON_REGMM3, -1,
    ICON_REGMM4, -1,
    ICON_REGMM5, -1,
    ICON_REGMM6, -1,
    ICON_REGMM7, -1,
    ICON_REGXMM0,
    ICON_REGXMM1,
    ICON_REGXMM2,
    ICON_REGXMM3,
    ICON_REGXMM4,
    ICON_REGXMM5,
    ICON_REGXMM6,
    ICON_REGXMM7,
    ICON_REGXMM0, -1, -1, -1,
    ICON_REGXMM1, -1, -1, -1,
    ICON_REGXMM2, -1, -1, -1,
    ICON_REGXMM3, -1, -1, -1,
    ICON_REGXMM4, -1, -1, -1,
    ICON_REGXMM5, -1, -1, -1,
    ICON_REGXMM6, -1, -1, -1,
    ICON_REGXMM7, -1, -1, -1,
    ICON_REGMXCSR
};

// -----------------------------------------------------------------------
// Variables
HWND hDebugToolbar;
HWND hTreeViewDebug;
HWND hDebugPager;
HWND hDebugContainer;
HWND hRegistersContainer;
HWND hRegistersListView;
char *DebugToolbarToolTip[DEBUGTOOLBAR_STEPOUT + 1];
STARTUPINFO MyStartup;
PROCESS_INFORMATION DebuggedProc;
DEBUG_EVENT MyDebug;
CStr DeCStr;
BYTE OldOpCode;
const BYTE X86INT3 = 0xcc;
long AdrToRead;
unsigned long ExCode;
long ProcessBase;
CONTEXT MyContext;
BYTE AdrBuffer[3 + 1];
BYTE ESPBuffer[266 + 1];
BYTE SMBuffer[73 + 1];
long SnapRunning;
long Threadrunning;
HANDLE ChildhFile;
CList <HWND> DbWinH;
long CurrentDThread;
FILETIME lpProcCreationTm;
FILETIME lpProcExitTm;
FILETIME lpProcKModeTm;
FILETIME lpProcUModeTm;
SYSTEMTIME lpProcCreationSysTm;
SYSTEMTIME lpProcExitSysTm;
SYSTEMTIME lpProcKModeSysTm;
SYSTEMTIME lpProcUModeSysTm;
FILETIME lpThreadCreationTm;
FILETIME lpThreadExitTm;
FILETIME lpThreadKModeTm;
FILETIME lpThreadUModeTm;
SYSTEMTIME lpThreadCreationSysTm;
SYSTEMTIME lpThreadExitSysTm;
SYSTEMTIME lpThreadKModeSysTm;
SYSTEMTIME lpThreadUModeSysTm;
long InException;
long SysTabItem;
long EntryPoint;

DWORD WasBreakPoint;

int ScheduleQuit;
int InStepMode;
int InStepType;
CStr DebuggedFile;
IMAGEHLP_LINE ModuleLine;

CList <LPSNAPDLL> DebDlls;
CList <LPSNAPPROCESS> DebProcesses;
CList <LPSNAPBP> DebBreakPoints;
CList <PIMAGEHLP_MODULE> DebModules;
CList <HWND> DebWindows;

HTREEITEM hTreeViewDebugRoot;
HTREEITEM hTreeViewDebugProcesses;
HTREEITEM hTreeViewDebugDLLs;

// -----------------------------------------------------------------------
// Functions declarations
long SetBreakPoint(CList <LPSNAPBP> *BPList, DWORD BPAddress, int BPType);
long RestoreBreakPoint(DWORD BPAddress);
long KillBreakPoint(DWORD BPAddress);
void FlushBranchBreakPoints(void);
int BreakPointType(DWORD BPAddress);
void StopThread(long ThreadID);
void DebugStepBack(long dwThreadID);
void DebugSetEIP(long dwThreadID, DWORD Address);
void GetCurrentContext(long dwThreadID, LPCONTEXT Context);
LPCONTEXT GetLastContext(long dwThreadID);
void SetCurrentContext(long dwThreadID, LPCONTEXT Context);
DWORD GetCurrentEIP(long dwThreadID);
DWORD GetCurrentESP(long dwThreadID);
DWORD GetCurrentWord(long dwThreadID, DWORD Address);
DWORD GetCurrentDWord(long dwThreadID, DWORD Address);
long ReadThreadbytes(long dwThreadID, DWORD Address, BYTE *Dest, DWORD Bytes);
void SetSingleStep(long ThreadID);
void ClearSingleStep(long ThreadID);
void RunThread(long ThreadID);
void UpdateRegs(long dwThreadID);
LRESULT CALLBACK TreeviewDebugContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RegistersContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int DisplaySourceLine(long ThreadID, DWORD Address);
void SetTopFromLine(HWND hWnd, long LineNumber);
int IsSourceLine(DWORD Address);
int GetSourceLineNumber(DWORD Address);
void BringThreadToForeGround(void);
int CALLBACK EnumThreadWins(HWND hWnd, long lParam);
void SetNextBreak(int StepOver);
int BreakOnCaller(void);
DWORD GetCaller(void);
long CALLBACK DebuggedFilesProc(HWND hWnd);

// -----------------------------------------------------------------------
// Open a process and trap the OutDebugString() function
long WaitForDebugOutput(CStr FileName, CStr Arguments)
{
    long ThreadClass = 0;
    long i = 0;
    long j = 0;
    long BtnPos = 0;
    long No_Debug = 0;
    CStr DebugDir;
    SECURITY_ATTRIBUTES DebSecAt;
    PSAPI_WS_WATCH_INFORMATION MyWatch;
    unsigned long VolatValue = 0;
    unsigned long VolatValue2 = 0;
    CStr BufString;
    LPSNAPDLL NewDll;
    LPSNAPPROCESS NewProcess;
    CStr ProgDir;
    DWORD BaseModuleAddr = 0;
    PIMAGEHLP_MODULE NewModule;
    RECT Rec;
    LPSNAPPROCESS hProcessThread;
    //HANDLE hCurProcess;
    LPSNAPPROCESS ParentProcess;

    if(Threadrunning == 1) goto DoubleInstance;
    if(FileName.Len() == 0) goto DoubleInstance;
    FileName = FileName.Trim();
    memset(&MyStartup, 0, sizeof(MyStartup));
    memset(&DebSecAt, 0, sizeof(DebSecAt));
    memset(&DebuggedProc, 0, sizeof(DebuggedProc));
    DebSecAt.nLength = sizeof(DebSecAt);
    DebSecAt.lpSecurityDescriptor = 0;
    DebSecAt.bInheritHandle = 0;
    MyStartup.cb = sizeof(MyStartup);
    MyStartup.dwFlags = STARTF_USESHOWWINDOW;
    MyStartup.wShowWindow = SW_SHOWDEFAULT;
    MyStartup.lpTitle = "Chromatic";
    if(SnapRunning == 1) goto DoubleInstance;
    // Create application as debugged process
    DebugDir = FileGetDirectory(FileName);
    if(DebugDir.Len() == 0) DebugDir = CurDir();
    if(strcmp(DebugDir.Right(1).Get_String(), "\\") == 0) DebugDir = DebugDir.Mid(1, DebugDir.Len() - 1);
    Arguments = Arguments.Trim();
    Arguments = StringReplace(Arguments, "\"", "", 1, -1, Binary_Compare);
    if(Arguments.Len() != 0) Arguments = " " + (CStr) Arguments;
    BufString = FileName + (CStr) Arguments;
    DebuggedFile = FileName;
    if(CreateProcess(FileName.Get_String(),
                     BufString.Get_String(),
                     &DebSecAt, &DebSecAt, 0,
                     DEBUG_PROCESS, NULL,
                     DebugDir.Get_String(),
                     &MyStartup, &DebuggedProc) == 0)
    {
DoubleInstance:
        return(0);
    }
    SnapRunning = 1;
    EntryPoint = 0;
    WasBreakPoint = 0;

    hRegistersContainer = CreateDockingBox("Registers", &RegistersContainerProc, hMDIform.hDock, MainIniFile.Get_String());
    //hDumpRegisters = CreateDumpBox(0, 0, 10, 10, hRegistersContainer, 0, GDIGetCourierNew9(), 0, 10, 10, 10, 0, WS_HSCROLL | WS_VSCROLL, WS_EX_STATICEDGE);
    hRegistersListView = CreateListView(0, 0, 10, 10, hRegistersContainer, 13, GlobalImageList1, NULL, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_EDITLABELS | LVS_SINGLESEL | WS_TABSTOP | LVS_NOCOLUMNHEADER, WS_EX_STATICEDGE);
    ControlSetFont(hRegistersListView, WACourrierNewFont9);
    ListViewAddCol(hRegistersListView, "", 10, 0);
    // Add the registers into the listview
    for(i = 0; i < REG_NBR; i++)
    {
        ListViewAddItem(hRegistersListView, "", ListViewItemCount(hRegistersListView), Reg_Icons[i]);
    }
          
    DockingBoxShow(hRegistersContainer, GetMDIWidth(), GetMDIHeight(), 300, 120, DOCKINGBOX_BOTTOM);
    SetRegistersDockMenuState(ShowRegistersDockWin);
    
    hDebugContainer = CreateDockingBox("Debug", &TreeviewDebugContainerProc, hMDIform.hDock, MainIniFile.Get_String());
    hTreeViewDebug = CreateTreeView(0, 23, ControlWidth(hDebugContainer), 10, hDebugContainer, 14, GlobalImageList1, 0, TVS_SHOWSELALWAYS, WS_EX_STATICEDGE);
    TreeViewSetIndent(hTreeViewDebug, 1);
    CreateSnapTreeView();

    hDebugToolbar = CreateToolBar(0, 0, 0, 23, hDebugContainer, GlobalImageList1, 1, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | TBS_VERT | TBSTYLE_WRAPABLE, 0);
    SendMessage(hDebugToolbar, TB_SETROWS, 0x60001, (long) &Rec);
    ToolBarAddButton(hDebugToolbar, "", DEBUGTOOLBAR_RUN, ICON_RUNPROJECT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDebugToolbar, "", DEBUGTOOLBAR_PAUSE, ICON_PAUSE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDebugToolbar, "", DEBUGTOOLBAR_STOP, ICON_STOP, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddSeparator(hDebugToolbar, 0);
    ToolBarAddButton(hDebugToolbar, "", DEBUGTOOLBAR_STEPINTO, ICON_STEPINTO, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDebugToolbar, "", DEBUGTOOLBAR_STEPOVER, ICON_STEPOVER, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDebugToolbar, "", DEBUGTOOLBAR_STEPOUT, ICON_STEPOUT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    
    hDebugPager = CreatePager(0, 0, 0, 23, hDebugContainer, hDebugToolbar, 0, PGS_HORZ);
    DebugToolbarToolTip[DEBUGTOOLBAR_RUN] = "Run debugged process";
    DebugToolbarToolTip[DEBUGTOOLBAR_PAUSE] = "Pause debugging (Ctrl+Break)";
    DebugToolbarToolTip[DEBUGTOOLBAR_STOP] = "Stop debugging (Ctrl+C)";
    DebugToolbarToolTip[DEBUGTOOLBAR_STEPINTO] = "Step into";
    DebugToolbarToolTip[DEBUGTOOLBAR_STEPOVER] = "Step over";
    DebugToolbarToolTip[DEBUGTOOLBAR_STEPOUT] = "Step out";

    MenuEnable(hViewMenuDisplay, MENU_VIEW_DEBUGDOCK_ID, 1);
    MenuEnable(hViewMenuDisplay, MENU_VIEW_REGISTERSDOCK_ID, 1);
    DockingBoxShow(hDebugContainer, GetMDIWidth(), GetMDIHeight(), 216, 230, DOCKINGBOX_LEFT);
    SetDebugDockMenuState(ShowDebugDockWin);

    // Clear & select the debug tab
    if(AutoClearOut == 1) ClearDebug();
    SysTabSetFocusItem(hOutputSysTab, 1);

    MiscDoEvents(hMDIform.hClient, hGlobAccelerators, hMDIform.hWnd);

    x86_init(opt_none, NULL, NULL);

    // Init the FPU for full 80 bits calculations
    _asm
    {
        fninit
    }

    if(SymInitialize(DebuggedProc.hProcess, NULL, FALSE) == 0)
    {
        WriteToDebug("Can't initialize symbols handler.");
        KillProcess();
        goto TchaoProc;
    }

    InStepMode = 0;
    SymSetOptions(SYMOPT_LOAD_LINES | SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS | SYMOPT_EXACT_SYMBOLS | SYMOPT_FAIL_CRITICAL_ERRORS);

    InException = 0;
    Threadrunning = 1;
    CurrentDThread = DebuggedProc.dwThreadId;
    SnapRunstate();

    // -------------------------------------------
    // Main debugger loop

WaitEnd:
    memset(&MyDebug, 0, sizeof(DEBUG_EVENT));
    // Try to avoid double events (due to MiscDoEvents)
    if(Threadrunning == 1)
    {
        WaitForDebugEvent(&MyDebug, 0);
    }
    MiscDoEvents(hMDIform.hClient, hGlobAccelerators, hMDIform.hWnd);
    switch(MyDebug.dwDebugEventCode)
    {
        // A new process has been created
        // (should be unique as far as i know)
        case CREATE_PROCESS_DEBUG_EVENT:
            CurrentDThread = MyDebug.dwThreadId;
            NewProcess = (LPSNAPPROCESS) calloc(sizeof(SNAPPROCESS), 1);
            memset(NewProcess, 0, sizeof(SNAPPROCESS));

            NewProcess->hFile = MyDebug.u.CreateProcessInfo.hFile;
            NewProcess->hProcess = MyDebug.u.CreateProcessInfo.hProcess;
            NewProcess->dwProcessId = MyDebug.dwProcessId;
            NewProcess->PriorityClass = GetPriorityClass(MyDebug.u.CreateProcessInfo.hProcess);
            NewProcess->WorkSize1 = 0;
            NewProcess->WorkSize2 = 0;
            if(GetProcessWorkingSetSize(MyDebug.u.CreateProcessInfo.hProcess, &VolatValue, &VolatValue2) != 0)
            {
                NewProcess->WorkSize1 = VolatValue;
                NewProcess->WorkSize2 = VolatValue2;
            }
            switch(GetProcessBoost(MyDebug.u.CreateProcessInfo.hProcess))
            {
                case 1:
                    NewProcess->PriorityBoost = 0;
                    break;
                case 2:
                    NewProcess->PriorityBoost = 1;
                    break;
            }
            if(GetProcessTime(MyDebug.u.CreateProcessInfo.hProcess) == 1)
            {
                NewProcess->CreationTime = lpProcCreationSysTm;
            }
            NewProcess->ProcVersion = GetProcessVersion(MyDebug.dwProcessId);
            if(Windows2K == 1)
            {
                if(Ptr_InitializeProcessForWsWatch(MyDebug.u.CreateProcessInfo.hProcess) != 0)
                {
                    NewProcess->ProcMonitor = 1;
                }
                else
                {
                    NewProcess->ProcMonitor = 0;
                }
            }
            NewProcess->hThread = MyDebug.u.CreateProcessInfo.hThread;
            NewProcess->dwThreadId = MyDebug.dwThreadId;
            NewProcess->ThreadPriority = GetThreadPriority(MyDebug.u.CreateProcessInfo.hThread);
            NewProcess->ThreadStartAddress = MyDebug.u.CreateProcessInfo.lpStartAddress;
            EntryPoint = (long) MyDebug.u.CreateProcessInfo.lpStartAddress;
            // Sticky breakpoint at entry point
            SetBreakPoint(&DebBreakPoints, EntryPoint, BP_BRANCH);
            NewProcess->ImageBase = MyDebug.u.CreateProcessInfo.lpBaseOfImage;
            switch(GetThreadBoost(MyDebug.u.CreateProcessInfo.hThread))
            {
                case 1:
                    NewProcess->ThreadPriorityBoost = 0;
                    break;
                case 2:
                    NewProcess->ThreadPriorityBoost = 1;
                    break;
            }
            if(GetThreadTime(MyDebug.u.CreateProcessInfo.hThread) == 1)
            {
                NewProcess->ThreadCreationTime = lpThreadCreationSysTm;
            }
            //THREAD_ALL_ACCESS
            DuplicateHandle(GetCurrentProcess(), NewProcess->hThread, GetCurrentProcess(), &NewProcess->hThread, THREAD_ALL_ACCESS, 0, 0);
            WriteToDebug("Create process: 0x" + (CStr) StringHexNumberComplement((long) MyDebug.u.CreateProcessInfo.hProcess, 8));
            WriteToDebug("Create thread: 0x" + (CStr) StringHexNumberComplement((long) NewProcess->hThread, 8));

            NewProcess->ProcessTreeViewEntry = TreeViewAddItem(hTreeViewDebug, "Process: 0x" + (CStr) StringHex32((long) NewProcess->hProcess), hTreeViewDebugProcesses, 0, ICON_PROCESS, ICON_PROCESS, 0, 1);
            NewProcess->DatasTreeViewEntry = TreeViewAddItem(hTreeViewDebug, "Thread: 0x" + (CStr) StringHex32((long) NewProcess->hThread) + (CStr) " (Start address: 0x" + (CStr) StringHex32((long) NewProcess->ThreadStartAddress) + (CStr) ")", NewProcess->ProcessTreeViewEntry, 0, ICON_THREAD, ICON_THREAD, 0, 1);
            DebProcesses.Add(NewProcess);

            // Try to load the symbol table
            NewModule = (PIMAGEHLP_MODULE) calloc(sizeof(IMAGEHLP_MODULE), 1);
            memset(NewModule, 0, sizeof(IMAGEHLP_MODULE));
            NewModule->SizeOfStruct = sizeof(IMAGEHLP_MODULE);
            BaseModuleAddr = SymLoadModule(DebuggedProc.hProcess, NULL, BufString.Get_String(), NULL, 0, 0);
            ProcessBase = BaseModuleAddr;
            SymGetModuleInfo(DebuggedProc.hProcess, (DWORD) BaseModuleAddr, NewModule);
            DebModules.Add(NewModule);
            if(BaseModuleAddr == 0)
            {
                No_Debug = DEBUG_ERR_SYMBTABLE;
                KillProcess();
            }
            else
            {
                if(NewModule->SymType != SymCoff &&
                   NewModule->SymType != SymCv &&
                   NewModule->SymType != SymDia &&
                   NewModule->SymType != SymPdb)
                {
                    No_Debug = DEBUG_ERR_NODEBUG;
                    KillProcess();
                }
            }
            GetCurrentContext(CurrentDThread, &NewProcess->LastContext);
            NewProcess->LastContext.Esi = 0;
            NewProcess->LastContext.Edi = 0;
            // Clear the FPU, MMX & XMM registers
            for(i = 0; i < SIZE_OF_80387_REGISTERS; i++)
            {
                NewProcess->LastContext.FloatSave.RegisterArea[i] = 0;
            }
            for(i = (10 * 16); i < (10 * 16) + (8 * 16); i++)
            {
                NewProcess->LastContext.ExtendedRegisters[i] = 0;
            }
            SetCurrentContext(CurrentDThread, &NewProcess->LastContext);
            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            break;

        // A new thread has been created
        case CREATE_THREAD_DEBUG_EVENT:
            CurrentDThread = MyDebug.dwThreadId;
            NewProcess = (LPSNAPPROCESS) calloc(sizeof(SNAPPROCESS), 1);
            memset(NewProcess, 0, sizeof(SNAPPROCESS));
            NewProcess->hThread = MyDebug.u.CreateThread.hThread;
            NewProcess->ThreadPriority = GetThreadPriority(MyDebug.u.CreateThread.hThread);
            NewProcess->ThreadStartAddress = MyDebug.u.CreateThread.lpStartAddress;
            NewProcess->hThreadLocalBase = MyDebug.u.CreateThread.lpThreadLocalBase;
            switch(GetThreadBoost(MyDebug.u.CreateThread.hThread))
            {
                case 1:
                    NewProcess->ThreadPriorityBoost = 0;
                    break;
                case 2:
                    NewProcess->ThreadPriorityBoost = 1;
                    break;
            }
            if(GetThreadTime(MyDebug.u.CreateThread.hThread) == 1)
            {
                NewProcess->ThreadCreationTime = lpThreadCreationSysTm;
            }

            //hCurProcess = MyDebug.dwProcessId;
            DuplicateHandle(GetCurrentProcess(), NewProcess->hThread, GetCurrentProcess(), &NewProcess->hThread, THREAD_ALL_ACCESS, 0, 0);
            WriteToDebug("Create thread: 0x" + (CStr) StringHexNumberComplement((long) NewProcess->hThread, 8));
            
            // Search the process
            hProcessThread = 0;
            
            for(i = 0; i < DebProcesses.Amount(); i++)
            {
                ParentProcess = DebProcesses.Get(i)->Content;
                if(ParentProcess->dwProcessId == MyDebug.dwProcessId)
                {
                    hProcessThread = ParentProcess;
                    break;
                }
            }
            if(hProcessThread)
            {
                NewProcess->ProcessTreeViewEntry = hProcessThread->ProcessTreeViewEntry;
                NewProcess->DatasTreeViewEntry = TreeViewAddItem(hTreeViewDebug, "Thread: 0x" + (CStr) StringHex32((long) NewProcess->hThread) + (CStr) " (Start address: 0x" + (CStr) StringHex32((long) NewProcess->ThreadStartAddress) + (CStr) ")", NewProcess->ProcessTreeViewEntry, 0, ICON_THREAD, ICON_THREAD, 0, 1);
                DebProcesses.Add(NewProcess);
            }
            GetCurrentContext(CurrentDThread, &NewProcess->LastContext);
            NewProcess->LastContext.Esi = 0;
            NewProcess->LastContext.Edi = 0;
            // Clear the FPU, MMX & XMM registers
            for(i = 0; i < SIZE_OF_80387_REGISTERS; i++)
            {
                NewProcess->LastContext.FloatSave.RegisterArea[i] = 0;
            }
            for(i = (10 * 16); i < (10 * 16) + (8 * 16); i++)
            {
                NewProcess->LastContext.ExtendedRegisters[i] = 0;
            }
            SetCurrentContext(CurrentDThread, &NewProcess->LastContext);
            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            break;

        // A new dll has been loaded
        case LOAD_DLL_DEBUG_EVENT:
            CurrentDThread = MyDebug.dwThreadId;
            NewDll = (LPSNAPDLL) calloc(sizeof(SNAPDLL), 1);
            memset(NewDll, 0, sizeof(SNAPDLL));
            NewDll->hFile = MyDebug.u.LoadDll.hFile;
            NewDll->lpBaseOfDll = MyDebug.u.LoadDll.lpBaseOfDll;
            NewDll->DllName = new CStr;
            // Add the dll datas
            BufString = BufString.String(MAX_PATH, 1);
            if(GetModuleFileName((HMODULE) MyDebug.u.LoadDll.lpBaseOfDll, BufString.Get_String(), MAX_PATH) == 0)
            {
                // Empty it
                BufString = "0x" + (CStr) StringHexNumberComplement((long) MyDebug.u.LoadDll.lpBaseOfDll, 8);
            }
            NewDll->DllName->Set_String(BufString.Get_String());
            NewDll->TreeViewEntry = TreeViewAddItem(hTreeViewDebug, BufString, hTreeViewDebugDLLs, 0, ICON_STATIC, ICON_STATIC, 0, 1);
            DebDlls.Add(NewDll);

            // Try to load the symbol table
            NewModule = (PIMAGEHLP_MODULE) calloc(sizeof(IMAGEHLP_MODULE), 1);
            memset(NewModule, 0, sizeof(IMAGEHLP_MODULE));
            BaseModuleAddr = SymLoadModule(DebuggedProc.hProcess, NULL, BufString.Get_String(), NULL, 0, 0);
            if(BaseModuleAddr == 0)
            {
                WriteToDebug((CStr) "Load DLL: " + BufString + (CStr) " (No exports)");
            }
            else
            {
                NewModule->SizeOfStruct = sizeof(IMAGEHLP_MODULE);
                SymGetModuleInfo(DebuggedProc.hProcess, BaseModuleAddr, NewModule);
                DebModules.Add(NewModule);
                WriteToDebug((CStr) "Load DLL: " + BufString + (CStr) " (Exports loaded)");
            }
            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            break;

        // A dll has been unloaded
        case UNLOAD_DLL_DEBUG_EVENT:
            CurrentDThread = MyDebug.dwThreadId;
            for(i = 0; i < DebDlls.Amount(); i++)
            {
                NewDll = DebDlls.Get(i)->Content;
                if(NewDll->lpBaseOfDll == MyDebug.u.UnloadDll.lpBaseOfDll)
                {
                    WriteToDebug("Unload DLL: " + (CStr) NewDll->DllName);
                    // Delete it from the treeview
                    TreeViewRemoveItem(hTreeViewDebug, NewDll->TreeViewEntry);
                    // Delete it from the list
                    for(j = 0; j < DebModules.Amount(); j++)
                    {
                        NewModule = DebModules.Get(j)->Content;
                        if((long) NewModule->BaseOfImage == (long) NewDll->lpBaseOfDll)
                        {
                            SymUnloadModule(DebuggedProc.hProcess, (DWORD) NewModule->BaseOfImage);
                            DebModules.Del(j);
                            break;
                        }
                    }
                    DebDlls.Del(i);
                    break;
                }
            }
            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            break;

        // An exception has been encountered
        case EXCEPTION_DEBUG_EVENT:
            CurrentDThread = MyDebug.dwThreadId;
            InException = 1;
            if(MyDebug.u.Exception.ExceptionRecord.ExceptionCode != EXCEPTION_BREAKPOINT)
            {
                switch(MyDebug.u.Exception.ExceptionRecord.ExceptionCode)
                {
                    case EXCEPTION_SINGLE_STEP:
                        break;
                    default:
                        WriteToDebug("In process: 0x" + (CStr) StringHexNumberComplement((long) GetProcessHandle(MyDebug.dwProcessId), 8));
                        WriteToDebug("In thread: 0x" + (CStr) StringHexNumberComplement((long) GetThreadHandle(CurrentDThread), 8));
                        WriteToDebug("*** Exception raised at address: 0x" + (CStr) StringHex32((long) MyDebug.u.Exception.ExceptionRecord.ExceptionAddress).Get_String());
                }
                if(Windows2K == 1)
                {
                    if(Ptr_GetWsChanges(DebuggedProc.hProcess, &MyWatch, sizeof(MyWatch)) != 0)
                    {
                        WriteToDebug("*** Instruction at: 0x" + (CStr) StringHex32((long) MyWatch.FaultingPc).Get_String());
                        WriteToDebug("*** Page at: 0x" + (CStr) StringHex32((long) MyWatch.FaultingVa).Get_String());
                    }
                }
                switch(MyDebug.u.Exception.ExceptionRecord.ExceptionCode)
                {
                    case EXCEPTION_ACCESS_VIOLATION:
                        WriteToDebug("*** Access violation");
                        if(MyDebug.u.Exception.ExceptionRecord.NumberParameters != 0)
                        {
                            switch(MyDebug.u.Exception.ExceptionRecord.ExceptionInformation[0])
                            {
                                case 0:
                                    WriteToDebug("*** Reading attempt from address: 0x" + (CStr) BufString.Hex_To_String(MyDebug.u.Exception.ExceptionRecord.ExceptionInformation[1]).Get_String());
                                    break;
                                case 1:
                                    WriteToDebug("*** Writing attempt to address: 0x" + (CStr) BufString.Hex_To_String(MyDebug.u.Exception.ExceptionRecord.ExceptionInformation[1]).Get_String());
                                    break;
                            }
                        }
                        break;
                    case EXCEPTION_DATATYPE_MISALIGNMENT:
                        WriteToDebug("*** Data type misalignment");
                        break;
                    case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
                        WriteToDebug("*** Array bounds exceeded");
                        break;
                    case EXCEPTION_FLT_DENORMAL_OPERAND:
                        WriteToDebug("*** Floating point operand too small");
                        break;
                    case EXCEPTION_FLT_DIVIDE_BY_ZERO:
                        WriteToDebug("*** Floating point divide by zero");
                        break;
                    case EXCEPTION_FLT_INEXACT_RESULT:
                        WriteToDebug("*** Floating point result can't be represented as decimal fraction");
                        break;
                    case EXCEPTION_FLT_INVALID_OPERATION:
                        WriteToDebug("*** Floating point invalid operation");
                        break;
                    case EXCEPTION_FLT_OVERFLOW:
                        WriteToDebug("*** Floating point overflow");
                        break;
                    case EXCEPTION_FLT_STACK_CHECK:
                        WriteToDebug("*** Floating point stack underflow/overflow");
                        break;
                    case EXCEPTION_FLT_UNDERFLOW:
                        WriteToDebug("*** Floating point underflow");
                        break;
                    case EXCEPTION_INT_DIVIDE_BY_ZERO:
                        WriteToDebug("*** Divide by zero");
                        break;
                    case EXCEPTION_INT_OVERFLOW:
                        WriteToDebug("*** Integer overflow");
                        break;
                    case EXCEPTION_ILLEGAL_INSTRUCTION:
                        WriteToDebug("*** Illegal instruction");
                        break;
                    case EXCEPTION_PRIV_INSTRUCTION:
                        WriteToDebug("*** Privileged instruction");
                        break;
                    case EXCEPTION_SINGLE_STEP:
                        // Coming from a breakpoint ?
                        // Restore the code before everything else
                        // so it can be executed
                        if(WasBreakPoint)
                        {
                            // Handle correctly the breakpoint
                            // for that address
                            // (sticky breakpoints are restored)
                            // (volatile breakpoints are destroyed)
                            if(BreakPointType(GetCurrentEIP(CurrentDThread)) == BP_STICKY)
                            {
                                // Set it back if it was a sticky one
                                // (ie. defined by user)
                                SetBreakPoint(&DebBreakPoints, GetCurrentEIP(CurrentDThread), BP_STICKY);
                            //} else {
                            //  KillBreakPoint(GetCurrentEIP(CurrentDThread));
                            }
                            // Remove all other possible breakpoints
                            FlushBranchBreakPoints();
                            WasBreakPoint = 0;
                        }
                        // Was triggered from run command ?
                        if(InStepMode == 0)
                        {
                            // Run!
                            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
                            goto WaitEnd;
                        }
                        ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
                        RunThread(CurrentDThread);
                        goto WaitEnd;
                    case EXCEPTION_IN_PAGE_ERROR:
                        WriteToDebug("*** Access to inexistant page");
                        break;
                    case EXCEPTION_STACK_OVERFLOW:
                        WriteToDebug("*** Stack overflow");
                        break;
                    case EXCEPTION_INVALID_DISPOSITION:
                        WriteToDebug("*** Invalid disposition in exception dispatcher");
                        break;
                    case EXCEPTION_NONCONTINUABLE_EXCEPTION:
                        WriteToDebug("*** Dead end exception");
                        break;
                    case DBG_CONTROL_C:
                        WriteToDebug("*** Control+C notification");
                        break;
                    case DBG_CONTROL_BREAK:
                        WriteToDebug("*** Control+Break notification");
                        break;
                    default:
                        if(MyDebug.u.Exception.dwFirstChance)
                        {
                            WriteToDebug("*** First-chance exception");
                    //      ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
                    //      goto Continue_Unknown;
                    //  } else {
                        }
                        WriteToDebug("*** Unknown exception: " + (CStr) MyDebug.u.Exception.ExceptionRecord.ExceptionCode);
                    //      break;
                        //}
                }
                if(MyDebug.u.Exception.ExceptionRecord.ExceptionFlags == EXCEPTION_NONCONTINUABLE)
                {
                    // Can't continue after that
                    SnapDeadEndstate();
                }
                else
                {
                    SnapPausestate();
                }
                switch(MyDebug.u.Exception.ExceptionRecord.ExceptionCode)
                {
                    case EXCEPTION_SINGLE_STEP:
                    case DBG_CONTROL_C:
                    case DBG_CONTROL_BREAK:
                        ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
                        break;
                    default:
                        // Other exception aren't handled
                        ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
                        break;
                }
                StopThread(CurrentDThread);
                UpdateRegs(CurrentDThread);
            }
            else
            {
                // Save breakpoint address for backup in single step
                WasBreakPoint = (DWORD) MyDebug.u.Exception.ExceptionRecord.ExceptionAddress;
                // Restore old code
                StopThread(CurrentDThread);
                if(RestoreBreakPoint(WasBreakPoint) != 0)
                {
                    // Step back now
                    DebugStepBack(CurrentDThread);
                }
                UpdateRegs(CurrentDThread);
                if(DisplaySourceLine(MyDebug.dwThreadId, GetCurrentEIP(CurrentDThread)))
                {
                    // Don't notify if it wasn't defined by the user
                    // (take hardcoded int3 into account)
                    WasBreakPoint = BreakPointType(WasBreakPoint);
                    if(WasBreakPoint != BP_VOLATILE && WasBreakPoint != BP_BRANCH)
                    {
                        WriteToDebug("*** Breakpoint reached at address: 0x" + (CStr) StringHex32((long) MyDebug.u.Exception.ExceptionRecord.ExceptionAddress).Get_String());
                    }
                    SnapPausestate();
                }
                else
                {
                    RunThread(CurrentDThread);
                }
                WasBreakPoint = 1;
                ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            }
            break;

        // A string has been passed to us
        case OUTPUT_DEBUG_STRING_EVENT:
            CurrentDThread = MyDebug.dwThreadId;
            if(MyDebug.u.DebugString.nDebugStringLength > 1)
            {
                DeCStr = DeCStr.String(MyDebug.u.DebugString.nDebugStringLength - 1, 1);
                AdrToRead = (long) MyDebug.u.DebugString.lpDebugStringData;
                if(DeCStr.Right(1).Asc() == 10) DeCStr = DeCStr.Mid(1, DeCStr.Len() - 1);
                WriteToDebug(DeCStr);
            }
            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            break;

        // A thread has been terminated
        case EXIT_THREAD_DEBUG_EVENT:
            CurrentDThread = MyDebug.dwThreadId;
            WriteToDebug("Thread exited with code : 0x" + (CStr) StringHexNumberComplement((long) MyDebug.u.ExitThread.dwExitCode, 8));
            RemoveThread(MyDebug.dwThreadId);
            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            break;

        // Process has been terminated
        // (Should occurs only one time as far as i know)
        case EXIT_PROCESS_DEBUG_EVENT:
            CurrentDThread = MyDebug.dwThreadId;
            WriteToDebug("Process exited with code : 0x" + (CStr) StringHexNumberComplement((long) MyDebug.u.ExitProcess.dwExitCode, 8));
            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            SnapRunning = 0;
            goto TchaoProc;

        // Other events aren't handled: pass'em to the system
        default:
            if(Threadrunning == 1)
            {
                ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_EXCEPTION_NOT_HANDLED);
            }
            break;
    }
    if(SnapRunning == 0) goto TchaoProc;
    goto WaitEnd;
TchaoProc:

    x86_cleanup();

    // -----------------------------------
    // End of debugger loop

    switch(No_Debug)
    {
        case DEBUG_ERR_SYMBTABLE:
            WriteToDebug("Can't load symbol table.");
            break;
        case DEBUG_ERR_NODEBUG:
            WriteToDebug("File contains no suitable debugging informations.");
            break;
    }

    // Remove the hgihlighted line if necessary
    if(NbForms != 0)
    {
        // Remove them all
        WAMMEnumSourceWindows((long) &DebuggedFilesProc);
        if(ShowH == 1)
        {
            ChildStruct = LoadStructure(CurrentForm);
            LoadCurrentSel(ChildStruct->hChildCodeMax);
            WritePositionInStatus(ChildStruct->hChildCodeMax);
            CM_SetHighlightedLine(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine);
        } 
    }

    // Remove the docking windows
    DockingBoxRemove(hDebugContainer);
    DockingBoxRemove(hRegistersContainer);
    MenuEnable(hViewMenuDisplay, MENU_VIEW_DEBUGDOCK_ID, 0);
    MenuEnable(hViewMenuDisplay, MENU_VIEW_REGISTERSDOCK_ID, 0);

    // Clear the lists
    DebDlls.Erase();
    // Free loaded modules
    for(i = 0; i < DebModules.Amount(); i++)
    {
        NewModule = DebModules.Get(i)->Content;
        SymUnloadModule(DebuggedProc.hProcess, (DWORD) NewModule->BaseOfImage);
    }
    if(DebuggedProc.hProcess) SymCleanup(DebuggedProc.hProcess);

    // Free processes handles
    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        NewProcess = DebProcesses.Get(i)->Content;
        if(NewProcess->hFile != 0) FileClose(NewProcess->hFile);
    }
    DebProcesses.Erase();
    DebModules.Erase();
    DebBreakPoints.Erase();
    DebWindows.Erase();
    // Close the file handle
    if(DebuggedProc.hThread) CloseHandle(DebuggedProc.hThread);
    if(DebuggedProc.hProcess) CloseHandle(DebuggedProc.hProcess);
    Threadrunning = 0;
    SnapRunning = 0;
    // Complete shutdown was required
    if(ScheduleQuit == 1) ControlClose(hMDIform.hWnd);
    return(1);
}

// -----------------------------------------------------------------------
// Callback for WAMMEnumChilds
long CALLBACK DebuggedFilesProc(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    // Remove it
    CM_SetHighlightedLine(ChildStruct->hChildCodeMax, -1);
    return(1);
}

// -----------------------------------------------------------------------
// Refresh snap screen
void UpdateRegs(long dwThreadID)
{
    int i = 0;
    LPSNAPPROCESS Process = 0;
    CONTEXT Context;
    CStr HexDat;
    TCHAR szScratchPad[255];
    long double pDoubleReg;
    float pFloatReg;
    long *lMXCSR;
    long *lOldMXCSR;
    LPCONTEXT LastContext;
    int HighLightReg = FALSE;
    int j;
    BYTE RegByte;
    BYTE OldRegByte;
    long *pReg;
    LONGLONG *pFpuReg;

    GetCurrentContext(dwThreadID, &Context);
    LastContext = GetLastContext(dwThreadID);

    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Eax, 8), REG_EAX, 0);
    if(LastContext->Eax != Context.Eax) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_EAX, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Ebx, 8), REG_EBX, 0);
    if(LastContext->Ebx != Context.Ebx) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_EBX, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Ecx, 8), REG_ECX, 0);
    if(LastContext->Ecx != Context.Ecx) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_ECX, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Edx, 8), REG_EDX, 0);
    if(LastContext->Edx != Context.Edx) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_EDX, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Esi, 8), REG_ESI, 0);
    if(LastContext->Esi != Context.Esi) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_ESI, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Edi, 8), REG_EDI, 0);
    if(LastContext->Edi != Context.Edi) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_EDI, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Eip, 8), REG_EIP, 0);
    if(LastContext->Eip != Context.Eip) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_EIP, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Esp, 8), REG_ESP, 0);
    if(LastContext->Esp != Context.Esp) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_ESP, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.Ebp, 8), REG_EBP, 0);
    if(LastContext->Ebp != Context.Ebp) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_EBP, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.EFlags, 8), REG_EFL, 0);
    if(LastContext->EFlags != Context.EFlags) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_EFL, HighLightReg);
    
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.SegCs, 4), REG_CS, 0);
    if(LastContext->SegCs != Context.SegCs) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_CS, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.SegDs, 4), REG_DS, 0);
    if(LastContext->SegDs != Context.SegDs) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_DS, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.SegEs, 4), REG_ES, 0);
    if(LastContext->SegEs != Context.SegEs) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_ES, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.SegFs, 4), REG_FS, 0);
    if(LastContext->SegFs != Context.SegFs) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_FS, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.SegGs, 4), REG_GS, 0);
    if(LastContext->SegGs != Context.SegGs) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_GS, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.SegSs, 4), REG_SS, 0);
    if(LastContext->SegSs != Context.SegSs) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_SS, HighLightReg);

    // Display FPU registers
    for(i = 0; i < 8; i++)
    {
        pFpuReg = (LONGLONG *) (Context.FloatSave.RegisterArea + (10 * i));
        _asm
        {
            mov eax,[pFpuReg]
            fld TBYTE ptr [eax]
            fstp qword ptr [pDoubleReg]
        }
		sprintf(szScratchPad, "%+1.17Le", pDoubleReg);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_ST0 + i, 0);
        HighLightReg = FALSE;
        for(j = 0; j < 10; j++)
        {
            RegByte = (Context.FloatSave.RegisterArea[(10 * i) + j]);
            OldRegByte = (LastContext->FloatSave.RegisterArea[(10 * i) + j]);
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_ST0 + i, HighLightReg);
    }

    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.FloatSave.ControlWord & 0xffff, 4), REG_FPUCTRL, 0);
    if((LastContext->FloatSave.ControlWord & 0xffff) != (Context.FloatSave.ControlWord & 0xffff)) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_FPUCTRL, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.FloatSave.StatusWord & 0xffff, 4), REG_FPUSTAT, 0);
    if((LastContext->FloatSave.StatusWord & 0xffff) != (Context.FloatSave.StatusWord & 0xffff)) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_FPUSTAT, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.FloatSave.TagWord & 0xffff, 4), REG_FPUTAGS, 0);
    if((LastContext->FloatSave.TagWord & 0xffff) != (Context.FloatSave.TagWord & 0xffff)) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_FPUTAGS, HighLightReg);

    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.FloatSave.ErrorSelector & 0xffff, 4), REG_FPUCS, 0);
    if((LastContext->FloatSave.ErrorSelector & 0xffff) != (Context.FloatSave.ErrorSelector & 0xffff)) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_FPUCS, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.FloatSave.ErrorOffset, 8), REG_FPUEIP, 0);
    if(LastContext->FloatSave.ErrorOffset != Context.FloatSave.ErrorOffset) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_FPUEIP, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.FloatSave.DataSelector & 0xffff, 4), REG_FPUDS, 0);
    if((LastContext->FloatSave.DataSelector & 0xffff) != (Context.FloatSave.DataSelector & 0xffff)) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_FPUDS, HighLightReg);
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(Context.FloatSave.DataOffset, 8), REG_FPUEDO, 0);
    if(LastContext->FloatSave.DataOffset != Context.FloatSave.DataOffset) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_FPUEDO, HighLightReg);

    // Display MMX registers
    //int tos = (Context.FloatSave.StatusWord & 0x3800) >> 11;
    for(i = 0; i < 8; i++)
    {
        pReg = (long *) (Context.FloatSave.RegisterArea + (10 * i));
        sprintf(szScratchPad, "%08lx%08lx", pReg[1], pReg[0]);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_MM0 + i, 0);

        HighLightReg = FALSE;
        for(j = 0; j < 10; j++)
        {
            RegByte = (Context.FloatSave.RegisterArea[(10 * i) + j]);
            OldRegByte = (LastContext->FloatSave.RegisterArea[(10 * i) + j]);
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_MM0 + i, HighLightReg);
        _asm
        {
            mov eax, [pReg]
            fld dword ptr [eax]
            fstp dword ptr [pFloatReg]
        }
        sprintf(szScratchPad, "%+1.5le", pFloatReg);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_MM00 + (i * 2), 0);

        HighLightReg = FALSE;
        for(j = 0; j < 5; j++)
        {
            RegByte = (Context.FloatSave.RegisterArea[(10 * i) + j]);
            OldRegByte = (LastContext->FloatSave.RegisterArea[(10 * i) + j]);
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_MM00 + (i * 2), HighLightReg);
        _asm
        {
            mov eax, [pReg]
            fld dword ptr [eax + 4]
            fstp dword ptr [pFloatReg]
        }
        sprintf(szScratchPad, "%+1.5le", pFloatReg);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_MM01 + (i * 2), 0);

        HighLightReg = FALSE;
        for(j = 5; j < 10; j++)
        {
            RegByte = (Context.FloatSave.RegisterArea[(10 * i) + j]);
            OldRegByte = (LastContext->FloatSave.RegisterArea[(10 * i) + j]);
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_MM01 + (i * 2), HighLightReg);
    }

    // Display XMM registers
    //int tos = (Context.FloatSave.StatusWord & 0x3800) >> 11;
    for(i = 0; i < 8; i++)
    {
        pReg = (long *) (Context.ExtendedRegisters + (16 * (i + 10)));
        sprintf(szScratchPad, "%08lx%08lx%08lx%08lx", pReg[3], pReg[2], pReg[1], pReg[0]);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_XMM0 + i, 0);

        HighLightReg = FALSE;
        for(j = 0; j < 16; j++)
        {
            RegByte = Context.ExtendedRegisters[(16 * (i + 10)) + j];
            OldRegByte = LastContext->ExtendedRegisters[(16 * (i + 10)) + j];
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_XMM0 + i, HighLightReg);

        _asm
        {
            mov eax, [pReg]
            fld dword ptr [eax]
            fstp dword ptr [pFloatReg]
        }
        sprintf(szScratchPad, "%+1.5le", pFloatReg);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_XMM00 + (i * 4), 0);
        HighLightReg = FALSE;
        for(j = 0; j < 4; j++)
        {
            RegByte = Context.ExtendedRegisters[(16 * (i + 10)) + j];
            OldRegByte = LastContext->ExtendedRegisters[(16 * (i + 10)) + j];
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_XMM00 + (i * 4), HighLightReg);
        _asm
        {
            mov eax, [pReg]
            fld dword ptr [eax + 4]
            fstp dword ptr [pFloatReg]
        }
        sprintf(szScratchPad, "%+1.5le", pFloatReg);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_XMM01 + (i * 4), 0);
        HighLightReg = FALSE;
        for(j = 4; j < 8; j++)
        {
            RegByte = Context.ExtendedRegisters[(16 * (i + 10)) + j];
            OldRegByte = LastContext->ExtendedRegisters[(16 * (i + 10)) + j];
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_XMM01 + (i * 4), HighLightReg);
        _asm
        {
            mov eax, [pReg]
            fld dword ptr [eax + 8]
            fstp dword ptr [pFloatReg]
        }
        sprintf(szScratchPad, "%+1.5le", pFloatReg);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_XMM02 + (i * 4), 0);
        HighLightReg = FALSE;
        for(j = 8; j < 12; j++)
        {
            RegByte = Context.ExtendedRegisters[(16 * (i + 10)) + j];
            OldRegByte = LastContext->ExtendedRegisters[(16 * (i + 10)) + j];
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_XMM02 + (i * 4), HighLightReg);
        _asm
        {
            mov eax, [pReg]
            fld dword ptr [eax + 12]
            fstp dword ptr [pFloatReg]
        }
        sprintf(szScratchPad, "%+1.5le", pFloatReg);
        ListViewSetSubItem(hRegistersListView, szScratchPad, REG_XMM03 + (i * 4), 0);
        HighLightReg = FALSE;
        for(j = 12; j < 16; j++)
        {
            RegByte = Context.ExtendedRegisters[(16 * (i + 10)) + j];
            OldRegByte = LastContext->ExtendedRegisters[(16 * (i + 10)) + j];
            if(RegByte != OldRegByte)
            {
                HighLightReg = TRUE;
                break;
            }
        }
        ListViewSetItemHighlight(hRegistersListView, REG_XMM03 + (i * 4), HighLightReg);
    }
    lMXCSR = (long *) &Context.ExtendedRegisters[24];
    lOldMXCSR = (long *) &LastContext->ExtendedRegisters[24];
    ListViewSetSubItem(hRegistersListView, HexDat.Hex_Fix_To_String(*lMXCSR, 8), REG_MXCSR, 0);
    if(*lOldMXCSR != *lMXCSR) HighLightReg = TRUE;
    else HighLightReg = FALSE;
    ListViewSetItemHighlight(hRegistersListView, REG_MXCSR, HighLightReg);
    
    // Update the last context now
    memcpy(LastContext, &Context, sizeof(CONTEXT));
}

// -----------------------------------------------------------------------
// Put toolbar in run state
void SnapRunstate(void)
{
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_RUN, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_PAUSE, 1);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STOP, 1);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPINTO, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPOVER, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPOUT, 0);
}

// -----------------------------------------------------------------------
// Put toolbar in dead end state
void SnapDeadEndstate(void)
{
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_RUN, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_PAUSE, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STOP, 1);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPINTO, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPOVER, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPOUT, 0);
}

// -----------------------------------------------------------------------
// Put toolbar in pause state
void SnapPausestate(void)
{
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_RUN, 1);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_PAUSE, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STOP, 1);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPINTO, 1);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPOVER, 1);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPOUT, 1);
}

// -----------------------------------------------------------------------
// Put toolbar in dead state
void SnapDeadstate(void)
{
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_RUN, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_PAUSE, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STOP, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPINTO, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPOVER, 0);
    ToolBarSetButtonEnabled(hDebugToolbar, DEBUGTOOLBAR_STEPOUT, 0);
}

// -----------------------------------------------------------------------
// Stop the thread
void StopThread(long ThreadID)
{
    SuspendThread(GetThreadHandle(ThreadID));
    Threadrunning = 0;
}

// -----------------------------------------------------------------------
// Resume the thread
void RunThread(long ThreadID)
{
    ResumeThread(GetThreadHandle(ThreadID));
    InException = 0;
    Threadrunning = 1;
}

// -----------------------------------------------------------------------
// Remove the process
void KillProcess(void)
{
    Threadrunning = 1;
    GetExitCodeThread(DebuggedProc.hThread, &ExCode);
    TerminateThread(DebuggedProc.hThread, ExCode);
    GetExitCodeProcess(DebuggedProc.hProcess, &ExCode);
    TerminateProcess(DebuggedProc.hProcess, ExCode);
    SnapDeadstate();
}

// -----------------------------------------------------------------------
// Retrieve debugged thread windows informations
/*
void GetThreadWindows(void)
{
    int i = 0;
    CStr Up32;
    long LastLbCount = 0;
    PROCESS_MEMORY_COUNTERS MyProcMem;
    IO_COUNTERS MyIoCount;

    LastLbCount = ListBoxCount(FRMDebugLbWindows);
    ListBoxReset(FRMDebugLbWindows);
    if(InException == 1)
    {
        ListBoxAddItem(FRMDebugLbWindows, "Can't retrieve informations in current context", -1);
        return;
    }
    if(Windows2K == 1)
    {
        if(GetProcessIoCounters(DebuggedProc.hProcess, &MyIoCount) != 0)
        {
            Up32 = (long) MyIoCount.ReadOperationCount;
            if(strcmpi(Up32.Get_String(), "0") == 0) Up32 = "";
            ListBoxAddItem(FRMDebugLbWindows, "Read operations: " + (CStr) Up32 + (CStr) (long) Int64ShrlMod32(MyIoCount.ReadOperationCount,32), -1);
            Up32 = (long) MyIoCount.ReadTransferCount;
            if(strcmpi(Up32.Get_String(), "0") == 0) Up32 = "";
            ListBoxAddItem(FRMDebugLbWindows, "Read transfers: " + (CStr) Up32 + (CStr) (long) Int64ShrlMod32(MyIoCount.ReadTransferCount,32), -1);
            Up32 = (long) MyIoCount.WriteOperationCount;
            if(strcmpi(Up32.Get_String(), "0") == 0) Up32 = "";
            ListBoxAddItem(FRMDebugLbWindows, "Write operations: " + (CStr) Up32 + (CStr) (long) Int64ShrlMod32(MyIoCount.WriteOperationCount,32), -1);
            Up32 = (long) MyIoCount.WriteTransferCount;
            if(strcmpi(Up32.Get_String(), "0") == 0) Up32 = "";
            ListBoxAddItem(FRMDebugLbWindows, "Write transfers: " + (CStr) Up32 + (CStr) (long) Int64ShrlMod32(MyIoCount.WriteTransferCount,32), -1);
            Up32 = (long) MyIoCount.OtherOperationCount;
            if(strcmpi(Up32.Get_String(), "0") == 0) Up32 = "";
            ListBoxAddItem(FRMDebugLbWindows, "Other operations: " + (CStr) Up32 + (CStr) (long) Int64ShrlMod32(MyIoCount.OtherOperationCount,32), -1);
            Up32 = (long) MyIoCount.OtherTransferCount;
            if(strcmpi(Up32.Get_String(), "0") == 0) Up32 = "";
            ListBoxAddItem(FRMDebugLbWindows, "Other transfers: " + (CStr) Up32 + (CStr) (long) Int64ShrlMod32(MyIoCount.OtherTransferCount,32), -1);
        }
        MyProcMem.cb = sizeof(MyProcMem);
        GetProcessMemoryInfo(DebuggedProc.hProcess, &MyProcMem, sizeof(MyProcMem));
        ListBoxAddItem(FRMDebugLbWindows, "Pages faults: " + (CStr) MyProcMem.PageFaultCount, -1);
        ListBoxAddItem(FRMDebugLbWindows, "Pages usage: " + (CStr) MyProcMem.PagefileUsage + (CStr) "/" + (CStr) MyProcMem.PeakPagefileUsage, -1);
        ListBoxAddItem(FRMDebugLbWindows, "Working size: " + (CStr) MyProcMem.WorkingSetSize + (CStr) "/" + (CStr) MyProcMem.PeakWorkingSetSize, -1);
        ListBoxAddItem(FRMDebugLbWindows, "Non paged pool usage: " + (CStr) MyProcMem.QuotaNonPagedPoolUsage + (CStr) "/" + (CStr) MyProcMem.QuotaPeakNonPagedPoolUsage, -1);
        ListBoxAddItem(FRMDebugLbWindows, "Paged pool usage: " + (CStr) MyProcMem.QuotaPagedPoolUsage + (CStr) "/" + (CStr) MyProcMem.QuotaPeakPagedPoolUsage, -1);
        ListBoxAddItem(FRMDebugLbWindows, "", -1);
        ListBoxAddItem(FRMDebugLbWindows, "GDI objects: " + (CStr) GetGuiResources(DebuggedProc.hProcess, GR_GDIOBJECTS), -1);
        ListBoxAddItem(FRMDebugLbWindows, "User objects: " + (CStr) GetGuiResources(DebuggedProc.hProcess, GR_USEROBJECTS), -1);
    }
    ListBoxAddItem(FRMDebugLbWindows, "", -1);
    for(i = 0; i < DbWinH.Amount(); i++)
    {
        if(DbWinH.Get(i)->Content != 0)
        {
            AffDWindowInfo((HWND) DbWinH.Get(i)->Content);
            // Retrieve the child windows
            EnumChildWindows((HWND) DbWinH.Get(i)->Content, &EnumChildWins, 0);
        }
    }
    if(SysTabGetCurrentItem(FRMDebugSB) != 6) if(ListBoxCount(FRMDebugLbWindows) != LastLbCount) SysTabHighLightItem(FRMDebugSB, 6, 1);
}

int CALLBACK EnumChildWins(HWND hWnd, long lParam)
{
    AffDWindowInfo(hWnd);
    return(1);
}

// -----------------------------------------------------------------------
// Display window informations
void AffDWindowInfo(HWND hWnd)
{
    CStr WClass;
    CStr RealWClass;
    HIMAGELIST hImg = 0;
    HGDIOBJ hStatic = 0;
    CStr EdPChar;
    long EdSel = 0;
    long HKe = 0;
    HFONT Fonth = 0;
    long CDat = 0;
    CStr BufString;

    ListBoxAddItem(FRMDebugLbWindows, "Handle: 0x" + (CStr) StringHex32((long) hWnd), -1);
    WClass = ControlGetClassName(hWnd);
    WClass = WClass.Trim();
    ListBoxAddItem(FRMDebugLbWindows, "    Class name: " + (CStr) WClass, -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class atom: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCW_ATOM))->Get_String() + (CStr) "h (" + (CStr) GetClassLong(hWnd, GCW_ATOM) + (CStr) ")", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class extra bytes: " + (CStr) GetClassLong(hWnd, GCL_CBCLSEXTRA), -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class windows extra bytes: " + (CStr) GetClassLong(hWnd, GCL_CBWNDEXTRA), -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class background brush: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_HBRBACKGROUND))->Get_String() + (CStr) "h (" + (CStr) GetClassLong(hWnd, GCL_HBRBACKGROUND) + (CStr) ")", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class cursor handle: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_HCURSOR)) + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class large icon handle: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_HICON)) + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class small icon handle: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_HICONSM)) + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class registering module handle: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_HMODULE)) + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class menu resource: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_MENUNAME)) + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class style: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_STYLE))->Get_String() + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Class wproc: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_WNDPROC))->Get_String() + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Text: " + (CStr) ControlGetText(hWnd)->Get_String(), -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Text length: " + (CStr) ControlGetTextLen(hWnd), -1);
    ListBoxAddItem(FRMDebugLbWindows, "    WProc: 0" + (CStr) StringHex32(GetWindowLong(hWnd, GWL_WNDPROC))->Get_String() + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Style: 0" + (CStr) StringHex32(GetWindowLong(hWnd, GWL_STYLE))->Get_String() + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    ExStyle: 0" + (CStr) StringHex32(GetWindowLong(hWnd, GWL_EXSTYLE))->Get_String() + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    CProc: 0" + (CStr) StringHex32(GetClassLong(hWnd, GCL_WNDPROC))->Get_String() + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Menu handle: 0" + (CStr) StringHex32((long) GetMenu(hWnd)) + (CStr) "h", -1);
    Fonth = ControlGetFont(hWnd);
    if(Fonth == 0)
    {
        ListBoxAddItem(FRMDebugLbWindows, "    Font handle: <System>", -1);
    }
    else
    {
        ListBoxAddItem(FRMDebugLbWindows, "    Font handle: 0" + (CStr) StringHex32((long) ControlGetFont(hWnd))->Get_String() + (CStr) "h", -1);
    }
    ListBoxAddItem(FRMDebugLbWindows, "    Small icon: 0" + (CStr) StringHex32((long) ControlGetIcon(hWnd, ICON_SMALL))->Get_String() + (CStr) "h", -1);
    ListBoxAddItem(FRMDebugLbWindows, "    Large icon: 0" + (CStr) StringHex32((long) ControlGetIcon(hWnd, ICON_BIG))->Get_String() + (CStr) "h", -1);
    HKe = ControlGetHotKey(hWnd);
    if(HKe != 0) ListBoxAddItem(FRMDebugLbWindows, "    Associated hotkey: " + (CStr) HKe, -1);
    switch(ControlIsWindowChild(hWnd))
    {
        case 0:
            ListBoxAddItem(FRMDebugLbWindows, "    MDIChild: No", -1);
            break;
        case 1:
            ListBoxAddItem(FRMDebugLbWindows, "    MDIChild: Yes", -1);
            break;
    }
    switch(ControlIsEnabled(hWnd))
    {
        case 0:
            ListBoxAddItem(FRMDebugLbWindows, "    Enabled: No", -1);
            break;
        default:
            ListBoxAddItem(FRMDebugLbWindows, "    Enabled: Yes", -1);
            break;
    }
    switch(ControlIsVisible(hWnd))
    {
        case 0:
            ListBoxAddItem(FRMDebugLbWindows, "    Visible: No", -1);
            break;
        default:
            ListBoxAddItem(FRMDebugLbWindows, "    Visible: Yes", -1);
            ListBoxAddItem(FRMDebugLbWindows, "    Rect: " + (CStr) ControlLeft(hWnd) + (CStr) "," + (CStr) ControlTop(hWnd) + (CStr) "," + (CStr) ControlWidth(hWnd) + (CStr) "," + (CStr) ControlHeight(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "    Client rect: " + (CStr) ControlClientLeft(hWnd) + (CStr) "," + (CStr) ControlClientTop(hWnd) + (CStr) "," + (CStr) ControlClientWidth(hWnd) + (CStr) "," + (CStr) ControlClientHeight(hWnd), -1);
            break;
    }
    RealWClass = WClass.Left(WClass.Len());
    BufString = RealWClass.Upper_Case();
    if(BufString == "LISTBOX")
    {
        ListBoxAddItem(FRMDebugLbWindows, "        Entries: " + (CStr) ListBoxCount(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Selected item: " + (CStr) ListBoxGetSelItemIndex(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Selected text: " + (CStr) ListBoxGetCurrentItem(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Select item data: 0" + (CStr) StringHex32(ListBoxGetItemData(hWnd, ListBoxGetSelItemIndex(hWnd)))->Get_String() + (CStr) "h (" + (CStr) ListBoxGetItemData(hWnd, ListBoxGetSelItemIndex(hWnd)) + (CStr) ")", -1);
    }
    else if(BufString == "COMBOBOX")
    {
        ListBoxAddItem(FRMDebugLbWindows, "        Entries: " + (CStr) ComboBoxCount(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Selected item: " + (CStr) ComboBoxGetIndex(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Selected text: " + (CStr) ComboBoxGetItem(hWnd, ComboBoxGetIndex(hWnd)), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Select item data: 0" + (CStr) StringHex32(ComboBoxGetItemData(hWnd, ComboBoxGetIndex(hWnd)))->Get_String() + (CStr) "h (" + (CStr) ComboBoxGetItemData(hWnd, ComboBoxGetIndex(hWnd)) + (CStr) ")", -1);
    }
    else if(BufString == "TOOLBARWINDOW32")
    {
        hImg = ToolBarGetImagelist(hWnd);
        ListBoxAddItem(FRMDebugLbWindows, "        Imagelist handle: 0" + (CStr) StringHex32((long) hImg) + (CStr) "h", -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Icons entries: " + (CStr) ImageListGetIconsCount(hImg), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Entries: " + (CStr) ToolBarGetButtonsCount(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Rows: " + (CStr) ToolBarGetRowsCount(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Buttons size: " + (CStr) ToolBarGetButtonXSize(hWnd) + (CStr) "," + (CStr) ToolBarGetButtonYSize(hWnd), -1);
    }
    else if(BufString == "SYSTABCONTROL32")
    {
        hImg = ToolBarGetImagelist(hWnd);
        ListBoxAddItem(FRMDebugLbWindows, "        Imagelist handle: 0" + (CStr) StringHex32((long) hImg) + (CStr) "h", -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Icons entries: " + (CStr) ImageListGetIconsCount(hImg), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Selected item: " + (CStr) SysTabGetCurrentItem(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Entries: " + (CStr) SysTabItemsCount(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Rows: " + (CStr) SysTabGetRowsCount(hWnd), -1);
    }
    else if(BufString == "STATIC")
    {
        hStatic = LabelGetImage(hWnd, IMAGE_BITMAP);
        if(hStatic != 0)
        {
            ListBoxAddItem(FRMDebugLbWindows, "        Bitmap handle: 0" + (CStr) StringHex32((long) hStatic)->Get_String() + (CStr) "h", -1);
            goto DoneStatic;
        }
        hStatic = LabelGetImage(hWnd, IMAGE_CURSOR);
        if(hStatic != 0)
        {
            ListBoxAddItem(FRMDebugLbWindows, "        Cursor handle: 0" + (CStr) StringHex32((long) hStatic)->Get_String() + (CStr) "h", -1);
            goto DoneStatic;
        }
        hStatic = LabelGetImage(hWnd, IMAGE_ENHMETAFILE);
        if(hStatic != 0)
        {
            ListBoxAddItem(FRMDebugLbWindows, "        Metafile handle: 0" + (CStr) StringHex32((long) hStatic)->Get_String() + (CStr) "h", -1);
            goto DoneStatic;
        }
        hStatic = LabelGetImage(hWnd, IMAGE_ICON);
        if(hStatic != 0)
        {
            ListBoxAddItem(FRMDebugLbWindows, "        Icon handle: 0" + (CStr) StringHex32((long) hStatic)->Get_String() + (CStr) "h", -1);
            goto DoneStatic;
        }
DoneStatic:;
    }
    else if(BufString == "BUTTON")
    {
        ListBoxAddItem(FRMDebugLbWindows, "        State: " + (CStr) ButtonGetState(hWnd), -1);
        hStatic = ButtonGetImage(hWnd, IMAGE_BITMAP);
        if(hStatic != 0)
        {
            ListBoxAddItem(FRMDebugLbWindows, "        Bitmap handle: 0" + (CStr) StringHex32((long) hStatic)->Get_String() + (CStr) "h", -1);
            goto DoneButton;
        }
        hStatic = ButtonGetImage(hWnd, IMAGE_ICON);
        if(hStatic != 0)
        {
            ListBoxAddItem(FRMDebugLbWindows, "        Icon handle: 0" + (CStr) StringHex32((long) hStatic) + (CStr) "h", -1);
            goto DoneButton;
        }
DoneButton:;
    }
    else if(BufString == "EDIT")
    {
        ListBoxAddItem(FRMDebugLbWindows, "        Text memory: 0" + (CStr) StringHex32(TextBoxGetTextHandle(hWnd))->Get_String() + (CStr) "h", -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Used word break: 0" + (CStr) StringHex32(TextBoxGetBreakProcAddress(hWnd))->Get_String() + (CStr) "h", -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Max length: " + (CStr) TextBoxGetMaxLen(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Lines: " + (CStr) TextBoxLinesCount(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Margin width: " + (CStr) TextBoxGetMarginWidth(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        First visible line: " + (CStr) TextBoxGetFirstVisibleLine(hWnd), -1);
        ListBoxAddItem(FRMDebugLbWindows, "        Scroll position: " + (CStr) TextBoxGetScrollPos(hWnd), -1);
        switch(TextBoxIsModified(hWnd))
        {
            case 0:
                ListBoxAddItem(FRMDebugLbWindows, "        Modified: No", -1);
                break;
            default:
                ListBoxAddItem(FRMDebugLbWindows, "        Modified: Yes", -1);
                break;
        }
        ListBoxAddItem(FRMDebugLbWindows, "        Options: " + (CStr) TextBoxGetCurrentOptions(hWnd), -1);
        EdPChar = TextBoxGetPasswordChar(hWnd);
        if(EdPChar.Len() != 0) ListBoxAddItem(FRMDebugLbWindows, "        Password char: " + (CStr) EdPChar, -1);
        EdSel = TextBoxGetSelection(hWnd);
        if(EdSel != -1) ListBoxAddItem(FRMDebugLbWindows, "        Selection: " + (CStr) ((EdSel & 0xFFFF0000) / 0x10000) + (CStr) "," + (CStr) (EdSel & 0xFFFF), -1);
        switch(TextBoxCanPaste(hWnd))
        {
            case 0:
                ListBoxAddItem(FRMDebugLbWindows, "        Clipboard datas can be pasted: No", -1);
                break;
            default:
                ListBoxAddItem(FRMDebugLbWindows, "        Clipboard datas can be pasted: Yes", -1);
                break;
        }
        switch(TextBoxCanUndo(hWnd))
        {
            case 0:
                ListBoxAddItem(FRMDebugLbWindows, "        Current operation can be undone: No", -1);
                break;
            case 1:
                ListBoxAddItem(FRMDebugLbWindows, "        Current operation can be undone: Yes", -1);
                break;
        }
    }
    else if(BufString == "REBARWINDOW32")
    {
            ListBoxAddItem(FRMDebugLbWindows, "        Imagelist handle: 0" + (CStr) StringHex32((long) RebarGetImageList(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Palette handle: 0" + (CStr) StringHex32((long) RebarGetPalette(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Entries: " + (CStr) RebarGetBandsCount(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Actual height: " + (CStr) RebarGetHeight(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used background color: 0" + (CStr) StringHex32(RebarGetBackGroundColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used text color: 0" + (CStr) StringHex32(RebarGetTextColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Rows: " + (CStr) RebarGetRowsCount(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Tooltips handle: 0" + (CStr) StringHex32(RebarGetToolTips(hWnd))->Get_String() + (CStr) "h", -1);
    }
    else if(BufString == "SYSPAGER")
    {
            ListBoxAddItem(FRMDebugLbWindows, "        Used background color: 0" + (CStr) StringHex32(PagerGetBackColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Border size: " + (CStr) PagerGetBorderSize(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Buttons size: " + (CStr) PagerGetButtonSize(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Current scroll position: " + (CStr) PagerGetScrollPos(hWnd), -1);
    }
    else if(BufString == "SYSLISTVIEW32")
    {
            ListBoxAddItem(FRMDebugLbWindows, "        Entries: " + (CStr) ListViewItemCount(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Selected entries: " + (CStr) ListViewSelItemCount(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        First Selected entry: " + (CStr) (CDat = ListViewMultiSelFirstItem(hWnd)), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Focused item: " + (CStr) ListViewGetFirstFocusItem(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Selected item: " + (CStr) ListViewGetSelItem(hWnd, -1), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used background color: 0" + (CStr) StringHex32(ListViewGetBackColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used text background color: 0" + (CStr) StringHex32(ListViewGetTextBackColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used text foreground color: 0" + (CStr) StringHex32(ListViewGetTextColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Tooltips handle: 0" + (CStr) StringHex32(ListViewGetToolTips(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Items per page: " + (CStr) ListViewGetCountPerPage(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        First visible item: " + (CStr) ListViewGetTopIndex(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Edit control handle: 0" + (CStr) StringHex32((long) ListViewGetEditControl(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Extended styles: 0" + (CStr) StringHex32(ListViewGetExStyle(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Hot cursor handle: 0" + (CStr) StringHex32(ListViewGetHotCursor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Large imagelist handle: 0" + (CStr) StringHex32(ListViewGetImageList(hWnd, LVSIL_NORMAL))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Small imagelist handle: 0" + (CStr) StringHex32(ListViewGetImageList(hWnd, LVSIL_SMALL))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        State imagelist handle: 0" + (CStr) StringHex32(ListViewGetImageList(hWnd, LVSIL_STATE))->Get_String() + (CStr) "h", -1);
    }
    else if(BufString == "SYSTREEVIEW32")
    {
            ListBoxAddItem(FRMDebugLbWindows, "        Entries: " + (CStr) TreeViewGetItemCount(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Visible entries: " + (CStr) TreeViewGetVisibleCount(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Relative indents: " + (CStr) TreeViewGetIndent(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used background color: 0" + (CStr) StringHex32(TreeViewGetBackColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used insert mark color: 0" + (CStr) StringHex32(TreeViewGetInsertMarkColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used lines color: 0" + (CStr) StringHex32(TreeViewGetLineColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Used text color: 0" + (CStr) StringHex32(TreeViewGetTextColor(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Tooltips handle: 0" + (CStr) StringHex32(TreeViewGetToolTips(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Edit control handle: 0" + (CStr) StringHex32((long) TreeViewGetEditControl(hWnd))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Items height: " + (CStr) TreeViewGetItemHeight(hWnd), -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Normal imagelist handle: 0" + (CStr) StringHex32(ListViewGetImageList(hWnd, TVSIL_NORMAL))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        State imagelist handle: 0" + (CStr) StringHex32(ListViewGetImageList(hWnd, TVSIL_STATE))->Get_String() + (CStr) "h", -1);
            ListBoxAddItem(FRMDebugLbWindows, "        Scroll time: " + (CStr) (TreeViewGetScrollTime(hWnd)) + (CStr) " milliseconds", -1);
    }
    ListBoxAddItem(FRMDebugLbWindows, "", -1);
}
*/
// -----------------------------------------------------------------------
// Retrieve the boost state of a process
long GetProcessBoost(HANDLE hProcess)
{
    int ProcBoost = 0;

    if(Windows2K == 1)
    {
        if(GetProcessPriorityBoost(hProcess, &ProcBoost) != 0)
        {
            switch(ProcBoost)
            {
                case 0:
                    return(1);
                    break;
                default:
                    return(2);
            }
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Retrieve the boost state of a thread
long GetThreadBoost(HANDLE hThread)
{
    int ThBoost = 0;

    if(Windows2K == 1)
    {
        if(GetThreadPriorityBoost(hThread, &ThBoost) != 0)
        {
            switch(ThBoost)
            {
                case 0:
                    return(1);
                default:
                    return(2);
            }
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Retrieve process timing
long GetProcessTime(HANDLE hProcess)
{
    FILETIME LocFTime;
    
    if(Windows2K == 1)
    {
        GetProcessTimes(hProcess, &lpProcCreationTm, &lpProcExitTm, &lpProcKModeTm, &lpProcUModeTm);
        FileTimeToLocalFileTime(&lpProcCreationTm, &LocFTime);
        FileTimeToSystemTime(&LocFTime, &lpProcCreationSysTm);
        FileTimeToLocalFileTime(&lpProcExitTm, &LocFTime);
        FileTimeToSystemTime(&LocFTime, &lpProcExitSysTm);
        FileTimeToLocalFileTime(&lpProcKModeTm, &LocFTime);
        FileTimeToSystemTime(&LocFTime, &lpProcKModeSysTm);
        FileTimeToLocalFileTime(&lpProcUModeTm, &LocFTime);
        FileTimeToSystemTime(&LocFTime, &lpProcUModeSysTm);
        return(TRUE);
    }
    return(FALSE);
}

// -----------------------------------------------------------------------
// Retrieve thread timing
long GetThreadTime(HANDLE hThread)
{
    FILETIME LocFTime;
    
    if(Windows2K == 1)
    {
        GetThreadTimes(hThread, &lpThreadCreationTm, &lpThreadExitTm, &lpThreadKModeTm, &lpThreadUModeTm);
        FileTimeToLocalFileTime(&lpThreadCreationTm, &LocFTime);
        FileTimeToSystemTime(&LocFTime, &lpThreadCreationSysTm);
        FileTimeToLocalFileTime(&lpThreadExitTm, &LocFTime);
        FileTimeToSystemTime(&LocFTime, &lpThreadExitSysTm);
        FileTimeToLocalFileTime(&lpThreadKModeTm, &LocFTime);
        FileTimeToSystemTime(&LocFTime, &lpThreadKModeSysTm);
        FileTimeToLocalFileTime(&lpThreadUModeTm, &LocFTime);
        FileTimeToSystemTime(&LocFTime, &lpThreadUModeSysTm);
        return(TRUE);
    }
    return(FALSE);
}

// -----------------------------------------------------------------------
// Create the treeview structure
void CreateSnapTreeView(void)
{
    hTreeViewDebugRoot = TreeViewAddItem(hTreeViewDebug, DebuggedFile, TVI_ROOT, 0, ICON_SNAP, ICON_SNAP, TVIS_BOLD, 0);
    hTreeViewDebugProcesses = TreeViewAddItem(hTreeViewDebug, "Processes", hTreeViewDebugRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewDebugDLLs = TreeViewAddItem(hTreeViewDebug, "Dlls", hTreeViewDebugRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
}

// -----------------------------------------------------------------------
// Add a breakpoint
long SetBreakPoint(CList <LPSNAPBP> *BPList, DWORD BPAddress, int BPType)
{
    int i = 0;
    DWORD cbBytes = 0;
    LPSNAPBP NewBreakPoint = 0;

    // Check if breakpoint already exists
    for(i = 0; i < BPList->Amount(); i++)
    {
        // Search breakpoint address in bp list
        NewBreakPoint = BPList->Get(i)->Content;
        if(BPAddress == NewBreakPoint->Address)
        {
            // Already set = correct
            return(1);
        }
    }

    // Retrieve old byte
    if(!ReadProcessMemory(DebuggedProc.hProcess, (LPCVOID) BPAddress, (LPVOID) &OldOpCode, 1, &cbBytes)) return(FALSE);
    // Patch it
    if(!WriteProcessMemory(DebuggedProc.hProcess, (LPVOID) BPAddress, (LPVOID) &X86INT3, 1, &cbBytes)) return(FALSE);

    NewBreakPoint = (LPSNAPBP) calloc(sizeof(SNAPBP), 1);
    memset(NewBreakPoint, 0, sizeof(SNAPBP));

    // Store breakpoint infos
    NewBreakPoint->Address = BPAddress;
    NewBreakPoint->OldByte = OldOpCode;
    NewBreakPoint->BPType = BPType;

    // Store it into the list
    BPList->Add(NewBreakPoint);
    return(TRUE);
}

// -----------------------------------------------------------------------
// Restore the original code of a breakpoint
long RestoreBreakPoint(DWORD BPAddress)
{
    DWORD cbBytes;
    LPSNAPBP BreakPoint;
    int i;

    for(i = 0; i < DebBreakPoints.Amount(); i++)
    {
        // Search breakpoint address in bp list
        BreakPoint = DebBreakPoints.Get(i)->Content;
        if(BPAddress == BreakPoint->Address)
        {
            // Write back old code byte
            if(!WriteProcessMemory(DebuggedProc.hProcess, (LPVOID) BPAddress, (LPVOID) &BreakPoint->OldByte, 1, &cbBytes)) return(FALSE);
            return(TRUE);
        }
    }
    return(FALSE);
}

// -----------------------------------------------------------------------
// Kill a volatile breakpoint (sticky are handled by the user)
long KillBreakPoint(DWORD BPAddress)
{
    DWORD cbBytes;
    LPSNAPBP BreakPoint;
    int i;

    for(i = 0; i < DebBreakPoints.Amount(); i++)
    {
        // Search breakpoint address in bp list
        BreakPoint = DebBreakPoints.Get(i)->Content;
        if(BPAddress == BreakPoint->Address)
        {
            if(BreakPoint->BPType == BP_VOLATILE)
            {
                // Only remove volatile breakpoints (from single steps)
                // sticky ones are removed during end of debugging session
                if(!WriteProcessMemory(DebuggedProc.hProcess, (LPVOID) BPAddress, (LPVOID) &BreakPoint->OldByte, 1, &cbBytes)) return(FALSE);
                DebBreakPoints.Del(i);
                return(TRUE);
            }
        }
    }
    return(FALSE);
}

// -----------------------------------------------------------------------
// Remove all branch breakpoints from the list
void FlushBranchBreakPoints(void)
{
    DWORD cbBytes;
    LPSNAPBP BreakPoint;
    int i = 0;

    while(i < DebBreakPoints.Amount())
    {
        // Search breakpoint address in bp list
        BreakPoint = DebBreakPoints.Get(i)->Content;
        if(BreakPoint->BPType == BP_BRANCH)
        {
            // Only remove volatile breakpoints (from single steps)
            // sticky ones are removed during end of debugging session
            WriteProcessMemory(DebuggedProc.hProcess, (LPVOID) BreakPoint->Address, (LPVOID) &BreakPoint->OldByte, 1, &cbBytes);
            DebBreakPoints.Del(i);
            // Restart the counting
            i = -1;
        }
        i++;
    }
    return;
}

// -----------------------------------------------------------------------
// Retrieve the type of a breakpoint or 0
int BreakPointType(DWORD BPAddress)
{
    LPSNAPBP BreakPoint;
    int i;

    for(i = 0; i < DebBreakPoints.Amount(); i++)
    {
        // Search breakpoint address in bp list
        BreakPoint = DebBreakPoints.Get(i)->Content;
        if(BPAddress == BreakPoint->Address)
        {
            // Write back old code byte
            return(BreakPoint->BPType);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Step back in code
void DebugStepBack(long dwThreadID)
{
    HANDLE hThread = 0;
    int i = 0;
    CONTEXT Context;
    LPSNAPPROCESS Process = 0;

    memset(&Context, 0, sizeof(CONTEXT));
    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            hThread = Process->hThread;
            Context.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;
            GetThreadContext(hThread, &Context);
            Context.Eip--;
            SetThreadContext(hThread, &Context);
            break;
        }
    }
}

// -----------------------------------------------------------------------
// Set the current code address
void DebugSetEIP(long dwThreadID, DWORD Address)
{
    HANDLE hThread = 0;
    int i = 0;
    CONTEXT Context;
    LPSNAPPROCESS Process = 0;

    memset(&Context, 0, sizeof(CONTEXT));
    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            hThread = Process->hThread;
            Context.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;
            GetThreadContext(hThread, &Context);
            Context.Eip = Address;
            SetThreadContext(hThread, &Context);
            break;
        }
    }
}

// -----------------------------------------------------------------------
// Retrieve the current process's context
void GetCurrentContext(long dwThreadID, LPCONTEXT Context)
{
    HANDLE hThread = 0;
    int i = 0;
    LPSNAPPROCESS Process = 0;

    memset(Context, 0, sizeof(CONTEXT));
    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            hThread = Process->hThread;
            Context->ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;
            GetThreadContext(hThread, Context);
            break;
        }
    }
    return;
}

// -----------------------------------------------------------------------
// Retrieve the last stored process's context
LPCONTEXT GetLastContext(long dwThreadID)
{
    int i = 0;
    LPSNAPPROCESS Process = 0;

    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            return(&Process->LastContext);
        }
    }
    return(NULL);
}

// -----------------------------------------------------------------------
// Store the current process's context
void SetCurrentContext(long dwThreadID, LPCONTEXT Context)
{
    HANDLE hThread = 0;
    int i = 0;
    LPSNAPPROCESS Process = 0;

    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            hThread = Process->hThread;
            SetThreadContext(hThread, Context);
            break;
        }
    }
    return;
}

// -----------------------------------------------------------------------
// Retrieve the current code address or 0
DWORD GetCurrentEIP(long dwThreadID)
{
    HANDLE hThread = 0;
    int i = 0;
    CONTEXT Context;
    LPSNAPPROCESS Process = 0;
    DWORD EIP = 0;

    memset(&Context, 0, sizeof(CONTEXT));
    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            hThread = Process->hThread;
            Context.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;
            GetThreadContext(hThread, &Context);
            EIP = Context.Eip;
            break;
        }
    }
    return(EIP);
}

// -----------------------------------------------------------------------
// Retrieve the current stack pointer or 0
DWORD GetCurrentESP(long dwThreadID)
{
    HANDLE hThread = 0;
    int i = 0;
    CONTEXT Context;
    LPSNAPPROCESS Process = 0;
    DWORD ESP = 0;

    memset(&Context, 0, sizeof(CONTEXT));
    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            hThread = Process->hThread;
            Context.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;
            GetThreadContext(hThread, &Context);
            ESP = Context.Esp;
            break;
        }
    }
    return(ESP);
}

// -----------------------------------------------------------------------
// Return the current word at given address or 0
DWORD GetCurrentWord(long dwThreadID, DWORD Address)
{
    int i = 0;
    LPSNAPPROCESS Process = 0;
    DWORD Code = 0;
    DWORD cbBytes;

    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            ReadProcessMemory(DebuggedProc.hProcess, (LPCVOID) Address, (LPVOID) &Code, 2, &cbBytes);
            break;
        }
    }
    return(Code);
}

// -----------------------------------------------------------------------
// Return the current dword at given address or 0
DWORD GetCurrentDWord(long dwThreadID, DWORD Address)
{
    int i = 0;
    LPSNAPPROCESS Process = 0;
    DWORD Code = 0;
    DWORD cbBytes;

    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            ReadProcessMemory(DebuggedProc.hProcess, (LPCVOID) Address, (LPVOID) &Code, 4, &cbBytes);
            break;
        }
    }
    return(Code);
}

// -----------------------------------------------------------------------
// Retrieve a given number of bytes from a given thread at given address
// Return the number of bytes read
long ReadThreadbytes(long dwThreadID, DWORD Address, BYTE *Dest, DWORD Bytes)
{
    int i = 0;
    LPSNAPPROCESS Process = 0;
    DWORD cbBytes = 0;

    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if ((long) Process->dwThreadId == (long) dwThreadID)
        {
            ReadProcessMemory(DebuggedProc.hProcess, (LPCVOID) Address, (LPVOID) Dest, Bytes, &cbBytes);
            break;
        }
    }
    return(cbBytes);
}

// -----------------------------------------------------------------------
// Set thread in single step mode
void SetSingleStep(long dwThreadID)
{
    HANDLE hThread = 0;
    int i = 0;
    CONTEXT Context;
    LPSNAPPROCESS Process = 0;
    long ThreadID = 0;

    memset(&Context, 0, sizeof(CONTEXT));
    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            hThread = Process->hThread;
            Context.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;
            GetThreadContext(hThread, &Context);
            // Trap flag on
            Context.EFlags = Context.EFlags | 0x100;
            SetThreadContext(hThread, &Context);
            break;
        }
    }
}

// -----------------------------------------------------------------------
// Set thread in normal mode
void ClearSingleStep(long dwThreadID)
{
    HANDLE hThread = 0;
    int i = 0;
    CONTEXT Context;
    LPSNAPPROCESS Process = 0;
    long ThreadID = 0;

    memset(&Context, 0, sizeof(CONTEXT));
    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            hThread = Process->hThread;
            Context.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;
            GetThreadContext(hThread, &Context);
            // Trap flag off
            Context.EFlags = Context.EFlags & ~0x100;
            SetThreadContext(hThread, &Context);
            break;
        }
    }
}

// -----------------------------------------------------------------------
// Remove a thread from the list
void RemoveThread(long dwThreadID)
{
    int i = 0;
    LPSNAPPROCESS Process = 0;

    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            CloseHandle(Process->hThread);
            DebProcesses.Del(i);
            break;
        }           
    }   
}

// -----------------------------------------------------------------------
// Retrieve a thread handle from it's ID
HANDLE GetThreadHandle(long dwThreadID)
{
    int i = 0;
    LPSNAPPROCESS Process = 0;

    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwThreadId == (long) dwThreadID)
        {
            return(Process->hThread);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Retrieve a process handle from it's ID
HANDLE GetProcessHandle(long dwProcessID)
{
    int i = 0;
    LPSNAPPROCESS Process = 0;

    for(i = 0; i < DebProcesses.Amount(); i++)
    {
        Process = DebProcesses.Get(i)->Content;
        if((long) Process->dwProcessId == (long) dwProcessID)
        {
            return(Process->hProcess);
        }           
    }   
    return(0);
}

// -----------------------------------------------------------------------
// Obtain some functions from PSAPI.dll so it can work on Win98
void Get_PSAPI(void)
{
    if(Windows2K == 1)
    {
        PSApiDll = LoadLibrary("PSAPI.DLL");
        Ptr_GetWsChanges = 0;
        Ptr_InitializeProcessForWsWatch = 0;
        if(PSApiDll != NULL)
        {
            Ptr_GetWsChanges = (BOOL (__stdcall *)(HANDLE, PPSAPI_WS_WATCH_INFORMATION, DWORD)) GetProcAddress((HMODULE) PSApiDll, "GetWsChanges");
            Ptr_InitializeProcessForWsWatch = (BOOL (__stdcall *)(HANDLE)) GetProcAddress((HMODULE) PSApiDll, "InitializeProcessForWsWatch");
        }
    }
}

// -----------------------------------------------------------------------
// Close the PSAPI.dll
void Close_PSAPI(void)
{
    if(Windows2K == 1) if(PSApiDll != NULL) FreeLibrary(PSApiDll);
}

// -----------------------------------------------------------------------
// Ask the user if we should break the debugging process
int Ask_Debug_Break(void)
{
    switch(MiscMsgBox(hMDIform.hWnd, "The debugger is currently running.\rStop the process now ?", MB_QUESTION, Requesters)) {
        case IDYES:
            KillProcess();
            ScheduleQuit = 1;
            return(1);
        case IDNO:
            return(1);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Debug dock proc
LRESULT CALLBACK TreeviewDebugContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SIZE MySize;
    PAINTSTRUCT VSplitPs;
    SIZE TBSize;
    long SpTooltipId = 0;
    HWND hTreeViewFrom = 0;
    long DockState;
    
    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &VSplitPs);
            DockState = DockingBoxGetState(hDebugContainer);
            switch(DockState)
            {
                case DOCKINGBOX_LEFT:
                case DOCKINGBOX_RIGHT:
                case DOCKINGBOX_FLOATING:
                    // Windows toolbar
                    ToolBarGetXYSize(hDebugToolbar, &TBSize);
                    MoveWindow(hDebugPager, 0, 0, ControlWidth(hWnd), TBSize.cy, 1);
                    MoveWindow(hTreeViewDebug, 0, (23 + 1), ControlWidth(hWnd), ControlHeight(hWnd) - (23 + 1), 1);
                    break;
                case DOCKINGBOX_TOP:
                case DOCKINGBOX_BOTTOM:
                    MoveWindow(hDebugPager, 0, 0, 24, ControlHeight(hWnd), 1);
                    MoveWindow(hTreeViewDebug, (24 + 1), 0, ControlWidth(hWnd) - (24 + 1), ControlHeight(hWnd), 1);
                    break;
            }
            EndPaint(hWnd, &VSplitPs);
            break;
        case WM_COMMAND:
            switch(wParam)
            {
                case (DEBUGTOOLBAR_RUN):
                    InStepMode = 0;
                    SnapRunstate();
                    FlushBranchBreakPoints();
                    RunThread(CurrentDThread);
                    BringThreadToForeGround();
                    return(0);
                case (DEBUGTOOLBAR_STEPINTO):
                    ClearSingleStep(CurrentDThread);
                    InStepMode = 0;
                    InStepType = STEP_INTO;
                    // No: we're in single step mode
                    // so find what the next breakpoint shall be
                    SetNextBreak(InStepType);
                    //ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
                    RunThread(CurrentDThread);
                    return(0);
                case (DEBUGTOOLBAR_STEPOVER):
                    ClearSingleStep(CurrentDThread);
                    ////SetSingleStep(CurrentDThread);
                    InStepMode = 0;
                    InStepType = STEP_OVER;
                    // No: we're in single step mode
                    // so find what the next breakpoint shall be
                    SetNextBreak(InStepType);
                    RunThread(CurrentDThread);
                    return(0);
                case (DEBUGTOOLBAR_STEPOUT):
                    ClearSingleStep(CurrentDThread);
                    //SetSingleStep(CurrentDThread);
                    InStepMode = 0;
                    InStepType = STEP_OUT;
                    // No: we're in single step mode
                    // so find what the next breakpoint shall be
                    SetNextBreak(InStepType);
                    RunThread(CurrentDThread);
                    return(0);
                case (DEBUGTOOLBAR_PAUSE):
                    StopThread(CurrentDThread);
                    // (Ensure that we'll not mess with InStepType)
                    InStepMode = 0;
                    if(!IsSourceLine(GetCurrentEIP(CurrentDThread)))
                    {
                        if(BreakOnCaller()) BringThreadToForeGround();
                    }
                    else
                    {
                        // Found it: Set a breakpoint on this line
                        SetBreakPoint(&DebBreakPoints, GetCurrentEIP(CurrentDThread), BP_BRANCH);
                        BringThreadToForeGround();
                    }
                    RunThread(CurrentDThread);
                    return(0);
                case (DEBUGTOOLBAR_STOP):
                    // Close the debugging window
                    KillProcess();
                    return(0);
            }
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case PGN_CALCSIZE:
                    ToolBarGetXYSize(PagerGetNotifiedChild(lParam), &MySize);
                    DockState = DockingBoxGetState(hDebugContainer);
                    ToolBarGetXYSize(PagerGetNotifiedChild(lParam), &MySize);
                    switch(DockState)
                    {
                        case DOCKINGBOX_TOP:
                        case DOCKINGBOX_BOTTOM:
                            PagerChangeOrientation(hDebugPager, (LPNMPGCALCSIZE) lParam, PGS_VERT, &MySize);
                            break;
                        default:
                            PagerChangeOrientation(hDebugPager, (LPNMPGCALCSIZE) lParam, PGS_HORZ, &MySize);
                            break;
                    }
                    return(0);
                case TTN_NEEDTEXT:
                    SpTooltipId = ControlGetNotifiedID(lParam);
                    if(SpTooltipId <= DEBUGTOOLBAR_STEPOUT)
                    {
                        ToolBarDisplayToolTip(DebugToolbarToolTip[SpTooltipId], lParam);
                    }
                    return(0);
                case TVN_ITEMEXPANDING:
                    hTreeViewFrom = ControlGetNotifiedhWnd(lParam);
                    if(TreeViewGetExpandingItem(lParam) != hTreeViewDebugRoot)
                    {
                        switch(TreeViewGetExpandingState(lParam))
                        {
                            case TVE_EXPAND:
                                TreeViewSetItemIcon(hTreeViewFrom, TreeViewGetExpandingItem(lParam), ICON_OPEN);
                                break;
                            case TVE_COLLAPSE:
                                TreeViewSetItemIcon(hTreeViewFrom, TreeViewGetExpandingItem(lParam), ICON_FILES);
                                break;
                        }
                    }
                    return(0);
            }
            break;
        case DOCKINGBOX_MSG_QUERY_STATE:
            return(ShowDebugDockWin);
        case DOCKINGBOX_MSG_CLOSE:
            ShowDebugDockWin = 0;
        case WM_DESTROY:
            SetDebugDockMenuState(ShowDebugDockWin);
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Show/Hide debug treeview
void SetDebugDockState(long bShow)
{
    SetDebugDockMenuState(bShow);
    switch(bShow)
    {
        case 0:
            DockingBoxRemove(hDebugContainer);
            break;
        case 1:
            DockingBoxShow(hDebugContainer, GetMDIWidth(), GetMDIHeight(), 120, 230, DOCKINGBOX_LEFT);
            break;
    }
}

// -----------------------------------------------------------------------
// Set debug treeview menu state
// (MF_BYPOSITION so -1)
void SetDebugDockMenuState(long bShow)
{
    switch(bShow)
    {
        case 0:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_DEBUGDOCK_ID, MF_BYPOSITION | MF_UNCHECKED);
            break;
        case 1:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_DEBUGDOCK_ID, MF_BYPOSITION | MF_CHECKED);
            break;
    }
}

// -----------------------------------------------------------------------
// Registers dock proc
LRESULT CALLBACK RegistersContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT VSplitPs;
    long DockState;
    
    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &VSplitPs);
            DockState = DockingBoxGetState(hRegistersContainer);
            switch(DockState)
            {
                case DOCKINGBOX_LEFT:
                case DOCKINGBOX_RIGHT:
                case DOCKINGBOX_FLOATING:
                case DOCKINGBOX_TOP:
                case DOCKINGBOX_BOTTOM:
                    MoveWindow(hRegistersListView, 0, 0, ControlWidth(hWnd), ControlHeight(hWnd), 1);
                    ListViewSetColWidth(hRegistersListView, 0, ControlClientWidth(hWnd) - GetSystemMetrics(SM_CXVSCROLL));
                    break;
            }
            EndPaint(hWnd, &VSplitPs);
            break;
        case DOCKINGBOX_MSG_QUERY_STATE:
            return(ShowRegistersDockWin);
        case DOCKINGBOX_MSG_CLOSE:
            ShowRegistersDockWin = 0;
        case WM_DESTROY:
            SetDebugDockMenuState(ShowRegistersDockWin);
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Show/Hide registers window
void SetRegistersDockState(long bShow)
{
    SetRegistersDockMenuState(bShow);
    switch(bShow)
    {
        case 0:
            DockingBoxRemove(hRegistersContainer);
            break;
        case 1:
            DockingBoxShow(hRegistersContainer, GetMDIWidth(), GetMDIHeight(), 300, 120, DOCKINGBOX_BOTTOM);
            break;
    }
}

// -----------------------------------------------------------------------
// Set registers menu state
// (MF_BYPOSITION so -1)
void SetRegistersDockMenuState(long bShow)
{
    switch(bShow)
    {
        case 0:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_REGISTERSDOCK_ID, MF_BYPOSITION | MF_UNCHECKED);
            break;
        case 1:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_REGISTERSDOCK_ID, MF_BYPOSITION | MF_CHECKED);
            break;
    }
}

// -----------------------------------------------------------------------
// Goto the relevant source line of an address
// Return 1 if line found and displayed / 0 otherwise
int DisplaySourceLine(long ThreadID, DWORD Address)
{
    DWORD LineDisplacement = 0;
    HWND SourcehWnd;
    int i;
    int Found = FALSE;

    memset(&ModuleLine, 0, sizeof(IMAGEHLP_LINE));
    ModuleLine.SizeOfStruct = sizeof(ModuleLine);
    ModuleLine.Address = (DWORD) Address;

    LineDisplacement = 0;
    if(SymGetLineFromAddr(DebuggedProc.hProcess, Address, &LineDisplacement, &ModuleLine) != 0)
    {
        // Handle breakpoint
        SourcehWnd = GoToLine(ModuleLine.FileName, ModuleLine.LineNumber - 1, 1);
        if(SourcehWnd != 0)
        {
            for(i = 0; i < DebWindows.Amount(); i++)
            {
                if(DebWindows.Get(i)->Content == SourcehWnd)
                {
                    Found = TRUE;
                    break;
                }
            }
            ChildStruct = LoadStructure(SourcehWnd);
            SetTopFromLine(ChildStruct->hChildCodeMax, ModuleLine.LineNumber - 1);
            // Set the highlihghted line
            CM_SetHighlightedLine(ChildStruct->hChildCodeMax, ModuleLine.LineNumber - 1);
            if(Found == FALSE)
            {
                // Window is already in the list
                SetChildReadOnly(SourcehWnd, TRUE);
                DebWindows.Add(SourcehWnd);
                RefreshChildTitle(SourcehWnd);
            }
        }
        // Bring us in front
        SetForegroundWindow(hMDIform.hWnd);
        // Pause and mark
        SetSingleStep(ThreadID);
        InStepMode = 1;
        return(1);
    }
    else
    {
        return(0);
    }
}

// -----------------------------------------------------------------------
// Return 1 if address is legal and can be displayed / 0 otherwise
int IsSourceLine(DWORD Address)
{
    DWORD LineDisplacement = 0;
    IMAGEHLP_LINE ModuleLineTest;

    memset(&ModuleLineTest, 0, sizeof(IMAGEHLP_LINE));
    ModuleLineTest.SizeOfStruct = sizeof(ModuleLineTest);
    ModuleLineTest.Address = (DWORD) Address;
    LineDisplacement = 0;
    return(SymGetLineFromAddr(DebuggedProc.hProcess, Address, &LineDisplacement, &ModuleLineTest));
}

// -----------------------------------------------------------------------
// Return 1 if address is legal and can be displayed / 0 otherwise
int GetSourceLineNumber(DWORD Address)
{
    DWORD LineDisplacement = 0;
    IMAGEHLP_LINE ModuleLineTest;

    memset(&ModuleLineTest, 0, sizeof(IMAGEHLP_LINE));
    ModuleLineTest.SizeOfStruct = sizeof(ModuleLineTest);
    ModuleLineTest.Address = (DWORD) Address;
    LineDisplacement = 0;
    SymGetLineFromAddr(DebuggedProc.hProcess, Address, &LineDisplacement, &ModuleLineTest);
    return(ModuleLineTest.LineNumber);
}

// -----------------------------------------------------------------------
// Move text to ensure that a line is visible in a codemax control
void SetTopFromLine(HWND hWnd, long LineNumber)
{
    if(LineNumber < GetCurrentTopIndex(hWnd))
    {
        SetCurrentTopIndex(hWnd, LineNumber);
    }
    else
    {
        if(LineNumber > (GetCurrentTopIndex(hWnd) + CM_GetVisibleLineCount(hWnd, CM_GetCurrentView(hWnd), 1)))
        {
            SetCurrentTopIndex(hWnd, LineNumber);
        }
    }
}

// -----------------------------------------------------------------------
// Bring any window of the currently debugged thread into foreground
void BringThreadToForeGround(void)
{
    EnumThreadWindows(DebuggedProc.dwThreadId, &EnumThreadWins, 0);
    for(int i = 0; i < DbWinH.Amount(); i++)
    {
        if(DbWinH.Get(i)->Content != 0)
        {
            // Bring'em to light !
            SetForegroundWindow(DbWinH.Get(i)->Content);
        }
    }
    DbWinH.Erase();
}

int CALLBACK EnumThreadWins(HWND hWnd, long lParam)
{
    DbWinH.Add(hWnd);
    return(1);
}

/* ------------------------------------------------------- REPORTER */
/* this is x86dis.c code that needs to be ripped off and used in x86dis.c */
void x86dis_reporter(enum x86_report_codes code, void *arg)
{
    /* here we could examine the error and do something useful;
     * instead we just print that an error occurred */
    switch(code)
    {
        case report_disasm_bounds:
            break;
        case report_insn_bounds:
            break;
        case report_invalid_insn:
            break;
        case report_unknown:
        default:    /* make GCC shut up */
            break;
    }
}

// -----------------------------------------------------------------------
// Set breakpoint in the line(s) on our next step
// by handling machine instructions:
// jxx
// Loopxx
// Call
// Jmp
// (Note: if we're here ModuleLine has been initialized because we're coming
// from the single step exception).

#define Get_DWORD1 ((Byte_Buffer[1]) | (Byte_Buffer[2] << (8 * 1)) | (Byte_Buffer[3] << (8 * 2)) | (Byte_Buffer[4] << (8 * 3)))
#define Get_DWORD2 ((Byte_Buffer[2]) | (Byte_Buffer[3] << (8 * 1)) | (Byte_Buffer[4] << (8 * 2)) | (Byte_Buffer[5] << (8 * 3)))

void SetNextBreak(int StepType)
{
    int instr_size;
    DWORD UpperBound;
    DWORD LowerBound;
    BYTE CodeByte;
    WORD CodeWord;
    DWORD CodeDWord;
    BYTE *Byte_Buffer;
    IMAGEHLP_LINE TestModuleLine;
    signed char Signed_byte;
    DWORD LineDisplacement = 0;
    DWORD StartAddress;
    DWORD CurrentEIP;
    x86_insn_t insn;
    int FirstLine;
    int WasCondition;

    // First we need to get the address of the next line
    // to obtain an upper boundary.
    memset(&TestModuleLine, 0, sizeof(IMAGEHLP_LINE));
    TestModuleLine.SizeOfStruct = sizeof(TestModuleLine);
    CurrentEIP = GetCurrentEIP(CurrentDThread);
    TestModuleLine.Address = CurrentEIP;
    // Check if the single step made the code to run out of sight
    if(!IsSourceLine(TestModuleLine.Address))
    {
        // Yes: it must have been modified by somekind of call
        // Let's try to obtain the caller
        // Return address should be on the stack at offset 0 since there
        // can't be arguments here
        TestModuleLine.Address = GetCurrentDWord(CurrentDThread, GetCurrentESP(CurrentDThread));
        if(!IsSourceLine(TestModuleLine.Address))
        {
            // We can't obtain it: must be a jmp to valhalla, let's run directly.
            SnapRunstate();
            goto SetOnCaller;
        }
        CurrentEIP = TestModuleLine.Address;
    }
    if(StepType == STEP_OUT)
    {
        if(BreakOnCaller())
        {
            // Since we don't know how many instructions will be executed...
            SnapRunstate();
            goto SetOnCaller;
        }
        // Breakpoint couldn't be set correctly: proceed with next line
    }
    SymGetLineFromAddr(DebuggedProc.hProcess, TestModuleLine.Address, &LineDisplacement, &TestModuleLine);
    StartAddress = TestModuleLine.Address;
    // Get next line
    if(!SymGetLineNext(DebuggedProc.hProcess, &TestModuleLine))
    {
        // (Try this)
        TestModuleLine.Address = StartAddress + 32;
    }

    // Set lower bound
    LowerBound = CurrentEIP;
    // Set upper bound
    UpperBound = TestModuleLine.Address;
    FirstLine = GetSourceLineNumber(LowerBound);
    while(LowerBound < UpperBound)
    {
        // Allocate required buffer
        Byte_Buffer = (BYTE *) malloc(UpperBound - LowerBound);
        // Clear the buffer in case ReadThreadbytes reach a premature end.
        memset(Byte_Buffer, 0, UpperBound - LowerBound);
        if(!Byte_Buffer)
        {
            // We're *really* unlucky if we're getting here ;(
            WriteToDebug("Not enough memory for operation.");
            KillProcess();
            ContinueDebugEvent(MyDebug.dwProcessId, MyDebug.dwThreadId, DBG_CONTINUE);
            return;
        }
        // Read the bytes in the process
        ReadThreadbytes(CurrentDThread, LowerBound, Byte_Buffer, UpperBound - LowerBound);

        // Now disasemble it...
        instr_size = x86_disasm(Byte_Buffer, (UpperBound - LowerBound), 0, 0, &insn);
        if(instr_size == 0)
        {
            // Illegal instruction  encountered
            free(Byte_Buffer);
            goto SetOnCaller;
        }

        // Check if the instruction is known to be a branch...
        CodeWord = (Byte_Buffer[0] << (8 * 1)) | Byte_Buffer[1];
        CodeDWord = 0;
        WasCondition = FALSE;

        // 2 bytes instructions
        switch(CodeWord)
        {
            // near versions
            case 0x0f87:    // JA/JNBE
            case 0x0f83:    // JAE/JNB/JNC
            case 0x0f82:    // JB/JC/JNAE
            case 0x0f86:    // JBE/JNA
            case 0x0f84:    // JE/JZ/
            case 0x0f8f:    // JG/JNLE
            case 0x0f8d:    // JGE/JNL/JNP
            case 0x0f8c:    // JL/JNGE
            case 0x0f8e:    // JLE/JNG
            case 0x0f85:    // JNE/JNZ
            case 0x0f81:    // JNO
            case 0x0f89:    // JNS
            case 0x0f80:    // JO
            case 0x0f8a:    // JP/JPE
            case 0x0f8b:    // JPO
            case 0x0f88:    // JS
                // Verified condition
                CodeDWord = LowerBound + 6 + Get_DWORD2;
                WasCondition = TRUE;    // (We'll need a second breakpoint right after the condition)
                break;
            default:
                // 1 byte instructions
                CodeByte = Byte_Buffer[0];
                switch(CodeByte)
                {
                    // short version
                    case 0x77:      // JA/JNBE
                    case 0x73:      // JAE/JNB/JNC
                    case 0x72:      // JB/JC/JNAE
                    case 0x76:      // JBE/JNA
                    case 0xe3:      // JCXZ
                    case 0x74:      // JE/JZ
                    case 0x7f:      // JG/JNLE
                    case 0x7d:      // JGE/JNL
                    case 0x7c:      // JL/JNGE
                    case 0x7e:      // JLE/JNG/JNS
                    case 0x75:      // JNE/JNZ
                    case 0x71:      // JNO
                    case 0x7b:      // JNP/JPO
                    case 0x70:      // JO
                    case 0x7a:      // JP/JPE
                    case 0x78:      // JS
                        Signed_byte = (signed char) Byte_Buffer[1];
                        CodeDWord = LowerBound + 2 + ((signed long) Signed_byte);
                        WasCondition = TRUE;
                        break;

                    case 0xeb:      // JMP rel8
                        Signed_byte = (signed char) Byte_Buffer[1];
                        CodeDWord = LowerBound + 2 + ((signed long) Signed_byte);
                        break;
                    case 0xe9:      // JMP rel16/rel32
                        CodeDWord = LowerBound + 5 + Get_DWORD1;
                        break;
                    case 0xea:      // JMP far (16:16/32:32)
                        CodeDWord = Get_DWORD1;
                        break;

                    case 0xe8:      // CALL rel16/rel32
                        if(StepType == STEP_INTO) CodeDWord = LowerBound + 5 + Get_DWORD1;
                        break;
                    case 0x9a:      // CALL far (16:16/32:32)
                        if(StepType == STEP_INTO) CodeDWord = Get_DWORD1;
                        break;

                    case 0xe2:      // LOOP
                    case 0xe1:      // LOOPE/LOOPZ
                    case 0xe0:      // LOOPNE/LOOPNZ
                        CodeDWord = LowerBound + 2 + Get_DWORD1;
                        break;
            
                    case 0xc3:      // RET near
                    case 0xcb:      // RET far
                    case 0xc2:      // RET imm16 near
                    case 0xca:      // RET imm16 far
                    case 0xcf:      // IRET/IRETD
                        // Set a breakpoint after the calling instruction and go on...
                        if(BreakOnCaller()) goto SetOnCaller;
                        break;
                }
        }
        free(Byte_Buffer);
        // a branch has been found..
        if(CodeDWord)
        {
            // Check if the destination is suitable for us
            if(IsSourceLine(CodeDWord))
            {
                LowerBound = CodeDWord;
                // Yes: add a new break point into the list
                // of possible branching
                if(CurrentEIP != LowerBound)
                {
                    SetBreakPoint(&DebBreakPoints, CodeDWord, BP_BRANCH);
                    if(WasCondition)
                    {
                        memset(&TestModuleLine, 0, sizeof(IMAGEHLP_LINE));
                        TestModuleLine.SizeOfStruct = sizeof(TestModuleLine);
                        TestModuleLine.Address = CurrentEIP;
                        SymGetLineFromAddr(DebuggedProc.hProcess, CurrentEIP, &LineDisplacement, &TestModuleLine);
                        if(SymGetLineNext(DebuggedProc.hProcess, &TestModuleLine))
                        {
                            SetBreakPoint(&DebBreakPoints, TestModuleLine.Address, BP_BRANCH);
                        }
                    }
                    goto SetOnCaller;
                }
            }
            else
            {
                // Not suitable: pass over and continue
                LowerBound += instr_size;
                if(GetSourceLineNumber(LowerBound) != FirstLine) break;
            }
        }
        else
        {
            // Use calculated length to jump to the next instruction
            // if it lower than the upperbound.
            LowerBound += instr_size;
        }
    }
    // Proceed normally with the next line
    // Except in the case of a jump
    memset(&TestModuleLine, 0, sizeof(IMAGEHLP_LINE));
    TestModuleLine.SizeOfStruct = sizeof(TestModuleLine);
    TestModuleLine.Address = CurrentEIP;
    SymGetLineFromAddr(DebuggedProc.hProcess, CurrentEIP, &LineDisplacement, &TestModuleLine);
    // Was it a composed line (pointer went backward to beginning of it) ?
    if(SymGetLineNext(DebuggedProc.hProcess, &TestModuleLine))
    {
        // Put a branch breakpoint at the next known line
        // (instead of a volatile one so that it should resolve the JMP issue)
        // Since it will be discarded anyway even if the code
        // doesn't step on it.
        SetBreakPoint(&DebBreakPoints, TestModuleLine.Address, BP_BRANCH);
    } // Invalid line will be executed directly (and triggers an exception).

SetOnCaller:
    return;
}

// -----------------------------------------------------------------------
// Set a breakpoint on a return address
// Return 1 if breakpoint was set 0 otherwise
int BreakOnCaller(void)
{
    DWORD CallerAddress;

    CallerAddress = GetCaller();
    if(IsSourceLine(CallerAddress))
    {
        FlushBranchBreakPoints();
        SetBreakPoint(&DebBreakPoints, CallerAddress, BP_BRANCH);
        return(1);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Retrieve a return address or 0
DWORD GetCaller(void)
{
    STACKFRAME Frame;
    CONTEXT FrameContext;
    
    // The code is running out of scope:
    // Backtrace it!
    memset(&FrameContext, 0, sizeof(CONTEXT));
    memset(&Frame, 0, sizeof(STACKFRAME));
    FrameContext.ContextFlags = CONTEXT_FULL | CONTEXT_FLOATING_POINT | CONTEXT_DEBUG_REGISTERS | CONTEXT_EXTENDED_REGISTERS;
    GetThreadContext(GetThreadHandle(CurrentDThread), &FrameContext);
    Frame.AddrStack.Mode = AddrModeFlat;
    Frame.AddrStack.Offset = FrameContext.Esp;
    Frame.AddrPC.Mode = AddrModeFlat;
    Frame.AddrPC.Offset = FrameContext.Eip;
    Frame.AddrFrame.Mode = AddrModeFlat;
    Frame.AddrFrame.Offset = FrameContext.Ebp;
    while(1)
    {
        if(!StackWalk(IMAGE_FILE_MACHINE_I386,
                      DebuggedProc.hProcess,
                      GetThreadHandle(CurrentDThread),
                      &Frame, NULL, NULL, &SymFunctionTableAccess,
                      &SymGetModuleBase, NULL))
        {
            return(0);
        }
        if(IsSourceLine(Frame.AddrReturn.Offset)) return(Frame.AddrReturn.Offset);
    }
    return(Frame.AddrReturn.Offset);
}
