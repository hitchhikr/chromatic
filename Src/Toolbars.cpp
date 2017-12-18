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
// Toolbars.cpp: Rebars/Toolbars handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ConstMenus.h"
#include "Globals.h"
#include "ImageLists.h"
#include "ConstMenus.h"
#include "ConstRes.h"
#include "MDI_Form.h"

// -----------------------------------------------------------------------
// Variables
HWND hMainToolbar;
HWND hBookMarkToolbar;
HWND hBreakpointToolbar;
HWND hRebar;
HWND hMainPager;
HWND hBookMarksPager;
HWND hBreakpointsPager;
CStr ToolBarKey;
CStr ToolBarType;
CStr ToolBarShow;
CStr BtnIcon;
long BtnID;
CStr BtnMenu;
CStr BtnToolTip;
CStr BtnComment;
CStr BtnCommand;
long CurrenthToolbar;
CList <HWND> ToolBarArray;
CList <int> ToolBarMenus;
CList <char *> ToolBarToolTips;
CList <char *> ToolBarComments;
CList <int> ToolBarCommands;
CList <HWND> PagersHwnd;
CList <int> PagersSize;
CList <HWND> ToolbarsHwnd;
CList <int> ToolBarsShow;
CList <char *> ToolBarsName;
long MaxToolbarBtn;
SIZE TtSize;
long OldTBSize;

long ToolbarFileIcons[MENU_FILE_LAST_ID + 1];
long ToolbarEditIcons[MENU_EDIT_LAST_ID + 1];
long ToolbarViewIcons[MENU_VIEW_LAST_ID + 1];
long ToolbarProjectIcons[MENU_PROJECT_LAST_ID + 1];
long ToolbarWindowIcons[MENU_WINDOW_LAST_ID + 1];

// -----------------------------------------------------------------------
// Create the rebar and attained toolbars
void InitReBarContent(HWND hParent)
{
    long i = 0;
    long j = 0;
    bool RealCreated = false;
    long NbrPager = 0;
    long CreatedBtn = 0;
    HWND hCurrentPager = 0;
    HWND hCurrentToolBar = 0;
    long NbrDrop = 0;
    CStr BufString;
    CStr BufString2;
    long CurrentCommandNumber;
    long MarginWidth = 13;

    MARGINS MarginDatas;

    if(WindowsXP)
    {
        // Retrieve the width of the margins
        SendMessage(hRebar, RB_GETBANDMARGINS, 0, (LPARAM) &MarginDatas);
        MarginWidth += MarginDatas.cxLeftWidth;
    }

    ToolBarArray.Erase();
    ToolBarMenus.Erase();
    ToolBarToolTips.Erase();
    ToolBarComments.Erase();
    ToolBarCommands.Erase();
    PagersHwnd.Erase();
    PagersSize.Erase();
    ToolbarsHwnd.Erase();
    ToolBarsShow.Erase();
    ToolBarsName.Erase();
    MaxToolbarBtn = 0;
    BtnID = 0;
    NbrPager = 0;
    for(i = 0; i <= 999; i++)
    {
        BufString2 = "TbName";
        BufString2 = BufString2 + StringNumberComplement(i, 3);
        ToolBarKey = IniReadKey("Toolbars", BufString2, ToolbarsIniFile);
        BufString2 = "TbType";
        BufString2 = BufString2 + StringNumberComplement(i, 3);
        ToolBarType = IniReadKey("Toolbars", BufString2, ToolbarsIniFile);
        BufString2 = "TbShow";
        BufString2 = BufString2 + StringNumberComplement(i, 3);
        ToolBarShow = IniReadKey("Toolbars", BufString2, ToolbarsIniFile);
        if(ToolBarKey.Len() == 0) break;
        if(ToolBarType.Len() == 0) ToolBarType = "0";
        if(ToolBarShow.Len() == 0) ToolBarShow = "1";
        // Create a new toolbar
        hCurrentToolBar = CreateToolBar(0, 0, 0, 23, hRebar, GlobalImageList1, 0, -1, 0, TBSTYLE_TOOLTIPS | TBSTYLE_LIST | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH, TBSTYLE_EX_MIXEDBUTTONS | TBSTYLE_EX_DRAWDDARROWS);
        //SendMessage(hCurrentToolBar, TB_SETPADDING, 0, 0x00000);
        //SendMessage(hCurrentToolBar, TB_SETBUTTONSIZE, 0, 0x170018);
        //SendMessage(hCurrentToolBar, TB_SETBUTTONWIDTH, 0, MAKELONG(2, 0x18));
        SendMessage(hCurrentToolBar, TB_AUTOSIZE, 0, 0);
        CreatedBtn = 0;
        NbrDrop = 0;
        OldTBSize = 0;
        for(j = 0; j <= 999; j++)
        {
            BufString = "BtnIcon";
            BufString = BufString + StringNumberComplement(j, 3);
            BtnIcon = IniReadKey(ToolBarKey, BufString, ToolbarsIniFile);
            BufString = "BtnMenu";
            BufString = BufString + StringNumberComplement(j, 3);
            BtnMenu = IniReadKey(ToolBarKey, BufString, ToolbarsIniFile);
            BufString = "BtnTool";
            BufString = BufString + StringNumberComplement(j, 3);
            BtnToolTip = IniReadKey(ToolBarKey, BufString, ToolbarsIniFile);
            BufString = "BtnHelp";
            BufString = BufString + StringNumberComplement(j, 3);
            BtnComment = IniReadKey(ToolBarKey, BufString, ToolbarsIniFile);
            BufString = "BtnComd";
            BufString = BufString + StringNumberComplement(j, 3);
            BtnCommand = IniReadKey(ToolBarKey, BufString, ToolbarsIniFile);
            if(BtnIcon.Len() == 0) break;
            if(BtnMenu.Len() == 0) break;
            switch(BtnMenu.Get_Long())
            {
                // Separator
                case -2:
                    ToolBarAddSeparator(hCurrentToolBar, 0);
                    // Hack it to 8 pixels
                    OldTBSize += 8;
                    break;
                // Simple button
                case -1:
                    if(BtnIcon.Is_Numeric())
                    {
                        ToolBarAddButton(hCurrentToolBar, "", BtnID, BtnIcon.Get_Long(), TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE, TBSTATE_ENABLED, 1);
                        OldTBSize += ToolBarGetButtonIndexXSize(hCurrentToolBar, BtnID);
                    }
                    else
                    {
                        ToolBarAddButton(hCurrentToolBar, BtnIcon, BtnID, 0, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE, TBSTATE_ENABLED, 1);
                        OldTBSize += ToolBarGetButtonIndexXSize(hCurrentToolBar, BtnID);
                    }
                    break;
                // Dropdown button
                default:
                    if(BtnIcon.Is_Numeric())
                    {
                        switch(BtnMenu.Get_Long())
                        {
                            case 9:
                                ToolBarAddButton(hCurrentToolBar, "", BtnID, BtnIcon.Get_Long(), TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE | BTNS_WHOLEDROPDOWN, TBSTATE_ENABLED, 1);
                                OldTBSize += ToolBarGetButtonIndexXSize(hCurrentToolBar, BtnID);
                                break;
                            case 10:
                                ToolBarAddButton(hCurrentToolBar, "", BtnID, BtnIcon.Get_Long(), TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE | BTNS_WHOLEDROPDOWN, TBSTATE_ENABLED, 1);
                                OldTBSize += ToolBarGetButtonIndexXSize(hCurrentToolBar, BtnID);
                                break;
                            default:
                                ToolBarAddButton(hCurrentToolBar, "", BtnID, BtnIcon.Get_Long(), TBSTYLE_BUTTON | BTNS_DROPDOWN | TBSTYLE_AUTOSIZE, TBSTATE_ENABLED, 1);
                                OldTBSize += ToolBarGetButtonIndexXSize(hCurrentToolBar, BtnID);
                                break;
                        }
                    }
                    else
                    {
                        switch(BtnMenu.Get_Long())
                        {
                            case 9:
                                ToolBarAddButton(hCurrentToolBar, BtnIcon, BtnID, 0, TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE | BTNS_WHOLEDROPDOWN, TBSTATE_ENABLED, 1);
                                OldTBSize += ToolBarGetButtonIndexXSize(hCurrentToolBar, BtnID);
                                break;
                            case 10:
                                ToolBarAddButton(hCurrentToolBar, BtnIcon, BtnID, 0, TBSTYLE_DROPDOWN | TBSTYLE_AUTOSIZE | BTNS_WHOLEDROPDOWN, TBSTATE_ENABLED, 1);
                                OldTBSize += ToolBarGetButtonIndexXSize(hCurrentToolBar, BtnID);
                                break;
                            default:
                                ToolBarAddButton(hCurrentToolBar, BtnIcon, BtnID, 0, TBSTYLE_BUTTON | BTNS_DROPDOWN | TBSTYLE_AUTOSIZE, TBSTATE_ENABLED, 1);
                                OldTBSize += ToolBarGetButtonIndexXSize(hCurrentToolBar, BtnID);
                                break;
                        }
                    }
                    NbrDrop++;
            }
            ToolBarMenus.Add(BtnMenu.Get_Long());
            ToolBarArray.Add(hCurrentToolBar);
            ToolBarToolTips.Add(BtnToolTip.Get_String());
            ToolBarComments.Add(BtnComment.Get_String());
            CurrentCommandNumber = 0;
            if(BtnCommand.Len() != 0)
            {
                // Correct a bug that can happens with some older common controls.
                // The dropdown is not included with the button in recent lists
                // (not a bug but i wanted it to act like that).
                switch(BtnMenu.Get_Long())
                {
                    case 9:
                        CurrentCommandNumber = -1;
                        break;
                    case 10:
                        CurrentCommandNumber = -1;
                        break;
                    default:
                        CurrentCommandNumber = BtnCommand.Get_Long();
                        break;
                }
            }
            ToolBarCommands.Add(CurrentCommandNumber);
            CreatedBtn++;
            MaxToolbarBtn++;
            BtnID++;
        }
        if(CreatedBtn != 0)
        {
            ToolBarResize(hCurrentToolBar);
            hCurrentPager = CreatePager(0, 0, OldTBSize, ToolBarGetYSize(hCurrentToolBar), hParent, hCurrentToolBar, NbrPager, PGS_HORZ);
            NbrPager++;
            RealCreated = true;
            PagersHwnd.Add(hCurrentPager);
            PagersSize.Add(OldTBSize);
            ToolbarsHwnd.Add(hCurrentToolBar);
            ToolBarsShow.Add(ToolBarShow.Get_Long());
            ToolBarsName.Add(ToolBarKey.Get_String());
            switch(ToolBarType.Get_Long())
            {
                case 1:
                    RebarAddBand(hRebar, hCurrentPager, "", ToolBarGetButtonYSize(hCurrentToolBar), RBBS_BREAK | RBBS_GRIPPERALWAYS, OldTBSize + MarginWidth);
                    break;
                case 0:
                    RebarAddBand(hRebar, hCurrentPager, "", ToolBarGetButtonYSize(hCurrentToolBar), RBBS_GRIPPERALWAYS, OldTBSize + MarginWidth);
                    break;
            }
            RebarBandVisible(hRebar, i, ToolBarShow.Get_Long());
        }
    }
    if(RealCreated == false) ControlVisible(hRebar, 0);
    RebarResize(hRebar);
    ResizeMDIform();
}

// -----------------------------------------------------------------------
// Init comments blocks for factory menus
void InitToolBarsIcons(void)
{
    ToolbarFileIcons[MENU_FILE_NEW_ID] = ICON_NEW;
    ToolbarFileIcons[MENU_FILE_CLEAR_ID] = ICON_NEW;
    ToolbarFileIcons[MENU_FILE_DUPLICATEFILE_ID] = ICON_OPEN;
    ToolbarFileIcons[MENU_FILE_OPENFILE_ID] = ICON_OPEN;
    ToolbarFileIcons[MENU_FILE_FILTERS_ID] = ICON_LAB;
    ToolbarFileIcons[MENU_FILE_OPENWORKSPACE_ID] = ICON_OPEN;
    ToolbarFileIcons[MENU_FILE_INCLUDEFILE_ID] = ICON_OPEN;
    ToolbarFileIcons[MENU_FILE_FTPMANAGER_ID] = ICON_EARTH;
    ToolbarFileIcons[MENU_FILE_CLOSE_ID] = ICON_CLOSE;
    ToolbarFileIcons[MENU_FILE_CLOSEALL_ID] = ICON_CLOSE;
    ToolbarFileIcons[MENU_FILE_SAVE_ID] = ICON_SAVE;
    ToolbarFileIcons[MENU_FILE_SAVEAS_ID] = ICON_SAVE;
    ToolbarFileIcons[MENU_FILE_SAVEALL_ID] = ICON_SAVE;
    ToolbarFileIcons[MENU_FILE_SAVEWORKSPACE_ID] = ICON_SAVE;
    ToolbarFileIcons[MENU_FILE_REGISTERASSNIPPET_ID] = ICON_CUBES;
    ToolbarFileIcons[MENU_FILE_PRINTFILE_ID] = ICON_PRINT;
    ToolbarFileIcons[MENU_FILE_CONSOLE_RUN_ID] = ICON_CONSOLE;
    ToolbarFileIcons[MENU_FILE_CONSOLE_CHANGE_ID] = ICON_MSDOS;
    ToolbarFileIcons[MENU_FILE_RUNEXTERNALPROGRAM_ID] = ICON_RUNPROG;
    ToolbarFileIcons[MENU_FILE_SETWORKINGDIRECTORY_ID] = ICON_DIRVIEW;
    ToolbarFileIcons[MENU_FILE_EXIT_ID] = ICON_WINDOWS;
    ToolbarFileIcons[MENU_FILE_RELOADFILE_ID] = ICON_REFRESH;
    ToolbarFileIcons[MENU_FILE_INCLUDEFILTERS_ID] = ICON_LAB;
    ToolbarFileIcons[MENU_FILE_REGISTERASTEMPLATE_ID] = ICON_CUBES;
    ToolbarFileIcons[MENU_FILE_FAVORITES_ID] = ICON_HEARTOFGOLD;
    ToolbarFileIcons[MENU_FILE_ADD_FAVORITE_ID] = ICON_HEARTOFGOLDPLUS;

    ToolbarEditIcons[MENU_EDIT_UNDO_ID] = ICON_UNDO;
    ToolbarEditIcons[MENU_EDIT_REDO_ID] = ICON_REDO;
    ToolbarEditIcons[MENU_EDIT_CUT_ID] = ICON_CUT;
    ToolbarEditIcons[MENU_EDIT_COPY_ID] = ICON_COPY;
    ToolbarEditIcons[MENU_EDIT_COPYFILEPATHNAME_ID] = ICON_COPY;
    ToolbarEditIcons[MENU_EDIT_COPYENTIRETEXT_ID] = ICON_COPY;
    ToolbarEditIcons[MENU_EDIT_PASTE_ID] = ICON_PASTE;
    ToolbarEditIcons[MENU_EDIT_DELETE_DELETESELECTION_ID] = ICON_DELETE;
    ToolbarEditIcons[MENU_EDIT_DELETE_DELETELINE_ID] = ICON_DELETE;
    ToolbarEditIcons[MENU_EDIT_DELETE_DELETETOSTARTOFLINE_ID] = ICON_DELETE;
    ToolbarEditIcons[MENU_EDIT_DELETE_DELETETOENDOFLINE_ID] = ICON_DELETE;
    ToolbarEditIcons[MENU_EDIT_SELECTLINE_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_SELECTALL_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_SELECTPROCEDURE_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_SELECTPROCANDCOLLAPSE_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_UNCOLLAPSEPROCEDURE_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_FIND_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_FINDSELECTION_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_FINDNEXT_ID] = ICON_FINDNEXT;
    ToolbarEditIcons[MENU_EDIT_FINDPREVIOUS_ID] = ICON_FINDPREV;
    ToolbarEditIcons[MENU_EDIT_REPLACE_ID] = ICON_REPLACE;
    ToolbarEditIcons[MENU_EDIT_PROCVARIABLESSEARCH_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_SEARCHINFILES_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_BOOKMARKS_TOGGLE_ID] = ICON_SETBOOK;
    ToolbarEditIcons[MENU_EDIT_BOOKMARKS_FIRST_ID] = ICON_FIRSTBOOK;
    ToolbarEditIcons[MENU_EDIT_BOOKMARKS_PREVIOUS_ID] = ICON_PREVBOOK;
    ToolbarEditIcons[MENU_EDIT_BOOKMARKS_NEXT_ID] = ICON_NEXTBOOK;
    ToolbarEditIcons[MENU_EDIT_BOOKMARKS_LAST_ID] = ICON_LASTBOOK;
    ToolbarEditIcons[MENU_EDIT_BOOKMARKS_CLEARALL_ID] = ICON_CLEARBOOK;
    ToolbarEditIcons[MENU_EDIT_GOTO_TOP_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_BOTTOM_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_LINE_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_MATCHINGBRACKET_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_PREVIOUSWORD_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_NEXTWORD_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_PREVIOUSPROCEDURE_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_NEXTPROCEDURE_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_VARIABLEDECLARATION_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_GOTO_VARIABLENEXTUSE_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_GOTO_PROCEDUREPROTOTYPE_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_GOTO_PROCEDUREDECLARATION_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_GOTO_PROCEDURENEXTUSE_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_BLOCK_INDENT_ID] = ICON_INDENT;
    ToolbarEditIcons[MENU_EDIT_BLOCK_UNINDENT_ID] = ICON_OUTDENT;
    ToolbarEditIcons[MENU_EDIT_BLOCK_UPPERCASE_ID] = ICON_UPPERCASE;
    ToolbarEditIcons[MENU_EDIT_BLOCK_LOWERCASE_ID] = ICON_LOWERCASE;
    ToolbarEditIcons[MENU_EDIT_BLOCK_TOGGLECASE_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_BLOCK_COMMENT_ID] = ICON_COMMENT;
    ToolbarEditIcons[MENU_EDIT_BLOCK_UNCOMMENT_ID] = ICON_UNCOMMENT;
    ToolbarEditIcons[MENU_EDIT_BLOCK_TABIFY_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_BLOCK_UNTABIFY_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_BLOCK_SPACESTOTABS_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_BLOCK_CONVERTTOINCLUDE_ID] = ICON_SAVE;
    ToolbarEditIcons[MENU_EDIT_BLOCK_REGISTERS_ID] = ICON_FIND;
    ToolbarEditIcons[MENU_EDIT_BLOCK_BLKCOMMENT_ID] = ICON_COMMENT;
    ToolbarEditIcons[MENU_EDIT_BLOCK_CUSTOMCOMMENT_ID] = ICON_COMMENT;
    ToolbarEditIcons[MENU_EDIT_BLOCK_SAVEAS_ID] = ICON_SAVE;
    ToolbarEditIcons[MENU_EDIT_BLOCK_PRINT_ID] = ICON_PRINT;
    ToolbarEditIcons[MENU_EDIT_PASTE_NEWWINDOW_ID] = ICON_PASTE;
    ToolbarEditIcons[MENU_EDIT_DELETE_DELETEPARAGRAPH_ID] = ICON_DELETE;
    ToolbarEditIcons[MENU_EDIT_GOTO_PREVIOUSPARAGRAPH_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_GOTO_NEXTPARAGRAPH_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_BLOCK_TRIMSPACES_ID] = ICON_WINDOWS;
    ToolbarEditIcons[MENU_EDIT_BREAKPOINTS_TOGGLE_ID] = ICON_SETBREAK;
    ToolbarEditIcons[MENU_EDIT_BREAKPOINTS_FIRST_ID] = ICON_FIRSTBREAK;
    ToolbarEditIcons[MENU_EDIT_BREAKPOINTS_PREVIOUS_ID] = ICON_PREVBREAK;
    ToolbarEditIcons[MENU_EDIT_BREAKPOINTS_NEXT_ID] = ICON_NEXTBREAK;
    ToolbarEditIcons[MENU_EDIT_BREAKPOINTS_LAST_ID] = ICON_LASTBREAK;
    ToolbarEditIcons[MENU_EDIT_BREAKPOINTS_CLEARALL_ID] = ICON_CLEARBREAK;

    ToolbarViewIcons[MENU_VIEW_STATUSBAR_ID] = ICON_STATUSBAR;
    ToolbarViewIcons[MENU_VIEW_PROJECTDOCK_ID] = ICON_CASCADE;
    ToolbarViewIcons[MENU_VIEW_WINDOWSDOCK_ID] = ICON_CASCADE;
    ToolbarViewIcons[MENU_VIEW_OUTPUTDOCK_ID] = ICON_CASCADE;
    ToolbarViewIcons[MENU_VIEW_DEBUGDOCK_ID] = ICON_CASCADE;
    ToolbarViewIcons[MENU_VIEW_FULLSCREEN_ID] = ICON_FULLSCREEN;
    ToolbarViewIcons[MENU_VIEW_PROPERTIES_ID] = ICON_PROPERTIES;
    ToolbarViewIcons[MENU_VIEW_USERMENUSEDITOR_ID] = ICON_MENUED;
    ToolbarViewIcons[MENU_VIEW_USERTOOLBARSEDITOR_ID] = ICON_TOOLBARS;
    
    ToolbarProjectIcons[MENU_PROJECT_CREATENEWPROJECT_ID] = ICON_NEWDIR;
    ToolbarProjectIcons[MENU_PROJECT_OPENPROJECT_ID] = ICON_OPEN;
    ToolbarProjectIcons[MENU_PROJECT_SAVEPROJECT_ID] = ICON_SAVE;
    ToolbarProjectIcons[MENU_PROJECT_SAVEPROJECTAS_ID] = ICON_SAVE;
    ToolbarProjectIcons[MENU_PROJECT_ADDINCLUDE_ID] = ICON_ADDFILES;
    ToolbarProjectIcons[MENU_PROJECT_ADDLIBRARY_ID] = ICON_ADDLIBRARY;
    ToolbarProjectIcons[MENU_PROJECT_ADDMODULE_ID] = ICON_ADDFILES;
    ToolbarProjectIcons[MENU_PROJECT_ADDOBJECT_ID] = ICON_ADDOBJECT;
    ToolbarProjectIcons[MENU_PROJECT_ADDRESOURCE_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_ADDTEXT_ID] = ICON_ADDFILES;
    ToolbarProjectIcons[MENU_PROJECT_ADDICON_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_ADDCURSOR_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_ADDBITMAP_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_ADDSTRINGS_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_ADDACCELERATORS_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_ADDMENUS_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_ADDDIALOG_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_ADDRAWDATA_ID] = ICON_ADDRESOURCE;
    ToolbarProjectIcons[MENU_PROJECT_PROJECTPROPERTIES_ID] = ICON_PROPERTIES;
    ToolbarProjectIcons[MENU_PROJECT_ASSEMBLEPROJECT_ID] = ICON_BUILDPROJECT;
    ToolbarProjectIcons[MENU_PROJECT_RUNPROJECT_ID] = ICON_RUNPROJECT;
    ToolbarProjectIcons[MENU_PROJECT_DEBUGPROJECT_ID] = ICON_DEBUGPROJECT;
    
    ToolbarWindowIcons[MENU_WINDOW_CONTEXTHELP_ID] = ICON_HELPFILES;
    ToolbarWindowIcons[MENU_WINDOW_MSDNHELP_ID] = ICON_HELPFILES;
    ToolbarWindowIcons[MENU_WINDOW_DDKHELP_ID] = ICON_HELPFILES;
    ToolbarWindowIcons[MENU_WINDOW_DIRECTXSDKHELP_ID] = ICON_HELPFILES;
    ToolbarWindowIcons[MENU_WINDOW_ABOUT_ID] = ICON_KEYBOARD;
    ToolbarWindowIcons[MENU_WINDOW_SPLIT_ID] = ICON_SPLIT;
    ToolbarWindowIcons[MENU_WINDOW_TILEHORIZONTALLY_ID] = ICON_TILEHORZ;
    ToolbarWindowIcons[MENU_WINDOW_TILEVERTICALLY_ID] = ICON_TILEVERT;
    ToolbarWindowIcons[MENU_WINDOW_CASCADE_ID] = ICON_CASCADE;
    ToolbarWindowIcons[MENU_WINDOW_ARRANGEICONS_ID] = ICON_WINDOWS;
    ToolbarWindowIcons[MENU_WINDOW_NEXTWINDOW_ID] = ICON_NEXTWINDOW;
    ToolbarWindowIcons[MENU_WINDOW_PREVIOUSWINDOW_ID] = ICON_PREVWINDOW;
    ToolbarWindowIcons[MENU_WINDOW_TIPOFTHEDAY_ID] = ICON_KEYBOARD;
}

// -----------------------------------------------------------------------
// Rebar hook
//LRESULT CALLBACK RebarHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//  RECT rect;

//  if(WindowsXP) {
//      switch(uMsg) {
//          case WM_ERASEBKGND:
//              GetClientRect(hWnd, &rect);
//              FillRect((HDC) wParam, &rect, GetSysColorBrush(COLOR_BTNFACE));
//              return(1);
//      }
//  }
  //  return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
//}

// -----------------------------------------------------------------------
// Refresh the toolbars
void RefreshToolbars(void)
{
    DestroyWindow(hRebar);
    hRebar = CreateRebar(0, 0, 0, 18, hMDIform.hWnd, 0, 0, CCS_TOP | RBS_BANDBORDERS | WS_BORDER);
    RebarResize(hRebar);
    InitReBarContent(hMDIform.hWnd);
}
