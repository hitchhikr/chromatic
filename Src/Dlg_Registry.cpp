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
// Dlg_Registry.cpp: Registers extensions form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MiscFunctions.h"
#include "Globals.h"
#include "ConstRes.h"
#include "Languages.h"
#include "FilesExtensions.h"
#include "Dlg_Registry.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMRegsListview;
HWND FRMRegsCmdRegister;
HWND FRMRegsCmdUnRegister;
HWND FRMRegsCmdSelAll;
HWND FRMRegsCmdUnSelAll;
HWND FRMRegsCmdCancel;
CStr FRMRegsRetVal;
CStr FRMRegsLangName;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMRegsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMRegsListview, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, "Register files extensions");
            FRMRegsCmdRegister = CreateButton(256, 1, 77, 23, hwndDlg, "Register", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
            FRMRegsCmdUnRegister = CreateButton(256, 25, 77, 23, hwndDlg, "Unregister", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMRegsCmdCancel = CreateButton(256, 49, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMRegsCmdSelAll = CreateButton(256, 75, 77, 23, hwndDlg, "Select all", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMRegsCmdUnSelAll = CreateButton(256, 99, 77, 23, hwndDlg, "Unselect all", 5, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMRegsListview = CreateListView(2, 1, 251, 211, hwndDlg, 6, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | WS_TABSTOP | LVS_NOSORTHEADER, WS_EX_STATICEDGE);
            ListViewAddCol(FRMRegsListview, "Language", 145, 1);
            ListViewAddCol(FRMRegsListview, "Extension", 83, 0);
            FillExtLanguage();
            SetFocus(FRMRegsListview);
            return(0);
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMRegsListview)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        ListViewCheckBoxItemDoubleClick(FRMRegsListview);
                        return(0);
                }
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == FRMRegsCmdSelAll)
            {
                for(i = 0; i <= (long) ListViewItemCount(FRMRegsListview) - 1; i++)
                {
                    ListViewSetItemCheckbox(FRMRegsListview, i, 1);
                }
                return(0);
            }
            else if((HWND) lParam == FRMRegsCmdUnSelAll)
            {
                for(i = 0; i <= (long) ListViewItemCount(FRMRegsListview) - 1; i++)
                {
                    ListViewSetItemCheckbox(FRMRegsListview, i, 0);
                }
                return(0);
            }
            else if((HWND) lParam == FRMRegsCmdRegister)
            {
                RegisterExtensions();
                SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_FLUSHNOWAIT, NULL, NULL);
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMRegsCmdUnRegister)
            {
                UnRegisterExtensions();
                SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_FLUSHNOWAIT, NULL, NULL);
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMRegsCmdCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            break;
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Fill the languages extensions
void FillExtLanguage(void)
{
    long *ExtArray = 0;
    CStr LangToCheck;
    CStr RegEntry;
    CStr BufString;
    int i = 0;
    int j = 0;
    int k = 0;
    
    // Get default extensions (.med/.mws)
    ListViewAddItem(FRMRegsListview, "med", k, 0);
    RegEntry = RegistryGetKeyValue(HKEY_CLASSES_ROOT, ".med", "");
    if(RegEntry.Len() != 0)
    {
        if(_strcmpi(RegEntry.Get_String(), "Chromatic project file") == 0)
        {
            ListViewSetItemCheckbox(FRMRegsListview, k, 1);
        }
    }
    ListViewSetSubItem(FRMRegsListview, "(Chromatic projects)", k, 1);
    k++;
    ListViewAddItem(FRMRegsListview, "mws", k, 0);
    RegEntry = RegistryGetKeyValue(HKEY_CLASSES_ROOT, ".mws", "");
    if(RegEntry.Len() != 0)
    {
        if(_strcmpi(RegEntry.Get_String(), "Chromatic workspace file") == 0)
        {
            ListViewSetItemCheckbox(FRMRegsListview, k, 1);
        }
    }
    ListViewSetSubItem(FRMRegsListview, "(Chromatic workspaces)", k, 1);
    k++;
    for(i = 0; i <= 999; i++)
    {
        FRMRegsRetVal = IniReadKey("RefLanguages", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile).Trim();
        LangToCheck = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(FRMRegsRetVal.Len() == 0) break;
        LangToCheck = ChangeRelativePaths(LangToCheck);
        LangFileToRead = LangToCheck;
        FRMRegsLangName = IniReadKey("LangSpec", "LangName", LangFileToRead);
        if(FRMRegsLangName.Len() == 0)
        {
            LangToCheck = "<Error>";
        }
        else
        {
            LangToCheck = FRMRegsLangName;
        }
        ExtArray = StringSplit(FRMRegsRetVal, " ");
        if(StringGetSplitUBound(ExtArray) != -1)
        {
            for(j = 0; j <= StringGetSplitUBound(ExtArray); j++)
            {
                ListViewAddItem(FRMRegsListview, StringGetSplitElement(FRMRegsRetVal, ExtArray, j), k, 0);
                RegEntry = RegistryGetKeyValue(HKEY_CLASSES_ROOT, "." + (CStr) StringGetSplitElement(FRMRegsRetVal, ExtArray, j).Get_String(), "");
                if(RegEntry.Len() != 0)
                {
                    BufString = "Chromatic " + (CStr) StringGetSplitElement(FRMRegsRetVal, ExtArray, j).Get_String() + (CStr) " file";
                    if(_strcmpi(RegEntry.Get_String(), BufString.Get_String()) == 0)
                    {
                        ListViewSetItemCheckbox(FRMRegsListview, k, 1);
                    }
                }
                ListViewSetSubItem(FRMRegsListview, LangToCheck, k, 1);
                k++;
            }
        }
        StringReleaseSplit(ExtArray);
    }
}

// -----------------------------------------------------------------------
// Register all selected extensions
void RegisterExtensions(void)
{
    int i = 0;
    CStr BufString;

    for(i = 0; i <= (long) ListViewItemCount(FRMRegsListview) - 1; i++)
    {
        if(ListViewGetItemCheckbox(FRMRegsListview, i) == 1)
        {
            BufString = ListViewGetItemText(FRMRegsListview, i, 0).Upper_Case();
            if(BufString == "MED")
            {
                RegisterExt(ListViewGetItemText(FRMRegsListview, i, 0), ICON_ASM + 1);
            }
            else if(BufString == "MWS")
            {
                RegisterExt(ListViewGetItemText(FRMRegsListview, i, 0), ICON_KEYBOARD + 1);
            }
            else
            {
                RegisterExt(ListViewGetItemText(FRMRegsListview, i, 0), ICON_NEW + 1);
            }
        }
    }
}

// -----------------------------------------------------------------------
// Unregister all selected extensions
void UnRegisterExtensions(void)
{
    int i = 0;

    for(i = 0; i <= (long) ListViewItemCount(FRMRegsListview) - 1; i++)
    {
        if(ListViewGetItemCheckbox(FRMRegsListview, i) == 1)
        {
            UnRegisterExt(ListViewGetItemText(FRMRegsListview, i, 0));
        }
    }
}
