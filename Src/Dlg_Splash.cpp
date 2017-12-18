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
// Dlg_Splash.cpp: Splash Form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "ConstRes.h"
#include "Dlg_Splash.h"

// -----------------------------------------------------------------------
// Constants
#define SPLASH_BACKCOLOR 0xFFFFFF
#define SPLASH_LABELSCOLOR 0xEBEBEB
#define SPLASH_BANDCOLOR 0x0 //0x6E5920

// -----------------------------------------------------------------------
// Variables
HWND hSplash;
HWND FRMSplashStatusLabel;
HWND FRMSplashVersionLabel;
HFONT SerFont8;
HBRUSH hBandBrush;

// -----------------------------------------------------------------------
// Private functions
LRESULT CALLBACK FrmSplashLabelVersionHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrmSplashLabelInfoHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------
// Initialize splash window
void CALLBACK FrmSplashInitProc(HWND hWnd)
{
    CreatePictureBox(0, 0, 500, 194, hWnd,
                     LoadBitmap(ApphInstance,
                     MAKEINTRESOURCE(MBMP_BASE + MBMP_TITLE)),
                     IMAGE_BITMAP, 0, 0, SS_CENTERIMAGE);
    //441
    FRMSplashVersionLabel = CreateLabel(436, 198, 58, 16, hWnd, "", 2, &FrmSplashLabelVersionHook, 0, 0);
    FRMSplashStatusLabel = CreateLabel(6, 198, 264, 16, hWnd, "", 3, &FrmSplashLabelInfoHook, 0, 0);
    FrmSplashSetState("");
    hBandBrush = GDICreateColorBrush(SPLASH_BANDCOLOR);
    CursorSetWait();
}

// -----------------------------------------------------------------------
// Splash window hook
LRESULT CALLBACK FrmSplashWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT SplashPS;
    RECT BandRect;

    switch(uMsg)
    {
        case MSG_SPLASH_PAINT:
            return(SPLASH_BACKCOLOR);
        case WM_PAINT:
            BeginPaint(hWnd, &SplashPS);
            GetClientRect(hWnd, &BandRect);
            BandRect.top = BandRect.bottom - 19;
            BandRect.bottom--;
            BandRect.right--;
            BandRect.left++;
            FillRect(SplashPS.hdc, &BandRect, hBandBrush);
            EndPaint(hWnd, &SplashPS);
            return(0);
        case WM_DESTROY:
            CursorSetNormal();
            if(hBandBrush != 0) DeleteObject(hBandBrush);
            return(0);
    }
    return(ControlCallwindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Version label hook
LRESULT CALLBACK FrmSplashLabelVersionHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT ClearRect;
    PAINTSTRUCT SplashPS;
    
    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &SplashPS);
            EndPaint(hWnd, &SplashPS);
            return(0);
        case WM_ERASEBKGND:
            GetClientRect(hWnd, &ClearRect);
            FillRect((HDC) wParam, &ClearRect, hBandBrush);
            GDIWriteText((HDC) wParam, 0, 0, &VersionString, SPLASH_LABELSCOLOR, WASerifFont, 0, SPLASH_BANDCOLOR);
            return(1);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Status label hook
LRESULT CALLBACK FrmSplashLabelInfoHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT ClearRect;
    PAINTSTRUCT SplashPS;

    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &SplashPS);
            EndPaint(hWnd, &SplashPS);
            return(0);
        case WM_ERASEBKGND:
            GetClientRect(hWnd, &ClearRect);
            FillRect((HDC) wParam, &ClearRect, hBandBrush);
            GDIWriteText((HDC) wParam, 0, 0, &LoadingState, SPLASH_LABELSCOLOR, WASerifFont, 0, SPLASH_BANDCOLOR);
            return(1);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Display state message
void FrmSplashSetState(CStr TxtToSet)
{
    if(FRMSplashStatusLabel != 0)
    {
        LoadingState = TxtToSet;
        RedrawWindow(FRMSplashStatusLabel, 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW);
        RedrawWindow(FRMSplashVersionLabel, 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_ERASENOW);
        MiscDoEvents(0, 0, 0);
    }
}
