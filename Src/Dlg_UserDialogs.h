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
// Dlg_UserDialogs.h: declaration of used variables / structures for Dlg_UserDialogs.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_USERDIALOGS_H_
#define _DLG_USERDIALOGS_H_

// -----------------------------------------------------------------------
// Constants
#define DIALOGCURSOR 1
#define DIALOGPICTURE 2
#define DIALOGLABEL 3
#define DIALOGTEXTBOX 4
#define DIALOGFRAME 5
#define DIALOGBUTTON 6
#define DIALOGCHECKBOX 7
#define DIALOGRADIOBUTTON 8
#define DIALOGCOMBOBOX 9
#define DIALOGLISTBOX 10
#define DIALOGHSCROLLBAR 11
#define DIALOGTREEVIEW 12
#define DIALOGLISTVIEW 13
#define DIALOGSYSTAB 14
#define DIALOGPROGRESSBAR 15
#define DIALOGTRACKBAR 16
#define DIALOGSPIN 17
#define DIALOGXCOMBOBOX 18
#define DIALOGTEXTIP 19
#define DIALOGHOTKEY 20
#define DIALOGCALENDAR 21
#define DIALOGDATEPICKER 22
#define DIALOGANIMATION 23
#define DIALOGTOOLBAR 24
#define DIALOGSTATUSBAR 25
#define DIALOGRICHTEXT 26
#define DIALOGPAGER 27
#define DIALOGHEADER 28
#define DIALOGREBAR 29
#define DIALOGTOOLTIP 30

#define DIALOGTOOLS_ALIGNLEFT 1
#define DIALOGTOOLS_ALIGNRIGHT 2
#define DIALOGTOOLS_ALIGNTOP 3
#define DIALOGTOOLS_ALIGNBOTTOM 4
#define DIALOGTOOLS_CENTERHORZ 5
#define DIALOGTOOLS_CENTERVERT 6
#define DIALOGTOOLS_SAMEWIDTH 7
#define DIALOGTOOLS_SAMEHEIGHT 8
#define DIALOGTOOLS_SAMESIZE 9
#define DIALOGTOOLS_SPACEACROSS 10
#define DIALOGTOOLS_SPACEDOWN 11
#define DIALOGTOOLS_ALIGNGRID 12
#define DIALOGTOOLS_LOCKCTRL 13
#define DIALOGTOOLS_GRID 14
#define DIALOGTOOLS_TOOLBAR 15
#define DIALOGTOOLS_PROPERTIES 16
#define DIALOGTOOLS_TEST 17

#define CONTROL_NONE 0
#define CONTROL_PICTURE 1
#define CONTROL_LABEL 2
#define CONTROL_TEXTBOX 3
#define CONTROL_FRAME 4
#define CONTROL_BUTTON 5
#undef CONTROL_CHECKBOX
#define CONTROL_CHECKBOX 6
#undef CONTROL_RADIOBUTTON
#define CONTROL_RADIOBUTTON 7
#undef CONTROL_COMBOBOX
#define CONTROL_COMBOBOX 8
#undef CONTROL_LISTBOX
#define CONTROL_LISTBOX 9
#define CONTROL_HSCROLLBAR 10
#define CONTROL_TREEVIEW 11
#define CONTROL_LISTVIEW 12
#define CONTROL_SYSTAB 13
#define CONTROL_PROGRESSBAR 14
#define CONTROL_TRACKBAR 15
#define CONTROL_UPDOWN 16
#define CONTROL_XCOMBOBOX 17
#define CONTROL_TEXTIP 18
#define CONTROL_HOTKEY 19
#define CONTROL_CALENDAR 20
#define CONTROL_DATEPICKER 21
#define CONTROL_ANIMATION 22
#define CONTROL_TOOLBAR 23
#define CONTROL_STATUSBAR 24
#define CONTROL_RICHTEXT 25
#define CONTROL_PAGER 26
#define CONTROL_HEADER 27
#define CONTROL_REBAR 28
#define CONTROL_TOOLTIP 29
#define CONTROL_DIALOG 30

#define GRID_IDBASE 4000
#define GRID_TOGGLE_ID 0
#define GRID_SETTINGS_ID 1

// -----------------------------------------------------------------------
// Structures
typedef struct _CTRLSELECTION
{
    HWND hControl;
    LPGRIPBOXDAT hGrip;
    RECT Ctrl_Rect;
    int Captured;
} CTRLSELECTION, *LPCTRLSELECTION;

// -----------------------------------------------------------------------
// Variables
extern CStr DialogTitle;
extern HWND FRMDiaghwnd;
extern HWND FRMDiagTBhwnd;
extern HWND FRMDiagToolhwnd;
extern HWND FRMDiagPropshwnd;
extern HWND hDiagToolBar;
extern HWND hDiagTools;
extern HWND hPropCombobox;
extern long DiagEditMode;
extern long DiagControlNumber;
extern HMENU hGridMenu;
extern long DiagX1;
extern long DiagY1;
extern long DiagX2;
extern long DiagY2;
extern long OldDiagX1;
extern long OldDiagY1;
extern long OldDiagX2;
extern long OldDiagY2;
extern long DiagInDrag;
extern RECT DragRect;
extern RECT OldDragRect;
extern RECT ClipRect;
extern long FirstDrawn;
extern HDC DiaghDC;

extern long GridMode;
extern long GridXLarg;
extern long GridYLong;
extern long GridAlign;

extern long ToolbarMode;
extern long PropertiesMode;
extern HWND hPropListbox;

extern long ToolbarXPos;
extern long ToolbarYPos;

extern long PropertiesXPos;
extern long PropertiesYPos;
extern long PropertiesXLarg;
extern long PropertiesYLong;

extern HDC BackGridDC;
extern HGDIOBJ OldGridObject;
extern HGDIOBJ BackGridhBitmap;
extern HBRUSH GridhBrush;
extern RECT GridRect;
extern BITMAPINFO BitmapInfos;
extern void *DIBBits;

extern long DialogBase;
extern CStr DialogName;
extern HBRUSH hSubEditBrush;

extern long CurrentLeft;
extern long CurrentTop;
extern long CurrentWidth;
extern long CurrentHeight;
extern HFONT CurrentFont;

extern long ObjectX;
extern long ObjectY;
extern long ObjectWidth;
extern long ObjectHeight;

extern HWND hGridOk;
extern HWND hGridCancel;
extern HWND hGridWidth;
extern HWND hGridHeight;
extern HWND hGridAlign;
extern HFONT IPSerif;

extern CList <int *> ControlsList;

// -----------------------------------------------------------------------
// Functions declarations
void CreateUserDialog(void);
int CALLBACK FRMUserDiagProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CALLBACK FrmUserDiagToolBarInitProc(HWND hWnd);
LRESULT CALLBACK FrmUserDiagToolBarProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CALLBACK FrmUserDiagToolsInitProc(HWND hWnd);
LRESULT CALLBACK FrmUserDiagToolsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void SetDialogEditModeOn(void);
void SetDialogEditModeOff(void);
void FillOldRectDatas(void);
void FillNewRectDatas(void);
void DrawNewSelectRect(HWND hWnd);
void DrawOldSelectRect(HWND hWnd);
void CreateGrid(HWND hWnd);
void DiscardGrid(void);
void ToggleGridMode(void);
void SetGridOn(void);
void SetGridOff(void);
int *CreateNewControl(HWND hWnd);
void PropSelectFont(void);
void CreateSubEditBrush(long Color);
CStr GetMenusIDs(void);
int CALLBACK FRMGridDiagProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

// -----------------------------------------------------------------------
// sub includes
#include "Dlg_UserDialogsProps.h"
#include "Dlg_UserDialogsObjects.h"

#endif
