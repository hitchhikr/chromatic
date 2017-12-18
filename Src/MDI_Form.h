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
// MDI_Form.h: declaration of used variables / structures for MDI_ßForm.cpp
// -----------------------------------------------------------------------

#ifndef _MDI_FORM_H_
#define _MDI_FORM_H_

// -----------------------------------------------------------------------
// Variables
extern MDIDAT hMDIform;
extern CStr MDIRetVal;
extern long TimerHandle;
extern long FreezeTimer;
extern CStr EmptyFile;
extern MDICREATESTRUCT CurMDICreate;

// -----------------------------------------------------------------------
// Functions declarations
void CreateMDIForm(void);
void CALLBACK MDIInitProc(HWND hWnd);
LRESULT CALLBACK MDIProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ResizeMDIform(void);
void WriteComment(CStr Comment);
void CloseAllChilds(HWND hWnd);
int CALLBACK EnumCloseChildProc(HWND hWnd, long lParam);
void ChFontAllChilds(HWND hWnd);
int CALLBACK EnumChFontChildProc(HWND hWnd, long lParam);
void SearchOpenedFile(HWND hWnd, CStr FileName);
int CALLBACK EnumSearchFileProc(HWND hWnd, long lParam);
void SaveAllChilds(HWND hWnd);
int CALLBACK EnumSaveChildProc(HWND hWnd, long lParam);
HWND OpenFileNorm(CStr FileName, long ReadO, long CheckOp, long ForceCreate, long NoAddRecent);
void StoreLanguageToOpen(CStr LanguageName);
HWND CreateNewFile(CStr FileN);
//void OpenFileAsDB(HWND hWnd, CStr FileName);
void OpenFileAsRC(HWND hWnd, CStr FileLabel, CStr FileName, long ResIndex);
void OpenWorkSpc(CStr FileName);
void SaveAllChildsInWorkSpace(HWND hWnd);
int CALLBACK EnumSaveAllChildsInWorkSpace(HWND hWnd, long lParam);
CStr SaveIt(HWND hWnd);
CStr SaveItAs(HWND hWnd, long RefreshTree, CStr TreeKey, CStr OldTreeFile);
LRESULT CALLBACK ClientProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FocusOnActiveChild(void);
void SearchModified(HWND hWnd);
int CALLBACK EnumSearchDateModified(HWND hWnd, long lParam);
long CheckLastModified(HWND hWnd, int CheckProject);
int CALLBACK EnumSearchModified(HWND hWnd, long lParam);
long RefreshTimer(HWND hWnd);
long CreateTimer(HWND hWnd);
void CALLBACK TimerNotify(HWND hWnd, UINT uMsg, UINT_PTR idEvent, unsigned long dwTime);
void CheckAutoSave(HWND hWnd);
LRESULT CALLBACK StatusHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void RestoreMDI(HWND hWnd);

#endif
