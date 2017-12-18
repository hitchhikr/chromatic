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
// ChromaticLib.cpp: implementation of the Chromatic library
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "WALib.h"
#include "WADock.h"

// -----------------------------------------------------------------------
// Variables
CDockingWindow *cDock;
HFONT WASerifFont;
HFONT WASerifFont10;
HFONT WACourrierNewFont9;
HFONT WAMarlettFont9;
HFONT WAArialFont8;
HBRUSH WABgBrush;
long WABgColor;
long WATxtColor;
long ToolTipFont;
long _Btn_StaticEdge = 0;
HWND LastDialog;
CAppModule _Module;

HICON WALocalhIcon;
HINSTANCE WALocalhInst;
WNDCLASSEX WAMyClass;
void (CALLBACK *WACurrentInitProc)(HWND hwnd);
void (CALLBACK *WACurrentChildInitProc)(HWND hwnd);
LPSPLITTERDAT WACurrentSplitterStruct;
long WADragListMsg;
DLGTEMPLATE2 WAMyTemplate;
long FirstDraggingIndex = 0;
HCURSOR OldDragCursor = 0;
HCURSOR OldGripCursor = 0;
HINSTANCE RichEditDll;
CStr RichedClass;
DLGPROC DialogTempProc;
CStr DialogTempTitle;
long DialogTempWidth;
long DialogTempHeight;
long DialogTempExtra;
CList <HGLOBAL> Memory_Blocks;
// Garbage collector
CList <char *> Garbage_Collector_List;
CHOOSECOLOR MyColor;
COLORREF MyCustomColors[17];
LOGFONT MyLogFont;
CHOOSEFONT MyFont;
OPENFILENAME MyOpenFName;
OPENFILENAME MySaveFName;
HGLOBAL hMemFont;
int _OnWindows2K;
int _OnOlderWindows;

// -----------------------------------------------------------------------
// Functions
void CALLBACK StringFlushGarbageCollector(void);
long CALLBACK WASetDialogClass(HINSTANCE hInst);
long CALLBACK WASetSplashClass(HINSTANCE hInst);
long CALLBACK WASetMDIClass(HINSTANCE hInst);
long CALLBACK WASetMDIChildClass(HINSTANCE hInst);
long CALLBACK WASetContainerClass(HINSTANCE hInst);
long CALLBACK WASetSplitterClass(HINSTANCE hInst);
long CALLBACK WASetVertSplitBarClass(HINSTANCE hInst, long vCursor);
long CALLBACK WASetHorzSplitBarClass(HINSTANCE hInst, long hCursor);
long CALLBACK WASetColorBoxClass(HINSTANCE hInst);
long CALLBACK WASetDumpBoxClass(HINSTANCE hInst);
long CALLBACK WASetGripBoxClass(HINSTANCE hInst);
long CALLBACK WASetGripBoxSquareClass(HINSTANCE hInst);
void CALLBACK DockingBoxReadState(CStr DockName, int &State, RECT &Rect, int &Size, int DefaultState, int DefaultSizeX, int DefaultSizeY, CStr IniFile);
void CALLBACK DockingBoxSaveState(HWND DockhWnd, CStr DockName, int Visible, CStr IniFile);
LRESULT CALLBACK WADefWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefMDIClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefMDIChildClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefContainerClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefSplitterClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefVertSplitBarClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefHorzSplitBarClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefColorBoxClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefDumpBoxClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefGripBoxClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WADefSplashClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WAHexHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK FRMStockModalDlgEmptyProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK FRMStockModalDlgStdCloseProc(HWND hwndDlg, UINT uMsg, WPARAM wParam , LPARAM lParam);
int CALLBACK FRMStockModalDlgStdOkCancelProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK FRMStockModalDlgStdWizardProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DefaultDockingProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK EnumDockChilds(HWND hWnd, long lParam);

// -----------------------------------------------------------------------
// Init all
int CALLBACK WidgetInit(HINSTANCE hInst, HICON AppIcon, long vSplitCursor, long hSplitCursor)
{
    INITCOMMONCONTROLSEX MyCommonStruct;
    int ReturnValue = 0;

    int OSType = MiscGetOSClass();
    if((OSType & 2) || (OSType & 4)) _OnWindows2K = 1;
    if(OSType & 1) _OnOlderWindows = 1;                 // NT4
    if(OSType == 0) _OnOlderWindows = 1;                // 9X/ME
    _Btn_StaticEdge = WS_EX_STATICEDGE; 
    if(OSType & 4) _Btn_StaticEdge = 0;

    MyCommonStruct.dwSize = sizeof(MyCommonStruct);
    MyCommonStruct.dwICC = ICC_LISTVIEW_CLASSES | ICC_TREEVIEW_CLASSES | ICC_WIN95_CLASSES |
                           ICC_BAR_CLASSES | ICC_TAB_CLASSES | ICC_PROGRESS_CLASS | ICC_COOL_CLASSES |
                           ICC_USEREX_CLASSES | ICC_INTERNET_CLASSES | ICC_PAGESCROLLER_CLASS | ICC_DATE_CLASSES |
                           ICC_HOTKEY_CLASS | ICC_UPDOWN_CLASS | ICC_ANIMATE_CLASS;
    // Try new commoncontrols
    ReturnValue = INIT_OK;
    if(InitCommonControlsEx(&MyCommonStruct) == 0)
    {
        ReturnValue = ReturnValue | INIT_NOEXCONTROLS;
        InitCommonControls();
    }
    if(WASerifFont == 0) WASerifFont = CreateFont(-8, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "MS Sans Serif");
    if(WASerifFont10 == 0) WASerifFont10 = CreateFont(-10, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "MS Sans Serif");
    if(WACourrierNewFont9 == 0) WACourrierNewFont9 = GDIObtainFont("Courier New", 9, 0, 0, 0);
    if(WAMarlettFont9 == 0) WAMarlettFont9 = GDIObtainFont("Marlett", 9, 0, 0, 0);
    if(WAArialFont8 == 0) WAArialFont8 = GDIObtainFont("Arial", 8, 0, 0, 0);

    WABgBrush = GetSysColorBrush(COLOR_WINDOW);
    WABgColor = GetSysColor(COLOR_WINDOW);
    WATxtColor = GetSysColor(COLOR_WINDOWTEXT);
    WALocalhInst = hInst;
    WALocalhIcon = AppIcon;
    WASetMDIClass(hInst);
    WASetDialogClass(hInst);
    WASetSplashClass(hInst);
    WASetMDIChildClass(hInst);
    WASetContainerClass(hInst);
    WASetSplitterClass(hInst);
    WASetVertSplitBarClass(hInst, vSplitCursor);
    WASetHorzSplitBarClass(hInst, hSplitCursor);
    WASetColorBoxClass(hInst);
    WASetDumpBoxClass(hInst);
    WASetGripBoxClass(hInst);
    Memory_Blocks.Erase();
    // Register the draglist message
    WADragListMsg = RegisterWindowMessage(DRAGLISTMSGSTRING);
    // Load RichEdit 2.0
    RichEditDll = LoadLibrary("riched20.dll");
    RichedClass = "RichEdit20a";
    if(RichEditDll == 0)
    {
        RichedClass = "RICHEDIT";
        RichEditDll = LoadLibrary("riched32.dll");
        // Mark it as 1.0
        ReturnValue = ReturnValue | INIT_RICHEDCONTROL10;
        // Can't find any richedit
        if(RichEditDll == 0) ReturnValue = ReturnValue | INIT_NORICHEDCONTROL;
    }

    // Init it now so that we can use ATL/WTL classes
    _Module.Init(NULL, hInst);
    // HACK to init the cdock class
    CDockingWindow *m_dock = new CDockingWindow;
    cDock = m_dock;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Reset all
void CALLBACK WidgetUnInit(HINSTANCE hInst)
{
    StringFlushGarbageCollector();
    if(RichEditDll != 0) FreeLibrary(RichEditDll);
    if(WACourrierNewFont9 != 0) DeleteObject(WACourrierNewFont9);
    if(WASerifFont10 != 0) DeleteObject(WASerifFont10);
    if(WASerifFont != 0) DeleteObject(WASerifFont);
    // Unregister the classes
    UnregisterClass("GripBoxClass", hInst);
    UnregisterClass("DumpBoxClass", hInst);
    UnregisterClass("ColorBoxClass", hInst);
    UnregisterClass("WAHorzSplitBarClass", hInst);
    UnregisterClass("WAVertSplitBarClass", hInst);
    UnregisterClass("SplitterClass", hInst);
    UnregisterClass("WAContainerClass", hInst);
    UnregisterClass("WAMDIChildDialogClass", hInst);
    UnregisterClass("WASplashDialogClass", hInst);
    UnregisterClass("DialogClass", hInst);
    UnregisterClass("WAMDIDialogClass", hInst);
    // Free collected memory and list
    Memory_Blocks.Erase();
    _Module.Term();
 }

// -----------------------------------------------------------------------
// Splash controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a splash form
HWND CALLBACK CreateSplashDialog(long DLeft, long DTop, long DWidth, long DHeight, CStr DTitle,
                                 HWND hParent, HMENU hMenu, HICON hIcon, void (CALLBACK *InitProc)(HWND hwnd),
                                 WNDPROC WindowProc, long WExStyle, long WStyle, long ShowType)
{
    HWND ReturnValue = 0;

    if(DLeft == -1) DLeft = (GetSystemMetrics(SM_CXSCREEN) - DWidth) / 2;
    if(DTop == -1) DTop = (GetSystemMetrics(SM_CYSCREEN) - DHeight) / 2;
    WACurrentInitProc = InitProc;
    ReturnValue = CreateWindowEx(WExStyle, "WASplashDialogClass", DTitle.Get_String(), WS_POPUP | WS_CLIPSIBLINGS + WStyle, DLeft, DTop, DWidth, DHeight, hParent, hMenu, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlBringToBottom(ReturnValue);
    UpdateWindow(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Dialogs controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a modal dialog form from a template
long CALLBACK CreateStockModalDialog(long TemplateNumber, long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, CStr DTitle, DLGPROC WindowProc, long Centered, long ExtraDatas)
{
    DialogTempProc = WindowProc;
    DialogTempTitle = DTitle;
    DialogTempWidth = DWidth;
    DialogTempHeight = DHeight;
    DialogTempExtra = ExtraDatas;
    switch(TemplateNumber)
    {
        case MODALDLG_STOCK_EMPTY:
            return(CreateModalDialog(DLeft, DTop, DWidth, DHeight, hParent, &FRMStockModalDlgEmptyProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, Centered));
        case MODALDLG_STOCK_STDCLOSE:
            return(CreateModalDialog(DLeft, DTop, DWidth, DHeight, hParent, &FRMStockModalDlgStdCloseProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, Centered));
        case MODALDLG_STOCK_STDOKCANCEL:
            return(CreateModalDialog(DLeft, DTop, DWidth, DHeight, hParent, &FRMStockModalDlgStdOkCancelProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, Centered));
        case MODALDLG_STOCK_WIZARD:
            return(CreateModalDialog(DLeft, DTop, DWidth, DHeight, hParent, &FRMStockModalDlgStdWizardProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, Centered));
        default:
            return(0);
    }
}

// -----------------------------------------------------------------------
// Create a modal dialog form
long CALLBACK CreateModalDialog(long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, DLGPROC WindowProc, long ExtraStyle, long Centered)
{
    long BaseDialogX = 0;
    long BaseDialogY = 0;

    if(DLeft == -1) DLeft = (GetSystemMetrics(SM_CXSCREEN) - DWidth) / 2;
    if(DTop == -1) DTop = (GetSystemMetrics(SM_CYSCREEN) - DHeight) / 2;
    BaseDialogX = DialogGetXUnit();
    BaseDialogY = DialogGetYUnit();
    WAMyTemplate.Diag.dwExtendedStyle = 0;
    WAMyTemplate.Diag.x = (WORD) DialogXPixelToUnit(DLeft, BaseDialogX);
    WAMyTemplate.Diag.y = (WORD) DialogYPixelToUnit(DTop, BaseDialogY);
    WAMyTemplate.Diag.cx = (WORD) DialogXPixelToUnit(DWidth, BaseDialogX);
    WAMyTemplate.Diag.cy = (WORD) DialogYPixelToUnit(DHeight, BaseDialogY);
    WAMyTemplate.Diag.style = 4 | WS_VISIBLE | DS_3DLOOK | DS_NOIDLEMSG | DS_SETFOREGROUND | DS_MODALFRAME | ExtraStyle;
    if(Centered == 1) WAMyTemplate.Diag.style = WAMyTemplate.Diag.style | DS_CENTER;
    WAMyTemplate.Diag.cdit = 0;
    return(DialogBoxIndirectParam(WALocalhInst, (LPCDLGTEMPLATE) &WAMyTemplate, hParent, WindowProc, 0));
}

// -----------------------------------------------------------------------
// Create a dialog form
HWND CALLBACK CreateNonModalDialog(long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, HMENU hMenu, HICON hIcon, CStr DTitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long WExStyle, long WStyle, long ShowType)
{
    HWND ReturnValue = 0;

    if(DLeft == -1) DLeft = (GetSystemMetrics(SM_CXSCREEN) - DWidth) / 2;
    if(DTop == -1) DTop = (GetSystemMetrics(SM_CYSCREEN) - DHeight) / 2;
    WACurrentInitProc = InitProc;
    // Correct dialog dimensions with an hardcoded value (from windows standard)
    DHeight = DHeight + (GetSystemMetrics(SM_CYCAPTION) - 19);
    ReturnValue = CreateWindowEx(WExStyle, "DialogClass", DTitle.Get_String(), WS_CLIPSIBLINGS + WStyle, DLeft, DTop, DWidth, DHeight, hParent, hMenu, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    DialogSetIcon(ReturnValue, hIcon);
    ShowWindow(ReturnValue, ShowType);
    UpdateWindow(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set a dialog icon
LRESULT CALLBACK DialogSetIcon(HWND hWnd, HICON hIcon)
{
    return(SendMessage(hWnd, WM_SETICON, ICON_SMALL, (long) hIcon));
}

// -----------------------------------------------------------------------
// Retrieve X unit for dialog based resources
long CALLBACK DialogGetXUnit(void)
{
    long ReturnValue = 0;

    ReturnValue = GetDialogBaseUnits() & 0xFFFF;
    if(ReturnValue == 0) ReturnValue = 1;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve Y unit for dialog based resources
long CALLBACK DialogGetYUnit(void)
{
    long ReturnValue = 0;

    ReturnValue = (short) ((GetDialogBaseUnits() & 0xFFFF0000) / 0x10000);
    if(ReturnValue == 0) ReturnValue = 1;
    return(ReturnValue);
}
    
// -----------------------------------------------------------------------
// Convert a resource object coordinate from pixels to dialogs unit on X axis
long CALLBACK DialogXPixelToUnit(long XCoord, long XUnit)
{
    long ReturnValue = 0;

    if(XUnit != 0) ReturnValue = (XCoord * 4) / XUnit;
    else ReturnValue = (XCoord * 4);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a resource object coordinate from pixels to dialogs unit on Y axis
long CALLBACK DialogYPixelToUnit(long YCoord, long YUnit)
{
    long ReturnValue = 0;

    if(YUnit != 0) ReturnValue = (YCoord * 8) / YUnit;
    else ReturnValue = (YCoord * 8);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a resource object coordinate from dialogs unit to pixels on X axis
long CALLBACK DialogXUnitToPixel(long XCoord, long XUnit)
{
    return((XCoord * XUnit) / 4);
}

// -----------------------------------------------------------------------
// Convert a resource object coordinate from dialogs unit to pixels on Y axis
long CALLBACK DialogYUnitToPixel(long YCoord, long YUnit)
{
    return((YCoord * YUnit) / 8);
}

// -----------------------------------------------------------------------
// Container controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a container control
HWND CALLBACK CreateContainer(long SPLeft, long SPTop, long SPWidth, long SPHeight, long CtrlID,
                                HWND hParent, CStr Name, WNDPROC WindowProc)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, SPLeft, SPTop, SPWidth, SPHeight);
    ReturnValue = CreateWindowEx(0, "WAContainerClass", Name.Get_String(), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, SPLeft, SPTop, SPWidth, SPHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Splitters controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a splitter control
HWND CALLBACK CreateSplitter(long SPLeft, long SPTop, long SPDim, long CtrlID,
                               LPSPLITTERDAT SplitterStruct, WNDPROC WindowProc)
{
    HWND ReturnValue = 0;
    long SPWidth = 0;
    long SPHeight = 0;

    switch(SplitterStruct->Orientation)
    {
        case SPLITTER_HORZ:
            SPWidth = SPDim;
            SPHeight = SplitterStruct->StartupPos;
            ControlBound(SplitterStruct->hParent, SPLeft, SPTop, SPWidth, SPHeight);
            break;
        case SPLITTER_VERT:
            SPWidth = SplitterStruct->StartupPos;
            SPHeight = SPDim;
            ControlBound(SplitterStruct->hParent, SPLeft, SPTop, SPWidth, SPHeight);
            break;
    }
    WACurrentSplitterStruct = SplitterStruct;
    ReturnValue = CreateWindowEx(0, "SplitterClass", "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, SPLeft, SPTop, SPWidth, SPHeight, SplitterStruct->hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    if(WindowProc != 0) SplitterStruct->OldWndProc = SetWindowLong(ReturnValue, GWL_WNDPROC, (long) WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Create a splitbar control
HWND CALLBACK CreateSplitBar(long SPLeft, long SPTop, long SPWidth, long SPHeight,
                               HWND hParent, long CtrlID, LPSPLITTERDAT SplitStruct)
{
    HWND ReturnValue = 0;
    SPLITTERDAT TmpSplitBarDatC;

    RtlMoveMemory(&TmpSplitBarDatC, SplitStruct, sizeof(TmpSplitBarDatC));
    ControlBound(hParent, SPLeft, SPTop, SPWidth, SPHeight);
    switch(TmpSplitBarDatC.Orientation)
    {
        case SPLITTER_HORZ:
            ReturnValue = CreateWindowEx(0, "WAHorzSplitBarClass", "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, SPLeft, SPTop, SPWidth, SPHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
            break;
        case SPLITTER_VERT:
            ReturnValue = CreateWindowEx(0, "WAVertSplitBarClass", "", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, SPLeft, SPTop, SPWidth, SPHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
            break;
    }
    if(ReturnValue == 0) return(0);
    SetWindowLong(ReturnValue, GWL_USERDATA, (long) SplitStruct);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// MDIDialogs controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a MDIDialog form
HWND CALLBACK CreateMDIDialog(long MDILeft, long MDITop, long MDIWidth, long MDIHeight,
                                HWND hParent, HMENU hMenu, HICON hIcon, CStr MDITitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long ExtraStyle, long ShowType)
{
    HWND ReturnValue = 0;

    if(MDILeft == -1) MDILeft = (GetSystemMetrics(SM_CXSCREEN) - MDIWidth) / 2;
    if(MDITop == -1) MDITop = (GetSystemMetrics(SM_CYSCREEN) - MDIHeight) / 2;
    WACurrentInitProc = InitProc;
    ReturnValue = CreateWindowEx(WS_EX_LEFT, "WAMDIDialogClass", MDITitle.Get_String(), WS_THICKFRAME | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_CAPTION | ExtraStyle, MDILeft, MDITop, MDIWidth, MDIHeight, 0, (HMENU) hMenu, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    DialogSetIcon(ReturnValue, hIcon);
    ShowWindow(ReturnValue, ShowType);
    UpdateWindow(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// MDI Child Dialogs controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a MDI child dialog form
HWND CALLBACK CreateMDIChildDialog(HWND hParent, CStr CTitle, HICON hIcon,
                                     void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WindowProc, long ExtraStyle, long ZeroPos, CStr CustomClass)
{
    HWND ReturnValue = 0;
    MDICREATESTRUCT NewMDIChild;

    memset(&NewMDIChild, 0, sizeof(NewMDIChild));
    if(CustomClass.Len() != 0)
    {
        NewMDIChild.szClass = CustomClass.Get_String();
    }
    else
    {
        NewMDIChild.szClass = "WAMDIChildDialogClass";
        WACurrentChildInitProc = InitProc;
    }
    NewMDIChild.szTitle = CTitle.Get_String();
    NewMDIChild.hOwner = WALocalhInst;
    if(ZeroPos == 0)
    {
        NewMDIChild.x = CW_USEDEFAULT;
        NewMDIChild.y = CW_USEDEFAULT;
    }
    else
    {
        NewMDIChild.x = 0;
        NewMDIChild.y = 0;
    }
    NewMDIChild.cx = CW_USEDEFAULT;
    NewMDIChild.cy = CW_USEDEFAULT;
    NewMDIChild.style = ExtraStyle | 1;
    NewMDIChild.lParam = 0;
    ReturnValue = (HWND) SendMessage(hParent, WM_MDICREATE, 0, (long) &NewMDIChild);
    if(ReturnValue == 0) return(0);
    // Change default windowproc (no subclass here)
    if(CustomClass.Len() == 0) SetWindowLong(ReturnValue, GWL_WNDPROC, (long) WindowProc);
    DialogSetIcon(ReturnValue, hIcon);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Toolbar controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a toolbar control
HWND CALLBACK CreateToolBar(long TBLeft, long TBTop, long TBWidth, long TBHeight,
                            HWND hParent, HIMAGELIST hImageList, long CtrlID, long StdBitmapsType,
                            WNDPROC WindowProc, long ExtraStyle, long ExtendedStyle)
{
    HWND ReturnValue = 0;
    TBADDBITMAP BitmapToAdd;

    memset(&BitmapToAdd, 0, sizeof(BitmapToAdd));
    ControlBound(hParent, TBLeft, TBTop, TBWidth, TBHeight);
    ReturnValue = CreateWindowEx(0, "ToolBarWindow32", "", WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_CHILD | WS_VISIBLE | ExtraStyle, TBLeft, TBTop, TBWidth, TBHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    SendMessage(ReturnValue, TB_BUTTONSTRUCTSIZE, sizeof(TBBUTTON), (long) 0);
    if(StdBitmapsType != -1)
    {
        BitmapToAdd.hInst = HINST_COMMCTRL;
        BitmapToAdd.nID = StdBitmapsType;
        SendMessage(ReturnValue, TB_ADDBITMAP, 1, (long) &BitmapToAdd);
    }
    else if(hImageList != 0)
    {
        SendMessage(ReturnValue, TB_SETIMAGELIST, 0, (long) hImageList);
    }
    if(ExtendedStyle != 0) SendMessage(ReturnValue, TB_SETEXTENDEDSTYLE, 0, (long) ExtendedStyle);
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add a button to a toolbar control
long CALLBACK ToolBarAddButton(HWND hToolbar, CStr ButtonText, long TBButtonID, long ImgIndex,
                                 long ButtonStyle, long ButtonState, long NoImage)
{
    long ReturnValue = 0;
    TBADDBITMAP BitmapToAdd;
    TBBUTTON ButtonToAdd;

    memset(&BitmapToAdd, 0, sizeof(BitmapToAdd));
    memset(&ButtonToAdd, 0, sizeof(ButtonToAdd));
    ButtonToAdd.idCommand = TBButtonID;
    ButtonToAdd.fsState = (BYTE) ButtonState;
    ButtonToAdd.fsStyle = (BYTE) ButtonStyle;
    ButtonToAdd.iBitmap = ImgIndex;
    if(ButtonText.Len() == 0) ButtonToAdd.iString = -1;
    else ButtonToAdd.iString = SendMessage(hToolbar, TB_ADDSTRING, 0, (long) ButtonText.Get_String());
    ButtonToAdd.dwData = 0;
    ReturnValue = SendMessage(hToolbar, TB_ADDBUTTONS, 1, (long) &ButtonToAdd);
    if(NoImage != 0)
    {
        if(ButtonText.Len() != 0)
        {
            SendMessage(hToolbar, TB_CHANGEBITMAP, TBButtonID, I_IMAGENONE);
            SendMessage(hToolbar, TB_SETDRAWTEXTFLAGS, DT_CENTER, DT_CENTER);
        }
    }
    else
    {
        SendMessage(hToolbar, TB_SETDRAWTEXTFLAGS, DT_LEFT, DT_LEFT);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve a button from a toolbar control
long CALLBACK ToolBarGetButton(HWND hToolbar, long TBButtonID, LPTBBUTTON ButtonStruct)
{
    return(SendMessage(hToolbar, TB_GETBUTTON, TBButtonID, (long) ButtonStruct));
}

// -----------------------------------------------------------------------
// Retrieve the X size of a button from a toolbar control
long CALLBACK ToolBarGetButtonXSize(HWND hToolbar)
{
    return(SendMessage(hToolbar, TB_GETBUTTONSIZE, 0, (long) 0) & 0xFFFF);
}

// -----------------------------------------------------------------------
// Retrieve the Y size of a button from a toolbar control
long CALLBACK ToolBarGetButtonYSize(HWND hToolbar)
{
    return((short) ((SendMessage(hToolbar, TB_GETBUTTONSIZE, 0, (long) 0) & 0xFFFF0000) / 0x10000));
}

// -----------------------------------------------------------------------
// Retrieve number of buttons of a toolbar control
long CALLBACK ToolBarGetButtonsCount(HWND hToolbar)
{
    return(SendMessage(hToolbar, TB_BUTTONCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve number of rows of a toolbar control
long CALLBACK ToolBarGetRowsCount(HWND hToolbar)
{
    return(SendMessage(hToolbar, TB_GETROWS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the imagelist of a toolbar control
HIMAGELIST CALLBACK ToolBarGetImagelist(HWND hToolbar)
{
    return((HIMAGELIST) SendMessage(hToolbar, TB_GETIMAGELIST, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Add a separator button to a toolbar control
long CALLBACK ToolBarAddSeparator(HWND hToolbar, long TBButtonID)
{
    TBBUTTON ButtonToAdd;

    memset(&ButtonToAdd, 0, sizeof(ButtonToAdd));
    ButtonToAdd.iBitmap = 0;
    ButtonToAdd.idCommand = TBButtonID;
    ButtonToAdd.fsState = 0;
    ButtonToAdd.fsStyle = TBSTYLE_SEP;
    ButtonToAdd.iString = 0;
    return(SendMessage(hToolbar, TB_ADDBUTTONS, 1, (long) &ButtonToAdd));
}

// -----------------------------------------------------------------------
// Retrieve button number on a tooltip demand of a toolbar control
long CALLBACK ToolBarGetNotifiedToolTip(long lParam)
{
    long ReturnValue = 0;
    NMHDR HeaderNotify;

    ReturnValue = -1;
    RtlMoveMemory(&HeaderNotify, (void *) lParam, sizeof(HeaderNotify));
    if(HeaderNotify.code == TTN_NEEDTEXT) ReturnValue = HeaderNotify.idFrom;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve button number on a dropdown demand of a toolbar control
long CALLBACK ToolBarGetNotifiedDropDownItem(long lParam)
{
    NMTOOLBAR HeaderNotify;

    RtlMoveMemory(&HeaderNotify, (void *) lParam, sizeof(HeaderNotify));
    return(HeaderNotify.iItem);
}

// -----------------------------------------------------------------------
// Proceed a standard tooltip demand of a toolbar control
void CALLBACK ToolBarDisplayToolTip(CStr TextToShow, long lParam)
{
    TOOLTIPTEXT ToolTipNotify;
    long TTLen = 0;

    RtlMoveMemory(&ToolTipNotify, (void *) lParam, sizeof(ToolTipNotify));
    memset(&ToolTipNotify.szText, 0, 80);
    TTLen = TextToShow.Len();
    if(TTLen > 79) TTLen = 79;
    RtlMoveMemory(&ToolTipNotify.szText, TextToShow.Get_String(), TTLen);
    RtlMoveMemory((void *)lParam, &ToolTipNotify, sizeof(ToolTipNotify));
}

// -----------------------------------------------------------------------
// Resize a toolbar according to it's parent control
long CALLBACK ToolBarResize(HWND hToolbar)
{
    return(SendMessage(hToolbar, TB_AUTOSIZE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set a toolbar button pressed state
long CALLBACK ToolBarSetButtonPressed(HWND hToolbar, long TBButtonID, long Pressed)
{
    return(SendMessage(hToolbar, TB_PRESSBUTTON, TBButtonID, Pressed));
}

// -----------------------------------------------------------------------
// Set a toolbar button checked state
long CALLBACK ToolBarSetButtonChecked(HWND hToolbar, long TBButtonID, long Checked)
{
    return(SendMessage(hToolbar, TB_CHECKBUTTON, TBButtonID, Checked));
}

// -----------------------------------------------------------------------
// Set a toolbar button enable state
long CALLBACK ToolBarSetButtonEnabled(HWND hToolbar, long TBButtonID, long EnableState)
{
    return(SendMessage(hToolbar, TB_ENABLEBUTTON, TBButtonID, EnableState));
}

// -----------------------------------------------------------------------
// Is a toolbar button pressed ?
long CALLBACK ToolBarIsButtonPressed(HWND hToolbar, long TBButtonID)
{
    return(SendMessage(hToolbar, TB_ISBUTTONPRESSED, TBButtonID, (long) 0));
}

// -----------------------------------------------------------------------
// Is a toolbar button checked ?
long CALLBACK ToolBarIsButtonChecked(HWND hToolbar, long TBButtonID)
{
    return(SendMessage(hToolbar, TB_ISBUTTONCHECKED, TBButtonID, (long) 0));
}

// -----------------------------------------------------------------------
// Display a menu below a toolbar button
long CALLBACK ToolBarDisplayPopupMenu(HWND hToolbar, long TBButtonID, HMENU hMenu, HWND hParent)
{
    RECT rc;
    TPMPARAMS tpm;

    memset(&rc, 0, sizeof(rc));
    memset(&tpm, 0, sizeof(tpm));
    SendMessage(hToolbar, TB_GETRECT, TBButtonID, (long) &rc);
    MapWindowPoints(hToolbar, HWND_DESKTOP, (LPPOINT) &rc, 2);
    tpm.cbSize = sizeof(tpm);
    rc.left = rc.left - 1;
    if(rc.top < 0) rc.top = 0;
    if(rc.left < 0) rc.left = 0;
    tpm.rcExclude.top = rc.top;
    tpm.rcExclude.left = rc.left;
    tpm.rcExclude.bottom = rc.bottom;
    tpm.rcExclude.right = rc.right;
    return(TrackPopupMenuEx(hMenu, TPM_LEFTALIGN | TPM_LEFTBUTTON | TPM_VERTICAL, rc.left, rc.bottom, hParent, &tpm));
}

// -----------------------------------------------------------------------
// Retrieve the Y size of a toolbar
long CALLBACK ToolBarGetYSize(HWND hToolbar)
{
    RECT TBRct;

    GetWindowRect(hToolbar, &TBRct);
    return(TBRct.bottom - TBRct.top);
}

// -----------------------------------------------------------------------
// Retrieve the X size of a toolbar
long CALLBACK ToolBarGetXSize(HWND hToolbar)
{
    RECT TBRct;

    GetWindowRect(hToolbar, &TBRct);
    return(TBRct.right - TBRct.left);
}

// -----------------------------------------------------------------------
// Retrieve the X position of a toolbar button
long CALLBACK ToolBarGetButtonXPosition(HWND hToolbar, long TBButtonID)
{
    RECT TBRct;

    SendMessage(hToolbar, TB_GETITEMRECT, TBButtonID, (long) &TBRct);
    return(TBRct.left);
}

// -----------------------------------------------------------------------
// Retrieve the X/Y size of a toolbar
long CALLBACK ToolBarGetXYSize(HWND hToolbar, LPSIZE TBSize)
{
    return(SendMessage(hToolbar, TB_GETMAXSIZE, 0, (long) TBSize));
}

// -----------------------------------------------------------------------
// Retrieve the real position of a toolbar button
long CALLBACK ToolBarGetRealPos(HWND hToolbar, LPPOINT TBPosition)
{
    long ReturnValue = 0;
    RECT TBRct;

    ReturnValue = GetWindowRect(hToolbar, &TBRct);
    TBPosition->x = TBRct.left;
    TBPosition->y = TBRct.top;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the X/Y position of a toolbar button (relative to toolbar)
long CALLBACK ToolBarGetButtonXYPos(HWND hToolbar, long TBButtonID, LPPOINT TBPosition)
{
    long ReturnValue = 0;
    RECT TBRct;

    ReturnValue = SendMessage(hToolbar, TB_GETITEMRECT, TBButtonID, (long) &TBRct);
    TBPosition->x = TBRct.left;
    TBPosition->y = TBRct.top;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the X size of a toolbar button by button id
long CALLBACK ToolBarGetButtonIndexXSize(HWND hToolbar, long TBButtonID)
{
    RECT TBRct;
    SendMessage(hToolbar, TB_GETRECT, TBButtonID, (long) &TBRct);
    return(TBRct.right - TBRct.left);
}

// -----------------------------------------------------------------------
// Retrieve the X padding of a toolbar
long CALLBACK ToolBarGetXPadding(HWND hToolbar)
{
    return((short) ((SendMessage(hToolbar, TB_GETPADDING, 0, (long) 0) & 0xFFFF0000) / 0x10000));
}

// -----------------------------------------------------------------------
// Retrieve the Y padding of a toolbar
long CALLBACK ToolBarGetYPadding(HWND hToolbar)
{
    return(SendMessage(hToolbar, TB_GETPADDING, 0, (long) 0) & 0xFFFF);
}

// -----------------------------------------------------------------------
// Retrieve button number on a dropdown demand of a toolbar control
long CALLBACK ToolBarGetNotifiedHotItem(long lParam)
{
    long ReturnValue = 0;
    NMTBHOTITEM HeaderNotify;

    ReturnValue = -1;
    RtlMoveMemory(&HeaderNotify, (void *) lParam, sizeof(HeaderNotify));
    if(HeaderNotify.dwFlags != 33) ReturnValue = HeaderNotify.idNew;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Remove a button from a toolbar control
long CALLBACK ToolBarRemoveButton(HWND hToolbar, long ButtonID)
{
    return(SendMessage(hToolbar, TB_DELETEBUTTON, (WPARAM) ButtonID, 0));
}

// -----------------------------------------------------------------------
// Set the bitmap used for a button of a toolbar control
long CALLBACK ToolBarSetButtonBitmap(HWND hToolbar, long TBButtonID, long BtnIndex)
{
    return(SendMessage(hToolbar, TB_CHANGEBITMAP, TBButtonID, BtnIndex));
}

// -----------------------------------------------------------------------
// Image lists
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create an imagelist control
HIMAGELIST CALLBACK CreateImageList(long IconWidth, long IconHeight, long MaxIcon)
{
    return(ImageList_Create(IconWidth, IconHeight, ILC_MASK | ILC_COLOR16, 0, MaxIcon));
}

// -----------------------------------------------------------------------
// Destroy an imagelist control
void CALLBACK ImageListDestroy(HIMAGELIST hImageList)
{
    ImageList_Destroy(hImageList);
}

// -----------------------------------------------------------------------
// Add an icon to an imagelist control
long CALLBACK ImageListAddIcon(HIMAGELIST hImageList, long IconNumber)
{
    return(ImageList_AddIcon(hImageList, LoadIcon(WALocalhInst, MAKEINTRESOURCE(IconNumber))));
}

// -----------------------------------------------------------------------
// Retrieve the number of icons in an imagelist control
long CALLBACK ImageListGetIconsCount(HIMAGELIST hImageList)
{
    return(ImageList_GetImageCount(hImageList));
}

// -----------------------------------------------------------------------
// Retrieve an icon from an imagelist control
HICON CALLBACK ImageListGetIcon(HIMAGELIST hImageList, long IconIdx)
{
    return(ImageList_GetIcon(hImageList, IconIdx, ILD_NORMAL));
}

// -----------------------------------------------------------------------
// Systab controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a systab control
HWND CALLBACK CreateSysTab(long FLeft, long FTop, long FWidth, long FHeight,
                             HWND hParent, long CtrlID, WNDPROC WindowProc, HIMAGELIST hImageList, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, FLeft, FTop, FWidth, FHeight);
    ReturnValue = CreateWindowEx(0, "SysTabControl32", "", WS_VISIBLE | WS_CHILD | ExtraStyle, FLeft, FTop, FWidth, FHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    if(hImageList != 0) SendMessage(ReturnValue, TCM_SETIMAGELIST, 0, (long) hImageList);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add a tab to a systab control
long CALLBACK SysTabAddItem(HWND htab, CStr TabText, long TabIndex, long ImgIndex)
{
    TC_ITEM TabItem;

    memset(&TabItem, 0, sizeof(TabItem));
    TabItem.iImage = ImgIndex;
    TabItem.mask = TCIF_IMAGE;
    if(TabText.Len() != 0)
    {
        TabItem.mask = TabItem.mask | TCIF_TEXT;
        TabItem.pszText = TabText.Get_String();
        TabItem.cchTextMax = TabText.Len();
    }
    return(SendMessage(htab, TCM_INSERTITEM, TabIndex, (long) &TabItem));
}

// -----------------------------------------------------------------------
// Retrieve the current selection of a tab control
long CALLBACK SysTabGetCurrentItem(HWND htab)
{
    return(SendMessage(htab, TCM_GETCURSEL, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the current imagelist associated of a tab control
HIMAGELIST CALLBACK SysTabGetImagelist(HWND htab)
{
    return((HIMAGELIST) SendMessage(htab, TCM_GETIMAGELIST, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set the current selection of a tab control
long CALLBACK SysTabSetCurrentItem(HWND htab, long TabItem)
{
    return(SendMessage(htab, TCM_SETCURSEL, TabItem, (long) 0));
}

// -----------------------------------------------------------------------
// Set the current focus of a tab control
long CALLBACK SysTabSetFocusItem(HWND htab, long TabItem)
{
    return(SendMessage(htab, TCM_SETCURFOCUS, TabItem, (long) 0));
}

// -----------------------------------------------------------------------
// Set the current selection of a tab control
long CALLBACK SysTabHighLightItem(HWND htab, long TabItem, long HighState)
{
    return(SendMessage(htab, TCM_HIGHLIGHTITEM, TabItem, HighState));
}

// -----------------------------------------------------------------------
// Retrieve the number of items of a tab control
long CALLBACK SysTabItemsCount(HWND htab)
{
    return(SendMessage(htab, TCM_GETITEMCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number of rows of a tab control
long CALLBACK SysTabGetRowsCount(HWND htab)
{
    return(SendMessage(htab, TCM_GETROWCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Enable/Disable rows separators of a tab control
long CALLBACK SysTabSetSeparators(HWND htab, long Enable)
{
    if(Enable != 0) return(SendMessage(htab, TCM_SETEXTENDEDSTYLE, TCS_EX_FLATSEPARATORS, TCS_EX_FLATSEPARATORS));
    else return(SendMessage(htab, TCM_SETEXTENDEDSTYLE, TCS_EX_FLATSEPARATORS, (long) 0));
}

// -----------------------------------------------------------------------
// Set the size of the items of a tab control
long CALLBACK SysTabSetItemsSize(HWND htab,long Width,long Height)
{
    return(SendMessage(htab, TCM_SETITEMSIZE, 0, MAKELPARAM(Width,Height)));
}

// -----------------------------------------------------------------------
// Delete an item in a tab control
long CALLBACK SysTabRemoveItem(HWND htab,long ItemIndex)
{
    return(SendMessage(htab, TCM_DELETEITEM, (WPARAM) ItemIndex, 0));
}

// -----------------------------------------------------------------------
// Set the minimum width of items in a tab control
long CALLBACK SysTabSetMinWidth(HWND htab,long Width)
{
    return(SendMessage(htab, TCM_SETMINTABWIDTH, 0, (LPARAM) Width));
}

// -----------------------------------------------------------------------
// Set the size of the items of a tab control
long CALLBACK SysTabSetPadding(HWND htab,long Width,long Height)
{
    return(SendMessage(htab, TCM_SETPADDING, 0, MAKELPARAM(Width,Height)));
}

// -----------------------------------------------------------------------
// TrackBar controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a trackbar control
HWND CALLBACK CreateTrackBar(long PBLeft, long PBTop, long PBWidth, long PBHeight, HWND hParent,
                               long CtrlID, WNDPROC WindowProc, long TBMin, long TBMax, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, PBLeft, PBTop, PBWidth, PBHeight);
    ReturnValue = CreateWindowEx(0, "msctls_trackbar32", "", WS_VISIBLE | WS_CHILD | TBS_AUTOTICKS | ExtraStyle, PBLeft, PBTop, PBWidth, PBHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    TrackBarSetRange(ReturnValue, TBMin, TBMax, 1);
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set a trackbar range
long CALLBACK TrackBarSetRange(HWND hTrackBar, long TBMin, long TBMax, long Redraw)
{
    return(SendMessage(hTrackBar, TBM_SETRANGE, Redraw, ((TBMax * 0x10000) + TBMin)));
}

// -----------------------------------------------------------------------
// Set a new position in a trackbar
long CALLBACK TrackBarSetPos(HWND hTrackBar, long TBNewPos)
{
    return(SendMessage(hTrackBar, TBM_SETPOS, 1, TBNewPos));
}

// -----------------------------------------------------------------------
// Get position in a trackbar
long CALLBACK TrackBarGetPos(HWND hTrackBar)
{
    return(SendMessage(hTrackBar, TBM_GETPOS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set trackbar's ticks
long CALLBACK TrackBarSetTicks(HWND hTrackBar, long TicksFreq)
{
    return(SendMessage(hTrackBar, TBM_SETTICFREQ, TicksFreq, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the handle of the tooltip associated with the trackbar
HWND CALLBACK TrackBarGetToolTips(HWND hTrackBar)
{
    return((HWND) SendMessage(hTrackBar, TBM_GETTOOLTIPS, (WPARAM) 0, (LPARAM) 0));
}

// -----------------------------------------------------------------------
// ToolTip controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a tooltip control
HWND CALLBACK CreateToolTip(HWND hParent, long CtrlID, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ReturnValue = CreateWindowEx(0, TOOLTIPS_CLASS, NULL, WS_VISIBLE | WS_CHILD | TTS_NOPREFIX | TTS_ALWAYSTIP | ExtraStyle, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    SetWindowPos(ReturnValue, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add a tool to a tooltip control
long CALLBACK ToolTipAddTool(HWND hToolTip, HWND hWndOwner, long ID, long ExtraStyle)
{
    TOOLINFO TTip_Info;
    
    TTip_Info.cbSize = sizeof(TOOLINFO);
    TTip_Info.uFlags = TTF_SUBCLASS | ExtraStyle;
    TTip_Info.hwnd = hWndOwner;
    TTip_Info.hinst = WALocalhInst;
    TTip_Info.uId = ID;
    TTip_Info.lpszText = LPSTR_TEXTCALLBACK;
    // HITTEST Inside the owner window
    GetClientRect(hWndOwner, &TTip_Info.rect);
    return(SendMessage(hToolTip, TTM_ADDTOOL, 0, (LPARAM) (LPTOOLINFO) &TTip_Info));
}

// -----------------------------------------------------------------------
// UpDown controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create an updown control
HWND CALLBACK CreateUpDown(long PBLeft, long PBTop, long PBWidth, long PBHeight,
                             HWND hParent, long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, PBLeft, PBTop, PBWidth, PBHeight);
    ReturnValue = CreateWindowEx(0, "msctls_updown32", "", WS_VISIBLE | WS_CHILD | ExtraStyle, PBLeft, PBTop, PBWidth, PBHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// ProgressBar controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a progressbar control
HWND CALLBACK CreateProgressBar(long PBLeft, long PBTop, long PBWidth, long PBHeight,
                                  HWND hParent, long CtrlID, WNDPROC WindowProc, long PBMin, long PBMax, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, PBLeft, PBTop, PBWidth, PBHeight);
    ReturnValue = CreateWindowEx(0, "msctls_progress32", "", WS_VISIBLE | WS_CHILD | ExtraStyle, PBLeft, PBTop, PBWidth, PBHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    SendMessage(ReturnValue, PBM_SETRANGE, 0, ((PBMax * 0x10000) + PBMin));
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set a progressbar range
long CALLBACK ProgressBarSetRange(HWND hProgressBar, long PBMin, long PBMax)
{
    return(SendMessage(hProgressBar, PBM_SETRANGE, 0, ((PBMax * 0x10000) + PBMin)));
}

// -----------------------------------------------------------------------
// Set a new position in a progressbar
long CALLBACK ProgressBarSetRelPos(HWND hProgressBar, long PBNewPos)
{
    return(SendMessage(hProgressBar, PBM_DELTAPOS, PBNewPos, (long) 0));
}

// -----------------------------------------------------------------------
// StatusBar controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a statusbar control
HWND CALLBACK CreateStatusBar(CStr SBText, long RaiseType, HWND hParent, long CtrlID,
                                WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ReturnValue = CreateStatusWindow(WS_CHILD | WS_VISIBLE | ExtraStyle, "", hParent, CtrlID);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ToolTipFont = SendMessage(ReturnValue, WM_GETFONT, 0, (long) 0);
    ControlSetSerif8(ReturnValue);
    StatusBarSetText(ReturnValue, 0, RaiseType, SBText, 0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the number of parts of a statusbar
long CALLBACK StatusBarSetParts(HWND hStatusBar, long SBNbParts, long *PartsArray)
{
    return(SendMessage(hStatusBar, SB_SETPARTS, SBNbParts, (long) PartsArray));
}

// -----------------------------------------------------------------------
// Set the text of a part in a statusbar
long CALLBACK StatusBarSetText(HWND hStatusBar, long SBPart, long RaiseType, CStr SBText, long OwnerDatas)
{
    long RType = 0;

    if((RaiseType & STATUSBAR_PARTSUNKEN) != 0) RType = 0;
    if((RaiseType & STATUSBAR_PARTNORMAL) != 0) RType = RType | SBT_NOBORDERS;
    if((RaiseType & STATUSBAR_PARTRAISED) != 0) RType = RType | SBT_POPOUT;
    if((RaiseType & STATUSBAR_PARTOWNER) != 0) RType = RType | SBT_OWNERDRAW;
    if((RType & SBT_OWNERDRAW) != 0) return(SendMessage(hStatusBar, SB_SETTEXT, SBPart + RType, OwnerDatas));
    else return(SendMessage(hStatusBar, SB_SETTEXT, SBPart + RType, (long) SBText.Get_String()));
}

// -----------------------------------------------------------------------
// Retrieve the Y size of a statusbar
long CALLBACK StatusBarGetYSize(HWND hStatusBar)
{
    RECT SBRct;

    GetWindowRect(hStatusBar, &SBRct);
    return(SBRct.bottom - SBRct.top);
}

// -----------------------------------------------------------------------
// Colorbox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a colorbox control
HWND CALLBACK CreateColorBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, long CtrlID, COLORREF Color, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, LLeft, LTop, LWidth, LHeight);
    ReturnValue = CreateWindowEx(0, "ColorBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft, LTop, LWidth, LHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    SetWindowLong(ReturnValue, GWL_USERDATA, Color);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve color from a colorbox control
COLORREF CALLBACK ColorBoxGetColor(HWND hWnd)
{
    return(GetWindowLong(hWnd, GWL_USERDATA));
}

// -----------------------------------------------------------------------
// Retrieve color from a colorbox control
long CALLBACK ColorBoxSetColor(HWND hWnd, COLORREF Color)
{
    SetWindowLong(hWnd, GWL_USERDATA, Color);
    return(RedrawWindow(hWnd, 0, 0, RDW_ERASE | RDW_INVALIDATE));
}

// -----------------------------------------------------------------------
// DumpBox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a dumpbox control
HWND CALLBACK CreateDumpBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent,
                              long CtrlID, HFONT hFont, long MaxX, long MaxY, long PageValueX,
                              long PageValueY, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, LLeft, LTop, LWidth, LHeight);
    ReturnValue = CreateWindowEx(ExtraExStyle, "DumpBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft, LTop, LWidth, LHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetFont(ReturnValue, hFont);
    ScrollBarSetPageRange(ReturnValue, SB_HORZ, PageValueX);
    ScrollBarSetPageRange(ReturnValue, SB_VERT, PageValueY);
    if(MaxX == 0) MaxX = 1;
    if(MaxY == 0) MaxY = 1;
    ScrollBarSetMinMaxRange(ReturnValue, SB_HORZ, 0, MaxX);
    ScrollBarSetMinMaxRange(ReturnValue, SB_VERT, 0, MaxY);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Resize a dumpbox
void CALLBACK DumpBoxResize(HWND hWnd, long left, long top, long Width, long Height)
{
    ControlResize(hWnd, left, top, Width, Height);
}

// -----------------------------------------------------------------------
// Scroll down a dumpbox
long CALLBACK DumpBoxScrollDown(HWND hWnd, long Lines, long Factor, long Integral)
{
    RECT DumpRect;
    long ReturnValue = 0;

    DumpRect.left = 0;
    DumpRect.top = 0;
    DumpRect.right = ControlClientWidth(hWnd);
    DumpRect.bottom = DumpBoxGetVisibleLines(hWnd, Factor, Integral) + 1;
    ReturnValue = 0;
    if(DumpRect.bottom != 0)
    {
        DumpRect.bottom = DumpRect.bottom * Factor;
        ReturnValue = ScrollWindow(hWnd, 0, -(long)(Factor * Lines), 0, &DumpRect);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Scroll up a dumpbox
long CALLBACK DumpBoxScrollUp(HWND hWnd, long Lines, long Factor, long Integral)
{
    RECT DumpRect;
    RECT DumpRectClip;
    long ReturnValue = 0;

    DumpRect.left = 0;
    DumpRect.top = 0;
    DumpRectClip.left = 0;
    DumpRectClip.top = 0;
    DumpRect.right = ControlClientWidth(hWnd);
    DumpRectClip.right = DumpRect.right;
    DumpRect.bottom = DumpBoxGetVisibleLines(hWnd, Factor, Integral);
    ReturnValue = 0;
    if(DumpRect.bottom != 0)
    {
        DumpRect.bottom = DumpRect.bottom * Factor;
        DumpRectClip.bottom = DumpRect.bottom + Factor;
        if((long) ControlClientHeight(hWnd) < (long) DumpRectClip.bottom) DumpRectClip.bottom = DumpRect.bottom;
        ReturnValue = ScrollWindow(hWnd, 0, (Factor * Lines), 0, &DumpRectClip);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the number of lines of a dumpbox
long CALLBACK DumpBoxGetVisibleLines(HWND hWnd, long Factor, long Integral)
{
    long Returnvalue = 0;

    Returnvalue = ControlClientHeight(hWnd) / Factor;
    if(Integral == 1) if((ControlClientHeight(hWnd) % Factor) != 0) Returnvalue = Returnvalue - 1;
    return(Returnvalue);
}

// -----------------------------------------------------------------------
// Retrieve the number of columns of a dumpbox
long CALLBACK DumpBoxGetVisibleColumns(HWND hWnd, long Factor, long Integral)
{
    long Returnvalue = 0;

    Returnvalue = ControlClientWidth(hWnd) / Factor;
    if(Integral == 1) if((ControlClientWidth(hWnd) % Factor) != 0) Returnvalue--;
    return(Returnvalue);
}

// -----------------------------------------------------------------------
// Gripbox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a colorbox control
LPGRIPBOXDAT CALLBACK CreateGripBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent,
                                      long CtrlID, COLORREF Color, long ExtraStyle)
{
    LPGRIPBOXDAT NewGripDats;
    LPGRIPBOXSQRBOXDAT NewGripSquare;

    ControlBound(hParent, LLeft, LTop, LWidth, LHeight);

    // Create the datas slot
    NewGripDats = (LPGRIPBOXDAT) AllocMem(sizeof(GRIPBOXDAT));
    // Store the color

    // Create the squares
    NewGripDats->TopLeftBox = CreateWindowEx(0, "GripBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft, LTop, 8, 8, hParent, 0, WALocalhInst, NULL);
    NewGripSquare = (LPGRIPBOXSQRBOXDAT) AllocMem(sizeof(GRIPBOXSQRBOXDAT));
    NewGripSquare->Type = GRIPBOX_TOPLEFT;
    NewGripSquare->State = GRIPBOX_STATE_RESET;
    // Default steps
    NewGripSquare->StepX = 1;
    NewGripSquare->StepY = 1;
    NewGripSquare->Color = Color;
    NewGripSquare->Container = NewGripDats;
    SetWindowLong(NewGripDats->TopLeftBox, GWL_USERDATA, (long) NewGripSquare);

    NewGripDats->TopBox = CreateWindowEx(0, "GripBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft + ((LWidth >> 1) - 4), LTop, 8, 8, hParent, 0, WALocalhInst, NULL);
    NewGripSquare = (LPGRIPBOXSQRBOXDAT) AllocMem(sizeof(GRIPBOXSQRBOXDAT));
    NewGripSquare->Type = GRIPBOX_TOP;
    NewGripSquare->State = GRIPBOX_STATE_RESET;
    NewGripSquare->StepX = 1;
    NewGripSquare->StepY = 1;
    NewGripSquare->Color = Color;
    NewGripSquare->Container = NewGripDats;
    SetWindowLong(NewGripDats->TopBox, GWL_USERDATA, (long) NewGripSquare);

    NewGripDats->TopRightBox = CreateWindowEx(0, "GripBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft + (LWidth - 8), LTop, 8, 8, hParent, 0, WALocalhInst, NULL);
    NewGripSquare = (LPGRIPBOXSQRBOXDAT) AllocMem(sizeof(GRIPBOXSQRBOXDAT));
    NewGripSquare->Type = GRIPBOX_TOPRIGHT;
    NewGripSquare->State = GRIPBOX_STATE_RESET;
    NewGripSquare->StepX = 1;
    NewGripSquare->StepY = 1;
    NewGripSquare->Color = Color;
    NewGripSquare->Container = NewGripDats;
    SetWindowLong(NewGripDats->TopRightBox, GWL_USERDATA, (long) NewGripSquare);

    NewGripDats->BottomLeftBox = CreateWindowEx(0, "GripBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft, LTop + (LHeight - 8), 8, 8, hParent, 0, WALocalhInst, NULL);
    NewGripSquare = (LPGRIPBOXSQRBOXDAT) AllocMem(sizeof(GRIPBOXSQRBOXDAT));
    NewGripSquare->Type = GRIPBOX_BOTTOMLEFT;
    NewGripSquare->State = GRIPBOX_STATE_RESET;
    NewGripSquare->StepX = 1;
    NewGripSquare->StepY = 1;
    NewGripSquare->Color = Color;
    NewGripSquare->Container = NewGripDats;
    SetWindowLong(NewGripDats->BottomLeftBox, GWL_USERDATA, (long) NewGripSquare);

    NewGripDats->BottomBox = CreateWindowEx(0, "GripBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft + ((LWidth >> 1) - 4), LTop + (LHeight - 8), 8, 8, hParent, 0, WALocalhInst, NULL);
    NewGripSquare = (LPGRIPBOXSQRBOXDAT) AllocMem(sizeof(GRIPBOXSQRBOXDAT));
    NewGripSquare->Type = GRIPBOX_BOTTOM;
    NewGripSquare->State = GRIPBOX_STATE_RESET;
    NewGripSquare->StepX = 1;
    NewGripSquare->StepY = 1;
    NewGripSquare->Color = Color;
    NewGripSquare->Container = NewGripDats;
    SetWindowLong(NewGripDats->BottomBox, GWL_USERDATA, (long) NewGripSquare);

    NewGripDats->BottomRightBox = CreateWindowEx(0, "GripBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft + (LWidth - 8), LTop + (LHeight - 8), 8, 8, hParent, 0, WALocalhInst, NULL);
    NewGripSquare = (LPGRIPBOXSQRBOXDAT) AllocMem(sizeof(GRIPBOXSQRBOXDAT));
    NewGripSquare->Type = GRIPBOX_BOTTOMRIGHT;
    NewGripSquare->State = GRIPBOX_STATE_RESET;
    NewGripSquare->StepX = 1;
    NewGripSquare->StepY = 1;
    NewGripSquare->Color = Color;
    NewGripSquare->Container = NewGripDats;
    SetWindowLong(NewGripDats->BottomRightBox, GWL_USERDATA, (long) NewGripSquare);

    NewGripDats->LeftBox = CreateWindowEx(0, "GripBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft, LTop + ((LHeight >> 1) - 4), 8, 8, hParent, 0, WALocalhInst, NULL);
    NewGripSquare = (LPGRIPBOXSQRBOXDAT) AllocMem(sizeof(GRIPBOXSQRBOXDAT));
    NewGripSquare->Type = GRIPBOX_LEFT;
    NewGripSquare->State = GRIPBOX_STATE_RESET;
    NewGripSquare->StepX = 1;
    NewGripSquare->StepY = 1;
    NewGripSquare->Color = Color;
    NewGripSquare->Container = NewGripDats;
    SetWindowLong(NewGripDats->LeftBox, GWL_USERDATA, (long) NewGripSquare);

    NewGripDats->RightBox = CreateWindowEx(0, "GripBoxClass", "", WS_VISIBLE | WS_CHILD | ExtraStyle, LLeft + (LWidth - 8), LTop + ((LHeight >> 1) - 4), 8, 8, hParent, 0, WALocalhInst, NULL);
    NewGripSquare = (LPGRIPBOXSQRBOXDAT) AllocMem(sizeof(GRIPBOXSQRBOXDAT));
    NewGripSquare->Type = GRIPBOX_RIGHT;
    NewGripSquare->State = GRIPBOX_STATE_RESET;
    NewGripSquare->StepX = 1;
    NewGripSquare->StepY = 1;
    NewGripSquare->Color = Color;
    NewGripSquare->Container = NewGripDats;
    SetWindowLong(NewGripDats->RightBox, GWL_USERDATA, (long) NewGripSquare);

    // Store the slot in window's user datas
    return(NewGripDats);
}

// -----------------------------------------------------------------------
// Retrieve color from a gripbox control
COLORREF CALLBACK GripBoxGetColor(LPGRIPBOXDAT GripDats)
{
    LPGRIPBOXSQRBOXDAT GripSquare;

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->LeftBox, GWL_USERDATA);
    return(GripSquare->Color);
}

// -----------------------------------------------------------------------
// Retrieve color from a gripbox control
void CALLBACK GripBoxSetColor(LPGRIPBOXDAT GripDats, COLORREF Color)
{
    LPGRIPBOXSQRBOXDAT GripSquare;

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->BottomBox, GWL_USERDATA);
    GripSquare->Color = Color;
    RedrawWindow(GripDats->BottomBox, 0, 0, RDW_ERASE | RDW_INVALIDATE);

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->BottomLeftBox, GWL_USERDATA);
    GripSquare->Color = Color;
    RedrawWindow(GripDats->BottomLeftBox, 0, 0, RDW_ERASE | RDW_INVALIDATE);

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->BottomRightBox, GWL_USERDATA);
    GripSquare->Color = Color;
    RedrawWindow(GripDats->BottomRightBox, 0, 0, RDW_ERASE | RDW_INVALIDATE);

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->LeftBox, GWL_USERDATA);
    GripSquare->Color = Color;
    RedrawWindow(GripDats->LeftBox, 0, 0, RDW_ERASE | RDW_INVALIDATE);

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->RightBox, GWL_USERDATA);
    GripSquare->Color = Color;
    RedrawWindow(GripDats->RightBox, 0, 0, RDW_ERASE | RDW_INVALIDATE);

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->TopBox, GWL_USERDATA);
    GripSquare->Color = Color;
    RedrawWindow(GripDats->TopBox, 0, 0, RDW_ERASE | RDW_INVALIDATE);

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->TopLeftBox, GWL_USERDATA);
    GripSquare->Color = Color;
    RedrawWindow(GripDats->TopLeftBox, 0, 0, RDW_ERASE | RDW_INVALIDATE);

    GripSquare = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->TopRightBox, GWL_USERDATA);
    GripSquare->Color = Color;
    RedrawWindow(GripDats->TopRightBox, 0, 0, RDW_ERASE | RDW_INVALIDATE);
}

// -----------------------------------------------------------------------
// Close a gripbox control
void CALLBACK GripBoxClose(LPGRIPBOXDAT GripDats)
{
    LPGRIPBOXSQRBOXDAT GripSquareDats;
    LPGRIPBOXDAT GripDats_Container;

    GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->TopLeftBox, GWL_USERDATA);
    if(GripSquareDats)
    {
        GripDats_Container = GripSquareDats->Container;
        ControlClose(GripDats->TopLeftBox);
        ControlClose(GripDats->TopBox);
        ControlClose(GripDats->TopRightBox);
        ControlClose(GripDats->LeftBox);
        ControlClose(GripDats->RightBox);
        ControlClose(GripDats->BottomLeftBox);
        ControlClose(GripDats->BottomBox);
        ControlClose(GripDats->BottomRightBox);
        // Free it now
        FreeMem((long) GripDats_Container);
    }
}

// -----------------------------------------------------------------------
// Close a gripbox control
void CALLBACK GripBoxVisible(LPGRIPBOXDAT GripDats, long CState)
{
    ControlVisible(GripDats->TopLeftBox, CState);
    ControlVisible(GripDats->TopBox, CState);
    ControlVisible(GripDats->TopRightBox, CState);
    ControlVisible(GripDats->LeftBox, CState);
    ControlVisible(GripDats->RightBox, CState);
    ControlVisible(GripDats->BottomLeftBox, CState);
    ControlVisible(GripDats->BottomBox, CState);
    ControlVisible(GripDats->BottomRightBox, CState);
}

// -----------------------------------------------------------------------
// Retrieve color from a gripbox control
void CALLBACK GripBoxResize(LPGRIPBOXDAT GripDats, LPRECT NewSizeRect)
{
    long LWidth;
    long LHeight;
    long LLeft;
    long LTop;

    LLeft = NewSizeRect->left;
    LTop = NewSizeRect->top;
    LWidth = NewSizeRect->right - LLeft;
    LHeight = NewSizeRect->bottom - LTop;
    MoveWindow(GripDats->TopLeftBox, LLeft - 8, LTop - 8, 8, 8, 1);
    MoveWindow(GripDats->TopBox, LLeft + ((LWidth >> 1) - 4), LTop - 8, 8, 8, 1);
    MoveWindow(GripDats->TopRightBox, LLeft + (LWidth - 8) + 8, LTop - 8, 8, 8, 1);
    MoveWindow(GripDats->BottomLeftBox, LLeft - 8, LTop + (LHeight - 8) + 8, 8, 8, 1);
    MoveWindow(GripDats->BottomBox, LLeft + ((LWidth >> 1) - 4), LTop + (LHeight - 8) + 8, 8, 8, 1);
    MoveWindow(GripDats->BottomRightBox, LLeft + (LWidth - 8) + 8, LTop + (LHeight - 8) + 8, 8, 8, 1);
    MoveWindow(GripDats->LeftBox, LLeft - 8, LTop + ((LHeight >> 1) - 4), 8, 8, 1);
    MoveWindow(GripDats->RightBox, LLeft + (LWidth - 8) + 8, LTop + ((LHeight >> 1) - 4), 8, 8, 1);
}

// -----------------------------------------------------------------------
// Label controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a label control
HWND CALLBACK CreateLabel(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent,
                            CStr LText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, HFONT hFont)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, LLeft, LTop, LWidth, LHeight);
    ReturnValue = CreateWindowEx(0, "STATIC", LText.Get_String(), WS_VISIBLE | WS_CHILD | SS_LEFT | ExtraStyle, LLeft, LTop, LWidth, LHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    if(hFont == 0) ControlSetSerif8(ReturnValue);
    else SendMessage(ReturnValue, WM_SETFONT, (UINT) hFont, (long) 0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get the image associated with a label control
HGDIOBJ CALLBACK LabelGetImage(HWND hLabel, long ImageType)
{
    return((HGDIOBJ) SendMessage(hLabel, STM_GETIMAGE, ImageType, (long) 0));
}

// -----------------------------------------------------------------------
// Imagebox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create an imagebox control
HWND CALLBACK CreatePictureBox(long LLeft, long LTop, long LWidth, long LHeight, HWND hParent, HGDIOBJ hPicture,
                                 long PictureType, long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;
    long PType = 0;

    ControlBound(hParent, LLeft, LTop, LWidth, LHeight);
    switch(PictureType)
    {
        case IMAGE_BITMAP:
            PType = SS_BITMAP;
            break;
        case IMAGE_ICON:
            PType = SS_ICON;
            break;
    }
    ReturnValue = CreateWindowEx(0, "STATIC", (char *) 0, WS_VISIBLE | WS_CHILD | PType | ExtraStyle, LLeft, LTop, LWidth, LHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    switch(PictureType)
    {
        case IMAGE_BITMAP:
            SendMessage(ReturnValue, STM_SETIMAGE, IMAGE_BITMAP, (long) hPicture);
            break;
        case IMAGE_ICON:
            SendMessage(ReturnValue, STM_SETICON, (UINT) hPicture, (long) 0);
            break;
    }
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Change the icon associated with a picturebox control
long CALLBACK PictureBoxChangeIcon(HWND hPictureBox, HICON IconHandle)
{
    SendMessage(hPictureBox, STM_SETICON, 0, (long) 0);
    return(SendMessage(hPictureBox, STM_SETICON, (UINT) IconHandle, (long) 0));
}

// -----------------------------------------------------------------------
// Change the image associated with a picturebox control
long CALLBACK PictureBoxChangeImage(HWND hPictureBox, HANDLE ImageHandle)
{
    SendMessage(hPictureBox, STM_SETIMAGE, 0, (long) 0);
    return(SendMessage(hPictureBox, STM_SETIMAGE, IMAGE_BITMAP, (long) ImageHandle));
}

// -----------------------------------------------------------------------
// Frame controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a frame control
HWND CALLBACK CreateFrame(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent,
                            CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(WS_EX_CONTROLPARENT, "BUTTON", BText.Get_String(), WS_VISIBLE | WS_CHILD | BS_GROUPBOX | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Refresh a toolbar included inside a frame
long CALLBACK FrameRefreshToolbar(HWND hToolbar)
{
    long ReturnValue = 0;
    HDC TbHdc = 0;

    TbHdc = GetDC(hToolbar);
    ReturnValue = SendMessage(hToolbar, WM_PAINT, (UINT) TbHdc, (long) 0);
    ReleaseDC(hToolbar, TbHdc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Button controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a button control
HWND CALLBACK CreateButton(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText, long CtrlID, long ImgType, HICON hImage, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle)
{
    HWND ReturnValue = 0;
    long BtStyle = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    BtStyle = WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | ExtraStyle;
    if(hImage != 0) BtStyle = BtStyle | BS_ICON;
    ReturnValue = CreateWindowEx(ExtraExStyle | WS_EX_NOPARENTNOTIFY, "BUTTON", BText.Get_String(), BtStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ButtonSetIcon(ReturnValue, ImgType, hImage);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set icon to be displayed in a button control
long CALLBACK ButtonSetIcon(HWND hbutton, long ImType, HICON hImage)
{
    return(SendMessage(hbutton, BM_SETIMAGE, ImType, (long) hImage));
}

// -----------------------------------------------------------------------
// Retrieve handle of a bitmap or icon associated with a button control
HANDLE CALLBACK ButtonGetImage(HWND hbutton, long ImageType)
{
    return((HANDLE) SendMessage(hbutton, BM_GETSTATE, ImageType, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve check state of a button control
long CALLBACK ButtonGetState(HWND hbutton)
{
    return(SendMessage(hbutton, BM_GETSTATE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// IPbox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create an IPbox control
HWND CALLBACK CreateIPBox(long IPBLeft, long IPBTop, long IPBWidth, long IPBHeight, HWND hParent,
                            long IPAddress, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long SetBorder,
                            HFONT hFont)
{
    long ExStyle = 0;
    HWND ReturnValue = 0;

    ControlBound(hParent, IPBLeft, IPBTop, IPBWidth, IPBHeight);
    ExStyle = WS_EX_NOPARENTNOTIFY;
    if(SetBorder) ExStyle = ExStyle | WS_EX_CLIENTEDGE;
    ReturnValue = CreateWindowEx(ExStyle, "SysIPAddress32", "", WS_VISIBLE | WS_CHILD | ExtraStyle, IPBLeft, IPBTop, IPBWidth, IPBHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    IPBoxSetIP(ReturnValue, IPAddress);
    ControlSetFont(ReturnValue, hFont);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Clear an IPbox control
long CALLBACK IPBoxClearIP(HWND hIPBox)
{
    return(SendMessage(hIPBox, IPM_CLEARADDRESS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the IP address of an IPbox control
long CALLBACK IPBoxGetIP(HWND hIPBox)
{
    long ReturnValue = 0;

    SendMessage(hIPBox, IPM_GETADDRESS, 0, (long) &ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the IP address of an IPbox control
long CALLBACK IPBoxSetIP(HWND hIPBox, long IPAddress)
{
    return(SendMessage(hIPBox, IPM_SETADDRESS, 0, IPAddress));
}

// -----------------------------------------------------------------------
// Give the keyboard focus to an IPbox control
long CALLBACK IPBoxSetFocus(HWND hIPBox, long FieldNumber)
{
    return(SendMessage(hIPBox, IPM_SETFOCUS, FieldNumber, (long) 0));
}

// -----------------------------------------------------------------------
// Set the min/max range of an IPbox control field
long CALLBACK IPBoxSetRange(HWND hIPBox, long FieldNumber, long LowValue, long HighValue)
{
    return(SendMessage(hIPBox, IPM_SETRANGE, FieldNumber, ((HighValue * 0x100) + LowValue)));
}

// -----------------------------------------------------------------------
// Check if an IPBox control is blank
long CALLBACK IPBoxIsBlank(HWND hIPBox)
{
    return(SendMessage(hIPBox, IPM_ISBLANK, 0, (long) 0));
}

// -----------------------------------------------------------------------
// RichTextbox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a richtextbox control
HWND CALLBACK CreateRichTextBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent,
                                  CStr EDText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long SetBorder)
{
    long ExStyle = 0;
    HWND ReturnValue = 0;

    ControlBound(hParent, EDLeft, EDTop, EDWidth, EDHeight);
    ExStyle = WS_EX_NOPARENTNOTIFY;
    if(SetBorder != 0) ExStyle = ExStyle | WS_EX_CLIENTEDGE;
    ReturnValue = CreateWindowEx(ExStyle, RichedClass.Get_String(), EDText.Get_String(), WS_VISIBLE | WS_CHILD | ExtraStyle, EDLeft, EDTop, EDWidth, EDHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Textbox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a textbox control
HWND CALLBACK CreateTextBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent,
                              CStr EDText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, EDLeft, EDTop, EDWidth, EDHeight);
    if((ExtraExStyle & WS_EX_STATICEDGE) != 0)
    {
        EDTop = EDTop + 2;
        EDHeight = EDHeight - 4;
    }
    ReturnValue = CreateWindowEx(WS_EX_NOPARENTNOTIFY | ExtraExStyle, "EDIT", EDText.Get_String(), WS_VISIBLE | WS_CHILD | ES_LEFT | ExtraStyle, EDLeft, EDTop, EDWidth, EDHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    if(ExtraStyle & ES_PASSWORD) TextBoxSetPasswordChar(ReturnValue, "*");
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the number of chars in a textbox control
long CALLBACK TextBoxGetLen(HWND hTextBox)
{
    return(GetWindowTextLength(hTextBox));
}

// -----------------------------------------------------------------------
// Set the chars maximum in a textbox control
long CALLBACK TextBoxSetMaxLen(HWND hTextBox, long TBMaxLen)
{
    return(SendMessage(hTextBox, EM_LIMITTEXT, TBMaxLen, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the maximum chars in a textbox control
long CALLBACK TextBoxGetMaxLen(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETLIMITTEXT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the current memory position of the text contained by a textbox
long CALLBACK TextBoxGetTextHandle(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETHANDLE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Add a text at the end of a textbox
void CALLBACK TextBoxAddText(HWND hTextBox, CStr TbText)
{
    CStr Txt;

    Txt = ControlGetText(hTextBox);
    Txt = Txt + TbText;
    ControlSetText(hTextBox, Txt);
}

// -----------------------------------------------------------------------
// Select the content of a textbox control
long CALLBACK TextBoxSelText(HWND hTextBox, long NbrToSel)
{
    long ReturnValue = 0;

    if(NbrToSel == -1) ReturnValue = SendMessage(hTextBox, EM_SETSEL, 0, ControlGetTextLen(hTextBox));
    else ReturnValue = SendMessage(hTextBox, EM_SETSEL, 0, NbrToSel);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Unselect text in a textbox control
long CALLBACK TextBoxRemoveSel(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_SETSEL, -1, (long) 0));
}

// -----------------------------------------------------------------------
// Set the caret position in a textbox control
long CALLBACK TextBoxSetCaretPos(HWND hTextBox, long PosToSel)
{
    long ReturnValue = 0;
    long NCaretPos = 0;

    if(PosToSel == -1)
    {
        NCaretPos = ControlGetTextLen(hTextBox);
        ReturnValue = SendMessage(hTextBox, EM_SETSEL, NCaretPos, NCaretPos);
    }
    else
    {
        ReturnValue = SendMessage(hTextBox, EM_SETSEL, PosToSel, PosToSel);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the number of lines of a textbox control
long CALLBACK TextBoxLinesCount(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETLINECOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number of lines of a textbox control
long CALLBACK TextBoxGetMarginWidth(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETMARGINS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number of the first visible line of a textbox control
long CALLBACK TextBoxGetFirstVisibleLine(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETFIRSTVISIBLELINE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the modified state of a textbox control
long CALLBACK TextBoxIsModified(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETMODIFY, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the options of a textbox control
long CALLBACK TextBoxGetCurrentOptions(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETOPTIONS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the password char of a textbox control
CStr CALLBACK TextBoxGetPasswordChar(HWND hTextBox)
{
    CStr ReturnValue;
    long Pchar = 0;

    Pchar = SendMessage(hTextBox, EM_GETPASSWORDCHAR, 0, (long) 0);
    if(Pchar != 0) ReturnValue = ReturnValue.Chr(Pchar);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the password char of a textbox control
long CALLBACK TextBoxSetPasswordChar(HWND hTextBox, CStr PassChar)
{
    return(SendMessage(hTextBox, EM_SETPASSWORDCHAR, (WPARAM) PassChar.Get_String()[0], (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the current selection bounds of a textbox control
long CALLBACK TextBoxGetSelection(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETSEL, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the current scrolling position of a textbox control
long CALLBACK TextBoxGetScrollPos(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETTHUMB, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the current selection bounds of a textbox control
long CALLBACK TextBoxGetBreakProcAddress(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_GETWORDBREAKPROC, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Check if datas stored in clipboard can be pasted in a textbox control
long CALLBACK TextBoxCanPaste(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_CANPASTE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Check if current can be undone in a textbox control
long CALLBACK TextBoxCanUndo(HWND hTextBox)
{
    return(SendMessage(hTextBox, EM_CANUNDO, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Hexbox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a hexbox control
HWND CALLBACK CreateHexBox(long EDLeft, long EDTop, long EDWidth, long EDHeight, HWND hParent,
                             CStr EDText, long CtrlID, long MaxDigits, long ExtraStyle, long ExtraExStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, EDLeft, EDTop, EDWidth, EDHeight);
    if((ExtraExStyle & WS_EX_STATICEDGE) != 0)
    {
        EDTop = EDTop + 2;
        EDHeight = EDHeight - 4;
    }
    ReturnValue = CreateWindowEx(WS_EX_NOPARENTNOTIFY | ExtraExStyle, "EDIT", EDText.Get_String(), WS_VISIBLE | WS_CHILD | ES_LEFT | ExtraStyle, EDLeft, EDTop, EDWidth, EDHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    SetWindowLong(ReturnValue, GWL_USERDATA, SetWindowLong(ReturnValue, GWL_WNDPROC, (long) &WAHexHook));
    if(MaxDigits == 0) MaxDigits = 1;
    SendMessage(ReturnValue, EM_LIMITTEXT, MaxDigits, (long) 0);
    SendMessage(ReturnValue, WM_SETFONT, (UINT) WACourrierNewFont9, (long) 0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Checkbox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a checkbox control
HWND CALLBACK CreateCheckBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent,
                               CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle, long UndefState)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    if(UndefState == 1) ReturnValue = CreateWindowEx(WS_EX_TRANSPARENT, "BUTTON", BText.Get_String(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_LEFT | BS_VCENTER | BS_AUTO3STATE | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    else ReturnValue = CreateWindowEx(WS_EX_TRANSPARENT, "BUTTON", BText.Get_String(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_LEFT | BS_VCENTER | BS_AUTOCHECKBOX | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve click state of a checkbox
long CALLBACK CheckBoxGetState(HWND hCB)
{
    return(SendMessage(hCB, BM_GETCHECK, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set click state of a checkbox
long CALLBACK CheckBoxSetState(HWND hCB, long CheckedState)
{
    return(SendMessage(hCB, BM_SETCHECK, CheckedState, (long) 0));
}

// -----------------------------------------------------------------------
// Rebar controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a Rebar control
HWND CALLBACK CreateRebar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, WNDPROC WindowProc,
                            long CtrlID, long ExtraStyle)
{
    HWND ReturnValue = 0;
    REBARINFO Rbi;

    ReturnValue = CreateWindowEx(0, "ReBarWindow32", "", WS_CLIPSIBLINGS | WS_VISIBLE | WS_CHILD | WS_CLIPCHILDREN | RBS_DBLCLKTOGGLE | RBS_VARHEIGHT | CCS_NODIVIDER | CCS_NOPARENTALIGN | ExtraStyle, 0, 0, 0, 0, hParent, 0, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    Rbi.cbSize = sizeof(Rbi);
    Rbi.fMask = 0;
    Rbi.himl = 0;
    SendMessage(ReturnValue, RB_SETBARINFO, 0, (long) &Rbi);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Create a band to a rebar control
long CALLBACK RebarAddBand(HWND hRebar, HWND hChild, CStr RBText, long RBYSize, long ExtraStyle, long MinSize)
{
    REBARBANDINFO RBand;

    RBand.cbSize = sizeof(RBand);
    RBand.fMask = RBBIM_STYLE | RBBIM_SIZE;
    RBand.fStyle = ExtraStyle;
    if(RBText.Len() != 0) RBand.fMask = RBand.fMask | RBBIM_TEXT;
    if(hChild != 0) RBand.fMask = RBand.fMask | RBBIM_CHILDSIZE | RBBIM_CHILD;
    RBand.iImage = 0;
    RBand.lpText = RBText.Get_String();
    RBand.hwndChild = hChild;
    RBand.cch = RBText.Len();
    RBand.cyChild = 0;
    RBand.cxMinChild = 0;
    RBand.cyMinChild = RBYSize;
    RBand.cyMaxChild = RBYSize;
    RBand.cyIntegral = RBYSize;
    RBand.cxHeader = 0;
    RBand.cxIdeal = 0;
    RBand.cx = MinSize;
    return(SendMessage(hRebar, RB_INSERTBAND, -1, (long) &RBand));
}

// -----------------------------------------------------------------------
// Create a band to a rebar control
long CALLBACK RebarRemoveBand(HWND hRebar, long BandNumber)
{
    return(SendMessage(hRebar, RB_DELETEBAND, BandNumber, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the X size of a rebar control
long CALLBACK RebarGetXSize(HWND hRebar)
{
    long ReturnValue = 0;
    RECT BBRct;

    if(ControlIsVisible(hRebar) != 0)
    {
        GetWindowRect(hRebar, &BBRct);
        ReturnValue = BBRct.right - BBRct.left;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the Y size of a rebar control
long CALLBACK RebarGetYSize(HWND hRebar)
{
    long ReturnValue = 0;
    RECT BBRct;

    if(ControlIsVisible(hRebar) != 0)
    {
        GetWindowRect(hRebar, &BBRct);
        ReturnValue = BBRct.bottom - BBRct.top;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Resize a rebar control
long CALLBACK RebarResize(HWND hRebar)
{
    RECT RBRct;
    RECT PRBRct;
    HWND hParent = 0;
    long ReturnValue = 0;

    if(ControlIsVisible(hRebar) != 0)
    {
        hParent = GetParent(hRebar);
        GetClientRect(hRebar, &RBRct);
        GetClientRect(hParent, &PRBRct);
        ReturnValue = SetWindowPos(hRebar, 0, 0, 0, PRBRct.right - PRBRct.left, RBRct.bottom - RBRct.top, SWP_NOOWNERZORDER + SWP_NOMOVE + SWP_NOZORDER + SWP_SHOWWINDOW);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Change the background color of a rebar control
long CALLBACK RebarSetBackColor(HWND hRebar, COLORREF RBColor)
{
    return(SendMessage(hRebar, RB_SETBKCOLOR, 0, RBColor));
}

// -----------------------------------------------------------------------
// Change the text color of a rebar control
long CALLBACK RebarSetTextColor(HWND hRebar, COLORREF RBColor)
{
    return(SendMessage(hRebar, RB_SETTEXTCOLOR, 0, RBColor));
}

// -----------------------------------------------------------------------
// Change the visible state of a rebar band
long CALLBACK RebarBandVisible(HWND hRebar, long RBBand, long RBVisible)
{
    return(SendMessage(hRebar, RB_SHOWBAND, RBBand, RBVisible));
}

// -----------------------------------------------------------------------
// Retrieve the number of bands of a rebar
long CALLBACK RebarGetBandsCount(HWND hRebar)
{
    return(SendMessage(hRebar, RB_GETBANDCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the height of a rebar
long CALLBACK RebarGetHeight(HWND hRebar)
{
    return(SendMessage(hRebar, RB_GETBARHEIGHT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the used default background color of a rebar
COLORREF CALLBACK RebarGetBackGroundColor(HWND hRebar)
{
    return(SendMessage(hRebar, RB_GETBKCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number of rows of a rebar
long CALLBACK RebarGetRowsCount(HWND hRebar)
{
    return(SendMessage(hRebar, RB_GETROWCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the handle of the tooltips control associated with a rebar
long CALLBACK RebarGetToolTips(HWND hRebar)
{
    return(SendMessage(hRebar, RB_GETTOOLTIPS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve used text color of a rebar
COLORREF CALLBACK RebarGetTextColor(HWND hRebar)
{
    return(SendMessage(hRebar, RB_GETTEXTCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the handle of the palette used in a rebar
long CALLBACK RebarGetPalette(HWND hRebar)
{
    return(SendMessage(hRebar, RB_GETPALETTE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve used image in a rebar
HIMAGELIST CALLBACK RebarGetImageList(HWND hRebar)
{
    REBARINFO RbInfo;

    memset(&RbInfo, 0, sizeof(RbInfo));
    RbInfo.cbSize = sizeof(RbInfo);
    RbInfo.fMask = RBIM_IMAGELIST;
    return((HIMAGELIST) SendMessage(hRebar, RB_GETBARINFO, 0, (long) &RbInfo));
}

// -----------------------------------------------------------------------
// Pager controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a Pager control
HWND CALLBACK CreatePager(long PLeft, long PTop, long PWidth, long PHeight, HWND hParent, HWND hChild,
                            long CtrlID, long ExtraStyle)
{
    HWND ReturnValue = 0;
    long Extra = 0;

    ControlBound(hParent, PLeft, PTop, PWidth, PHeight);
    if((ExtraStyle & WS_BORDER) != 0) Extra = WS_EX_CLIENTEDGE;
    ReturnValue = CreateWindowEx(Extra, "SysPager", "", WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_CHILD | PGS_VERT | CCS_NORESIZE | ExtraStyle, PLeft, PTop, PWidth, PHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    SetWindowLong(ReturnValue, GWL_USERDATA, (long) hChild);
    SendMessage(ReturnValue, PGM_SETCHILD, 0, (long) hChild);
    SetParent(hChild, ReturnValue);
    SendMessage(ReturnValue, PGM_RECALCSIZE, 0, (long) 0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Proceed a pagesize demand from a pager control
void CALLBACK PagerDisplaySetSize(LPNMPGCALCSIZE CalcSizeNotify, LPSIZE PGNewSize)
{
    switch(CalcSizeNotify->dwFlag)
    {
        case PGF_CALCHEIGHT:
            CalcSizeNotify->iHeight = PGNewSize->cx;
            CalcSizeNotify->iWidth = PGNewSize->cy;
            break;
        case PGF_CALCWIDTH:
            CalcSizeNotify->iHeight = PGNewSize->cy;
            CalcSizeNotify->iWidth = PGNewSize->cx;
            break;
    }
}

// -----------------------------------------------------------------------
// Maximize a pager on X axis according to it's parent control
long CALLBACK PagerMaximizeX(HWND hPager, long ChildYSize)
{
    RECT Rct;
    HWND hParent = 0;

    hParent = GetParent(hPager);
    GetClientRect(hParent, &Rct);
    return(MoveWindow(hPager, Rct.left, Rct.top, Rct.right - Rct.left, ChildYSize, 1));
}

// -----------------------------------------------------------------------
// Maximize a pager on Y axis according to it's parent control
long CALLBACK PagerMaximizeY(HWND hPager, long ChildXSize)
{
    RECT Rct;
    HWND hParent = 0;

    hParent = GetParent(hPager);
    GetClientRect(hParent, &Rct);
    return(MoveWindow(hPager, Rct.left, Rct.top, ChildXSize, Rct.bottom - Rct.top, 1));
}

// -----------------------------------------------------------------------
// Retrieve the child of a notified pager
HWND CALLBACK PagerGetNotifiedChild(long lParam)
{
    return((HWND) GetWindowLong(ControlGetNotifiedhWnd(lParam), GWL_USERDATA));
}

// -----------------------------------------------------------------------
// Set the background color of a pager control
long CALLBACK PagerSetBackColor(HWND hPager, COLORREF Color)
{
    return(SendMessage(hPager, PGM_SETBKCOLOR, 0, Color));
}

// -----------------------------------------------------------------------
// Retrieve the background color of a pager control
COLORREF CALLBACK PagerGetBackColor(HWND hPager)
{
    return(SendMessage(hPager, PGM_GETBKCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the used border size of a pager control
long CALLBACK PagerGetBorderSize(HWND hPager)
{
    return(SendMessage(hPager, PGM_GETBORDER, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the used button size of a pager control
long CALLBACK PagerGetButtonSize(HWND hPager)
{
    return(SendMessage(hPager, PGM_GETBUTTONSIZE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the scroll position in a pager control
long CALLBACK PagerGetScrollPos(HWND hPager)
{
    return(SendMessage(hPager, PGM_GETPOS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the orientation of a pager control
long CALLBACK PagerGetOrientation(HWND hPager)
{
    return(GetWindowLong(hPager, GWL_STYLE) & PGS_HORZ);
}

// -----------------------------------------------------------------------
// Set the orientation of a pager control
long CALLBACK PagerSetOrientation(HWND hPager, long Orientation)
{
    if(Orientation != PGS_HORZ) return(SetWindowLong(hPager, GWL_STYLE, GetWindowLong(hPager, GWL_STYLE) & ~PGS_HORZ));
    else return(SetWindowLong(hPager, GWL_STYLE, GetWindowLong(hPager, GWL_STYLE) | PGS_HORZ));
}

// -----------------------------------------------------------------------
// Set a new orientation for a pager control
long CALLBACK PagerChangeOrientation(HWND hPager, LPNMPGCALCSIZE CalcSizeNotify,
                                       long NewOrientation, LPSIZE NewSize)
{
    long OldOrientation;

    OldOrientation = PagerGetOrientation(hPager);
    if(NewOrientation == PGS_VERT)
    {
        if(OldOrientation != PGS_VERT)
        {
            CalcSizeNotify->dwFlag = PGF_CALCHEIGHT;
            OldOrientation = SetWindowLong(hPager, GWL_STYLE, GetWindowLong(hPager, GWL_STYLE) & ~PGS_HORZ);
        }
    }
    else
    {
        if(OldOrientation != PGS_HORZ)
        {
            CalcSizeNotify->dwFlag = PGF_CALCWIDTH;
            OldOrientation = SetWindowLong(hPager, GWL_STYLE, GetWindowLong(hPager, GWL_STYLE) | PGS_HORZ);
        }
    }
    PagerDisplaySetSize(CalcSizeNotify, NewSize);
    return(OldOrientation);
}

// -----------------------------------------------------------------------
// Client controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a MDI client
HWND CALLBACK CreateClient(HWND hParent, HMENU hMenuWindowList, HICON hIcon)
{
    HWND ReturnValue = 0;
    CLIENTCREATESTRUCT MyClient;

    MyClient.hWindowMenu = hMenuWindowList;
    MyClient.idFirstChild = 0;
    ReturnValue = CreateWindowEx(WS_EX_RIGHTSCROLLBAR | WS_EX_CLIENTEDGE | WS_EX_LTRREADING, "MDICLIENT", "", WS_CLIPCHILDREN | WS_CHILD | WS_HSCROLL | WS_VSCROLL, 0, 0, 0, 0, hParent, 0, WALocalhInst, &MyClient);
    if(ReturnValue == 0) return(0);
    ControlSetSerif8(ReturnValue);
    DialogSetIcon(ReturnValue, hIcon);
    ShowWindow(ReturnValue, SW_SHOW);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Resize the client array of a window
long CALLBACK ClientResize(HWND hClient, long AddX, long AddY, long SubX, long SubY)
{
    RECT Rct;
    HWND hParent = 0;

    hParent = GetParent(hClient);
    GetClientRect(hParent, &Rct);
    return(SetWindowPos(hClient, 0, Rct.left + AddX, Rct.top + AddY, Rct.right - SubX, Rct.bottom - SubY, SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_NOACTIVATE));
}

// -----------------------------------------------------------------------
// Retrieve active child of a MDI client
HWND CALLBACK ClientGetActiveChild(HWND hClient)
{
    return((HWND) SendMessage(hClient, WM_MDIGETACTIVE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Focus on next child of a MDI client 
HWND CALLBACK ClientSetNextChild(HWND hClient)
{
    return((HWND) SendMessage(hClient, WM_MDINEXT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Focus on previous child of a MDI client
HWND CALLBACK ClientSetPreviousChild(HWND hClient)
{
    return((HWND) SendMessage(hClient, WM_MDINEXT, 0, (long) 1));
}

// -----------------------------------------------------------------------
// Tile childs windows of a MDI client horizontally
long CALLBACK ClientTileHorizontal(HWND hClient)
{
    return(SendMessage(hClient, WM_MDITILE, MDITILE_HORIZONTAL, (long) 0));
}

// -----------------------------------------------------------------------
// Tile childs windows of a MDI client vertically
long CALLBACK ClientTileVertical(HWND hClient)
{
    return(SendMessage(hClient, WM_MDITILE, MDITILE_VERTICAL, (long) 0));
}

// -----------------------------------------------------------------------
// Cascade childs windows of a MDI client
long CALLBACK ClientTileCascade(HWND hClient)
{
    return(SendMessage(hClient, WM_MDICASCADE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Arrange iconic childs windows of a MDI client
long CALLBACK ClientTileArrangeIconic(HWND hClient)
{
    return(SendMessage(hClient, WM_MDIICONARRANGE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Radio button controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a radio button control
HWND CALLBACK CreateRadioButton(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent,
                                  CStr BText, long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(WS_EX_TRANSPARENT, "BUTTON", BText.Get_String(), WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_LEFT | BS_VCENTER | BS_AUTORADIOBUTTON | BS_MULTILINE | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve click state of a radio button
long CALLBACK RadioButtonGetState(HWND hRB)
{
    return(SendMessage(hRB, BM_GETCHECK, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set click state of a radio button
long CALLBACK RadioButtonSetState(HWND hRB, long CheckedState)
{
    return(SendMessage(hRB, BM_SETCHECK, CheckedState, (long) 0));
}

// -----------------------------------------------------------------------
// Listview controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a listview control
HWND CALLBACK CreateListView(long LVLeft, long LVTop, long LVWidth, long LVHeight, HWND hParent,
                               long CtrlID, HIMAGELIST hImageList, WNDPROC WindowProc, long ExtraStyle,
                               long WStyle, long ExtraExStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, LVLeft, LVTop, LVWidth, LVHeight);
    ReturnValue = CreateWindowEx(WS_EX_NOPARENTNOTIFY | ExtraExStyle, "SysListView32", "", WS_HSCROLL | WS_VSCROLL | WS_VISIBLE | WS_CHILD | LVS_SHOWSELALWAYS | LVS_SHAREIMAGELISTS | WStyle, LVLeft, LVTop, LVWidth, LVHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlSetSerif8(ReturnValue);
    if(hImageList != 0) SendMessage(ReturnValue, LVM_SETIMAGELIST, LVSIL_SMALL, (long) hImageList);
    if(ExtraStyle != 0) SendMessage(ReturnValue, LVM_SETEXTENDEDLISTVIEWSTYLE, ExtraStyle, ExtraStyle);
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add a column to a listview control
long CALLBACK ListViewAddCol(HWND hListview, CStr LVColText, long LVWidth, long ColPosition)
{
    RECT CRect;
    LV_COLUMN ListViewColumn;

    GetClientRect(hListview, &CRect);
    if(LVWidth < 0) LVWidth = CRect.right / -(long) LVWidth;
    ListViewColumn.mask = LVCF_FMT | LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
    ListViewColumn.fmt = LVCFMT_LEFT;
    ListViewColumn.cx = LVWidth;
    ListViewColumn.pszText = LVColText.Get_String();
    ListViewColumn.cchTextMax = LVColText.Len();
    ListViewColumn.iSubItem = ColPosition;
    return(SendMessage(hListview, LVM_INSERTCOLUMN, 0, (long) &ListViewColumn));
}

// -----------------------------------------------------------------------
// Set a column width of a listview control
long CALLBACK ListViewSetColWidth(HWND hListview, long LVColNumber, long LVWidth)
{
    return(SendMessage(hListview, LVM_SETCOLUMNWIDTH, LVColNumber, LVWidth));
}

// -----------------------------------------------------------------------
// Retrieve a column width of a listview control
long CALLBACK ListViewGetColWidth(HWND hListview, long LVColNumber)
{
    return(SendMessage(hListview, LVM_GETCOLUMNWIDTH, LVColNumber, (long) 0));
}

// -----------------------------------------------------------------------
// Set a position in a listview
long CALLBACK ListViewSetScroll(HWND hLV, long DeltaX, long DeltaY)
{
    return(SendMessage(hLV, LVM_SCROLL, DeltaX, DeltaY));
}

// -----------------------------------------------------------------------
// Clear a listview
long CALLBACK ListViewClear(HWND hLV)
{
    return(SendMessage(hLV, LVM_DELETEALLITEMS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Detach an imagelist from a listview
long CALLBACK ListViewDetachImageList(HWND hLV)
{
    return(SendMessage(hLV, LVM_SETIMAGELIST, LVSIL_SMALL, (long) 0));
}

// -----------------------------------------------------------------------
// Add an item to a listview control
long CALLBACK ListViewAddItem(HWND hListview, CStr LVItemText, long LVItemPos, long LVImage)
{
    LV_ITEM ListViewItem;

    memset(&ListViewItem, 0, sizeof(ListViewItem));
    ListViewItem.mask = LVIF_STATE | LVIF_PARAM | LVIF_IMAGE;
    if(LVItemText.Len() != 0) ListViewItem.mask = ListViewItem.mask | LVIF_TEXT;
    ListViewItem.iItem = LVItemPos;
    ListViewItem.iSubItem = 0;
    ListViewItem.pszText = LVItemText.Get_String();
    ListViewItem.state = 0;
    ListViewItem.stateMask = 0;
    ListViewItem.iImage = LVImage;
    ListViewItem.cchTextMax = LVItemText.Len();
    // Stored here for sorting purpose
    ListViewItem.lParam = LVItemPos;
    return(SendMessage(hListview, LVM_INSERTITEM, 0, (long) &ListViewItem));
}

// -----------------------------------------------------------------------
// Delete an item from a listview control
long CALLBACK ListViewDeleteItem(HWND hListview, long LVItemPos)
{
    return(SendMessage(hListview, LVM_DELETEITEM, LVItemPos, (long) 0));
}

// -----------------------------------------------------------------------
// Add a subitem to a listview control
long CALLBACK ListViewSetSubItem(HWND hListview, CStr LVItemText, long LVItemPos, long LVSubItemPos)
{
    LV_ITEM ListViewItem;

    memset(&ListViewItem, 0, sizeof(ListViewItem));
    ListViewItem.mask = LVIF_TEXT;
    ListViewItem.iItem = LVItemPos;
    ListViewItem.iSubItem = LVSubItemPos;
    ListViewItem.pszText = LVItemText.Get_String();
    ListViewItem.cchTextMax = LVItemText.Len();
    ListViewItem.lParam = LVItemPos;
    return(SendMessage(hListview, LVM_SETITEM, 0, (long) &ListViewItem));
}

// -----------------------------------------------------------------------
// Add a subitem icon to a listview control
long CALLBACK ListViewSetSubItemImage(HWND hListview, long LVItemImage, long LVItemPos, long LVSubItemPos)
{
    LV_ITEM ListViewItem;

    ListViewItem.mask = LVIF_IMAGE;
    ListViewItem.iItem = LVItemPos;
    ListViewItem.iSubItem = LVSubItemPos;
    ListViewItem.iImage = LVItemImage;
    ListViewItem.lParam = LVItemPos;
    return(SendMessage(hListview, LVM_SETITEM, 0, (long) &ListViewItem));
}

// -----------------------------------------------------------------------
// Retrieve subitem icon to a listview control
long CALLBACK ListViewGetSubItemImage(HWND hListview, long LVItemPos, long LVSubItemPos)
{
    LV_ITEM ListViewItem;
    long LVItemImage = 0;

    ListViewItem.mask = LVIF_IMAGE;
    ListViewItem.iItem = LVItemPos;
    ListViewItem.iSubItem = LVSubItemPos;
    ListViewItem.iImage = LVItemImage;
    ListViewItem.lParam = LVItemPos;
    SendMessage(hListview, LVM_GETITEM, 0, (long) &ListViewItem);
    return(ListViewItem.iImage);
}

// -----------------------------------------------------------------------
// Retrieve the editing result state of a listview control
long CALLBACK ListViewEditValidated(long lParam)
{
    long ReturnValue = 0;
    LV_DISPINFO HeaderNotify;

    RtlMoveMemory(&HeaderNotify, (void *) lParam, sizeof(HeaderNotify));
    if(HeaderNotify.item.pszText != 0) ReturnValue = 1;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the editing result of a listview control
CStr CALLBACK ListViewGetEditResult(long lParam)
{
    CStr ReturnValue;
    LV_DISPINFO HeaderNotify;

    RtlMoveMemory(&HeaderNotify, (void *) lParam, sizeof(HeaderNotify));
    if(HeaderNotify.item.pszText != 0)
    {
        ReturnValue = HeaderNotify.item.pszText;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the number of items of a listview control
long CALLBACK ListViewItemCount(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETITEMCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number of selected items of a listview control
long CALLBACK ListViewSelItemCount(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETSELECTEDCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve first selected item of a listview control
long CALLBACK ListViewMultiSelFirstItem(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETSELECTIONMARK, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve selection state of a listview item
long CALLBACK ListViewGetItemSelState(HWND hListview, long LVItemIndex)
{
    return(SendMessage(hListview, LVM_GETITEMSTATE, LVItemIndex, LVIS_FOCUSED));
}

// -----------------------------------------------------------------------
// Display the edit control a listview and return the handle of the text control
HWND CALLBACK ListViewSetEditModeOn(HWND hListview, long LVItemIndex, long ExtraEditStyle)
{
    SendMessage(hListview, LVM_EDITLABELA, LVItemIndex, (long) 0);
    return(ListViewGetEditControl(hListview));
}

// -----------------------------------------------------------------------
// Set the selection state of a listview item
long CALLBACK ListViewSetItemSel(HWND hListview, long LVItemIndex)
{
    LV_ITEM GetListViewItem;

    GetListViewItem.mask = LVIF_STATE;
    GetListViewItem.state = LVIS_SELECTED | LVIS_FOCUSED;
    GetListViewItem.stateMask = LVIS_SELECTED | LVIS_FOCUSED;
    GetListViewItem.iItem = LVItemIndex;
    GetListViewItem.iSubItem = 0;
    GetListViewItem.pszText = 0;
    GetListViewItem.cchTextMax = 0;
    return(SendMessage(hListview, LVM_SETITEMSTATE, LVItemIndex, (long) &GetListViewItem));
}

// -----------------------------------------------------------------------
// Set the highlight state of a listview item
long CALLBACK ListViewSetItemHighlight(HWND hListview, long LVItemIndex, long Enable)
{
    LV_ITEM GetListViewItem;

    GetListViewItem.mask = LVIF_STATE;
    if(Enable)
    {
        GetListViewItem.state = LVIS_DROPHILITED;
    }
    else
    {
        GetListViewItem.state = 0;
    }
    GetListViewItem.stateMask = LVIS_DROPHILITED;
    GetListViewItem.iItem = LVItemIndex;
    GetListViewItem.iSubItem = 0;
    GetListViewItem.pszText = 0;
    GetListViewItem.cchTextMax = 0;
    return(SendMessage(hListview, LVM_SETITEMSTATE, LVItemIndex, (long) &GetListViewItem));
}

// -----------------------------------------------------------------------
// Set the focus state of a listview item
long CALLBACK ListViewSetItemFocus(HWND hListview, long LVItemIndex)
{
    LV_ITEM GetListViewItem;

    GetListViewItem.mask = LVIF_STATE;
    GetListViewItem.state = LVIS_FOCUSED;
    GetListViewItem.stateMask = LVIS_FOCUSED;
    GetListViewItem.iItem = LVItemIndex;
    GetListViewItem.iSubItem = 0;
    GetListViewItem.pszText = 0;
    GetListViewItem.cchTextMax = 0;
    return(SendMessage(hListview, LVM_SETITEMSTATE, LVItemIndex, (long) &GetListViewItem));
}

// -----------------------------------------------------------------------
// Retrieve the item number currently under the cursor
long CALLBACK ListViewGetItemUnderCursor(HWND hListview)
{
    LV_HITTESTINFO MyTest;

    memset(&MyTest, 0, sizeof(MyTest));
    GetCursorPos(&MyTest.pt);
    MyTest.pt.x = MyTest.pt.x - ControlLeft(hListview);
    MyTest.pt.y = MyTest.pt.y - ControlTop(hListview);
    SendMessage(hListview, LVM_HITTEST, 0, (long) &MyTest);
    return(MyTest.iItem);
}

// -----------------------------------------------------------------------
// Retrieve selection state of the first selected item in a listview
long CALLBACK ListViewGetFirstFocusItem(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETNEXTITEM, -1, LVNI_FOCUSED));
}

// -----------------------------------------------------------------------
// Retrieve selection state of the next selected item in a listview
long CALLBACK ListViewGetSelItem(HWND hListview, long LVIndex)
{
    return(SendMessage(hListview, LVM_GETNEXTITEM, LVIndex, LVNI_SELECTED));
}

// -----------------------------------------------------------------------
// Retrieve selection state of the listview selected item
CStr CALLBACK ListViewGetSelItemText(HWND hListview, long LVSubItemNumber)
{
    return(ListViewGetItemText(hListview, ListViewGetFirstFocusItem(hListview), LVSubItemNumber));
}

// -----------------------------------------------------------------------
// Set the text of a selected item
long CALLBACK ListViewSetSelItemText(HWND hListview, CStr LVItemText, long LVSubItemNumber)
{
    return(ListViewSetSubItem(hListview, LVItemText, ListViewGetSelItem(hListview, -1), LVSubItemNumber));
}

// -----------------------------------------------------------------------
// Retrieve selection state of a listview item
long CALLBACK ListViewCheckBoxItemDoubleClick(HWND hListview)
{
    long CurrentLvItem = 0;

    CurrentLvItem = ListViewGetItemUnderCursor(hListview);
    if(CurrentLvItem != -1)
    {
        ListViewSetItemSel(hListview, CurrentLvItem);
        switch(ListViewGetItemCheckbox(hListview, CurrentLvItem))
        {
            case 0:
                ListViewSetItemCheckbox(hListview, CurrentLvItem, 1);
                break;
            case 1:
                ListViewSetItemCheckbox(hListview, CurrentLvItem, 0);
                break;
        }
    }
    return(CurrentLvItem);
}

// -----------------------------------------------------------------------
// Retrieve selection state of a listview item
CStr CALLBACK ListViewGetItemText(HWND hListview, long LVItemIndex, long LVSubItemNumber)
{
    CStr ReturnValue;
    LV_ITEM GetListViewItem;
    CStr LVItemTxt;

    LVItemTxt = LVItemTxt.String(1024, 1);
    LVItemTxt.Get_String()[0] = 0;
    GetListViewItem.mask = LVIF_TEXT;
    GetListViewItem.iItem = LVItemIndex;
    GetListViewItem.iSubItem = LVSubItemNumber;
    GetListViewItem.pszText = LVItemTxt.Get_String();
    GetListViewItem.cchTextMax = 1024;
    SendMessage(hListview, LVM_GETITEM, 0, (long) &GetListViewItem);
    ReturnValue = GetListViewItem.pszText;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the icon associated with an item of a listview item
HICON CALLBACK ListViewGetItemIcon(HWND hListview, long LVItemIndex, long LVSubItemNumber)
{
    LV_ITEM GetListViewItem;

    GetListViewItem.mask = LVIF_IMAGE;
    GetListViewItem.iItem = LVItemIndex;
    GetListViewItem.iSubItem = LVSubItemNumber;
    GetListViewItem.pszText = "";
    GetListViewItem.cchTextMax = 0;
    GetListViewItem.iImage = 0;
    SendMessage(hListview, LVM_GETITEM, 0, (long) &GetListViewItem);
    return((HICON) GetListViewItem.iImage);
}

// -----------------------------------------------------------------------
// Retrieve the column number of a notified listview event
long CALLBACK ListViewGetNotifiedColumnIndex(long lParam)
{
    NM_LISTVIEW SubItemNotify;

    RtlMoveMemory(&SubItemNotify, (void *) lParam, sizeof(SubItemNotify));
    return(SubItemNotify.iSubItem);
}

// -----------------------------------------------------------------------
// Sort a listview by item
long CALLBACK ListViewSort(HWND hListview, long LVSubItemNumber, PFNLVCOMPARE SortProc)
{
    return(SendMessage(hListview, LVM_SORTITEMS, LVSubItemNumber, (long) SortProc));
}

// -----------------------------------------------------------------------
// Reorder a listview by lparam
long CALLBACK ListViewReOrder(HWND hListview)
{
    LV_ITEM ReordItem;
    long i;
    long ReturnValue;

    ReordItem.mask = LVIF_PARAM;
    ReordItem.iItem = 0;
    ReordItem.iSubItem = 0;
    for(i = 0; (i <= ListViewItemCount(hListview) - 1) ; i++)
    {
        ReordItem.lParam = ReordItem.iItem;
        ReturnValue = SendMessage(hListview, LVM_SETITEM, 0, (long) &ReordItem);
        if(ReturnValue == 0) break;
        ReordItem.iItem++;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve listview column label
CStr CALLBACK ListViewGetHeaderLabel(HWND hListview, long LVColNumber)
{
    CStr ReturnValue;
    LV_COLUMN MyColHead;
    CStr LVItemTxt;
    
    LVItemTxt = ReturnValue.String(1024, 1);
    LVItemTxt.Get_String()[0] = 0;
    MyColHead.mask = LVCF_TEXT;
    MyColHead.fmt = 0;
    MyColHead.cx = 0;
    MyColHead.pszText = LVItemTxt.Get_String();
    MyColHead.cchTextMax = 1024;
    MyColHead.iSubItem = 0;
    SendMessage(hListview, LVM_GETCOLUMN, LVColNumber, (long) &MyColHead);
    ReturnValue = MyColHead.pszText;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve listview column position
long CALLBACK ListViewGetHeaderPosition(HWND hListview, long LVColNumber)
{
    LV_COLUMN MyColHead;
    
    MyColHead.mask = LVCF_ORDER;
    MyColHead.fmt = 0;
    MyColHead.cx = 0;
    MyColHead.pszText = 0;
    MyColHead.cchTextMax = 0;
    MyColHead.iSubItem = 0;
    MyColHead.iOrder = 0;
    SendMessage(hListview, LVM_GETCOLUMN, LVColNumber, (long) &MyColHead);
    return(MyColHead.iOrder);
}

// -----------------------------------------------------------------------
// Set listview column position
long CALLBACK ListViewSetHeaderPosition(HWND hListview, long LVColNumber, long NewPosition)
{
    LV_COLUMN MyColHead;
    
    MyColHead.mask = LVCF_ORDER;
    MyColHead.fmt = 0;
    MyColHead.cx = 0;
    MyColHead.pszText = 0;
    MyColHead.cchTextMax = 0;
    MyColHead.iSubItem = 0;
    MyColHead.iOrder = NewPosition;
    return(SendMessage(hListview, LVM_SETCOLUMN, LVColNumber, (long) &MyColHead));
}

// -----------------------------------------------------------------------
// Change the background color of a listview
long CALLBACK ListViewSetBackColor(HWND hListview, COLORREF LVColor)
{
    return(SendMessage(hListview, LVM_SETBKCOLOR, 0, (long) LVColor));
}

// -----------------------------------------------------------------------
// Retrieve the background color of a listview
COLORREF CALLBACK ListViewGetBackColor(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETBKCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the text background color of a listview
COLORREF CALLBACK ListViewGetTextBackColor(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETTEXTBKCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the text color of a listview
COLORREF CALLBACK ListViewGetTextColor(HWND hListview) {
    return(SendMessage(hListview, LVM_GETTEXTCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the tooltips control associated with a listview
long CALLBACK ListViewGetToolTips(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETTOOLTIPS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the top index item of a listview
long CALLBACK ListViewGetTopIndex(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETTOPINDEX, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number of items that can fit vertically in a listview
long CALLBACK ListViewGetCountPerPage(HWND hListview) {
    return(SendMessage(hListview, LVM_GETCOUNTPERPAGE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the handle of the edit control of a listview
HWND CALLBACK ListViewGetEditControl(HWND hListview)
{
    return((HWND) SendMessage(hListview, LVM_GETEDITCONTROL, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the extended styles of a listview
long CALLBACK ListViewGetExStyle(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETEXTENDEDLISTVIEWSTYLE, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the sysheader of a listview
HWND CALLBACK ListViewGetSysHeader(HWND hListview)
{
    return((HWND) SendMessage(hListview, LVM_GETHEADER, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the hot cursor of a listview
long CALLBACK ListViewGetHotCursor(HWND hListview)
{
    return(SendMessage(hListview, LVM_GETHOTCURSOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the imagelist associated with a listview
HIMAGELIST CALLBACK ListViewGetImageList(HWND hListview, long ListType)
{
    return((HIMAGELIST) SendMessage(hListview, LVM_GETIMAGELIST, ListType, (long) 0));
}

// -----------------------------------------------------------------------
// Set the checkbox state of an item in a listview control
long CALLBACK ListViewSetItemCheckbox(HWND hListview, long LVItemIndex, long LVCheckState)
{
    LV_ITEM GetListViewItem;
 
    GetListViewItem.mask = LVIF_STATE;
    switch(LVCheckState)
    {
        case 0:
            GetListViewItem.state = 0x1000;
            break;
        default:
            GetListViewItem.state = 0x2000;
            break;
    }
    GetListViewItem.stateMask = LVIS_STATEIMAGEMASK;
    GetListViewItem.iItem = LVItemIndex;
    GetListViewItem.iSubItem = 0;
    GetListViewItem.pszText = 0;
    GetListViewItem.cchTextMax = 0;
    return(SendMessage(hListview, LVM_SETITEMSTATE, LVItemIndex, (long) &GetListViewItem));
}

// -----------------------------------------------------------------------
// Retrieve the checkbox state of an item in a listview control
long CALLBACK ListViewGetItemCheckbox(HWND hListview, long LVItemIndex)
{
    long ReturnValue = 0;

    switch(SendMessage(hListview, LVM_GETITEMSTATE, LVItemIndex, LVIS_STATEIMAGEMASK))
    {
        case 0x1000:
            ReturnValue = 0;
            break;
        case 0x2000:
            ReturnValue = 1;
            break;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if the notification message is a checkbox changes
long CALLBACK ListViewIsCheckboxNotify(HWND hListview, long LVItemIndex)
{
    long ReturnValue = 0;

    switch(SendMessage(hListview, LVM_GETITEMSTATE, LVItemIndex, LVIS_STATEIMAGEMASK))
    {
        case 0x1000:
            ReturnValue = 1;
            break;
        case 0x2000:
            ReturnValue = 1;
            break;
        default:
            ReturnValue = 0;
            break;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Search an item in a listview control
long CALLBACK ListViewFindItem(HWND hListview, CStr LVItemText)
{
    LV_FINDINFO ListViewFind;

    ListViewFind.flags = LVFI_STRING;
    ListViewFind.psz = LVItemText.Get_String();
    ListViewFind.lParam = 0;
    ListViewFind.pt.x = 0;
    ListViewFind.pt.y = 0;
    ListViewFind.vkDirection = 0;
    return(SendMessage(hListview, LVM_FINDITEM, -1, (long) &ListViewFind));
}

// -----------------------------------------------------------------------
// Search a subitem in a listview control
long CALLBACK ListViewFindSubItem(HWND hListview, CStr TextToFind, long SubItemNbr, long SearchCase)
{
    long ReturnValue = 0;
    long i = 0;

    ReturnValue = -1;
    if(SearchCase == CASE_SENSITIVE)
    {
        for(i = 0 ; (i <= ListViewItemCount(hListview) - 1) ; i++)
        {
            if(ListViewGetItemText(hListview, i, SubItemNbr) == TextToFind)
            {
                ReturnValue = i;
                break;
            }
        }
    }
    else
    {
        TextToFind = TextToFind.Upper_Case();
        for(i = 0 ; i <= ListViewItemCount(hListview) - 1 ; i++)
        {
            if((ListViewGetItemText(hListview, i, SubItemNbr)).Upper_Case() == TextToFind)
            {
                ReturnValue = i;
                break;
            }
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Begin a dragging operation in a listview 
long CALLBACK ListViewBeginDrag(long lParam, HWND hParent, LPPOINT DragRect)
{
    long ReturnValue = 0;
    NM_LISTVIEW NmList;
    HIMAGELIST LVImageList = 0;
    POINT CursLVPos;

    ReturnValue = 0;
    RtlMoveMemory(&NmList, (void *) lParam, sizeof(NmList));
    LVImageList = (HIMAGELIST) SendMessage(NmList.hdr.hwndFrom, LVM_CREATEDRAGIMAGE, NmList.iItem, (long) &NmList.ptAction);
    if(LVImageList != 0)
    {
        SetCapture(hParent);
        GetCursorPos(&CursLVPos);
        CursLVPos.x = CursLVPos.x - ControlLeft(NmList.hdr.hwndFrom);
        CursLVPos.y = CursLVPos.y - ControlTop(NmList.hdr.hwndFrom);
        ImageList_BeginDrag(LVImageList, 0, CursLVPos.x, CursLVPos.y);
        ImageList_DragEnter(0, ControlLeft(NmList.hdr.hwndFrom) + CursLVPos.x, ControlTop(NmList.hdr.hwndFrom) + CursLVPos.y + NmList.ptAction.y);
        DragRect->y = NmList.ptAction.y;
        ReturnValue = 1;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Perform a dragging move operation in a listview
long CALLBACK ListViewMoveDrag(LPPOINT DragRect)
{
    POINT CursLVPos;

    GetCursorPos(&CursLVPos);
    return(ImageList_DragMove(CursLVPos.x, CursLVPos.y + DragRect->y));
}

// -----------------------------------------------------------------------
// Terminate a dragging operation in a listview
void CALLBACK ListViewEndDrag(HWND hParent)
{
    ImageList_EndDrag();
    ImageList_DragLeave(hParent);
    ReleaseCapture();
}

// -----------------------------------------------------------------------
// Create and paste listview edit control
long CALLBACK ListViewPasteAutoEdit(HWND hListview, long wParam, long ExtraEditStyle)
{
    long ReturnValue = 0;
    HWND LVEdithwnd = 0;
    CStr ChartoSend;
    long CurrentSelected = 0;
    long LV_TextbStyle = 0;

    switch(wParam)
    {
        case 8:
            break;
        default:
            CurrentSelected = ListViewGetFirstFocusItem(hListview);
            if(CurrentSelected != -1)
            {
                ListViewSetItemSel(hListview, CurrentSelected);
                ChartoSend = ChartoSend.Chr(wParam);
                ChartoSend = ChartoSend.Lower_Case();
                if((GetKeyState(VK_LSHIFT) & 0x80) != 0) ChartoSend = ChartoSend.Upper_Case();
                if((GetKeyState(VK_RSHIFT) & 0x80) != 0) ChartoSend = ChartoSend.Upper_Case();
                if((GetKeyState(VK_CAPITAL) & 1) != 0) ChartoSend = ChartoSend.Upper_Case();
                LVEdithwnd = ListViewSetEditModeOn(hListview, CurrentSelected, ExtraEditStyle);
                LV_TextbStyle = GetWindowLong(LVEdithwnd, GWL_STYLE);
                if((LV_TextbStyle & ES_NUMBER) != 0)
                {
                    if(StringIsDigitChar(ChartoSend) == 0) goto NumberRestricted;
                }
                ControlSetText(LVEdithwnd, ChartoSend);
NumberRestricted:;
                TextBoxSetCaretPos(LVEdithwnd, -1);
                ReturnValue = 1;
            }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Treeview controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a treeview control 
HWND CALLBACK CreateTreeView(long TVLeft, long TVTop, long TVWidth, long TVHeight, HWND hParent, long CtrlID,
                               HIMAGELIST hImageList, WNDPROC WindowProc, long ExtraStyle, long ExtraExStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, TVLeft, TVTop, TVWidth, TVHeight);
    ReturnValue = CreateWindowEx(ExtraExStyle, "SysTreeView32", "", WS_VISIBLE | WS_CHILD | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT | ExtraStyle, TVLeft, TVTop, TVWidth, TVHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    SendMessage(ReturnValue, TVM_SETIMAGELIST, TVSIL_NORMAL, (long) hImageList);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add an item to a treeview control
HTREEITEM CALLBACK TreeViewAddItem(HWND hTreeView, CStr TVItemText, HTREEITEM TVParent, HTREEITEM TVChildPos,
                                   long TVImage, long TVSelImage, long ExtraStyle, long RefreshParentAfterAdd)
{
    HTREEITEM ReturnValue = 0;
    HTREEITEM ParentToCheck = 0;
    TV_INSERTSTRUCT TreeViewItem;

    memset(&TreeViewItem, 0, sizeof(TreeViewItem));
    TreeViewItem.hParent = TVParent;
    TreeViewItem.hInsertAfter = TVChildPos;
    TreeViewItem.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_STATE;
    TreeViewItem.item.iImage = TVImage;
    TreeViewItem.item.iSelectedImage = TVSelImage;
    TreeViewItem.item.pszText = TVItemText.Get_String();
    TreeViewItem.item.cchTextMax = TVItemText.Len();
    TreeViewItem.item.cChildren = 1;
    TreeViewItem.item.state = ExtraStyle;
    TreeViewItem.item.stateMask = ExtraStyle;
    ReturnValue = (HTREEITEM) SendMessage(hTreeView, TVM_INSERTITEM, 0, (long) &TreeViewItem);
    if(RefreshParentAfterAdd != 0)
    {
        ParentToCheck = TreeViewGetItemParent(hTreeView, ReturnValue);
        if(TreeViewGetChildItemsCount(hTreeView, ParentToCheck) == 1) ControlRefreshLocal(hTreeView);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the treeview tooltips
long CALLBACK TreeViewSetToolTips(HWND hTreeView, long hToolTips)
{
    return(SendMessage(hTreeView, TVM_SETTOOLTIPS, hToolTips, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the treeview tooltips
long CALLBACK TreeViewGetToolTips(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETTOOLTIPS, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set the treeview members indentation
long CALLBACK TreeViewSetIndent(HWND hTreeView, long TVIndent)
{
    return(SendMessage(hTreeView, TVM_SETINDENT, TVIndent, (long) 0));
}

// -----------------------------------------------------------------------
// Remove treeview items
long CALLBACK TreeViewRemoveItem(HWND hTreeView, HTREEITEM hItem)
{
    return(SendMessage(hTreeView, TVM_DELETEITEM, 0, (long) hItem));
}

// -----------------------------------------------------------------------
// Set treeview background color
long CALLBACK TreeViewSetBackColor(HWND hTreeView, COLORREF TVColor)
{
    return(SendMessage(hTreeView, TVM_SETBKCOLOR, 0, TVColor));
}

// -----------------------------------------------------------------------
// Retrieve treeview expanding node state
long CALLBACK TreeViewGetExpandingState(long lParam)
{
    NM_TREEVIEWEXPAND NmTree;

    RtlMoveMemory(&NmTree, (void *) lParam, sizeof(NmTree));
    return(NmTree.action);
}

// -----------------------------------------------------------------------
// Retrieve treeview expanding node handle
HTREEITEM CALLBACK TreeViewGetExpandingItem(long lParam)
{
    NM_TREEVIEWEXPAND NmTree;

    RtlMoveMemory(&NmTree, (void *)lParam, sizeof(NmTree));
    return((HTREEITEM) NmTree.itemNew9);
}

// -----------------------------------------------------------------------
// Retrieve treeview deleted node handle
HTREEITEM CALLBACK TreeViewGetDeletedItem(long lParam)
{
    NM_TREEVIEWEXPAND NmTree;

    RtlMoveMemory(&NmTree, (void *)lParam, sizeof(NmTree));
    return((HTREEITEM) NmTree.itemNew);
}

// -----------------------------------------------------------------------
// Set the image displayed with a treeview entry
long CALLBACK TreeViewSetItemIcon(HWND hTreeView, HTREEITEM hItem, long IconNumber)
{
    TV_ITEM TreeViewItem;

    memset(&TreeViewItem, 0, sizeof(TreeViewItem));
    TreeViewItem.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    TreeViewItem.hItem = hItem;
    TreeViewItem.iImage = IconNumber;
    TreeViewItem.iSelectedImage = IconNumber;
    return(SendMessage(hTreeView, TVM_SETITEM, 0, (long) &TreeViewItem));
}

// -----------------------------------------------------------------------
// Set the expanding state of a treeview entry
long CALLBACK TreeViewSetItemExpandedState(HWND hTreeView, HTREEITEM hItem, long ExpandState)
{
    TV_ITEM TreeViewItem;

    memset(&TreeViewItem, 0, sizeof(TreeViewItem));
    TreeViewItem.hItem = hItem;
    TreeViewItem.mask = TVIF_STATE;
    TreeViewItem.stateMask = TVIS_EXPANDED;
    TreeViewItem.state = 0;
    if(ExpandState != 0) TreeViewItem.state = TreeViewItem.state | TVIS_EXPANDED;
    return(SendMessage(hTreeView, TVM_SETITEM, 0, (long) &TreeViewItem));
}

// -----------------------------------------------------------------------
// Retrieve the current expanded state of a treeview entry
long CALLBACK TreeViewGetItemExpandedState(HWND hTreeView, HTREEITEM hItem)
{
    TV_ITEM TreeViewItem;

    memset(&TreeViewItem, 0, sizeof(TreeViewItem));
    TreeViewItem.hItem = hItem;
    TreeViewItem.mask = TVIF_STATE;
    TreeViewItem.stateMask = TVIS_EXPANDED;
    TreeViewItem.state = 0;
    SendMessage(hTreeView, TVM_GETITEM, 0, (long) &TreeViewItem);
    return(TreeViewItem.state & TVIS_EXPANDED);
}

// -----------------------------------------------------------------------
// Set the expanding type of a treeview control
long CALLBACK TreeViewSetSingleExpandState(HWND hTreeView, long ExpandState)
{
    if(ExpandState != 0) return(SetWindowLong(hTreeView, GWL_STYLE, GetWindowLong(hTreeView, GWL_STYLE) | TVS_SINGLEEXPAND));
    else return(SetWindowLong(hTreeView, GWL_STYLE, GetWindowLong(hTreeView, GWL_STYLE) & (~TVS_SINGLEEXPAND)));
}

// -----------------------------------------------------------------------
// Retrieve the handle of a selected treeview entry
HTREEITEM CALLBACK TreeViewGetSelectedItem(HWND hTreeView)
{
    return((HTREEITEM) SendMessage(hTreeView, TVM_GETNEXTITEM, TVGN_CARET, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the handle of a treeview entry under cursor
HTREEITEM CALLBACK TreeViewGetItemFromPos(HWND hTreeView, long XPos, long YPos)
{
    TV_HITTESTINFO THit;

    memset(&THit, 0, sizeof(THit));
    THit.pt.x = XPos;
    THit.pt.y = YPos;
    return((HTREEITEM) SendMessage(hTreeView, TVM_HITTEST, 0, (long) &THit));
}

// -----------------------------------------------------------------------
// Select an item in a treeview control
long CALLBACK TreeViewSetSelectedItem(HWND hTreeView, HTREEITEM hItem)
{
    return(SendMessage(hTreeView, TVM_SELECTITEM, TVGN_CARET, (long) hItem));
}

// -----------------------------------------------------------------------
// Retrieve the text of an item of a treeview control
CStr CALLBACK TreeViewGetItemText(HWND hTreeView, HTREEITEM hItem)
{
    CStr ReturnValue;
    TV_ITEM TreeViewItem;
    CStr TVItemTxt;

    memset(&TreeViewItem, 0, sizeof(TreeViewItem));
    TVItemTxt = ReturnValue.String(1024, 1);
    TVItemTxt.Get_String()[0] = 0;
    TreeViewItem.mask = TVIF_TEXT | TVIF_HANDLE;
    TreeViewItem.hItem = hItem;
    TreeViewItem.cchTextMax = 1024;
    TreeViewItem.pszText = TVItemTxt.Get_String();
    SendMessage(hTreeView, TVM_GETITEM, 0, (long) &TreeViewItem);
    ReturnValue = TreeViewItem.pszText;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the text of an item of a treeview control
long CALLBACK TreeViewSetItemText(HWND hTreeView, HTREEITEM hItem, CStr ItemText)
{
    CStr ReturnValue;
    TV_ITEM TreeViewItem;

    memset(&TreeViewItem, 0, sizeof(TreeViewItem));
    TreeViewItem.mask = TVIF_TEXT | TVIF_HANDLE;
    TreeViewItem.hItem = hItem;
    TreeViewItem.cchTextMax = ItemText.Len();
    TreeViewItem.pszText = ItemText.Get_String();
    return(SendMessage(hTreeView, TVM_SETITEM, 0, (long) &TreeViewItem));
}

// -----------------------------------------------------------------------
// Retrieve the parent handle of a treeview entry
HTREEITEM CALLBACK TreeViewGetItemParent(HWND hTreeView, HTREEITEM hItem)
{
    return((HTREEITEM) SendMessage(hTreeView, TVM_GETNEXTITEM, TVGN_PARENT, (long) hItem));
}

// -----------------------------------------------------------------------
// Retrieve the parent handle of a treeview entry
HTREEITEM CALLBACK TreeViewGetRoot(HWND hTreeView)
{
    return((HTREEITEM) SendMessage(hTreeView, TVM_GETNEXTITEM, TVGN_ROOT, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number of treeview entries
long CALLBACK TreeViewGetItemCount(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number visible of treeview entries
long CALLBACK TreeViewGetVisibleCount(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETVISIBLECOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the first child handle of a treeview entry
HTREEITEM CALLBACK TreeViewGetFirstItemChild(HWND hTreeView, HTREEITEM hItem)
{
    return((HTREEITEM) SendMessage(hTreeView, TVM_GETNEXTITEM, TVGN_CHILD, (long) hItem));
}

// -----------------------------------------------------------------------
// Retrieve the next item of a treeview entry
HTREEITEM CALLBACK TreeViewGetNextItem(HWND hTreeView, HTREEITEM hItem)
{
    return((HTREEITEM) SendMessage(hTreeView, TVM_GETNEXTITEM, TVGN_NEXT, (long) hItem));
}

// -----------------------------------------------------------------------
// Retrieve the previous item of a treeview entry
HTREEITEM CALLBACK TreeViewGetPreviousItem(HWND hTreeView, HTREEITEM hItem)
{
    return((HTREEITEM) SendMessage(hTreeView, TVM_GETNEXTITEM, TVGN_PREVIOUS, (long) hItem));
}

// -----------------------------------------------------------------------
// Search an item text in a treeview node
long CALLBACK TreeViewSearchItemText(HWND hTreeView, HTREEITEM hNode, CStr ItemText)
{
    long ReturnValue = -1;
    HTREEITEM TmpChild = 0;
    CStr TmpChildText;
    long i = 0;

    TmpChild = TreeViewGetFirstItemChild(hTreeView, hNode);
    while(TmpChild != 0)
    {
        TmpChildText = TreeViewGetItemText(hTreeView, TmpChild);
        if(_strcmpi(TmpChildText.Get_String(), ItemText.Get_String()) == 0)
        {
            ReturnValue = i;
            break;
        }
        TmpChild = TreeViewGetNextItem(hTreeView, TmpChild);
        i++;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Search a partial text in a treeview node
long CALLBACK TreeViewSearchChildPartialText(HWND hTreeView, HTREEITEM hNode, CStr ItemText)
{
    long ReturnValue = -1;
    HTREEITEM TmpChild = 0;
    CStr TmpChildText;
    long i = 0;

    TmpChild = TreeViewGetFirstItemChild(hTreeView, hNode);
    while(TmpChild != 0)
    {
        TmpChildText = TreeViewGetItemText(hTreeView, TmpChild);
        if(TmpChildText.In_Str(1,ItemText,Text_Compare) != 0)
        {
            ReturnValue = i;
            break;
        }
        TmpChild = TreeViewGetNextItem(hTreeView, TmpChild);
        i++;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return the number of childs of a treeview node
long CALLBACK TreeViewGetChildItemsCount(HWND hTreeView, HTREEITEM hNode)
{
    HTREEITEM TmpChild = 0;
    long i = 0;

    TmpChild = TreeViewGetFirstItemChild(hTreeView, hNode);
    while(TmpChild != 0)
    {
        i++;
        TmpChild = TreeViewGetNextItem(hTreeView, TmpChild);
    }
    return(i);
}

// -----------------------------------------------------------------------
// Return the position of the selected child of a treeview node
long CALLBACK TreeViewGetSelChildItemPos(HWND hTreeView, HTREEITEM hNode, HTREEITEM hParent)
{
    HTREEITEM TmpChild;
    long i = 0;

    TmpChild = TreeViewGetFirstItemChild(hTreeView, hParent);
    while(TmpChild != 0)
    {
        if(hNode == TmpChild) break;
        i++;
        TmpChild = TreeViewGetNextItem(hTreeView, TmpChild);
    }
    return(i);
}

// -----------------------------------------------------------------------
// Return the level of a treeview node from the root entry (root=0)
long CALLBACK TreeViewGetItemLevel(HWND hTreeView, HTREEITEM hNode)
{
    HTREEITEM TmpChild = 0;
    long i = 0;

    TmpChild = TreeViewGetItemParent(hTreeView, hNode);
    while(TmpChild != 0)
    {
        i++;
        TmpChild = TreeViewGetItemParent(hTreeView, TmpChild);
    }
    return(i);
}

// -----------------------------------------------------------------------
// Get the used backcolor of a treeview
COLORREF CALLBACK TreeViewGetBackColor(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETBKCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Get the coords of the text of a treeview item
long CALLBACK TreeViewGetItemTextRect(HWND hTreeView, HTREEITEM hItem, LPRECT ItemRect)
{
    ItemRect->left = (long) hItem;
    return(SendMessage(hTreeView, TVM_GETITEMRECT, 1, (long) ItemRect));
}

// -----------------------------------------------------------------------
// Get the coords of a complete treeview item
long CALLBACK TreeViewGetItemRect(HWND hTreeView, HTREEITEM hItem, LPRECT ItemRect)
{
    ItemRect->left = (long) hItem;
    return(SendMessage(hTreeView, TVM_GETITEMRECT, 0, (long) ItemRect));
}

// -----------------------------------------------------------------------
// Retrieve the handle of the edit control of a treeview
HWND CALLBACK TreeViewGetEditControl(HWND hTreeView)
{
    return((HWND) SendMessage(hTreeView, TVM_GETEDITCONTROL, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the imagelist associated with a treeview
HIMAGELIST CALLBACK TreeViewGetImageList(HWND hTreeView, long ListType)
{
    return((HIMAGELIST) SendMessage(hTreeView, TVM_GETIMAGELIST, ListType, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve pixels indent of child nodes of a treeview
long CALLBACK TreeViewGetIndent(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETINDENT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the color of the insertion mark of a treeview
COLORREF CALLBACK TreeViewGetInsertMarkColor(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETINSERTMARKCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the color of lines of a treeview
COLORREF CALLBACK TreeViewGetLineColor(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETLINECOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the color of the texts of a treeview
COLORREF CALLBACK TreeViewGetTextColor(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETTEXTCOLOR, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the height of an element of a treeview
long CALLBACK TreeViewGetItemHeight(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETITEMHEIGHT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the scrolltime of a treeview
long CALLBACK TreeViewGetScrollTime(HWND hTreeView)
{
    return(SendMessage(hTreeView, TVM_GETSCROLLTIME, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Fictive controls
// -----------------------------------------------------------------------

// Create a fictive control
HWND CALLBACK CreateFictive(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent,
                              long CtrlID, WNDPROC WindowProc, HICON hImage)
{
    HWND ReturnValue = 0;

    ReturnValue = CreateButton(BLeft, BTop, BWidth, BHeight, hParent, "", CtrlID, IMAGE_ICON, hImage, WindowProc, BS_CENTER | BS_VCENTER, 0);
    if(ReturnValue == 0) return(0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// ToolTips controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a tooltip control
HWND CALLBACK CreateToolTip(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID,
                              WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(0, "tooltips_class32", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | TTS_NOPREFIX | TTS_ALWAYSTIP | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    SetWindowPos(ReturnValue, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Animation controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create an animation control
HWND CALLBACK CreateAnimation(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent,
                                long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(0, "SysAnimate32", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Hotkey controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a Hotkey control
HWND CALLBACK CreateHotkey(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID,
                             WNDPROC WindowProc, long InvalidCombination, long ValidCombination, long VirtualKey,
                             long Modifiers, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(WS_EX_CLIENTEDGE, "msctls_hotkey32", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    HotkeySetRules(ReturnValue, InvalidCombination, ValidCombination);
    HotkeySetKey(ReturnValue, VirtualKey, Modifiers);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the rules of a Hotkey control
long CALLBACK HotkeySetRules(HWND hHotkey, long InvalidCombination, long ValidCombination)
{
    return(SendMessage(hHotkey, HKM_SETRULES, InvalidCombination, ValidCombination));
}

// -----------------------------------------------------------------------
// Set the key combination of a Hotkey control
long CALLBACK HotkeySetKey(HWND hHotkey, long VirtualKey, long Modifiers)
{
    return(SendMessage(hHotkey, HKM_SETHOTKEY, ((VirtualKey * 0x100) + Modifiers), (long) 0));
}

// -----------------------------------------------------------------------
// Date/Time picker controls
// -----------------------------------------------------------------------

// Create a date/time picker control
HWND CALLBACK CreateDateTimePicker(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent,
                                     long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(WS_EX_CLIENTEDGE, "SysDateTimePick32", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Month calendar controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a month calendar control
HWND CALLBACK CreateMonthCalendar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, long CtrlID,
                                    WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(WS_EX_CLIENTEDGE, "SysMonthCal32", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | WS_BORDER | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Header controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a Header control
HWND CALLBACK CreateHeader(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText,
                             long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(0, "SysHeader32", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    ControlSetText(ReturnValue, BText);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add an item to a header control
long CALLBACK WAHeaderAddItem(HWND hHeader, CStr HeaderText, long Headerwidth, long HeaderPosition, long ExtraStyle)
{
    HDITEM HeaderItem;

    HeaderItem.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH;
    HeaderItem.pszText = HeaderText.Get_String();
    HeaderItem.cchTextMax = HeaderText.Len();
    HeaderItem.cxy = Headerwidth;
    HeaderItem.fmt = HDF_LEFT | HDF_STRING;
    HeaderItem.fmt = HeaderItem.fmt | ExtraStyle;
    return(SendMessage(hHeader, HDM_INSERTITEM, HeaderPosition, (long) &HeaderItem));
}

// -----------------------------------------------------------------------
// Extended Combobox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create an extended combobox control
HWND CALLBACK CreateExComboBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText,
                                 long CtrlID, WNDPROC WindowProc, HIMAGELIST hImageList, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(0, "ComboBoxEx32", "", WS_TABSTOP | WS_VSCROLL | WS_VISIBLE | WS_CHILD | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    ControlSetText(ReturnValue, BText);
    WAExCBSetImageList(ReturnValue, hImageList);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the imagelist of an extended combobox
long CALLBACK WAExCBSetImageList(HWND hExCB, HIMAGELIST hImageList)
{
    return(SendMessage(hExCB, CBEM_SETIMAGELIST, 0, (long) hImageList));
}

// -----------------------------------------------------------------------
// Combobox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a combobox control
HWND CALLBACK CreateComboBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent, CStr BText,
                               long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;
    long Test = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(0, "COMBOBOX", "", WS_TABSTOP | WS_VSCROLL | WS_VISIBLE | WS_CHILD | CBS_NOINTEGRALHEIGHT | CBS_AUTOHSCROLL | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    ControlSetText(ReturnValue, BText);
    Test = GetWindowLong(ReturnValue, GWL_EXSTYLE);
    Test = Test & (~ WS_EX_CLIENTEDGE);
    SetWindowLong(ReturnValue, GWL_EXSTYLE, Test);
    Test = GetWindowLong(ReturnValue, GWL_STYLE);
    Test = Test & (~ WS_BORDER);
    SetWindowLong(ReturnValue, GWL_STYLE, Test);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add an item to a combobox control
long CALLBACK ComboBoxAddItem(HWND hCB, CStr CBItemText, long CBItemIndex)
{
    return(SendMessage(hCB, CB_INSERTSTRING, CBItemIndex, (long) CBItemText.Get_String()));
}

// -----------------------------------------------------------------------
// Delete an item from a combobox control
long CALLBACK ComboBoxDeleteItem(HWND hCB, long CBItemIndex)
{
    return(SendMessage(hCB, CB_DELETESTRING, CBItemIndex, (long) 0));
}

// -----------------------------------------------------------------------
// Add an item to a combobox control
long CALLBACK ComboBoxReset(HWND hCB)
{
    return(SendMessage(hCB, CB_RESETCONTENT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set the chars maximum in a combobox control
long CALLBACK ComboBoxSetMaxLen(HWND hCB, long TBMaxLen)
{
    return(SendMessage(hCB, CB_LIMITTEXT, TBMaxLen, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the number of items of a combobox control
long CALLBACK ComboBoxCount(HWND hCB)
{
    return(SendMessage(hCB, CB_GETCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the current selected index of a combobox control
long CALLBACK ComboBoxGetIndex(HWND hCB)
{
    return(SendMessage(hCB, CB_GETCURSEL, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set the current selected index of a combobox control
long CALLBACK ComboBoxSetIndex(HWND hCB, long CBIndex)
{
    return(SendMessage(hCB, CB_SETCURSEL, CBIndex, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve an item from a combobox control
CStr CALLBACK ComboBoxGetItem(HWND hCB, long CBIndex)
{
    CStr ReturnValue;
    
    if(CBIndex != -1)
    {
        ReturnValue = ReturnValue.String(SendMessage(hCB, CB_GETLBTEXTLEN, CBIndex, (long) 0), 1);
        SendMessage(hCB, CB_GETLBTEXT, CBIndex, (long) ReturnValue.Get_String());
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve current item from a combobox control
CStr CALLBACK ComboBoxGetCurrentItem(HWND hCB)
{
    CStr ReturnValue;
    long CBIdx = 0;
    
    CBIdx = ComboBoxGetIndex(hCB);
    ReturnValue = ReturnValue.String(SendMessage(hCB, CB_GETLBTEXTLEN, CBIdx, (long) 0), 1);
    SendMessage(hCB, CB_GETLBTEXT, CBIdx, (long) ReturnValue.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if an item exists in a combobox control
long CALLBACK ComboBoxItemExist(HWND hCB, CStr CBItem)
{
    return(SendMessage(hCB, CB_FINDSTRINGEXACT, -1, (long) CBItem.Get_String()));
}

// -----------------------------------------------------------------------
// Retrieve the associated 32 bits value from an item of a combobox
long CALLBACK ComboBoxGetItemData(HWND hLB, long CBIndex)
{
    return(SendMessage(hLB, CB_GETITEMDATA, CBIndex, (long) 0));
}

// -----------------------------------------------------------------------
// Save combobox entries in an ini file
void ComboBoxSaveInIniFile(HWND hCombo, CStr EntryToAdd, CStr IniKey, CStr IniFile)
{
    int i = 0;
    int j = 0;
    long SearchArg = 0;

    if(EntryToAdd.Len() != 0)
    {
        IniDeleteKey(IniKey, "", IniFile);
        for(i = 0; i <= ComboBoxCount(hCombo) - 1; i++)
        {
            IniWriteKey(IniKey, (CStr) "Entry" + (CStr) StringNumberComplement(i, 3).Get_String(), ComboBoxGetItem(hCombo, i), IniFile);
        }
        SearchArg = 0;
        for(j = 0; j <= ComboBoxCount(hCombo) - 1; j++)
        {
            if(_strcmpi(ComboBoxGetItem(hCombo, j).Get_String(), EntryToAdd.Get_String()) == 0)
            {
                SearchArg = 1;
                break;
            }
        }
        if(SearchArg == 0) IniWriteKey(IniKey, (CStr) "Entry" + (CStr) StringNumberComplement(i, 3).Get_String(), EntryToAdd, IniFile);
    }
}

// -----------------------------------------------------------------------
// Load a combobox with ini file entries
void ComboBoxFillFromIniFile(HWND hCombo, CStr IniKey, CStr IniFile)
{
    int i;
    CStr ArgToAdd;

    for(i = 0; i <= 999; i++)
    {
        ArgToAdd = IniReadKey(IniKey, (CStr) "Entry" + (CStr) StringNumberComplement(i, 3).Get_String(), IniFile);
        if(ArgToAdd.Len() == 0) break;
        ComboBoxAddItem(hCombo, ArgToAdd, -1);
    }
}

// -----------------------------------------------------------------------
// Listbox controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a listbox control
HWND CALLBACK CreateListBox(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent,
                            long CtrlID, WNDPROC WindowProc, long DragOn, long ExtraStyle, long ExtraExStyle)
{
    HWND ReturnValue = 0;
    
    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    if((ExtraExStyle & WS_EX_STATICEDGE) != 0)
    {
        BLeft++;
        BTop++;
        BWidth = BWidth - 2;
        BHeight = BHeight - 2;
    }
    ReturnValue = CreateWindowEx(ExtraExStyle, "LISTBOX", "", WS_TABSTOP | WS_VSCROLL | WS_VISIBLE | WS_CHILD | LBS_NOINTEGRALHEIGHT | LBS_NOTIFY | LBS_HASSTRINGS | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == 0) return(0);
    if(DragOn != 0) MakeDragList(ReturnValue);
    ControlHookWin(ReturnValue, WindowProc);
    ControlSetSerif8(ReturnValue);
    ListBoxSetHorzScrollWidth(ReturnValue, 0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add an item to a listbox control
long CALLBACK ListBoxAddItem(HWND hLB, CStr LBItemText, long LBItemIndex)
{
    return(SendMessage(hLB, LB_INSERTSTRING, LBItemIndex, (long) LBItemText.Get_String()));
}

// -----------------------------------------------------------------------
// Clear the entries of a listbox control
long CALLBACK ListBoxReset(HWND hLB)
{
    return(SendMessage(hLB, LB_RESETCONTENT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set the horizontal internal width of a listbox control
long CALLBACK ListBoxSetHorzScrollWidth(HWND hLB, long LBWidth)
{
    long HSize = LBWidth;
    if(HSize < ControlClientWidth(hLB)) HSize = 0;
    if(HSize == 0)
    {
        SetWindowLong(hLB, GWL_STYLE, GetWindowLong(hLB, GWL_STYLE) & ~WM_HSCROLL);
    }
    else
    {
        SetWindowLong(hLB, GWL_STYLE, GetWindowLong(hLB, GWL_STYLE) | WM_HSCROLL);
        if(GetWindowLong(hLB, GWL_STYLE) & WM_VSCROLL)
        {
            HSize += GetSystemMetrics(SM_CXVSCROLL);
        }
    }
    return(SendMessage(hLB, LB_SETHORIZONTALEXTENT, HSize, (long) 0));
}

// -----------------------------------------------------------------------
// Set the width of a listbox control column
long CALLBACK ListBoxSetColumnsWidth(HWND hLB, long LBWidth)
{
    return(SendMessage(hLB, LB_SETCOLUMNWIDTH, LBWidth, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve number of items in a listbox control
long CALLBACK ListBoxCount(HWND hLB)
{
    return(SendMessage(hLB, LB_GETCOUNT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the top index of a listbox
long CALLBACK ListBoxGetTopIndex(HWND hLB)
{
    return(SendMessage(hLB, LB_GETTOPINDEX, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set the top index of a listbox
long CALLBACK ListBoxSetTopIndex(HWND hLB, long LBIndex)
{
    return(SendMessage(hLB, LB_SETTOPINDEX, LBIndex, (long) 0));
}

// -----------------------------------------------------------------------
// Associate a 32 bits value to an item of a listbox
long CALLBACK ListBoxSetItemData(HWND hLB, long LBIndex, long LBDat)
{
    return(SendMessage(hLB, LB_SETITEMDATA, LBIndex, LBDat));
}

// -----------------------------------------------------------------------
// Retrieve the associated 32 bits value from an item of a listbox
long CALLBACK ListBoxGetItemData(HWND hLB, long LBIndex)
{
    return(SendMessage(hLB, LB_GETITEMDATA, LBIndex, (long) 0));
}

// -----------------------------------------------------------------------
// Check if an item is selected of a listbox
long CALLBACK ListBoxIsItemSelected(HWND hLB, long LBIndex)
{
    return(SendMessage(hLB, LB_GETSEL, LBIndex, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the current selected index of a listbox
long CALLBACK ListBoxGetSelItemIndex(HWND hLB)
{
    return(SendMessage(hLB, LB_GETCURSEL, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set the current selected index of a listbox
long CALLBACK ListBoxSetIndex(HWND hLB, long LBIndex)
{
    return(SendMessage(hLB, LB_SETCURSEL, LBIndex, (long) 0));
}

// -----------------------------------------------------------------------
// Retrieve the current item from a listbox
CStr CALLBACK ListBoxGetCurrentItem(HWND hLB)
{
    return(ListBoxGetItem(hLB, ListBoxGetSelItemIndex(hLB)));
}

// -----------------------------------------------------------------------
// Retrieve an item from a listbox
CStr CALLBACK ListBoxGetItem(HWND hLB, long LBIndex)
{
    CStr ReturnValue;
    long ItemLen = 0;

    ItemLen = SendMessage(hLB, LB_GETTEXTLEN, LBIndex, (long) 0);
    if(ItemLen != LB_ERR)
    {
        ReturnValue = ReturnValue.String(ItemLen, 1);
        SendMessage(hLB, LB_GETTEXT, LBIndex, (long) ReturnValue.Get_String());
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Delete an item from a listbox
long CALLBACK ListBoxDeleteItem(HWND hLB, long LBIndex)
{
    return(SendMessage(hLB, LB_DELETESTRING, LBIndex, (long) 0));
}

// -----------------------------------------------------------------------
// Replace an item from a listbox
long CALLBACK ListBoxReplaceItem(HWND hLB, long LBIndex, CStr LBNewItemText)
{
    SendMessage(hLB, LB_DELETESTRING, LBIndex, (long) 0);
    return(SendMessage(hLB, LB_INSERTSTRING, LBIndex, (long) LBNewItemText.Get_String()));
}

// -----------------------------------------------------------------------
// Replace current item from a listbox
long CALLBACK ListBoxReplaceSelItem(HWND hLB, CStr LBNewItemText)
{
    long LbCurIdx = 0;

    LbCurIdx = ListBoxGetSelItemIndex(hLB);
    SendMessage(hLB, LB_DELETESTRING, LbCurIdx, (long) 0);
    return(SendMessage(hLB, LB_INSERTSTRING, LbCurIdx, (long) LBNewItemText.Get_String()));
}

// -----------------------------------------------------------------------
// Process drag listbox message
void CALLBACK ListBoxProcessDrag(HWND hLB, long Message, long lParam, long DragCursor)
{
    DRAGLISTINFO DragMsg;
    long NewIdx = 0;
    CStr OldListEntry;
    long NbrCopy = 0;
    long i = 0;
    long j = 0;
    
    if(Message != WADragListMsg) return;
    // Local copy of the message datas
    RtlMoveMemory(&DragMsg, (void *) lParam, sizeof(DragMsg));
    switch(DragMsg.uNotification)
    {
        // Begin the dragging
        case DL_BEGINDRAG:
            if(ListBoxCount(hLB) <= 1) return;
            FirstDraggingIndex = LBItemFromPt(DragMsg.hWnd, DragMsg.ptCursor, 0);
//            FirstDraggingIndex = LBItemFromPt(DragMsg.hWnd, DragMsg.ptCursor.x, DragMsg.ptCursor.y, 0);
            OldDragCursor = SetCursor(LoadCursor(WALocalhInst, MAKEINTRESOURCE(DragCursor)));
            return;
        // Cancel the dragging
        case DL_CANCELDRAG:
            if(ListBoxCount(hLB) <= 1) return;
            SetCursor(OldDragCursor);
            return;
        case DL_DROPPED:
            if(ListBoxCount(hLB) <= 1) return;
            SetCursor(OldDragCursor);
            // Check if new index is same as old one
            NewIdx = LBItemFromPt(DragMsg.hWnd, DragMsg.ptCursor, 0);
//            NewIdx = LBItemFromPt(DragMsg.hWnd, DragMsg.ptCursor.x, DragMsg.ptCursor.y, 0);
            if(NewIdx != FirstDraggingIndex)
            {
                if(NewIdx != -1)
                {
                    NbrCopy = FirstDraggingIndex - NewIdx;
                    // Save old entry
                    OldListEntry = ListBoxGetItem(hLB, FirstDraggingIndex);
                    if(NbrCopy > 0)
                    {
                        // Entry up
                        i = FirstDraggingIndex;
                        j = 0;
                        while(j != NbrCopy)
                        {
                            ListBoxDeleteItem(hLB, i);
                            ListBoxAddItem(hLB, ListBoxGetItem(hLB, i - 1), i);
                            i--;
                            j++;
                        }
                        ListBoxDeleteItem(hLB, NewIdx);
                        ListBoxAddItem(hLB, OldListEntry, NewIdx);
                    }
                    else
                    {
                        // Entry down
                        NbrCopy = -(long) NbrCopy;
                        i = FirstDraggingIndex;
                        j = 0;
                        while(j != NbrCopy)
                        {
                            ListBoxDeleteItem(hLB, i);
                            ListBoxAddItem(hLB, ListBoxGetItem(hLB, i), i);
                            i++;
                            j++;
                        }
                        ListBoxDeleteItem(hLB, NewIdx);
                        ListBoxAddItem(hLB, OldListEntry, NewIdx);
                    }
                    ListBoxSetIndex(hLB, NewIdx);
                }
            }
            return;
    }
}

// -----------------------------------------------------------------------
// Push an item down in a listbox
long CALLBACK ListBoxSelItemDown(HWND hLB)
{
    CStr OldItem;
    long CurrentIdx = 0;
    long ReturnValue = 0;

    if(ListBoxGetSelItemIndex(hLB) != -1)
    {
        // Max down
        if(ListBoxGetSelItemIndex(hLB) == (ListBoxCount(hLB) - 1)) return(ReturnValue);
        OldItem = ListBoxGetCurrentItem(hLB);
        CurrentIdx = ListBoxGetSelItemIndex(hLB);
        ListBoxReplaceItem(hLB, CurrentIdx, ListBoxGetItem(hLB, CurrentIdx + 1));
        ListBoxReplaceItem(hLB, CurrentIdx + 1, OldItem);
        ListBoxSetIndex(hLB, CurrentIdx + 1);
        ReturnValue = 1;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Push an item up in a listbox
long CALLBACK ListBoxSelItemUp(HWND hLB)
{
    CStr OldItem;
    long CurrentIdx = 0;
    long ReturnValue = 0;

    if(ListBoxGetSelItemIndex(hLB) > 0)
    {
        // Max up
        if(ListBoxCount(hLB) == 0) return(ReturnValue);
        OldItem = ListBoxGetItem(hLB, ListBoxGetSelItemIndex(hLB) - 1);
        CurrentIdx = ListBoxGetSelItemIndex(hLB);
        ListBoxReplaceItem(hLB, CurrentIdx - 1, ListBoxGetItem(hLB, CurrentIdx));
        ListBoxReplaceItem(hLB, CurrentIdx, OldItem);
        ListBoxSetIndex(hLB, CurrentIdx - 1);
        ReturnValue = 1;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Remove an item in a listbox
long CALLBACK ListBoxSelItemRemove(HWND hLB)
{
    long CurrentIdx = 0;
    long NewLBCount = 0;
    long ReturnValue = 0;

    if(ListBoxGetSelItemIndex(hLB) != -1)
    {
        if(ListBoxCount(hLB) == 0) return(ReturnValue);
        CurrentIdx = ListBoxGetSelItemIndex(hLB);
        ReturnValue = ListBoxDeleteItem(hLB, CurrentIdx);
        NewLBCount = ListBoxCount(hLB);
        if(NewLBCount == 0)
        {
            ListBoxSetIndex(hLB, 0);
            return(ReturnValue);
        }
        if(CurrentIdx >= NewLBCount) CurrentIdx--;
        ListBoxSetIndex(hLB, CurrentIdx);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if an item exists
long CALLBACK ListBoxItemExist(HWND hListBox, CStr CBItem)
{
    return(SendMessage(hListBox, LB_FINDSTRINGEXACT, -1, (long) CBItem.Get_String()));
}

// -----------------------------------------------------------------------
// Return the index of the item under the cursor
long CALLBACK ListBoxItemUnderCursor(HWND hListBox)
{
    POINT ItemPoint;

    GetCursorPos(&ItemPoint);
//    return(LBItemFromPt(hListBox, ItemPoint.x, ItemPoint.y, 0));
    return(LBItemFromPt(hListBox, ItemPoint, 0));
}

// -----------------------------------------------------------------------
// Scrollbars controls
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a Scrollbar control
HWND CALLBACK CreateScrollBar(long BLeft, long BTop, long BWidth, long BHeight, HWND hParent,
                                long CtrlID, WNDPROC WindowProc, long ExtraStyle)
{
    HWND ReturnValue = 0;

    ControlBound(hParent, BLeft, BTop, BWidth, BHeight);
    ReturnValue = CreateWindowEx(0, "SCROLLBAR", "", WS_TABSTOP | WS_VISIBLE | WS_CHILD | ExtraStyle, BLeft, BTop, BWidth, BHeight, hParent, (HMENU) CtrlID, WALocalhInst, NULL);
    if(ReturnValue == NULL) return(0);
    ControlHookWin(ReturnValue, WindowProc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the maximum value of a scrollbar
long CALLBACK ScrollBarGetMinRange(HWND hWnd, long ScrollBarType)
{
    SCROLLINFO ScrollInf;

    ScrollInf.cbSize = sizeof(ScrollInf);
    ScrollInf.fMask = SIF_RANGE;
    GetScrollInfo(hWnd, ScrollBarType, &ScrollInf);
    return(ScrollInf.nMin);
}

// -----------------------------------------------------------------------
// Retrieve the maximum value of a scrollbar
long CALLBACK ScrollBarGetMaxRange(HWND hWnd, long ScrollBarType) {
    SCROLLINFO ScrollInf;

    ScrollInf.cbSize = sizeof(ScrollInf);
    ScrollInf.fMask = SIF_RANGE;
    GetScrollInfo(hWnd, ScrollBarType, &ScrollInf);
    return(ScrollInf.nMax);
}

// -----------------------------------------------------------------------
// Retrieve the value of page scrolling in a scrollbar
long CALLBACK ScrollBarGetPageRange(HWND hWnd, long ScrollBarType)
{
    SCROLLINFO ScrollInf;

    ScrollInf.cbSize = sizeof(ScrollInf);
    ScrollInf.fMask = SIF_PAGE;
    GetScrollInfo(hWnd, ScrollBarType, &ScrollInf);
    return(ScrollInf.nPage);
}

// -----------------------------------------------------------------------
// Set the value of page scrolling in a scrollbar
long CALLBACK ScrollBarSetPageRange(HWND hWnd, long ScrollBarType, long PageValue)
{
    SCROLLINFO ScrollInf;

    ScrollInf.cbSize = sizeof(ScrollInf);
    ScrollInf.fMask = SIF_PAGE;
    ScrollInf.nPage = PageValue;
    return(SetScrollInfo(hWnd, ScrollBarType, &ScrollInf, 1));
}

// -----------------------------------------------------------------------
// Set the minimun and maximum values of a scrollbar
long CALLBACK ScrollBarSetMinMaxRange(HWND hWnd, long ScrollBarType, long nMin, long nMax)
{
    if(nMax == nMin) nMax = nMin + 1;
    return(SetScrollRange(hWnd, ScrollBarType, nMin, nMax, 1));
}

// -----------------------------------------------------------------------
// Show or hide scrollbars
void CALLBACK ScrollBarShowHide(HWND hWnd, long ObjWidth, long ObjHeight, long Useclient)
{
    long XLarg = 0;
    long YLong = 0;
    RECT ScrollRect;
    SCROLLINFO ScrollInf;

    if(Useclient == 1) GetClientRect(hWnd, &ScrollRect);
    else GetWindowRect(hWnd, &ScrollRect);
    XLarg = ScrollRect.right - ScrollRect.left;
    YLong = ScrollRect.bottom - ScrollRect.top;
    if(XLarg > ObjWidth)
    {
        ShowScrollBar(hWnd, SB_HORZ, 0);
        SetScrollPos(hWnd, SB_HORZ, GetScrollPos(hWnd, SB_HORZ), 0);
    }
    else
    {
        ShowScrollBar(hWnd, SB_HORZ, 1);
        SetScrollPos(hWnd, SB_HORZ, GetScrollPos(hWnd, SB_HORZ), 0);
        ScrollInf.cbSize = sizeof(ScrollInf);
        ScrollInf.fMask = SIF_RANGE;
        ScrollInf.nMin = 0;
        ScrollInf.nMax = ObjWidth - XLarg;
        SetScrollInfo(hWnd, SB_HORZ, &ScrollInf, 1);
    }
    if(YLong > ObjHeight)
    {
        ShowScrollBar(hWnd, SB_VERT, 0);
        SetScrollPos(hWnd, SB_VERT, GetScrollPos(hWnd, SB_VERT), 0);
    }
    else
    {
        ShowScrollBar(hWnd, SB_VERT, 1);
        SetScrollPos(hWnd, SB_VERT, GetScrollPos(hWnd, SB_VERT), 0);
        RtlZeroMemory(&ScrollInf, sizeof(ScrollInf));
        ScrollInf.cbSize = sizeof(ScrollInf);
        ScrollInf.fMask = SIF_RANGE;
        ScrollInf.nMin = 0;
        ScrollInf.nMax = ObjHeight - YLong;
        SetScrollInfo(hWnd, SB_VERT, &ScrollInf, 1);
    }
}

// -----------------------------------------------------------------------
// Docking box
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a new docking box
HWND CALLBACK CreateDockingBox(CStr Title, WNDPROC WindowProc, HWND hRoot, char *IniFile)
{
    HWND ReturnValue;

    ReturnValue = CreateContainer(0, 0, 1, 1, 1, hRoot, Title, &DefaultDockingProc);
    if(ReturnValue == NULL) return(NULL);
    // Set it in allocated extra bytes (defined in the class)
    SetWindowLong(ReturnValue, 0, (long) WindowProc);
    // Another extra
    SetWindowLong(ReturnValue, 4, (long) IniFile);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return the current docking state of a docking box
long CALLBACK DockingBoxGetState(HWND hDock)
{
    return(cDock->GetWindowState(hDock));
}

// -----------------------------------------------------------------------
// Remove the specified window from the docking frame
void CALLBACK DockingBoxRemove(HWND hDock)
{
    IniWriteKey("Layout", "Dock" + ControlGetText(hDock) + "Visible", 0, (char *) GetWindowLong(hDock, 4));
    cDock->RemoveWindow(hDock);
}

// -----------------------------------------------------------------------
// Show project window
void CALLBACK DockingBoxShow(HWND hDock, int MaxXSize, int MaxYSize,
                             int DefaultXSize, int DefaultYSize,
                             int DefaultState)
{
    int DockState;
    RECT DockRect;
    int DockSize;
    int DefaultSizeX = DefaultXSize;
    int DefaultSizeY = DefaultYSize;

    if(!cDock->IsDockPane(hDock))
    {
        DockingBoxReadState(ControlGetText(hDock), DockState, DockRect, DockSize, DefaultState, DefaultSizeX, DefaultSizeY, (char *) GetWindowLong(hDock, 4));
        DefaultSizeX = DockRect.right - DockRect.left;
        DefaultSizeY = DockRect.bottom - DockRect.top;
        cDock->AddWindow(hDock, MaxXSize, MaxYSize, DefaultSizeX, DefaultSizeY);
        // DockSize is used for docked windows while 
        // DockRect is used for floating ones
        cDock->SetWindow(hDock, DockState, DockRect, DockSize);
    }
}

// -----------------------------------------------------------------------
// Read the latest visible state of a docking window
long CALLBACK DockingBoxWasVisible(CStr DockName, CStr IniFile)
{
    CStr InitValue;

    InitValue = IniReadKey("Layout", "Dock" + DockName + "Visible", IniFile);
    if(InitValue.Len() == 0) return(FALSE);
    else return(InitValue.Get_Long());
}

// -----------------------------------------------------------------------
// Repaint a docking box and all it's childs
void CALLBACK DockingBoxRepaint(HWND hWnd)
{
    ControlRefreshLocal(hWnd);
    EnumChildWindows(hWnd, &EnumDockChilds, 0);
}
                    
int CALLBACK EnumDockChilds(HWND hWnd, long lParam)
{
    ControlRefreshLocal(hWnd);
    return(TRUE);
}

// -----------------------------------------------------------------------
// Read the latest state of a docking window
void CALLBACK DockingBoxReadState(CStr DockName, int &State, RECT &Rect, int &Size, int DefaultState, int DefaultSizeX,
                                  int DefaultSizeY, CStr IniFile)
{
    CStr InitValue;

    if(!DefaultSizeX) DefaultSizeX = 160;
    if(!DefaultSizeY) DefaultSizeY = 120;

    InitValue = IniReadKey("Layout", "Dock" + DockName + "State", IniFile);
    if(InitValue.Len() == 0) State = DefaultState;
    else State = InitValue.Get_Long();

    InitValue = IniReadKey("Layout", "Dock" + DockName + "Left", IniFile);
    if(InitValue.Len() == 0)
    {
        Rect.left = 0;
        State = DefaultState;
    }
    else
    {
        Rect.left = InitValue.Get_Long();
    }
    if(Rect.left < 0) Rect.left = 0;

    InitValue = IniReadKey("Layout", "Dock" + DockName + "Top", IniFile);
    if(InitValue.Len() == 0)
    {
        Rect.top = 0;
        State = DefaultState;
    }
    else
    {
        Rect.top = InitValue.Get_Long();
    }
    if(Rect.top < 0) Rect.top = 0;

    InitValue = IniReadKey("Layout", "Dock" + DockName + "Width", IniFile);
    if(InitValue.Len() == 0)
    {
        Rect.left = 0;
        Rect.right = DefaultSizeX;
        State = DefaultState;
    }
    else
    {
        Rect.right = Rect.left + InitValue.Get_Long();
    }
    if(Rect.right < 0) Rect.right = 0;

    InitValue = IniReadKey("Layout", "Dock" + DockName + "Height", IniFile);
    if(InitValue.Len() == 0)
    {
        Rect.top = 0;
        Rect.bottom = DefaultSizeY;
        State = DefaultState;
    }
    else
    {
        Rect.bottom = Rect.top + InitValue.Get_Long();
    }
    if(Rect.bottom < 0) Rect.bottom = 0;

    if(IsRectEmpty(&Rect))
    {
        Rect.left = 0;
        Rect.right = DefaultSizeX;
        Rect.top = 0;
        Rect.bottom = DefaultSizeY;
    }

    switch(State)
    {
        case DOCKINGBOX_LEFT:
        case DOCKINGBOX_RIGHT:
            Size = Rect.right - Rect.left;
            break;
        case DOCKINGBOX_TOP:
        case DOCKINGBOX_BOTTOM:
            Size = Rect.bottom - Rect.top;
            break;
    }
}

// -----------------------------------------------------------------------
// Save the current state of a docking window
void CALLBACK DockingBoxSaveState(HWND hDock, CStr DockName, int Visible, CStr IniFile)
{
    int DockState;
    RECT DockRect;

    if(cDock->IsDockPane(hDock))
    {
        DockState = cDock->GetWindowState(hDock, &DockRect);
        if(DockRect.left < 0) DockRect.left = 0;
        if(DockRect.top < 0) DockRect.top = 0;
        if(DockRect.right < 0) DockRect.right = 0;
        if(DockRect.bottom < 0) DockRect.bottom = 0;
        IniWriteKey("Layout", "Dock" + DockName + "Visible", Visible, IniFile);
        IniWriteKey("Layout", "Dock" + DockName + "State", DockState, IniFile);
        IniWriteKey("Layout", "Dock" + DockName + "Left", DockRect.left, IniFile);
        IniWriteKey("Layout", "Dock" + DockName + "Top", DockRect.top, IniFile);
        IniWriteKey("Layout", "Dock" + DockName + "Width", DockRect.right - DockRect.left, IniFile);
        IniWriteKey("Layout", "Dock" + DockName + "Height", DockRect.bottom - DockRect.top, IniFile);
    }
}

// -----------------------------------------------------------------------
// Original docking box proc (private)
LRESULT CALLBACK DefaultDockingProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WNDPROC UserHook;
    
    switch(uMsg)
    {
        case WM_CREATE:
            return(0);
        case DOCKINGBOX_MSG_CLOSE:
            UserHook = (WNDPROC) GetWindowLong(hWnd, 0);
            if(UserHook != NULL)
            {
                if(IsBadCodePtr((FARPROC) UserHook) == 0) CallWindowProc(UserHook, hWnd, uMsg, wParam, lParam);
            }
            DockingBoxSaveState(hWnd, ControlGetText(hWnd), SendMessage(hWnd, DOCKINGBOX_MSG_QUERY_STATE, 0, 0), (char *) GetWindowLong(hWnd, 4));
            return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
        case WM_DESTROY:
            // Note: send a message DOCKINGBOX_MSG_QUERY_STATE to ourself
            DockingBoxSaveState(hWnd, ControlGetText(hWnd), SendMessage(hWnd, DOCKINGBOX_MSG_QUERY_STATE, 0, 0), (char *) GetWindowLong(hWnd, 4));
            return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
    }
    UserHook = (WNDPROC) GetWindowLong(hWnd, 0);
    if(UserHook != NULL)
    {
        if(IsBadCodePtr((FARPROC) UserHook) == 0) return(CallWindowProc(UserHook, hWnd, uMsg, wParam, lParam));
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));

}

// -----------------------------------------------------------------------
// Menus functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Retrieve menu text
CStr CALLBACK MenuGetString(HMENU hMenu, long ItemID)
{
    CStr ReturnValue;
    long MenuTxtLen = 0;

    MenuTxtLen = GetMenuString(hMenu, ItemID, "", 0, MF_BYCOMMAND);
    if(MenuTxtLen != 0) MenuTxtLen++;
    ReturnValue = ReturnValue.String(MenuTxtLen, 1);
    GetMenuString(hMenu, ItemID, ReturnValue.Get_String(), MenuTxtLen, MF_BYCOMMAND);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Enable/Disable a menu or menu item
long CALLBACK MenuEnable(HMENU hMenu, long ItemPosition, long Enable)
{
    if(Enable == 0) return(EnableMenuItem(hMenu, ItemPosition, MF_BYPOSITION | MF_DISABLED | MF_GRAYED));
    else return(EnableMenuItem(hMenu, ItemPosition, MF_BYPOSITION | MF_ENABLED));
}

// -----------------------------------------------------------------------
// Set a menu item to bold (default)
long CALLBACK MenuSetDefaultItem(HMENU hMenu, long ItemPosition)
{
    MENUITEMINFO InfosToChange;

    ZeroMemory(&InfosToChange, sizeof(MENUITEMINFO));
    InfosToChange.cbSize = sizeof(InfosToChange);
    InfosToChange.fMask = MIIM_STATE;
    InfosToChange.fState = MFS_DEFAULT;
    return(SetMenuItemInfo(hMenu, ItemPosition, 1, &InfosToChange));
}

// -----------------------------------------------------------------------
// Set a menu item state
long CALLBACK MenuSetItemState(HMENU hMenu, long ItemState, long ItemID)
{
    MENUITEMINFO InfosToChange;

    ZeroMemory(&InfosToChange, sizeof(MENUITEMINFO));
    InfosToChange.cbSize = sizeof(InfosToChange);
    InfosToChange.fMask = MIIM_STATE;
    InfosToChange.fState = ItemState;
    return(SetMenuItemInfo(hMenu, ItemID, 0, &InfosToChange));
}

// -----------------------------------------------------------------------
// Set a menu item type
long CALLBACK MenuSetItemType(HMENU hMenu, long ItemType, long ItemID)
{
    MENUITEMINFO InfosToChange;

    ZeroMemory(&InfosToChange, sizeof(MENUITEMINFO));
    InfosToChange.cbSize = sizeof(InfosToChange);
    InfosToChange.fMask = MIIM_TYPE;
    InfosToChange.fType = ItemType;
    return(SetMenuItemInfo(hMenu, ItemID, 0, &InfosToChange));
}

// -----------------------------------------------------------------------
// Set a menu item text
long CALLBACK MenuSetItemText(HMENU hMenu, CStr ItemText, long ItemID)
{
    MENUITEMINFO InfosToChange;

    ZeroMemory(&InfosToChange, sizeof(MENUITEMINFO));
    InfosToChange.cbSize = sizeof(InfosToChange);
    InfosToChange.fMask = MIIM_TYPE;
    InfosToChange.fType = MFT_STRING;
    InfosToChange.dwTypeData = ItemText.Get_String();
    return(SetMenuItemInfo(hMenu, ItemID, 0, &InfosToChange));
}

// -----------------------------------------------------------------------
// Add a separator entry into a menu bar
long CALLBACK MenuAddSeparator(HMENU hMenu)
{
    return(AppendMenu(hMenu, MF_SEPARATOR, -1, "-"));
}

// -----------------------------------------------------------------------
// Add a string entry into a menu bar
long CALLBACK MenuAddString(HMENU hMenu, CStr ItemText, long ItemID, long Enabled)
{
    return(AppendMenu(hMenu, MF_STRING | (Enabled ? 0 : MF_GRAYED), ItemID, ItemText.Get_String()));
}

// -----------------------------------------------------------------------
// Screen functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Get screen width
long CALLBACK ScreenWidth(void)
{
    return(GetSystemMetrics(SM_CXSCREEN));
}

// -----------------------------------------------------------------------
// Get screen height
long CALLBACK ScreenHeight(void)
{
    return(GetSystemMetrics(SM_CYSCREEN));
}

// -----------------------------------------------------------------------
// Convert a RECT structure from screen coordinate
// to given window coordinate
void CALLBACK ScreenRectToClient(HWND hWnd, LPRECT RectToFill)
{
    POINT ScreenPoint;
    
    ScreenPoint.x = RectToFill->left;
    ScreenPoint.y = RectToFill->top;
    ScreenToClient(hWnd, &ScreenPoint);
    RectToFill->left = ScreenPoint.x;
    RectToFill->top = ScreenPoint.y;
    ScreenPoint.x = RectToFill->right;
    ScreenPoint.y = RectToFill->bottom;
    ScreenToClient(hWnd, &ScreenPoint);
    RectToFill->right = ScreenPoint.x;
    RectToFill->bottom = ScreenPoint.y;
    return;
}
// -----------------------------------------------------------------------
// Cursor functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Display hourglass cursor
HCURSOR CALLBACK CursorSetWait(void)
{
    return(SetCursor(LoadCursor(0, IDC_WAIT)));
}

// -----------------------------------------------------------------------
// Display no cursor
HCURSOR CALLBACK CursorSetNo(void)
{
    return(SetCursor(LoadCursor(0, IDC_NO)));
}

// -----------------------------------------------------------------------
// Display cross cursor
HCURSOR CALLBACK CursorSetCross(void)
{
    return(SetCursor(LoadCursor(0, IDC_CROSS)));
}

// -----------------------------------------------------------------------
// Display normal arrow cursor
HCURSOR CALLBACK CursorSetNormal(void)
{
    return(SetCursor(LoadCursor(0, IDC_ARROW)));
}

// -----------------------------------------------------------------------
// Set mouse pointer off
long CALLBACK CursorDisable(void)
{
    return((long) SetCursor(0));
}

// -----------------------------------------------------------------------
// Set mouse pointer on
long CALLBACK CursorEnable(long OldCursorState)
{
    long ReturnValue = 0;

    ReturnValue = OldCursorState;
    if(OldCursorState != 0)
    {
        SetCursor((HCURSOR) OldCursorState);
        ReturnValue = 0;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Ini files functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Read .ini value
CStr CALLBACK IniReadKey(CStr Section, CStr Key, CStr File)
{
    CStr RetSec;
    long CheckNull = 0;

    RetSec = RetSec.Space(4096);
    GetPrivateProfileString(Section.Get_String(), Key.Get_String(), "", RetSec.Get_String(), 4096, File.Get_String());
    CheckNull = RetSec.Len();
    if(CheckNull == 0) return(RetSec);
    return(RetSec.Mid(1, CheckNull));
}

// -----------------------------------------------------------------------
// Read a boolean .ini value
long CALLBACK IniReadBoolKey(CStr Section, CStr Key, CStr File)
{
    CStr BufString;

    BufString = IniReadKey(Section, Key, File);
    if(BufString.Len() == 0) return(0);
    if(strcmp(BufString.Get_String(), "0") == 0) return(0);
    // True on alien value
    return(1);
}

// -----------------------------------------------------------------------
// Write .ini section
long CALLBACK IniWriteSection(CStr Section, CStr Key, CStr File)
{
    return(WritePrivateProfileSection(Section.Get_String(), Key.Get_String(), File.Get_String()));
}

// -----------------------------------------------------------------------
// Delete .ini value
long CALLBACK IniDeleteKey(CStr Section, CStr Key, CStr File)
{
    if(Key.Len() == 0) return(WritePrivateProfileStruct(Section.Get_String(), (char *) 0, 0, 0, File.Get_String()));
    else return(WritePrivateProfileStruct(Section.Get_String(), Key.Get_String(), 0, 0, File.Get_String()));
}

// -----------------------------------------------------------------------
// Write .ini value
long CALLBACK IniWriteKey(CStr Section, CStr Key, CStr Value, CStr File)
{
    return(WritePrivateProfileString(Section.Get_String(), Key.Get_String(), Value.Get_String(), File.Get_String()));
}

// -----------------------------------------------------------------------
// Math functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Return a random generated number
long CALLBACK MathGenerateRandomNumber(long MaxNumber)
{
    time_t CurTime;

    CurTime = time(NULL);
    srand((unsigned int) CurTime);
    return(rand() % MaxNumber);
}

// -----------------------------------------------------------------------
// Misc functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Copy a text into the clipboard
long CALLBACK MiscClipBoardCopyText(CStr Txt)
{
    long ReturnValue = 0;
    HANDLE hClip = 0;
    long hClipLen = 0;

    if(OpenClipboard(0) != 0)
    {
        if(EmptyClipboard != 0)
        {
            hClipLen = Txt.Len();
            hClip = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT,
                                hClipLen + 1);
            RtlCopyMemory(GlobalLock(hClip),
                          Txt.Get_String(),
                          hClipLen);
            GlobalUnlock(hClip);
            SetClipboardData(CF_TEXT, hClip);
            ReturnValue = 1;
        }
        CloseClipboard();
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve a text from the clipboard
CStr CALLBACK MiscClipBoardPasteText(void)
{
    CStr ReturnValue;
    HANDLE hClip = 0;
    HANDLE hClipOwned = 0;
    long hClipLen = 0;

    if(OpenClipboard(0) != 0)
    {
        hClip = GetClipboardData(CF_TEXT);
        hClipOwned = GlobalLock(hClip);
        hClipLen = strlen((char *) hClipOwned);
        if(hClipLen != 0)
        {
            ReturnValue = ReturnValue.String(hClipLen + 1, 1);
            RtlCopyMemory(ReturnValue.Get_String(), hClipOwned, hClipLen);
        }
        GlobalUnlock(hClip);
        CloseClipboard();
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if the clipboard is empty or not
long CALLBACK MiscClipBoardIsEmpty(void)
{
    long ReturnValue =1;
    HANDLE hClip = 0;
    long hClipLen = 0;

    if(OpenClipboard(0) != 0)
    {
        hClip = GetClipboardData(CF_TEXT);
        if(hClip != 0)
        {
            hClipLen = strlen((char *) GlobalLock(hClip));
            if(hClipLen != 0) ReturnValue = 0;
            GlobalUnlock(hClip);
        }
        CloseClipboard();
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Add a filename into recents documents list
void CALLBACK MiscShellAddFileToRecents(CStr FileName)
{
    SHAddToRecentDocs(SHARD_PATH, FileName.Get_String());
}

// -----------------------------------------------------------------------
// Wait for events
long CALLBACK MiscWaitEvents(HWND hClient, HACCEL hAccelerator, HWND hWndAccelerators)
{
    MSG WAMsg;

    memset(&WAMsg, 0, sizeof(WAMsg));
    if((hClient != 0) && (hAccelerator != 0))
    {
        while(GetMessage(&WAMsg, 0, 0, 0) != 0)
        {
            if(TranslateMDISysAccel(hClient, &WAMsg) == 0)
            {
                if(TranslateAccelerator(hWndAccelerators, hAccelerator, &WAMsg) == 0)
                {
                    TranslateMessage(&WAMsg);
                    DispatchMessage(&WAMsg);
                }
            }
        }
        return(WAMsg.wParam);
    }
    if((hClient != 0) && (hAccelerator == 0))
    {
        while(GetMessage(&WAMsg, 0, 0, 0) != 0)
        {
            if(TranslateMDISysAccel(hClient, &WAMsg) == 0)
            {
                TranslateMessage(&WAMsg);
                DispatchMessage(&WAMsg);
            }
        }
        return(WAMsg.wParam);
    }
    if((hClient == 0) && (hAccelerator != 0))
    {
        while(GetMessage(&WAMsg, 0, 0, 0) != 0)
        {
            if(TranslateAccelerator(hWndAccelerators, hAccelerator, &WAMsg) == 0)
            {
                TranslateMessage(&WAMsg);
                DispatchMessage(&WAMsg);
            }
        }
        return(WAMsg.wParam);
    }
    if((hClient == 0) && (hAccelerator == 0))
    {
        while(GetMessage(&WAMsg, 0, 0, 0) != 0)
        {
            TranslateMessage(&WAMsg);
            DispatchMessage(&WAMsg);
        }
        return(WAMsg.wParam);
    }
    return(WAMsg.wParam);
}

// -----------------------------------------------------------------------
// Display a standard messagebox
long CALLBACK MiscMsgBox(HWND hParent, CStr MBText, long MBType, CStr MBTitle)
{
    if(hParent == 0) hParent = GetActiveWindow();
    return(MessageBox(hParent, MBText.Get_String(), MBTitle.Get_String(), MBType));
}

// -----------------------------------------------------------------------
// Send a broadcasted message
long CALLBACK MiscSendBroadCastMsg(long BDMessage)
{
    return(SendMessage(HWND_BROADCAST, BDMessage, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Wait for time slice
void CALLBACK MiscDoEvents(HWND hClient, HACCEL hAccelerator, HWND hWndAccelerators)
{
    MSG WAMsg;

    memset(&WAMsg, 0, sizeof(WAMsg));
    if((hClient != 0) && (hAccelerator != 0))
    {
        if(PeekMessage(&WAMsg, 0, 0, 0, PM_REMOVE) != 0)
        {
            if(TranslateMDISysAccel(hClient, &WAMsg) == 0)
            {
                if(TranslateAccelerator(hWndAccelerators, hAccelerator, &WAMsg) == 0)
                {
                    TranslateMessage(&WAMsg);
                    DispatchMessage(&WAMsg);
                }
            }
        }
        return;
    }
    if((hClient != 0) && (hAccelerator == 0))
    {
        if(PeekMessage(&WAMsg, 0, 0, 0, PM_REMOVE) != 0)
        {
            if(TranslateMDISysAccel(hClient, &WAMsg) == 0)
            {
                TranslateMessage(&WAMsg);
                DispatchMessage(&WAMsg);
            }
        }
        return;
    }
    if((hClient == 0) && (hAccelerator != 0))
    {
        if(PeekMessage(&WAMsg, 0, 0, 0, PM_REMOVE) != 0)
        {
            if(TranslateAccelerator(hWndAccelerators, hAccelerator, &WAMsg) == 0)
            {
                TranslateMessage(&WAMsg);
                DispatchMessage(&WAMsg);
            }
        }
        return;
    }
    if((hClient == 0) && (hAccelerator == 0))
    {
        if(PeekMessage(&WAMsg, 0, 0, 0, PM_REMOVE) != 0)
        {
            TranslateMessage(&WAMsg);
            DispatchMessage(&WAMsg);
        }
    }
    return;
}

// -----------------------------------------------------------------------
// Obtain a GUID from the system
HRESULT CALLBACK MiscObtainGUID(LPGUID GUIDToObtain)
{
    HRESULT ReturnValue = 0;
    CoInitialize(0);
    ReturnValue = CoCreateGuid(GUIDToObtain);
    CoUninitialize();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Obtain a block of memory from the system
// + garbage collector
HGLOBAL CALLBACK MiscAllocMem(long Amount)
{
    HGLOBAL Allocated_Mem;

    if(Amount != 0)
    {
        Allocated_Mem = calloc(Amount, 1);
        Memory_Blocks.Add(Allocated_Mem);
        return(Allocated_Mem);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Frees a block of memory from the system
// + garbage collector
long CALLBACK MiscFreeMem(void *MemHandle)
{
    int i;

    if(MemHandle != NULL)
    {
        free(MemHandle);
        for(i = 0; i < Memory_Blocks.Amount(); i++)
        {
            if(Memory_Blocks.Get(i)->Content == MemHandle)
            {
                Memory_Blocks.Del(i);
                break;
            }
        }
    }
    return(1);
}

// -----------------------------------------------------------------------
// Copy a memory block several times
void CALLBACK MiscCopyMemLoop(void *MemSource, void *MemDest, long Length, long MemLeftOffset, long RepeatCount)
{
    CopyMem((char *) MemSource, (char *) MemDest, Length, MemLeftOffset, RepeatCount);
}

// -----------------------------------------------------------------------
// Copy a memory block several times
void CALLBACK MiscCopyMemWithOffsets(void *MemSource, void *MemDest, long Length, long SourceOffset, long DestOffset)
{
    CopyMem2((char *) MemSource,(char *)  MemDest, Length, SourceOffset, DestOffset);
}

// -----------------------------------------------------------------------
// Return the class of the operating system
long CALLBACK MiscGetOSClass(void)
{
    int Cl = 0;         // Default is Windows 9x
    OSVERSIONINFO MyOsVersion;

    MyOsVersion.dwOSVersionInfoSize = sizeof(MyOsVersion);
    if(GetVersionEx(&MyOsVersion) != 0)
    {
        if(MyOsVersion.dwMajorVersion >= 5)
        {
            if(MyOsVersion.dwMajorVersion >= 6)
            {
                // Windows 7 (6.1)
                if(MyOsVersion.dwPlatformId == VER_PLATFORM_WIN32_NT)
                {
                    Cl |= 4;
                    Cl |= 8;
                }
            }
            else
            {
                if(MyOsVersion.dwMinorVersion >= 1)
                {
                    // Windows XP (5.1)
                    if(MyOsVersion.dwPlatformId == VER_PLATFORM_WIN32_NT) Cl |= 4;
                }
                else
                {
                    // Windows 2k or Me (5.0)
                    if(MyOsVersion.dwPlatformId == VER_PLATFORM_WIN32_NT) Cl |= 2;
                }
            }
        }
        else
        {
            // Windows NT or 9x
            if(MyOsVersion.dwPlatformId == VER_PLATFORM_WIN32_NT) Cl |= 1;
        }    
    }
    return(Cl);
}

// -----------------------------------------------------------------------
// Dates functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Get current date
CStr CALLBACK DateGetNow(long ReportsSeconds)
{
    CStr ReturnedDate;
    CStr ReturnedTime;
    CStr TempReturnValue;
    CStr ReturnValue;
    SYSTEMTIME DtNow;
    long DateLen = 0;
    long DateMem = 0;
    long TmFormat = 0;

    GetLocalTime(&DtNow);
    // Retrieve the needed length
    DateLen = GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &DtNow, (char *) 0, (char *) 0, 0);
    DateMem = AllocMem(DateLen);
    if(DateMem != 0)
    {
        // Let system format it
        GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &DtNow, (char *) 0, (char *) DateMem, DateLen);
        // Save it
        ReturnedDate = ReturnedDate.String(strlen((char *) DateMem), 1);
        RtlCopyMemory(ReturnedDate.Get_String(), (void *) DateMem, strlen((char *) DateMem));
        FreeMem(DateMem);
        TempReturnValue = ReturnedDate;
        // Do the same for the time
        if(ReportsSeconds == DATE_GET_SECONDS) TmFormat = TIME_FORCE24HOURFORMAT;
        else TmFormat = TIME_FORCE24HOURFORMAT | TIME_NOSECONDS;
        DateLen = GetTimeFormat(LOCALE_USER_DEFAULT, TmFormat, &DtNow, (char *) 0, (char *) 0, 0);
        DateMem = AllocMem(DateLen);
        if(DateMem != 0)
        {
            TempReturnValue = TempReturnValue + " ";
            GetTimeFormat(LOCALE_USER_DEFAULT, TmFormat, &DtNow, (char *) 0, (char *) DateMem, DateLen);
            // Save it
            ReturnedTime = ReturnedTime.String(strlen((char *) DateMem), 1);
            RtlCopyMemory(ReturnedTime.Get_String(), (void *) DateMem, strlen((char *) DateMem));
            FreeMem(DateMem);
            TempReturnValue = TempReturnValue + ReturnedTime;
        }
    }
    ReturnValue = TempReturnValue;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get current year
CStr CALLBACK DateGetYear(void)
{
    return(DateGetFormat("yyyy"));
}

// -----------------------------------------------------------------------
// Get current month
CStr CALLBACK DateGetMonth(void)
{
    return(DateGetFormat("M"));
}

// -----------------------------------------------------------------------
// Get current day
CStr CALLBACK DateGetDay(void)
{
    return(DateGetFormat("d"));
}

// -----------------------------------------------------------------------
// Get current hour
CStr CALLBACK DateGetHour(void)
{
    return(DateGetTimeFormat("H"));
}

// -----------------------------------------------------------------------
// Get current minutes
CStr CALLBACK DateGetMinutes(void)
{
    return(DateGetTimeFormat("m"));
}

// -----------------------------------------------------------------------
// Get current seconds
CStr CALLBACK DateGetSeconds(void)
{
    return(DateGetTimeFormat("s"));
}

// -----------------------------------------------------------------------
// Get a formatted date --- '
CStr CALLBACK DateGetFormat(CStr DateFormat)
{
    CStr ReturnedDate;
    CStr ReturnValue;
    SYSTEMTIME DtNow;
    long DateLen = 0;
    long DateMem = 0;
    
    GetLocalTime(&DtNow);
    // Retrieve the needed length
    DateLen = GetDateFormat(LOCALE_USER_DEFAULT, 0, &DtNow, DateFormat.Get_String(), (char *) 0, 0);
    DateMem = AllocMem(DateLen);
    if(DateMem != 0)
    {
        // Let system format it
        GetDateFormat(LOCALE_USER_DEFAULT, 0, &DtNow, DateFormat.Get_String(), (char *) DateMem, DateLen);
        // Save it
        ReturnedDate = ReturnedDate.String(strlen((char *) DateMem), 1);
        RtlCopyMemory(ReturnedDate.Get_String(), (void *) DateMem, strlen((char *) DateMem));
        FreeMem(DateMem);
        ReturnValue = ReturnedDate;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get a formatted time
CStr CALLBACK DateGetTimeFormat(CStr DateFormat)
{
    CStr ReturnedDate;
    CStr ReturnValue;
    SYSTEMTIME DtNow;
    long DateLen = 0;
    long DateMem = 0;
    
    GetLocalTime(&DtNow);
    // Retrieve the needed length
    DateLen = GetTimeFormat(LOCALE_USER_DEFAULT, 0, &DtNow, DateFormat.Get_String(), (char *) 0, 0);
    DateMem = AllocMem(DateLen);
    if(DateMem != 0)
    {
        // Let system format it
        GetTimeFormat(LOCALE_USER_DEFAULT, 0, &DtNow, DateFormat.Get_String(), (char *) DateMem, DateLen);
        // Save it
        ReturnedDate = ReturnedDate.String(strlen((char *) DateMem), 1);
        RtlCopyMemory(ReturnedDate.Get_String(), (void *) DateMem, strlen((char *) DateMem));
        FreeMem(DateMem);
        ReturnValue = ReturnedDate;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve a date from a string
long CALLBACK DateStringToDate(CStr StrDate, FILETIME *DestDate)
{
    char *DateToConvert;
    char ReadChar;
    SYSTEMTIME TmpDate;

    memset(&TmpDate, 0, sizeof(TmpDate));
    // Should handle formats:
    // dd[/|-]mm[/|-]yy[yy] [hh:mm[:ss]]
    // (Also verify the coherency of the date)
    DateToConvert = StrDate.Get_String();

    // Get the day
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wDay = (ReadChar - 0x30) * 10;
    DateToConvert++;
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wDay = TmpDate.wDay + (ReadChar - 0x30);
    DateToConvert++;

    // Separator...
    if(DateToConvert[0] != '-' && DateToConvert[0] != '/') return(0);
    DateToConvert++;

    // Get the month
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wMonth = (ReadChar - 0x30) * 10;
    DateToConvert++;
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wMonth = TmpDate.wMonth + (ReadChar - 0x30);
    DateToConvert++;

    // Separator...
    if(DateToConvert[0] != '-' && DateToConvert[0] != '/') return(0);
    DateToConvert++;

    // Get the year
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wYear = (ReadChar - 0x30) * 10;
    DateToConvert++;
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wYear = TmpDate.wYear + (ReadChar - 0x30);
    DateToConvert++;
    // 4 digits ?
    ReadChar = DateToConvert[0];
    if(isdigit(ReadChar))
    {
        TmpDate.wYear = TmpDate.wYear * 10;
        TmpDate.wYear = TmpDate.wYear + (ReadChar - 0x30);
        DateToConvert++;
        ReadChar = DateToConvert[0];
        if(!isdigit(ReadChar)) return(0);
        TmpDate.wYear = TmpDate.wYear * 10;
        TmpDate.wYear = TmpDate.wYear + (ReadChar - 0x30);
        DateToConvert++;
    }
    // There must be at least one blank char
    if(!isspace(DateToConvert[0] & 0xff)) return(0);
    while(isspace(DateToConvert[0] & 0xff))
    {
        DateToConvert++;
    }

    // Get the hours
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wHour = (ReadChar - 0x30) * 10;
    DateToConvert++;
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wHour = TmpDate.wHour + (ReadChar - 0x30);
    DateToConvert++;

    // Separator...
    if(DateToConvert[0] != ':') return(0);
    DateToConvert++;

    // Get the minutes
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wMinute = (ReadChar - 0x30) * 10;
    DateToConvert++;
    ReadChar = DateToConvert[0];
    if(!isdigit(ReadChar)) return(0);
    TmpDate.wMinute = TmpDate.wMinute + (ReadChar - 0x30);
    DateToConvert++;

    // Optional separator for seconds
    if(DateToConvert[0] == ':')
    {
        DateToConvert++;
        // Get the seconds
        ReadChar = DateToConvert[0];
        if(!isdigit(ReadChar)) return(0);
        TmpDate.wSecond = (ReadChar - 0x30) * 10;
        DateToConvert++;
        ReadChar = DateToConvert[0];
        if(!isdigit(ReadChar)) return(0);
        TmpDate.wSecond = TmpDate.wSecond + (ReadChar - 0x30);
        DateToConvert++;
    }
    // (Return value is syncrhonized since SystemTimeToFileTime() will
    // return 0 on failure)
    return(SystemTimeToFileTime(&TmpDate, DestDate));
}

// -----------------------------------------------------------------------
// Strings functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Check if a char is printable or not and return CorrectChar if necessary
long CALLBACK StringFilterNonPrintableChars(long ASCIIChar, long CorrectChar)
{
    if(ASCIIChar == 0 ||
       ASCIIChar == 7 ||
       ASCIIChar == 8 ||
       ASCIIChar == 9 ||
       ASCIIChar == 10 ||
       ASCIIChar == 13) return(CorrectChar);
    return(ASCIIChar);
}

// -----------------------------------------------------------------------
// Check the validity of an asm label (wih or wihtout space chars)
long CALLBACK StringIsLabel(CStr AsmText, int Allow_Space)
{
    int i = 0;
    CStr CharToTest;

    // Can't begin by a number
    if(!Allow_Space)
    {
        if(StringIsDigitChar(AsmText.Mid(1, 1))) return(0);
    }
    // Check first char
    for(i = 1 ; i <= (long) AsmText.Len(); i++)
    {
        CharToTest = AsmText.Mid(i, 1);
        CharToTest = CharToTest.Upper_Case();
        if(Allow_Space)
        {
            if(StringIsSpaceChar(CharToTest) == 0)
            {
                if(StringIsLabelChar(CharToTest) == 0)
                {
                    return(0);
                }
            }
        }
        else
        {
            if(StringIsLabelChar(CharToTest) == 0)
            {
                return(0);
            }
        }
    }
    return(1);
}

// -----------------------------------------------------------------------
// Check the validity of an hexadecimal number
long CALLBACK StringIsHexaDecimal(CStr HexaText)
{
    int i = 1;
    int Max_Len = 8;
    CStr CharToTest;

    // Remove possible 0x prefix
    if(HexaText.Len() >= 2)
    {
        if(HexaText[0] == '0' && (HexaText[1] == 'X' || HexaText[1] == 'x'))
        {
            i += 2;
            Max_Len += 2;
        }
        // 0x only ?
        if(i == 3 && HexaText.Len() == 2) return(0);
    }
    // Look for real chars number (either 8 or 10)
    if(HexaText.Len() > Max_Len) return(0);
    // Check if content is valid
    for(; i <= (long) HexaText.Len(); i++)
    {
        CharToTest = HexaText.Mid(i, 1);
        if(StringIsDigitChar(CharToTest) == 0)
        {
            if(StringIsHexaChar(CharToTest) == 0)
            {
                return(0);
            }
        }
    }
    return(1);
}

// -----------------------------------------------------------------------
// Check the validity of a decimal number
long CALLBACK StringIsDecimal(CStr DecText, long MaxLen)
{
    long ReturnValue = 1;
    int i = 0;
    CStr CharToTest;

    // Check first char
    for(i = 1; i <= (long) DecText.Len() ; i++)
    {
        if(i > (long) MaxLen) goto OverRideDecimal;
        CharToTest = DecText.Mid(i, 1);
        CharToTest = CharToTest.Upper_Case();
        if(StringIsDigitChar(CharToTest) == 0)
        {
OverRideDecimal:;
            ReturnValue = 0;
            break;
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if letter is valid for an asm label name
long CALLBACK StringIsLabelChar(CStr Letter)
{
    if(StringIsAlphaChar(Letter) || StringIsUnderLineChar(Letter) || StringIsDigitChar(Letter)) return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is a digit
long CALLBACK StringIsDigitChar(CStr Letter)
{
    if(Letter.Len() != 0) if((Letter.Asc() >= '0') && (Letter.Asc() <= '9')) return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is hexadecimal
long CALLBACK StringIsHexaChar(CStr Letter)
{
    CStr LetterUpper_Case;
    
    if(Letter.Len() != 0)
    {
        LetterUpper_Case = Letter.Upper_Case();
        if(LetterUpper_Case.Asc() >= 'A' && LetterUpper_Case.Asc() <= 'F')
        {
            return(1);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is alpha
long CALLBACK StringIsAlphaChar(CStr Letter)
{
    CStr LetterUpper_Case;
    
    if(Letter.Len() != 0)
    {
        LetterUpper_Case = Letter.Upper_Case();
        if(LetterUpper_Case.Asc() >= 'A' && LetterUpper_Case.Asc() <= 'Z')
        {
            return(1);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is a space
long CALLBACK StringIsBlankChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == 32 || Letter.Asc() == 9) return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is an open bracket
long CALLBACK StringIsOpenBracketChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == '(') return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is a close bracket
long CALLBACK StringIsCloseBracketChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == ')') return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is an underline char
long CALLBACK StringIsUnderLineChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == '_') return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is a dot char
long CALLBACK StringIsDotChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == '.') return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is a quote char
long CALLBACK StringIsQuoteChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == '"') return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is a comma char
long CALLBACK StringIsCommaChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == ',') return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is an arobas char
long CALLBACK StringIsArobasChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == '@') return(1);
    return(0);
}
// -----------------------------------------------------------------------
// Check if letter is a space char
long CALLBACK StringIsSpaceChar(CStr Letter)
{
    if(Letter.Len() != 0) if(Letter.Asc() == ' ') return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Check if letter is an end of line marker
long CALLBACK StringIsEOL(CStr Letter)
{
    if(Letter.Len() == 0) return(1);
    if(Letter.Asc() == 13 || Letter.Asc() == 10 || Letter.Asc() == 0) return(1);
    return(0);
}

// -----------------------------------------------------------------------
// Complement a decimal number
CStr CALLBACK StringNumberComplement(long Nbr, long Complement)
{
    CStr ReturnValue;
    CStr BufString;
    CStr BufStringSource;
    
    if(Complement > 0)
    {
        BufString = BufString.String(16, 1);
        BufStringSource = "%0";
        BufStringSource = BufStringSource + Complement;
        BufStringSource = BufStringSource + "lu";
        wsprintf(BufString.Get_String(), BufStringSource.Get_String(), Nbr);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Complement a hexadecimal number
CStr CALLBACK StringHexNumberComplement(long Nbr, long Complement)
{
    CStr ReturnValue;
    CStr BufString;
    CStr BufStringSource;
    
    if(Complement > 0)
    {
        BufString = BufString.String(Complement, 1);
        BufStringSource = "%0";
        BufStringSource = BufStringSource + Complement;
        BufStringSource = BufStringSource + "lX";
        wsprintf(BufString.Get_String(), BufStringSource.Get_String(), Nbr);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Complement an octal number
CStr CALLBACK StringOctNumberComplement(long Nbr, long Complement)
{
    CStr ReturnValue;
    CStr BufString;
    CStr BufStringSource;
    
    if(Complement > 0)
    {
        BufString = BufString.String(Complement, 1);
        BufStringSource = "%0";
        BufStringSource = BufStringSource + Complement;
        BufStringSource = BufStringSource + "lo";
        sprintf(BufString.Get_String(), BufStringSource.Get_String(), Nbr);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Complement a binary number
CStr CALLBACK StringBinNumberComplement(long Nbr, long Complement)
{
    CStr ReturnValue;
    CStr BufString;
    CStr BufStringMid;
    long BinMask = 1;
    int i = 0;
    
    if(Complement > 32) Complement = 32;
    if(Complement < 0) Complement = 0;
    for(i = 0; i <= 31 ; i++)
    {
        if((Nbr & BinMask) != 0) BufString = "1" + BufString;
        else BufString = "0" + BufString;
        BinMask = BinMask << 1;
    }
    for(i = 1; i <= (long) BufString.Len(); i++)
    {
        BufStringMid = BufString.Mid(i, 1);
        if(strcmp(BufStringMid.Get_String(), "0") != 0) break;
    }
    BufString = BufString.Mid(i);
    if(BufString.Len() < (long) Complement)
    {
        BufStringMid = BufString.String(Complement - BufString.Len(), '0');
        BufString = BufStringMid + BufString;
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Free a VB string
void CALLBACK StringFree(long StringToFree)
{
    if(StringToFree != 0) free((void *) StringToFree);
}

// -----------------------------------------------------------------------
// Return the length of a string
long CALLBACK StringLen(long StringToCalc)
{
    if(StringToCalc != 0) return(strlen((char *) StringToCalc));
    return(0);
}

// -----------------------------------------------------------------------
// Cat 2 strings
CStr CALLBACK StringCat(CStr String1, CStr String2)
{
//  CStr ReturnValue;
//  CStr BufString1;
//  CStr BufString2;

//  BufString1 = String1;
//  BufString2 = String2;
//  BufString1 = BufString1 + BufString2;
//  ReturnValue = BufString1;
    return(String1 + String2);
}

// -----------------------------------------------------------------------
// Convert a decimal into a string
CStr CALLBACK StringDecToString(long DecimalNbr)
{
    //CStr ReturnValue;
    //CStr BufString1;

    //BufString1 = DecimalNbr;
    //ReturnValue = BufString1;
    return(DecimalNbr);
}

// -----------------------------------------------------------------------
// Convert a decimal string into a decimal
long CALLBACK StringStringToDec(CStr DecString)
{
    return(DecString.Get_Long());
}

// -----------------------------------------------------------------------
// Convert a hexadecimal string into a decimal
long CALLBACK StringStringToHex(CStr HexString)
{
    return(HexString.Get_Hex());
}

// -----------------------------------------------------------------------
// Count strings occurences
long CALLBACK StringCount(CStr MaIn_String, CStr StringToCount, long CountStart, long CompareType)
{
    long PosIn_String = 0;
    long LenToFind = 0;
    long FoundCount = 0;
    CStr SunMaIn_String;

    // Search sub string
    LenToFind = StringToCount.Len();
    if(LenToFind != 0)
    {
        if(CountStart <= 0) CountStart = 1;
        SunMaIn_String = MaIn_String.Mid(CountStart);
        PosIn_String = 1;
CountAll:;
        PosIn_String = SunMaIn_String.In_Str(PosIn_String, StringToCount, CompareType);
        if(PosIn_String != 0)
        {
            PosIn_String = PosIn_String + LenToFind;
            FoundCount++;
            goto CountAll;
        }
    }
    return(FoundCount);
}

// -----------------------------------------------------------------------
// Replace strings occurences
CStr CALLBACK StringReplace(CStr MaIn_String, CStr StringToFind, CStr StringToReplace,
                            long ReplaceStart, long RepCount, long CompareType)
{
    CStr ReturnValue;

    ReturnValue = MaIn_String.Replace(StringToFind,StringToReplace,ReplaceStart,RepCount,CompareType);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a long to hex with at least 8 chars
CStr CALLBACK StringHex32(long Number)
{
    CStr ReturnValue;
    CStr BufString;

    ReturnValue = ReturnValue.Hex_To_String(Number);
    if(ReturnValue.Len() < 8)
    {
        BufString = BufString.String(8 - ReturnValue.Len(), '0');
        BufString = BufString + ReturnValue;
        ReturnValue = BufString;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a long to hex with at least 4 chars
CStr CALLBACK StringHex16(long Number)
{
    CStr ReturnValue;
    CStr BufString;

    ReturnValue = ReturnValue.Hex_To_String(Number);
    if(ReturnValue.Len() < 4)
    {
        BufString = BufString.String(4 - ReturnValue.Len(), '0');
        BufString = BufString + ReturnValue;
        ReturnValue = BufString;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a long to hex with at least 4 chars
CStr CALLBACK StringHex8(long Number)
{
    CStr ReturnValue;
    CStr BufString;

    ReturnValue = ReturnValue.Hex_To_String(Number);
    if(ReturnValue.Len() < 2)
    {
        BufString = BufString.String(2 - ReturnValue.Len(), '0');
        BufString = BufString + ReturnValue;
        ReturnValue = BufString;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Copy a string
CStr CALLBACK StringCopy(CStr DestString, CStr SourceString, long SourceStringLen, long DestStringLen)
{
    switch(SourceStringLen)
    {
        case -1:
            // Alloc and fill string
            DestString = DestString.String(DestStringLen, 1);
            break;
        default:
            // Fill string
            RtlZeroMemory(DestString.Get_String(), SourceStringLen);
            if(DestStringLen > SourceStringLen) DestStringLen = SourceStringLen;
            break;
    }
    RtlCopyMemory(DestString.Get_String(), SourceString.Get_String(), DestStringLen);
    return(DestString);
}

// -----------------------------------------------------------------------
// Copy a string by calculating source len and append a null terminating char
CStr CALLBACK StringCopyAppendZero(CStr DestString, CStr SourceString)
{
    long StLen = 0;

    StLen = SourceString.Len();
    DestString = DestString.String(StLen, 1);
    RtlCopyMemory(DestString.Get_String(), SourceString.Get_String(), StLen);
    return(DestString);
}

// -----------------------------------------------------------------------
// Copy a string by calculating source len
CStr CALLBACK StringCopyFixed(CStr DestString, CStr SourceString)
{
    DestString.Set_String(SourceString.Get_String());
    return(DestString);
}

// -----------------------------------------------------------------------
// Clear a string
CStr CALLBACK StringClear(CStr DestString, long DestStringLen)
{
    RtlZeroMemory(DestString.Get_String(), DestStringLen);
    return(DestString);
}

// -----------------------------------------------------------------------
// Append a string
CStr CALLBACK StringAppend(CStr DestString, CStr SourceString)
{
    CStr BufString;

    BufString = DestString.Get_String();
    BufString = BufString + (CStr) SourceString.Get_String();
    return(BufString);
}

// -----------------------------------------------------------------------
// Split a string
long * CALLBACK StringSplit(CStr StrToSplit, CStr StrSplitter)
{
    long OldPosSplitter = 0;
    long PosSplitter = 0;
    long * MemSplitter = 0;
    long LngSplitter = 0;
    CStr GlobaltoSplit;
    long LenSplitter = 0;
    long LenElements = 0;

    OldPosSplitter = 1;
    if(StrToSplit.Len() == 0) return(0);
    GlobaltoSplit = StrToSplit;
    GlobaltoSplit = GlobaltoSplit + StrSplitter;
    LenSplitter = StrSplitter.Len();
    // Count occurences
CountSplits:
    PosSplitter = GlobaltoSplit.In_Str(OldPosSplitter, StrSplitter);
    if(PosSplitter != 0)
    {
        LngSplitter = LngSplitter + 8;
        LenElements++;
        OldPosSplitter = PosSplitter + LenSplitter;
        goto CountSplits;
    }
    if(LenElements == 0) return(0);
    MemSplitter = (long *) AllocMem(LngSplitter + 4);
    if(MemSplitter == 0) return(0);
    OldPosSplitter = 1;
    // Store number of elements
    MemSplitter[0] = LenElements;
    LngSplitter = 1;
StoreSplits:;
    PosSplitter = GlobaltoSplit.In_Str(OldPosSplitter, StrSplitter);
    if(PosSplitter != 0)
    {
        // Store position of string and length
        MemSplitter[LngSplitter] = OldPosSplitter;
        MemSplitter[LngSplitter + 1] = (PosSplitter - OldPosSplitter);
        LngSplitter = LngSplitter + 2;
        OldPosSplitter = PosSplitter + LenSplitter;
        goto StoreSplits;
    }
    return(MemSplitter);
}

// -----------------------------------------------------------------------
// Retrieve number of element of a splitted string
long CALLBACK StringGetSplitUBound(long *SplitMem)
{
    if(SplitMem == 0) return(-1);
    else return(SplitMem[0] - 1);
}

// -----------------------------------------------------------------------
// Retrieve an element of a splitted string
CStr CALLBACK StringGetSplitElement(CStr StrFromSplit, long *SplitMem, long Element)
{
    long ElementAddr = 0;
    long ElementLen = 0;

    CStr ReturnValue;
    if(SplitMem == 0) return(ReturnValue);
    if(Element > (SplitMem[0] - 1)) return(ReturnValue);
    ElementAddr = SplitMem[((Element * 2) + 1)] - 1;
    ElementLen = SplitMem[(Element * 2) + 2];
    ReturnValue = ReturnValue.String(ElementLen, 1);
    CopyMem2(StrFromSplit.Get_String(), ReturnValue.Get_String(), ElementLen, ElementAddr, 0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Free a splitted string
void CALLBACK StringReleaseSplit(long *SplitMem)
{
    if(SplitMem != 0) FreeMem((long) SplitMem);
}

// -----------------------------------------------------------------------
// Join a string
CStr CALLBACK StringJoin(CStr StrFromSplit, long *SplitMem, CStr JoIn_String, long FirstElement, long ElementLimit)
{
    CStr ReturnValue;
    CStr BufString;
    long JoinLimit = 0;
    long i = 0;

    if(SplitMem != 0)
    {
        if(StringGetSplitUBound(SplitMem) != -1)
        {
            if(ElementLimit == -1) JoinLimit = StringGetSplitUBound(SplitMem);
            else JoinLimit = ElementLimit;
            if(FirstElement <= JoinLimit)
            {
                for(i = FirstElement; i <= JoinLimit; i++)
                {
                    BufString = BufString + StringGetSplitElement(StrFromSplit, SplitMem, i);
                    BufString = BufString + JoIn_String;
                }
            }
        }
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a char * to a CStr
CStr CALLBACK StringConvertToCStr(char *StringToConvert)
{
    CStr ReturnValue;

    ReturnValue = StringToConvert;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a CStr to a char *
char * CALLBACK StringCStrConvertToString(CStr StringToConvert)
{
    // Return the datas of the newly created entry
    return(Garbage_Collector_List.Add(StringToConvert.Get_String())->Content);
}

void CALLBACK StringFlushGarbageCollector(void)
{
    Garbage_Collector_List.Erase();
}

// -----------------------------------------------------------------------
// Retrieve the number of lines of a text
long CALLBACK StringCountLines(CStr StringToCount, long Length)
{
    return(CountFileLines(StringToCount.Get_String(), Length));
}

// -----------------------------------------------------------------------
// Retrieve the offsets of lines of a text
long CALLBACK StringGetLinesOffsets(CStr StringToCount)
{
    return(GetLinesOffsets(StringToCount.Get_String()));
}

// -----------------------------------------------------------------------
// Fill an ascii tab with hexadecimal bytes
void CALLBACK StringFillHexBytes(long Source, char *Dest, long Length, long Offset, 
                                 long LineOffset, long NbrLines, long ElementsOffset)
{
    FillHexBytes(Source, Dest, Length, Offset, LineOffset, NbrLines, ElementsOffset);
    return;
}

// -----------------------------------------------------------------------
// Fill an ascii tab with hexadecimal words
void CALLBACK StringFillHexWords(long Source, char *Dest, long Length, long Offset,
                                 long LineOffset, long NbrLines, long ElementsOffset)
{
    FillHexWords(Source, Dest, Length, Offset, LineOffset, NbrLines, ElementsOffset);
    return;
}

// -----------------------------------------------------------------------
// Fill an ascii tab with hexadecimal dwords
void CALLBACK StringFillHexDWords(long Source, char *Dest, long Length, long Offset,
                                  long LineOffset, long NbrLines, long ElementsOffset)
{
    FillHexDWords(Source, Dest, Length, Offset, LineOffset, NbrLines, ElementsOffset);
    return;
}

// -----------------------------------------------------------------------
// Replace all non label chars with an underscore
void CALLBACK StringFileNameToLabel(char *FileName)
{
    int i;
    if(StringIsDigitChar(FileName[0])) FileName[0] = '_';
    for(i = 0; i < ((int) strlen(FileName)); i++)
    {
        if(!StringIsLabelChar(FileName[i])) FileName[i] = '_';
    }
}

// -----------------------------------------------------------------------
// Files functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Retrieve last write date of a file
long CALLBACK FileGetWriteTime(CStr FileName, LPFILETIME FileWriteTime)
{
    FILETIME FileCTime;
    FILETIME FileATime;
    HANDLE FileHandle = 0;
    long ReturnValue = 0;

    FileHandle = CreateFile(FileName.Get_String(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        ReturnValue = GetFileTime(FileHandle, &FileCTime, &FileATime, FileWriteTime);
        FileClose(FileHandle);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve creation date of a file
long CALLBACK FileGetCreationTime(CStr FileName, LPFILETIME FileCreationTime)
{
    FILETIME FileATime;
    FILETIME FileWTime;
    HANDLE FileHandle = 0;
    long ReturnValue = 0;

    FileHandle = CreateFile(FileName.Get_String(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        ReturnValue = GetFileTime(FileHandle, FileCreationTime, &FileATime, &FileWTime);
        FileClose(FileHandle);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve last accessed time of a file
long CALLBACK FileGetAccessedTime(CStr FileName, LPFILETIME FileAccessedTime)
{
    FILETIME FileCTime; 
    FILETIME FileWTime;
    HANDLE FileHandle = 0;
    long ReturnValue = 0;

    FileHandle = CreateFile(FileName.Get_String(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(FileHandle != INVALID_HANDLE_VALUE)
    {
        ReturnValue = GetFileTime(FileHandle, &FileCTime, FileAccessedTime, &FileWTime);
        FileClose(FileHandle);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the extension of a filename
CStr CALLBACK FileGetExtension(CStr FileName)
{
    CStr ReturnValue;

    ReturnValue = FileName.Extract_FileName_Extension();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the extension of a filename
CStr CALLBACK FileRemoveExtension(CStr FileName)
{
    CStr ReturnValue;

    ReturnValue = FileName.Remove_FileName_Extension();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Replace the extension in a filename
CStr CALLBACK FileReplaceExtension(CStr FileName, CStr NewExtension)
{
    CStr ReturnValue;

    ReturnValue = FileName.Replace_FileName_Extension(NewExtension);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Extract a directory from a filename
CStr CALLBACK FileGetDirectory(CStr FileName)
{
    CStr ReturnValue;

    ReturnValue = FileName.Extract_Directory();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Extract a filename
CStr CALLBACK FileGetFileName(CStr FileName)
{
    CStr ReturnValue;

    ReturnValue = FileName.Extract_FileName();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Open a file for reading
HANDLE CALLBACK FileOpenR(CStr FileName)
{
    return(CreateFile(FileName.Get_String(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0));
}

// -----------------------------------------------------------------------
// Open a file for writing
HANDLE CALLBACK FileOpenW(CStr FileName, long FilePos)
{
    HANDLE ReturnValue = 0;

    ReturnValue = CreateFile(FileName.Get_String(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(ReturnValue != INVALID_HANDLE_VALUE) SetFilePointer(ReturnValue, FilePos, 0, FILE_BEGIN);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Open a file for appending
HANDLE CALLBACK FileOpenWAppend(CStr FileName)
{
    HANDLE ReturnValue = 0;

    ReturnValue = CreateFile(FileName.Get_String(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if(ReturnValue != INVALID_HANDLE_VALUE) SetFilePointer(ReturnValue, 0, 0, FILE_END);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Close an opened file
long CALLBACK FileClose(HANDLE hFile)
{
    long ReturnValue = 0;

    if(hFile != 0) ReturnValue = CloseHandle(hFile);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Create a new file
HANDLE CALLBACK FileCreateEmpty(CStr FileName, long Security)
{
    HANDLE ReturnValue = 0;
    SECURITY_ATTRIBUTES SecAt;

    memset(&SecAt, 0, sizeof(SecAt));
    DeleteFile(FileName.Get_String());
    if(Security == 1)
    {
        SecAt.nLength = sizeof(SecAt);
        SecAt.lpSecurityDescriptor = 0;
        SecAt.bInheritHandle = 1;
        ReturnValue = CreateFile(FileName.Get_String(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, &SecAt, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    }
    else
    {
        ReturnValue = CreateFile(FileName.Get_String(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Write a buffer into a file
long CALLBACK FileWriteBuffer(HANDLE FileHandle, LPCVOID BufferToWrite, long LenToWrite)
{
    long ReturnValue = 0;
    DWORD BytesWritten = 0;

    ReturnValue = WriteFile(FileHandle, BufferToWrite, LenToWrite, &BytesWritten, 0);
    if(ReturnValue != 0) ReturnValue = BytesWritten;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Write a buffer into a file (String version)
long CALLBACK FileWriteBufferVB(HANDLE FileHandle, CStr BufferToWrite, long LenToWrite)
{
    long ReturnValue = 0;
    DWORD BytesWritten = 0;

    ReturnValue = WriteFile(FileHandle, BufferToWrite.Get_String(), LenToWrite, &BytesWritten, 0);
    if(ReturnValue != 0) ReturnValue = BytesWritten;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Write a string line into a file
long CALLBACK FileWriteLine(HANDLE FileHandle, CStr LineToWrite)
{
    long ReturnValue = 0;
    CStr RealLineToWrite;
    DWORD BytesWritten = 0;

    RealLineToWrite = LineToWrite;
    RealLineToWrite = RealLineToWrite + "\r\n";
    ReturnValue = WriteFile(FileHandle, RealLineToWrite.Get_String(), RealLineToWrite.Len(), &BytesWritten, 0);
    if(ReturnValue != 0) ReturnValue = BytesWritten;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if a filename is only a directory
long CALLBACK FileIsDirectory(CStr FileToSearch)
{
    long ReturnValue = 0;
    WIN32_FIND_DATA FindDat;
    HANDLE hFindFile = 0;
    CStr RealFileToSearch;

    RealFileToSearch = FileToSearch;
    if(strcmp(FileToSearch.Right(1).Get_String(), "\\") == 0) RealFileToSearch = FileToSearch.Mid(1, FileToSearch.Len() - 1);
    hFindFile = FindFirstFile(RealFileToSearch.Get_String(), &FindDat);
    if(hFindFile != INVALID_HANDLE_VALUE)
    {
        FindClose(hFindFile);
        if((FindDat.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0) ReturnValue = 1;
        else ReturnValue = 0;
    } 
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return the length of a file
long CALLBACK FileGetSize(CStr FileName)
{
    HANDLE hFileSize = 0;
    long ReturnValue = 0;
    SECURITY_ATTRIBUTES SecAt;

    memset(&SecAt, 0, sizeof(SecAt));
    SecAt.nLength = sizeof(SecAt);
    SecAt.lpSecurityDescriptor = 0;
    SecAt.bInheritHandle = -1;
    hFileSize = CreateFile(FileName.Get_String(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, &SecAt, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if(hFileSize != INVALID_HANDLE_VALUE)
    {
        ReturnValue = GetFileSize(hFileSize, NULL);
        FileClose(hFileSize);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Search a given file or directory
long CALLBACK FileExist(CStr FileToSearch)
{
    long ReturnValue = 0;
    WIN32_FIND_DATA FindDat;
    HANDLE hFindFile = 0;
    CStr RealFileToSearch;

    RealFileToSearch = FileToSearch;
    if(strcmp(FileToSearch.Right(1).Get_String(), "\\") == 0) RealFileToSearch = FileToSearch.Mid(1, FileToSearch.Len() - 1);
    hFindFile = FindFirstFile(RealFileToSearch.Get_String(), &FindDat);
    if(hFindFile != INVALID_HANDLE_VALUE)
    {
        FindClose(hFindFile);
        ReturnValue = 1;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return writing state of a file
// Return FILE_ATTRIBUTE_READONLY or 0
long CALLBACK FileIsReadOnly(CStr FileToCheck)
{
    CStr RealFileToCheck;

    RealFileToCheck = FileToCheck;
    if(strcmp(FileToCheck.Right(1).Get_String(), "\\") == 0) RealFileToCheck = FileToCheck.Mid(1, FileToCheck.Len() - 1);
    return((long) GetFileAttributes(RealFileToCheck.Get_String()) & FILE_ATTRIBUTE_READONLY);
}

// -----------------------------------------------------------------------
// Check if a file is a possible unix file or not
long CALLBACK FileIsUnix(CStr FileToCheck)
{
    long ReturnValue = 0;
    HANDLE hUnixFile = 0;
    HANDLE hUnixMem = 0;
    DWORD UnixFileRead = 0;
    long PosInFile = 0;
    CStr BufString;
    BYTE *hUnixMemPtr;

    BufString = "\n";
    hUnixFile = FileOpenR(FileToCheck);
    if(hUnixFile != INVALID_HANDLE_VALUE)
    {
        // Get 512 bytes of the file ("should be enough for everyone")
        hUnixMem = (HANDLE) AllocMem(518);       // +4
        ReadFile(hUnixFile, hUnixMem, 512, &UnixFileRead, 0);
        PosInFile = 1;
NextCheck:
        PosInFile = Str_InString(PosInFile, (char *) hUnixMem, BufString.Get_String());
        // First char ?
        if(PosInFile == 1)
        {
            PosInFile++;
            goto NextCheck;
        }
        if(PosInFile != 0)
        {
            // Check 1310
            hUnixMemPtr = (BYTE *) hUnixMem;
            if(hUnixMemPtr[PosInFile - 2] != 13) ReturnValue = 1;
        }
        FreeMem((long) hUnixMem);
        FileClose(hUnixFile);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Load a file into memory
HGLOBAL CALLBACK FileLoadIntoMem(CStr FileName, long *BytesRead)
{
    return((HGLOBAL) MLoadFile(FileName.Get_String(), BytesRead));
}

// -----------------------------------------------------------------------
// Save a file from memory
long CALLBACK FileSaveFromMem(CStr FileName, long MemToSave, long LenToSave)
{
    return(MSaveFile(FileName.Get_String(), MemToSave, LenToSave));
}

// -----------------------------------------------------------------------
// Return files from a directory
// (Function accept no argument to return next file)
CStr CALLBACK FileDir(CStr *PathName, int Attributes)
{
    CStr ReturnValue;
    static WIN32_FIND_DATA DirDat;
    static HANDLE hDirFound;
    static int CurrentAttributes;
    int FoundAttributes = 0;

    if(PathName == 0)
    {
CantFoundFirst:
        while(1)
        {
            if(FindNextFile(hDirFound, &DirDat) == 0)
            {
                if(GetLastError() == ERROR_NO_MORE_FILES)
                {
                    FindClose(hDirFound);
                    return(ReturnValue);
                }
            }
            FoundAttributes = DirDat.dwFileAttributes & 0xff;
            if((FoundAttributes == 0x20) || (FoundAttributes == 0))
            {
                ReturnValue = DirDat.cFileName;
                return(ReturnValue);
            }
            if((FoundAttributes & CurrentAttributes) != 0)
            {
                ReturnValue = DirDat.cFileName;
                return(ReturnValue);
            }
        }
    }
    CurrentAttributes = Attributes;
    hDirFound = FindFirstFile(PathName->Get_String(), &DirDat);
    if(hDirFound == INVALID_HANDLE_VALUE) return(ReturnValue);
    FoundAttributes = DirDat.dwFileAttributes & 0xff;
    if((FoundAttributes == 0x20) || (FoundAttributes == 0))
    {
        ReturnValue = DirDat.cFileName;
        return(ReturnValue);
    }
    if((FoundAttributes & CurrentAttributes) != 0)
    {
        ReturnValue = DirDat.cFileName;
        return(ReturnValue);
    }
    goto CantFoundFirst;
}

// -----------------------------------------------------------------------
// Registry functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Update a registry key
long CALLBACK RegistryUpdateKey(HKEY KeyRoot, CStr KeyName, CStr SubKeyName, CStr SubKeyValue, long KeyType)
{
    long rc = 0;
    HKEY hKey = 0;
    DWORD hDepth = 0;
    CStr BinValBase;
    long BinLen = 0;
    CStr BinChar;
    CStr BinChar2;
    CStr BIn_String;
    CStr SubKeyBuf;
    int i = 0;
    long HexTab[16];
    SECURITY_ATTRIBUTES lpAttr;
    CStr BufString;
    
    HexTab[0] = 0;
    HexTab[1] = 1;
    HexTab[2] = 2;
    HexTab[3] = 3;
    HexTab[4] = 4;
    HexTab[5] = 5;
    HexTab[6] = 6;
    HexTab[7] = 7;
    HexTab[8] = 8;
    HexTab[9] = 9;
    HexTab[10] = 10;
    HexTab[11] = 11;
    HexTab[12] = 12;
    HexTab[13] = 13;
    HexTab[14] = 14;
    HexTab[15] = 15;
    lpAttr.nLength = 50;
    lpAttr.lpSecurityDescriptor = 0;
    lpAttr.bInheritHandle = 1;
    BufString = KeyType;
    rc = RegCreateKeyEx(KeyRoot, KeyName.Get_String(), 0, BufString.Get_String(), REG_OPTION_NON_VOLATILE,
                        KEY_ALL_ACCESS, &lpAttr, &hKey, &hDepth);
    if(rc != ERROR_SUCCESS) goto CreateKeyError;
    SubKeyBuf = SubKeyValue;
    if(SubKeyBuf.Len() == 0) SubKeyBuf = " ";
    switch(KeyType)
    {
        case REG_SZ:
            rc = RegSetValueEx(hKey, SubKeyName.Get_String(), 0, KeyType, (BYTE *) SubKeyBuf.Get_String(), SubKeyBuf.Len() * 2);
            break;
        case REG_BINARY:
            BinValBase = SubKeyBuf;
            if((BinValBase.Len() & 1) != 0)
            {
                BinValBase = BinValBase + "0";
            }
            for(i = 1; i <= (long) BinValBase.Len() ; i = i + 2)
            {
                BinChar = BinValBase.Mid(i, 1);
                BinChar = BinChar.Upper_Case();
                BinChar2 = BinValBase.Mid(i+1, 1);
                BinChar2 = BinChar.Upper_Case();
                if(StringIsHexaChar(BinChar) != 0) BinChar = BinChar.Chr((BinChar.Asc() - 55));
                if(StringIsHexaChar(BinChar2) != 0) BinChar2 = BinChar2.Chr((BinChar2.Asc() - 55));
                if(StringIsDigitChar(BinChar) != 0) BinChar = BinChar.Chr(BinChar.Asc());
                if(StringIsDigitChar(BinChar2) != 0) BinChar = BinChar2.Chr(BinChar2.Asc());
                BIn_String = BIn_String + BIn_String.Chr(HexTab[BinChar.Asc()] * 16 + HexTab[BinChar2.Asc()]);
                BinLen++;
            }
            rc = RegSetValueEx(hKey, SubKeyName.Get_String(), 0, KeyType, (BYTE *) BIn_String.Get_String(), BinLen);
    }
    if(rc != ERROR_SUCCESS) goto CreateKeyError;
    rc = RegCloseKey(hKey);
    return(1);
CreateKeyError:
    rc = RegCloseKey(hKey);
    return(0);
}

// -----------------------------------------------------------------------
// Delete a registry key
long CALLBACK RegistryDeleteKey(HKEY KeyRoot, CStr KeyName, CStr SubKeyName, long KeyType)
{
    long rc = 0;
    HKEY hKey = 0;
    DWORD hDepth = 0;
    SECURITY_ATTRIBUTES lpAttr;
    CStr BufString;
    
    lpAttr.nLength = 50;
    lpAttr.lpSecurityDescriptor = 0;
    lpAttr.bInheritHandle = 1;
    BufString = KeyType;
    rc = RegCreateKeyEx(KeyRoot, KeyName.Get_String(), 0, BufString.Get_String(),
                        REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, &lpAttr,
                        &hKey, &hDepth);
    if(rc != ERROR_SUCCESS) goto DeleteKeyError;
    rc = RegDeleteKey(hKey, SubKeyName.Get_String());
    if(rc != ERROR_SUCCESS) goto DeleteKeyError;
    rc = RegCloseKey(hKey);
    return(1);
DeleteKeyError:
    rc = RegCloseKey(hKey);
    return(0);
}

// -----------------------------------------------------------------------
// Retrieve a registry key
CStr CALLBACK RegistryGetKeyValue(HKEY KeyRoot, CStr KeyName, CStr SubKeyRef)
{
    long i = 0;
    long rc = 0;
    HKEY hKey = 0;
    CStr sKeyVal;
    CStr ReturnValue;
    DWORD lKeyValType = 0;
    CStr tmpVal;
    CStr tmpVal2;
    unsigned long KeyValSize = 0;
    long KeyValLen = 0;
    
    rc = RegOpenKeyEx(KeyRoot, KeyName.Get_String(), 0, KEY_ALL_ACCESS, &hKey);
    if(rc != ERROR_SUCCESS) goto GetKeyError;
    tmpVal = tmpVal.String(1024, 1);
    KeyValSize = 1024;
    rc = RegQueryValueEx(hKey, SubKeyRef.Get_String(), 0, &lKeyValType, (BYTE *) tmpVal.Get_String(), &KeyValSize);
    if(rc != ERROR_SUCCESS) goto GetKeyError;
    KeyValLen = tmpVal.Len();
    switch(lKeyValType)
    {
        case REG_SZ:
        case REG_EXPAND_SZ:
            tmpVal = tmpVal.Left(KeyValLen);
            sKeyVal = tmpVal;
            break;
        case REG_DWORD:
            tmpVal = tmpVal.Left(KeyValLen);
            for(i = tmpVal.Len(); i >= 1 ; i--)
            {
                tmpVal2 = tmpVal.Mid(i, 1);
                tmpVal2 = tmpVal2.Hex_To_String(tmpVal2.Asc());
                sKeyVal = sKeyVal + tmpVal2;
            }
            break;
        case REG_BINARY:
            sKeyVal = tmpVal;
            break;
    }
    rc = RegCloseKey(hKey);
    ReturnValue = sKeyVal;
    return(ReturnValue);
GetKeyError:
    rc = RegCloseKey(hKey);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// GDI functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Erase a windows backbround
void CALLBACK GDIFillWindow(HWND hWnd, COLORREF Color)
{
    LOGBRUSH CleanBrush;
    RECT CleanRect;
    HBRUSH hCleanBrush = 0;
    HDC hDCClean = 0;

    GetClientRect(hWnd, &CleanRect);
    CleanBrush.lbStyle = BS_SOLID;
    CleanBrush.lbHatch = 0;
    CleanBrush.lbColor = Color;
    hCleanBrush = CreateBrushIndirect(&CleanBrush);
    hDCClean = GetDC(hWnd);
    FillRect(hDCClean, &CleanRect, hCleanBrush);
    DeleteObject(hCleanBrush);
    ReleaseDC(hWnd, hDCClean);
}

// -----------------------------------------------------------------------
// Fill a window with a given brush
void CALLBACK GDIFillWindowFromBrush(HWND hWnd, HBRUSH EraseBrush)
{
    RECT CleanRect;
    HDC hDCClean = 0;

    GetClientRect(hWnd, &CleanRect);
    hDCClean = GetDC(hWnd);
    FillRect(hDCClean, &CleanRect, EraseBrush);
    ReleaseDC(hWnd, hDCClean);
}

// -----------------------------------------------------------------------
// Fill a window with a given brush and a given device context
void CALLBACK GDIFillWindowFromBrushAndhDC(HWND hWnd, HDC hDC, HBRUSH EraseBrush)
{
    RECT CleanRect;

    GetClientRect(hWnd, &CleanRect);
    FillRect(hDC, &CleanRect, EraseBrush);
}

// -----------------------------------------------------------------------
// Create a colored brush
HBRUSH CALLBACK GDICreateColorBrush(COLORREF Color)
{
    LOGBRUSH SubEditBrush;

    SubEditBrush.lbStyle = BS_SOLID;
    SubEditBrush.lbHatch = 0;
    SubEditBrush.lbColor = Color;
    return(CreateBrushIndirect(&SubEditBrush));
}

// -----------------------------------------------------------------------
// Create a font
HFONT CALLBACK GDIObtainFont(CStr FontNameToObtain, long FontSizeToObtain, HWND hWnd, long Bold, long Italic)
{
    HFONT ReturnValue = 0;
    LOGFONT WACMFont;
    CStr BufString;
    int i = 0;
    HDC WAFhDC = 0;

    WAFhDC = GetDC(hWnd);
    WACMFont.lfHeight = -(long)((FontSizeToObtain * GetDeviceCaps(WAFhDC, LOGPIXELSY)) / 72);
    WACMFont.lfWidth = 0;
    WACMFont.lfEscapement = 0;
    WACMFont.lfOrientation = 0;
    if(Bold == 1) WACMFont.lfWeight = FW_BOLD;
    else WACMFont.lfWeight = FW_NORMAL;
    WACMFont.lfItalic = (BYTE) Italic;
    WACMFont.lfUnderline = 0;
    WACMFont.lfStrikeOut = 0;
    WACMFont.lfCharSet = DEFAULT_CHARSET;
    WACMFont.lfOutPrecision = OUT_OUTLINE_PRECIS;
    WACMFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    WACMFont.lfQuality = ANTIALIASED_QUALITY;
    WACMFont.lfPitchAndFamily = FIXED_PITCH | FF_DONTCARE;
    for(i = 0 ; i <= LF_FACESIZE - 1; i++)
    {
        if(i == (long) FontNameToObtain.Len()) break;
        BufString = FontNameToObtain.Mid(i + 1, 1);
        WACMFont.lfFaceName[i] = BufString.Asc();
    }
    WACMFont.lfFaceName[i] = 0;
    ReturnValue = CreateFontIndirect(&WACMFont);
    ReleaseDC(hWnd, WAFhDC);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the width of a font
long CALLBACK GDIGetFontWidth(HWND hWnd, HFONT hFont)
{
    HDC FonthDC = 0;
    HGDIOBJ hOldFont = 0;
    TEXTMETRIC FontMetrics;

    FonthDC = GetDC(hWnd);
    hOldFont = SelectObject(FonthDC, hFont);
    GetTextMetrics(FonthDC, &FontMetrics);
    SelectObject(FonthDC, hOldFont);
    ReleaseDC(hWnd, FonthDC);
    return(FontMetrics.tmAveCharWidth);
}

// -----------------------------------------------------------------------
// Retrieve the height of a font
long CALLBACK GDIGetFontHeight(HWND hWnd, HFONT hFont)
{
    HDC FonthDC = 0;
    HGDIOBJ hOldFont = 0;
    TEXTMETRIC FontMetrics;

    FonthDC = GetDC(hWnd);
    hOldFont = SelectObject(FonthDC, hFont);
    GetTextMetrics(FonthDC, &FontMetrics);
    SelectObject(FonthDC, hOldFont);
    ReleaseDC(hWnd, FonthDC);
    return(FontMetrics.tmHeight + FontMetrics.tmExternalLeading);
}

// -----------------------------------------------------------------------
// Draw a horizontal separation
void CALLBACK GDIDrawHorzSep(HWND hWnd, long X, long Y, long Width)
{
    GDIDrawLine(hWnd, X, Y, X + Width, Y, GetSysColor(COLOR_BTNSHADOW));
    GDIDrawLine(hWnd, X, Y + 1, X + Width, Y + 1, GetSysColor(COLOR_BTNHILIGHT));
}

// -----------------------------------------------------------------------
// Draw a vertical separation
void CALLBACK GDIDrawVertSep(HWND hWnd, long X, long Y, long Height)
{
    GDIDrawLine(hWnd, X, Y, X, Y + Height, GetSysColor(COLOR_BTNSHADOW));
    GDIDrawLine(hWnd, X + 1, Y, X + 1, Y + Height, GetSysColor(COLOR_BTNHILIGHT));
}

// -----------------------------------------------------------------------
// Draw a line
void CALLBACK GDIDrawLine(HWND hWnd, long X1, long Y1, long X2, long Y2, COLORREF LineColor)
{
    POINT WALinePT;
    HPEN WAhPen = 0;
    HGDIOBJ WAOldhPen = 0;
    HDC WALineHDC = 0;

    WALineHDC = GetDC(hWnd);
    WAhPen = CreatePen(PS_SOLID, 0, LineColor);
    MoveToEx(WALineHDC, X1, Y1, &WALinePT);
    WAOldhPen = SelectObject(WALineHDC, WAhPen);
    LineTo(WALineHDC, X2, Y2);
    SelectObject(WALineHDC, WAOldhPen);
    DeleteObject(WAhPen);
    ReleaseDC(hWnd, WALineHDC);
}

// -----------------------------------------------------------------------
// Draw a line with XOR combination
void CALLBACK GDIDrawLineXOR(HWND hWnd, long X1, long Y1, long X2, long Y2, COLORREF LineColor)
{
    POINT WALinePT;
    HPEN WAhPen = 0;
    HGDIOBJ WAOldhPen = 0;
    HDC WALineHDC = 0;
    long OldROPMode = 0;

    WALineHDC = GetDC(hWnd);
    WAhPen = CreatePen(PS_SOLID, 0, LineColor);
    MoveToEx(WALineHDC, X1, Y1, &WALinePT);
    WAOldhPen = SelectObject(WALineHDC, WAhPen);
    OldROPMode = SetROP2(WALineHDC, R2_XORPEN);
    LineTo(WALineHDC, X2, Y2);
    SetROP2(WALineHDC, OldROPMode);
    SelectObject(WALineHDC, WAOldhPen);
    DeleteObject(WAhPen);
    ReleaseDC(hWnd, WALineHDC);
}

// -----------------------------------------------------------------------
// Draw a pixel
long CALLBACK GDIDrawPixel(HWND hWnd, long X, long Y, long PixelColor)
{
    HDC hDC = 0;
    long ReturnValue = 0;

    hDC = GetDC(hWnd);
    ReturnValue = SetPixel(hDC, X, Y, PixelColor);
    ReleaseDC(hWnd, hDC);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Draw a text on screen
void CALLBACK GDIWriteText(HDC hDC, long X, long Y, CStr Text, long TextColor,
                             HFONT hTextFont, long TextTransparent, long BackGroundColor)
{
    HGDIOBJ OldhObj = 0;
    long OldBkMode = 0;
    long OldTextColor = 0;
    long OldBkColor = 0;

    OldhObj = SelectObject(hDC, hTextFont);
    OldTextColor = SetTextColor(hDC, TextColor);
    if(TextTransparent == 1)
    {
        OldBkMode = SetBkMode(hDC, TRANSPARENT);
    }
    else
    {
        OldBkMode = SetBkMode(hDC, OPAQUE);
        OldBkColor = SetBkColor(hDC, BackGroundColor);
    }
    TextOut(hDC, X, Y, Text.Get_String(), Text.Len());
    if(OldBkColor != 0) SetBkColor(hDC, OldBkColor);
    SetTextColor(hDC, OldTextColor);
    SetBkMode(hDC, OldBkMode);
    SelectObject(hDC, OldhObj);
}

// -----------------------------------------------------------------------
// Draw a clipped text on screen
void CALLBACK GDIWriteClippedText(HDC hDC, long X, long Y, long Width, long Height, CStr Text,
                                    long TextColor, HFONT hTextFont, long TextTransparent, long BackGroundColor)
{
    RECT ClipRect;
    HGDIOBJ OldhObj = 0;
    long OldBkMode = 0;
    long OldTextColor = 0;
    long OldBkColor = 0;

    OldhObj = SelectObject(hDC, hTextFont);
    OldTextColor = SetTextColor(hDC, TextColor);
    ClipRect.left = X;
    ClipRect.top = Y;
    ClipRect.right = X + Width;
    ClipRect.bottom = Y + Height;
    if(TextTransparent == 1)
    {
        OldBkMode = SetBkMode(hDC, TRANSPARENT);
        ExtTextOut(hDC, X, Y, ETO_CLIPPED, &ClipRect, Text.Get_String(), Text.Len(), NULL);
    }
    else
    {
        OldBkMode = SetBkMode(hDC, OPAQUE);
        OldBkColor = SetBkColor(hDC, BackGroundColor);
        ExtTextOut(hDC, X, Y, ETO_CLIPPED | ETO_OPAQUE, &ClipRect, Text.Get_String(), Text.Len(), NULL);
        SetBkColor(hDC, OldBkColor);
    }
    SetTextColor(hDC, OldTextColor);
    SetBkMode(hDC, OldBkMode);
    SelectObject(hDC, OldhObj);
}

// -----------------------------------------------------------------------
// Retrieve text width
long CALLBACK GDIGetTextWidth(HWND hWnd, HFONT hFont, CStr Txt)
{
    long ReturnValue = 0;
    HDC TWHdc = 0;
    HGDIOBJ OldFont = 0;
    SIZE FontSize;

    TWHdc = GetDC(hWnd);
    // Select the font
    OldFont = SelectObject(TWHdc, hFont);
    GetTextExtentPoint32(TWHdc, Txt.Get_String(), Txt.Len(), &FontSize);
    ReturnValue = FontSize.cx;
    SelectObject(TWHdc, OldFont);
    ReleaseDC(hWnd, TWHdc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve text height
long CALLBACK GDIGetTextHeight(HWND hWnd, HFONT hFont, CStr Txt)
{
    long ReturnValue = 0;
    HDC TWHdc = 0;
    HGDIOBJ OldFont = 0;
    SIZE FontSize;

    TWHdc = GetDC(hWnd);
    // Select the font
    OldFont = SelectObject(TWHdc, hFont);
    GetTextExtentPoint32(TWHdc, Txt.Get_String(), Txt.Len(), &FontSize);
    ReturnValue = FontSize.cy;
    SelectObject(TWHdc, OldFont);
    ReleaseDC(hWnd, TWHdc);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Create a back DC for double buffering purposes
HDC CALLBACK GDICreateBackDC(LPBACKDC BackStruct)
{
    RECT GridRect;
    RECT WinRect;
    BITMAPINFO BitmapInfos;
    void *DIBBits = 0;
    HDC BackDC = 0;
    HBITMAP BackhBitmap = 0;
    HGDIOBJ OldObject = 0;
    HBRUSH hBrush = 0;
    
    BackDC = CreateCompatibleDC(0);
    BackStruct->hDC = BackDC;
    if(BackDC != 0)
    {
        GetClientRect(BackStruct->hWnd, &WinRect);
        GridRect.left = 0;
        GridRect.top = 0;
        GridRect.right = WinRect.right - WinRect.left;
        GridRect.bottom = WinRect.bottom - WinRect.top;
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
        BackhBitmap = CreateDIBSection(BackDC, &BitmapInfos, DIB_RGB_COLORS, &DIBBits, NULL, 0);
        BackStruct->Bitmap = BackhBitmap;
        if(BackhBitmap != 0)
        {
            OldObject = SelectObject(BackDC, BackhBitmap);
            BackStruct->OldObject = OldObject;
            hBrush = GDICreateColorBrush(BackStruct->Color);
            FillRect(BackDC, &GridRect, hBrush);
        }
    }
    return(BackDC);
}

// -----------------------------------------------------------------------
// Destroy a back DC for double buffering
void CALLBACK GDIDestroyBackDC(LPBACKDC BackStruct)
{
    if(BackStruct->hDC != 0)
    {
        SelectObject(BackStruct->hDC, BackStruct->OldObject);
        DeleteObject(BackStruct->Bitmap);
        DeleteDC(BackStruct->hDC);
    }
}

// -----------------------------------------------------------------------
// Display a back DC
long CALLBACK GDIBlitBackDC(LPRECT Dimensions, LPBACKDC BackStruct)
{
    long ReturnValue = 0;
    HDC DesthDC = 0;

    DesthDC = GetDC(BackStruct->hWnd);
    ReturnValue = BitBlt(DesthDC, Dimensions->left, Dimensions->top, Dimensions->right, Dimensions->bottom, BackStruct->hDC, 0, 0, SRCCOPY);
    ReleaseDC(BackStruct->hWnd, DesthDC);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Calc the luminosity of a color
// 0 <= LuminosityFactor <= x
long CALLBACK GDIColorCalcLuminosity(COLORREF Color, long LuminosityFactor)
{
    COLORREF Light_Color;
    COLORREF Light_ColorR;
    COLORREF Light_ColorG;
    COLORREF Light_ColorB;
    
    if(LuminosityFactor < 0) LuminosityFactor = 0;
    Light_ColorR = (Color & 0xFF0000) >> 16;
    Light_ColorG = (Color & 0xFF00) >> 8;
    Light_ColorB = (Color & 0xFF);
    Light_ColorR = Light_ColorR * LuminosityFactor / 100;
    Light_ColorG = Light_ColorG * LuminosityFactor / 100;
    Light_ColorB = Light_ColorB * LuminosityFactor / 100;
    if((long) Light_ColorR > (long) 0xFF) Light_ColorR = 0xFF;
    if((long) Light_ColorG > (long) 0xFF) Light_ColorG = 0xFF;
    if((long) Light_ColorB > (long) 0xFF) Light_ColorB = 0xFF;
    if((long) Light_ColorR < (long) 0) Light_ColorR = 0;
    if((long) Light_ColorG < (long) 0) Light_ColorG = 0;
    if((long) Light_ColorB < (long) 0) Light_ColorB = 0;
    Light_Color = (Light_ColorR << 16) | (Light_ColorG << 8) | Light_ColorB;
    return(Light_Color);
}

// -----------------------------------------------------------------------
// Retrieve stock fonts
HFONT CALLBACK GDIGetSerif(void)
{
    return(WASerifFont);
}

HFONT CALLBACK GDIGetSerif10(void)
{
    return(WASerifFont10);
}

HFONT CALLBACK GDIGetCourierNew9(void)
{
    return(WACourrierNewFont9);
}

// -----------------------------------------------------------------------
// Windows objects functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Terminate a window hook
long CALLBACK ControlNextHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Send a command to a control
long CALLBACK ControlSendMessage(HWND hWnd, long Command, long lParam)
{
    return(SendMessage(hWnd, WM_COMMAND, Command, lParam));
}

// -----------------------------------------------------------------------
// Get control height
long CALLBACK ControlHeight(HWND hWnd)
{
    long ReturnValue = 0;
    RECT CRct;
    
    GetWindowRect(hWnd, &CRct);
    if(ControlIsVisible(hWnd) == 0) ReturnValue = 0;
    else ReturnValue = CRct.bottom - CRct.top;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get control client top
long CALLBACK ControlClientTop(HWND hWnd)
{
    long ReturnValue = 0;
    RECT CRct;

    GetClientRect(hWnd, &CRct);
    if(ControlIsVisible(hWnd) == 0) ReturnValue = 0;
    else ReturnValue = CRct.top;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get control client left
long CALLBACK ControlClientLeft(HWND hWnd)
{
    long ReturnValue = 0;
    RECT CRct;

    GetClientRect(hWnd, &CRct);
    if(ControlIsVisible(hWnd) == 0) ReturnValue = 0;
    else ReturnValue = CRct.left;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get control client width
long CALLBACK ControlClientWidth(HWND hWnd)
{
    long ReturnValue = 0;
    RECT CRct;

    GetClientRect(hWnd, &CRct);
    if(ControlIsVisible(hWnd) == 0) ReturnValue = 0;
    else ReturnValue = CRct.right - CRct.left;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get control client height
long CALLBACK ControlClientHeight(HWND hWnd)
{
    long ReturnValue = 0;
    RECT CRct;

    GetClientRect(hWnd, &CRct);
    if(ControlIsVisible(hWnd) == 0) ReturnValue = 0;
    else ReturnValue = CRct.bottom - CRct.top;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Bring a control to top
long CALLBACK ControlBringToTop(HWND hWnd)
{
    return(SetWindowPos(hWnd, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DEFERERASE | SWP_SHOWWINDOW));
}

// -----------------------------------------------------------------------
// Bring a control to bottom
long CALLBACK ControlBringToBottom(HWND hWnd)
{
    return(SetWindowPos(hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_DEFERERASE | SWP_SHOWWINDOW));
}

// -----------------------------------------------------------------------
// Set a control on top of all others
long CALLBACK ControlSetTopMost(HWND hWnd)
{
    return(SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE));
}

// -----------------------------------------------------------------------
// Remove TOPMOST status of a control
long CALLBACK ControlRemTopMost(HWND hWnd)
{
    return(SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOMOVE | SWP_NOSIZE));
}

// -----------------------------------------------------------------------
// Refresh a control
void CALLBACK ControlRefresh(HWND hWnd)
{
    RECT RRect;
    HWND CParent = 0;

    CParent = GetParent(hWnd);
    RRect.left = ControlLeft(hWnd) - ControlLeft(CParent) - GetSystemMetrics(SM_CXDLGFRAME);
    RRect.top = ControlTop(hWnd) - ControlTop(CParent) - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYDLGFRAME);
    RRect.right = ControlClientWidth(hWnd) + RRect.left;
    RRect.bottom = ControlClientHeight(hWnd) + RRect.top;
    InvalidateRect(CParent, &RRect, 1);
    UpdateWindow(CParent);
}

// -----------------------------------------------------------------------
// Refresh a control (local to a control)
void CALLBACK ControlRefreshLocal(HWND hWnd)
{
    RECT RRect;
    
    RRect.left = 0;
    RRect.top = 0;
    RRect.right = ControlClientWidth(hWnd);
    RRect.bottom = ControlClientHeight(hWnd);
    InvalidateRect(hWnd, &RRect, 1);
    UpdateWindow(hWnd);
}

// -----------------------------------------------------------------------
// Trigger a WM_PAINT event for a given control
void CALLBACK ControlRepaint(HWND hWnd)
{
    RECT RRect;
    
    GetClientRect(hWnd, &RRect);
    InvalidateRect(hWnd, &RRect, 1);
    UpdateWindow(hWnd);
}

// -----------------------------------------------------------------------
// Resize a control
long CALLBACK ControlResize(HWND hControl, long CtlLeft, long CtlTop, long CtlWidth, long CtlHeight)
{
    ControlBound(GetParent(hControl), CtlLeft, CtlTop, CtlWidth, CtlHeight);
    return(MoveWindow(hControl, CtlLeft, CtlTop, CtlWidth, CtlHeight, 1));
}

// -----------------------------------------------------------------------
// Bound the dimensions of a control
long CALLBACK ControlFreeze(HWND hControl, long FreezeState)
{
    long ReturnValue = 0;
    
    switch(FreezeState)
    {
        case 0:
            ReturnValue = SendMessage(hControl, WM_SETREDRAW, 1, (long) 0);
            break;
        default:
            ReturnValue = SendMessage(hControl, WM_SETREDRAW, 0, (long) 0);
            break;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Bound the dimensions of a control
void CALLBACK ControlBound(HWND hParentControl, long &CtlLeft, long &CtlTop, long &CtlWidth, long &CtlHeight)
{
    RECT WRect;

    GetClientRect(hParentControl, &WRect);
    if(CtlTop == -1) CtlTop = WRect.top;
    if(CtlLeft == -1) CtlLeft = WRect.left;
    if(CtlWidth == -1) CtlWidth = WRect.right - CtlLeft;
    if(CtlHeight == -1) CtlHeight = WRect.bottom - CtlTop;
}

// -----------------------------------------------------------------------
// Enable/Disable a control
long CALLBACK ControlEnable(HWND hWnd, long CState)
{
    return(EnableWindow(hWnd, CState));
}

// -----------------------------------------------------------------------
// Show/Hide a control
long CALLBACK ControlVisible(HWND hWnd, long CState)
{
    long ReturnValue = 0;
    
    switch(CState)
    {
        case 0:
            ReturnValue = ShowWindow(hWnd, SW_HIDE);
            break;
        default:
            ReturnValue = ShowWindow(hWnd, SW_SHOW);
            break;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve visible state of a control
long CALLBACK ControlIsVisible(HWND hWnd)
{
    return(IsWindowVisible(hWnd));
}

// -----------------------------------------------------------------------
// Retrieve enable state of a control
long CALLBACK ControlIsEnabled(HWND hWnd)
{
    return(IsWindowEnabled(hWnd));
}

// -----------------------------------------------------------------------
// Retrieve a text from a control
CStr CALLBACK ControlGetText(HWND hWnd)
{
    CStr ReturnValue;
    long TxtSize = 0;

    TxtSize = SendMessage(hWnd, WM_GETTEXTLENGTH, 0, (long) 0);
    if(TxtSize > 0)
    {
        ReturnValue = ReturnValue.String(TxtSize, 1);
        SendMessage(hWnd, WM_GETTEXT, TxtSize + 1, (long) ReturnValue.Get_String());
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the length of a text control
long CALLBACK ControlGetTextLen(HWND hWnd)
{
    return(SendMessage(hWnd, WM_GETTEXTLENGTH, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set the text of a control
long CALLBACK ControlSetText(HWND hWnd, CStr TextToSet)
{
    return(SendMessage(hWnd, WM_SETTEXT, 0, (long) TextToSet.Get_String()));
}

// -----------------------------------------------------------------------
// Set a text to a control by passing it's address
long CALLBACK ControlSetTextByAddr(HWND hWnd, long TextToSet)
{
    return(SendMessage(hWnd, WM_SETTEXT, 0, TextToSet));
}

// -----------------------------------------------------------------------
// Retrieve type of a notified message
long CALLBACK ControlGetNotifiedMsg(long lParam)
{
    NMHDR HeaderNotify;

    CopyMemory(&HeaderNotify, (void *) lParam, sizeof(HeaderNotify));
    return(HeaderNotify.code);
}

// -----------------------------------------------------------------------
// Retrieve window handle of a notified message
HWND CALLBACK ControlGetNotifiedhWnd(long lParam)
{
    NMHDR HeaderNotify;
    
    CopyMemory(&HeaderNotify, (void *) lParam, sizeof(HeaderNotify));
    return(HeaderNotify.hwndFrom);
}

// -----------------------------------------------------------------------
// Retrieve window handle of a notified message
long CALLBACK ControlGetNotifiedID(long lParam)
{
    NMHDR HeaderNotify;

    CopyMemory(&HeaderNotify, (void *) lParam, sizeof(HeaderNotify));
    return(HeaderNotify.idFrom);
}

// -----------------------------------------------------------------------
// Get control top
long CALLBACK ControlTop(HWND hWnd)
{
    long ReturnValue = 0;
    RECT CRct;
    
    GetWindowRect(hWnd, &CRct);
    if(ControlIsVisible(hWnd) == 0) ReturnValue = 0;
    else ReturnValue = CRct.top;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get control left
long CALLBACK ControlLeft(HWND hWnd)
{
    long ReturnValue = 0;
    RECT CRct;

    GetWindowRect(hWnd, &CRct);
    if(ControlIsVisible(hWnd) == 0) ReturnValue = 0;
    else ReturnValue = CRct.left;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get control width
long CALLBACK ControlWidth(HWND hWnd)
{
    long ReturnValue = 0;
    RECT CRct;

    GetWindowRect(hWnd, &CRct);
    if(ControlIsVisible(hWnd) == 0) ReturnValue = 0;
    else ReturnValue = CRct.right - CRct.left;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve window handle under cursor
HWND CALLBACK ControlGetHwndFromPoint(void)
{
    POINT DragPoint;
    GetCursorPos(&DragPoint);
//    return(WindowFromPoint(DragPoint.x, DragPoint.y));
    return(WindowFromPoint(DragPoint));
}

// -----------------------------------------------------------------------
// Retrieve the notified command of a control
long CALLBACK ControlGetNotifiedCommand(long wParam)
{
    return((short) ((wParam & 0xFFF0000) / 0x10000));
}

// -----------------------------------------------------------------------
// Retrieve the X position of a passed mouse position
long CALLBACK ControlGetXMousePos(long lParam)
{
    long ReturnValue = 0;

    ReturnValue = (lParam & 0xFFFF);
    if(ReturnValue & 0x8000) ReturnValue = 0;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the Y position of a passed mouse position
long CALLBACK ControlGetYMousePos(long lParam)
{
    long ReturnValue = 0;

    ReturnValue = (short) ((lParam & 0xFFFF0000) / 0x10000);
    if(ReturnValue < 0) ReturnValue = 0;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set MS Sans Serif size 8 font for a control
long CALLBACK ControlSetSerif8(HWND hWnd)
{
    return(SendMessage(hWnd, WM_SETFONT, (UINT) WASerifFont, (long) 0));
}

// -----------------------------------------------------------------------
// Retieve the font used by a control
HFONT CALLBACK ControlGetFont(HWND hWnd)
{
    return((HFONT) SendMessage(hWnd, WM_GETFONT, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Retieve an icon associated with a control
HICON CALLBACK ControlGetIcon(HWND hWnd, long IconType)
{
    return((HICON) SendMessage(hWnd, WM_GETICON, IconType, (long) 0));
}

// -----------------------------------------------------------------------
// Retieve the hotkey used by a control
long CALLBACK ControlGetHotKey(HWND hWnd)
{
    return(SendMessage(hWnd, WM_GETHOTKEY, 0, (long) 0));
}

// -----------------------------------------------------------------------
// Set a font for a control
long CALLBACK ControlSetFont(HWND hWnd, HFONT hFont)
{
    return(SendMessage(hWnd, WM_SETFONT, (long) hFont, (long) 1));
}

// -----------------------------------------------------------------------
// Set a font for a control without redrawing it
long CALLBACK ControlSetFontNoRedraw(HWND hWnd, HFONT hFont)
{
    return(SendMessage(hWnd, WM_SETFONT, (long) hFont, (long) 0));
}

// -----------------------------------------------------------------------
// Check if window is a correct child window
long CALLBACK ControlIsWindowChild(HWND hWnd)
{
    long ReturnValue = 0;
    CStr ClassNameToRetrieve;

    ClassNameToRetrieve = ClassNameToRetrieve.String(22, 1);
    GetClassName(hWnd, ClassNameToRetrieve.Get_String(), 22);
    if(_strcmpi(ClassNameToRetrieve.Left(21).Get_String(), "WAMDIChildDialogClass") == 0) ReturnValue = 1;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the class name of a window
CStr CALLBACK ControlGetClassName(HWND hWnd)
{
    CStr ReturnValue;

    ReturnValue = ReturnValue.String(128, 1);
    GetClassName(hWnd, ReturnValue.Get_String(), 128);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set mouse pointer of a control class
long CALLBACK ControlSetClassCursor(HWND hWnd, HCURSOR hCursor)
{
    return(SetClassLong(hWnd, GCL_HCURSOR, (long) hCursor));
}

// -----------------------------------------------------------------------
// Hook a window
long CALLBACK ControlHookWin(HWND hWnd, WNDPROC WindowProc)
{
    long ReturnValue = 0;

    switch((long) WindowProc)
    {
        case 0:
            ReturnValue = SetWindowLong(hWnd, GWL_USERDATA, SetWindowLong(hWnd, GWL_WNDPROC, (long) &WADefWinHook));
            break;
        case -1:
            // Default class hook
            ReturnValue = SetWindowLong(hWnd, GWL_WNDPROC, (long) 0);
            break;
        default:
            ReturnValue = SetWindowLong(hWnd, GWL_USERDATA, SetWindowLong(hWnd, GWL_WNDPROC, (long) WindowProc));
            break;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// UnHook a window
long CALLBACK ControlUnHookWin(HWND hWnd)
{
    return(SetWindowLong(hWnd, GWL_WNDPROC, GetWindowLong(hWnd, GWL_USERDATA)));
}

// -----------------------------------------------------------------------
// Retrieve window show state
long CALLBACK ControlGetWindowState(HWND hWnd)
{
    WINDOWPLACEMENT GetWinPlacement;

    GetWinPlacement.length = sizeof(GetWinPlacement);
    GetWindowPlacement(hWnd, &GetWinPlacement);
    return(GetWinPlacement.showCmd);
}

// -----------------------------------------------------------------------
// Close a windows object
long CALLBACK ControlClose(HWND hWnd)
{
    long ReturnValue = 0;

    if(hWnd != 0) ReturnValue = SendMessage(hWnd, WM_CLOSE, 0, (long) 0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Call a dialog window proc
LRESULT CALLBACK ControlCallwindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Windows common dialogs functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Choose a color from the palette
long ComDlgChooseColor(HWND hWnd, long DefaultCol)
{
    memset(&MyColor, 0, sizeof(MyColor));
    MyColor.hInstance = (HWND) ApphInstance;
    MyColor.hwndOwner = hWnd;
    MyColor.rgbResult = DefaultCol;
    MyColor.lCustData = 0;
    MyColor.lpCustColors = (COLORREF *) &MyCustomColors;
    MyColor.lpfnHook = 0;
    MyColor.Flags = CC_FULLOPEN | CC_RGBINIT;
    MyColor.lStructSize = sizeof(MyColor);
    return(ChooseColor(&MyColor));
}

// -----------------------------------------------------------------------
// Choose a file to open
CStr ComDlgGetOpenFileName(HWND hWnd, CStr OFilters, CStr InitDir, long MultiSelect, CStr DefaultDir)
{
    CStr TmpOp;
    CStr ReturnValue;
    CStr BufString;
    CStr BufString1;
    char *BufStringString = 0;
    int i = 0;
    int LenOpenFile = 0;
    long *SplitArray = 0;
    CStr SelectedFilesConcat;
    CStr FirstElement;

    memset(&MyOpenFName, 0, sizeof(MyOpenFName));
    if(InitDir.Len() == 0) InitDir = DefaultDir;
    if(_OnOlderWindows) MyOpenFName.lStructSize = 0x4c;     // Old size
    else MyOpenFName.lStructSize = sizeof(MyOpenFName);
    MyOpenFName.hwndOwner = hWnd;
    MyOpenFName.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_LONGNAMES;
    if(MultiSelect == 1) MyOpenFName.Flags = MyOpenFName.Flags | OFN_ALLOWMULTISELECT;
    MyOpenFName.lpstrDefExt = "";
    MyOpenFName.lpstrInitialDir = InitDir.Get_String();
    BufString1 = TmpOp.String(256, 1);
    MyOpenFName.lpstrFile = BufString1.Get_String();
    MyOpenFName.lpstrFile[0] = 0;
    MyOpenFName.nMaxFile = 255;
    BufString = OFilters + (CStr) BufString.String(2, '|').Get_String();
    LenOpenFile = BufString.Len();
    BufStringString = BufString.Get_String();
    for(i = 1; i <= LenOpenFile; i++)
    {
        if(BufStringString[i - 1] == '|') BufStringString[i - 1] = 0;
    }
    MyOpenFName.lpstrFilter = BufStringString;
    MyOpenFName.nFilterIndex = 1;
    if(MultiSelect == 1) MyOpenFName.lpstrTitle = "Select file(s) to open";
    else MyOpenFName.lpstrTitle = "Select a file to open";
    MyOpenFName.hInstance = ApphInstance;
    if(GetOpenFileName(&MyOpenFName) != 0)
    {
        if(MultiSelect == 0)
        {
            TmpOp = MyOpenFName.lpstrFile;
            TmpOp = TmpOp.Trim();
            TmpOp = TmpOp.Left(TmpOp.Len());
            TmpOp = TmpOp.Trim();
        }
        else
        {
            // The results will be separated with 0s
            // Replace them with char 2
            for(i = 1; i <= (long) MyOpenFName.nMaxFile; i++)
            {
                if(MyOpenFName.lpstrFile[i - 1] == 1)
                {
                    i--;
                    break;
                }
                if(MyOpenFName.lpstrFile[i - 1] == 0) MyOpenFName.lpstrFile[i - 1] = 2;
            }
            // Remove trailing chars
            while(MyOpenFName.lpstrFile[i - 1] == 2)
            {
                MyOpenFName.lpstrFile[i - 1] = 0;
                i--;
            }
            TmpOp = MyOpenFName.lpstrFile;
            SplitArray = StringSplit(&TmpOp, BufString.Chr(2));
            if(StringGetSplitUBound(SplitArray) > 0)
            {
                FirstElement = StringGetSplitElement(&TmpOp, SplitArray, 0);
                // Check anti-slash of first entry (directory)
                if(strcmp(FirstElement.Right(1).Get_String(), "\\") == 0)
                {
                    SelectedFilesConcat = StringReplace(&TmpOp, BufString.Chr(2), BufString.Chr(2) + (CStr) FirstElement, 1, -1, Binary_Compare);
                }
                else
                {
                    SelectedFilesConcat = StringReplace(&TmpOp, BufString.Chr(2), BufString.Chr(2) + (CStr) FirstElement + (CStr) "\\", 1, -1, Binary_Compare);
                }
                SelectedFilesConcat = SelectedFilesConcat.Mid(SelectedFilesConcat.In_Str(1, BufString.Chr(2)) + 1);
                StringReleaseSplit(SplitArray);
                TmpOp = SelectedFilesConcat;
                ReturnValue = TmpOp;
                return(ReturnValue);
            }
            // Single file selected ?
            if(StringGetSplitUBound(SplitArray) == 0)
            {
                FirstElement = StringGetSplitElement(&TmpOp, SplitArray, 0);
                StringReleaseSplit(SplitArray);
                // Must be first element not 0
                SelectedFilesConcat = FirstElement + (CStr) BufString.Chr(2).Get_String();
                TmpOp = SelectedFilesConcat;
            }
            else
            {
                // No files
                StringReleaseSplit(SplitArray);
                TmpOp = "";
            }

        }
    }
    ReturnValue = TmpOp;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Choose a file to save
CStr ComDlgGetSaveFileName(HWND hWnd, CStr OFilters, CStr InitDir, CStr DefaultDir)
{
    CStr TmpSv;
    CStr BufString;
    CStr BufString1;
    CStr ReturnValue;
    char *BufStringString = 0;
    int i = 0;
    int LenSaveFile = 0;
    
    memset(&MySaveFName, 0, sizeof(MySaveFName));
    if(InitDir.Len() == 0) InitDir = DefaultDir;
    if(_OnOlderWindows) MySaveFName.lStructSize = 0x4c;     // Old size
    else MySaveFName.lStructSize = sizeof(MySaveFName);
    MySaveFName.hwndOwner = hWnd;
    MySaveFName.Flags = OFN_EXPLORER | OFN_HIDEREADONLY | OFN_SHOWHELP | OFN_PATHMUSTEXIST | OFN_LONGNAMES | OFN_OVERWRITEPROMPT;
    MySaveFName.lpstrDefExt = "";
    MySaveFName.lpstrInitialDir = InitDir.Get_String();
    BufString1 = TmpSv.String(255, 1);
    MySaveFName.lpstrFile = BufString1.Get_String();
    MySaveFName.lpstrFile[0] = 0;
    MySaveFName.nMaxFile = 255;
    BufString = OFilters + (CStr) BufString.String(2, '|').Get_String();
    LenSaveFile = BufString.Len();
    BufStringString = BufString.Get_String();
    for(i = 1; i <= LenSaveFile; i++)
    {
        if(BufStringString[i - 1] == '|') BufStringString[i - 1] = 0;
    }
    MySaveFName.lpstrFilter = BufStringString;
    MySaveFName.nFilterIndex = 1;
    MySaveFName.lpstrTitle = "Select a file to save";
    MySaveFName.hInstance = ApphInstance;
    if(GetSaveFileName(&MySaveFName) != 0)
    {
        TmpSv = MySaveFName.lpstrFile;
        TmpSv = TmpSv.Trim();
    }
    TmpSv = TmpSv.Left(TmpSv.Len());
    TmpSv = TmpSv.Trim();
    ReturnValue = TmpSv;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Choose a font
long ComDlgChooseFont(HWND hWnd, CStr FntName, long FntSize, long FixedOnly)
{
    int i = 0;
    HDC FhDC = 0;
    long ReturnValue = 0;

    FhDC = GetDC(hWnd);
    memset(&MyLogFont, 0, sizeof(MyLogFont));
    memset(&MyFont, 0, sizeof(MyFont));
    MyLogFont.lfHeight = -((long) (FntSize * GetDeviceCaps(FhDC, LOGPIXELSY)) / 72);
    MyLogFont.lfWidth = 0;
    MyLogFont.lfEscapement = 0;
    MyLogFont.lfOrientation = 0;
    MyLogFont.lfWeight = FW_NORMAL;
    MyLogFont.lfItalic = 0;
    MyLogFont.lfUnderline = 0;
    MyLogFont.lfStrikeOut = 0;
    MyLogFont.lfCharSet = DEFAULT_CHARSET;
    MyLogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    MyLogFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    MyLogFont.lfQuality = DEFAULT_QUALITY;
    MyLogFont.lfPitchAndFamily = DEFAULT_PITCH;
    for(i = 0; i <= LF_FACESIZE - 1; i++)
    {
        if(i == FntName.Len()) break;
        MyLogFont.lfFaceName[i] = FntName.Mid(i + 1, 1).Asc();
    }
    MyLogFont.lfFaceName[i] = 0;
    hMemFont = calloc(sizeof(MyLogFont), 1);
    RtlCopyMemory(hMemFont, &MyLogFont, sizeof(MyLogFont));
    MyFont.lStructSize = sizeof(MyFont);
    MyFont.hwndOwner = hWnd;
    MyFont.lpLogFont = (LPLOGFONT) hMemFont;
    MyFont.iPointSize = FntSize * 10;
    if(FixedOnly == 1) MyFont.Flags = CF_FIXEDPITCHONLY | CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_LIMITSIZE;
    else MyFont.Flags = CF_SCREENFONTS | CF_INITTOLOGFONTSTRUCT | CF_LIMITSIZE;
    MyFont.rgbColors = 0;
    MyFont.lCustData = 0;
    MyFont.lpfnHook = 0;
    MyFont.lpTemplateName = "";
    MyFont.hInstance = 0;
    MyFont.lpszStyle = "";
    MyFont.nFontType = REGULAR_FONTTYPE;
    MyFont.nSizeMin = 8;
    MyFont.nSizeMax = 72;
    ReturnValue = ChooseFont(&MyFont);
    RtlCopyMemory(&MyLogFont, hMemFont, sizeof(MyLogFont));
    free(hMemFont);
    ReleaseDC(hWnd, FhDC);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Display a browse dialog
CStr ComDlgBrowseForFolder(HWND hWnd, CStr Title)
{
    CStr ReturnValue;
    BROWSEINFO MyBrowse;
    CStr TempPath;
    CStr SelPath;
    ITEMIDLIST *PtrItemLst = 0;

    memset(&MyBrowse,0,sizeof(MyBrowse));
    TempPath = TempPath.Space(MAX_PATH);
    MyBrowse.hwndOwner = hWnd;
    MyBrowse.pidlRoot = 0;
    MyBrowse.lpszTitle = Title.Get_String();
    MyBrowse.pszDisplayName = TempPath.Get_String();
    MyBrowse.lParam = 0;
    if(_OnWindows2K == 1)
    {
        MyBrowse.ulFlags = BIF_RETURNONLYFSDIRS | BIF_SHAREABLE | BIF_USENEWUI;
        CoInitialize(0);
        PtrItemLst = SHBrowseForFolder(&MyBrowse);
        CoUninitialize();
    }
    else
    {
        MyBrowse.ulFlags = BIF_RETURNONLYFSDIRS;
        PtrItemLst = SHBrowseForFolder(&MyBrowse);
    }
    if(PtrItemLst != 0)
    {
        SelPath = SelPath.Space(MAX_PATH);
        SHGetPathFromIDList(PtrItemLst, SelPath.Get_String());
        ReturnValue = SelPath;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Parse a multi files selection
// Return directory of selected files (if any)
CStr ComDlgParseMultiFilesSelection(CStr SelectedFiles, long (CALLBACK *EnumProc)(char *, long), long EnumDirection, long UserValue)
{
    CStr BufString;
    CStr ReturnValue;
    long *SelArray;
    int i;

    if(SelectedFiles.Len() == 0) return(ReturnValue);
    SelArray = StringSplit(SelectedFiles, BufString.Chr(2));
    if(SelArray != 0)
    {
        switch(EnumDirection)
        {
            case MULTIFILESENUM_FORWARD:
                for(i = 0; i <= StringGetSplitUBound(SelArray); i++)
                {
                    BufString = StringGetSplitElement(SelectedFiles, SelArray, i);
                    if(BufString.Len() != 0)
                    {
                        // Pass it as char *
                        if(EnumProc(BufString.Get_String(), UserValue) == 0) break;
                    }
                }
                break;
            case MULTIFILESENUM_BACKWARD:
                for(i = StringGetSplitUBound(SelArray); i >= 0; i--)
                {
                    BufString = StringGetSplitElement(SelectedFiles, SelArray, i);
                    if(BufString.Len() != 0)
                    {
                        // Pass it as char *
                        if(EnumProc(BufString.Get_String(), UserValue) == 0) break;
                    }
                }
                break;
        }
        BufString = FileGetDirectory(StringGetSplitElement(SelectedFiles, SelArray, 0));
        ReturnValue = BufString;
        StringReleaseSplit(SelArray);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Classes
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Register a standard window class
long CALLBACK WASetDialogClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "DialogClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(0, IDC_ARROW);
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a standard splash class
long CALLBACK WASetSplashClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefSplashClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = 0;
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "WASplashDialogClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(0, IDC_ARROW);
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a MDI window class
long CALLBACK WASetMDIClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefMDIClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = (HBRUSH) (COLOR_APPWORKSPACE + 1);
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "WAMDIDialogClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(0, IDC_ARROW);
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a MDIChild window class
long CALLBACK WASetMDIChildClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefMDIChildClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "WAMDIChildDialogClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(0, IDC_ARROW);
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a standard container class
long CALLBACK WASetContainerClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefContainerClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "WAContainerClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(0, IDC_ARROW);
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a standard splitter class
long CALLBACK WASetSplitterClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefSplitterClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "SplitterClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(0, IDC_ARROW);
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a standard vertical splitbar class
long CALLBACK WASetVertSplitBarClass(HINSTANCE hInst, long vCursor)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefVertSplitBarClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "WAVertSplitBarClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(hInst, MAKEINTRESOURCE(vCursor));
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a standard vertical splitbar class
long CALLBACK WASetHorzSplitBarClass(HINSTANCE hInst, long hCursor)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefHorzSplitBarClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "WAHorzSplitBarClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(hInst, MAKEINTRESOURCE(hCursor));
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a standard colorbox class
long CALLBACK WASetColorBoxClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefColorBoxClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = 0;
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "ColorBoxClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(0, IDC_HAND);
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a standard dumpbox class
long CALLBACK WASetDumpBoxClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefDumpBoxClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = 0;
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "DumpBoxClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = LoadCursor(0, IDC_ARROW);
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Register a standard Gripbox square (element) class
long CALLBACK WASetGripBoxClass(HINSTANCE hInst)
{
    memset(&WAMyClass, 0, sizeof(WAMyClass));
    WAMyClass.cbSize = sizeof(WAMyClass);
    WAMyClass.style = CS_BYTEALIGNWINDOW | CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT;
    WAMyClass.lpfnWndProc = &WADefGripBoxClassHook;
    WAMyClass.cbClsExtra = 0;
    WAMyClass.cbWndExtra = 16;
    WAMyClass.hInstance = hInst;
    WAMyClass.hbrBackground = 0;
    WAMyClass.lpszMenuName = "";
    WAMyClass.lpszClassName = "GripBoxClass";
    WAMyClass.hIcon = WALocalhIcon;
    WAMyClass.hCursor = 0;
    WAMyClass.hIconSm = WALocalhIcon;
    return(RegisterClassEx(&WAMyClass));
}

// -----------------------------------------------------------------------
// Hooks functions
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Default window hook
LRESULT CALLBACK WADefWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for standard dialogs
LRESULT CALLBACK WADefClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
            if(WACurrentInitProc != 0) WACurrentInitProc(hWnd);
            return(0);
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for MDI forms
LRESULT CALLBACK WADefMDIClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            return(0);
        case WM_MENUCHAR:
            return(0);
        case WM_SETFOCUS:
            return(0);
        case WM_SIZE:
            return(0);
        case WM_ERASEBKGND:
            return(0);
        case WM_CREATE:
            if(WACurrentInitProc != 0) WACurrentInitProc(hWnd);
            return(0);
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return(0);
    }
    return(DefFrameProc(hWnd, 0, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook MDI childs
LRESULT CALLBACK WADefMDIChildClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CREATE:
            if(WACurrentChildInitProc != 0) WACurrentChildInitProc(hWnd);
            return(0);
        case WM_ERASEBKGND:
            return(0);
    }
    return(DefMDIChildProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for containers
LRESULT CALLBACK WADefContainerClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ContPs;

    switch(uMsg)
    {
        case WM_CREATE:
            return(0);
        case WM_SIZE:
            return(0);
        case WM_PAINT:
            BeginPaint(hWnd, &ContPs);
            EndPaint(hWnd, &ContPs);
            return(0);
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for splitters
LRESULT CALLBACK WADefSplitterClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hChild = 0;
    SPLITTERDAT TmpSplitterDat;
    PAINTSTRUCT SplitPs;

    switch(uMsg)
    {
        case WM_CREATE:
            // Create a splitbar and store the new handle
            CopyMemory(&TmpSplitterDat, &WACurrentSplitterStruct, sizeof(TmpSplitterDat));
            if(TmpSplitterDat.Orientation == SPLITTER_HORZ) SetWindowLong(hWnd, GWL_USERDATA, (long) CreateSplitBar(0, 0, -1, TmpSplitterDat.SplitBarSize, hWnd, 0, WACurrentSplitterStruct));
            else SetWindowLong(hWnd, GWL_USERDATA, (long) CreateSplitBar(ControlWidth(hWnd) - TmpSplitterDat.SplitBarSize, -1, TmpSplitterDat.SplitBarSize, -1, hWnd, 0, WACurrentSplitterStruct));
            return(0);
        case WM_SIZE:
            hChild = (HWND) GetWindowLong(hWnd, GWL_USERDATA);
            CopyMemory(&TmpSplitterDat, (void *) GetWindowLong(hChild, GWL_USERDATA), sizeof(TmpSplitterDat));
            if(TmpSplitterDat.Orientation == SPLITTER_HORZ) MoveWindow(hChild, 0, 0, ControlWidth(hWnd), TmpSplitterDat.SplitBarSize + 1, 1);
            else MoveWindow(hChild, ControlWidth(hWnd) - TmpSplitterDat.SplitBarSize, 0, TmpSplitterDat.SplitBarSize, ControlHeight(hWnd), 1);
            return(0);
        case WM_PAINT:
            BeginPaint(hWnd, &SplitPs);
            hChild = (HWND) GetWindowLong(hWnd, GWL_USERDATA);
            CopyMemory(&TmpSplitterDat, (void *) GetWindowLong(hChild, GWL_USERDATA), sizeof(TmpSplitterDat));
            if(TmpSplitterDat.Orientation == SPLITTER_HORZ) MoveWindow(hChild, 0, 0, ControlWidth(hWnd), TmpSplitterDat.SplitBarSize + 1, 1);
            else MoveWindow(hChild, ControlWidth(hWnd) - TmpSplitterDat.SplitBarSize, 0, TmpSplitterDat.SplitBarSize, ControlHeight(hWnd), 1);
            EndPaint(hWnd, &SplitPs);
            return(0);
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for vertical splitbars
LRESULT CALLBACK WADefVertSplitBarClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hScreenDC = 0;
    SPLITTERDAT TmpSplitBarDat;
    RECT MaxRect;
    RECT PosRect;
    POINT CursScrPos;
    long ParentLeft = 0;
    long ParentWidth = 0;
    long RealLeft = 0;

    switch(uMsg)
    {
        case WM_LBUTTONDOWN:
            RtlCopyMemory(&TmpSplitBarDat, (void *) GetWindowLong(hWnd, GWL_USERDATA), sizeof(TmpSplitBarDat));
            TmpSplitBarDat.Sizing = SPLITTER_RESIZING;
            TmpSplitBarDat.OldXPos = -1;
            RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &TmpSplitBarDat, sizeof(TmpSplitBarDat));
            hScreenDC = GetDC(0);
            GetWindowRect(TmpSplitBarDat.hClient, &MaxRect);
            GetCursorPos(&CursScrPos);
            ParentLeft = ControlLeft(TmpSplitBarDat.hParent);
            ParentWidth = ControlWidth(TmpSplitBarDat.hParent);
            RealLeft = ControlLeft(hWnd);
            if(CursScrPos.x < (ParentLeft + (TmpSplitBarDat.MinLeftTop + 5))) CursScrPos.x = (ParentLeft + (TmpSplitBarDat.MinLeftTop + 5));
            if(CursScrPos.x > ((ParentLeft + ParentWidth) - (TmpSplitBarDat.MinLeftTop + 8))) CursScrPos.x = (ParentLeft + ParentWidth) - (TmpSplitBarDat.MinLeftTop + 8);
            if(TmpSplitBarDat.OldXPos != CursScrPos.x)
            {
                if(TmpSplitBarDat.OldXPos != -1)
                {
                    PosRect.left = (TmpSplitBarDat.OldXPos);
                    PosRect.right = (TmpSplitBarDat.OldXPos) + 4;
                    PosRect.top = MaxRect.top;
                    PosRect.bottom = MaxRect.bottom;
                    DrawFocusRect(hScreenDC, &PosRect);
                    PosRect.left = PosRect.left + 1;
                    PosRect.right = PosRect.right - 1;
                    DrawFocusRect(hScreenDC, &PosRect);
                }
                // New bar
                PosRect.left = RealLeft;
                PosRect.right = RealLeft + 4;
                PosRect.top = MaxRect.top;
                PosRect.bottom = MaxRect.bottom;
                DrawFocusRect(hScreenDC, &PosRect);
                PosRect.left = PosRect.left + 1;
                PosRect.right = PosRect.right - 1;
                DrawFocusRect(hScreenDC, &PosRect);
                TmpSplitBarDat.OldXPos = RealLeft;
                TmpSplitBarDat.DisplacementX = CursScrPos.x - RealLeft;
                TmpSplitBarDat.StartDragX = CursScrPos.x;
                RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &TmpSplitBarDat, sizeof(TmpSplitBarDat));
            }
            SetCapture(hWnd);
            return(0);
        case WM_MOUSEMOVE:
            RtlCopyMemory(&TmpSplitBarDat, (void *) GetWindowLong(hWnd, GWL_USERDATA), sizeof(TmpSplitBarDat));
            if(TmpSplitBarDat.Sizing == SPLITTER_RESIZING)
            {
                // Get screen DC
                hScreenDC = GetDC(0);
                GetWindowRect(TmpSplitBarDat.hClient, &MaxRect);
                GetCursorPos(&CursScrPos);
                ParentLeft = ControlLeft(TmpSplitBarDat.hParent);
                ParentWidth = ControlWidth(TmpSplitBarDat.hParent);
                if(CursScrPos.x < (ParentLeft + (TmpSplitBarDat.MinLeftTop + 5))) CursScrPos.x = (ParentLeft + (TmpSplitBarDat.MinLeftTop + 5));
                if(CursScrPos.x > ((ParentLeft + ParentWidth) - (TmpSplitBarDat.MinLeftTop + 8))) CursScrPos.x = (ParentLeft + ParentWidth) - (TmpSplitBarDat.MinLeftTop + 8);
                if(TmpSplitBarDat.OldXPos != CursScrPos.x)
                {
                    if(TmpSplitBarDat.OldXPos != -1)
                    {
                        PosRect.left = (TmpSplitBarDat.OldXPos);
                        PosRect.right = (TmpSplitBarDat.OldXPos) + 4;
                        PosRect.top = MaxRect.top;
                        PosRect.bottom = MaxRect.bottom;
                        DrawFocusRect(hScreenDC, &PosRect);
                        PosRect.left = PosRect.left + 1;
                        PosRect.right = PosRect.right - 1;
                        DrawFocusRect(hScreenDC, &PosRect);
                    }
                    // New bar
                    PosRect.left = (CursScrPos.x - TmpSplitBarDat.DisplacementX);
                    PosRect.right = (CursScrPos.x - TmpSplitBarDat.DisplacementX) + 4;
                    PosRect.top = MaxRect.top;
                    PosRect.bottom = MaxRect.bottom;
                    DrawFocusRect(hScreenDC, &PosRect);
                    PosRect.left = PosRect.left + 1;
                    PosRect.right = PosRect.right - 1;
                    DrawFocusRect(hScreenDC, &PosRect);
                    TmpSplitBarDat.OldXPos = CursScrPos.x - TmpSplitBarDat.DisplacementX;
                    RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &TmpSplitBarDat, sizeof(TmpSplitBarDat));
                }
            }
            return(0);
        case WM_LBUTTONUP:
            RtlCopyMemory(&TmpSplitBarDat, (void *) GetWindowLong(hWnd, GWL_USERDATA), sizeof(TmpSplitBarDat));
            if(TmpSplitBarDat.Sizing == SPLITTER_RESIZING)
            {
                GetCursorPos(&CursScrPos);
                GetWindowRect(TmpSplitBarDat.hClient, &MaxRect);
                hScreenDC = GetDC(0);
                if(TmpSplitBarDat.OldXPos != -1)
                {
                    PosRect.left = (TmpSplitBarDat.OldXPos);
                    PosRect.right = (TmpSplitBarDat.OldXPos) + 4;
                    PosRect.top = MaxRect.top;
                    PosRect.bottom = MaxRect.bottom;
                    DrawFocusRect(hScreenDC, &PosRect);
                    PosRect.left = PosRect.left + 1;
                    PosRect.right = PosRect.right - 1;
                    DrawFocusRect(hScreenDC, &PosRect);
                    TmpSplitBarDat.OldXPos = TmpSplitBarDat.OldXPos - ControlLeft(TmpSplitBarDat.hParent);
                }
                TmpSplitBarDat.Sizing = SPLITTER_NORESIZING;
                RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &TmpSplitBarDat, sizeof(TmpSplitBarDat));
                ReleaseCapture();
                // Notify Resize
                if(TmpSplitBarDat.OldXPos != -1) if(CursScrPos.x != TmpSplitBarDat.StartDragX) SendMessage(TmpSplitBarDat.hParent, MSG_SPLITTER_RESIZED, (UINT) GetParent(hWnd), TmpSplitBarDat.OldXPos);
                return(0);
            }
            ReleaseCapture();
            return(0);
        case WM_CREATE:
            return(0);
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for horizontal splitbars
LRESULT CALLBACK WADefHorzSplitBarClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HDC hScreenDC = 0;
    SPLITTERDAT TmpSplitBarDat;
    RECT MaxRect;
    RECT PosRect;
    POINT CursScrPos;
    long ParentLeft = 0;
    long ParentTop = 0;
    long ParentHeight = 0;
    long ParentWidth = 0;
    long RealTop = 0;

    switch(uMsg)
    {
        case WM_LBUTTONDOWN:
            RtlCopyMemory(&TmpSplitBarDat, (void *) GetWindowLong(hWnd, GWL_USERDATA), sizeof(TmpSplitBarDat));
            TmpSplitBarDat.Sizing = SPLITTER_RESIZING;
            // Mark new resize
            TmpSplitBarDat.OldYPos = -1;
            RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &TmpSplitBarDat, sizeof(TmpSplitBarDat));
            hScreenDC = GetDC(0);
            GetWindowRect(TmpSplitBarDat.hClient, &MaxRect);
            GetCursorPos(&CursScrPos);
            ParentLeft = ControlLeft(TmpSplitBarDat.hParent);
            ParentTop = ControlTop(TmpSplitBarDat.hParent);
            ParentHeight = ControlHeight(TmpSplitBarDat.hParent);
            ParentWidth = ControlWidth(TmpSplitBarDat.hParent);
            RealTop = ControlTop(hWnd);
            if(CursScrPos.y < (MaxRect.top + (TmpSplitBarDat.MaxRightBottom + 2))) CursScrPos.y = (MaxRect.top + (TmpSplitBarDat.MaxRightBottom + 2));
            if(CursScrPos.y > ((MaxRect.bottom + ControlHeight(GetParent(hWnd))) - (TmpSplitBarDat.MaxRightBottom + 5))) CursScrPos.y = (MaxRect.bottom + ControlHeight(GetParent(hWnd))) - (TmpSplitBarDat.MaxRightBottom + 5);
            if(TmpSplitBarDat.OldYPos != CursScrPos.y)
            {
                if(TmpSplitBarDat.OldYPos != -1)
                {
                    PosRect.left = ParentLeft + 2;
                    PosRect.right = ParentWidth + ParentLeft - 2;
                    PosRect.top = TmpSplitBarDat.OldYPos;
                    PosRect.bottom = TmpSplitBarDat.OldYPos + 4;
                    DrawFocusRect(hScreenDC, &PosRect);
                    PosRect.top = PosRect.top + 1;
                    PosRect.bottom = PosRect.bottom - 1;
                    DrawFocusRect(hScreenDC, &PosRect);
                }
                // New bar
                PosRect.left = ParentLeft + 2;
                PosRect.right = ParentWidth + ParentLeft - 2;
                PosRect.bottom = RealTop + 4;
                PosRect.top = RealTop;
                DrawFocusRect(hScreenDC, &PosRect);
                PosRect.top = PosRect.top + 1;
                PosRect.bottom = PosRect.bottom - 1;
                DrawFocusRect(hScreenDC, &PosRect);
                TmpSplitBarDat.OldYPos = RealTop;
                TmpSplitBarDat.DisplacementY = CursScrPos.y - RealTop;
                TmpSplitBarDat.StartDragY = CursScrPos.y;
                RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &TmpSplitBarDat, sizeof(TmpSplitBarDat));
            }
            SetCapture(hWnd);
            return(0);
        case WM_MOUSEMOVE:
            RtlCopyMemory(&TmpSplitBarDat, (void *) GetWindowLong(hWnd, GWL_USERDATA), sizeof(TmpSplitBarDat));
            if(TmpSplitBarDat.Sizing == SPLITTER_RESIZING)
            {
                hScreenDC = GetDC(0);
                GetWindowRect(TmpSplitBarDat.hClient, &MaxRect);
                GetCursorPos(&CursScrPos);
                ParentLeft = ControlLeft(TmpSplitBarDat.hParent);
                ParentTop = ControlTop(TmpSplitBarDat.hParent);
                ParentHeight = ControlHeight(TmpSplitBarDat.hParent);
                ParentWidth = ControlWidth(TmpSplitBarDat.hParent);
                if(CursScrPos.y < (MaxRect.top + (TmpSplitBarDat.MaxRightBottom + 2))) CursScrPos.y = (MaxRect.top + (TmpSplitBarDat.MaxRightBottom + 2));
                if(CursScrPos.y > ((MaxRect.bottom + ControlHeight(GetParent(hWnd))) - (TmpSplitBarDat.MaxRightBottom + 5))) CursScrPos.y = (MaxRect.bottom + ControlHeight(GetParent(hWnd))) - (TmpSplitBarDat.MaxRightBottom + 5);
                if(TmpSplitBarDat.OldYPos != CursScrPos.y)
                {
                    if(TmpSplitBarDat.OldYPos != -1)
                    {
                        PosRect.left = ParentLeft + 2;
                        PosRect.right = ParentWidth + ParentLeft - 2;
                        PosRect.top = TmpSplitBarDat.OldYPos;
                        PosRect.bottom = TmpSplitBarDat.OldYPos + 4;
                        DrawFocusRect(hScreenDC, &PosRect);
                        PosRect.top = PosRect.top + 1;
                        PosRect.bottom = PosRect.bottom - 1;
                        DrawFocusRect(hScreenDC, &PosRect);
                    }
                    // New bar
                    PosRect.left = ParentLeft + 2;
                    PosRect.right = ParentWidth + ParentLeft - 2;
                    PosRect.bottom = CursScrPos.y - TmpSplitBarDat.DisplacementY + 4;
                    PosRect.top = CursScrPos.y - TmpSplitBarDat.DisplacementY;
                    DrawFocusRect(hScreenDC, &PosRect);
                    PosRect.top = PosRect.top + 1;
                    PosRect.bottom = PosRect.bottom - 1;
                    DrawFocusRect(hScreenDC, &PosRect);
                    TmpSplitBarDat.OldYPos = CursScrPos.y - TmpSplitBarDat.DisplacementY;
                    RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &TmpSplitBarDat, sizeof(TmpSplitBarDat));
                }
            }
            return(0);
        case WM_LBUTTONUP:
            RtlCopyMemory(&TmpSplitBarDat, (void *) GetWindowLong(hWnd, GWL_USERDATA), sizeof(TmpSplitBarDat));
            if(TmpSplitBarDat.Sizing == SPLITTER_RESIZING)
            {
                hScreenDC = GetDC(0);
                GetCursorPos(&CursScrPos);
                GetWindowRect(TmpSplitBarDat.hClient, &MaxRect);
                if(TmpSplitBarDat.OldYPos != -1)
                {
                    ParentLeft = ControlLeft(TmpSplitBarDat.hParent);
                    ParentWidth = ControlWidth(TmpSplitBarDat.hParent);
                    PosRect.left = ParentLeft + 2;
                    PosRect.right = ParentWidth + ParentLeft - 2;
                    PosRect.top = TmpSplitBarDat.OldYPos;
                    PosRect.bottom = TmpSplitBarDat.OldYPos + 4;
                    DrawFocusRect(hScreenDC, &PosRect);
                    PosRect.top = PosRect.top + 1;
                    PosRect.bottom = PosRect.bottom - 1;
                    DrawFocusRect(hScreenDC, &PosRect);
                    TmpSplitBarDat.OldYPos = TmpSplitBarDat.OldYPos - ControlTop(TmpSplitBarDat.hClient) + TmpSplitBarDat.DisplacementY;
                }
                TmpSplitBarDat.Sizing = SPLITTER_NORESIZING;
                RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &TmpSplitBarDat, sizeof(TmpSplitBarDat));
                ReleaseCapture();
                // Notify Resize
                if(TmpSplitBarDat.OldYPos != -1) if(CursScrPos.y != TmpSplitBarDat.StartDragY) SendMessage(TmpSplitBarDat.hParent, MSG_SPLITTER_RESIZED,(UINT) GetParent(hWnd), TmpSplitBarDat.OldYPos);
                return(0);
            }
            ReleaseCapture();
            return(0);
        case WM_CREATE:
            return(0);
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for colorbox controls
LRESULT CALLBACK WADefColorBoxClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT PaintRect;
    LOGBRUSH BlackBrush;
    HBRUSH hBlackBrush;
    COLORREF Hicolor = 0;
    COLORREF HicolorR = 0;
    COLORREF HicolorG = 0;
    COLORREF HicolorB = 0;
    COLORREF LoColor = 0;
    COLORREF LoColorR = 0;
    COLORREF LoColorG = 0;
    COLORREF LoColorB = 0;

    switch(uMsg)
    {
        case WM_LBUTTONDOWN:
            SendMessage(GetParent(hWnd), MSG_COLORBOX_CLICKED, (UINT) hWnd, (long) 0);
            return(0);
        case WM_ERASEBKGND:
            GetClientRect(hWnd, &PaintRect);
            BlackBrush.lbStyle = BS_SOLID;
            BlackBrush.lbHatch = 0;
            BlackBrush.lbColor = GetWindowLong(hWnd, GWL_USERDATA);
            hBlackBrush = CreateBrushIndirect(&BlackBrush);
            FillRect((HDC) wParam, &PaintRect, hBlackBrush);
            Hicolor = BlackBrush.lbColor;
            HicolorR = (Hicolor & 0xFF0000) >> 16;
            HicolorG = (Hicolor & 0xFF00) >> 8;
            HicolorB = (Hicolor & 0xFF);
            LoColorR = HicolorR - 64;
            LoColorG = HicolorG - 64;
            LoColorB = HicolorB - 64;
            HicolorR = HicolorR + 64;
            HicolorG = HicolorG + 64;
            HicolorB = HicolorB + 64;
            if((long) HicolorR > (long) 0xFF) HicolorR = 0xFF;
            if((long) HicolorG > (long) 0xFF) HicolorG = 0xFF;
            if((long) HicolorB > (long) 0xFF) HicolorB = 0xFF;
            if((long) LoColorR < (long) 0) LoColorR = 0;
            if((long) LoColorG < (long) 0) LoColorG = 0;
            if((long) LoColorB < (long) 0) LoColorB = 0;
            Hicolor = (HicolorR << 16) | (HicolorG << 8) | HicolorB;
            LoColor = (LoColorR << 16) | (LoColorG << 8) | LoColorB;
            GDIDrawLine(hWnd, 0, 0, 0, (PaintRect.bottom - PaintRect.top) - 1, Hicolor);
            GDIDrawLine(hWnd, 0, 0, (PaintRect.right - PaintRect.left) - 1, 0, Hicolor);
            GDIDrawLine(hWnd, 1, (PaintRect.bottom - PaintRect.top) - 1, (PaintRect.right - PaintRect.left), (PaintRect.bottom - PaintRect.top) - 1, LoColor);
            GDIDrawLine(hWnd, (PaintRect.right - PaintRect.left) - 1, 1, (PaintRect.right - PaintRect.left) - 1, (PaintRect.bottom - PaintRect.top) - 1, LoColor);
            DeleteObject(hBlackBrush);
            return(1);
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for dumpbox controls
LRESULT CALLBACK WADefDumpBoxClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long ReturnScrollValue = 0;

    switch(uMsg)
    {
        case WM_CLOSE:
            DestroyWindow(hWnd);
            return(0);
        case WM_HSCROLL:
            switch(wParam & 0xFFFF)
            {
                case SB_LINELEFT:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_LINELEFT, GetScrollPos(hWnd, SB_HORZ), (long) 1);
                    SetScrollPos(hWnd, SB_HORZ, ReturnScrollValue, 1);
                    break;
                case SB_LINERIGHT:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_LINERIGHT, GetScrollPos(hWnd, SB_HORZ), (long) -1);
                    SetScrollPos(hWnd, SB_HORZ, ReturnScrollValue, 1);
                    break;
                case SB_PAGELEFT:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_PAGELEFT, GetScrollPos(hWnd, SB_HORZ), ControlClientWidth(hWnd));
                    SetScrollPos(hWnd, SB_HORZ, ReturnScrollValue, 1);
                    break;
                case SB_PAGERIGHT:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_PAGERIGHT, GetScrollPos(hWnd, SB_HORZ), ControlClientWidth(hWnd));
                    SetScrollPos(hWnd, SB_HORZ, ReturnScrollValue, 1);
                    break;
                case SB_THUMBTRACK:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_TRACKLEFTRIGHT, GetScrollPos(hWnd, SB_HORZ), (short) ((wParam & 0xFFFF0000) / 0x10000));
                    SetScrollPos(hWnd, SB_HORZ, ReturnScrollValue, 1);
                    break;
                case SB_THUMBPOSITION:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_POSITIONLEFTRIGHT, GetScrollPos(hWnd, SB_HORZ), (short) ((wParam & 0xFFFF0000) / 0x10000));
                    SetScrollPos(hWnd, SB_HORZ, ReturnScrollValue, 1);
                    break;
                case SB_TOP:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_LEFT, GetScrollPos(hWnd, SB_HORZ), 0);
                    SetScrollPos(hWnd, SB_HORZ, ReturnScrollValue, 1);
                    break;
                case SB_BOTTOM:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_RIGHT, GetScrollPos(hWnd, SB_HORZ), 0);
                    SetScrollPos(hWnd, SB_HORZ, ReturnScrollValue, 1);
                    break;
            }
            return(0);
        case WM_VSCROLL:
            switch(wParam & 0xFFFF)
            {
                case SB_LINEUP:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_LINEUP, GetScrollPos(hWnd, SB_VERT), 1);
                    SetScrollPos(hWnd, SB_VERT, ReturnScrollValue, 1);
                    break;
                case SB_LINEDOWN:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_LINEDOWN, GetScrollPos(hWnd, SB_VERT), -1);
                    SetScrollPos(hWnd, SB_VERT, ReturnScrollValue, 1);
                    break;
                case SB_PAGEUP:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_PAGEUP, GetScrollPos(hWnd, SB_VERT), ControlClientHeight(hWnd));
                    SetScrollPos(hWnd, SB_VERT, ReturnScrollValue, 1);
                    break;
                case SB_PAGEDOWN:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_PAGEDOWN, GetScrollPos(hWnd, SB_VERT), ControlClientHeight(hWnd));
                    SetScrollPos(hWnd, SB_VERT, ReturnScrollValue, 1);
                    break;
                case SB_THUMBTRACK:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_TRACKUPDOWN, GetScrollPos(hWnd, SB_VERT), (short) ((wParam & 0xFFFF0000) / 0x10000));
                    SetScrollPos(hWnd, SB_VERT, ReturnScrollValue, 1);
                    break;
                case SB_THUMBPOSITION:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_POSITIONUPDOWN, GetScrollPos(hWnd, SB_VERT), (short) ((wParam & 0xFFFF0000) / 0x10000));
                    SetScrollPos(hWnd, SB_VERT, ReturnScrollValue, 1);
                    break;
                case SB_TOP:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_TOP, GetScrollPos(hWnd, SB_VERT), 0);
                    SetScrollPos(hWnd, SB_VERT, ReturnScrollValue, 1);
                    break;
                case SB_BOTTOM:
                    ReturnScrollValue = SendMessage(GetParent(hWnd), DUMPBOX_BOTTOM, GetScrollPos(hWnd, SB_VERT), 0);
                    SetScrollPos(hWnd, SB_VERT, ReturnScrollValue, 1);
                    break;
            }
            return(0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for Gripbox controls
LRESULT CALLBACK WADefGripBoxClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LOGBRUSH BlackBrush;
    HBRUSH hBlackBrush = 0;
    HBRUSH hWhiteBrush = 0;
    PAINTSTRUCT SplitPs;
    LPGRIPBOXDAT GripDats;
    LPGRIPBOXSQRBOXDAT GripSquareDats;
    RECT PaintRect;
    RECT ParentRect;
    HWND ParentParent;
    HDC Size_HDC;
    long DragX;
    long DragY;
    POINT DragPos;

    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &SplitPs);
            BlackBrush.lbStyle = BS_SOLID;
            BlackBrush.lbHatch = 0;
            GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(hWnd, GWL_USERDATA);
            if(GripSquareDats != 0) {
                BlackBrush.lbColor = GripSquareDats->Color;
                hBlackBrush = CreateBrushIndirect(&BlackBrush);
                BlackBrush.lbColor = 0xffffff;
                hWhiteBrush = CreateBrushIndirect(&BlackBrush);
                // Fill it
                GetClientRect(hWnd, &PaintRect);
                FillRect(SplitPs.hdc, &PaintRect, hWhiteBrush);
                PaintRect.left++;
                PaintRect.top++;
                PaintRect.right--;
                PaintRect.bottom--;
                FillRect(SplitPs.hdc, &PaintRect, hBlackBrush);
                DeleteObject(hWhiteBrush);
                DeleteObject(hBlackBrush);
            }
            EndPaint(hWnd, &SplitPs);
            return(0);
        case WM_LBUTTONDOWN:
            SetCapture(hWnd);
            GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(hWnd, GWL_USERDATA);
            // Ask parent to fill the rect structure
            GripSquareDats->State = GRIPBOX_STATE_RESIZING;
            ParentParent = GetParent(hWnd);
            SendMessage(ParentParent, GRIPBOX_MSG_ASKSIZE, (WPARAM) GripSquareDats->Container, (LPARAM) &GripSquareDats->ResizeRect);
            SendMessage(ParentParent, GRIPBOX_MSG_ASKSTEPS, (WPARAM) 0, (LPARAM) GripSquareDats);

            Size_HDC = GetDC(ParentParent);
            DrawFocusRect(Size_HDC, &GripSquareDats->ResizeRect);
            ReleaseDC(ParentParent, Size_HDC);

            GripSquareDats->OldRect.left = GripSquareDats->ResizeRect.left;
            GripSquareDats->OldRect.right = GripSquareDats->ResizeRect.right;
            GripSquareDats->OldRect.top = GripSquareDats->ResizeRect.top;
            GripSquareDats->OldRect.bottom = GripSquareDats->ResizeRect.bottom;
            return(0);
        case WM_MOUSEMOVE:
            GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(hWnd, GWL_USERDATA);
            if(GripSquareDats->State == GRIPBOX_STATE_RESIZING)
            {
                ParentParent = GetParent(hWnd);
                GetCursorPos(&DragPos);
                ScreenToClient(ParentParent, &DragPos);
                DragX = DragPos.x;
                DragY = DragPos.y;
                // Bound coordinates
                GetClientRect(ParentParent, &ParentRect);
                if(DragX < 0) DragX = 0;
                if(DragY < 0) DragY = 0;
                if(DragX > ParentRect.right) DragX = ParentRect.right;
                if(DragY > ParentRect.bottom) DragY = ParentRect.bottom;
                if(GripSquareDats->StepX > 1) DragX = ((DragX / GripSquareDats->StepX) * GripSquareDats->StepX) + 1;
                if(GripSquareDats->StepY > 1) DragY = ((DragY / GripSquareDats->StepY) * GripSquareDats->StepY) + 1;
                switch(GripSquareDats->Type)
                {
                    // Free moves
                    case GRIPBOX_TOPLEFT:
                        GripSquareDats->ResizeRect.left = DragX - 1;
                        GripSquareDats->ResizeRect.top = DragY - 1;
                        break;
                    case GRIPBOX_TOPRIGHT:
                        GripSquareDats->ResizeRect.right = DragX;
                        GripSquareDats->ResizeRect.top = DragY - 1;
                        break;
                    case GRIPBOX_BOTTOMLEFT:
                        GripSquareDats->ResizeRect.left = DragX - 1;
                        GripSquareDats->ResizeRect.bottom = DragY;
                        break;
                    case GRIPBOX_BOTTOMRIGHT:
                        GripSquareDats->ResizeRect.right = DragX;
                        GripSquareDats->ResizeRect.bottom = DragY;
                        break;
                    // Up / Down
                    case GRIPBOX_TOP:
                        GripSquareDats->ResizeRect.top = DragY - 1;
                        break;
                    case GRIPBOX_BOTTOM:
                        GripSquareDats->ResizeRect.bottom = DragY;
                        break;
                    // Left / Right
                    case GRIPBOX_LEFT:
                        GripSquareDats->ResizeRect.left = DragX - 1;
                        break;
                    case GRIPBOX_RIGHT:
                        GripSquareDats->ResizeRect.right = DragX;
                        break;
                }

                if((GripSquareDats->ResizeRect.right - GripSquareDats->ResizeRect.left) <= 0) GripSquareDats->ResizeRect.right--;
                if((GripSquareDats->ResizeRect.bottom - GripSquareDats->ResizeRect.top) <= 0) GripSquareDats->ResizeRect.bottom--;

                if(GripSquareDats->ResizeRect.right < (GripSquareDats->ResizeRect.left + GripSquareDats->StepX))
                {
                    GripSquareDats->ResizeRect.right = (GripSquareDats->ResizeRect.left + GripSquareDats->StepX) + 1;
                }
                if(GripSquareDats->ResizeRect.bottom < (GripSquareDats->ResizeRect.top + GripSquareDats->StepY))
                {
                    GripSquareDats->ResizeRect.bottom = (GripSquareDats->ResizeRect.top + GripSquareDats->StepY) + 1; 
                }

                Size_HDC = GetDC(ParentParent);
                DrawFocusRect(Size_HDC, &GripSquareDats->OldRect);
                ReleaseDC(ParentParent, Size_HDC);

                Size_HDC = GetDC(ParentParent);
                DrawFocusRect(Size_HDC, &GripSquareDats->ResizeRect);
                ReleaseDC(ParentParent, Size_HDC);

                GripSquareDats->OldRect.left = GripSquareDats->ResizeRect.left;
                GripSquareDats->OldRect.right = GripSquareDats->ResizeRect.right;
                GripSquareDats->OldRect.top = GripSquareDats->ResizeRect.top;
                GripSquareDats->OldRect.bottom = GripSquareDats->ResizeRect.bottom;
            }
            return(0);
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
            // Cancel resizing
            GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(hWnd, GWL_USERDATA);
            if(GripSquareDats->State == GRIPBOX_STATE_RESIZING)
            {
                ParentParent = GetParent(hWnd);
                Size_HDC = GetDC(ParentParent);
                DrawFocusRect(Size_HDC, &GripSquareDats->ResizeRect);
                ReleaseDC(ParentParent, Size_HDC);
                GripSquareDats->State = GRIPBOX_STATE_RESET;
                ReleaseCapture();
            }
            return(0);
        case WM_LBUTTONUP:
            // Resizing done
            GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(hWnd, GWL_USERDATA);
            if(GripSquareDats->State == GRIPBOX_STATE_RESIZING)
            {
                ParentParent = GetParent(hWnd);
                Size_HDC = GetDC(ParentParent);
                DrawFocusRect(Size_HDC, &GripSquareDats->ResizeRect);
                ReleaseDC(ParentParent, Size_HDC);
                GripSquareDats->State = GRIPBOX_STATE_RESET;
                // Notify parent that the control was correctly resized
                SendMessage(ParentParent, GRIPBOX_MSG_RESIZED, (WPARAM) GripSquareDats->Container, (LPARAM) &GripSquareDats->ResizeRect);
                ReleaseCapture();
            }
            return(0);
        case WM_SETCURSOR:
            GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(hWnd, GWL_USERDATA);
            switch(GripSquareDats->Type)
            {
                case GRIPBOX_TOPLEFT:
                    SetCursor(LoadCursor(0, IDC_SIZENWSE));
                    break;
                case GRIPBOX_TOP:
                    SetCursor(LoadCursor(0, IDC_SIZENS));
                    break;
                case GRIPBOX_TOPRIGHT:
                    SetCursor(LoadCursor(0, IDC_SIZENESW));
                    break;
                case GRIPBOX_BOTTOMLEFT:
                    SetCursor(LoadCursor(0, IDC_SIZENESW));
                    break;
                case GRIPBOX_BOTTOM:
                    SetCursor(LoadCursor(0, IDC_SIZENS));
                    break;
                case GRIPBOX_BOTTOMRIGHT:
                    SetCursor(LoadCursor(0, IDC_SIZENWSE));
                    break;
                case GRIPBOX_LEFT:
                    SetCursor(LoadCursor(0, IDC_SIZEWE));
                    break;
                case GRIPBOX_RIGHT:
                    SetCursor(LoadCursor(0, IDC_SIZEWE));
                    break;
            }
            return(1);
        case WM_CLOSE:
            // First square closed erases all references to the ocntainer structure
            // So that it is freed only one time
            GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(hWnd, GWL_USERDATA);
            if(GripSquareDats->Container)
            {
                // Get the container from the current square
                GripDats = GripSquareDats->Container;
                GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->BottomBox, GWL_USERDATA);
                GripSquareDats->Container = 0;
                GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->BottomLeftBox, GWL_USERDATA);
                GripSquareDats->Container = 0;
                GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->BottomRightBox, GWL_USERDATA);
                GripSquareDats->Container = 0;
                GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->LeftBox, GWL_USERDATA);
                GripSquareDats->Container = 0;
                GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->RightBox, GWL_USERDATA);
                GripSquareDats->Container = 0;
                GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->TopBox, GWL_USERDATA);
                GripSquareDats->Container = 0;
                GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->TopLeftBox, GWL_USERDATA);
                GripSquareDats->Container = 0;
                GripSquareDats = (LPGRIPBOXSQRBOXDAT) GetWindowLong(GripDats->TopRightBox, GWL_USERDATA);
                GripSquareDats->Container = 0;
            }
            // Free the structure of the square
            FreeMem(GetWindowLong(hWnd, GWL_USERDATA));
            SetWindowLong(hWnd, GWL_USERDATA, 0);
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default class hook for splash dialog
LRESULT CALLBACK WADefSplashClassHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    RECT SplashRect;
    LOGBRUSH SplashBrush;
    HBRUSH hSplashBrush = 0;

    switch(uMsg)
    {
        case WM_CREATE:
            if(WACurrentInitProc != 0) WACurrentInitProc(hWnd);
            return(0);
        case WM_ERASEBKGND:
            GetClientRect(hWnd, &SplashRect);
            SplashBrush.lbStyle = BS_SOLID;
            SplashBrush.lbHatch = 0;
            SplashBrush.lbColor = SendMessage(hWnd, MSG_SPLASH_PAINT, 0, 0);
            hSplashBrush = CreateBrushIndirect(&SplashBrush);
            FillRect((HDC) wParam, &SplashRect, hSplashBrush);
            DeleteObject(hSplashBrush);
            return(1);
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Default window hook
LRESULT CALLBACK WAHexHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CStr CharPress;

    switch(uMsg)
    {
        case WM_CONTEXTMENU:
            return(0);
        case WM_MENUSELECT:
            return(0);
        case WM_RBUTTONDOWN:
            return(0);
        case WM_CHAR:
            if((wParam != 8) && (wParam != 13))
            {
                CharPress = CharPress.Chr(wParam);
                CharPress = CharPress.Upper_Case();
                if(CharPress == "0") goto HexOk;
                if(CharPress == "1") goto HexOk;
                if(CharPress == "2") goto HexOk;
                if(CharPress == "3") goto HexOk;
                if(CharPress == "4") goto HexOk;
                if(CharPress == "5") goto HexOk;
                if(CharPress == "6") goto HexOk;
                if(CharPress == "7") goto HexOk;
                if(CharPress == "8") goto HexOk;
                if(CharPress == "9") goto HexOk;
                if(_strcmpi(CharPress.Get_String(), "A") == 0)
                {
                    wParam = 0x41;
                    goto HexOk;
                }
                if(_strcmpi(CharPress.Get_String(), "B") == 0)
                {
                    wParam = 0x42;
                    goto HexOk;
                }
                if(_strcmpi(CharPress.Get_String(), "C") == 0)
                {
                    wParam = 0x43;
                    goto HexOk;
                }
                if(_strcmpi(CharPress.Get_String(), "D") == 0)
                {
                    wParam = 0x44;
                    goto HexOk;
                }
                if(_strcmpi(CharPress.Get_String(), "E") == 0)
                {
                    wParam = 0x45;
                    goto HexOk;
                }
                if(_strcmpi(CharPress.Get_String(), "F") == 0)
                {
                    wParam = 0x46;
                    goto HexOk;
                }
                return(0);
            }
            break;
        case WM_CUT:
            return(0);
        case WM_PASTE:
            return(0);
    }
HexOk:
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Initialize Stock Form (Empty model)
int CALLBACK FRMStockModalDlgEmptyProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long ReturnValue = 0;
    PAINTSTRUCT Ps;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, DialogTempTitle);
            LastDialog = hwndDlg;
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &Ps);
            if(DialogTempProc != 0) ReturnValue = DialogTempProc(hwndDlg, uMsg, wParam, lParam);
            EndPaint(hwndDlg, &Ps);
            return(0);
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            break;
    }
    if(uMsg) if(DialogTempProc != 0) ReturnValue = DialogTempProc(hwndDlg, uMsg, wParam, lParam);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Initialize Stock Form (Standard close)
int CALLBACK FRMStockModalDlgStdCloseProc(HWND hwndDlg, UINT uMsg, WPARAM wParam , LPARAM lParam)
{
    long ReturnValue = 0;
    PAINTSTRUCT Ps;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, DialogTempTitle);
            CreateButton(DialogTempWidth - 80, DialogTempHeight - 26, 77, 23, hwndDlg, "Close", BUTTON_STOCK_OK_ID, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP, _Btn_StaticEdge);
            LastDialog = hwndDlg;
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &Ps);
            GDIDrawHorzSep(hwndDlg, 0, ControlClientHeight(hwndDlg) - 33, ControlClientWidth(hwndDlg));
            if(DialogTempProc != 0) ReturnValue = DialogTempProc(hwndDlg, uMsg, wParam, lParam);
            EndPaint(hwndDlg, &Ps);
            return(0);
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            break;
    }
    if(uMsg) if(DialogTempProc != 0) ReturnValue = DialogTempProc(hwndDlg, uMsg, wParam, lParam);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Initialize Stock Form (Standard ok/cancel)
int CALLBACK FRMStockModalDlgStdOkCancelProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long ReturnValue = 0;
    PAINTSTRUCT Ps;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, DialogTempTitle);
            CreateButton(DialogTempWidth - 159, DialogTempHeight - 26, 77, 23, hwndDlg, "Ok", BUTTON_STOCK_OK_ID, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, _Btn_StaticEdge);
            CreateButton(DialogTempWidth - 80, DialogTempHeight - 26, 77, 23, hwndDlg, "Cancel", BUTTON_STOCK_CANCEL_ID, 0, 0, 0, WS_TABSTOP, _Btn_StaticEdge);
            LastDialog = hwndDlg;
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &Ps);
            GDIDrawHorzSep(hwndDlg, 0, ControlClientHeight(hwndDlg) - 33, ControlClientWidth(hwndDlg));
            if(DialogTempProc != 0) ReturnValue = DialogTempProc(hwndDlg, uMsg, wParam, lParam);
            EndPaint(hwndDlg, &Ps);
            return(0);
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            break;
    }
    if(uMsg) if(DialogTempProc != 0) ReturnValue = DialogTempProc(hwndDlg, uMsg, wParam, lParam);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Initialize Stock Form (Standard wizard)
int CALLBACK FRMStockModalDlgStdWizardProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long ReturnValue = 0;
    CStr BufString;
    PAINTSTRUCT Ps;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, DialogTempTitle);
            CreateButton(DialogTempWidth - (159 + 5), DialogTempHeight - 26, 77, 23, hwndDlg, "Finish", BUTTON_STOCK_FINISH_ID, 0, 0, 0, WS_GROUP | BS_DEFPUSHBUTTON | WS_TABSTOP, _Btn_StaticEdge);
            CreateButton(DialogTempWidth - 80, DialogTempHeight - 26, 77, 23, hwndDlg, "Cancel", BUTTON_STOCK_CANCEL_ID, 0, 0, 0, WS_TABSTOP, _Btn_StaticEdge);
            CreateButton(DialogTempWidth - (159 + 10 + (79 * 2)), DialogTempHeight - 26, 77, 23, hwndDlg, "Previous", BUTTON_STOCK_PREVIOUS_ID, 0, 0, 0, WS_TABSTOP, _Btn_StaticEdge);
            CreateButton(DialogTempWidth - (159 + 10 + 79), DialogTempHeight - 26, 77, 23, hwndDlg, "Next", BUTTON_STOCK_NEXT_ID, 0, 0, 0, WS_TABSTOP, _Btn_StaticEdge);
            ControlEnable(GetDlgItem(hwndDlg, BUTTON_STOCK_PREVIOUS_ID), 0);
            ControlEnable(GetDlgItem(hwndDlg, BUTTON_STOCK_NEXT_ID), 0);
            CreatePictureBox(0, 0, 200, DialogTempHeight - 34, hwndDlg, LoadBitmap(ApphInstance, (char *) DialogTempExtra), IMAGE_BITMAP, 0, 0, SS_CENTERIMAGE);
            LastDialog = hwndDlg;
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &Ps);
            GDIDrawHorzSep(hwndDlg, 0, ControlClientHeight(hwndDlg) - 33, ControlClientWidth(hwndDlg));
            if(DialogTempProc != 0) ReturnValue = DialogTempProc(hwndDlg, uMsg, wParam, lParam);
            EndPaint(hwndDlg, &Ps);
            return(0);
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
            break;
    }
    if(uMsg) if(DialogTempProc != 0) ReturnValue = DialogTempProc(hwndDlg, uMsg, wParam, lParam);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Allocate block in global memory (private)
extern "C"
long CALLBACK AllocMem(long MemSize)
{
    if(MemSize) return((long) calloc(MemSize + 1, 1));
    return(0);
}

// -----------------------------------------------------------------------
// Free a block allocated in global memory (private)
extern "C"
void CALLBACK FreeMem(long MemHandle)
{
    if(MemHandle) free((void *) MemHandle);
}
