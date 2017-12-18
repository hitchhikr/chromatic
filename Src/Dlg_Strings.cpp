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
// Dlg_Strings.cpp: Strings resource creation
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Splitters.h"
#include "Globals.h"
#include "PRojectsBuilder.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "MDI_Form.h"
#include "Dlg_Strings.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMStringshwnd;
HWND hFRMStringsOk;
HWND hFRMStringsCancel;
CStr StringsTitle;
HWND FRMStringsListview;
long StringBase;
long EditStrings;
long LVStringsInEdit;
HANDLE StringsFileHandle;
CStr StringsFileToEdit;
CStr StringBaseToEdit;

// -----------------------------------------------------------------------
// Create an empty string resource
void CreateStrings(void)
{
    long EntryStrCount = 0;

    EditStrings = 0;
    EntryStrCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewStrings);
    // Check if entry already exists
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewStrings, "STRINGS_" + (CStr) EntryStrCount) != -1)
    {
        EntryStrCount++;
    }
    StringBase = EntryStrCount;
    StringsTitle = "Create new strings resource";
    CreateModalDialog(-1, -1, 410, 289, hMDIform.hWnd, &FRMStringsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Edit strings resource
void EditStringsTable(CStr StringEditBase, CStr StringsFile, long Base)
{
    EditStrings = 1;
    StringsTitle = "Edit strings resource";
    StringBaseToEdit = StringEditBase;
    StringsFileToEdit = StringsFile;
    StringBase = Base;
    CreateModalDialog(-1, -1, 410, 289, hMDIform.hWnd, &FRMStringsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMStringsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    PAINTSTRUCT EnterValuePs;
    long CurrentSelected = 0;
    CStr CurrentStringFromStrings;
    
    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMStringsListview, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, StringsTitle);
            FRMStringshwnd = hwndDlg;
            hFRMStringsOk = CreateButton(251, 263, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            hFRMStringsCancel = CreateButton(330, 263, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMStringsListview = CreateListView(2, 1, 406, 250, hwndDlg, 3, GlobalImageList3, &LVStringsHook, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_NOSORTHEADER | LVS_EDITLABELS | LVS_SINGLESEL | WS_TABSTOP, WS_EX_STATICEDGE);
            ListViewAddCol(FRMStringsListview, "String ID", 80, 1);
            ListViewAddCol(FRMStringsListview, "Value", 303, 0);
            if(EditStrings == 0)
            {
                for(i = 0; i <= 15; i++)
                {
                    ListViewAddItem(FRMStringsListview, "", i, -1);
                }
                for(i = 0; i <= 15; i++)
                {
                    ListViewSetSubItem(FRMStringsListview, (i + (StringBase * 16)), i, 1);
                }
            }
            else
            {
                for(i = 0; i <= 15; i++)
                {
                    CurrentStringFromStrings = IniReadKey("STRDAT", "STR" + (CStr) StringNumberComplement(i, 3).Get_String(), StringsFileToEdit);
                    ListViewAddItem(FRMStringsListview, CurrentStringFromStrings, i, -1);
                }
                for(i = 0; i <= 15; i++)
                {
                    ListViewSetSubItem(FRMStringsListview, (i + (StringBase * 16)), i, 1);
                }
            }
            ListViewSetHeaderPosition(FRMStringsListview, 0, 1);
            ListViewSetHeaderPosition(FRMStringsListview, 1, 0);
            ListViewSetItemSel(FRMStringsListview, 0);
            SetFocus(FRMStringsListview);
            FreezeTimer = 1;
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &EnterValuePs);
            GDIDrawHorzSep(hwndDlg, 0, 256, 410);
            EndPaint(hwndDlg, &EnterValuePs);
            break;
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMStringsListview)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        CurrentSelected = ListViewGetSelItem(FRMStringsListview, -1);
                        if(CurrentSelected != -1)
                        {
                            ListViewSetEditModeOn(FRMStringsListview, CurrentSelected, 0);
                            LVStringsInEdit = 1;
                        }
                        return(0);
                    case LVN_ENDLABELEDIT:
                        CurrentSelected = ListViewGetSelItem(FRMStringsListview, -1);
                        if(CurrentSelected != -1)
                        {
                            if(ListViewEditValidated(lParam) == 1)
                            {
                                ListViewSetSubItem(FRMStringsListview, ListViewGetEditResult(lParam), CurrentSelected, 0);
                            }
                        }
                        LVStringsInEdit = 0;
                        return(0);
                }
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == hFRMStringsOk)
            {
                if(EditStrings == 0)
                {
                    CreateNewStringsFile();
                    ProjectModified = TRUE;
                }
                else
                {
                    SaveStringsFile();
                }
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == hFRMStringsCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            break;
        case WM_CLOSE:
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Create a new strings ini file for current project
void CreateNewStringsFile(void)
{
    int i = 0;
    HANDLE StrFileHandle = 0;
    CStr StrFileName;
    CStr StrName;
    CStr EntryToAdd;

    StrName = "STRINGS_" + (CStr) StringBase;
    StrFileName = FileGetDirectory(ProjectFName) + (CStr) StrName + (CStr) ".str";
    StrFileHandle = FileCreateEmpty(StrFileName, NO_SECURITY_DESCRIPTOR);
    FileWriteLine(StrFileHandle, "; Chromatic resource strings file");
    FileWriteLine(StrFileHandle, "; Do NOT edit this file manually");
    FileWriteLine(StrFileHandle, "; unless you know what you're doing !!!");
    FileWriteLine(StrFileHandle, "; (C) 2001-2010 Franck Charlet.");
    FileWriteLine(StrFileHandle, "");
    FileWriteLine(StrFileHandle, "[STRDAT]");
    FileClose(StrFileHandle);
    EntryToAdd = StrName + (CStr) " (" + (CStr) StrFileName + (CStr) ")";
    for(i = 0; i <= 15; i++)
    {
        IniWriteKey("STRDAT", "STR" + (CStr) StringNumberComplement(i, 3).Get_String(),
                      ListViewGetItemText(FRMStringsListview, i, 0), StrFileName);
    }
    AddStringInArray(RESPROPS_DEFAULT, 0);
    TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewStrings, 0, ICON_RES, ICON_RES, 0, 1);
}

// -----------------------------------------------------------------------
// Save a strings ini file for current project
void SaveStringsFile(void)
{
    int i = 0;
    HANDLE StrFileHandle = 0;

    StrFileHandle = FileCreateEmpty(StringsFileToEdit, NO_SECURITY_DESCRIPTOR);
    if(StrFileHandle != INVALID_HANDLE_VALUE)
    {
        FileWriteLine(StrFileHandle, "; Chromatic resource strings file");
        FileWriteLine(StrFileHandle, "; Do NOT edit this file manually");
        FileWriteLine(StrFileHandle, "; unless you know what you're doing !!!");
        FileWriteLine(StrFileHandle, "; (C) 2001-2010 Franck Charlet.");
        FileWriteLine(StrFileHandle, "");
        FileWriteLine(StrFileHandle, "[STRDAT]");
        FileClose(StrFileHandle);
        for(i = 0; i <= 15; i++)
        {
            IniWriteKey("STRDAT", "STR" + (CStr) StringNumberComplement(i, 3).Get_String(), ListViewGetItemText(FRMStringsListview, i, 0), StringsFileToEdit);
        }
    }
    else
    {
        MiscMsgBox(FRMStringshwnd, "Can't save strings file.", MB_ERROR, Requesters);
    }
}

// -----------------------------------------------------------------------
// Listview hook
LRESULT CALLBACK LVStringsHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(LVStringsInEdit == 0)
    {
        switch(uMsg)
        {
            case WM_CHAR:
                if(ListViewPasteAutoEdit(FRMStringsListview, wParam, 0) == 1)
                {
                    LVStringsInEdit = 1;
                    return(0);
                }
        }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}
