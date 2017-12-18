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
// Splitters.cpp: Splitters (Output Box/TreeView handling)
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Classes/WADock.h"
#include "ConstMenus.h"
#include "ConstRes.h"
#include "Splitters.h"
#include "ImageLists.h"
#include "Globals.h"
#include "CmdMenus.h"
#include "ProjectsBuilder.h"
#include "MiscFunctions.h"
#include "Menus.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_Strings.h"
#include "Dlg_Accelerators.h"
#include "Dlg_Menus.h"
#include "Dlg_DllDef.h"
#include "Dlg_ImageView.h"
#include "Dlg_UserDialogs.h"
#include "Dlg_ResProps.h"

// -----------------------------------------------------------------------
// Variables
extern HWND hTreeViewDebug;
HWND hProjectContainer;
HWND hWindowsContainer;
HWND hOutputContainer;
HWND hProjectToolbar;
HWND hWindowsToolbar;
HWND hStatusToolBar;
HWND hProjectPager;
HWND hWindowsPager;
HWND hStatusPager;
HWND hTreeView;
HWND hWindowsListView;
SPLITTERDAT MyVSplitterDAT;
SPLITTERDAT MyHSplitterDAT;
HWND hOutputSysTab;
HWND hStatusBox;
HWND hDebugBox;
char *ProjectToolbarToolTip[PROJECTTOOLBAR_DEBUG + 1];
char *WindowsToolbarToolTip[WINDOWSTOOLBAR_RELOAD + 1];
char *StatusToolbarToolTip[STATUSTOOLBAR_CLEAR + 1];
CStr DOSReportStr;
long LastSelectTreePop;
long OldTreeViewToolTips;
NM_TREEVIEWEXPAND DragTreeView;
long TreeDragging;
HIMAGELIST hTreeImageList;
HTREEITEM hDragParent;
HTREEITEM CurrentTreeItemDrag;
long FirstDropped;
long LastDropped;
HTREEITEM DragNewhItem;
long DraggingCursor;
TV_HITTESTINFO TreeViewhit;
TV_HITTESTINFO TreeViewhitNew;
long TVLeftCoord;
long TVTopCoord;
long InContextMenu;
long StatusTextLarg;
long DebugTextLarg;
long StatusDebugMode;
long PreviouslySelectedWindow;

// -----------------------------------------------------------------------
// Functions declarations
LRESULT CALLBACK ProjectContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowsContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK OutputContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK TreeViewHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK StatusBoxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK DebugBoxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WindowsListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void ReadProjectDockState(int &State, RECT &Rect, int &Size);
void ReadWindowsDockState(int &State, RECT &Rect, int &Size);
void ReadOutputDockState(int &State, RECT &Rect, int &Size);
void MoveProjectItems(HTREEITEM OldItem, HTREEITEM NewItem);
void RefreshCurrentWindowFromList(void);
void SetPreviousChildInList(void);
void SetNextChildInList(void);
HWND GetSelectedWindowFromList(void);

// -----------------------------------------------------------------------
// Create the vertical splitter
void CreateDockingProject(HWND hParent)
{
    RECT Rec;

    hProjectContainer = CreateDockingBox("Project", &ProjectContainerProc, hMDIform.hDock, MainIniFile.Get_String());
    hTreeView = CreateTreeView(0, 23, ControlWidth(hProjectContainer), 10, hProjectContainer, 14, GlobalImageList1, &TreeViewHook, TVS_SHOWSELALWAYS, WS_EX_STATICEDGE);
    OldTreeViewToolTips = TreeViewGetToolTips(hTreeView);
    TreeViewSetIndent(hTreeView, 1);
    SetTreeviewTT();
    SetTreeviewExpand();
    hProjectToolbar = CreateToolBar(0, 0, 0, 15, hProjectContainer, GlobalImageList2, 4, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | TBS_VERT | TBSTYLE_WRAPABLE, 0);
    SendMessage(hProjectToolbar, TB_SETROWS, 0x60001, (long) &Rec);
    ToolBarAddButton(hProjectToolbar, "", PROJECTTOOLBAR_SAVE, ICON_SMALL_SAVELITTLE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddSeparator(hProjectToolbar, 0);
    ToolBarAddButton(hProjectToolbar, "", PROJECTTOOLBAR_ASSEMBLE, ICON_SMALL_ASSEMBLE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hProjectToolbar, "", PROJECTTOOLBAR_DEBUG, ICON_SMALL_DEBUG, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hProjectToolbar, "", PROJECTTOOLBAR_RUN, ICON_SMALL_EXEC, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    hProjectPager = CreatePager(0, 0, 0, 15, hProjectContainer, hProjectToolbar, 0, PGS_HORZ);
    ProjectToolbarToolTip[PROJECTTOOLBAR_SAVE] = "Save project (Ctrl+Shift+S)";
    ProjectToolbarToolTip[PROJECTTOOLBAR_ASSEMBLE] = "Build project (Shift+F5)";
    ProjectToolbarToolTip[PROJECTTOOLBAR_RUN] = "Run project (Ctrl+F5)";
    ProjectToolbarToolTip[PROJECTTOOLBAR_DEBUG] = "Debug project (Ctrl+Shift+F5)";
    DockingBoxShow(hProjectContainer, GetMDIWidth(), GetMDIHeight(), 216, 230, DOCKINGBOX_LEFT);
    SetProjectDockMenuState(ShowProjetDockWin);
}

// -----------------------------------------------------------------------
// Create the windows list
void CreateDockingWindows(HWND hParent)
{
    RECT Rec;

    hWindowsContainer = CreateDockingBox("Windows", &WindowsContainerProc, hMDIform.hDock, MainIniFile.Get_String());
    hWindowsListView = CreateListView(0, 0, ControlWidth(hWindowsContainer), 10, hWindowsContainer, 13, GlobalImageList1, &WindowsListProc, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | WS_TABSTOP | LVS_NOSORTHEADER | LVS_NOCOLUMNHEADER, WS_EX_STATICEDGE);
    ListViewAddCol(hWindowsListView, "", ControlWidth(hWindowsContainer), 0);
    hWindowsToolbar = CreateToolBar(0, 0, 0, 15, hWindowsContainer, GlobalImageList2, 4, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | TBS_VERT | TBSTYLE_WRAPABLE, 0);
    SendMessage(hWindowsToolbar, TB_SETROWS, 0x60001, (long) &Rec);
    ToolBarAddButton(hWindowsToolbar, "", WINDOWSTOOLBAR_PREVIOUS, ICON_SMALL_BACK, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hWindowsToolbar, "", WINDOWSTOOLBAR_NEXT, ICON_SMALL_DEBUG, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddSeparator(hWindowsToolbar, 0);
    ToolBarAddButton(hWindowsToolbar, "", WINDOWSTOOLBAR_RELOAD, ICON_SMALL_RELOAD, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    hWindowsPager = CreatePager(0, 0, 0, 15, hWindowsContainer, hWindowsToolbar, 0, PGS_HORZ);
    WindowsToolbarToolTip[WINDOWSTOOLBAR_PREVIOUS] = "Previous window (Ctrl+Shift+Tab)";
    WindowsToolbarToolTip[WINDOWSTOOLBAR_NEXT] = "Next window (Ctrl+Tab)";
    WindowsToolbarToolTip[WINDOWSTOOLBAR_RELOAD] = "Reload (Ctrl+Shift+N)";
    DockingBoxShow(hWindowsContainer, GetMDIWidth(), GetMDIHeight(), 216, 230, DOCKINGBOX_LEFT);
    SetWindowsDockMenuState(ShowWindowsDockWin);
}

// -----------------------------------------------------------------------
// Create the horizontal splitter
void CreateDockingOutput(HWND hParent)
{
    hOutputContainer = CreateDockingBox("Output", &OutputContainerProc, hMDIform.hDock, MainIniFile.Get_String());

    hOutputSysTab = CreateSysTab(17, 0, 600, 70, hOutputContainer, 0, 0, 0, WS_TABSTOP | TCS_HOTTRACK);
    SysTabAddItem(hOutputSysTab, "Misc.", 0, 0);
    SysTabAddItem(hOutputSysTab, "Debug", 1, 0);
    hStatusBox = CreateListBox(0, 0, 600, 70, hOutputSysTab, 0, &StatusBoxHook, 0, LBS_USETABSTOPS | WS_HSCROLL, WS_EX_STATICEDGE);
    hDebugBox = CreateListBox(0, 0, 600, 70, hOutputSysTab, 0, &DebugBoxHook, 0, LBS_USETABSTOPS | WS_HSCROLL, WS_EX_STATICEDGE);
    ControlVisible(hStatusBox, 1);
    ControlVisible(hDebugBox, 0);
    StatusDebugMode = FALSE;
    hStatusToolBar = CreateToolBar(1, 0, 17, 0, hOutputContainer, GlobalImageList2, 1, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | TBS_VERT | TBSTYLE_WRAPABLE, 0);
    ToolBarAddButton(hStatusToolBar, "", STATUSTOOLBAR_SAVE, ICON_SMALL_SAVELITTLE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(hStatusToolBar, "", STATUSTOOLBAR_NEW, ICON_SMALL_TOWIN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddSeparator(hStatusToolBar, 0);
    ToolBarAddButton(hStatusToolBar, "", STATUSTOOLBAR_CLEAR, ICON_SMALL_DELSTATUS, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    hStatusPager = CreatePager(0, 0, 17, 0, hOutputContainer, hStatusToolBar, 0, PGS_VERT);
    StatusToolbarToolTip[STATUSTOOLBAR_SAVE] = "Save content to file";
    StatusToolbarToolTip[STATUSTOOLBAR_NEW] = "Create new window with content";
    StatusToolbarToolTip[STATUSTOOLBAR_CLEAR] = "Empty output window";
    DockingBoxShow(hOutputContainer, GetMDIWidth(), GetMDIHeight(), 500, 120, DOCKINGBOX_BOTTOM);
    SetOutputDockMenuState(ShowOutputDockWin);
}

// -----------------------------------------------------------------------
// Project dock proc
LRESULT CALLBACK ProjectContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SIZE MySize;
    CStr DragOldText;
    CStr DragNewText;
    PAINTSTRUCT VSplitPs;
    SIZE TBSize;
    long SpTooltipId = 0;
    HTREEITEM CurrentTreeItem;
    HTREEITEM CurrentTreeParent;
    long TreeViewKey = 0;
    HTREEITEM hTreeParent;
    HTREEITEM DroppedItem;
    RECT FromDragRect;
    long *lParamKey = 0;
    long *lParamDrag = 0;
    HWND hTreeViewFrom = 0;
    long TbTooltipId = 0;
    long DockState;
    HTREEITEM TreeChild = 0;
    
    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &VSplitPs);
            DockState = DockingBoxGetState(hProjectContainer);
            switch(DockState)
            {
                case DOCKINGBOX_LEFT:
                case DOCKINGBOX_RIGHT:
                case DOCKINGBOX_FLOATING:
                    // Windows toolbar
                    ToolBarGetXYSize(hProjectToolbar, &TBSize);
                    MoveWindow(hProjectPager, 0, 0, ControlWidth(hWnd), TBSize.cy, 1);
                    MoveWindow(hTreeView, 0, 16, ControlWidth(hWnd), ControlHeight(hWnd) - 16, 1);
                    break;
                case DOCKINGBOX_TOP:
                case DOCKINGBOX_BOTTOM:
                    MoveWindow(hProjectPager, 0, 0, 17, ControlHeight(hWnd), 1);
                    MoveWindow(hTreeView, 18, 0, ControlWidth(hWnd) - 18, ControlHeight(hWnd), 1);
                    break;
            }
            EndPaint(hWnd, &VSplitPs);
            break;
        case WM_COMMAND:
            switch(wParam)
            {
                case (PROJECTTOOLBAR_SAVE):
                    MCMD_SaveProject();
                    return(0);
                case (PROJECTTOOLBAR_ASSEMBLE):
                    MCMD_AssembleProject();
                    return(0);
                case (PROJECTTOOLBAR_RUN):
                    MCMD_RunProject();
                    return(0);
                case (PROJECTTOOLBAR_DEBUG):
                    MCMD_DebugProject();
                    return(0);
            }
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case PGN_CALCSIZE:
                    ToolBarGetXYSize(PagerGetNotifiedChild(lParam), &MySize);
                    DockState = DockingBoxGetState(hProjectContainer);
                    ToolBarGetXYSize(PagerGetNotifiedChild(lParam), &MySize);
                    switch(DockState)
                    {
                        case DOCKINGBOX_TOP:
                        case DOCKINGBOX_BOTTOM:
                            PagerChangeOrientation(hProjectPager, (LPNMPGCALCSIZE) lParam, PGS_VERT, &MySize);
                            break;
                        default:
                            PagerChangeOrientation(hProjectPager, (LPNMPGCALCSIZE) lParam, PGS_HORZ, &MySize);
                            break;
                    }
                    return(0);
                case NM_DBLCLK:
                    if(ControlGetNotifiedhWnd(lParam) == hTreeView)
                    {
                        CurrentTreeItem = TreeViewGetSelectedItem(hTreeView);
                        // Pass the parents
                        if(CurrentTreeItem == hTreeViewRoot) return(0);
                        if(CurrentTreeItem == hTreeViewIncludes) return(0);
                        if(CurrentTreeItem == hTreeViewLibs) return(0);
                        if(CurrentTreeItem == hTreeViewModules) return(0);
                        if(CurrentTreeItem == hTreeViewObjects) return(0);
                        if(CurrentTreeItem == hTreeViewResources) return(0);
                        if(CurrentTreeItem == hTreeViewIcons) return(0);
                        if(CurrentTreeItem == hTreeViewCursors) return(0);
                        if(CurrentTreeItem == hTreeViewBitmaps) return(0);
                        if(CurrentTreeItem == hTreeViewStrings) return(0);
                        if(CurrentTreeItem == hTreeViewAccelerators) return(0);
                        if(CurrentTreeItem == hTreeViewMenus) return(0);
                        if(CurrentTreeItem == hTreeViewDialogs) return(0);
                        if(CurrentTreeItem == hTreeViewRawdatas) return(0);
                        if(CurrentTreeItem == hTreeViewDefs) return(0);
                        if(CurrentTreeItem == hTreeViewTexts) return(0);
                        // Handle the childs
                        CurrentTreeParent = TreeViewGetItemParent(hTreeView, CurrentTreeItem);
                        if(CurrentTreeParent == hTreeViewIncludes)
                        {
                            FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewLibs)
                        {
                            GenProto(GetFileNameFromTreeView(CurrentTreeItem));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewModules)
                        {
                            FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewObjects)
                        {
                            return(0);
                        }
                        // Normal resource
                        if(CurrentTreeParent == hTreeViewResources)
                        {
                            if(GetFileNameFromTreeView(CurrentTreeItem).Len() != 0) FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                            return(0);
                        }
                        // New resources
                        if(CurrentTreeParent == hTreeViewIcons)
                        {
                            DisplayImageView(VIEW_ICON, GetFileNameFromTreeView(CurrentTreeItem));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewCursors)
                        {
                            DisplayImageView(VIEW_CURSOR, GetFileNameFromTreeView(CurrentTreeItem));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewBitmaps)
                        {
                            DisplayImageView(VIEW_BITMAP, GetFileNameFromTreeView(CurrentTreeItem));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewStrings)
                        {
                            EditStringsTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, hTreeViewStrings));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewAccelerators)
                        {
                            EditAcceleratorsTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewMenus)
                        {
                            EditMenusTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewDialogs)
                        {
                            // TODO
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewRawdatas)
                        {
                            OpenFileAsRC(0, GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewDefs)
                        {
                            DisplayDefForm(GetFileNameFromTreeView(CurrentTreeItem));
                            return(0);
                        }
                        if(CurrentTreeParent == hTreeViewTexts)
                        {
                            FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                            return(0);
                        }
                    }
                    return(0);
                case TTN_NEEDTEXT:
                    SpTooltipId = ControlGetNotifiedID(lParam);
                    if(SpTooltipId <= PROJECTTOOLBAR_DEBUG)
                    {
                        ToolBarDisplayToolTip(ProjectToolbarToolTip[SpTooltipId], lParam);
                    }
                    return(0);
                // Delete an entry in the treeview
                case TVN_KEYDOWN:
                    lParamKey = (long *) lParam;
                    TreeViewKey = lParamKey[3] & 0xffff;
                    CurrentTreeItem = TreeViewGetSelectedItem(hTreeView);
                    if(CurrentTreeItem == hTreeViewRoot) break;
                    if(CurrentTreeItem == hTreeViewIncludes) break;
                    if(CurrentTreeItem == hTreeViewLibs) break;
                    if(CurrentTreeItem == hTreeViewModules) break;
                    if(CurrentTreeItem == hTreeViewObjects) break;
                    if(CurrentTreeItem == hTreeViewResources) break;
                    if(CurrentTreeItem == hTreeViewIcons) break;
                    if(CurrentTreeItem == hTreeViewCursors) break;
                    if(CurrentTreeItem == hTreeViewBitmaps) break;
                    if(CurrentTreeItem == hTreeViewStrings) break;
                    if(CurrentTreeItem == hTreeViewAccelerators) break;
                    if(CurrentTreeItem == hTreeViewMenus) break;
                    if(CurrentTreeItem == hTreeViewDialogs) break;
                    if(CurrentTreeItem == hTreeViewRawdatas) break;
                    if(CurrentTreeItem == hTreeViewDefs) break;
                    if(CurrentTreeItem == hTreeViewTexts) break;
                    switch(TreeViewKey)
                    {
                        case 46:
                            hTreeParent = TreeViewGetItemParent(hTreeView, CurrentTreeItem);
                            if(hTreeParent == hTreeViewIncludes)
                            {
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewLibs)
                            {
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewModules)
                            {
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewObjects)
                            {
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewResources)
                            {
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewIcons)
                            {
                                RemIconFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewCursors)
                            {
                                RemCursorFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewBitmaps)
                            {
                                RemBitmapFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewStrings)
                            {
                                RemStringFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewAccelerators)
                            {
                                RemAcceleratorFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewMenus)
                            {
                                RemMenuFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewDialogs)
                            {
                                RemDialogFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewRawdatas)
                            {
                                RemRawDataFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            if(hTreeParent == hTreeViewTexts)
                            {
                                TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                ProjectModified = TRUE;
                                return(0);
                            }
                            break;
                        case 13:
                            hTreeParent = TreeViewGetItemParent(hTreeView, CurrentTreeItem);
                            if(hTreeParent == hTreeViewIncludes)
                            {
                                FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                                return(0);
                            }
                            if(hTreeParent == hTreeViewLibs)
                            {
                                GenProto(GetFileNameFromTreeView(CurrentTreeItem));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewModules)
                            {
                                FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                                return(0);
                            }
                            if(hTreeParent == hTreeViewObjects)
                            {
                                return(0);
                            }
                            if(hTreeParent == hTreeViewResources)
                            {
                                if(GetFileNameFromTreeView(CurrentTreeItem).Len() != 0) FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                                return(0);
                            }
                            if(hTreeParent == hTreeViewIcons)
                            {
                                DisplayImageView(VIEW_ICON, GetFileNameFromTreeView(CurrentTreeItem));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewCursors)
                            {
                                DisplayImageView(VIEW_CURSOR, GetFileNameFromTreeView(CurrentTreeItem));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewBitmaps)
                            {
                                DisplayImageView(VIEW_BITMAP, GetFileNameFromTreeView(CurrentTreeItem));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewStrings) {
                                EditStringsTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, hTreeViewStrings));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewAccelerators)
                            {
                                EditAcceleratorsTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewMenus)
                            {
                                EditMenusTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewDialogs)
                            {
                                // TODO
                                return(0);
                            }
                            if(hTreeParent == hTreeViewRawdatas)
                            {
                                OpenFileAsRC(0, GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewDefs)
                            {
                                DisplayDefForm(GetFileNameFromTreeView(CurrentTreeItem));
                                return(0);
                            }
                            if(hTreeParent == hTreeViewTexts)
                            {
                                FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                                return(0);
                            }
                            break;
                    }
                    break;
                case TVN_ITEMEXPANDING:
                    hTreeViewFrom = ControlGetNotifiedhWnd(lParam);
                    switch(TreeViewGetExpandingState(lParam))
                    {
                        case TVE_EXPAND:
                            TreeViewSetItemIcon(hTreeViewFrom, TreeViewGetExpandingItem(lParam), ICON_OPEN);
                            break;
                        case TVE_COLLAPSE:
                            TreeViewSetItemIcon(hTreeViewFrom, TreeViewGetExpandingItem(lParam), ICON_FILES);
                            break;
                    }
                    return(0);
                case TVN_SELCHANGED:
                    CurrentTreeItem = TreeViewGetSelectedItem(hTreeView);
                    if(CurrentTreeItem == hTreeViewRoot ||
                        CurrentTreeItem == hTreeViewIncludes ||
                        CurrentTreeItem == hTreeViewLibs ||
                        CurrentTreeItem == hTreeViewModules ||
                        CurrentTreeItem == hTreeViewObjects ||
                        CurrentTreeItem == hTreeViewResources ||
                        CurrentTreeItem == hTreeViewIcons ||
                        CurrentTreeItem == hTreeViewCursors ||
                        CurrentTreeItem == hTreeViewBitmaps ||
                        CurrentTreeItem == hTreeViewStrings ||
                        CurrentTreeItem == hTreeViewAccelerators ||
                        CurrentTreeItem == hTreeViewMenus ||
                        CurrentTreeItem == hTreeViewDialogs ||
                        CurrentTreeItem == hTreeViewRawdatas ||
                        CurrentTreeItem == hTreeViewDefs ||
                        CurrentTreeItem == hTreeViewTexts)
                    {
                        TreeChild = TreeViewGetFirstItemChild(hTreeView,CurrentTreeItem);
                        if(TreeChild == NULL)
                        {
                            // Collapse the dir if it is empty
                            TreeViewSetItemIcon(hTreeView, CurrentTreeItem, ICON_FILES);
                        }
                    }
                    break;

// ----------------------------------------
// Handle Treeview drag'n'drop
// ----------------------------------------
                case TVN_BEGINDRAG:
                    if(ControlGetNotifiedhWnd(lParam) == hTreeView)
                    {
                        DraggingCursor = 0;
                        // Hack here (should be NM_TREEVIEW)
                        // New.hItem
                        lParamDrag = (long *) lParam;
                        DragNewhItem = (HTREEITEM) lParamDrag[3 + 1 + 10 + 1];
                        // Parents
                        if(DragNewhItem == hTreeViewRoot) return(0);
                        if(DragNewhItem == hTreeViewIcons) return(0);
                        if(DragNewhItem == hTreeViewCursors) return(0);
                        if(DragNewhItem == hTreeViewBitmaps) return(0);
                        if(DragNewhItem == hTreeViewStrings) return(0);
                        if(DragNewhItem == hTreeViewAccelerators) return(0);
                        if(DragNewhItem == hTreeViewMenus) return(0);
                        if(DragNewhItem == hTreeViewDialogs) return(0);
                        if(DragNewhItem == hTreeViewRawdatas) return(0);
                        hDragParent = TreeViewGetItemParent(hTreeView, DragNewhItem);
                        if(hDragParent != 0)
                        {
                            if(hTreeViewRoot != hDragParent)
                            {
                                hTreeImageList = (HIMAGELIST) SendMessage(hTreeView, TVM_CREATEDRAGIMAGE, 0, (long) DragNewhItem);
                                if(hTreeImageList != 0)
                                {
                                    SetCapture(hWnd);
                                    GetCursorPos(&TreeViewhit.pt);
                                    TreeViewhit.pt.x = TreeViewhit.pt.x - ControlLeft(hWnd);
                                    TreeViewhit.pt.y = TreeViewhit.pt.y - ControlTop(hWnd);
                                    ImageList_BeginDrag(hTreeImageList, 0, TreeViewhit.pt.x, TreeViewhit.pt.y);
                                    ImageList_DragEnter(0, ControlLeft(hWnd) + TreeViewhit.pt.x, ControlTop(hWnd) + TreeViewhit.pt.y);
                                    if(TreeViewGetItemTextRect(hTreeView, DragNewhItem, &FromDragRect) != 0)
                                    {
                                        TVLeftCoord = FromDragRect.left - 17 - (ControlLeft(hTreeView) - ControlLeft(hWnd));
                                    }
                                    else
                                    {
                                        TVLeftCoord = TreeViewhit.pt.x;
                                    }
                                    if(TreeViewGetItemRect(hTreeView, DragNewhItem, &FromDragRect) != 0)
                                    {
                                        TVTopCoord = FromDragRect.top - (17 / 2) + ((TreeViewhit.pt.y - FromDragRect.top));
                                    }
                                    else
                                    {
                                        TVTopCoord = TreeViewhit.pt.y;
                                    }
                                    TreeDragging = 1;
                                }
                            }
                        }
                    }
                    return(0);
            }
            break;
        case WM_LBUTTONUP:
            if(TreeDragging == 1)
            {
                TreeDragging = 0;
                CursorSetNormal();
                ImageList_DragLeave(hTreeView);
                ImageList_EndDrag();
                ImageList_Destroy(hTreeImageList);
                if(DraggingCursor == 0) MoveProjectItems(DragNewhItem, CurrentTreeItemDrag);
                DroppedItem = (HTREEITEM) SendMessage(hTreeView, TVM_GETNEXTITEM, TVGN_DROPHILITE, 0);
                SendMessage(hTreeView, TVM_SELECTITEM, TVGN_CARET, (long) DroppedItem);
                SendMessage(hTreeView, TVM_SELECTITEM, TVGN_DROPHILITE, 0);
                if(FirstDropped != LastDropped) ProjectModified = TRUE;
                DraggingCursor = 0;
                ReleaseCapture();
            }
            return(0);
        case WM_MOUSEMOVE:
            if(TreeDragging == 1)
            {
                GetCursorPos(&TreeViewhitNew.pt);
                TreeViewhitNew.pt.x = TreeViewhitNew.pt.x + TVLeftCoord;
                TreeViewhitNew.pt.y = TreeViewhitNew.pt.y + TVTopCoord;
                ImageList_DragMove(TreeViewhitNew.pt.x, TreeViewhitNew.pt.y);
                TreeViewhitNew.pt.x = lParam & 0xFFFF;
                TreeViewhitNew.pt.y = ((lParam & 0xFFFF0000) >> 16) - 17;
                ImageList_DragShowNolock(0);
                CurrentTreeItemDrag = (HTREEITEM) SendMessage(hTreeView, TVM_HITTEST, 0, (long) &TreeViewhitNew);
                if(CurrentTreeItemDrag != 0)
                {
                    if(hDragParent == CurrentTreeItemDrag) goto DragToParent;
                    if(TreeViewGetItemParent(hTreeView, CurrentTreeItemDrag) == hDragParent)
                    {
DragToParent:           if(CurrentTreeItemDrag == hTreeViewIcons)
                        {
                            if(hDragParent == hTreeViewIcons) goto ForceDrag;
                        }
                        else if(CurrentTreeItemDrag == hTreeViewCursors)
                        {
                            if(hDragParent == hTreeViewCursors) goto ForceDrag;
                        }
                        else if(CurrentTreeItemDrag == hTreeViewBitmaps)
                        {
                            if(hDragParent == hTreeViewBitmaps) goto ForceDrag;
                        }
                        else if(CurrentTreeItemDrag == hTreeViewStrings)
                        {
                            if(hDragParent == hTreeViewStrings) goto ForceDrag;
                        }
                        else if(CurrentTreeItemDrag == hTreeViewAccelerators)
                        {
                            if(hDragParent == hTreeViewAccelerators) goto ForceDrag;
                        }
                        else if(CurrentTreeItemDrag == hTreeViewMenus)
                        {
                            if(hDragParent == hTreeViewMenus) goto ForceDrag;
                        }
                        else if(CurrentTreeItemDrag == hTreeViewDialogs)
                        {
                            if(hDragParent == hTreeViewDialogs) goto ForceDrag;
                        }
                        else if(CurrentTreeItemDrag == hTreeViewRawdatas)
                        {
                            if(hDragParent == hTreeViewRawdatas) goto ForceDrag;
                        }
                        else
                        {
ForceDrag:                  SendMessage(hTreeView, TVM_SELECTITEM, TVGN_DROPHILITE, (long) CurrentTreeItemDrag);
                        }
                    }
                    if(DraggingCursor == 1)
                    {
                        CursorSetNormal();
                        DraggingCursor = 0;
                    }
                }
                else
                {
                    if(DraggingCursor == 0)
                    {
                        CursorSetNo();
                        DraggingCursor = 1;
                    }
                }
                ImageList_DragShowNolock(1);
            }
            return(0);
        case DOCKINGBOX_MSG_QUERY_STATE:
            return(ShowProjetDockWin);
        case DOCKINGBOX_MSG_CLOSE:
            ShowProjetDockWin = 0;
        case WM_DESTROY:
            SetProjectDockMenuState(ShowProjetDockWin);
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Windows dock proc
LRESULT CALLBACK WindowsContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SIZE MySize;
    CStr DragOldText;
    CStr DragNewText;
    PAINTSTRUCT VSplitPs;
    SIZE TBSize;
    long SpTooltipId = 0;
    HWND hTreeViewFrom = 0;
    long TbTooltipId = 0;
    long DockState;
    HMENU hPop;
    POINT PopupPoint;
    HWND hSel;
    
    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &VSplitPs);
            DockState = DockingBoxGetState(hWindowsContainer);
            switch(DockState)
            {
                case DOCKINGBOX_LEFT:
                case DOCKINGBOX_RIGHT:
                case DOCKINGBOX_FLOATING:
                    // Windows toolbar
                    ToolBarGetXYSize(hWindowsToolbar, &TBSize);
                    MoveWindow(hWindowsPager, 0, 0, ControlWidth(hWnd), TBSize.cy, 1);
                    MoveWindow(hWindowsListView, 0, 16, ControlWidth(hWnd), ControlHeight(hWnd) - 16, 1);
                    break;
                case DOCKINGBOX_TOP:
                case DOCKINGBOX_BOTTOM:
                    MoveWindow(hWindowsPager, 0, 0, 17, ControlHeight(hWnd), 1);
                    MoveWindow(hWindowsListView, 18, 0, ControlWidth(hWnd) - 18, ControlHeight(hWnd), 1);
                    break;
            }
            ListViewSetColWidth(hWindowsListView, 0, ControlClientWidth(hWindowsListView) - 2);
            EndPaint(hWnd, &VSplitPs);
            break;
        case WM_COMMAND:
            switch(wParam)
            {
                case WINDOWSTOOLBAR_PREVIOUS:
                    SetPreviousChildInList();
                    return(0);
                case WINDOWSTOOLBAR_NEXT:
                    SetNextChildInList();
                    return(0);
                case WINDOWSTOOLBAR_RELOAD:
                    MCMD_Reload();
                    return(0);

                case MENU_WINDOWSBOX_CLOSE_ID + MENU_WINDOWSBOX_IDBASE:
                    // Close the selected file
                    hSel = GetSelectedWindowFromList();
                    if(hSel)
                    {
                        ControlClose(hSel);
                    }
                    return(0);

                case MENU_WINDOWSBOX_SAVE_ID + MENU_WINDOWSBOX_IDBASE:
                    // Save the selected file
                    hSel = GetSelectedWindowFromList();
                    if(hSel)
                    {
                        SaveIt(hSel);
                    }
                    return(0);

                case MENU_WINDOWSBOX_SAVEAS_ID + MENU_WINDOWSBOX_IDBASE:
                    // Save the selected file under a new name
                    hSel = GetSelectedWindowFromList();
                    if(hSel)
                    {
                        SaveItAs(hSel, 0, "", "");
                    }
                    return(0);

                case MENU_WINDOWSBOX_TOGGLE_ID + MENU_WINDOWSBOX_IDBASE:
                    // Toggle read only flag
                    hSel = GetSelectedWindowFromList();
                    if(hSel)
                    {
                        if(IsChildReadOnly(hSel))
                        {
                            SetChildReadOnly(hSel, FALSE);
                        }
                        else
                        {
                            SetChildReadOnly(hSel, TRUE);
                        }
                        RefreshChildTitle(hSel);
                    }
                    return(0);

            }
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case PGN_CALCSIZE:
                    ToolBarGetXYSize(PagerGetNotifiedChild(lParam), &MySize);
                    DockState = DockingBoxGetState(hWindowsContainer);
                    ToolBarGetXYSize(PagerGetNotifiedChild(lParam), &MySize);
                    switch(DockState)
                    {
                        case DOCKINGBOX_TOP:
                        case DOCKINGBOX_BOTTOM:
                            PagerChangeOrientation(hWindowsPager, (LPNMPGCALCSIZE) lParam, PGS_VERT, &MySize);
                            break;
                        default:
                            PagerChangeOrientation(hWindowsPager, (LPNMPGCALCSIZE) lParam, PGS_HORZ, &MySize);
                            break;
                    }
                    return(0);
                case TTN_NEEDTEXT:
                    SpTooltipId = ControlGetNotifiedID(lParam);
                    if(SpTooltipId <= WINDOWSTOOLBAR_RELOAD)
                    {
                        ToolBarDisplayToolTip(WindowsToolbarToolTip[SpTooltipId], lParam);
                    }
                    return(0);
                case NM_RCLICK:
                    // Create Contextual menu
                    hPop = CreatePopupMenu();
                    MenuAddString(hPop, "Close", MENU_WINDOWSBOX_CLOSE_ID + MENU_WINDOWSBOX_IDBASE, TRUE);
                    MenuAddSeparator(hPop);
                    MenuAddString(hPop, "Save", MENU_WINDOWSBOX_SAVE_ID + MENU_WINDOWSBOX_IDBASE, TRUE);
                    MenuAddString(hPop, "Save as...", MENU_WINDOWSBOX_SAVEAS_ID + MENU_WINDOWSBOX_IDBASE, TRUE);
                    MenuAddSeparator(hPop);
                    MenuAddString(hPop, "Toggle read only", MENU_WINDOWSBOX_TOGGLE_ID + MENU_WINDOWSBOX_IDBASE, TRUE);
                    GetCursorPos(&PopupPoint);
                    TrackPopupMenu(hPop, TPM_LEFTALIGN + TPM_LEFTBUTTON, PopupPoint.x, PopupPoint.y, 0, hWnd, NULL);
                    return(0);
                case NM_CLICK:
                    if(ControlGetNotifiedhWnd(lParam) == hWindowsListView) RefreshCurrentWindowFromList();
                    return(0);
            }
            break;

        case WM_EXITMENULOOP:
            return(0);

        case DOCKINGBOX_MSG_QUERY_STATE:
            return(ShowWindowsDockWin);
        case DOCKINGBOX_MSG_CLOSE:
            ShowWindowsDockWin = 0;
        case WM_DESTROY:
            SetWindowsDockMenuState(ShowWindowsDockWin);
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Horizontal splitter proc
LRESULT CALLBACK OutputContainerProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    SIZE MySize;
    PAINTSTRUCT HSplitPs;
    SIZE TBSize;
    long StTooltipId = 0;
    long MaxMdi = 0;
    long DockState = 0;
    RECT Rect;

    switch(uMsg)
    {
        case WM_PAINT:
            BeginPaint(hWnd, &HSplitPs);
            DockState = DockingBoxGetState(hOutputContainer);

            switch(DockState)
            {
                case DOCKINGBOX_LEFT:
                case DOCKINGBOX_RIGHT:
                case DOCKINGBOX_FLOATING:
                    // Windows toolbar
                    ToolBarGetXYSize(hStatusToolBar, &TBSize);
                    MoveWindow(hStatusPager, 0, 0, ControlWidth(hWnd), TBSize.cy, 1);
                    MoveWindow(hOutputSysTab, 0, 16, ControlClientWidth(hWnd), ControlClientHeight(hWnd) - 16, 1);
                    SendMessage(hOutputSysTab, TCM_GETITEMRECT, 0, (LPARAM) &Rect); 
                    MoveWindow(hStatusBox, 5, Rect.bottom + 5, ControlClientWidth(hOutputSysTab) - 10, ControlClientHeight(hOutputSysTab) - (Rect.bottom + 5) - 5, 1);
                    MoveWindow(hDebugBox, 5, Rect.bottom + 5, ControlClientWidth(hOutputSysTab) - 10, ControlClientHeight(hOutputSysTab) - (Rect.bottom + 5) - 5, 1);
                    break;
                case DOCKINGBOX_TOP:
                case DOCKINGBOX_BOTTOM:
                    MoveWindow(hStatusPager, 0, 0, 17, ControlHeight(hWnd), 1);
                    MoveWindow(hOutputSysTab, 18, 0, ControlClientWidth(hWnd) - 18, ControlClientHeight(hWnd), 1);
                    SendMessage(hOutputSysTab, TCM_GETITEMRECT, 0, (LPARAM) &Rect); 
                    MoveWindow(hStatusBox, 5, Rect.bottom + 5, ControlClientWidth(hOutputSysTab) - 10, ControlClientHeight(hOutputSysTab) - (Rect.bottom + 5) - 5, 1);
                    MoveWindow(hDebugBox, 5, Rect.bottom + 5, ControlClientWidth(hOutputSysTab) - 10, ControlClientHeight(hOutputSysTab) - (Rect.bottom + 5) - 5, 1);
                    break;
            }
            ListBoxSetHorzScrollWidth(hStatusBox, StatusTextLarg);
            ListBoxSetHorzScrollWidth(hDebugBox, DebugTextLarg);
            EndPaint(hWnd, &HSplitPs);
            break;
        case WM_COMMAND:
            if (lParam == (long) hStatusToolBar)
            {
                switch(wParam)
                {
                    case STATUSTOOLBAR_SAVE:
                        if(StatusDebugMode) SaveBuildReport(hDebugBox);
                        else SaveBuildReport(hStatusBox);
                        return(0);
                    case STATUSTOOLBAR_NEW:
                        if(StatusDebugMode) CreateBuildReport(hDebugBox);
                        else CreateBuildReport(hStatusBox);
                        return(0);
                    case STATUSTOOLBAR_CLEAR:
                        if(StatusDebugMode) ClearDebug();
                        else ClearStatus();
                        return(0);
                }
            }
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case PGN_CALCSIZE:
                    DockState = DockingBoxGetState(hOutputContainer);
                    ToolBarGetXYSize(PagerGetNotifiedChild(lParam), &MySize);
                    switch(DockState)
                    {
                        case DOCKINGBOX_TOP:
                        case DOCKINGBOX_BOTTOM:
                            PagerChangeOrientation(hStatusPager, (LPNMPGCALCSIZE) lParam, PGS_VERT, &MySize);
                            break;
                        default:
                            PagerChangeOrientation(hStatusPager, (LPNMPGCALCSIZE) lParam, PGS_HORZ, &MySize);
                            break;
                    }
                    return(0);
                case TTN_NEEDTEXT:
                    StTooltipId = ControlGetNotifiedID(lParam);
                    if(StTooltipId <= STATUSTOOLBAR_CLEAR)
                    {
                        ToolBarDisplayToolTip(StatusToolbarToolTip[StTooltipId], lParam);
                    }
                    return(0);
                case TCN_SELCHANGE:
                    switch(SysTabGetCurrentItem(hOutputSysTab))
                    {
                        case 0:
                            ControlVisible(hStatusBox, TRUE);
                            ControlVisible(hDebugBox, FALSE);
                            StatusToolbarToolTip[STATUSTOOLBAR_CLEAR] = "Empty output window";
                            StatusDebugMode = FALSE;
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_SAVE, TRUE);
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_NEW, TRUE);
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_CLEAR, TRUE);
                            break;
                        case 1:
                            ControlVisible(hDebugBox, TRUE);
                            ControlVisible(hStatusBox, FALSE);
                            StatusToolbarToolTip[STATUSTOOLBAR_CLEAR] = "Empty debug window";
                            StatusDebugMode = TRUE;
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_SAVE, TRUE);
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_NEW, TRUE);
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_CLEAR, TRUE);
                            break;
                        default:
                            ControlVisible(hStatusBox, FALSE);
                            ControlVisible(hDebugBox, FALSE);
                            StatusDebugMode = FALSE;
                            StatusToolbarToolTip[STATUSTOOLBAR_CLEAR] = "";
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_SAVE, FALSE);
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_NEW, FALSE);
                            ToolBarSetButtonEnabled(hStatusToolBar, STATUSTOOLBAR_CLEAR, FALSE);
                            break;
                    }
                    return(0);
            }
            break;
        case DOCKINGBOX_MSG_QUERY_STATE:
            return(ShowOutputDockWin);
        case DOCKINGBOX_MSG_CLOSE:
            ShowOutputDockWin = 0;
        case WM_DESTROY:
            SetOutputDockMenuState(ShowOutputDockWin);
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Show/Hide project window
void SetProjectDockState(long bShow)
{
    SetProjectDockMenuState(bShow);
    switch(bShow)
    {
        case 0:
            DockingBoxRemove(hProjectContainer);
            break;
        case 1:
            DockingBoxShow(hProjectContainer, GetMDIWidth(), GetMDIHeight(), 120, 230, DOCKINGBOX_LEFT);
            break;
    }
}

// -----------------------------------------------------------------------
// Set project window menu state
// (MF_BYPOSITION so -1)
void SetProjectDockMenuState(long bShow)
{
    switch(bShow)
    {
        case 0:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_PROJECTDOCK_ID - 1, MF_BYPOSITION | MF_UNCHECKED);
            break;
        case 1:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_PROJECTDOCK_ID - 1, MF_BYPOSITION | MF_CHECKED);
            break;
    }
}

// -----------------------------------------------------------------------
// Show/Hide windows list window
void SetWindowsDockState(long bShow)
{
    SetWindowsDockMenuState(bShow);
    switch(bShow)
    {
        case 0:
            DockingBoxRemove(hWindowsContainer);
            break;
        case 1:
            DockingBoxShow(hWindowsContainer, GetMDIWidth(), GetMDIHeight(), 120, 230, DOCKINGBOX_LEFT);
            break;
    }
}

// -----------------------------------------------------------------------
// Set windows list window menu state
void SetWindowsDockMenuState(long bShow)
{
    switch(bShow)
    {
        case 0:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_WINDOWSDOCK_ID - 1, MF_BYPOSITION | MF_UNCHECKED);
            break;
        case 1:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_WINDOWSDOCK_ID - 1, MF_BYPOSITION | MF_CHECKED);
            break;
    }
}

// -----------------------------------------------------------------------
// Show/Hide output window
void SetOutputDockState(long bShow)
{
    SetOutputDockMenuState(bShow);
    switch(bShow)
    {
        case 0:
            DockingBoxRemove(hOutputContainer);
            break;
        case 1:
            DockingBoxShow(hOutputContainer, GetMDIWidth(), GetMDIHeight(), 500, 120, DOCKINGBOX_BOTTOM);
            break;
    }
}

// -----------------------------------------------------------------------
// Set output window menu state
void SetOutputDockMenuState(long bShow)
{
    switch(bShow)
    {
        case 0:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_OUTPUTDOCK_ID - 1, MF_BYPOSITION | MF_UNCHECKED);
            break;
        case 1:
            CheckMenuItem(hViewMenuDisplay, MENU_VIEW_OUTPUTDOCK_ID - 1, MF_BYPOSITION | MF_CHECKED);
            break;
    }
}

// -----------------------------------------------------------------------
// Add a text to the status window
void WriteToStatus(CStr Txt)
{
    CStr SilentAutoValue;
    long TextWidth;

    // Popup window if hidden
    SilentAutoValue = IniReadKey("Layout", "SilentAuto", MainIniFile);
    if(SilentAutoValue.Len() == 0)
    {
        if(ShowOutputDockWin == 0)
        {
            ShowOutputDockWin = 1;
            SetOutputDockState(ShowOutputDockWin);
        }
    }
    else
    {
        if(strcmp(SilentAutoValue.Get_String(), "0") == 0)
        {
            if(ShowOutputDockWin == 0)
            {
                ShowOutputDockWin = 1;
                SetOutputDockState(ShowOutputDockWin);
            }
        }
    }
    // Store length
    TextWidth = GDIGetTextWidth(hStatusBox,WASerifFont,Txt);
    if(TextWidth > StatusTextLarg) StatusTextLarg = TextWidth;
    ListBoxAddItem(hStatusBox, Txt, -1);
    ListBoxSetTopIndex(hStatusBox, ListBoxCount(hStatusBox) - 1);
}

// -----------------------------------------------------------------------
// Add a text to the debug window
void WriteToDebug(CStr Txt)
{
    CStr SilentAutoValue;
    long TextWidth;

    // Popup window if hidden
    SilentAutoValue = IniReadKey("Layout", "SilentAuto", MainIniFile);
    if(SilentAutoValue.Len() == 0)
    {
        if(ShowOutputDockWin == 0)
        {
            ShowOutputDockWin = 1;
            SetOutputDockState(ShowOutputDockWin);
        }
    }
    else
    {
        if(strcmp(SilentAutoValue.Get_String(), "0") == 0)
        {
            if(ShowOutputDockWin == 0)
            {
                ShowOutputDockWin = 1;
                SetOutputDockState(ShowOutputDockWin);
            }
        }
    }
    // Store length
    TextWidth = GDIGetTextWidth(hDebugBox, WACourrierNewFont9, Txt);
    if(TextWidth > DebugTextLarg) DebugTextLarg = TextWidth;
    ListBoxAddItem(hDebugBox, Txt, -1);
    ListBoxSetTopIndex(hDebugBox, ListBoxCount(hDebugBox) - 1);
}

// -----------------------------------------------------------------------
// Clear the status window
void ClearStatus(void)
{
    ListBoxReset(hStatusBox);
    StatusTextLarg = 0;
}

// -----------------------------------------------------------------------
// Clear the debug window
void ClearDebug(void)
{
    ListBoxReset(hDebugBox);
    DebugTextLarg = 0;
}

// -----------------------------------------------------------------------
// Save the build report into a file
void SaveBuildReport(HWND hWnd)
{
    CStr FName;
    CStr Filters;
    CStr CCCDir;
    int i = 0;
    
    if(ListBoxCount(hWnd) == 0) return;
    Filters = "All files (*.*)|*.*";
    FName = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, "", CurrentDir);
    DeleteFile(FName.Get_String());
    if(FName.Len() != 0)
    {
        DOSReportStr = "";
        for(i = 0; i <= (long) ListBoxCount(hWnd) - 1; i++)
        {
            DOSReportStr = DOSReportStr + ListBoxGetItem(hWnd, i);
            DOSReportStr = DOSReportStr + DOSReportStr.New_Line();
        }
        if(MSaveFile(FName.Get_String(), (long) DOSReportStr.Get_String(), DOSReportStr.Len()) == 0)
        {
            MsgBoxStr = "Error in saving file: '" + (CStr) FName + (CStr) "'.";
            MiscMsgBox(hMDIform.hWnd, MsgBoxStr, MB_ERROR, Requesters);
        }
        else
        {
            MsgBoxStr = "Output saved as '" + (CStr) FName + (CStr) "'.";
            MiscMsgBox(hMDIform.hWnd, MsgBoxStr, MB_INFORMATION, Requesters);
        }
    }
}

// -----------------------------------------------------------------------
// Save the build report into a new window
void CreateBuildReport(HWND hWnd)
{
    int i = 0;
    CStr ReportText;
    CStr BufString;
    HWND NewChildHandle = 0;
    
    if(ListBoxCount(hWnd) == 0) return;
    ReportText = "";
    for(i = 0; i <= (long) ListBoxCount(hWnd) - 1; i++)
    {
        ReportText = ReportText + (CStr) ListBoxGetItem(hWnd, i).Get_String();
        ReportText = ReportText + (CStr) ReportText.New_Line().Get_String();
    }
    StoreLanguageToOpen("");
    BufString = "Output" + (CStr) StringNumberComplement(DateGetDay().Get_Long(), 2).Get_String();
    BufString = BufString + StringNumberComplement(DateGetMonth().Get_Long(), 2);
    BufString = BufString + StringNumberComplement(DateGetYear().Get_Long(), 2);
    BufString = BufString + StringNumberComplement(DateGetHour().Get_Long(), 2);
    BufString = BufString + StringNumberComplement(DateGetMinutes().Get_Long(), 2);
    BufString = BufString + ".log";
    NewChildHandle = CreateNewFile(BufString);

    if(NewChildHandle == 0) goto ErrorOpenOutput;
    SendTextToChild(NewChildHandle, ReportText);
    return;
ErrorOpenOutput:
    MiscMsgBox(hMDIform.hWnd, "Can't create new window.", MB_ERROR, Requesters);
}

// -----------------------------------------------------------------------
// Treeview control hook
LRESULT CALLBACK TreeViewHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HTREEITEM RightSelection;
    HTREEITEM PopSelection;
    RECT myPopRect;
    HMENU hPop;
    HTREEITEM PopTreeParent;
    CStr NodeTp;
    long CurrentParam = 0;
    HTREEITEM CurrentTreeItem = 0;
    HANDLE DefFileHandle = 0;
    CStr EntryToAdd;
    CStr BufString;
    HTREEITEM TreeChild = 0;
    POINT PopupPoint;
    
    switch(uMsg)
    {
        case WM_RBUTTONDBLCLK:
            return(0);
        case WM_LBUTTONDOWN:
            if(GetTreeviewExpand() == 0)
            {
                RightSelection = TreeViewGetItemFromPos(hTreeView, lParam & 0xFFFF, ((lParam & 0xFFFF0000) >> 16));
                if((RightSelection != (HTREEITEM) 0) && (RightSelection != (HTREEITEM) -1))
                {
                    TreeViewSetSelectedItem(hTreeView, RightSelection);
                }
            }
            break;
        case WM_RBUTTONDOWN:
            if(InContextMenu == 0)
            {
                RightSelection = TreeViewGetItemFromPos(hTreeView, lParam & 0xFFFF, ((lParam & 0xFFFF0000) >> 16));
                if((RightSelection != (HTREEITEM) 0) && (RightSelection != (HTREEITEM) -1))
                {
                    TreeViewSetSelectedItem(hTreeView, RightSelection);
                    SetFocus(hTreeView);
                }
            }
            return(0);
        case WM_CONTEXTMENU:
            if(TreeViewGetItemCount(hTreeView) != 0)
            {
                PopSelection = TreeViewGetSelectedItem(hTreeView);
                hPop = CreatePopupMenu();
                LastSelectTreePop = MENU_POP_NONE;
                // Parents menus
                if(PopSelection == hTreeViewRoot)
                {
                    MenuAddString(hPop, "Project properties...", MENU_POP_BASEID, TRUE);
                    MenuAddSeparator(hPop);
                    MenuAddString(hPop, "Assemble project\tShift+F5", MENU_POP_BASEID + 1, TRUE);
                    MenuAddString(hPop, "Run project\tCtrl+F5", MENU_POP_BASEID + 2, TRUE);
                    MenuAddString(hPop, "Debug project\tCtrl+Shift+F5", MENU_POP_BASEID + 3, TRUE);
                    MenuAddSeparator(hPop);
                    MenuAddString(hPop, "Run only", MENU_POP_BASEID + 4, TRUE);
                    MenuAddString(hPop, "Debug only", MENU_POP_BASEID + 5, TRUE);
                    LastSelectTreePop = MENU_POP_PROJECT;
                }
                else if(PopSelection == hTreeViewIncludes)
                {
                    MenuAddString(hPop, "Add new include file(s)...", MENU_POP_BASEID, TRUE);
                    MenuAddString(hPop, "Add current file", MENU_POP_BASEID + 1, TRUE);
                    MenuAddSeparator(hPop);
                    MenuAddString(hPop, "Open all include files", MENU_POP_BASEID + 2, TRUE);
                    LastSelectTreePop = MENU_POP_NEWINCLUDE;
                }
                else if(PopSelection == hTreeViewLibs)
                {
                    MenuAddString(hPop, "Add new library file(s)...", MENU_POP_BASEID, TRUE);
                    LastSelectTreePop = MENU_POP_NEWLIBRARY;
                }
                else if(PopSelection == hTreeViewModules)
                {
                    MenuAddString(hPop, "Add new module file(s)...", MENU_POP_BASEID, TRUE);
                    MenuAddString(hPop, "Add current file", MENU_POP_BASEID + 1, TRUE);
                    MenuAddSeparator(hPop);
                    MenuAddString(hPop, "Open all module files", MENU_POP_BASEID + 2, TRUE);
                    LastSelectTreePop = MENU_POP_NEWMODULE;
                }
                else if(PopSelection == hTreeViewObjects)
                {
                    MenuAddString(hPop, "Add new object file(s)...", MENU_POP_BASEID, TRUE);
                    LastSelectTreePop = MENU_POP_NEWOBJECT;
                }
                else if(PopSelection == hTreeViewResources)
                {
                    MenuAddString(hPop, "Add new resource file(s)...", MENU_POP_BASEID, TRUE);
                    MenuAddString(hPop, "Add current file", MENU_POP_BASEID + 1, TRUE);
                    MenuAddSeparator(hPop);
                    MenuAddString(hPop, "Open all resource files", MENU_POP_BASEID + 2, TRUE);
                    LastSelectTreePop = MENU_POP_NEWRESOURCE;
                }
                else if(PopSelection == hTreeViewDefs)
                {
                    if(TreeViewGetFirstItemChild(hTreeView, hTreeViewDefs) == 0)
                    {
                        MenuAddString(hPop, "Create new exports file", MENU_POP_BASEID, TRUE);
                        LastSelectTreePop = MENU_POP_NEWDEF;
                    }
                    else
                    {
                        return(0);
                    }
                }
                else if(PopSelection == hTreeViewTexts)
                {
                    MenuAddString(hPop, "Add new document file(s)...", MENU_POP_BASEID, TRUE);
                    MenuAddString(hPop, "Add current file", MENU_POP_BASEID + 1, TRUE);
                    LastSelectTreePop = MENU_POP_NEWTEXT;
                }
                if(LastSelectTreePop == MENU_POP_NONE)
                {
                    // Childs menus
                    PopTreeParent = TreeViewGetItemParent(hTreeView, PopSelection);
                    if(PopTreeParent == hTreeViewIncludes)
                    {
                        NodeTp = "include file";
                        LastSelectTreePop = MENU_POP_INCLUDEDETAILS;
                        BufString = "Open " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 1, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewLibs)
                    {
                        NodeTp = "library file";
                        LastSelectTreePop = MENU_POP_LIBRARYDETAILS;
                        BufString = "Add new " + (CStr)  NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 1, TRUE);
                        MenuAddSeparator(hPop);
                        MenuAddString(hPop, "Generate prototypes", MENU_POP_BASEID + 2, TRUE);
                        MenuSetDefaultItem(hPop, 3);
                    }
                    else if(PopTreeParent == hTreeViewModules)
                    {
                        NodeTp = "module file";
                        LastSelectTreePop = MENU_POP_MODULEDETAILS;
                        BufString = "Open " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 1, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewObjects)
                    {
                        NodeTp = "object file";
                        LastSelectTreePop = MENU_POP_OBJECTDETAILS;
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 1, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewResources)
                    {
                        // New resource sub directories
                        if(PopSelection == hTreeViewIcons)
                        {
                            MenuAddString(hPop, "Add new icon file(s)...", MENU_POP_BASEID, TRUE);
                            LastSelectTreePop = MENU_POP_NEWICON;
                        }
                        else if(PopSelection == hTreeViewCursors)
                        {
                            MenuAddString(hPop, "Add new cursor file(s)...", MENU_POP_BASEID, TRUE);
                            LastSelectTreePop = MENU_POP_NEWCURSOR;
                        }
                        else if(PopSelection == hTreeViewBitmaps)
                        {
                            MenuAddString(hPop, "Add new bitmap file(s)...", MENU_POP_BASEID, TRUE);
                            LastSelectTreePop = MENU_POP_NEWBITMAP;
                        }
                        else if(PopSelection == hTreeViewStrings)
                        {
                            MenuAddString(hPop, "Create new strings", MENU_POP_BASEID, TRUE);
                            MenuAddString(hPop, "Add new strings file(s)...", MENU_POP_BASEID + 1, TRUE);
                            LastSelectTreePop = MENU_POP_NEWSTRINGS;
                        }
                        else if(PopSelection == hTreeViewAccelerators)
                        {
                            MenuAddString(hPop, "Create new accelerators", MENU_POP_BASEID, TRUE);
                            MenuAddString(hPop, "Add new accelerators file(s)...", MENU_POP_BASEID + 1, TRUE);
                            LastSelectTreePop = MENU_POP_NEWACCELERATORS;
                        }
                        else if(PopSelection == hTreeViewMenus)
                        {
                            MenuAddString(hPop, "Create new menus", MENU_POP_BASEID, TRUE);
                            MenuAddString(hPop, "Add new menus file(s)...", MENU_POP_BASEID + 1, TRUE);
                            LastSelectTreePop = MENU_POP_NEWMENUS;
                        }
                        else if(PopSelection == hTreeViewDialogs)
                        {
                            MenuAddString(hPop, "Create new dialog", MENU_POP_BASEID, TRUE);
                            MenuAddString(hPop, "Add new dialog file(s)...", MENU_POP_BASEID + 1, TRUE);
                            LastSelectTreePop = MENU_POP_NEWDIALOG;
                        }
                        else if(PopSelection == hTreeViewRawdatas)
                        {
                            MenuAddString(hPop, "Add new raw datas file(s)...", MENU_POP_BASEID, TRUE);
                            LastSelectTreePop = MENU_POP_NEWRAWDATA;
                        }
                        else
                        {
                            // Old ones
                            NodeTp = "resource file";
                            LastSelectTreePop = MENU_POP_RESOURCEDETAILS;
                            BufString = "Open " + (CStr) NodeTp;
                            MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                            MenuSetDefaultItem(hPop, 0);
                            MenuAddSeparator(hPop);
                            BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                            MenuAddString(hPop, BufString, MENU_POP_BASEID + 1, TRUE);
                            BufString = "Remove " + (CStr) NodeTp;
                            MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        }
                    }
                    else if(PopTreeParent == hTreeViewIcons)
                    {
                        NodeTp = "icon resource";
                        LastSelectTreePop = MENU_POP_ICONDETAILS;
                        BufString = "Open " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddString(hPop, "Icon properties", MENU_POP_BASEID + 1, TRUE);
                        BufString = "Convert " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 4, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewCursors)
                    {
                        NodeTp = "cursor resource";
                        LastSelectTreePop = MENU_POP_CURSORDETAILS;
                        BufString = "Open " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddString(hPop, "Cursor properties", MENU_POP_BASEID + 1, TRUE);
                        BufString = "Convert " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 4, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewBitmaps)
                    {
                        NodeTp = "bitmap resource";
                        LastSelectTreePop = MENU_POP_BITMAPDETAILS;
                        BufString = "Open " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddString(hPop, "Bitmap properties", MENU_POP_BASEID + 1, TRUE);
                        BufString = "Convert " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 4, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewStrings)
                    {
                        NodeTp = "strings resource";
                        LastSelectTreePop = MENU_POP_STRINGDETAILS;
                        BufString = "Edit " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddString(hPop, "Strings properties", MENU_POP_BASEID + 1, TRUE);
                        BufString = "Convert " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 4, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewAccelerators)
                    {
                        NodeTp = "accelerators resource";
                        LastSelectTreePop = MENU_POP_ACCELERATORDETAILS;
                        BufString = "Edit " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddString(hPop, "Accelerators properties", MENU_POP_BASEID + 1, TRUE);
                        BufString = "Convert " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 4, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewMenus)
                    {
                        NodeTp = "menus resource";
                        LastSelectTreePop = MENU_POP_MENUDETAILS;
                        BufString = "Edit " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddString(hPop, "Menus properties", MENU_POP_BASEID + 1, TRUE);
                        BufString = "Convert " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 4, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewDialogs)
                    {
                        NodeTp = "dialog resource";
                        LastSelectTreePop = MENU_POP_DIALOGDETAILS;
                        BufString = "Edit " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        MenuAddString(hPop, "Dialog properties", MENU_POP_BASEID + 1, TRUE);
                        BufString = "Convert " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 4, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewRawdatas)
                    {
                        NodeTp = "raw datas file";
                        LastSelectTreePop = MENU_POP_RAWDATADETAILS;
                        MenuAddString(hPop, "Raw datas properties", MENU_POP_BASEID, TRUE);
                        BufString = "Convert " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 1, TRUE);
                        MenuSetDefaultItem(hPop, 1);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                    }
                    else if(PopTreeParent == hTreeViewDefs)
                    {
                        NodeTp = "exports file";
                        LastSelectTreePop = MENU_POP_DEFDETAILS;
                        BufString = "Edit " + (CStr) NodeTp + (CStr) "...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                    }
                    else if(PopTreeParent == hTreeViewTexts)
                    {
                        NodeTp = "document file";
                        LastSelectTreePop = MENU_POP_TEXTDETAILS;
                        BufString = "View " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID, TRUE);
                        MenuSetDefaultItem(hPop, 0);
                        BufString = "Execute " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 1, TRUE);
                        MenuAddSeparator(hPop);
                        BufString = "Add new " + (CStr) NodeTp + (CStr) "(s)...";
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 2, TRUE);
                        BufString = "Remove " + (CStr) NodeTp;
                        MenuAddString(hPop, BufString, MENU_POP_BASEID + 3, TRUE);
                    }
                }
                InContextMenu = 1;
                GetCursorPos(&PopupPoint);
                TrackPopupMenu(hPop, TPM_LEFTALIGN + TPM_LEFTBUTTON, PopupPoint.x, PopupPoint.y, 0, hWnd, &myPopRect);
            }
            break;
        // --- Contextual menu selections
        case WM_MENUSELECT:
            // Process user menus
            if((wParam & 0x8000) == 0)
            {
                if((wParam & 0x7FFF) >= MENU_POP_BASEID)
                {
                    CurrentParam = (wParam & 0x7FFF) - MENU_POP_BASEID;
                    switch(LastSelectTreePop)
                    {
                        case MENU_POP_PROJECT:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_PROJECTPROPERTIES_ID]);
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ASSEMBLEPROJECT_ID]);
                                    break;
                                case 2:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_RUNPROJECT_ID]);
                                    break;
                                case 3:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_DEBUGPROJECT_ID]);
                                    break;
                                case 4:
                                    WriteComment("Run project without compilation");
                                    break;
                                case 5:
                                    WriteComment("Debug project without compilation");
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWINCLUDE:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDINCLUDE_ID]);
                                    break;
                                case 1:
                                    WriteComment("Add current opened file to includes entries");
                                    break;
                                case 2:
                                    WriteComment("Open all includes entries");
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWLIBRARY:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDLIBRARY_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWMODULE:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDMODULE_ID]);
                                    break;
                                case 1:
                                    WriteComment("Add current opened file to modules entries");
                                    break;
                                case 2:
                                    WriteComment("Open all modules entries");
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWOBJECT:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDOBJECT_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWRESOURCE:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDRESOURCE_ID]);
                                    break;
                                case 1:
                                    WriteComment("Add current opened file to resources entries");
                                    break;
                                case 2:
                                    WriteComment("Open all resources entries");
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWDEF:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Create exported procedures file");
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWTEXT:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDTEXT_ID]);
                                    break;
                                case 1:
                                    WriteComment("Add current opened file to documents entries");
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWICON:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDICON_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWCURSOR:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDCURSOR_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWBITMAP:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDBITMAP_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWSTRINGS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Create new strings resource");
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDSTRINGS_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWACCELERATORS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Create new accelerators resource");
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDACCELERATORS_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWMENUS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Create new menus resource");
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDMENUS_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWDIALOG:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Create new dialog resource");
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDDIALOG_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWRAWDATA:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDRAWDATA_ID]);
                                    break;
                            }
                            return(0);
                        case MENU_POP_INCLUDEDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Open selected include file");
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDINCLUDE_ID]);
                                    break;
                                case 2:
                                    WriteComment("Remove include file from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_LIBRARYDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDLIBRARY_ID]);
                                    break;
                                case 1:
                                    WriteComment("Remove library file from current project");
                                    break;
                                case 2:
                                    WriteComment("Generate prototypes for selected library file");
                                    break;
                            }
                            return(0);
                        case MENU_POP_MODULEDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Open selected module file");
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDMODULE_ID]);
                                    break;
                                case 2:
                                    WriteComment("Remove module file from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_OBJECTDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDOBJECT_ID]);
                                    break;
                                case 1:
                                    WriteComment("Remove object file from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_RESOURCEDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Open current resource file");
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDRESOURCE_ID]);
                                    break;
                                case 2:
                                    WriteComment("Remove resource file from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_ICONDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Display selected icon resource");
                                    break;
                                case 1:
                                    WriteComment("Edit icon resource properties");
                                    break;
                                case 2:
                                    WriteComment("Convert icon resource to rc file");
                                    break;
                                case 3:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDICON_ID]);
                                    break;
                                case 4:
                                    WriteComment("Remove icon resource from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_CURSORDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Display selected cursor resource");
                                    break;
                                case 1:
                                    WriteComment("Edit cursor resource properties");
                                    break;
                                case 2:
                                    WriteComment("Convert cursor resource to rc file");
                                    break;
                                case 3:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDCURSOR_ID]);
                                    break;
                                case 4:
                                    WriteComment("Remove cursor resource from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_BITMAPDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Display selected bitmap resource");
                                    break;
                                case 1:
                                    WriteComment("Edit bitmap resource properties");
                                    break;
                                case 2:
                                    WriteComment("Convert bitmap resource to rc file");
                                    break;
                                case 3:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDBITMAP_ID]);
                                    break;
                                case 4:
                                    WriteComment("Remove bitmap resource from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_STRINGDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Edit selected strings resource");
                                    break;
                                case 1:
                                    WriteComment("Edit strings resource properties");
                                    break;
                                case 2:
                                    WriteComment("Convert strings resource to rc file");
                                    break;
                                case 3:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDSTRINGS_ID]);
                                    break;
                                case 4:
                                    WriteComment("Remove strings resource from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_ACCELERATORDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Edit selected accelerators resource");
                                    break;
                                case 1:
                                    WriteComment("Edit accelerators resource properties");
                                    break;
                                case 2:
                                    WriteComment("Convert accelerators resource to rc file");
                                    break;
                                case 3:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDACCELERATORS_ID]);
                                    break;
                                case 4:
                                    WriteComment("Remove accelerators resource from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_MENUDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Edit selected menus resource");
                                    break;
                                case 1:
                                    WriteComment("Edit menus resource properties");
                                    break;
                                case 2:
                                    WriteComment("Convert menus resource to rc file");
                                    break;
                                case 3:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDMENUS_ID]);
                                    break;
                                case 4:
                                    WriteComment("Remove menus resource from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_DIALOGDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Edit selected dialog resource");
                                    break;
                                case 1:
                                    WriteComment("Edit dialog resource properties");
                                    break;
                                case 2:
                                    WriteComment("Convert dialog resource to rc file");
                                    break;
                                case 3:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDDIALOG_ID]);
                                    break;
                                case 4:
                                    WriteComment("Remove dialog resource from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_RAWDATADETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Edit raw datas resource properties");
                                    break;
                                case 1:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDRAWDATA_ID]);
                                    break;
                                case 2:
                                    WriteComment("Convert selected raw datas resource to rc file");
                                    break;
                                case 3:
                                    WriteComment("Remove raw datas resource from current project");
                                    break;
                            }
                            return(0);
                        case MENU_POP_DEFDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Edit exported procedures list");
                                    break;
                            }
                            return(0);
                        case MENU_POP_TEXTDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    WriteComment("Open selected document file in " + (CStr) AppTitle);
                                    break;
                                case 1:
                                    WriteComment("Open selected document file with default application");
                                    break;
                                case 2:
                                    WriteComment(MenuProjectComments[MENU_PROJECT_ADDTEXT_ID]);
                                    break;
                                case 3:
                                    WriteComment("Remove document file from current project");
                                    break;
                            }
                            return(0);
                    }
                    WriteComment("");
                    return(0);
                }
                else
                {
                    WriteComment("");
                    return(0);
                }
            }
            else
            {
                WriteComment("");
                return(0);
            }
            break;
        case WM_EXITMENULOOP:
            InContextMenu = 0;
            WriteComment("");
            return(0);
        case WM_COMMAND:
            // Process user menus
            if((wParam & 0x8000) == 0)
            {
                if((wParam & 0x7FFF) >= MENU_POP_BASEID)
                {
                    CurrentParam = (wParam & 0x7FFF) - MENU_POP_BASEID;
                    CurrentTreeItem = TreeViewGetSelectedItem(hTreeView);
                    switch(LastSelectTreePop)
                    {
                        case MENU_POP_PROJECT:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_ProjectProperties();
                                    SetFocus(hTreeView);
                                    break;
                                case 1:
                                    MCMD_AssembleProject();
                                    break;
                                case 2:
                                    MCMD_RunProject();
                                    break;
                                case 3:
                                    MCMD_DebugProject();
                                    break;
                                case 4:
                                    RunProject();
                                    break;
                                case 5:
                                    DebugProject();
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWINCLUDE:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddInclude();
                                    SetFocus(hTreeView);
                                    break;
                                case 1:
                                    MCMD_AddCurrentInclude();
                                    SetFocus(hTreeView);
                                    break;
                                case 2:
                                    // Open all includes
                                    TreeChild = TreeViewGetFirstItemChild(hTreeView,CurrentTreeItem);
                                    while(TreeChild != 0)
                                    {
                                        FocusFile(GetFileNameFromTreeView(TreeChild), 1);
                                        TreeChild = TreeViewGetNextItem(hTreeView, TreeChild);
                                    }
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWLIBRARY:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddLibrary();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWMODULE:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddModule();
                                    SetFocus(hTreeView);
                                    break;
                                case 1:
                                    MCMD_AddCurrentModule();
                                    SetFocus(hTreeView);
                                    break;
                                case 2:
                                    // Open all modules
                                    TreeChild = TreeViewGetFirstItemChild(hTreeView,CurrentTreeItem);
                                    while(TreeChild != 0)
                                    {
                                        FocusFile(GetFileNameFromTreeView(TreeChild), 1);
                                        TreeChild = TreeViewGetNextItem(hTreeView, TreeChild);
                                    }
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWOBJECT:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddObject();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWRESOURCE:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddResource();
                                    SetFocus(hTreeView);
                                    break;
                                case 1:
                                    MCMD_AddCurrentRc();
                                    SetFocus(hTreeView);
                                    break;
                                case 2:
                                    // Open all resources files
                                    TreeChild = TreeViewGetFirstItemChild(hTreeView,CurrentTreeItem);
                                    while(TreeChild != 0)
                                    {
                                        if((TreeChild != hTreeViewIcons) &&
                                            (TreeChild != hTreeViewCursors) &&
                                            (TreeChild != hTreeViewBitmaps) &&
                                            (TreeChild != hTreeViewStrings) &&
                                            (TreeChild != hTreeViewAccelerators) &&
                                            (TreeChild != hTreeViewMenus) &&
                                            (TreeChild != hTreeViewDialogs) &&
                                            (TreeChild != hTreeViewRawdatas))
                                        {
                                                FocusFile(GetFileNameFromTreeView(TreeChild), 1);
                                        }
                                        TreeChild = TreeViewGetNextItem(hTreeView, TreeChild);
                                    }
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWDEF:
                            switch(CurrentParam)
                            {
                                case 0:
                                    BufString = FileGetDirectory(ProjectFName);
                                    BufString = BufString + ProjectTitle + (CStr) ".mdf";
                                    DefFileHandle = FileCreateEmpty(BufString, NO_SECURITY_DESCRIPTOR);
                                    FileWriteLine(DefFileHandle, "; Chromatic exports file");
                                    FileWriteLine(DefFileHandle, "; Do NOT edit this file manually");
                                    FileWriteLine(DefFileHandle, "; unless you know what you're doing !!!");
                                    FileWriteLine(DefFileHandle, "; (C) 2001-2010 Franck Charlet.");
                                    FileWriteLine(DefFileHandle, "[EXPORTS]");
                                    FileClose(DefFileHandle);
                                    EntryToAdd = ProjectTitle + (CStr) ".mdf (" + (CStr) FileGetDirectory(ProjectFName).Get_String() + (CStr) ProjectTitle + (CStr) ".mdf" + (CStr) ")";
                                    TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewDefs, 0, ICON_NEW, ICON_NEW, 0, 1);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWTEXT:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddText();
                                    SetFocus(hTreeView);
                                    break;
                                case 1:
                                    MCMD_AddCurrentText();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWICON:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddIcon();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWCURSOR:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddCursor();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWBITMAP:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddBitmap();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWSTRINGS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    CreateStrings();
                                    break;
                                case 1:
                                    MCMD_AddStrings();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWACCELERATORS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    CreateAccelerators();
                                    break;
                                case 1:
                                    MCMD_AddAccelerators();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWMENUS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    CreateMenus();
                                    break;
                                case 1:
                                    MCMD_AddMenus();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWDIALOG:
                            switch(CurrentParam)
                            {
                                case 0:
                                    CreateUserDialog();
                                    break;
                                case 1:
                                    MCMD_AddDialog();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_NEWRAWDATA:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddRawDatas();
                                    SetFocus(hTreeView);
                                    break;
                            }
                            return(0);
                        case MENU_POP_INCLUDEDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                                    break;
                                case 1:
                                    MCMD_AddInclude();
                                    SetFocus(hTreeView);
                                    break;
                                case 2:
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_LIBRARYDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddLibrary();
                                    SetFocus(hTreeView);
                                    break;
                                case 1:
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                                case 2:
                                    GenProto(GetFileNameFromTreeView(CurrentTreeItem));
                                    break;
                            }
                            return(0);
                        case MENU_POP_MODULEDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                                    break;
                                case 1:
                                    MCMD_AddModule();
                                    SetFocus(hTreeView);
                                    break;
                                case 2:
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_OBJECTDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    MCMD_AddObject();
                                    SetFocus(hTreeView);
                                    break;
                                case 1:
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_RESOURCEDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                                    break;
                                case 1:
                                    MCMD_AddResource();
                                    SetFocus(hTreeView);
                                    break;
                                case 2:
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_ICONDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    DisplayImageView(VIEW_ICON, GetFileNameFromTreeView(CurrentTreeItem));
                                    break;
                                case 1:
                                    DisplayResProp(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)), RESPROP_ICON, GetLabelFromTreeView(CurrentTreeItem), CurrentTreeItem);
                                    break;
                                case 2:
                                    Convert_Icon(GetFileNameFromTreeView(CurrentTreeItem), GetLabelFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    break;
                                case 3:
                                    MCMD_AddIcon();
                                    break;
                                case 4:
                                    RemIconFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_CURSORDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    DisplayImageView(VIEW_CURSOR, GetFileNameFromTreeView(CurrentTreeItem));
                                    break;
                                case 1:
                                    DisplayResProp(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)), RESPROP_CURSOR, GetLabelFromTreeView(CurrentTreeItem), CurrentTreeItem);
                                    break;
                                case 2:
                                    Convert_Cursor(GetFileNameFromTreeView(CurrentTreeItem), GetLabelFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    break;
                                case 3:
                                    MCMD_AddCursor();
                                    break;
                                case 4:
                                    RemCursorFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_BITMAPDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    DisplayImageView(VIEW_BITMAP, GetFileNameFromTreeView(CurrentTreeItem));
                                    break;
                                case 1:
                                    DisplayResProp(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)), RESPROP_BITMAP, GetLabelFromTreeView(CurrentTreeItem), CurrentTreeItem);
                                    break;
                                case 2:
                                    Convert_Bitmap(GetFileNameFromTreeView(CurrentTreeItem), GetLabelFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    break;
                                case 3:
                                    MCMD_AddBitmap();
                                    break;
                                case 4:
                                    RemBitmapFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_STRINGDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    EditStringsTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, hTreeViewStrings));
                                    break;
                                case 1:
                                    DisplayResProp(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)), RESPROP_STRINGS, GetLabelFromTreeView(CurrentTreeItem), CurrentTreeItem);
                                    break;
                                case 2:
                                    Convert_Strings(GetFileNameFromTreeView(CurrentTreeItem), GetLabelFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    break;
                                case 3:
                                    MCMD_AddStrings();
                                    break;
                                case 4:
                                    RemStringFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_ACCELERATORDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    EditAcceleratorsTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem));
                                    break;
                                case 1:
                                    DisplayResProp(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)), RESPROP_ACCELERATORS, GetLabelFromTreeView(CurrentTreeItem), CurrentTreeItem);
                                    break;
                                case 2:
                                    Convert_Accelerators(GetFileNameFromTreeView(CurrentTreeItem), GetLabelFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    break;
                                case 3:
                                    MCMD_AddAccelerators();
                                    break;
                                case 4:
                                    RemAcceleratorFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_MENUDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    EditMenusTable(GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem));
                                    break;
                                case 1:
                                    DisplayResProp(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)), RESPROP_MENUS, GetLabelFromTreeView(CurrentTreeItem), CurrentTreeItem);
                                    break;
                                case 2:
                                    Convert_Menus(GetFileNameFromTreeView(CurrentTreeItem), GetLabelFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    break;
                                case 3:
                                    MCMD_AddMenus();
                                    break;
                                case 4:
                                    RemMenuFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_DIALOGDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    // TODO
                                    break;
                                case 1:
                                    // TODO
                                    break;
                                case 2:
                                    // TODO
                                    break;
                                case 3:
                                    MCMD_AddDialog();
                                    break;
                                case 4:
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_RAWDATADETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    DisplayResProp(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)), RESPROP_RAWDATA, GetLabelFromTreeView(CurrentTreeItem), CurrentTreeItem);
                                    break;
                                case 1:
                                    OpenFileAsRC(0, GetLabelFromTreeView(CurrentTreeItem), GetFileNameFromTreeView(CurrentTreeItem), TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    break;
                                case 2:
                                    MCMD_AddRawDatas();
                                    break;
                                case 3:
                                    RemRawDataFromArray(TreeViewGetSelChildItemPos(hTreeView, CurrentTreeItem, TreeViewGetItemParent(hTreeView, CurrentTreeItem)));
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                        case MENU_POP_DEFDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    DisplayDefForm(GetFileNameFromTreeView(CurrentTreeItem));
                                    break;
                            }
                            return(0);
                        case MENU_POP_TEXTDETAILS:
                            switch(CurrentParam)
                            {
                                case 0:
                                    FocusFile(GetFileNameFromTreeView(CurrentTreeItem), 1);
                                    break;
                                case 1:
                                    ShellExecute(hMDIform.hWnd, "open", GetFileNameFromTreeView(CurrentTreeItem).Get_String(), "", FileGetDirectory(GetFileNameFromTreeView(CurrentTreeItem)).Get_String(), SW_SHOW);
                                    break;
                                case 2:
                                    MCMD_AddText();
                                    SetFocus(hTreeView);
                                    break;
                                case 3:
                                    TreeViewRemoveItem(hTreeView, CurrentTreeItem);
                                    ProjectModified = TRUE;
                                    break;
                            }
                            return(0);
                    }
                }
            }
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Status listbox control hook
LRESULT CALLBACK StatusBoxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HMENU hPop = 0;
    long wParamSelOB = 0;
    long SelectedListItem = 0;
    POINT PopupPoint;

    switch(uMsg)
    {
        case WM_LBUTTONDBLCLK:
            JumpToErrorFromOutBox();
            return(0);
        case WM_CONTEXTMENU:
            if(ListBoxCount(hWnd) != 0)
            {
                SelectedListItem = ListBoxItemUnderCursor(hWnd);
                if(SelectedListItem != -1)
                {
                    ListBoxSetIndex(hWnd, SelectedListItem);
                    // Create Contextual menu
                    hPop = CreatePopupMenu();
                    MenuAddString(hPop, "Save content to file", MENU_OUTBOX_SAVE_ID + MENU_OUTBOX_IDBASE, TRUE);
                    MenuAddString(hPop, "Send content to window", MENU_OUTBOX_NEW_ID + MENU_OUTBOX_IDBASE, TRUE);
                    MenuAddString(hPop, "Empty output", MENU_OUTBOX_EMPTY_ID + MENU_OUTBOX_IDBASE, TRUE);
                    if(ListBoxGetCurrentItem(hWnd).Len() != 0)
                    {
                        MenuAddSeparator(hPop);
                        MenuAddString(hPop, "Go to error/warning", MENU_OUTBOX_GOTO_ID + MENU_OUTBOX_IDBASE, TRUE);
                    }
                    GetCursorPos(&PopupPoint);
                    TrackPopupMenu(hPop, TPM_LEFTALIGN + TPM_LEFTBUTTON, PopupPoint.x, PopupPoint.y, 0, hWnd, NULL);
                }
            }
            return(0);
        case WM_MENUSELECT:
            wParamSelOB = (wParam & 0x7FFF);
            switch(wParamSelOB)
            {
                case MENU_OUTBOX_SAVE_ID + MENU_OUTBOX_IDBASE:
                    WriteComment("Save content to file");
                    break;
                case MENU_OUTBOX_NEW_ID + MENU_OUTBOX_IDBASE:
                    WriteComment("Create new window with content");
                    break;
                case MENU_OUTBOX_EMPTY_ID + MENU_OUTBOX_IDBASE:
                    WriteComment("Empty output window");
                    break;
                case MENU_OUTBOX_GOTO_ID + MENU_OUTBOX_IDBASE:
                    WriteComment("Go to referenced error/warning");
                    break;
                default:
                    WriteComment("");
                    break;
            }
            return(0);
        case WM_EXITMENULOOP:
            WriteComment("");
            return(0);
        case WM_COMMAND:
            // Contextual menu commands
            switch(wParam)
            {
                case MENU_OUTBOX_SAVE_ID + MENU_OUTBOX_IDBASE:
                    SaveBuildReport(hStatusBox);
                    break;
                case MENU_OUTBOX_NEW_ID + MENU_OUTBOX_IDBASE:
                    CreateBuildReport(hStatusBox);
                    break;
                case MENU_OUTBOX_EMPTY_ID + MENU_OUTBOX_IDBASE:
                    ClearStatus();
                    break;
                case MENU_OUTBOX_GOTO_ID + MENU_OUTBOX_IDBASE:
                    JumpToErrorFromOutBox();
                    break;
            }
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Debug listbox control hook
LRESULT CALLBACK DebugBoxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HMENU hPop = 0;
    long wParamSelOB = 0;
    long SelectedListItem = 0;
    POINT PopupPoint;

    switch(uMsg)
    {
        case WM_CONTEXTMENU:
            if(ListBoxCount(hWnd) != 0)
            {
                SelectedListItem = ListBoxItemUnderCursor(hWnd);
                if(SelectedListItem != -1)
                {
                    ListBoxSetIndex(hWnd, SelectedListItem);
                    // Create Contextual menu
                    hPop = CreatePopupMenu();
                    MenuAddString(hPop, "Save content to file", MENU_DEBUGBOX_SAVE_ID + MENU_DEBUGBOX_IDBASE, TRUE);
                    MenuAddString(hPop, "Send content to window", MENU_DEBUGBOX_NEW_ID + MENU_DEBUGBOX_IDBASE, TRUE);
                    MenuAddString(hPop, "Empty debug window", MENU_DEBUGBOX_EMPTY_ID + MENU_DEBUGBOX_IDBASE, TRUE);
                    GetCursorPos(&PopupPoint);
                    TrackPopupMenu(hPop, TPM_LEFTALIGN + TPM_LEFTBUTTON, PopupPoint.x, PopupPoint.y, 0, hWnd, NULL);
                }
            }
            return(0);
        case WM_MENUSELECT:
            wParamSelOB = (wParam & 0x7FFF);
            switch(wParamSelOB)
            {
                case MENU_DEBUGBOX_SAVE_ID + MENU_DEBUGBOX_IDBASE:
                    WriteComment("Save content to file");
                    break;
                case MENU_DEBUGBOX_NEW_ID + MENU_DEBUGBOX_IDBASE:
                    WriteComment("Create new window with content");
                    break;
                case MENU_DEBUGBOX_EMPTY_ID + MENU_DEBUGBOX_IDBASE:
                    WriteComment("Empty debug window");
                    break;
                default:
                    WriteComment("");
                    break;
            }
            return(0);
        case WM_EXITMENULOOP:
            WriteComment("");
            return(0);
        case WM_COMMAND:
            // Contextual menu commands
            switch(wParam)
            {
                case MENU_DEBUGBOX_SAVE_ID + MENU_DEBUGBOX_IDBASE:
                    SaveBuildReport(hDebugBox);
                    break;
                case MENU_DEBUGBOX_NEW_ID + MENU_DEBUGBOX_IDBASE:
                    CreateBuildReport(hDebugBox);
                    break;
                case MENU_DEBUGBOX_EMPTY_ID + MENU_DEBUGBOX_IDBASE:
                    ClearDebug();
                    break;
            }
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Set the tooltips of the treeview
void SetTreeviewTT(void)
{
    CStr TreeValue;

    TreeValue = IniReadKey("Layout", "TVToolTips", MainIniFile);
    if(TreeValue.Len() == 0) TreeValue = "0";
    switch(TreeValue.Get_Long())
    {
        case 0:
            OldTreeViewToolTips = TreeViewSetToolTips(hTreeView, 0);
            TreeViewSetToolTips(hTreeViewDebug, 0);
            break;
        case 1:
            TreeViewSetToolTips(hTreeView, OldTreeViewToolTips);
            TreeViewSetToolTips(hTreeViewDebug, OldTreeViewToolTips);
            break;
    }
}

// -----------------------------------------------------------------------
// Set the expanding type of the treeview's items
void SetTreeviewExpand(void)
{
    CStr TreeValue;

    TreeValue = IniReadKey("Layout", "TVSingle", MainIniFile);
    if(TreeValue.Len() == 0) TreeValue = "0";
    switch(TreeValue.Get_Long())
    {
        case 0:
            TreeViewSetSingleExpandState(hTreeView, 0);
            TreeViewSetSingleExpandState(hTreeViewDebug, 0);
            break;
        case 1:
            TreeViewSetSingleExpandState(hTreeView, 1);
            TreeViewSetSingleExpandState(hTreeViewDebug, 1);
            break;
    }
}

// -----------------------------------------------------------------------
// Set the expanding type of the treeview's items
long GetTreeviewExpand(void)
{
    CStr TreeValue;

    TreeValue = IniReadKey("Layout", "TVSingle", MainIniFile);
    if(TreeValue.Len() == 0) TreeValue = "0";
    switch(TreeValue.Get_Long())
    {
        case 1:
            return(1);
        default:
            return(0);
    }
}

// -----------------------------------------------------------------------
// Jump to referenced line/file error
void JumpToErrorFromOutBox(void)
{
    CStr StatusLine;
    long PosAsm = 0;
    long PosAsmStrip = 0;
    long LineAsm = 0;
    long LineNumber = 0;
    long DoLCC32 = 0;
    CStr LCC32Dir;
    CStr BufString;
    CStr TakeLine;
    CStr FError;
    CStr LCC32LineNumber;
    int Pos_Error;

    if(ListBoxGetSelItemIndex(hStatusBox) != -1)
    {
        StatusLine = ListBoxGetItem(hStatusBox, ListBoxGetSelItemIndex(hStatusBox));
        // Check for a reported error in line
        Pos_Error = StatusLine.In_Str(1, "error", Text_Compare);
        if(Pos_Error)
        {
            PosAsm = StatusLine.In_Str(1, "(");
            if(Pos_Error < PosAsm) goto TestLCC32Error;
            if(PosAsm != 0)
            {
                PosAsm++;
                LineAsm = StatusLine.In_Str(PosAsm, ")");
                LineNumber = StatusLine.Mid(PosAsm, LineAsm - PosAsm).Get_Long();
                PosAsm = PosAsm - 2;
                TakeLine = StatusLine.Replace("**Error**", "", 1, -1, Binary_Compare);
                TakeLine = TakeLine.Trim();
                PosAsm = TakeLine.In_Str(1, "(");
                FError = TakeLine.Mid(1, PosAsm - 1);
                FError = FError.Trim();
                if(GoToLine(FError, LineNumber - 1, 1) == 0)
                {
                    BufString = CurrentDir + (CStr) "\\" + (CStr) FError;
                    if(GoToLine(BufString, LineNumber - 1, 1) == 0 && ProjectOn == 1)
                    {
                        // Load it from project
                        BufString = ProjectDir + (CStr) "\\" + (CStr) FError;
                        GoToLine(BufString, LineNumber - 1, 1);
                    }
                }
            } else {
TestLCC32Error: // LCC32
                DoLCC32 = 0;
                if(_strcmpi(StatusLine.Left(6).Get_String(), "error ") == 0)
                {
                    DoLCC32++;
                    // Format it to be GNU compatible
                    StatusLine = StatusLine.Mid(7);
                }
                else if(_strcmpi(StatusLine.Left(8).Get_String(), "warning ") == 0)
                {
                    DoLCC32++;
                    // Format it to be GNU compatible
                    StatusLine = StatusLine.Mid(9);
                }
                if(DoLCC32)
                {
                    PosAsm = StatusLine.In_Str(3, ":");
                    PosAsmStrip = StatusLine.In_Str(PosAsm + 1, ":");
                    LCC32Dir = StatusLine.Left(PosAsm);
                    LCC32Dir = LCC32Dir.Extract_Directory();
                    PosAsm++;
                    LCC32Dir = LCC32Dir + (CStr) StatusLine.Mid(PosAsm, PosAsmStrip - PosAsm).Trim();
                    PosAsmStrip++;
                    // Pass blank chars
                    while(!isdigit(StatusLine.Mid(PosAsmStrip, 1).Asc()) && (StatusLine.Mid(PosAsmStrip, 1).Len() != 0) )
                    {
                        PosAsmStrip++;
                    }
                    while(isdigit(StatusLine.Mid(PosAsmStrip, 1).Asc()))
                    {
                        LCC32LineNumber = LCC32LineNumber + StatusLine.Mid(PosAsmStrip, 1);
                        PosAsmStrip++;
                    }
                    if(GoToLine(LCC32Dir, LCC32LineNumber.Get_Long() - 1, 1) == 0)
                    {
                        BufString = CurrentDir + (CStr) "\\" + (CStr) LCC32Dir;
                        if(GoToLine(BufString, LCC32LineNumber.Get_Long() - 1, 1) == 0 && ProjectOn == 1)
                        {
                            // Load it from project
                            BufString = ProjectDir + (CStr) "\\" + (CStr) LCC32Dir;
                            GoToLine(BufString, LCC32LineNumber.Get_Long() - 1, 1);
                        }
                    }
                }
                else
                {
                    PosAsm = StatusLine.In_Str(3, ":");
                    if(PosAsm != 0) goto TestGNUError;
                }
            }
        }
        else
        {
            if(_strcmpi(StatusLine.Left(8).Get_String(), "warning ") == 0) goto TestLCC32Error;
            if(StatusLine.In_Str(1, "warning", Text_Compare) != 0)
            {
                PosAsm = StatusLine.In_Str(1, "(");
                if(PosAsm != 0)
                {
                    PosAsm++;
                    LineAsm = StatusLine.In_Str(PosAsm, ")");
                    LineNumber = StatusLine.Mid(PosAsm, LineAsm - PosAsm).Get_Long();
                    PosAsm = PosAsm - 2;
                    TakeLine = StatusLine.Replace("**Warning**", "", 1, -1, Binary_Compare);
                    TakeLine = TakeLine.Trim();
                    PosAsm = TakeLine.In_Str(1, "(");
                    FError = TakeLine.Mid(1, PosAsm - 1);
                    FError = FError.Trim();
                    if(GoToLine(FError, LineNumber - 1, 1) == 0)
                    {
                        BufString = CurrentDir + (CStr) "\\" + (CStr) FError;
                        if(GoToLine(BufString, LineNumber - 1, 1) == 0 && ProjectOn == 1)
                        {
                            // Load it from project
                            BufString = ProjectDir + (CStr) "\\" + (CStr) FError;
                            GoToLine(BufString, LineNumber - 1, 1);
                        }
                    }
                }
                else
                {
                    PosAsm = StatusLine.In_Str(3, ":");
                    if(PosAsm != 0) goto TestGNUError;
                }
            }
            else
            {
                // Handle NASM
                // 3=pass drive
TestGNUError:   PosAsm = StatusLine.In_Str(3, ":");
                if(PosAsm != 0)
                {
                    PosAsm++;
                    // LineNumber
                    LineAsm = StatusLine.In_Str(PosAsm, ":");
                    if((LineAsm - PosAsm) > 0)
                    {
                        if(PosAsm > 0)
                        {
                            if(StatusLine.Mid(PosAsm, LineAsm - PosAsm).Is_Numeric())
                            {
                                LineNumber = StatusLine.Mid(PosAsm, LineAsm - PosAsm).Get_Long();
                                PosAsm = StatusLine.In_Str(3, ":");
                                if(StatusLine.Len() > (long) PosAsm)
                                {
                                    FError = StatusLine.Mid(1, PosAsm - 1);
                                    FError = FError.Trim();
                                    if(GoToLine(FError, LineNumber - 1, 1) == 0)
                                    {
                                        BufString = CurrentDir + (CStr) "\\" + (CStr) FError;
                                        if(GoToLine(BufString, LineNumber - 1, 1) == 0 && ProjectOn == 1)
                                        {
                                            // Load it from project
                                            BufString = ProjectDir + (CStr) "\\" + (CStr) FError;
                                            GoToLine(BufString, LineNumber - 1, 1);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Move treeview entries
void MoveProjectItems(HTREEITEM OldItem, HTREEITEM NewItem)
{
    int i = 0;
    HTREEITEM hParent = 0;
    CList <char *> ItemsArray;
    CList <HTREEITEM> ItemsArrayhWnd;
    HTREEITEM TreeViewChildEntry = 0;
    CStr SaveItem;
    
    // Get 9th child (and not the 7th son)
    if(NewItem == hTreeViewResources)
    {
        NewItem = TreeViewGetFirstItemChild(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    else if(NewItem == hTreeViewIcons)
    {
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    else if(NewItem == hTreeViewCursors)
    {
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    else if(NewItem == hTreeViewBitmaps)
    {
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    else if(NewItem == hTreeViewStrings)
    {
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    else if(NewItem == hTreeViewAccelerators)
    {
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    else if(NewItem == hTreeViewMenus)
    {
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    else if(NewItem == hTreeViewDialogs)
    {
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    else if(NewItem == hTreeViewRawdatas)
    {
        NewItem = TreeViewGetNextItem(hTreeView, NewItem);
    }
    FirstDropped = -1;
    LastDropped = -1;
    // Parent should be the same for both items
    hParent = TreeViewGetItemParent(hTreeView, OldItem);
    // Fill array with all childs
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hParent);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        // Mark source item
        if(TreeViewChildEntry == OldItem) FirstDropped = i;
        // Mark dest item
        if(TreeViewChildEntry == NewItem) LastDropped = i;
        // Fill entry in array
        ItemsArrayhWnd.Add(TreeViewChildEntry);
        ItemsArray.Add(TreeViewGetItemText(hTreeView, TreeViewChildEntry).Get_String());
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    if(LastDropped == -1) LastDropped = 0;
    if(FirstDropped == LastDropped) return;
    if(FirstDropped > LastDropped)
    {
        SaveItem = ItemsArray.Get(FirstDropped)->Content;
        for(i = FirstDropped - 1; i >= (long) LastDropped; i--)
        {
            ItemsArray.Set(i + 1, ItemsArray.Get(i)->Content);
        }
        ItemsArray.Set(LastDropped, SaveItem.Get_String());
        for(i = 0; i < ItemsArray.Amount(); i++)
        {
            TreeViewSetItemText(hTreeView,
                                  ItemsArrayhWnd.Get(i)->Content,
                                  ItemsArray.Get(i)->Content);
        }
    }
    else
    {
        SaveItem = ItemsArray.Get(FirstDropped)->Content;
        for(i = FirstDropped; i <= (long) LastDropped - 1; i++)
        {
            ItemsArray.Set(i, ItemsArray.Get(i + 1)->Content);
        }
        ItemsArray.Set(LastDropped, SaveItem.Get_String());
        for(i = 0; i < ItemsArray.Amount(); i++)
        {
            TreeViewSetItemText(hTreeView,
                                  ItemsArrayhWnd.Get(i)->Content,
                                  ItemsArray.Get(i)->Content);
        }
    }
    ItemsArray.Erase();
    ItemsArrayhWnd.Erase();
}

// -----------------------------------------------------------------------
// List of windows proc
LRESULT CALLBACK WindowsListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYUP:
            RefreshCurrentWindowFromList();
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Send a message to MDI client according to selected window entry in list
void RefreshCurrentWindowFromList(void)
{
    int i = 0;
    long CurrentSelectedWindow = 0;

    CurrentSelectedWindow = ListViewGetSelItem(hWindowsListView, -1);
    PreviouslySelectedWindow = CurrentSelectedWindow;
    if(CurrentSelectedWindow != -1)
    {
        for(i = 0; i < ChildsLVPos.Amount(); i++)
        {
            if(ChildsLVPos.Get(i)->Content == CurrentSelectedWindow)
            {
                // Focus on selected window
                if(ChildshWnd.Get(i)->Content != ClientGetActiveChild(hMDIform.hClient))
                {
                    SendMessage(hMDIform.hClient, WM_MDIACTIVATE, (long) ChildshWnd.Get(i)->Content, 0);
                }
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------
// Find the previous child of the windows list
HWND GetPreviousChildInList(HWND hWnd)
{
    for(int i = 0; i < ChildshWnd.Amount(); i++)
    {
        if(ChildshWnd.Get(i)->Content == hWnd)
        {
            if(i > 0)
            {
                return(ChildshWnd.Get(i - 1)->Content);
            }
            else
            {
                // Wrap in list and return last entry
                return(ChildshWnd.Get(ChildshWnd.Amount() - 1)->Content);
            }
            break;
        }
    }
    return(NULL);
}

// -----------------------------------------------------------------------
// Find the next child of the windows list
HWND GetNextChildInList(HWND hWnd)
{
    for(int i = 0; i < ChildsLVPos.Amount(); i++)
    {
        if(ChildshWnd.Get(i)->Content == hWnd)
        {
            if(i < (ChildshWnd.Amount() - 1))
            {
                return(ChildshWnd.Get(i + 1)->Content);
            }
            else
            {
                // Wrap in list and return first entry
                return(ChildshWnd.Get(0)->Content);
            }
            break;
        }
    }
    return(NULL);
}

// -----------------------------------------------------------------------
// Find and set the previous child of the windows list
void SetPreviousChildInList(void)
{
    int i = 0;
    long CurrentSelectedWindow = 0;

    CurrentSelectedWindow = ListViewGetSelItem(hWindowsListView, -1);
    PreviouslySelectedWindow = CurrentSelectedWindow;
    if(CurrentSelectedWindow != -1)
    {
        CurrentSelectedWindow--;
        if(CurrentSelectedWindow >= 0)
        {
            for(i = 0; i < ChildsLVPos.Amount(); i++)
            {
                if(ChildsLVPos.Get(i)->Content == CurrentSelectedWindow)
                {
                    // focus on selected window
                    if(ChildshWnd.Get(i)->Content != ClientGetActiveChild(hMDIform.hClient))
                    {
                        SendMessage(hMDIform.hClient, WM_MDIACTIVATE, (long) ChildshWnd.Get(i)->Content, 0);
                    }
                    break;
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Find and set the next child of the windows list
void SetNextChildInList(void)
{
    int i = 0;
    long CurrentSelectedWindow = 0;

    CurrentSelectedWindow = ListViewGetSelItem(hWindowsListView, -1);
    PreviouslySelectedWindow = CurrentSelectedWindow;
    if(CurrentSelectedWindow != -1)
    {
        CurrentSelectedWindow++;
        if(CurrentSelectedWindow <= ListViewItemCount(hWindowsListView))
        {
            for(i = 0; i < ChildsLVPos.Amount(); i++)
            {
                if(ChildsLVPos.Get(i)->Content == CurrentSelectedWindow)
                {
                    // focus on selected window
                    if(ChildshWnd.Get(i)->Content != ClientGetActiveChild(hMDIform.hClient))
                    {
                        SendMessage(hMDIform.hClient, WM_MDIACTIVATE, (long) ChildshWnd.Get(i)->Content, 0);
                    }
                    break;
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Return the handle of a selected window from the windows list 
// without selecting it
HWND GetSelectedWindowFromList(void)
{
    long CurrentLvItem;
    int i;
    HWND hRes = NULL;

    CurrentLvItem = ListViewGetSelItem(hWindowsListView, -1);
    if(CurrentLvItem != -1)
    {
        for(i = 0; i < ChildsLVPos.Amount(); i++)
        {
            if(ChildsLVPos.Get(i)->Content == CurrentLvItem)
            {
                hRes = ChildshWnd.Get(i)->Content;
                break;
            }
        }
    }
    ListViewSetItemSel(hWindowsListView, PreviouslySelectedWindow);
    return(hRes);
}
