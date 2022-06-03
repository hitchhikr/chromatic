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
// Dlg_Menus.h: declaration of used variables / structures for Dlg_Menus.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_MENUS_H_
#define _DLG_MENUS_H_

// -----------------------------------------------------------------------
// Constants
#define MENUSADDBAR 1
#define MENUSADDPOPUP 2
#define MENUSADDITEM 3
#define MENUSADDSEPARATOR 4
#define MENUSDELETE 5
#define MENUSUP 6
#define MENUSDOWN 7
#define MENUSVIEW 8

#define MENUTYPE_BAR 0
#define MENUTYPE_POPUP 1
#define MENUTYPE_ITEM 2
#define MENUTYPE_SEPS 3

#define MENUPROP_CHECKED 1
#define MENUPROP_GRAYED 2
#define MENUPROP_RADIO 4
#define MENUPROP_BOLD 8
#define MENUPROP_PICMASK 0xFF00
#define MENUPROP_BREAKMASK 0xFF0000

#define MENUPROP_BARBREAK 1
#define MENUPROP_BREAK 2
#define MENUPROP_RIGHTJUSTIFY 3

#define MENUPROP_PIC_CALLBACK 1
#define MENUPROP_PIC_MBARCLOSE 2
#define MENUPROP_PIC_MBARCLOSE_D 3
#define MENUPROP_PIC_MBARMINIMIZE 4
#define MENUPROP_PIC_MBARMINIMIZE_D 5
#define MENUPROP_PIC_MBARRESTORE 6
#define MENUPROP_PIC_POPUPCLOSE 7
#define MENUPROP_PIC_POPUPMAXIMIZE 8
#define MENUPROP_PIC_POPUPMINIMIZE 9
#define MENUPROP_PIC_POPUPRESTORE 10
#define MENUPROP_PIC_SYSTEM 11

// -----------------------------------------------------------------------
// Variables
extern HWND FRMMenushwnd;
extern CStr MenusTitle;
extern HWND FRMenusOk;
extern HWND FRMMenusCancel;
extern HWND FRMMenusToolbar;
extern HWND FRMMenusTreeView;
extern HWND FRMMenusFrame;
extern HWND FRMMenusCaption;
extern HWND FRMMenusCommand;
extern HWND FRMMenusChkChecked;
extern HWND FRMMenusChkGrayed;
extern HWND FRMMenusChkRadio;
extern HWND FRMMenusCbBreak;
extern HWND FRMMenusChkBold;

extern long MenusBase;
extern long EditMenus;
extern CStr MenusFileToEdit;
extern CStr MenusBaseToEdit;

extern CList <HTREEITEM> MenusArray;
extern CList <int> MenusTypeArray;
extern CList <int> MenusCmdArray;
extern CList <int> MenusPropsArray;

extern long InCloseProcess;

// -----------------------------------------------------------------------
// Functions declarations
void CreateMenus(void);
void EditMenusTable(CStr MenusEditBase, CStr MenusFile);
int CALLBACK FRMMenusProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK MenuTreeViewHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long GetEntryIndex(HTREEITEM hwnd);
LRESULT CALLBACK MenusFrameHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void PutMenuProps(long PropIndex);
long GetCurrentPropIndex(void);
void CreateNewMenusFile(void);
long SaveMenusFile(CStr MenusFileToSave);
void FillTReeViewMenus(void);
void RemoveMenuEntry(HTREEITEM hItem);
void MoveMenuEntryUp(HTREEITEM hItem);
void MoveMenuEntryDown(HTREEITEM hItem);
void MoveEntryChilds(HTREEITEM hTVChild, HTREEITEM MainParent);
void TestMenus(void);
int CALLBACK FRMTestMenusProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ConstructTestMenus(HWND hwnd);
CStr NormalizeMenuString(CStr StrToCalc);

#endif
