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
// Dlg_Args.h: declaration of used variables / structures for Dlg_Args.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_ARGS_H_
#define _DLG_ARGS_H_

// -----------------------------------------------------------------------
// Constants
#define ARGSCLEAR 1

// -----------------------------------------------------------------------
// Variables
extern HWND hFRMArgsLabel;
extern HWND hFRMArgsOk;
extern HWND hFRMArgsCancel;
extern HWND hFRMArgsCombo;
extern HWND FRMArgshWnd;
extern HWND FRMArgshTB;
extern CStr FRMArgsRetVal;
extern long FRMArgsEntered;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK FRMArgsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void RefreshIniFileComboList(HWND hCombo, CStr EntryToAdd, CStr IniKey, CStr IniFile);

#endif
