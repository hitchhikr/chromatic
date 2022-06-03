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
// Dlg_About.cpp: About form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "ConstRes.h"
#include "MDI_Form.h"
#include "Dlg_About.h"

// -----------------------------------------------------------------------
// Constants
#define ABOUT_BACKCOLOR 0xffffff
#define ABOUT_LABELSCOLOR1 0xdddddd
#define ABOUT_LABELSCOLOR2 0xbbbbbb
#define ABOUT_LABELSCOLOR3 0x999999
#define ABOUT_LABELSCOLOR4 0x777777
#define ABOUT_LABELSCOLOR5 0x555555
#define ABOUT_LABELSCOLOR6 0x333333
#define ABOUT_LABELSCOLOR7 0x111111
#define ABOUT_LABELSCOLOR8 0x000000

// -----------------------------------------------------------------------
// Variables
HFONT AboutSerFont8;
HFONT AboutCourFont8;
long AboutTimerHandle;
HWND AbouthWnd;
HWND MainAbouthWnd;
CStr AboutText;
long AboutPosInText;
long AboutPosOnScreen;
long AboutScrollAmount;
CStr AbLine1;
CStr AbLine2;
CStr AbLine3;
CStr AbLine4;
CStr AbLine5;
CStr AbLine6;
CStr AbLine7;
CStr AbLine8;
CStr AboutTxtLine;
LOGBRUSH AboutBrush;
HBRUSH hAboutBrush;
long LetterWidth;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMAboutProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT SplashRect;
    PAINTSTRUCT AboutPs;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            MainAbouthWnd = hwndDlg;
            AboutSerFont8 = GDIObtainFont("MS Sans Serif", 8, hwndDlg, 0, 0);
            AboutCourFont8 = GDIObtainFont("Courier New", 9, hwndDlg, 0, 0);
            CreatePictureBox(0, 0, 500, 194, hwndDlg,
                             LoadBitmap(ApphInstance, MAKEINTRESOURCE(MBMP_BASE + MBMP_TITLE)),
                             IMAGE_BITMAP, 0, 0, SS_CENTERIMAGE);
            AbouthWnd = CreateLabel(4, 195, 302, 104, hwndDlg, "", 2, &FrmAboutTypeWriterHook, 0, 0);
            ControlSetText(hwndDlg, "About " + (CStr) AppTitle);
            FreezeTimer = 1;
            AboutPosInText = 1;
            AboutPosOnScreen = 0;
            AbLine1 = "";
            AbLine2 = "";
            AbLine3 = "";
            AbLine4 = "";
            AbLine5 = "";
            AbLine6 = "";
            AbLine7 = "";
            AbLine8 = "";
            AboutScrollAmount = GDIGetTextHeight(AbouthWnd, AboutCourFont8, "hg");
            AboutText = "\r";
			AboutText = AboutText + (CStr) Requesters + (CStr) "\r";
            AboutText = AboutText + (CStr) "Integrated Development Environment\r";
            AboutText = AboutText + (CStr) "(Build: " __DATE__ + (CStr) ")\r\r";
            AboutText = AboutText + (CStr) "Written by Franck Charlet.\r\r";
            AboutText = AboutText + (CStr) "I would like to thank the following\r";
            AboutText = AboutText + (CStr) "people who have more or less,\r";
            AboutText = AboutText + (CStr) "directly or indirectly contributed\r";
            AboutText = AboutText + (CStr) "to this marvellous tool of mine:\r\r";
            AboutText = AboutText + (CStr) "Barry Allyn (cmax)\r";
            AboutText = AboutText + (CStr) "EliCZ (nt driver model)\r";
            AboutText = AboutText + (CStr) "Cppreference.com people (cpp.chm)\r";
            AboutText = AboutText + (CStr) "Jordan Russel (Inno setup)\r";
            AboutText = AboutText + (CStr) "Me (who ?)\r";
            AboutText = AboutText + (CStr) "Roman Novgorodov (x86eas.hlp)\r";
            AboutText = AboutText + (CStr) "Steve Hutchesson (masm32)\r";
            AboutText = AboutText + (CStr) "So long...\r\r";
            AboutText = AboutText + (CStr) "If you can read this you're dying.\r\r\r\r\r\r\r";
            AboutTimerHandle = SetTimer(hwndDlg, 1, 105, &AboutTimerNotify);
            AboutBrush.lbStyle = BS_SOLID;
            AboutBrush.lbHatch = 0;
            AboutBrush.lbColor = ABOUT_BACKCOLOR;
            hAboutBrush = CreateBrushIndirect(&AboutBrush);
            return(0);
        case WM_ERASEBKGND:
            GetClientRect(hwndDlg, &SplashRect);
            FillRect((HDC) wParam, &SplashRect, hAboutBrush);
            return(1);
        case WM_PAINT:
            BeginPaint(hwndDlg, &AboutPs);
            EndPaint(hwndDlg, &AboutPs);
            break;
        case WM_CLOSE:
            if(hAboutBrush != 0) DeleteObject(hAboutBrush);
            if(AboutTimerHandle != 0) KillTimer(hwndDlg, 1);
            if(AboutCourFont8 != 0) DeleteObject(AboutCourFont8);
            if(AboutSerFont8 != 0) DeleteObject(AboutSerFont8);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Typewriter hook
LRESULT CALLBACK FrmAboutTypeWriterHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT TypeWriterPS;

    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &TypeWriterPS);
            EndPaint(hWnd, &TypeWriterPS);
            return(0);
        case WM_ERASEBKGND:
            DisplayTypeWriterLine((HDC) wParam, AbLine1, 0, ABOUT_LABELSCOLOR1);
            DisplayTypeWriterLine((HDC) wParam, AbLine2, AboutScrollAmount, ABOUT_LABELSCOLOR2);
            DisplayTypeWriterLine((HDC) wParam, AbLine3, AboutScrollAmount * 2, ABOUT_LABELSCOLOR3);
            DisplayTypeWriterLine((HDC) wParam, AbLine4, AboutScrollAmount * 3, ABOUT_LABELSCOLOR4);
            DisplayTypeWriterLine((HDC) wParam, AbLine5, AboutScrollAmount * 4, ABOUT_LABELSCOLOR5);
            DisplayTypeWriterLine((HDC) wParam, AbLine6, AboutScrollAmount * 5, ABOUT_LABELSCOLOR6);
            DisplayTypeWriterLine((HDC) wParam, AbLine7, AboutScrollAmount * 6, ABOUT_LABELSCOLOR7);
            DisplayTypeWriterLine((HDC) wParam, AbLine8, AboutScrollAmount * 7, ABOUT_LABELSCOLOR8);
            GDIWriteClippedText((HDC) wParam, AboutPosOnScreen, AboutScrollAmount * 7, GDIGetTextWidth(AbouthWnd, AboutCourFont8, "_"), AboutScrollAmount, "_", ABOUT_LABELSCOLOR8, AboutCourFont8, 0, ABOUT_BACKCOLOR);
            return(1);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

void CALLBACK AboutTimerNotify(HWND hWnd, UINT uMsg, UINT_PTR idEvent, unsigned long dwTime)
{
    HDC AbouthDC = 0;

    switch(uMsg)
    {
        case WM_TIMER:
            AboutPosInText++;
            if((long) AboutPosInText > (long) AboutText.Len())
            {
                AboutPosInText = 1;
                AboutPosOnScreen = 0;
                AbLine1 = AbLine2;
                AbLine2 = AbLine3;
                AbLine3 = AbLine4;
                AbLine4 = AbLine5;
                AbLine5 = AbLine6;
                AbLine6 = AbLine7;
                AbLine7 = AbLine8;
                AbLine8 = "";
            }
            if(strcmp(AboutText.Mid(AboutPosInText, 1).Get_String(), "\r") == 0)
            {
                AboutPosOnScreen = 0;
                AbLine1 = AbLine2;
                AbLine2 = AbLine3;
                AbLine3 = AbLine4;
                AbLine4 = AbLine5;
                AbLine5 = AbLine6;
                AbLine6 = AbLine7;
                AbLine7 = AbLine8;
                AbLine8 = "";
                LetterWidth = 0;
            }
            else
            {
                AbLine8 = AbLine8 + (CStr) AboutText.Mid(AboutPosInText, 1).Get_String();
                LetterWidth = GDIGetTextWidth(AbouthWnd, AboutCourFont8, AboutText.Mid(AboutPosInText, 1));
            }
            AboutPosOnScreen = AboutPosOnScreen + LetterWidth;
            AbouthDC = GetDC(AbouthWnd);
            DisplayTypeWriterLine(AbouthDC, AbLine1, 0, ABOUT_LABELSCOLOR1);
            DisplayTypeWriterLine(AbouthDC, AbLine2, AboutScrollAmount, ABOUT_LABELSCOLOR2);
            DisplayTypeWriterLine(AbouthDC, AbLine3, AboutScrollAmount * 2, ABOUT_LABELSCOLOR3);
            DisplayTypeWriterLine(AbouthDC, AbLine4, AboutScrollAmount * 3, ABOUT_LABELSCOLOR4);
            DisplayTypeWriterLine(AbouthDC, AbLine5, AboutScrollAmount * 4, ABOUT_LABELSCOLOR5);
            DisplayTypeWriterLine(AbouthDC, AbLine6, AboutScrollAmount * 5, ABOUT_LABELSCOLOR6);
            DisplayTypeWriterLine(AbouthDC, AbLine7, AboutScrollAmount * 6, ABOUT_LABELSCOLOR7);
            DisplayTypeWriterLine(AbouthDC, AbLine8, AboutScrollAmount * 7, ABOUT_LABELSCOLOR8);
            GDIWriteClippedText(AbouthDC, AboutPosOnScreen, AboutScrollAmount * 7, GDIGetTextWidth(AbouthWnd, AboutCourFont8, "_"), AboutScrollAmount, "_", ABOUT_LABELSCOLOR8, AboutCourFont8, 0, ABOUT_BACKCOLOR);
            ReleaseDC(AbouthWnd, AbouthDC);
    }
}

// -----------------------------------------------------------------------
// Display a line of text in the typewriter
void DisplayTypeWriterLine(HDC hDC, CStr TextToShow, long Position, long Col)
{
    GDIWriteClippedText(hDC, 0, Position, 302, AboutScrollAmount, TextToShow, Col, AboutCourFont8, 0, ABOUT_BACKCOLOR);
}
