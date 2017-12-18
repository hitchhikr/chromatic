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
// MDI_Form.cpp: MDI form handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Classes/WADock.h"
#include "Globals.h"
#include "ConstRes.h"
#include "CmdMenus.h"
#include "Statusbar.h"
#include "Menus.h"
#include "ConstRes.h"
#include "ConstMisc.h"
#include "Toolbars.h"
#include "Splitters.h"
#include "AddIns.h"
#include "MiscFunctions.h"
#include "Languages.h"
#include "ProjectsBuilder.h"
#include "Debugger.h"
#include "ConstCodeMax.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_Splash.h"
#include "Dlg_Args.h"
#include "Dlg_APIList.h"
#include "Dlg_Tips.h"
#include "Dlg_Filters.h"
#include "Dlg_API.h"
#include "Dlg_SelFiles.h"

// -----------------------------------------------------------------------
// Variables
MDIDAT hMDIform;
CStr MDIRetVal;
long TimerHandle;
long FreezeTimer;
CStr EmptyFile;
MDICREATESTRUCT CurMDICreate;
extern CDockingWindow *cDock;
HWND Wrap_Child;

// -----------------------------------------------------------------------
// Create the MDI window
void CreateMDIForm(void)
{
    CStr TopMostProp;
    WINDOWPLACEMENT Mdi_Datas;
    CStr strMDI_Datas;
    int Nbr_Params = 0;
    
    // Set main window on top or not
    TopMostProp = IniReadKey("Layout", "AlwaysOnTop", MainIniFile);

    memset(&Mdi_Datas, 0, sizeof(WINDOWPLACEMENT));
    Mdi_Datas.length = sizeof(WINDOWPLACEMENT);
    strMDI_Datas = IniReadKey("Layout", "MDI_Flags", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.flags = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_MinPosX", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.ptMinPosition.x = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_MinPosY", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.ptMinPosition.y = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_MaxPosX", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.ptMaxPosition.x = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_MaxPosY", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.ptMaxPosition.y = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_PosL", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.rcNormalPosition.left = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_PosT", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.rcNormalPosition.top = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_PosR", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.rcNormalPosition.right = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_PosB", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.rcNormalPosition.bottom = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    strMDI_Datas = IniReadKey("Layout", "MDI_Show", MainIniFile);
    if(strMDI_Datas.Len())
    {
        Mdi_Datas.showCmd = strMDI_Datas.Get_Long();
        Nbr_Params++;
    }
    if(Nbr_Params != 10)
    {
        memset(&Mdi_Datas, 0, sizeof(WINDOWPLACEMENT));
        Mdi_Datas.length = sizeof(WINDOWPLACEMENT);
        Mdi_Datas.showCmd = SW_SHOWMAXIMIZED;
        Mdi_Datas.rcNormalPosition.left = 0;
        Mdi_Datas.rcNormalPosition.top = 0;
        Mdi_Datas.rcNormalPosition.right = 800;
        Mdi_Datas.rcNormalPosition.bottom = 600;
    }
    hMDIform.hWnd = CreateMDIDialog(Mdi_Datas.rcNormalPosition.left, Mdi_Datas.rcNormalPosition.top, Mdi_Datas.rcNormalPosition.right - Mdi_Datas.rcNormalPosition.left, Mdi_Datas.rcNormalPosition.bottom - Mdi_Datas.rcNormalPosition.top, 0, 0, LoadIcon(ApphInstance, MAKEINTRESOURCE(APP_ICON)), Requesters, &MDIInitProc, CurrentMDIProc, 0, Mdi_Datas.showCmd);
    if(Nbr_Params == 10) SetWindowPlacement(hMDIform.hWnd, &Mdi_Datas);
    DragAcceptFiles(hMDIform.hClient, AcceptFiles);
    if(TopMostProp.Len() != 0) if(_strcmpi(TopMostProp.Get_String(), "1") == 0) ControlSetTopMost(hMDIform.hWnd);
    if(FullScreenMode == 1) SetFullScreenMode();
}

// -----------------------------------------------------------------------
// Initialize MDI window
void CALLBACK MDIInitProc(HWND hWnd)
{
    int i = 0;
    void (CALLBACK * LocInitMDIHook)(HWND hWnd);
    RECT rcDefault;

    hRebar = CreateRebar(0, 0, 0, 18, hWnd, 0, 0, CCS_TOP | RBS_BANDBORDERS | WS_BORDER);
    InitReBarContent(hWnd);

    hStatusBar = CreateStatusBar("", STATUSBAR_PARTNORMAL, hWnd, 6, &StatusHook, WS_BORDER);
    CreateMenuBar(hWnd);
    hMDIform.hClient = CreateClient(hWnd, hWindowMenu, LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + ICON_NEW)));
    ControlHookWin(hMDIform.hClient, &ClientProc);

    // We need to update it here bcoz some addins might need it
    hMDIform.hWnd = hWnd;

    // Create docking context
    rcDefault.left = 0;
    rcDefault.top = 0;
    rcDefault.right = ControlWidth(hMDIform.hWnd);
    rcDefault.bottom = ControlHeight(hMDIform.hWnd);
    hMDIform.hDock = cDock->Create(hWnd, rcDefault);
    cDock->SetClient(hMDIform.hClient);

    CreateDockingProject(hWnd);
    CreateDockingWindows(hWnd);
    CreateDockingOutput(hWnd);
    SetStatusBarParts();
    
    // (Don't display it again, only hide it if necessary)
    // In all case update menus states
    if(!ShowProjetDockWin) SetProjectDockState(ShowProjetDockWin);
    else SetProjectDockMenuState(ShowProjetDockWin);
    if(!ShowWindowsDockWin) SetWindowsDockState(ShowWindowsDockWin);
    else SetWindowsDockMenuState(ShowWindowsDockWin);
    if(!ShowOutputDockWin) SetOutputDockState(ShowOutputDockWin);
    else SetOutputDockMenuState(ShowOutputDockWin);

    ShowHideStatusBar(ShowStatusbar);
    CreateTimer(hWnd);
    CurFontHandle = GDIObtainFont(CurFontName, CurFontSize, hMDIform.hWnd, 0, 0);

    // All context is set
    // We can run AddIns system now
    FirstStart = TRUE;
    FrmSplashSetState("Starting AddIns...");
    SetAddInsRunningState();
    // Execute possible hooks
    for(i = 0; i < MDICreateHooks.Amount(); i++)
    {
        if(MDICreateHooks.Get(i)->Content != 0)
        {
            LocInitMDIHook = (void (CALLBACK *) (HWND hWnd)) MDICreateHooks.Get(i)->Content;
            LocInitMDIHook(hWnd);
        }
    }
    // FALSE for the rest of the program lifetime
    FirstStart = FALSE;
}

// -----------------------------------------------------------------------
// MDI Winproc
LRESULT CALLBACK MDIProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CStr DropedFileName;
    long ToolBarCommand = 0;
    long AddInRun = 0;
    CStr SysTrayPref;
    long MinY = 0;
    SIZE MySize;
    long NewhHeight = 0;
    long NHTop = 0;
    long NHHeight = 0;
    int i = 0;
    CStr LdFile;
    CStr OpFilters;
    CStr CmdToOpen;
    CStr TmpRecentFile;
    long wParamSelect = 0;
    CStr PassedCmd;
    long PassedFileLen = 0;
    unsigned long PassedFileRead = 0;
    HANDLE PassedFileHandle;
    long TbHotId = 0;
    long TbDroppedId = 0;
    long TbTooltipId = 0;
    CStr SCStr;
    long PagerID = 0;
    CStr NewSnippetVal;
    CStr OpenedLanguage;
    HWND SnippetForm = 0;
    POINT TrayPoint;
    HMENU hTrayMenu = 0;
    long HaveToDie = 0;
    CStr BufString;
    DLGPROC LocMDIHook;
    int SkipPrjLoadRecent;
    POINT New_Mouse_Coords;
    HWND Next_Child;

    // Execute possible hooks
    for(i = 0; i < MDIHooks.Amount(); i++)
    {
        if(MDIHooks.Get(i)->Content != 0)
        {
            LocMDIHook = MDIHooks.Get(i)->Content;
            if(LocMDIHook(hWnd, uMsg, wParam, lParam) == HOOK_DIE)
            {
                HaveToDie = HaveToDie | HOOK_DIE;
            }
        }
    }
    if((long) HaveToDie == HOOK_DIE) return(0);
    
    // Another instance is calling
    if (uMsg == BroadCastMessage)
    {
        // Free the sending thread
        ReplyMessage(0);
        // Retrieve the file
        PassedFileLen = FileGetSize(Dirs[DIR_WIN] + (CStr) "\\TempOp.CF");
        if(PassedFileLen != 0)
        {
            PassedCmd = PassedCmd.String(PassedFileLen, 1);
            PassedFileHandle = FileOpenR(Dirs[DIR_WIN] + (CStr) "\\TempOp.CF");
            if(PassedFileHandle != INVALID_HANDLE_VALUE)
            {
                ReadFile(PassedFileHandle, PassedCmd.Get_String(), PassedFileLen, &PassedFileRead, NULL);
                FileClose(PassedFileHandle);
            }
            if(PassedCmd.Len() != 0) OpenUnknownFile(PassedCmd, TRUE);
        }
        BufString = Dirs[DIR_WIN] + (CStr) "\\TempOp.CF";
        DeleteFile(BufString.Get_String());
        SetForegroundWindow(hMDIform.hWnd);
        return(0);
    }
    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            TreeViewSetBackColor(hTreeView, GetSysColor(COLOR_WINDOW));
            RebarSetBackColor(hRebar, GetSysColor(COLOR_BTNFACE));
            for(i = 0; i < PagersHwnd.Amount(); i++)
            {
                if(PagersHwnd.Get(i)->Content != 0)
                {
                    PagerSetBackColor(PagersHwnd.Get(i)->Content,
                                        GetSysColor(COLOR_BTNFACE));
                }
            }
            PagerSetBackColor(hProjectPager, GetSysColor(COLOR_BTNFACE));
            PagerSetBackColor(hStatusPager, GetSysColor(COLOR_BTNFACE));
            ListViewSetBackColor(hWindowsListView, GetSysColor(COLOR_WINDOW));
//          ResizeMDIform();
            DockingBoxRepaint(hProjectContainer);
            DockingBoxRepaint(hWindowsContainer);
            DockingBoxRepaint(hOutputContainer);
            DockingBoxRepaint(hDebugContainer);
            DockingBoxRepaint(hRegistersContainer);
//          ControlRepaint(hMDIform.hDock);
//          if(ControlIsVisible(hProjectContainer)) ControlRepaint(hProjectContainer);
//          if(ControlIsVisible(hWindowsContainer)) ControlRepaint(hWindowsContainer);
//          if(ControlIsVisible(hOutputContainer)) ControlRepaint(hOutputContainer);
//          if(ControlIsVisible(hDebugContainer)) ControlRepaint(hDebugContainer);
//          if(ControlIsVisible(hRegistersContainer)) ControlRepaint(hRegistersContainer);

            break;

        case WM_ACTIVATEAPP:
            if(QuitNotif == 0) if(wParam != 0) if(DetectChange == 1) SearchModified(hMDIform.hClient);           
            Check_Update_Language();
            if(APIListMode == 1) ControlClose(FRMAPIListhWnd);
            return(0);

        // Handle search wrapping

        case WM_FINDMARKALL_WRAP:
            if(Wrap_Child) break;
            Wrap_Child = (HWND) lParam;
            Next_Child = GetNextChildInList(Wrap_Child);
            while(Wrap_Child != Next_Child)
            {
                // Mark in all childs
                ChildStruct = LoadStructure(Next_Child);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, 0);
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDMARKALL, 0);
                }
                Next_Child = GetNextChildInList(Next_Child);
            }
            Wrap_Child = 0;
            break;
 
        case WM_FINDREPLACEALL_WRAP:
            if(Wrap_Child) break;
            Wrap_Child = (HWND) lParam;
            Next_Child = GetNextChildInList(Wrap_Child);
            while(Wrap_Child != Next_Child)
            {
                // Mark in all childs
                ChildStruct = LoadStructure(Next_Child);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, 0);
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_REPLACEALLINBUFFER, 0);
                }
                Next_Child = GetNextChildInList(Next_Child);
            }
            Wrap_Child = 0;
            break;
        
        case WM_FOUND_TEXT:
            SendMessage(hMDIform.hClient, WM_MDIACTIVATE, lParam, 0);
            return(0);
        case WM_FIND_FORWARD_WRAP:
            if(Wrap_Child) break;
            Wrap_Child = (HWND) lParam;
            Next_Child = GetNextChildInList(Wrap_Child);
            while(Wrap_Child != Next_Child)
            {
                // Mark in all childs
                ChildStruct = LoadStructure(Next_Child);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, 0);
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_HOME, 0);
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDNEXT, 0);
                }
                Next_Child = GetNextChildInList(Next_Child);
            }
            Wrap_Child = 0;
            break;
        case WM_FIND_BACKWARD_WRAP:
            if(Wrap_Child) break;
            Wrap_Child = (HWND) lParam;
            Next_Child = GetPreviousChildInList(Wrap_Child);
            while(Wrap_Child != Next_Child)
            {
                // Mark in all childs
                ChildStruct = LoadStructure(Next_Child);
                if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
                {
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, CM_GetLineCount(ChildStruct->hChildCodeMax) - 1);
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_END, 0);
                    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDPREV, 0);
                }
                Next_Child = GetPreviousChildInList(Next_Child);
            }
            Wrap_Child = 0;
            break;
        
// -------------------------------------------------------------------------------
// Menu comments
// -------------------------------------------------------------------------------
        case WM_MENUSELECT:
            if(((wParam >> 16) & MF_SYSMENU) == 0)
            {
                if((wParam & 0x8000) == 0)
                {
                    if(CursorDisabled) DisplayCursor();
                    wParamSelect = (wParam & 0xFFFF);
                    if(wParamSelect < MENU_FILE_IDBASE)
                    {
                        if(wParamSelect == MENU_RUNPROGCLEAR_IDBASE)
                        {
                            WriteComment("Erase run programs list");
                            return(0);
                        }
                        if(wParamSelect == MENU_RECENTFILESPURGEALL_IDBASE)
                        {
                            WriteComment("Erase recent files list");
                            return(0);
                        }
                        if(wParamSelect == MENU_RECENTPROJECTSPURGEALL_IDBASE)
                        {
                            WriteComment("Erase recent projects list");
                            return(0);
                        }
                        if(wParamSelect == MENU_RECENTFILESPURGE_IDBASE)
                        {
                            WriteComment("Check and remove obsoletes entries from recent files list");
                            return(0);
                        }
                        if(wParamSelect == MENU_RECENTPROJECTSPURGE_IDBASE)
                        {
                            WriteComment("Check and remove obsoletes entries from recent projects list");
                            return(0);
                        }
                        WriteComment("");
                    }
                    else if(wParamSelect < MENU_EDIT_IDBASE)
                    {
                        WriteComment((char *) MenuFileComments[wParamSelect - MENU_FILE_IDBASE]);
                        return(0);
                    }
                    else if(wParamSelect < MENU_VIEW_IDBASE)
                    {
                        WriteComment((char *) MenuEditComments[wParamSelect - MENU_EDIT_IDBASE]);
                        return(0);
                    }
                    else if(wParamSelect < MENU_PROJECT_IDBASE)
                    {
                        WriteComment((char *) MenuViewComments[wParamSelect - MENU_VIEW_IDBASE]);
                        return(0);
                    }
                    else if(wParamSelect < MENU_WINDOW_IDBASE)
                    {
                        WriteComment((char *) MenuProjectComments[wParamSelect - MENU_PROJECT_IDBASE]);
                        return(0);
                    }
                    else if(wParamSelect < MENU_USER_IDBASE)
                    {
                        WriteComment((char *) MenuWindowComments[wParamSelect - MENU_WINDOW_IDBASE]);
                        return(0);
                    }
                    else if(wParamSelect < MENU_MAXIMA_IDBASE)
                    {
                        WriteComment(MenusCmdsComments.Get((wParamSelect - MENU_USER_IDBASE) - 1)->Content);
                        return(0);
                    }
                    else if(wParamSelect < MENU_CODEMAXIMA_IDBASE)
                    {
                        WriteComment(TemplatesArray.Get(wParamSelect - MENU_TEMPLATES_IDBASE)->Content);
                        return(0);
                    }
                    else if(wParamSelect < MENU_ADDINS_IDBASE)
                    {
                        WriteComment("");
                        return(0);
                    }
                    else if(wParamSelect == MENU_ADDINS_IDBASE)
                    {
                        WriteComment((char *) MenuAddInComments[wParamSelect - MENU_ADDINS_IDBASE]);
                        return(0);
                    }
                    else if(wParamSelect >= MENU_CODEMAXIMA_IDBASE)
                    {
                        WriteComment("");
                        return(0);
                    }
                }
            }
            break;

        case WM_EXITMENULOOP:
            WriteComment("");
            return(0);

        case WM_COMMAND:
            if(CursorDisabled) DisplayCursor();
            switch(wParam & 0xFFFF)
            {

// -------------------------------------------------------------------------------
// Systray menu
// -------------------------------------------------------------------------------
                case MENU_SYSTRAY_RESTORE:
                    SysTrayMode = 0;
                    RestoreMDI(hWnd);
                    return(0);
                
                case MENU_SYSTRAY_EXIT:
                    SysTrayMode = 0;
                    RestoreMDI(hWnd);
                    ControlClose(hWnd);
                    return(0);

// -------------------------------------------------------------------------------
// File Menu
// -------------------------------------------------------------------------------
                case MENU_FILE_IDBASE + MENU_FILE_NEW_ID:
                    MCMD_New();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_CLEAR_ID:
                    MCMD_Clear();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_DUPLICATEFILE_ID:
                    MCMD_Duplicate();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_RELOADFILE_ID:
                    MCMD_Reload();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_OPENFILE_ID:
                    MCMD_OpenFile();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_INCLUDEFILE_ID:
                    MCMD_IncludeFile();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_INCLUDEFILTERS_ID:
                    MCMD_IncludeFilters();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_FILTERS_ID:
                    MCMD_OpenFileAsDB();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_OPENWORKSPACE_ID:
                    MCMD_OpenWorkSpace();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_FTPMANAGER_ID:
                    MCMD_FTPManager();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_CLOSE_ID:
                    MCMD_CloseFile();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_CLOSEALL_ID:
                    MCMD_CloseAll();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_SAVE_ID:
                    MCMD_SaveFile();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_SAVEAS_ID:
                    MCMD_SaveFileAs();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_SAVEWORKSPACE_ID:
                    MCMD_SaveWorkSpace();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_REGISTERASSNIPPET_ID:
                    MCMD_RegSnippet();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_REGISTERASTEMPLATE_ID:
                    MCMD_RegTemplate();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_SAVEALL_ID:
                    MCMD_SaveAll();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_PRINTFILE_ID:
                    MCMD_PrintFile();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_CONSOLE_RUN_ID:
                    MCMD_RunConsole();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_CONSOLE_CHANGE_ID:
                    MCMD_ChangeConsole();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_RUNEXTERNALPROGRAM_ID:
                    MCMD_RunExtProg();
                    return(0);

                case MENU_RUNPROGCLEAR_IDBASE:
                    MCMD_PurgeRunProgAll();
                    return(0);
                
                case MENU_FILE_IDBASE + MENU_FILE_FAVORITES_ID:
                    MCMD_Favorites();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_ADD_FAVORITE_ID:
                    MCMD_AddToFavorites();
                    return(0);

                case MENU_RECENTFILESPURGE_IDBASE:
                    MCMD_PurgeRecent();
                    return(0);
                
                case MENU_RECENTFILESPURGEALL_IDBASE:
                    MCMD_PurgeRecentAll();
                    return(0);
                
                case MENU_RECENTPROJECTSPURGE_IDBASE:
                    MCMD_PurgeRecentPrj();
                    return(0);
                
                case MENU_RECENTPROJECTSPURGEALL_IDBASE:
                    MCMD_PurgeRecentPrjAll();
                    return(0);
                
                case MENU_FILE_IDBASE + MENU_FILE_SETWORKINGDIRECTORY_ID:
                    MCMD_SetWorkingDir();
                    return(0);

                case MENU_FILE_IDBASE + MENU_FILE_EXIT_ID:
                    MCMD_Exit();
                    return(0);

// -------------------------------------------------------------------------------
// Edit Menu
// -------------------------------------------------------------------------------
                case MENU_EDIT_IDBASE + MENU_EDIT_UNDO_ID:
                    MCMD_Undo();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_REDO_ID:
                    MCMD_Redo();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_CUT_ID:
                    MCMD_Cut();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_COPY_ID:
                    MCMD_Copy();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_COPYFILEPATHNAME_ID:
                    MCMD_CopyFileNamePath();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_COPYENTIRETEXT_ID:
                    MCMD_CopyEntireText();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_PASTE_ID:
                    MCMD_Paste();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_PASTE_NEWWINDOW_ID:
                    MCMD_PasteNewWindow();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_DELETE_DELETESELECTION_ID:
                    MCMD_DeleteSelection();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_DELETE_DELETELINE_ID:
                    MCMD_DeleteLine();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID:
                    MCMD_DeleteToStart();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_DELETE_DELETETOENDOFLINE_ID:
                    MCMD_DeleteToEnd();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_DELETE_DELETEPARAGRAPH_ID:
                    MCMD_DeleteParagraph();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_SELECTLINE_ID:
                    MCMD_SelectLine();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_SELECTALL_ID:
                    MCMD_SelectAll();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_SELECTPROCEDURE_ID:
                    MCMD_SelectProc();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_SELECTPROCANDCOLLAPSE_ID:
                    MCMD_SelectProcCollapse();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_UNCOLLAPSEPROCEDURE_ID:
                    MCMD_UnCollapse();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_FIND_ID:
                    MCMD_Find();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_FINDNEXT_ID:
                    MCMD_FindNext();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_FINDPREVIOUS_ID:
                    MCMD_FindPrevious();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_FINDSELECTION_ID:
                    MCMD_FindSelection();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_REPLACE_ID:
                    MCMD_Replace();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_PROCVARIABLESSEARCH_ID:
                    MCMD_ProcVarSearch();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_SEARCHINFILES_ID:
                    MCMD_SearchInFiles();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BOOKMARKS_TOGGLE_ID:
                    MCMD_ToggleBookMark();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BOOKMARKS_FIRST_ID:
                    MCMD_FirstBookMark();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BOOKMARKS_PREVIOUS_ID:
                    MCMD_PreviousBookMark();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BOOKMARKS_NEXT_ID:
                    MCMD_NextBookMark();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BOOKMARKS_LAST_ID:
                    MCMD_LastBookMark();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BOOKMARKS_CLEARALL_ID:
                    MCMD_ClearAllBookMark();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BREAKPOINTS_TOGGLE_ID:
                    MCMD_ToggleBreakpoint();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BREAKPOINTS_FIRST_ID:
                    MCMD_FirstBreakpoint();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BREAKPOINTS_PREVIOUS_ID:
                    MCMD_PreviousBreakpoint();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BREAKPOINTS_NEXT_ID:
                    MCMD_NextBreakpoint();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BREAKPOINTS_LAST_ID:
                    MCMD_LastBreakpoint();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BREAKPOINTS_CLEARALL_ID:
                    MCMD_ClearAllBreakpoint();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_TOP_ID:
                    MCMD_GotoTop();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_BOTTOM_ID:
                    MCMD_GotoBottom();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_LINE_ID:
                    MCMD_GotoLine();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_MATCHINGBRACKET_ID:
                    MCMD_GotoMatchingBracket();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_PREVIOUSWORD_ID:
                    MCMD_PrevWord();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_NEXTWORD_ID:
                    MCMD_NextWord();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID:
                    MCMD_PreviousProc();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_NEXTPROCEDURE_ID:
                    MCMD_NextProc();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID:
                    MCMD_PreviousParagraph();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_NEXTPARAGRAPH_ID:
                    MCMD_NextParagraph();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_VARIABLEDECLARATION_ID:
                    MCMD_VarDeclaration();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_VARIABLENEXTUSE_ID:
                    MCMD_VarNextUse();
                    return(0);
                
                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID:
                    MCMD_ProcProto();
                    return(0);
                
                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID:
                    MCMD_ProcDeclare();
                    return(0);
                
                case MENU_EDIT_IDBASE + MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID:
                    MCMD_ProcNextUse();
                    return(0);
                
                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_INDENT_ID:
                    MCMD_BlockIndent();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_UNINDENT_ID:
                    MCMD_BlockUnindent();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_UPPERCASE_ID:
                    MCMD_BlockUpperCase();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_LOWERCASE_ID:
                    MCMD_BlockLowerCase();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_TOGGLECASE_ID:
                    MCMD_BlockToggleCase();
                    return(0);
                
                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_COMMENT_ID:
                    MCMD_BlockComment();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_UNCOMMENT_ID:
                    MCMD_BlockUnComment();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_TABIFY_ID:
                    MCMD_BlockTabify();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_UNTABIFY_ID:
                    MCMD_BlockUnTabify();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_SPACESTOTABS_ID:
                    MCMD_BlockSpacesToTabs();
                    return(0);
                
                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_REGISTERS_ID:
                    MCMD_Registers();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_BLKCOMMENT_ID:
                    MCMD_BlockCommentMultiLine();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID:
                    MCMD_CustomBlockComment();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_TRIMSPACES_ID:
                    MCMD_BlockTrimSpaces();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID:
                    MCMD_BlockConvertToInclude();
                    return(0);
                
                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_SAVEAS_ID:
                    MCMD_BlockSave();
                    return(0);

                case MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_PRINT_ID:
                    MCMD_BlockPrint();
                    return(0);

// -------------------------------------------------------------------------------
// View Menu
// -------------------------------------------------------------------------------
                case MENU_VIEW_IDBASE + MENU_VIEW_STATUSBAR_ID:
                    MCMD_Statusbar();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_PROJECTDOCK_ID:
                    MCMD_ProjectDock();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_WINDOWSDOCK_ID:
                    MCMD_WindowsDock();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_OUTPUTDOCK_ID:
                    MCMD_OutputDock();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_DEBUGDOCK_ID:
                    MCMD_DebugDock();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_REGISTERSDOCK_ID:
                    MCMD_RegistersDock();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_FULLSCREEN_ID:
                    MCMD_FullScreen();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_PROPERTIES_ID:
                    MCMD_Properties();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_USERMENUSEDITOR_ID:
                    MCMD_UserMenusEd();
                    return(0);

                case MENU_VIEW_IDBASE + MENU_VIEW_USERTOOLBARSEDITOR_ID:
                    MCMD_ToolbarsManager();
                    return(0);

// -------------------------------------------------------------------------------
// Project Menu
// -------------------------------------------------------------------------------
                case MENU_PROJECT_IDBASE + MENU_PROJECT_CREATENEWPROJECT_ID:
                    MCMD_CreateProject();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_OPENPROJECT_ID:
                    MCMD_OpenProject();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_SAVEPROJECT_ID:
                    MCMD_SaveProject();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_SAVEPROJECTAS_ID:
                    MCMD_SaveProjectAs();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDINCLUDE_ID:
                    MCMD_AddInclude();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDLIBRARY_ID:
                    MCMD_AddLibrary();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDMODULE_ID:
                    MCMD_AddModule();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDOBJECT_ID:
                    MCMD_AddObject();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDRESOURCE_ID:
                    MCMD_AddResource();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDTEXT_ID:
                    MCMD_AddText();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDICON_ID:
                    MCMD_AddIcon();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDCURSOR_ID:
                    MCMD_AddCursor();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDBITMAP_ID:
                    MCMD_AddBitmap();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDSTRINGS_ID:
                    MCMD_AddStrings();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDACCELERATORS_ID:
                    MCMD_AddAccelerators();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDMENUS_ID:
                    MCMD_AddMenus();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDDIALOG_ID:
                    MCMD_AddDialog();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_ADDRAWDATA_ID:
                    MCMD_AddRawDatas();
                    return(0);
                
                case MENU_PROJECT_IDBASE + MENU_PROJECT_PROJECTPROPERTIES_ID:
                    MCMD_ProjectProperties();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_ASSEMBLEPROJECT_ID:
                    MCMD_AssembleProject();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_RUNPROJECT_ID:
                    MCMD_RunProject();
                    return(0);

                case MENU_PROJECT_IDBASE + MENU_PROJECT_DEBUGPROJECT_ID:
                    MCMD_DebugProject();
                    return(0);

// -------------------------------------------------------------------------------
// AddIns Menu
// -------------------------------------------------------------------------------
                case MENU_ADDINS_IDBASE + MENU_ADDINS_MANAGER_ID:
                    MCMD_AddInsManager();
                    return(0);

// -------------------------------------------------------------------------------
// Window Menu
// -------------------------------------------------------------------------------
                case MENU_WINDOW_IDBASE + MENU_WINDOW_CONTEXTHELP_ID:
                    MCMD_ContextHelp();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_MSDNHELP_ID:
                    MCMD_MSDNHelp();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_DDKHELP_ID:
                    MCMD_DDKHelp();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_DIRECTXSDKHELP_ID:
                    MCMD_DirectXSDKHelp();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_TIPOFTHEDAY_ID:
                    DisplayTip(hMDIform.hWnd);
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_ABOUT_ID:
                    MCMD_About();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_SPLIT_ID:
                    MCMD_Split();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_TILEHORIZONTALLY_ID:
                    MCMD_TileHorizontally();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_TILEVERTICALLY_ID:
                    MCMD_TileVertically();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_CASCADE_ID:
                    MCMD_Cascade();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_ARRANGEICONS_ID:
                    MCMD_ArrangeIcons();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_NEXTWINDOW_ID:
                    MCMD_NextWindow();
                    return(0);

                case MENU_WINDOW_IDBASE + MENU_WINDOW_PREVIOUSWINDOW_ID:
                    MCMD_PreviousWindow();
                    return(0);
            }
// -------------------------------------------------------------------------------
// User Menus
// -------------------------------------------------------------------------------
            if((wParam & 0xFFFF) >= MENU_USER_IDBASE)
            {
                if((wParam & 0xFFFF) < (MENU_USER_IDBASE + 1000))
                {
                    FRMArgsEntered = 0;
                    RunUserCommand(wParam & 0xFFFF);
                    return(0);
                }
                else
                {

// -------------------------------------------------------------------------------
// Templates menu from File/New
// -------------------------------------------------------------------------------
                    if((wParam & 0xFFFF) >= MENU_TEMPLATES_IDBASE)
                    {
                        if((wParam & 0xFFFF) < (MENU_TEMPLATES_IDBASE + 1000))
                        {
                            NewSnippetVal = TemplatesArray.Get((wParam & 0xFFFF) - MENU_TEMPLATES_IDBASE)->Content;
                            if(NewSnippetVal.Len() != 0)
                            {
                                NewSnippetVal = ChangeRelativePaths(NewSnippetVal);
                                // Get snippet language
                                OpenedLanguage = GetLanguageToOpen(NewSnippetVal);
                                if(OpenedLanguage.Len() == 0)
                                {
                                    // Can't guess: get default language
                                    OpenedLanguage = IniReadKey("RefLanguages", "DefLang", LanguagesIniFile);
                                    if(OpenedLanguage.Len() == 0) OpenedLanguage = "X86 Assembler";
                                }
                                StoreLanguageToOpen(OpenedLanguage);
                                SnippetForm = CreateNewFile("<Untitled document " + (CStr) (NbForms + 1) + (CStr) ">");
                                InsertSnippetCode(SnippetForm, NewSnippetVal);
                            }
                            return(0);
                        }
                        else
                        {
                            if((wParam & 0xFFFF) > (MENU_ADDINS_IDBASE))
                            {
                                if((wParam & 0xFFFF) < (MENU_ADDINS_IDBASE + 1000))
                                {
                                    // Launch an AddIn
                                    AddInRun = ((wParam & 0xFFFF) - MENU_ADDINS_IDBASE) - 1;
                                    if(RunningAddIns.Amount() >= AddInRun)
                                    {
                                        // Already running or not ?
                                        if(RunningAddIns.Get(AddInRun)->Content == 0)
                                        {
                                            if(RunAddIn(AddInRun) == ADDIN_PERSISTANT) goto ForcePersistantMenu;
                                        }
                                        else
                                        {
ForcePersistantMenu:                        // Just trigger the menu routine if the addin was already running
                                            if(AddInsMenu.Get(AddInRun)->Content != 0)
                                            {
                                                if(JumpToAddrArg(AddInsMenu.Get(AddInRun)->Content, (long) &AddInLib) == ADDIN_DIE)
                                                {
                                                    if(AddInsUnLoad.Get(AddInRun)->Content != 0)
                                                    {
                                                        JumpToAddr(AddInsUnLoad.Get(AddInRun)->Content);
                                                    }
                                                    // Store running state
                                                    RunningAddIns.Set(AddInRun, 0);
                                                    IniWriteKey("AddIns", "Running" + (CStr) StringNumberComplement(AddInRun, 3).Get_String(), RunningAddIns.Get(AddInRun)->Content, MainIniFile);
                                                }
                                            }
                                        }
                                        return(0);
                                    }
                                }
                            }
                        }
                    }
                }

// -------------------------------------------------------------------------------
// Recent files
// -------------------------------------------------------------------------------
            } else if((wParam & 0xFFFF) >= MENU_RECENTFILES_IDBASE)
            {
                if((wParam & 0xFFFF) < MENU_RECENTPROJECTS_IDBASE)
                {
                    // Recent files
                    if(SysTrayMode == 1) RestoreMDI(hWnd);
                    TmpRecentFile = Recents.Get((wParam & 0xFFFF) - MENU_RECENTFILES_IDBASE)->Content;
                    if(FileExist(TmpRecentFile) == 0)
                    {
                        MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) TmpRecentFile + (CStr) "' not found.", MB_ERROR, Requesters);
                        AddRecentFile(TmpRecentFile, 0, 0);
                        return(0);
                    }
                    if(CheckWorkSpaceFile(TmpRecentFile) == 0)
                    {
                        // Open it as normal file
                        OpenFileNorm(TmpRecentFile, 0, 0, 0, 0);
                        // Store this path as last used
                        LastLoadDir = FileGetDirectory(TmpRecentFile);
                    }
                    else
                    {
                        // Open it as workspace file
                        OpenWorkSpc(TmpRecentFile);
                    }
                    return(0);
                }
                else
                {
                    if((wParam & 0xFFFF) < MENU_SNIPPET_IDBASE)
                    {
                        if((wParam & 0xFFFF) < MENU_RECENTAPPLICATIONS_IDBASE)
                        {
                            // Recent projects
                            TmpRecentFile = RecentsPrj.Get((wParam & 0xFFFF) - MENU_RECENTPROJECTS_IDBASE)->Content;
                            if(FileExist(TmpRecentFile) == 0)
                            {
                                MiscMsgBox(hMDIform.hWnd, "Project file not found.", MB_ERROR, Requesters);
                                AddRecentPrj(TmpRecentFile, 0, 0);
                                return(0);
                            }
                            if(CheckProjectFile(TmpRecentFile) == 0)
                            {
                                BufString = "Not a " + (CStr) AppTitle + (CStr) " project file.";
                                MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
                                AddRecentPrj(TmpRecentFile, 0, 0);
                                return(0);
                            }
                            else
                            {
                                if(SysTrayMode == 1) RestoreMDI(hWnd);
                                SkipPrjLoadRecent = RealOpenPrj(TmpRecentFile);
                                if(SkipPrjLoadRecent != -1) AddRecentPrj(TmpRecentFile, SkipPrjLoadRecent, 0);
                                return(0);
                            }
                        }
                        else
                        {
                            if((wParam & 0xFFFF) < MENU_FAVORITESFILES_IDBASE)
                            {
                                // Recent programs
                                TmpRecentFile = RunProgArray.Get((wParam & 0xFFFF) - MENU_RECENTAPPLICATIONS_IDBASE)->Content;
                                RunExtProg(TmpRecentFile);
                            }
                            else
                            {
                                if((wParam & 0xFFFF) < MENU_FILTERS_IDBASE)
                                {
                                    // Favorites files
                                    TmpRecentFile = FavoritesArray.Get((wParam & 0xFFFF) - MENU_FAVORITESFILES_IDBASE)->Content;
                                    OpenUnknownFile(TmpRecentFile, TRUE);
                                }
                                else
                                {
                                    if((wParam & 0xFFFF) < MENU_FILTERS_INCLUDE_IDBASE)
                                    {
                                        // Filters: open mode
                                        Run_Filter_Profile((wParam & 0xFFFF) - MENU_FILTERS_IDBASE, FILTER_OPEN_MODE);
                                    }
                                    else
                                    {
                                        // Filters: include mode
                                        Run_Filter_Profile((wParam & 0xFFFF) - MENU_FILTERS_INCLUDE_IDBASE, FILTER_INCLUDE_MODE);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                if(lParam != 0)
                {
                    // Compare the command with all registered toolbars
                    for(i = 0; i < ToolbarsHwnd.Amount(); i++)
                    {
                        if(ToolbarsHwnd.Get(i)->Content != 0)
                        {
                            if(lParam == (long) ToolbarsHwnd.Get(i)->Content)
                            {
                                if((long) (wParam & 0xFFFF) > (long) ToolBarCommands.Amount()) break;
                                ToolBarCommand = ToolBarCommands.Get((wParam & 0xFFFF))->Content;
                                if(ToolBarCommand != -1) ControlSendMessage(hWnd, ToolBarCommand, 0);
                                break;
                            }
                        }
                    }
                    return(0);
                }
            }
            break;

// -------------------------------------------------------------------------------
// Notifications
// -------------------------------------------------------------------------------
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case RBN_HEIGHTCHANGE:
                    ResizeMDIform();
                    return(0);
                // Resize Pagers
                case PGN_CALCSIZE:
                    ToolBarGetXYSize(PagerGetNotifiedChild(lParam), &MySize);
                    PagerID = ControlGetNotifiedID(lParam);
                    if(PagersSize.Amount() > PagerID) MySize.cx = PagersSize.Get(PagerID)->Content;
                    PagerDisplaySetSize((LPNMPGCALCSIZE) lParam, &MySize);
                    return(0);
                case TTN_NEEDTEXT:
                    TbTooltipId = ControlGetNotifiedID(lParam);
                    ToolBarDisplayToolTip(ToolBarToolTips.Get(TbTooltipId)->Content, lParam);
                    return(0);
                case TBN_DROPDOWN:
                    TbDroppedId = ToolBarGetNotifiedDropDownItem(lParam);
                    if(ToolBarArray.Get(TbDroppedId)->Content != 0)
                    {
                        if(ToolBarMenus.Get(TbDroppedId)->Content == -1) goto NoDropDown;
                        else if(ToolBarMenus.Get(TbDroppedId)->Content > MAXSUBMENUS) goto NoDropDown;
                        else ToolBarDisplayPopupMenu(ToolBarArray.Get(TbDroppedId)->Content, TbDroppedId, SubMenusArray[ToolBarMenus.Get(TbDroppedId)->Content], hWnd);
                    }
NoDropDown:         return(0);
                case TBN_HOTITEMCHANGE:
                    TbHotId = ToolBarGetNotifiedHotItem(lParam);
                    if(TbHotId == -1) WriteComment("");
                    else WriteComment(ToolBarComments.Get(TbHotId)->Content);
                    return(0);
            }
            break;

// -------------------------------------------------------------------------------
// Resize
// -------------------------------------------------------------------------------
        case WM_MOVE:
            if(APITTip == 1) if(APIMode == 1) ControlClose(FRMAPIhwnd);
            if(APIListMode == 1) ControlClose(FRMAPIListhWnd);
            break;
        case WM_SIZE:
            if(wParam != SIZE_MINIMIZED)
            {
                SendMessage(hStatusBar, uMsg, wParam, lParam);
                SetStatusBarParts();
                RebarResize(hRebar);
                ResizeMDIform();
                if(NbForms != 0) RefreshStatusBar(CurrentForm);
                else ClearStatusBarParts();
            }
            else
            {
                // Minimize to SysTray
                SysTrayPref = IniReadKey("Layout", "UseSysTray", MainIniFile);
                if(SysTrayPref.Len() != 0) if(_strcmpi(SysTrayPref.Get_String(), "1") == 0) ControlSetSysToTray(ApphInstance, hWnd, SYSTRAY_MSG, APP_ICON, Requesters);
            }
            return(DefWindowProc(hWnd, uMsg, wParam, lParam));

        case WM_NCMOUSEMOVE:
            if(CursorDisabled) DisplayCursor();
            break;
        case WM_MOUSEMOVE:
            if(CursorDisabled)
            {
                GetCursorPos(&New_Mouse_Coords);
                if(Mouse_Coords.x != New_Mouse_Coords.x || Mouse_Coords.y != New_Mouse_Coords.y)
                {
                    DisplayCursor();
                }
            }
            break;

// -------------------------------------------------------------------------------
// SysTray handling
// -------------------------------------------------------------------------------
        case WM_SHELLNOTIFY:
            if(wParam == SYSTRAY_MSG)
            {
                switch(lParam)
                {
                    case WM_LBUTTONDBLCLK:
                        SysTrayMode = 0;
                        RestoreMDI(hWnd);
                        break;
                    case WM_RBUTTONDOWN:
                        SetForegroundWindow(hWnd);
                        GetCursorPos(&TrayPoint);
                        hTrayMenu = CreatePopupMenu();
                        MenuAddString(hTrayMenu, "Restore", MENU_SYSTRAY_RESTORE, TRUE);
                        MenuAddSeparator(hTrayMenu);
                        MenuSetDefaultItem(hTrayMenu, 0);
                        if(hMenuRecent != 0) AppendMenu(hTrayMenu, MF_POPUP, (UINT) hMenuRecent, "Recent files");
                        if(hMenuRecentPrj != 0) AppendMenu(hTrayMenu, MF_POPUP, (UINT) hMenuRecentPrj, "Recent projects");
                        if((hMenuRecent != 0) || (hMenuRecentPrj != 0)) MenuAddSeparator(hTrayMenu);
                        MenuAddString(hTrayMenu, "Exit", MENU_SYSTRAY_EXIT, TRUE);
                        TrackPopupMenu(hTrayMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_RIGHTBUTTON, TrayPoint.x, TrayPoint.y, 0, hWnd, NULL);
                }
            }
            break;
        case WM_ERASEBKGND:
            return(0);
        case WM_CLOSE:
            // Check if a program is being debugged
            if(SnapRunning != 0) if(Ask_Debug_Break()) return(0);
            // Proceed...
            ForcePrj = 0;
            InQuitProc = 1;
            if(CheckLastModified(hMDIform.hClient, TRUE) != IDCANCEL)
            {
                if(TimerHandle != 0) KillTimer(hWnd, 1);
                QuitNotif = 1;
                ForcePrj = 1;
                // Save opened files/project
                SaveState(PrivateWorkSpaceFile);
                DestroyWindow(hWnd);
                return(0);
            }
            InQuitProc = 0;
            return(0);
        case WM_DESTROY:
            PostQuitMessage(0);
            return(0);
    }
    return(DefFrameProc(hWnd, hMDIform.hClient, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Resize the MDI Form
void ResizeMDIform(void)
{
    long StatusHeight = 0;
    long MinY = RebarGetYSize(hRebar);
    if(ControlIsVisible(hStatusBar)) StatusHeight = ControlHeight(hStatusBar);
    MoveWindow(hMDIform.hDock, 0, MinY, ControlClientWidth(hMDIform.hWnd),
               ControlClientHeight(hMDIform.hWnd) - (MinY + StatusHeight), 1);
}

// -----------------------------------------------------------------------
// Write a comment in status bar
void WriteComment(CStr Comment)
{
    if(ControlIsVisible(hStatusBar) != 0)
    {
        StatusBarSetText(hStatusBar, 0, STATUSBAR_PARTNORMAL, Comment, 0);
    }
}

// -----------------------------------------------------------------------
// Close all source windows
void CloseAllChilds(HWND hWnd)
{
    if(NbForms != 0)
    {
        if(CheckLastModified(hMDIform.hClient, FALSE) != IDCANCEL)
        {
            EnumChildWindows(hWnd, &EnumCloseChildProc, 0);
        }
    }
}
                    
int CALLBACK EnumCloseChildProc(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(FALSE);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                // Mark them as unmodified
                ChildStruct = LoadStructure(hWnd);
                ChildStruct->ModifFlag = 0;
                ControlClose(hWnd);
            }
        }
        return(TRUE);
    }
}

// -----------------------------------------------------------------------
// Change font in source windows
void ChFontAllChilds(HWND hWnd)
{
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumChFontChildProc, 0);
}
                    
int CALLBACK EnumChFontChildProc(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(FALSE);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                SetCodeMaxFont(ChildStruct->hChildCodeMax);
            }
        }
        return(TRUE);
    }
}

// -----------------------------------------------------------------------
// Search for an opened file in child windows list
void SearchOpenedFile(HWND hWnd, CStr FileName)
{
    FoundOpened = 0;
    FoundOpenedhWnd = 0;
    SearchFileName = FileName;
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchFileProc, 0);
}
                    
int CALLBACK EnumSearchFileProc(HWND hWnd, long lParam)
{
    CStr ChildFile;

    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 0) goto NoSearchFileProc;
        if(GetWindowLong(hWnd, GWL_USERDATA) == 0) goto NoSearchFileProc;
        ChildStruct = LoadStructure(hWnd);
        ChildFile = ChildStruct->RFile;
        if(_strcmpi(ChildFile.Get_String(), SearchFileName.Get_String()) == 0)
        {
            FoundOpened = 1;
            FoundOpenedhWnd = hWnd;
            return(0);
        }
        else
        {
NoSearchFileProc:
            return(1);
        }
    }
}

// -----------------------------------------------------------------------
// Save all source windows
void SaveAllChilds(HWND hWnd)
{
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSaveChildProc, 0);
}
                    
int CALLBACK EnumSaveChildProc(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(CMGetRealFile(ChildStruct->RFile).Len() != 0) SaveIt(hWnd);
            }
        }
        return(1);
    }
}

// -----------------------------------------------------------------------
// Open a file
HWND OpenFileNorm(CStr FileName, long ReadO, long CheckOp, long ForceCreate, long NoAddRecent)
{
    long LoadResult = 0;
    long FoundF = 0;
    long FoundInArray = 0;
    CStr DirectZoom;
    int i = 0;
    CStr TestBook;
    CStr BufString;
    HWND ReturnValue = 0;
    OFSTRUCT File_Dat;

    // Set return value
    LoadResultGlob = 0;
    FoundF = 1;

    // Remove the relative path
    OpenFile(FileName.Get_String(), &File_Dat, OF_EXIST);
    FileName = File_Dat.szPathName;

    if(FileExist(FileName) == 0) FoundF = 0;
    if(CheckOp == 0)
    {
        // Check for already opened file
        SearchOpenedFile(hMDIform.hWnd, FileName);
        if(FoundOpened == 1)
        {
            DirectZoom = IniReadKey("Layout", "AutoZoomFiles", MainIniFile);
            if(DirectZoom.Len() != 0)
            {
                if(_strcmpi(DirectZoom.Get_String(), "1") == 0)
                {
                    ControlVisible(FoundOpenedhWnd, 1);
                    SetFocus(FoundOpenedhWnd);
                    return(ReturnValue);
                }
            }
            BufString = "File '" + (CStr) FileName.Get_String() + (CStr) "' is already opened. Reload it ?";
            switch(MiscMsgBox(hMDIform.hWnd, BufString, MB_QUESTION, Requesters))
            {
                case IDYES:
                    break;
                case IDNO:
                    ControlVisible(FoundOpenedhWnd, 1);
                    SetFocus(FoundOpenedhWnd);
                    return(ReturnValue);
            }
        }
    }
    if(ForceCreate == 0)
    {
        if(FoundF == 0)
        {
            LoadResult = 0;
            // Remove it from recent files list
            if(NoAddRecent == 0) AddRecentFile(FileName, LoadResult, 0);
            return(ReturnValue);
        }
    }
    else
    {
        if(FoundF == 0)
        {
            LoadResultGlob = 0;
            return(ReturnValue);
        }
    }
    StoreLanguageToOpen(GetLanguageToOpen(FileName));
    ReturnValue = CreateNewFile(FileName);
    LoadResult = LoadFile(ReturnValue);
    if(LoadResult == 1)
    {
        // Check for presence of the file in array of files (where else ?)
        FoundInArray = 0;
        for(i = 0; i < LastOpened.Amount(); i++)
        {
            if(_strcmpi(LastOpened.Get(i)->Content, FileName.Get_String()) == 0)
            {
                FoundInArray = 1;
                break;
            }
        }
        if(FoundInArray == 0)
        {
            // If not found store it at the end of the array
            // With Coords at 0
            LastOpened.Add(FileName.Upper_Case().Get_String());
            LastOpenedLine.Add(0L);
            LastOpenedCol.Add(0L);
            LastOpenedFirst.Add(0L);
        }
        else
        {
            // User wants a position remember ?
            if(Remember == 1)
            {
                ChildStruct = LoadStructure(ReturnValue);
                SetCurrentTopIndex(ChildStruct->hChildCodeMax, LastOpenedFirst.Get(i)->Content);
                CMSetCaretPos(ChildStruct->hChildCodeMax,
                              LastOpenedLine.Get(i)->Content,
                              LastOpenedCol.Get(i)->Content);
                LoadCurrentSel(ChildStruct->hChildCodeMax);
                RefreshSBStat = 1;
                WritePositionInStatus(ChildStruct->hChildCodeMax);
            }
        }
        // Just to avoid some unnecessary tests
        TestBook = IniReadKey("Layout", "SaveBookmarks", MainIniFile);
        if(TestBook.Len() != 0)
        {
            if(strcmp(TestBook.Get_String(), "1") == 0)
            {
                LoadStructure(ReturnValue);
                LoadBookmarks(ReturnValue, FileName, 0);
            }
        }
    }
    if(NoAddRecent == 0) AddRecentFile(FileName, LoadResult, 0);
    LoadResultGlob = LoadResult;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Fill the global language variable
void StoreLanguageToOpen(CStr LanguageName)
{
    LanguageToOpen.Set_String(LanguageName.Get_String());
}

// -----------------------------------------------------------------------
// Create a new source file
HWND CreateNewFile(CStr FileN)
{
    HWND ReturnValue = 0;

    FileToOpen = StringCopyAppendZero(FileToOpen, FileN);
    if(NbForms != 0) if(ControlGetWindowState(CurrentForm) == SW_SHOWMAXIMIZED) goto ForceMaximize;
    switch(AutoMaximizeChilds)
    {
        case 0:
            ReturnValue = CreateMDIChildDialog(hMDIform.hClient, "", LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + ICON_NEW)),
                                                 CurrentMDIChildInitProc, CurrentMDIChildProc, 0, 1, "");
            break;
        case 1:
ForceMaximize:
            ReturnValue = CreateMDIChildDialog(hMDIform.hClient, "", LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + ICON_NEW)),
                                                 CurrentMDIChildInitProc, CurrentMDIChildProc, WS_MAXIMIZE, 1, "");
            break;
    }
    DrawMenuBar(hMDIform.hWnd);
    ResizeChildCodeMax(ReturnValue);
    LoadStructure(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Menu: Open as RC
void OpenFileAsRC(HWND hWnd, CStr FileLabel, CStr FileName, long ResIndex)
{
    long ReadedDB = 0;
    long DBFileMem = 0;
    CStr OpHeader;
    long FLen = 0;
    HWND NewDbhwnd = 0;
    CStr Label;
    CStr TmpRawStr;

    FLen = FileGetSize(FileName);
    if(FLen == 0)
    {
        WriteToStatus("File '" + (CStr) FileName + (CStr) "' is empty.");
        return;
    }
    DBFileMem = (long) MLoadFile(FileName.Get_String(), &ReadedDB);
    if(DBFileMem == 0) goto NoOpenFileRC;
    // Set buffer to max len
    TextToOp = TextToOp.String(((FLen / 16) * strlen(TabRc[0])), ' ');
    // Remaining bytes
    TextToOpEnd = TextToOpEnd.String(strlen(TabRc[0]), ' ');
    // Fill the buffer
    CopyMem(TabRc[0], TextToOp.Get_String(), strlen(TabRc[0]), 0, (FLen / 16));
    // Fill the remaining buffer
    if((FLen % 16) != 0) TextToOpEnd = TabRc[(16 - (FLen % 16)) / 2];
    // Convert main part
    if(FLen > 14) FillHexWords(DBFileMem, TextToOp.Get_String(), 16, 3, 2, (FLen / 16), 3);
    // Convert remaining bytes
    if((FLen % 16) != 0) FillHexWords(DBFileMem + (FLen - (FLen % 16)), TextToOpEnd.Get_String(), (FLen % 16), 3, 0, 1, 3);
    // Write the header
    OpHeader = "// Created from file: " + (CStr) FileName.Get_String() + (CStr) "\r\n// Original length: " + (CStr) FLen + (CStr) " byte";
    if(FLen > 1) OpHeader = OpHeader + "s";
    OpHeader = OpHeader + "\r\n";
    Label = StringReplace(FileReplaceExtension(FileGetFileName(FileName), ""), ".", "", 1, -1, Binary_Compare);
    OpHeader = OpHeader + FileLabel.Get_String() + (CStr) " RCDATA ";
    // Put header
    OpHeader = OpHeader + (CStr) CalcResourceHeader(GetRawdataProps(ResIndex)).Trim().Get_String() + (CStr) "\r\n";
    // Copy raw datas
    OpHeader = OpHeader + "BEGIN\r\n";
    if(TextToOpEnd.Len() != 0)
    {
        TextToOpEnd = "\t" + TextToOpEnd.Trim() + "\r\n";
    }
    OpHeader = OpHeader + TextToOp + (CStr) TextToOpEnd;
    OpHeader = OpHeader + "END\r\n";
    // Free the allocated memory for the file
    FreeMem(DBFileMem);
    // Create the window and store datas into it
    StoreLanguageToOpen("Resource");
    if(hWnd == 0) NewDbhwnd = CreateNewFile(FileName);
    else NewDbhwnd = hWnd;
    if(NewDbhwnd != 0)
    {
        ChildStruct = LoadStructure(NewDbhwnd);
        // Copy text
        CM_SetText(ChildStruct->hChildCodeMax, OpHeader.Get_String());
        // Set modified state on
        ChildStruct->ModifFlag = 1;
        // Set window referenced file
        ChildStruct->RFile->Set_String(StringCopyAppendZero(ChildStruct->RFile, FileRemoveExtension(FileName).Get_String() + (CStr) ".rc").Get_String());
        // Set window title
        ControlSetText(NewDbhwnd, FileRemoveExtension(FileName).Get_String() + (CStr) ".rc *");
        LastLoadAsDBDir = FileGetDirectory(FileName);
        LoadCurrentSel(ChildStruct->hChildCodeMax);
        RefreshSBStat = 1;
        WritePositionInStatus(ChildStruct->hChildCodeMax);
    }
    else
    {
        WriteToStatus("Can't create a window for file '" + (CStr) FileName + (CStr) "'.");
    }
    return;
NoOpenFileRC:
    WriteToStatus("Can't open file '" + (CStr) FileName + (CStr) "' for conversion.");
    return;
}

// -----------------------------------------------------------------------
// Open a Workspace file
void OpenWorkSpc(CStr FileName)
{
    int i = 0;
    CStr BufString;

    if(CheckWorkSpaceFile(FileName) == 0)
    {
        BufString = "Not a " + (CStr) AppTitle + (CStr) " workspace file.";
        MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
        return;
    }
    // Add the workspace name into recents file list
    // Proceed with files
    for(i = 0; i <= 999; i++)
    {
        MDIRetVal = IniReadKey("Files", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), FileName);
        if(MDIRetVal.Len() == 0) break;
        // Open the file without adding it in recent list
        OpenUnknownFile(MDIRetVal, FALSE);
    }
    // Now add the workspace file into recents files
    AddRecentFile(FileName, 1, 0);
    // Store last used workspace dir
    LastWorkSpaceDir = FileGetDirectory(FileName);
}

// -----------------------------------------------------------------------
// Save all source windows into a workspace file
void SaveAllChildsInWorkSpace(HWND hWnd)
{
    WorkSpaceNumber = 0;
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSaveAllChildsInWorkSpace, 0);
}

int CALLBACK EnumSaveAllChildsInWorkSpace(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(ChildStruct->FileLoaded != 0)
                {
                    if(ChildStruct->RFile->Len() != 0)
                    {
                        IniWriteKey("Files", "File" + (CStr) StringNumberComplement(WorkSpaceNumber, 3).Get_String(), ChildStruct->RFile->Left(ChildStruct->RFile->Len()), WorkSpaceFileName);
                        WorkSpaceNumber++;
                    }
                }
            }
        }
        return(1);
    }
}

// -----------------------------------------------------------------------
// Save a file
CStr SaveIt(HWND hWnd)
{
    CStr ReturnValue;
    CStr BufString;

    if(NbForms != 0)
    {
        ChildStruct = LoadStructure(hWnd);
        if(ChildStruct->ModifFlag != 0)
        {
            // Don't save read only files
            if(ChildStruct->ReadOnly == 0)
            {
                if(ChildStruct->RFile->Len() != 0)
                {
                    // File already named ?
                    if(_strcmpi(ChildStruct->RFile->Left(18).Get_String(), "<untitled document") == 0)
                    {
                        BufString = SaveItAs(hWnd, 0, "", "");
                    }
                    else
                    {
                        BufString = CMGetRealFile(ChildStruct->RFile).Get_String();
                        if(SaveCodeMax(hWnd, ChildStruct->RFile) == 0) BufString = "";
                    }
                }
            }
        }
        else
        {
            if(ChildStruct->FileLoaded != 0) BufString = CMGetRealFile(ChildStruct->RFile).Get_String();
        }
    }
    // Check if it's a language that must be updated.
    Update_Language(CMGetRealFile(ChildStruct->RFile));
    ReturnValue.Set_String(BufString.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Save a file under a new name
CStr SaveItAs(HWND hWnd, long RefreshTree, CStr TreeKey, CStr OldTreeFile)
{
    CStr FName;
    CStr Filters;
    CStr ReturnValue;
    int i;

    FName = "";
    if(NbForms == 0) return(ReturnValue);
    Filters = "";
    for(i = 0; i < RegisteredExts.Amount(); i++)
    {
        Filters = Filters + (CStr) RegisteredExts.Get(i)->Content + (CStr) "|";
    }
    if(Filters.Len() == 0) Filters = "All files (*.*)|*.*";
    if(GetUseFileDir() == 1)
    {
        ChildStruct = LoadStructure(hWnd);
        FName = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, FileGetDirectory(ChildStruct->RFile), CurrentDir);
    }
    else
    {
        FName = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, LastSaveDir, CurrentDir);
    }
    if(FName.Len() != 0)
    {
        if(SaveCodeMax(hWnd, FName) == 0) goto NoSaveAs;
        AddRecentFile(FName, 1, 0);
// This was meant to refresh the project tree but i don't remember why right now
// (And i didn't use it anywhere)
//        If RefreshTree = True Then
//            PosPar = In_Str(1, OldTreeFile, "(")
//            If PosPar <> 0 Then
//                OldTreeFile = Mid(OldTreeFile, PosPar + 1)
//                OldTreeFile = Mid(OldTreeFile, 1, Len(OldTreeFile) - 1)
//                TreePrj.Nodes(SelKey).Text = StringReplace(TreePrj.Nodes(SelKey).Text, OldTreeFile, FName)
//            End If
//        End If
        LastSaveDir = FileGetDirectory(FName);
    }
    // Check if it's a language that must be updated.
    Update_Language(FName);
NoSaveAs:
    ReturnValue.Set_String(FName.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Client Winproc
LRESULT CALLBACK ClientProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long NbrDroped = 0;
    long FNameSize = 0;
    CStr DropedFileName;
    int i = 0;
    int j = 0;
    long HaveToDie = 0;
    long PosInListView = 0;
    long FoundEmptyHwnd = 0;
    DLGPROC LocMDIClientHook;
    
    // Execute possible hooks
    for(i = 0; i < MDIClientHooks.Amount(); i++)
    {
        if(MDIClientHooks.Get(i)->Content != 0)
        {
            LocMDIClientHook = MDIClientHooks.Get(i)->Content;
            if(LocMDIClientHook(hWnd, uMsg, wParam, lParam) == HOOK_DIE)
            {
                HaveToDie = HaveToDie | HOOK_DIE;
            }
        }
    }
    if((long) HaveToDie == (long) HOOK_DIE) return(0);
    
    switch(uMsg)
    {
        // Monitor childs windows creations/destructions
        case CHILD_MSG_CREATED:
            if(lParam != 0)
            {
                PosInListView = ListViewItemCount(hWindowsListView);
                ListViewAddItem(hWindowsListView, ControlGetText((HWND) lParam), PosInListView, wParam);
                // Keep it up to date
                PreviouslySelectedWindow = ListViewGetSelItem(hWindowsListView, -1);
                
                FoundEmptyHwnd = 0;
                // Search an empty position in childs handles array
                for(i = 0; i < ChildshWnd.Amount(); i++)
                {
                    if(ChildshWnd.Get(i)->Content == 0)
                    {
                        FoundEmptyHwnd = 1;
                        break;
                    }
                }
                // Save handle and listview ordinal position in found empty position
                // Arrays have to grow ?
                if(FoundEmptyHwnd == 0)
                {
                    // Create new empty entries
                    ChildshWnd.Add((HWND) lParam);
                    ChildsLVPos.Add(PosInListView);
                }
                else
                {
                    ChildshWnd.Set(i, (HWND) lParam);
                    ChildsLVPos.Set(i, PosInListView);
                }
            }
            return(0);
        case CHILD_MSG_DESTROYED:
            if(lParam != 0)
            {
                // Search concerned child handle
                for(i = 0; i < ChildshWnd.Amount(); i++)
                {
                    if(lParam == (long) ChildshWnd.Get(i)->Content)
                    {
                        // Remove it from the list
                        ListViewDeleteItem(hWindowsListView, ChildsLVPos.Get(i)->Content);
                        // And make it an empty entry
                        ChildshWnd.Set(i, 0);
                        ChildsLVPos.Set(i, -1);
                        // All following position going down
                        for(j = i + 1; j < ChildsLVPos.Amount(); j++)
                        {
                            ChildsLVPos.Set(j, ChildsLVPos.Get(j)->Content - 1);
                        }
                        PreviouslySelectedWindow = ListViewGetSelItem(hWindowsListView, -1);
                        break;
                    }
                }
            }
            return(0);
        case CHILD_MSG_SETTITLE:
            if(lParam != 0)
            {
                // Search concerned child handle
                for(i = 0; i < ChildshWnd.Amount(); i++)
                {
                    if(lParam == (long) ChildshWnd.Get(i)->Content)
                    {
                        // Modify datas
                        if(wParam != 0) ListViewSetSubItemImage(hWindowsListView, wParam, ChildsLVPos.Get(i)->Content, 0);
                        ListViewSetSubItem(hWindowsListView, ControlGetText((HWND) lParam), ChildsLVPos.Get(i)->Content, 0);
                        break;
                    }
                }
            }
            return(0);
        case WM_KILLFOCUS:
            FocusOnActiveChild();
            return(0);
        // Process dropped files from shell
        case WM_DROPFILES:
            if(AcceptFiles == 1)
            {
                if(wParam != 0)
                {
                    // Retrieve the number of droped files
                    NbrDroped = DragQueryFile((HDROP) wParam, 0xFFFFFFFF, 0, 0);
                    if(NbrDroped != 0)
                    {
                        for(i = 0; i <= (long) NbrDroped - 1; i++)
                        {
                            // Retrieve the size to allocate for filename (+1)
                            FNameSize = DragQueryFile((HDROP) wParam, i, NULL, 0) + 1;
                            if(FNameSize != 0)
                            {
                                // Allocate space
                                DropedFileName = DropedFileName.String(FNameSize + 1, ' ');
                                // Retrieve filename
                                DragQueryFile((HDROP) wParam, i, DropedFileName.Get_String(), FNameSize);
                                DropedFileName = DropedFileName.Trim();
                                OpenUnknownFile(DropedFileName, TRUE);
                            }
                        }
                    }
                }
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Set the focus on the active window child
void FocusOnActiveChild(void)
{
    int i = 0;
    HWND Forehwnd = 0;

    if(NbForms != 0)
    {
        Forehwnd = ClientGetActiveChild(hMDIform.hClient);
        CurrentForm = Forehwnd;
        ChildStruct = LoadStructure(Forehwnd);
        
        SetFocus(ChildStruct->hChildCodeMax);
        RefreshSBStat = 1;
        WritePositionInStatus(ChildStruct->hChildCodeMax);
        for(i = 0; i < ChildshWnd.Amount(); i++)
        {
            if(Forehwnd == ChildshWnd.Get(i)->Content)
            {
                // Modify datas
                ListViewSetItemSel(hWindowsListView, ChildsLVPos.Get(i)->Content);
                PreviouslySelectedWindow = ListViewGetSelItem(hWindowsListView, -1);
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------
// Search for externally Modified files
void SearchModified(HWND hWnd)
{
    int i = 0;
    FILETIME TmpProjectFDate;
    int RetPrjLoad;

    if(InSelFile == 1) return;
    ExternModified = 0;
    ModifiedArrayhWnd.Erase();
    // Compare project file
    if(ProjectOn)
    {
        if(FileGetWriteTime(ProjectFName, &TmpProjectFDate) != 0)
        {
            if(CompareFileTime(&TmpProjectFDate, &ProjectFDate) > 0)
            {
                ExternModified += 1;
                // (Phony entry)
                ModifiedArrayhWnd.Add((HWND) 0L);
            }
        }
    }

    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchDateModified, 0);
    switch(ExternModified)
    {
        case 0:
            break;
        default:
            FRMFilesTitle = "Modified files detected";
            // Scheme: Reload/Cancel
            FRMFilesScheme = FILESSCHEME_EXTERNMODIFIED;
            FRMFilesSelArray.Erase();
            FRMFilesSelArray.MAdd(ExternModified, 0L);
            CreateModalDialog(-1, -1, 402, 244, hMDIform.hWnd, &FRMFilesProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
            switch(FRMFilesChoice)
            {
                case IDYES:
                    for(i = 0; i < FRMFilesSelArray.Amount(); i++)
                    {
                        if(FRMFilesSelArray.Get(i)->Content != 0)
                        {
                            if(ModifiedArrayhWnd.Get(i)->Content)
                            {
                                ClearFile(ModifiedArrayhWnd.Get(i)->Content);
                                LoadFile(ModifiedArrayhWnd.Get(i)->Content);
                            }
                            else
                            {
                                // Reload project
                                ProjectModified = FALSE;
                                RetPrjLoad = RealOpenPrj(ProjectFName);
                                if(RetPrjLoad != -1) AddRecentPrj(ProjectFName, RetPrjLoad, 0);
                            }
                        }
                        else
                        {
                            if(ModifiedArrayhWnd.Get(i)->Content)
                            {
                                ChildStruct = LoadStructure(ModifiedArrayhWnd.Get(i)->Content);
                                FileGetWriteTime(CMGetRealFile(ChildStruct->RFile), ChildStruct->FileDateOpen);
                            }
                            else
                            {
                                FileGetWriteTime(ProjectFName, &ProjectFDate);
                            }
                        }
                    }
                    break;
                case IDCANCEL:
                    // Update all dates
                    for(i = 0; i < FRMFilesSelArray.Amount(); i++)
                    {
                        if(ModifiedArrayhWnd.Get(i)->Content)
                        {
                            ChildStruct = LoadStructure(ModifiedArrayhWnd.Get(i)->Content);
                            FileGetWriteTime(CMGetRealFile(ChildStruct->RFile), ChildStruct->FileDateOpen);
                        }
                        else
                        {
                            FileGetWriteTime(ProjectFName, &ProjectFDate);
                        }
                    }
                    break;
            }
    }
}

int CALLBACK EnumSearchDateModified(HWND hWnd, long lParam)
{
    FILETIME FileDT;
    
    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                // Check if it contains a loaded file
                if(ChildStruct->FileLoaded == 1)
                {
                    // Only check for existing files
                    if(FileExist(CMGetRealFile(ChildStruct->RFile)) != 0)
                    {
                        // Take the new date
                        if(FileGetWriteTime(CMGetRealFile(ChildStruct->RFile), &FileDT) != 0)
                        {
                            if(CompareFileTime(&FileDT, ChildStruct->FileDateOpen) > 0)
                             {
                                // Store modified window handle
                                ModifiedArrayhWnd.Add(hWnd);
                                ExternModified++;
                            }
                        }
                    }
                    else
                    {
                        // Deleted file
                        ForceChildModified(hWnd);
                    }
                }
            }
        }
        return(1);
    }
}

// -----------------------------------------------------------------------
// Search for internally Modified files
long CheckLastModified(HWND hWnd, int CheckProject)
{
    long ReturnValue = 0;
    int i = 0;

    if(InSelFile == 1) return(IDCANCEL);
    ExternModified = 0;
    ModifiedArrayhWnd.Erase();
    if(CheckProject)
    {
        if(ProjectOn)
        {
            if(ProjectModified)
            {
                ExternModified++;
                // Phony entry
                ModifiedArrayhWnd.Add((HWND) 0L);
            }
        }
    }
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchModified, 0);
    ReturnValue = IDYES;
    switch(ExternModified)
    {
        case 0:
            if(CheckProject)
            {
                if(IniReadBoolKey("Layout", "ConfirmBeforeExit", MainIniFile))
                {
                    ReturnValue = MiscMsgBox(hMDIform.hWnd, "Do you really want to quit ?", MB_QUESTION, Requesters);
                    if(ReturnValue == IDNO) ReturnValue = IDCANCEL;
                }
            }
            break;
        default:
            // Prompt for several files
            ReturnValue = IDCANCEL;
            FRMFilesTitle = "Modified files";
            // Scheme: Save/Cancel
            FRMFilesScheme = FILESSCHEME_INTERNMODIFIED;
            FRMFilesSelArray.Erase();
            // Add the number of entries corresponding to the number of modified files found.
            FRMFilesSelArray.MAdd(ExternModified, 0L);
            CreateModalDialog(-1, -1, 402, 244, hMDIform.hWnd, &FRMFilesProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
            switch(FRMFilesChoice)
            {
                case IDYES:
                    // Save selected files
                    for(i = 0; i < FRMFilesSelArray.Amount(); i++)
                    {
                        if(FRMFilesSelArray.Get(i)->Content != 0)
                        {
                            if(ModifiedArrayhWnd.Get(i)->Content)
                            {
                                LoadStructure(ModifiedArrayhWnd.Get(i)->Content);
                                SaveIt(ModifiedArrayhWnd.Get(i)->Content);
                            }
                            else
                            {
                                // Save the project
                                MCMD_SaveProject();
                            }
                        }
                    }
                    ReturnValue = IDYES;
                    break;
                case IDNO:
                    ReturnValue = IDNO;
                    break;
                case IDCANCEL:
                    break;
            }
    }
    return(ReturnValue);
}

int CALLBACK EnumSearchModified(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(FALSE);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                // Check if it contains a loaded file
                if(ChildStruct->ModifFlag == 1)
                {
                    // Store modified window handle
                    ModifiedArrayhWnd.Add(hWnd);
                    ExternModified++;
                }
            }
        }
        return(TRUE);
    }
}

// -----------------------------------------------------------------------
// Refresh the timer for autosaving notifications
// (after a modification happens in properties)
long RefreshTimer(HWND hWnd)
{
    if(TimerHandle != 0) KillTimer(hWnd, 1);
    return(CreateTimer(hWnd));
}

// -----------------------------------------------------------------------
// Create a timer for autosaving notifications
long CreateTimer(HWND hWnd)
{
    long ReturnValue = -1;
    long Interval = 0;

    switch(AutoSaveDelay)
    {
        case 0:
            Interval = 0;
            break;
        case 1:
            Interval = 1;
            break;
        case 2:
            Interval = 2;
            break;
        case 3:
            Interval = 5;
            break;
        case 4:
            Interval = 10;
            break;
        case 5:
            Interval = 15;
            break;
        case 6:
            Interval = 30;
            break;
        case 7:
            Interval = 45;
            break;
        case 8:
            Interval = 60;
            break;
    }
    TimerHandle = 0;
    if(Interval != 0)
    {
        TimerHandle = SetTimer(hWnd, 1, Interval * 60000, &TimerNotify);
        ReturnValue = TimerHandle;
    }
    return(ReturnValue);
}

void CALLBACK TimerNotify(HWND hWnd, UINT uMsg, UINT_PTR idEvent, unsigned long dwTime)
{
    switch(uMsg)
    {
        case WM_TIMER:
            if(AutoSaveDelay != 0)
            {
                if(FreezeTimer == 0)
                {
                    KillTimer(hWnd, 1);
                    CheckAutoSave(hMDIform.hClient);
                    CreateTimer(hMDIform.hWnd);
                }
            }
            break;
    }
}

// -----------------------------------------------------------------------
// Search for internally Modified files
void CheckAutoSave(HWND hWnd)
{
    int i = 0;

    ExternModified = 0;
    if(InSelFile == 1) return;
    ModifiedArrayhWnd.Erase();
    if(ProjectOn)
    {
        if(ProjectModified)
        {
            ExternModified += 1;
            // Phony entry
            ModifiedArrayhWnd.Add((HWND) 0L);
        }
    }
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSearchModified, 0);
    if(ExternModified != 0)
    {
        FRMFilesTitle = "Autosave notification";
        // Scheme Save/Cancel
        FRMFilesScheme = FILESSCHEME_AUTOSAVE;
        FRMFilesSelArray.Erase();
        FRMFilesSelArray.MAdd(ExternModified, 0L);
        if(VerboseSave == 1)
        {
            CreateModalDialog(-1, -1, 402, 244, hMDIform.hWnd, &FRMFilesProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
        }
        else
        {
            // Select all files if in silent mode
            for(i = 0; i < FRMFilesSelArray.Amount(); i++)
            {
                FRMFilesSelArray.Set(i, 1);
            }
            // Schedule the saving (right below).
            FRMFilesChoice = IDYES;
        }
        switch(FRMFilesChoice)
        {
            case IDYES:
                // Save selected files
                for(i = 0; i < FRMFilesSelArray.Amount(); i++)
                {
                    if(FRMFilesSelArray.Get(i)->Content != 0)
                    {
                        if(ModifiedArrayhWnd.Get(i)->Content)
                        {
                            LoadStructure(ModifiedArrayhWnd.Get(i)->Content);
                            SaveIt(ModifiedArrayhWnd.Get(i)->Content);
                        }
                        else
                        {
                            // Save the project
                            MCMD_SaveProject();
                        }
                    }
                }
                break;
            case IDCANCEL:
                break;
        }
    }
}

// -----------------------------------------------------------------------
// Statusbar window hook
LRESULT CALLBACK StatusHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Remove TrayIcon, restore and bring MDI to foreground
void RestoreMDI(HWND hWnd)
{
    Shell_NotifyIcon(NIM_DELETE, &SysTrayStruct);
    ShowWindow(hWnd, SW_RESTORE);
    SetForegroundWindow(hWnd);
    SetActiveWindow(hWnd);
}
