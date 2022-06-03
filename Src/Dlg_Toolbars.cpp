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
// Dlg_Toolbars.cpp: Toolbars editor
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "ConstMenus.h"
#include "Menus.h"
#include "Toolbars.h"
#include "MDI_Form.h"
#include "Dlg_Toolbars.h"
#include "Dlg_EnterValue.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMToolbarshwnd;
HWND FRMToolbarsSysTab;
HWND FRMToolbarsCmdClose;
HWND FRMToolbarsListview;
HWND FRMToolbarsCmdNew;
HWND FRMToolbarsCmdRename;
HWND FRMToolbarsCmdDelete;
HWND FRMToolbarsCombo;
HWND FRMToolbarsCheckB;
HWND FRMToolbarsLabel1;
HWND FRMToolbarsListDrop;
HWND FRMToolbarsListDrag;
long LVDrag1;
long LVDrag2;
POINT LVDragPoint;
HICON DragIcon;
CStr DragText;
long DragIndex;
CList <int> TBArray;
CList <int> TBArrayBreak;
long BegDrag;
long NbrTmp;
CList <int> TBIconsArray;
CList <char *> TBToolsArray;
CList <char *> TBBtnCommentArray;
long OldComboIndex;
CStr OldComboItem;
CStr OldToolbarName;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMToolbarsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT HexPs;
    long CurrentLvItem = 0;
    CStr ToolbarNName;
    long ItemToAddDbl = 0;
    CStr TmpKey;
    LV_HITTESTINFO MyTest;
    long TMPToolbar = 0;
    CStr TMPToolbarStr;
    CStr NTBNumber;
    int i = 0;
    int j = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMToolbarsListview, GetSysColor(COLOR_WINDOW));
            ListViewSetBackColor(FRMToolbarsListDrag, GetSysColor(COLOR_WINDOW));
            ListViewSetBackColor(FRMToolbarsListDrop, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            FRMToolbarshwnd = hwndDlg;
            DeleteFile(ToolbarsTMPIniFile.Get_String());
            ControlSetText(hwndDlg, "Toolbars manager");
            FRMToolbarsCmdClose = CreateButton(279, 275, 77, 23, hwndDlg, "Close", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP, Buttons_StaticEdge);
            FRMToolbarsListview = CreateListView(6 - 3, 26, 272, 236, hwndDlg, 2, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
            ListViewAddCol(FRMToolbarsListview, "", 250, 0);
            FillToolbarsNames();
            FRMToolbarsCmdNew = CreateButton(279, 26, 77, 23, hwndDlg, "New", 3, 0, 0, 0, WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
            FRMToolbarsCmdRename = CreateButton(279, 52, 77, 23, hwndDlg, "Rename", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMToolbarsCmdDelete = CreateButton(279, 76, 77, 23, hwndDlg, "Delete", 5, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMToolbarsCombo = CreateComboBox(6 - 3, 26, 188, 150, hwndDlg, "", 6, 0, WS_TABSTOP | CBS_DROPDOWNLIST);
            FRMToolbarsCheckB = CreateCheckBox(195, 29, 130, 15, hwndDlg, "Toolbar on new line", 7, 0, WS_TABSTOP, 0);
            FRMToolbarsFillCombo();
            FRMToolbarsLabel1 = CreateLabel(6 - 3, 50, 188, 15, hwndDlg, "Available commands :", 0, 0, 0, 0);
            FRMToolbarsListDrag = CreateListView(6 - 3, 64, 188, 198, hwndDlg, 8, GlobalImageList1, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
            FRMToolbarsListDrop = CreateListView(196 - 3 + 1, 51, 146 + 15, 211, hwndDlg, 9, GlobalImageList1, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
            ListViewAddCol(FRMToolbarsListDrag, "", 250, 3);
            ListViewAddCol(FRMToolbarsListDrop, "", 250, 3);
            ControlVisible(FRMToolbarsCombo, 0);
            ControlVisible(FRMToolbarsLabel1, 0);
            ControlVisible(FRMToolbarsListDrag, 0);
            ControlVisible(FRMToolbarsListDrop, 0);
            ControlVisible(FRMToolbarsCheckB, 0);
            FRMToolbarsSysTab = CreateSysTab(1, 1, 345, 266, hwndDlg, 10, 0, GlobalImageList1, WS_TABSTOP | TCS_BUTTONS | TCS_FLATBUTTONS | TCS_HOTTRACK);
            SysTabSetSeparators(FRMToolbarsSysTab, 0);
            SysTabAddItem(FRMToolbarsSysTab, "Toolbars", 0, ICON_TOOLBARS);
            SysTabAddItem(FRMToolbarsSysTab, "Customize", 1, ICON_CUSTTOOLBARS);
            FillToolbarCommands();
            FreezeTimer = 1;
            return(1);
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMToolbarsListview)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        CurrentLvItem = ListViewGetItemUnderCursor(FRMToolbarsListview);
                        if(CurrentLvItem != -1)
                        {
                            ListViewSetItemSel(FRMToolbarsListview, CurrentLvItem);
                            switch(ListViewGetItemCheckbox(FRMToolbarsListview, CurrentLvItem))
                            {
                                case 0:
                                    ListViewSetItemCheckbox(FRMToolbarsListview, CurrentLvItem, 1);
                                    ChangeToolbarVisible(ListViewGetSelItemText(FRMToolbarsListview, 0), 1);
                                    break;
                                case 1:
                                    ListViewSetItemCheckbox(FRMToolbarsListview, CurrentLvItem, 0);
                                    ChangeToolbarVisible(ListViewGetSelItemText(FRMToolbarsListview, 0), 0);
                                    break;
                            }
                        }
                        return(0);
                }
            }
            switch(ControlGetNotifiedMsg(lParam))
            {
                case TCN_SELCHANGE:
                    if(ControlGetNotifiedhWnd(lParam) == FRMToolbarsSysTab)
                    {
                        switch(SysTabGetCurrentItem(FRMToolbarsSysTab))
                        {
                            case 0:
                                OldComboIndex = ComboBoxGetIndex(FRMToolbarsCombo);
                                OldComboItem = ComboBoxGetItem(FRMToolbarsCombo, OldComboIndex);
                                ControlVisible(FRMToolbarsCombo, 0);
                                ControlVisible(FRMToolbarsLabel1, 0);
                                ControlVisible(FRMToolbarsListDrag, 0);
                                ControlVisible(FRMToolbarsListDrop, 0);
                                ControlVisible(FRMToolbarsCheckB, 0);
                                ControlVisible(FRMToolbarsListview, 1);
                                ControlVisible(FRMToolbarsCmdNew, 1);
                                ControlVisible(FRMToolbarsCmdRename, 1);
                                ControlVisible(FRMToolbarsCmdDelete, 1);
                                break;
                            case 1:
                                FRMToolbarsFillCombo();
                                // Check if selected toolbar still exists
                                // If not try to display first toolbar in list
                                if(ComboBoxItemExist(FRMToolbarsCombo, OldComboItem) == CB_ERR)
                                {
                                    OldComboIndex = 0;
                                }
                                ComboBoxSetIndex(FRMToolbarsCombo, OldComboIndex);
                                FillToolbarDatas();
                                ControlVisible(FRMToolbarsListview, 0);
                                ControlVisible(FRMToolbarsCmdNew, 0);
                                ControlVisible(FRMToolbarsCmdRename, 0);
                                ControlVisible(FRMToolbarsCmdDelete, 0);
                                ControlVisible(FRMToolbarsCombo, 1);
                                ControlVisible(FRMToolbarsLabel1, 1);
                                ControlVisible(FRMToolbarsListDrag, 1);
                                ControlVisible(FRMToolbarsListDrop, 1);
                                ControlVisible(FRMToolbarsCheckB, 1);
                        }
                    }
                    return(0);
                case LVN_BEGINDRAG:
                    if(ControlGetNotifiedhWnd(lParam) == FRMToolbarsListDrag)
                    {
                        LVDrag1 = ListViewBeginDrag(lParam, hwndDlg, &LVDragPoint);
                        DragIndex = ListViewGetSelItem(FRMToolbarsListDrag, -1);
                        BegDrag = DragIndex;
                        DragIcon = ListViewGetItemIcon(FRMToolbarsListDrag, DragIndex, 0);
                        DragText = ListViewGetItemText(FRMToolbarsListDrag, DragIndex, 0);
                    }
                    if(ControlGetNotifiedhWnd(lParam) == FRMToolbarsListDrop)
                    {
                        LVDrag2 = ListViewBeginDrag(lParam, hwndDlg, &LVDragPoint);
                        DragIndex = ListViewGetSelItem(FRMToolbarsListDrop, -1);
                        BegDrag = DragIndex;
                        DragIcon = ListViewGetItemIcon(FRMToolbarsListDrop, DragIndex, 0);
                        DragText = ListViewGetItemText(FRMToolbarsListDrop, DragIndex, 0);
                    }
                    return(0);
                case NM_DBLCLK:
                    if(ControlGetNotifiedhWnd(lParam) == FRMToolbarsListDrag)
                    {
                        DragIndex = ListViewGetSelItem(FRMToolbarsListDrag, -1);
                        ItemToAddDbl = ListViewItemCount(FRMToolbarsListDrop);
                        ListViewAddItem(FRMToolbarsListDrop, ListViewGetSelItemText(FRMToolbarsListDrag, 0), ItemToAddDbl, (long) ListViewGetItemIcon(FRMToolbarsListDrag, DragIndex, 0));
                        TmpKey = ComboBoxGetItem(FRMToolbarsCombo, ComboBoxGetIndex(FRMToolbarsCombo));
                        // Add an entry into the temp file
                        TMPToolbar = IniReadKey(TmpKey, "Number", ToolbarsTMPIniFile).Get_Long();
                        TMPToolbar++;
                        IniWriteKey(TmpKey, "Number", TMPToolbar, ToolbarsTMPIniFile);
                        IniWriteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(ItemToAddDbl, 3).Get_String(), MenuRealCommandsID.Get(DragIndex)->Content, ToolbarsTMPIniFile);
                        return(0);
                    }
                    else if(ControlGetNotifiedhWnd(lParam) == FRMToolbarsListDrop)
                    {
                        DragIndex = ListViewGetSelItem(FRMToolbarsListDrop, -1);
                        if(DragIndex != -1)
                        {
                            ListViewDeleteItem(FRMToolbarsListDrop, DragIndex);
                            TmpKey = ComboBoxGetItem(FRMToolbarsCombo, ComboBoxGetIndex(FRMToolbarsCombo));
                            // Remove an entry from the temp file
                            IniDeleteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(DragIndex, 3).Get_String(), ToolbarsTMPIniFile);
                            TMPToolbar = IniReadKey(TmpKey, "Number", ToolbarsTMPIniFile).Get_Long();
                            TMPToolbar--;
                            IniWriteKey(TmpKey, "Number", TMPToolbar, ToolbarsTMPIniFile);
                            for(j = DragIndex + 1; j <= TMPToolbar; j++)
                            {
                                TMPToolbarStr = IniReadKey(TmpKey, "Tool" + (CStr) StringNumberComplement(j, 3).Get_String(), ToolbarsTMPIniFile);
                                if(TMPToolbarStr.Len() == 0) break;
                                IniWriteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(j - 1, 3).Get_String(), TMPToolbarStr, ToolbarsTMPIniFile);
                                IniDeleteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(j, 3).Get_String(), ToolbarsTMPIniFile);
                            }
                        }
                        return(0);
                    }
            }
            break;
        case WM_MOUSEMOVE:
            if(LVDrag1 == 1)
            {
                ListViewMoveDrag(&LVDragPoint);
                if(ControlGetHwndFromPoint() != FRMToolbarsListDrop) CursorSetNo();
                else CursorSetNormal();
            }
            if(LVDrag2 == 1)
            {
                ListViewMoveDrag(&LVDragPoint);
                if(ControlGetHwndFromPoint() != FRMToolbarsListDrag) CursorSetNo();
                else CursorSetNormal();
            }
            break;
        case WM_LBUTTONUP:
            if(LVDrag1 == 1)
            {
                ListViewEndDrag(hwndDlg);
                CursorSetNormal();
                if(ControlGetHwndFromPoint() == FRMToolbarsListDrop)
                {
                    GetCursorPos(&MyTest.pt);
                    MyTest.pt.x = MyTest.pt.x - ControlLeft(FRMToolbarsListDrop);
                    MyTest.pt.y = MyTest.pt.y - ControlTop(FRMToolbarsListDrop);
                    SendMessage(FRMToolbarsListDrop, LVM_HITTEST, 0, (long) &MyTest);
                    if(MyTest.iItem != -1)
                    {
                        ListViewAddItem(FRMToolbarsListDrop, DragText, MyTest.iItem, (long) DragIcon);
                        TmpKey = ComboBoxGetItem(FRMToolbarsCombo, ComboBoxGetIndex(FRMToolbarsCombo));
                        // Add an entry into the temp file
                        TMPToolbar = IniReadKey(TmpKey, "Number", ToolbarsTMPIniFile).Get_Long();
                        TMPToolbar++;
                        IniWriteKey(TmpKey, "Number", TMPToolbar, ToolbarsTMPIniFile);
                        for(j = TMPToolbar - 1; j >= MyTest.iItem; j--)
                        {
                            // Get the entry
                            TMPToolbarStr = IniReadKey(TmpKey, "Tool" + (CStr) StringNumberComplement(j - 1, 3).Get_String(), ToolbarsTMPIniFile);
                            if(TMPToolbarStr.Len() == 0) break;
                            // Re-create it at higher position
                            IniWriteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(j, 3).Get_String(), TMPToolbarStr, ToolbarsTMPIniFile);
                        }
                        IniWriteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(MyTest.iItem, 3).Get_String(), MenuRealCommandsID.Get(BegDrag)->Content, ToolbarsTMPIniFile);
                    }
                    else
                    {
                        if((MyTest.flags & LVHT_NOWHERE) != 0)
                        {
                            ItemToAddDbl = ListViewItemCount(FRMToolbarsListDrop);
                            ListViewAddItem(FRMToolbarsListDrop, DragText, ItemToAddDbl, (long) DragIcon);
                            TmpKey = ComboBoxGetItem(FRMToolbarsCombo, ComboBoxGetIndex(FRMToolbarsCombo));
                            // Add an entry into the temp file
                            TMPToolbar = IniReadKey(TmpKey, "Number", ToolbarsTMPIniFile).Get_Long();
                            TMPToolbar++;
                            IniWriteKey(TmpKey, "Number", TMPToolbar, ToolbarsTMPIniFile);
                            IniWriteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(ItemToAddDbl, 3).Get_String(), MenuRealCommandsID.Get(BegDrag)->Content, ToolbarsTMPIniFile);
                        }
                    }
                }
                LVDrag1 = 0;
            }
            if(LVDrag2 == 1)
            {
                ListViewEndDrag(hwndDlg);
                CursorSetNormal();
                if(ControlGetHwndFromPoint() == FRMToolbarsListDrag)
                {
                    ListViewDeleteItem(FRMToolbarsListDrop, DragIndex);
                    TmpKey = ComboBoxGetItem(FRMToolbarsCombo, ComboBoxGetIndex(FRMToolbarsCombo));
                    // Remove an entry from the temp file
                    IniDeleteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(DragIndex, 3).Get_String(), ToolbarsTMPIniFile);
                    TMPToolbar = IniReadKey(TmpKey, "Number", ToolbarsTMPIniFile).Get_Long();
                    TMPToolbar--;
                    IniWriteKey(TmpKey, "Number", TMPToolbar, ToolbarsTMPIniFile);
                    for(j = DragIndex + 1; j <= (long) TMPToolbar; j++)
                    {
                        TMPToolbarStr = IniReadKey(TmpKey, "Tool" + (CStr) StringNumberComplement(j, 3).Get_String(), ToolbarsTMPIniFile);
                        if(TMPToolbarStr.Len() == 0) break;
                        IniWriteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(j - 1, 3).Get_String(), TMPToolbarStr, ToolbarsTMPIniFile);
                        IniDeleteKey(TmpKey, "Tool" + (CStr) StringNumberComplement(j, 3).Get_String(), ToolbarsTMPIniFile);
                    }
                }
                LVDrag2 = 0;
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == FRMToolbarsCombo)
            {
                if((wParam & 0x7FFF0000) == 0x10000) FillToolbarDatas();
                return(0);
            }
            else if((HWND) lParam == FRMToolbarsCmdNew)
            {
                ToolbarNName = MiscInputBox(hwndDlg, "Enter new toolbar name", ICON_TOOLBARS, "", 0, INPUTBOX_SIMPLETEXT, "");
                if(ToolbarNName.Len() != 0)
                {
                    if(ListViewFindItem(FRMToolbarsListview, ToolbarNName) != -1)
                    {
                        MiscMsgBox(FRMToolbarshwnd, "Toolbar already exists.", MB_ERROR, Requesters);
                        return(0);
                    }
                    CurrentLvItem = ListViewItemCount(FRMToolbarsListview);
                    ListViewAddItem(FRMToolbarsListview, ToolbarNName, CurrentLvItem, 0);
                    ListViewSetItemCheckbox(FRMToolbarsListview, CurrentLvItem, 1);
                    AddToolbarEntry(ToolbarNName);
                }
                return(0);
            }
            else if((HWND) lParam == FRMToolbarsCmdRename)
            {
                if(ListViewGetSelItemText(FRMToolbarsListview, 0).Len() == 0)
                {
                    MiscMsgBox(FRMToolbarshwnd, "Select a toolbar name.", MB_ERROR, Requesters);
                    return(0);
                }
                OldToolbarName = ListViewGetSelItemText(FRMToolbarsListview, 0);
                ToolbarNName = MiscInputBox(hwndDlg, "Enter new toolbar name", ICON_TOOLBARS, OldToolbarName, 0, INPUTBOX_SIMPLETEXT, "");
                if(ToolbarNName.Len() != 0)
                {
                    if(ListViewFindItem(FRMToolbarsListview, ToolbarNName) != -1)
                    {
                        MiscMsgBox(FRMToolbarshwnd, "Toolbar already exists.", MB_ERROR, Requesters);
                        return(0);
                    }
                    RenameToolbarEntry(OldToolbarName, ToolbarNName);
                    ListViewSetSelItemText(FRMToolbarsListview, ToolbarNName, 0);
                    // Re-create entries for renamed toolbar
                    NTBNumber = IniReadKey(OldToolbarName, "Number", ToolbarsTMPIniFile);
                    IniWriteKey(ToolbarNName, "Number", NTBNumber, ToolbarsTMPIniFile);
                    for(j = 0; j <= 999; j++)
                    {
                        TMPToolbarStr = IniReadKey(OldToolbarName, "Tool" + (CStr) StringNumberComplement(j, 3).Get_String(), ToolbarsTMPIniFile);
                        if(TMPToolbarStr.Len() == 0) break;
                        IniWriteKey(ToolbarNName, "Tool" + (CStr) StringNumberComplement(j, 3).Get_String(), TMPToolbarStr, ToolbarsTMPIniFile);
                    }
                    // Delete old toolbar infos
                    IniWriteSection(OldToolbarName, "", ToolbarsTMPIniFile);
                }
                return(0);
            }
            else if((HWND) lParam == FRMToolbarsCmdDelete)
            {
                OldToolbarName = ListViewGetSelItemText(FRMToolbarsListview, 0);
                if(OldToolbarName.Len() == 0)
                {
                    MiscMsgBox(FRMToolbarshwnd, "Select a toolbar name.", MB_ERROR, Requesters);
                    return(0);
                }
                if(MiscMsgBox(hwndDlg, "Really want to remove toolbar '" + (CStr) OldToolbarName + (CStr) "' ?",
                                MB_QUESTION, Requesters) == IDYES)
                {
                    ListViewDeleteItem(FRMToolbarsListview, ListViewGetSelItem(FRMToolbarsListview, -1));
                    RemoveToolbarEntry(OldToolbarName);
                    IniDeleteKey(OldToolbarName, "", ToolbarsTMPIniFile);
                }
                return(0);
            }
            else if((HWND) lParam == FRMToolbarsCmdClose)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMToolbarsCheckB)
            {
                OldToolbarName = ComboBoxGetItem(FRMToolbarsCombo, ComboBoxGetIndex(FRMToolbarsCombo));
                if(OldToolbarName.Len() != 0) ChangeToolbarBreak(OldToolbarName, CheckBoxGetState(FRMToolbarsCheckB));
                return(0);
            }
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &HexPs);
            GDIDrawHorzSep(hwndDlg, 0, 268, 359);
            EndPaint(hwndDlg, &HexPs);
            break;
        case WM_CLOSE:
            FillToolbarsHeader();
            for(i = 0; i < ToolbarsHwnd.Amount(); i++)
            {
                if(ToolbarsHwnd.Get(i)->Content != 0) DestroyWindow(ToolbarsHwnd.Get(i)->Content);
            }
            for(i = 0; i < PagersHwnd.Amount(); i++)
            {
                if(PagersHwnd.Get(i)->Content != 0) DestroyWindow(PagersHwnd.Get(i)->Content);
            }
            RefreshToolbars();
            DeleteFile(ToolbarsTMPIniFile.Get_String());
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Fill the listview with toolbars names
void FillToolbarsNames(void)
{
    int i = 0;

    TBArray.Erase();
    TBArrayBreak.Erase();
    for(i = 0; i < ToolBarsName.Amount(); i++)
    {
        ListViewAddItem(FRMToolbarsListview, ToolBarsName.Get(i)->Content, i, 0);
        ListViewSetItemCheckbox(FRMToolbarsListview, i, ToolBarsShow.Get(i)->Content);
        TBArray.Add(0L);
        TBArrayBreak.Add(0L);
    }
}

// -----------------------------------------------------------------------
// Save the toolbars states
void FillToolbarsHeader(void)
{
    int i = 0;
    CStr TbValue;
    long TbValueLng = 0;
    CStr TBK;
    CStr TBBtnIcon;
    CStr TBBtnTT;
    CStr TBBtnMenu;
    CStr TBBtnComment;
    int j = 0;

    for(i = 0; i <= ListViewItemCount(FRMToolbarsListview) - 1; i++)
    {
        switch(ListViewGetItemCheckbox(FRMToolbarsListview, i))
        {
            case 1:
                ChangeToolbarVisible(ListViewGetItemText(FRMToolbarsListview, i, 0), 1);
                break;
            case 0:
                ChangeToolbarVisible(ListViewGetItemText(FRMToolbarsListview, i, 0), 0);
                break;
        }
        TBK = ListViewGetItemText(FRMToolbarsListview, i, 0);
        // Check if toolbar have been modified
        TbValue = IniReadKey(TBK, "Tool000", ToolbarsTMPIniFile);
        if(TbValue.Len() != 0)
        {
            // Delete all entries in main toolbars file
            IniWriteSection(ListViewGetItemText(FRMToolbarsListview, i, 0), "", ToolbarsIniFile);
            for(j = 0; j <= 999; j++)
            {
                // Get entry index
                TbValue = IniReadKey(TBK, "Tool" + (CStr) StringNumberComplement(j, 3).Get_String(), ToolbarsTMPIniFile);
                if(TbValue.Len() == 0) break;
                TbValueLng = TbValue.Get_Long();
                switch(TbValueLng)
                {
                    case 0:
                        TBBtnIcon = TBIconsArray.Get(0)->Content;
                        TBBtnTT = TBToolsArray.Get(0)->Content;
                        TBBtnMenu = "";
                        TBBtnComment = "";
                        TbValueLng = -2;
                        break;
                    case -3:
                        TBBtnIcon = TBIconsArray.Get(1)->Content;
                        TBBtnTT = TBToolsArray.Get(1)->Content;
                        TBBtnMenu = MenuCommandsID.Get(1)->Content;
                        TBBtnComment = TBBtnCommentArray.Get(1)->Content;
                        TbValueLng = 9;
                        break;
                    case -4:
                        TBBtnIcon = TBIconsArray.Get(2)->Content;
                        TBBtnTT = TBToolsArray.Get(2)->Content;
                        TBBtnMenu = MenuCommandsID.Get(2)->Content;
                        TBBtnComment = TBBtnCommentArray.Get(2)->Content;
                        TbValueLng = 10;
                        break;
                    default:
                        TBBtnIcon = TBIconsArray.Get(TbValueLng)->Content;
                        TBBtnTT = TBToolsArray.Get(TbValueLng)->Content;
                        TBBtnComment = TBBtnCommentArray.Get(TbValueLng)->Content;
                        TBBtnMenu = MenuCommandsID.Get(TbValueLng)->Content;
                        switch(TBBtnMenu.Get_Long())
                        {
                            case (MENU_FILE_IDBASE + MENU_FILE_OPENFILE_ID):
                                TbValueLng = 0;
                                break;
                            case (MENU_FILE_IDBASE + MENU_FILE_SAVE_ID):
                                TbValueLng = 1;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_EDIT_COPY_ID):
                                TbValueLng = 2;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_EDIT_DELETE_DELETESELECTION_ID):
                                TbValueLng = 3;
                                break;
                            case (MENU_FILE_IDBASE + MENU_FILE_CONSOLE_RUN_ID):
                                TbValueLng = 4;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_EDIT_BOOKMARKS_TOGGLE_ID):
                                TbValueLng = 5;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_EDIT_SELECTPROCEDURE_ID):
                                TbValueLng = 6;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_EDIT_GOTO_TOP_ID):
                                TbValueLng = 7;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_EDIT_BLOCK_INDENT_ID):
                                TbValueLng = 8;
                                break;
                            case (MENU_PROJECT_IDBASE + MENU_PROJECT_ADDINCLUDE_ID):
                                TbValueLng = 11;
                                break;
                            case (MENU_FILE_IDBASE + MENU_FILE_INCLUDEFILE_ID):
                                TbValueLng = 12;
                                break;
                            case (MENU_FILE_IDBASE + MENU_FILE_NEW_ID):
                                TbValueLng = 13;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_EDIT_PASTE_ID):
                                TbValueLng = 14;
                                break;
                            case (MENU_FILE_IDBASE + MENU_FILE_RUNEXTERNALPROGRAM_ID):
                                TbValueLng = 15;
                                break;
                            case (MENU_FILE_IDBASE + MENU_FILE_FAVORITES_ID):
                                TbValueLng = 16;
                                break;
                            case (MENU_FILE_IDBASE + MENU_FILE_FILTERS_ID):
                                TbValueLng = 17;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_FILE_INCLUDEFILTERS_ID):
                                TbValueLng = 18;
                                break;
                            case (MENU_EDIT_IDBASE + MENU_EDIT_BREAKPOINTS_TOGGLE_ID):
                                TbValueLng = 19;
                                break;
                            case (MENU_VIEW_IDBASE + MENU_VIEW_PROJECTDOCK_ID):
                                TbValueLng = 20;
                                break;
                            default:
                                TbValueLng = -1;
                                break;
                        }
                }
                switch(TBBtnMenu.Get_Long())
                {
                    case -3:
                        TBBtnMenu = 10;
                        break;
                    case -4:
                        TBBtnMenu = 11;
                        break;
                }
                // Save new entries datas
                IniWriteKey(TBK, "BtnIcon" + (CStr) StringNumberComplement(j, 3).Get_String(), TBBtnIcon, ToolbarsIniFile);
                IniWriteKey(TBK, "BtnMenu" + (CStr) StringNumberComplement(j, 3).Get_String(), TbValueLng, ToolbarsIniFile);
                IniWriteKey(TBK, "BtnTool" + (CStr) StringNumberComplement(j, 3).Get_String(), TBBtnTT, ToolbarsIniFile);
                IniWriteKey(TBK, "BtnHelp" + (CStr) StringNumberComplement(j, 3).Get_String(), TBBtnComment, ToolbarsIniFile);
                IniWriteKey(TBK, "BtnComd" + (CStr) StringNumberComplement(j, 3).Get_String(), TBBtnMenu, ToolbarsIniFile);
            }
        }
    }
}

// -----------------------------------------------------------------------
// Add a toolbar entry into the ini file
void AddToolbarEntry(CStr ToolBarName)
{
    int i = 0;
    long FoundEntry = 0;
    CStr CurEntry;

    // Check if entry already exists in file
    for(i = 0; i <= 999; i++)
    {
        CurEntry = IniReadKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
        if(CurEntry.Len() == 0) break;
        if(_strcmpi(CurEntry.Get_String(), ToolBarName.Get_String()) == 0)
        {
            FoundEntry = 1;
            break;
        }
    }
    // Nope
    if(FoundEntry == 0)
    {
        for(i = 0; i <= 999; i++)
        {
            if(IniReadKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile).Len() == 0) break;
        }
    }
    IniWriteKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolBarName, ToolbarsIniFile);
    IniWriteKey("Toolbars", "TbType" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ToolbarsIniFile);
    IniWriteKey("Toolbars", "TbShow" + (CStr) StringNumberComplement(i, 3).Get_String(), "1", ToolbarsIniFile);
}

// -----------------------------------------------------------------------
// Rename a toolbar entry from the ini file
void RenameToolbarEntry(CStr OldToolbarName, CStr ToolBarName)
{
    int i = 0;
    long FoundEntry = 0;
    CStr CurEntry;

    for(i = 0; i <= 999; i++)
    {
        CurEntry = IniReadKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
        if(CurEntry.Len() == 0) break;
        if(_strcmpi(CurEntry.Get_String(), OldToolbarName.Get_String()) == 0)
        {
            FoundEntry = 1;
            break;
        }
    }
    if(FoundEntry == 1) IniWriteKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolBarName, ToolbarsIniFile);
}

// -----------------------------------------------------------------------
// Change a toolbar break type from the ini file
void ChangeToolbarBreak(CStr ToolBarName, long TBBreak)
{
    int i = 0;
    long FoundEntry = 0;
    CStr CurEntry;

    for(i = 0; i <= 999; i++)
    {
        CurEntry = IniReadKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
        if(CurEntry.Len() == 0) break;
        if(_strcmpi(CurEntry.Get_String(), ToolBarName.Get_String()) == 0)
        {
            FoundEntry = 1;
            break;
        }
    }
    if(FoundEntry == 1) IniWriteKey("Toolbars", "TbType" + (CStr) StringNumberComplement(i, 3).Get_String(), TBBreak, ToolbarsIniFile);
}

// -----------------------------------------------------------------------
// Change a toolbar visiable on/off in the ini file
void ChangeToolbarVisible(CStr ToolBarName, long TBVisible)
{
    int i = 0;
    long FoundEntry = 0;
    CStr CurEntry;

    for(i = 0; i <= 999; i++)
    {
        CurEntry = IniReadKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
        if(CurEntry.Len() == 0) break;
        if(_strcmpi(CurEntry.Get_String(), ToolBarName.Get_String()) == 0)
        {
            FoundEntry = 1;
            break;
        }
    }
    if(FoundEntry == 1) IniWriteKey("Toolbars", "TbShow" + (CStr) StringNumberComplement(i, 3).Get_String(), TBVisible, ToolbarsIniFile);
}

// -----------------------------------------------------------------------
// Remove a toolbar entry from the ini file
void RemoveToolbarEntry(CStr ToolBarName)
{
    int i = 0;
    long FoundEntry = 0;
    CStr CurEntry;
 
    for(i = 0; i <= 999; i++)
    {
        CurEntry = IniReadKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
        if(CurEntry.Len() == 0) break;
        if(_strcmpi(CurEntry.Get_String(), ToolBarName.Get_String()) == 0)
        {
            FoundEntry = 1;
            break;
        }
    }
    if(FoundEntry == 1)
    {
        IniDeleteKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
        IniDeleteKey("Toolbars", "TbType" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
        IniDeleteKey("Toolbars", "TbShow" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
    }
    IniDeleteKey(ToolBarName, "", ToolbarsIniFile);
}

// -----------------------------------------------------------------------
// Fill the toolbar selection combobox
void FRMToolbarsFillCombo(void)
{
    int i = 0;

    ComboBoxReset(FRMToolbarsCombo);
    for(i = 0; i <= ListViewItemCount(FRMToolbarsListview) - 1; i++)
    {
        ComboBoxAddItem(FRMToolbarsCombo, ListViewGetItemText(FRMToolbarsListview, i, 0), -1);
    }
    if(ComboBoxCount(FRMToolbarsCombo) != 0) ComboBoxSetIndex(FRMToolbarsCombo, 0);
}

// -----------------------------------------------------------------------
// Fill toolbar listview for select entry in combobox
void FillToolbarDatas(void)
{
    CStr TBBtnIcon;
    CStr TBBtnMenu;
    CStr TBBtnToolTip;
    CStr TBBtnComment;
    CStr TBBtnCommand;
    long TBBtnCommandLng = 0;
    CStr TBToolBarKey;
    CStr TBToolBarModified;
    CStr TBToolBarPrime;
    long TBToolBarModifiedLng = 0;
    long TBReload = 0;
    CStr TBBreak;
    int i = 0;
    CStr BufString;
    
    TBToolBarKey = ComboBoxGetItem(FRMToolbarsCombo, ComboBoxGetIndex(FRMToolbarsCombo));
    if(TBToolBarKey.Len() != 0)
    {
        TBBreak = IniReadKey("Toolbars", "TbType" + (CStr) StringNumberComplement(ComboBoxGetIndex(FRMToolbarsCombo), 3).Get_String(),
                               ToolbarsIniFile);
        if(TBBreak.Len() == 0) TBBreak = "0";
        CheckBoxSetState(FRMToolbarsCheckB, TBBreak.Get_Long());
        ListViewClear(FRMToolbarsListDrop);
        TBToolBarModified = IniReadKey(TBToolBarKey, "Tool000", ToolbarsTMPIniFile);
        if(TBToolBarModified.Len() != 0)
        {
            TBReload = 1;
        }
        else
        {
            // Toolbar initialized but
            // all items have been removed from toolbar ?
            TBToolBarPrime = IniReadKey(TBToolBarKey, "Number", ToolbarsTMPIniFile);
            if(TBToolBarPrime.Len() != 0) TBReload = 1;
        }
        // Check if temp file is already filled
        for(i = 0; i <= 999; i++)
        {
            TBToolBarModified = IniReadKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsTMPIniFile);
            // Check if entry have already been created
            if(TBToolBarModified.Len() != 0)
            {
                // Created and modified ?
                TBToolBarModifiedLng = TBToolBarModified.Get_Long();
                // Then retrieve the datas from the available commands
                switch(TBToolBarModifiedLng)
                {
                    case -3:
                        TBBtnIcon = TBIconsArray.Get(1)->Content;
                        TBBtnMenu = MenuCommandsID.Get(1)->Content;
                        TBBtnComment = TBBtnCommentArray.Get(1)->Content;
                        break;
                    case -4:
                        TBBtnIcon = TBIconsArray.Get(2)->Content;
                        TBBtnMenu = MenuCommandsID.Get(2)->Content;
                        TBBtnComment = TBBtnCommentArray.Get(2)->Content;
                        break;
                    default:
                        TBBtnIcon = TBIconsArray.Get(TBToolBarModifiedLng)->Content;
                        TBBtnMenu = MenuCommandsID.Get(TBToolBarModifiedLng)->Content;
                        TBBtnComment = TBBtnCommentArray.Get(TBToolBarModifiedLng)->Content;
                        break;
                }
                goto AddModifiedCommandToList;
            }
            if(TBReload == 1) break;
            TBBtnIcon = IniReadKey(TBToolBarKey, "BtnIcon" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
            TBBtnMenu = IniReadKey(TBToolBarKey, "BtnMenu" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
            TBBtnComment = IniReadKey(TBToolBarKey, "BtnHelp" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
            TBBtnCommand = IniReadKey(TBToolBarKey, "BtnComd" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
            if(TBBtnIcon.Len() == 0) break;
            if(TBBtnMenu.Len() == 0) break;
            switch(TBBtnMenu.Get_Long())
            {
                case -2:
                    // Separators
                    IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ToolbarsTMPIniFile);
                    break;
                case 9:
                    // Recent files list
                    IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), "-3", ToolbarsTMPIniFile);
                    break;
                case 10:
                    // Recent projects list
                    IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), "-4", ToolbarsTMPIniFile);
                    break;
                default:
                    // Single commands
                    // and drop down menus
                    TBBtnCommandLng = TBBtnCommand.Get_Long();
                    // File
                    if(TBBtnCommandLng < MENU_EDIT_IDBASE)
                    {
                        TBBtnCommandLng = (TBBtnCommandLng - MENU_FILE_IDBASE) + 3;
                        IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), TBBtnCommandLng, ToolbarsTMPIniFile);
                        goto AddModifiedCommandToList;
                    }
                    // Edit
                    if(TBBtnCommandLng < MENU_VIEW_IDBASE)
                    {
                        TBBtnCommandLng = (TBBtnCommandLng - MENU_EDIT_IDBASE) + 3 + (sizeof(MenuFileComments) / 4);
                        IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), TBBtnCommandLng, ToolbarsTMPIniFile);
                        goto AddModifiedCommandToList;
                    }
                    // View
                    if(TBBtnCommandLng < MENU_PROJECT_IDBASE)
                    {
                        TBBtnCommandLng = (TBBtnCommandLng - MENU_VIEW_IDBASE) + 3 + (sizeof(MenuFileComments) / 4) + (sizeof(MenuEditComments) / 4);
                        IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), TBBtnCommandLng, ToolbarsTMPIniFile);
                        goto AddModifiedCommandToList;
                    }
                    // Project
                    if(TBBtnCommandLng < MENU_WINDOW_IDBASE)
                    {
                        TBBtnCommandLng = (TBBtnCommandLng - MENU_PROJECT_IDBASE) + 3 + (sizeof(MenuFileComments) / 4) + (sizeof(MenuEditComments) / 4) + (sizeof(MenuViewComments) / 4);
                        IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), TBBtnCommandLng, ToolbarsTMPIniFile);
                        goto AddModifiedCommandToList;
                    }
                    // Window
                    if(TBBtnCommandLng < MENU_USER_IDBASE)
                    {
                        TBBtnCommandLng = (TBBtnCommandLng - MENU_WINDOW_IDBASE) + 3 + (sizeof(MenuFileComments) / 4) + (sizeof(MenuEditComments) / 4) + (sizeof(MenuViewComments) / 4) + (sizeof(MenuProjectComments) / 4);
                        IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), TBBtnCommandLng, ToolbarsTMPIniFile);
                        goto AddModifiedCommandToList;
                    }
                    TBBtnCommandLng = (TBBtnCommandLng - MENU_USER_IDBASE) + 3 + (sizeof(MenuFileComments) / 4) + (sizeof(MenuEditComments) / 4) + (sizeof(MenuViewComments) / 4) + (sizeof(MenuProjectComments) / 4) + (sizeof(MenuWindowComments) / 4);
                    IniWriteKey(TBToolBarKey, "Tool" + (CStr) StringNumberComplement(i, 3).Get_String(), TBBtnCommandLng, ToolbarsTMPIniFile);
            }
AddModifiedCommandToList:
            switch(TBBtnMenu.Get_Long())
            {
                // Separator
                case -2:
                    ListViewAddItem(FRMToolbarsListDrop, "<Separator>", i, -1);
                    break;
                // Simple button
                default:
                    if(TBBtnIcon.Is_Numeric())
                    {
                        if(TBBtnIcon.Get_Long() != 0)
                        {
                            ListViewAddItem(FRMToolbarsListDrop, TBBtnComment, i, TBBtnIcon.Get_Long());
                        }
                        else
                        {
                            ListViewAddItem(FRMToolbarsListDrop, TBBtnComment, i, -1);
                        }
                    }
                    else
                    {
                        ListViewAddItem(FRMToolbarsListDrop, TBBtnIcon, i, -1);
                    }
            }
        }
        IniWriteKey(TBToolBarKey, "Number", i, ToolbarsTMPIniFile);
    }
}

// -----------------------------------------------------------------------
// Fill toolbar listview with available commands
void FillToolbarCommands(void)
{
    int i = 0;
    long PosInLV = 0;
    CStr MenuStr;
    long UserIcon = 0;
    long RealPos = 0;
    CStr BufString;

    ListViewClear(FRMToolbarsListDrag);
    PosInLV = 0;
    TBIconsArray.Erase();
    TBToolsArray.Erase();
    TBBtnCommentArray.Erase();
    MenuCommandsID.Erase();
    MenuRealCommandsID.Erase();
    
    // Store toolbar ID
    TBIconsArray.Add(0L);
    TBToolsArray.Add("");
    MenuCommandsID.Add(-2L);
    TBBtnCommentArray.Add("<Separator>");
    MenuRealCommandsID.Add(RealPos);

    RealPos++;
    TBIconsArray.Add(ICON_FILES);
    TBToolsArray.Add("Recent files list");
    MenuCommandsID.Add(-3L);
    TBBtnCommentArray.Add("Display the recent files list");
    MenuRealCommandsID.Add(RealPos);

    RealPos++;
    TBIconsArray.Add(ICON_FILES);
    TBToolsArray.Add("Recent projects list");
    MenuCommandsID.Add(-4L);
    TBBtnCommentArray.Add("Display the recent projects list");
    MenuRealCommandsID.Add(RealPos);
    
    RealPos++;
    for(i = 0; i < ((sizeof(MenuFileComments) / 4) ) ; i++)
    {
        ListViewAddItem(FRMToolbarsListDrag, MenuFileComments[i], PosInLV, ToolbarFileIcons[i]);
        TBIconsArray.Add(ToolbarFileIcons[i]);
        TBToolsArray.Add(MenuFileToolTips[i]);
        MenuCommandsID.Add((i + MENU_FILE_IDBASE));
        MenuRealCommandsID.Add(RealPos);
        TBBtnCommentArray.Add(MenuFileComments[i]);
        RealPos++;
        PosInLV++;
    }
    for(i = 0; i < ((sizeof(MenuEditComments) / 4) ); i++)
    {
        ListViewAddItem(FRMToolbarsListDrag, MenuEditComments[i], PosInLV, ToolbarEditIcons[i]);
        TBIconsArray.Add(ToolbarEditIcons[i]);
        TBToolsArray.Add(MenuEditToolTips[i]);
        MenuCommandsID.Add((i + MENU_EDIT_IDBASE));
        MenuRealCommandsID.Add(RealPos);
        TBBtnCommentArray.Add(MenuEditComments[i]);
        RealPos++;
        PosInLV++;
    }
    for(i = 0; i < ((sizeof(MenuViewComments) / 4) ); i++)
    {
        ListViewAddItem(FRMToolbarsListDrag, MenuViewComments[i], PosInLV, ToolbarViewIcons[i]);
        TBIconsArray.Add(ToolbarViewIcons[i]);
        TBToolsArray.Add(MenuViewToolTips[i]);
        MenuCommandsID.Add((i + MENU_VIEW_IDBASE));
        MenuRealCommandsID.Add(RealPos);
        TBBtnCommentArray.Add(MenuViewComments[i]);
        RealPos++;
        PosInLV++;
    }
    for(i = 0; i < ((sizeof(MenuProjectComments) / 4) ); i++)
    {
        ListViewAddItem(FRMToolbarsListDrag, MenuProjectComments[i], PosInLV, ToolbarProjectIcons[i]);
        TBIconsArray.Add(ToolbarProjectIcons[i]);
        TBToolsArray.Add(MenuProjectToolTips[i]);
        MenuCommandsID.Add((i + MENU_PROJECT_IDBASE));
        MenuRealCommandsID.Add(RealPos);
        TBBtnCommentArray.Add(MenuProjectComments[i]);
        RealPos++;
        PosInLV++;
    }
    for(i = 0; i < ((sizeof(MenuWindowComments) / 4) ); i++)
    {
        ListViewAddItem(FRMToolbarsListDrag, MenuWindowComments[i], PosInLV, ToolbarWindowIcons[i]);
        TBIconsArray.Add(ToolbarWindowIcons[i]);
        TBToolsArray.Add(MenuWindowToolTips[i]);
        MenuCommandsID.Add((i + MENU_WINDOW_IDBASE));
        MenuRealCommandsID.Add(RealPos);
        TBBtnCommentArray.Add(MenuWindowComments[i]);
        RealPos++;
        PosInLV++;
    }
    // User commands
    for(i = 0; i < MenusCmds.Amount(); i++)
    {
        MenuStr = MenuGetString(hMainMenu, MENU_USER_IDBASE + i + 1);
        if(MenuStr.Len() != 0)
        {
            if(MenusCmdsHelp.Get(i)->Content == 1)
            {
                UserIcon = ICON_HELPFILES;
                ListViewAddItem(FRMToolbarsListDrag, MenuStr, PosInLV, UserIcon);
            }
            else
            {
                BufString = MenusCmdsComments.Get(i)->Content;
                BufString = BufString.Left(5);
                if(BufString == "CAPT ")
                {
                    UserIcon = ICON_CONSOLE;
                    ListViewAddItem(FRMToolbarsListDrag, MenuStr, PosInLV, UserIcon);
                }
                else if(BufString == "HTML ")
                {
                    UserIcon = ICON_INTERNET;
                    ListViewAddItem(FRMToolbarsListDrag, MenuStr, PosInLV, UserIcon);
                }
                else if(BufString == "PLUG ")
                {
                    UserIcon = ICON_PLUG;
                    ListViewAddItem(FRMToolbarsListDrag, MenuStr, PosInLV, UserIcon);
                }
                else if(BufString == "SNAP ")
                {
                    UserIcon = ICON_SNAP;
                    ListViewAddItem(FRMToolbarsListDrag, MenuStr, PosInLV, UserIcon);
                }
                else if(BufString == "DOCU ")
                {
                    UserIcon = ICON_FILEASSOC;
                    ListViewAddItem(FRMToolbarsListDrag, MenuStr, PosInLV, UserIcon);
                }
                else if(BufString == "PREP ")
                {
                    UserIcon = ICON_LAB;
                    ListViewAddItem(FRMToolbarsListDrag, MenuStr, PosInLV, UserIcon);
                }
                else
                {
                    UserIcon = ICON_WINDOWS;
                    ListViewAddItem(FRMToolbarsListDrag, MenuStr, PosInLV, UserIcon);
                }
            }
            TBIconsArray.Add(UserIcon);
            TBToolsArray.Add(MenuStr.Get_String());
            MenuCommandsID.Add((i + MENU_USER_IDBASE + 1));
            MenuRealCommandsID.Add(RealPos);
            TBBtnCommentArray.Add(MenuStr.Get_String());
            RealPos++;
            PosInLV++;
        }
        else
        {
            RealPos++;
            TBIconsArray.Add(0L);
            TBToolsArray.Add("");
            MenuCommandsID.Add(-2L);
            TBBtnCommentArray.Add("<Separator>");
        }
    }
    ListViewAddItem(FRMToolbarsListDrag, "<Separator>", 0, -1);
    ListViewAddItem(FRMToolbarsListDrag, "Display the recent files list", 1, ICON_FILES);
    ListViewAddItem(FRMToolbarsListDrag, "Display the recent projects list", 2, ICON_FILES);
}
