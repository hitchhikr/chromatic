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
// Dlg_EnterValue.cpp: Value prompt form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "Dlg_EnterValue.h"

// -----------------------------------------------------------------------
// Private constants
#define ENTERVALUE_TB_CLEAR 1

// -----------------------------------------------------------------------
// Private variables
HWND hFRMEnterValueTextBox;
HWND hFRMEnterValuehTB;
HWND hFRMEnterValueOk;
HWND hFRMEnterValueCancel;
HWND FRMEnterhwnd;

long EnterValueLen;
long EnterValueType;
long EnterValueAllowEmpty;
CStr EnterValueKey;
CStr EnterValueTxt;
CStr PassEdValue;
CStr PassValue;
long ValidValue;

HICON EnterValueIcon;

// -----------------------------------------------------------------------
// Private functions declarations
int CALLBACK FRMEnterValueProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
long CheckEnteredValue(void);

// -----------------------------------------------------------------------
// Initialize form
int CALLBACK FRMEnterValueProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT EnterValuePs;
    int i = 0;
    CStr KeyRetVal;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, "Prompt");
            FRMEnterhwnd = hwndDlg;
            CreatePictureBox(7, 10, 32, 32, hwndDlg, EnterValueIcon, IMAGE_ICON, 0, 0, SS_CENTERIMAGE);
            CreateLabel(4 + 44, 8, 253, 17, hwndDlg, EnterValueTxt, 0, 0, 0, 0);
            PassValue = PassEdValue;
            hFRMEnterValuehTB = 0;
            hFRMEnterValueOk = CreateButton(145, 64, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            hFRMEnterValueCancel = CreateButton(224, 64, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            switch(EnterValueType)
            {
                case INPUTBOX_SIMPLETEXT:
                    hFRMEnterValueTextBox = CreateTextBox(48, 30, 252, 20, hwndDlg, PassValue, 3, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
                    TextBoxSetMaxLen(hFRMEnterValueTextBox, EnterValueLen);
                    TextBoxSelText(hFRMEnterValueTextBox, -1);
                    ControlSendMessage(hwndDlg, (EN_CHANGE << 16), (long) hFRMEnterValueTextBox);
                    break;
                case INPUTBOX_PASSWORD:
                    hFRMEnterValueTextBox = CreateTextBox(48, 30, 252, 20, hwndDlg, PassValue, 3, 0, WS_TABSTOP | ES_AUTOHSCROLL | ES_PASSWORD, WS_EX_STATICEDGE);
                    TextBoxSetMaxLen(hFRMEnterValueTextBox, EnterValueLen);
                    TextBoxSelText(hFRMEnterValueTextBox, -1);
                    ControlSendMessage(hwndDlg, (EN_CHANGE << 16), (long) hFRMEnterValueTextBox);
                    break;
                case INPUTBOX_HEXA:
                    hFRMEnterValueTextBox = CreateHexBox(48, 30, 252, 20, hwndDlg, PassValue, 3, EnterValueLen, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
                    TextBoxSelText(hFRMEnterValueTextBox, -1);
                    ControlSendMessage(hwndDlg, (EN_CHANGE << 16), (long) hFRMEnterValueTextBox);
                    break;
                case INPUTBOX_COMBO:
                    hFRMEnterValueTextBox = CreateComboBox(48, 30, 227, 150, hwndDlg, PassValue, 3, 0, WS_TABSTOP | CBS_DROPDOWN);
                    hFRMEnterValuehTB = CreateToolBar(276, 29, 25, 23, hwndDlg, GlobalImageList1, 4, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
                    ToolBarAddButton(hFRMEnterValuehTB, "", ENTERVALUE_TB_CLEAR, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
                    ComboBoxSetMaxLen(hFRMEnterValueTextBox, EnterValueLen);
                    TextBoxSelText(hFRMEnterValueTextBox, -1);
                    if(EnterValueKey.Len() != 0) ComboBoxFillFromIniFile(hFRMEnterValueTextBox, EnterValueKey, MainIniFile);
                    ControlSendMessage(hwndDlg, (CBN_EDITCHANGE << 16), (long) hFRMEnterValueTextBox);
                    break;
            }
            ValidValue = 0;
            FreezeTimer = 1;
            SetFocus(hFRMEnterValueTextBox);
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &EnterValuePs);
            GDIDrawHorzSep(hwndDlg, 0, 57, 304);
            EndPaint(hwndDlg, &EnterValuePs);
            break;
        case WM_NOTIFY:
            if(EnterValueType == INPUTBOX_COMBO)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case TTN_NEEDTEXT:
                        switch(ControlGetNotifiedID(lParam))
                        {
                            case ENTERVALUE_TB_CLEAR:
                                ToolBarDisplayToolTip("Clear list", lParam);
                                return(0);
                        }
                        break;
                }
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == hFRMEnterValueOk)
            {
                if(CheckEnteredValue() == 1) ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == hFRMEnterValueCancel)
            {
                PassValue = "";
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == hFRMEnterValueTextBox)
            {
                if(EnterValueType == INPUTBOX_COMBO)
                {
                    switch(ControlGetNotifiedCommand(wParam))
                    {
                        case CBN_SELENDOK:
                        case CBN_EDITCHANGE:
                            if(EnterValueAllowEmpty)
                            {
                                ControlEnable(hFRMEnterValueOk, 1);
                            }
                            else
                            {
                                if(ControlGetTextLen(hFRMEnterValueTextBox) > 0)
                                {
                                    ControlEnable(hFRMEnterValueOk, 1);
                                }
                                else
                                {
                                    ControlEnable(hFRMEnterValueOk, 0);
                                }
                            }
                            break;
                        case CBN_SELCHANGE:
                            if(ComboBoxGetIndex(hFRMEnterValueTextBox) != CB_ERR)
                            {
                                ControlEnable(hFRMEnterValueOk, 1);
                            }
                            else
                            {
                                ControlEnable(hFRMEnterValueOk, 0);
                            }
                            break;
                    }
                }
                else
                {
                    switch(ControlGetNotifiedCommand(wParam))
                    {
                        case EN_CHANGE:
                            if(EnterValueAllowEmpty)
                            {
                                ControlEnable(hFRMEnterValueOk, 1);
                            }
                            else
                            {
                                if(TextBoxGetLen(hFRMEnterValueTextBox) > 0)
                                {
                                    ControlEnable(hFRMEnterValueOk, 1);
                                }
                                else
                                {
                                    ControlEnable(hFRMEnterValueOk, 0);
                                }
                            }
                            break;
                    }
                }
            }
            else if((HWND) lParam == hFRMEnterValuehTB)
            {
                if(EnterValueType == INPUTBOX_COMBO)
                {
                    switch(wParam)
                    {
                        case ENTERVALUE_TB_CLEAR:
                            ComboBoxReset(hFRMEnterValueTextBox);
                            IniDeleteKey(EnterValueKey, "", MainIniFile);
                            ControlSendMessage(hwndDlg, (CBN_EDITCHANGE << 16), (long) hFRMEnterValueTextBox);
                            return(0);
                    }
                }
            }
            break;
        case WM_CLOSE:
            FreezeTimer = 0;
            EnterValueType = INPUTBOX_SIMPLETEXT;
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Validate entered value
long CheckEnteredValue(void)
{
    // Retrieve user's input
    PassValue = ControlGetText(hFRMEnterValueTextBox);
    // Check if it's empty or not
    if(!EnterValueAllowEmpty)
    {
        if(PassValue.Len() == 0)
        {
            MiscMsgBox(FRMEnterhwnd, "Empty value is not allowed.", MB_ERROR, Requesters);
            SetFocus(hFRMEnterValueTextBox);
            return(0);
        }
    }
    switch(EnterValueType)
    {
        // SIMPLETEXT and PASSWORD don't requires any checkup
        // (No rules are defined for these and Windows ensures that
        // the number of chars is correct).
        case INPUTBOX_SIMPLETEXT:
        case INPUTBOX_PASSWORD:
            break;
        case INPUTBOX_HEXA:
            // Check if the length matches the requested one
            if(PassValue.Len() < TextBoxGetMaxLen(hFRMEnterValueTextBox))
            {
                // No: halt process
                MiscMsgBox(FRMEnterhwnd, TextBoxGetMaxLen(hFRMEnterValueTextBox) + (CStr) " chars required.", MB_ERROR, Requesters);
                SetFocus(hFRMEnterValueTextBox);
                return(0);
            }
            // Check if value is a hexadecimal string
            if(StringIsHexaDecimal(PassValue) != 0)
            {
                // No: halt process
                MiscMsgBox(FRMEnterhwnd, "Invalid hexadecimal value.", MB_ERROR, Requesters);
                SetFocus(hFRMEnterValueTextBox);
                return(0);
            }
            break;
        case INPUTBOX_COMBO:
            // Save combo datas in requested key/ini file.
            ComboBoxSaveInIniFile(hFRMEnterValueTextBox, PassValue, EnterValueKey, MainIniFile);
            break;
    }
    // (Internally validated)
    ValidValue = 1;
    return(1);
}

// -----------------------------------------------------------------------
// Prompt user for new datas
CStr MiscInputBox(HWND hParent, CStr NameTitle, long Icon, CStr ValueToPass, long PromptLen, long PromptType, CStr PromptIniKey)
{
    CStr ReturnValue;
 
    EnterValueTxt = NameTitle;
    PassEdValue = ValueToPass;
    EnterValueLen = PromptLen;
    EnterValueType = PromptType & ~INPUTBOX_ALLOW_EMPTY;
    EnterValueAllowEmpty = PromptType & INPUTBOX_ALLOW_EMPTY;
    EnterValueKey = PromptIniKey;
    EnterValueIcon = LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + Icon));
    CreateModalDialog(-1, -1, 304, 90, hParent, &FRMEnterValueProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    if(ValidValue == 1) ReturnValue = PassValue;
    return(ReturnValue);
}
