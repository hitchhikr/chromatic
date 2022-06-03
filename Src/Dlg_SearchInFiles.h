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
// Dlg_SearchInFiles.h: declaration of used variables / structures for Dlg_SearchInFiles.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_SEARCHINFILES_H_
#define _DLG_SEARCHINFILES_H_

// -----------------------------------------------------------------------
// Constants
#define FRMFileSearchBUTTONEXT 1
#define FRMFileSearchBUTTONOPEN 2
#define FRMFileSearchBUTTONCLEARKEY 3
#define FRMFileSearchBUTTONCLEAREXT 4
#define FRMFileSearchBUTTONCLEAROPEN 5

// -----------------------------------------------------------------------
// Variables
extern HWND FRMFileSearchhWnd;
extern HWND FRMFileSearchhFrame;
extern HWND FRMFileSearchComboKeyWord;
extern HWND FRMFileSearchComboExt;
extern HWND FRMFileSearchComboDir;
extern HWND FRMFileSearchTBSel;
extern HWND FRMFileSearchChkVariables;
extern HWND FRMFileSearchChkCaseSens;
extern HWND FRMFileSearchChkIncludes;
extern HWND FRMFileSearchChkSubDirs;
extern HWND FRMFileSearchChkRaw;
extern HWND FRMFileSearchOk;
extern HWND FRMFileSearchCancel;
extern HWND FRMFileSearchFileLabel;
extern long SearchRawResult;
extern CStr FRMFileSearchSearchDir;
extern CList <char *> Directories;
extern CStr FRMFileSearchCommChar;
extern CStr FRMFileSearchSearchWord;
extern long FRMFileSearchInCalc;
extern long FRMFileSearchStopCalc;
extern WIN32_FIND_DATA FindIncludeDat;

extern long FRMFileSearchChkVariablesValue;
extern long FRMFileSearchChkCaseSensValue;
extern long FRMFileSearchChkIncludesValue;
extern long FRMFileSearchChkSubDirsValue;
extern long FRMFileSearchChkRawValue;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK FRMFileSearchProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CheckSubDirs(CStr DirToCheck);
void RunfileSearch(HWND hWnd);
void SelectDirToSearch(void);

#endif
