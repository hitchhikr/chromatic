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
// Dlg_SelFiles.cpp: Files selection form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "ProjectsBuilder.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_SelFiles.h"

// -----------------------------------------------------------------------
// Variables
CStr FRMFilesTitle;
long FRMFilesScheme;
long FRMFilesChoice;
HDC FRMFileshDC;
long InSelFile;

HWND FRMFilesListview;
HWND FRMFilesCmdYes;
HWND FRMFilesCmdNo;
HWND FRMFilesCmdCancel;
HWND FRMFilesCmdSelAll;
HWND FRMFilesCmdUnSelAll;
CList <int> FRMFilesSelArray;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMFilesProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    long LngCol = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMFilesListview, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            InSelFile = 1;
            FRMFilesChoice = IDCANCEL;
            ControlSetText(hwndDlg, FRMFilesTitle);
            switch(FRMFilesScheme)
            {
                case FILESSCHEME_EXTERNMODIFIED:
                    FRMFilesCmdYes = CreateButton(324, 1, 77, 23, hwndDlg, "Reload", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
                    FRMFilesCmdCancel = CreateButton(324, 25, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    FRMFilesCmdSelAll = CreateButton(324, 51, 77, 23, hwndDlg, "Select all", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    FRMFilesCmdUnSelAll = CreateButton(324, 75, 77, 23, hwndDlg, "Unselect all", 5, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    break;
                case FILESSCHEME_INTERNMODIFIED:
                    FRMFilesCmdYes = CreateButton(324, 1, 77, 23, hwndDlg, "Yes", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
                    FRMFilesCmdNo = CreateButton(324, 25, 77, 23, hwndDlg, "No", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    FRMFilesCmdCancel = CreateButton(324, 49, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    FRMFilesCmdSelAll = CreateButton(324, 75, 77, 23, hwndDlg, "Select all", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    FRMFilesCmdUnSelAll = CreateButton(324, 99, 77, 23, hwndDlg, "Unselect all", 5, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    break;
                case FILESSCHEME_AUTOSAVE:
                    FRMFilesCmdYes = CreateButton(324, 1, 77, 23, hwndDlg, "Save", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
                    FRMFilesCmdCancel = CreateButton(324, 25, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    FRMFilesCmdSelAll = CreateButton(324, 51, 77, 23, hwndDlg, "Select all", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    FRMFilesCmdUnSelAll = CreateButton(324, 75, 77, 23, hwndDlg, "Unselect all", 5, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
                    break;
            }
            FRMFilesListview = CreateListView(2, 2, 319, 240, hwndDlg, 6, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
            ListViewAddCol(FRMFilesListview, "", 320, 0);
            FRMFileshDC = GetDC(hwndDlg);
            LngCol = FillFilesList(FRMFileshDC);
            if(LngCol < 314) LngCol = 314;
            ListViewSetColWidth(FRMFilesListview, 0, LngCol);
            ReleaseDC(hwndDlg, FRMFileshDC);
            FreezeTimer = 1;
            ListViewSetItemSel(FRMFilesListview, 0);
            SetFocus(FRMFilesListview);
            return(0);
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMFilesListview)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        ListViewCheckBoxItemDoubleClick(FRMFilesListview);
                        return(0);
                }
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == FRMFilesCmdSelAll)
            {
                for(i = 0; i <= (long) ListViewItemCount(FRMFilesListview) - 1; i++)
                {
                    ListViewSetItemCheckbox(FRMFilesListview, i, 1);
                }
                return(0);
            }
            else if((HWND) lParam == FRMFilesCmdUnSelAll)
            {
                for(i = 0; i <= (long) ListViewItemCount(FRMFilesListview) - 1; i++)
                {
                    ListViewSetItemCheckbox(FRMFilesListview, i, 0);
                }
                return(0);
            }
            else if((HWND) lParam == FRMFilesCmdYes)
            {
                FRMFilesChoice = IDYES;
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFilesCmdNo)
            {
                FRMFilesChoice = IDNO;
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFilesCmdCancel)
            {
                FRMFilesChoice = IDCANCEL;
                ControlClose(hwndDlg);
                return(0);
            }
            break;
        case WM_CLOSE:
            FillFilesSelArray();
            FreezeTimer = 0;
            InSelFile = 0;
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Fill the listview with filenames
long FillFilesList(HDC hDC)
{
    int i = 0;
    long ReturnValue = 0;
    CStr FileName;
    SIZE FileNameSize;
    HGDIOBJ OldFontObj = 0;

    for(i = 0; i < ModifiedArrayhWnd.Amount(); i++)
    {
        if(ModifiedArrayhWnd.Get(i)->Content)
        {
            ChildStruct = LoadStructure(ModifiedArrayhWnd.Get(i)->Content);
            FileName = CMGetRealFile(ChildStruct->RFile);
            ListViewAddItem(FRMFilesListview, FileName, i + 1, 0);
            ListViewSetItemCheckbox(FRMFilesListview, i, 1);
        }
        else
        {
            FileName = ProjectFName;
            ListViewAddItem(FRMFilesListview, FileName, i + 1, 0);
            ListViewSetItemCheckbox(FRMFilesListview, i, 1);
        }
        // Get filename len to adapt listview control
        OldFontObj = SelectObject(hDC, WASerifFont);
        GetTextExtentPoint32(hDC, FileName.Get_String(), FileName.Len(), &FileNameSize);
        SelectObject(hDC, OldFontObj);
        if((long) ReturnValue < (long) (FileNameSize.cx + 30)) ReturnValue = (FileNameSize.cx + 30);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Fill the array with selected states
void FillFilesSelArray(void)
{
    int i = 0;
    
    for(i = 0; i < FRMFilesSelArray.Amount(); i++)
    {
        if(ListViewGetItemCheckbox(FRMFilesListview, i) == 1) FRMFilesSelArray.Set(i, 1);
    }
}
