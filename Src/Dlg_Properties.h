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
// Dlg_Properties.h: declaration of used variables / structures for Dlg_Properties.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_PROPERTIES_H_
#define _DLG_PROPERTIES_H_

// -----------------------------------------------------------------------
// Variables
extern HWND FRMPropertiesSysTab;
extern HWND FRMPropertiesCmdOk;
extern HWND FRMPropertiesCmdCancel;
extern HWND FRMPropertieshwnd;

// Other variables
extern CStr FRMPropertiesRetVal;
extern HFONT LblFont;

// Profiles variables
extern COLORREF Prof_BackColor;
extern COLORREF Prof_NormText;
extern COLORREF Prof_HighText;
extern COLORREF Prof_MarginColor;
extern COLORREF Prof_LinesColor;
extern COLORREF Prof_NumbersColorValue;
extern COLORREF Prof_StringsColorValue;
extern COLORREF Prof_CommentsColorValue;
// Scope keywords
extern COLORREF Prof_KeyWordsColorValue;
extern COLORREF Prof_APIHighColor;
// FRMPropertiesColKeywords
extern COLORREF Prof_ColorAValue;
// FRMPropertiesColSeparators
extern COLORREF Prof_ColorBValue;
// FRMPropertiesColDirectives
extern COLORREF Prof_ColorCValue;
// FRMPropertiesColRegisters
extern COLORREF Prof_ColorDValue;
// FRMPropertiesColExtraFunctions
extern COLORREF Prof_ColorEValue;
// FRMPropertiesColExtraHighLight1
extern COLORREF Prof_ColorFValue;
// FRMPropertiesColExtraHighLight2
extern COLORREF Prof_ColorGValue;
// FRMPropertiesColExtraHighLight3
extern COLORREF Prof_ColorHValue;

extern long Prof_ItalicComment;

extern long FRMPropertiesFirstTab;

extern CM_RANGE LineRange;
extern CStr NewAccountName;
extern long FRMPropertiesInFtpCreation;

extern CStr PrefsGenProg;
extern CStr PrefsGenCmd;
extern CStr PrefsGenDest;

extern CStr PrfCurrentSkin;
extern CList <char *> PrgSkinArray;
extern CList <HWND> PrgSkinLabels;
extern CList <HWND> PrgSkinControls;
extern CList <char *> PrgSkinControlsDest;
extern CList <HWND> SkinListViews;

// -----------------------------------------------------------------------
// Functions declarations
void DisplayProperties(void);
int CALLBACK FRMPropertiesProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void CreatePropPage1(HWND hParent);
void CreatePropPage2(HWND hParent);
void CreatePropPage3(HWND hParent);
void CreatePropPage4(HWND hParent);
void CreatePropPage5(HWND hParent);
void FillProfil(void);
void FillProperties(void);
void SavePrefs(void);
void SetBackupEn(void);
void SetFontView(void);
void FRMPropertiesSetColor(HWND hwnd);
void FRMPropertiesInitCodeMax(HWND hwnd);
void BuildPreview(HWND hwnd);
void GetHelpFile(HWND hwnd);
void GetGenProtoProg(void);
void FRMPropertiesSelectFont(void);
void FRMPropertiesChooseROOTDIR(HWND hwnd);
void LoadColorsProfile(CStr ProfileName);
long SaveColorsProfile(CStr ProfileName);
void SetLines(HWND hWnd);
long FRMPropertiesGetCurrentLineNumber(HWND hwnd);
void FRMPropertiesSetItalicComments(HWND hwnd);
void FRMPropertiesSetCodeMaxColors(HWND hwnd);
void FRMPropertiesSelectBakDir(void);
void FRMProPertiesConfineCaret(void);
void RefreshChilds(void);
int CALLBACK EnumRefreshChilds(HWND hWnd, long lParam);
void BuildFTP(HWND hComboBox);
void FillFTPProps(void);
void ResetFTPProps(void);
void SetAnonymousState(long DontFocus);
void SaveFTPProps(void);
void FRMPropertiesChangeCurrentTab(void);
void DeleteAccountFromIni(CStr AccountToDelete);
void Fillskins(void);
void FillCurrentSkin(void);
void ClearSkincontrols(void);
void FillDefLanguage(void);
void FillLanguagesList(void);
void GetDebugger(HWND hwnd);

#endif
