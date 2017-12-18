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
// Dlg_UserDialogsBoxProps.h: declaration of used variables / structures for Dlg_UserDialogsProps.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_USERDIALOGSPROPS_H_
#define _DLG_USERDIALOGSPROPS_H_

// -----------------------------------------------------------------------
// Constants
#define PROP_STRING 0
#define PROP_INTEGER 1
#define PROP_LONG 2
#define PROP_BOOLEAN 3
#define PROP_COMBO 4
#define PROP_FILESELECT 5
#define PROP_FONTSELECT 6

#define PROPFLAG_NONE 0
#define PROPFLAG_LEFT 1
#define PROPFLAG_TOP 2
#define PROPFLAG_WIDTH 3
#define PROPFLAG_HEIGHT 4
#define PROPFLAG_CAPTION 5
#define PROPFLAG_BORDER 6
#define PROPFLAG_MAXIMIZE 7
#define PROPFLAG_MINIMIZE 8
#define PROPFLAG_SYSMENU 9
#define PROPFLAG_SCROLLBARS 10

#define PROPDIAG_NBRPROPS 21

// -----------------------------------------------------------------------
// Functions declarations
void CALLBACK FrmUserDiagPropsInitProc(HWND hWnd);
LRESULT CALLBACK FrmUserDiagPropsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillPropertiesStrings(void);
long AllocSubListBox(CStr StringToStore);
void FillPropertiesListbox(long Entries);
LRESULT CALLBACK PropBooleanHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropComboHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropFileHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropFontHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropSubComboHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropEditHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropListboxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropSubListboxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void RefreshProperty(long Position, long Value);

void Prop_DialogDimWidth(void);
void Prop_DialogDimHeight(void);
void Prop_DialogCaption(void);
void Prop_DialogWidth(void);
void Prop_DialogHeight(void);
void Prop_DialogLeft(void);
void Prop_DialogTop(void);
void Prop_DialogMinButton(void);
void Prop_DialogMaxButton(void);
void Prop_DialogScrollBars(void);
void Prop_DialogFontName(void);
void Prop_DialogFontSize(void);

#endif
