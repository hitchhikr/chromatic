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
// CmdMenus.h: declaration of used variables / structures for CmdMenus.cpp
// -----------------------------------------------------------------------

#ifndef _CMDMENUS_H_
#define _CMDMENUS_H_

// -----------------------------------------------------------------------
// Constants
#define FILTER_OPEN_MODE 0
#define FILTER_INCLUDE_MODE 1

// -----------------------------------------------------------------------
// Variables
extern CStr MenusRetVal;
extern CStr SelToSave;

// -----------------------------------------------------------------------
// Functions declarations
HWND MCMD_New(void);
void MCMD_Clear(void);
void MCMD_Duplicate(void);
void MCMD_Reload(void);
void MCMD_OpenFile(void);
long CALLBACK EnumOpenFiles(char *FileToAdd, long UserValue);
void MCMD_OpenFileAsDB(void);
void OpenFileAsDB(void);
long CALLBACK EnumOpenAsDBFiles(char *FileToAdd, long UserValue);
void MCMD_OpenWorkSpace(void);
void MCMD_IncludeFile(void);
long CALLBACK EnumIncludeFiles(char *FileToAdd, long UserValue);
void MCMD_IncludeFilters(void);
void IncludeFileAsDB(void);
void MCMD_FTPManager(void);
void MCMD_SaveWorkSpace(void);
void MCMD_CloseFile(void);
void MCMD_CloseAll(void);
void MCMD_SaveFile(void);
void MCMD_SaveFileAs(void);
void MCMD_RegSnippet(void);
void MCMD_RegTemplate(void);
void MCMD_SaveAll(void);
void MCMD_PrintFile(void);
void MCMD_RunConsole(void);
void MCMD_ChangeConsole(void);
void MCMD_RunExtProg(void);
void MCMD_PurgeRunProgAll(void);
void MCMD_PurgeRecent(void);
void MCMD_PurgeRecentAll(void);
void MCMD_PurgeRecentPrj(void);
void MCMD_PurgeRecentPrjAll(void);
void MCMD_Favorites(void);
void MCMD_AddToFavorites(void);
void MCMD_SetWorkingDir(void);
void MCMD_Exit(void);
void MCMD_Undo(void);
void MCMD_Redo(void);
void MCMD_Cut(void);
void MCMD_Copy(void);
void MCMD_CopyFileNamePath(void);
void MCMD_CopyEntireText(void);
void MCMD_Paste(void);
void MCMD_PasteNewWindow(void);
void MCMD_DeleteSelection(void);
void MCMD_DeleteLine(void);
void MCMD_DeleteToStart(void);
void MCMD_DeleteToEnd(void);
void MCMD_DeleteParagraph(void);
void MCMD_SelectLine(void);
void MCMD_SelectAll(void);
void MCMD_SelectProc(void);
void MCMD_SelectProcCollapse(void);
void MCMD_UnCollapse(void);
void MCMD_Find(void);
void MCMD_FindNext(void);
void MCMD_FindPrevious(void);
void MCMD_FindSelection(void);
void MCMD_Replace(void);
void MCMD_ProcVarSearch(void);
void MCMD_SearchInFiles(void);
void MCMD_ToggleBookMark(void);
void MCMD_FirstBookMark(void);
void MCMD_PreviousBookMark(void);
void MCMD_NextBookMark(void);
void MCMD_LastBookMark(void);
void MCMD_ClearAllBookMark(void);
void MCMD_ToggleBreakpoint(void);
void MCMD_FirstBreakpoint(void);
void MCMD_PreviousBreakpoint(void);
void MCMD_NextBreakpoint(void);
void MCMD_LastBreakpoint(void);
void MCMD_ClearAllBreakpoint(void);
void MCMD_GotoTop(void);
void MCMD_GotoBottom(void);
void MCMD_GotoLine(void);
void MCMD_GotoMatchingBracket(void);
void MCMD_PrevWord(void);
void MCMD_NextWord(void);
void MCMD_PreviousProc(void);
void MCMD_NextProc(void);
void MCMD_PreviousParagraph(void);
void MCMD_NextParagraph(void);
void MCMD_VarDeclaration(void);
void MCMD_VarNextUse(void);
void MCMD_ProcProto(void);
void MCMD_ProcDeclare(void);
void MCMD_ProcNextUse(void);
void MCMD_BlockIndent(void);
void MCMD_BlockUnindent(void);
void MCMD_BlockUpperCase(void);
void MCMD_BlockLowerCase(void);
void MCMD_BlockToggleCase(void);
void MCMD_BlockComment(void);
void MCMD_BlockUnComment(void);
void MCMD_BlockTabify(void);
void MCMD_BlockUnTabify(void);
void MCMD_BlockSpacesToTabs(void);
void MCMD_Registers(void);
void MCMD_BlockCommentMultiLine(void);
void MCMD_CustomBlockComment(void);
void MCMD_BlockTrimSpaces(void);
void MCMD_BlockConvertToInclude(void);
void MCMD_BlockSave(void);
void MCMD_BlockPrint(void);
void MCMD_Statusbar(void);
void MCMD_ProjectDock(void);
void MCMD_WindowsDock(void);
void MCMD_OutputDock(void);
void MCMD_DebugDock(void);
void MCMD_RegistersDock(void);
void MCMD_FullScreen(void);
void SetFullScreenMode(void);
void MCMD_Properties(void);
void MCMD_UserMenusEd(void);
void MCMD_ToolbarsManager(void);
void MCMD_CreateProject(void);
void MCMD_OpenProject(void);
void MCMD_SaveProject(void);
void MCMD_AddInclude(void);
long CALLBACK EnumAddInclude(char *FileToAdd, long UserValue);
void MCMD_AddCurrentInclude(void);
void MCMD_AddLibrary(void);
long CALLBACK EnumAddLibrary(char *FileToAdd, long UserValue);
void MCMD_AddModule(void);
long CALLBACK EnumAddModule(char *FileToAdd, long UserValue);
void MCMD_AddCurrentModule(void);
void MCMD_AddObject(void);
long CALLBACK EnumAddObject(char *FileToAdd, long UserValue);
void MCMD_AddResource(void);
long CALLBACK EnumAddResource(char *FileToAdd, long UserValue);
void MCMD_AddCurrentRc(void);
void MCMD_AddText(void);
long CALLBACK EnumAddText(char *FileToAdd, long UserValue);
void MCMD_AddCurrentText(void);
void MCMD_AddIcon(void);
long CALLBACK EnumAddIcon(char *FileToAdd, long UserValue);
void MCMD_AddCursor(void);
long CALLBACK EnumAddCursor(char *FileToAdd, long UserValue);
void MCMD_AddBitmap(void);
long CALLBACK EnumAddBitmap(char *FileToAdd, long UserValue);
void MCMD_AddStrings(void);
long CALLBACK EnumAddStrings(char *FileToAdd, long UserValue);
void MCMD_AddAccelerators(void);
long CALLBACK EnumAddAccelerators(char *FileToAdd, long UserValue);
void MCMD_AddMenus(void);
long CALLBACK EnumAddMenus(char *FileToAdd, long UserValue);
void MCMD_AddDialog(void);
long CALLBACK EnumAddDialog(char *FileToAdd, long UserValue);
void MCMD_AddRawDatas(void);
void MCMD_SaveProjectAs(void);
void MCMD_ProjectProperties(void);
void MCMD_AssembleProject(void);
void MCMD_RunProject(void);
void MCMD_DebugProject(void);
void MCMD_AddInsManager(void);
void MCMD_ContextHelp(void);
void MCMD_MSDNHelp(void);
void MCMD_DDKHelp(void);
void MCMD_DirectXSDKHelp(void);
void MCMD_About(void);
void MCMD_UpDate(void);
void MCMD_BugReport(void);
void MCMD_NewFeature(void);
void MCMD_Split(void);
void MCMD_TileHorizontally(void);
void MCMD_TileVertically(void);
void MCMD_Cascade(void);
void MCMD_ArrangeIcons(void);
void MCMD_NextWindow(void);
void MCMD_PreviousWindow(void);

#endif
