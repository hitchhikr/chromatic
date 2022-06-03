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
// Dlg_Favorites.cpp: Favorite files form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Globals.h"
#include "AddIns.h"
#include "Menus.h"
#include "MiscFunctions.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_Favorites.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMFavListView;
HWND FRMFavCmdSave;
HWND FRMFavCmdCancel;
HWND FRMFavCmdOpen;
HWND FRMFavToolbar;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMFavoritesProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT Ps;
    int i = 0;
    int j = 0;
    CStr NameToAdd;
    long CurrentSelected = 0;
    long *FavFilesArray = 0;
    CStr SelFavFiles;
    LV_KEYDOWN *lParamKey = 0;
    
    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMFavListView, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, "Favorite files");
            FRMFavCmdSave = CreateButton(327, 292, 77, 23, hwndDlg, "Save", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMFavCmdCancel = CreateButton(406, 292, 77, 23, hwndDlg, "Close", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMFavCmdOpen = CreateButton(243, 292, 77, 23, hwndDlg, "Open", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMFavToolbar = CreateToolBar(2, 1, 449, 23, hwndDlg, GlobalImageList1, 4, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            ToolBarAddButton(FRMFavToolbar, "", FAVADDFILES, ICON_ADDFILES, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddButton(FRMFavToolbar, "", FAVADDALLDIRECTORY, ICON_ADDDIRECTORY, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddButton(FRMFavToolbar, "", FAVADDACTIVEFILE, ICON_ADDACTIVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddButton(FRMFavToolbar, "", FAVADDALLACTIVEFILES, ICON_ADDALLACTIVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddSeparator(FRMFavToolbar, 0);
            ToolBarAddButton(FRMFavToolbar, "", FAVREMSELECTION, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            FRMFavListView = CreateListView(2, 26, 482, 253, hwndDlg, 5, GlobalImageList1, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | WS_TABSTOP | LVS_NOSORTHEADER, WS_EX_STATICEDGE);
            ListViewAddCol(FRMFavListView, "Path", 280, 0);
            ListViewAddCol(FRMFavListView, "Name", 180, 1);
            LoadFavList();
            FreezeTimer = 1;
            return(0);
        case WM_COMMAND:
            if((HWND) lParam == FRMFavCmdOpen)
            {
                OpenFavFiles();
                return(0);
            }
            else if((HWND) lParam == FRMFavCmdSave)
            {
                SaveFavList();
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFavCmdCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFavToolbar)
            {
                switch(wParam)
                {
                    case FAVADDFILES:
                        SelFavFiles = ComDlgGetOpenFileName(hwndDlg, "All files (*.*)|*.*","", 1, CurrentDir);
                        ComDlgParseMultiFilesSelection(SelFavFiles, &EnumFavFiles, MULTIFILESENUM_FORWARD, 0);
                        return(0);
                    case FAVADDALLDIRECTORY:
                        NameToAdd = ComDlgBrowseForFolder(hwndDlg, "Select the directory to add...");
                        if(NameToAdd.Len() != 0)
                        {
                            if(ListViewFindSubItem(FRMFavListView, NameToAdd, 1, CASE_UNSENSITIVE) == -1)
                            {
                                if(CheckFavFile(NameToAdd) == 0)
                                {
                                    j = ListViewItemCount(FRMFavListView);
                                    ListViewAddItem(FRMFavListView, NameToAdd, j, ICON_NEW);
                                    ListViewSetSubItem(FRMFavListView, NameToAdd, j, 1);
                                }
                            }
                        }
                        return(0);
                    case FAVADDACTIVEFILE:
                        if(NbForms != 0)
                        {
                            ChildStruct = LoadStructure(CurrentForm);
                            if(ChildStruct->FileLoaded == 1)
                            {
                                NameToAdd = CMGetRealFile(ChildStruct->RFile);
                                if(ListViewFindSubItem(FRMFavListView, NameToAdd, 1, CASE_UNSENSITIVE) == -1)
                                {
                                    if(CheckFavFile(NameToAdd) == 0)
                                    {
                                        j = ListViewItemCount(FRMFavListView);
                                        ListViewAddItem(FRMFavListView, FileGetFileName(NameToAdd), j, ICON_NEW);
                                        ListViewSetSubItem(FRMFavListView, NameToAdd, j, 1);
                                    }
                                }
                            }
                        }
                        return(0);
                    case FAVADDALLACTIVEFILES:
                        WAMMEnumSourceWindows((long) &AddAllActiveFilesProc);
                        return(0);
                    case FAVREMSELECTION:
                        RemFavFiles();
                        return(0);
                }
            }
            break;
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMFavListView)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                        case LVN_KEYDOWN:
                            lParamKey = (LV_KEYDOWN *) lParam;
                            if((lParamKey->wVKey & 0xffff) == 46) RemFavFiles();
                            break;
                        case NM_DBLCLK:
                            OpenFavFiles();
                            break;
                }
            }
            else
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case TTN_NEEDTEXT:
                        switch(ControlGetNotifiedID(lParam))
                        {
                            case FAVADDFILES:
                                ToolBarDisplayToolTip("Add file(s) into list", lParam);
                                return(0);
                            case FAVADDACTIVEFILE:
                                ToolBarDisplayToolTip("Add active file into list", lParam);
                                return(0);
                            case FAVADDALLACTIVEFILES:
                                ToolBarDisplayToolTip("Add all opened files into list", lParam);
                                return(0);
                            case FAVADDALLDIRECTORY:
                                ToolBarDisplayToolTip("Add a directory into list", lParam);
                                return(0);
                            case FAVREMSELECTION:
                                ToolBarDisplayToolTip("Remove selected file(s) from the list", lParam);
                                return(0);
                        }
                        break;
                }
            }
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &Ps);
            GDIDrawHorzSep(hwndDlg, 0, ControlClientHeight(hwndDlg) - 33, 486);
            EndPaint(hwndDlg, &Ps);
            break;
        case WM_CLOSE:
            CreateFavoritesMenu();
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Callback for WAMMEnumChilds
long CALLBACK AddAllActiveFilesProc(HWND hWnd)
{
    CStr NameToAdd;
    int i = 0;

    ChildStruct = LoadStructure(hWnd);
    if(ChildStruct->FileLoaded == 1)
    {
        NameToAdd = CMGetRealFile(ChildStruct->RFile);
        if(ListViewFindSubItem(FRMFavListView, NameToAdd, 1, CASE_UNSENSITIVE) == -1)
        {
            if(CheckFavFile(NameToAdd) == 0)
            {
                i = ListViewItemCount(FRMFavListView);
                ListViewAddItem(FRMFavListView, FileGetFileName(NameToAdd), i, ICON_NEW);
                ListViewSetSubItem(FRMFavListView, NameToAdd, i, 1);
            }
        }
    }
    return(1);
}

// -----------------------------------------------------------------------
// save the list of favorite files into main ini file
void SaveFavList(void)
{
    int i = 0;

    // Remove the section
    IniDeleteKey("Favorites", "", MainIniFile);
    // And reconstruct it
    for(i = 0; i <= ListViewItemCount(FRMFavListView) - 1; i++)
    {
        IniWriteKey("Favorites", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), ListViewGetItemText(FRMFavListView, i, 1), MainIniFile);
    }
}

// -----------------------------------------------------------------------
// Laod the list of favorite files from main ini file
void LoadFavList(void)
{
    int i = 0;
    int j = 0;
    CStr FileToAdd;

    ListViewClear(FRMFavListView);
    for(i = 0; i <= 999; i++)
    {
        FileToAdd = IniReadKey("Favorites", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(FileToAdd.Len() == 0) break;
        j = ListViewItemCount(FRMFavListView);
        ListViewAddItem(FRMFavListView, FileGetFileName(FileToAdd), j, ICON_NEW);
        ListViewSetSubItem(FRMFavListView, FileToAdd, j, 1);
    }
}

// -----------------------------------------------------------------------
// Check if an entry is already in favorite files list
long CheckFavFile(CStr FavFileToCheck)
{
    int i = 0;

    for(i = 0; i <= ListViewItemCount(FRMFavListView) - 1; i++)
    {
        if(_strcmpi(ListViewGetItemText(FRMFavListView, i, 1).Get_String(), FavFileToCheck.Get_String()) == 0)
        {
            return(1);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Open the selected favorites files
void OpenFavFiles(void)
{
    long CurrentSelected = 0;

    CurrentSelected = ListViewGetSelItem(FRMFavListView, -1);
    while(CurrentSelected != -1)
    {
        OpenUnknownFile(ListViewGetItemText(FRMFavListView, CurrentSelected, 1), TRUE);
        CurrentSelected = ListViewGetSelItem(FRMFavListView, CurrentSelected);
    }
}

// -----------------------------------------------------------------------
// Remove the selected favorites files
void RemFavFiles(void)
{
    long CurrentSelected;

    CurrentSelected = ListViewGetSelItem(FRMFavListView, -1);
    while(CurrentSelected != -1)
    {
        ListViewDeleteItem(FRMFavListView, CurrentSelected);
        CurrentSelected = ListViewGetSelItem(FRMFavListView, -1);
    }
}

// -----------------------------------------------------------------------
// Callback to add the selected favorites files
long CALLBACK EnumFavFiles(char *FileToAdd, long UserValue)
{
    CStr BufString;
    int j;

    BufString = FileToAdd;
    if(ListViewFindSubItem(FRMFavListView, BufString, 1, CASE_UNSENSITIVE) == -1)
    {
        if(CheckFavFile(BufString) == 0)
        {
            j = ListViewItemCount(FRMFavListView);
            ListViewAddItem(FRMFavListView, FileGetFileName(BufString), j, ICON_NEW);
            ListViewSetSubItem(FRMFavListView, BufString, j, 1);
        }
    }
    // Continue enumeration
    return(1);
}
