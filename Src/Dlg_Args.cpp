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
// Dlg_Args.cpp: Arguments prompt form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "MDI_Form.h"
#include "Dlg_Args.h"

// -----------------------------------------------------------------------
// Variables
HWND hFRMArgsLabel;
HWND hFRMArgsOk;
HWND hFRMArgsCancel;
HWND hFRMArgsCombo;
HWND FRMArgshWnd;
HWND FRMArgshTB;
CStr FRMArgsRetVal;
long FRMArgsEntered;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMArgsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ArgsPs;
    int i = 0;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            FRMArgshWnd = hwndDlg;
            hFRMArgsLabel = CreateLabel(4, 2, 268, 50, hwndDlg, "Command to run: \r\n" + (CStr) CommandToEx, 0, 0, 0, 0);
            hFRMArgsOk = CreateButton(118, 94, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            hFRMArgsCancel = CreateButton(197, 94, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            hFRMArgsCombo = CreateComboBox(4, 60, 243, 150, hwndDlg, "", 3, 0, CBS_DROPDOWN | WS_TABSTOP);
            FRMArgshTB = CreateToolBar(248, 59, 25, 23, hwndDlg, GlobalImageList1, 4, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            ToolBarAddButton(FRMArgshTB, "", ARGSCLEAR, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ControlSetText(hwndDlg, "Enter command arguments");
            ComboBoxFillFromIniFile(hFRMArgsCombo, "Args", MainIniFile);
            EnteredArg = "";
            UserArgs = 0;
            FRMArgsEntered = 1;
            FreezeTimer = 1;
            SetFocus(hFRMArgsCombo);
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &ArgsPs);
            GDIDrawHorzSep(hwndDlg, 0, 87, 277);
            EndPaint(hwndDlg, &ArgsPs);
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    switch(ControlGetNotifiedID(lParam))
                    {
                        case ARGSCLEAR:
                            ToolBarDisplayToolTip("Clear arguments list", lParam);
                            return(0);
                    }
                    break;
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == hFRMArgsOk)
            {
                UserArgs = 1;
                if(ArgRequired == 1)
                {
                    if(UserArgs == 1)
                    {
                        if(ControlGetText(hFRMArgsCombo).Len() == 0)
                        {
                            MiscMsgBox(hwndDlg, "Arguments are required.", MB_ERROR, Requesters);
                            SetFocus(hFRMArgsCombo);
                            return(0);
                        }
                    }
                }
                ControlClose(hwndDlg);
                FRMArgsEntered = 0;
                return(0);
            }
            else if((HWND) lParam == hFRMArgsCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMArgshTB)
            {
                switch(wParam)
                {
                    case ARGSCLEAR:
                        ComboBoxReset(hFRMArgsCombo);
                        IniDeleteKey("Args", "", MainIniFile);
                        return(0);
                }
            }
            break;
        case WM_CLOSE:
            EnteredArg = ControlGetText(hFRMArgsCombo);
            ComboBoxSaveInIniFile(hFRMArgsCombo, EnteredArg, "Args", MainIniFile);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}
