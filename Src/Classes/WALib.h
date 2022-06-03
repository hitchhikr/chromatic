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
// ChromaticLib.h: declaration of used variables / structures for ChromaticLib.cpp
// -----------------------------------------------------------------------

#ifndef _CHROMATICLIB_H_
#define _CHROMATICLIB_H_

// -----------------------------------------------------------------------
// Includes
#define _WIN32_WINNT 0x0501
#define _WIN32_IE 0x0501

#define HAVE_SSE_REGS
#include <windows.h>
#if (_MSC_VER < 1300)
#define DECLSPEC_DEPRECATED
#endif
#include <CommCtrl.h>
#include <CommDlg.h>
#include <richedit.h>
#include <shlobj.h>
#include <ShellAPI.h>
#include <Winsock.h>
#include <Psapi.h>           // Coming from the platform SDK
#include <Dbghelp.h>         // Coming from the platform SDK
#include <time.h>
#include <string.h>
#include <uxtheme.h>
#include <htmlhelp.h>

// Classes
#include "CList.h"
#include "CStr.h"

// External DLLs
#include "ChromaticSupDLL.h"

#include "../Miscfunctions.h"

// -----------------------------------------------------------------------
// Constants
#define MB_INFORMATION MB_ICONINFORMATION + MB_OK
#define MB_QUESTION MB_ICONQUESTION + MB_YESNO
#define MB_ERRORQUESTION MB_ICONERROR + MB_YESNO
#define MB_ERROR MB_ICONERROR + MB_OK
#define MB_QUESTIONCANCEL MB_ICONQUESTION + MB_YESNOCANCEL

#define SPLITTER_HORZ 1
#define SPLITTER_VERT 2
#define SPLITTER_RESIZING 1
#define SPLITTER_NORESIZING 0

#define MSG_SPLITTER_RESIZED WM_USER + 100
#define MSG_COLORBOX_CLICKED WM_USER + 101
#define MSG_SPLASH_PAINT WM_USER + 102

#define STATUSBAR_PARTSUNKEN 2
#define STATUSBAR_PARTNORMAL 4
#define STATUSBAR_PARTRAISED 8
#define STATUSBAR_PARTOWNER 16

#define INIT_OK 0
#define INIT_NOEXCONTROLS 1
#define INIT_RICHEDCONTROL10 2
#define INIT_NORICHEDCONTROL 4

#define DUMPBOX_LINELEFT WM_USER + 201
#define DUMPBOX_LINERIGHT WM_USER + 202
#define DUMPBOX_PAGELEFT WM_USER + 203
#define DUMPBOX_PAGERIGHT WM_USER + 204
#define DUMPBOX_LINEUP WM_USER + 205
#define DUMPBOX_LINEDOWN WM_USER + 206
#define DUMPBOX_PAGEUP WM_USER + 207
#define DUMPBOX_PAGEDOWN WM_USER + 208
#define DUMPBOX_POSITIONLEFTRIGHT WM_USER + 209
#define DUMPBOX_POSITIONUPDOWN WM_USER + 210
#define DUMPBOX_TRACKLEFTRIGHT WM_USER + 211
#define DUMPBOX_TRACKUPDOWN WM_USER + 212
#define DUMPBOX_TOP WM_USER + 213
#define DUMPBOX_BOTTOM WM_USER + 214
#define DUMPBOX_LEFT WM_USER + 215
#define DUMPBOX_RIGHT WM_USER + 216

#define MODALDLG_STOCK_EMPTY 0
#define MODALDLG_STOCK_STDCLOSE 1
#define MODALDLG_STOCK_STDOKCANCEL 2
#define MODALDLG_STOCK_WIZARD 3

#define WIZARD_STOCK_STDX 560
#define WIZARD_STOCK_STDY 357
#define WIZARD_STOCK_PICTUREX 200

#define BUTTON_STOCK_CLOSE_ID 1
#define BUTTON_STOCK_OK_ID 1
#define BUTTON_STOCK_FINISH_ID 1
#define BUTTON_STOCK_CANCEL_ID 2
#define BUTTON_STOCK_PREVIOUS_ID 3
#define BUTTON_STOCK_NEXT_ID 4

#define CHILD_MSG_CREATED WM_USER + 217
#define CHILD_MSG_DESTROYED WM_USER + 218
#define CHILD_MSG_SETTITLE WM_USER + 219

#define GRIPBOX_MSG_ASKSIZE WM_USER + 220
#define GRIPBOX_MSG_ASKSTEPS WM_USER + 221
#define GRIPBOX_MSG_RESIZED WM_USER + 222
#define GRIPBOX_TOPLEFT 0
#define GRIPBOX_TOP 1
#define GRIPBOX_TOPRIGHT 2
#define GRIPBOX_BOTTOMLEFT 3
#define GRIPBOX_BOTTOM 4
#define GRIPBOX_BOTTOMRIGHT 5
#define GRIPBOX_LEFT 6
#define GRIPBOX_RIGHT 7

#define GRIPBOX_STATE_RESET 0
#define GRIPBOX_STATE_RESIZING 1

#define CASE_SENSITIVE 0
#define CASE_UNSENSITIVE 1

#define MULTIFILESENUM_FORWARD 0
#define MULTIFILESENUM_BACKWARD 1

#define DATE_GET_SECONDS 1
#define DATE_NO_SECONDS 0

// -----------------------------------------------------------------------
// Structures
typedef struct _MDIDAT
{
    HWND hWnd;
    HWND hClient;
    HWND hDock;
} MDIDAT, *LPMDIDAT;

typedef struct _SPLITTERDAT
{
    HWND hClient;
    HWND hParent;
    long Orientation;
    long Sizing;
    long MinLeftTop;
    long MaxRightBottom;
    long SplitBarSize;
    long StartupPos;
    long OldXPos;
    long OldYPos;
    long OldWndProc;
    long StartDragX;
    long StartDragY;
    long DisplacementX;
    long DisplacementY;
} SPLITTERDAT, *LPSPLITTERDAT;

typedef struct _GRIPBOXDAT
{
    HWND TopLeftBox;
    HWND TopBox;
    HWND TopRightBox;
    HWND BottomLeftBox;
    HWND BottomBox;
    HWND BottomRightBox;
    HWND LeftBox;
    HWND RightBox;
} GRIPBOXDAT, *LPGRIPBOXDAT;

typedef struct _GRIPBOXSQRBOXDAT
{
    long Type;
    long State;
    long StepX;
    long StepY;
    RECT ResizeRect;
    RECT OldRect;
    COLORREF Color;
    LPGRIPBOXDAT Container;
} GRIPBOXSQRBOXDAT, *LPGRIPBOXSQRBOXDAT;

typedef struct _BACKDC
{
    HWND hWnd;
    COLORREF Color;
    HDC hDC;
    HBITMAP Bitmap;
    HGDIOBJ OldObject;
} BACKDC, *LPBACKDC;

typedef struct _DLGTEMPLATE2
{
    DLGTEMPLATE Diag;
    long Padding[6 + 1];
} DLGTEMPLATE2, *LPDLGTEMPLATE2;

typedef struct _NM_TREEVIEWEXPAND
{
    NMHDR hdr;
    long action;
    long itemOld;
    long itemNew;
    long itemNew2;
    long itemNew3;
    long itemNew4;
    long itemNew5;
    long itemNew6;
    long itemNew7;
    long itemNew8;
    long itemNew9;
} NM_TREEVIEWEXPAND, *LPNM_TREEVIEWEXPAND;

typedef struct _FILTERFILE
{
    long FileLength;
    HGLOBAL FileMem;
} FILTERFILE, *LPFILTERFILE;

// -----------------------------------------------------------------------
// Variables
extern HFONT WASerifFont;
extern HFONT WASerifFont10;
extern HFONT WACourrierNewFont9;
extern HFONT WAMarlettFont9;
extern HFONT WAArialFont8;
extern HBRUSH WABgBrush;
extern HWND LastDialog;
extern long WABgColor;
extern long WATxtColor;
extern long ToolTipFont;
extern CHOOSECOLOR MyColor;
extern COLORREF MyCustomColors[17];
extern LOGFONT MyLogFont;
extern CHOOSEFONT MyFont;
extern OPENFILENAME MyOpenFName;
extern OPENFILENAME MySaveFName;
extern HGLOBAL hMemFont;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK WidgetInit(HINSTANCE hInst, HICON AppIcon, long vSplitCursor, long hSplitCursor);
void CALLBACK WidgetUnInit(HINSTANCE hInst);
HWND CALLBACK CreateSplashDialog(long DLeft, long DTop, long DWidth, long DHeight, CStr DTitle, HWND hParent, HMENU hMenu, HICON hIcon,
                                   void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long WExStyle, long WStyle, long ShowType);
long CALLBACK CreateStockModalDialog(long TemplateNumber, long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, CStr DTitle, DLGPROC WindowProc, long Centered, long ExtraDatas);
long CALLBACK CreateModalDialog(long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, DLGPROC WindowProc, long ExtraStyle, long Centered);
HWND CALLBACK CreateNonModalDialog(long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, HMENU hMenu, HICON hIcon, CStr DTitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long WExStyle, long WStyle, long ShowType);
LRESULT CALLBACK DialogSetIcon(HWND hWnd, HICON hIcon);
long CALLBACK DialogGetXUnit(void);
long CALLBACK DialogGetYUnit(void);
long CALLBACK DialogXPixelToUnit(long XCoord, long XUnit);
long CALLBACK DialogYPixelToUnit(long YCoord, long YUnit);
long CALLBACK DialogXUnitToPixel(long XCoord, long XUnit);
long CALLBACK DialogYUnitToPixel(long YCoord, long YUnit);
HWND CALLBACK CreateContainer(long SPLeft, long SPTop, long SPWidth, long SPHeight, long CtrlID, HWND hParent, CStr Name, WNDPROC WindowProc);
HWND CALLBACK CreateSplitter(long SPLeft, long SPTop, long SPDim, long CtrlID, LPSPLITTERDAT SplitterStruct, WNDPROC WindowProc);
HWND CALLBACK CreateSplitBar(long SPLeft, long SPTop, long SPWidth, long SPHeight, HWND hParent, long CtrlID, LPSPLITTERDAT SplitStruct);
HWND CALLBACK CreateMDIDialog(long MDILeft, long MDITop, long MDIWidth, long MDIHeight, HWND hParent, HMENU hMenu, HICON hIcon, CStr MDITitle, 
                                void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long ExtraStyle, long ShowType);
HWND CALLBACK CreateMDIChildDialog(HWND hParent, CStr CTitle, HICON hIcon, 
                                     void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long ExtraStyle, long ZeroPos, CStr CustomClass);
HWND CALLBACK CreateToolBar(long TBLeft, long TBTop, long TBWidth, long TBHeight, HWND hParent, HIMAGELIST hImageList, long CtrlID, long StdBitmapsType, WNDPROC WindowProc, long ExtraStyle, long ExtendedStyle);
long CALLBACK ToolBarAddButton(HWND hToolbar, CStr ButtonText, long TBButtonID, long ImgIndex, long ButtonStyle, long ButtonState, long NoImage);
long CALLBACK ToolBarGetButton(HWND hToolbar, long TBButtonID, LPTBBUTTON ButtonStruct);
long CALLBACK ToolBarGetButtonXSize(HWND hToolbar);
long CALLBACK ToolBarGetButtonYSize(HWND hToolbar);
long CALLBACK ToolBarGetButtonsCount(HWND hToolbar);
long CALLBACK ToolBarGetRowsCount(HWND hToolbar);
HIMAGELIST CALLBACK ToolBarGetImagelist(HWND hToolbar);
long CALLBACK ToolBarAddSeparator(HWND hToolbar, long TBButtonID);
long CALLBACK ToolBarGetNotifiedToolTip(long lParam);
long CALLBACK ToolBarGetNotifiedDropDownItem(long lParam);
void CALLBACK ToolBarDisplayToolTip(CStr TextToShow, long lParam);
long CALLBACK ToolBarResize(HWND hToolbar);
long CALLBACK ToolBarSetButtonPressed(HWND hToolbar, long TBButtonID, long Pressed);
long CALLBACK ToolBarSetButtonChecked(HWND hToolbar, long TBButtonID, long Checked);
long CALLBACK ToolBarSetButtonEnabled(HWND hToolbar, long TBButtonID, long EnableState);
long CALLBACK ToolBarIsButtonPressed(HWND hToolbar, long TBButtonID);
long CALLBACK ToolBarIsButtonChecked(HWND hToolbar, long TBButtonID);
long CALLBACK ToolBarDisplayPopupMenu(HWND hToolbar, long TBButtonID, HMENU hMenu, HWND hParent);
long CALLBACK ToolBarGetYSize(HWND hToolbar);
long CALLBACK ToolBarGetXSize(HWND hToolbar);
long CALLBACK ToolBarGetButtonXPosition(HWND hToolbar, long TBButtonID);
long CALLBACK ToolBarGetXYSize(HWND hToolbar, LPSIZE TBSize);
long CALLBACK ToolBarGetRealPos(HWND hToolbar, LPPOINT TBPosition);
long CALLBACK ToolBarGetButtonXYPos(HWND hToolbar, long TBButtonID, LPPOINT TBPosition);
long CALLBACK ToolBarGetButtonIndexXSize(HWND hToolbar, long TBButtonID);
long CALLBACK ToolBarGetXPadding(HWND hToolbar);
long CALLBACK ToolBarGetYPadding(HWND hToolbar);
long CALLBACK ToolBarGetNotifiedHotItem(long lParam);
long CALLBACK ToolBarRemoveButton(HWND hToolbar,long ButtonID);
long CALLBACK ToolBarSetButtonBitmap(HWND hToolbar, long TBButtonID, long BtnIndex);
HIMAGELIST CALLBACK CreateImageList(long IconWidth, long IconHeight, long MaxIcon);
void CALLBACK ImageListDestroy(HIMAGELIST hImageList);
long CALLBACK ImageListAddIcon(HIMAGELIST hImageList, long IconNumber);
long CALLBACK ImageListGetIconsCount(HIMAGELIST hImageList);
HICON CALLBACK ImageListGetIcon(HIMAGELIST hImageList, long IconIdx);
HWND CALLBACK CreateSysTab(long FLeft, long FTop, long FWidth, long FHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, HIMAGELIST hImageList, long ExtraStyle);
long CALLBACK SysTabAddItem(HWND htab, CStr TabText, long TabIndex, long ImgIndex);
long CALLBACK SysTabGetCurrentItem(HWND htab);
HIMAGELIST CALLBACK SysTabGetImagelist(HWND htab);
long CALLBACK SysTabSetCurrentItem(HWND htab, long TabItem);
long CALLBACK SysTabSetFocusItem(HWND htab, long TabItem);
long CALLBACK SysTabHighLightItem(HWND htab, long TabItem, long HighState);
long CALLBACK SysTabItemsCount(HWND htab);
long CALLBACK SysTabGetRowsCount(HWND htab);
long CALLBACK SysTabSetSeparators(HWND htab, long Enable);
long CALLBACK SysTabSetItemsSize(HWND htab,long Width,long Height);
long CALLBACK SysTabRemoveItem(HWND htab,long ItemIndex);
long CALLBACK SysTabSetMinWidth(HWND htab,long Width);
long CALLBACK SysTabSetPadding(HWND htab,long Width,long Height);
HWND CALLBACK CreateTrackBar(long PBLeft, long PBTop, long PBWidth, long PBHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long TBMin, long TBMax, long ExtraStyle);
long CALLBACK TrackBarSetRange(HWND hTrackBar, long TBMin, long TBMax, long Redraw);
long CALLBACK TrackBarSetPos(HWND hTrackBar, long TBNewPos);
long CALLBACK TrackBarGetPos(HWND hTrackBar);
long CALLBACK TrackBarSetTicks(HWND hTrackBar, long TicksFreq);
HWND CALLBACK TrackBarGetToolTips(HWND hTrackBar);
HWND CALLBACK CreateToolTip(HWND hParent, long CtrlID, long ExtraStyle);
long CALLBACK ToolTipAddTool(HWND hToolTip, HWND hWndOwner, long ID, long ExtraStyle);
HWND CALLBACK CreateUpDown(long PBLeft, long PBTop, long PBWidth, long PBHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK CreateProgressBar(long PBLeft, long PBTop, long PBWidth, long PBHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long PBMin, long PBMax, long ExtraStyle);
long CALLBACK ProgressBarSetRange(HWND hProgressBar, long PBMin, long PBMax);
long CALLBACK ProgressBarSetRelPos(HWND hProgressBar, long PBNewPos);
HWND CALLBACK CreateStatusBar(CStr SBText, long RaiseType, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK StatusBarSetParts(HWND hStatusBar, long SBNbParts, long *PartsArray);
long CALLBACK StatusBarSetText(HWND hStatusBar, long SBPart, long RaiseType, CStr SBText, long OwnerDatas);
long CALLBACK StatusBarGetYSize(HWND hStatusBar);
HWND CALLBACK CreateColorBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, long CtrlID, COLORREF Color, long ExtraStyle);
COLORREF CALLBACK ColorBoxGetColor(HWND hWnd);
long CALLBACK ColorBoxSetColor(HWND hWnd, COLORREF Color);
HWND CALLBACK CreateDumpBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, long CtrlID, HFONT hFont, long MaxX, long MaxY, long PageValueX, long PageValueY, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle);
void CALLBACK DumpBoxResize(HWND hWnd, long left, long top, long Width, long Height);
long CALLBACK DumpBoxScrollDown(HWND hWnd, long Lines, long Factor, long Integral);
long CALLBACK DumpBoxScrollUp(HWND hWnd, long Lines, long Factor, long Integral);
long CALLBACK DumpBoxGetVisibleLines(HWND hWnd, long Factor, long Intregral);
long CALLBACK DumpBoxGetVisibleColumns(HWND hWnd, long Factor, long Integral);
LPGRIPBOXDAT CALLBACK CreateGripBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, long CtrlID, COLORREF Color, long ExtraStyle);
COLORREF CALLBACK GripBoxGetColor(LPGRIPBOXDAT GripDats);
void CALLBACK GripBoxSetColor(LPGRIPBOXDAT GripDats, COLORREF Color);
void CALLBACK GripBoxClose(LPGRIPBOXDAT GripDats);
void CALLBACK GripBoxVisible(LPGRIPBOXDAT GripDats, long CState);
void CALLBACK GripBoxResize(LPGRIPBOXDAT GripDats, LPRECT NewSizeRect);
HWND CALLBACK CreateLabel(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, CStr LText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, HFONT hFont);
HGDIOBJ CALLBACK LabelGetImage(HWND hLabel, long ImageType);
HWND CALLBACK CreatePictureBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, HGDIOBJ hPicture, long PictureType, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK PictureBoxChangeIcon(HWND hPictureBox, HICON IconHandle);
long CALLBACK PictureBoxChangeImage(HWND hPictureBox, HANDLE ImageHandle);
HWND CALLBACK CreateFrame(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK FrameRefreshToolbar(HWND hToolbar);
HWND CALLBACK CreateButton(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, long ImgType, HICON hImage, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle);
long CALLBACK ButtonSetIcon(HWND hbutton, long ImType, HICON hImage);
HANDLE CALLBACK ButtonGetImage(HWND hbutton, long ImageType);
long CALLBACK ButtonGetState(HWND hbutton);
HWND CALLBACK CreateIPBox(long IPBLeft, long IPBTop, long IPBWidth, long IPBHeight, HWND hParent, long IPAddress, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long SetBorder, HFONT hFont);
long CALLBACK IPBoxClearIP(HWND hIPBox);
long CALLBACK IPBoxGetIP(HWND hIPBox);
long CALLBACK IPBoxSetIP(HWND hIPBox, long IPAddress);
long CALLBACK IPBoxSetFocus(HWND hIPBox, long FieldNumber);
long CALLBACK IPBoxSetRange(HWND hIPBox, long FieldNumber, long LowValue, long HighValue);
long CALLBACK IPBoxIsBlank(HWND hIPBox);
HWND CALLBACK CreateRichTextBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent, CStr EDText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long SetBorder);
HWND CALLBACK CreateTextBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent, CStr EDText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle);
long CALLBACK TextBoxGetLen(HWND hTextBox);
long CALLBACK TextBoxSetMaxLen(HWND hTextBox, long TBMaxLen);
long CALLBACK TextBoxGetMaxLen(HWND hTextBox);
long CALLBACK TextBoxGetTextHandle(HWND hTextBox);
void CALLBACK TextBoxAddText(HWND hTextBox, CStr TbText);
long CALLBACK TextBoxSelText(HWND hTextBox, long NbrToSel);
long CALLBACK TextBoxRemoveSel(HWND hTextBox);
long CALLBACK TextBoxSetCaretPos(HWND hTextBox, long PosToSel);
long CALLBACK TextBoxLinesCount(HWND hTextBox);
long CALLBACK TextBoxGetMarginWidth(HWND hTextBox);
long CALLBACK TextBoxGetFirstVisibleLine(HWND hTextBox);
long CALLBACK TextBoxIsModified(HWND hTextBox);
long CALLBACK TextBoxGetCurrentOptions(HWND hTextBox);
CStr CALLBACK TextBoxGetPasswordChar(HWND hTextBox);
long CALLBACK TextBoxSetPasswordChar(HWND hTextBox, CStr PassChar);
long CALLBACK TextBoxGetSelection(HWND hTextBox);
long CALLBACK TextBoxGetScrollPos(HWND hTextBox);
long CALLBACK TextBoxGetBreakProcAddress(HWND hTextBox);
long CALLBACK TextBoxCanPaste(HWND hTextBox);
long CALLBACK TextBoxCanUndo(HWND hTextBox);
HWND CALLBACK CreateHexBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent, CStr EDText, long CtrlID, long MaxDigits, long ExtraStyle, long ExtraExStyle);
HWND CALLBACK CreateCheckBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long UndefState);
long CALLBACK CheckBoxGetState(HWND hCB);
long CALLBACK CheckBoxSetState(HWND hCB, long CheckedState);
HWND CALLBACK CreateRebar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, WNDPROC WindowProc, long CtrlID, long ExtraStyle);
long CALLBACK RebarAddBand(HWND hRebar, HWND hChild, CStr RBText, long RBYSize, long ExtraStyle, long MinSize);
long CALLBACK RebarRemoveBand(HWND hRebar, long BandNumber);
long CALLBACK RebarGetXSize(HWND hRebar);
long CALLBACK RebarGetYSize(HWND hRebar);
long CALLBACK RebarResize(HWND hRebar);
long CALLBACK RebarSetBackColor(HWND hRebar, COLORREF RBColor);
long CALLBACK RebarSetTextColor(HWND hRebar, COLORREF RBColor);
long CALLBACK RebarBandVisible(HWND hRebar, long RBBand, long RBVisible);
long CALLBACK RebarGetBandsCount(HWND hRebar);
long CALLBACK RebarGetHeight(HWND hRebar);
COLORREF CALLBACK RebarGetBackGroundColor(HWND hRebar);
long CALLBACK RebarGetRowsCount(HWND hRebar);
long CALLBACK RebarGetToolTips(HWND hRebar);
COLORREF CALLBACK RebarGetTextColor(HWND hRebar);
long CALLBACK RebarGetPalette(HWND hRebar);
HIMAGELIST CALLBACK RebarGetImageList(HWND hRebar);
HWND CALLBACK CreatePager(long PLeft, long PTop, long PWidth, long PHeight, HWND hParent, HWND hChild, long CtrlID, long ExtraStyle);
void CALLBACK PagerDisplaySetSize(LPNMPGCALCSIZE lParam, LPSIZE PGNewSize);
long CALLBACK PagerMaximizeX(HWND hPager, long ChildYSize);
long CALLBACK PagerMaximizeY(HWND hPager, long ChildXSize);
HWND CALLBACK PagerGetNotifiedChild(long lParam);
long CALLBACK PagerSetBackColor(HWND hPager, COLORREF Color);
COLORREF CALLBACK PagerGetBackColor(HWND hPager);
long CALLBACK PagerGetBorderSize(HWND hPager);
long CALLBACK PagerGetButtonSize(HWND hPager);
long CALLBACK PagerGetScrollPos(HWND hPager);
long CALLBACK PagerGetOrientation(HWND hPager);
long CALLBACK PagerSetOrientation(HWND hPager, long Orientation);
long CALLBACK PagerChangeOrientation(HWND hPager, LPNMPGCALCSIZE CalcSizeNotify, long NewOrientation, LPSIZE NewSize);
HWND CALLBACK CreateClient(HWND hParent, HMENU hMenuWindowList, HICON hIcon);
long CALLBACK ClientResize(HWND hClient, long AddX, long AddY, long SubX, long SubY);
HWND CALLBACK ClientGetActiveChild(HWND hClient);
HWND CALLBACK ClientSetNextChild(HWND hClient);
HWND CALLBACK ClientSetPreviousChild(HWND hClient);
long CALLBACK ClientTileHorizontal(HWND hClient);
long CALLBACK ClientTileVertical(HWND hClient);
long CALLBACK ClientTileCascade(HWND hClient);
long CALLBACK ClientTileArrangeIconic(HWND hClient);
HWND CALLBACK CreateRadioButton(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK RadioButtonGetState(HWND hRB);
long CALLBACK RadioButtonSetState(HWND hRB, long CheckedState);
HWND CALLBACK CreateListView(long LVLeft, long LVTop, long LVWidth, long LVHeight, HWND hParent, long CtrlID, HIMAGELIST hImageList, WNDPROC WindowProc, long ExtraStyle, long WStyle, long ExtraExStyle);
long CALLBACK ListViewAddCol(HWND hListview, CStr LVColText, long LVWidth, long ColPosition);
long CALLBACK ListViewSetColWidth(HWND hListview, long LVColNumber, long LVWidth);
long CALLBACK ListViewGetColWidth(HWND hListview, long LVColNumber);
long CALLBACK ListViewSetScroll(HWND hLV, long DeltaX, long DeltaY);
long CALLBACK ListViewClear(HWND hLV);
long CALLBACK ListViewDetachImageList(HWND hLV);
long CALLBACK ListViewAddItem(HWND hListview, CStr LVItemText, long LVItemPos, long LVImage);
long CALLBACK ListViewDeleteItem(HWND hListview, long LVItemPos);
long CALLBACK ListViewSetSubItem(HWND hListview, CStr LVItemText, long LVItemPos, long LVSubItemPos);
long CALLBACK ListViewSetSubItemImage(HWND hListview, long LVItemImage, long LVItemPos, long LVSubItemPos);
long CALLBACK ListViewGetSubItemImage(HWND hListview, long LVItemPos, long LVSubItemPos);
long CALLBACK ListViewEditValidated(long lParam);
CStr CALLBACK ListViewGetEditResult(long lParam);
long CALLBACK ListViewItemCount(HWND hListview);
long CALLBACK ListViewSelItemCount(HWND hListview);
long CALLBACK ListViewMultiSelFirstItem(HWND hListview);
long CALLBACK ListViewGetItemSelState(HWND hListview, long LVItemIndex);
HWND CALLBACK ListViewSetEditModeOn(HWND hListview, long LVItemIndex, long ExtraEditStyle);
long CALLBACK ListViewSetItemSel(HWND hListview, long LVItemIndex);
long CALLBACK ListViewSetItemHighlight(HWND hListview, long LVItemIndex, long Enable);
HWND CALLBACK ListViewGetEditControl(HWND hListview);
long CALLBACK ListViewSetItemFocus(HWND hListview, long LVItemIndex);
long CALLBACK ListViewGetItemUnderCursor(HWND hListview);
long CALLBACK ListViewGetFirstFocusItem(HWND hListview);
long CALLBACK ListViewGetSelItem(HWND hListview, long LVIndex);
CStr CALLBACK ListViewGetSelItemText(HWND hListview, long LVSubItemNumber);
long CALLBACK ListViewSetSelItemText(HWND hListview, CStr LVItemText, long LVSubItemNumber);
long CALLBACK ListViewCheckBoxItemDoubleClick(HWND hListview);
CStr CALLBACK ListViewGetItemText(HWND hListview, long LVItemIndex, long LVSubItemNumber);
HICON CALLBACK ListViewGetItemIcon(HWND hListview, long LVItemIndex, long LVSubItemNumber);
long CALLBACK ListViewGetNotifiedColumnIndex(long lParam);
long CALLBACK ListViewSort(HWND hListview, long LVSubItemNumber, PFNLVCOMPARE SortProc);
long CALLBACK ListViewReOrder(HWND hListview);
CStr CALLBACK ListViewGetHeaderLabel(HWND hListview, long LVColNumber);
long CALLBACK ListViewGetHeaderPosition(HWND hListview, long LVColNumber);
long CALLBACK ListViewSetHeaderPosition(HWND hListview, long LVColNumber, long NewPosition);
long CALLBACK ListViewSetBackColor(HWND hListview, COLORREF LVColor);
COLORREF CALLBACK ListViewGetBackColor(HWND hListview);
COLORREF CALLBACK ListViewGetTextBackColor(HWND hListview);
COLORREF CALLBACK ListViewGetTextColor(HWND hListview);
long CALLBACK ListViewGetToolTips(HWND hListview);
long CALLBACK ListViewGetTopIndex(HWND hListview);
long CALLBACK ListViewGetCountPerPage(HWND hListview);
long CALLBACK ListViewGetExStyle(HWND hListview);
HWND CALLBACK ListViewGetSysHeader(HWND hListview);
long CALLBACK ListViewGetHotCursor(HWND hListview);
HIMAGELIST CALLBACK ListViewGetImageList(HWND hListview, long ListType);
long CALLBACK ListViewSetItemCheckbox(HWND hListview, long LVItemIndex, long LVCheckState);
long CALLBACK ListViewGetItemCheckbox(HWND hListview, long LVItemIndex);
long CALLBACK ListViewIsCheckboxNotify(HWND hListview, long LVItemIndex);
long CALLBACK ListViewFindItem(HWND hListview, CStr LVItemText);
long CALLBACK ListViewFindSubItem(HWND hListview, CStr TextToFind, long SubItemNbr, long SearchCase);
long CALLBACK ListViewBeginDrag(long lParam, HWND hParent, LPPOINT DragRect);
long CALLBACK ListViewMoveDrag(LPPOINT DragRect);
void CALLBACK ListViewEndDrag(HWND hParent);
long CALLBACK ListViewPasteAutoEdit(HWND hListview, long wParam, long ExtraEditStyle);
HWND CALLBACK CreateTreeView(long TVLeft, long TVTop, long TVWidth, long TVHeight, HWND hParent, long CtrlID, HIMAGELIST hImageList, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle);
HTREEITEM CALLBACK TreeViewAddItem(HWND hTreeView, CStr TVItemText, HTREEITEM TVParent, HTREEITEM TVChildPos, long TVImage, long TVSelImage, long ExtraStyle, long RefreshParentAfterAdd);
long CALLBACK TreeViewSetToolTips(HWND hTreeView, long hToolTips);
long CALLBACK TreeViewGetToolTips(HWND hTreeView);
long CALLBACK TreeViewSetIndent(HWND hTreeView, long TVIndent);
long CALLBACK TreeViewRemoveItem(HWND hTreeView, HTREEITEM hItem);
long CALLBACK TreeViewSetBackColor(HWND hTreeView, COLORREF TVColor);
long CALLBACK TreeViewGetExpandingState(long lParam);
HTREEITEM CALLBACK TreeViewGetExpandingItem(long lParam);
HTREEITEM CALLBACK TreeViewGetDeletedItem(long lParam);
long CALLBACK TreeViewSetItemIcon(HWND hTreeView, HTREEITEM hItem, long IconNumber);
long CALLBACK TreeViewSetItemExpandedState(HWND hTreeView, HTREEITEM hItem, long ExpandState);
long CALLBACK TreeViewGetItemExpandedState(HWND hTreeView, HTREEITEM hItem);
long CALLBACK TreeViewSetSingleExpandState(HWND hTreeView, long ExpandState);
HTREEITEM CALLBACK TreeViewGetSelectedItem(HWND hTreeView);
HTREEITEM CALLBACK TreeViewGetItemFromPos(HWND hTreeView, long XPos, long YPos);
long CALLBACK TreeViewSetSelectedItem(HWND hTreeView, HTREEITEM hItem);
CStr CALLBACK TreeViewGetItemText(HWND hTreeView, HTREEITEM hItem);
long CALLBACK TreeViewSetItemText(HWND hTreeView, HTREEITEM hItem, CStr ItemText);
HTREEITEM CALLBACK TreeViewGetItemParent(HWND hTreeView, HTREEITEM hItem);
HTREEITEM CALLBACK TreeViewGetRoot(HWND hTreeView);
long CALLBACK TreeViewGetItemCount(HWND hTreeView);
long CALLBACK TreeViewGetVisibleCount(HWND hTreeView);
HTREEITEM CALLBACK TreeViewGetFirstItemChild(HWND hTreeView, HTREEITEM hItem);
HTREEITEM CALLBACK TreeViewGetNextItem(HWND hTreeView, HTREEITEM hItem);
HTREEITEM CALLBACK TreeViewGetPreviousItem(HWND hTreeView, HTREEITEM hItem);
long CALLBACK TreeViewSearchItemText(HWND hTreeView, HTREEITEM hNode, CStr ItemText);
long CALLBACK TreeViewSearchChildPartialText(HWND hTreeView, HTREEITEM hNode, CStr ItemText);
long CALLBACK TreeViewGetChildItemsCount(HWND hTreeView, HTREEITEM hNode);
long CALLBACK TreeViewGetSelChildItemPos(HWND hTreeView, HTREEITEM hNode, HTREEITEM hParent);
long CALLBACK TreeViewGetItemLevel(HWND hTreeView, HTREEITEM hNode);
COLORREF CALLBACK TreeViewGetBackColor(HWND hTreeView);
long CALLBACK TreeViewGetItemTextRect(HWND hTreeView, HTREEITEM hItem, LPRECT ItemRect);
long CALLBACK TreeViewGetItemRect(HWND hTreeView, HTREEITEM hItem, LPRECT ItemRect);
HWND CALLBACK TreeViewGetEditControl(HWND hTreeView);
HIMAGELIST CALLBACK TreeViewGetImageList(HWND hTreeView, long ListType);
long CALLBACK TreeViewGetIndent(HWND hTreeView);
COLORREF CALLBACK TreeViewGetInsertMarkColor(HWND hTreeView);
COLORREF CALLBACK TreeViewGetLineColor(HWND hTreeView);
COLORREF CALLBACK TreeViewGetTextColor(HWND hTreeView);
long CALLBACK TreeViewGetItemHeight(HWND hTreeView);
long CALLBACK TreeViewGetScrollTime(HWND hTreeView);
HWND CALLBACK CreateFictive(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, HICON hImage);
HWND CALLBACK CreateToolTip(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK CreateAnimation(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK CreateHotkey(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long InvalidCombination, long ValidCombination, long VirtualKey, long Modifiers, long ExtraStyle);
long CALLBACK HotkeySetRules(HWND hHotkey, long InvalidCombination, long ValidCombination);
long CALLBACK HotkeySetKey(HWND hHotkey, long VirtualKey, long Modifiers);
HWND CALLBACK CreateDateTimePicker(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK CreateMonthCalendar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
HWND CALLBACK CreateHeader(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK WAHeaderAddItem(HWND hHeader, CStr HeaderText, long Headerwidth, long HeaderPosition, long ExtraStyle);
HWND CALLBACK CreateExComboBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, HIMAGELIST hImageList, long ExtraStyle);
long CALLBACK WAExCBSetImageList(HWND hExCB, HIMAGELIST hImageList);
HWND CALLBACK CreateComboBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK ComboBoxAddItem(HWND hCB, CStr CBItemText, long CBItemIndex);
long CALLBACK ComboBoxDeleteItem(HWND hCB, long CBItemIndex);
long CALLBACK ComboBoxReset(HWND hCB);
long CALLBACK ComboBoxSetMaxLen(HWND hCB, long TBMaxLen);
long CALLBACK ComboBoxCount(HWND hCB);
long CALLBACK ComboBoxGetIndex(HWND hCB);
long CALLBACK ComboBoxSetIndex(HWND hCB, long CBIndex);
CStr CALLBACK ComboBoxGetItem(HWND hCB, long CBIndex);
CStr CALLBACK ComboBoxGetCurrentItem(HWND hCB);
long CALLBACK ComboBoxItemExist(HWND hCB, CStr CBItem);
long CALLBACK ComboBoxGetItemData(HWND hLB, long CBIndex);
void ComboBoxSaveInIniFile(HWND hCombo, CStr EntryToAdd, CStr IniKey, CStr IniFile);
void ComboBoxFillFromIniFile(HWND hCombo, CStr IniKey, CStr IniFile);
HWND CALLBACK CreateListBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long DragOn, long ExtraStyle, long ExtraExStyle);
long CALLBACK ListBoxAddItem(HWND hLB, CStr LBItemText, long LBItemIndex);
long CALLBACK ListBoxReset(HWND hLB);
long CALLBACK ListBoxSetHorzScrollWidth(HWND hLB, long LBWidth);
long CALLBACK ListBoxSetColumnsWidth(HWND hLB, long LBWidth);
long CALLBACK ListBoxCount(HWND hLB);
long CALLBACK ListBoxGetTopIndex(HWND hLB);
long CALLBACK ListBoxSetTopIndex(HWND hLB, long LBIndex);
long CALLBACK ListBoxSetItemData(HWND hLB, long LBIndex, long LBDat);
long CALLBACK ListBoxGetItemData(HWND hLB, long LBIndex);
long CALLBACK ListBoxIsItemSelected(HWND hLB, long LBIndex);
long CALLBACK ListBoxGetSelItemIndex(HWND hLB);
long CALLBACK ListBoxSetIndex(HWND hLB, long LBIndex);
CStr CALLBACK ListBoxGetCurrentItem(HWND hLB);
CStr CALLBACK ListBoxGetItem(HWND hLB, long LBIndex);
long CALLBACK ListBoxDeleteItem(HWND hLB, long LBIndex);
long CALLBACK ListBoxReplaceItem(HWND hLB, long LBIndex, CStr LBNewItemText);
long CALLBACK ListBoxReplaceSelItem(HWND hLB, CStr LBNewItemText);
void CALLBACK ListBoxProcessDrag(HWND hLB, long Message, long lParam, long DragCursor);
long CALLBACK ListBoxSelItemDown(HWND hLB);
long CALLBACK ListBoxSelItemUp(HWND hLB);
long CALLBACK ListBoxSelItemRemove(HWND hLB);
long CALLBACK ListBoxItemExist(HWND hListBox, CStr CBItem);
long CALLBACK ListBoxItemUnderCursor(HWND hListBox);
HWND CALLBACK CreateScrollBar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle);
long CALLBACK ScrollBarGetMinRange(HWND hWnd, long ScrollBarType);
long CALLBACK ScrollBarGetMaxRange(HWND hWnd, long ScrollBarType);
long CALLBACK ScrollBarGetPageRange(HWND hWnd, long ScrollBarType);
long CALLBACK ScrollBarSetPageRange(HWND hWnd, long ScrollBarType, long PageValue);
long CALLBACK ScrollBarSetMinMaxRange(HWND hWnd, long ScrollBarType, long nMin, long nMax);
void CALLBACK ScrollBarShowHide(HWND hWnd, long ObjWidth, long ObjHeight, long Useclient);
HWND CALLBACK CreateDockingBox(CStr Title, WNDPROC WindowProc, HWND hRoot, char *IniFile);
long CALLBACK DockingBoxWasVisible(CStr DockName, CStr IniFile);
void CALLBACK DockingBoxRepaint(HWND hWnd);
long CALLBACK DockingBoxGetState(HWND hDock);
void CALLBACK DockingBoxRemove(HWND hDock);
void CALLBACK DockingBoxShow(HWND hDock, int MaxXSize, int MaxYSize, int DefaultXSize, int DefaultYSize, int DefaultState);
CStr CALLBACK MenuGetString(HMENU hMenu, long ItemID);
long CALLBACK MenuEnable(HMENU hMenu, long ItemPosition, long Enable);
long CALLBACK MenuSetDefaultItem(HMENU hMenu, long ItemPosition);
long CALLBACK MenuSetItemState(HMENU hMenu, long ItemState, long ItemID);
long CALLBACK MenuSetItemType(HMENU hMenu, long ItemType, long ItemID);
long CALLBACK MenuSetItemText(HMENU hMenu, CStr ItemText, long ItemID);
long CALLBACK MenuAddSeparator(HMENU hMenu);
long CALLBACK MenuAddString(HMENU hMenu, CStr ItemText, long ItemID, long Enabled);
long CALLBACK ScreenWidth(void);
long CALLBACK ScreenHeight(void);
void CALLBACK ScreenRectToClient(HWND hWnd, LPRECT RectToFill);
HCURSOR CALLBACK CursorSetWait(void);
HCURSOR CALLBACK CursorSetNo(void);
HCURSOR CALLBACK CursorSetCross(void);
HCURSOR CALLBACK CursorSetNormal(void);
long CALLBACK CursorDisable(void);
long CALLBACK CursorEnable(long OldCursorState);
CStr CALLBACK IniReadKey(CStr Section, CStr Key, CStr File);
long CALLBACK IniReadBoolKey(CStr Section, CStr Key, CStr File);
long CALLBACK IniWriteSection(CStr Section, CStr Key, CStr File);
long CALLBACK IniDeleteKey(CStr Section, CStr Key, CStr File);
long CALLBACK IniWriteKey(CStr Section, CStr Key, CStr Value, CStr File);
long CALLBACK MiscClipBoardCopyText(CStr Txt);
CStr CALLBACK MiscClipBoardPasteText(void);
long CALLBACK MiscClipBoardIsEmpty(void);
void CALLBACK MiscShellAddFileToRecents(CStr FileName);
long CALLBACK MathGenerateRandomNumber(long MaxNumber);
long CALLBACK MiscWaitEvents(HWND hClient, HACCEL hAccelerator, HWND hWndAccelerators);
long CALLBACK MiscMsgBox(HWND hParent, CStr MBText, long MBType, CStr MBTitle);
long CALLBACK MiscSendBroadCastMsg(long BDMessage);
void CALLBACK MiscDoEvents(HWND hClient, HACCEL hAccelerator, HWND hWndAccelerators);
HRESULT CALLBACK MiscObtainGUID(LPGUID GUIDToObtain);
HGLOBAL CALLBACK MiscAllocMem(long Amount);
long CALLBACK MiscFreeMem(void *MemHandle);
void CALLBACK MiscCopyMemLoop(void *MemSource, void *MemDest, long Length, long MemLeftOffset, long RepeatCount);
void CALLBACK MiscCopyMemWithOffsets(void *MemSource, void *MemDest, long Length, long SourceOffset, long DestOffset);
long CALLBACK MiscGetOSClass(void);
CStr CALLBACK DateGetNow(long ReportsSeconds);
CStr CALLBACK DateGetYear(void);
CStr CALLBACK DateGetMonth(void);
CStr CALLBACK DateGetDay(void);
CStr CALLBACK DateGetHour(void);
CStr CALLBACK DateGetMinutes(void);
CStr CALLBACK DateGetSeconds(void);
CStr CALLBACK DateGetFormat(CStr DateFormat);
CStr CALLBACK DateGetTimeFormat(CStr DateFormat);
long CALLBACK DateStringToDate(CStr StrDate, FILETIME *DestDate);
long CALLBACK StringFilterNonPrintableChars(long ASCIIChar, long CorrectChar);
long CALLBACK StringIsLabel(CStr AsmText, int Allow_Space);
long CALLBACK StringIsHexaDecimal(CStr HexaText);
long CALLBACK StringIsDecimal(CStr DecText, long MaxLen);
long CALLBACK StringIsLabelChar(CStr CLetter);
long CALLBACK StringIsDigitChar(CStr Letter);
long CALLBACK StringIsHexaChar(CStr Letter);
long CALLBACK StringIsAlphaChar(CStr Letter);
long CALLBACK StringIsBlankChar(CStr Letter);
long CALLBACK StringIsOpenBracketChar(CStr Letter);
long CALLBACK StringIsCloseBracketChar(CStr Letter);
long CALLBACK StringIsUnderLineChar(CStr Letter);
long CALLBACK StringIsDotChar(CStr Letter);
long CALLBACK StringIsQuoteChar(CStr Letter);
long CALLBACK StringIsCommaChar(CStr Letter);
long CALLBACK StringIsArobasChar(CStr Letter);
long CALLBACK StringIsSpaceChar(CStr Letter);
long CALLBACK StringIsEOL(CStr Letter);
CStr CALLBACK StringNumberComplement(long Nbr, long Complement);
CStr CALLBACK StringHexNumberComplement(long Nbr, long Complement);
CStr CALLBACK StringOctNumberComplement(long Nbr, long Complement);
CStr CALLBACK StringBinNumberComplement(long Nbr, long Complement);
void CALLBACK StringFree(long StringToFree);
long CALLBACK StringLen(long StringToCalc);
CStr CALLBACK StringCat(CStr String1, CStr String2);
CStr CALLBACK StringDecToString(long DecimalNbr);
long CALLBACK StringStringToDec(CStr DecString);
long CALLBACK StringStringToHex(CStr HexString);
long CALLBACK StringCount(CStr MaIn_String, CStr StringToCount, long CountStart, long CompareType);
CStr CALLBACK StringReplace(CStr MaIn_String, CStr StringToFind, CStr StringToReplace, long ReplaceStart, long RepCount, long CompareType);
CStr CALLBACK StringHex32(long Number);
CStr CALLBACK StringHex16(long Number);
CStr CALLBACK StringHex8(long Number);
CStr CALLBACK StringCopy(CStr DestString, CStr SourceString, long SourceStringLen, long DestStringLen);
CStr CALLBACK StringCopyAppendZero(CStr DestString, CStr SourceString);
CStr CALLBACK StringCopyFixed(CStr DestString, CStr SourceString);
CStr CALLBACK StringClear(CStr DestString, long DestStringLen);
CStr CALLBACK StringAppend(CStr DestString, CStr SourceString);
long * CALLBACK StringSplit(CStr StrToSplit, CStr StrSplitter);
long CALLBACK StringGetSplitUBound(long * SplitMem);
CStr CALLBACK StringGetSplitElement(CStr StrFromSplit, long *SplitMem, long Element);
void CALLBACK StringReleaseSplit(long *SplitMem);
CStr CALLBACK StringJoin(CStr StrFromSplit, long *SplitMem, CStr JoIn_String, long FirstElement, long ElementLimit);
CStr CALLBACK StringConvertToCStr(char *StringToConvert);
char * CALLBACK StringCStrConvertToString(CStr StringToConvert);
long CALLBACK StringCountLines(CStr StringToCount, long Length);
long CALLBACK StringGetLinesOffsets(CStr StringToCount);
void CALLBACK StringFillHexBytes(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
void CALLBACK StringFillHexWords(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
void CALLBACK StringFillHexDWords(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
void CALLBACK StringFileNameToLabel(char *FileName);
long CALLBACK FileGetWriteTime(CStr FileName, LPFILETIME FileWriteTime);
long CALLBACK FileGetCreationTime(CStr FileName, LPFILETIME FileCreationTime);
long CALLBACK FileGetAccessedTime(CStr FileName, LPFILETIME FileAccessedTime);
CStr CALLBACK FileGetExtension(CStr FileName);
CStr CALLBACK FileRemoveExtension(CStr FileName);
CStr CALLBACK FileReplaceExtension(CStr FileName, CStr NewExtension);
CStr CALLBACK FileGetDirectory(CStr FileName);
CStr CALLBACK FileGetFileName(CStr FileName);
HANDLE CALLBACK FileOpenR(CStr FileName);
HANDLE CALLBACK FileOpenW(CStr FileName, long FilePos);
HANDLE CALLBACK FileOpenWAppend(CStr FileName);
long CALLBACK FileClose(HANDLE hFile);
HANDLE CALLBACK FileCreateEmpty(CStr FileName, long Security);
long CALLBACK FileWriteBuffer(HANDLE FileHandle, LPCVOID BufferToWrite, long LenToWrite);
long CALLBACK FileWriteBufferVB(HANDLE FileHandle, CStr BufferToWrite, long LenToWrite);
long CALLBACK FileWriteLine(HANDLE FileHandle, CStr LineToWrite);
long CALLBACK FileIsDirectory(CStr FileToSearch);
long CALLBACK FileGetSize(CStr FileName);
long CALLBACK FileExist(CStr FileToSearch);
long CALLBACK FileIsReadOnly(CStr FileToCheck);
long CALLBACK FileIsUnix(CStr FileToCheck);
HGLOBAL CALLBACK FileLoadIntoMem(CStr FileName, long *BytesRead);
long CALLBACK FileSaveFromMem(CStr FileName, long MemToSave, long LenToSave);
CStr CALLBACK FileDir(CStr *PathName = 0,int Attributes = FILE_ATTRIBUTE_NORMAL);
long CALLBACK RegistryUpdateKey(HKEY KeyRoot, CStr KeyName, CStr SubKeyName, CStr SubKeyValue, long KeyType);
long CALLBACK RegistryDeleteKey(HKEY KeyRoot, CStr KeyName, CStr SubKeyName, long KeyType);
CStr CALLBACK RegistryGetKeyValue(HKEY KeyRoot, CStr KeyName, CStr SubKeyRef);
void CALLBACK GDIFillWindow(HWND hWnd, COLORREF Color);
void CALLBACK GDIFillWindowFromBrush(HWND hWnd, HBRUSH EraseBrush);
void CALLBACK GDIFillWindowFromBrushAndhDC(HWND hWnd, HDC hDC, HBRUSH EraseBrush);
HBRUSH CALLBACK GDICreateColorBrush(COLORREF Color);
HFONT CALLBACK GDIObtainFont(CStr FontNameToObtain, long FontSizeToObtain, HWND hWnd, long Bold, long Italic);
long CALLBACK GDIGetFontWidth(HWND hWnd, HFONT hFont);
long CALLBACK GDIGetFontHeight(HWND hWnd, HFONT hFont);
void CALLBACK GDIDrawHorzSep(HWND hWnd, long X, long Y, long Width);
void CALLBACK GDIDrawVertSep(HWND hWnd, long X, long Y, long Height);
void CALLBACK GDIDrawLine(HWND hWnd, long X1, long Y1, long X2, long Y2, COLORREF LineColor);
void CALLBACK GDIDrawLineXOR(HWND hWnd, long X1, long Y1, long X2, long Y2, COLORREF LineColor);
long CALLBACK GDIDrawPixel(HWND hWnd, long X, long Y, long PixelColor);
void CALLBACK GDIWriteText(HDC hDC, long X, long Y, CStr Text, long TextColor, HFONT hTextFont, long TextTransparent, long BackGroundColor);
void CALLBACK GDIWriteClippedText(HDC hDC, long X, long Y, long Width, long Height, CStr Text, long TextColor, HFONT hTextFont, long TextTransparent, long BackGroundColor);
long CALLBACK GDIGetTextWidth(HWND hWnd, HFONT hFont, CStr Txt);
long CALLBACK GDIGetTextHeight(HWND hWnd, HFONT hFont, CStr Txt);
HDC CALLBACK GDICreateBackDC(LPBACKDC BackStruct);
void CALLBACK GDIDestroyBackDC(LPBACKDC BackStruct);
long CALLBACK GDIBlitBackDC(LPRECT Dimensions, LPBACKDC BackStruct);
long CALLBACK GDIColorCalcLuminosity(COLORREF Color, long LuminosityFactor);
HFONT CALLBACK GDIGetSerif(void);
HFONT CALLBACK GDIGetSerif10(void);
HFONT CALLBACK GDIGetCourierNew9(void);
long CALLBACK ControlNextHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long CALLBACK ControlSendMessage(HWND hWnd, long Command, long lParam);
long CALLBACK ControlHeight(HWND hWnd);
long CALLBACK ControlClientTop(HWND hWnd);
long CALLBACK ControlClientLeft(HWND hWnd);
long CALLBACK ControlClientWidth(HWND hWnd);
long CALLBACK ControlClientHeight(HWND hWnd);
long CALLBACK ControlBringToTop(HWND hWnd);
long CALLBACK ControlBringToBottom(HWND hWnd);
long CALLBACK ControlSetTopMost(HWND hWnd);
long CALLBACK ControlRemTopMost(HWND hWnd);
void CALLBACK ControlRefresh(HWND hWnd);
void CALLBACK ControlRefreshLocal(HWND hWnd);
void CALLBACK ControlRepaint(HWND hWnd);
long CALLBACK ControlResize(HWND hControl, long CtlLeft, long CtlTop, long CtlWidth, long CtlHeight);
long CALLBACK ControlFreeze(HWND hControl, long FreezeState);
void CALLBACK ControlBound(HWND hParentControl, long &CtlLeft, long &CtlTop, long &CtlWidth, long &CtlHeight);
long CALLBACK ControlEnable(HWND hWnd, long CState);
long CALLBACK ControlVisible(HWND hWnd, long CState);
long CALLBACK ControlIsVisible(HWND hWnd);
long CALLBACK ControlIsEnabled(HWND hWnd);
CStr CALLBACK ControlGetText(HWND hWnd);
long CALLBACK ControlGetTextLen(HWND hWnd);
long CALLBACK ControlSetText(HWND hWnd, CStr TextToSet);
long CALLBACK ControlSetTextByAddr(HWND hWnd, long TextToSet);
long CALLBACK ControlGetNotifiedMsg(long lParam);
HWND CALLBACK ControlGetNotifiedhWnd(long lParam);
long CALLBACK ControlGetNotifiedID(long lParam);
long CALLBACK ControlTop(HWND hWnd);
long CALLBACK ControlLeft(HWND hWnd);
long CALLBACK ControlWidth(HWND hWnd);
HWND CALLBACK ControlGetHwndFromPoint(void);
long CALLBACK ControlGetNotifiedCommand(long wParam);
long CALLBACK ControlGetXMousePos(long lParam);
long CALLBACK ControlGetYMousePos(long lParam);
long CALLBACK ControlSetSerif8(HWND hWnd);
HFONT CALLBACK ControlGetFont(HWND hWnd);
HICON CALLBACK ControlGetIcon(HWND hWnd, long IconType);
long CALLBACK ControlGetHotKey(HWND hWnd);
long CALLBACK ControlSetFont(HWND hWnd, HFONT hFont);
long CALLBACK ControlSetFontNoRedraw(HWND hWnd, HFONT hFont);
long CALLBACK ControlIsWindowChild(HWND hWnd);
CStr CALLBACK ControlGetClassName(HWND hWnd);
long CALLBACK ControlSetClassCursor(HWND hWnd, HCURSOR hCursor);
long CALLBACK ControlHookWin(HWND hWnd, WNDPROC WindowProc);
long CALLBACK ControlUnHookWin(HWND hWnd);
long CALLBACK ControlGetWindowState(HWND hWnd);
long CALLBACK ControlClose(HWND hWnd);
LRESULT CALLBACK ControlCallwindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
long ComDlgChooseColor(HWND hWnd, long DefaultCol);
CStr ComDlgGetOpenFileName(HWND  hWnd, CStr OFilters, CStr InitDir, long MultiSelect, CStr DefaultDir);
CStr ComDlgGetSaveFileName(HWND hWnd, CStr OFilters, CStr InitDir, CStr DefaultDir);
long ComDlgChooseFont(HWND hWnd, CStr FntName, long FntSize, long FixedOnly);
CStr ComDlgBrowseForFolder(HWND hWnd, CStr Title);
CStr ComDlgParseMultiFilesSelection(CStr SelectedFiles, long (CALLBACK *EnumProc)(char *, long), long SearchDirection, long UserValue);
extern "C" long CALLBACK AllocMem(long MemSize);
extern "C" void CALLBACK FreeMem(long MemHandle);

#endif
