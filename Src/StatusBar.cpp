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
// StatusBar.cpp: Status bar handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "MsgCodeMax.h"
#include "Menus.h"
#include "ConstMenus.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"

// -----------------------------------------------------------------------
// Variables
long SBParts[9+1];
HWND hStatusBar;

// -----------------------------------------------------------------------
// Redim the status bar
void SetStatusBarParts(void)
{
    if(NbForms != 0)
    {
        SBParts[0] = ControlWidth(hStatusBar) / 2;
        SBParts[1] = ControlWidth(hStatusBar) - (14 * 9) - 4 - 1 - 3;
        SBParts[2] = ControlWidth(hStatusBar) - (14 * 8) - 4 - 3;
        SBParts[3] = ControlWidth(hStatusBar) - (14 * 7) - 4 - 3;
        SBParts[4] = ControlWidth(hStatusBar) - (14 * 6) - 3;
        SBParts[5] = ControlWidth(hStatusBar) - (14 * 5) - 3;
        SBParts[6] = ControlWidth(hStatusBar) - (14 * 4) - 3;
        SBParts[7] = ControlWidth(hStatusBar) - (14 * 3) - 3;
        SBParts[8] = ControlWidth(hStatusBar) - (14 * 2) - 3;
        SBParts[9] = ControlWidth(hStatusBar) - 14 - 3;
        StatusBarSetParts(hStatusBar, 10, (long *) SBParts);
        ChildStruct = LoadStructure(CurrentForm);
        if(CM_IsOvertypeEnabled(ChildStruct->hChildCodeMax) == 1)
        {
            StatusBarSetText(hStatusBar, 2, STATUSBAR_PARTSUNKEN, "O", 0);
        }
        else
        {
            StatusBarSetText(hStatusBar, 2, STATUSBAR_PARTRAISED, "O", 0);
        }
        if(CM_IsColorSyntaxEnabled(ChildStruct->hChildCodeMax) == 1)
        {
            StatusBarSetText(hStatusBar, 3, STATUSBAR_PARTSUNKEN, "X", 0);
        }
        else
        {
            StatusBarSetText(hStatusBar, 3, STATUSBAR_PARTRAISED, "X", 0);
        }
        if(CM_IsWhitespaceDisplayEnabled(ChildStruct->hChildCodeMax) == 1)
        {
            StatusBarSetText(hStatusBar, 4, STATUSBAR_PARTSUNKEN, "W", 0);
        }
        else
        {
            StatusBarSetText(hStatusBar, 4, STATUSBAR_PARTRAISED, "W", 0);
        }
        if(CM_IsTabExpandEnabled(ChildStruct->hChildCodeMax) == 1)
        {
            StatusBarSetText(hStatusBar, 5, STATUSBAR_PARTSUNKEN, "T", 0);
        }
        else
        {
            StatusBarSetText(hStatusBar, 5, STATUSBAR_PARTRAISED, "T", 0);
        }
        if(CM_IsSelBoundsEnabled(ChildStruct->hChildCodeMax) == 1)
        {
            StatusBarSetText(hStatusBar, 6, STATUSBAR_PARTSUNKEN, "B", 0);
        }
        else
        {
            StatusBarSetText(hStatusBar, 6, STATUSBAR_PARTRAISED, "B", 0);
        }
        if(CM_IsSmoothScrollingEnabled(ChildStruct->hChildCodeMax) == 1)
        {
            StatusBarSetText(hStatusBar, 7, STATUSBAR_PARTSUNKEN, "S", 0);
        }
        else
        {
            StatusBarSetText(hStatusBar, 7, STATUSBAR_PARTRAISED, "S", 0);
        }
        if(CM_IsLineToolTipsEnabled(ChildStruct->hChildCodeMax) == 1)
        {
            StatusBarSetText(hStatusBar, 8, STATUSBAR_PARTSUNKEN, "L", 0);
        }
        else
        {
            StatusBarSetText(hStatusBar, 8, STATUSBAR_PARTRAISED, "L", 0);
        }
        if(CM_IsNormalizeCaseEnabled(ChildStruct->hChildCodeMax) == 1)
        {
            StatusBarSetText(hStatusBar, 9, STATUSBAR_PARTSUNKEN, "N", 0);
        }
        else
        {
            StatusBarSetText(hStatusBar, 9, STATUSBAR_PARTRAISED, "N", 0);
        }
    }
    else
    {
        SBParts[0] = -1;
        StatusBarSetParts(hStatusBar, 1, (long *) &SBParts);
    }
}

// -----------------------------------------------------------------------
// Clear the status bar
void ClearStatusBarParts(void)
{
    if(ControlIsVisible(hStatusBar) != 0)
    {
        SBParts[0] = -1;
        StatusBarSetParts(hStatusBar, 1, (long *) SBParts);
    }
}

// -----------------------------------------------------------------------
// Show/Hide statusbar
void ShowHideStatusBar(long bShow)
{
    switch(bShow)
    {
        case 0:
            CheckMenuItem(hViewMenu, MENU_VIEW_STATUSBAR_ID, MF_BYPOSITION | MF_UNCHECKED);
            IniWriteKey("Layout", "StatusBar", "0", MainIniFile);
            ControlVisible(hStatusBar, 0);
            ResizeMDIform();
            break;
        case 1:
            CheckMenuItem(hViewMenu, MENU_VIEW_STATUSBAR_ID, MF_BYPOSITION | MF_CHECKED);
            IniWriteKey("Layout", "StatusBar", "1", MainIniFile);
            ControlVisible(hStatusBar, 1);
            SetStatusBarParts();
            ResizeMDIform();
            if(NbForms != 0) RefreshStatusBar(CurrentForm);
            else ClearStatusBarParts();
            break;
    }
}
