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
// Dlg_APIList.cpp: API list form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "APIDb.h"
#include "Globals.h"
#include "ConstMisc.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_APIList.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMAPIListhWnd;
HDC APIListhDC;
long APIListWidth;
long APIListHeight;
HFONT APIListhFont;

// -----------------------------------------------------------------------
// Initialize API list window --- '
void CALLBACK FRMAPIListInitProc(HWND hWnd)
{
    FRMAPIListhWnd = hWnd;
    APIhListBox = CreateListBox(1, 1, 0, 1, hWnd, 0, &FRMAPIhListBoxHook, 0, WS_VSCROLL | LBS_SORT, 0);
    APIListhFont = GDIObtainFont(CurFontName, CurFontSize, hMDIform.hWnd, 0, 0);
    ControlSetFont(APIhListBox, APIListhFont);
}

// -----------------------------------------------------------------------
// API list window hook
LRESULT CALLBACK FRMAPIListWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT APIPaint;
    HPEN APIPen = 0;
    RECT ListRect;
    POINT ToolPOINT;

    switch(uMsg)
    {
        case WM_DESTROY:
            if(APIListhFont != 0) DeleteObject(APIListhFont);
            APIListMode = 0;
            APIListhFont = 0;
            APIhListBox = 0;
            FRMAPIListhWnd = 0;
            return(0);
        case WM_PAINT:
            APIListhDC = BeginPaint(hWnd, &APIPaint);
            GetWindowRect(hWnd, &ListRect);
            APIPen = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNFACE));
            SelectObject(APIListhDC, APIPen);
            MoveToEx(APIListhDC, 0, 0, &ToolPOINT);
            LineTo(APIListhDC, ListRect.right - 1, 0);
            MoveToEx(APIListhDC, 0, 0, &ToolPOINT);
            LineTo(APIListhDC, 0, ListRect.bottom - 1);
            DeleteObject(APIPen);
            APIPen = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW));
            SelectObject(APIListhDC, APIPen);
            MoveToEx(APIListhDC, 0, ListRect.bottom - 1, &ToolPOINT);
            LineTo(APIListhDC, ListRect.right - 1, ListRect.bottom - 1);
            MoveToEx(APIListhDC, ListRect.right - 1, ListRect.bottom - 1, &ToolPOINT);
            LineTo(APIListhDC, ListRect.right - 1, 0);
            DeleteObject(APIPen);
            EndPaint(hWnd, &APIPaint);
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Status listbox control hook
LRESULT CALLBACK FRMAPIhListBoxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            if(wParam == 32 || wParam == 13)
            {
                CompleteWord(hWnd);
                return(0);
            }
            // (
            if(wParam == 0x35)
            {
                CompleteWord(hWnd);
                SmartInsert(CurrentForm, "(");
                return(0);
            }
            if(wParam == 27)
            {
                SendMessage(CurrentForm, WM_URGENTKILLCLIST, 0, 0);
                return(0);
            }
            break;
        case WM_LBUTTONDBLCLK:
            CompleteWord(hWnd);
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Replace a word with the selected entry
void CompleteWord(HWND hWnd)
{
    CStr WordToComplete;

    WordToComplete = ListBoxGetCurrentItem(hWnd);
    ChildStruct = LoadStructure(CurrentForm);
    SelectLeftWord(ChildStruct->hChildCodeMax, LineToDisplay);
    SmartInsert(CurrentForm, &WordToComplete);
    SendMessage(CurrentForm, WM_URGENTKILLCLIST, 0, 0);
}
