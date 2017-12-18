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
// Dlg_Accelerators.h: declaration of used variables / structures for Dlg_Accelerators.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_ACCELERATORS_H_
#define _DLG_ACCELERATORS_H_

// -----------------------------------------------------------------------
// Constants
#define ACCELCREATE 1
#define ACCELREPLACE 2
#define ACCELDELETE 3

// -----------------------------------------------------------------------
// Variables
extern HWND FRMAcceleratorshwnd;
extern long EditAccelerators;
extern long AccelBase;
extern CStr AcceleratorsTitle;
extern CStr AcceleratorsFileToEdit;
extern CStr AcceleratorsBaseToEdit;
extern HWND FRMAcceleratorsListview;
extern long LVAcceleratorsInEdit;
extern HWND FRMAcceleratorsOk;
extern HWND FRMAcceleratorsCancel;
extern HWND FRMAcceleratorsToolbar;
extern HWND FRMAcceleratorsFrame;
extern HWND FRMAcceleratorsChkCtrl;
extern HWND FRMAcceleratorsChkAlt;
extern HWND FRMAcceleratorsChkShift;
extern HWND FRMAcceleratorsCombo;
extern HWND FRMAcceleratorsGetKey;

// -----------------------------------------------------------------------
// Functions declarations
void CreateAccelerators(void);
void EditAcceleratorsTable(CStr AcceleratorsEditBase, CStr AcceleratorsFile);
int CALLBACK FRMAcceleratorsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK LVAcceleratorsHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long CheckAcceleratorValidity(CStr AccelToCheck);
long CheckAcceCList(void);
void CreateNewAcceleratorsFile(void);
void SaveAcceleratorsFile(void);

#endif
