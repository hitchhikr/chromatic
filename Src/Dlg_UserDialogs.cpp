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
// Dlg_UserDialogs.cpp: User dialogs creation
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "Splitters.h"
#include "ProjectsBuilder.h"
#include "MDI_Form.h"
#include "Dlg_UserDialogs.h"

// -----------------------------------------------------------------------
// Constants
#define MOUSE_MODE_SELECT 0
#define MOUSE_MODE_EDIT 1
#define MOUSE_MODE_RESIZE 2

// -----------------------------------------------------------------------
// Variables
CStr DialogTitle;
HWND FRMDiaghwnd;
HWND FRMDiagTBhwnd;
HWND FRMDiagToolhwnd;
HWND FRMDiagPropshwnd;
HWND hDiagToolBar;
HWND hDiagTools;
HWND hPropCombobox;
long DiagEditMode;
long DiagControlNumber;
HMENU hGridMenu;
long DiagX1;
long DiagY1;
long DiagX2;
long DiagY2;
long OldDiagX1;
long OldDiagY1;
long OldDiagX2;
long OldDiagY2;
long DiagInDrag;
RECT DragRect;
RECT OldDragRect;
RECT ClipRect;
long FirstDrawn;
HDC DiaghDC;

long GridMode;
long GridXLarg;
long GridYLong;
long GridAlign;

long ToolbarMode;
long PropertiesMode;

// Arrays of pointers of created controls

long ToolbarXPos;
long ToolbarYPos;

long PropertiesXPos;
long PropertiesYPos;
long PropertiesXLarg;
long PropertiesYLong;

HDC BackGridDC;
HGDIOBJ OldGridObject;
HGDIOBJ BackGridhBitmap;
HBRUSH GridhBrush;
RECT GridRect;
BITMAPINFO BitmapInfos;
void *DIBBits;

long DialogBase;
CStr DialogName;
HBRUSH hSubEditBrush;

long CurrentLeft;
long CurrentTop;
long CurrentWidth;
long CurrentHeight;

HFONT CurrentFont;

long ObjectX;
long ObjectY;
long ObjectWidth;
long ObjectHeight;

HWND hGridOk;
HWND hGridCancel;
HWND hGridWidth;
HWND hGridHeight;
HWND hGridAlign;
HFONT IPSerif;

int Tab_DiagTools[] =
{
    DIALOGCURSOR,
    DIALOGPICTURE,
    DIALOGLABEL,
    DIALOGTEXTBOX,
    DIALOGFRAME,
    DIALOGBUTTON,
    DIALOGCHECKBOX,
    DIALOGRADIOBUTTON,
    DIALOGCOMBOBOX,
    DIALOGLISTBOX,
    DIALOGHSCROLLBAR,
    DIALOGTREEVIEW,
    DIALOGLISTVIEW,
    DIALOGSYSTAB,
    DIALOGPROGRESSBAR,
    DIALOGTRACKBAR,
    DIALOGSPIN,
    DIALOGXCOMBOBOX,
    DIALOGTEXTIP,
    DIALOGHOTKEY,
    DIALOGCALENDAR,
    DIALOGDATEPICKER,
    DIALOGANIMATION,
    DIALOGTOOLBAR,
    DIALOGSTATUSBAR,
    DIALOGRICHTEXT,
    DIALOGPAGER,
    DIALOGHEADER,
    DIALOGREBAR,
    DIALOGTOOLTIP
};

int Tab_Tools[] =
{
    DIALOGTOOLS_ALIGNLEFT,
    DIALOGTOOLS_ALIGNRIGHT,
    DIALOGTOOLS_ALIGNTOP,
    DIALOGTOOLS_ALIGNBOTTOM,
    DIALOGTOOLS_CENTERHORZ,
    DIALOGTOOLS_CENTERVERT,
    DIALOGTOOLS_SAMEWIDTH,
    DIALOGTOOLS_SAMEHEIGHT,
    DIALOGTOOLS_SAMESIZE,
    DIALOGTOOLS_SPACEACROSS,
    DIALOGTOOLS_SPACEDOWN,
    DIALOGTOOLS_ALIGNGRID,
    DIALOGTOOLS_LOCKCTRL,
    0,
    DIALOGTOOLS_GRID,
    DIALOGTOOLS_TOOLBAR,
    DIALOGTOOLS_PROPERTIES,
    DIALOGTOOLS_TEST
};

// -----------------------------------------------------------------------
// Private functions
void SaveDialog(void);
void RefreshDialog(void);
void ResizeFromToolbar(HWND Dialog, HWND Toolbar, int PosX, int PosY, int NbrColumns, int NbrRows, int Tab_Buttons[]);

// -----------------------------------------------------------------------
// Create new dialog resource
void CreateUserDialog(void)
{
    long EntryAccCount = 0;

    EntryAccCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewDialogs);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewDialogs, "DIALOG_" + (CStr) EntryAccCount) != -1)
    {
        EntryAccCount++;
    }
    DialogBase = EntryAccCount;
    DialogName = "DIALOG_" + (CStr) DialogBase;
    DialogTitle = "New dialog";
    CurrentLeft = 10 + 200; // PropertiesXPos + PropertiesXLarg
    CurrentWidth = ((ScreenWidth() * 98) / 100) - CurrentLeft;
    CurrentHeight = ((ScreenHeight() * 70) / 100) - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYBORDER);
    CurrentTop = (ScreenHeight() - ((ScreenHeight() * 70) / 100)) / 2;
    CreateModalDialog(CurrentLeft, CurrentTop, CurrentWidth, CurrentHeight, hMDIform.hWnd, &FRMUserDiagProc, WS_BORDER | WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX | DS_ABSALIGN, 0);
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMUserDiagProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long ModWidth = 0;
    long ModHeight = 0;
    long SaveCoord = 0;
    CStr PrimGridMode;
    CStr PrimGridAlign;
    CStr PrimGridWidth;
    CStr PrimGridHeight;
    CStr PrimToolbarMode;
    CStr PrimPropertiesMode;
    RECT CRect;
    HWND SizehWnd;
    LPRECT RequestedRect;
    LPGRIPBOXSQRBOXDAT RequestedSteps;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            if(hSubEditBrush != 0) DeleteObject(hSubEditBrush);
            CreateSubEditBrush(GetSysColor(COLOR_WINDOW));
            CreateGrid(hwndDlg);
            ResizeFromToolbar(FRMDiagTBhwnd, hDiagToolBar, ControlLeft(FRMDiagTBhwnd), ControlTop(FRMDiagTBhwnd), 15, 2, Tab_DiagTools);
            ResizeFromToolbar(FRMDiagToolhwnd, hDiagTools, ControlLeft(FRMDiagToolhwnd), ControlTop(FRMDiagToolhwnd), 18, 1, Tab_Tools);
            ControlRefresh(FRMDiagTBhwnd);
            ControlRefresh(FRMDiagToolhwnd);
            break;
        case WM_INITDIALOG:
            CurrentWidth = 390 + 8;
            CurrentHeight = 260 + 27;
            FRMDiaghwnd = hwndDlg;
            ControlSetText(hwndDlg, DialogTitle);
            CreateSubEditBrush(GetSysColor(COLOR_WINDOW));
            FillPropertiesStrings();

            hGridMenu = CreatePopupMenu();
            MenuAddString(hGridMenu, "Show grid", GRID_IDBASE + GRID_TOGGLE_ID, TRUE);
            MenuSetDefaultItem(hGridMenu, 0);
            MenuAddSeparator(hGridMenu);
            MenuAddString(hGridMenu, "Grid settings...", GRID_IDBASE + GRID_SETTINGS_ID, TRUE);
            ToolbarMode = 0;
            // Read toolbar settings
            PrimToolbarMode = IniReadKey("Layout", "DialogToolbar", MainIniFile);
            if(PrimToolbarMode.Len() != 0) if(strcmp(PrimToolbarMode.Get_String(), "1") == 0) ToolbarMode = 1;
            PropertiesMode = 0;
            // Read properties settings
            PrimPropertiesMode = IniReadKey("Layout", "DialogProperties", MainIniFile);
            if(PrimPropertiesMode.Len() != 0) if(strcmp(PrimPropertiesMode.Get_String(), "1") == 0) PropertiesMode = 1;
            GridMode = 0;
            GridAlign = FALSE;
            GridXLarg = 16;
            GridYLong = 16;
            // Read grid settings
            PrimGridMode = IniReadKey("Layout", "DialogGrid", MainIniFile);
            if(PrimGridMode.Len() != 0) if(strcmp(PrimGridMode.Get_String(), "1") == 0) GridMode = 1;
            PrimGridAlign = IniReadKey("Layout", "AlignGrid", MainIniFile);
            if(PrimGridAlign.Len() != 0) if(strcmp(PrimGridAlign.Get_String(), "1") == 0) GridAlign = TRUE;
            PrimGridWidth = IniReadKey("Layout", "GridWidth", MainIniFile);
            if(PrimGridWidth.Get_String() != 0) GridXLarg = PrimGridWidth.Get_Long();
            if(GridXLarg == 0) GridXLarg = 16;
            PrimGridHeight = IniReadKey("Layout", "GridHeight", MainIniFile);
            if(PrimGridHeight.Len() != 0) GridYLong = PrimGridHeight.Get_Long();
            if(GridYLong == 0) GridYLong = 16;
            PropertiesXPos = 10;
            PropertiesYPos = (ScreenHeight() - ((ScreenHeight() * 70) / 100)) / 2;
            PropertiesXLarg = 200;
            PropertiesYLong = (ScreenHeight() * 70) / 100;
            ToolbarXPos = 10;
            ToolbarYPos = PropertiesYPos - 82;
            if(ToolbarMode == 1) FRMDiagTBhwnd = CreateNonModalDialog(ToolbarXPos, ToolbarYPos, 398 + 60, 82, hwndDlg, 0, 0, "Controls", &FrmUserDiagToolBarInitProc, &FrmUserDiagToolBarProc, WS_EX_TOOLWINDOW, WS_SYSMENU, SW_SHOW);
            FRMDiagToolhwnd = CreateNonModalDialog(10, PropertiesYLong + PropertiesYPos, 437, 47, hwndDlg, 0, 0, "Tools", &FrmUserDiagToolsInitProc, &FrmUserDiagToolsProc, WS_EX_TOOLWINDOW, 0, SW_SHOW);
            
            ResizeFromToolbar(FRMDiagTBhwnd, hDiagToolBar, ToolbarXPos, ToolbarYPos, 15, 2, Tab_DiagTools);
            ResizeFromToolbar(FRMDiagToolhwnd, hDiagTools, 10, PropertiesYLong + PropertiesYPos, 18, 1, Tab_Tools);

            if(PropertiesMode == 1) FRMDiagPropshwnd = CreateNonModalDialog(PropertiesXPos, PropertiesYPos, PropertiesXLarg, PropertiesYLong, hwndDlg, 0, 0, "Properties", &FrmUserDiagPropsInitProc, &FrmUserDiagPropsProc, WS_EX_TOOLWINDOW, WS_SYSMENU | WS_SIZEBOX, SW_SHOW);
            CreateGrid(hwndDlg);
            // Obtain a font for IPBox
            IPSerif = GDIObtainFont("MS Sans Serif", 8, hwndDlg, 0, 0);
            FreezeTimer = 1;
            return(1);
//        case WM_PAINT:
  //          BeginPaint(hwndDlg, &DiagPs);
            //if(GridMode == 1) BitBlt(DiagPs.hdc, 0, 0, DiagPs.rcPaint.right, DiagPs.rcPaint.bottom, BackGridDC, 0, 0, SRCCOPY);
    //        EndPaint(hwndDlg, &DiagPs);
    //      break;
        case WM_ERASEBKGND:
            if(GridMode == 1)
            {
                GetClientRect(hwndDlg, &CRect);
                BitBlt((HDC) wParam, 0, 0, CRect.right, CRect.bottom, BackGridDC, 0, 0, SRCCOPY);
                return(TRUE);
            }
            else
            {
                return(FALSE);
            }
            break;
        case WM_LBUTTONDOWN:
            if((wParam & MK_LBUTTON) != 0)
            {
                // Remove all selections if a control is about to be
                // created.
                UnSelectControlsList();
                DiagX1 = ControlGetXMousePos(lParam);
                DiagY1 = ControlGetYMousePos(lParam);
                GetClientRect(hwndDlg, &CRect);
                if(DiagX1 > CRect.right) DiagX1 = CRect.right;
                if(DiagY1 > CRect.bottom) DiagY1 = CRect.bottom;
                if(GridAlign && DiagEditMode == MOUSE_MODE_EDIT)
                {
                    DiagX1 = (DiagX1 / GridXLarg) * GridXLarg;
                    DiagY1 = (DiagY1 / GridYLong) * GridYLong;
                }
                OldDiagX1 = DiagX1;
                OldDiagY1 = DiagY1;
                OldDiagX2 = DiagX1;
                OldDiagY2 = DiagY1;
                DiagInDrag = 1;
                FirstDrawn = 1;
                SetCapture(hwndDlg);
                return(0);
            }
            break;
        case WM_MOUSEMOVE:
            if((wParam & MK_LBUTTON) != 0)
            {
                if(DiagInDrag == 1)
                {
                    DiagX2 = ControlGetXMousePos(lParam);
                    DiagY2 = ControlGetYMousePos(lParam);
                    GetClientRect(hwndDlg, &CRect);
                    if(DiagX2 > CRect.right) DiagX2 = CRect.right;
                    if(DiagY2 > CRect.bottom) DiagY2 = CRect.bottom;
                    if(GridAlign && DiagEditMode == MOUSE_MODE_EDIT)
                    {
                        DiagX2 = ((DiagX2 / GridXLarg) * GridXLarg) + 1;
                        DiagY2 = ((DiagY2 / GridYLong) * GridYLong) + 1;
                        if((DiagX2 - DiagX1) <= 0) DiagX2--;
                        if((DiagY2 - DiagY1) <= 0) DiagY2--;
                    }
                    FillOldRectDatas();
                    FillNewRectDatas();
                    if(FirstDrawn == 0) DrawOldSelectRect(hwndDlg);
                    FirstDrawn = 0;
                    DrawNewSelectRect(hwndDlg);
                    OldDiagX1 = DiagX1;
                    OldDiagY1 = DiagY1;
                    OldDiagX2 = DiagX2;
                    OldDiagY2 = DiagY2;
                    return(0);
                }
            }
            break;
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            // Cancel dragging with right mouse button
            if(DiagInDrag == 1)
            {
                DiagInDrag = 0;
                DiagX2 = ControlGetXMousePos(lParam);
                DiagY2 = ControlGetYMousePos(lParam);
                GetClientRect(hwndDlg, &CRect);
                if(DiagX2 > CRect.right) DiagX2 = CRect.right;
                if(DiagY2 > CRect.bottom) DiagY2 = CRect.bottom;
                if(GridAlign && DiagEditMode == MOUSE_MODE_EDIT)
                {
                    DiagX2 = ((DiagX2 / GridXLarg) * GridXLarg) + 1;
                    DiagY2 = ((DiagY2 / GridYLong) * GridYLong) + 1;
                    if((DiagX2 - DiagX1) <= 0) DiagX2--;
                    if((DiagY2 - DiagY1) <= 0) DiagY2--;
                }
                FillNewRectDatas();
                if(FirstDrawn == 0) DrawNewSelectRect(hwndDlg);
                ReleaseCapture();
            }
            break;
        case WM_LBUTTONUP:
            if(DiagInDrag == 1)
            {
                DiagInDrag = 0;
                DiagX2 = ControlGetXMousePos(lParam);
                DiagY2 = ControlGetYMousePos(lParam);
                GetClientRect(hwndDlg, &CRect);
                if(DiagX2 > CRect.right) DiagX2 = CRect.right;
                if(DiagY2 > CRect.bottom) DiagY2 = CRect.bottom;
                if(GridAlign && DiagEditMode == MOUSE_MODE_EDIT)
                {
                    DiagX2 = ((DiagX2 / GridXLarg) * GridXLarg) + 1;
                    DiagY2 = ((DiagY2 / GridYLong) * GridYLong) + 1;
                    if((DiagX2 - DiagX1) <= 0) DiagX2--;
                    if((DiagY2 - DiagY1) <= 0) DiagY2--;
                }
                FillNewRectDatas();
                if(FirstDrawn == 0) DrawNewSelectRect(hwndDlg);
                ReleaseCapture();
                // Correct coordinates
                DiagX2--;
                DiagY2--;
                if(DiagX1 > DiagX2)
                {
                    SaveCoord = DiagX1;
                    DiagX1 = DiagX2;
                    DiagX2 = SaveCoord;
                }
                if(DiagY1 > DiagY2)
                {
                    SaveCoord = DiagY1;
                    DiagY1 = DiagY2;
                    DiagY2 = SaveCoord;
                }
                if(DiagEditMode == MOUSE_MODE_EDIT)
                {
                    if((DiagX1 != DiagX2) && (DiagY1 != DiagY2))
                    {
                        // Avoid creation of controls with dimensions of 1x1
                        // (Since 0x0 isn't possible due to coordinates corrections done above)
                        if((DiagX1 != (DiagX2 - 1)) && (DiagY1 != (DiagY2 - 1)))
                        {
                            // Place an object on dialog
                            ObjectX = DiagX1;
                            ObjectY = DiagY1;
                            ObjectWidth = (DiagX2 - DiagX1) + 1;
                            ObjectHeight = (DiagY2 - DiagY1) + 1;
                            CreateNewControl(hwndDlg);
                        }
                    }
                }
                else
                {
                    // Select objects
                }
            }
            break;
        case WM_SIZE:
            // Modify width
            ModWidth = (lParam & 0xFFFF);
            if(CurrentWidth != ModWidth)
            {
                // Refresh properties listbox
                CurrentWidth = ModWidth;
                //If DiagLabelsFormDimRoutine(2) <> 0 Then JumpToAddr DiagLabelsFormDimRoutine(2)
            }
            // Modify height
            ModHeight = (short) ((lParam & 0xFFFF0000) / 0x10000);
            if(CurrentHeight != ModHeight)
            {
                CurrentHeight = ModHeight;
                // Refresh properties listbox
                //If DiagLabelsFormDimRoutine(3) <> 0 Then JumpToAddr DiagLabelsFormDimRoutine(3)
            }
            break;
        case GRIPBOX_MSG_ASKSIZE:
            // A GripBox asks for the position of a control
            // wParam = GripBox hwnd
            // lParam = RECT struct to fill
            SizehWnd = GetControlFromGripBox((LPGRIPBOXDAT) wParam);
            if(SizehWnd != NULL)
            {
                // Fill the RECT struct
                RequestedRect = (LPRECT) lParam;
                GetWindowRect(SizehWnd, RequestedRect);
                ScreenRectToClient(hwndDlg, RequestedRect);
            }
            return(0);
        case GRIPBOX_MSG_ASKSTEPS:
            // A Gripbox asks for the X/Y steps
            // wParam = 0
            // lParam = Pointer to GRIPBOXSQRBOXDAT structure to fill
            RequestedSteps = (LPGRIPBOXSQRBOXDAT) lParam;
            if(GridAlign)
            {
                RequestedSteps->StepX = GridXLarg;
                RequestedSteps->StepY = GridYLong;
            }
            else
            {
                RequestedSteps->StepX = 1;
                RequestedSteps->StepY = 1;
            }
            return(0);

        case GRIPBOX_MSG_RESIZED:
            // A GripBox tells that a control has been resized
            // wParam = GripBox hwnd
            // lParam = RECT struct containing new coordinates
            ResizeControlFromGripBox((LPGRIPBOXDAT) wParam, (LPRECT) lParam);
            return(0);

        case WM_CLOSE:
            DiscardGrid();
            if(hSubEditBrush != 0) DeleteObject(hSubEditBrush);
            if(IPSerif != 0) DeleteObject(IPSerif);
            if(hGridMenu != 0) DestroyMenu(hGridMenu);
            if(CurrentFont != 0) DeleteObject(CurrentFont);
            SetDialogEditModeOff();
            UnSelectControlsList();
            SaveDialog();
            EraseControlsList();
            CursorSetNormal();
            ControlClose(FRMDiagToolhwnd);
            ControlClose(FRMDiagTBhwnd);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Toolbar form init
void CALLBACK FrmUserDiagToolBarInitProc(HWND hWnd)
{
    hDiagToolBar = CreateToolBar(1, 1, 460, 66, hWnd, GlobalImageList4, 0, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | TBSTYLE_WRAPABLE | WS_TABSTOP, 0);
    ToolBarAddButton(hDiagToolBar, "", DIALOGCURSOR, ICON_RES_CURSOR, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGPICTURE, ICON_RES_PICTURE, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGLABEL, ICON_RES_LABEL, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGTEXTBOX, ICON_RES_TEXT, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGFRAME, ICON_RES_FRAME, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGBUTTON, ICON_RES_BUTTON, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGCHECKBOX, ICON_RES_CHECKBOX, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGRADIOBUTTON, ICON_RES_RADIOBUTTON, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGCOMBOBOX, ICON_RES_COMBOBOX, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGLISTBOX, ICON_RES_LISTBOX, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGHSCROLLBAR, ICON_RES_HSCROLLBAR, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGTREEVIEW, ICON_RES_TREEVIEW, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGLISTVIEW, ICON_RES_LISTVIEW, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGSYSTAB, ICON_RES_SYSTAB, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGPROGRESSBAR, ICON_RES_PROGRESSBAR, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGTRACKBAR, ICON_RES_SLIDER, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGSPIN, ICON_RES_SPIN, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGXCOMBOBOX, ICON_RES_XCOMBOBOX, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGTEXTIP, ICON_RES_TEXTIP, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGHOTKEY, ICON_RES_HOTKEY, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGCALENDAR, ICON_RES_CALENDAR, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGDATEPICKER, ICON_RES_DATEPICKER, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGANIMATION, ICON_RES_ANIMATION, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGTOOLBAR, ICON_RES_TOOLBAR, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGSTATUSBAR, ICON_RES_STATUSBAR, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGRICHTEXT, ICON_RES_RICHTEXT, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGPAGER, ICON_RES_PAGER, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGHEADER, ICON_RES_HEADER, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGREBAR, ICON_RES_REBAR, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagToolBar, "", DIALOGTOOLTIP, ICON_RES_TOOLTIP, TBSTYLE_CHECKGROUP, TBSTATE_ENABLED, 1);
    // Edit mode off
    ToolBarSetButtonChecked(hDiagToolBar, DIALOGCURSOR, 1);
    SetDialogEditModeOff();
}

// -----------------------------------------------------------------------
// Toolbar form hook
LRESULT CALLBACK FrmUserDiagToolBarProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == hDiagToolBar)
            {
                switch(wParam)
                {
                    case DIALOGCURSOR:
                        SetDialogEditModeOff();
                        DiagControlNumber = CONTROL_NONE;
                        return(0);
                    case DIALOGPICTURE:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_PICTURE;
                        return(0);
                    case DIALOGLABEL:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_LABEL;
                        return(0);
                    case DIALOGTEXTBOX:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_TEXTBOX;
                        return(0);
                    case DIALOGFRAME:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_FRAME;
                        return(0);
                    case DIALOGBUTTON:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_BUTTON;
                        return(0);
                    case DIALOGCHECKBOX:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_CHECKBOX;
                        return(0);
                    case DIALOGRADIOBUTTON:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_RADIOBUTTON;
                        return(0);
                    case DIALOGCOMBOBOX:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_COMBOBOX;
                        return(0);
                    case DIALOGLISTBOX:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_LISTBOX;
                        return(0);
                    case DIALOGHSCROLLBAR:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_HSCROLLBAR;
                        return(0);
                    case DIALOGTREEVIEW:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_TREEVIEW;
                        return(0);
                    case DIALOGLISTVIEW:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_LISTVIEW;
                        return(0);
                    case DIALOGSYSTAB:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_SYSTAB;
                        return(0);
                    case DIALOGPROGRESSBAR:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_PROGRESSBAR;
                        return(0);
                    case DIALOGTRACKBAR:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_TRACKBAR;
                        return(0);
                    case DIALOGSPIN:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_UPDOWN;
                        return(0);
                    case DIALOGXCOMBOBOX:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_XCOMBOBOX;
                        return(0);
                    case DIALOGTEXTIP:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_TEXTIP;
                        return(0);
                    case DIALOGHOTKEY:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_HOTKEY;
                        return(0);
                    case DIALOGCALENDAR:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_CALENDAR;
                        return(0);
                    case DIALOGDATEPICKER:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_DATEPICKER;
                        return(0);
                    case DIALOGANIMATION:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_ANIMATION;
                        return(0);
                    case DIALOGTOOLBAR:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_TOOLBAR;
                        return(0);
                    case DIALOGSTATUSBAR:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_STATUSBAR;
                        return(0);
                    case DIALOGRICHTEXT:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_RICHTEXT;
                        return(0);
                    case DIALOGPAGER:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_PAGER;
                        return(0);
                    case DIALOGHEADER:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_HEADER;
                        return(0);
                    case DIALOGREBAR:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_REBAR;
                        return(0);
                    case DIALOGTOOLTIP:
                        SetDialogEditModeOn();
                        DiagControlNumber = CONTROL_TOOLTIP;
                        return(0);
                }
            }
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    switch(ControlGetNotifiedID(lParam))
                    {
                        case DIALOGCURSOR:
                            ToolBarDisplayToolTip("Pointer", lParam);
                            return(0);
                        case DIALOGPICTURE:
                            ToolBarDisplayToolTip("Picture", lParam);
                            return(0);
                        case DIALOGLABEL:
                            ToolBarDisplayToolTip("Static Text", lParam);
                            return(0);
                        case DIALOGTEXTBOX:
                            ToolBarDisplayToolTip("EditBox", lParam);
                            return(0);
                        case DIALOGFRAME:
                            ToolBarDisplayToolTip("GroupBox", lParam);
                            return(0);
                        case DIALOGBUTTON:
                            ToolBarDisplayToolTip("Button", lParam);
                            return(0);
                        case DIALOGCHECKBOX:
                            ToolBarDisplayToolTip("CheckBox", lParam);
                            return(0);
                        case DIALOGRADIOBUTTON:
                            ToolBarDisplayToolTip("Radio Button", lParam);
                            return(0);
                        case DIALOGCOMBOBOX:
                            ToolBarDisplayToolTip("ComboBox", lParam);
                            return(0);
                        case DIALOGLISTBOX:
                            ToolBarDisplayToolTip("ListBox", lParam);
                            return(0);
                        case DIALOGHSCROLLBAR:
                            ToolBarDisplayToolTip("ScrollBar", lParam);
                            return(0);
                        case DIALOGTREEVIEW:
                            ToolBarDisplayToolTip("Tree Control", lParam);
                            return(0);
                        case DIALOGLISTVIEW:
                            ToolBarDisplayToolTip("List Control", lParam);
                            return(0);
                        case DIALOGSYSTAB:
                            ToolBarDisplayToolTip("Tab Control", lParam);
                            return(0);
                        case DIALOGPROGRESSBAR:
                            ToolBarDisplayToolTip("ProgressBar", lParam);
                            return(0);
                        case DIALOGTRACKBAR:
                            ToolBarDisplayToolTip("TrackBar", lParam);
                            return(0);
                        case DIALOGSPIN:
                            ToolBarDisplayToolTip("Up/Down", lParam);
                            return(0);
                        case DIALOGXCOMBOBOX:
                            ToolBarDisplayToolTip("Extended ComboBox", lParam);
                            return(0);
                        case DIALOGTEXTIP:
                            ToolBarDisplayToolTip("IP Address", lParam);
                            return(0);
                        case DIALOGHOTKEY:
                            ToolBarDisplayToolTip("Hot Key", lParam);
                            return(0);
                        case DIALOGCALENDAR:
                            ToolBarDisplayToolTip("Month Calendar", lParam);
                            return(0);
                        case DIALOGDATEPICKER:
                            ToolBarDisplayToolTip("Date/Time Picker", lParam);
                            return(0);
                        case DIALOGANIMATION:
                            ToolBarDisplayToolTip("Animate", lParam);
                            return(0);
                        case DIALOGTOOLBAR:
                            ToolBarDisplayToolTip("ToolBar", lParam);
                            return(0);
                        case DIALOGSTATUSBAR:
                            ToolBarDisplayToolTip("StatusBar", lParam);
                            return(0);
                        case DIALOGRICHTEXT:
                            ToolBarDisplayToolTip("Rich EditBox", lParam);
                            return(0);
                        case DIALOGPAGER:
                            ToolBarDisplayToolTip("Pager", lParam);
                            return(0);
                        case DIALOGHEADER:
                            ToolBarDisplayToolTip("Header", lParam);
                            return(0);
                        case DIALOGREBAR:
                            ToolBarDisplayToolTip("ReBar", lParam);
                            return(0);
                        case DIALOGTOOLTIP:
                            ToolBarDisplayToolTip("ToolTip", lParam);
                            return(0);
                    }
            }
            break;
        case WM_CLOSE:
            ToolbarXPos = ControlLeft(hWnd);
            ToolbarYPos = ControlTop(hWnd);
            break;
        case WM_DESTROY:
            ToolBarSetButtonChecked(hDiagTools, DIALOGTOOLS_TOOLBAR, 0);
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Tools form init
void CALLBACK FrmUserDiagToolsInitProc(HWND hWnd)
{
    hDiagTools = CreateToolBar(1, 1, 429, 45, hWnd, GlobalImageList5, 0, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, TBSTYLE_EX_DRAWDDARROWS);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_ALIGNLEFT, ICON_RES_TOOL_ALIGNLEFT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_ALIGNRIGHT, ICON_RES_TOOL_ALIGNRIGHT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_ALIGNTOP, ICON_RES_TOOL_ALIGNTOP, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_ALIGNBOTTOM, ICON_RES_TOOL_ALIGNBOTTOM, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_CENTERHORZ, ICON_RES_TOOL_CENTERHORZ, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_CENTERVERT, ICON_RES_TOOL_CENTERVERT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_SAMEWIDTH, ICON_RES_TOOL_SAMEWIDTH, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_SAMEHEIGHT, ICON_RES_TOOL_SAMEHEIGHT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_SAMESIZE, ICON_RES_TOOL_SAMESIZE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_SPACEACROSS, ICON_RES_TOOL_SPACEACROSS, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_SPACEDOWN, ICON_RES_TOOL_SPACEDOWN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_ALIGNGRID, ICON_RES_TOOL_ALIGNGRID, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_LOCKCTRL, ICON_RES_TOOL_LOCKCTRL, TBSTYLE_CHECK, TBSTATE_ENABLED, 1);
    ToolBarAddSeparator(hDiagTools, 0);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_GRID, ICON_RES_TOOL_GRID, TBSTYLE_DROPDOWN | TBSTYLE_CHECK, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_TOOLBAR, ICON_RES_TOOL_TOOLBAR, TBSTYLE_CHECK, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_PROPERTIES, ICON_RES_TOOL_PROPERTIES, TBSTYLE_CHECK, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hDiagTools, "", DIALOGTOOLS_TEST, ICON_RES_TOOL_TEST, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    if(ToolbarMode == 1) ToolBarSetButtonChecked(hDiagTools, DIALOGTOOLS_TOOLBAR, 1);
    if(PropertiesMode == 1) ToolBarSetButtonChecked(hDiagTools, DIALOGTOOLS_PROPERTIES, 1);
    if(GridMode == 1)
    {
        ToolBarSetButtonChecked(hDiagTools, DIALOGTOOLS_GRID, 1);
        CheckMenuItem(hGridMenu, GRID_TOGGLE_ID, MF_BYPOSITION | MF_CHECKED);
    }
}

// -----------------------------------------------------------------------
// Toolbar form hook
LRESULT CALLBACK FrmUserDiagToolsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT FrmDiagToolsPs;

    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == hDiagTools)
            {
                switch(wParam)
                {
                    case DIALOGTOOLS_ALIGNLEFT:
                        return(0);
                    case DIALOGTOOLS_ALIGNRIGHT:
                        return(0);
                    case DIALOGTOOLS_ALIGNTOP:
                        return(0);
                    case DIALOGTOOLS_ALIGNBOTTOM:
                        return(0);
                    case DIALOGTOOLS_CENTERHORZ:
                        return(0);
                    case DIALOGTOOLS_CENTERVERT:
                        return(0);
                    case DIALOGTOOLS_SAMEWIDTH:
                        return(0);
                    case DIALOGTOOLS_SAMEHEIGHT:
                        return(0);
                    case DIALOGTOOLS_SAMESIZE:
                        return(0);
                    case DIALOGTOOLS_SPACEACROSS:
                        return(0);
                    case DIALOGTOOLS_SPACEDOWN:
                        return(0);
                    case DIALOGTOOLS_GRID:
                        if(ToolBarIsButtonChecked(hDiagTools, DIALOGTOOLS_GRID) == 1)
                        {
                            SetGridOn();
                        }
                        else
                        {
                            SetGridOff();
                        }
                        return(0);
                    case DIALOGTOOLS_LOCKCTRL:
                        return(0);
                    case DIALOGTOOLS_TOOLBAR:
                        if(ToolBarIsButtonChecked(hDiagTools, DIALOGTOOLS_TOOLBAR) == 1)
                        {
                            FRMDiagTBhwnd = CreateNonModalDialog(ToolbarXPos, ToolbarYPos, 398, 82, FRMDiaghwnd, 0, 0, "Controls", &FrmUserDiagToolBarInitProc, &FrmUserDiagToolBarProc, WS_EX_TOOLWINDOW, WS_SYSMENU, SW_SHOW);
                            IniWriteKey("Layout", "DialogToolbar", "1", MainIniFile);
                            ResizeFromToolbar(FRMDiagTBhwnd, hDiagToolBar, ToolbarXPos, ToolbarYPos, 15, 2, Tab_DiagTools);
                        }
                        else
                        {
                            ControlClose(FRMDiagTBhwnd);
                            IniWriteKey("Layout", "DialogToolbar", "0", MainIniFile);
                        }
                        return(0);
                    case DIALOGTOOLS_PROPERTIES:
                        if(ToolBarIsButtonChecked(hDiagTools, DIALOGTOOLS_PROPERTIES) == 1)
                        {
                            FRMDiagPropshwnd = CreateNonModalDialog(PropertiesXPos, PropertiesYPos, PropertiesXLarg, PropertiesYLong, FRMDiaghwnd, 0, 0, "Properties", &FrmUserDiagPropsInitProc, &FrmUserDiagPropsProc, WS_EX_TOOLWINDOW, WS_SYSMENU | WS_SIZEBOX, SW_SHOW);
                            IniWriteKey("Layout", "DialogProperties", "1", MainIniFile);
                        }
                        else
                        {
                            ControlClose(FRMDiagPropshwnd);
                            IniWriteKey("Layout", "DialogProperties", "0", MainIniFile);
                        }
                        return(0);
                    case DIALOGTOOLS_TEST:
                        return(0);
                }
            }
            else
            {
                switch(wParam & 0xFFFF)
                {
                    case (GRID_IDBASE + GRID_TOGGLE_ID):
                        GridMode = GridMode ^ 1;
                        ToggleGridMode();
                        return(0);
                    case (GRID_IDBASE + GRID_SETTINGS_ID):
                        CreateModalDialog(-1, -1, 190, 110, FRMDiaghwnd, &FRMGridDiagProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
                        return(0);
                }
            }
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    switch(ControlGetNotifiedID(lParam))
                    {
                        case DIALOGTOOLS_ALIGNLEFT:
                            ToolBarDisplayToolTip("Align Left", lParam);
                            return(0);
                        case DIALOGTOOLS_ALIGNRIGHT:
                            ToolBarDisplayToolTip("Align Right", lParam);
                            return(0);
                        case DIALOGTOOLS_ALIGNTOP:
                            ToolBarDisplayToolTip("Align Top", lParam);
                            return(0);
                        case DIALOGTOOLS_ALIGNBOTTOM:
                            ToolBarDisplayToolTip("Align Bottom", lParam);
                            return(0);
                        case DIALOGTOOLS_CENTERHORZ:
                            ToolBarDisplayToolTip("Center Horizontally", lParam);
                            return(0);
                        case DIALOGTOOLS_CENTERVERT:
                            ToolBarDisplayToolTip("Center Vertically", lParam);
                            return(0);
                        case DIALOGTOOLS_SAMEWIDTH:
                            ToolBarDisplayToolTip("Make Same Width", lParam);
                            return(0);
                        case DIALOGTOOLS_SAMEHEIGHT:
                            ToolBarDisplayToolTip("Make Same Height", lParam);
                            return(0);
                        case DIALOGTOOLS_SAMESIZE:
                            ToolBarDisplayToolTip("Make Same Size", lParam);
                            return(0);
                        case DIALOGTOOLS_SPACEACROSS:
                            ToolBarDisplayToolTip("Space Across", lParam);
                            return(0);
                        case DIALOGTOOLS_SPACEDOWN:
                            ToolBarDisplayToolTip("Space Down", lParam);
                            return(0);
                        case DIALOGTOOLS_ALIGNGRID:
                            ToolBarDisplayToolTip("Align To Grid", lParam);
                            return(0);
                        case DIALOGTOOLS_GRID:
                            ToolBarDisplayToolTip("Show Grid", lParam);
                            return(0);
                        case DIALOGTOOLS_LOCKCTRL:
                            ToolBarDisplayToolTip("Toggle Controls Lock", lParam);
                            return(0);
                        case DIALOGTOOLS_TOOLBAR:
                            ToolBarDisplayToolTip("Toggle Controls Toolbar", lParam);
                            return(0);
                        case DIALOGTOOLS_PROPERTIES:
                            ToolBarDisplayToolTip("Toggle Properties Page", lParam);
                            return(0);
                        case DIALOGTOOLS_TEST:
                            ToolBarDisplayToolTip("Test Dialog", lParam);
                            return(0);
                    }
                    break;
                case TBN_DROPDOWN:
                    ToolBarDisplayPopupMenu(hDiagTools, ToolBarGetNotifiedDropDownItem(lParam), hGridMenu, hWnd);
                    return(0);
            }
            break;
        case WM_PAINT:
            BeginPaint(hWnd, &FrmDiagToolsPs);
            GDIDrawHorzSep(hWnd, 0, 72, 104);
            EndPaint(hWnd, &FrmDiagToolsPs);
            break;
        case WM_DESTROY:
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Edit mode on
void SetDialogEditModeOn(void)
{
    DiagEditMode = MOUSE_MODE_EDIT;
    SetClassLong(FRMDiaghwnd, GCL_HCURSOR, (long) LoadCursor(0, IDC_CROSS));
}

// -----------------------------------------------------------------------
// Edit mode off
void SetDialogEditModeOff(void)
{
    DiagEditMode = MOUSE_MODE_SELECT;
    SetClassLong(FRMDiaghwnd, GCL_HCURSOR, (long) LoadCursor(0, IDC_ARROW));
}

// -----------------------------------------------------------------------
// Fill old selection rect datas
void FillOldRectDatas(void)
{
    OldDragRect.left = OldDiagX1;
    OldDragRect.right = OldDiagX2;
    OldDragRect.top = OldDiagY1;
    OldDragRect.bottom = OldDiagY2;
    if(OldDiagX2 < OldDiagX1)
    {
        OldDragRect.left = OldDiagX2;
        OldDragRect.right = OldDiagX1 + 1;
    }
    if(OldDiagY2 < OldDiagY1)
    {
        OldDragRect.top = OldDiagY2;
        OldDragRect.bottom = OldDiagY1 + 1;
    }
}

// -----------------------------------------------------------------------
// Fill new selection rect datas
void FillNewRectDatas(void)
{
    DragRect.left = DiagX1;
    DragRect.right = DiagX2;
    DragRect.top = DiagY1;
    DragRect.bottom = DiagY2;
    if(DiagX2 < DiagX1)
    {
        DragRect.left = DiagX2;
        DragRect.right = DiagX1 + 1;
    }
    if(DiagY2 < DiagY1)
    {
        DragRect.top = DiagY2;
        DragRect.bottom = DiagY1 + 1;
    }
}

// -----------------------------------------------------------------------
// Draw new selection rect on dialog window
void DrawNewSelectRect(HWND hWnd) {
    DiaghDC = GetDC(hWnd);
    DrawFocusRect(DiaghDC, &DragRect);
    ReleaseDC(hWnd, DiaghDC);
}

// -----------------------------------------------------------------------
// Draw old selection rect on dialog window
void DrawOldSelectRect(HWND hWnd)
{
    DiaghDC = GetDC(hWnd);
    DrawFocusRect(DiaghDC, &OldDragRect);
    ReleaseDC(hWnd, DiaghDC);
}

// -----------------------------------------------------------------------
// Create back grid
void CreateGrid(HWND hWnd)
{
    int i = 0;
    long SysBackCol = 0;

    if(BackGridDC != 0) DiscardGrid();
    BackGridDC = CreateCompatibleDC(0);
    if(BackGridDC != 0)
    {
        GridRect.left = 0;
        GridRect.top = 0;
        GridRect.right = ScreenWidth();
        GridRect.bottom = ScreenHeight();
        BitmapInfos.bmiHeader.biSize = sizeof(BitmapInfos);
        BitmapInfos.bmiHeader.biWidth = GridRect.right;
        BitmapInfos.bmiHeader.biHeight = GridRect.bottom;
        BitmapInfos.bmiHeader.biPlanes = 1;
        BitmapInfos.bmiHeader.biBitCount = 32;
        BitmapInfos.bmiHeader.biCompression = BI_RGB;
        BitmapInfos.bmiHeader.biSizeImage = 0;
        BitmapInfos.bmiHeader.biXPelsPerMeter = 0;
        BitmapInfos.bmiHeader.biYPelsPerMeter = 0;
        BitmapInfos.bmiHeader.biClrUsed = 0;
        BitmapInfos.bmiHeader.biClrImportant = 0;
        BackGridhBitmap = CreateDIBSection(BackGridDC, &BitmapInfos, DIB_RGB_COLORS, &DIBBits, 0, 0);
        if(BackGridhBitmap != 0)
        {
            OldGridObject = SelectObject(BackGridDC, BackGridhBitmap);
            GridhBrush = GetSysColorBrush(COLOR_BTNFACE);
            FillRect(BackGridDC, &GridRect, GridhBrush);
            SysBackCol = GetSysColor(COLOR_WINDOWTEXT);
            for(i = 0; i <= GridRect.right; i = i + GridXLarg)
            {
                SetPixel(BackGridDC, i, 0, SysBackCol);
            }
            for(i = 0; i <= GridRect.bottom; i = i + GridYLong)
            {
                BitBlt(BackGridDC, 0, i + GridYLong, GridRect.right, 1, BackGridDC, 0, 0, SRCCOPY);
            }
        }
    }
}

// -----------------------------------------------------------------------
// Remove back grid
void DiscardGrid(void)
{
    if(BackGridDC != 0)
    {
        SelectObject(BackGridDC, OldGridObject);
        DeleteObject(BackGridhBitmap);
        DeleteDC(BackGridDC);
    }
    BackGridDC = 0;
}

// -----------------------------------------------------------------------
// Toggle grid mode
void ToggleGridMode(void)
{
    ToolBarSetButtonChecked(hDiagTools, DIALOGTOOLS_GRID, GridMode);
    if(GridMode == 1) SetGridOn();
    else SetGridOff();
}

// -----------------------------------------------------------------------
// Set grid mode on
void SetGridOn(void)
{
    CheckMenuItem(hGridMenu, GRID_TOGGLE_ID, MF_BYPOSITION | MF_CHECKED);
    IniWriteKey("Layout", "DialogGrid", "1", MainIniFile);
    GridMode = 1;
    RefreshDialog();
}
                                
// -----------------------------------------------------------------------
// Set grid mode off
void SetGridOff(void)
{
    CheckMenuItem(hGridMenu, GRID_TOGGLE_ID, MF_BYPOSITION | MF_UNCHECKED);
    IniWriteKey("Layout", "DialogGrid", "0", MainIniFile);
    GridMode = 0;
    RefreshDialog();
}

// -----------------------------------------------------------------------
// Create a brush for read only edit boxes
void CreateSubEditBrush(long Color)
{
    LOGBRUSH SubEditBrush;

    SubEditBrush.lbStyle = BS_SOLID;
    SubEditBrush.lbHatch = 0;
    SubEditBrush.lbColor = Color;
    hSubEditBrush = CreateBrushIndirect(&SubEditBrush);
}

// -----------------------------------------------------------------------
// Grid settings
int CALLBACK FRMGridDiagProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ArgsPs;
    long TmpGridWidth = 0;
    long TmpGridHeight = 0;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, "Grid settings");
            hGridOk = CreateButton(31, 84, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            hGridCancel = CreateButton(110, 84, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            CreateLabel(14, 8, 253, 17, hwndDlg, "Grid width :", 0, 0, 0, 0);
            CreateLabel(14, 33, 253, 17, hwndDlg, "Grid height :", 0, 0, 0, 0);
            hGridWidth = CreateTextBox(76, 5, 53, 20, hwndDlg, GridXLarg, 3, 0, WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER, WS_EX_STATICEDGE);
            hGridHeight = CreateTextBox(76, 30, 53, 20, hwndDlg, GridYLong, 4, 0, WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER, WS_EX_STATICEDGE);
            hGridAlign = CreateCheckBox(15, 55, 117, 19, hwndDlg, "Align controls to grid", 5, 0, WS_TABSTOP, 0);
            if(GridAlign) CheckBoxSetState(hGridAlign, 1);
            else CheckBoxSetState(hGridAlign, 0);
            TextBoxSetMaxLen(hGridWidth, 3);
            TextBoxSetMaxLen(hGridHeight, 3);
            SetFocus(hGridWidth);
            return(0);
        case WM_COMMAND:
            if((HWND) lParam == hGridOk)
            {
                if(StringIsDecimal(ControlGetText(hGridWidth), 3) == false)
                {
                    MiscMsgBox(hwndDlg, "Invalid width value.", MB_ERROR, Requesters);
                    SetFocus(hGridWidth);
                    return(0);
                }
                if(StringIsDecimal(ControlGetText(hGridHeight), 3) == false)
                {
                    MiscMsgBox(hwndDlg, "Invalid height value.", MB_ERROR, Requesters);
                    SetFocus(hGridHeight);
                    return(0);
                }
                TmpGridWidth = ControlGetText(hGridWidth).Get_Long();
                if(TmpGridWidth < 2 || TmpGridWidth > 128)
                {
                    MiscMsgBox(hwndDlg, "Width value out of bounds (allowed values: 2 to 128).", MB_ERROR, Requesters);
                    SetFocus(hGridWidth);
                    return(0);
                }
                TmpGridHeight = ControlGetText(hGridHeight).Get_Long();
                if(TmpGridHeight < 2 || TmpGridHeight > 128)
                {
                    MiscMsgBox(hwndDlg, "Height value out of bounds (allowed values: 2 to 128).", MB_ERROR, Requesters);
                    SetFocus(hGridHeight);
                    return(0);
                }
                if(CheckBoxGetState(hGridAlign) != 0)
                {
                    GridAlign = TRUE;
                    IniWriteKey("Layout", "AlignGrid", "1", MainIniFile);
                }
                else
                {
                    GridAlign = FALSE;
                    IniWriteKey("Layout", "AlignGrid", "0", MainIniFile);
                }
                if((GridXLarg != TmpGridWidth) || (GridYLong != TmpGridHeight))
                {
                    GridXLarg = TmpGridWidth;
                    GridYLong = TmpGridHeight;
                    IniWriteKey("Layout", "GridWidth", GridXLarg, MainIniFile);
                    IniWriteKey("Layout", "GridHeight", GridYLong, MainIniFile);
                    CreateGrid(FRMDiaghwnd);
                    if(GridMode == 1) RefreshDialog();
                }
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == hGridCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &ArgsPs);
            GDIDrawHorzSep(hwndDlg, 0, 77, 190);
            EndPaint(hwndDlg, &ArgsPs);
            break;
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Save dialogs datas
void SaveDialog(void)
{
    
}

// -----------------------------------------------------------------------
// Refresh dialogs and controls
void RefreshDialog(void)
{
    ControlRepaint(FRMDiaghwnd);
    for(int i = 0; i < ControlsList.Amount(); i++)
    {
        int *CurMem = ControlsList.Get(i)->Content;
        if(CurMem[OBJECT_HWNDCHILDCHILD]) ControlRepaint((HWND) CurMem[OBJECT_HWNDCHILDCHILD]);
        if(CurMem[OBJECT_HWNDCHILD]) ControlRepaint((HWND) CurMem[OBJECT_HWNDCHILD]);
        if(CurMem[OBJECT_HWND]) ControlRepaint((HWND) CurMem[OBJECT_HWND]);
    }
}

// -----------------------------------------------------------------------
// Refresh dialogs and controls
void ResizeFromToolbar(HWND Dialog, HWND Toolbar, int PosX, int PosY, int NbrColumns, int NbrRows, int Tab_Buttons[])
{
    int X_Size = 0;

    for(int i = 0; i < NbrColumns; i++)
    {
        X_Size += ToolBarGetButtonIndexXSize(Toolbar, Tab_Buttons[i]);
    }
    int Y_Size = ToolBarGetButtonYSize(Toolbar) * NbrRows;
    MoveWindow(Dialog, PosX, PosY, X_Size + GetSystemMetrics(SM_CXFIXEDFRAME) + GetSystemMetrics(SM_CXEDGE) + 1, Y_Size + GetSystemMetrics(SM_CYSMCAPTION) + GetSystemMetrics(SM_CYFIXEDFRAME) + GetSystemMetrics(SM_CYEDGE) + 1, 1);
    MoveWindow(Toolbar, 0, 0, X_Size, Y_Size, 1);
}
