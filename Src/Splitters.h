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
// Splitters.h: declaration of used variables / structures for Splitters.cpp
// -----------------------------------------------------------------------

#ifndef _SPLITTERS_H_
#define _SPLITTERS_H_

// -----------------------------------------------------------------------
// Variables
extern HWND hProjectContainer;
extern HWND hWindowsContainer;
extern HWND hOutputContainer;
extern HWND hProjectToolbar;
extern HWND hWindowsToolbar;
extern HWND hStatusToolBar;
extern HWND hProjectPager;
extern HWND hWindowsPager;
extern HWND hStatusPager;
extern HWND hTreeView;
extern HWND hWindowsListView;
extern SPLITTERDAT MyVSplitterDAT;
extern SPLITTERDAT MyHSplitterDAT;
extern HWND hOutputSysTab;
extern HWND hStatusBox;
extern HWND hDebugBox;
extern CStr DOSReportStr;
extern long LastSelectTreePop;
extern long OldTreeViewToolTips;
extern NM_TREEVIEWEXPAND DragTreeView;
extern long TreeDragging;
extern HIMAGELIST hTreeImageList;
extern HTREEITEM hDragParent;
extern HTREEITEM CurrentTreeItemDrag;
extern long FirstDropped;
extern long LastDropped;
extern HTREEITEM DragNewhItem;
extern long DraggingCursor;
extern TV_HITTESTINFO TreeViewhit;
extern TV_HITTESTINFO TreeViewhitNew;
extern long TVLeftCoord;
extern long TVTopCoord;
extern long InContextMenu;
extern long PreviouslySelectedWindow;

// -----------------------------------------------------------------------
// Treeview popup menu constants
#define MENU_POP_BASEID 4000

#define MENU_POP_NONE 0
#define MENU_POP_PROJECT 1
#define MENU_POP_NEWINCLUDE 2
#define MENU_POP_NEWLIBRARY 3
#define MENU_POP_NEWMODULE 4
#define MENU_POP_NEWOBJECT 5
#define MENU_POP_NEWRESOURCE 6
#define MENU_POP_NEWDEF 7
#define MENU_POP_NEWTEXT 8
#define MENU_POP_INCLUDEDETAILS 9
#define MENU_POP_LIBRARYDETAILS 10
#define MENU_POP_MODULEDETAILS 11
#define MENU_POP_OBJECTDETAILS 12
#define MENU_POP_RESOURCEDETAILS 13
#define MENU_POP_DEFDETAILS 14
#define MENU_POP_TEXTDETAILS 15

#define MENU_POP_NEWICON 16
#define MENU_POP_NEWCURSOR 17
#define MENU_POP_NEWBITMAP 18
#define MENU_POP_NEWSTRINGS 19
#define MENU_POP_NEWACCELERATORS 20
#define MENU_POP_NEWMENUS 21
#define MENU_POP_NEWDIALOG 22
#define MENU_POP_NEWRAWDATA 23

#define MENU_POP_ICONDETAILS 24
#define MENU_POP_CURSORDETAILS 25
#define MENU_POP_BITMAPDETAILS 26
#define MENU_POP_STRINGDETAILS 27
#define MENU_POP_ACCELERATORDETAILS 28
#define MENU_POP_MENUDETAILS 29
#define MENU_POP_DIALOGDETAILS 30
#define MENU_POP_RAWDATADETAILS 31

// -----------------------------------------------------------------------
// Outbox contextual menu
#define MENU_OUTBOX_SAVE_ID 1
#define MENU_OUTBOX_NEW_ID 2
#define MENU_OUTBOX_EMPTY_ID 3
#define MENU_OUTBOX_GOTO_ID 4

#define MENU_DEBUGBOX_SAVE_ID 1
#define MENU_DEBUGBOX_NEW_ID 2
#define MENU_DEBUGBOX_EMPTY_ID 3
#define MENU_DEBUGBOX_GOTO_ID 4

#define MENU_WINDOWSBOX_CLOSE_ID 1
#define MENU_WINDOWSBOX_SAVE_ID 2
#define MENU_WINDOWSBOX_SAVEAS_ID 3
#define MENU_WINDOWSBOX_TOGGLE_ID 4

// -----------------------------------------------------------------------
// Functions declarations
void CreateDockingProject(HWND hParent);
void CreateDockingWindows(HWND hParent);
void CreateDockingOutput(HWND hParent);
void SetProjectDockState(long bShow);
void SetWindowsDockState(long bShow);
void SetOutputDockState(long bShow);
void SetProjectDockMenuState(long bShow);
void SetWindowsDockMenuState(long bShow);
void SetOutputDockMenuState(long bShow);
void SaveProjectDockState(int Visible);
void SaveWindowsDockState(int Visible);
void SaveOutputDockState(int Visible);
void WriteToStatus(CStr Txt);
void WriteToDebug(CStr Txt);
void ClearStatus(void);
void ClearDebug(void);
void SaveBuildReport(HWND hWnd);
void CreateBuildReport(HWND hWnd);
void SetTreeviewTT(void);
void SetTreeviewExpand(void);
long GetTreeviewExpand(void);
void JumpToErrorFromOutBox(void);
void ReadDockState(CStr DockName, int &State, RECT &Rect, int &Size, int DefaultState, int DefaultSize);
void SaveDockState(HWND DockhWnd, CStr DockName, int visible);
HWND GetPreviousChildInList(HWND hWnd);
HWND GetNextChildInList(HWND hWnd);

#endif
