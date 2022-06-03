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
// Toolbars.h: declaration of used variables / structures for Toolbars.cpp
// -----------------------------------------------------------------------

#ifndef _TOOLBARS_H_
#define _TOOLBARS_H_

// -----------------------------------------------------------------------
// Variables
extern HWND hMainToolbar;
extern HWND hBookMarkToolbar;
extern HWND hRebar;
extern HWND hMainPager;
extern HWND hBookMarksPager;
extern CStr ToolBarKey;
extern CStr ToolBarType;
extern CStr ToolBarShow;
extern CStr BtnIcon;
extern long BtnID;
extern CStr BtnMenu;
extern CStr BtnToolTip;
extern CStr BtnComment;
extern CStr BtnCommand;
extern long CurrenthToolbar;
extern CList <HWND> ToolBarArray;
extern CList <int> ToolBarMenus;
extern CList <char *> ToolBarToolTips;
extern CList <char *> ToolBarComments;
extern CList <int> ToolBarCommands;
extern CList <HWND> PagersHwnd;
extern CList <int> PagersSize;
extern CList <HWND> ToolbarsHwnd;
extern CList <int> ToolBarsShow;
extern CList <char *> ToolBarsName;
extern long MaxToolbarBtn;
extern SIZE TtSize;
extern long OldTBSize;

extern long ToolbarFileIcons[MENU_FILE_LAST_ID + 1];
extern long ToolbarEditIcons[MENU_EDIT_LAST_ID + 1];
extern long ToolbarViewIcons[MENU_VIEW_LAST_ID + 1];
extern long ToolbarProjectIcons[MENU_PROJECT_LAST_ID + 1];
extern long ToolbarWindowIcons[MENU_WINDOW_LAST_ID + 1];

// -----------------------------------------------------------------------
// Functions declarations
void InitReBarContent(HWND hParent);
void InitToolBarsIcons(void);
void RefreshToolbars(void);
LRESULT CALLBACK RebarHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

#endif
