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
// Dlg_Filters.cpp: Filters form
// -----------------------------------------------------------------------
// Send output to: window/file...
// if a filter return 0 then output previous LPFILTERFILE to selected target
// and input 0 to possible next filter (so that it start from the FileName)
// Filters manager must handle the the child window (open/include text).
// The include really appends datas when output to file is selected.

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "ConstMenus.h"
#include "Globals.h"
#include "Menus.h"
#include "CmdMenus.h"
#include "AddIns.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_Filters.h"

// -----------------------------------------------------------------------
// Constants
#define OPEN_FILTER_PROFILE 1
#define SAVE_FILTER_PROFILE 2
#define ADD_FILTERS 3
#define REM_FILTERS 4
#define OUTPUT_WINDOW 5
#define OUTPUT_FILE 6

// -----------------------------------------------------------------------
// Variables
HWND FRMFiltershWnd;
HWND FRMFiltersCmdOk;
HWND FRMFiltersCmdCancel;
HWND FRMFiltersCombo;
HWND FRMFiltersListView;
HWND FRMFiltersSelectedListView;
HWND FRMFiltersToolbar;
int Filter_Cancel;
int Filter_Output_Type;
CList <char *> Chosen_Filters;

// -----------------------------------------------------------------------
// Functions Declarations
LRESULT CALLBACK FRMFiltersFrameHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void GetOutputFile(HWND hwnd);
void FillFiltersList(void);
void Add_Filters(void);
void Rem_Filters(void);
int Check_Filter(char *FilterName);
void Fill_Filters_List(void);
void Fill_Filter_Profils(void);
int Save_Filter_Profil(CStr Profile_Name);
void Load_Filter_Profil(CStr Profile_Name);

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMFiltersProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT Ps;
    LV_KEYDOWN *lParamKey = 0;
    
    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMFiltersListView, GetSysColor(COLOR_WINDOW));
            ListViewSetBackColor(FRMFiltersSelectedListView, GetSysColor(COLOR_WINDOW));
            break;
        case WM_INITDIALOG:
            Filter_Cancel = 1;
            FRMFiltershWnd = hwndDlg;

            ControlSetText(hwndDlg, "Filters laboratory");
            FRMFiltersCmdOk = CreateButton(546 - 159, 268 - 26, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMFiltersCmdCancel = CreateButton(546 - 80, 268 - 26, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);

            FRMFiltersCombo = CreateComboBox(2, 2, 202, 150, hwndDlg, "", 0, 0, CBS_DROPDOWN | WS_TABSTOP);
            FRMFiltersToolbar = CreateToolBar(205, 1, 300, 23, hwndDlg, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
            ToolBarAddButton(FRMFiltersToolbar, "", OPEN_FILTER_PROFILE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddButton(FRMFiltersToolbar, "", SAVE_FILTER_PROFILE, ICON_SAVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddSeparator(FRMFiltersToolbar, 0);
            ToolBarAddButton(FRMFiltersToolbar, "", ADD_FILTERS, ICON_ADDFILTER, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            ToolBarAddButton(FRMFiltersToolbar, "", REM_FILTERS, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
            FRMFiltersListView = CreateListView(2, 26, 403, 203, hwndDlg, 4, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | WS_TABSTOP | LVS_NOSORTHEADER, WS_EX_STATICEDGE);
            ListViewAddCol(FRMFiltersListView, "Author", 100, 0);
            ListViewAddCol(FRMFiltersListView, "Description", 170, 1);
            ListViewAddCol(FRMFiltersListView, "Name", 110, 2);
            // Fill the list
            FillFiltersList();
            FRMFiltersSelectedListView = CreateListView(408, 26, 136, 203, hwndDlg, 5, 0, 0, LVS_EX_FULLROWSELECT | LVS_EX_LABELTIP, LVS_REPORT | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
            ListViewAddCol(FRMFiltersSelectedListView, "", 130, 0);
            Filter_Output_Type = FILTER_OUTPUT_WINDOW;
            // Fill the combo   
            Fill_Filter_Profils();
            FreezeTimer = 1;
            return(0);
        case WM_COMMAND:
            if((HWND) lParam == FRMFiltersCmdOk)
            {
                if(ListViewItemCount(FRMFiltersSelectedListView) == 0)
                {
                    MiscMsgBox(FRMFiltershWnd, "No filters selected.", MB_ERROR, Requesters);
                    return(0);
                }
                Fill_Filters_List();
                if(ToolBarIsButtonChecked(FRMFiltersToolbar, OUTPUT_WINDOW))
                {
                    Filter_Output_Type = FILTER_OUTPUT_WINDOW;
                }
                else
                {
                    Filter_Output_Type = FILTER_OUTPUT_FILE;
                }
                Filter_Cancel = 0;
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFiltersCmdCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMFiltersToolbar)
            {
                switch(wParam)
                {
                    case OPEN_FILTER_PROFILE:
                        if(ControlGetText(FRMFiltersCombo).Len() == 0)
                        {
                            MiscMsgBox(FRMFiltershWnd, "Select a profile name.", MB_ERROR, Requesters);
                            SetFocus(FRMFiltersCombo);
                        }
                        else
                        {
                            Load_Filter_Profil(ControlGetText(FRMFiltersCombo));
                        }
                        return(0);
                    case SAVE_FILTER_PROFILE:
                        if(Save_Filter_Profil(ControlGetText(FRMFiltersCombo)) == 0)
                        {
                            MiscMsgBox(FRMFiltershWnd, "Enter a profile name.", MB_ERROR, Requesters);
                            SetFocus(FRMFiltersCombo);
                        }
                        return(0);
                    case ADD_FILTERS:
                        Add_Filters();
                        return(0);
                    case REM_FILTERS:
                        Rem_Filters();
                        return(0);
                }
            }
            break;
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMFiltersListView)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case LVN_KEYDOWN:
                        lParamKey = (LV_KEYDOWN *) lParam;
                        if((lParamKey->wVKey & 0xffff) == 32) Add_Filters();
                        break;
                    case NM_DBLCLK:
                        Add_Filters();
                        break;
                }
            }
            else
            {
                if(ControlGetNotifiedhWnd(lParam) == FRMFiltersSelectedListView)
                {
                    switch(ControlGetNotifiedMsg(lParam))
                    {
                        case LVN_KEYDOWN:
                            lParamKey = (LV_KEYDOWN *) lParam;
                            if((lParamKey->wVKey & 0xffff) == 46) Rem_Filters();
                            break;
                        case NM_DBLCLK:
                            Rem_Filters();
                            break;
                    }
                }
                else
                {
                    switch(ControlGetNotifiedMsg(lParam))
                    {
                        case TTN_NEEDTEXT:
                            switch(ControlGetNotifiedID(lParam))
                            {
                                case OPEN_FILTER_PROFILE:
                                    ToolBarDisplayToolTip("Load selected filters profile", lParam);
                                    return(0);
                                case SAVE_FILTER_PROFILE:
                                    ToolBarDisplayToolTip("Save filters profile", lParam);
                                    return(0);
                                case ADD_FILTERS:
                                    ToolBarDisplayToolTip("Add selected filters(s) into profile list", lParam);
                                    return(0);
                                case REM_FILTERS:
                                    ToolBarDisplayToolTip("Remove selected filter(s) from profile list", lParam);
                                    return(0);
                                case OUTPUT_WINDOW:
                                    ToolBarDisplayToolTip("Sent output to window", lParam);
                                    return(0);
                                case OUTPUT_FILE:
                                    ToolBarDisplayToolTip("Sent output to file", lParam);
                                    return(0);
                            }
                            break;
                    }
                }
            }
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &Ps);
            GDIDrawHorzSep(hwndDlg, 0, ControlClientHeight(hwndDlg) - 33, 546);
            EndPaint(hwndDlg, &Ps);
            break;
        case WM_CLOSE:
            // Refresh the filters quick menus (both in "open" and in "include")
            CreateFiltersMenu(hFileMenuFilters, 17, MENU_FILE_IDBASE + MENU_FILE_FILTERS_ID, "Laboratory...\tCtrl+Shift+O", 0);
            CreateFiltersMenu(hFileMenuIncludeFilters, 18, MENU_FILE_IDBASE + MENU_FILE_INCLUDEFILTERS_ID, "Laboratory...\tCtrl+Shift+I", 1);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Fill the listview with found filters
void FillFiltersList(void)
{
    int i = 0;
    CStr FilterName;
    HMODULE FilterLib = 0;
    FARPROC FilterDesc = 0;
    FARPROC FilterAuth = 0;
    FARPROC FilterProc = 0;
    CStr Description;
    CStr Auth;
    CStr BufString;

    FilterName = FileDir(&((CStr) Dirs[DIR_FILTERS] + (CStr) "\\*.dll"));
    while(FilterName.Len() != 0)
    {
        BufString = Dirs[DIR_FILTERS] + (CStr) "\\" + (CStr) FilterName;
        FilterLib = LoadLibrary(BufString.Get_String());
        if(FilterLib != 0)
        {
            FilterDesc = GetProcAddress(FilterLib, "FilterDescription");
            FilterAuth = GetProcAddress(FilterLib, "FilterAuthor");
            FilterProc = GetProcAddress(FilterLib, "FilterProc");
            // Must have all procedures
            if(FilterDesc != 0 && FilterAuth != 0 && FilterProc != 0)
            {
                Description = GetDLLDescription(FilterName, FilterDesc);
                Auth = GetDLLAuthor(FilterAuth);
                ListViewAddItem(FRMFiltersListView, FileRemoveExtension(FileGetFileName(FilterName)), i, -1);
                ListViewSetSubItem(FRMFiltersListView, Description, i, 1);
                ListViewSetSubItem(FRMFiltersListView, Auth, i, 2);
                i++;
            }
            FreeLibrary(FilterLib);
            // This one is a weird bug and depends on the target OS
            // Actually it will fire an exception in debug mode
            // but still need to be close in any other cases otherwise the file stays locked
            #ifndef _DEBUG
                CloseHandle(FilterLib);
            #endif
        }
        FilterName = FileDir();
    }
}

// -----------------------------------------------------------------------
// Add the selected favorites files
void Add_Filters(void)
{
    long CurrentSelected;
    CStr Current_Item_Text;

    CurrentSelected = ListViewGetSelItem(FRMFiltersListView, -1);
    while(CurrentSelected != -1)
    {
        Current_Item_Text = ListViewGetItemText(FRMFiltersListView, CurrentSelected, 0);
        if(!Check_Filter(Current_Item_Text.Get_String()))
        {
            ListViewAddItem(FRMFiltersSelectedListView, Current_Item_Text, ListViewItemCount(FRMFiltersSelectedListView), -1);
        }
        CurrentSelected = ListViewGetSelItem(FRMFiltersListView, CurrentSelected);
    }
}

// -----------------------------------------------------------------------
// Remove the selected favorites files
void Rem_Filters(void)
{
    long CurrentSelected;

    CurrentSelected = ListViewGetSelItem(FRMFiltersSelectedListView, -1);
    while(CurrentSelected != -1)
    {
        ListViewDeleteItem(FRMFiltersSelectedListView, CurrentSelected);
        CurrentSelected = ListViewGetSelItem(FRMFiltersSelectedListView, -1);
    }
}

// -----------------------------------------------------------------------
// Check if a filter is already in the list of selected filters
int Check_Filter(char *FilterName)
{
    int i;

    for(i = 0; i < ListViewItemCount(FRMFiltersSelectedListView); i++)
    {
        if(lstrcmpi(FilterName, ListViewGetItemText(FRMFiltersSelectedListView, i, 0).Get_String()) == 0)
        {
            return(1);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Fill the list of filters to be used
void Fill_Filters_List(void)
{
    int i;

    for(i = 0; i < ListViewItemCount(FRMFiltersSelectedListView); i++)
    {
        Chosen_Filters.Add(ListViewGetItemText(FRMFiltersSelectedListView, i, 0).Get_String());
    }
}

// -----------------------------------------------------------------------
// Fill the profiles combo
void Fill_Filter_Profils(void)
{
    int i = 0;
    CStr Filter_Name;
    
    ComboBoxReset(FRMFiltersCombo);
    for(i = 0; i <= 999; i++)
    {
        Filter_Name = IniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), FiltersIniFile);
        if(Filter_Name.Len() == 0) break;
        ComboBoxAddItem(FRMFiltersCombo, Filter_Name, -1);
    }
}

// -----------------------------------------------------------------------
// Save a filter profile
int Save_Filter_Profil(CStr Profile_Name)
{
    CStr Profile_Name_To_Register;
    int i = 0;
    long NoReg = 0;
    CStr BufString;

    Profile_Name_To_Register = StringReplace(Profile_Name.Trim(), "\t", "", 1, -1, Binary_Compare);
    if(Profile_Name_To_Register.Len() == 0) return(0);
    if(ListViewItemCount(FRMFiltersSelectedListView) == 0)
    {
        MiscMsgBox(FRMFiltershWnd, "No selected filters.", MB_ERROR, Requesters);
        SetFocus(FRMFiltersListView);
        return(1);
    }
    NoReg = 0;
    for(i = 0; i <= 999; i++)
    {
        BufString = IniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), FiltersIniFile);
        if(BufString.Len() == 0) break;
        if(_strcmpi(BufString.Get_String(), Profile_Name_To_Register.Get_String()) == 0)
        {
            NoReg = 1;
            if(MiscMsgBox(FRMFiltershWnd, "Profile already exists. Overwrite it ?", MB_QUESTION, Requesters) == IDNO)
            {
                NoReg = 2;
                break;
            }
            else
            {
                break;
            }
        }
    }
    switch(NoReg)
    {
        // New profil
        case 0:
            // Register name
            IniWriteKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), Profile_Name_To_Register, FiltersIniFile);
            break;
        case 2:
            return(1);
            break;
    }
    // Save output type
    if(ToolBarIsButtonChecked(FRMFiltersToolbar, OUTPUT_WINDOW) == 1)
    {
        IniWriteKey(Profile_Name, "OutputType", 0, FiltersIniFile);
    }
    else
    {
        IniWriteKey(Profile_Name, "OutputType", 1, FiltersIniFile);
    }
    // Write selected filters
    for(i = 0; i < ListViewItemCount(FRMFiltersSelectedListView); i++)
    {
        IniWriteKey(Profile_Name, "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), ListViewGetItemText(FRMFiltersSelectedListView, i, 0), FiltersIniFile);
    }
    Fill_Filter_Profils();
    ControlSetText(FRMFiltersCombo, Profile_Name);
    MiscMsgBox(FRMFiltershWnd, "Profile '" + (CStr) Profile_Name + (CStr) "' saved.", MB_INFORMATION, Requesters);
    return(1);
}

// -----------------------------------------------------------------------
// Load a filter profile
void Load_Filter_Profil(CStr Profile_Name)
{
    CStr Filter_Name;
    int i;

    // Remove previous entries from the list
    ListViewClear(FRMFiltersSelectedListView);
    for(i = 0; i <= 999; i++)
    {
        // Read the datas
        Filter_Name = IniReadKey(Profile_Name, "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), FiltersIniFile);
        if(Filter_Name.Len() == 0) break;
        // Fill the list
        ListViewAddItem(FRMFiltersSelectedListView, Filter_Name, i, -1);
    }   
}

// -----------------------------------------------------------------------
// Run a filter from it's profile number
void Run_Filter_Profile(long Filter_Index, long Open_Mode)
{
    CStr Profile_Name;
    LPFILTER_ENTRY Filter_Profile;
    int i;

    Filter_Profile = FiltersArray.Get(Filter_Index)->Content;
    Chosen_Filters.Erase();
    // Fill the list of filters with the entries found in the ini file
    for(i = 0; i <= 999; i++)
    {
        Profile_Name = IniReadKey(Filter_Profile->Name, "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), FiltersIniFile);
        if(Profile_Name.Len() == 0) break;
        Chosen_Filters.Add(Profile_Name.Get_String());
    }
    // Get output type
    Filter_Output_Type = Filter_Profile->Type;
    switch(Open_Mode)
    {
        case FILTER_OPEN_MODE:
            OpenFileAsDB();
            break;
        case FILTER_INCLUDE_MODE:
            // Conditions:
            // Output mode must be in window
            // There must be a least 1 child window opened
            // The current opened child window must be writable
            if(Filter_Output_Type == FILTER_OUTPUT_WINDOW)
            {
                if(NbForms == 0 || IsChildReadOnly(CurrentForm) == 1)
                {
                    switch(MiscMsgBox(hMDIform.hWnd, "No suitable windows found.\rRedirect output to file ?",
                           MB_ICONQUESTION | MB_YESNO, Requesters))
                    {
                        case IDYES:
                            Filter_Output_Type = FILTER_OUTPUT_FILE;
                            break;
                        case IDNO:
                            return;
                    }
                }
            }
            IncludeFileAsDB();
            break;
    }
}
