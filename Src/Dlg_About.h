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
// Dlg_About.h: declaration of used variables / structures for Dlg_About.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_ABOUT_H_
#define _DLG_ABOUT_H_

// -----------------------------------------------------------------------
// Variables
extern HFONT AboutSerFont8;
extern HFONT AboutCourFont8;
extern long AboutTimerHandle;
extern HWND AbouthWnd;
extern HWND MainAbouthWnd;
extern CStr AboutText;
extern long AboutPosInText;
extern long AboutPosOnScreen;
extern long AboutScrollAmount;
extern CStr AbLine1;
extern CStr AbLine2;
extern CStr AbLine3;
extern CStr AbLine4;
extern CStr AbLine5;
extern CStr AbLine6;
extern CStr AbLine7;
extern CStr AbLine8;
extern CStr AboutTxtLine;
extern LOGBRUSH AboutBrush;
extern HBRUSH hAboutBrush;
extern long LetterWidth;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK FRMAboutProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FrmAboutTypeWriterHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CALLBACK AboutTimerNotify(HWND hWnd, UINT uMsg, UINT_PTR idEvent, unsigned long dwTime);
void DisplayTypeWriterLine(HDC hDC, CStr TextToShow, long Position, long Col);

#endif
