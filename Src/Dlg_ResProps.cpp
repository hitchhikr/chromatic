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
// Dlg_ResProps.cpp: Resources elements properties form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ProjectsBuilder.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "Splitters.h"
#include "MDI_Form.h"
#include "Dlg_ResProps.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMResPropshWnd;
long ResIndex;
long ResType;
HWND FRMResPropsOk;
HWND FRMResPropsCancel;
HWND FRMResPropsMem;
HWND FRMResPropsLoad;
HWND FRMResPropsMoveable;
HWND FRMResPropsDiscardable;
HWND FRMResPropsPure;
HWND FRMResPropsLanguage;
HWND FRMResPropsName;
CStr ResName;
HTREEITEM ResTree;

// -----------------------------------------------------------------------
// Display resource properties
void DisplayResProp(long Idx, long ResTp, CStr ResN, HTREEITEM hResTree)
{
    ResTree = hResTree;
    ResName = ResN;
    ResIndex = Idx;
    ResType = ResTp;
    CreateModalDialog(-1, -1, 230, 113, hMDIform.hWnd, &FRMResProps, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMResProps(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    int j = 0;
    CStr NewResName;
    long TmpProp = 0;
    long TmpLang = 0;
    long TmpLang2 = 0;
    PAINTSTRUCT EnterValuePerms;
    CStr LangName;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, "Change resource properties");
            FRMResPropshWnd = hwndDlg;
            FRMResPropsOk = CreateButton(71, 87, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMResPropsCancel = CreateButton(150, 87, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMResPropsName = CreateTextBox(2, 2, 225, 20, hwndDlg, ResName, 3, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
            FRMResPropsLoad = CreateCheckBox(3, 25, 80, 15, hwndDlg, "Load on call", 4, 0, WS_TABSTOP | WS_GROUP, 0);
            FRMResPropsMoveable = CreateCheckBox(3, 43, 80, 15, hwndDlg, "Moveable", 5, 0, WS_TABSTOP, 0);
            FRMResPropsDiscardable = CreateCheckBox(3, 61, 80, 15, hwndDlg, "Discardable", 6, 0, WS_TABSTOP, 0);
            FRMResPropsPure = CreateCheckBox(110, 25, 80, 15, hwndDlg, "Pure", 7, 0, WS_TABSTOP, 0);
            FRMResPropsLanguage = CreateComboBox(110, 48, 108, 90, hwndDlg, "", 8, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
            switch(ResType)
            {
                case RESPROP_ICON:
                    TmpLang = GetIconLang(ResIndex);
                    break;
                case RESPROP_CURSOR:
                    TmpLang = GetCursorLang(ResIndex);
                    break;
                case RESPROP_BITMAP:
                    TmpLang = GetBitmapLang(ResIndex);
                    break;
                case RESPROP_STRINGS:
                    TmpLang = Get_StringLang(ResIndex);
                    break;
                case RESPROP_ACCELERATORS:
                    TmpLang = GetAcceleratorLang(ResIndex);
                    break;
                case RESPROP_MENUS:
                    TmpLang = GetMenuLang(ResIndex);
                    break;
                case RESPROP_RAWDATA:
                    TmpLang = GetRawdataLang(ResIndex);
                    break;
            }
            j = 0;
            for(i = 0; i <= 87; i++)
            {
                if((long) TmpLang == i) TmpLang2 = j;
                if(strlen(TabLangRc[i].Get_String()) != 0)
                {
                    ComboBoxAddItem(FRMResPropsLanguage, TabLangRc[i], -1);
                    j++;
                }
            }
            switch(ResType)
            {
                case RESPROP_ICON:
                    TmpProp = GetIconProps(ResIndex);
                    ComboBoxSetIndex(FRMResPropsLanguage, TmpLang2);
                    break;
                case RESPROP_CURSOR:
                    TmpProp = GetCursorProps(ResIndex);
                    ComboBoxSetIndex(FRMResPropsLanguage, TmpLang2);
                    break;
                case RESPROP_BITMAP:
                    TmpProp = GetBitmapProps(ResIndex);
                    ComboBoxSetIndex(FRMResPropsLanguage, TmpLang2);
                    break;
                case RESPROP_STRINGS:
                    TmpProp = Get_StringProps(ResIndex);
                    ComboBoxSetIndex(FRMResPropsLanguage, TmpLang2);
                    break;
                case RESPROP_ACCELERATORS:
                    TmpProp = GetAcceleratorProps(ResIndex);
                    ComboBoxSetIndex(FRMResPropsLanguage, TmpLang2);
                    break;
                case RESPROP_MENUS:
                    TmpProp = GetMenuProps(ResIndex);
                    ComboBoxSetIndex(FRMResPropsLanguage, TmpLang2);
                    break;
                case RESPROP_RAWDATA:
                    TmpProp = GetRawdataProps(ResIndex);
                    ComboBoxSetIndex(FRMResPropsLanguage, TmpLang2);
                    break;
            }
            if((TmpProp & RESPROPDAT_LOAD) != 0) CheckBoxSetState(FRMResPropsLoad, 1);
            if((TmpProp & RESPROPDAT_MOVE) != 0) CheckBoxSetState(FRMResPropsMoveable, 1);
            if((TmpProp & RESPROPDAT_DISCARD) != 0) CheckBoxSetState(FRMResPropsDiscardable, 1);
            if((TmpProp & RESPROPDAT_PURE) != 0) CheckBoxSetState(FRMResPropsPure, 1);
            FreezeTimer = 1;
            ControlSendMessage(hwndDlg, (EN_CHANGE << 16), (long) FRMResPropsName);
            SetFocus(FRMResPropsName);
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &EnterValuePerms);
            GDIDrawHorzSep(hwndDlg, 0, 80, 240);
            EndPaint(hwndDlg, &EnterValuePerms);
            break;
        case WM_COMMAND:
            if((HWND) lParam == FRMResPropsOk)
            {
                NewResName = ControlGetText(FRMResPropsName);
                if(_strcmpi(ResName.Get_String(), NewResName.Get_String()) != 0)
                {
                    // Check first char
                    if(StringIsAlphaChar(NewResName.Mid(1, 1)) == 0)
                    {
                        if(StringIsUnderLineChar(NewResName.Mid(1, 1)) == 0)
                        {
                            MiscMsgBox(hwndDlg, "Illegal resource name.\rMust begin with an alphanumeric char or underline.", MB_ERROR, Requesters);
                            SetFocus(FRMResPropsName);
                            return(0);
                        }
                    }
                    if(StringIsLabel(NewResName, FALSE) == 0)
                    {
                        MiscMsgBox(hwndDlg, "Illegal resource name.\rAllowed chars are: A-Z a-z 0-9 _", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewIcons, NewResName + (CStr) " (") != -1)
                    {
                        MiscMsgBox(hwndDlg, "Resource name already exists.", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewCursors, NewResName + (CStr) " (") != -1)
                    {
                        MiscMsgBox(hwndDlg, "Resource name already exists.", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewBitmaps, NewResName + (CStr) " (") != -1)
                    {
                        MiscMsgBox(hwndDlg, "Resource name already exists.", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewStrings, NewResName + (CStr) " (") != -1)
                    {
                        MiscMsgBox(hwndDlg, "Resource name already exists.", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewAccelerators, NewResName + (CStr) " (") != -1)
                    {
                        MiscMsgBox(hwndDlg, "Resource name already exists.", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewMenus, NewResName + (CStr) " (") != -1)
                    {
                        MiscMsgBox(hwndDlg, "Resource name already exists.", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewDialogs, NewResName + (CStr) " (") != -1)
                    {
                        MiscMsgBox(hwndDlg, "Resource name already exists.", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewRawdatas, NewResName + (CStr) " (") != -1)
                    {
                        MiscMsgBox(hwndDlg, "Resource name already exists.", MB_ERROR, Requesters);
                        SetFocus(FRMResPropsName);
                        return(0);
                    }
                }
                NewResName = NewResName + (CStr) " (" + (CStr) GetFileNameFromTreeView(ResTree) + (CStr) ")";
                TreeViewSetItemText(hTreeView, ResTree, NewResName);
                TmpProp = 0;
                if(CheckBoxGetState(FRMResPropsLoad) == 1) TmpProp = TmpProp | RESPROPDAT_LOAD;
                if(CheckBoxGetState(FRMResPropsMoveable) == 1) TmpProp = TmpProp | RESPROPDAT_MOVE;
                if(CheckBoxGetState(FRMResPropsDiscardable) == 1) TmpProp = TmpProp | RESPROPDAT_DISCARD;
                if(CheckBoxGetState(FRMResPropsPure) == 1) TmpProp = TmpProp | RESPROPDAT_PURE;
                for(i = 0; i <= 87; i++)
                {
                    if(strlen(TabLangRc[i].Get_String()) != 0)
                    {
                        if(_strcmpi(TabLangRc[i].Get_String(), ComboBoxGetItem(FRMResPropsLanguage,
                                    ComboBoxGetIndex(FRMResPropsLanguage)).Get_String()) == 0) break;
                    }
                }
                switch(ResType)
                {
                    case RESPROP_ICON:
                        SetIconProps(ResIndex, TmpProp);
                        SetIconLang(ResIndex, i);
                        break;
                    case RESPROP_CURSOR:
                        SetCursorProps(ResIndex, TmpProp);
                        SetCursorLang(ResIndex, i);
                        break;
                    case RESPROP_BITMAP:
                        SetBitmapProps(ResIndex, TmpProp);
                        SetBitmapLang(ResIndex, i);
                        break;
                    case RESPROP_STRINGS:
                        Set_StringProps(ResIndex, TmpProp);
                        Set_StringLang(ResIndex, i);
                        break;
                    case RESPROP_ACCELERATORS:
                        SetAcceleratorProps(ResIndex, TmpProp);
                        SetAcceleratorLang(ResIndex, i);
                        break;
                    case RESPROP_MENUS:
                        SetMenuProps(ResIndex, TmpProp);
                        SetMenuLang(ResIndex, i);
                        break;
                    case RESPROP_RAWDATA:
                        SetRawDataProps(ResIndex, TmpProp);
                        SetRawDataLang(ResIndex, i);
                        break;
                }
                ProjectModified = TRUE;
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMResPropsCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMResPropsName)
            {
                switch(ControlGetNotifiedCommand(wParam))
                {
                    case EN_CHANGE:
                        if(TextBoxGetLen(FRMResPropsName) > 0)
                        {
                            ControlEnable(FRMResPropsOk, 1);
                        }
                        else
                        {
                            ControlEnable(FRMResPropsOk, 0);
                        }
                        break;
                }
            }
            break;
        case WM_CLOSE:
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}
