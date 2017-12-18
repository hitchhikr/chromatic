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
// Dlg_Tips.cpp: Tip of the Day form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "ConstRes.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMTiphwnd;
HWND hFRMTipNextTip;
HWND hFRMTipClose;
HWND hFRMTipShow;
HWND hFRMTipTip;
HWND hFRMTipDetails;
HFONT hFRMTipBigFont;
long TipFile;
long TipFileLen;
long TipFileLines;
long TipNumberToDisplay;
CStr TipLine;

// -----------------------------------------------------------------------
// Private functions declarations
int CALLBACK FRMTipProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrmTipLabelHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
CStr ParseTipLine(long LineNumber);
long GetRandomTipLine(void);
long LoadTipFile(void);
long IncreaseTipLine(long CurrentLineNumber);

// -----------------------------------------------------------------------
// Display form
void DisplayTip(HWND hParent)
{
    if(LoadTipFile() == 0) return;
    TipNumberToDisplay = GetRandomTipLine();
    CreateModalDialog(-1, -1, 354, 250, hParent, &FRMTipProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    return;
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMTipProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT TipPs;
    int i = 0;
    CStr TipsIniKey;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            FRMTiphwnd = hwndDlg;
            ControlSetText(hwndDlg, "Tip of the day");
            hFRMTipTip = CreateLabel(0, 0, 354, 217, hwndDlg, "", 0, &FrmTipLabelHook, 0, 0);
            hFRMTipDetails = CreateLabel(39, 44, 354 - 49, 217 - 50, hwndDlg, ParseTipLine(TipNumberToDisplay), 0, 0, 0, 0);
            hFRMTipNextTip = CreateButton(195, 224, 77, 23, hwndDlg, "Next tip", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            hFRMTipClose = CreateButton(274, 224, 77, 23, hwndDlg, "Close", 2, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            hFRMTipShow = CreateCheckBox(8, 226, 117, 19, hwndDlg, "Show tips on startup", 3, 0, WS_TABSTOP, 0);
            TipsIniKey = IniReadKey("Layout", "ShowTips", MainIniFile);
            if(TipsIniKey.Len() != 0) CheckBoxSetState(hFRMTipShow, TipsIniKey.Get_Long());
            else CheckBoxSetState(hFRMTipShow, 1);
            hFRMTipBigFont = GDIObtainFont("MS Sans Serif", 9, 0, 1, 0);
            FreezeTimer = 1;
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &TipPs);
            GDIDrawHorzSep(hwndDlg, 0, 217, 354);
            EndPaint(hwndDlg, &TipPs);
            break;
        case WM_CTLCOLORSTATIC:
            if((HWND) lParam != hFRMTipDetails) break;
            SetBkColor((HDC) wParam, GetSysColor(COLOR_WINDOW));
            return((int) GetSysColorBrush(COLOR_WINDOW));
        case WM_COMMAND:
            if((HWND) lParam == hFRMTipNextTip)
            {
                TipNumberToDisplay = IncreaseTipLine(TipNumberToDisplay);
                ControlSetText(hFRMTipDetails, ParseTipLine(TipNumberToDisplay));
                return(0);
            }
            else if((HWND) lParam == hFRMTipClose)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            break;
        case WM_CLOSE:
            if(hFRMTipBigFont != 0) DeleteObject(hFRMTipBigFont);
            IniWriteKey("Layout", "ShowTips", CheckBoxGetState(hFRMTipShow), MainIniFile);
            FreeMem(TipFile);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Tip header label hook
LRESULT CALLBACK FrmTipLabelHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HICON TipIcon;
    PAINTSTRUCT TipPs;

    switch(uMsg)
    {
        case WM_ERASEBKGND:
            GDIFillWindow(hWnd, GetSysColor(COLOR_WINDOW));
            return(1);
        case WM_PAINT:
            BeginPaint(hWnd, &TipPs);
            TipIcon = LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_BASE + ICON_LIGHT));
            DrawIconEx(TipPs.hdc, 4, 8, TipIcon, 0, 0, 0, 0, DI_NORMAL);
            GDIWriteText(TipPs.hdc, 39, 17, "DON'T PANIC !" , GetSysColor(COLOR_WINDOWTEXT), hFRMTipBigFont, 1, 0);
            EndPaint(hWnd, &TipPs);
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Parse a line from the tips file
CStr ParseTipLine(long LineNumber)
{
    CStr ReturnValue;
    CStr BufString;
    int i = 0;
    int InSlash = 0;
    char *TipLineAddress;

    TipLineAddress = GetFileLineAddress((char *) TipFile, TipFileLen, LineNumber);
    while(TipLineAddress[i] != '\r' && TipLineAddress[i] != '\n' && i != TipFileLen && TipLineAddress[i] != 0)
    {
        if(TipLineAddress[i] == '\\')
        {
            InSlash = 1;
        }
        else
        {
            if(InSlash == 1)
            {
                if(TipLineAddress[i] == 'n') BufString = BufString + BufString.Chr(13);
                else BufString = BufString + "\\";
                InSlash = 0;
            }
            else
            {
                BufString = BufString + (CStr) TipLineAddress[i];
            }
        }
        i++;
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return a random line number
// and ensure that it doesn't point to an empty line.
long GetRandomTipLine(void)
{
    long RndLine;
    char *TempLineAddress;

WrongLineNumber:
    RndLine = MathGenerateRandomNumber(TipFileLines);
    TempLineAddress = GetFileLineAddress((char *) TipFile, TipFileLen, RndLine);
    // Wrong line: ask for another number
    if(TempLineAddress[0] == 0 || TempLineAddress[0] == 13 || TempLineAddress[0] == 10) goto WrongLineNumber;
    return(RndLine);
}

// -----------------------------------------------------------------------
// Load the Tips file
long LoadTipFile(void)
{
    TipFile = (long) MLoadFile(TipFileName.Get_String(), &TipFileLen);
    // Abort process
    if(TipFile == 0 || TipFileLen == 0)
    {
        FreeMem(TipFile);
        return(0);
    }
    TipFileLines = CountFileLines((char *) TipFile, TipFileLen);
    return(1);
}

// -----------------------------------------------------------------------
// Increase the current line number
// and ensure that the it doesn't point to an empty line.
long IncreaseTipLine(long CurrentLineNumber)
{
    char *TempLineAddress;

WrongLineNumber:
    CurrentLineNumber++;
    // Reset number if necessary
    if(CurrentLineNumber >= TipFileLines) CurrentLineNumber = 0;
    TempLineAddress = GetFileLineAddress((char *) TipFile, TipFileLen, CurrentLineNumber);
    // Wrong line: ask for another number
    if(TempLineAddress[0] == 0 || TempLineAddress[0] == 13 || TempLineAddress[0] == 10) goto WrongLineNumber;
    return(CurrentLineNumber);
}
