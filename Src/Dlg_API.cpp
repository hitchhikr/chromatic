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
// Dlg_API.cpp: API tooltips form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "APIDb.h"
#include "Globals.h"
#include "MDI_Childs.h"
#include "Dlg_API.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMAPIhwnd;
HDC APIhDC;
HGDIOBJ APIOldObject;
SIZE APISize;
HBRUSH APIhBrush;
HPEN APIPen;
long APIXDim;
long APIYDim;

// -----------------------------------------------------------------------
// Initialize API window
void CALLBACK FRMAPIInitProc(HWND hWnd)
{
    FRMAPIhwnd = hWnd;
    APIhText = CreateLabel(0, 0, 0, 0, hWnd, "", 0, &FRMAPIHookProc, WS_DISABLED, 0);
}

// -----------------------------------------------------------------------
// API tooltip window hook
LRESULT CALLBACK FRMAPIWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case MSG_SPLASH_PAINT:
            return((long) GetSysColorBrush(COLOR_INFOBK));
        case WM_DESTROY:
            APIMode = 0;
            if(APIhFont != 0) DeleteObject(APIhFont);
            if(APIhFontBold != 0) DeleteObject(APIhFontBold);
            StringReleaseSplit(APIArrParse);
            APIhFont = 0;
            APIhFontBold = 0;
            APIhText = 0;
            APINoRedraw = 0;
            HighOn = 0;
            HighPar = 0;
            FRMAPIhwnd = 0;
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Hook for API tooltip informations label handler
LRESULT CALLBACK FRMAPIHookProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT APIPaint;
    CStr VirgToParse;
    long YComput = 0;
    long MaxStrX = 0;
    long MaxStrY = 0;
    long TxtPosX = 0;
    long TxtPosY = 0;
    RECT BackRect;
    POINT ToolPOINT;
    long *ParsedVirg = 0;
    CStr Virg;
    long Bot = 0;
    long Rig = 0;
    int i = 0;
    int j = 0;
    long CurAffiche = 0;
    long ParentTop = 0;
    long ParentLeft = 0;
    long ParentWidth = 0;
    long ParentHeight = 0;

    switch(uMsg)
    {
        case WM_PAINT:
            APIhDC = BeginPaint(APIhText, &APIPaint);
            if(APIhDC != 0)
            {
                APIOldObject = SelectObject(APIhDC, APIhFontBold);
                GetTextExtentPoint32(APIhDC, StringGetSplitElement(APIString, APIArrParse, 0).Get_String(), StringGetSplitElement(APIString, APIArrParse, 0).Len(), &APISize);
                YComput++;
                if(APISize.cx > MaxStrX) MaxStrX = APISize.cx;
                if(APISize.cy > MaxStrY) MaxStrY = APISize.cy;
                SelectObject(APIhDC, APIhFont);
                for(i = 1; i <= 2; i++)
                {
                    GetTextExtentPoint32(APIhDC, StringGetSplitElement(APIString, APIArrParse, i).Get_String(), StringGetSplitElement(APIString, APIArrParse, i).Len(), &APISize);
                    YComput++;
                    if(APISize.cx > MaxStrX) MaxStrX = APISize.cx;
                    if(APISize.cy > MaxStrY) MaxStrY = APISize.cy;
                }
                SelectObject(APIhDC, APIhFontBold);
                GetTextExtentPoint32(APIhDC, StringGetSplitElement(APIString, APIArrParse, 0).Get_String(), StringGetSplitElement(APIString, APIArrParse, 0).Len(), &APISize);
                if(APISize.cx > MaxStrX) MaxStrX = APISize.cx;
                if(APISize.cy > MaxStrY) MaxStrY = APISize.cy;
                SelectObject(APIhDC, APIhFont);
                j = 4;
                if(StringGetSplitElement(APIString, APIArrParse, j).Len() == 0) j++;
                for(i = j; i <= StringGetSplitUBound(APIArrParse); i++)
                {
                    GetTextExtentPoint32(APIhDC, StringGetSplitElement(APIString, APIArrParse, i).Get_String(), StringGetSplitElement(APIString, APIArrParse, i).Len(), &APISize);
                    YComput++;
                    if(APISize.cx > MaxStrX) MaxStrX = APISize.cx;
                    if(APISize.cy > MaxStrY) MaxStrY = APISize.cy;
                }
                // Draw background
                BackRect.top = 0;
                BackRect.left = 0;
                BackRect.right = (MaxStrX + 6);
                Rig = (MaxStrX + 6);
                BackRect.bottom = (MaxStrY * YComput + 6);
                Bot = (MaxStrY * YComput + 6);
                SetWindowPos(APIhText, HWND_TOP, APIX, APIY, BackRect.right, BackRect.bottom, 0);
                APIhBrush = GetSysColorBrush(COLOR_INFOBK);
                // Draw borders and background
                FillRect(APIhDC, &BackRect, APIhBrush);
                APIPen = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_BTNFACE));
                SelectObject(APIhDC, APIPen);
                MoveToEx(APIhDC, 0, 0, &ToolPOINT);
                LineTo(APIhDC, Rig - 1, 0);
                MoveToEx(APIhDC, 0, 0, &ToolPOINT);
                LineTo(APIhDC, 0, Bot - 1);
                DeleteObject(APIPen);
                APIPen = CreatePen(PS_SOLID, 0, GetSysColor(COLOR_3DDKSHADOW));
                SelectObject(APIhDC, APIPen);
                MoveToEx(APIhDC, 0, Bot - 1, &ToolPOINT);
                LineTo(APIhDC, Rig - 1, Bot - 1);
                MoveToEx(APIhDC, Rig - 1, Bot - 1, &ToolPOINT);
                LineTo(APIhDC, Rig - 1, 0);
                DeleteObject(APIPen);
                // Draw contents
                if(HighOn == 1)
                {
                    if(APICurHiglight == -1)
                    {
                        SetTextColor(APIhDC, APIColor);
                    }
                    else
                    {
                        SetTextColor(APIhDC, GetSysColor(COLOR_INFOTEXT));
                    }
                }
                else
                {
                    SetTextColor(APIhDC, GetSysColor(COLOR_INFOTEXT));
                }
                SetBkColor(APIhDC, GetSysColor(COLOR_INFOBK));
                SelectObject(APIhDC, APIhFontBold);
                TextOut(APIhDC, 3, TxtPosY + 3, StringGetSplitElement(APIString, APIArrParse, 0).Get_String(), StringGetSplitElement(APIString, APIArrParse, 0).Len());
                SetTextColor(APIhDC, GetSysColor(COLOR_INFOTEXT));
                TxtPosY = TxtPosY + MaxStrY;
                SelectObject(APIhDC, APIhFont);
                for(i = 1; i <= 2; i++)
                {
                    TextOut(APIhDC, 3, TxtPosY + 3, StringGetSplitElement(APIString, APIArrParse, i).Get_String(), StringGetSplitElement(APIString, APIArrParse, i).Len());
                    TxtPosY = TxtPosY + MaxStrY;
                }
                if(strcmp(StringGetSplitElement(APIString, APIArrParse, 3).Get_String(), "-") != 0)
                {
                    SelectObject(APIhDC, APIhFontBold);
                    for(i = 3; i <= 3; i++)
                    {
                        TextOut(APIhDC, 3, TxtPosY + 3, StringGetSplitElement(APIString, APIArrParse, i).Get_String(), StringGetSplitElement(APIString, APIArrParse, i).Len());
                        TxtPosY = TxtPosY + MaxStrY;
                    }
                    CurAffiche = 0;
                }
                j = 4;
                SelectObject(APIhDC, APIhFont);
                if(StringGetSplitElement(APIString, APIArrParse, j).Len() == 0)
                {
                    j = 5;
                    goto EndArgs;
                }
NextArgsLine:   VirgToParse = StringGetSplitElement(APIString, APIArrParse, j);
                ParsedVirg = StringSplit(VirgToParse, ",");
                if(StringGetSplitUBound(ParsedVirg) == -1) goto EndArgs;
                TxtPosX = 3;
                for(i = 0; i <= StringGetSplitUBound(ParsedVirg); i++)
                {
                    if(StringGetSplitElement(VirgToParse, ParsedVirg, i).Len() != 0)
                    {
                        if(CurAffiche == MaxAPIArgs) Virg = StringGetSplitElement(VirgToParse, ParsedVirg, i);
                        else Virg = StringGetSplitElement(VirgToParse, ParsedVirg, i).Get_String() + (CStr) ",";
                        if(HighOn == 0) goto NoAPIhighLight;
                        if(APICurHiglight == -1) goto NoAPIhighLight;
                        if(CurAffiche == APICurHiglight)
                        {
                            SetTextColor(APIhDC, APIColor);
                        }
                        else
                        {
NoAPIhighLight:             SetTextColor(APIhDC, GetSysColor(COLOR_INFOTEXT));
                        }
                        TextOut(APIhDC, TxtPosX, TxtPosY + 3, Virg.Get_String(), Virg.Len());
                        GetTextExtentPoint32(APIhDC, Virg.Get_String(), Virg.Len(), &APISize);
                        TxtPosX = TxtPosX + APISize.cx;
                        CurAffiche++;
                    }
                }
                // Next line of arguments
                TxtPosY = TxtPosY + MaxStrY;
                j++;
                goto NextArgsLine;
EndArgs:        if(StringGetSplitElement(APIString, APIArrParse, j).Len() == 0)
                {
                    if(_strcmpi(StringGetSplitElement(APIString, APIArrParse, j + 1).Get_String(), "Note: -") != 0)
                    {
                        SetTextColor(APIhDC, 0x55FF);
                    }
                    else
                    {
                        SetTextColor(APIhDC, GetSysColor(COLOR_INFOTEXT));
                    }
                }
                else
                {
                    if(_strcmpi(StringGetSplitElement(APIString, APIArrParse, j).Get_String(), "Note: -") != 0)
                    {
                        SetTextColor(APIhDC, 0x55FF);
                    }
                    else
                    {
                        SetTextColor(APIhDC, GetSysColor(COLOR_INFOTEXT));
                    }
                }
                for(i = j; i <= StringGetSplitUBound(APIArrParse); i++)
                {
                    if(StringGetSplitElement(APIString, APIArrParse, i).Len() != 0)
                    {
                        TextOut(APIhDC, 3, TxtPosY + 3, StringGetSplitElement(APIString, APIArrParse, i).Get_String(),
                                StringGetSplitElement(APIString, APIArrParse, i).Len());
                        TxtPosY = TxtPosY + MaxStrY;
                        SetTextColor(APIhDC, GetSysColor(COLOR_INFOTEXT));
                    }
                }
                SelectObject(APIhDC, APIOldObject);
                // Move the window in a safe place
                ParentTop = ControlTop(FRMAPIhwnd);
                ParentLeft = ControlLeft(FRMAPIhwnd);
                ParentWidth = ControlWidth(FRMAPIhwnd);
                ParentHeight = ControlHeight(FRMAPIhwnd);
                if((long) (ParentTop + (Bot)) > (long) GetSystemMetrics(SM_CYFULLSCREEN)) ParentTop = ParentTop - (Bot) - (APIOldRealPosition);
                if((long) (ParentLeft + (Rig)) > (long) GetSystemMetrics(SM_CXFULLSCREEN)) ParentLeft = ParentLeft - (Rig);
                if(ParentTop < 0) ParentTop = 0;
                if(ParentLeft < 0) ParentLeft = 0;
                // Set width/height
                ParentWidth = Rig;
                ParentHeight = Bot;
                MoveWindow(FRMAPIhwnd, ParentLeft, ParentTop, ParentWidth, ParentHeight, 1);
                APIXDim = Rig;
                APIYDim = Bot;
                APINoRedraw = 0;
            }
            EndPaint(APIhText, &APIPaint);
            StringReleaseSplit(ParsedVirg);
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}
