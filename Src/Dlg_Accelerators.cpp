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
// Dlg_Accelerators.cpp: Accelerators resource creation form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Splitters.h"
#include "ProjectsBuilder.h"
#include "MDI_Form.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "Dlg_Accelerators.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMAcceleratorshwnd;
long EditAccelerators;
long AccelBase;
CStr AcceleratorsTitle;
CStr AcceleratorsFileToEdit;
CStr AcceleratorsBaseToEdit;
HWND FRMAcceleratorsListview;
long LVAcceleratorsInEdit;
HWND FRMAcceleratorsOk;
HWND FRMAcceleratorsCancel;
HWND FRMAcceleratorsToolbar;
HWND FRMAcceleratorsFrame;
HWND FRMAcceleratorsChkCtrl;
HWND FRMAcceleratorsChkAlt;
HWND FRMAcceleratorsChkShift;
HWND FRMAcceleratorsCombo;
HWND FRMAcceleratorsGetKey;

// -----------------------------------------------------------------------
// Create an empty accelerators resource
void CreateAccelerators(void)
{
    long EntryAccCount = 0;

    EditAccelerators = 0;
    EntryAccCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewAccelerators);
    // Check if entry already exists
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewAccelerators, "ACCELERATORS_" + (CStr) EntryAccCount) != -1)
    {
        EntryAccCount++;
    }
    AccelBase = EntryAccCount;
    AcceleratorsTitle = "Create new accelerators resource";
    CreateModalDialog(-1, -1, 410, 313, hMDIform.hWnd, &FRMAcceleratorsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Edit accelerators resource
void EditAcceleratorsTable(CStr AcceleratorsEditBase, CStr AcceleratorsFile)
{
    EditAccelerators = 1;
    AcceleratorsBaseToEdit = AcceleratorsEditBase;
    AcceleratorsFileToEdit = AcceleratorsFile;
    AcceleratorsTitle = "Edit accelerators resource";
    CreateModalDialog(-1, -1, 410, 313, hMDIform.hWnd, &FRMAcceleratorsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMAcceleratorsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT EnterValuePs;
    long CurrentSelected = 0;
    long OldSelected = 0;
    CStr AccelToPut;
    HWND LvEdit = 0;
    CStr AccelText;
    long CurrentSelectedAccel = 0;
    int i = 0;
    long LV_TextbStyle = 0;
    CStr CurrentKe;
    long PosInKey = 0;
    long CurCtrlState = 0;
    long CurAltState = 0;
    long CurShiftState = 0;
    CStr CurrentAccelFromIni;
    CStr CurrentAccelCmdFromIni;
    NM_LISTVIEW SelLV;
    CStr BufString;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMAcceleratorsListview, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            FRMAcceleratorshwnd = hwndDlg;
            ControlSetText(hwndDlg, AcceleratorsTitle);
            CreateLabel(4, 39, 33, 17, hwndDlg, "Key :", 0, 0, 0, 0);
            FRMAcceleratorsOk = CreateButton(251, 287, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMAcceleratorsCancel = CreateButton(330, 287, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMAcceleratorsCombo = CreateComboBox(33, 36, 132, 150, hwndDlg, "", 4, 0, WS_TABSTOP | CBS_DROPDOWN);
            FRMAcceleratorsGetKey = CreateButton(170, 35, 70, 23, hwndDlg, "Input key", 10, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            ControlEnable(FRMAcceleratorsGetKey, 0);
            FRMAcceleratorsFrame = CreateFrame(247, 27, 162, 35, hwndDlg, "Modifiers", 5, 0, 0);
            FRMAcceleratorsChkCtrl = CreateCheckBox(10, 12, 45, 19, FRMAcceleratorsFrame, "Ctrl", 6, 0, WS_TABSTOP, 0);
            FRMAcceleratorsChkAlt = CreateCheckBox(62, 12, 45, 19, FRMAcceleratorsFrame, "Alt", 7, 0, WS_TABSTOP, 0);
            FRMAcceleratorsChkShift = CreateCheckBox(110, 12, 45, 19, FRMAcceleratorsFrame, "Shift", 8, 0, WS_TABSTOP, 0);
            FRMAcceleratorsListview = CreateListView(2, 65, 406, 209, hwndDlg, 9, GlobalImageList3, &LVAcceleratorsHook, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_NOSORTHEADER | LVS_EDITLABELS | LVS_SINGLESEL | WS_TABSTOP, WS_EX_STATICEDGE);
            ListViewAddCol(FRMAcceleratorsListview, "Accelerator", 283, 1);
            ListViewAddCol(FRMAcceleratorsListview, "Command number", 102, 0);
            ListViewSetHeaderPosition(FRMAcceleratorsListview, 0, 1);
            ListViewSetHeaderPosition(FRMAcceleratorsListview, 1, 0);
            FRMAcceleratorsToolbar = CreateToolBar(1, 1, 410, 23, hwndDlg, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            ToolBarAddButton(FRMAcceleratorsToolbar, "", ACCELCREATE, ICON_KEY, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddButton(FRMAcceleratorsToolbar, "", ACCELREPLACE, ICON_KEYREPLACE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddSeparator(FRMAcceleratorsToolbar, 0);
            ToolBarAddButton(FRMAcceleratorsToolbar, "", ACCELDELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            for(i = 0; i <= 84; i++)
            {
                ComboBoxAddItem(FRMAcceleratorsCombo, TabAccelerators[i], -1);
            }
            if(EditAccelerators == 1)
            {
                for(i = 0; i <= 999; i++)
                {
                    CurrentAccelFromIni = IniReadKey("ACCELDAT", "ACCEL" + (CStr) StringNumberComplement(i, 3).Get_String(), AcceleratorsFileToEdit);
                    CurrentAccelCmdFromIni = IniReadKey("ACCELDAT", "COMMAND" + (CStr) StringNumberComplement(i, 3).Get_String(), AcceleratorsFileToEdit);
                    if(CurrentAccelFromIni.Len() == 0) break;
                    ListViewAddItem(FRMAcceleratorsListview, CurrentAccelCmdFromIni, i, -1);
                    ListViewSetSubItem(FRMAcceleratorsListview, CurrentAccelFromIni, i, 1);
                }
            }
            FreezeTimer = 1;
            SetFocus(FRMAcceleratorsCombo);
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &EnterValuePs);
            GDIDrawHorzSep(hwndDlg, 0, 25, 410);
            GDIDrawHorzSep(hwndDlg, 0, 280, 410);
            EndPaint(hwndDlg, &EnterValuePs);
            break;
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMAcceleratorsListview)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        CurrentSelected = ListViewGetSelItem(FRMAcceleratorsListview, -1);
                        if(CurrentSelected != -1)
                        {
                            ListViewSetEditModeOn(FRMAcceleratorsListview, CurrentSelected, ES_NUMBER);
                            LVAcceleratorsInEdit = 1;
                        }
                        return(0);
                    case LVN_BEGINLABELEDIT:
                        LvEdit = ListViewGetEditControl(FRMAcceleratorsListview);
                        LV_TextbStyle = GetWindowLong(LvEdit, GWL_STYLE);
                        LV_TextbStyle = LV_TextbStyle | ES_NUMBER;
                        SetWindowLong(LvEdit, GWL_STYLE, LV_TextbStyle);
                        TextBoxSetMaxLen(LvEdit, 5);
                        return(0);
                    case LVN_ENDLABELEDIT:
                        CurrentSelected = ListViewGetSelItem(FRMAcceleratorsListview, -1);
                        if(CurrentSelected != -1)
                        {
                            if(ListViewEditValidated(lParam) == 1)
                            {
                                ListViewSetSubItem(FRMAcceleratorsListview, ListViewGetEditResult(lParam), CurrentSelected, 0);
                            }
                        }
                        LVAcceleratorsInEdit = 0;
                        return(0);
                    case LVN_ITEMCHANGED:
                        CopyMemory(&SelLV, (void *) lParam, sizeof(SelLV));
                        if((SelLV.uNewState & LVIS_SELECTED) != 0)
                        {
                            CurrentSelectedAccel = ListViewGetSelItem(FRMAcceleratorsListview, -1);
                            if(CurrentSelectedAccel != -1)
                            {
                                PosInKey = 1;
                                CurrentKe = ListViewGetSelItemText(FRMAcceleratorsListview, 1);
GetCurrentAccel:                BufString = CurrentKe.Mid(PosInKey, 5);
                                if(BufString == "CTRL ")
                                {
                                    CurCtrlState = 1;
                                    PosInKey = PosInKey + 7;
                                    goto GetCurrentAccel;
                                }
                                else if(BufString == "ALT +")
                                {
                                    CurAltState = 1;
                                    PosInKey = PosInKey + 6;
                                    goto GetCurrentAccel;
                                }
                                else if(BufString == "SHIFT")
                                {
                                    CurShiftState = 1;
                                    PosInKey = PosInKey + 8;
                                    goto GetCurrentAccel;
                                }
                                else
                                {
                                    BufString =  CurrentKe.Mid(PosInKey).Trim();
                                    ControlSetText(FRMAcceleratorsCombo, BufString);
                                }
                                CheckBoxSetState(FRMAcceleratorsChkCtrl, CurCtrlState);
                                CheckBoxSetState(FRMAcceleratorsChkAlt, CurAltState);
                                CheckBoxSetState(FRMAcceleratorsChkShift, CurShiftState);
                            }
                        }
                        return(0);
                }
            }               
            switch(ControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    switch(ControlGetNotifiedID(lParam))
                    {
                        case ACCELCREATE:
                            ToolBarDisplayToolTip("Create new accelerator", lParam);
                            return(0);
                        case ACCELREPLACE:
                            ToolBarDisplayToolTip("Replace accelerator", lParam);
                            return(0);
                        case ACCELDELETE:
                            ToolBarDisplayToolTip("Delete accelerator", lParam);
                            return(0);
                    }
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == FRMAcceleratorsToolbar)
            {
                switch(wParam)
                {
                    case ACCELCREATE:
                        AccelText = ControlGetText(FRMAcceleratorsCombo).Trim();
                        if(CheckAcceleratorValidity(AccelText) == 0)
                        {
                            MiscMsgBox(hwndDlg, "Invalid accelerator.", MB_ERROR, Requesters);
                            SetFocus(FRMAcceleratorsCombo);
                            return(0);
                        }
                        AccelToPut = "";
                        if(CheckBoxGetState(FRMAcceleratorsChkCtrl) == 1) AccelToPut = "CTRL";
                        if(CheckBoxGetState(FRMAcceleratorsChkAlt) == 1)
                        {
                            if(AccelToPut.Len() != 0) AccelToPut = AccelToPut + (CStr) " + ";
                            AccelToPut = AccelToPut + (CStr) "ALT";
                        }
                        if(CheckBoxGetState(FRMAcceleratorsChkShift) == 1)
                        {
                            if(AccelToPut.Len() != 0) AccelToPut = AccelToPut + (CStr) " + ";
                            AccelToPut = AccelToPut + (CStr) "SHIFT";
                        }
                        if(AccelToPut.Len() != 0) AccelToPut = AccelToPut + (CStr) " + ";
                        AccelToPut = AccelToPut + (CStr) AccelText;
                        ListViewAddItem(FRMAcceleratorsListview, "", ListViewItemCount(FRMAcceleratorsListview), -1);
                        ListViewSetSubItem(FRMAcceleratorsListview, AccelToPut, ListViewItemCount(FRMAcceleratorsListview) - 1, 1);
                        ListViewSetItemSel(FRMAcceleratorsListview, ListViewItemCount(FRMAcceleratorsListview) - 1);
                        SetFocus(FRMAcceleratorsListview);
                        return(0);
                    case ACCELREPLACE:
                        if(ListViewItemCount(FRMAcceleratorsListview) != 0)
                        {
                            CurrentSelectedAccel = ListViewGetSelItem(FRMAcceleratorsListview, -1);
                            if(CurrentSelectedAccel == -1)
                            {
                                MiscMsgBox(hwndDlg, "No accelerator selected.", MB_ERROR, Requesters);
                                SetFocus(FRMAcceleratorsListview);
                                return(0);
                            }
                            AccelText = ControlGetText(FRMAcceleratorsCombo).Trim();
                            if(CheckAcceleratorValidity(AccelText) == 0)
                            {
                                MiscMsgBox(hwndDlg, "Invalid accelerator.", MB_ERROR, Requesters);
                                SetFocus(FRMAcceleratorsCombo);
                                return(0);
                            }
                            AccelToPut = "";
                            if(CheckBoxGetState(FRMAcceleratorsChkCtrl) == 1) AccelToPut = "CTRL";
                            if(CheckBoxGetState(FRMAcceleratorsChkAlt) == 1)
                            {
                                if(AccelToPut.Len() != 0) AccelToPut = AccelToPut + (CStr) " + ";
                                AccelToPut = AccelToPut + (CStr) "ALT";
                            }
                            if(CheckBoxGetState(FRMAcceleratorsChkShift) == 1)
                            {
                                if(AccelToPut.Len() != 0) AccelToPut = AccelToPut + (CStr) " + ";
                                AccelToPut = AccelToPut + (CStr) "SHIFT";
                            }
                            if(AccelToPut.Len() != 0)
                            {
                                AccelToPut = AccelToPut + (CStr) " + ";
                            }
                            AccelToPut = AccelToPut + (CStr) AccelText;
                            ListViewSetSubItem(FRMAcceleratorsListview, AccelToPut, CurrentSelectedAccel, 1);
                            ListViewSetItemSel(FRMAcceleratorsListview, CurrentSelectedAccel);
                            SetFocus(FRMAcceleratorsListview);
                        }
                        return(0);
                    case ACCELDELETE:
                        OldSelected = ListViewGetSelItem(FRMAcceleratorsListview, -1);
                        ListViewDeleteItem(FRMAcceleratorsListview, ListViewGetSelItem(FRMAcceleratorsListview, -1));
                        if((long) OldSelected > (long) (ListViewItemCount(FRMAcceleratorsListview) - 1)) OldSelected = ListViewItemCount(FRMAcceleratorsListview) - 1;
                        ListViewSetItemSel(FRMAcceleratorsListview, OldSelected);
                        if(ListViewItemCount(FRMAcceleratorsListview) != 0)
                        {
                            SetFocus(FRMAcceleratorsListview);
                        }
                        else
                        {
                            SetFocus(FRMAcceleratorsCombo);
                        }
                        return(0);
                }
            }
            else if((HWND) lParam == FRMAcceleratorsOk)
            {
                if(CheckAcceCList() == 0)
                {
                    MiscMsgBox(hwndDlg, "Found invalid entries in accelerators list.", MB_ERROR, Requesters);
                    SetFocus(FRMAcceleratorsListview);
                    return(0);
                }
                if(EditAccelerators == 0)
                {
                    CreateNewAcceleratorsFile();
                    ProjectModified = TRUE;
                }
                else
                {
                    SaveAcceleratorsFile();
                }
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMAcceleratorsCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            break;
        case WM_CLOSE:
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Listview hook
LRESULT CALLBACK LVAcceleratorsHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if(LVAcceleratorsInEdit == 0)
    {
        switch(uMsg)
        {
            case WM_CHAR:
                if(ListViewPasteAutoEdit(FRMAcceleratorsListview, wParam, ES_NUMBER) == 1)
                {
                    LVAcceleratorsInEdit = 1;
                    return(0);
                }
                break;
        }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Check the validity of an accelerator entry
long CheckAcceleratorValidity(CStr AccelToCheck)
{
    int i = 0;
    long FoundVirtualKey = 0;
    
    if(AccelToCheck.Len() == 0) return(0);
    FoundVirtualKey = 0;
    for(i = 0; i <= 84; i++)
    {
        if(_strcmpi(ComboBoxGetItem(FRMAcceleratorsCombo, i).Get_String(), AccelToCheck.Get_String()) == 0)
        {
            FoundVirtualKey = 1;
            break;
        }
    }
    if(FoundVirtualKey == 0) if(AccelToCheck.Len() != 1) return(0);
    return(1);
}

// -----------------------------------------------------------------------
// Check validity of accelerators list
long CheckAcceCList(void)
{
    long ReturnValue = 1;
    CStr AccelToCheck;
    int i = 0;
    
    for(i = 0; i <= (long) (ListViewItemCount(FRMAcceleratorsListview) - 1); i++)
    {
        AccelToCheck = ListViewGetItemText(FRMAcceleratorsListview, i, 0);
        if(AccelToCheck.Len() == 0)
        {
            ReturnValue = 0;
            break;
        }
        if(StringIsDecimal(AccelToCheck, 5) == 0)
        {
            ReturnValue = 0;
            break;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Create a new string ini file for current project
void CreateNewAcceleratorsFile(void)
{
    int i = 0;
    HANDLE AccelFileHandle = 0;
    CStr AccelFileName;
    CStr AccelName;
    CStr EntryToAdd;
    
    AccelName = "ACCELERATORS_" + (CStr) AccelBase;
    AccelFileName = FileGetDirectory(ProjectFName) + (CStr) AccelName + (CStr) ".acr";
    AccelFileHandle = FileCreateEmpty(AccelFileName, NO_SECURITY_DESCRIPTOR);
    FileWriteLine(AccelFileHandle, "; Chromatic resource accelerators file");
    FileWriteLine(AccelFileHandle, "; Do NOT edit this file manually");
    FileWriteLine(AccelFileHandle, "; unless you know what you're doing !!!");
    FileWriteLine(AccelFileHandle, "; (C) 2001-2010 Franck Charlet.");
    FileWriteLine(AccelFileHandle, "");
    FileWriteLine(AccelFileHandle, "[ACCELDAT]");
    FileClose(AccelFileHandle);
    EntryToAdd = AccelName + (CStr) " (" + (CStr) AccelFileName + (CStr) ")";
    for(i = 0; i <= (long) (ListViewItemCount(FRMAcceleratorsListview) - 1); i++)
    {
        IniWriteKey("ACCELDAT", "ACCEL" + (CStr) StringNumberComplement(i, 3).Get_String(),
                      ListViewGetItemText(FRMAcceleratorsListview, i, 1), AccelFileName);
        IniWriteKey("ACCELDAT", "COMMAND" + (CStr) StringNumberComplement(i, 3).Get_String(),
                      ListViewGetItemText(FRMAcceleratorsListview, i, 0), AccelFileName);
    }
    AddAcceleratorInArray(RESPROPS_DEFAULT, 0);
    TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewAccelerators, 0, ICON_RES, ICON_RES, 0, 1);
}

// -----------------------------------------------------------------------
// Save an accelerator ini file for current project
void SaveAcceleratorsFile(void)
{
    int i = 0;
    HANDLE AccelFileHandle;

    AccelFileHandle = FileCreateEmpty(AcceleratorsFileToEdit, NO_SECURITY_DESCRIPTOR);
    if(AccelFileHandle != INVALID_HANDLE_VALUE)
    {
        FileWriteLine(AccelFileHandle, "; Chromatic resource accelerators file");
        FileWriteLine(AccelFileHandle, "; Do NOT edit this file manually");
        FileWriteLine(AccelFileHandle, "; unless you know what you're doing !!!");
        FileWriteLine(AccelFileHandle, "; (C) 2001-2010 Franck Charlet.");
        FileWriteLine(AccelFileHandle, "");
        FileWriteLine(AccelFileHandle, "[ACCELDAT]");
        FileClose(AccelFileHandle);
        for(i = 0; i <= (long) ListViewItemCount(FRMAcceleratorsListview) - 1; i++)
        {
            IniWriteKey("ACCELDAT", "ACCEL" + (CStr) StringNumberComplement(i, 3).Get_String(), ListViewGetItemText(FRMAcceleratorsListview, i, 1), AcceleratorsFileToEdit);
            IniWriteKey("ACCELDAT", "COMMAND" + (CStr) StringNumberComplement(i, 3).Get_String(), ListViewGetItemText(FRMAcceleratorsListview, i, 0), AcceleratorsFileToEdit);
        }
    }
    else
    {
        MiscMsgBox(FRMAcceleratorshwnd, "Can't save accelerators file.", MB_ERROR, Requesters);
    }
}
