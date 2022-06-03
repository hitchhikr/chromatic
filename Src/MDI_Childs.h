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
// MDI_Childs.h: declaration of used variables / structures for MDI_Childs.cpp
// -----------------------------------------------------------------------

#ifndef _MDI_CHILDS_H_
#define _MDI_CHILDS_H_

// -----------------------------------------------------------------------
// Includes
#include "MsgCodeMax.h"

// -----------------------------------------------------------------------
// Structures
typedef struct _CHILDDAT
{
    // CodeMax handle
    HWND hChildCodeMax;
    // Opened file
    CStr *RFile;
    // Opened language type
    CStr *Language;
    // Read only flag
    long ReadOnly;
    // File loaded on/off
    long FileLoaded;
    long ModifFlag;
    long OldCodeMaxHook;
    long oldAPILine;
    long oldAPICol;
    long oldCMLine;
    long oldAPIhighLight;
    CStr *oldAPIFnc;
    CStr *oldAPILineFnc;
    LPFILETIME FileDateOpen;
    long oldAPILineLine;
    long oldAPILineCol;
    WNDPROC CurCallBackProc;
    void (CALLBACK *CodeMaxPropertiesHook)(HWND);
} CHILDDAT, *LPCHILDDAT;

// -----------------------------------------------------------------------
// Variables
extern LPCHILDDAT ChildStruct;
extern CHILDDAT ChildStructBack;
extern CStr LanguageToReg;
extern CStr TempRfile;
extern CM_RANGE CodeMaxCurRange;
extern CStr ChildRetVal;
extern CM_MOUSEDATA MyMouseData;
extern CM_POSITION MyCRect;
extern POINT Mouse_Coords;
extern long APIMode;
extern long APIListMode;
extern long InvalidAPI;
extern CStr LineToDisplay;
extern long LineNumberToDisplay;

// -----------------------------------------------------------------------
// Functions declarations
void CALLBACK MDIChildInitProc(HWND hWnd);
LRESULT CALLBACK MDIChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
LPCHILDDAT LoadStructure(HWND hWnd);
void ResizeChildCodeMax(HWND hWnd);
void InitMinimumCodeMax(HWND hWnd);
void CALLBACK SetCodeMaxProperties(HWND hWnd);
void RefreshIdentScope(HWND hWnd);
void StoreLanguage(CStr LngToStore);
void SetCodeMaxFont(HWND hWnd);
void SetCodeMaxColors(HWND hWnd);
void SetCodeMaxLineStyle(HWND hWnd, long LnStyle);
void WritePositionInStatus(HWND hWnd);
void SetStatusInsert(HWND hWnd);
void ClearStatusBarParts(void);
void LoadCurrentSel(HWND hWnd);
void RemCurrentSel(HWND hWnd);
long GetCurrentTopIndex(HWND hWnd);
void SetCurrentTopIndex(HWND hWnd, long NewTopIndex);
long GetCurrentLineNumber(HWND hWnd);
long GetCurrentColNumber(HWND hWnd);
CStr CMGetCurrentLine(HWND hWnd, long LineNumber);
CStr CMGetRealFile(CStr CMFile);
CStr CMGetRealLanguage(CStr CMFile);
CStr RetrieveCurrentLine(HWND hWnd);
long GetCurrentLogicalSelLen(HWND hWnd);
CStr GetCurrentLogicalSelText(HWND hWnd);
void CMSetCaretPos(HWND hWnd, long nLine, long nCol);
void CMSelectWordInLine(HWND hWnd, long nLine, CStr KeyWord);
void ClearFile(HWND hWnd);
long LoadFile(HWND hWnd);
long IncludeFile(HWND hWnd, CStr FName);
long SaveCodeMax(HWND hWnd, CStr FName);
void SaveBookmarks(HWND hWnd, CStr FileName, long BaseLine, long Lines);
void LoadBookmarks(HWND hWnd, CStr BkFName, long BaseLine);
void UpdateLanguage(HWND hWnd, CStr FileName);
void PrintFile(HWND hWnd, long Selection);
CStr GetCurrentCharWord(HWND hWnd);
CStr SelectCurrentWord(HWND hWnd);
CStr GetCurrentWord(HWND hWnd);
LRESULT CALLBACK CodeMaxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void InsertSnippetCode(HWND hWnd, CStr FileName);
void SmartInsert(HWND hWnd, CStr Txt);
void SmartInsertLine(HWND hWnd, CStr Txt);
void SmartInsertFile(HWND hWnd, long File);
void SendTextToChild(HWND hWnd, CStr TxtToSet);
void RefreshStatusBar(HWND hWnd);
void ForceChildFile(HWND hWnd);
void ForceChildModified(HWND hWnd);
void SetHighLine(HWND hWnd);
void ConfineCaret(HWND hWnd);
void SetCodeMaxLanguage(HWND hWnd);
void StoreLastPosInChild(HWND hWnd);
void CreateSnippetMenu(HMENU hsMenu, HWND hWnd);
void CreateTemplateMenu(HMENU hsMenu);
void DisplaySubContextMenu(HWND hWnd, HMENU hsMenu);
long CreateLangScripts(HMENU hsMenu);
void CMDecToHex(HWND hWnd);
void CMDecToBin(HWND hWnd);
void CMHexToDec(HWND hWnd);
void CMHexToBin(HWND hWnd);
void CMBinToDec(HWND hWnd);
void CMBinToHex(HWND hWnd);
void CMHexToFlt(HWND hWnd);
void CMFltToHex(HWND hWnd);
void DispFnc(HWND hWnd, CStr FnctoShow, CStr CompleteLine, long SetHighLight, long CStyle);
void SaveAPIName(HWND hwndCM, CStr FncToSave);
long GetRealYposition(HWND hWnd);
long CountComma(CStr Line, long XPos);
void MoveFncX(HWND hWnd, HWND DesthWnd);
void MoveFncY(HWND hWnd, HWND DesthWnd);
void MoveFncXY(HWND hWnd, HWND DesthWnd);
long WAMMGetCaretRealXCoordinate(HWND hWnd);
long WAMMGetCaretRealYCoordinate(HWND hWnd);
void CorrectLine(HWND hWnd, CStr LineToCheck);
void SkipCorrectBlank(void);
void ReadCorrectEntity(void);
void ReadCorrectStrings(void);
void ReadCorrectChar(void);
CStr GetLeftWord(HWND hWnd, CStr LineToCheck);
CStr SelectLeftWord(HWND hWnd, CStr LineToCheck);
void DisplayAPIList(HWND hWnd);
void RefreshChildTitle(HWND hWnd);
void SetCorrectionHook(HWND hWnd);
int CALLBACK CorrectKeyWordsHook(LPTSTR Token, long Pos, long PosBefore);
long IsChildReadOnly(HWND hWnd);
void SetChildReadOnly(HWND hWnd, int State);

#endif
