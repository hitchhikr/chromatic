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
// Dlg_UserMenusEd.cpp: User menus editor
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "ConstMenus.h"
#include "MiscFunctions.h"
#include "Menus.h"
#include "Toolbars.h"
#include "MDI_Form.h"
#include "Dlg_UserMenusEd.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMUserMenushwnd;
HWND FRMUserMenusListbox;
HWND FRMUserMenusCmdOk;
HWND FRMUserMenusCmdCancel;
HWND FRMUserMenusCmdAddCmd;
HWND FRMUserMenusCmdAddMenu;
HWND FRMUserMenusCmdReplaceEntry;
HWND FRMUserMenusCmdRemoveEntry;
HWND FRMUserMenusCmdUp;
HWND FRMUserMenusCmdDown;
HWND FRMUserMenusCmdClear;
HWND FRMUserMenusCmdLoad;
HWND FRMUserMenusCmdSave;
HWND FRMUserMenusCmdShortcuts;
HWND FRMUserMenusCmdDirs;
HWND FRMUserMenusTxtDescription;
HWND FRMUserMenusTxtDat;
HWND FRMUserMenusTbOpen;
HWND FRMUserMenusCbRunType;
CStr FRMUserMenusRetVal;
CList <char *> NewMenus;
CList <int> NewMenusNbr;
CList <char *> NewMenusDetails;

CList <char *> TempMenusCommands;
CList <char *> TempMenusLbl;
CList <int> TempToolbarsCommands;
CList <int> TempToolbarsCmdIndex;

HWND FRMMenusListview;
HWND FRMMenusClCmdClear;
HWND FRMMenusClCmdCancel;
HWND FRMMenusClCmdSelAll;
HWND FRMMenusClCmdUnSelAll;
HDC FRMMenushDC;
CList <char *> ClearMenusArray;
CList <int> ClearMenusArrayBeg;
CList <int> ClearMenusArrayEnd;
HMENU hUsrMenu;
CStr CurrentMenusFileName;
long TempNbrCmds;
//CList TempMenusCmdsLbl;
//CList TempMenusCmdsComments;

// -----------------------------------------------------------------------
// Functions
LRESULT CALLBACK FRMUserMenusLsbProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMUserMenusTxtDatProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FRMUserMenusListboxReplace(void);
void FRMUserMenusListboxAddCmd(void);
void FRMUserMenusListboxAddMenu(void);
void FRMUserMenusSelFile(void);
long FRMUserMenusValidate(void);
void SetTempUserArray(void);
long UpdateToolbars(void);
void DeleteToolbarEntry(CStr ToolBarName, long PrimIndex);
int CALLBACK FRMMenusClearProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
long FillMenusArray(void);
long FillMenusList(HDC hDC);
void ClearSelectedMenus(void);
void LoadMenusFile(CStr MenusFileName);
void SetCurrentMenusWinTitle(CStr FileToUse);
void SaveUserMenus(CStr FileToSave);

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMUserMenusProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long PosEMName = 0;
    CStr Entry;
    long *ArEntry = 0;
    PAINTSTRUCT EnterValuePs;
    CStr OpFilters;
    CStr LdFile;
    CStr BufString;
    CStr MsgBoxStr;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            FRMUserMenushwnd = hwndDlg;
            CurrentMenusFileName = UserMenusIniFile;
            SetTempUserArray();
            FRMUserMenusCmdOk = CreateButton(371, 340, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdCancel = CreateButton(450, 340, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdAddCmd = CreateButton(452, 1, 77, 23, hwndDlg, "Add cmd", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdAddMenu = CreateButton(452, 25, 77, 23, hwndDlg, "Add menu", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdReplaceEntry = CreateButton(452, 51, 77, 23, hwndDlg, "Repl. entry", 5, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdRemoveEntry = CreateButton(452, 77, 77, 23, hwndDlg, "Rem. entry", 6, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdUp = CreateButton(452, 103, 77, 23, hwndDlg, "Move up", 7, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdDown = CreateButton(452, 127, 77, 23, hwndDlg, "Move down", 8, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdClear = CreateButton(452, 153, 77, 23, hwndDlg, "Erase...", 10, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdLoad = CreateButton(452, 179, 77, 23, hwndDlg, "Load...", 11, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdSave = CreateButton(452, 203, 77, 23, hwndDlg, "Save as...", 12, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdShortcuts = CreateButton(452, 233, 77, 23, hwndDlg, "Shortcuts", 13, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusCmdDirs = CreateButton(452, 257, 77, 23, hwndDlg, "Dirs", 14, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMUserMenusListbox = CreateListBox(1, 0, 450, 225 + 32 + 24, hwndDlg, 15, &FRMUserMenusLsbProc, 1, WS_TABSTOP | WS_HSCROLL, WS_EX_STATICEDGE);
            CreateLabel(2, 230 + 32 + 24, 63, 15, hwndDlg, "Description :", 16, 0, 0, 0);
            FRMUserMenusTxtDescription = CreateTextBox(67, 227 + 32 + 24, 461, 20, hwndDlg, "", 17, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
            CreateLabel(2, 253 + 32 + 24, 63, 15, hwndDlg, "Command :", 18, 0, 0, 0);
            FRMUserMenusTxtDat = CreateTextBox(67, 250 + 32 + 24, 316, 20, hwndDlg, "", 19, &FRMUserMenusTxtDatProc, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
            FRMUserMenusTbOpen = CreateToolBar(385, 249 + 32 + 24, 25, 23, hwndDlg, GlobalImageList1, 20, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            ToolBarAddButton(FRMUserMenusTbOpen, "", FRMUserMenusBUTTONOPEN, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            FRMUserMenusCbRunType = CreateComboBox(410, 250 + 32 + 24, 118, 121, hwndDlg, "", 21, 0, WS_TABSTOP | CBS_DROPDOWNLIST);
            ComboBoxAddItem(FRMUserMenusCbRunType, "Windows app", -1);
            ComboBoxAddItem(FRMUserMenusCbRunType, "WinHelp file", -1);
            ComboBoxAddItem(FRMUserMenusCbRunType, "Capture DOS output", -1);
            ComboBoxAddItem(FRMUserMenusCbRunType, "QEditor Plugin", -1);
            ComboBoxAddItem(FRMUserMenusCbRunType, "Run with Snap", -1);
            ComboBoxAddItem(FRMUserMenusCbRunType, "Html page", -1);
            ComboBoxAddItem(FRMUserMenusCbRunType, "Document file", -1);
            ComboBoxAddItem(FRMUserMenusCbRunType, "Builtin directives", -1);
            ComboBoxAddItem(FRMUserMenusCbRunType, "LUA script", -1);
            ComboBoxSetIndex(FRMUserMenusCbRunType, 0);
            LoadMenusFile(UserMenusIniFile);
            hUsrMenu = CreatePopupMenu();
            MenuAddString(hUsrMenu, "Delete entry", USRM_IDBASE + USRM_DEL, TRUE);
            MenuAddString(hUsrMenu, "Move entry up", USRM_IDBASE + USRM_UP, TRUE);
            MenuAddString(hUsrMenu, "Move entry down", USRM_IDBASE + USRM_DOWN, TRUE);
            FreezeTimer = 1;
            SetFocus(FRMUserMenusListbox);
            return(0);
        case WM_PAINT:
            BeginPaint(hwndDlg, &EnterValuePs);
            GDIDrawHorzSep(hwndDlg, 0, 333, 530);
            EndPaint(hwndDlg, &EnterValuePs);
            break;
        case WM_SIZE:
            ListBoxSetHorzScrollWidth(FRMUserMenusListbox, ControlWidth(FRMUserMenusListbox) * 2);
            break;
        case WM_COMMAND:
            if((HWND) lParam == FRMUserMenusListbox)
            {
                if(ControlGetNotifiedCommand(wParam) == LBN_SELCHANGE)
                {
                    if(ListBoxCount(FRMUserMenusListbox) != 0)
                    {
                        Entry = ListBoxGetCurrentItem(FRMUserMenusListbox);
                        if(strcmp(Entry.Left(1).Get_String(), "[") == 0)
                        {
                            PosEMName = Entry.In_Str(2, "]");
                            ControlSetText(FRMUserMenusTxtDescription, Entry.Mid(2, PosEMName - 2));
                            ControlSetText(FRMUserMenusTxtDat, "");
                            ControlEnable(FRMUserMenusCbRunType, 0);
                        }
                        else
                        {
                            ArEntry = StringSplit(Entry, "|");
                            if(StringGetSplitUBound(ArEntry) == 0)
                            {
                                // It's just a space
                                ControlSetText(FRMUserMenusTxtDescription, "-");
                                ControlSetText(FRMUserMenusTxtDat, "");
                                ControlEnable(FRMUserMenusCbRunType, 0);
                            }
                            else
                            {
                                ControlEnable(FRMUserMenusCbRunType, 1);
                                // Command
                                ControlSetText(FRMUserMenusTxtDescription, StringGetSplitElement(Entry, ArEntry, 0).Mid(4, StringGetSplitElement(Entry, ArEntry, 0).Len() - 4));
                                ControlSetText(FRMUserMenusTxtDat, StringGetSplitElement(Entry, ArEntry, 1).Mid(2));
                                // + help command
                                if(StringGetSplitUBound(ArEntry) == 2)
                                {
                                    ComboBoxSetIndex(FRMUserMenusCbRunType, 1);
                                }
                                else
                                {
                                    BufString = ControlGetText(FRMUserMenusTxtDat).Upper_Case();
                                    BufString = BufString.Left(5);
                                    if(BufString == "CAPT ")
                                    {
                                        ComboBoxSetIndex(FRMUserMenusCbRunType, 2);
                                        ControlSetText(FRMUserMenusTxtDat, ControlGetText(FRMUserMenusTxtDat).Mid(6));
                                    }
                                    else if(BufString == "PLUG ")
                                    {
                                        ComboBoxSetIndex(FRMUserMenusCbRunType, 3);
                                        ControlSetText(FRMUserMenusTxtDat, ControlGetText(FRMUserMenusTxtDat).Mid(6));
                                    }
                                    else if(BufString == "SNAP ")
                                    {
                                        ComboBoxSetIndex(FRMUserMenusCbRunType, 4);
                                        ControlSetText(FRMUserMenusTxtDat, ControlGetText(FRMUserMenusTxtDat).Mid(6));
                                    }
                                    else if(BufString == "HTML ")
                                    {
                                        ComboBoxSetIndex(FRMUserMenusCbRunType, 5);
                                        ControlSetText(FRMUserMenusTxtDat, ControlGetText(FRMUserMenusTxtDat).Mid(6));
                                    }
                                    else if(BufString == "DOCU ")
                                    {
                                        ComboBoxSetIndex(FRMUserMenusCbRunType, 6);
                                        ControlSetText(FRMUserMenusTxtDat, ControlGetText(FRMUserMenusTxtDat).Mid(6));
                                    }
                                    else if(BufString == "PREP ")
                                    {
                                        ComboBoxSetIndex(FRMUserMenusCbRunType, 7);
                                        ControlSetText(FRMUserMenusTxtDat, ControlGetText(FRMUserMenusTxtDat).Mid(6));
                                    }
                                    else if(BufString == "LUAS ")
                                    {
                                        ComboBoxSetIndex(FRMUserMenusCbRunType, 8);
                                        ControlSetText(FRMUserMenusTxtDat, ControlGetText(FRMUserMenusTxtDat).Mid(6));
                                    }
                                    else
                                    {
                                        ComboBoxSetIndex(FRMUserMenusCbRunType, 0);
                                    }
                                }
                                ControlSetText(FRMUserMenusTxtDat, ControlGetText(FRMUserMenusTxtDat).Trim());
                            }
                            StringReleaseSplit(ArEntry);
                        }
                    }
                }
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdOk)
            {
                if(FRMUserMenusValidate() == 1) ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdDown)
            {
                ListBoxSelItemDown(FRMUserMenusListbox);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdUp)
            {
                ListBoxSelItemUp(FRMUserMenusListbox);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdShortcuts)
            {
                MsgBoxStr = "Available shortcuts:\r\r";
                MsgBoxStr = MsgBoxStr + "%1 - Filename with directory / without extension\r";
                MsgBoxStr = MsgBoxStr + "%2 - Filename with directory / with extension\r";
                MsgBoxStr = MsgBoxStr + "%3 - Filename without directory / without extension\r";
                MsgBoxStr = MsgBoxStr + "%4 - Filename without directory / with extension\r";
                MsgBoxStr = MsgBoxStr + "%5 - Directory without filename\r";
                MsgBoxStr = MsgBoxStr + "%6 - Current selected word in active window\r";
                MsgBoxStr = MsgBoxStr + "%7 - Extension of filename\r";
                MsgBoxStr = MsgBoxStr + "%8 - Prompt user with an open file dialog and return %3\r";
                MsgBoxStr = MsgBoxStr + "%9 - Prompt user with an open file dialog and return %4\r";
                MsgBoxStr = MsgBoxStr + "%C - Prompt user with a color dialog (result as decimal)\r";
                MsgBoxStr = MsgBoxStr + "%D - Prompt user with a color dialog (result as hexadecimal)\r";
                MsgBoxStr = MsgBoxStr + "%N - Create a new child window\r";
                MsgBoxStr = MsgBoxStr + "%? - Ask user to enter a required argument\r";
                MsgBoxStr = MsgBoxStr + "%@ - Ask user to enter an optional argument\r";
                MsgBoxStr = MsgBoxStr + "%# - Prompt user with an open file dialog\r";
                MsgBoxStr = MsgBoxStr + "%& - Prompt user with a save file dialog\r";
                MsgBoxStr = MsgBoxStr + "%* - Prompt user with a browse for folder dialog";
                MiscMsgBox(hwndDlg, MsgBoxStr, MB_INFORMATION, Requesters);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdDirs)
            {
                MsgBoxStr = "Available quick directories:\r\r";
                MsgBoxStr = MsgBoxStr + "[ROOTDIR] - Compiler/Assembler main directory\r";
                MsgBoxStr = MsgBoxStr + "[BINDIR] - Compiler/Assembler binaries directories\r";
                MsgBoxStr = MsgBoxStr + "[INCLUDEDIR] - Compiler/Assembler includes directories\r";
                MsgBoxStr = MsgBoxStr + "[LIBDIR] - Compiler/Assembler libraries directories\r";
                MsgBoxStr = MsgBoxStr + "[HELPDIR] - Compiler/Assembler help files directories\r";
                MsgBoxStr = MsgBoxStr + "[MAINDIR] - Chromatic root directory\r";
                MsgBoxStr = MsgBoxStr + "[WINDIR] - Windows root directory\r" ;
                MsgBoxStr = MsgBoxStr + "[WINSYSDIR] - Windows system directory\r";
                MsgBoxStr = MsgBoxStr + "[TEMPDIR] - Windows temporary storage directory\r";
                MsgBoxStr = MsgBoxStr + "[TEMPLATESDIR] - Chromatic templates directory\r";
                MsgBoxStr = MsgBoxStr + "[PROJECTSDIR] - Chromatic projects directory\r";
                MsgBoxStr = MsgBoxStr + "[MAINHELPDIR] - Chromatic help directory\r";
                MsgBoxStr = MsgBoxStr + "[SNIPPETSDIR] - Chromatic registered snippets directory\r";
                MsgBoxStr = MsgBoxStr + "[SOURCEDIR] - Current source directory\r";
                MsgBoxStr = MsgBoxStr + "[DATABASESDIR] - Windows databases directory\r";
                MsgBoxStr = MsgBoxStr + "[SKINSDIR] - Chromatic projects skins directory\r";
                MsgBoxStr = MsgBoxStr + "[LANGSCRIPTSDIR] - Chromatic languages scripts directory\r";
                MsgBoxStr = MsgBoxStr + "[LANGUAGESDIR] - Chromatic languages definitions directory\r";
                MsgBoxStr = MsgBoxStr + "[DOWNLOADSDIR] - Chromatic downloads directory\r";
                MsgBoxStr = MsgBoxStr + "[ADDINSDIR] - Chromatic AddIns directory\r";
                MsgBoxStr = MsgBoxStr + "[CONFIGDIR] - Chromatic config directory\r";
                MsgBoxStr = MsgBoxStr + "[MENUSDIR] - Chromatic user menus directory\r";
                MsgBoxStr = MsgBoxStr + "[WIZARDSDIR] - Chromatic wizards directory\r";
                MsgBoxStr = MsgBoxStr + "[FILTERSDIR] - Chromatic filters directory\r";
                MsgBoxStr = MsgBoxStr + "[EXTRACODEDIR] - Chromatic skins' extra code directory\n";
                MsgBoxStr = MsgBoxStr + "[TOOLSDIR] - Chromatic extra tools directory\n";
                MsgBoxStr = MsgBoxStr + "[SCRIPTSDIR] - Chromatic scripts directory";
                MiscMsgBox(hwndDlg, MsgBoxStr, MB_INFORMATION, Requesters);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdClear)
            {
                if(FillMenusArray() > 0) CreateModalDialog(-1, -1, 302, 204, hwndDlg, &FRMMenusClearProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdLoad)
            {
                OpFilters = AppTitle + (CStr) " user menus files (*.Ume)|*.Ume|Old " + (CStr) AppTitle + (CStr) " user menus files (UserMenus.ini)|UserMenus.ini";
                LdFile = ComDlgGetOpenFileName(hwndDlg, OpFilters, Dirs[DIR_MENUS], 0, CurrentDir);
                if(LdFile.Len() != 0) LoadMenusFile(LdFile);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdSave)
            {
                OpFilters = AppTitle + " user menus files (*.Ume)|*.Ume";
                LdFile = ComDlgGetSaveFileName(hwndDlg, OpFilters, Dirs[DIR_MENUS], CurrentDir);
                if(LdFile.Len() != 0) SaveUserMenus(LdFile);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdRemoveEntry)
            {
                ListBoxSelItemRemove(FRMUserMenusListbox);
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdReplaceEntry)
            {
                FRMUserMenusListboxReplace();
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdAddCmd)
            {
                FRMUserMenusListboxAddCmd();
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusCmdAddMenu)
            {
                FRMUserMenusListboxAddMenu();
                return(0);
            }
            else if((HWND) lParam == FRMUserMenusTbOpen)
            {
                if((HWND) wParam == FRMUserMenusBUTTONOPEN) {
                    FRMUserMenusSelFile();
                }
                return(0);
            }
            switch(wParam)
            {
                case (USRM_DEL + USRM_IDBASE):
                    ListBoxSelItemRemove(FRMUserMenusListbox);
                    return(0);
                case (USRM_UP + USRM_IDBASE):
                    ListBoxSelItemUp(FRMUserMenusListbox);
                    return(0);
                case (USRM_DOWN + USRM_IDBASE):
                    ListBoxSelItemDown(FRMUserMenusListbox);
                    return(0);
            }
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    switch(ControlGetNotifiedID(lParam))
                    {
                        case 0:
                            ToolBarDisplayToolTip("Select command to run", lParam);
                            return(0);
                    }
            }
            break;
        case WM_CLOSE:
            if(hUsrMenu != 0) DestroyMenu(hUsrMenu);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
        default:
            ListBoxProcessDrag(FRMUserMenusListbox, uMsg, lParam, CURSOR_DRAG + CURSOR_BASE);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Listbox hook
LRESULT CALLBACK FRMUserMenusLsbProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    POINT PopMenuCoords;
    long ItemToSelect = 0;

    switch(uMsg)
    {
        case WM_KEYDOWN:
            // Up
            if(wParam == 40)
            {
                if((GetKeyState(VK_SHIFT) & 0x80) != 0)
                {
                    ListBoxSelItemDown(FRMUserMenusListbox);
                    return(0);
                }
            }
            // Down
            if(wParam == 38)
            {
                if((GetKeyState(VK_SHIFT) & 0x80) != 0)
                {
                    ListBoxSelItemUp(FRMUserMenusListbox);
                    return(0);
                }
            }
            // Delete
            if(wParam == 46) ListBoxSelItemRemove(FRMUserMenusListbox);
            break;
        case WM_RBUTTONDOWN:
            ItemToSelect = ListBoxItemUnderCursor(hWnd);
            if(ItemToSelect != -1) ListBoxSetIndex(hWnd, ItemToSelect);
            return(0);
        case WM_CONTEXTMENU:
            ItemToSelect = ListBoxItemUnderCursor(hWnd);
            if(ItemToSelect != -1)
            {
                ListBoxSetIndex(hWnd, ItemToSelect);
                GetCursorPos(&PopMenuCoords);
                TrackPopupMenu(hUsrMenu, TPM_LEFTALIGN + TPM_LEFTBUTTON, PopMenuCoords.x, PopMenuCoords.y, 0, FRMUserMenushwnd, NULL);
            }
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Text change
LRESULT CALLBACK FRMUserMenusTxtDatProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETFOCUS:
            ControlEnable(FRMUserMenusCbRunType, 1);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Replace entry in listbox
void FRMUserMenusListboxReplace(void)
{
    int i = 0;
    CStr CmdToRep;
    CStr CmdToReplace;
    long OldIdx = 0;
    CStr BufString;
    
    if(ListBoxCount(FRMUserMenusListbox) == 0) return;
    // Replace menu name
    if(strcmp(ListBoxGetCurrentItem(FRMUserMenusListbox).Left(1).Get_String(), "[") == 0)
    {
        for(i = 0; i <= (long) ListBoxCount(FRMUserMenusListbox) - 1; i++)
        {
            BufString = "[" + (CStr) ControlGetText(FRMUserMenusTxtDescription) + (CStr) "]";
            if(_strcmpi(ListBoxGetItem(FRMUserMenusListbox, i).Get_String(), BufString.Get_String()) == 0)
            {
                MiscMsgBox(FRMUserMenushwnd, "Menu already exists in list.", MB_ERROR, Requesters);
                SetFocus(FRMUserMenusTxtDescription);
                return;
            }
        }
        OldIdx = ListBoxGetSelItemIndex(FRMUserMenusListbox);
        ControlSetText(FRMUserMenusTxtDat, "");
        ListBoxReplaceSelItem(FRMUserMenusListbox, "[" + (CStr) ControlGetText(FRMUserMenusTxtDescription).Trim() + (CStr) "]");
        ListBoxSetIndex(FRMUserMenusListbox, OldIdx);
        SetFocus(FRMUserMenusListbox);
        return;
    }
    if(ControlGetText(FRMUserMenusTxtDescription).Len() == 0)
    {
        MiscMsgBox(FRMUserMenushwnd, "Description not specified.", MB_ERROR, Requesters);
        SetFocus(FRMUserMenusTxtDescription);
        return;
    }
    // Replace with a separator
    if(strcmp(ControlGetText(FRMUserMenusTxtDescription).Get_String(), "-") == 0)
    {
        ControlSetText(FRMUserMenusTxtDat, "");
        ListBoxReplaceSelItem(FRMUserMenusListbox, "..." + (CStr) ControlGetText(FRMUserMenusTxtDescription).Trim().Get_String());
        return;
    }
    if(ControlGetText(FRMUserMenusTxtDat).Len() == 0)
    {
        MiscMsgBox(FRMUserMenushwnd, "Command line not specified.", MB_ERROR, Requesters);
        SetFocus(FRMUserMenusTxtDat);
        return;
    }
    CmdToReplace = ControlGetText(FRMUserMenusTxtDat);
    OldIdx = ListBoxGetSelItemIndex(FRMUserMenusListbox);
    switch(ComboBoxGetIndex(FRMUserMenusCbRunType))
    {
        case 0:
            break;
        case 1:
            CmdToReplace = CmdToReplace + (CStr) " | HELP";
            break;
        case 2:
            CmdToReplace = "CAPT " + (CStr) CmdToReplace;
            break;
        case 3:
            CmdToReplace = "PLUG " + (CStr) CmdToReplace;
            break;
        case 4:
            CmdToReplace = "SNAP " + (CStr) CmdToReplace;
            break;
        case 5:
            CmdToReplace = "HTML " + (CStr) CmdToReplace;
            break;
        case 6:
            CmdToReplace = "DOCU " + (CStr) CmdToReplace;
            break;
        case 7:
            CmdToReplace = "PREP " + (CStr) CmdToReplace;
            break;
        case 8:
            CmdToReplace = "LUAS " + (CStr) CmdToReplace;
            break;
    }
    BufString = ControlGetText(FRMUserMenusTxtDescription);
    BufString = BufString + (CStr) " | " + (CStr) CmdToReplace;
    CmdToRep = "..." + (CStr) BufString.Trim().Get_String();
    ListBoxReplaceSelItem(FRMUserMenusListbox, CmdToRep);
    ListBoxSetIndex(FRMUserMenusListbox, OldIdx);
    SetFocus(FRMUserMenusListbox);
}

// -----------------------------------------------------------------------
// Add a command to listbox
void FRMUserMenusListboxAddCmd(void)
{
    CStr CmdToReplace;
    long OldIdx = 0;
    CStr BufString;

    if(ControlGetText(FRMUserMenusTxtDescription).Len() == 0)
    {
        MiscMsgBox(FRMUserMenushwnd, "Description not specified.", MB_ERROR, Requesters);
        SetFocus(FRMUserMenusTxtDescription);
        return;
    }
    // Add a separator
    if(strcmp(ControlGetText(FRMUserMenusTxtDescription).Get_String(), "-") == 0)
    {
        OldIdx = ListBoxGetSelItemIndex(FRMUserMenusListbox);
        ListBoxAddItem(FRMUserMenusListbox, "..." + (CStr) ControlGetText(FRMUserMenusTxtDescription).Trim(), ListBoxGetSelItemIndex(FRMUserMenusListbox));
        if(OldIdx == -1) OldIdx = 0;
        ListBoxSetIndex(FRMUserMenusListbox, OldIdx);
        return;
    }
    if(ControlGetText(FRMUserMenusTxtDat).Len() == 0)
    {
        MiscMsgBox(FRMUserMenushwnd, "Command line not specified.", MB_ERROR, Requesters);
        SetFocus(FRMUserMenusTxtDat);
        return;
    }
    CmdToReplace = ControlGetText(FRMUserMenusTxtDat);
    switch(ComboBoxGetIndex(FRMUserMenusCbRunType))
    {
        case 0:
            break;
        case 1:
            CmdToReplace = CmdToReplace + " | HELP";
            break;
        case 2:
            CmdToReplace = "CAPT " + CmdToReplace;
            break;
        case 3:
            CmdToReplace = "PLUG " + CmdToReplace;
            break;
        case 4:
            CmdToReplace = "SNAP " + CmdToReplace;
            break;
        case 5:
            CmdToReplace = "HTML " + CmdToReplace;
            break;
        case 6:
            CmdToReplace = "DOCU " + CmdToReplace;
            break;
        case 7:
            CmdToReplace = "PREP " + CmdToReplace;
            break;
        case 8:
            CmdToReplace = "LUAS " + CmdToReplace;
            break;
    }
    BufString = ControlGetText(FRMUserMenusTxtDescription);
    BufString = BufString + (CStr) " | " + (CStr) CmdToReplace;
    CmdToReplace = "..." + (CStr) BufString.Trim().Get_String();
    OldIdx = ListBoxGetSelItemIndex(FRMUserMenusListbox);
    ListBoxAddItem(FRMUserMenusListbox, CmdToReplace, ListBoxGetSelItemIndex(FRMUserMenusListbox));
    if(OldIdx == -1) OldIdx = 0;
    ListBoxSetIndex(FRMUserMenusListbox, OldIdx);
    SetFocus(FRMUserMenusListbox);
}

// -----------------------------------------------------------------------
// Add a menu to listbox
void FRMUserMenusListboxAddMenu(void)
{
    int i = 0;
    long OldIdx = 0;
    CStr BufString;

    if(ControlGetText(FRMUserMenusTxtDescription).Len() == 0)
    {
        MiscMsgBox(FRMUserMenushwnd, "Menu name not specified.", MB_ERROR, Requesters);
        SetFocus(FRMUserMenusTxtDescription);
        return;
    }
    if(strcmp(ControlGetText(FRMUserMenusTxtDescription).Get_String(), "-") == 0)
    {
        MiscMsgBox(FRMUserMenushwnd, "Separator not allowed as menu name.", MB_ERROR, Requesters);
        SetFocus(FRMUserMenusTxtDescription);
        return;
    }
    for(i = 0; i <= (long) ListBoxCount(FRMUserMenusListbox) - 1; i++)
    {
        BufString = "[" + (CStr) ControlGetText(FRMUserMenusTxtDescription).Get_String() + (CStr) "]";
        if(_strcmpi(ListBoxGetItem(FRMUserMenusListbox, i).Get_String(), BufString.Get_String()) == 0)
        {
            MiscMsgBox(FRMUserMenushwnd, "Menu already exists in list.", MB_ERROR, Requesters);
            SetFocus(FRMUserMenusTxtDescription);
            return;
        }
    }
    ControlSetText(FRMUserMenusTxtDat, "");
    OldIdx = ListBoxGetSelItemIndex(FRMUserMenusListbox);
    BufString = ControlGetText(FRMUserMenusTxtDescription).Trim();
    ListBoxAddItem(FRMUserMenusListbox, "[" + (CStr) BufString.Get_String() + (CStr) "]", ListBoxGetSelItemIndex(FRMUserMenusListbox));
    if(OldIdx == -1) OldIdx = 0;
    ListBoxSetIndex(FRMUserMenusListbox, OldIdx);
    SetFocus(FRMUserMenusListbox);
}

// -----------------------------------------------------------------------
// Select a command file
void FRMUserMenusSelFile(void)
{
    CStr FileN;
    CStr OpFilters;

    OpFilters = "All files (*.*)|*.*";
    FileN = ComDlgGetOpenFileName(FRMUserMenushwnd, OpFilters, "", 0, CurrentDir);
    if(FileN.Len() == 0) return;
    FileN = ChangeAbsolutePaths(FileN);
    ControlSetText(FRMUserMenusTxtDat, FileN);
    SetFocus(FRMUserMenusTxtDat);
    TextBoxSetCaretPos(FRMUserMenusTxtDat, -1);
}

// -----------------------------------------------------------------------
// Validate changes
long FRMUserMenusValidate(void)
{
    long ReturnValue = 0;
    CStr CurEntr;
    CStr CurEntr2;
    long *ArCur = 0;
    CStr DemEntr2;
    long NbMenus = 0;
    long NbCmd = 0;
    long NbCmd2 = 0;
    int i = 0;
    int j = 0;
    CStr MName;
    long PosJ = 0;
    CList <char *> TempOldMenus;
    CStr BufString;

    if(ListBoxCount(FRMUserMenusListbox) != 0)
    {
        if(strcmp(ListBoxGetItem(FRMUserMenusListbox, 0).Left(1).Get_String(), "[") != 0)
        {
            MiscMsgBox(FRMUserMenushwnd, "First entry must be a menu name.", MB_ERROR, Requesters);
            SetFocus(FRMUserMenusListbox);
            return(ReturnValue);
        }
    }
    NewMenus.Erase();
    UserMenusIniFile = CurrentMenusFileName;
    UserMenusIniFile = ChangeAbsolutePaths(UserMenusIniFile);
    IniWriteKey("Layout", "CurrentMenus", UserMenusIniFile, MainIniFile);
    UserMenusIniFile = CurrentMenusFileName;
    ReturnValue = 0;
    if(ListBoxCount(FRMUserMenusListbox) != 0)
    {
        NewMenusDetails.Erase();
        NewMenusNbr.Erase();
        NbMenus = 0;
        NbCmd = 0;
        NbCmd2 = 0;
        for(i = 0; i <= (long) ListBoxCount(FRMUserMenusListbox) - 1; i++)
        {
            CurEntr = ListBoxGetItem(FRMUserMenusListbox, i);
            // Menu entry
            if(strcmp(CurEntr.Left(1).Get_String(), "[") == 0)
            {
                BufString = CurEntr.Mid(2, CurEntr.Len() - 2);
                // Create an empty slot for menus elements
                NewMenusNbr.Add(0L);
                NewMenus.Add(BufString.Get_String());
                NbMenus++;
                NbCmd2 = 0;
            }
            else
            {
                // Separator entry
                if(strcmp(CurEntr.Get_String(), "...-") == 0)
                {
                    NewMenusDetails.Add("-");
                    NbCmd++;
                    NbCmd2++;
                    NewMenusNbr.Set(NbMenus - 1, NbCmd2);
                }
                else
                {
                    // Command entry
                    CurEntr2 = CurEntr.Trim();
                    ArCur = StringSplit(CurEntr2, " | ");
                    DemEntr2 = StringReplace(StringGetSplitElement(CurEntr2, ArCur, 1), " ", "$", 1, -1, Binary_Compare);
                    BufString = StringGetSplitElement(CurEntr2, ArCur, 0).Mid(4) + (CStr) " | " + (CStr) DemEntr2;
                    if(StringGetSplitUBound(ArCur) == 2) BufString = BufString + " | HELP";
                    NewMenusDetails.Add(BufString.Get_String());
                    StringReleaseSplit(ArCur);
                    NbCmd++;
                    NbCmd2++;
                    NewMenusNbr.Set(NbMenus - 1, NbCmd2);
                }
            }
        }
    }
    // Delete old menus
    TempOldMenus.Erase();
    for(i = 0; i <= 999; i++)
    {
        MName = IniReadKey("Menus", "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(), UserMenusIniFile);
        if(MName.Len() == 0) break;
        TempOldMenus.Add(MName.Get_String());
        IniDeleteKey("Menus", "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(), UserMenusIniFile);
    }
    for(i = 0; i < TempOldMenus.Amount(); i++)
    {
        if(strlen(TempOldMenus.Get(i)->Content) == 0) break;
        for(j = 0; j <= 999; j++)
        {
            MName = IniReadKey(TempOldMenus.Get(i)->Content, "Menu" + (CStr)StringNumberComplement(j, 3).Get_String(), UserMenusIniFile);
            if(strlen(MName.Get_String()) == 0) break;
            IniDeleteKey(TempOldMenus.Get(i)->Content, "Menu" + (CStr) StringNumberComplement(j, 3).Get_String(), UserMenusIniFile);
        }
    }
    // Recreate ini file
    PosJ = 0;
    if(NewMenus.Amount() != 0)
    {
        for(i = 0; i < NewMenus.Amount(); i++)
        {
            IniWriteKey("Menus", "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(), NewMenus.Get(i)->Content, UserMenusIniFile);
            for(j = 0; j <= (long) NewMenusNbr.Get(i)->Content - 1; j++)
            {
                IniWriteKey(NewMenus.Get(i)->Content, "Menu" + (CStr) StringNumberComplement(j, 3).Get_String(), NewMenusDetails.Get(PosJ)->Content, UserMenusIniFile);
                PosJ++;
            }
        }
    }
    TempOldMenus.Erase();
    ClearMenus();
    SetMenus();
    ReturnValue = 1;
    if(UpdateToolbars() == 1) RefreshToolbars();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Construct a temporary array with user menus
void SetTempUserArray(void)
{
    int i = 0;

    TempMenusCommands.Erase();
    TempMenusLbl.Erase();
    for(i = 0; i < MenusCmdsComments.Amount(); i++)
    {
        TempMenusCommands.Add(MenusCmdsComments.Get(i)->Content);
        TempMenusLbl.Add(MenusCmdsLbl.Get(i)->Content);
    }
}

// -----------------------------------------------------------------------
// Check modified user commands and update toolbars file
long UpdateToolbars(void)
{
    long ReturnValue = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    CStr CurTBName;
    CStr CurTbEntry;
    long CurTbEntryLng = 0;
    long FoundSimilar = 0;

    for(i = 0; i < 999; i++)
    {
        // Retrieve toolbar name
        CurTBName = IniReadKey("Toolbars", "TbName" + (CStr) StringNumberComplement(i, 3).Get_String(), ToolbarsIniFile);
        if(CurTBName.Len() == 0) break;
        TempToolbarsCommands.Erase();
        TempToolbarsCmdIndex.Erase();
        // Retrieve all user commands references in toolbar
        for(j = 0; j <= 999; j++)
        {
            CurTbEntry = IniReadKey(CurTBName, "BtnMenu" + (CStr) StringNumberComplement(j, 3).Get_String(), ToolbarsIniFile);
            if(CurTbEntry.Len() == 0) break;
            CurTbEntryLng = CurTbEntry.Get_Long();
            // Pass spaces
            if(CurTbEntryLng != -2)
            {
                CurTbEntry = IniReadKey(CurTBName, "BtnComd" + (CStr) StringNumberComplement(j, 3).Get_String(), ToolbarsIniFile);
                if(CurTbEntry.Len() == 0) break;
                CurTbEntryLng = CurTbEntry.Get_Long();
                // Check if it's an user command
                if((long) CurTbEntryLng >= MENU_USER_IDBASE)
                {
                    // Store the number of the command
                    TempToolbarsCommands.Add(CurTbEntryLng);
                    // Store the index from toolbar file
                    TempToolbarsCmdIndex.Add(j);
                }
            }
        }
        // User commands referenced in toolbars ?
        if(TempToolbarsCommands.Amount() != 0)
        {
            for(j = 0; j < TempToolbarsCommands.Amount(); j++)
            {
                if(TempToolbarsCommands.Get(j)->Content != 0)
                {
                    // Command -> Index
                    CurTbEntryLng = TempToolbarsCommands.Get(j)->Content - MENU_USER_IDBASE;
                    // Check if command had been suppressed
                    if((long) MenusCmdsComments.Amount() < (long) CurTbEntryLng - 1)
                    {
                        // Mark as modified
                        ReturnValue = 1;
                        // Delete entry
                        DeleteToolbarEntry(CurTBName, j);
                    }
                    else
                    {
                        // Check if command had been moved from original list
                        if(_strcmpi(TempMenusCommands.Get(CurTbEntryLng - 1)->Content,
                                    MenusCmdsComments.Get(CurTbEntryLng - 1)->Content) != 0)
                        {
                            // Moved: search for similar entry in the array
ForceTBRefresh:             FoundSimilar = 0;
                            for(k = 0; k < MenusCmdsComments.Amount(); k++) {
                                if(_strcmpi(TempMenusCommands.Get(CurTbEntryLng - 1)->Content,
                                            MenusCmdsComments.Get(k)->Content) == 0)
                                {
                                    FoundSimilar = 1;
                                    break;
                                }
                            }
                            switch(FoundSimilar)
                            {
                                case 1:
                                    // Found: set new command index
                                    ReturnValue = 1;
                                    IniWriteKey(CurTBName, "BtnComd" + (CStr) StringNumberComplement(j, 3).Get_String(), (k + MENU_USER_IDBASE + 1), ToolbarsIniFile);
                                    // Change command label
                                    IniWriteKey(CurTBName, "BtnTool" + (CStr) StringNumberComplement(j, 3).Get_String(), MenusCmdsLbl.Get(k)->Content, ToolbarsIniFile);
                                    IniWriteKey(CurTBName, "BtnHelp" + (CStr) StringNumberComplement(j, 3).Get_String(), MenusCmdsLbl.Get(k)->Content, ToolbarsIniFile);
                                case 0:
                                    // Not found: delete
                                    ReturnValue = 1;
                                    DeleteToolbarEntry(CurTBName, j);
                            }
                        }
                        else
                        {
                            // Same place: check if label have changed
                            if(strlen(MenusCmdsLbl.Get(CurTbEntryLng - 1)->Content) != 0)
                            {
                                if(_strcmpi(TempMenusLbl.Get(CurTbEntryLng - 1)->Content, MenusCmdsLbl.Get(CurTbEntryLng - 1)->Content) != 0) goto ForceTBRefresh;
                            }
                        }
                    }
                }
            }
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Delete a toolbar entry and move list down
void DeleteToolbarEntry(CStr ToolBarName, long PrimIndex)
{
    CStr EntrIcon;
    CStr EntrMenu;
    CStr EntrTool;
    CStr EntrHelp;
    CStr EntrComd;
    long PrimIndex2 = 0;

    IniDeleteKey(ToolBarName, "BtnIcon" + (CStr) StringNumberComplement(PrimIndex, 3).Get_String(), ToolbarsIniFile);
    IniDeleteKey(ToolBarName, "BtnMenu" + (CStr) StringNumberComplement(PrimIndex, 3).Get_String(), ToolbarsIniFile);
    IniDeleteKey(ToolBarName, "BtnTool" + (CStr) StringNumberComplement(PrimIndex, 3).Get_String(), ToolbarsIniFile);
    IniDeleteKey(ToolBarName, "BtnHelp" + (CStr) StringNumberComplement(PrimIndex, 3).Get_String(), ToolbarsIniFile);
    IniDeleteKey(ToolBarName, "BtnComd" + (CStr) StringNumberComplement(PrimIndex, 3).Get_String(), ToolbarsIniFile);
    // Move upper entries into current position
    for(PrimIndex2 = PrimIndex; PrimIndex2 <= 999; PrimIndex2++)
    {
        EntrIcon = IniReadKey(ToolBarName, "BtnIcon" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        EntrMenu = IniReadKey(ToolBarName, "BtnMenu" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        if(EntrMenu.Len() == 0) break;
        EntrTool = IniReadKey(ToolBarName, "BtnTool" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        EntrHelp = IniReadKey(ToolBarName, "BtnHelp" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        EntrComd = IniReadKey(ToolBarName, "BtnComd" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        IniWriteKey(ToolBarName, "BtnIcon" + (CStr) StringNumberComplement(PrimIndex2, 3).Get_String(), EntrIcon, ToolbarsIniFile);
        IniWriteKey(ToolBarName, "BtnMenu" + (CStr) StringNumberComplement(PrimIndex2, 3).Get_String(), EntrMenu, ToolbarsIniFile);
        IniWriteKey(ToolBarName, "BtnTool" + (CStr) StringNumberComplement(PrimIndex2, 3).Get_String(), EntrTool, ToolbarsIniFile);
        IniWriteKey(ToolBarName, "BtnHelp" + (CStr) StringNumberComplement(PrimIndex2, 3).Get_String(), EntrHelp, ToolbarsIniFile);
        IniWriteKey(ToolBarName, "BtnComd" + (CStr) StringNumberComplement(PrimIndex2, 3).Get_String(), EntrComd, ToolbarsIniFile);
        IniDeleteKey(ToolBarName, "BtnIcon" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        IniDeleteKey(ToolBarName, "BtnMenu" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        IniDeleteKey(ToolBarName, "BtnTool" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        IniDeleteKey(ToolBarName, "BtnHelp" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
        IniDeleteKey(ToolBarName, "BtnComd" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
    }
    IniDeleteKey(ToolBarName, "BtnIcon" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
    IniDeleteKey(ToolBarName, "BtnMenu" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
    IniDeleteKey(ToolBarName, "BtnTool" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
    IniDeleteKey(ToolBarName, "BtnHelp" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
    IniDeleteKey(ToolBarName, "BtnComd" + (CStr) StringNumberComplement(PrimIndex2 + 1, 3).Get_String(), ToolbarsIniFile);
}

// -----------------------------------------------------------------------
// Initialize Menus clearing form
int CALLBACK FRMMenusClearProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    long LngCol = 0;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMMenusListview, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            ControlSetText(hwndDlg, "Select menus to erase");
            FRMMenusClCmdClear = CreateButton(224, 1, 77, 23, hwndDlg, "Erase", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
            FRMMenusClCmdCancel = CreateButton(224, 25, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMMenusClCmdSelAll = CreateButton(224, 51, 77, 23, hwndDlg, "Select all", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMMenusClCmdUnSelAll = CreateButton(224, 75, 77, 23, hwndDlg, "Unselect all", 5, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMMenusListview = CreateListView(1, 1, 221, 202, hwndDlg, 6, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
            ListViewAddCol(FRMMenusListview, "", 220, 0);
            FRMMenushDC = GetDC(hwndDlg);
            LngCol = FillMenusList(FRMMenushDC);
            if(LngCol < 314) LngCol = 214;
            ListViewSetColWidth(FRMMenusListview, 0, LngCol);
            ReleaseDC(hwndDlg, FRMMenushDC);
            ListViewSetItemSel(FRMMenusListview, 0);
            SetFocus(FRMMenusListview);
            return(0);
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMMenusListview)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        ListViewCheckBoxItemDoubleClick(FRMMenusListview);
                        return(0);
                }
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == FRMMenusClCmdSelAll)
            {
                for(i = 0; i <= (long) ListViewItemCount(FRMMenusListview) - 1; i++)
                {
                    ListViewSetItemCheckbox(FRMMenusListview, i, 1);
                }
                return(0);
            }
            else if((HWND) lParam == FRMMenusClCmdUnSelAll)
            {
                for(i = 0; i <= (long) ListViewItemCount(FRMMenusListview) - 1; i++)
                {
                    ListViewSetItemCheckbox(FRMMenusListview, i, 0);
                }
                return(0);
            }
            else if((HWND) lParam == FRMMenusClCmdClear)
            {
                ClearSelectedMenus();
                ControlClose(hwndDlg);
                if(ListBoxCount(FRMUserMenusListbox) <= 0)
                {
                    ControlSetText(FRMUserMenusTxtDescription, "");
                    ControlSetText(FRMUserMenusTxtDat, "");
                }
                else
                {
                    ListBoxSetIndex(FRMUserMenusListbox, 0);
                    ControlSendMessage(FRMUserMenushwnd, 0x10000, (long) FRMUserMenusListbox);
                    SetFocus(FRMUserMenusListbox);
                }
                return(0);
            }
            else if((HWND) lParam == FRMMenusClCmdCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            break;
        case WM_CLOSE:
            EndDialog(hwndDlg, 0);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Fill the menus arrays with names
long FillMenusArray(void)
{
    int i = 0;
    CStr CurEntr;
    CStr BufString;
    long ReturnValue = 0;

    ClearMenusArray.Erase();
    ClearMenusArrayBeg.Erase();
    ClearMenusArrayEnd.Erase();
    for(i = 0; i <= (long) ListBoxCount(FRMUserMenusListbox) - 1; i++)
    {
        CurEntr = ListBoxGetItem(FRMUserMenusListbox, i);
        if(strcmp(CurEntr.Left(1).Get_String(), "[") == 0)
        {
            BufString = CurEntr.Mid(2, CurEntr.Len() - 2);
            ClearMenusArray.Add(BufString.Get_String());
            ClearMenusArrayBeg.Add(i);
            ClearMenusArrayEnd.Add(0L);
            ReturnValue++;
        }
        if(ClearMenusArrayEnd.Amount() > 0) ClearMenusArrayEnd.Set(ClearMenusArrayEnd.Amount() - 1, ClearMenusArrayEnd.Get(ClearMenusArrayEnd.Amount() - 1)->Content + 1);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Fill the listview with menus names
long FillMenusList(HDC hDC)
{
    int i = 0;
    long ReturnValue = 0;
    CStr MenuName;
    SIZE MenuNameSize;
    HGDIOBJ OldFontObj = 0;

    for(i = 0; i < ClearMenusArray.Amount(); i++)
    {
        MenuName = ClearMenusArray.Get(i)->Content;
        ListViewAddItem(FRMMenusListview, MenuName, i + 1, 0);
        ListViewSetItemCheckbox(FRMMenusListview, i, 0);
        OldFontObj = SelectObject(hDC, WASerifFont);
        GetTextExtentPoint32(hDC, MenuName.Get_String(), MenuName.Len(), &MenuNameSize);
        SelectObject(hDC, OldFontObj);
        if((long) ReturnValue < (MenuNameSize.cx + 30)) ReturnValue = (MenuNameSize.cx + 30);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Clear the selected menus
void ClearSelectedMenus(void)
{
    int i = 0;
    int j = 0;
    long LastEntry = 0;

    // Clear them in reverse order
    for(i = (ClearMenusArray.Amount() - 1); i >= 0; i--)
    {
        if(ListViewGetItemCheckbox(FRMMenusListview, i) == 1)
        {
            LastEntry = ClearMenusArrayBeg.Get(i)->Content + ClearMenusArrayEnd.Get(i)->Content - 1;
            for(j = LastEntry; j >= ClearMenusArrayBeg.Get(i)->Content; j--)
            {
                ListBoxDeleteItem(FRMUserMenusListbox, j);
            }
        }
    }
}

// -----------------------------------------------------------------------
// Load an user menus file
void LoadMenusFile(CStr MenusFileName)
{
    // Create the menus
    int i = 0;
    int j = 0;
    CStr MName;

    ControlFreeze(FRMUserMenusListbox, 1);
    ListBoxReset(FRMUserMenusListbox);
    for(i = 0; i <= 999; i++)
    {
        MName = IniReadKey("Menus", "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(), MenusFileName);
        if(MName.Len() == 0) break;
        ListBoxAddItem(FRMUserMenusListbox, "[" + (CStr) MName + (CStr) "]", -1);
        for(j = 0; j <= 999; j++)
        {
            FRMUserMenusRetVal = StringReplace(IniReadKey(MName, "Menu" + (CStr) StringNumberComplement(j, 3).Get_String(), MenusFileName), "$", " ", 1, -1, Binary_Compare);
            if(FRMUserMenusRetVal.Len() == 0) break;
            ListBoxAddItem(FRMUserMenusListbox, "..." + (CStr) FRMUserMenusRetVal, -1);
        }
    }
    if(ListBoxCount(FRMUserMenusListbox) != 0)
    {
        ListBoxSetIndex(FRMUserMenusListbox, 0);
        ControlSendMessage(FRMUserMenushwnd, 0x1000C, (long) FRMUserMenusListbox);
    }
    SetCurrentMenusWinTitle(MenusFileName);
    CurrentMenusFileName = MenusFileName;
    ControlFreeze(FRMUserMenusListbox, 0);
}
    
// -----------------------------------------------------------------------
// Set the title of the dialog with the currently used file
void SetCurrentMenusWinTitle(CStr FileToUse)
{
    ControlSetText(FRMUserMenushwnd, "User menus editor (Current file: " +
                     (CStr) FileRemoveExtension(FileGetFileName(FileToUse)) + (CStr) ")");
}

// -----------------------------------------------------------------------
// Save current menus into a given file
void SaveUserMenus(CStr FileToSave)
{
    CStr CurEntr;
    CStr CurEntr2;
    long *ArCur = 0;
    CStr DemEntr2;
    long NbMenus = 0;
    long NbCmd = 0;
    long NbCmd2 = 0;
    int i = 0;
    int j = 0;
    long PosJ = 0;
    CList <char *> SaveNewMenus;
    CList <int> SaveNewMenusNbr;
    CList <char *> SaveNewMenusDetails;
    CStr BufString;
    HANDLE MenusFileHandle;

    if(ListBoxCount(FRMUserMenusListbox) != 0)
    {
        if(strcmp(ListBoxGetItem(FRMUserMenusListbox, 0).Left(1).Get_String(), "[") != 0)
        {
            MiscMsgBox(FRMUserMenushwnd, "First entry must be a menu name.", MB_ERROR, Requesters);
            SetFocus(FRMUserMenusListbox);
            return;
        }
    }
    
    SaveNewMenus.Erase();
    // Make it the current used file
    CurrentMenusFileName = FileToSave;
    
    if(ListBoxCount(FRMUserMenusListbox) != 0)
    {
        SaveNewMenusDetails.Erase();
        SaveNewMenusNbr.Erase();
        NbMenus = 0;
        NbCmd = 0;
        NbCmd2 = 0;
        for(i = 0; i <= (long) ListBoxCount(FRMUserMenusListbox) - 1; i++)
        {
            CurEntr = ListBoxGetItem(FRMUserMenusListbox, i);
            // Menu entry
            if(strcmp(CurEntr.Left(1).Get_String(), "[") == 0)
            {
                BufString = CurEntr.Mid(2, CurEntr.Len() - 2);
                SaveNewMenus.Add(BufString.Get_String());
                SaveNewMenusNbr.Add(0);
                NbMenus++;
                NbCmd2 = 0;
            }
            else
            {
                // Separator entry
                if(strcmp(CurEntr.Get_String(), "...-") == 0)
                {
                    SaveNewMenusDetails.Add("-");
                    NbCmd++;
                    NbCmd2++;
                    SaveNewMenusNbr.Set(NbMenus - 1, NbCmd2);
                }
                else
                {
                    // Command entry
                    CurEntr2 = CurEntr.Trim();
                    ArCur = StringSplit(CurEntr2, " | ");
                    DemEntr2 = StringReplace(StringGetSplitElement(CurEntr2, ArCur, 1), " ", "$", 1, -1, Binary_Compare);
                    BufString = StringGetSplitElement(CurEntr2, ArCur, 0);
                    BufString = BufString.Mid(4) + (CStr) " | " + (CStr) DemEntr2;
                    if(StringGetSplitUBound(ArCur) == 2) BufString = BufString + " | HELP";
                    SaveNewMenusDetails.Add(BufString.Get_String());
                    StringReleaseSplit(ArCur);
                    NbCmd++;
                    NbCmd2++;
                    SaveNewMenusNbr.Set(NbMenus - 1, NbCmd2);
                }
            }
        }
    }
    
    // Remove the file
    MenusFileHandle = FileCreateEmpty(CurrentMenusFileName, NO_SECURITY_DESCRIPTOR);
    FileWriteLine(MenusFileHandle, "; Chromatic user menus file");
    FileWriteLine(MenusFileHandle, "; Do NOT edit this file manually");
    FileWriteLine(MenusFileHandle, "; unless you know what you're doing !!!");
    FileWriteLine(MenusFileHandle, "; (C) 2001-2010 Franck Charlet.");
    FileClose(MenusFileHandle);
    
    // Recreate ini file
    PosJ = 0;
    if(SaveNewMenus.Amount() != 0)
    {
        for(i = 0; i < SaveNewMenus.Amount(); i++)
        {
            IniWriteKey("Menus", "Menu" + (CStr) StringNumberComplement(i, 3).Get_String(),
                          SaveNewMenus.Get(i)->Content, CurrentMenusFileName);
            for(j = 0; j <= SaveNewMenusNbr.Get(i)->Content - 1; j++)
            {
                IniWriteKey(SaveNewMenus.Get(i)->Content, "Menu" + (CStr) StringNumberComplement(j, 3).Get_String(),
                              SaveNewMenusDetails.Get(PosJ)->Content, CurrentMenusFileName);
                PosJ++;
            }
        }
    }
    SetCurrentMenusWinTitle(CurrentMenusFileName);

    SaveNewMenus.Erase();
    SaveNewMenusDetails.Erase();
    SaveNewMenusNbr.Erase();
}
