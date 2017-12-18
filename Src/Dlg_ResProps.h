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
// Dlg_ResProps.h: declaration of used variables / structures for Dlg_ResProps.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_RESPROPS_H_
#define _DLG_RESPROPS_H_

// -----------------------------------------------------------------------
// Constants
#define RESPROP_ICON 0
#define RESPROP_CURSOR 1
#define RESPROP_BITMAP 2
#define RESPROP_STRINGS 3
#define RESPROP_ACCELERATORS 4
#define RESPROP_MENUS 5
#define RESPROP_RAWDATA 6

#define RESPROPDAT_LOAD 1
#define RESPROPDAT_MOVE 2
#define RESPROPDAT_DISCARD 4
#define RESPROPDAT_PURE 8

// -----------------------------------------------------------------------
// Variables
extern HWND FRMResPropshWnd;
extern long ResIndex;
extern long ResType;
extern HWND FRMResPropsOk;
extern HWND FRMResPropsCancel;
extern HWND FRMResPropsMem;
extern HWND FRMResPropsLoad;
extern HWND FRMResPropsMoveable;
extern HWND FRMResPropsDiscardable;
extern HWND FRMResPropsPure;
extern HWND FRMResPropsLanguage;
extern HWND FRMResPropsName;
extern CStr ResName;
extern HTREEITEM ResTree;

// -----------------------------------------------------------------------
// Functions declarations
void DisplayResProp(long Idx, long ResTp, CStr ResN, HTREEITEM hResTree);
int CALLBACK FRMResProps(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
