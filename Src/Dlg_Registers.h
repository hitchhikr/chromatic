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
// Dlg_Registers.h: declaration of used variables / structures for Dlg_Registers.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_REGISTERS_H_
#define _DLG_REGISTERS_H_

// -----------------------------------------------------------------------
// Variables
extern long FrmRegsOn;
extern HWND FrmRegshWnd;
extern CStr FrmRegsFoundFile;
extern CStr FrmRegsPrimLinesTxt;
extern long *FrmRegsFoundLinesTxt;
extern long FrmRegsSelFirstLine;

extern CStr FrmRegsCmTextReg;

extern HWND FrmRegsListbox1;
extern HWND FrmRegsListbox2;
extern HWND FrmRegsCmdClose;
extern HWND FrmRegsCmdSave;
extern HWND FrmRegsCmdView;
extern HWND FrmRegsCmdToDoc;

extern long FrmRegsWidth;
extern long FrmRegsHeight;

extern CList <int> FoundLines;
extern CList <int> FoundPos;
extern CList <char *> FoundReg;
extern CList <char *> TabLines;
extern long ResultMem;
extern CStr ResultStr;

// -----------------------------------------------------------------------
// Functions declarations
void CALLBACK FrmRegsInitProc(HWND hWnd);
LRESULT CALLBACK FrmRegsWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillRegisterList(void);
void FrmRegsSaveReport(void);
void FrmRegsToDoc(void);

#endif
