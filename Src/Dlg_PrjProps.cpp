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
// Dlg_PrjProps.cpp: Project properties form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ProjectsBuilder.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "Dlg_Constants.h"
#include "Dlg_PrjProps.h"

// -----------------------------------------------------------------------
// Variables
HWND FRMPrjPropshWnd;
HWND FRMPrjPropsFr1;
HWND FRMPrjPropsFr2;
HWND FRMPrjPropsFr3;
HWND FRMPrjPropsTxtTitle;
HWND FRMPrjPropsFileType;
HWND FRMPrjPropsSetIcon;
HWND FRMPrjPropsIcon;
HWND FRMPrjPropsConstants;
HANDLE FRMPrjPropsIconhandle;
HWND FRMPrjPropsTxtMajor;
HWND FRMPrjPropsTxtMinor;
HWND FRMPrjPropsTxtRevision;
HWND FRMPrjPropsTxtRelease;
HWND FRMPrjPropsOS;
HWND FRMPrjPropsTypeProp;
HWND FRMPrjPropsTypeTxt;

HWND FRMPrjPropsIncludeVersion;
HWND FRMPrjPropsIncludeXPManifest;

HWND FRMPrjPropsReg;
HWND FRMPrjPropsOk;
HWND FRMPrjPropsCancel;

CStr LastPrjIconDir;
long LastListBoxIndex;

CStr TmpPrj_Comments;
CStr TmpPrj_CompanyName;
CStr TmpPrj_FileDescription;
CStr TmpPrj_LegalCopyRight;
CStr TmpPrj_LegalTrademarks;
CStr TmpPrj_ProductName;

CStr PrjDir;
CStr NewIconFile;

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMPrjPropsProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT CreatePs;
    CStr BufString;

    switch(uMsg)
    {
        case WM_INITDIALOG:
            FRMPrjPropshWnd = hwndDlg;
            ControlSetText(hwndDlg, "Project properties");
            FRMPrjPropsFr1 = CreateFrame(2, -1, 202, 108, hwndDlg, "Application", 0, 0, 0);
            FRMPrjPropsFr2 = CreateFrame(206, -1, 194, 108, hwndDlg, "Version", 0, 0, 0);
            FRMPrjPropsFr3 = CreateFrame(2, 108, 398, 94, hwndDlg, "Version informations", 14, &FRMPrjPropsFrame3, 0);
            CreateLabel(5, 21 + 20, 35, 15, FRMPrjPropsFr1, "Title :", 0, 0, 0, 0);
            CreateLabel(5, 41 + 25, 52, 15, FRMPrjPropsFr1, "File type :", 0, 0, 0, 0);
            CreateLabel(131, 41 + 25, 34, 15, FRMPrjPropsFr1, "Icon :", 0, 0, 0, 0);
            FRMPrjPropsOk = CreateButton(243, 214, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMPrjPropsCancel = CreateButton(322, 214, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMPrjPropsReg = CreateButton(159, 214, 77, 23, hwndDlg, "Register", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMPrjPropsConstants = CreateButton(3, 214, 77, 23, hwndDlg, "Constants", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FRMPrjPropsIncludeVersion = CreateCheckBox(7, 18, 140, 15, FRMPrjPropsFr1, "Include version resource", 5, 0, WS_TABSTOP | WS_GROUP, 0);
            FRMPrjPropsTxtTitle = CreateTextBox(36, 18 + 20, 159, 20, FRMPrjPropsFr1, "", 6, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
            FRMPrjPropsFileType = CreateComboBox(5, 56 + 25, 123, 150, FRMPrjPropsFr1, "", 7, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
            ComboBoxAddItem(FRMPrjPropsFileType, "UNKNOWN", -1);
            ComboBoxAddItem(FRMPrjPropsFileType, "APP", -1);
            ComboBoxAddItem(FRMPrjPropsFileType, "DLL", -1);
            ComboBoxAddItem(FRMPrjPropsFileType, "DRV", -1);
            ComboBoxAddItem(FRMPrjPropsFileType, "FONT", -1);
            ComboBoxAddItem(FRMPrjPropsFileType, "VXD", -1);
            ComboBoxAddItem(FRMPrjPropsFileType, "STATIC_LIB", -1);
            ComboBoxSetIndex(FRMPrjPropsFileType, 0);
            FRMPrjPropsSetIcon = CreateButton(133, 56 + 25, 25, 21, hwndDlg, "...", 8, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            // Current icon
            if(ProjectTypeScheduledImg.Len()) BufString = ProjectTypeScheduledImg;
            else BufString = ProjectDir + (CStr) "\\" + (CStr) ProjectTypeImg;
            FRMPrjPropsIconhandle = LoadImage(0, BufString.Get_String(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
            FRMPrjPropsIcon = CreatePictureBox(163, 43 + 25, 35, 34, FRMPrjPropsFr1, FRMPrjPropsIconhandle, IMAGE_ICON, 9, 0, SS_CENTERIMAGE);
            CreateLabel(7, 15, 52, 13, FRMPrjPropsFr2, "Major :", 0, 0, 0, 0);
            CreateLabel(99, 15, 52, 13, FRMPrjPropsFr2, "Minor :", 0, 0, 0, 0);
            CreateLabel(7, 35, 52, 13, FRMPrjPropsFr2, "Revision :", 0, 0, 0, 0);
            CreateLabel(99, 35, 52, 13, FRMPrjPropsFr2, "Release :", 0, 0, 0, 0);
            FRMPrjPropsTxtMajor = CreateTextBox(59, 12, 33, 20, FRMPrjPropsFr2, "", 10, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
            FRMPrjPropsTxtMinor = CreateTextBox(151, 12, 33, 20, FRMPrjPropsFr2, "", 11, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
            FRMPrjPropsTxtRevision = CreateTextBox(59, 32, 33, 20, FRMPrjPropsFr2, "", 12, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
            FRMPrjPropsTxtRelease = CreateTextBox(151, 32, 33, 20, FRMPrjPropsFr2, "", 13, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
            CreateLabel(7, 61, 31, 15, FRMPrjPropsFr2, "OS :", 0, 0, 0, 0);
            FRMPrjPropsOS = CreateComboBox(34, 57, 150, 150, FRMPrjPropsFr2, "", 14, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
            ComboBoxAddItem(FRMPrjPropsOS, "UNKNOWN", -1);
            ComboBoxAddItem(FRMPrjPropsOS, "DOS", -1);
            ComboBoxAddItem(FRMPrjPropsOS, "NT", -1);
            ComboBoxAddItem(FRMPrjPropsOS, "WINDOWS16", -1);
            ComboBoxAddItem(FRMPrjPropsOS, "WINDOWS32", -1);
            ComboBoxAddItem(FRMPrjPropsOS, "DOS_WINDOWS16", -1);
            ComboBoxAddItem(FRMPrjPropsOS, "DOS_WINDOWS32", -1);
            ComboBoxAddItem(FRMPrjPropsOS, "NT_WINDOWS32", -1);
            ComboBoxSetIndex(FRMPrjPropsOS, 0);
            FRMPrjPropsIncludeXPManifest = CreateCheckBox(16, 85, 170, 15, FRMPrjPropsFr2, "Include Windows XP manifest", 15, 0, WS_TABSTOP | WS_GROUP, 0);
            CreateLabel(5, 16, 52, 13, FRMPrjPropsFr3, "Type :", 0, 0, 0, 0);
            FRMPrjPropsTypeProp = CreateListBox(5, 30, 140, 59, FRMPrjPropsFr3, 16, 0, 0, WS_TABSTOP | WS_HSCROLL, WS_EX_STATICEDGE);
            ListBoxAddItem(FRMPrjPropsTypeProp, "Comments", -1);
            ListBoxAddItem(FRMPrjPropsTypeProp, "Company name", -1);
            ListBoxAddItem(FRMPrjPropsTypeProp, "File description", -1);
            ListBoxAddItem(FRMPrjPropsTypeProp, "Legal copyright", -1);
            ListBoxAddItem( FRMPrjPropsTypeProp, "Legal trademarks", -1);
            ListBoxAddItem(FRMPrjPropsTypeProp, "Product name", -1);
            ListBoxSetIndex(FRMPrjPropsTypeProp, 0);
            FRMPrjPropsTypeTxt = CreateTextBox(147, 13 - 2, 245, 79, FRMPrjPropsFr3, "", 17, &FRMPrjPropsTypeTxtHook, WS_TABSTOP | ES_MULTILINE | ES_AUTOVSCROLL | WS_VSCROLL | ES_WANTRETURN, WS_EX_STATICEDGE);
            LoadUserInfos();
            ControlSetText(FRMPrjPropsTxtMajor, Prj_Major);
            ControlSetText(FRMPrjPropsTxtMinor, Prj_Minor);
            ControlSetText(FRMPrjPropsTxtRevision, Prj_Revision);
            ControlSetText(FRMPrjPropsTxtRelease, Prj_Release);
            CheckBoxSetState(FRMPrjPropsIncludeVersion, Prj_IncludeVersion);
            CheckBoxSetState(FRMPrjPropsIncludeXPManifest, Prj_IncludeManifest);
            ComboBoxSetIndex(FRMPrjPropsOS, Prj_OS);
            ComboBoxSetIndex(FRMPrjPropsFileType, Prj_FileType);
            ControlSetText(FRMPrjPropsTxtTitle, ProjectTitle);
            TmpPrj_Comments = Prj_Comments;
            if(Prj_CompanyName.Len()) TmpPrj_CompanyName = Prj_CompanyName;
            TmpPrj_FileDescription = Prj_FileDescription;
            if(Prj_LegalCopyRight.Len()) TmpPrj_LegalCopyRight = Prj_LegalCopyRight;
            if(Prj_LegalTrademarks.Len()) TmpPrj_LegalTrademarks = Prj_LegalTrademarks;
            TmpPrj_ProductName = Prj_ProductName;
            ControlSetText(FRMPrjPropsTypeTxt, TmpPrj_Comments);
            FreezeTimer = 1;
            return(1);
        case WM_COMMAND:
            if((HWND) lParam == FRMPrjPropsOk)
            {
                ProjectTypeScheduledImg = NewIconFile;
                SaveProjectProperties();
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsConstants)
            {
                CreateConstants(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsReg)
            {
                RegisterUserInfos();
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsSetIcon)
            {
                SetProjectIcon();
                return(0);
            }
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &CreatePs);
            GDIDrawHorzSep(hwndDlg, 0, 207, 402);
            EndPaint(hwndDlg, &CreatePs);
            break;
        case WM_CLOSE:
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Load user infos from main ini file
void LoadUserInfos(void)
{
    TmpPrj_CompanyName = StringReplace(IniReadKey("UserInfos", "CompanyName", MainIniFile), TmpPrj_CompanyName.Chr(2), TmpPrj_CompanyName.Chr(13) + TmpPrj_CompanyName.Chr(10), 1, -1, Binary_Compare);
    TmpPrj_LegalCopyRight = StringReplace(IniReadKey("UserInfos", "LegalCopyRight", MainIniFile), TmpPrj_LegalCopyRight.Chr(2), TmpPrj_LegalCopyRight.Chr(13) + TmpPrj_LegalCopyRight.Chr(10), 1, -1, Binary_Compare);
    TmpPrj_LegalTrademarks = StringReplace(IniReadKey("UserInfos", "LegalTrademarks", MainIniFile), TmpPrj_LegalTrademarks.Chr(2), TmpPrj_LegalTrademarks.Chr(13) + TmpPrj_LegalTrademarks.Chr(10), 1, -1, Binary_Compare);
}

// -----------------------------------------------------------------------
// Register user infos in main ini file
void RegisterUserInfos(void)
{
    IniWriteKey("UserInfos", "CompanyName", StringReplace(TmpPrj_CompanyName, TmpPrj_CompanyName.Chr(13) + TmpPrj_CompanyName.Chr(10), TmpPrj_CompanyName.Chr(2), 1, -1, Binary_Compare), MainIniFile);
    IniWriteKey("UserInfos", "LegalCopyRight", StringReplace(TmpPrj_LegalCopyRight, TmpPrj_LegalCopyRight.Chr(13) + TmpPrj_LegalCopyRight.Chr(10), TmpPrj_LegalCopyRight.Chr(2), 1, -1, Binary_Compare), MainIniFile);
    IniWriteKey("UserInfos", "LegalTrademarks", StringReplace(TmpPrj_LegalTrademarks, TmpPrj_LegalTrademarks.Chr(13) + TmpPrj_LegalTrademarks.Chr(10), TmpPrj_LegalTrademarks.Chr(2), 1, -1, Binary_Compare), MainIniFile);
    MiscMsgBox(hMDIform.hWnd, "User infos registered.", MB_INFORMATION, Requesters);
}

// -----------------------------------------------------------------------
// Version frame
LRESULT CALLBACK FRMPrjPropsFrame3(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPrjPropsTypeProp)
            {
                switch(ListBoxGetSelItemIndex(FRMPrjPropsTypeProp))
                {
                    case 0:
                        ControlSetText(FRMPrjPropsTypeTxt, TmpPrj_Comments);
                        LastListBoxIndex = 0;
                        break;
                    case 1:
                        ControlSetText(FRMPrjPropsTypeTxt, TmpPrj_CompanyName);
                        LastListBoxIndex = 1;
                        break;
                    case 2:
                        ControlSetText(FRMPrjPropsTypeTxt, TmpPrj_FileDescription);
                        LastListBoxIndex = 2;
                        break;
                    case 3:
                        ControlSetText(FRMPrjPropsTypeTxt, TmpPrj_LegalCopyRight);
                        LastListBoxIndex = 3;
                        break;
                    case 4:
                        ControlSetText(FRMPrjPropsTypeTxt, TmpPrj_LegalTrademarks);
                        LastListBoxIndex = 4;
                        break;
                    case 5:
                        ControlSetText(FRMPrjPropsTypeTxt, TmpPrj_ProductName);
                        LastListBoxIndex = 5;
                        break;
                }
                return(0);
            }
            else if((HWND) lParam == FRMPrjPropsTypeTxt)
            {
                switch(ControlGetNotifiedCommand(wParam))
                {
                    case EN_CHANGE:
                        switch(ListBoxGetSelItemIndex(FRMPrjPropsTypeProp))
                        {
                            case 0:
                                TmpPrj_Comments = ControlGetText(FRMPrjPropsTypeTxt);
                                break;
                            case 1:
                                TmpPrj_CompanyName = ControlGetText(FRMPrjPropsTypeTxt);
                                break;
                            case 2:
                                TmpPrj_FileDescription = ControlGetText(FRMPrjPropsTypeTxt);
                                break;
                            case 3:
                                TmpPrj_LegalCopyRight = ControlGetText(FRMPrjPropsTypeTxt);
                                break;
                            case 4:
                                TmpPrj_LegalTrademarks = ControlGetText(FRMPrjPropsTypeTxt);
                                break;
                            case 5:
                                TmpPrj_ProductName = ControlGetText(FRMPrjPropsTypeTxt);
                                break;
                        }
                        break;
                }
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// A hack to avoid some repainting problems with the parent frame
LRESULT CALLBACK FRMPrjPropsTypeTxtHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_KEYDOWN:
            if(wParam == 27)
            {
                ControlSendMessage(FRMPrjPropshWnd, WM_CLOSE, (long) FRMPrjPropsCancel);
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Change the icon associated with the project
void SetProjectIcon(void)
{
    CStr LdFile;
    CStr OpFilters;
    CStr IconDir;

    OpFilters = "Icon files (*.ico)|*.ico";
    LdFile = ComDlgGetOpenFileName(FRMPrjPropshWnd, OpFilters, LastPrjIconDir, 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    IconDir = FileGetDirectory(LdFile);
    if(strcmp(IconDir.Right(1).Get_String(), "\\") == 0) IconDir = IconDir.Left(IconDir.Len() - 1);
    LastPrjIconDir = IconDir;
    NewIconFile = LdFile;
    PictureBoxChangeIcon(FRMPrjPropsIcon, (HICON) LoadImage(0, LdFile.Get_String(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE));
}

// -----------------------------------------------------------------------
// Dispatch the properties
void SaveProjectProperties(void)
{
    CStr BufString;

    Prj_Major = 0;
    Prj_Minor = 0;
    Prj_Revision = 0;
    Prj_Release = 0;
    if(ControlGetText(FRMPrjPropsTxtMajor).Len() != 0) Prj_Major = ControlGetText(FRMPrjPropsTxtMajor).Get_Long();
    if(ControlGetText(FRMPrjPropsTxtMinor).Len() != 0) Prj_Minor = ControlGetText(FRMPrjPropsTxtMinor).Get_Long();
    if(ControlGetText(FRMPrjPropsTxtRevision).Len() != 0) Prj_Revision = ControlGetText(FRMPrjPropsTxtRevision).Get_Long();
    if(ControlGetText(FRMPrjPropsTxtRelease).Len() != 0) Prj_Release = ControlGetText(FRMPrjPropsTxtRelease).Get_Long();
    Prj_IncludeVersion = CheckBoxGetState(FRMPrjPropsIncludeVersion);
    Prj_IncludeManifest = CheckBoxGetState(FRMPrjPropsIncludeXPManifest);
    if(ComboBoxGetIndex(FRMPrjPropsOS) == -1) ComboBoxSetIndex(FRMPrjPropsOS, 0);
    Prj_OS = ComboBoxGetIndex(FRMPrjPropsOS);
    if(ComboBoxGetIndex(FRMPrjPropsFileType) == -1) ComboBoxSetIndex(FRMPrjPropsFileType, 0);
    Prj_FileType = ComboBoxGetIndex(FRMPrjPropsFileType);
    ProjectTitle = ControlGetText(FRMPrjPropsTxtTitle);
    Prj_Comments = TmpPrj_Comments;
    Prj_CompanyName = TmpPrj_CompanyName;
    Prj_FileDescription = TmpPrj_FileDescription;
    Prj_LegalCopyRight = TmpPrj_LegalCopyRight;
    Prj_LegalTrademarks = TmpPrj_LegalTrademarks;
    Prj_ProductName = TmpPrj_ProductName;
    ProjectModified = TRUE;
}
