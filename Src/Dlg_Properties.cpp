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
// Dlg_Properties.cpp: Properties form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MDI_Form.h"
#include "MsgCodeMax.h"
#include "MiscFunctions.h"
#include "Globals.h"
#include "Statusbar.h"
#include "Menus.h"
#include "Languages.h"
#include "ImageLists.h"
#include "ConstRes.h"
#include "ConstCodeMax.h"
#include "Splitters.h"
#include "APIDb.h"
#include "Crypt.h"
#include "MDI_Childs.h"
#include "Dlg_Properties.h"
#include "Dlg_EnterValue.h"
#include "Dlg_Registry.h"

// -----------------------------------------------------------------------
// Constants
#define TOOLTIP_ID 1000
#define MAX_TABS 4

#define EXT_REPLACE 1
#define EXT_INSERT 2
#define EXT_DELETE 3

#define BACKUP_SELDIR 4
#define BACKUP_DELETE 5

#define PROFILE_LOAD 6
#define PROFILE_SAVE 7

#define FOLDER_CHOOSE 8

#define FTPACC_CREATE 9
#define FTPACC_SAVE 10
#define FTPACC_DELETE 11

#define DEBUGGER_VALID 12

#define EXT_REPLACE_LANG 13
#define EXT_INSERT_LANG 14
#define EXT_DELETE_LANG 15
#define EXT_UP_LANG 16
#define EXT_DOWN_LANG 17

// -----------------------------------------------------------------------
// Variables
HWND FRMPropertiesSysTab;
HWND FRMPropertiesCmdOk;
HWND FRMPropertiesCmdCancel;
HWND FRMPropertieshwnd;

// Controls (Page 1)
HWND FRMPropertiesPageEditor;
HWND FRMPropertiesPageFileExtensions;
HWND FRMPropertiesPageOther;
HWND FRMPropertiesPageFont;
HWND FRMPropertiesPageAutoSave;
HWND FRMPropertiesPageEditor2;
HWND FRMPropertiesPageLineNumbering;
HWND FRMPropertiesPageBackup;
HWND FRMPropertiesChkLeftMargin;
HWND FRMPropertiesChkWhiteSpaces;
HWND FRMPropertiesChkHighlightLine;
HWND FRMPropertiesChkCursorwrap;
HWND FRMPropertiesChkSyntaxHighlighting;
HWND FRMPropertiesChkLanguageScopeIndent;
HWND FRMPropertiesChkTextDragAndDrop;
HWND FRMPropertiesChkSmoothScrolling;
HWND FRMPropertiesChkLineNumberTooltip;
HWND FRMPropertiesChkCaretAlwaysVisible;
HWND FRMPropertiesChkTabsToSpaces;
HWND FRMPropertiesChkAutoCorrectKeywords;
HWND FRMPropertiesChkHorizontalSplitter;
HWND FRMPropertiesChkDetectChange;
HWND FRMPropertiesChkRememberState;
HWND FRMPropertiesChkUNIX;
HWND FRMPropertiesChkAutoZoom;
HWND FRMPropertiesChkOutputDates;
HWND FRMPropertiesCbLines;
HWND FRMPropertiesTxtLines;
HWND FRMPropertiesTxtFileExt;
HWND FRMPropertiesLbFileExt;
HWND FRMPropertiesTbFileExt;
HWND FRMPropertiesLbFont;
HWND FRMPropertiesCmdFontChoose;
HWND FRMPropertiesTabulations;
HWND FRMPropertiesRecentFiles;
HWND FRMPropertiesRecentProjects;
HWND FRMPropertiesCbSave;
HWND FRMPropertiesChkVerbose;
HWND FRMPropertiesChkSaveAll;
HWND FRMPropertiesChkMultiplesInst;
HWND FRMPropertiesChkRemember;
HWND FRMPropertiesChkBakFiles;
HWND FRMPropertiesChkDecorateBak;
HWND FRMPropertiesChkAcceptFiles;
HWND FRMPropertiesChkAutoMax;
HWND FRMPropertiesChkAutoClean;
HWND FRMPropertiesChkMinimize;
HWND FRMPropertiesChkOnTop;
HWND FRMPropertiesChkSaveBk;
HWND FRMPropertiesTxtBackupDir;
HWND FRMPropertiesTbBackup;

// Controls (Page 2)
HWND FRMPropertiesPageColors;
HWND FRMPropertiesPageAPIHelp;
HWND FRMPropertiesPageSDKHelp;
HWND FRMPropertiesPageCodeMax;
HWND FRMPropertiesPageDDKHelp;
HWND FRMPropertiesPageDirectXSDKHelp;
HWND FRMPropertiesPageExtrasLayouts;
HWND FRMPropertiesCodeMax;
HWND FRMPropertiesCbColors;
HWND FRMPropertiesTbColors;
HWND FRMPropertiesColBackGround;
HWND FRMPropertiesColLineNumber;
HWND FRMPropertiesColNumber;
HWND FRMPropertiesColStrings;
HWND FRMPropertiesColComments;
HWND FRMPropertiesColKeywords;
HWND FRMPropertiesColDirectives;
HWND FRMPropertiesColAPIHighlighting;
HWND FRMPropertiesColExtraHighLight1;
HWND FRMPropertiesColExtraHighLight2;
HWND FRMPropertiesColExtraHighLight3;
HWND FRMPropertiesColLuminosity;
HWND FRMPropertiesColToolTip;
HWND FRMPropertiesColNormalText;
HWND FRMPropertiesColMargin;
HWND FRMPropertiesColHighlightText;
HWND FRMPropertiesColRegisters;
HWND FRMPropertiesColExtraFunctions;
HWND FRMPropertiesColScopeKeywords;
HWND FRMPropertiesColSeparators;
HWND FRMPropertiesChkItalic;
HWND FRMPropertiesTxtWindowsAPI;
HWND FRMPropertiesCmdWindowsAPI;
HWND FRMPropertiesTxtMSDN;
HWND FRMPropertiesCmdMSDN;
HWND FRMPropertiesTxtDDK;
HWND FRMPropertiesCmdDDK;
HWND FRMPropertiesTxtDirectXSDK;
HWND FRMPropertiesCmdDirectXSDK;
HWND FRMPropertiesCbDefLang;
HWND FRMPropertiesCmdRegs;

// Controls (Page 3)
HWND FRMPropertiesPageFolders;
HWND FRMPropertiesPageProjects;
HWND FRMPropertiesPageProtoGen;
HWND FRMStepListView;
HWND FRMPropertiesTxtROOTDIR;
HWND FRMPropertiesTxtBINDIR;
HWND FRMPropertiesTxtINCLUDEDIR;
HWND FRMPropertiesTxtLIBDIR;
HWND FRMPropertiesTxtHELPDIR;
HWND FRMPropertiesTbROOTDIR;
HWND FRMPropertiesTbBINDIR;
HWND FRMPropertiesTbINCLUDEDIR;
HWND FRMPropertiesTbLIBDIR;
HWND FRMPropertiesTbHELPDIR;
HWND FRMPropertiesTxtPROJECTSDIR;
HWND FRMPropertiesTbPROJECTSDIR;
HWND FRMPropertiesTxtArgs;
HWND FRMPropertiesCbDebugger;
HWND FRMPropertiesCmdDebugger;
HWND FRMPropertiesTbValidDebugger;
HWND FRMPropertiesCbSkin;
HWND FRMPropertiesTxtGenProg;
HWND FRMPropertiesCmdGenProg;
HWND FRMPropertiesTxtGenSource;
HWND FRMPropertiesTxtGenDest;

// Controls (Page 4)
HWND FRMPropertiesPageFTPManage;
HWND FRMPropertiesPageAccountInfos;
HWND FRMPropertiesPageTransfersTypes;
HWND FRMPropertiesCbAccounts;
HWND FRMPropertiesTbAccounts;
HWND FRMPropertiesTxtIP;
HWND FRMPropertiesChkAnonymous;
HWND FRMPropertiesTxtUserName;
HWND FRMPropertiesTxtPassword;
HWND FRMPropertiesCbPasswordType;
HWND FRMPropertiesTxtInitialPath;
HWND FRMPropertiesTxtFTPPort;
HWND FRMPropertiesTxtFTPTimeOut;
HWND FRMPropertiesRBASCII;
HWND FRMPropertiesRBBinary;
HWND FRMPropertiesChkPassive;

// Controls (Page 5)
HWND FRMPropertiesPageCodingHelp;
HWND FRMPropertiesPageExtrasLayouts2;
HWND FRMPropertiesChkWinAPITooltipInfos;
HWND FRMPropertiesChkUseFncDB;
HWND FRMPropertiesChkUseConstDB;
HWND FRMPropertiesTxtAPILines;
HWND FRMPropertiesChkShowSplash;
HWND FRMPropertiesChkTVSingle;
HWND FRMPropertiesChkTVTool;
HWND FRMPropertiesChkRecentDoc;
HWND FRMPropertiesChkAutoOut;
HWND FRMPropertiesChkUseFileDir;
HWND FRMPropertiesChkCreateDrop;
HWND FRMPropertiesChkSaveProjectState;
HWND FRMPropertiesChkConfirmBeforeExit;
HWND FRMPropertiesPageLanguages;
HWND FRMPropertiesTbLanguages;
HWND FRMPropertiesLbLanguages;
HWND FRMPropertiesTxtLanguageFile;
HWND FRMPropertiesTxtLanguageExtensions;
HWND FRMPropertiesLbLanguageFile;
HWND FRMPropertiesLbLanguageExtensions;
HWND FRMPropertiesLbLanguageDescName;
HWND FRMPropertiesLbLanguageDescCopyright;
HWND FRMPropertiesLbLanguageDescComment;

// Other variables
CStr FRMPropertiesRetVal;
HFONT LblFont;

// Profiles variables
COLORREF Prof_BackColor;
COLORREF Prof_NormText;
COLORREF Prof_HighText;
COLORREF Prof_MarginColor;
COLORREF Prof_LinesColor;
COLORREF Prof_NumbersColorValue;
COLORREF Prof_StringsColorValue;
COLORREF Prof_CommentsColorValue;
// Scope keywords
COLORREF Prof_KeyWordsColorValue;
COLORREF Prof_APIHighColor;
// FRMPropertiesColKeywords
COLORREF Prof_ColorAValue;
// FRMPropertiesColSeparators
COLORREF Prof_ColorBValue;
// FRMPropertiesColDirectives
COLORREF Prof_ColorCValue;
// FRMPropertiesColRegisters
COLORREF Prof_ColorDValue;
// FRMPropertiesColExtraFunctions
COLORREF Prof_ColorEValue;
// FRMPropertiesColExtraHighLight1
COLORREF Prof_ColorFValue;
// FRMPropertiesColExtraHighLight2
COLORREF Prof_ColorGValue;
// FRMPropertiesColExtraHighLight3
COLORREF Prof_ColorHValue;

// Used for luminosity
COLORREF Orig_Prof_BackColor;
COLORREF Orig_Prof_NormText;
COLORREF Orig_Prof_HighText;
COLORREF Orig_Prof_MarginColor;
COLORREF Orig_Prof_LinesColor;
COLORREF Orig_Prof_NumbersColorValue;
COLORREF Orig_Prof_StringsColorValue;
COLORREF Orig_Prof_CommentsColorValue;
COLORREF Orig_Prof_KeyWordsColorValue;
COLORREF Orig_Prof_APIHighColor;
COLORREF Orig_Prof_ColorAValue;
COLORREF Orig_Prof_ColorBValue;
COLORREF Orig_Prof_ColorCValue;
COLORREF Orig_Prof_ColorDValue;
COLORREF Orig_Prof_ColorEValue;
COLORREF Orig_Prof_ColorFValue;
COLORREF Orig_Prof_ColorGValue;
COLORREF Orig_Prof_ColorHValue;

long Prof_ItalicComment;
long Prof_Luminosity;

long FRMPropertiesFirstTab;

CM_RANGE LineRange;
CStr NewAccountName;
long FRMPropertiesInFtpCreation;

CStr PrefsGenProg;
CStr PrefsGenCmd;
CStr PrefsGenDest;

CStr PrfCurrentSkin;
CList <char *> PrgSkinArray;
CList <HWND> PrgSkinLabels;
CList <HWND> PrgSkinControls;
CList <char *> PrgSkinControlsDest;
CList <HWND> SkinListViews;

HWND Assigned_ToolTip;

// -----------------------------------------------------------------------
// Private functions
LRESULT CALLBACK FRMPropertiesFrameGeneral(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameColors(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameExtensions(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesCMHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameCodeMax(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameManagment(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK RadioHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameBuilding(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK PropStepListViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK FRMPropertiesFrameLanguages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
CStr Get_LanguageFileName(int Idx);
CStr Get_LanguageExtensions(int Idx);
void AddNewDebugger(HWND hCombo, CStr Debugger);
void Dispatch_LanguageName(int Idx);
int Is_LanguagePresent(CStr Name);
CStr Get_Store_LanguageName(void);
CStr Get_Store_LanguageExtensions(void);
int Save_Language_Idx(HWND hwnd);

// -----------------------------------------------------------------------
// Display properties according to FRMPropertiesFirstTab variable
void DisplayProperties(void)
{
    CreateModalDialog(-1, -1, 527, 370, hMDIform.hWnd, FRMPropertiesProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    if(RefreshProperties == 1) RefreshChilds();
    if(NbForms != 0) RefreshStatusBar(CurrentForm);
    else ClearStatusBarParts();
}

// -----------------------------------------------------------------------
// Initialize Form
int CALLBACK FRMPropertiesProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT CreatePs;
    int i = 0;
    CStr BufString;
    long LbOldIdx = 0;
    CStr Name;
    CStr Extensions;
    CStr Name1;
    CStr Name2;

    switch(uMsg)
    {
        case WM_SYSCOLORCHANGE:
            ListViewSetBackColor(FRMStepListView, GetSysColor(COLOR_WINDOW));
            for(i = 0; i < SkinListViews.Amount(); i++)
            {
                if(SkinListViews.Get(i)->Content != 0) ListViewSetBackColor(SkinListViews.Get(i)->Content, GetSysColor(COLOR_WINDOW));
            }
            break;
        case WM_INITDIALOG:
            FRMPropertiesInFtpCreation = 0;
            RefreshProperties = 0;
            FRMPropertieshwnd = hwndDlg;
            ControlSetText(hwndDlg, AppTitle + (CStr) " properties");
            FRMPropertiesSysTab = CreateSysTab(2, 1, 520, 334, hwndDlg, 0, 0, GlobalImageList1, WS_TABSTOP | TCS_BUTTONS | TCS_FLATBUTTONS | TCS_HOTTRACK);
            SysTabSetSeparators(FRMPropertiesSysTab, 0);
            SysTabAddItem(FRMPropertiesSysTab, "General", 0, ICON_PROPERTIES);
            SysTabAddItem(FRMPropertiesSysTab, "Editor layouts", 1, ICON_COLORS);
            SysTabAddItem(FRMPropertiesSysTab, "Compiler/Assembler", 2, ICON_ASM);
            SysTabAddItem(FRMPropertiesSysTab, "FTP accounts", 3, ICON_EARTH);
            SysTabAddItem(FRMPropertiesSysTab, "Extras options", 4, ICON_STAR);
            CreatePropPage1(hwndDlg);
            CreatePropPage2(hwndDlg);
            CreatePropPage3(hwndDlg);
            CreatePropPage4(hwndDlg);
            CreatePropPage5(hwndDlg);
            BuildFTP(FRMPropertiesCbAccounts);
            FRMPropertiesCmdOk = CreateButton(368, 344, 77, 23, hwndDlg, "Ok", 1, 0, 0, 0, BS_DEFPUSHBUTTON | WS_GROUP | WS_TABSTOP, Buttons_StaticEdge);
            FRMPropertiesCmdCancel = CreateButton(447, 344, 77, 23, hwndDlg, "Cancel", 2, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
            FillProfil();
            FillProperties();
            Fillskins();
            FillCurrentSkin();
            FillDefLanguage();
            FillLanguagesList();
            FRMPropertiesInitCodeMax(FRMPropertiesCodeMax);
            SysTabSetCurrentItem(FRMPropertiesSysTab, FRMPropertiesFirstTab);
            FRMPropertiesChangeCurrentTab();
            FreezeTimer = 1;
            return(1);
        case WM_SIZE:
            ListBoxSetHorzScrollWidth(FRMPropertiesLbFileExt, ControlWidth(FRMPropertiesLbFileExt) * 2);
            ListBoxSetHorzScrollWidth(FRMPropertiesLbLanguages, ControlWidth(FRMPropertiesLbLanguages) * 2);
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case TCN_SELCHANGE:
                    FRMPropertiesChangeCurrentTab();
                    return(0);
                case TTN_NEEDTEXT:
                    switch(ControlGetNotifiedID(lParam))
                    {
                        case BACKUP_SELDIR:
                            ToolBarDisplayToolTip("Select new backup dir", lParam);
                            return(0);
                        case BACKUP_DELETE:
                            ToolBarDisplayToolTip("Empty selected dir", lParam);
                            return(0);
                        case EXT_REPLACE:
                        case EXT_REPLACE_LANG:
                            ToolBarDisplayToolTip("Replace entry", lParam);
                            return(0);
                        case EXT_INSERT:
                        case EXT_INSERT_LANG:
                            ToolBarDisplayToolTip("Insert new entry", lParam);
                            return(0);
                        case EXT_DELETE:
                        case EXT_DELETE_LANG:
                            ToolBarDisplayToolTip("Delete entry", lParam);
                            return(0);
                        case PROFILE_LOAD:
                            ToolBarDisplayToolTip("Load selected colors profile", lParam);
                            return(0);
                        case PROFILE_SAVE:
                            ToolBarDisplayToolTip("Save colors profile", lParam);
                            return(0);
                        case FOLDER_CHOOSE:
                            ToolBarDisplayToolTip("Select a directory", lParam);
                            return(0);
                        case FTPACC_CREATE:
                            ToolBarDisplayToolTip("Create a new account", lParam);
                            return(0);
                        case FTPACC_SAVE:
                            ToolBarDisplayToolTip("Update current account", lParam);
                            return(0);
                        case FTPACC_DELETE:
                            ToolBarDisplayToolTip("Delete current account", lParam);
                            return(0);
                        case DEBUGGER_VALID:
                            ToolBarDisplayToolTip("Add entry into list", lParam);
                            return(0);
                        case EXT_UP_LANG:
                            ToolBarDisplayToolTip("Move entry up", lParam);
                            return(0);
                        case EXT_DOWN_LANG:
                            ToolBarDisplayToolTip("Move entry down", lParam);
                            return(0);

                    }
            }
            break;
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesCmdOk)
            {
                if(!Save_Language_Idx(hwndDlg))
                {
                    return(0);
                }
                if(FRMPropertiesInFtpCreation == 1)
                {
                    if(MiscMsgBox(hwndDlg, "Current FTP account not saved.\rSave it now ?", MB_QUESTION, Requesters) == IDYES) SaveFTPProps();
                }
                SavePrefs();
                RefreshProperties = 1;
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdCancel)
            {
                ControlClose(hwndDlg);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdMSDN)
            {
                GetHelpFile(FRMPropertiesTxtMSDN);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdDDK)
            {
                GetHelpFile(FRMPropertiesTxtDDK);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdWindowsAPI)
            {
                GetHelpFile(FRMPropertiesTxtWindowsAPI);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdDirectXSDK)
            {
                GetHelpFile(FRMPropertiesTxtDirectXSDK);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdRegs)
            {
                CreateModalDialog(-1, -1, 334, 215, FRMPropertieshwnd, FRMRegsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdGenProg)
            {
                GetGenProtoProg();
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesCmdDebugger)
            {
                GetDebugger(FRMPropertiesCbDebugger);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbBackup)
            {
                switch(wParam)
                {
                    case BACKUP_SELDIR:
                        FRMPropertiesSelectBakDir();
                        return(0);
                    case BACKUP_DELETE:
                        ControlSetText(FRMPropertiesTxtBackupDir, "");
                        return(0);
                }
            }
            else if((HWND) lParam == FRMPropertiesTbFileExt)
            {
                switch(wParam)
                {
                    case EXT_REPLACE:
                        if(ListBoxCount(FRMPropertiesLbFileExt) != 0)
                        {
                            LbOldIdx = ListBoxGetSelItemIndex(FRMPropertiesLbFileExt);
                            if(ControlGetText(FRMPropertiesTxtFileExt).Len() != 0)
                            {
                                if(ListBoxGetSelItemIndex(FRMPropertiesLbFileExt) != -1)
                                {
                                    ListBoxReplaceItem(FRMPropertiesLbFileExt, ListBoxGetSelItemIndex(FRMPropertiesLbFileExt), ControlGetText(FRMPropertiesTxtFileExt));
                                    ListBoxSetIndex(FRMPropertiesLbFileExt, LbOldIdx);
                                    FillDefLanguage();
                                    SetFocus(FRMPropertiesLbFileExt);
                                }
                            }
                        }
                        return(0);
                    case EXT_INSERT:
                        LbOldIdx = ListBoxGetSelItemIndex(FRMPropertiesLbFileExt);
                        if(ControlGetText(FRMPropertiesTxtFileExt).Len() != 0)
                        {
                            if(ListBoxGetSelItemIndex(FRMPropertiesLbFileExt) != -1)
                            {
AddFileExt:                     ListBoxAddItem(FRMPropertiesLbFileExt, ControlGetText(FRMPropertiesTxtFileExt), ListBoxGetSelItemIndex(FRMPropertiesLbFileExt));
                                ListBoxSetIndex(FRMPropertiesLbFileExt, LbOldIdx);
                                FillDefLanguage();
                                SetFocus(FRMPropertiesLbFileExt);
                            }
                            else
                            {
                                LbOldIdx = ListBoxCount(FRMPropertiesLbFileExt);
                                ListBoxSetIndex(FRMPropertiesLbFileExt, ListBoxCount(FRMPropertiesLbFileExt));
                                goto AddFileExt;
                            }
                        }
                        return(0);
                    case EXT_DELETE:
                        LbOldIdx = ListBoxGetSelItemIndex(FRMPropertiesLbFileExt);
                        if(ListBoxCount(FRMPropertiesLbFileExt) != 0)
                        {
                            if(ListBoxGetSelItemIndex(FRMPropertiesLbFileExt) != -1)
                            {
                                ListBoxDeleteItem(FRMPropertiesLbFileExt, ListBoxGetSelItemIndex(FRMPropertiesLbFileExt));
                                if(ListBoxCount(FRMPropertiesLbFileExt) != 0)
                                {
                                    if(LbOldIdx > (ListBoxCount(FRMPropertiesLbFileExt) - 1)) LbOldIdx = ListBoxCount(FRMPropertiesLbFileExt) - 1;
                                    if(LbOldIdx != -1) ListBoxSetIndex(FRMPropertiesLbFileExt, LbOldIdx);
                                    ControlSetText(FRMPropertiesTxtFileExt, ListBoxGetItem(FRMPropertiesLbFileExt, ListBoxGetSelItemIndex(FRMPropertiesLbFileExt)));
                                    FillDefLanguage();
                                    SetFocus(FRMPropertiesLbFileExt);
                                }
                                else
                                {
                                    ControlSetText(FRMPropertiesTxtFileExt, "");
                                    FillDefLanguage();
                                }
                            }
                            if(ListBoxCount(FRMPropertiesLbFileExt) == 0) SetFocus(FRMPropertiesTxtFileExt);
                        }
                        else
                        {
                            SetFocus(FRMPropertiesTxtFileExt);
                        }
                        return(0);
                }
            }
            else if((HWND) lParam == FRMPropertiesTbColors)
            {
                switch(wParam)
                {
                    case PROFILE_LOAD:
                        if(ControlGetText(FRMPropertiesCbColors).Len() == 0)
                        {
                            MiscMsgBox(FRMPropertieshwnd, "Select a profile name.", MB_ERROR, Requesters);
                            SetFocus(FRMPropertiesCbColors);
                        }
                        else
                        {
                            LoadColorsProfile(ControlGetText(FRMPropertiesCbColors));
                        }
                        return(0);
                    case PROFILE_SAVE:
                        if(SaveColorsProfile(ControlGetText(FRMPropertiesCbColors)) == 0)
                        {
                            MiscMsgBox(FRMPropertieshwnd, "Enter a profile name.", MB_ERROR, Requesters);
                            SetFocus(FRMPropertiesCbColors);
                        }
                        return(0);
                }
            }
            else if((HWND) lParam == FRMPropertiesTbROOTDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtROOTDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbBINDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtBINDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbINCLUDEDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtINCLUDEDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbLIBDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtLIBDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbHELPDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtHELPDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbPROJECTSDIR)
            {
                FRMPropertiesChooseROOTDIR(FRMPropertiesTxtPROJECTSDIR);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbValidDebugger)
            {
                // Add teh entry into the list
                BufString = ControlGetText(FRMPropertiesCbDebugger);
                ComboBoxSaveInIniFile(FRMPropertiesCbDebugger, BufString, "ExtDebuggers", MainIniFile);
                AddNewDebugger(FRMPropertiesCbDebugger, BufString);
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbAccounts)
            {
                switch(wParam)
                {
                    // New FTP account
                    case FTPACC_CREATE:
                        FRMPropertiesInFtpCreation = 0;
                        // Ask user for a new account name
                        NewAccountName = MiscInputBox(FRMPropertieshwnd, "Enter new FTP account name", ICON_ASKDIR, "", 0, INPUTBOX_SIMPLETEXT, "");
                        if(NewAccountName.Len() != 0)
                        {
                            if(ComboBoxItemExist(FRMPropertiesCbAccounts, NewAccountName) != -1)
                            {
                                MiscMsgBox(FRMPropertieshwnd, "This account already exists.", MB_ERROR, Requesters);
                                return(0);
                            }
                            ComboBoxSetIndex(FRMPropertiesCbAccounts, ComboBoxAddItem(FRMPropertiesCbAccounts, NewAccountName, -1));
                            ResetFTPProps();
                            FRMPropertiesInFtpCreation = 1;
                        }
                        SetFocus(FRMPropertiesTxtIP);
                        return(0);
                    // Save FTP account
                    case FTPACC_SAVE:
                        SaveFTPProps();
                        FRMPropertiesInFtpCreation = 0;
                        return(0);
                    // Delete FTP account
                    case FTPACC_DELETE:
                        if(ComboBoxCount(FRMPropertiesCbAccounts) != 0)
                        {
                            if(MiscMsgBox(FRMPropertieshwnd, "Do you really want to delete this account ?", MB_QUESTION, Requesters) == IDYES) {
                                DeleteAccountFromIni(ControlGetText(FRMPropertiesCbAccounts));
                                ComboBoxDeleteItem(FRMPropertiesCbAccounts, ComboBoxGetIndex(FRMPropertiesCbAccounts));
                                if(ComboBoxCount(FRMPropertiesCbAccounts) != 0)
                                {
                                    ComboBoxSetIndex(FRMPropertiesCbAccounts, 0);
                                    FillFTPProps();
                                }
                                else
                                {
                                    ResetFTPProps();
                                    ComboBoxSetIndex(FRMPropertiesCbAccounts, -1);
                                }
                            }
                        }
                        FRMPropertiesInFtpCreation = 0;
                        return(0);
                }
            }
            else if((HWND) lParam == FRMPropertiesCmdFontChoose)
            {
                FRMPropertiesSelectFont();
                return(0);
            }
            else if((HWND) lParam == FRMPropertiesTbLanguages)
            {
                switch(wParam)
                {
                    case EXT_REPLACE_LANG:
                        if(ListBoxCount(FRMPropertiesLbLanguages) != 0)
                        {
                            LbOldIdx = ListBoxGetSelItemIndex(FRMPropertiesLbLanguages);
                            if(ListBoxGetSelItemIndex(FRMPropertiesLbLanguages) != -1)
                            {
                                Name = Get_Store_LanguageName();
                                if(Name.Len())
                                {
                                    Extensions = Get_Store_LanguageExtensions();
                                    if(Extensions.Len())
                                    {
                                        ListBoxReplaceItem(FRMPropertiesLbLanguages,
                                                           ListBoxGetSelItemIndex(FRMPropertiesLbLanguages),
                                                           Name + " (" + Extensions + ")");
                                        ListBoxSetIndex(FRMPropertiesLbLanguages, LbOldIdx);
                                        SetFocus(FRMPropertiesLbLanguages);
                                    }
                                    else
                                    {
                                        MiscMsgBox(hwndDlg, "Invalid language extensions.", MB_ERROR, Requesters);
                                    }
                                }
                                else
                                {
                                    MiscMsgBox(hwndDlg, "Invalid language name.", MB_ERROR, Requesters);
                                }
                            }
                        }
                        return(0);
                    case EXT_INSERT_LANG:
                        LbOldIdx = ListBoxGetSelItemIndex(FRMPropertiesLbLanguages);
                        Name = Get_Store_LanguageName();
                        if(Name.Len())
                        {
                            Extensions = Get_Store_LanguageExtensions();
                            if(Extensions.Len())
                            {
                                if(ListBoxGetSelItemIndex(FRMPropertiesLbLanguages) != -1)
                                {
                                    if(!Is_LanguagePresent(Name))
                                    {
AddLanguage:                            ListBoxAddItem(FRMPropertiesLbLanguages, Name + " (" + Extensions + ")",
                                                       ListBoxGetSelItemIndex(FRMPropertiesLbLanguages));
                                        ListBoxSetIndex(FRMPropertiesLbLanguages, LbOldIdx);
                                        SetFocus(FRMPropertiesLbLanguages);
                                    }
                                    else
                                    {
                                        MiscMsgBox(hwndDlg, "Language already exists.", MB_ERROR, Requesters);
                                    }
                                }
                                else
                                {
                                    LbOldIdx = ListBoxCount(FRMPropertiesLbLanguages);
                                    ListBoxSetIndex(FRMPropertiesLbLanguages, ListBoxCount(FRMPropertiesLbLanguages));
                                    goto AddLanguage;
                                }
                            }
                            else
                            {
                                MiscMsgBox(hwndDlg, "Invalid language extensions.", MB_ERROR, Requesters);
                            }
                        }
                        else
                        {
                            MiscMsgBox(hwndDlg, "Invalid language name.", MB_ERROR, Requesters);
                        }
                        return(0);
                    case EXT_DELETE_LANG:
                        LbOldIdx = ListBoxGetSelItemIndex(FRMPropertiesLbLanguages);
                        if(ListBoxCount(FRMPropertiesLbLanguages) != 0)
                        {
                            if(ListBoxGetSelItemIndex(FRMPropertiesLbLanguages) != -1)
                            {
                                ListBoxDeleteItem(FRMPropertiesLbLanguages, ListBoxGetSelItemIndex(FRMPropertiesLbLanguages));
                                if(ListBoxCount(FRMPropertiesLbLanguages) != 0)
                                {
                                    if(LbOldIdx > (ListBoxCount(FRMPropertiesLbLanguages) - 1)) LbOldIdx = ListBoxCount(FRMPropertiesLbLanguages) - 1;
                                    if(LbOldIdx != -1) ListBoxSetIndex(FRMPropertiesLbLanguages, LbOldIdx);
                                    Dispatch_LanguageName(ListBoxGetSelItemIndex(FRMPropertiesLbLanguages));
                                    SetFocus(FRMPropertiesLbLanguages);
                                }
                                else
                                {
                                    Dispatch_LanguageName(0);
                                    SetFocus(FRMPropertiesTxtFileExt);
                                }
                            }
                            if(ListBoxCount(FRMPropertiesLbLanguages) == 0) SetFocus(FRMPropertiesLbLanguages);
                        }
                        else
                        {
                            SetFocus(FRMPropertiesLbLanguages);
                        }
                        return(0);
                    case EXT_UP_LANG:
                        if(ListBoxGetSelItemIndex(FRMPropertiesLbLanguages) != -1)
                        {
                            LbOldIdx = ListBoxGetSelItemIndex(FRMPropertiesLbLanguages);
                            if(LbOldIdx > 0)
                            {
                                Name1 = ListBoxGetItem(FRMPropertiesLbLanguages, LbOldIdx);
                                Name2 = ListBoxGetItem(FRMPropertiesLbLanguages, LbOldIdx - 1);
                                ListBoxReplaceItem(FRMPropertiesLbLanguages, LbOldIdx - 1, Name1);
                                ListBoxReplaceItem(FRMPropertiesLbLanguages, LbOldIdx, Name2);
                                ListBoxSetIndex(FRMPropertiesLbLanguages, LbOldIdx - 1);
                                SetFocus(FRMPropertiesLbLanguages);
                            }
                        }
                        return(0);
                    case EXT_DOWN_LANG:
                        if(ListBoxGetSelItemIndex(FRMPropertiesLbLanguages) != -1)
                        {
                            LbOldIdx = ListBoxGetSelItemIndex(FRMPropertiesLbLanguages);
                            if(LbOldIdx < ListBoxCount(FRMPropertiesLbLanguages) - 1)
                            {
                                Name1 = ListBoxGetItem(FRMPropertiesLbLanguages, LbOldIdx);
                                Name2 = ListBoxGetItem(FRMPropertiesLbLanguages, LbOldIdx + 1);
                                ListBoxReplaceItem(FRMPropertiesLbLanguages, LbOldIdx + 1, Name1);
                                ListBoxReplaceItem(FRMPropertiesLbLanguages, LbOldIdx, Name2);
                                ListBoxSetIndex(FRMPropertiesLbLanguages, LbOldIdx + 1);
                                SetFocus(FRMPropertiesLbLanguages);
                            }
                        }
                        return(0);
                }
            }
            break;
        case WM_PAINT:
            BeginPaint(hwndDlg, &CreatePs);
            GDIDrawHorzSep(hwndDlg, 0, 337, 527);
            EndPaint(hwndDlg, &CreatePs);
            break;
        case WM_CLOSE:
            if(LblFont != 0) DeleteObject(LblFont);
            FreezeTimer = 0;
            EndDialog(hwndDlg, 0);
            FocusOnActiveChild();
            break;
    }
    return(0);
}

// -----------------------------------------------------------------------
// Create first properties page
void CreatePropPage1(HWND hParent)
{
    FRMPropertiesPageEditor = CreateFrame(4, 26, 155, 306, hParent, "Editor", 0, 0, 0);
    FRMPropertiesPageFileExtensions = CreateFrame(161, 26, 232, 183, hParent, "Files extensions", 0, &FRMPropertiesFrameExtensions, 0);
    FRMPropertiesPageFont = CreateFrame(161, 209, 232, 41, hParent, "Font", 0, 0, 0);
    FRMPropertiesPageLineNumbering = CreateFrame(161, 250, 232, 41, hParent, "Lines numbering", 0, 0, 0);
    FRMPropertiesPageOther = CreateFrame(395, 26, 128, 72, hParent, "Other", 0, 0, 0);
    FRMPropertiesPageAutoSave = CreateFrame(395, 98, 128, 56, hParent, "AutoSave", 0, 0, 0);
    FRMPropertiesPageEditor2 = CreateFrame(395, 154, 128, 178, hParent, "Editor", 0, &FRMPropertiesFrameGeneral, 0);
    FRMPropertiesPageBackup = CreateFrame(161, 291, 232, 41, hParent, "Default backup directory", 0, 0, 0);
    FRMPropertiesChkLeftMargin = CreateCheckBox(11, 21 - 7, 140, 15, FRMPropertiesPageEditor, "Left margin", 11, 0, WS_TABSTOP | WS_GROUP, 0);
    FRMPropertiesChkWhiteSpaces = CreateCheckBox(11, 37 - 7, 140, 15, FRMPropertiesPageEditor, "White spaces", 12, 0, WS_TABSTOP, 0);
    FRMPropertiesChkHighlightLine = CreateCheckBox(11, 53 - 7, 140, 15, FRMPropertiesPageEditor, "Highlight selected line", 13, 0, WS_TABSTOP, 0);
    FRMPropertiesChkCursorwrap = CreateCheckBox(11, 69 - 7, 140, 15, FRMPropertiesPageEditor, "Cursor wrap", 14, 0, WS_TABSTOP, 0);
    FRMPropertiesChkSyntaxHighlighting = CreateCheckBox(11, 85 - 7, 140, 15, FRMPropertiesPageEditor, "Syntax highlighting", 15, 0, WS_TABSTOP, 0);
    FRMPropertiesChkLanguageScopeIndent = CreateCheckBox(11, 101 - 7, 140, 15, FRMPropertiesPageEditor, "Language scope indent", 16, 0, WS_TABSTOP, 0);
    FRMPropertiesChkTextDragAndDrop = CreateCheckBox(11, 117 - 7, 140, 15, FRMPropertiesPageEditor, "Text drag and drop", 17, 0, WS_TABSTOP, 0);
    CheckBoxSetState(FRMPropertiesChkTextDragAndDrop, 1);
    FRMPropertiesChkSmoothScrolling = CreateCheckBox(11, 133 - 7, 140, 15, FRMPropertiesPageEditor, "Smooth scrolling", 18, 0, WS_TABSTOP, 0);
    FRMPropertiesChkLineNumberTooltip = CreateCheckBox(11, 149 - 7, 140, 15, FRMPropertiesPageEditor, "Line number tooltip", 19, 0, WS_TABSTOP, 0);
    FRMPropertiesChkCaretAlwaysVisible = CreateCheckBox(11, 165 - 7, 140, 15, FRMPropertiesPageEditor, "Caret always visible", 20, 0, WS_TABSTOP, 0);
    FRMPropertiesChkTabsToSpaces = CreateCheckBox(11, 181 - 7, 140, 15, FRMPropertiesPageEditor, "Tabs to spaces", 21, 0, WS_TABSTOP, 0);
    FRMPropertiesChkHorizontalSplitter = CreateCheckBox(11, 197 - 7, 140, 15, FRMPropertiesPageEditor, "Horizontal splitter", 23, 0, WS_TABSTOP, 0);
    FRMPropertiesChkDetectChange = CreateCheckBox(11, 213- 7, 140, 15, FRMPropertiesPageEditor, "File change detection", 24, 0, WS_TABSTOP, 0);
    FRMPropertiesChkRememberState = CreateCheckBox(11, 229 - 7, 140, 15, FRMPropertiesPageEditor, "Remember last state", 25, 0, WS_TABSTOP, 0);
    FRMPropertiesChkUNIX = CreateCheckBox(11, 245 - 7, 140, 15, FRMPropertiesPageEditor, "Auto convert Unix files", 26, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAutoZoom = CreateCheckBox(11, 261 - 7, 140, 15, FRMPropertiesPageEditor, "Auto focus opened files", 27, 0, WS_TABSTOP, 0);
    FRMPropertiesChkOutputDates = CreateCheckBox(11, 277 - 7, 140, 15, FRMPropertiesPageEditor, "Date prefix in output", 28, 0, WS_TABSTOP, 0);
    FRMPropertiesChkSaveAll = CreateCheckBox(11, 293 - 7, 140, 15, FRMPropertiesPageEditor, "Save before user cmd", 29, 0, WS_TABSTOP, 0);
    FRMPropertiesCbLines = CreateComboBox(39, 15, 84, 150, FRMPropertiesPageLineNumbering, "", 30, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    ComboBoxAddItem(FRMPropertiesCbLines, "(None)", -1);
    ComboBoxAddItem(FRMPropertiesCbLines, "Binary", -1);
    ComboBoxAddItem(FRMPropertiesCbLines, "Decimal", -1);
    ComboBoxAddItem(FRMPropertiesCbLines, "Hexadecimal", -1);
    ComboBoxAddItem(FRMPropertiesCbLines, "Octal", -1);
    ComboBoxSetIndex(FRMPropertiesCbLines, 0);
    CreateLabel(5, 18, 33, 15, FRMPropertiesPageLineNumbering, "Style :", 0, 0, 0, 0);
    CreateLabel(126, 18, 44, 15, FRMPropertiesPageLineNumbering, "Start at :", 0, 0, 0, 0);
    FRMPropertiesTxtLines = CreateTextBox(172, 15, 54, 20, FRMPropertiesPageLineNumbering, "", 31, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    FRMPropertiesTxtFileExt = CreateTextBox(5, 15, 222, 20, FRMPropertiesPageFileExtensions, "", 32, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesLbFileExt = CreateListBox(4, 36, 197, 143, FRMPropertiesPageFileExtensions, 33, 0, 0, WS_TABSTOP | WS_HSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbFileExt = CreateToolBar(203 + 161, 37 + 26, 25, 72, hParent, GlobalImageList1, 34, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBSTYLE_WRAPABLE | WS_TABSTOP | TBS_FIXEDLENGTH, 0);
    ToolBarAddButton(FRMPropertiesTbFileExt, "", EXT_REPLACE, ICON_REPLACE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbFileExt, "", EXT_INSERT, ICON_INSERT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbFileExt, "", EXT_DELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    FRMPropertiesLbFont = CreateLabel(7, 17, 195, 15, FRMPropertiesPageFont, "", 0, 0, 0, 0);
    FRMPropertiesCmdFontChoose = CreateButton(202 + 161, 14 + 209, 25, 21, hParent, "...", 35, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    CreateLabel(27, 12, 63, 15, FRMPropertiesPageOther, "Tabulations :", 0, 0, 0, 0);
    FRMPropertiesTabulations = CreateTextBox(92, 10, 27, 20, FRMPropertiesPageOther, "", 36, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    CreateLabel(26, 31, 63, 15, FRMPropertiesPageOther, "Recent files :", 0, 0, 0, 0);
    FRMPropertiesRecentFiles = CreateTextBox(92, 29, 27, 20, FRMPropertiesPageOther, "", 37, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    CreateLabel(7, 50, 84, 15, FRMPropertiesPageOther, "Recent projects :", 0, 0, 0, 0);
    FRMPropertiesRecentProjects = CreateTextBox(92, 48, 27, 20, FRMPropertiesPageOther, "", 38, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    FRMPropertiesCbSave = CreateComboBox(7, 14, 114, 150, FRMPropertiesPageAutoSave, "", 39, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    ComboBoxAddItem(FRMPropertiesCbSave, "(None)", -1);
    ComboBoxAddItem(FRMPropertiesCbSave, "1 minute", -1);
    ComboBoxAddItem(FRMPropertiesCbSave, "2 minutes", -1);
    ComboBoxAddItem(FRMPropertiesCbSave, "5 minutes", -1);
    ComboBoxAddItem(FRMPropertiesCbSave, "10 minutes", -1);
    ComboBoxAddItem(FRMPropertiesCbSave, "15 minutes", -1);
    ComboBoxAddItem(FRMPropertiesCbSave, "30 minutes", -1);
    ComboBoxAddItem(FRMPropertiesCbSave, "45 minutes", -1);
    ComboBoxAddItem(FRMPropertiesCbSave, "1 hour", -1);
    ComboBoxSetIndex(FRMPropertiesCbSave, 0);
    FRMPropertiesChkVerbose = CreateCheckBox(7, 37, 115, 15, FRMPropertiesPageAutoSave, "Display files to save", 40, 0, WS_TABSTOP, 0);
    FRMPropertiesChkMultiplesInst = CreateCheckBox(5, 9 + 5, 120, 15, FRMPropertiesPageEditor2, "Multiple instances", 41, 0, WS_TABSTOP, 0);
    FRMPropertiesChkRemember = CreateCheckBox(5, 25 + 5, 120, 15, FRMPropertiesPageEditor2, "Remember last pos", 42, 0, WS_TABSTOP, 0);
    FRMPropertiesChkBakFiles = CreateCheckBox(5, 41 + 5, 120, 15, FRMPropertiesPageEditor2, "Create .BAK files", 43, 0, WS_TABSTOP, 0);
    FRMPropertiesChkDecorateBak = CreateCheckBox(5, 57 + 5, 120, 15, FRMPropertiesPageEditor2, "Decorate .BAK files", 44, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAcceptFiles = CreateCheckBox(5, 73 + 5, 120, 15, FRMPropertiesPageEditor2, "Accept dropped files", 45, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAutoMax = CreateCheckBox(5, 89 + 5, 118, 15, FRMPropertiesPageEditor2, "Auto maximize childs", 46, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAutoClean = CreateCheckBox(5, 105 + 5, 118, 15, FRMPropertiesPageEditor2, "Auto clear output", 47, 0, WS_TABSTOP, 0);
    FRMPropertiesChkMinimize = CreateCheckBox(5, 121 + 5, 118, 15, FRMPropertiesPageEditor2, "Minimize to systray", 48, 0, WS_TABSTOP, 0);
    FRMPropertiesChkOnTop = CreateCheckBox(5, 137 + 5, 118, 15, FRMPropertiesPageEditor2, "Always on top", 49, 0, WS_TABSTOP, 0);
    FRMPropertiesChkSaveBk = CreateCheckBox(5, 153 + 5, 118, 15, FRMPropertiesPageEditor2, "Save bookmarks", 50, 0, WS_TABSTOP, 0);
    FRMPropertiesTxtBackupDir = CreateTextBox(6, 14, 169, 20, FRMPropertiesPageBackup, "", 50, 0, ES_READONLY | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbBackup = CreateToolBar(177 + 161, 13 + 291, 50, 24, hParent, GlobalImageList1, 51, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | WS_TABSTOP | TBS_FIXEDLENGTH, 0);
    ToolBarAddButton(FRMPropertiesTbBackup, "", BACKUP_SELDIR, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbBackup, "", BACKUP_DELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
}

// -----------------------------------------------------------------------
// Create second properties page
void CreatePropPage2(HWND hParent)
{
    FRMPropertiesPageColors = CreateFrame(4, 26, 257, 210, hParent, "Editor colors", 0, &FRMPropertiesFrameColors, 0);
    FRMPropertiesPageAPIHelp = CreateFrame(263, 26, 260, 43, hParent, "Windows API help", 0, 0, 0);
    FRMPropertiesPageSDKHelp = CreateFrame(263, 69, 260, 43, hParent, "MSDN/Platform SDK help", 0, 0, 0);
    FRMPropertiesPageDDKHelp = CreateFrame(263, 112, 260, 43, hParent, "Windows DDK help", 0, 0, 0);
    FRMPropertiesPageDirectXSDKHelp = CreateFrame(263, 155, 260, 43, hParent, "DirectX SDK help", 0, 0, 0);
    FRMPropertiesPageExtrasLayouts = CreateFrame(263, 198, 260, 38, hParent, "Extra layouts", 0, 0, 0);
    FRMPropertiesPageCodeMax = CreateFrame(6 - 2, 235, 518, 97, hParent, "", 0, &FRMPropertiesFrameCodeMax, 0);
    FRMPropertiesCodeMax = CreateCodeMax(0, 4, 518, 93, FRMPropertiesPageCodeMax, ApphInstance, 0);
    ControlHookWin(FRMPropertiesCodeMax, &FRMPropertiesCMHook);
    FRMPropertiesCbColors = CreateComboBox(7, 14, 116, 150, FRMPropertiesPageColors, "", 0, 0, CBS_DROPDOWN | WS_TABSTOP);
    FRMPropertiesTbColors = CreateToolBar(125 + 4 , 13 + 26, 50, 24, hParent, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | WS_TABSTOP | TBS_FIXEDLENGTH, 0);
    ToolBarAddButton(FRMPropertiesTbColors, "", PROFILE_LOAD, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbColors, "", PROFILE_SAVE, ICON_SAVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    CreateLabel(7, 39, 97, 15, FRMPropertiesPageColors, "Background :", 0, 0, 0, 0);
    CreateLabel(7, 39 + (16 * 1), 97, 15, FRMPropertiesPageColors, "Lines numbers :", 0, 0, 0, 0);
    CreateLabel(7, 39 + (16 * 2), 97, 15, FRMPropertiesPageColors, "Numbers :", 0, 0, 0, 0);
    CreateLabel(7, 39 + (16 * 3), 97, 15, FRMPropertiesPageColors, "Strings :", 0, 0, 0, 0);
    CreateLabel(7, 39 + (16 * 4), 97, 15, FRMPropertiesPageColors, "Comments :", 0, 0, 0, 0);
    CreateLabel(7, 39 + (16 * 5), 97, 15, FRMPropertiesPageColors, "Keywords :", 0, 0, 0, 0);
    CreateLabel(7, 39 + (16 * 6), 97, 15, FRMPropertiesPageColors, "Directives :", 0, 0, 0, 0);
    CreateLabel(7, 39 + (16 * 7), 97, 15, FRMPropertiesPageColors, "API highlighting :", 0, 0, 0, 0);
    CreateLabel(7, 39 + (16 * 8), 97, 15, FRMPropertiesPageColors, "Extra keywords :", 0, 0, 0, 0);
    CreateLabel(131, 39, 97, 15, FRMPropertiesPageColors, "Normal text :", 0, 0, 0, 0);
    CreateLabel(131, 39 + (16 * 1), 97, 15, FRMPropertiesPageColors, "Margin :", 0, 0, 0, 0);
    CreateLabel(131, 39 + (16 * 2), 97, 15, FRMPropertiesPageColors, "Highlight text :", 0, 0, 0, 0);
    CreateLabel(131, 39 + (16 * 3), 97, 15, FRMPropertiesPageColors, "Registers :", 0, 0, 0, 0);
    CreateLabel(131, 39 + (16 * 4), 97, 15, FRMPropertiesPageColors, "Extra functions :", 0, 0, 0, 0);
    CreateLabel(131, 39 + (16 * 5), 97, 15, FRMPropertiesPageColors, "Scope keywords :", 0, 0, 0, 0);
    CreateLabel(131, 39 + (16 * 6), 97, 15, FRMPropertiesPageColors, "Separators :", 0, 0, 0, 0);
    CreateLabel(131, 39 + (16 * 7), 97, 15, FRMPropertiesPageColors, "Extra keywords :", 0, 0, 0, 0);
    CreateLabel(131, 39 + (16 * 8), 97, 15, FRMPropertiesPageColors, "Extra keywords :", 0, 0, 0, 0);
    FRMPropertiesColBackGround = CreateColorBox(96, 38, 26, 15, FRMPropertiesPageColors, 2, 0, 0);
    FRMPropertiesColLineNumber = CreateColorBox(96, 38 + (16 * 1), 26, 15, FRMPropertiesPageColors, 3, 0, 0);
    FRMPropertiesColNumber = CreateColorBox(96, 38 + (16 * 2), 26, 15, FRMPropertiesPageColors, 4, 0, 0);
    FRMPropertiesColStrings = CreateColorBox(96, 38 + (16 * 3), 26, 15, FRMPropertiesPageColors, 5, 0, 0);
    FRMPropertiesColComments = CreateColorBox(96, 38 + (16 * 4), 26, 15, FRMPropertiesPageColors, 6, 0, 0);
    FRMPropertiesColKeywords = CreateColorBox(96, 38 + (16 * 5), 26, 15, FRMPropertiesPageColors, 7, 0, 0);
    FRMPropertiesColDirectives = CreateColorBox(96, 38 + (16 * 6), 26, 15, FRMPropertiesPageColors, 8, 0, 0);
    FRMPropertiesColAPIHighlighting = CreateColorBox(96, 38 + (16 * 7), 26, 15, FRMPropertiesPageColors, 9, 0, 0);
    FRMPropertiesColExtraHighLight1 = CreateColorBox(96, 38 + (16 * 8), 26, 15, FRMPropertiesPageColors, 10, 0, 0);
    FRMPropertiesColNormalText = CreateColorBox(221, 38, 26, 15, FRMPropertiesPageColors, 11, 0, 0);
    FRMPropertiesColMargin = CreateColorBox(221, 38 + (16 * 1), 26, 15, FRMPropertiesPageColors, 12, 0, 0);
    FRMPropertiesColHighlightText = CreateColorBox(221, 38 + (16 * 2), 26, 15, FRMPropertiesPageColors, 13, 0, 0);
    FRMPropertiesColRegisters = CreateColorBox(221, 38 + (16 * 3), 26, 15, FRMPropertiesPageColors, 14, 0, 0);
    FRMPropertiesColExtraFunctions = CreateColorBox(221, 38 + (16 * 4), 26, 15, FRMPropertiesPageColors, 15, 0, 0);
    FRMPropertiesColScopeKeywords = CreateColorBox(221, 38 + (16 * 5), 26, 15, FRMPropertiesPageColors, 16, 0, 0);
    FRMPropertiesColSeparators = CreateColorBox(221, 38 + (16 * 6), 26, 15, FRMPropertiesPageColors, 17, 0, 0);
    FRMPropertiesColExtraHighLight2 = CreateColorBox(221, 38 + (16 * 7), 26, 15, FRMPropertiesPageColors, 18, 0, 0);
    FRMPropertiesColExtraHighLight3 = CreateColorBox(221, 38 + (16 * 8), 26, 15, FRMPropertiesPageColors, 19, 0, 0);
    FRMPropertiesColLuminosity = CreateTrackBar(2, 187, 152, 20, FRMPropertiesPageColors, 20, 0, 0, 200, TBS_NOTICKS | TBS_TOOLTIPS | TBS_BOTH);
    TrackBarSetPos(FRMPropertiesColLuminosity, 100);
    Assigned_ToolTip = TrackBarGetToolTips(FRMPropertiesColLuminosity);
    FRMPropertiesChkItalic = CreateCheckBox(155, 188, 97, 15, FRMPropertiesPageColors, "Italic comments", 20, 0, WS_TABSTOP, 0);
    FRMPropertiesTxtWindowsAPI = CreateTextBox(5, 16, 223, 20, FRMPropertiesPageAPIHelp, "", 21, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesCmdWindowsAPI = CreateButton(230 + 263, 16 + 26, 25, 21, hParent, "...", 22, 0, 0, 0, WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
    FRMPropertiesTxtMSDN = CreateTextBox(5, 16, 223, 20, FRMPropertiesPageSDKHelp, "", 23, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesCmdMSDN = CreateButton(230 + 263, 16 + 69, 25, 21, hParent, "...", 24, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    FRMPropertiesTxtDDK = CreateTextBox(5, 16, 223, 20, FRMPropertiesPageDDKHelp, "", 25, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesCmdDDK = CreateButton(230 + 263, 16 + 112, 25, 21, hParent, "...", 26, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    FRMPropertiesTxtDirectXSDK = CreateTextBox(5, 16, 223, 20, FRMPropertiesPageDirectXSDKHelp, "", 27, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesCmdDirectXSDK = CreateButton(230 + 263, 16 + 155, 25, 21, hParent, "...", 28, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    CreateLabel(5, 15, 97, 15, FRMPropertiesPageExtrasLayouts, "Default language :", 0, 0, 0, 0);
    FRMPropertiesCbDefLang = CreateComboBox(100, 12, 87, 150, FRMPropertiesPageExtrasLayouts, "", 29, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    FRMPropertiesCmdRegs = CreateButton(189 + 263, 12 + 198, 66, 21, hParent, "Extensions", 30, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
}

// -----------------------------------------------------------------------
// Create third properties page
void CreatePropPage3(HWND hParent)
{
    FRMPropertiesPageFolders = CreateFrame(4, 26, 275, 212, hParent, "Select folders", 0, 0, 0);
    FRMPropertiesPageProjects = CreateFrame(281, 26, 242, 306, hParent, "Project building", 0, &FRMPropertiesFrameBuilding, 0);
    FRMPropertiesPageProtoGen = CreateFrame(4, 238, 275, 94, hParent, "Prototypes generator", 0, 0, 0);
    CreateLabel(6, 13, 236, 15, FRMPropertiesPageFolders, "Main directory [ROOTDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtROOTDIR = CreateTextBox(6, 26, 236, 20, FRMPropertiesPageFolders, "", 1, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbROOTDIR = CreateToolBar(244 + 4, 25 + 26, 25, 23, hParent, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    ToolBarAddButton(FRMPropertiesTbROOTDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    CreateLabel(6, 13 + 32, 236, 15, FRMPropertiesPageFolders, "Bin directory [BINDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtBINDIR = CreateTextBox(6, 26 + 32, 236, 20, FRMPropertiesPageFolders, "", 3, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbBINDIR = CreateToolBar(244 + 4, 25 + 26 + 32, 25, 23, hParent, GlobalImageList1, 4, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    ToolBarAddButton(FRMPropertiesTbBINDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    CreateLabel(6, 13 + (32 * 2), 236, 15, FRMPropertiesPageFolders, "Include directory [INCLUDEDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtINCLUDEDIR = CreateTextBox(6, 26 + (32 * 2), 236, 20, FRMPropertiesPageFolders, "", 5, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbINCLUDEDIR = CreateToolBar(244 + 4, 25 + 26 + (32 * 2), 25, 23, hParent, GlobalImageList1, 6, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    ToolBarAddButton(FRMPropertiesTbINCLUDEDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    CreateLabel(6, 13 + (32 * 3), 236, 15, FRMPropertiesPageFolders, "Lib/Units directory [LIBDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtLIBDIR = CreateTextBox(6, 26 + (32 * 3), 236, 20, FRMPropertiesPageFolders, "", 7, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbLIBDIR = CreateToolBar(244 + 4, 25 + 26 + (32 * 3), 25, 23, hParent, GlobalImageList1, 8, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    ToolBarAddButton(FRMPropertiesTbLIBDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    CreateLabel(6, 13 + (32 * 4), 236, 15, FRMPropertiesPageFolders, "Help directory [HELPDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtHELPDIR = CreateTextBox(6, 26 + (32 * 4), 236, 20, FRMPropertiesPageFolders, "", 9, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbHELPDIR = CreateToolBar(244 + 4, 25 + 26 + (32 * 4), 25, 23, hParent, GlobalImageList1, 10, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    ToolBarAddButton(FRMPropertiesTbHELPDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    CreateLabel(6, 13 + (32 * 5), 236, 15, FRMPropertiesPageFolders, "Projects directory [PROJECTSDIR] :", 0, 0, 0, 0);
    FRMPropertiesTxtPROJECTSDIR = CreateTextBox(6, 26 + (32 * 5), 236, 20, FRMPropertiesPageFolders, "", 11, 0, ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesTbPROJECTSDIR = CreateToolBar(244 + 4, 25 + 26 + (32 * 5), 25, 23, hParent, GlobalImageList1, 12, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    ToolBarAddButton(FRMPropertiesTbPROJECTSDIR, "", FOLDER_CHOOSE, ICON_OPEN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    CreateLabel(6, 20, 106, 15, FRMPropertiesPageProtoGen, "Program :", 0, 0, 0, 0);
    FRMPropertiesTxtGenProg = CreateTextBox(58, 17, 183, 20, FRMPropertiesPageProtoGen, "", 13, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    FRMPropertiesCmdGenProg = CreateButton(243 + 4, 17 + 238, 25, 21, hParent, "...", 14, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    CreateLabel(6, 20 + 25, 106, 15, FRMPropertiesPageProtoGen, "Source :", 0, 0, 0, 0);
    FRMPropertiesTxtGenSource = CreateTextBox(58, 17 + 25, 210, 20, FRMPropertiesPageProtoGen, "", 15, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    CreateLabel(6, 20 + 25 + 25, 106, 15, FRMPropertiesPageProtoGen, "Dest. :", 0, 0, 0, 0);
    FRMPropertiesTxtGenDest = CreateTextBox(58, 17 + 25 + 25, 210, 20, FRMPropertiesPageProtoGen, "", 16, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    CreateLabel(7, 13, 145, 15, FRMPropertiesPageProjects, "Debugger :", 0, 0, 0, 0);
    FRMPropertiesCbDebugger = CreateComboBox(7, 28, 177, 150, FRMPropertiesPageProjects, "", 17, 0, CBS_DROPDOWN | WS_TABSTOP);
    ComboBoxFillFromIniFile(FRMPropertiesCbDebugger, "ExtDebuggers", MainIniFile);
    FRMPropertiesCmdDebugger = CreateButton(177 + 289, 28 + 26, 25, 21, hParent, "...", 18, 0, 0, 0, WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
    FRMPropertiesTbValidDebugger = CreateToolBar(181 + 289 + 22, 25 + 28, 25, 23, hParent, GlobalImageList1, 19, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    ToolBarAddButton(FRMPropertiesTbValidDebugger, "", DEBUGGER_VALID, ICON_INSERT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    CreateLabel(7, 52, 131, 15, FRMPropertiesPageProjects, "Run arguments :", 0, 0, 0, 0);
    FRMPropertiesTxtArgs = CreateTextBox(7, 65, 130, 20, FRMPropertiesPageProjects, "", 20, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    CreateLabel(7, 88, 131, 15, FRMPropertiesPageProjects, "Building skin :", 0, 0, 0, 0);
    FRMPropertiesCbSkin = CreateComboBox(7, 103, 130, 150, FRMPropertiesPageProjects, "", 21, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    FRMStepListView = CreateListView(140, 52, 95, 72, FRMPropertiesPageProjects, 22, 0, &PropStepListViewProc, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
    ListViewAddCol(FRMStepListView, "", 76, 0);
    ListViewAddItem(FRMStepListView, "Build Res.", 0, -1);
    ListViewAddItem(FRMStepListView, "Compile", 1, -1);
    ListViewAddItem(FRMStepListView, "Link", 2, -1);
    ListViewAddItem(FRMStepListView, "Cleanup", 3, -1);
}

// -----------------------------------------------------------------------
// Create fourth properties page
void CreatePropPage4(HWND hParent)
{
    FRMPropertiesPageFTPManage = CreateFrame(4, 26, 519, 49, hParent, "Managment", 0, &FRMPropertiesFrameManagment, 0);
    FRMPropertiesPageAccountInfos = CreateFrame(4, 75, 303, 170 + 17 + 8 + 20, hParent, "Account informations", 0, &FRMPropertiesFrameMain, 0);
    FRMPropertiesPageTransfersTypes = CreateFrame(318, 75, 205, 69, hParent, "Transfers type", 0, 0, 0);
    CreateLabel(9, 20, 78, 15, FRMPropertiesPageFTPManage, "Account name :", 0, 0, 0, 0);

    FRMPropertiesCbAccounts = CreateComboBox(90, 17, 152, 150, FRMPropertiesPageFTPManage, "", 0, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    FRMPropertiesTbAccounts = CreateToolBar(244 + 4, 16 + 26, 84, 23, hParent, GlobalImageList1, 3, -1, 0, TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBS_FIXEDLENGTH | WS_TABSTOP, 0);
    ToolBarAddButton(FRMPropertiesTbAccounts, "", FTPACC_CREATE, ICON_ASKDIR, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbAccounts, "", FTPACC_SAVE, ICON_SAVE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbAccounts, "", FTPACC_DELETE, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);

    CreateLabel(9, 20, 116, 15, FRMPropertiesPageAccountInfos, "Domain or IP address :", 0, 0, 0, 0);
    CreateLabel(9, 45 + 17, 116, 15, FRMPropertiesPageAccountInfos, "User name :", 0, 0, 0, 0);
    CreateLabel(9, 71 + 17, 116, 15, FRMPropertiesPageAccountInfos, "Password :", 0, 0, 0, 0);
    CreateLabel(9, 95 + 17, 116, 15, FRMPropertiesPageAccountInfos, "Save password :", 0, 0, 0, 0);
    CreateLabel(9, 112 + 17 + 8, 116, 15, FRMPropertiesPageAccountInfos, "Initial path :", 0, 0, 0, 0);
    CreateLabel(9, 137 + 17 + 8, 56, 15, FRMPropertiesPageAccountInfos, "FTP port :", 0, 0, 0, 0);
    FRMPropertiesTxtIP = CreateTextBox(124, 17, 170, 20, FRMPropertiesPageAccountInfos, "", 2, 0, ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_STATICEDGE);
    FRMPropertiesChkAnonymous = CreateCheckBox(124, 41, 129, 15, FRMPropertiesPageAccountInfos, "Anonymous login", 3, 0, WS_TABSTOP, 0);
    FRMPropertiesTxtUserName = CreateTextBox(124, 42 + 17, 170, 20, FRMPropertiesPageAccountInfos, "", 4, 0, ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_STATICEDGE);
    FRMPropertiesTxtPassword = CreateTextBox(124, 67 + 17, 170, 20, FRMPropertiesPageAccountInfos, "", 5, 0, ES_AUTOHSCROLL | WS_TABSTOP | ES_PASSWORD, WS_EX_STATICEDGE);
    FRMPropertiesCbPasswordType = CreateComboBox(205, 92 + 17, 89, 150, FRMPropertiesPageAccountInfos, "", 0, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    ComboBoxAddItem(FRMPropertiesCbPasswordType, "Don't save", -1);
    ComboBoxAddItem(FRMPropertiesCbPasswordType, "Plain text", -1);
    ComboBoxAddItem(FRMPropertiesCbPasswordType, "Encrypted", -1);
    FRMPropertiesTxtInitialPath = CreateTextBox(124, 109 + 17 + 8, 170, 20, FRMPropertiesPageAccountInfos, "", 7, 0, ES_AUTOHSCROLL | WS_TABSTOP, WS_EX_STATICEDGE);
    FRMPropertiesTxtFTPPort = CreateTextBox(249, 134 + 17 + 8, 45, 20, FRMPropertiesPageAccountInfos, "", 8, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    CreateLabel(9, 162 + 17 + 8, 116, 15, FRMPropertiesPageAccountInfos, "Connection timeout :", 0, 0, 0, 0);
    FRMPropertiesTxtFTPTimeOut = CreateTextBox(249, 159 + 17 + 8, 45, 20, FRMPropertiesPageAccountInfos, "", 9, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    FRMPropertiesRBASCII = CreateRadioButton(21, 21, 75, 17, FRMPropertiesPageTransfersTypes, "ASCII type", 10, 0, WS_TABSTOP | WS_GROUP);
    FRMPropertiesRBBinary = CreateRadioButton(105, 21, 81, 17, FRMPropertiesPageTransfersTypes, "Binary type", 11, 0, WS_TABSTOP);
    FRMPropertiesChkPassive = CreateCheckBox(21, 42, 103, 15, FRMPropertiesPageTransfersTypes, "Passive transfers", 12, 0, WS_TABSTOP, 0);
    RadioButtonSetState(FRMPropertiesRBBinary, 1);
}

// -----------------------------------------------------------------------
// Create fifth properties page
void CreatePropPage5(HWND hParent)
{
    FRMPropertiesPageCodingHelp = CreateFrame(4, 26, 185, 123, hParent, "Coding help", 0, 0, 0);
    FRMPropertiesPageExtrasLayouts2 = CreateFrame(4, 149, 185, 183, hParent, "Extras layouts", 0, 0, 0);
   
    FRMPropertiesChkAutoCorrectKeywords = CreateCheckBox(11, 18, 160, 15, FRMPropertiesPageCodingHelp, "Auto correct keywords", 22, 0, WS_TABSTOP, 0);
    FRMPropertiesChkWinAPITooltipInfos = CreateCheckBox(11, 18 + (19 * 1), 160, 15, FRMPropertiesPageCodingHelp, "Display functions tooltips", 2, 0, WS_TABSTOP | WS_GROUP, 0);
    CreateLabel(37, 18 + (19 * 2) + 3, 110, 15, FRMPropertiesPageCodingHelp, "lines in keywords list", 0, 0, 0, 0);
    FRMPropertiesTxtAPILines = CreateTextBox(11, 18 + (19 * 2), 20, 20, FRMPropertiesPageCodingHelp, "", 3, 0, WS_TABSTOP | ES_NUMBER, WS_EX_STATICEDGE);
    FRMPropertiesChkUseFncDB = CreateCheckBox(11, 18 + (19 * 3) + 5, 160, 15, FRMPropertiesPageCodingHelp, "Use functions database", 4, 0, WS_TABSTOP, 0);
    FRMPropertiesChkUseConstDB = CreateCheckBox(11, 18 + + (19 * 4) + 5, 160, 15, FRMPropertiesPageCodingHelp, "Use constants database", 5, 0, WS_TABSTOP, 0);

    FRMPropertiesChkShowSplash = CreateCheckBox(11, 19, 160, 15, FRMPropertiesPageExtrasLayouts2, "Show splash screen", 6, 0, WS_TABSTOP | WS_GROUP, 0);
    FRMPropertiesChkTVSingle = CreateCheckBox(11, 19 + (18 * 1), 160, 15, FRMPropertiesPageExtrasLayouts2, "Single click expand treeview", 7, 0, WS_TABSTOP | WS_GROUP, 0);
    FRMPropertiesChkTVTool = CreateCheckBox(11, 19 + (18 * 2), 160, 15, FRMPropertiesPageExtrasLayouts2, "Tooltips in treeview", 8, 0, WS_TABSTOP, 0);
    FRMPropertiesChkRecentDoc = CreateCheckBox(11, 19 + (18 * 3), 160, 15, FRMPropertiesPageExtrasLayouts2, "Store in Windows docs", 9, 0, WS_TABSTOP, 0);
    FRMPropertiesChkAutoOut = CreateCheckBox(11, 19 + (18 * 4), 160, 15, FRMPropertiesPageExtrasLayouts2, "Silent output", 10, 0, WS_TABSTOP, 0);
    FRMPropertiesChkUseFileDir = CreateCheckBox(11, 19 + (18 * 5), 160, 15, FRMPropertiesPageExtrasLayouts2, "Use files directories", 11, 0, WS_TABSTOP, 0);
    FRMPropertiesChkCreateDrop = CreateCheckBox(11, 19 + (18 * 6), 160, 15, FRMPropertiesPageExtrasLayouts2, "New window for dropped files", 12, 0, WS_TABSTOP, 0);
    FRMPropertiesChkSaveProjectState = CreateCheckBox(11, 19 + (18 * 7), 160, 15, FRMPropertiesPageExtrasLayouts2, "Save project state", 13, 0, WS_TABSTOP, 0);
    FRMPropertiesChkConfirmBeforeExit = CreateCheckBox(11, 19 + (18 * 8), 160, 15, FRMPropertiesPageExtrasLayouts2, "Confirm before exit", 14, 0, WS_TABSTOP, 0);

    FRMPropertiesPageLanguages = CreateFrame(191, 26, 234, 306, hParent, "Languages", 15, &FRMPropertiesFrameLanguages, 0);

    FRMPropertiesTbLanguages = CreateToolBar(396, 16 + 27, 25, 136,
                                             hParent,
                                             GlobalImageList1,
                                             16, -1, 0,
                                             TBSTYLE_TOOLTIPS | CCS_NORESIZE | TBSTYLE_FLAT | TBSTYLE_WRAPABLE | WS_TABSTOP | TBS_FIXEDLENGTH,
                                             0);
    ToolBarAddButton(FRMPropertiesTbLanguages, "", EXT_REPLACE_LANG, ICON_REPLACE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbLanguages, "", EXT_INSERT_LANG, ICON_INSERT, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbLanguages, "", EXT_DELETE_LANG, ICON_DELETE, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddSeparator(FRMPropertiesTbLanguages, 0);
    ToolBarAddButton(FRMPropertiesTbLanguages, "", EXT_UP_LANG, ICON_UP, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    ToolBarAddButton(FRMPropertiesTbLanguages, "", EXT_DOWN_LANG, ICON_DOWN, TBSTYLE_BUTTON, TBSTATE_ENABLED, 1);
    FRMPropertiesLbLanguages = CreateListBox(4, 16, 199, 143, FRMPropertiesPageLanguages, 17, 0, 0, WS_TABSTOP | WS_HSCROLL, WS_EX_STATICEDGE);
    
    FRMPropertiesLbLanguageFile = CreateLabel(5, 158, 195, 15, FRMPropertiesPageLanguages, "Filename", 0, 0, 0, 0);
    FRMPropertiesTxtLanguageFile = CreateTextBox(5, 170, 224, 20, FRMPropertiesPageLanguages, "", 18, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
    
    FRMPropertiesLbLanguageExtensions = CreateLabel(5, 188, 195, 15, FRMPropertiesPageLanguages, "Extensions", 0, 0, 0, 0);
    FRMPropertiesTxtLanguageExtensions = CreateTextBox(5, 200, 224, 20, FRMPropertiesPageLanguages, "", 19, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);

    FRMPropertiesLbLanguageDescName = CreateLabel(5, 222, 195, 15, FRMPropertiesPageLanguages, "", 0, 0, 0, 0);
    FRMPropertiesLbLanguageDescCopyright = CreateLabel(5, 237, 195, 15, FRMPropertiesPageLanguages, "", 0, 0, 0, 0);
    FRMPropertiesLbLanguageDescComment = CreateLabel(5, 252, 195, 50, FRMPropertiesPageLanguages, "", 0, 0, 0, 0);
}

// -----------------------------------------------------------------------
// Fill the profiles combo
void FillProfil(void)
{
    int i = 0;
    
    ComboBoxReset(FRMPropertiesCbColors);
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = IniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), ColorsIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        ComboBoxAddItem(FRMPropertiesCbColors, FRMPropertiesRetVal, -1);
    }
}

// -----------------------------------------------------------------------
// Fill all the properties
void FillProperties(void)
{
    int i = 0;

    FRMPropertiesRetVal = IniReadKey("Layout", "AutoSaveDelay", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) ComboBoxSetIndex(FRMPropertiesCbSave, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "VerboseSave", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkVerbose, FRMPropertiesRetVal.Get_Long());
    ControlSetText(FRMPropertiesTxtROOTDIR, IniReadKey("Dirs", DirsRepl[DIR_ROOT], MainIniFile));
    ControlSetText(FRMPropertiesTxtBINDIR, IniReadKey("Dirs", DirsRepl[DIR_BIN], MainIniFile));
    ControlSetText(FRMPropertiesTxtINCLUDEDIR, IniReadKey("Dirs", DirsRepl[DIR_INCLUDE], MainIniFile));
    ControlSetText(FRMPropertiesTxtLIBDIR, IniReadKey("Dirs", DirsRepl[DIR_LIB], MainIniFile));
    ControlSetText(FRMPropertiesTxtHELPDIR, IniReadKey("Dirs", DirsRepl[DIR_HELP], MainIniFile));
    ControlSetText(FRMPropertiesTxtPROJECTSDIR, IniReadKey("Dirs", DirsRepl[DIR_PROJECTS], MainIniFile));
    FRMPropertiesRetVal = IniReadKey("Layout", "LinesStyle", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) ComboBoxSetIndex(FRMPropertiesCbLines, FRMPropertiesRetVal.Get_Long());
    ControlSetText(FRMPropertiesTxtLines, "0");
    FRMPropertiesRetVal = IniReadKey("Layout", "LinesStart", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) ControlSetText(FRMPropertiesTxtLines, FRMPropertiesRetVal);
    ControlSetText(FRMPropertiesTabulations, "8");
    FRMPropertiesRetVal = IniReadKey("Layout", "Tabs", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) ControlSetText(FRMPropertiesTabulations, FRMPropertiesRetVal);

    // Added fallback for non existant colors (should put default ones)
    FRMPropertiesRetVal = IniReadKey("Layout", "BackColor", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColBackGround, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColBackGround, CurBackColor);
    }
    FRMPropertiesRetVal = IniReadKey("Layout", "NormText", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColNormalText, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColNormalText, CurNormTextColor);
    }
    FRMPropertiesRetVal = IniReadKey("Layout", "HighText", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColHighlightText, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColHighlightText, CurHighTextColor);
    }
    FRMPropertiesRetVal = IniReadKey("Layout", "MarginColor", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColMargin, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColMargin, CurMarginColor);
    }
    FRMPropertiesRetVal = IniReadKey("Layout", "LinesColor", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColLineNumber, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColLineNumber, CurLineColor);
    }
    FRMPropertiesRetVal = IniReadKey("Layout", "APIHighColor", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ColorBoxSetColor(FRMPropertiesColAPIHighlighting, FRMPropertiesRetVal.Get_Hex());
    }
    else
    {
        ColorBoxSetColor(FRMPropertiesColAPIHighlighting, APIColor);
    }
    ColorBoxSetColor(FRMPropertiesColNumber, LangNumbersColor);
    ColorBoxSetColor(FRMPropertiesColStrings, LangStringsColor);
    ColorBoxSetColor(FRMPropertiesColComments, LangCommentsColor);
    ColorBoxSetColor(FRMPropertiesColScopeKeywords, LangKeyWordsColor);
    ColorBoxSetColor(FRMPropertiesColKeywords, LangColorAColor);
    ColorBoxSetColor(FRMPropertiesColDirectives, LangColorCColor);
    ColorBoxSetColor(FRMPropertiesColRegisters, LangColorDColor);
    ColorBoxSetColor(FRMPropertiesColExtraFunctions, LangColorEColor);
    ColorBoxSetColor(FRMPropertiesColSeparators, LangColorBColor);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight1, LangColorFColor);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight2, LangColorGColor);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight3, LangColorHColor);
    Prof_BackColor = ColorBoxGetColor(FRMPropertiesColBackGround);
    Orig_Prof_BackColor = Prof_BackColor;
    Prof_NormText = ColorBoxGetColor(FRMPropertiesColNormalText);
    Orig_Prof_NormText = Prof_NormText;
    Prof_HighText = ColorBoxGetColor(FRMPropertiesColHighlightText);
    Orig_Prof_HighText = Prof_HighText;
    Prof_MarginColor = ColorBoxGetColor(FRMPropertiesColMargin);
    Orig_Prof_MarginColor = Prof_MarginColor;
    Prof_LinesColor = ColorBoxGetColor(FRMPropertiesColLineNumber);
    Orig_Prof_LinesColor = Prof_LinesColor;
    Prof_APIHighColor = ColorBoxGetColor(FRMPropertiesColAPIHighlighting);
    Orig_Prof_APIHighColor = Prof_APIHighColor;
    Prof_NumbersColorValue = ColorBoxGetColor(FRMPropertiesColNumber);
    Orig_Prof_NumbersColorValue = Prof_NumbersColorValue;
    Prof_StringsColorValue = ColorBoxGetColor(FRMPropertiesColStrings);
    Orig_Prof_StringsColorValue = Prof_StringsColorValue;
    Prof_CommentsColorValue = ColorBoxGetColor(FRMPropertiesColComments);
    Orig_Prof_CommentsColorValue = Prof_CommentsColorValue;
    Prof_KeyWordsColorValue = ColorBoxGetColor(FRMPropertiesColScopeKeywords);
    Orig_Prof_KeyWordsColorValue = Prof_KeyWordsColorValue;
    Prof_ColorAValue = ColorBoxGetColor(FRMPropertiesColKeywords);
    Orig_Prof_ColorAValue = Prof_ColorAValue;
    Prof_ColorBValue = ColorBoxGetColor(FRMPropertiesColSeparators);
    Orig_Prof_ColorBValue = Prof_ColorBValue;
    Prof_ColorCValue = ColorBoxGetColor(FRMPropertiesColDirectives);
    Orig_Prof_ColorCValue = Prof_ColorCValue;
    Prof_ColorDValue = ColorBoxGetColor(FRMPropertiesColRegisters);
    Orig_Prof_ColorDValue = Prof_ColorDValue;
    Prof_ColorEValue = ColorBoxGetColor(FRMPropertiesColExtraFunctions);
    Orig_Prof_ColorEValue = Prof_ColorEValue;
    Prof_ColorFValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight1);
    Orig_Prof_ColorFValue = Prof_ColorFValue;
    Prof_ColorGValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight2);
    Orig_Prof_ColorGValue = Prof_ColorGValue;
    Prof_ColorHValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight3);
    Orig_Prof_ColorHValue = Prof_ColorHValue;
    FRMPropertiesRetVal = IniReadKey("Layout", "CommentItalic", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkItalic, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "Margin", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkLeftMargin, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "WhiteSpace", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkWhiteSpaces, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "HighLight", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkHighlightLine, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "WordWrap", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkCursorwrap, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "Syntax", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSyntaxHighlighting, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "IndentScope", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkLanguageScopeIndent, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "EnDragDrop", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkTextDragAndDrop, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "APIToolTip", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkWinAPITooltipInfos, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "SmoothScrolling", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSmoothScrolling, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "LineToolTip", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkLineNumberTooltip, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "FollowScrolling", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkCaretAlwaysVisible, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "ExpandTabs", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkTabsToSpaces, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "AutoCorrect", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoCorrectKeywords, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "HorizSplit", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkHorizontalSplitter, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "DetectChange", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkDetectChange, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "RememberState", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkRememberState, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "AutoConvUNIX", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkUNIX, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "AutoZoomFiles", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoZoom, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "OutputDates", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkOutputDates, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "AutoSave", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSaveAll, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "Multiple", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkMultiplesInst, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "Remember", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkRemember, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "CreateBak", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkBakFiles, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "DecorateBak", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkDecorateBak, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "AcceptFiles", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAcceptFiles, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "AutoMax", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoMax, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "AutoClear", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoClean, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "UseSysTray", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkMinimize, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "AlwaysOnTop", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkOnTop, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "SaveBookmarks", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSaveBk, FRMPropertiesRetVal.Get_Long());
    ControlSetText(FRMPropertiesTxtBackupDir, IniReadKey("Layout", "BackupDir", MainIniFile));
    SetBackupEn();
    FRMPropertiesRetVal = IniReadKey("Layout", "Recent", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ControlSetText(FRMPropertiesRecentFiles, FRMPropertiesRetVal);
    }
    else
    {
        ControlSetText(FRMPropertiesRecentFiles, MaxRecent);
    }
    FRMPropertiesRetVal = IniReadKey("Layout", "RecentPrj", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ControlSetText(FRMPropertiesRecentProjects, FRMPropertiesRetVal);
    }
    else
    {
        ControlSetText(FRMPropertiesRecentProjects, MaxRecentPrj);
    }
    SetFontView();
    for(i = 0; i < RegisteredExts.Amount(); i++)
    {
        ListBoxAddItem(FRMPropertiesLbFileExt, RegisteredExts.Get(i)->Content, -1);
    }
    if(ListBoxCount(FRMPropertiesLbFileExt) != 0)
    {
        ListBoxSetIndex(FRMPropertiesLbFileExt, 0);
        ControlSetText(FRMPropertiesTxtFileExt, ListBoxGetItem(FRMPropertiesLbFileExt, ListBoxGetSelItemIndex(FRMPropertiesLbFileExt)));
    }
    ControlSetText(FRMPropertiesTxtArgs, IniReadKey("Layout", "PrjArg", MainIniFile));
    ControlSetText(FRMPropertiesCbDebugger, IniReadKey("Layout", "PrjDebugger", MainIniFile));
    ControlSetText(FRMPropertiesTxtWindowsAPI, IniReadKey("Layout", "WinHelp", MainIniFile));
    ControlSetText(FRMPropertiesTxtMSDN, IniReadKey("Layout", "MSDNHelp", MainIniFile));
    ControlSetText(FRMPropertiesTxtDDK, IniReadKey("Layout", "DDKHelp", MainIniFile));
    ControlSetText(FRMPropertiesTxtDirectXSDK, IniReadKey("Layout", "DirectXSDKHelp", MainIniFile));
    FRMPropertiesRetVal = IniReadKey("Layout", "TVToolTips", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkTVTool, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "WinRecent", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkRecentDoc, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "SilentAuto", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkAutoOut, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "UseFileDir", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkUseFileDir, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "NewWindowOnDrop", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkCreateDrop, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "SaveProjectState", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkSaveProjectState, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "ConfirmBeforeExit", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkConfirmBeforeExit, FRMPropertiesRetVal.Get_Long());
    // Load building steps
    FRMPropertiesRetVal = IniReadKey("Layout", "StepRes", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) ListViewSetItemCheckbox(FRMStepListView, 0, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "StepAsm", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) ListViewSetItemCheckbox(FRMStepListView, 1, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "StepLink", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) ListViewSetItemCheckbox(FRMStepListView, 2, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "StepClean", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) ListViewSetItemCheckbox(FRMStepListView, 3, FRMPropertiesRetVal.Get_Long());
    // Load prototypes generator datas
    PrefsGenProg = IniReadKey("Layout", "GenProto", MainIniFile);
    PrefsGenCmd = IniReadKey("Layout", "GenCmd", MainIniFile);
    PrefsGenDest = IniReadKey("Layout", "GenDest", MainIniFile);
    ControlSetText(FRMPropertiesTxtGenProg, PrefsGenProg);
    ControlSetText(FRMPropertiesTxtGenSource, PrefsGenCmd);
    ControlSetText(FRMPropertiesTxtGenDest, PrefsGenDest);
    FRMPropertiesRetVal = IniReadKey("Layout", "UseDbFnc", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkUseFncDB, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "UseDbConst", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkUseConstDB, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "DbLinesNumbers", MainIniFile);
    // Put a default value (6 lines)
    if(FRMPropertiesRetVal.Len() == 0) FRMPropertiesRetVal = "6";
    ControlSetText(FRMPropertiesTxtAPILines, FRMPropertiesRetVal);
    FRMPropertiesRetVal = IniReadKey("Layout", "ShowSplash", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkShowSplash, FRMPropertiesRetVal.Get_Long());
    FRMPropertiesRetVal = IniReadKey("Layout", "TVSingle", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkTVSingle, FRMPropertiesRetVal.Get_Long());
}

// -----------------------------------------------------------------------
// Save the preferences
void SavePrefs(void)
{
    int i = 0;
    int k = 0;
    CStr TmpSkinName;
    long SkinValue = 0;
    long RadioValue = 0;
    CStr TextBoxValue;
    CStr TmpBakDir;
    CStr TmpHelpWinAPI;
    CStr TmpHelpMSDN;
    CStr TmpHelpDDK;
    CStr TmpHelpDirectXSDK;
    CStr DefLangToStore;
    CStr BufString;

    RegisteredExts.Erase();
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = IniReadKey("Extensions", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(FRMPropertiesRetVal.Len()  == 0) break;
        IniDeleteKey("Extensions", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
    }
    for(i = 0; i <= (long) ListBoxCount(FRMPropertiesLbFileExt) - 1; i++)
    {
        RegisteredExts.Add(ListBoxGetItem(FRMPropertiesLbFileExt, i).Get_String());
        IniWriteKey("Extensions", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), RegisteredExts.Get(i)->Content, MainIniFile);
    }
    // Correct extensions size
    //ReDim Preserve RegisteredExts(i)

    // Editor
    IniWriteKey("Layout", "Margin", CheckBoxGetState(FRMPropertiesChkLeftMargin), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkLeftMargin) == 0) LeftM = 0;
    else LeftM = 1;
    IniWriteKey("Layout", "WhiteSpace", CheckBoxGetState(FRMPropertiesChkWhiteSpaces), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkWhiteSpaces) == 0) White = 0;
    else White = 1;
    IniWriteKey("Layout", "Highlight", CheckBoxGetState(FRMPropertiesChkHighlightLine), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkHighlightLine) == 0) ShowH = 0;
    else ShowH = 1;
    IniWriteKey("Layout", "WordWrap", CheckBoxGetState(FRMPropertiesChkCursorwrap), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkCursorwrap) == 0) Wrap = 0;
    else Wrap = 1;
    IniWriteKey("Layout", "Syntax", CheckBoxGetState(FRMPropertiesChkSyntaxHighlighting), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkSyntaxHighlighting) == 0) SyntaxH = 0;
    else SyntaxH = 1;
    IniWriteKey("Layout", "IndentScope", CheckBoxGetState(FRMPropertiesChkLanguageScopeIndent), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkLanguageScopeIndent) == 0) IndentScope = 0;
    else IndentScope = 1;
    IniWriteKey("Layout", "EnDragDrop", CheckBoxGetState(FRMPropertiesChkTextDragAndDrop), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkTextDragAndDrop) == 0) EnDragDrop = 0;
    else EnDragDrop = 1;
    IniWriteKey("Layout", "APIToolTip", CheckBoxGetState(FRMPropertiesChkWinAPITooltipInfos), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkWinAPITooltipInfos) == 0) APITTip = 0;
    else APITTip = 1;
    IniWriteKey("Layout", "SmoothScrolling", CheckBoxGetState(FRMPropertiesChkSmoothScrolling), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkSmoothScrolling) == 0) Smooth = 0;
    else Smooth = 1;
    IniWriteKey("Layout", "LineToolTip", CheckBoxGetState(FRMPropertiesChkLineNumberTooltip), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkLineNumberTooltip) == 0) LineTTip = 0;
    else LineTTip = 1;
    IniWriteKey("Layout", "FollowScrolling", CheckBoxGetState(FRMPropertiesChkCaretAlwaysVisible), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkCaretAlwaysVisible) == 0) CursorFollow = 0;
    else CursorFollow = 1;
    IniWriteKey("Layout", "ExpandTabs", CheckBoxGetState(FRMPropertiesChkTabsToSpaces), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkTabsToSpaces) == 0) AutoExpand = 0;
    else AutoExpand = 1;
    IniWriteKey("Layout", "AutoCorrect", CheckBoxGetState(FRMPropertiesChkAutoCorrectKeywords), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkAutoCorrectKeywords) == 0) AutoCorrectKey = 0;
    else AutoCorrectKey = 1;
    IniWriteKey("Layout", "HorizSplit", CheckBoxGetState(FRMPropertiesChkHorizontalSplitter), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkHorizontalSplitter) == 0) HorizSplit = 0;
    else HorizSplit = 1;
    IniWriteKey("Layout", "DetectChange", CheckBoxGetState(FRMPropertiesChkDetectChange), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkDetectChange) == 0) DetectChange = 0;
    else DetectChange = 1;
    IniWriteKey("Layout", "RememberState", CheckBoxGetState(FRMPropertiesChkRememberState), MainIniFile);
    IniWriteKey("Layout", "AutoConvUNIX", CheckBoxGetState(FRMPropertiesChkUNIX), MainIniFile);
    IniWriteKey("Layout", "AutoZoomFiles", CheckBoxGetState(FRMPropertiesChkAutoZoom), MainIniFile);
    IniWriteKey("Layout", "OutputDates", CheckBoxGetState(FRMPropertiesChkOutputDates), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkOutputDates) == 0) OutputDates = 0;
    else OutputDates = 1;
    
    // Lines numbering
    IniWriteKey("Layout", "LinesStyle", ComboBoxGetIndex(FRMPropertiesCbLines), MainIniFile);
    LnStyle = ComboBoxGetIndex(FRMPropertiesCbLines);
    if(ControlGetText(FRMPropertiesTxtLines).Len() == 0) ControlSetText(FRMPropertiesTxtLines, "0");
    IniWriteKey("Layout", "LinesStart", ControlGetText(FRMPropertiesTxtLines), MainIniFile);
    LnStart = ControlGetText(FRMPropertiesTxtLines).Get_Long();

    // Font
    IniWriteKey("Layout", "FontName", CurFontName, MainIniFile);
    IniWriteKey("Layout", "FontSize", CurFontSize, MainIniFile);
    if(CurFontHandle != 0) DeleteObject(CurFontHandle);
    CurFontHandle = GDIObtainFont(CurFontName, CurFontSize, hMDIform.hWnd, 0, 0);
    if(NbForms != 0) ChFontAllChilds(hMDIform.hClient);

    // Other
    if(ControlGetText(FRMPropertiesTabulations).Len() == 0) ControlSetText(FRMPropertiesTabulations, "8");
    IniWriteKey("Layout", "Tabs", ControlGetText(FRMPropertiesTabulations), MainIniFile);
    Tabulations = ControlGetText(FRMPropertiesTabulations).Get_Long();
    if(ControlGetText(FRMPropertiesRecentFiles).Len() == 0) ControlSetText(FRMPropertiesRecentFiles, "12");
    IniWriteKey("Layout", "Recent", ControlGetText(FRMPropertiesRecentFiles), MainIniFile);
    MaxRecent = ControlGetText(FRMPropertiesRecentFiles).Get_Long();
    if(ControlGetText(FRMPropertiesRecentProjects).Len() == 0) ControlSetText(FRMPropertiesRecentProjects, "5");
    IniWriteKey("Layout", "RecentPrj", ControlGetText(FRMPropertiesRecentProjects), MainIniFile);
    MaxRecentPrj = ControlGetText(FRMPropertiesRecentProjects).Get_Long();

    // AutoSave delay
    IniWriteKey("Layout", "AutoSaveDelay", ComboBoxGetIndex(FRMPropertiesCbSave), MainIniFile);
    AutoSaveDelay = ComboBoxGetIndex(FRMPropertiesCbSave);
    IniWriteKey("Layout", "VerboseSave", CheckBoxGetState(FRMPropertiesChkVerbose), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkVerbose) == 0) VerboseSave = 0;
    else VerboseSave = 1;
    FRMPropertiesRetVal = IniReadKey("Layout", "VerboseSave", MainIniFile);
    if(FRMPropertiesRetVal.Len() != 0) CheckBoxSetState(FRMPropertiesChkVerbose, FRMPropertiesRetVal.Get_Long());
    
    // General
    IniWriteKey("Layout", "AutoSave", CheckBoxGetState(FRMPropertiesChkSaveAll), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkSaveAll) == 0) AutoSave = 0;
    else AutoSave = 1;
    IniWriteKey("Layout", "Multiple", CheckBoxGetState(FRMPropertiesChkMultiplesInst), MainIniFile);
    IniWriteKey("Layout", "Remember", CheckBoxGetState(FRMPropertiesChkRemember), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkRemember) == 0) Remember = 0;
    else Remember = 1;
    IniWriteKey("Layout", "CreateBak", CheckBoxGetState(FRMPropertiesChkBakFiles), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkBakFiles) == 0) CreateBak = 0;
    else CreateBak = 1;
    IniWriteKey("Layout", "DecorateBak", CheckBoxGetState(FRMPropertiesChkDecorateBak), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkDecorateBak) == 0) DecorateBak = 0;
    else DecorateBak = 1;
    IniWriteKey("Layout", "AcceptFiles", CheckBoxGetState(FRMPropertiesChkAcceptFiles), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkAcceptFiles) == 0) AcceptFiles = 0;
    else AcceptFiles = 1;
    
    // Set client state
    DragAcceptFiles(hMDIform.hClient, AcceptFiles);
    IniWriteKey("Layout", "AutoMax", CheckBoxGetState(FRMPropertiesChkAutoMax), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkAutoMax) == 0) AutoMaximizeChilds = 0;
    else AutoMaximizeChilds = 1;
    IniWriteKey("Layout", "AutoClear", CheckBoxGetState(FRMPropertiesChkAutoClean), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkAutoClean) == 0) AutoClearOut = 0;
    else AutoClearOut = 1;
    
    IniWriteKey("Layout", "UseSysTray", CheckBoxGetState(FRMPropertiesChkMinimize), MainIniFile);
    IniWriteKey("Layout", "AlwaysOnTop", CheckBoxGetState(FRMPropertiesChkOnTop), MainIniFile);
    IniWriteKey("Layout", "SaveBookmarks", CheckBoxGetState(FRMPropertiesChkSaveBk), MainIniFile);
    IniWriteKey("Layout", "ShowSplash", CheckBoxGetState(FRMPropertiesChkShowSplash), MainIniFile);
    IniWriteKey("Layout", "TVSingle", CheckBoxGetState(FRMPropertiesChkTVSingle), MainIniFile);
    
    // Refresh the treeview's expand type
    SetTreeviewExpand();
    IniWriteKey("Layout", "UseDbFnc", CheckBoxGetState(FRMPropertiesChkUseFncDB), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkUseFncDB) == 0) UseDbFnc = 0;
    else UseDbFnc = 1;
    IniWriteKey("Layout", "UseDbConst", CheckBoxGetState(FRMPropertiesChkUseConstDB), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkUseConstDB) == 0) UseDbConst = 0;
    else UseDbConst = 1;
    
    // Number of lines in API fncs/constants popup list
    if(ControlGetText(FRMPropertiesTxtAPILines).Len() == 0) ControlSetText(FRMPropertiesTxtAPILines, "1");
    IniWriteKey("Layout", "DbLinesNumbers", ControlGetText(FRMPropertiesTxtAPILines), MainIniFile);
    DbLinesNumbers = ControlGetText(FRMPropertiesTxtAPILines).Get_Long();
    
    IniWriteKey("Layout", "TVToolTips", CheckBoxGetState(FRMPropertiesChkTVTool), MainIniFile);
    // Refresh the treeview's tooltips
    SetTreeviewTT();

    IniWriteKey("Layout", "WinRecent", CheckBoxGetState(FRMPropertiesChkRecentDoc), MainIniFile);
    IniWriteKey("Layout", "SilentAuto", CheckBoxGetState(FRMPropertiesChkAutoOut), MainIniFile);
    IniWriteKey("Layout", "UseFileDir", CheckBoxGetState(FRMPropertiesChkUseFileDir), MainIniFile);
    IniWriteKey("Layout", "NewWindowOnDrop", CheckBoxGetState(FRMPropertiesChkCreateDrop), MainIniFile);
    IniWriteKey("Layout", "SaveProjectState", CheckBoxGetState(FRMPropertiesChkSaveProjectState), MainIniFile);
    IniWriteKey("Layout", "ConfirmBeforeExit", CheckBoxGetState(FRMPropertiesChkConfirmBeforeExit), MainIniFile);

    // Default backup directory
    TmpBakDir = ControlGetText(FRMPropertiesTxtBackupDir);
    TmpBakDir = ChangeAbsolutePaths(TmpBakDir);
    IniWriteKey("Layout", "BackupDir", TmpBakDir, MainIniFile);
    ControlSetText(FRMPropertiesTxtBackupDir, TmpBakDir);

    // Help files
    TmpHelpWinAPI = ControlGetText(FRMPropertiesTxtWindowsAPI);
    TmpHelpMSDN = ControlGetText(FRMPropertiesTxtMSDN);
    TmpHelpDDK = ControlGetText(FRMPropertiesTxtDDK);
    TmpHelpDirectXSDK = ControlGetText(FRMPropertiesTxtDirectXSDK);
    TmpHelpWinAPI = ChangeAbsolutePaths(TmpHelpWinAPI);
    TmpHelpMSDN = ChangeAbsolutePaths(TmpHelpMSDN);
    TmpHelpDDK = ChangeAbsolutePaths(TmpHelpDDK);
    TmpHelpDirectXSDK = ChangeAbsolutePaths(TmpHelpDirectXSDK);
    IniWriteKey("Layout", "WinHelp", TmpHelpWinAPI, MainIniFile);
    IniWriteKey("Layout", "MSDNHelp", TmpHelpMSDN, MainIniFile);
    IniWriteKey("Layout", "DDKHelp", TmpHelpDDK, MainIniFile);
    IniWriteKey("Layout", "DirectXSDKHelp", TmpHelpDirectXSDK, MainIniFile);
    ControlSetText(FRMPropertiesTxtWindowsAPI, TmpHelpWinAPI);
    ControlSetText(FRMPropertiesTxtMSDN, TmpHelpMSDN);

    // Refresh masm directories
    free(Dirs[DIR_ROOT]);
    Dirs[DIR_ROOT] = _strdup(ControlGetText(FRMPropertiesTxtROOTDIR).Get_String());
    IniWriteKey("Dirs", DirsRepl[DIR_ROOT], Dirs[DIR_ROOT], MainIniFile);
    free(Dirs[DIR_BIN]);
    Dirs[DIR_BIN] = _strdup(ControlGetText(FRMPropertiesTxtBINDIR).Get_String());
    IniWriteKey("Dirs", DirsRepl[DIR_BIN], Dirs[DIR_BIN], MainIniFile);
    free(Dirs[DIR_INCLUDE]);
    Dirs[DIR_INCLUDE] = _strdup(ControlGetText(FRMPropertiesTxtINCLUDEDIR).Get_String());
    IniWriteKey("Dirs",DirsRepl[DIR_INCLUDE], Dirs[DIR_INCLUDE], MainIniFile);
    free(Dirs[DIR_LIB]);
    Dirs[DIR_LIB] = _strdup(ControlGetText(FRMPropertiesTxtLIBDIR).Get_String());
    IniWriteKey("Dirs", DirsRepl[DIR_LIB], Dirs[DIR_LIB], MainIniFile);
    free(Dirs[DIR_HELP]);
    Dirs[DIR_HELP] = _strdup(ControlGetText(FRMPropertiesTxtHELPDIR).Get_String());
    IniWriteKey("Dirs", DirsRepl[DIR_HELP], Dirs[DIR_HELP], MainIniFile);
    free(Dirs[DIR_PROJECTS]);
    Dirs[DIR_PROJECTS] = _strdup(ControlGetText(FRMPropertiesTxtPROJECTSDIR).Get_String());
    // Set it by default just in case
    if(strlen(Dirs[DIR_PROJECTS]) == 0)
    {
        free(Dirs[DIR_PROJECTS]);
        BufString = AppPath + (CStr) "\\Projects";
        Dirs[DIR_PROJECTS] = _strdup(BufString.Get_String());
    }
    IniWriteKey("Dirs", DirsRepl[DIR_PROJECTS], Dirs[DIR_PROJECTS], MainIniFile);
    
    // Save colors
    Prof_BackColor = ColorBoxGetColor(FRMPropertiesColBackGround);
    Prof_NormText = ColorBoxGetColor(FRMPropertiesColNormalText);
    Prof_HighText = ColorBoxGetColor(FRMPropertiesColHighlightText);
    Prof_MarginColor = ColorBoxGetColor(FRMPropertiesColMargin);
    Prof_LinesColor = ColorBoxGetColor(FRMPropertiesColLineNumber);
    Prof_NumbersColorValue = ColorBoxGetColor(FRMPropertiesColNumber);
    Prof_StringsColorValue = ColorBoxGetColor(FRMPropertiesColStrings);
    Prof_CommentsColorValue = ColorBoxGetColor(FRMPropertiesColComments);
    Prof_KeyWordsColorValue = ColorBoxGetColor(FRMPropertiesColScopeKeywords);
    Prof_APIHighColor = ColorBoxGetColor(FRMPropertiesColAPIHighlighting);
    Prof_ColorAValue = ColorBoxGetColor(FRMPropertiesColKeywords);
    Prof_ColorBValue = ColorBoxGetColor(FRMPropertiesColSeparators);
    Prof_ColorCValue = ColorBoxGetColor(FRMPropertiesColDirectives);
    Prof_ColorDValue = ColorBoxGetColor(FRMPropertiesColRegisters);
    Prof_ColorEValue = ColorBoxGetColor(FRMPropertiesColExtraFunctions);
    Prof_ColorFValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight1);
    Prof_ColorGValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight2);
    Prof_ColorHValue = ColorBoxGetColor(FRMPropertiesColExtraHighLight3);
    
    // Change global colors variables
    CurBackColor = Prof_BackColor;
    CurMarginColor = Prof_MarginColor;
    CurNormTextColor = Prof_NormText;
    CurHighTextColor = Prof_HighText;
    CurLineColor = Prof_LinesColor;
    APIColor = Prof_APIHighColor;
    LangNumbersColor = Prof_NumbersColorValue;
    LangStringsColor = Prof_StringsColorValue;
    LangCommentsColor = Prof_CommentsColorValue;
    LangColorAColor = Prof_ColorAValue;
    LangColorBColor = Prof_ColorBValue;
    LangColorCColor = Prof_ColorCValue;
    LangColorDColor = Prof_ColorDValue;
    LangColorEColor = Prof_ColorEValue;
    LangColorFColor = Prof_ColorFValue;
    LangColorGColor = Prof_ColorGValue;
    LangColorHColor = Prof_ColorHValue;
    LangKeyWordsColor = Prof_KeyWordsColorValue;
 
    IniWriteKey("Layout", "BackColor", "0x" + (CStr) BufString.Hex_To_String(Prof_BackColor).Get_String(), MainIniFile);
    IniWriteKey("Layout", "NormText", "0x" + (CStr) BufString.Hex_To_String(Prof_NormText).Get_String(), MainIniFile);
    IniWriteKey("Layout", "HighText", "0x" + (CStr) BufString.Hex_To_String(Prof_HighText).Get_String(), MainIniFile);
    IniWriteKey("Layout", "MarginColor", "0x" + (CStr) BufString.Hex_To_String(Prof_MarginColor).Get_String(), MainIniFile);
    IniWriteKey("Layout", "LinesColor", "0x" + (CStr) BufString.Hex_To_String(Prof_LinesColor).Get_String(), MainIniFile);
    IniWriteKey("Layout", "NumbersColorValue", "0x" + (CStr) BufString.Hex_To_String(Prof_NumbersColorValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "APIHighColor", "0x" + (CStr) BufString.Hex_To_String(Prof_APIHighColor).Get_String(), MainIniFile);
    IniWriteKey("Layout", "StringsColorValue", "0x" + (CStr) BufString.Hex_To_String(Prof_StringsColorValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "CommentsColorValue", "0x" + (CStr) BufString.Hex_To_String(Prof_CommentsColorValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "KeyWordsColorValue", "0x" + (CStr) BufString.Hex_To_String(Prof_KeyWordsColorValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "ColorAValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorAValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "ColorBValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorBValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "ColorCValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorCValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "ColorDValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorDValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "ColorEValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorEValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "ColorFValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorFValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "ColorGValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorGValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "ColorHValue", "0x" + (CStr) BufString.Hex_To_String(Prof_ColorHValue).Get_String(), MainIniFile);
    IniWriteKey("Layout", "CommentItalic", CheckBoxGetState(FRMPropertiesChkItalic), MainIniFile);
    if(CheckBoxGetState(FRMPropertiesChkItalic) == 0) ItalicFont = 0;
    else ItalicFont = 1;
   
    // Save prototypes generator datas
    IniWriteKey("Layout", "GenProto", ControlGetText(FRMPropertiesTxtGenProg), MainIniFile);
    IniWriteKey("Layout", "GenCmd", ControlGetText(FRMPropertiesTxtGenSource), MainIniFile);
    IniWriteKey("Layout", "GenDest", ControlGetText(FRMPropertiesTxtGenDest), MainIniFile);

    // Project building
    IniWriteKey("Layout", "PrjArg", ControlGetText(FRMPropertiesTxtArgs), MainIniFile);
    IniWriteKey("Layout", "PrjDebugger", ControlGetText(FRMPropertiesCbDebugger), MainIniFile);
    // Save the list of debuggers now
    //ComboBoxSaveInIniFile(FRMPropertiesCbDebugger, ControlGetText(FRMPropertiesCbDebugger), "ExtDebuggers", MainIniFile);
    //AddNewDebugger(FRMPropertiesCbDebugger, ControlGetText(FRMPropertiesCbDebugger));

    // Save building steps
    IniWriteKey("Layout", "StepRes", ListViewGetItemCheckbox(FRMStepListView, 0), MainIniFile);
    IniWriteKey("Layout", "StepAsm", ListViewGetItemCheckbox(FRMStepListView, 1), MainIniFile);
    IniWriteKey("Layout", "StepLink", ListViewGetItemCheckbox(FRMStepListView, 2), MainIniFile);
    IniWriteKey("Layout", "StepClean", ListViewGetItemCheckbox(FRMStepListView, 3), MainIniFile);
    
    // Get skin filename
    if(ComboBoxCount(FRMPropertiesCbSkin) != 0)
    {
        TmpSkinName = PrgSkinArray.Get(ComboBoxGetIndex(FRMPropertiesCbSkin))->Content;
    }
    // Save skin
    for(i = 0; i < PrgSkinControlsDest.Amount(); i++)
    {
        BufString = PrgSkinControlsDest.Get(i)->Content;
        BufString = BufString.Left(5).Upper_Case();
        if(BufString == "COMBO")
        {
            SkinValue = ComboBoxGetIndex(PrgSkinControls.Get(i)->Content);
            IniWriteKey("CURRENT", PrgSkinControlsDest.Get(i)->Content, SkinValue, TmpSkinName);
        }
        else if(BufString == "CHECK")
        {
            SkinValue = CheckBoxGetState(PrgSkinControls.Get(i)->Content);
            IniWriteKey("CURRENT", PrgSkinControlsDest.Get(i)->Content, SkinValue, TmpSkinName);
        }
        else if(BufString == "TEXTB")
        {
            TextBoxValue = ControlGetText(PrgSkinControls.Get(i)->Content);
            IniWriteKey("CURRENT", PrgSkinControlsDest.Get(i)->Content, TextBoxValue, TmpSkinName);
        }
        else if(BufString == "LISTC")
        {
            for(k = 0; k <= (long) ListViewItemCount(PrgSkinControls.Get(i)->Content); k++)
            {
                BufString = PrgSkinControlsDest.Get(i)->Content;
                BufString = BufString + (CStr) StringNumberComplement(k, 3).Get_String();
                IniWriteKey("CURRENT", BufString, ListViewGetItemCheckbox(PrgSkinControls.Get(i)->Content, k), TmpSkinName);
            }
        }
        else if(BufString == "RADIO")
        {
            SkinValue = RadioButtonGetState(PrgSkinControls.Get(i)->Content);
            if(SkinValue != 0)
            {
                BufString = PrgSkinControlsDest.Get(i)->Content;
                BufString = BufString.Mid(9);
                RadioValue = BufString.Get_Long();
                BufString = PrgSkinControlsDest.Get(i)->Content;
                BufString = BufString.Left(8);
                IniWriteKey("CURRENT", BufString, RadioValue, TmpSkinName);
            }
        }
    }
    TmpSkinName = ChangeAbsolutePaths(TmpSkinName);
    IniWriteKey("BuildSkins", "CurrentSkin", TmpSkinName, SkinsIniFile);

    // Set main window on top or not
    if(CheckBoxGetState(FRMPropertiesChkOnTop) == 1) ControlSetTopMost(hMDIform.hWnd);
    else ControlRemTopMost(hMDIform.hWnd);
    
    // Save default selected language
    DefLangToStore = ControlGetText(FRMPropertiesCbDefLang);
    if(DefLangToStore.Len() == 0) DefLangToStore = "";
    IniWriteKey("RefLanguages", "DefLang", DefLangToStore, LanguagesIniFile);

    // Re-init the autosave timer
    RefreshTimer(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// Enable/Disable backup directory selection
void SetBackupEn(void)
{
    switch(CheckBoxGetState(FRMPropertiesChkBakFiles))
    {
        case 0:
            ToolBarSetButtonEnabled(FRMPropertiesTbBackup, BACKUP_SELDIR, 0);
            ToolBarSetButtonEnabled(FRMPropertiesTbBackup, BACKUP_DELETE, 0);
            ControlEnable(FRMPropertiesChkDecorateBak, 0);
            break;
        case 1:
            ToolBarSetButtonEnabled(FRMPropertiesTbBackup, BACKUP_SELDIR, 1);
            ToolBarSetButtonEnabled(FRMPropertiesTbBackup, BACKUP_DELETE, 1);
            ControlEnable(FRMPropertiesChkDecorateBak, 1);
            break;
    }
}

// -----------------------------------------------------------------------
// View the font Name/Size
void SetFontView(void)
{
    ControlSetText(FRMPropertiesLbFont, CurFontName + (CStr) " (" + (CStr) CurFontSize + (CStr) ")");
    if(LblFont != 0) DeleteObject(LblFont);
    LblFont = GDIObtainFont(CurFontName, CurFontSize, hMDIform.hWnd, 0, 0);
}

// -----------------------------------------------------------------------
// General Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameGeneral(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesChkBakFiles)
            {
                SetBackupEn();
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Colors Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameColors(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int Luminosity;

    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesChkItalic)
            {
                FRMPropertiesSetItalicComments(FRMPropertiesCodeMax);
                return(0);
            }
            break;
        case MSG_COLORBOX_CLICKED:
            if((HWND) wParam == FRMPropertiesColBackGround)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColLineNumber)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColNumber)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColStrings)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColComments)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColKeywords)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColDirectives)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColAPIHighlighting)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColNormalText)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColMargin)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColHighlightText)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColRegisters)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColExtraFunctions)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColScopeKeywords)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColSeparators)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesChkItalic)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColExtraHighLight1)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColExtraHighLight2)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            else if((HWND) wParam == FRMPropertiesColExtraHighLight3)
            {
                FRMPropertiesSetColor((HWND) wParam);
                return(0);
            }
            break;
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case TTN_NEEDTEXT:
                    if(Assigned_ToolTip != NULL)
                    {
                        if(ControlGetNotifiedhWnd(lParam) == Assigned_ToolTip)
                        {
                            Luminosity = TrackBarGetPos(FRMPropertiesColLuminosity);
                            ToolBarDisplayToolTip("Luminosity: " + (CStr) Luminosity + (CStr) "%", lParam);
                            return(0);
                        }
                    }
                }
            break;
        case WM_HSCROLL:
            switch(wParam & 0xFFFF)
            {
                case TB_THUMBPOSITION:
                case TB_THUMBTRACK:
                case TB_TOP:
                case TB_BOTTOM:
                case TB_LINEUP:
                case TB_LINEDOWN:
                case TB_PAGEUP:
                case TB_PAGEDOWN:           
                    Prof_Luminosity = TrackBarGetPos(FRMPropertiesColLuminosity);
                    ColorBoxSetColor(FRMPropertiesColBackGround, GDIColorCalcLuminosity(Orig_Prof_BackColor, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColNormalText, GDIColorCalcLuminosity(Orig_Prof_NormText, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColHighlightText, GDIColorCalcLuminosity(Orig_Prof_HighText, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColMargin, GDIColorCalcLuminosity(Orig_Prof_MarginColor, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColLineNumber, GDIColorCalcLuminosity(Orig_Prof_LinesColor, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColNumber, GDIColorCalcLuminosity(Orig_Prof_NumbersColorValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColStrings, GDIColorCalcLuminosity(Orig_Prof_StringsColorValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColComments, GDIColorCalcLuminosity(Orig_Prof_CommentsColorValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColScopeKeywords, GDIColorCalcLuminosity(Orig_Prof_KeyWordsColorValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColAPIHighlighting, GDIColorCalcLuminosity(Orig_Prof_APIHighColor, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColKeywords, GDIColorCalcLuminosity(Orig_Prof_ColorAValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColSeparators, GDIColorCalcLuminosity(Orig_Prof_ColorBValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColDirectives, GDIColorCalcLuminosity(Orig_Prof_ColorCValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColRegisters, GDIColorCalcLuminosity(Orig_Prof_ColorDValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColExtraFunctions, GDIColorCalcLuminosity(Orig_Prof_ColorEValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColExtraHighLight1, GDIColorCalcLuminosity(Orig_Prof_ColorFValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColExtraHighLight2, GDIColorCalcLuminosity(Orig_Prof_ColorGValue, Prof_Luminosity));
                    ColorBoxSetColor(FRMPropertiesColExtraHighLight3, GDIColorCalcLuminosity(Orig_Prof_ColorHValue, Prof_Luminosity));
                    BuildPreview(FRMPropertiesCodeMax);
                    GdiFlush();
                    return(0);
            }
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Pick up a new color
void FRMPropertiesSetColor(HWND hwnd)
{
    if(ComDlgChooseColor(FRMPropertieshwnd, ColorBoxGetColor(hwnd)) != 0)
    {
        ColorBoxSetColor(hwnd, MyColor.rgbResult);
        BuildPreview(FRMPropertiesCodeMax);
    }
}

// -----------------------------------------------------------------------
// Extensions Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameExtensions(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesLbFileExt)
            {
                ControlSetText(FRMPropertiesTxtFileExt,
                               ListBoxGetItem(FRMPropertiesLbFileExt,
                               ListBoxGetSelItemIndex(FRMPropertiesLbFileExt)));
                return(0);
            } 
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Languages Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameLanguages(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesLbLanguages)
            {
                Dispatch_LanguageName(ListBoxGetSelItemIndex(FRMPropertiesLbLanguages));
                return(0);
            } 
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Init minima properties for codemax preview
void FRMPropertiesInitCodeMax(HWND hwnd)
{
    CStr BufString;
    
    CM_SetReadOnly(hwnd, 1);
    CM_ShowScrollBar(hwnd, 0, 1);
    CM_ShowScrollBar(hwnd, 1, 1);
    CM_EnableSplitter(hwnd, 0, 1);
    CM_EnableGlobalProps(hwnd, 1);
    CM_EnableColumnSel(hwnd, 0);
    CM_EnableHideSel(hwnd, 0);
    CM_SetLanguage(hwnd, "Cpp");

    BufString = "BOOL APIENTRY DllMain(HANDLE hModule,\r\n";
    BufString = BufString + "                      DWORD ul_reason_for_call,\r\n";
    BufString = BufString + "                      LPVOID lpReserved)\r\n";
    BufString = BufString + "{\r\n";
    BufString = BufString + "    switch(ul_reason_for_call)\r\n";
    BufString = BufString + "    {\r\n";
    BufString = BufString + "        case DLL_PROCESS_ATTACH:\r\n";
    BufString = BufString + "            break;\r\n";
    BufString = BufString + "        case DLL_THREAD_ATTACH:\r\n";
    BufString = BufString + "            break;\r\n";
    BufString = BufString + "        case DLL_THREAD_DETACH:\r\n";
    BufString = BufString + "            break;\r\n";
    BufString = BufString + "        case DLL_PROCESS_DETACH:\r\n";
    BufString = BufString + "            break;\r\n";
    BufString = BufString + "    }\r\n";
    BufString = BufString + "    return TRUE;\r\n";
    BufString = BufString + "}";
    CM_SetText(hwnd,BufString.Get_String());
}

// -----------------------------------------------------------------------
// Build the codemax preview control
void BuildPreview(HWND hwnd)
{
    ControlSetFont(hwnd, LblFont);
    CM_EnableSelBounds(hwnd, CheckBoxGetState(FRMPropertiesChkCursorwrap));
    CM_EnableLeftMargin(hwnd, CheckBoxGetState(FRMPropertiesChkLeftMargin));
    CM_EnableWhitespaceDisplay(hwnd, CheckBoxGetState(FRMPropertiesChkWhiteSpaces));
    SetLines(hwnd);
    if(CheckBoxGetState(FRMPropertiesChkHighlightLine) != 0) CM_SetHighlightedLine(hwnd, FRMPropertiesGetCurrentLineNumber(hwnd));
    else CM_SetHighlightedLine(hwnd, -1);
    if(ControlGetText(FRMPropertiesTabulations).Len() == 0) CM_SetTabSize(hwnd, 8);
    else CM_SetTabSize(hwnd, ControlGetText(FRMPropertiesTabulations).Get_Long());
    switch(CheckBoxGetState(FRMPropertiesChkLanguageScopeIndent))
    {
        case 0:
            CM_SetAutoIndentMode(hwnd, CM_INDENT_OFF);
            break;
        case 1:
            CM_SetAutoIndentMode(hwnd, CM_INDENT_SCOPE);
            break;
    }
    CM_EnableColorSyntax(hwnd, CheckBoxGetState(FRMPropertiesChkSyntaxHighlighting));
    CM_EnableLineToolTips(hwnd, CheckBoxGetState(FRMPropertiesChkLineNumberTooltip));
    CM_EnableSmoothScrolling(hwnd, CheckBoxGetState(FRMPropertiesChkSmoothScrolling));
    FRMPropertiesSetCodeMaxColors(hwnd);
    FRMPropertiesSetItalicComments(hwnd);
    CM_EnableSplitter(hwnd, 1, CheckBoxGetState(FRMPropertiesChkHorizontalSplitter));
}

// -----------------------------------------------------------------------
// Select context help files
void GetHelpFile(HWND hwnd)
{
    CStr LdFile;
    CStr OpFilters;

    OpFilters = "Help files (*.hlp;*.chm;*.col)|*.hlp;*.chm;*.col";
    LdFile = ComDlgGetOpenFileName(FRMPropertieshwnd, OpFilters, "", 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    LdFile = ChangeAbsolutePaths(LdFile);
    ControlSetText(hwnd, LdFile);
}

// -----------------------------------------------------------------------
// Select prototype generator
void GetGenProtoProg(void)
{
    CStr LdFile;
    CStr OpFilters;
 
    OpFilters = "Exe files (*.exe;*.com)|*.exe;*.com";
    LdFile = ComDlgGetOpenFileName(FRMPropertieshwnd, OpFilters, "", 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    LdFile = ChangeAbsolutePaths(LdFile);
    ControlSetText(FRMPropertiesTxtGenProg, LdFile);
}

// -----------------------------------------------------------------------
// Select a font
void FRMPropertiesSelectFont(void)
{
    CStr FName;
    int i = 0;

    if(ComDlgChooseFont(FRMPropertieshwnd, CurFontName, CurFontSize, 1) != 0)
    {
        for(i = 0; i <= LF_FACESIZE - 1; i++)
        {
            if(MyLogFont.lfFaceName[i] == 0) break;
            FName = FName + FName.Chr(MyLogFont.lfFaceName[i]);
        }
        if(FName.Len() == 0) return;
        CurFontName = FName;
        CurFontSize = MyFont.iPointSize / 10;
        SetFontView();
    }
}

// -----------------------------------------------------------------------
// Choose Masm32 directories
void FRMPropertiesChooseROOTDIR(HWND hwnd)
{
    CStr NewROOTDIR;
    CStr NewMAsmCaption;
    CStr BufString;

    if(hwnd == FRMPropertiesTxtROOTDIR) NewMAsmCaption = "Select main directory...";
    else if(hwnd == FRMPropertiesTxtBINDIR) NewMAsmCaption = "Select binaries directory...";
    else if(hwnd == FRMPropertiesTxtINCLUDEDIR) NewMAsmCaption = "Select includes directory...";
    else if(hwnd == FRMPropertiesTxtLIBDIR) NewMAsmCaption = "Select libraries/units directory...";
    else if(hwnd == FRMPropertiesTxtHELPDIR) NewMAsmCaption = "Select help directory...";
    else if(hwnd == FRMPropertiesTxtPROJECTSDIR) NewMAsmCaption = "Select Chromatic projects directory...";
    NewROOTDIR = ComDlgBrowseForFolder(FRMPropertieshwnd, NewMAsmCaption);
    if(NewROOTDIR.Len() != 0)
    {
        ControlSetText(hwnd, NewROOTDIR);
        if(hwnd == FRMPropertiesTxtROOTDIR)
        {
            BufString = "Populate directories from '" + (CStr) NewROOTDIR + (CStr) "' ?";
            if(MiscMsgBox(FRMPropertieshwnd, BufString, MB_QUESTION, Requesters) == IDYES)
            {
                if(strcmp(NewROOTDIR.Right(1).Get_String(), "\\") == 0) NewROOTDIR = NewROOTDIR.Left(NewROOTDIR.Len() - 1);
                if(FileExist(NewROOTDIR + (CStr) "\\bin\\win32"))
                {
                    ControlSetText(FRMPropertiesTxtBINDIR, NewROOTDIR + (CStr) "\\bin\\win32");
                }
                else
                {
                    if(FileExist(NewROOTDIR + (CStr) "\\bin"))
                    {
                        ControlSetText(FRMPropertiesTxtBINDIR, NewROOTDIR + (CStr) "\\bin");
                    }
                    else
                    {
                        ControlSetText(FRMPropertiesTxtBINDIR, "");
                    }
                }
                if(FileExist(NewROOTDIR + (CStr) "\\include"))
                {
                    ControlSetText(FRMPropertiesTxtINCLUDEDIR, NewROOTDIR + (CStr) "\\include");
                }
                else
                {
                    ControlSetText(FRMPropertiesTxtINCLUDEDIR, "");
                }
                if(FileExist(NewROOTDIR + (CStr) "\\lib"))
                {
                    ControlSetText(FRMPropertiesTxtLIBDIR, NewROOTDIR + (CStr) "\\lib");
                }
                else
                {
                    if(FileExist(NewROOTDIR + (CStr) "\\units\\win32\\rtl"))
                    {
                        ControlSetText(FRMPropertiesTxtLIBDIR, NewROOTDIR + (CStr) "\\units\\win32\\rtl");
                    }
                    else
                    {
                        if(FileExist(NewROOTDIR + (CStr) "\\units\\win32"))
                        {
                            ControlSetText(FRMPropertiesTxtLIBDIR, NewROOTDIR + (CStr) "\\units\\win32");
                        }
                        else
                        {
                            if(FileExist(NewROOTDIR + (CStr) "\\units"))
                            {
                                ControlSetText(FRMPropertiesTxtLIBDIR, NewROOTDIR + (CStr) "\\units");
                            }
                            else
                            {
                                ControlSetText(FRMPropertiesTxtLIBDIR, "");
                            }
                        }
                    }
                }
                if(FileExist(NewROOTDIR + (CStr) "\\help"))
                {
                    ControlSetText(FRMPropertiesTxtHELPDIR, NewROOTDIR + (CStr) "\\help");
                }
                else
                {
                    if(FileExist(NewROOTDIR + (CStr) "\\doc"))
                    {
                        ControlSetText(FRMPropertiesTxtHELPDIR, NewROOTDIR + (CStr) "\\doc");
                    }
                    else
                    {
                        ControlSetText(FRMPropertiesTxtHELPDIR, "");
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Load profiles datas
void LoadColorsProfile(CStr ProfileName)
{
    CStr RetColProf;
    CStr RetValProf;
    CStr BufString;

    // Load profiles datas
    RetColProf = IniReadKey(ProfileName, "BackColor", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_BackColor = RetColProf.Get_Hex();
    Orig_Prof_BackColor = Prof_BackColor;
    RetColProf = IniReadKey(ProfileName, "NormText", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_NormText = RetColProf.Get_Hex();
    Orig_Prof_NormText = Prof_NormText;
    RetColProf = IniReadKey(ProfileName, "HighText", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_HighText = RetColProf.Get_Hex();
    Orig_Prof_HighText = Prof_HighText;
    RetColProf = IniReadKey(ProfileName, "MarginColor", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_MarginColor = RetColProf.Get_Hex();
    Orig_Prof_MarginColor = Prof_MarginColor;
    RetColProf = IniReadKey(ProfileName, "LinesColor", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_LinesColor = RetColProf.Get_Hex();
    Orig_Prof_LinesColor = Prof_LinesColor;
    RetColProf = IniReadKey(ProfileName, "NumbersColorValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_NumbersColorValue = RetColProf.Get_Hex();
    Orig_Prof_NumbersColorValue = Prof_NumbersColorValue;
    RetColProf = IniReadKey(ProfileName, "StringsColorValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_StringsColorValue = RetColProf.Get_Hex();
    Orig_Prof_StringsColorValue = Prof_StringsColorValue;
    RetColProf = IniReadKey(ProfileName, "CommentsColorValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_CommentsColorValue = RetColProf.Get_Hex();
    Orig_Prof_CommentsColorValue = Prof_CommentsColorValue;
    RetColProf = IniReadKey(ProfileName, "KeyWordsColorValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_KeyWordsColorValue = RetColProf.Get_Hex();
    Orig_Prof_KeyWordsColorValue = Prof_KeyWordsColorValue;
    RetColProf = IniReadKey(ProfileName, "APIHighColor", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_APIHighColor = RetColProf.Get_Hex();
    Orig_Prof_APIHighColor = Prof_APIHighColor;
    RetColProf = IniReadKey(ProfileName, "ColorAValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorAValue = RetColProf.Get_Hex();
    Orig_Prof_ColorAValue = Prof_ColorAValue;
    RetColProf = IniReadKey(ProfileName, "ColorBValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorBValue = RetColProf.Get_Hex();
    Orig_Prof_ColorBValue = Prof_ColorBValue;
    RetColProf = IniReadKey(ProfileName, "ColorCValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorCValue = RetColProf.Get_Hex();
    Orig_Prof_ColorCValue = Prof_ColorCValue;
    RetColProf = IniReadKey(ProfileName, "ColorDValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorDValue = RetColProf.Get_Hex();
    Orig_Prof_ColorDValue = Prof_ColorDValue;
    RetColProf = IniReadKey(ProfileName, "ColorEValue", ColorsIniFile);
    if(RetColProf.Len() == 0) goto ProfileCorrupted;
    Prof_ColorEValue = RetColProf.Get_Hex();
    Orig_Prof_ColorEValue = Prof_ColorEValue;

    RetValProf = IniReadKey(ProfileName, "ItalicComment", ColorsIniFile);
    if(RetValProf.Len() == 0) goto ProfileCorrupted;
    Prof_ItalicComment = RetValProf.Get_Long();

    // Handle new profile keys
    RetColProf = IniReadKey(ProfileName, "ColorFValue", ColorsIniFile);
    if(RetColProf.Len() == 0) RetColProf = "0";
    Prof_ColorFValue = RetColProf.Get_Hex();
    RetColProf = IniReadKey(ProfileName, "ColorGValue", ColorsIniFile);
    if(RetColProf.Len() == 0) RetColProf = "0";
    Prof_ColorGValue = RetColProf.Get_Hex();
    RetColProf = IniReadKey(ProfileName, "ColorHValue", ColorsIniFile);
    if(RetColProf.Len() == 0) RetColProf = "0";
    Prof_ColorHValue = RetColProf.Get_Hex();
    
    ColorBoxSetColor(FRMPropertiesColBackGround, Prof_BackColor);
    ColorBoxSetColor(FRMPropertiesColNormalText, Prof_NormText);
    ColorBoxSetColor(FRMPropertiesColHighlightText, Prof_HighText);
    ColorBoxSetColor(FRMPropertiesColMargin, Prof_MarginColor);
    ColorBoxSetColor(FRMPropertiesColLineNumber, Prof_LinesColor);
    ColorBoxSetColor(FRMPropertiesColNumber, Prof_NumbersColorValue);
    ColorBoxSetColor(FRMPropertiesColStrings, Prof_StringsColorValue);
    ColorBoxSetColor(FRMPropertiesColComments, Prof_CommentsColorValue);
    ColorBoxSetColor(FRMPropertiesColScopeKeywords, Prof_KeyWordsColorValue);
    ColorBoxSetColor(FRMPropertiesColAPIHighlighting, Prof_APIHighColor);
    ColorBoxSetColor(FRMPropertiesColKeywords, Prof_ColorAValue);
    ColorBoxSetColor(FRMPropertiesColDirectives, Prof_ColorCValue);
    ColorBoxSetColor(FRMPropertiesColRegisters, Prof_ColorDValue);
    ColorBoxSetColor(FRMPropertiesColExtraFunctions, Prof_ColorEValue);
    ColorBoxSetColor(FRMPropertiesColSeparators, Prof_ColorBValue);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight1, Prof_ColorFValue);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight2, Prof_ColorGValue);
    ColorBoxSetColor(FRMPropertiesColExtraHighLight3, Prof_ColorHValue);
    
    // Reset slider
    TrackBarSetPos(FRMPropertiesColLuminosity, 100);
    CheckBoxSetState(FRMPropertiesChkItalic, Prof_ItalicComment);
    BuildPreview(FRMPropertiesCodeMax);
    return;
ProfileCorrupted:
    MiscMsgBox(FRMPropertieshwnd, "Color profile corrupted or missing.", MB_ERROR, Requesters);
    SetFocus(FRMPropertiesCbColors);
}

// -----------------------------------------------------------------------
// Save profiles datas
long SaveColorsProfile(CStr ProfileName)
{
    CStr ProfNameToRegister;
    int i = 0;
    long NoReg = 0;
    CStr UProfName;
    CStr BufString;

    ProfNameToRegister = StringReplace(ProfileName.Trim(), "\t", "", 1, -1, Binary_Compare);
    if(ProfNameToRegister.Len() == 0) return(0);
    UProfName = ProfNameToRegister;
    NoReg = 0;
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = IniReadKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), ColorsIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        if(_strcmpi(FRMPropertiesRetVal.Get_String(), UProfName.Get_String()) == 0)
        {
            NoReg = 1;
            if(MiscMsgBox(FRMPropertieshwnd, "Profile already exists. Overwrite it ?", MB_QUESTION, Requesters) == IDNO)
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
            IniWriteKey("Profiles", "Prof" + (CStr) StringNumberComplement(i, 3).Get_String(), ProfNameToRegister, ColorsIniFile);
            break;
        case 2:
            return(1);
    }
    IniWriteKey(ProfNameToRegister, "BackColor", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColBackGround)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "NormText", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColNormalText)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "HighText", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColHighlightText)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "MarginColor", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColMargin)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "LinesColor", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColLineNumber)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "NumbersColorValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColNumber)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "StringsColorValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColStrings)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "CommentsColorValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColComments)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "KeyWordsColorValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColScopeKeywords)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "APIHighColor", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColAPIHighlighting)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ColorAValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColKeywords)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ColorBValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColSeparators)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ColorCValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColDirectives)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ColorDValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColRegisters)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ColorEValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColExtraFunctions)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ColorFValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColExtraHighLight1)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ColorGValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColExtraHighLight2)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ColorHValue", "0x" + (CStr) BufString.Hex_To_String(ColorBoxGetColor(FRMPropertiesColExtraHighLight3)).Get_String(), ColorsIniFile);
    IniWriteKey(ProfNameToRegister, "ItalicComment", CheckBoxGetState(FRMPropertiesChkItalic), ColorsIniFile);
    FillProfil();
    ControlSetText(FRMPropertiesCbColors, ProfNameToRegister);
    BufString = "Profile '" + (CStr) ProfNameToRegister + (CStr) "' saved.";
    MiscMsgBox(FRMPropertieshwnd, BufString, MB_INFORMATION, Requesters);
    return(1);
}

// -----------------------------------------------------------------------
// Set line displaying
void SetLines(HWND hWnd)
{
    CStr LnSt;

    LnSt = ControlGetText(FRMPropertiesTxtLines);
    if(LnSt.Len() == 0)
    {
        LnSt = "0";
        ControlSetText(FRMPropertiesTxtLines, LnSt);
    }
    SetCodeMaxLineStyle(hWnd, ComboBoxGetIndex(FRMPropertiesCbLines));
}

// -----------------------------------------------------------------------
// Retrieve current line number
long FRMPropertiesGetCurrentLineNumber(HWND hwnd)
{
    CM_GetSel(hwnd, &LineRange, 1);
    return(LineRange.posStart.nLine);
}

// -----------------------------------------------------------------------
// Set comment font style
void FRMPropertiesSetItalicComments(HWND hwnd)
{
    CM_FONTSTYLES PrevFontStyle;
    
    memset(&PrevFontStyle, 0, sizeof(PrevFontStyle));
    switch(CheckBoxGetState(FRMPropertiesChkItalic))
    {
        case 0:
            PrevFontStyle.byComment = CM_FONT_NORMAL;
            break;
        case 1:
            PrevFontStyle.byComment = CM_FONT_ITALIC;
            break;
    }
    PrevFontStyle.byKeyword = CM_FONT_NORMAL;
    PrevFontStyle.byLineNumber = CM_FONT_NORMAL;
    PrevFontStyle.byNumber = CM_FONT_NORMAL;
    PrevFontStyle.byOperator = CM_FONT_NORMAL;
    PrevFontStyle.byScopeKeyword = CM_FONT_NORMAL;
    PrevFontStyle.byString = CM_FONT_NORMAL;
    PrevFontStyle.byExtra1 = CM_FONT_NORMAL;
    PrevFontStyle.byExtra2 = CM_FONT_NORMAL;
    PrevFontStyle.byExtra3 = CM_FONT_NORMAL;
    PrevFontStyle.byTagAttributeName = CM_FONT_NORMAL;
    PrevFontStyle.byTagElementName = CM_FONT_NORMAL;
    PrevFontStyle.byTagEntity = CM_FONT_NORMAL;
    PrevFontStyle.byTagText = CM_FONT_NORMAL;
    PrevFontStyle.byText = CM_FONT_NORMAL;
    CM_SetFontStyles(hwnd, &PrevFontStyle);
}

// -----------------------------------------------------------------------
// Set comment font style
void FRMPropertiesSetCodeMaxColors(HWND hwnd)
{
    CM_COLORS PrevColsToSet;

    memset(&PrevColsToSet, 0, sizeof(PrevColsToSet));
    PrevColsToSet.crText = ColorBoxGetColor(FRMPropertiesColNormalText);
    PrevColsToSet.crTextBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crBookmark = 0xDD7766;
    PrevColsToSet.crBookmarkBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crBreakpoint = 0x0011DD;
    PrevColsToSet.crBreakpointBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crComment = ColorBoxGetColor(FRMPropertiesColComments);
    PrevColsToSet.crCommentBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crKeyword = ColorBoxGetColor(FRMPropertiesColKeywords);
    PrevColsToSet.crKeywordBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crNumber = ColorBoxGetColor(FRMPropertiesColNumber);
    PrevColsToSet.crNumberBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crOperator = ColorBoxGetColor(FRMPropertiesColSeparators);
    PrevColsToSet.crOperatorBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crScopeKeyword = ColorBoxGetColor(FRMPropertiesColScopeKeywords);
    PrevColsToSet.crScopeKeywordBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crString = ColorBoxGetColor(FRMPropertiesColStrings);
    PrevColsToSet.crStringBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crTagAttributeName = ColorBoxGetColor(FRMPropertiesColDirectives);
    PrevColsToSet.crTagAttributeNameBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crTagElementName = ColorBoxGetColor(FRMPropertiesColRegisters);
    PrevColsToSet.crTagElementNameBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crExtra1 = ColorBoxGetColor(FRMPropertiesColExtraHighLight1);
    PrevColsToSet.crExtra1Bk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crExtra2 = ColorBoxGetColor(FRMPropertiesColExtraHighLight2);
    PrevColsToSet.crExtra2Bk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crExtra3 = ColorBoxGetColor(FRMPropertiesColExtraHighLight3);
    PrevColsToSet.crExtra3Bk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crLineNumber = ColorBoxGetColor(FRMPropertiesColLineNumber);
    PrevColsToSet.crLineNumberBk = ColorBoxGetColor(FRMPropertiesColMargin);
    PrevColsToSet.crTagEntity = ColorBoxGetColor(FRMPropertiesColExtraFunctions);
    PrevColsToSet.crTagEntityBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crTagTextBk = ColorBoxGetColor(FRMPropertiesColBackGround);
    PrevColsToSet.crHighlightedLine = ColorBoxGetColor(FRMPropertiesColHighlightText);
    PrevColsToSet.crLeftMargin = ColorBoxGetColor(FRMPropertiesColMargin);
    PrevColsToSet.crVDividerLines = ColorBoxGetColor(FRMPropertiesColMargin);
    PrevColsToSet.crHDividerLines = ColorBoxGetColor(FRMPropertiesColMargin);
    PrevColsToSet.crWindow = ColorBoxGetColor(FRMPropertiesColBackGround);
    // Set colors
    CM_SetColors(hwnd, &PrevColsToSet);
}

// -----------------------------------------------------------------------
// Properties CodeMax hook proc
LRESULT CALLBACK FRMPropertiesCMHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CONTEXTMENU:
            return(0);
        case WM_DROPFILES:
            return(0);
        case WM_MENUSELECT:
            return(0);
        case WM_EXITMENULOOP:
            return(0);
        case WM_COMMAND:
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Select the default backup directory
void FRMPropertiesSelectBakDir(void)
{
    CStr NewBakDir;
    
    NewBakDir = ComDlgBrowseForFolder(FRMPropertieshwnd, "Select default .BAK directory...");
    if(NewBakDir.Len() != 0)
    {
        NewBakDir = ChangeAbsolutePaths(NewBakDir);
        ControlSetText(FRMPropertiesTxtBackupDir, NewBakDir);
    }
}

// -----------------------------------------------------------------------
// CodeMax frame hook proc
LRESULT CALLBACK FRMPropertiesFrameCodeMax(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case CMN_SELCHANGE:
                    if(CheckBoxGetState(FRMPropertiesChkHighlightLine) == 1)
                    {
                        LoadCurrentSel(FRMPropertiesCodeMax);
                        if(CodeMaxCurRange.posStart.nLine != (long) CM_GetHighlightedLine(FRMPropertiesCodeMax)) CM_SetHighlightedLine(FRMPropertiesCodeMax, CodeMaxCurRange.posStart.nLine);
                    }
                    break;
                case CMN_VSCROLL:
                    FRMProPertiesConfineCaret();
                    break;
                case CMN_SHOWPROPS:
                    return(1);
                case CMN_REGISTEREDCMD:
                    return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Managment frame hook proc
LRESULT CALLBACK FRMPropertiesFrameManagment(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesCbAccounts)
            {
                if(ControlGetNotifiedCommand(wParam) == CBN_SELCHANGE)
                {
                    FRMPropertiesInFtpCreation = 0;
                    FillFTPProps();
                    SetFocus(FRMPropertiesTxtIP);
                }
                return(0);
            } 
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Main frame hook proc
LRESULT CALLBACK FRMPropertiesFrameMain(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesChkAnonymous)
            {
                SetAnonymousState(0);
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Make caret always visible
void FRMProPertiesConfineCaret(void)
{
    long ViCount = 0;
    long ViTop = 0;

    if(CheckBoxGetState(FRMPropertiesChkCaretAlwaysVisible) == 1)
    {
        LoadCurrentSel(FRMPropertiesCodeMax);
        CodeMaxCurRange.posStart.nLine = CodeMaxCurRange.posEnd.nLine;
        CodeMaxCurRange.posStart.nCol = CodeMaxCurRange.posEnd.nCol;
        ViCount = CM_GetVisibleLineCount(FRMPropertiesCodeMax, CM_GetCurrentView(FRMPropertiesCodeMax), 1);
        ViTop = CM_GetTopIndex(FRMPropertiesCodeMax, CM_GetCurrentView(FRMPropertiesCodeMax));
        if(CodeMaxCurRange.posStart.nLine < (long) ViTop)
        {
            CodeMaxCurRange.posStart.nLine = ViTop;
            CodeMaxCurRange.posEnd.nLine = ViTop;
            CM_SetSel(FRMPropertiesCodeMax, &CodeMaxCurRange, 1);
            return;
        }
        if(CodeMaxCurRange.posStart.nLine >= (long) (ViTop + ViCount))
        {
            CodeMaxCurRange.posStart.nLine = ViTop + ViCount - 1;
            CodeMaxCurRange.posEnd.nLine = ViTop + ViCount - 1;
            CM_SetSel(FRMPropertiesCodeMax, &CodeMaxCurRange, 1);
            return;
        }
    }
}
    
// -----------------------------------------------------------------------
// Refresh the codemax childs
void RefreshChilds(void)
{
    if(NbForms != 0) EnumChildWindows(hMDIform.hClient, &EnumRefreshChilds, 0);
}

int CALLBACK EnumRefreshChilds(HWND hWnd, long lParam)
{
    if(NbForms == 0)
    {
        return(0);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(CMGetRealFile(ChildStruct->RFile).Len() != 0)
                {
                    // hChildCodeMax can be null in case of a custom child
                    if(ChildStruct->hChildCodeMax != NULL)
                    {
                        SetCodeMaxColors(ChildStruct->hChildCodeMax);
                        if(ChildStruct->CodeMaxPropertiesHook != NULL)
                        {
                            // Run hook
                            ChildStruct->CodeMaxPropertiesHook(ChildStruct->hChildCodeMax);
                        }
                    }
                }
            }
        }
        return(1);
    }
}

// -----------------------------------------------------------------------
// Build ftp accounts list
void BuildFTP(HWND hComboBox)
{
    FillFTPCombo(hComboBox);
    if(ComboBoxCount(FRMPropertiesCbAccounts) != 0)
    {
        ComboBoxSetIndex(FRMPropertiesCbAccounts, 0);
        FillFTPProps();
    }
}

// -----------------------------------------------------------------------
// Fill all ftp account fields
void FillFTPProps(void)
{
    CStr AccountName;
    CStr DecryptedPass;

    AccountName = ControlGetText(FRMPropertiesCbAccounts);
    if(AccountName.Len() == 0) return;
    ControlSetText(FRMPropertiesTxtIP, IniReadKey(AccountName, "FTPIP", FtpAccountsIniFile));
    FRMPropertiesRetVal = IniReadKey(AccountName, "FTPAnon", FtpAccountsIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        CheckBoxSetState(FRMPropertiesChkAnonymous, FRMPropertiesRetVal.Get_Long());
    }
    else
    {
        CheckBoxSetState(FRMPropertiesChkAnonymous, 0);
    }
    SetAnonymousState(1);
    ControlSetText(FRMPropertiesTxtUserName, IniReadKey(AccountName, "FTPUserName", FtpAccountsIniFile));
    FRMPropertiesRetVal = IniReadKey(AccountName, "FTPSavePass", FtpAccountsIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        ComboBoxSetIndex(FRMPropertiesCbPasswordType, FRMPropertiesRetVal.Get_Long());
        switch(FRMPropertiesRetVal.Get_Long())
        {
            case 0:
                ControlSetText(FRMPropertiesTxtPassword, "");
                break;
            case 1:
                ControlSetText(FRMPropertiesTxtPassword, IniReadKey(AccountName, "FTPUserPass", FtpAccountsIniFile));
                break;
            case 2:
                // Load password and decrypt it
                DecryptedPass = IniReadKey(AccountName, "FTPUserPass", FtpAccountsIniFile);
                ControlSetText(FRMPropertiesTxtPassword, DecryptPassword(DecryptedPass));
                break;
        }
    }
    else
    {
        ComboBoxSetIndex(FRMPropertiesCbPasswordType, 0);
        ControlSetText(FRMPropertiesTxtPassword, "");
    }
    ControlSetText(FRMPropertiesTxtInitialPath, IniReadKey(AccountName, "FTPPath", FtpAccountsIniFile));
    ControlSetText(FRMPropertiesTxtFTPPort, IniReadKey(AccountName, "FTPPort", FtpAccountsIniFile));
    
    ControlSetText(FRMPropertiesTxtFTPTimeOut, IniReadKey(AccountName, "FTPTimeOut", FtpAccountsIniFile));
    FRMPropertiesRetVal = IniReadKey(AccountName, "FTPTransfersType", FtpAccountsIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        switch(FRMPropertiesRetVal.Get_Long())
        {
            case 1:
                RadioButtonSetState(FRMPropertiesRBASCII, 1);
                RadioButtonSetState(FRMPropertiesRBBinary, 0);
                break;
            case 2:
                RadioButtonSetState(FRMPropertiesRBBinary, 1);
                RadioButtonSetState(FRMPropertiesRBASCII, 0);
                break;
        }
    }
    else
    {
        RadioButtonSetState(FRMPropertiesRBASCII, 0);
        RadioButtonSetState(FRMPropertiesRBBinary, 1);
    }
    FRMPropertiesRetVal = IniReadKey(AccountName, "FTPPassive", FtpAccountsIniFile);
    if(FRMPropertiesRetVal.Len() != 0)
    {
        CheckBoxSetState(FRMPropertiesChkPassive, FRMPropertiesRetVal.Get_Long());
    }
    else
    {
        CheckBoxSetState(FRMPropertiesChkPassive, 0);
    }
}

// -----------------------------------------------------------------------
// Reset all ftp account fields
void ResetFTPProps(void)
{
    ControlSetText(FRMPropertiesTxtIP, "");
    CheckBoxSetState(FRMPropertiesChkAnonymous, 0);
    SetAnonymousState(0);
    ControlSetText(FRMPropertiesTxtUserName, "");
    ComboBoxSetIndex(FRMPropertiesCbPasswordType, 0);
    ControlSetText(FRMPropertiesTxtPassword, "");
    ControlSetText(FRMPropertiesTxtInitialPath, "");
    ControlSetText(FRMPropertiesTxtFTPPort, "");
    ControlSetText(FRMPropertiesTxtFTPTimeOut, "");
    RadioButtonSetState(FRMPropertiesRBASCII, 0);
    RadioButtonSetState(FRMPropertiesRBBinary, 1);
    CheckBoxSetState(FRMPropertiesChkPassive, 0);
}

// -----------------------------------------------------------------------
// Enable/Disable anonymous state
void SetAnonymousState(long DontFocus)
{
    switch(CheckBoxGetState(FRMPropertiesChkAnonymous))
    {
        case 0:
            ControlEnable(FRMPropertiesTxtUserName, 1);
            ControlEnable(FRMPropertiesTxtPassword, 1);
            ControlEnable(FRMPropertiesCbPasswordType, 1);
            if(DontFocus == 0) SetFocus(FRMPropertiesTxtUserName);
            break;
        case 1:
            ControlEnable(FRMPropertiesTxtUserName, 0);
            ControlEnable(FRMPropertiesTxtPassword, 0);
            ControlEnable(FRMPropertiesCbPasswordType, 0);
            break;
    }
}

// -----------------------------------------------------------------------
// Save all ftp account fields
void SaveFTPProps(void)
{
    int i = 0;
    CStr AccountName;
    CStr BufString;
    
    AccountName = ControlGetText(FRMPropertiesCbAccounts);
    if(AccountName.Len() == 0) return;
    // Search account key
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = IniReadKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), FtpAccountsIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        if(_strcmpi(AccountName.Get_String(), FRMPropertiesRetVal.Get_String()) == 0) break;
    }
    // Add account name
    IniWriteKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), AccountName, FtpAccountsIniFile);
    // Save account datas
    IniWriteKey(AccountName, "FTPIP", ControlGetText(FRMPropertiesTxtIP), FtpAccountsIniFile);
    IniWriteKey(AccountName, "FTPAnon", CheckBoxGetState(FRMPropertiesChkAnonymous), FtpAccountsIniFile);
    IniWriteKey(AccountName, "FTPUserName", ControlGetText(FRMPropertiesTxtUserName), FtpAccountsIniFile);
    switch(ComboBoxGetIndex(FRMPropertiesCbPasswordType))
    {
        case 0:
            IniWriteKey(AccountName, "FTPUserPass", "", FtpAccountsIniFile);
            break;
        case 1:
            IniWriteKey(AccountName, "FTPUserPass", ControlGetText(FRMPropertiesTxtPassword), FtpAccountsIniFile);
            break;
        case 2:
            IniWriteKey(AccountName, "FTPUserPass", CryptPassword(ControlGetText(FRMPropertiesTxtPassword)), FtpAccountsIniFile);
            break;
    }
    IniWriteKey(AccountName, "FTPSavePass ", ComboBoxGetIndex(FRMPropertiesCbPasswordType), FtpAccountsIniFile);
    IniWriteKey(AccountName, "FTPPath", ControlGetText(FRMPropertiesTxtInitialPath), FtpAccountsIniFile);
    IniWriteKey(AccountName, "FTPPort", ControlGetText(FRMPropertiesTxtFTPPort), FtpAccountsIniFile);
    IniWriteKey(AccountName, "FTPTimeOut", ControlGetText(FRMPropertiesTxtFTPTimeOut), FtpAccountsIniFile);
    switch(RadioButtonGetState(FRMPropertiesRBASCII))
    {
        case 1:
            IniWriteKey(AccountName, "FTPTransfersType", "1", FtpAccountsIniFile);
            break;
    }
    switch(RadioButtonGetState(FRMPropertiesRBBinary))
    {
        case 1:
            IniWriteKey(AccountName, "FTPTransfersType", "2", FtpAccountsIniFile);
            break;
    }
    IniWriteKey(AccountName, "FTPPassive", CheckBoxGetState(FRMPropertiesChkPassive), FtpAccountsIniFile);
    BufString = "FTP account '" + (CStr) AccountName + (CStr) "' updated.",
    MiscMsgBox(FRMPropertieshwnd, BufString, MB_INFORMATION, Requesters);
}

// -----------------------------------------------------------------------
// Set the current tab to view
void FRMPropertiesChangeCurrentTab(void)
{
    switch(SysTabGetCurrentItem(FRMPropertiesSysTab))
    {
        case 0:
            ControlVisible(FRMPropertiesPageColors, 0);
            ControlVisible(FRMPropertiesTbColors, 0);
            ControlVisible(FRMPropertiesPageAPIHelp, 0);
            ControlVisible(FRMPropertiesCmdWindowsAPI, 0);
            ControlVisible(FRMPropertiesPageSDKHelp, 0);
            ControlVisible(FRMPropertiesCmdMSDN, 0);
            ControlVisible(FRMPropertiesPageCodeMax, 0);
            ControlVisible(FRMPropertiesPageDDKHelp, 0);
            ControlVisible(FRMPropertiesCmdDDK, 0);
            ControlVisible(FRMPropertiesPageDirectXSDKHelp, 0);
            ControlVisible(FRMPropertiesCmdDirectXSDK, 0);
            ControlVisible(FRMPropertiesPageExtrasLayouts, 0);
            ControlVisible(FRMPropertiesCmdRegs, 0);
            ControlVisible(FRMPropertiesPageFolders, 0);
            ControlVisible(FRMPropertiesTbROOTDIR, 0);
            ControlVisible(FRMPropertiesTbBINDIR, 0);
            ControlVisible(FRMPropertiesTbINCLUDEDIR, 0);
            ControlVisible(FRMPropertiesTbLIBDIR, 0);
            ControlVisible(FRMPropertiesTbHELPDIR, 0);
            ControlVisible(FRMPropertiesTbPROJECTSDIR, 0);
            ControlVisible(FRMPropertiesPageProjects, 0);
            ControlVisible(FRMPropertiesCmdDebugger, 0);
            ControlVisible(FRMPropertiesTbValidDebugger, 0);
            ControlVisible(FRMPropertiesPageProtoGen, 0);
            ControlVisible(FRMPropertiesCmdGenProg, 0);
            ControlVisible(FRMPropertiesPageFTPManage, 0);
            ControlVisible(FRMPropertiesTbAccounts, 0);
            ControlVisible(FRMPropertiesPageAccountInfos, 0);
            ControlVisible(FRMPropertiesPageTransfersTypes, 0);
            ControlVisible(FRMPropertiesPageCodingHelp, 0);
            ControlVisible(FRMPropertiesPageLanguages, 0);
            ControlVisible(FRMPropertiesTbLanguages, 0);
            ControlVisible(FRMPropertiesLbLanguages, 0);
            ControlVisible(FRMPropertiesTxtLanguageFile, 0);
            ControlVisible(FRMPropertiesTxtLanguageExtensions, 0);
            ControlVisible(FRMPropertiesLbLanguageFile, 0);
            ControlVisible(FRMPropertiesLbLanguageExtensions, 0);
            ControlVisible(FRMPropertiesLbLanguageDescName, 0);
            ControlVisible(FRMPropertiesLbLanguageDescCopyright, 0);
            ControlVisible(FRMPropertiesLbLanguageDescComment, 0);
            ControlVisible(FRMPropertiesPageExtrasLayouts2, 0);
            ControlVisible(FRMPropertiesPageEditor, 1);
            ControlVisible(FRMPropertiesPageFileExtensions, 1);
            ControlVisible(FRMPropertiesTbFileExt, 1);
            ControlVisible(FRMPropertiesPageOther, 1);
            ControlVisible(FRMPropertiesPageFont, 1);
            ControlVisible(FRMPropertiesCmdFontChoose, 1);
            ControlVisible(FRMPropertiesPageAutoSave, 1);
            ControlVisible(FRMPropertiesPageEditor2, 1);
            ControlVisible(FRMPropertiesPageLineNumbering, 1);
            ControlVisible(FRMPropertiesPageBackup, 1);
            ControlVisible(FRMPropertiesTbBackup, 1);
            ControlBringToTop(FRMPropertiesTbBackup);
            ControlBringToTop(FRMPropertiesTbFileExt);
            ControlBringToTop(FRMPropertiesPageEditor);
            break;
        case 1:
            ControlVisible(FRMPropertiesPageEditor, 0);
            ControlVisible(FRMPropertiesPageFileExtensions, 0);
            ControlVisible(FRMPropertiesTbFileExt, 0);
            ControlVisible(FRMPropertiesPageOther, 0);
            ControlVisible(FRMPropertiesPageFont, 0);
            ControlVisible(FRMPropertiesCmdFontChoose, 0);
            ControlVisible(FRMPropertiesPageAutoSave, 0);
            ControlVisible(FRMPropertiesPageEditor2, 0);
            ControlVisible(FRMPropertiesPageLineNumbering, 0);
            ControlVisible(FRMPropertiesPageBackup, 0);
            ControlVisible(FRMPropertiesTbBackup, 0);
            ControlVisible(FRMPropertiesPageFolders, 0);
            ControlVisible(FRMPropertiesTbROOTDIR, 0);
            ControlVisible(FRMPropertiesTbBINDIR, 0);
            ControlVisible(FRMPropertiesTbINCLUDEDIR, 0);
            ControlVisible(FRMPropertiesTbLIBDIR, 0);
            ControlVisible(FRMPropertiesTbHELPDIR, 0);
            ControlVisible(FRMPropertiesTbPROJECTSDIR, 0);
            ControlVisible(FRMPropertiesPageProjects, 0);
            ControlVisible(FRMPropertiesCmdDebugger, 0);
            ControlVisible(FRMPropertiesTbValidDebugger, 0);
            ControlVisible(FRMPropertiesPageProtoGen, 0);
            ControlVisible(FRMPropertiesCmdGenProg, 0);
            ControlVisible(FRMPropertiesPageFTPManage, 0);
            ControlVisible(FRMPropertiesTbAccounts, 0);
            ControlVisible(FRMPropertiesPageAccountInfos, 0);
            ControlVisible(FRMPropertiesPageTransfersTypes, 0);
            ControlVisible(FRMPropertiesPageCodingHelp, 0);
            ControlVisible(FRMPropertiesPageLanguages, 0);
            ControlVisible(FRMPropertiesTbLanguages, 0);
            ControlVisible(FRMPropertiesLbLanguages, 0);
            ControlVisible(FRMPropertiesTxtLanguageFile, 0);
            ControlVisible(FRMPropertiesTxtLanguageExtensions, 0);
            ControlVisible(FRMPropertiesLbLanguageFile, 0);
            ControlVisible(FRMPropertiesLbLanguageExtensions, 0);
            ControlVisible(FRMPropertiesLbLanguageDescName, 0);
            ControlVisible(FRMPropertiesLbLanguageDescCopyright, 0);
            ControlVisible(FRMPropertiesLbLanguageDescComment, 0);
            ControlVisible(FRMPropertiesPageExtrasLayouts2, 0);
            BuildPreview(FRMPropertiesCodeMax);
            ControlVisible(FRMPropertiesPageColors, 1);
            ControlVisible(FRMPropertiesTbColors, 1);
            ControlVisible(FRMPropertiesPageAPIHelp, 1);
            ControlVisible(FRMPropertiesCmdWindowsAPI, 1);
            ControlVisible(FRMPropertiesPageSDKHelp, 1);
            ControlVisible(FRMPropertiesCmdMSDN, 1);
            ControlVisible(FRMPropertiesPageCodeMax, 1);
            ControlVisible(FRMPropertiesPageDDKHelp, 1);
            ControlVisible(FRMPropertiesCmdDDK, 1);
            ControlVisible(FRMPropertiesPageDirectXSDKHelp, 1);
            ControlVisible(FRMPropertiesCmdDirectXSDK, 1);
            ControlVisible(FRMPropertiesPageExtrasLayouts, 1);
            ControlVisible(FRMPropertiesCmdRegs, 1);
            ControlBringToTop(FRMPropertiesTbColors);
            break;
        case 2:
            ControlVisible(FRMPropertiesPageEditor, 0);
            ControlVisible(FRMPropertiesPageFileExtensions, 0);
            ControlVisible(FRMPropertiesTbFileExt, 0);
            ControlVisible(FRMPropertiesPageOther, 0);
            ControlVisible(FRMPropertiesPageFont, 0);
            ControlVisible(FRMPropertiesCmdFontChoose, 0);
            ControlVisible(FRMPropertiesPageAutoSave, 0);
            ControlVisible(FRMPropertiesPageEditor2, 0);
            ControlVisible(FRMPropertiesPageLineNumbering, 0);
            ControlVisible(FRMPropertiesPageBackup, 0);
            ControlVisible(FRMPropertiesTbBackup, 0);
            ControlVisible(FRMPropertiesPageColors, 0);
            ControlVisible(FRMPropertiesTbColors, 0);
            ControlVisible(FRMPropertiesPageAPIHelp, 0);
            ControlVisible(FRMPropertiesCmdWindowsAPI, 0);
            ControlVisible(FRMPropertiesPageSDKHelp, 0);
            ControlVisible(FRMPropertiesCmdMSDN, 0);
            ControlVisible(FRMPropertiesPageCodeMax, 0);
            ControlVisible(FRMPropertiesPageDDKHelp, 0);
            ControlVisible(FRMPropertiesCmdDDK, 0);
            ControlVisible(FRMPropertiesPageDirectXSDKHelp, 0);
            ControlVisible(FRMPropertiesCmdDirectXSDK, 0);
            ControlVisible(FRMPropertiesPageExtrasLayouts, 0);
            ControlVisible(FRMPropertiesCmdRegs, 0);
            ControlVisible(FRMPropertiesPageFTPManage, 0);
            ControlVisible(FRMPropertiesTbAccounts, 0);
            ControlVisible(FRMPropertiesPageAccountInfos, 0);
            ControlVisible(FRMPropertiesPageTransfersTypes, 0);
            ControlVisible(FRMPropertiesPageCodingHelp, 0);
            ControlVisible(FRMPropertiesPageLanguages, 0);
            ControlVisible(FRMPropertiesTbLanguages, 0);
            ControlVisible(FRMPropertiesLbLanguages, 0);
            ControlVisible(FRMPropertiesTxtLanguageFile, 0);
            ControlVisible(FRMPropertiesTxtLanguageExtensions, 0);
            ControlVisible(FRMPropertiesLbLanguageFile, 0);
            ControlVisible(FRMPropertiesLbLanguageExtensions, 0);
            ControlVisible(FRMPropertiesLbLanguageDescName, 0);
            ControlVisible(FRMPropertiesLbLanguageDescCopyright, 0);
            ControlVisible(FRMPropertiesLbLanguageDescComment, 0);
            ControlVisible(FRMPropertiesPageExtrasLayouts2, 0);
            ControlVisible(FRMPropertiesPageFolders, 1);
            ControlVisible(FRMPropertiesTbROOTDIR, 1);
            ControlVisible(FRMPropertiesTbBINDIR, 1);
            ControlVisible(FRMPropertiesTbINCLUDEDIR, 1);
            ControlVisible(FRMPropertiesTbLIBDIR, 1);
            ControlVisible(FRMPropertiesTbHELPDIR, 1);
            ControlVisible(FRMPropertiesTbPROJECTSDIR, 1);
            ControlVisible(FRMPropertiesPageProjects, 1);
            ControlVisible(FRMPropertiesCmdDebugger, 1);
            ControlVisible(FRMPropertiesTbValidDebugger, 1);
            ControlVisible(FRMPropertiesPageProtoGen, 1);
            ControlVisible(FRMPropertiesCmdGenProg, 1);
            ControlBringToTop(FRMPropertiesTbROOTDIR);
            ControlBringToTop(FRMPropertiesTbBINDIR);
            ControlBringToTop(FRMPropertiesTbINCLUDEDIR);
            ControlBringToTop(FRMPropertiesTbLIBDIR);
            ControlBringToTop(FRMPropertiesTbHELPDIR);
            ControlBringToTop(FRMPropertiesTbPROJECTSDIR);
            ControlBringToTop(FRMPropertiesTbValidDebugger);
            break;
        case 3:
            ControlVisible(FRMPropertiesPageEditor, 0);
            ControlVisible(FRMPropertiesPageFileExtensions, 0);
            ControlVisible(FRMPropertiesTbFileExt, 0);
            ControlVisible(FRMPropertiesPageOther, 0);
            ControlVisible(FRMPropertiesPageFont, 0);
            ControlVisible(FRMPropertiesCmdFontChoose, 0);
            ControlVisible(FRMPropertiesPageAutoSave, 0);
            ControlVisible(FRMPropertiesPageEditor2, 0);
            ControlVisible(FRMPropertiesPageLineNumbering, 0);
            ControlVisible(FRMPropertiesPageBackup, 0);
            ControlVisible(FRMPropertiesTbBackup, 0);
            ControlVisible(FRMPropertiesPageColors, 0);
            ControlVisible(FRMPropertiesTbColors, 0);
            ControlVisible(FRMPropertiesPageAPIHelp, 0);
            ControlVisible(FRMPropertiesCmdWindowsAPI, 0);
            ControlVisible(FRMPropertiesPageSDKHelp, 0);
            ControlVisible(FRMPropertiesCmdMSDN, 0);
            ControlVisible(FRMPropertiesPageCodeMax, 0);
            ControlVisible(FRMPropertiesPageDDKHelp, 0);
            ControlVisible(FRMPropertiesCmdDDK, 0);
            ControlVisible(FRMPropertiesPageDirectXSDKHelp, 0);
            ControlVisible(FRMPropertiesCmdDirectXSDK, 0);
            ControlVisible(FRMPropertiesPageExtrasLayouts, 0);
            ControlVisible(FRMPropertiesCmdRegs, 0);
            ControlVisible(FRMPropertiesPageFolders, 0);
            ControlVisible(FRMPropertiesTbROOTDIR, 0);
            ControlVisible(FRMPropertiesTbBINDIR, 0);
            ControlVisible(FRMPropertiesTbINCLUDEDIR, 0);
            ControlVisible(FRMPropertiesTbLIBDIR, 0);
            ControlVisible(FRMPropertiesTbHELPDIR, 0);
            ControlVisible(FRMPropertiesTbPROJECTSDIR, 0);
            ControlVisible(FRMPropertiesPageProjects, 0);
            ControlVisible(FRMPropertiesCmdDebugger, 0);
            ControlVisible(FRMPropertiesTbValidDebugger, 0);
            ControlVisible(FRMPropertiesPageProtoGen, 0);
            ControlVisible(FRMPropertiesCmdGenProg, 0);
            ControlVisible(FRMPropertiesPageCodingHelp, 0);
            ControlVisible(FRMPropertiesPageLanguages, 0);
            ControlVisible(FRMPropertiesTbLanguages, 0);
            ControlVisible(FRMPropertiesPageExtrasLayouts2, 0);
            ControlVisible(FRMPropertiesPageFTPManage, 1);
            ControlVisible(FRMPropertiesTbAccounts, 1);
            ControlVisible(FRMPropertiesPageAccountInfos, 1);
            ControlVisible(FRMPropertiesPageTransfersTypes, 1);
            ControlBringToTop(FRMPropertiesTbAccounts);
            break;
        case 4:
            ControlVisible(FRMPropertiesPageEditor, 0);
            ControlVisible(FRMPropertiesPageFileExtensions, 0);
            ControlVisible(FRMPropertiesTbFileExt, 0);
            ControlVisible(FRMPropertiesPageOther, 0);
            ControlVisible(FRMPropertiesPageFont, 0);
            ControlVisible(FRMPropertiesCmdFontChoose, 0);
            ControlVisible(FRMPropertiesPageAutoSave, 0);
            ControlVisible(FRMPropertiesPageEditor2, 0);
            ControlVisible(FRMPropertiesPageLineNumbering, 0);
            ControlVisible(FRMPropertiesPageBackup, 0);
            ControlVisible(FRMPropertiesTbBackup, 0);
            ControlVisible(FRMPropertiesPageColors, 0);
            ControlVisible(FRMPropertiesTbColors, 0);
            ControlVisible(FRMPropertiesPageAPIHelp, 0);
            ControlVisible(FRMPropertiesCmdWindowsAPI, 0);
            ControlVisible(FRMPropertiesPageSDKHelp, 0);
            ControlVisible(FRMPropertiesCmdMSDN, 0);
            ControlVisible(FRMPropertiesPageCodeMax, 0);
            ControlVisible(FRMPropertiesPageDDKHelp, 0);
            ControlVisible(FRMPropertiesCmdDDK, 0);
            ControlVisible(FRMPropertiesPageDirectXSDKHelp, 0);
            ControlVisible(FRMPropertiesCmdDirectXSDK, 0);
            ControlVisible(FRMPropertiesPageExtrasLayouts, 0);
            ControlVisible(FRMPropertiesCmdRegs, 0);
            ControlVisible(FRMPropertiesPageFolders, 0);
            ControlVisible(FRMPropertiesTbROOTDIR, 0);
            ControlVisible(FRMPropertiesTbBINDIR, 0);
            ControlVisible(FRMPropertiesTbINCLUDEDIR, 0);
            ControlVisible(FRMPropertiesTbLIBDIR, 0);
            ControlVisible(FRMPropertiesTbHELPDIR, 0);
            ControlVisible(FRMPropertiesTbPROJECTSDIR, 0);
            ControlVisible(FRMPropertiesPageProjects, 0);
            ControlVisible(FRMPropertiesCmdDebugger, 0);
            ControlVisible(FRMPropertiesTbValidDebugger, 0);
            ControlVisible(FRMPropertiesPageProtoGen, 0);
            ControlVisible(FRMPropertiesCmdGenProg, 0);
            ControlVisible(FRMPropertiesPageFTPManage, 0);
            ControlVisible(FRMPropertiesTbAccounts, 0);
            ControlVisible(FRMPropertiesPageAccountInfos, 0);
            ControlVisible(FRMPropertiesPageTransfersTypes, 0);
            ControlVisible(FRMPropertiesPageCodingHelp, 1);
            ControlVisible(FRMPropertiesPageLanguages, 1);
            ControlVisible(FRMPropertiesTbLanguages, 1);
            ControlVisible(FRMPropertiesLbLanguages, 1);
            ControlVisible(FRMPropertiesTxtLanguageFile, 1);
            ControlVisible(FRMPropertiesTxtLanguageExtensions, 1);
            ControlVisible(FRMPropertiesLbLanguageFile, 1);
            ControlVisible(FRMPropertiesLbLanguageExtensions, 1);
            ControlVisible(FRMPropertiesLbLanguageDescName, 1);
            ControlVisible(FRMPropertiesLbLanguageDescCopyright, 1);
            ControlVisible(FRMPropertiesLbLanguageDescComment, 1);
            ControlVisible(FRMPropertiesPageExtrasLayouts2, 1);
            ControlBringToTop(FRMPropertiesTbLanguages);
    }
}

// -----------------------------------------------------------------------
// Delete an account in ftpaccounts ini file
void DeleteAccountFromIni(CStr AccountToDelete)
{
    int i = 0;

    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = IniReadKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), FtpAccountsIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        if(_strcmpi(AccountToDelete.Get_String(), FRMPropertiesRetVal.Get_String()) == 0)
        {
            IniDeleteKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPIP", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPAnon", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPUserName", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPUserPass", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPSavePass ", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPPath", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPPort", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPUseProxy", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPTransfersType", FtpAccountsIniFile);
            IniDeleteKey(AccountToDelete, "FTPPassive", FtpAccountsIniFile);
            break;
        }
    }
}

// -----------------------------------------------------------------------
// Read skins definitions
void Fillskins(void)
{
    int i = 0;
    CStr SkinToAdd;
    long SelectedIdx = 0;
    
    PrgSkinArray.Erase();
    PrfCurrentSkin = IniReadKey("BuildSkins", "CurrentSkin", SkinsIniFile);
    PrfCurrentSkin = ChangeRelativePaths(PrfCurrentSkin);
    for(i = 0; i <= 999; i++)
    {
        SkinToAdd = IniReadKey("BuildSkins", "Skin" + (CStr) StringNumberComplement(i, 3).Get_String(), SkinsIniFile);
        if(SkinToAdd.Len() == 0) break;
        SkinToAdd = ChangeRelativePaths(SkinToAdd);
        if(_strcmpi(SkinToAdd.Get_String(), PrfCurrentSkin.Get_String()) == 0) SelectedIdx = i;
        PrgSkinArray.Add(SkinToAdd.Get_String());
        SkinToAdd = IniReadKey("SKINFOS", "Title", SkinToAdd);
        ComboBoxAddItem(FRMPropertiesCbSkin, SkinToAdd, -1);
    }
    ComboBoxSetIndex(FRMPropertiesCbSkin, SelectedIdx);
}

// -----------------------------------------------------------------------
// Read skins datas
void FillCurrentSkin(void)
{
    int i = 0;
    int j = 0;
    int k = 0;
    long ctId = 0;
    long CurrentRadio = 0;
    long FirstRadio = 0;
    long *ElementArray = 0;
    HWND ElementLabel = 0;
    CStr Skinelement;
    HWND ElementhWnd = 0;
    HWND ElementFrame = 0;
    CStr ElementName;
    CStr ElementCoords;
    CStr ElementSelected;
    CStr BufString;

    PrgSkinControls.Erase();
    PrgSkinControlsDest.Erase();
    PrgSkinLabels.Erase();
    SkinListViews.Erase();
    ctId = 22;               // Minimum ID
    for(i = 0; i <= 999; i++)
    {
        Skinelement = IniReadKey("TEMPHEADER", "Opt" + (CStr) StringNumberComplement(i, 3).Get_String(), PrfCurrentSkin);
        if(Skinelement.Len() == 0) break;
        BufString = Skinelement.Left(5).Upper_Case();
        if(BufString == "COMBO")
        {
            ElementName = IniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = IniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementLabel = CreateLabel(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, ctId, 0, 0, 0);
                StringReleaseSplit(ElementArray);
                PrgSkinLabels.Add(ElementLabel);
                ctId++;
                ElementCoords = IniReadKey(Skinelement, "ComboDat", PrfCurrentSkin);
                if(ElementCoords.Len() != 0)
                {
                    ElementArray = StringSplit(ElementCoords, "|");
                    ElementhWnd = CreateComboBox(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, "", ctId, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
                    StringReleaseSplit(ElementArray);
                    ctId++;
                    PrgSkinControlsDest.Add(Skinelement.Get_String());
                    PrgSkinControls.Add(ElementhWnd);
                    for(j = 0; j <= 999; j++)
                    {
                        ElementName = IniReadKey(Skinelement, "Resp" + (CStr) StringNumberComplement(j, 3).Get_String(), PrfCurrentSkin);
                        if(ElementName.Len() == 0) break;
                        ComboBoxAddItem(ElementhWnd, ElementName, -1);
                    }
                    ElementSelected = IniReadKey("CURRENT", Skinelement, PrfCurrentSkin);
                    if(ElementSelected.Len() == 0) ElementSelected = "0";
                    ComboBoxSetIndex(ElementhWnd, ElementSelected.Get_Long());
                }
            }
        }
        else if(BufString == "RADIO")
        {
            ElementName = IniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = IniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementFrame = CreateFrame(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, 0, 0, 0);
                StringReleaseSplit(ElementArray);
                PrgSkinLabels.Add(ElementFrame);
                ctId++;
                FirstRadio = WS_GROUP;
                for(j = 0; j <= 999; j++)
                {
                    ElementName = IniReadKey(Skinelement, "Resp" + (CStr) StringNumberComplement(j, 3).Get_String(), PrfCurrentSkin);
                    if(ElementName.Len() == 0) break;
                    ElementCoords = IniReadKey(Skinelement, "Resp" + (CStr) StringNumberComplement(j, 3).Get_String() + (CStr) "Dat", PrfCurrentSkin);
                    if(ElementCoords.Len() != 0)
                    {
                        ElementArray = StringSplit(ElementCoords, "|");
                        ElementhWnd = CreateRadioButton(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), ElementFrame, ElementName, ctId, 0, WS_TABSTOP | FirstRadio);
                        StringReleaseSplit(ElementArray);
                        BufString = Skinelement + (CStr) j;
                        PrgSkinControlsDest.Add(BufString.Get_String());
                        PrgSkinControls.Add(ElementhWnd);
                        ctId++;
                        ElementSelected = IniReadKey("CURRENT", Skinelement, PrfCurrentSkin);
                        if(ElementSelected.Len() == 0) ElementSelected = "0";
                        CurrentRadio = ElementSelected.Get_Long();
                        if((long) CurrentRadio == j) RadioButtonSetState(ElementhWnd, 1);
                        FirstRadio = 0;
                    }
                }
            }
        }
        else if(BufString == "CHECK")
        {
            ElementName = IniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = IniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementhWnd = CreateCheckBox(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, ctId, 0, WS_TABSTOP, 0);
                StringReleaseSplit(ElementArray);
                ctId++;
                PrgSkinControlsDest.Add(Skinelement.Get_String());
                PrgSkinControls.Add(ElementhWnd);
                ElementSelected = IniReadKey("CURRENT", Skinelement, PrfCurrentSkin);
                if(ElementSelected.Len() == 0) ElementSelected = "0";
                CheckBoxSetState(ElementhWnd, ElementSelected.Get_Long());
            }
        }
        else if(BufString == "TEXTB")
        {
            ElementName = IniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = IniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementLabel = CreateLabel(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, ctId, 0, 0, 0);
                StringReleaseSplit(ElementArray);
                PrgSkinLabels.Add(ElementLabel);
                ctId++;
                ElementCoords = IniReadKey(Skinelement, "TextDat", PrfCurrentSkin);
                if(ElementCoords.Len() != 0)
                {
                    ElementArray = StringSplit(ElementCoords, "|");
                    ElementhWnd = CreateTextBox(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, "", ctId, 0, WS_TABSTOP | ES_AUTOHSCROLL, WS_EX_STATICEDGE);
                    StringReleaseSplit(ElementArray);
                    ctId++;
                    PrgSkinControlsDest.Add(Skinelement.Get_String());
                    PrgSkinControls.Add(ElementhWnd);
                    ControlSetText(ElementhWnd, IniReadKey("CURRENT", Skinelement, PrfCurrentSkin));
                }
            }
        }
        else if(BufString == "LISTC")
        {
            ElementName = IniReadKey(Skinelement, "Name", PrfCurrentSkin);
            ElementCoords = IniReadKey(Skinelement, "NameDat", PrfCurrentSkin);
            if(ElementCoords.Len() != 0)
            {
                ElementArray = StringSplit(ElementCoords, "|");
                ElementLabel = CreateLabel(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ElementName, ctId, 0, 0, 0);
                StringReleaseSplit(ElementArray);
                PrgSkinLabels.Add(ElementLabel);
                ctId++;
                ElementCoords = IniReadKey(Skinelement, "ListDat", PrfCurrentSkin);
                if(ElementCoords.Len() != 0)
                {
                    ElementArray = StringSplit(ElementCoords, "|");
                    ElementhWnd = CreateListView(StringGetSplitElement(ElementCoords, ElementArray, 0).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 1).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long(), StringGetSplitElement(ElementCoords, ElementArray, 3).Get_Long(), FRMPropertiesPageProjects, ctId, 0, PropStepListViewProc, LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES | LVS_EX_LABELTIP, LVS_REPORT | LVS_SINGLESEL | LVS_NOCOLUMNHEADER | WS_TABSTOP, WS_EX_STATICEDGE);
                    ListViewAddCol(ElementhWnd, "", StringGetSplitElement(ElementCoords, ElementArray, 2).Get_Long() - 23, 0);
                    StringReleaseSplit(ElementArray);
                    ctId++;
                    SkinListViews.Add(ElementhWnd);
                    PrgSkinControlsDest.Add(Skinelement.Get_String());
                    PrgSkinControls.Add(ElementhWnd);
                    for(k = 0; k <= 999; k++)
                    {
                        ElementName = IniReadKey(Skinelement, "Resp" + (CStr) StringNumberComplement(k, 3).Get_String(), PrfCurrentSkin);
                        if(ElementName.Len() == 0) break;
                        ListViewAddItem(ElementhWnd, ElementName, k, -1);
                        ElementSelected = IniReadKey("CURRENT", Skinelement + (CStr) StringNumberComplement(k, 3).Get_String(), PrfCurrentSkin);
                        if(ElementSelected.Len() == 0) ElementSelected = "0";
                        ListViewSetItemCheckbox(ElementhWnd, k, ElementSelected.Get_Long());
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Project building Frame hook proc
LRESULT CALLBACK FRMPropertiesFrameBuilding(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CStr TempCurrentSkin;
    int i = 0;

    switch(uMsg)
    {
        case WM_COMMAND:
            if((HWND) lParam == FRMPropertiesCbSkin)
            {
                if(ControlGetNotifiedCommand(wParam) == CBN_SELCHANGE)
                {
                    TempCurrentSkin = PrgSkinArray.Get(ComboBoxGetIndex(FRMPropertiesCbSkin))->Content;
                    if(_strcmpi(TempCurrentSkin.Get_String(), PrfCurrentSkin.Get_String()) != 0)
                    {
                        ClearSkincontrols();
                        ControlRefresh(FRMPropertiesPageProjects);
                        PrfCurrentSkin = TempCurrentSkin;
                        FillCurrentSkin();
                    }
                    return(0);
                }
            }
            break;
        case WM_NOTIFY:
            if(ControlGetNotifiedhWnd(lParam) == FRMStepListView)
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        ListViewCheckBoxItemDoubleClick(FRMStepListView);
                        return(0);
                }
            }
            else
            {
                switch(ControlGetNotifiedMsg(lParam))
                {
                    case NM_DBLCLK:
                        for(i = 0; i < SkinListViews.Amount(); i++)
                        {
                            if(SkinListViews.Get(i)->Content != 0)
                            {
                                if(SkinListViews.Get(i)->Content == ControlGetNotifiedhWnd(lParam))
                                {
                                    ListViewCheckBoxItemDoubleClick(SkinListViews.Get(i)->Content);
                                    return(0);
                                }
                            }
                        }
                }
            }
            break;
        case WM_ERASEBKGND:
            GDIDrawHorzSep(hWnd, 2, 129, 238);
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Clear skin screen
void ClearSkincontrols(void)
{
    int i = 0;

    for(i = 0; i < PrgSkinLabels.Amount(); i++)
    {
        DestroyWindow(PrgSkinLabels.Get(i)->Content);
    }
    for(i = 0; i < PrgSkinControls.Amount(); i++)
    {
        DestroyWindow(PrgSkinControls.Get(i)->Content);
    }
}

// -----------------------------------------------------------------------
// Fill the default language combo
void FillDefLanguage(void)
{
    long AsmPos = 0;
    CStr OldDefault;
    int i = 0;

    OldDefault = IniReadKey("RefLanguages", "DefLang", LanguagesIniFile);
    ComboBoxReset(FRMPropertiesCbDefLang);
    ComboBoxAddItem(FRMPropertiesCbDefLang, "(None)", -1);
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        FRMPropertiesRetVal = ChangeRelativePaths(FRMPropertiesRetVal);
        FRMPropertiesRetVal = IniReadKey("LangSpec", "LangName", FRMPropertiesRetVal);
        if(FRMPropertiesRetVal.Len() != 0)
        {
            if(OldDefault.Len() != 0)
            {
                // Retrieve index of selected language
                if(_strcmpi(FRMPropertiesRetVal.Get_String(), OldDefault.Get_String()) == 0) AsmPos = i + 1;
            }
            ComboBoxAddItem(FRMPropertiesCbDefLang, FRMPropertiesRetVal, -1);
        }
    }
    ComboBoxSetIndex(FRMPropertiesCbDefLang, AsmPos);
}

// -----------------------------------------------------------------------
// Fill the languages list
void FillLanguagesList(void)
{
    long AsmPos = 0;
    CStr OldDefault;
    int i = 0;

    ListBoxReset(FRMPropertiesLbLanguages);
    for(i = 0; i <= 999; i++)
    {
        FRMPropertiesRetVal = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(FRMPropertiesRetVal.Len() == 0) break;
        /* Just retrieve the name */
        FRMPropertiesRetVal = ChangeRelativePaths(FRMPropertiesRetVal);
        FRMPropertiesRetVal = RemoveRelativePaths(FRMPropertiesRetVal);
        FRMPropertiesRetVal = FileRemoveExtension(FRMPropertiesRetVal);
        FRMPropertiesRetVal = FRMPropertiesRetVal + " (" + IniReadKey("RefLanguages", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile) + ")";
        ListBoxAddItem(FRMPropertiesLbLanguages, FRMPropertiesRetVal, -1);
    }
    ListBoxSetIndex(FRMPropertiesLbLanguages, 0);
    Dispatch_LanguageName(0);
}

// -----------------------------------------------------------------------
// Select debugger
void GetDebugger(HWND hWnd)
{
    CStr LdFile;
    CStr OpFilters;

    OpFilters = "Exe files (*.exe;*.com)|*.exe;*.com";
    LdFile = ComDlgGetOpenFileName(FRMPropertieshwnd, OpFilters, "", 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    LdFile = ChangeAbsolutePaths(LdFile);
    // Save the old entry into the combo
    //ComboBoxSaveInIniFile(hWnd, LdFile, "ExtDebuggers", MainIniFile);
    //AddNewDebugger(hWnd, LdFile);
    ControlSetText(hWnd, LdFile);
}

// -----------------------------------------------------------------------
// Hack here coz listview control sucks badly
LRESULT CALLBACK PropStepListViewProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETFOCUS:
            ControlBringToTop(hWnd);
            ListViewSetItemFocus(hWnd, ListViewGetFirstFocusItem(hWnd));
            return(1);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Add the new external debugger entry
void AddNewDebugger(HWND hCombo, CStr Debugger)
{
    int i;
    int Found = FALSE;

    for(i = 0; i < ComboBoxCount(hCombo); i++)
    {
        if(_strcmpi(ComboBoxGetItem(hCombo, i).Get_String(), Debugger.Get_String()) == 0)
        {
            Found = TRUE;
            break;
        }
    }
    if(!Found) ComboBoxAddItem(hCombo, Debugger, -1);
}

// -----------------------------------------------------------------------
// Construct a languages's filename from the textbox
CStr Get_Store_LanguageName(void)
{
    CStr Name;

    if(ControlGetText(FRMPropertiesTxtLanguageFile).Len() != 0)
    {
        Name = ControlGetText(FRMPropertiesTxtLanguageFile);
        if(StringIsLabel(Name, TRUE))
        {
            return(Name);
        }
    }
    return "";
}

// -----------------------------------------------------------------------
// Construct a languages's extensions from the textbox
CStr Get_Store_LanguageExtensions(void)
{
    CStr Extensions;

    if(ControlGetText(FRMPropertiesTxtLanguageExtensions).Len() != 0)
    {
        Extensions = ControlGetText(FRMPropertiesTxtLanguageExtensions);
        if(StringIsLabel(Extensions, TRUE))
        {
            return(Extensions);
        }
    }
    return "";
}

// -----------------------------------------------------------------------
// Search a language name inside the listbox
int Is_LanguagePresent(CStr Name)
{
    CStr Entry;
    CStr Lang_Name;
    int Start_Name;
    int i;

    for(i = 0; i < ListBoxCount(FRMPropertiesLbLanguages); i++)
    {
        Entry = ListBoxGetItem(FRMPropertiesLbLanguages, i);
        Start_Name = Entry.In_Str(1, "(");
        Lang_Name = Entry.Left(Start_Name - 2);
        if(Name == Lang_Name)
        {
            return 1;
        }
    }
    return 0;
}

// -----------------------------------------------------------------------
// Construct a language's filename from the list
CStr Get_LanguageFileName(int Idx)
{
    CStr Entry;
    CStr Lang_Name;
    int Start_Name;

    Entry = ListBoxGetItem(FRMPropertiesLbLanguages, Idx);
    Start_Name = Entry.In_Str(1, "(");
    if(Start_Name > 2)
    {
        Lang_Name = Entry.Left(Start_Name - 2);
        if(StringIsLabel(Lang_Name, TRUE))
        {
            Lang_Name = "[LANGUAGESDIR]\\" + Lang_Name + ".mln";
        }
        else
        {
            Lang_Name = "";
        }
    }
    return Lang_Name;
}

// -----------------------------------------------------------------------
// Construct a language's extensions from the list
CStr Get_LanguageExtensions(int Idx)
{
    CStr Entry;
    CStr Lang_Name;
    int Start_Name;

    Entry = ListBoxGetItem(FRMPropertiesLbLanguages, Idx);
    Start_Name = Entry.In_Str(1, "(");
    if(Start_Name > 2)
    {
        Lang_Name = Entry.Left(Start_Name - 2);
        Lang_Name = Entry.Mid(Start_Name + 1, Entry.Len() - 1 - Start_Name);
        if(!StringIsLabel(Lang_Name, TRUE))
        {
            Lang_Name = "";
        }
    }
    return Lang_Name;
}

// -----------------------------------------------------------------------
// Display the selected language in the 2 text boxes
void Dispatch_LanguageName(int Idx)
{
    CStr Entry;
    CStr Lang_Name;
    int Start_Name;

    Entry = ListBoxGetItem(FRMPropertiesLbLanguages, Idx);
    Start_Name = Entry.In_Str(1, "(");
    if(Start_Name > 2)
    {
        // Display the content in the textboxes
        Lang_Name = Entry.Left(Start_Name - 2);
        ControlSetText(FRMPropertiesTxtLanguageFile, Lang_Name);
        ControlSetText(FRMPropertiesTxtLanguageExtensions, Entry.Mid(Start_Name + 1, Entry.Len() - 1 - Start_Name));

        Lang_Name = "[LANGUAGESDIR]\\" + Lang_Name + ".mln";
        Lang_Name = ChangeRelativePaths(Lang_Name);
        if(FileExist(Lang_Name))
        {
            ControlSetText(FRMPropertiesLbLanguageDescName, "Name: " + IniReadKey("LangSpec", "LangName", Lang_Name));
            ControlSetText(FRMPropertiesLbLanguageDescCopyright, "Author: " + IniReadKey("LangSpec", "LangCopyright", Lang_Name));
            ControlSetText(FRMPropertiesLbLanguageDescComment, "Comment:\r\n" + IniReadKey("LangSpec", "LangComment", Lang_Name));
        }
        else
        {
            ControlSetText(FRMPropertiesLbLanguageDescName, "Name: <unknown>");
            ControlSetText(FRMPropertiesLbLanguageDescCopyright, "Author: <unknown>");
            ControlSetText(FRMPropertiesLbLanguageDescComment, "Comment:\r\n<unknown>");
        }
    }
    else
    {
        ControlSetText(FRMPropertiesTxtLanguageFile, "");
        ControlSetText(FRMPropertiesTxtLanguageExtensions, "");
        ControlSetText(FRMPropertiesLbLanguageDescName, "Name: <unknown>");
        ControlSetText(FRMPropertiesLbLanguageDescCopyright, "Author: <unknown>");
        ControlSetText(FRMPropertiesLbLanguageDescComment, "Comment:\r\n<unknown>");
    }
}

// -----------------------------------------------------------------------
// Save the language index file
int Save_Language_Idx(HWND hwnd)
{
    int i;
    CStr Name;
    CStr Extensions;

    /* Check the validity of the entries first */
    for(i = 0; i < ListBoxCount(FRMPropertiesLbLanguages); i++)
    {
        Name = Get_LanguageFileName(i);
        if(!Name.Len())
        {
            MiscMsgBox(hwnd, "Invalid language name.", MB_ERROR, Requesters);
            SysTabSetCurrentItem(FRMPropertiesSysTab, MAX_TABS);
            return 0;
        }
        Extensions = Get_LanguageExtensions(i);
        if(!Extensions.Len())
        {
            MiscMsgBox(hwnd, "Invalid language extensions.", MB_ERROR, Requesters);
            SysTabSetCurrentItem(FRMPropertiesSysTab, MAX_TABS);
            return 0;
        }
    }

    IniWriteSection("RefLanguages", "", LanguagesIniFile);
    for(i = 0; i < ListBoxCount(FRMPropertiesLbLanguages); i++)
    {
        Name = Get_LanguageFileName(i);
        Extensions = Get_LanguageExtensions(i);
        IniWriteKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), Name, LanguagesIniFile);
        IniWriteKey("RefLanguages", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), Extensions, LanguagesIniFile);
    }
    Update_All_Languages();
    RefreshNewMenu();
    return 1;
}
