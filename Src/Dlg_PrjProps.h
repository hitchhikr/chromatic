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
//	1. Redistributions of source code must retain the above copyright notice,
//	   this list of conditions and the following disclaimer.
//
//	2. Redistributions in binary form must reproduce the above copyright notice,
//	   this list of conditions and the following disclaimer in the documentation
//	   and/or other materials provided with the distribution.
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
// Dlg_PrjProps.h: declaration of used variables / structures for Dlg_PrjProps.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_PRJPROPS_H_
#define _DLG_PRJPROPS_H_

// -----------------------------------------------------------------------
// Variables
extern HWND FRMPrjPropshWnd;
extern HWND FRMPrjPropsFr1;
extern HWND FRMPrjPropsFr2;
extern HWND FRMPrjPropsFr3;
extern HWND FRMPrjPropsTxtTitle;
extern HWND FRMPrjPropsFileType;
extern HWND FRMPrjPropsSetIcon;
extern HWND FRMPrjPropsIcon;
extern HANDLE FRMPrjPropsIconhandle;
extern HWND FRMPrjPropsTxtMajor;
extern HWND FRMPrjPropsTxtMinor;
extern HWND FRMPrjPropsTxtRevision;
extern HWND FRMPrjPropsTxtRelease;
extern HWND FRMPrjPropsOS;
extern HWND FRMPrjPropsTypeProp;
extern HWND FRMPrjPropsTypeTxt;

extern HWND FRMPrjPropsIncludeVersion;
extern HWND FRMPrjPropsIncludeXPManifest;

extern HWND FRMPrjPropsReg;
extern HWND FRMPrjPropsOk;
extern HWND FRMPrjPropsCancel;

extern CStr LastPrjIconDir;
extern long LastListBoxIndex;

extern CStr TmpPrj_Comments;
extern CStr TmpPrj_CompanyName;
extern CStr TmpPrj_FileDescription;
extern CStr TmpPrj_LegalCopyRight;
extern CStr TmpPrj_LegalTrademarks;
extern CStr TmpPrj_ProductName;

extern CStr PrjDir;
extern CStr IconName;
extern CStr NewIconFile;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK FRMPrjPropsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void LoadUserInfos(void);
void RegisterUserInfos(void);
LRESULT CALLBACK FRMPrjPropsFrame1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPrjPropsFrame3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPrjPropsTypeTxtHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SetProjectIcon(void);
void SaveProjectProperties(void);

#endif
