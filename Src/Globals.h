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
// Globals.h: declarations of used variables / structures /constants for Globals.cpp
// -----------------------------------------------------------------------

#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// -----------------------------------------------------------------------
// Constants
#define POS_BEFORE 0
#define POS_AFTER 1

#define DIR_ROOT 0
#define DIR_BIN 1
#define DIR_INCLUDE 2
#define DIR_LIB 3
#define DIR_HELP 4
#define DIR_MAIN 5
#define DIR_WIN 6
#define DIR_WINSYS 7
#define DIR_TEMP 8
#define DIR_TEMPLATES 9
#define DIR_PROJECTS 10
#define DIR_MAINHELP 11
#define DIR_SNIPPETS 12
#define DIR_DATABASES 13
#define DIR_SKINS 14
#define DIR_LANGUAGES 15
#define DIR_DOWNLOADS 16
#define DIR_ADDINS 17
#define DIR_CONFIG 18
#define DIR_MENUS 19
#define DIR_WIZARDS 20
#define DIR_FILTERS 21
#define DIR_EXTRACODE 22
#define DIR_LANGSCRIPTS 23
#define DIR_TOOLS 24
#define DIR_SCRIPTS 25
#define DIR_MAX 25

// Type of os passed to addin contexts
#define OSTYPE_WINDOWS_9X 0
#define OSTYPE_WINDOWS_NT 1
#define OSTYPE_WINDOWS_2K 2
#define OSTYPE_WINDOWS_XP 4
#define OSTYPE_WINDOWS_7 8

#define USE_SECURITY_DESCRIPTOR 1
#define NO_SECURITY_DESCRIPTOR 0

// -----------------------------------------------------------------------
// Structures
typedef struct
{
    char *Name;
    int Type;
} FILTER_ENTRY, *LPFILTER_ENTRY;

// -----------------------------------------------------------------------
// Variables
extern HWND CurrentForm;
extern CStr CurrentLanguage;
extern long NbForms;
extern CList <char *> RegisteredExts;
extern CStr CurrentDir;
extern CStr NotifiedCurrentdir;
extern long DirChangedNotify;
extern CStr LanguageToOpen;
extern CStr FileToOpen;
extern CStr FileUnderCursor;
extern long LoadResultGlob;
extern CStr PassValueSnippet;
extern CStr PassValueTemplate;

extern CStr MSDNDir;
extern CStr MSDNWord;

extern CStr WorkSpaceFileName;
extern long WorkSpaceNumber;

extern CStr LastLoadDir;
extern CStr LastLoadAsDBDir;
extern CStr LastIncludeDir;
extern CStr LastIncludeDBDir;
extern CStr LastWorkSpaceDir;
extern CStr LastSaveDir;
extern CStr LastConvertBlockDir;
extern CStr LastSaveBlockDir;
extern CStr LastExeDir;
extern CStr LastUserLoadDir;
extern CStr LastUserSaveDir;
extern CStr LastAddInDir;

// -----------------------------------------------------------------------
// Last resources dirs
extern CStr LastIconsDir;
extern CStr LastCursorsDir;
extern CStr LastBitmapsDir;
extern CStr LastStringsDir;
extern CStr LastAcceleratorsDir;
extern CStr LastMenusDir;
extern CStr LastDialogsDir;
extern CStr LastRawsDir;

extern CList <char *> Recents;
extern CList <char *> RecentsPrj;

extern CStr TextToOp;
extern CStr TextToOpEnd;

extern long FoundOpened;
extern HWND FoundOpenedhWnd;
extern CStr SearchFileName;
//extern char *TabHex[16 + 1];
extern char *TabRc[16 + 1];
extern char *TabRcComma[16 + 1];
extern char *TabRcCommaEnd[16 + 1];
extern long TabConvHexCoher[23 + 1];
extern CStr TabLangRc[87 + 1];
extern char *TabAccelerators[84 + 1];

// -----------------------------------------------------------------------
// Startup variables
extern long AppVersion;
extern long AppRevMaj;
extern long AppRevMin;
extern CStr AppPath;
extern CStr VersionString;
extern CStr AppTitle;
extern CStr Requesters;
extern HINSTANCE ApphInstance;
extern CStr AppCredits;
extern CStr PassedFile;
extern HANDLE BroadCastMutex;
extern UINT BroadCastMessage;
extern long AddOnMessage;

extern CList <char *> SnipArray;
extern CList <char *> TemplatesArray;
extern CList <char *> LUAArray;
extern CList <char *> RunProgArray;
extern CList <char *> FavoritesArray;
extern long OldCursor;

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
extern char *Dirs[DIR_MAX + 1];
extern char *DirsRepl[DIR_MAX + 1];

// -----------------------------------------------------------------------
// CodeMax preferences
extern long ShowStatusbar;
extern long ShowProjetDockWin;
extern long ShowWindowsDockWin;
extern long ShowOutputDockWin;
extern long ShowDebugDockWin;
extern long ShowRegistersDockWin;
extern long LeftM;
extern long White;
extern long ShowH;
extern long SyntaxH;
extern long OutputDates;
extern long IndentScope;
extern long EnDragDrop;
extern long Remember;
extern long LineTTip;
extern long APITTip;
extern long Smooth;
extern long CursorFollow;
extern long AcceptFiles;
extern long ItalicFont;
extern long Wrap;
extern long AutoSaveDelay;
extern long VerboseSave;
extern long AutoSave;
extern long AutoExpand;
extern long CreateBak;
extern long DecorateBak;
extern long HorizSplit;
extern long DetectChange;
extern long AutoCorrectKey;
extern long MaxRecent;
extern long MaxRecentPrj;
extern long LnStyle;
extern long LnStart;
extern long Tabulations;
extern CStr CurFontName;
extern long CurFontSize;
extern long CurFontBold;
extern HFONT CurFontHandle;
extern long AutoMaximizeChilds;
extern long AutoClearOut;
extern COLORREF CurBackColor;
extern COLORREF CurNormTextColor;
extern COLORREF CurHighTextColor;
extern COLORREF CurMarginColor;
extern COLORREF CurLineColor;
extern long UseTVToolTips;
extern long UseWinRecent;
extern long FirstStart;

// -----------------------------------------------------------------------
// PSApi imports
extern HMODULE PSApiDll;
extern BOOL (__stdcall *Ptr_GetWsChanges)(HANDLE, PPSAPI_WS_WATCH_INFORMATION, DWORD);
extern BOOL (__stdcall *Ptr_InitializeProcessForWsWatch)(HANDLE);

// -----------------------------------------------------------------------
// User commands variables
extern long UserArgs;
extern CStr CommandToEx;
extern long ArgRequired;
extern CStr EnteredArg;

extern COLORREF LangNumbersColor;
extern COLORREF LangStringsColor;
extern COLORREF LangCommentsColor;
extern COLORREF LangKeyWordsColor;
extern COLORREF LangColorAColor;
extern COLORREF LangColorBColor;
extern COLORREF LangColorCColor;
extern COLORREF LangColorDColor;
extern COLORREF LangColorEColor;
extern COLORREF LangColorFColor;
extern COLORREF LangColorGColor;
extern COLORREF LangColorHColor;
extern COLORREF APIColor;

extern long NbrCmds;
extern CList <HWND> MainMenuH;
extern CList <char *> MenusCmds;
extern CList <char *> MenusCmdsLbl;
extern CList <char *> MenusCmdsComments;
extern CList <int> MenusCmdsHelp;

// -----------------------------------------------------------------------
// Richtextbox emulation variables
extern CStr EmulTxtToRep;

// -----------------------------------------------------------------------
// Search variables
// Search processed or not ?
extern long OkSearch;
extern CStr LastSearch;
extern CStr LastWild;
extern long LastSearchFiles;
extern long LastSearchLines;
extern long ProcLines;
extern CStr ProcType;

extern CStr CurrentFileOpen;
extern CStr CurrentFileOpenBis;
extern long CurrentFilePointer;
extern long CurrentFileSize;
extern long CurrentFilePointerBis;
extern long CurrentFileSizeBis;
extern long CurrentFilePointerBisL;
extern long CurrentFileSizeBisL;
extern long CurrentFileLines;
extern char *CurrentFileCLine;
extern long CurrentFileLineLen;
extern CStr CurrentFileDat;
extern CStr CurrentFileSearchKey;
extern long CurrentRetriPosition;

#define NBR_VARS 78 - 1
extern CList <char *> TabCheckVars;
extern CList <int> TabCheckVarsPos;
extern CList <char *> TabCheckVarsComment;

#define NBR_FNC 8 - 1
extern CList <char *> TabCheckFunc;
extern CList <int> TabCheckFuncPos;
extern CList <char *> TabCheckFuncComment;

extern CStr LoadingState;

// -----------------------------------------------------------------------
// Procedures / Variables searchs
extern CStr VarProcToSearch;
extern CStr VarProcToSearchReal;
extern CStr VarProcText;
extern CStr VarProcTextStrip;
extern long VarProcFound;
extern long VarProcFoundPos;
extern long VarProcFoundLine;
extern long VarProcFoundLineAddr;
extern CStr VarProcFoundLineTxt;
extern HWND VarProcFoundhwnd;

// -----------------------------------------------------------------------
// Projects variables
extern long ForcePrj;

// -----------------------------------------------------------------------
// Dos output datas
extern STARTUPINFO MyProcStartup;
extern PROCESS_INFORMATION MyProcInfos;

extern long RefreshProperties;
extern CStr RetVal;

extern HANDLE PipeIn;
extern HANDLE PipeOut;

extern long ExternModified;
extern CList <HWND> ModifiedArrayhWnd;
extern long QuitNotif;
extern long InQuitProc;

extern CList <char *> LastOpened;
extern CList <int> LastOpenedLine;
extern CList <int> LastOpenedCol;
extern CList <int> LastOpenedFirst;

// -----------------------------------------------------------------------
// Ini filenames
extern CStr MainIniFile;
extern CStr UserMenusIniFile;
extern CStr ToolbarsIniFile;
extern CStr ToolbarsTMPIniFile;
extern CStr LanguagesIniFile;
extern CStr ColorsIniFile;
extern CStr FtpAccountsIniFile;
extern CStr SkinsIniFile;
extern CStr cGrepIniFile;
extern CStr DatesIniFile;
extern CStr ProjectsIniFile;
extern CStr FiltersIniFile;
extern CStr PrivateWorkSpaceFile;

extern CStr MsgBoxStr;

extern CList <char *> FilesList;
extern CList <HWND> FilesListhWnd;

extern long RefreshSBStat;

extern long SBFlags[7 + 1];
extern long SBPosX[7 + 1];
extern long SBPosY[7 + 1];

// -----------------------------------------------------------------------
// OS infos
extern long WindowsNT;
extern long Windows2K;
extern long WindowsXP;
extern long Windows7;

extern long FullScreenMode;
extern long SysTrayMode;
extern long XPosToRestore;
extern long YPosToRestore;
extern long XToRestore;
extern long YToRestore;

extern long NotifGrepError;
extern long NotifDirStructureError;

extern long Buttons_StaticEdge;

extern CStr ConversionStringHead;
extern CStr ConversionStringBody;
extern CStr ConversionStringBodyEnd;

extern CStr ResMenusString;

extern NOTIFYICONDATA SysTrayStruct;
extern CStr LastProcTypeKeyword;

extern void (__stdcall *CurrentMDIChildInitProc)(struct HWND__ *);
extern WNDPROC CurrentMDIChildProc;
extern WNDPROC CurrentCodeMaxProc;
extern WNDPROC CurrentMDIProc;

extern long PrimaryMDIChildInitProc;
extern WNDPROC PrimaryMDIChildProc;
extern WNDPROC PrimaryCodeMaxProc;
extern WNDPROC PrimaryMDIProc;

extern CList <HWND> ChildshWnd;
extern CList <int> ChildsLVPos;

extern CStr EnteredCustComment;

extern long FncBase;
extern long ConstBase;

extern CStr TipFileName;

extern CList <LPFILTER_ENTRY> FiltersArray;

#endif
