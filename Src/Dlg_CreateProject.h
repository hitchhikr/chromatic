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
// Dlg_CreateProject.h: declaration of used variables / structures for Dlg_CreateProject.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_CREATEPROJECT_H_
#define _DLG_CREATEPROJECT_H_

// -----------------------------------------------------------------------
// Variables
extern HWND FRMCreateProjectSysTab;
extern HWND FRMCreateProjectListView;
extern HWND FRMCreateProjectTextBox;
extern HWND FRMCreateProjectLabel;
extern HWND FRMCreateProjectCmdOk;
extern HWND FRMCreateProjectCmdCancel;
extern HWND FRMCreateProjectStartup;
extern HWND FRMCreateProjectRecent;
extern long *PrjDats;
extern CStr FRMCreateProjectRetVal;
extern CStr FRMCreateProjectSelRecent;
extern long FRMCreateProjectRecentTabNbr;
extern CList <char *> FRMCreateProjectTabs;
extern CList <int> FRMCreateProjectTabsNbr;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK FRMCreateProjectProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMCreateProjectSysTabProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FRMCreateProjectChangeCurrentTab(HWND hwndDlg);
void FRMCreateProjectFilCListView(long CurrentTab);
void FRMCreateProjectFillProjectsTabs(long CurrentTab);
long GetTabNumber(CStr TabNameToSearch);
long RetrieveProjectDatas(void);
long FillProjectsRecents(void);
CStr RetrieveSelRecentPrj(void);

#endif
