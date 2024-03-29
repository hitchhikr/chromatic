// -----------------------------------------------------------------------
// Chromatic
// Integrated Development Environment
//
// Copyright (C) 2001-2022 Franck Charlet.
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
// Dlg_Strings.h: declaration of used variables / structures for Dlg_Strings.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_STRINGS_H_
#define _DLG_STRINGS_H_

// -----------------------------------------------------------------------
// Variables
extern HWND FRMStringshwnd;
extern HWND hFRMStringsOk;
extern HWND hFRMStringsCancel;
extern CStr StringsTitle;
extern HWND FRMStringsListview;
extern long StringBase;
extern long EditStrings;
extern long LVStringsInEdit;
extern HANDLE StringsFileHandle;
extern CStr StringsFileToEdit;
extern CStr StringBaseToEdit;

// -----------------------------------------------------------------------
// Functions declarations
void CreateStrings(void);
void EditStringsTable(CStr StringEditBase, CStr StringsFile, long Base);
int CALLBACK FRMStringsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreateNewStringsFile(void);
void SaveStringsFile(void);
LRESULT CALLBACK LVStringsHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
