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
// Debugger.h: declaration of used variables / structures for Debugger.cpp
// -----------------------------------------------------------------------

#ifndef _DEBUGGER_H_
#define _DEBUGGER_H_

// -----------------------------------------------------------------------
// Constants
#define DEBUGTOOLBAR_RUN 1
#define DEBUGTOOLBAR_PAUSE 2
#define DEBUGTOOLBAR_STOP 3
#define DEBUGTOOLBAR_STEPINTO 4
#define DEBUGTOOLBAR_STEPOVER 5
#define DEBUGTOOLBAR_STEPOUT 6

// -----------------------------------------------------------------------
// Structures
typedef struct _SNAPDLL
{
    HANDLE hFile;
    LPVOID lpBaseOfDll;
    CStr *DllName;
    HTREEITEM TreeViewEntry;
} SNAPDLL, *LPSNAPDLL;

typedef struct _SNAPPROCESS
{
    HANDLE hFile;
    HANDLE hProcess;
    HANDLE hThread;
    LPVOID hThreadLocalBase;
    LPTHREAD_START_ROUTINE ThreadStartAddress;
    CStr ImageName;
    DWORD dwProcessId;
    DWORD dwThreadId;
    DWORD PriorityClass;
    DWORD WorkSize1;
    DWORD WorkSize2;
    DWORD PriorityBoost;
    DWORD ThreadPriorityBoost;
    SYSTEMTIME CreationTime;
    SYSTEMTIME ThreadCreationTime;
    long ProcVersion;
    long ProcMonitor;
    long ThreadPriority;
    LPVOID ImageBase;
    HTREEITEM ProcessTreeViewEntry;
    HTREEITEM DatasTreeViewEntry;
    long SymbolsLoaded;
    CONTEXT LastContext;
} SNAPPROCESS, *LPSNAPPROCESS;

typedef struct _SNAPBP
{
    DWORD Address;
    BYTE OldByte;
    int BPType;
} SNAPBP, *LPSNAPBP;

// -----------------------------------------------------------------------
// Variables
extern HWND hWndDebug;
extern HWND hDebugContainer;
extern HWND hRegistersContainer;
extern STARTUPINFO MyStartup;
extern PROCESS_INFORMATION MyProc;
extern DEBUG_EVENT MyDebug;
extern CStr DeCStr;
extern long AdrToRead;
extern long DebugOutput;
extern unsigned long ExCode;
extern long ProcesshFile;
extern CONTEXT MyContext;
extern long ESPAddress;
extern BYTE AdrBuffer[3 + 1];
extern BYTE ESPBuffer[266 + 1];
extern BYTE SMBuffer[73 + 1];
extern long SnapRunning;
extern long showMeMAddr;
extern long ThreadAddr;
extern long OldshowMeMAddr;
extern long Datatoshow;
extern long SnapFileAddress;
extern long SnapFileLen;
extern long DebugfileMem;
extern long DebugFileAddr;
extern CStr DebugFileChar;
extern CStr DebugFile;
extern BYTE AdrShow[12 + 1];
extern long Threadrunning;
extern HANDLE ChildhFile;
extern long InCount;
extern long CountStart;
extern long CountEnd;
extern long InBytes;
extern long BytesStart;
extern long BytesEnd;
extern long ForcedExit;
extern CList <HWND> DbWinH;
extern unsigned long RWProcess;
extern long CurrentDThread;
extern FILETIME lpProcCreationTm;
extern FILETIME lpProcExitTm;
extern FILETIME lpProcKModeTm;
extern FILETIME lpProcUModeTm;
extern SYSTEMTIME lpProcCreationSysTm;
extern SYSTEMTIME lpProcExitSysTm;
extern SYSTEMTIME lpProcKModeSysTm;
extern SYSTEMTIME lpProcUModeSysTm;
extern FILETIME lpThreadCreationTm;
extern FILETIME lpThreadExitTm;
extern FILETIME lpThreadKModeTm;
extern FILETIME lpThreadUModeTm;
extern SYSTEMTIME lpThreadCreationSysTm;
extern SYSTEMTIME lpThreadExitSysTm;
extern SYSTEMTIME lpThreadKModeSysTm;
extern SYSTEMTIME lpThreadUModeSysTm;
extern long InException;
extern long SysTabItem;

// -----------------------------------------------------------------------
// Functions declarations
long WaitForDebugOutput(CStr FileName, CStr Arguments);
void SnapRunstate(void);
void SnapDeadEndstate(void);
void SnapPausestate(void);
void SnapDeadstate(void);
void KillProcess(void);
long GetProcessBoost(HANDLE hProcess);
long GetThreadBoost(HANDLE hThread);
long GetProcessTime(HANDLE hProcess);
long GetThreadTime(HANDLE hThread);
void CreateSnapTreeView(void);
void RemoveThread(long dwThreadID);
HANDLE GetThreadHandle(long dwThreadID);
HANDLE GetProcessHandle(long dwProcessID);
void SetDebugDockState(long bShow);
void SetDebugDockMenuState(long bShow);
void SetRegistersDockState(long bShow);
void SetRegistersDockMenuState(long bShow);

void Get_PSAPI(void);
void Close_PSAPI(void);
int Ask_Debug_Break(void);

#endif
