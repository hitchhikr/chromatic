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
// MiscFunctions.h: declaration of used variables / structures for MiscFunctions.cpp
// -----------------------------------------------------------------------

#ifndef _MISCFUNCTIONS_H_
#define _MISCFUNCTIONS_H_

// -----------------------------------------------------------------------
// Variables
extern CStr GlobalRetVal;
extern int CursorDisabled;

// -----------------------------------------------------------------------
// Structures
typedef void (*QEPROC)(HINSTANCE, HWND, HWND, HWND, HWND);

// -----------------------------------------------------------------------
// Functions declarations
CStr ChangeRelativePaths(CStr Path, CStr SubsidDir = "");
CStr RemoveRelativePaths(CStr Path);
CStr ChangeAbsolutePaths(CStr Path, long AvoidSource = 0);
void SetCurDir(HWND hWnd);
void RunUserCommandRec(CStr CommandRec);
void Exec(CStr CmdName, CStr CmdFakeName, long IsInHelp);
void RunUserCommand(long CommandNumber);
CStr TranslateUserCmd(CStr cmd);
CStr TranslateCommands(CStr Comd, CStr Cm);
CStr GetCommandArg(CStr *ComLine, long InitPos);
int ChDrive(CStr DriveStr);
int ChDir(CStr DirStr);
CStr CurDir(CStr Drive = 0);
CStr AffSelFile(CStr *SpecifiedDir = 0, CStr *SpecifiedFilters = 0);
CStr AffSelFileSave(CStr *SpecifiedDir = 0, CStr *SpecifiedFilters = 0);
CStr AffSelColor(void);
CStr AffSelColorHex(void);
CStr AffNewChild(void);
CStr Argument(void);
void DisplayChmVB(CStr ChmFile, CStr ChmWord);
long RunDOSCmd(CStr DOSCmdName, CStr DOSCmdArgs, CStr DOSCmdDir, CStr OutputFile);
void CaptureDOSOutput(CStr DOSCmdName, CStr DOSCmdArgs, CStr DOSCmdDir);
CStr GetCurrentFileDir(void);
void PrepareHexTab(void);
void PrepareRcTab(void);
void PrepareRcLanguages(void);
void PrepareAccelerators(void);
CStr PutStatusDatePrefix(void);
long CheckWorkSpaceFile(CStr FileName);
long GetChildState(void);
CStr RemoveQuotesVB(CStr Str);
void PrepareHexConvTab(void);

CStr FindProcedure(CStr FileName, long hFile, CStr hFileCase, long FileLen, long StartPos, LPPOINT ProcPos, CStr CurLangProc, CStr CurLangEndProc);
long CheckProcKeywordValidity(char *FileToCheck, long PosInFile, CStr KeyWord);
CStr GetProcedureName(CStr ProcBlock, long Pos, CStr ProcKeyWord, LPPOINT PointPos);
// Procedures of all modules
CStr GetModulesProc(void);
// Procedures of a file
CStr GetProcedureList(CStr FileName);

long SelectProcedure(HWND hForm);
CStr GetProcName(CStr ProcBlock, CStr ProcKeyWord);

long OpenFileUnderCursor(long FileToInclude, long ExecFile);
long TestUnderFile(CStr FileName);
CStr RemoveComments(CStr Str);
HWND GoToLine(CStr FileName, long LineNumber, long FNoAddRecent);
HWND FocusFile(CStr FileName, long FNoAddRecent);
CStr GetFileNameFromTreeView(HTREEITEM hItem);
CStr GetLabelFromTreeView(HTREEITEM hItem);
void RemFile(CStr FileKey);
void AddFile(CStr FileKey, long j, CStr Info, CStr Comment);
void SearchOpenFile(CStr FileToSearch);
void SearchOpenFileCM(HWND hWnd);
long SearchInText(long Txt, long TxtLen, CStr KeyW, long SearchType, long StartPo);
CStr SearchGetTextLine(long Txt, long TxtLen, long LineNumber);
CStr SearchGetTextLineStr(CStr Txt, long TxtLen, long LineNumber);
CStr SearchGetLines(long LineNumber);
long SearchInFileCM(HWND hWnd, CStr KeyW, long SearchType, long StartPo, CStr CommentC, long RawMode);
long ContinueSearchInFileCM(HWND hWnd, CStr KeyW, long SearchType, long StartPo, CStr CommentC);
long SearchInFile(CStr FileName, CStr KeyW, long SearchType, long StartPo, long RawMode);
long ContinueSearchInFile(CStr FileName, CStr KeyW, long SearchType, long StartPo);
void SearchCloseFile(void);
void FillVarCheckTable(void);
void FillVarEntry(CStr KeyWord, long Position, CStr Comment);
void FillFuncCheckTable(void);
void FillFuncEntry(CStr KeyWord, long Position, CStr Comment);
long SearchVarEntry(HWND hWnd, CStr KeyWord);
int CALLBACK EnumSearchVarEntry(HWND hWnd, long lParam);
long SearchVarNextUse(HWND hWnd, CStr KeyWord, long FirstLine);
int CALLBACK EnumSearchVarNextUse(HWND hWnd, long lParam);
long SearchProcProto(HWND hWnd, CStr KeyWord);
int CALLBACK EnumSearchProcProto(HWND hWnd, long lParam);
long SearchProcDeclare(HWND hWnd, CStr KeyWord);
int CALLBACK EnumSearchProcDeclare(HWND hWnd, long lParam);
long SearchProcNextUse(HWND hWnd, CStr KeyWord, long FirstLine);
int CALLBACK EnumSearchProcNextUse(HWND hWnd, long lParam);
CStr RemoveChr2(CStr Txt);
long ProcSearch(CStr Ln, CStr KeyWord, CStr BoundedKeyWord, long Position);
long OpenUnknownFile(CStr FileName, int AddInRecent);
long CALLBACK EnumUnknownFiles(char *FileToAdd, long UserValue);
void FillFTPCombo(HWND hGlobComboBox);
CStr HexToDec(HWND hWnd, CStr Number);
CStr DecToHex(HWND hWnd, CStr Number);
CStr DecToBin(HWND hWnd, CStr Number);
CStr HexToBin(HWND hWnd, CStr Number);
CStr BinToDec(HWND hWnd, CStr Number);
CStr BinToHex(HWND hWnd, CStr Number);
CStr HexToFlt(HWND hWnd, CStr Number);
CStr FltToHex(HWND hWnd, CStr Number);
long OpenContextHelpFile(CStr HelpFileName, CStr HelpKeyWord);
CStr GetTxtLineFromFile(long hFile, long FileLength, long LineNumber);
int IsFileInProject(char *FileName);
void GetAllFiles(void);
void SearchAllFiles(HWND hWnd);
int CALLBACK EnumFillFilesProc(HWND hWnd, long lParam);
long SearchNextTreeViewEntry(HWND hTree, HTREEITEM hChild, CStr Prefix);
long GetUseFileDir(void);
void SaveState(CStr WorkSpaceName);
void RestoreState(CStr WorkSpaceName);
void ControlSetSysToTray(HINSTANCE hInstance, HWND hWnd, long IDTray, long IconID, CStr ToolTipText);
void RemoveCommentBlocksFromFileByFile(long hFile, long FileLen, CStr FileName);
void RemoveCommentBlocksFromFile(long hFile, long FileLen, HWND hWnd);
void RemoveCommentBlocksFromFileByChild(long hFile, long FileLen, HWND hWnd);
void RemoveCommentBlocksFromFileByChildStr(CStr FileString, long FileLen, HWND hWnd);
void RunExtProg(CStr ProgToRun);
CStr CorrectFileName(CStr FileName);
long JumpToAddr(FARPROC Address);
long JumpToAddrArg(FARPROC Address,long Arg);
int GetMDIWidth(void);
int GetMDIHeight(void);
void DisplayCursor(void);
void HideCursor(void);
extern "C" void CALLBACK UpCase(char *string);
extern "C" void CALLBACK LoCase(char *string);
extern "C" void CALLBACK FillMemDat(BYTE *Mem, int Length, int Data);
extern "C" int CALLBACK CountFileLines(char *string, int MaxSize);
extern "C" char * CALLBACK GetFileLineAddress(char *String, int MaxSize, int LineNumber);
extern "C" int CALLBACK GetFileLineOffset(char *String, int MaxSize, int LineNumber);
extern "C" void CALLBACK ReplaceTabs(char *String, int MaxSize);
void CALLBACK SpaceToNewLine(char *String, int MaxSize);
extern "C" int CALLBACK RemCRLF(char *String, int MaxSize);
void CALLBACK CopyMem2(char *Source,
                       char *Dest,
                       int Len,
                       int Src_Offset,
                       int Dest_Offset);
void CALLBACK CopyMem(char *Source,
                      char *Dest,
                      int Len,
                      int Dest_Offset,
                      int RepeatCount);
void CALLBACK SpaceToTab(char *String, int MaxSize);
void CALLBACK ToggleCase(char *String, int Len);
void TabToSpace(char *String, int MaxSize);
int CALLBACK AsciiToDw(char *String);
HWND CALLBACK DisplayChm(char *FileName, char *KeyWord);
void CALLBACK QEPlug(QEPROC PlugAddr,
                     HINSTANCE hInstance,
                     HWND hWnd,
                     HWND hText,
                     HWND hToolBar,
                     HWND hStatusBar);
BYTE * CALLBACK MLoadFile(char *FileName, long *Bytes);

#endif
