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
// Menus.cpp: Menus related functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Menus.h"
#include "AddIns.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "MDI_Childs.h"
#include "MDI_Form.h"

// -----------------------------------------------------------------------
// Variables
HMENU hMainMenu;
HMENU hFileMenu;

HMENU hFileMenuNew;
HMENU hFileMenuOpen;
HMENU hFileMenuInclude;
HMENU hFileMenuSave;
HMENU hFileMenuConsole;
HMENU hFileMenuRunProg;
HMENU hFileMenuFavorites;
HMENU hFileMenuFilters;
HMENU hFileMenuIncludeFilters;
HMENU hEditMenuCopy;
HMENU hEditMenuPaste;
HMENU hEditMenuDelete;
HMENU hEditMenuBookmark;
HMENU hEditMenuBreakpoint;
HMENU hEditMenuProc;
HMENU hEditMenuGoto;
HMENU hEditMenuBlock;
HMENU hMenuRecent = 0;
HMENU hMenuRecentPrj = 0;
HMENU hViewMenuDisplay;
HMENU SubMenusArray[MAXSUBMENUS + 1];

HMENU hEditMenu;
HMENU hViewMenu;
HMENU hProjectMenu;
HMENU hProjectAddToMenu;
HMENU hProjectAddResToMenu;
HMENU hWindowMenu;
HMENU hEditAddInsMenu;

long EntryInRecent;
char *MenuFileComments[MENU_FILE_LAST_ID + 1];
char *MenuEditComments[MENU_EDIT_LAST_ID + 1];
char *MenuViewComments[MENU_VIEW_LAST_ID + 1];
char *MenuProjectComments[MENU_PROJECT_LAST_ID + 1];
char *MenuAddInComments[MENU_ADDINS_MANAGER_ID + 1];
char *MenuWindowComments[MENU_WINDOW_LAST_ID + 1];
char *MenuCodeMaxComments[MENU_CONTEXT_LAST_ID + 1];

char *MenuFileToolTips[MENU_FILE_LAST_ID + 1];
char *MenuEditToolTips[MENU_EDIT_LAST_ID + 1];
char *MenuViewToolTips[MENU_VIEW_LAST_ID + 1];
char *MenuProjectToolTips[MENU_PROJECT_LAST_ID + 1];
char *MenuWindowToolTips[MENU_WINDOW_LAST_ID + 1];

CList <int> MenuCommandsID;
CList <int> MenuRealCommandsID;
CStr MainMenusRetVal;
CM_HOTKEY CHotKey;

// -----------------------------------------------------------------------
// Create the main menu bar
void CreateMenuBar(HWND hParent)
{
    hMainMenu = CreateMenu();
    SetMenu(hParent, hMainMenu);
    CreateFileMenu(hMainMenu, "&File", MENU_FILE_IDBASE);
    CreateEditMenu(hMainMenu, "&Edit", MENU_EDIT_IDBASE);
    CreateViewMenu(hMainMenu, "&View", MENU_VIEW_IDBASE);
    CreateProjectMenu(hMainMenu, "&Project", MENU_PROJECT_IDBASE);
    CreateAddInsMenu(hMainMenu, "&AddIns", MENU_ADDINS_IDBASE);
    CreateWindowMenu(hMainMenu, "&Window", MENU_WINDOW_IDBASE);
    SetRecentFiles(hFileMenu);
    SetRecentProjects(hFileMenu);
    SetMenus();
    SubMenusArray[0] = hFileMenuOpen;
    SubMenusArray[1] = hFileMenuSave;
    SubMenusArray[2] = hEditMenuCopy;
    SubMenusArray[3] = hEditMenuDelete;
    SubMenusArray[4] = hFileMenuConsole;
    SubMenusArray[5] = hEditMenuBookmark;
    SubMenusArray[6] = hEditMenuProc;
    SubMenusArray[7] = hEditMenuGoto;
    SubMenusArray[8] = hEditMenuBlock;
    SubMenusArray[9] = hMenuRecent;
    SubMenusArray[10] = hMenuRecentPrj;
    SubMenusArray[11] = hProjectAddToMenu;
    SubMenusArray[12] = hFileMenuInclude;
    SubMenusArray[13] = hFileMenuNew;
    SubMenusArray[14] = hEditMenuPaste;
    SubMenusArray[15] = hFileMenuRunProg;
    SubMenusArray[16] = hFileMenuFavorites;
    SubMenusArray[17] = hFileMenuFilters;
    SubMenusArray[18] = hFileMenuIncludeFilters;
    SubMenusArray[19] = hEditMenuBreakpoint;
    SubMenusArray[20] = hViewMenuDisplay;
}

// -----------------------------------------------------------------------
// Create the "File" menu
void CreateFileMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hFileMenu = CreatePopupMenu();
    CreateNewMenu(BaseID);
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuNew, "New");
    MenuAddString(hFileMenu, "Clear", BaseID + MENU_FILE_CLEAR_ID, TRUE);
    MenuAddString(hFileMenu, "Duplicate file", BaseID + MENU_FILE_DUPLICATEFILE_ID, TRUE);
    MenuAddString(hFileMenu, "Reload file\tCtrl+Shift+N", BaseID + MENU_FILE_RELOADFILE_ID, TRUE);
    MenuAddSeparator(hFileMenu);

    // ------
    hFileMenuOpen = CreatePopupMenu();
    MenuAddString(hFileMenuOpen, "Source file(s)...\tCtrl+O", BaseID + MENU_FILE_OPENFILE_ID, TRUE);
    MenuSetDefaultItem(hFileMenuOpen, 0);
    hFileMenuFilters = CreatePopupMenu();
    CreateFiltersMenu(hFileMenuFilters, 17, BaseID + MENU_FILE_FILTERS_ID, "Laboratory...\tCtrl+Shift+O", 0);
    AppendMenu(hFileMenuOpen, MF_POPUP, (UINT) hFileMenuFilters, "Filters");
    MenuAddString(hFileMenuOpen, "Workspace...", BaseID + MENU_FILE_OPENWORKSPACE_ID, TRUE);
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuOpen, "Open");
    
    // ------
    hFileMenuInclude = CreatePopupMenu();
    MenuAddString(hFileMenuInclude, "Source file(s)...\tCtrl+I", BaseID + MENU_FILE_INCLUDEFILE_ID, TRUE);
    MenuSetDefaultItem(hFileMenuInclude, 0);
    hFileMenuIncludeFilters = CreatePopupMenu();
    CreateFiltersMenu(hFileMenuIncludeFilters, 18, BaseID + MENU_FILE_INCLUDEFILTERS_ID, "Laboratory...\tCtrl+Shift+I", 1);
    AppendMenu(hFileMenuInclude, MF_POPUP, (UINT) hFileMenuIncludeFilters, "Filters");
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuInclude, "Include");
    // ------

    MenuAddSeparator(hFileMenu);
    MenuAddString(hFileMenu, "FTP manager...", BaseID + MENU_FILE_FTPMANAGER_ID, TRUE);
    MenuAddSeparator(hFileMenu);
    MenuAddString(hFileMenu, "Close\tCtrl+F4", BaseID + MENU_FILE_CLOSE_ID, TRUE);
    MenuAddString(hFileMenu, "Close all", BaseID + MENU_FILE_CLOSEALL_ID, TRUE);
    MenuAddSeparator(hFileMenu);
    hFileMenuSave = CreatePopupMenu();
    MenuAddString(hFileMenuSave, "Current file\tCtrl+S", BaseID + MENU_FILE_SAVE_ID, TRUE);
    MenuSetDefaultItem(hFileMenuSave, 0);
    MenuAddString(hFileMenuSave, "To new file...\tAlt+Shift+S", BaseID + MENU_FILE_SAVEAS_ID, TRUE);
    MenuAddString(hFileMenuSave, "All opened", BaseID + MENU_FILE_SAVEALL_ID, TRUE);
    MenuAddString(hFileMenuSave, "To workspace...", BaseID + MENU_FILE_SAVEWORKSPACE_ID, TRUE);
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuSave, "Save");
    MenuAddString(hFileMenu, "Register as code snippet...\tCtrl+H", BaseID + MENU_FILE_REGISTERASSNIPPET_ID, TRUE);
    MenuAddString(hFileMenu, "Register as source template...\tCtrl+Shift+H", BaseID + MENU_FILE_REGISTERASTEMPLATE_ID, TRUE);
    MenuAddSeparator(hFileMenu);
    MenuAddString(hFileMenu, "Print file...\tCtrl+P", BaseID + MENU_FILE_PRINTFILE_ID, TRUE);
    MenuAddSeparator(hFileMenu);
    hFileMenuConsole = CreatePopupMenu();
    MenuAddString(hFileMenuConsole, "Run\tF5", BaseID + MENU_FILE_CONSOLE_RUN_ID, TRUE);
    MenuSetDefaultItem(hFileMenuConsole, 0);
    MenuAddString(hFileMenuConsole, "Change...", BaseID + MENU_FILE_CONSOLE_CHANGE_ID, TRUE);
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuConsole, "Console prompt");
    hFileMenuRunProg = CreatePopupMenu();
    CreateRunProgMenu();
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuRunProg, "External programs");
    MenuAddSeparator(hFileMenu);
    hFileMenuFavorites = CreatePopupMenu();
    CreateFavoritesMenu();
    AppendMenu(hFileMenu, MF_POPUP, (UINT) hFileMenuFavorites, "Favorite files");
    MenuAddSeparator(hFileMenu);
    MenuAddString(hFileMenu, "Set working directory...\tF12", BaseID + MENU_FILE_SETWORKINGDIRECTORY_ID, TRUE);
    MenuAddSeparator(hFileMenu);
    MenuAddString(hFileMenu, "Exit\tCtrl+Q", BaseID + MENU_FILE_EXIT_ID, TRUE);
    AppendMenu(hParent, MF_POPUP, (UINT) hFileMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Refresh "New" menu
void RefreshNewMenu(void)
{
    DeleteMenu(hFileMenu, 0, MF_BYPOSITION);
    DestroyMenu(hFileMenuNew);
    CreateNewMenu(MENU_FILE_IDBASE);
    InsertMenu(hFileMenu, 0, MF_POPUP + MF_BYPOSITION, (UINT) hFileMenuNew, "New");
}

// -----------------------------------------------------------------------
// Create the "New" menu
void CreateNewMenu(long BaseID)
{
    hFileMenuNew = CreatePopupMenu();
    MenuAddString(hFileMenuNew, "Empty source\tCtrl+N", BaseID + MENU_FILE_NEW_ID, TRUE);
    MenuSetDefaultItem(hFileMenuNew, 0);
    CreateTemplateMenu(hFileMenuNew);
    SubMenusArray[13] = hFileMenuNew;
}

// -----------------------------------------------------------------------
// Create the "Edit" menu
void CreateEditMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hEditMenu = CreatePopupMenu();
    MenuAddString(hEditMenu, "Undo\tCtrl+Z", BaseID + MENU_EDIT_UNDO_ID, TRUE);
    MenuAddString(hEditMenu, "Redo\tCtrl+Y", BaseID + MENU_EDIT_REDO_ID, TRUE);
    MenuAddSeparator(hEditMenu);
    MenuAddString(hEditMenu, "Cut\tCtrl+X", BaseID + MENU_EDIT_CUT_ID, TRUE);
    hEditMenuCopy = CreatePopupMenu();
    MenuAddString(hEditMenuCopy, "Copy selection\tCtrl+C", BaseID + MENU_EDIT_COPY_ID, TRUE);
    MenuSetDefaultItem(hEditMenuCopy, 0);
    MenuAddString(hEditMenuCopy, "Copy file path/name", BaseID + MENU_EDIT_COPYFILEPATHNAME_ID, TRUE);
    MenuAddString(hEditMenuCopy, "Copy entire text", BaseID + MENU_EDIT_COPYENTIRETEXT_ID, TRUE);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuCopy, "Copy");
    
    hEditMenuPaste = CreatePopupMenu();
    MenuAddString(hEditMenuPaste, "To current window\tCtrl+V", BaseID + MENU_EDIT_PASTE_ID, TRUE);
    MenuSetDefaultItem(hEditMenuPaste, 0);
    MenuAddString(hEditMenuPaste, "To new window\tCtrl+Shift+V", BaseID + MENU_EDIT_PASTE_NEWWINDOW_ID, TRUE);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuPaste, "Paste");
    
    MenuAddSeparator(hEditMenu);
    hEditMenuDelete = CreatePopupMenu();
    MenuAddString(hEditMenuDelete, "Delete selection\tDel", BaseID + MENU_EDIT_DELETE_DELETESELECTION_ID, TRUE);
    MenuSetDefaultItem(hEditMenuDelete, 0);
    MenuAddString(hEditMenuDelete, "Delete line\tCtrl+E", BaseID + MENU_EDIT_DELETE_DELETELINE_ID, TRUE);
    MenuAddString(hEditMenuDelete, "Delete to start of line\tAlt+Bksp", BaseID + MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID, TRUE);
    MenuAddString(hEditMenuDelete, "Delete to end of line\tAlt+Del", BaseID + MENU_EDIT_DELETE_DELETETOENDOFLINE_ID, TRUE);
    MenuAddString(hEditMenuDelete, "Delete paragraph\tCtrl+Alt+K", BaseID + MENU_EDIT_DELETE_DELETEPARAGRAPH_ID, TRUE);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuDelete, "Delete");
    MenuAddSeparator(hEditMenu);
    MenuAddString(hEditMenu, "Select line", BaseID + MENU_EDIT_SELECTLINE_ID, TRUE);
    MenuAddString(hEditMenu, "Select all\tCtrl+A", BaseID + MENU_EDIT_SELECTALL_ID, TRUE);
    hEditMenuProc = CreatePopupMenu();
    MenuAddString(hEditMenuProc, "Select\tCtrl+Shift+A", BaseID + MENU_EDIT_SELECTPROCEDURE_ID, TRUE);
    MenuSetDefaultItem(hEditMenuProc, 0);
    MenuAddString(hEditMenuProc, "Select and collapse\tCtrl+Shift+Q", BaseID + MENU_EDIT_SELECTPROCANDCOLLAPSE_ID, TRUE);
    MenuAddString(hEditMenuProc, "Expand\tCtrl+Shift+W", BaseID + MENU_EDIT_UNCOLLAPSEPROCEDURE_ID, TRUE);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuProc, "Procedure");
    MenuAddSeparator(hEditMenu);
    MenuAddString(hEditMenu, "Find...\tCtrl+F", BaseID + MENU_EDIT_FIND_ID, TRUE);
    MenuAddString(hEditMenu, "Find fast\tCtrl+Shift+F3", BaseID + MENU_EDIT_FINDSELECTION_ID, TRUE);
    MenuAddString(hEditMenu, "Find next\tF3", BaseID + MENU_EDIT_FINDNEXT_ID, TRUE);
    MenuAddString(hEditMenu, "Find previous\tCtrl+F3", BaseID + MENU_EDIT_FINDPREVIOUS_ID, TRUE);
    MenuAddString(hEditMenu, "Replace...\tF4", BaseID + MENU_EDIT_REPLACE_ID, TRUE);
    MenuAddSeparator(hEditMenu);
    MenuAddString(hEditMenu, "Proc/Var/Const search...\tCtrl+T", BaseID + MENU_EDIT_PROCVARIABLESSEARCH_ID, TRUE);
    MenuAddString(hEditMenu, "Search in files...\tCtrl+B", BaseID + MENU_EDIT_SEARCHINFILES_ID, TRUE);
    MenuAddSeparator(hEditMenu);
    hEditMenuBookmark = CreatePopupMenu();
    MenuAddString(hEditMenuBookmark, "Toggle\tF2", BaseID + MENU_EDIT_BOOKMARKS_TOGGLE_ID, TRUE);
    MenuSetDefaultItem(hEditMenuBookmark, 0);
    MenuAddString(hEditMenuBookmark, "First\tCtrl+F7", BaseID + MENU_EDIT_BOOKMARKS_FIRST_ID, TRUE);
    MenuAddString(hEditMenuBookmark, "Previous\tShift+F2", BaseID + MENU_EDIT_BOOKMARKS_PREVIOUS_ID, TRUE);
    MenuAddString(hEditMenuBookmark, "Next\tCtrl+F2", BaseID + MENU_EDIT_BOOKMARKS_NEXT_ID, TRUE);
    MenuAddString(hEditMenuBookmark, "Last\tCtrl+Shift+F7", BaseID + MENU_EDIT_BOOKMARKS_LAST_ID, TRUE);
    MenuAddString(hEditMenuBookmark, "Clear all\tCtrl+Shift+F2", BaseID + MENU_EDIT_BOOKMARKS_CLEARALL_ID, TRUE);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuBookmark, "Bookmarks");

    hEditMenuBreakpoint = CreatePopupMenu();
    MenuAddString(hEditMenuBreakpoint, "Toggle\tF10", BaseID + MENU_EDIT_BREAKPOINTS_TOGGLE_ID, TRUE);
    MenuSetDefaultItem(hEditMenuBreakpoint, 0);
    MenuAddString(hEditMenuBreakpoint, "First\tCtrl+F11", BaseID + MENU_EDIT_BREAKPOINTS_FIRST_ID, TRUE);
    MenuAddString(hEditMenuBreakpoint, "Previous\tShift+F10", BaseID + MENU_EDIT_BREAKPOINTS_PREVIOUS_ID, TRUE);
    MenuAddString(hEditMenuBreakpoint, "Next\tCtrl+F10", BaseID + MENU_EDIT_BREAKPOINTS_NEXT_ID, TRUE);
    MenuAddString(hEditMenuBreakpoint, "Last\tCtrl+Shift+F11", BaseID + MENU_EDIT_BREAKPOINTS_LAST_ID, TRUE);
    MenuAddString(hEditMenuBreakpoint, "Clear all\tCtrl+Shift+F10", BaseID + MENU_EDIT_BREAKPOINTS_CLEARALL_ID, TRUE);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuBreakpoint, "Code breakpoints");

    MenuAddSeparator(hEditMenu);
    hEditMenuGoto = CreatePopupMenu();
    MenuAddString(hEditMenuGoto, "Top\tCtrl+Home", BaseID + MENU_EDIT_GOTO_TOP_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Bottom\tCtrl+End", BaseID + MENU_EDIT_GOTO_BOTTOM_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Line\tCtrl+G", BaseID + MENU_EDIT_GOTO_LINE_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Matching bracket\tCtrl+[", BaseID + MENU_EDIT_GOTO_MATCHINGBRACKET_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Previous word\tCtrl+Left", BaseID + MENU_EDIT_GOTO_PREVIOUSWORD_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Next word\tCtrl+Right", BaseID + MENU_EDIT_GOTO_NEXTWORD_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Previous procedure\tAlt+Up", BaseID + MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Next procedure\tAlt+Down", BaseID + MENU_EDIT_GOTO_NEXTPROCEDURE_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Previous paragraph\tCtrl+Alt+Left", BaseID + MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Next paragraph\tCtrl+Alt+Right", BaseID + MENU_EDIT_GOTO_NEXTPARAGRAPH_ID, TRUE);
    MenuAddSeparator(hEditMenuGoto);
    MenuAddString(hEditMenuGoto, "Variable declaration\tCtrl+Alt+S", BaseID + MENU_EDIT_GOTO_VARIABLEDECLARATION_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Variable next use", BaseID + MENU_EDIT_GOTO_VARIABLENEXTUSE_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Procedure declaration", BaseID + MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Procedure entry point\tCtrl+Alt+F", BaseID + MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID, TRUE);
    MenuAddString(hEditMenuGoto, "Procedure next use", BaseID + MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID, TRUE);
    MenuSetDefaultItem(hEditMenuGoto, 0);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuGoto, "Goto");
    hEditMenuBlock = CreatePopupMenu();
    MenuAddString(hEditMenuBlock, "Indent\tTab", BaseID + MENU_EDIT_BLOCK_INDENT_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Unindent\tShift+Tab", BaseID + MENU_EDIT_BLOCK_UNINDENT_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Upper case\tCtrl+U", BaseID + MENU_EDIT_BLOCK_UPPERCASE_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Lower case\tCtrl+L", BaseID + MENU_EDIT_BLOCK_LOWERCASE_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Toggle case\tCtrl+Shift+U", BaseID + MENU_EDIT_BLOCK_TOGGLECASE_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Comment\tCtrl+K", BaseID + MENU_EDIT_BLOCK_COMMENT_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Uncomment\tCtrl+M", BaseID + MENU_EDIT_BLOCK_UNCOMMENT_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Tabify\tCtrl+Shift+T", BaseID + MENU_EDIT_BLOCK_TABIFY_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Untabify\tCtrl+Alt+T", BaseID + MENU_EDIT_BLOCK_UNTABIFY_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Spaces to tabs\tCtrl+Alt+R", BaseID + MENU_EDIT_BLOCK_SPACESTOTABS_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Registers\tCtrl+R", BaseID + MENU_EDIT_BLOCK_REGISTERS_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Comment (multi lines)\tCtrl+Shift+K", BaseID + MENU_EDIT_BLOCK_BLKCOMMENT_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Custom comment...\tCtrl+Shift+F8", BaseID + MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Trim trailing spaces\tCtrl+Alt+G", BaseID + MENU_EDIT_BLOCK_TRIMSPACES_ID, TRUE);
    MenuAddSeparator(hEditMenuBlock);
    MenuAddString(hEditMenuBlock, "Convert to include...\tCtrl+Shift+L", BaseID + MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Save as...\tCtrl+W", BaseID + MENU_EDIT_BLOCK_SAVEAS_ID, TRUE);
    MenuAddString(hEditMenuBlock, "Print...\tCtrl+Shift+P", BaseID + MENU_EDIT_BLOCK_PRINT_ID, TRUE);
    MenuSetDefaultItem(hEditMenuBlock, 0);
    AppendMenu(hEditMenu, MF_POPUP, (UINT) hEditMenuBlock, "Block");
    AppendMenu(hParent, MF_POPUP, (UINT) hEditMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the "View" menu
void CreateViewMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hViewMenu = CreatePopupMenu();
    MenuAddString(hViewMenu, "Status bar", BaseID + MENU_VIEW_STATUSBAR_ID, TRUE);
    hViewMenuDisplay = CreatePopupMenu();
    MenuAddString(hViewMenuDisplay, "Project\tAlt+1", BaseID + MENU_VIEW_PROJECTDOCK_ID, TRUE);
    MenuAddString(hViewMenuDisplay, "Windows\tAlt+2", BaseID + MENU_VIEW_WINDOWSDOCK_ID, TRUE);
    MenuAddString(hViewMenuDisplay, "Output\tAlt+3", BaseID + MENU_VIEW_OUTPUTDOCK_ID, TRUE);
    MenuAddSeparator(hViewMenuDisplay);
    // Disabled by default at startup (debugger related)
    MenuAddString(hViewMenuDisplay, "Process datas\tAlt+4", BaseID + MENU_VIEW_DEBUGDOCK_ID, FALSE);
    MenuAddString(hViewMenuDisplay, "Registers\tAlt+5", BaseID + MENU_VIEW_REGISTERSDOCK_ID, FALSE);
    AppendMenu(hViewMenu, MF_POPUP, (UINT) hViewMenuDisplay, "Display");
    MenuAddString(hViewMenu, "Full screen\tAlt+Shift+Z", BaseID + MENU_VIEW_FULLSCREEN_ID, TRUE);
    MenuAddSeparator(hViewMenu);
    MenuAddString(hViewMenu, "Properties...\tF8", BaseID + MENU_VIEW_PROPERTIES_ID, TRUE);
    MenuAddString(hViewMenu, "User menus editor...\tF9", BaseID + MENU_VIEW_USERMENUSEDITOR_ID, TRUE);
    MenuAddString(hViewMenu, "Toolbars manager...\tShift+F9", BaseID + MENU_VIEW_USERTOOLBARSEDITOR_ID, TRUE);
    AppendMenu(hParent, MF_POPUP, (UINT) hViewMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the "Project" menu
void CreateProjectMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hProjectMenu = CreatePopupMenu();
    MenuAddString(hProjectMenu, "Create new project...", BaseID + MENU_PROJECT_CREATENEWPROJECT_ID, TRUE);
    MenuAddString(hProjectMenu, "Open project...\tCtrl+Alt+O", BaseID + MENU_PROJECT_OPENPROJECT_ID, TRUE);
    MenuAddSeparator(hProjectMenu);
    MenuAddString(hProjectMenu, "Save project\tCtrl+Shift+S", BaseID + MENU_PROJECT_SAVEPROJECT_ID, TRUE);
    MenuAddString(hProjectMenu, "Save project as...", BaseID + MENU_PROJECT_SAVEPROJECTAS_ID, TRUE);
    MenuAddSeparator(hProjectMenu);
    hProjectAddToMenu = CreatePopupMenu();
    MenuAddString(hProjectAddToMenu, "Include file(s)...", BaseID + MENU_PROJECT_ADDINCLUDE_ID, TRUE);
    MenuAddString(hProjectAddToMenu, "Library file(s)...", BaseID + MENU_PROJECT_ADDLIBRARY_ID, TRUE);
    MenuAddString(hProjectAddToMenu, "Module file(s)...", BaseID + MENU_PROJECT_ADDMODULE_ID, TRUE);
    MenuAddString(hProjectAddToMenu, "Object file(s)...", BaseID + MENU_PROJECT_ADDOBJECT_ID, TRUE);
    hProjectAddResToMenu = CreatePopupMenu();
    MenuAddString(hProjectAddResToMenu, "Rc file(s)...", BaseID + MENU_PROJECT_ADDRESOURCE_ID, TRUE);
    MenuAddSeparator(hProjectAddResToMenu);
    MenuAddString(hProjectAddResToMenu, "Icon file(s)...", BaseID + MENU_PROJECT_ADDICON_ID, TRUE);
    MenuAddString(hProjectAddResToMenu, "Cursor file(s)...", BaseID + MENU_PROJECT_ADDCURSOR_ID, TRUE);
    MenuAddString(hProjectAddResToMenu, "Bitmap file(s)...", BaseID + MENU_PROJECT_ADDBITMAP_ID, TRUE);
    MenuAddString(hProjectAddResToMenu, "Strings file(s)...", BaseID + MENU_PROJECT_ADDSTRINGS_ID, TRUE);
    MenuAddString(hProjectAddResToMenu, "Accelerators file(s)...", BaseID + MENU_PROJECT_ADDACCELERATORS_ID, TRUE);
    MenuAddString(hProjectAddResToMenu, "Menus file(s)...", BaseID + MENU_PROJECT_ADDMENUS_ID, TRUE);
    MenuAddString(hProjectAddResToMenu, "Dialog file(s)...", BaseID + MENU_PROJECT_ADDDIALOG_ID, TRUE);
    MenuAddString(hProjectAddResToMenu, "Raw datas file(s)...", BaseID + MENU_PROJECT_ADDRAWDATA_ID, TRUE);
    AppendMenu(hProjectAddToMenu, MF_POPUP, (UINT) hProjectAddResToMenu, "Resources");
    MenuAddString(hProjectAddToMenu, "Document file(s)...", BaseID + MENU_PROJECT_ADDTEXT_ID, TRUE);
    AppendMenu(hProjectMenu, MF_POPUP, (UINT) hProjectAddToMenu, "Add to project");
    MenuAddSeparator(hProjectMenu);
    MenuAddString(hProjectMenu, "Project properties...", BaseID + MENU_PROJECT_PROJECTPROPERTIES_ID, TRUE);
    MenuAddSeparator(hProjectMenu);
    MenuAddString(hProjectMenu, "Build project\tShift+F5", BaseID + MENU_PROJECT_ASSEMBLEPROJECT_ID, TRUE);
    MenuAddString(hProjectMenu, "Run project\tCtrl+F5", BaseID + MENU_PROJECT_RUNPROJECT_ID, TRUE);
    MenuAddString(hProjectMenu, "Debug project\tCtrl+Shift+F5", BaseID + MENU_PROJECT_DEBUGPROJECT_ID, TRUE);
    AppendMenu(hParent, MF_POPUP, (UINT) hProjectMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the "AddIns" menu
void CreateAddInsMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hEditAddInsMenu = CreatePopupMenu();
    MenuAddString(hEditAddInsMenu, "AddIns manager...", BaseID + MENU_ADDINS_MANAGER_ID, TRUE);
    AppendMenu(hParent, MF_POPUP, (UINT) hEditAddInsMenu, MenuName.Get_String());
    FillAddInsArrays();
    FillAddInsMenu();
}

// -----------------------------------------------------------------------
// Create the "Window" menu
void CreateWindowMenu(HMENU hParent, CStr MenuName, long BaseID)
{
    hWindowMenu = CreatePopupMenu();
    MenuAddString(hWindowMenu, "Context help\tF1", BaseID + MENU_WINDOW_CONTEXTHELP_ID, TRUE);
    MenuAddString(hWindowMenu, "MSDN/Platform SDK help\tCtrl+F1", BaseID + MENU_WINDOW_MSDNHELP_ID, TRUE);
    MenuAddString(hWindowMenu, "Windows DDK help\tCtrl+Shift+F1", BaseID + MENU_WINDOW_DDKHELP_ID, TRUE);
    MenuAddString(hWindowMenu, "DirectX SDK help\tAlt+Shift+F1", BaseID + MENU_WINDOW_DIRECTXSDKHELP_ID, TRUE);
    MenuAddSeparator(hWindowMenu);
    MenuAddString(hWindowMenu, "Tip of the day...", BaseID + MENU_WINDOW_TIPOFTHEDAY_ID, TRUE);
    MenuAddString(hWindowMenu, "About...", BaseID + MENU_WINDOW_ABOUT_ID, TRUE);
    MenuAddSeparator(hWindowMenu);
    MenuAddString(hWindowMenu, "Split", BaseID + MENU_WINDOW_SPLIT_ID, TRUE);
    MenuAddString(hWindowMenu, "Tile horizontally", BaseID + MENU_WINDOW_TILEHORIZONTALLY_ID, TRUE);
    MenuAddString(hWindowMenu, "Tile vertically", BaseID + MENU_WINDOW_TILEVERTICALLY_ID, TRUE);
    MenuAddString(hWindowMenu, "Cascade", BaseID + MENU_WINDOW_CASCADE_ID, TRUE);
    MenuAddString(hWindowMenu, "Arrange icons", BaseID + MENU_WINDOW_ARRANGEICONS_ID, TRUE);
    MenuAddSeparator(hWindowMenu);
    MenuAddString(hWindowMenu, "Next window\tCtrl+F6 / Ctrl+Tab", BaseID + MENU_WINDOW_NEXTWINDOW_ID, TRUE);
    MenuAddString(hWindowMenu, "Previous window\tCtrl+Shift+F6 / Ctrl+Shift+Tab", BaseID + MENU_WINDOW_PREVIOUSWINDOW_ID, TRUE);
    AppendMenu(hParent, MF_POPUP, (UINT) hWindowMenu, MenuName.Get_String());
}

// -----------------------------------------------------------------------
// Create the recent files menu
void SetRecentFiles(HMENU hParent)
{
    HMENU hMenu = 0;
    long NbrRc = 0;
    int i = 0;

    EntryInRecent = 0;
    Recents.Erase();
    // Read recent opened files
    hMenu = hParent;
    // Already loaded ?
    if(hMenuRecent != 0)
    {
        // Delete it
        DeleteMenu(hMenu, 27 - 6 + 2, MF_BYPOSITION);
    }
    NbrRc = 0;
    hMenuRecent = CreateMenu();
    for(i = 0; i <= (long) MaxRecent - 1; i++)
    {
        MainMenusRetVal = IniReadKey("Recents", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(MainMenusRetVal.Len() == 0) break;
        Recents.Add(MainMenusRetVal.Get_String());
        MenuAddString(hMenuRecent, MainMenusRetVal, MENU_RECENTFILES_IDBASE + i, TRUE);
        NbrRc++;
    }
    if(NbrRc != 0)
    {
        MenuAddSeparator(hMenuRecent);
        MenuAddString(hMenuRecent, "Remove obsoletes entries", MENU_RECENTFILESPURGE_IDBASE, TRUE);
        MenuAddString(hMenuRecent, "Clear all entries", MENU_RECENTFILESPURGEALL_IDBASE, TRUE);
        InsertMenu(hMenu, 27 - 6 + 2, MF_POPUP + MF_BYPOSITION, (UINT) hMenuRecent, "Recent files");
        EntryInRecent = 1;
    }
    else
    {
        DestroyMenu(hMenuRecent);
        hMenuRecent = 0;
        EntryInRecent = 0;
    }
    SubMenusArray[9] = hMenuRecent;
}

// -----------------------------------------------------------------------
// Create the recent projects menu
void SetRecentProjects(HMENU hParent)
{
    HMENU hMenu = 0;
    long NbrRc = 0;
    int i = 0;

    RecentsPrj.Erase();
    // Read recent opened projects
    hMenu = hParent;
    // No recent projects ?
    if(hMenuRecentPrj != 0)
    {
        if(EntryInRecent == 0) DeleteMenu(hMenu, 27 - 6 + 2, MF_BYPOSITION);
        else DeleteMenu(hMenu, 28 - 6 + 2, MF_BYPOSITION);
    }
    NbrRc = 0;
    hMenuRecentPrj = CreateMenu();
    for(i = 0; i <= (long) MaxRecentPrj - 1; i++)
    {
        MainMenusRetVal = IniReadKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(MainMenusRetVal.Len() == 0) break;
        RecentsPrj.Add(MainMenusRetVal.Get_String());
        MenuAddString(hMenuRecentPrj, MainMenusRetVal, MENU_RECENTPROJECTS_IDBASE + i, TRUE);
        NbrRc++;
    }
    if(NbrRc != 0)
    {
        MenuAddSeparator(hMenuRecentPrj);
        MenuAddString(hMenuRecentPrj, "Remove obsoletes entries", MENU_RECENTPROJECTSPURGE_IDBASE, TRUE);
        MenuAddString(hMenuRecentPrj, "Clear all entries", MENU_RECENTPROJECTSPURGEALL_IDBASE, TRUE);
        // Recent file list there ?
        if(EntryInRecent == 1) InsertMenu(hMenu, 28 - 6 + 2, MF_BYPOSITION + MF_POPUP, (UINT) hMenuRecentPrj, "Recent projects");
        else InsertMenu(hMenu, 27 - 6 + 2, MF_BYPOSITION + MF_POPUP, (UINT) hMenuRecentPrj, "Recent projects");
    }
    else
    {
        DestroyMenu(hMenuRecentPrj);
        hMenuRecentPrj = 0;
    }
    SubMenusArray[10] = hMenuRecentPrj;
}

// -----------------------------------------------------------------------
// Add an entry in recent files list
void AddRecentFile(CStr FName, long AddFile, long ForceRemove)
{
    int i = 0;
    int j = 0;
    long FRcnt = 0;
    CStr TmpR;
    CStr TmpFName;

    if(ForceRemove == 1) goto SaveRecent;
    TmpFName = FName.Left(FName.Len());
    if(Recents.Amount() != 0)
    {
        FRcnt = 0;
        for(i = 0; i < Recents.Amount(); i++)
        {
            if(_strcmpi(Recents.Get(i)->Content, TmpFName.Get_String()) == 0)
            {
                FRcnt = 1;
                break;
            }
        }
        if(AddFile == 0)
        {
            if(FRcnt != 0) Recents.Set(i, "");
            goto SaveRecent;
        }
        if(FRcnt == 0)
        {
            Recents.Add("");
            // Move'em down
            for(j = (Recents.Amount() - 2); j >= 0; j--)
            {
                Recents.Set(j + 1, Recents.Get(j)->Content);
            }
            // Put new entry on top
            Recents.Set(0, TmpFName.Get_String());
            if((long) MaxRecent < (long) Recents.Amount()) Recents.Del(Recents.Amount() - 1);
        }
        else
        {
            // Found: move it in first position
            // Already in first position
            if(i != 0)
            {
                for(j = i; j >= 1; j--)
                {
                    TmpR = Recents.Get(j - 1)->Content;
                    Recents.Set(j - 1, Recents.Get(j)->Content);
                    Recents.Set(j, TmpR.Get_String());
                }
            }
            // Correction of Upper_Case
            Recents.Set(0, TmpFName.Get_String());
        }
        AddInWinRecent(TmpFName);
        goto SaveRecent;
    }
    // First filename to be added
    Recents.Add(TmpFName.Get_String());
SaveRecent:
    for(i = 0; i <= 999; i++)
    {
        if(IniReadKey("Recents", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile).Len() != 0)
        {
            IniDeleteKey("Recents", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
        else
        {
            break;
        }
    }
    j = 0;
    for(i = 0; i < Recents.Amount(); i++)
    {
        if(strlen(Recents.Get(i)->Content) != 0)
        {
            IniWriteKey("Recents", "Rcnt" + (CStr) StringNumberComplement(j, 3).Get_String(), Recents.Get(i)->Content, MainIniFile);
            j++;
        }
    }
    SetRecentFiles(hFileMenu);
    SetRecentProjects(hFileMenu);
}

// -----------------------------------------------------------------------
// Add an entry in recent files list
void AddRecentPrj(CStr FName, long AddFile, long ForceRemove)
{
    int i = 0;
    int j = 0;
    long FRcnt = 0;
    CStr TmpR;

    if(ForceRemove == 1) goto SaveRecent;
    FRcnt = 0;
    if(RecentsPrj.Amount() != 0)
    {
        for(i = 0; i < RecentsPrj.Amount(); i++)
        {
            if(_strcmpi(RecentsPrj.Get(i)->Content, FName.Get_String()) == 0)
            {
                FRcnt = 1;
                break;
            }
        }
        if(AddFile == 0)
        {
            if(FRcnt != 0) RecentsPrj.Set(i, "");
            goto SaveRecent;
        }
        if(FRcnt == 0)
        {
            RecentsPrj.Add("");
            for(j = (RecentsPrj.Amount() - 2); j >= 0; j--)
            {
                RecentsPrj.Set(j + 1, RecentsPrj.Get(j)->Content);
            }
            RecentsPrj.Set(0, FName.Get_String());
            if((long) MaxRecentPrj < (long) RecentsPrj.Amount()) RecentsPrj.Del(RecentsPrj.Amount() - 1);
        }
        else
        {
            // Found: move it in first position
            // Already in first position
            if(i != 0)
            {
                for(j = i; j >= 1; j--)
                {
                    TmpR = RecentsPrj.Get(j - 1)->Content;
                    RecentsPrj.Set(j - 1, RecentsPrj.Get(j)->Content);
                    RecentsPrj.Set(j, TmpR.Get_String());
                }
            }
            // Correction of Upper_Case
            RecentsPrj.Set(0, FName.Get_String());
        }
        AddInWinRecent(FName);
        goto SaveRecent;
    }

    // First filename to be added
    RecentsPrj.Add(FName.Get_String());
SaveRecent:
    for(i = 0; i <= 999; i++)
    {
        if((IniReadKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile)).Len() != 0)
        {
            IniDeleteKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
        else
        {
            break;
        }
    }
    j = 0;
    for(i = 0; i < RecentsPrj.Amount(); i++)
    {
        if(strlen(RecentsPrj.Get(i)->Content) != 0)
        {
            IniWriteKey("RecentsPrjs", "Rcnt" + (CStr) StringNumberComplement(j, 3).Get_String(),
                          RecentsPrj.Get(i)->Content, MainIniFile);
            j++;
        }
    }
    SetRecentFiles(hFileMenu);
    SetRecentProjects(hFileMenu);
}

// -----------------------------------------------------------------------
// Set the user menus
void SetMenus(void)
{
    // Create the menus
    int i = 0;
    CStr LocalMenuName;

    NbrCmds = 0;
    MainMenuH.Erase();
    MenusCmdsLbl.Erase();
    MenusCmds.Erase();
    MenusCmdsComments.Erase();
    MenusCmdsHelp.Erase();
    for(i = 0; i <= 999; i++)
    {
        MainMenusRetVal = IniReadKey("Menus", "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(), UserMenusIniFile);
        if(MainMenusRetVal.Len() == 0) break;
        LocalMenuName = MainMenusRetVal;
        CreateMenuEntry(LocalMenuName);
    }
    DrawMenuBar(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Create a menu
void CreateMenuEntry(CStr MName)
{
    int i = 0;
    HMENU hMenu = 0;
    HMENU hSubMenu = 0;
    CStr MenuName;
    long *ArEntry = 0;
    CStr SecondElement;
    CStr RecognizedPlug;
    CStr MMName;
    long MPlug = 0;
    CStr PlugToCheck;
    CStr BufString;

    hMenu = hMainMenu;
    hSubMenu = CreateMenu();
    for(i = 0; i <= 999; i++)
    {
        MainMenusRetVal = IniReadKey(MName, "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(), UserMenusIniFile);
        if(MainMenusRetVal.Len() == 0) break;
        MainMenusRetVal = MainMenusRetVal.Trim();
        ArEntry = StringSplit(MainMenusRetVal, " | ");
        if(StringGetSplitUBound(ArEntry) == 0)
        {
            MenusCmds.Add("");
            MenusCmdsLbl.Add("");
            MenusCmdsComments.Add("");
            MenusCmdsHelp.Add(0L);
            RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
            goto PlaceOnly;
        }
        SecondElement = StringReplace(StringGetSplitElement(MainMenusRetVal, ArEntry, 1), "$", " ", 1, -1, Binary_Compare);
        if(StringGetSplitElement(MainMenusRetVal, ArEntry, 0).Len() != 0)
        {
            // Command menu
            MPlug = 0;
            BufString = SecondElement.Left(4).Upper_Case();
            if(BufString == "PLUG") MPlug = 1;
            MenusCmdsLbl.Add(StringGetSplitElement(MainMenusRetVal, ArEntry, 0).Trim().Get_String());
            MenusCmds.Add(SecondElement.Get_String());
            if(StringGetSplitUBound(ArEntry) == 2)
            {
                BufString = SecondElement + " (Help file)";
                MenusCmdsComments.Add(BufString.Get_String());
                MenusCmdsHelp.Add(1L);
                RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
            }
            else
            {
                MenusCmdsComments.Add(SecondElement.Get_String());
                MenusCmdsHelp.Add(0L);
                // Try to recognize a plugin
                if(MPlug == 1)
                {
                    PlugToCheck = SecondElement.Mid(6).Trim();
                    PlugToCheck = ChangeRelativePaths(PlugToCheck);
                    PlugToCheck = StringReplace(PlugToCheck, "\"", "", 1, -1, Binary_Compare);
                    if(GetPlugInType(PlugToCheck) == 1) RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
                    else RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
                }
                else
                {
                    RecognizedPlug = StringGetSplitElement(MainMenusRetVal, ArEntry, 0);
                }
            }
        }
        else
        {
            // Simple menu (no command)
            RecognizedPlug = "";
            MenusCmds.Add("");
            MenusCmdsLbl.Add("");
            MenusCmdsComments.Add("");
            MenusCmdsHelp.Add(0L);
        }
PlaceOnly:
        StringReleaseSplit(ArEntry);
        if(strcmp(MainMenusRetVal.Get_String(), "-") == 0)
        {
            if(MenuAddSeparator(hSubMenu) == 0) break;
        }
        else
        {
            if(MenuAddString(hSubMenu, RecognizedPlug, NbrCmds + 1 + MENU_USER_IDBASE, TRUE) == 0) break;
        }
        NbrCmds++;
    }
    // Empty menu
    if(i == 0) return;
    MMName = MName.Trim();
    AppendMenu(hMenu, MF_POPUP, (UINT) hSubMenu, MMName.Get_String());
    MainMenuH.Add((HWND) hSubMenu);
}

// -----------------------------------------------------------------------
// Clear user and scripts menus
void ClearMenus(void)
{
    HMENU hMenu = 0;
    int i = 0;
    long ChildState = 0;
    
    hMenu = GetMenu(hMDIform.hWnd);
    ChildState = ControlGetWindowState(CurrentForm);
    for(i = 0; i < MainMenuH.Amount(); i++)
    {
        if(NbForms == 0)
        {
            DeleteMenu(hMenu, 6, MF_BYPOSITION);
        }
        else
        {
            if(ChildState == SW_SHOWMAXIMIZED) DeleteMenu(hMenu, 7, MF_BYPOSITION);
            else DeleteMenu(hMenu, 6, MF_BYPOSITION);
        }
    }
    DrawMenuBar(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Init comments blocks for factory tooltips
void InitMenuToolTips(void)
{
    MenuFileToolTips[MENU_FILE_NEW_ID] = "New (Ctrl+N)";
    MenuFileToolTips[MENU_FILE_CLEAR_ID] = "Clear";
    MenuFileToolTips[MENU_FILE_DUPLICATEFILE_ID] = "Duplicate file";
    MenuFileToolTips[MENU_FILE_OPENFILE_ID] = "Open source file(s) (Ctrl+O)";
    MenuFileToolTips[MENU_FILE_FILTERS_ID] = "Open file(s) and apply filters (Ctrl+Shift+O)";
    MenuFileToolTips[MENU_FILE_OPENWORKSPACE_ID] = "Open Workspace file";
    MenuFileToolTips[MENU_FILE_INCLUDEFILE_ID] = "Include file(s) (Ctrl+I)";
    MenuFileToolTips[MENU_FILE_FTPMANAGER_ID] = "FTP manager";
    MenuFileToolTips[MENU_FILE_CLOSE_ID] = "Close (Ctrl+F4)";
    MenuFileToolTips[MENU_FILE_CLOSEALL_ID] = "Close all";
    MenuFileToolTips[MENU_FILE_SAVE_ID] = "Save current file (Ctrl+S)";
    MenuFileToolTips[MENU_FILE_SAVEAS_ID] = "Save to new file (Shift+S)";
    MenuFileToolTips[MENU_FILE_SAVEALL_ID] = "Save all opened";
    MenuFileToolTips[MENU_FILE_SAVEWORKSPACE_ID] = "Save to workspace";
    MenuFileToolTips[MENU_FILE_REGISTERASSNIPPET_ID] = "Register as code snippet (Ctrl+H)";
    MenuFileToolTips[MENU_FILE_PRINTFILE_ID] = "Print file (Ctrl+P)";
    MenuFileToolTips[MENU_FILE_CONSOLE_RUN_ID] = "Run (F5)";
    MenuFileToolTips[MENU_FILE_CONSOLE_CHANGE_ID] = "Change console handler";
    MenuFileToolTips[MENU_FILE_RUNEXTERNALPROGRAM_ID] = "Run an external program (F6)";
    MenuFileToolTips[MENU_FILE_SETWORKINGDIRECTORY_ID] = "Set working directory (F12)";
    MenuFileToolTips[MENU_FILE_EXIT_ID] = "Exit (Ctrl+Q)";
    MenuFileToolTips[MENU_FILE_RELOADFILE_ID] = "Reload file (Ctrl+Shift+N)";
    MenuFileToolTips[MENU_FILE_INCLUDEFILTERS_ID] = "Include a file and apply filters (Ctrl+Shift+I)";
    MenuFileToolTips[MENU_FILE_REGISTERASTEMPLATE_ID] = "Register as source template (Ctrl+Shift+H)";
    MenuFileToolTips[MENU_FILE_FAVORITES_ID] = "Organize favorites";
    MenuFileToolTips[MENU_FILE_ADD_FAVORITE_ID] = "Add to favorites";

    MenuEditToolTips[MENU_EDIT_UNDO_ID] = "Undo (Ctrl+Z)";
    MenuEditToolTips[MENU_EDIT_REDO_ID] = "Redo (Ctrl+Y)";
    MenuEditToolTips[MENU_EDIT_CUT_ID] = "Cut (Ctrl+X)";
    MenuEditToolTips[MENU_EDIT_COPY_ID] = "Copy selection (Ctrl+C)";
    MenuEditToolTips[MENU_EDIT_COPYFILEPATHNAME_ID] = "Copy file path/name";
    MenuEditToolTips[MENU_EDIT_COPYENTIRETEXT_ID] = "Copy entire text";
    MenuEditToolTips[MENU_EDIT_PASTE_ID] = "Paste to current window (Ctrl+V)";
    MenuEditToolTips[MENU_EDIT_PASTE_NEWWINDOW_ID] = "Paste to new window (Ctrl+Shift+V)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETESELECTION_ID] = "Delete selection (Del)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETELINE_ID] = "Delete line (Ctrl+E)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID] = "Delete to start of line (Alt+Bksp)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETETOENDOFLINE_ID] = "Delete to end of line (Alt+Del)";
    MenuEditToolTips[MENU_EDIT_DELETE_DELETEPARAGRAPH_ID] = "Delete paragraph (Ctrl+Alt+K)";
    MenuEditToolTips[MENU_EDIT_SELECTLINE_ID] = "Select line (Ctrl+Alt+F8)";
    MenuEditToolTips[MENU_EDIT_SELECTALL_ID] = "Select all (Ctrl+A)";
    MenuEditToolTips[MENU_EDIT_SELECTPROCEDURE_ID] = "Select procedure (Ctrl+Shift+A)";
    MenuEditToolTips[MENU_EDIT_SELECTPROCANDCOLLAPSE_ID] = "Select and collapse procedure (Ctrl+Shift+Q)";
    MenuEditToolTips[MENU_EDIT_UNCOLLAPSEPROCEDURE_ID] = "Expand procedure (Ctrl+Shift+W)";
    MenuEditToolTips[MENU_EDIT_FIND_ID] = "Find (Ctrl+F)";
    MenuEditToolTips[MENU_EDIT_FINDSELECTION_ID] = "Find fast (Ctrl+Shift+F3)";
    MenuEditToolTips[MENU_EDIT_FINDNEXT_ID] = "Find next (F3)";
    MenuEditToolTips[MENU_EDIT_FINDPREVIOUS_ID] = "Find previous (Ctrl+F3)";
    MenuEditToolTips[MENU_EDIT_REPLACE_ID] = "Replace (F4)";
    MenuEditToolTips[MENU_EDIT_PROCVARIABLESSEARCH_ID] = "Proc/Var/Const search (Ctrl+T)";
    MenuEditToolTips[MENU_EDIT_SEARCHINFILES_ID] = "Search in files (Ctrl+B)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_TOGGLE_ID] = "Toggle bookmark (F2)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_FIRST_ID] = "First bookmark (Ctrl+F7)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_PREVIOUS_ID] = "Previous bookmark (Shift+F2)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_NEXT_ID] = "Next bookmark (Ctrl+F2)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_LAST_ID] = "Last bookmark (Ctrl+Shift+F7)";
    MenuEditToolTips[MENU_EDIT_BOOKMARKS_CLEARALL_ID] = "Clear all bookmarks (Ctrl+Shift+F2)";
    MenuEditToolTips[MENU_EDIT_GOTO_TOP_ID] = "Goto top (Ctrl+Home)";
    MenuEditToolTips[MENU_EDIT_GOTO_BOTTOM_ID] = "Goto bottom (Ctrl+End)";
    MenuEditToolTips[MENU_EDIT_GOTO_LINE_ID] = "Goto line (Ctrl+G)";
    MenuEditToolTips[MENU_EDIT_GOTO_MATCHINGBRACKET_ID] = "Goto matching bracket (Ctrl+[)";
    MenuEditToolTips[MENU_EDIT_GOTO_PREVIOUSWORD_ID] = "Goto previous word (Ctrl+Left)";
    MenuEditToolTips[MENU_EDIT_GOTO_NEXTWORD_ID] = "Goto next word (Ctrl+Right)";
    MenuEditToolTips[MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID] = "Goto previous procedure (Alt+Up)";
    MenuEditToolTips[MENU_EDIT_GOTO_NEXTPROCEDURE_ID] = "Goto next procedure (Alt+Down)";
    MenuEditToolTips[MENU_EDIT_GOTO_VARIABLEDECLARATION_ID] = "Goto variable declaration (Ctrl+Alt+S)";
    MenuEditToolTips[MENU_EDIT_GOTO_VARIABLENEXTUSE_ID] = "Goto variable next use";
    MenuEditToolTips[MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID] = "Goto procedure declaration (Ctrl+Alt+F)";
    MenuEditToolTips[MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID] = "Goto procedure entry point";
    MenuEditToolTips[MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID] = "Goto procedure next use";
    MenuEditToolTips[MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID] = "Goto previous paragraph (Ctrl+Alt+Left)";
    MenuEditToolTips[MENU_EDIT_GOTO_NEXTPARAGRAPH_ID] = "Goto next paragraph (Ctrl+Alt+Right)";
    MenuEditToolTips[MENU_EDIT_BLOCK_INDENT_ID] = "Indent block (Tab)";
    MenuEditToolTips[MENU_EDIT_BLOCK_UNINDENT_ID] = "Unindent block (Shift+Tab)";
    MenuEditToolTips[MENU_EDIT_BLOCK_UPPERCASE_ID] = "Upper case block (Ctrl+U)";
    MenuEditToolTips[MENU_EDIT_BLOCK_LOWERCASE_ID] = "Lower case block (Ctrl+L)";
    MenuEditToolTips[MENU_EDIT_BLOCK_TOGGLECASE_ID] = "Toggle case block (Ctrl+Shift+U)";
    MenuEditToolTips[MENU_EDIT_BLOCK_COMMENT_ID] = "Comment block (Ctrl+K)";
    MenuEditToolTips[MENU_EDIT_BLOCK_UNCOMMENT_ID] = "Uncomment block (Ctrl+M)";
    MenuEditToolTips[MENU_EDIT_BLOCK_TABIFY_ID] = "Tabify block (Ctrl+Shift+T)";
    MenuEditToolTips[MENU_EDIT_BLOCK_UNTABIFY_ID] = "Untabify block (Ctrl+Alt+T)";
    MenuEditToolTips[MENU_EDIT_BLOCK_SPACESTOTABS_ID] = "Spaces to tabs (Ctrl+Alt+R)";
    MenuEditToolTips[MENU_EDIT_BLOCK_REGISTERS_ID] = "Registers (Ctrl+R)";
    MenuEditToolTips[MENU_EDIT_BLOCK_BLKCOMMENT_ID] = "Comment (multi lines) (Ctrl+Shift+K)";
    MenuEditToolTips[MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID] = "Custom comment (Ctrl+Shift+F8)";
    MenuEditToolTips[MENU_EDIT_BLOCK_TRIMSPACES_ID] = "Trim trailing spaces (Ctrl+Alt+G)";
    MenuEditToolTips[MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID] = "Convert to include (Ctrl+Shift+L)";
    MenuEditToolTips[MENU_EDIT_BLOCK_SAVEAS_ID] = "Save block (Ctrl+W)";
    MenuEditToolTips[MENU_EDIT_BLOCK_PRINT_ID] = "Print block (Ctrl+Shift+P)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_TOGGLE_ID] = "Toggle code breakpoint (F10)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_FIRST_ID] = "First code breakpoint (Ctrl+F11)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_PREVIOUS_ID] = "Previous code breakpoint (Shift+F10)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_NEXT_ID] = "Next code breakpoint (Ctrl+F10)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_LAST_ID] = "Last code breakpoint (Ctrl+Shift+F11)";
    MenuEditToolTips[MENU_EDIT_BREAKPOINTS_CLEARALL_ID] = "Clear all code breakpoints (Ctrl+Shift+F10)";

    MenuViewToolTips[MENU_VIEW_STATUSBAR_ID] = "Status bar on/off";
    MenuViewToolTips[MENU_VIEW_PROJECTDOCK_ID] = "Project window on/off (Alt+1)";
    MenuViewToolTips[MENU_VIEW_WINDOWSDOCK_ID] = "Windows list on/off (Alt+2)";
    MenuViewToolTips[MENU_VIEW_OUTPUTDOCK_ID] = "Output window on/off (Alt+3)";
    MenuViewToolTips[MENU_VIEW_DEBUGDOCK_ID] = "Process datas window on/off (Alt+4)";
    MenuViewToolTips[MENU_VIEW_REGISTERSDOCK_ID] = "Registers window on/off (Alt+5)";
    MenuViewToolTips[MENU_VIEW_FULLSCREEN_ID] = "Full screen mode on/off (Alt+Shift+Z)";
    MenuViewToolTips[MENU_VIEW_PROPERTIES_ID] = "Properties (F8)";
    MenuViewToolTips[MENU_VIEW_USERMENUSEDITOR_ID] = "User menus editor (F9)";
    MenuViewToolTips[MENU_VIEW_USERTOOLBARSEDITOR_ID] = "Toolbars manager (Shift+F9)";

    MenuProjectToolTips[MENU_PROJECT_CREATENEWPROJECT_ID] = "Create new project";
    MenuProjectToolTips[MENU_PROJECT_OPENPROJECT_ID] = "Open project (Ctrl+Alt+O)";
    MenuProjectToolTips[MENU_PROJECT_SAVEPROJECT_ID] = "Save project (Ctrl+Shift+S)";
    MenuProjectToolTips[MENU_PROJECT_SAVEPROJECTAS_ID] = "Save project as";
    MenuProjectToolTips[MENU_PROJECT_ADDINCLUDE_ID] = "Add include file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDLIBRARY_ID] = "Add library file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDMODULE_ID] = "Add module file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDOBJECT_ID] = "Add object file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDRESOURCE_ID] = "Add resource file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDTEXT_ID] = "Add RC file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDICON_ID] = "Add icon file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDCURSOR_ID] = "Add cursor file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDBITMAP_ID] = "Add bitmap file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDSTRINGS_ID] = "Add strings file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDACCELERATORS_ID] = "Add accelerators file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDMENUS_ID] = "Add menus file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDDIALOG_ID] = "Add dialog file(s)";
    MenuProjectToolTips[MENU_PROJECT_ADDRAWDATA_ID] = "Add raw datas file(s)";
    MenuProjectToolTips[MENU_PROJECT_PROJECTPROPERTIES_ID] = "Project properties";
    MenuProjectToolTips[MENU_PROJECT_ASSEMBLEPROJECT_ID] = "Build project (Shift+F5)";
    MenuProjectToolTips[MENU_PROJECT_RUNPROJECT_ID] = "Run project (Ctrl+F5)";
    MenuProjectToolTips[MENU_PROJECT_DEBUGPROJECT_ID] = "Debug project (Ctrl+Shift+F5)";

    MenuWindowToolTips[MENU_WINDOW_CONTEXTHELP_ID] = "Context help (F1)";
    MenuWindowToolTips[MENU_WINDOW_MSDNHELP_ID] = "MSDN/Platform SDK help (Ctrl+F1)";
    MenuWindowToolTips[MENU_WINDOW_DDKHELP_ID] = "Windows DDK help (Ctrl+Shift+F1)";
    MenuWindowToolTips[MENU_WINDOW_DIRECTXSDKHELP_ID] = "DirectX SDK help (Alt+Shift+F1)";
    MenuWindowToolTips[MENU_WINDOW_ABOUT_ID] = "About";
    MenuWindowToolTips[MENU_WINDOW_SPLIT_ID] = "Split";
    MenuWindowToolTips[MENU_WINDOW_TILEHORIZONTALLY_ID] = "Tile horizontally";
    MenuWindowToolTips[MENU_WINDOW_TILEVERTICALLY_ID] = "Tile vertically";
    MenuWindowToolTips[MENU_WINDOW_CASCADE_ID] = "Cascade";
    MenuWindowToolTips[MENU_WINDOW_ARRANGEICONS_ID] = "Arrange icons";
    MenuWindowToolTips[MENU_WINDOW_NEXTWINDOW_ID] = "Next window (Ctrl+F6 / Ctrl+Tab)";
    MenuWindowToolTips[MENU_WINDOW_PREVIOUSWINDOW_ID] = "Previous window (Ctrl+Shift+F6 / Ctrl+Shift+Tab)";
    MenuWindowToolTips[MENU_WINDOW_TIPOFTHEDAY_ID] = "Tip of the day";
}

// -----------------------------------------------------------------------
// Init comments blocks for factory menus
void InitMenuComments(void)
{
    CStr BufString;

    MenuFileComments[MENU_FILE_NEW_ID] = "Create a new empty file";
    MenuFileComments[MENU_FILE_CLEAR_ID] = "Clear the current file";
    MenuFileComments[MENU_FILE_DUPLICATEFILE_ID] = "Create a new instance of the current file";
    MenuFileComments[MENU_FILE_OPENFILE_ID] = "Open file(s) from local resource";
    MenuFileComments[MENU_FILE_FILTERS_ID] = "Open file(s) and apply filters";
    MenuFileComments[MENU_FILE_OPENWORKSPACE_ID] = "Open a workspace file";
    MenuFileComments[MENU_FILE_INCLUDEFILE_ID] = "Insert file(s) at current position";
    MenuFileComments[MENU_FILE_FTPMANAGER_ID] = "Manage files on FTP resources";
    MenuFileComments[MENU_FILE_CLOSE_ID] = "Close the current file";
    MenuFileComments[MENU_FILE_CLOSEALL_ID] = "Close all files";
    MenuFileComments[MENU_FILE_SAVE_ID] = "Save current file";
    MenuFileComments[MENU_FILE_SAVEAS_ID] = "Save current file under a new name";
    MenuFileComments[MENU_FILE_SAVEALL_ID] = "Save all opened files";
    MenuFileComments[MENU_FILE_SAVEWORKSPACE_ID] = "Save all files as a workspace";
    MenuFileComments[MENU_FILE_REGISTERASSNIPPET_ID] = "Register current file as code snippet";
    MenuFileComments[MENU_FILE_PRINTFILE_ID] = "Print the current file";
    MenuFileComments[MENU_FILE_CONSOLE_RUN_ID] = "Open a console handler";
    MenuFileComments[MENU_FILE_CONSOLE_CHANGE_ID] = "Change the console handler";
    MenuFileComments[MENU_FILE_RUNEXTERNALPROGRAM_ID] = "Run a selected program";
    MenuFileComments[MENU_FILE_SETWORKINGDIRECTORY_ID] = "Set current directory";
    BufString = "Quit " + (CStr) AppTitle;
    MenuFileComments[MENU_FILE_EXIT_ID] = _strdup(BufString.Get_String());
    MenuFileComments[MENU_FILE_RELOADFILE_ID] = "Reload current file";
    MenuFileComments[MENU_FILE_INCLUDEFILTERS_ID] = "Insert filtered file(s) at current position";
    MenuFileComments[MENU_FILE_REGISTERASTEMPLATE_ID] = "Register current file as source template";
    MenuFileComments[MENU_FILE_FAVORITES_ID] = "Organize favorites documents";
    MenuFileComments[MENU_FILE_ADD_FAVORITE_ID] = "Add current document to favorites";

    MenuEditComments[MENU_EDIT_UNDO_ID] = "Undo last command";
    MenuEditComments[MENU_EDIT_REDO_ID] = "Redo last command";
    MenuEditComments[MENU_EDIT_CUT_ID] = "Cut and copy selected text into clipboard";
    MenuEditComments[MENU_EDIT_COPY_ID] = "Copy selected text into clipboard";
    MenuEditComments[MENU_EDIT_COPYFILEPATHNAME_ID] = "Copy file path and name into clipboard";
    MenuEditComments[MENU_EDIT_COPYENTIRETEXT_ID] = "Copy entire text into clipboard";
    MenuEditComments[MENU_EDIT_PASTE_ID] = "Paste text from clipboard into current window";
    MenuEditComments[MENU_EDIT_DELETE_DELETESELECTION_ID] = "Delete selected text";
    MenuEditComments[MENU_EDIT_DELETE_DELETELINE_ID] = "Delete current line";
    MenuEditComments[MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID] = "Delete text to start of line from current position";
    MenuEditComments[MENU_EDIT_DELETE_DELETETOENDOFLINE_ID] = "Delete text to end of line from current position";
    MenuEditComments[MENU_EDIT_SELECTLINE_ID] = "Select current line";
    MenuEditComments[MENU_EDIT_SELECTALL_ID] = "Select entire text";
    MenuEditComments[MENU_EDIT_SELECTPROCEDURE_ID] = "Select a procedure from caret position";
    MenuEditComments[MENU_EDIT_SELECTPROCANDCOLLAPSE_ID] = "Select a procedure and collapse it into a file";
    MenuEditComments[MENU_EDIT_UNCOLLAPSEPROCEDURE_ID] = "Expand a procedure from a file";
    MenuEditComments[MENU_EDIT_FIND_ID] = "Find in text";
    MenuEditComments[MENU_EDIT_FINDSELECTION_ID] = "Search with current selection or word under caret";
    MenuEditComments[MENU_EDIT_FINDNEXT_ID] = "Find next occurence";
    MenuEditComments[MENU_EDIT_FINDPREVIOUS_ID] = "Find previous occurence";
    MenuEditComments[MENU_EDIT_REPLACE_ID] = "Replace in text";
    MenuEditComments[MENU_EDIT_PROCVARIABLESSEARCH_ID] = "Search for procedures/variables/constants in windows or project";
    MenuEditComments[MENU_EDIT_SEARCHINFILES_ID] = "Search for procedures/variables in external files";
    MenuEditComments[MENU_EDIT_BOOKMARKS_TOGGLE_ID] = "Toggle bookmark at current line";
    MenuEditComments[MENU_EDIT_BOOKMARKS_FIRST_ID] = "Jump to first bookmark";
    MenuEditComments[MENU_EDIT_BOOKMARKS_PREVIOUS_ID] = "Jump to previous bookmark";
    MenuEditComments[MENU_EDIT_BOOKMARKS_NEXT_ID] = "Jump to next bookmark";
    MenuEditComments[MENU_EDIT_BOOKMARKS_LAST_ID] = "Jump to last bookmark";
    MenuEditComments[MENU_EDIT_BOOKMARKS_CLEARALL_ID] = "Clear all bookmarks";
    MenuEditComments[MENU_EDIT_GOTO_TOP_ID] = "Goto top of file";
    MenuEditComments[MENU_EDIT_GOTO_BOTTOM_ID] = "Goto bottom of file";
    MenuEditComments[MENU_EDIT_GOTO_LINE_ID] = "Goto specified line";
    MenuEditComments[MENU_EDIT_GOTO_MATCHINGBRACKET_ID] = "Goto bracket matching current one";
    MenuEditComments[MENU_EDIT_GOTO_PREVIOUSWORD_ID] = "Position caret on previous word";
    MenuEditComments[MENU_EDIT_GOTO_NEXTWORD_ID] = "Position caret on next word";
    MenuEditComments[MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID] = "Position caret on previous procedure entry point";
    MenuEditComments[MENU_EDIT_GOTO_NEXTPROCEDURE_ID] = "Position caret on next procedure entry point";
    MenuEditComments[MENU_EDIT_GOTO_VARIABLEDECLARATION_ID] = "Search for variable declaration";
    MenuEditComments[MENU_EDIT_GOTO_VARIABLENEXTUSE_ID] = "Search for next variable use";
    MenuEditComments[MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID] = "Search for procedure declaration (prototype)";
    MenuEditComments[MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID] = "Search for procedure entry point";
    MenuEditComments[MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID] = "Search for next procedure use";
    MenuEditComments[MENU_EDIT_BLOCK_INDENT_ID] = "Indent selected text";
    MenuEditComments[MENU_EDIT_BLOCK_UNINDENT_ID] = "Unindent selected text";
    MenuEditComments[MENU_EDIT_BLOCK_UPPERCASE_ID] = "Convert selected text to upper case";
    MenuEditComments[MENU_EDIT_BLOCK_LOWERCASE_ID] = "Convert selected text to lower case";
    MenuEditComments[MENU_EDIT_BLOCK_TOGGLECASE_ID] = "Toggle chars case in selected text";
    MenuEditComments[MENU_EDIT_BLOCK_COMMENT_ID] = "Comment selected text";
    MenuEditComments[MENU_EDIT_BLOCK_UNCOMMENT_ID] = "Uncomment selected text";
    MenuEditComments[MENU_EDIT_BLOCK_TABIFY_ID] = "Tabify selected text";
    MenuEditComments[MENU_EDIT_BLOCK_UNTABIFY_ID] = "Untabify selected text";
    MenuEditComments[MENU_EDIT_BLOCK_SPACESTOTABS_ID] = "Convert spaces into tabulations in selected text";
    MenuEditComments[MENU_EDIT_BLOCK_REGISTERS_ID] = "Search for used registers in selected text";
    MenuEditComments[MENU_EDIT_BLOCK_BLKCOMMENT_ID] = "Put selected text into multiline comment";
    MenuEditComments[MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID] = "Comment block with user specified text";
    MenuEditComments[MENU_EDIT_BLOCK_TRIMSPACES_ID] = "Remove trailing spaces from selected text";
    MenuEditComments[MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID] = "Save current selection and convert it to include statement";
    MenuEditComments[MENU_EDIT_BLOCK_SAVEAS_ID] = "Save current selection in a file";
    MenuEditComments[MENU_EDIT_BLOCK_PRINT_ID] = "Print current selection";
    MenuEditComments[MENU_EDIT_PASTE_NEWWINDOW_ID] = "Paste text from clipboard into a new window";
    MenuEditComments[MENU_EDIT_DELETE_DELETEPARAGRAPH_ID] = "Delete current paragraph";
    MenuEditComments[MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID] = "Position caret on previous paragraph";
    MenuEditComments[MENU_EDIT_GOTO_NEXTPARAGRAPH_ID] = "Position caret on next paragraph";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_TOGGLE_ID] = "Toggle code breakpoint at current line";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_FIRST_ID] = "Jump to first code breakpoint";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_PREVIOUS_ID] = "Jump to previous code breakpoint";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_NEXT_ID] = "Jump to next code breakpoint";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_LAST_ID] = "Jump to last code breakpoint";
    MenuEditComments[MENU_EDIT_BREAKPOINTS_CLEARALL_ID] = "Clear all code breakpoints";

    MenuViewComments[MENU_VIEW_STATUSBAR_ID] = "Turn statusbar on/off";
    MenuViewComments[MENU_VIEW_PROJECTDOCK_ID] = "Turn project window on/off";
    MenuViewComments[MENU_VIEW_WINDOWSDOCK_ID] = "Turn windows list on/off";
    MenuViewComments[MENU_VIEW_OUTPUTDOCK_ID] = "Turn output window on/off";
    MenuViewComments[MENU_VIEW_DEBUGDOCK_ID] = "Turn debugged process datas window on/off";
    MenuViewComments[MENU_VIEW_REGISTERSDOCK_ID] = "Turn registers window on/off";
    MenuViewComments[MENU_VIEW_FULLSCREEN_ID] = "Turn full screen mode on/off";
    BufString = "Modify " + (CStr) AppTitle + (CStr) "'s properties";
    MenuViewComments[MENU_VIEW_PROPERTIES_ID] = _strdup(BufString.Get_String());
    MenuViewComments[MENU_VIEW_USERMENUSEDITOR_ID] = "Modify user's menus";
    MenuViewComments[MENU_VIEW_USERTOOLBARSEDITOR_ID] = "Modify and create toolbars";
    
    MenuProjectComments[MENU_PROJECT_CREATENEWPROJECT_ID] = "Create a new project";
    MenuProjectComments[MENU_PROJECT_OPENPROJECT_ID] = "Open an existing project";
    MenuProjectComments[MENU_PROJECT_SAVEPROJECT_ID] = "Save current project";
    MenuProjectComments[MENU_PROJECT_SAVEPROJECTAS_ID] = "Save project under a new name";
    MenuProjectComments[MENU_PROJECT_ADDINCLUDE_ID] = "Add new include(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDLIBRARY_ID] = "Add new library(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDMODULE_ID] = "Add new module(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDOBJECT_ID] = "Add new object(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDRESOURCE_ID] = "Add new resource file(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDTEXT_ID] = "Add new document(s) to current project";
    MenuProjectComments[MENU_PROJECT_ADDICON_ID] = "Add new icon file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDCURSOR_ID] = "Add new cursor file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDBITMAP_ID] = "Add new bitmap file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDSTRINGS_ID] = "Add new strings file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDACCELERATORS_ID] = "Add new accelerators file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDMENUS_ID] = "Add new menus file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDDIALOG_ID] = "Add new dialog file(s) to project";
    MenuProjectComments[MENU_PROJECT_ADDRAWDATA_ID] = "Add new raw datas file(s) to project";
    MenuProjectComments[MENU_PROJECT_PROJECTPROPERTIES_ID] = "Modify project properties";
    MenuProjectComments[MENU_PROJECT_ASSEMBLEPROJECT_ID] = "Build complete project";
    MenuProjectComments[MENU_PROJECT_RUNPROJECT_ID] = "Build and run complete project";
    MenuProjectComments[MENU_PROJECT_DEBUGPROJECT_ID] = "Build and debug complete project";

    MenuAddInComments[MENU_ADDINS_MANAGER_ID] = "Manage Chromatic AddIns";
    
    MenuWindowComments[MENU_WINDOW_CONTEXTHELP_ID] = "Search for a keyword in help files";
    MenuWindowComments[MENU_WINDOW_MSDNHELP_ID] = "Search for a keyword in MSDN or Platform SDK";
    MenuWindowComments[MENU_WINDOW_DDKHELP_ID] = "Search for a keyword in Windows DDK";
    MenuWindowComments[MENU_WINDOW_DIRECTXSDKHELP_ID] = "Search for a keyword in DirectX SDK";
    BufString = "About " + (CStr) AppTitle;
    MenuWindowComments[MENU_WINDOW_ABOUT_ID] = _strdup(BufString.Get_String());
    MenuWindowComments[MENU_WINDOW_SPLIT_ID] = "Split current window";
    MenuWindowComments[MENU_WINDOW_TILEHORIZONTALLY_ID] = "Tile windows horizontally";
    MenuWindowComments[MENU_WINDOW_TILEVERTICALLY_ID] = "Tile windows vertically";
    MenuWindowComments[MENU_WINDOW_CASCADE_ID] = "Cascade windows";
    MenuWindowComments[MENU_WINDOW_ARRANGEICONS_ID] = "Arrange minimized windows";
    MenuWindowComments[MENU_WINDOW_NEXTWINDOW_ID] = "Activate next window";
    MenuWindowComments[MENU_WINDOW_PREVIOUSWINDOW_ID] = "Activate previous window";
    MenuWindowComments[MENU_WINDOW_TIPOFTHEDAY_ID] = "Display the 'Tip of the day' window";

    MenuCodeMaxComments[MENU_CONTEXT_CONTEXTHELP_ID] = "Search for a keyword in help files";
    MenuCodeMaxComments[MENU_CONTEXT_MSDNHELP_ID] = "Search for a keyword in MSDN or Platform SDK";
    MenuCodeMaxComments[MENU_CONTEXT_DDKHELP_ID] = "Search for a keyword in Windows DDK";
    MenuCodeMaxComments[MENU_CONTEXT_DIRECTXSDKHELP_ID] = "Search for a keyword in DirectX SDK";
    MenuCodeMaxComments[MENU_CONTEXT_OPENFILEUNDERCURSOR_ID] = "Open file referenced under caret";
    MenuCodeMaxComments[MENU_CONTEXT_EXECFILEUNDERCURSOR_ID] = "Execute file referenced under caret";
    MenuCodeMaxComments[MENU_CONTEXT_DECTOHEX_ID] = "Convert decimal number into hexadecimal";
    MenuCodeMaxComments[MENU_CONTEXT_DECTOBIN_ID] = "Convert decimal number into binary";
    MenuCodeMaxComments[MENU_CONTEXT_HEXTODEC_ID] = "Convert hexadecimal number into decimal";
    MenuCodeMaxComments[MENU_CONTEXT_HEXTOBIN_ID] = "Convert hexadecimal number into binary";
    MenuCodeMaxComments[MENU_CONTEXT_BINTODEC_ID] = "Convert binary number into decimal";
    MenuCodeMaxComments[MENU_CONTEXT_BINTOHEX_ID] = "Convert binary number into hexadecimal";
    MenuCodeMaxComments[MENU_CONTEXT_HEXTOFLT_ID] = "Convert hexadecimal number into floating point";
    MenuCodeMaxComments[MENU_CONTEXT_FLTTOHEX_ID] = "Convert floating point number into hexadecimal";
    MenuCodeMaxComments[MENU_CONTEXT_UNDO_ID] = "Undo last command";
    MenuCodeMaxComments[MENU_CONTEXT_REDO_ID] = "Redo last command";
    MenuCodeMaxComments[MENU_CONTEXT_CUT_ID] = "Cut and copy selected text into clipboard";
    MenuCodeMaxComments[MENU_CONTEXT_COPY_ID] = "Copy selected text into clipboard";
    MenuCodeMaxComments[MENU_CONTEXT_COPYFILEPATHNAME_ID] = "Copy file path and name into clipboard";
    MenuCodeMaxComments[MENU_CONTEXT_COPYENTIRETEXT_ID] = "Copy entire text into clipboard";
    MenuCodeMaxComments[MENU_CONTEXT_PASTE_ID] = "Paste text from clipboard into current window";
    MenuCodeMaxComments[MENU_CONTEXT_PASTENEWWINDOW_ID] = "Paste text from clipboard into a new window";
    MenuCodeMaxComments[MENU_CONTEXT_SELECTLINE_ID] = "Select current line";
    MenuCodeMaxComments[MENU_CONTEXT_SELECTALL_ID] = "Select entire text";
    MenuCodeMaxComments[MENU_CONTEXT_SELECTPROCEDURE_ID] = "Select a procedure from caret position";
    MenuCodeMaxComments[MENU_CONTEXT_SELECTPROCEDUREANDCOLLAPSE_ID] = "Select a procedure and collapse it into a file";
    MenuCodeMaxComments[MENU_CONTEXT_UNCOLLAPSEPROCEDURE_ID] = "Expand a procedure from a file";
    MenuCodeMaxComments[MENU_CONTEXT_FIND_ID] = "Find in text";
    MenuCodeMaxComments[MENU_CONTEXT_REPLACE_ID] = "Replace in text";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_TOGGLE_ID] = "Toggle bookmark at current line";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_FIRST_ID] = "Jump to first bookmark";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_PREVIOUS_ID] = "Jump to previous bookmark";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_NEXT_ID] = "Jump to next bookmark";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_LAST_ID] = "Jump to last bookmark";
    MenuCodeMaxComments[MENU_CONTEXT_BOOKMARKS_CLEARALL_ID] = "Clear all bookmarks";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_TOGGLE_ID] = "Toggle code breakpoint at current line";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_FIRST_ID] = "Jump to first code breakpoint";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_PREVIOUS_ID] = "Jump to previous code breakpoint";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_NEXT_ID] = "Jump to next code breakpoint";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_LAST_ID] = "Jump to last code breakpoint";
    MenuCodeMaxComments[MENU_CONTEXT_BREAKPOINTS_CLEARALL_ID] = "Clear all code breakpoints";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_TOP_ID] = "Goto top of file";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_BOTTOM_ID] = "Goto bottom of file";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_LINE_ID] = "Goto specified line";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_MATCHINGBRACKET_ID] = "Goto bracket matching current one";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PREVIOUSWORD_ID] = "Position caret on previous word";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_NEXTWORD_ID] = "Position caret on next word";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PREVIOUSPARAGRAPH_ID] = "Position caret on previous paragraph";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_NEXTPARAGRAPH_ID] = "Position caret on next paragraph";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PREVIOUSPROCEDURE_ID] = "Position caret on previous procedure entry point";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_NEXTPROCEDURE_ID] = "Position caret on next procedure entry point";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_VARIABLEDECLARATION_ID] = "Search for variable declaration";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_VARIABLENEXTUSE_ID] = "Search for next variable use";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PROCEDUREPROTOTYPE_ID] = "Search for procedure declaration (prototype)";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PROCEDUREENTRYPOINT_ID] = "Search for procedure entry point";
    MenuCodeMaxComments[MENU_CONTEXT_GOTO_PROCEDURENEXTUSE_ID] = "Search for next procedure use";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_INDENT_ID] = "Indent selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_UNINDENT_ID] = "Unindent selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_UPPERCASE_ID] = "Convert selected text to upper case";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_LOWERCASE_ID] = "Convert selected text to lower case";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_TOGGLECASE_ID] = "Toggle chars case in selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_COMMENT_ID] = "Comment selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_UNCOMMENT_ID] = "Uncomment selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_TABIFY_ID] = "Tabify selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_UNTABIFY_ID] = "Untabify selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_SPACESTOTABS_ID] = "Convert spaces into tabulations in selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_REGISTERS_ID] = "Search for used registers";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_BLKCOMMENT_ID] = "Put selected text into multiline comment";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_CUSTOMCOMMENT_ID] = "Comment block with user specified text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_TRIMSPACES_ID] = "Remove trailing spaces from selected text";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_CONVERTTOINCLUDE_ID] = "Save current selection and convert it to include statement";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_SAVEAS_ID] = "Save current selection in a file";
    MenuCodeMaxComments[MENU_CONTEXT_BLOCK_PRINT_ID] = "Print current selection";
    MenuCodeMaxComments[MENU_CONTEXT_NEW_ID] = "Create a new empty file";

    MenuCodeMaxComments[MENU_CONTEXT_INCLUDEFILE_ID] = "Insert file(s) at current position";
    MenuCodeMaxComments[MENU_CONTEXT_INCLUDEFILTERS_ID] = "Insert filtered file(s) at current position";

    MenuCodeMaxComments[MENU_CONTEXT_DUPLICATEFILE_ID] = "Create a new instance of the current file";
    MenuCodeMaxComments[MENU_CONTEXT_RELOADFILE_ID] = "Reload current file";
    BufString = "Modify " + (CStr) AppTitle + (CStr) "'s properties";
    MenuCodeMaxComments[MENU_CONTEXT_PROPERTIES_ID] = _strdup(BufString.Get_String());
    RegCodeMaxLocalCommands();
}

// -----------------------------------------------------------------------
// Register commands in codemax
void RegCodeMaxLocalCommands(void)
{
    // Ctrl+Enter
    RegCMCom("Open file under caret", MenuCodeMaxComments[MENU_CONTEXT_OPENFILEUNDERCURSOR_ID], 1000, 2, 13);
    // Ctrl+Shift+Enter
    RegCMCom("Execute file under caret", MenuCodeMaxComments[MENU_CONTEXT_EXECFILEUNDERCURSOR_ID], 1001, 2 + 1, 13);
    // Ctrl+Space
    RegCMCom("Insert snippet", "Insert a code snippet at current position", 1002, 2 + 4, 32);
    RegCMCom("Dec to hex", MenuCodeMaxComments[MENU_CONTEXT_DECTOHEX_ID], 1003, 2, 49);
    RegCMCom("Dec to bin", MenuCodeMaxComments[MENU_CONTEXT_DECTOBIN_ID], 1004, 2, 50);
    RegCMCom("Hex to dec", MenuCodeMaxComments[MENU_CONTEXT_HEXTODEC_ID], 1005, 2, 51);
    RegCMCom("Hex to bin", MenuCodeMaxComments[MENU_CONTEXT_HEXTOBIN_ID], 1006, 2, 52);
    RegCMCom("Bin to dec", MenuCodeMaxComments[MENU_CONTEXT_BINTODEC_ID], 1007, 2, 53);
    RegCMCom("Bin to hex", MenuCodeMaxComments[MENU_CONTEXT_BINTOHEX_ID], 1008, 2, 54);
    RegCMCom("Hex to flt", MenuCodeMaxComments[MENU_CONTEXT_HEXTOFLT_ID], 1009, 2, 55);
    RegCMCom("Flt to Hex", MenuCodeMaxComments[MENU_CONTEXT_FLTTOHEX_ID], 1010, 2, 56);
    RegCMCom("Scripts", "Execute a script at current position", 1011, 2 + 1, 32);
    RegCMCom("Display APIs", "Display APIs database names", 1012, 2, 32);
}

// -----------------------------------------------------------------------
// Register a command into CodeMax
void RegCMCom(CStr CmdName, CStr CmdComment, long CmdNumber, long CmdModif, long CmdKey)
{
    hCodeMaxCMRegisterCommand(CmdNumber, CmdName.Get_String(), CmdComment.Get_String());
    CHotKey.byModifiers1 = (BYTE) CmdModif;
    CHotKey.byModifiers2 = 0;
    CHotKey.nVirtKey1 = CmdKey;
    CHotKey.nVirtKey2 = 0;
    hCodeMaxCMRegisterHotKey((long) &CHotKey, CmdNumber);
}

// -----------------------------------------------------------------------
// Retrieve a plugin type
// 0=not recognized
// 1=QEditor
long GetPlugInType(CStr PlugInName)
{
    long ReturnValue = 0;
    HMODULE hPlug = 0;
    
    hPlug = LoadLibrary(PlugInName.Get_String());
    if(hPlug != 0)
    {
        if(GetProcAddress(hPlug, "QePlugIn") != 0) ReturnValue = 1;
        FreeLibrary(hPlug);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add a filename into windows recent files list
void AddInWinRecent(CStr FName)
{
    CStr RcntVal;

    RcntVal = IniReadKey("Layout", "WinRecent", MainIniFile);
    if(RcntVal.Len() == 0) return;
    if(RcntVal == "1") MiscShellAddFileToRecents(FName);
}

// -----------------------------------------------------------------------
// Create the history of the run programs
void CreateRunProgMenu(void)
{
    int i = 0;
    int k = 0;
    CStr ProgRetVal;
    long FoundProg = 0;
    
    RunProgArray.Erase();
    k = 0;
    while(DeleteMenu(hFileMenuRunProg, 0, MF_BYPOSITION) != 0)
    {
    }
    MenuAddString(hFileMenuRunProg, "Run...\tF6", MENU_FILE_IDBASE + MENU_FILE_RUNEXTERNALPROGRAM_ID, TRUE);
    MenuSetDefaultItem(hFileMenuRunProg, 0);
    for(i = 0; i <= 999; i++)
    {
        ProgRetVal = IniReadKey("RunProg", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(ProgRetVal.Len() == 0) break;
        ProgRetVal = ChangeRelativePaths(ProgRetVal);
        // Check the file
        if(FileExist(ProgRetVal) != 0)
        {
            // Add a separator at first row
            if(FoundProg == 0) MenuAddSeparator(hFileMenuRunProg);
            FoundProg = 1;
            // Get the language
            MenuAddString(hFileMenuRunProg, ProgRetVal, k + MENU_RECENTAPPLICATIONS_IDBASE, TRUE);
            RunProgArray.Add(ProgRetVal.Get_String());
            k++;
        }
    }
    if(k != 0)
    {
        MenuAddSeparator(hFileMenuRunProg);
        MenuAddString(hFileMenuRunProg, "Clear all entries", MENU_RUNPROGCLEAR_IDBASE, TRUE);
    }
    SubMenusArray[15] = hFileMenuRunProg;
}

// -----------------------------------------------------------------------
// Add an entry in run programs history
void AddRecentRunProg(CStr FName, long AddFile, long ForceRemove)
{
    int i = 0;
    int j = 0;
    long FRcnt = 0;
    CStr TmpR;
    CStr TmpFName;

    if(ForceRemove == 1) goto SaveRecent;
    TmpFName = FName.Left(FName.Len());
    if(RunProgArray.Amount() != 0)
    {
        FRcnt = 0;
        for(i = 0; i < RunProgArray.Amount(); i++)
        {
            if(_strcmpi(RunProgArray.Get(i)->Content, TmpFName.Get_String()) == 0)
            {
                FRcnt = 1;
                break;
            }
        }
        if(AddFile == 0)
        {
            if(FRcnt != 0) RunProgArray.Set(i, "");
            goto SaveRecent;
        }
        if(FRcnt == 0)
        {
            RunProgArray.Add("");
            // Move'em down
            for(j = (RunProgArray.Amount() - 2); j >= 0; j--)
            {
                RunProgArray.Set(j + 1, RunProgArray.Get(j)->Content);
            }
            RunProgArray.Set(0, TmpFName.Get_String());
            if((long) MaxRecent < (long) RunProgArray.Amount())
            {
                RunProgArray.Del(RunProgArray.Amount() - 1);
            }
        }
        else
        {
            // Found: move it in first position
            // Already in first position
            if(i != 0)
            {
                for(j = i; j >= 1; j--)
                {
                    TmpR = RunProgArray.Get(j - 1)->Content;
                    RunProgArray.Set(j - 1, RunProgArray.Get(j)->Content);
                    RunProgArray.Set(j, TmpR.Get_String());
                }
            }
            // Correction of Upper_Case
            RunProgArray.Set(0, TmpFName.Get_String());
        }
        AddInWinRecent(TmpFName);
        goto SaveRecent;
    }
    // First filename to be added
    RunProgArray.Add(TmpFName.Get_String());
SaveRecent:
    for(i = 0; i <= 999; i++)
    {
        if(IniReadKey("RunProg", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile).Len() != 0)
        {
            IniDeleteKey("RunProg", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        }
        else
        {
            break;
        }
    }
    j = 0;
    for(i = 0; i < RunProgArray.Amount(); i++)
    {
        if(RunProgArray.Get(i)->Content != 0)
        {
            IniWriteKey("RunProg", "Name" + (CStr) StringNumberComplement(j, 3).Get_String(),
                          RunProgArray.Get(i)->Content, MainIniFile);
            j++;
        }
    }
    CreateRunProgMenu();
}

// -----------------------------------------------------------------------
// Create the list of favorites files
void CreateFavoritesMenu(void)
{
    int i = 0;
    int k = 0;
    CStr ProgRetVal;
    long FoundProg = 0;
    
    FavoritesArray.Erase();
    k = 0;
    while(DeleteMenu(hFileMenuFavorites, 0, MF_BYPOSITION) != 0)
    {
    }
    MenuAddString(hFileMenuFavorites, "Add to favorites", MENU_FILE_IDBASE + MENU_FILE_ADD_FAVORITE_ID, TRUE);
    MenuAddString(hFileMenuFavorites, "Organize favorites...", MENU_FILE_IDBASE + MENU_FILE_FAVORITES_ID, TRUE);
    MenuSetDefaultItem(hFileMenuFavorites, 1);
    for(i = 0; i <= 999; i++)
    {
        ProgRetVal = IniReadKey("Favorites", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(ProgRetVal.Len() == 0) break;
        ProgRetVal = ChangeRelativePaths(ProgRetVal);
        // Check the file
        if(FileExist(ProgRetVal) != 0)
        {
            // Add a separator at first row
            if(FoundProg == 0) MenuAddSeparator(hFileMenuFavorites);
            FoundProg = 1;
            // Get the language
            MenuAddString(hFileMenuFavorites, ProgRetVal, k + MENU_FAVORITESFILES_IDBASE, TRUE);
            FavoritesArray.Add(ProgRetVal.Get_String());
            k++;
        }
    }
    SubMenusArray[16] = hFileMenuFavorites;
}

// -----------------------------------------------------------------------
// Clean up data before removing the list entry
void FiltersArray_Erase_CallBack(LPFILTER_ENTRY Entry)
{
    if(Entry->Name) free(Entry->Name);
    Entry->Name = NULL;
}

// -----------------------------------------------------------------------
// Create the list of filters profiles
void CreateFiltersMenu(HMENU Dest_Menu, int Index, int Lab_Index, CStr Label, int Include_Menu)
{
    int i = 0;
    int k = 0;
    int mnu_idx = 0;
    CStr ProgRetVal;
    long FoundProg = 0;
    
    LPFILTER_ENTRY Entry = (LPFILTER_ENTRY) calloc(sizeof(FILTER_ENTRY), 1);

    FiltersArray.Erase(FiltersArray_Erase_CallBack);
    k = 0;
    while(DeleteMenu(Dest_Menu, 0, MF_BYPOSITION) != 0)
    {
    }
    MenuAddString(Dest_Menu, Label, Lab_Index, TRUE);
    MenuSetDefaultItem(Dest_Menu, 0);

    for(i = 0; i <= 999; i++)
    {
        ProgRetVal = IniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), FiltersIniFile);
        if(ProgRetVal.Len() == 0) break;
        // Add a separator at first row
        if(FoundProg == 0) MenuAddSeparator(Dest_Menu);
        FoundProg = 1;
        if(Include_Menu) MenuAddString(Dest_Menu, ProgRetVal + "\tTo window", i + MENU_FILTERS_INCLUDE_IDBASE, TRUE);
        else MenuAddString(Dest_Menu, ProgRetVal + "\tTo window", i + MENU_FILTERS_IDBASE, TRUE);
        Entry->Name = _strdup(ProgRetVal.Get_String());
        Entry->Type = 0;
        FiltersArray.Add(Entry, sizeof(FILTER_ENTRY));
        mnu_idx++;
    }

    if(!Include_Menu)
    {
        MenuAddSeparator(Dest_Menu);
        for(i = 0; i <= 999; i++)
        {
            ProgRetVal = IniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), FiltersIniFile);
            if(ProgRetVal.Len() == 0) break;
            MenuAddString(Dest_Menu, ProgRetVal + "\tTo file", mnu_idx + MENU_FILTERS_IDBASE, TRUE);
            Entry->Name = _strdup(ProgRetVal.Get_String());
            Entry->Type = 1;
            FiltersArray.Add(Entry, sizeof(FILTER_ENTRY));
            mnu_idx++;
        }
    }
    if(Index != 0) SubMenusArray[Index] = Dest_Menu;
}
