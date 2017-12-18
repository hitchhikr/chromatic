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
// Menus.h: declaration of used variables / structures for Menus.cpp
// -----------------------------------------------------------------------

#ifndef _MENUS_H_
#define _MENUS_H_

// -----------------------------------------------------------------------
// Includes
#include "ConstMenus.h"
#include "MsgCodeMax.h"

// -----------------------------------------------------------------------
// Constants
#define MAXSUBMENUS 21 - 1

// -----------------------------------------------------------------------
// Variables
extern HMENU hMainMenu;
extern HMENU hFileMenu;

extern HMENU hFileMenuNew;
extern HMENU hFileMenuOpen;
extern HMENU hFileMenuInclude;
extern HMENU hFileMenuSave;
extern HMENU hFileMenuConsole;
extern HMENU hFileMenuRunProg;
extern HMENU hFileMenuFavorites;
extern HMENU hFileMenuFilters;
extern HMENU hFileMenuIncludeFilters;
extern HMENU hEditMenuCopy;
extern HMENU hEditMenuPaste;
extern HMENU hEditMenuDelete;
extern HMENU hEditMenuBookmark;
extern HMENU hEditMenuProc;
extern HMENU hEditMenuGoto;
extern HMENU hEditMenuBlock;
extern HMENU hMenuRecent;
extern HMENU hMenuRecentPrj;
extern HMENU hViewMenuDisplay;
extern HMENU SubMenusArray[MAXSUBMENUS + 1];

extern HMENU hEditMenu;
extern HMENU hViewMenu;
extern HMENU hProjectMenu;
extern HMENU hProjectAddToMenu;
extern HMENU hProjectAddResToMenu;
extern HMENU hWindowMenu;
extern HMENU hEditAddInsMenu;

extern long EntryInRecent;
extern char *MenuFileComments[MENU_FILE_LAST_ID + 1];
extern char *MenuEditComments[MENU_EDIT_LAST_ID + 1];
extern char *MenuViewComments[MENU_VIEW_LAST_ID + 1];
extern char *MenuProjectComments[MENU_PROJECT_LAST_ID + 1];
extern char *MenuAddInComments[MENU_ADDINS_MANAGER_ID + 1];
extern char *MenuWindowComments[MENU_WINDOW_LAST_ID + 1];
extern char *MenuCodeMaxComments[MENU_CONTEXT_LAST_ID + 1];

extern char *MenuFileToolTips[MENU_FILE_LAST_ID + 1];
extern char *MenuEditToolTips[MENU_EDIT_LAST_ID + 1];
extern char *MenuViewToolTips[MENU_VIEW_LAST_ID + 1];
extern char *MenuProjectToolTips[MENU_PROJECT_LAST_ID + 1];
extern char *MenuWindowToolTips[MENU_WINDOW_LAST_ID + 1];

extern CList <int> MenuCommandsID;
extern CList <int> MenuRealCommandsID;
extern CStr MainMenusRetVal;
extern CM_HOTKEY CHotKey;

// -----------------------------------------------------------------------
// Functions declarations
void CreateMenuBar(HWND hParent);
void CreateFileMenu(HMENU hParent, CStr MenuName, long BaseID);
void RefreshNewMenu(void);
void CreateNewMenu(long BaseID);
void CreateEditMenu(HMENU hParent, CStr MenuName, long BaseID);
void CreateViewMenu(HMENU hParent, CStr MenuName, long BaseID);
void CreateProjectMenu(HMENU hParent, CStr MenuName, long BaseID);
void CreateAddInsMenu(HMENU hParent, CStr MenuName, long BaseID);
void CreateWindowMenu(HMENU hParent, CStr MenuName, long BaseID);
void SetRecentFiles(HMENU hParent);
void SetRecentProjects(HMENU hParent);
void AddRecentFile(CStr FName, long AddFile, long ForceRemove);
void AddRecentPrj(CStr FName, long AddFile, long ForceRemove);
void SetMenus(void);
void CreateMenuEntry(CStr MName);
void ClearMenus(void);
void InitMenuToolTips(void);
void InitMenuComments(void);
void RegCodeMaxLocalCommands(void);
void RegCMCom(CStr CmdName, CStr CmdComment, long CmdNumber, long CmdModif, long CmdKey);
long GetPlugInType(CStr PlugInName);
void AddInWinRecent(CStr FName);
void CreateRunProgMenu(void);
void AddRecentRunProg(CStr FName, long AddFile, long ForceRemove);
void CreateFavoritesMenu(void);
void CreateFiltersMenu(HMENU Dest_Menu, int Index, int Lab_Index, CStr Label, int Include_Menu);

#endif
