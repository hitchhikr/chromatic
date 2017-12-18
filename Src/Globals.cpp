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
// Globals.cpp: declarations of global variables
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"

// -----------------------------------------------------------------------
// Variables
HWND CurrentForm;
CStr CurrentLanguage;
long NbForms;
CList <char *> RegisteredExts;
CStr CurrentDir;
CStr NotifiedCurrentdir;
long DirChangedNotify;
CStr LanguageToOpen;
CStr FileToOpen;
CStr FileUnderCursor;
long LoadResultGlob;
CStr PassValueSnippet;
CStr PassValueTemplate;

CStr MSDNDir;
CStr MSDNWord;

CStr WorkSpaceFileName;
long WorkSpaceNumber;

CStr LastLoadDir;
CStr LastLoadAsDBDir;
CStr LastIncludeDir;
CStr LastIncludeDBDir;
CStr LastWorkSpaceDir;
CStr LastSaveDir;
CStr LastConvertBlockDir;
CStr LastSaveBlockDir;
CStr LastExeDir;
CStr LastUserLoadDir;
CStr LastUserSaveDir;
CStr LastAddInDir;

// -----------------------------------------------------------------------
// Last resources dirs
CStr LastIconsDir;
CStr LastCursorsDir;
CStr LastBitmapsDir;
CStr LastStringsDir;
CStr LastAcceleratorsDir;
CStr LastMenusDir;
CStr LastDialogsDir;
CStr LastRawsDir;

CList <char *> Recents;
CList <char *> RecentsPrj;

CStr TextToOp;
CStr TextToOpEnd;

long FoundOpened;
HWND FoundOpenedhWnd;
CStr SearchFileName;
//char *TabHex[16 + 1];
char *TabRc[16 + 1];
char *TabRcComma[16 + 1];
char *TabRcCommaEnd[16 + 1];
long TabConvHexCoher[23 + 1];
CStr TabLangRc[87 + 1];
char *TabAccelerators[84 + 1];

// -----------------------------------------------------------------------
// Startup variables
long AppVersion;
long AppRevMaj;
long AppRevMin;
CStr AppPath;
CStr VersionString;
CStr AppTitle;
CStr Requesters;
HINSTANCE ApphInstance;
CStr AppCredits;
CStr PassedFile;
HANDLE BroadCastMutex;
UINT BroadCastMessage;
long AddOnMessage;

CList <char *> SnipArray;
CList <char *> TemplatesArray;
CList <char *> LUAArray;
CList <char *> RunProgArray;
CList <char *> FavoritesArray;
CList <LPFILTER_ENTRY> FiltersArray;
long OldCursor;

// -----------------------------------------------------------------------
// Public directories
// 0=ROOTDIR
// 1=BINDIR
// 2=INCLUDEDIR
// 3=LIBDIR
// 4=HELPDIR
// 5=MAINDIR
// 6=WINDIR
// 7=WINSYSDIR
// 8=TEMPDIR
// 9=TEMPLATESDIR
// 10=PROJECTSDIR
// 11=MAINHELPDIR
// 12=SNIPPETSDIR
// 13=DATABASESDIR
// 14=SKINSDIR
// 15=LANGUAGESDIR
// 16=DOWNLOADSDIR
// 17=ADDINSDIR
// 18=CONFIGDIR
// 19=MENUSDIR
// 20=WIZARDSDIR
// 21=FILTERSDIR
// 22=EXTRACODEDIR
// 23=LANGSCRIPTSDIR
// 24=TOOLSDIR
// 25=SCRIPTSDIR
char *Dirs[DIR_MAX + 1];
char *DirsRepl[DIR_MAX + 1];

// -----------------------------------------------------------------------
// CodeMax preferences
long ShowStatusbar;
long ShowProjetDockWin;
long ShowWindowsDockWin;
long ShowOutputDockWin;
long ShowDebugDockWin;
long ShowRegistersDockWin;
long LeftM;
long White;
long ShowH;
long SyntaxH;
long OutputDates;
long IndentScope;
long EnDragDrop;
long Remember;
long LineTTip;
long APITTip;
long Smooth;
long CursorFollow;
long AcceptFiles;
long ItalicFont;
long Wrap;
long AutoSaveDelay;
long VerboseSave;
long AutoSave;
long AutoExpand;
long CreateBak;
long DecorateBak;
long HorizSplit;
long DetectChange;
long AutoCorrectKey;
long MaxRecent;
long MaxRecentPrj;
long LnStyle;
long LnStart;
long Tabulations;
CStr CurFontName;
long CurFontSize;
long CurFontBold;
HFONT CurFontHandle;
long AutoMaximizeChilds;
long AutoClearOut;
COLORREF CurBackColor;
COLORREF CurNormTextColor;
COLORREF CurHighTextColor;
COLORREF CurMarginColor;
COLORREF CurLineColor;
long UseTVToolTips;
long UseWinRecent;
long FirstStart;

// -----------------------------------------------------------------------
// PSApi imports
HMODULE PSApiDll;
BOOL (__stdcall *Ptr_GetWsChanges)(HANDLE, PPSAPI_WS_WATCH_INFORMATION, DWORD);
BOOL (__stdcall *Ptr_InitializeProcessForWsWatch)(HANDLE);

// -----------------------------------------------------------------------
// User commands variables
long UserArgs;
CStr CommandToEx;
long ArgRequired;
CStr EnteredArg;

COLORREF LangNumbersColor;
COLORREF LangStringsColor;
COLORREF LangCommentsColor;
COLORREF LangKeyWordsColor;
COLORREF LangColorAColor;
COLORREF LangColorBColor;
COLORREF LangColorCColor;
COLORREF LangColorDColor;
COLORREF LangColorEColor;
COLORREF LangColorFColor;
COLORREF LangColorGColor;
COLORREF LangColorHColor;
COLORREF APIColor;

long NbrCmds;
CList <HWND> MainMenuH;
CList <char *> MenusCmds;
CList <char *> MenusCmdsLbl;
CList <char *> MenusCmdsComments;
CList <int> MenusCmdsHelp;

// -----------------------------------------------------------------------
// Richtextbox emulation variables
CStr EmulTxtToRep;

// -----------------------------------------------------------------------
// Search variables
// Search processed or not ?
long OkSearch;
CStr LastSearch;
CStr LastWild;
long LastSearchFiles;
long LastSearchLines;
long ProcLines;
CStr ProcType;

CStr CurrentFileOpen;
CStr CurrentFileOpenBis;
long CurrentFilePointer;
long CurrentFileSize;
long CurrentFilePointerBis;
long CurrentFileSizeBis;
long CurrentFilePointerBisL;
long CurrentFileSizeBisL;
long CurrentFileLines;
char *CurrentFileCLine;
long CurrentFileLineLen;
CStr CurrentFileDat;
CStr CurrentFileSearchKey;
long CurrentRetriPosition;

CList <char *> TabCheckVars;
CList <int> TabCheckVarsPos;
CList <char *> TabCheckVarsComment;

CList <char *> TabCheckFunc;
CList <int> TabCheckFuncPos;
CList <char *> TabCheckFuncComment;

CStr LoadingState;

// -----------------------------------------------------------------------
// Procedures / Variables searchs
CStr VarProcToSearch;
CStr VarProcToSearchReal;
CStr VarProcText;
CStr VarProcTextStrip;
long VarProcFound;
long VarProcFoundPos;
long VarProcFoundLine;
long VarProcFoundLineAddr;
CStr VarProcFoundLineTxt;
HWND VarProcFoundhwnd;

// -----------------------------------------------------------------------
// Projects variables
long ForcePrj;

// -----------------------------------------------------------------------
// Dos output datas
STARTUPINFO MyProcStartup;
PROCESS_INFORMATION MyProcInfos;

long RefreshProperties;
CStr RetVal;

HANDLE PipeIn;
HANDLE PipeOut;

long ExternModified;
CList <HWND> ModifiedArrayhWnd;
long QuitNotif;
long InQuitProc;

CList <char *> LastOpened;
CList <int> LastOpenedLine;
CList <int> LastOpenedCol;
CList <int> LastOpenedFirst;

// -----------------------------------------------------------------------
// Ini filenames
CStr MainIniFile;
CStr UserMenusIniFile;
CStr ToolbarsIniFile;
CStr ToolbarsTMPIniFile;
CStr LanguagesIniFile;
CStr ColorsIniFile;
CStr FtpAccountsIniFile;
CStr SkinsIniFile;
CStr cGrepIniFile;
CStr DatesIniFile;
CStr ProjectsIniFile;
CStr FiltersIniFile;
CStr PrivateWorkSpaceFile;

CStr MsgBoxStr;

CList <char *> FilesList;
CList <HWND> FilesListhWnd;

long RefreshSBStat;

long SBFlags[7 + 1];
long SBPosX[7 + 1];
long SBPosY[7 + 1];

// -----------------------------------------------------------------------
// OS infos
long WindowsNT = 0;
long Windows2K = 0;
long WindowsXP = 0;
long Windows7 = 0;

long FullScreenMode;
long SysTrayMode;
long XPosToRestore;
long YPosToRestore;
long XToRestore;
long YToRestore;

long NotifGrepError;
long NotifDirStructureError;

long Buttons_StaticEdge;

CStr ConversionStringHead;
CStr ConversionStringBody;
CStr ConversionStringBodyEnd;

CStr ResMenusString;

NOTIFYICONDATA SysTrayStruct;
CStr LastProcTypeKeyword;

void (__stdcall *CurrentMDIChildInitProc)(struct HWND__ *);
WNDPROC CurrentMDIChildProc;
WNDPROC CurrentCodeMaxProc;
WNDPROC CurrentMDIProc;

long PrimaryMDIChildInitProc;
WNDPROC PrimaryMDIChildProc;
WNDPROC PrimaryCodeMaxProc;
WNDPROC PrimaryMDIProc;

CList <HWND> ChildshWnd;
CList <int> ChildsLVPos;

CStr EnteredCustComment;

long FncBase;
long ConstBase;

CStr TipFileName;
