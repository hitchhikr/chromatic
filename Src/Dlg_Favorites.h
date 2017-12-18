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
// Dlg_Favorites.h: declaration of used variables / structures for Dlg_Favorites.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_FAVORITES_H_
#define _DLG_FAVORITES_H_

// -----------------------------------------------------------------------
// Constants
#define FAVADDFILES 1
#define FAVADDALLDIRECTORY 2
#define FAVADDACTIVEFILE 3
#define FAVADDALLACTIVEFILES 4
#define FAVREMSELECTION 5

// -----------------------------------------------------------------------
// Variables
extern HWND FRMFavListView;
extern HWND FRMFavCmdSave;
extern HWND FRMFavCmdCancel;
extern HWND FRMFavCmdOpen;
extern HWND FRMFavToolbar;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK FRMFavoritesProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
long CALLBACK AddAllActiveFilesProc(HWND hWnd);
void SaveFavList(void);
void LoadFavList(void);
long CheckFavFile(CStr FavFileToCheck);
void OpenFavFiles(void);
void RemFavFiles(void);
long CALLBACK EnumFavFiles(char *FileToAdd, long UserValue);

#endif
