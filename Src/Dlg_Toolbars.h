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
// Dlg_Toolbars.h: declaration of used variables / structures for Dlg_Toolbars.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_TOOLBARS_H_
#define _DLG_TOOLBARS_H_

// -----------------------------------------------------------------------
// Variables
extern HWND FRMToolbarshwnd;
extern HWND FRMToolbarsSysTab;
extern HWND FRMToolbarsCmdClose;
extern HWND FRMToolbarsListview;
extern HWND FRMToolbarsCmdNew;
extern HWND FRMToolbarsCmdRename;
extern HWND FRMToolbarsCmdDelete;
extern HWND FRMToolbarsCombo;
extern HWND FRMToolbarsCheckB;
extern HWND FRMToolbarsLabel1;
extern HWND FRMToolbarsListDrop;
extern HWND FRMToolbarsListDrag;
extern long LVDrag1;
extern long LVDrag2;
extern POINT LVDragPoint;
extern HICON DragIcon;
extern CStr DragText;
extern long DragIndex;
extern CList <int> TBArray;
extern CList <int> TBArrayBreak;
extern long BegDrag;
extern long NbrTmp;
extern CList <int> TBIconsArray;
extern CList <char *> TBToolsArray;
extern CList <char *> TBBtnCommentArray;
extern long OldComboIndex;
extern CStr OldComboItem;
extern CStr OldToolbarName;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK FRMToolbarsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillToolbarsNames(void);
void FillToolbarsHeader(void);
void AddToolbarEntry(CStr ToolBarName);
void RenameToolbarEntry(CStr OldToolbarName, CStr ToolBarName);
void ChangeToolbarBreak(CStr ToolBarName, long TBBreak);
void ChangeToolbarVisible(CStr ToolBarName, long TBVisible);
void RemoveToolbarEntry(CStr ToolBarName);
void FRMToolbarsFillCombo(void);
void FillToolbarDatas(void);
void FillToolbarCommands(void);

#endif
