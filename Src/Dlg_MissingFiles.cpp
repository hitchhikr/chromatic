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
// Dlg_MissingFiles.cpp: Projects missing files report
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ProjectsBuilder.h"
#include "Globals.h"
#include "ConstRes.h"
#include "Splitters.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_MissingFiles.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMMissingFileshwnd;
HWND FRMMissingFilesListview;
HWND FRMMissingFilesCmdClose;
HWND FRMMissingFilesCmdToFile;
HWND FRMMissingFilesCmdToDoc;
HWND FRMMissingFilesCmdBrowse;

// -----------------------------------------------------------------------
// Private functions
void ReplaceMissingFile(HWND hWnd);

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMMissingFiles(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long LngCol = 0;
    HDC FRMMissingFileshDC = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMMissingFilesListview, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            FRMMissingFileshwnd = hwndDlg;
            ControlSetText(hwndDlg, "Missing files");
            FRMMissingFilesListview = CreateListView(2, 1, 318, 201, hwndDlg, 0, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
            ListViewAddCol(FRMMissingFilesListview, "", 320, 0);
            FRMMissingFilesCmdClose = CreateButton(323, 1, 77, 23, hwndDlg, "Close", 2, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
            FRMMissingFilesCmdToFile = CreateButton(323, 26, 77, 23, hwndDlg, "To file", 3, 0, 0, 0, WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
            FRMMissingFilesCmdToDoc = CreateButton(323, 50, 77, 23, hwndDlg, "To doc", 4, 0, 0, 0, WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
            FRMMissingFilesCmdBrowse = CreateButton(323, 75, 77, 23, hwndDlg, "Replace...", 5, 0, 0, 0, WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
            FRMMissingFileshDC = GetDC(hwndDlg);
            LngCol = FillMissingFilesList(FRMMissingFileshDC);
            ReleaseDC(hwndDlg, FRMMissingFileshDC);
            if(LngCol < 314) LngCol = 314;
            ListViewSetColWidth(FRMMissingFilesListview, 0, LngCol);
            ListViewSetItemSel(FRMMissingFilesListview, 0);
            FreezeTimer = 1;
            SetFocus(FRMMissingFilesListview);
            return(0);
        case WM_COMMAND:
            if((HWND) lParam == FRMMissingFilesCmdClose)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMMissingFilesCmdToFile)
            {
                NewMissingFilesSave();
                return(0);
            }
            else if((HWND) lParam == FRMMissingFilesCmdToDoc)
            {
                NewMissingFilesDoc();
                return(0);
            }
            else if((HWND) lParam == FRMMissingFilesCmdBrowse)
            {
                ReplaceMissingFile(hwndDlg);
                return(0);
            }
            break;
        case WM_CLOSE:
            ProjectModified = TRUE;
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Fill the listview with filenames
long FillMissingFilesList(HDC hDC)
{
    int i = 0;
    long ReturnValue = 0;
    CStr FileName;
    SIZE FileNameSize;
    HGDIOBJ OldFontObj = 0;

    for(i = 0; i < PossibleMissingFiles.Amount(); i++)
    {
        FileName = PossibleMissingFiles.Get(i)->Content;
        ListViewAddItem(FRMMissingFilesListview, FileName, i, 0);
        // Get filename len to adapt listview control
        OldFontObj = SelectObject(hDC, WASerifFont);
        GetTextExtentPoint32(hDC, FileName.Get_String(), FileName.Len(), &FileNameSize);
        SelectObject(hDC, OldFontObj);
        if((long) ReturnValue < (long) (FileNameSize.cx + 30)) ReturnValue = (FileNameSize.cx + 30);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Save results into a file
void NewMissingFilesSave(void)
{
    CStr TextToAdd;
    CStr FileName;
    CStr Filters;
    CStr FName;
    int i;

    Filters = "All files (*.*)|*.*";
    FName = ComDlgGetSaveFileName(FRMMissingFileshwnd, Filters, "", CurrentDir);
    if(FName.Len() != 0)
    {
        TextToAdd = "Missing files for project: " + ProjectFName;
        TextToAdd = TextToAdd + (CStr) "\r\n" + (CStr) TextToAdd.String(TextToAdd.Len(),'-').Get_String() + (CStr) "\r\n\r\n";
        for(i = 0; i < PossibleMissingFiles.Amount(); i++)
        {
            FileName = PossibleMissingFiles.Get(i)->Content;
            TextToAdd = TextToAdd + FileName + (CStr) "\r\n";
        }
        if(MSaveFile(FName.Get_String(), (long) TextToAdd.Get_String(), TextToAdd.Len()) == 0)
        {
            MiscMsgBox(FRMMissingFileshwnd, "Error in saving file: '" + (CStr) FName + (CStr) "'.", MB_ERROR, Requesters);
        }
        else
        {
            MiscMsgBox(FRMMissingFileshwnd, "Results saved as '" + (CStr) FName + (CStr) "'.", MB_INFORMATION, Requesters);
        }
    }
    return;
}

// -----------------------------------------------------------------------
// Create a new window with results
void NewMissingFilesDoc(void)
{
    HWND NewChildHandle;
    CStr TextToAdd;
    CStr FileName;
    int i;

    StoreLanguageToOpen("");
    NewChildHandle = CreateNewFile("Missing files.txt");
    if(NewChildHandle == 0)
    {
        MiscMsgBox(FRMMissingFileshwnd, "Can't create new window.", MB_ERROR, Requesters);
        return;
    }
    ForceChildFile(NewChildHandle);
    TextToAdd = "Missing files for project: " + ProjectFName;
    TextToAdd = TextToAdd + (CStr) "\r\n" + (CStr) TextToAdd.String(TextToAdd.Len(), '-').Get_String() + (CStr) "\r\n\r\n";
    for(i = 0; i < PossibleMissingFiles.Amount(); i++)
    {
        FileName = PossibleMissingFiles.Get(i)->Content;
        TextToAdd = TextToAdd + FileName + (CStr) "\r\n";
    }
    SendTextToChild(NewChildHandle, TextToAdd);
    return;
}

// -----------------------------------------------------------------------
// Replace selected listbox entry with selected file
void ReplaceMissingFile(HWND hWnd)
{
    CStr ItemToReplace;
    CStr LdFile;
    HTREEITEM OrigItem;
    char *OrigResourceName;
    int OrigResourceProp;
    int OrigResourceLang;
    CStr EntryToAdd;

    int SelItem = ListViewGetSelItem(FRMMissingFilesListview, -1);
    if(SelItem != -1)
    {
        ItemToReplace = ListViewGetItemText(FRMMissingFilesListview, SelItem, 0);
        LdFile = ComDlgGetOpenFileName(hWnd, "All files (*.*)|*.*", "", FALSE, ItemToReplace.Extract_Directory());
        if(LdFile.Len() == 0) return;
        OrigItem = PossibleMissingFilesTree.Get(SelItem)->Content;
        OrigResourceName = PossibleMissingFilesResName.Get(SelItem)->Content;
        OrigResourceProp = PossibleMissingFilesResProp.Get(SelItem)->Content;
        OrigResourceLang = PossibleMissingFilesResLang.Get(SelItem)->Content;
        EntryToAdd = FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")";

        if(OrigItem == hTreeViewIncludes)
        {
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_NEW, ICON_NEW, 0, 0);
        }
        else if(OrigItem == hTreeViewLibs)
        {
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_STATIC, ICON_STATIC, 0, 0);
        }
        else if(OrigItem == hTreeViewModules)
        {
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_NEW, ICON_NEW, 0, 0);
        }
        else if(OrigItem == hTreeViewObjects)
        {
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_OBJECT, ICON_OBJECT, 0, 0);
        }
        else if(OrigItem == hTreeViewResources)
        {
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);

        // Resources
        }
        else if(OrigItem == hTreeViewIcons)
        {
            EntryToAdd = OrigResourceName + (CStr) " (" + (CStr) LdFile + (CStr) ")";
            AddIconInArray(OrigResourceProp, OrigResourceLang);
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        }
        else if(OrigItem == hTreeViewCursors)
        {
            EntryToAdd = OrigResourceName + (CStr) " (" + (CStr) LdFile + (CStr) ")";
            AddCursorInArray(OrigResourceProp, OrigResourceLang);
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        }
        else if(OrigItem == hTreeViewBitmaps)
        {
            EntryToAdd = OrigResourceName + (CStr) " (" + (CStr) LdFile + (CStr) ")";
            AddBitmapInArray(OrigResourceProp, OrigResourceLang);
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        }
        else if(OrigItem == hTreeViewStrings)
        {
            EntryToAdd = OrigResourceName + (CStr) " (" + (CStr) LdFile + (CStr) ")";
            AddStringInArray(OrigResourceProp, OrigResourceLang);
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        }
        else if(OrigItem == hTreeViewAccelerators)
        {
            EntryToAdd = OrigResourceName + (CStr) " (" + (CStr) LdFile + (CStr) ")";
            AddAcceleratorInArray(OrigResourceProp, OrigResourceLang);
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        }
        else if(OrigItem == hTreeViewMenus)
        {
            EntryToAdd = OrigResourceName + (CStr) " (" + (CStr) LdFile + (CStr) ")";
            AddMenuInArray(OrigResourceProp, OrigResourceLang);
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        }
        else if(OrigItem == hTreeViewDialogs)
        {
            EntryToAdd = OrigResourceName + (CStr) " (" + (CStr) LdFile + (CStr) ")";
            AddDialogInArray(OrigResourceProp, OrigResourceLang);
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        }
        else if(OrigItem == hTreeViewRawdatas)
        {
            EntryToAdd = OrigResourceName + (CStr) " (" + (CStr) LdFile + (CStr) ")";
            AddRawDatasInArray(OrigResourceProp, OrigResourceLang);
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);

        }
        else if(OrigItem == hTreeViewDefs)
        {
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        }
        else if(OrigItem == hTreeViewTexts)
        {
            TreeViewAddItem(hTreeView, EntryToAdd, OrigItem, 0, ICON_RES, ICON_RES, 0, 0);
        } 

        PossibleMissingFiles.Del(SelItem);
        ListViewDeleteItem(FRMMissingFilesListview, SelItem);
        if(ListViewItemCount(FRMMissingFilesListview) == 0)
        {
            // Exit if all files has been corrected
            ControlClose(hWnd);
            return;
        }
        ListViewSetItemSel(FRMMissingFilesListview, 0);
    }
}
