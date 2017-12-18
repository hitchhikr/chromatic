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
// MDI_Childs.cpp: MDI childs handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MDI_Childs.h"
#include "MDI_Form.h"
#include "MiscFunctions.h"
#include "Globals.h"
#include "Statusbar.h"
#include "AddIns.h"
#include "ConstRes.h"
#include "ConstMisc.h"
#include "ConstCodeMax.h"
#include "ConstMenus.h"
#include "APIDb.h"
#include "CmdMenus.h"
#include "Splitters.h"
#include "Menus.h"
#include "Languages.h"
#include "Script_Engine/Scripts.h"
#include "Languages.h"
#include "Dlg_API.h"
#include "Dlg_APIList.h"
#include "Dlg_Filters.h"

// -----------------------------------------------------------------------
// Variables
LPCHILDDAT ChildStruct;
CHILDDAT ChildStructBack;
CStr LanguageToReg;
CStr TempRfile;
CM_RANGE CodeMaxCurRange;
CStr ChildRetVal;
CM_MOUSEDATA MyMouseData;
CM_POSITION MyCRect;
long APIMode;
long APIListMode;
long InvalidAPI;
CStr LineToDisplay;
long LineNumberToDisplay;
POINT Mouse_Coords;

// -----------------------------------------------------------------------
// Initialize MDIChild Window
void CALLBACK MDIChildInitProc(HWND hWnd)
{
    int i = 0;
    LPCHILDDAT NewChildMem;
    CStr NewChildFile;
    void (CALLBACK * LocInitChildHook)(HWND hWnd);

    NewChildMem = (LPCHILDDAT) AllocMem(sizeof(CHILDDAT));
    NewChildMem->RFile = new CStr;
    NewChildMem->Language = new CStr;
    NewChildMem->oldAPIFnc = new CStr;
    NewChildMem->oldAPILineFnc = new CStr;
    NewChildMem->FileDateOpen = (LPFILETIME) AllocMem(sizeof(FILETIME));
    // Create a new instance of the codemax control
    NewChildMem->hChildCodeMax = CreateCodeMax(-1, -1, -1, -1, hWnd, ApphInstance, 1);
    InitMinimumCodeMax(NewChildMem->hChildCodeMax);
    NewChildMem->RFile->Set_String(FileToOpen.Get_String());
    ControlSetText(hWnd, NewChildMem->RFile);
    SetCodeMaxFont(NewChildMem->hChildCodeMax);
    // (StoreLanguage() uses ChildStruct)
    ChildStruct = NewChildMem;
    StoreLanguage(LanguageToOpen);
    CurrentForm = hWnd;
    SetWindowLong(hWnd, GWL_USERDATA, (long) NewChildMem);
    NewChildMem->CodeMaxPropertiesHook = &SetCodeMaxProperties;
    SetCodeMaxColors(NewChildMem->hChildCodeMax);
    SetCodeMaxProperties(NewChildMem->hChildCodeMax);
    SetCodeMaxLanguage(NewChildMem->hChildCodeMax);
    NbForms++;
    LoadCurrentSel(NewChildMem->hChildCodeMax);
    if(ControlIsVisible(hStatusBar) != 0) SetStatusBarParts();
    RefreshSBStat = 1;
    WritePositionInStatus(NewChildMem->hChildCodeMax);
    DragAcceptFiles(NewChildMem->hChildCodeMax, AcceptFiles);
    NewChildMem->OldCodeMaxHook = SetWindowLong(NewChildMem->hChildCodeMax, GWL_WNDPROC, (long) CurrentCodeMaxProc);
    // Execute possible hooks
    for(i = 0; i < ChildCreateHooks.Amount(); i++)
    {
        if(ChildCreateHooks.Get(i)->Content != 0)
        {
            LocInitChildHook = (void (CALLBACK *)(HWND)) ChildCreateHooks.Get(i)->Content;
            LocInitChildHook(hWnd);
        }
    }
    SendMessage(hMDIform.hClient, CHILD_MSG_CREATED, ICON_WINDOWNORM, (long) hWnd);
}

// -----------------------------------------------------------------------
// MDIChild Winproc
LRESULT CALLBACK MDIChildProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int i = 0;
    HMENU hSnippetMenu;
    CM_REGISTEREDCMDDATA RegCom;
    CStr LineToCorrect;
    PAINTSTRUCT ChildPs;
    long HaveToDie = 0;
    CStr BufString;
    DLGPROC LocChildHook;
    CM_CMDFAILUREDATA *Err_datas;

    // Execute possible hooks
    for(i = 0; i < ChildHooks.Amount(); i++)
    {
        if(ChildHooks.Get(i)->Content != 0)
        {
            LocChildHook = ChildHooks.Get(i)->Content;
            if(LocChildHook(hWnd, uMsg, wParam, lParam) == HOOK_DIE)
            {
                HaveToDie = HaveToDie | HOOK_DIE;
            }
        }
    }
    if((long) HaveToDie == HOOK_DIE) return(0);
    
    switch(uMsg)
    {

//        ChildStruct = LoadStructure(CurrentForm);
//        CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDMARKALL, 0);

        case WM_URGENTKILLCLIST:
            ControlClose(FRMAPIListhWnd);
            break;

        case WM_NOTIFY:
            switch(ControlGetNotifiedMsg(lParam))
            {
                case CMN_MARKALL:
                    ChildStruct = LoadStructure(hWnd);
                    CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 1);
                    break;
                case CMN_FOUNDTEXT:
                    SendMessage(GetParent(GetParent(hWnd)), WM_FOUND_TEXT, 0, (LPARAM) hWnd);
                    break;
                case CMN_MODIFIEDCHANGE:
                    ChildStruct = LoadStructure(hWnd);
                    ChildStruct->ModifFlag = CM_IsModified(ChildStruct->hChildCodeMax);
                    if(ChildStruct->ModifFlag == 1)
                    {
                        TempRfile = ChildStruct->RFile + (CStr) " *";
                        ControlSetText(hWnd, TempRfile);
                    }
                    else
                    {
                        ControlSetText(hWnd, ChildStruct->RFile);
                    }
                    RefreshChildTitle(hWnd);
                    break;

                case CMN_CMDFAILURE:
                    Err_datas = (CM_CMDFAILUREDATA *) lParam;
                    switch(Err_datas->dwErr)
                    {
                        case CMDERR_NOTFOUND_FORWARD_MARKALL:
                            // Notify the MDI
                            SendMessage(GetParent(GetParent(hWnd)), WM_FINDMARKALL_WRAP, 0, (LPARAM) hWnd);
                            break;
                        case CMDERR_NOTFOUND_FORWARD_REPLACEALL:
                            // Notify the MDI
                            SendMessage(GetParent(GetParent(hWnd)), WM_FINDREPLACEALL_WRAP, 0, (LPARAM) hWnd);
                            break;
                        case CMDERR_NOTFOUND_FORWARD:
                            SendMessage(GetParent(GetParent(hWnd)), WM_FIND_FORWARD_WRAP, 0, (LPARAM) hWnd);
                            break;
                        case CMDERR_NOTFOUND_BACKWARD:
                            SendMessage(GetParent(GetParent(hWnd)), WM_FIND_BACKWARD_WRAP, 0, (LPARAM) hWnd);
                            break;
                    }
                    break;

                case CMN_SELCHANGE:
                    ChildStruct = LoadStructure(hWnd);
                    // Load the current selection
                    LoadCurrentSel(ChildStruct->hChildCodeMax);
                    WritePositionInStatus(ChildStruct->hChildCodeMax);
                    if(ShowH == 1) if(CodeMaxCurRange.posStart.nLine != (long) CM_GetHighlightedLine(ChildStruct->hChildCodeMax)) CM_SetHighlightedLine(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine);
                    // Functions/Constants list
                    if(APIListMode == 1)
                    {
                        // Change line
                        if(ChildStruct->oldAPILine != GetCurrentLineNumber(ChildStruct->hChildCodeMax))
                        {
                            ControlClose(FRMAPIListhWnd);
                        }
                        else
                        {
                            // Change column
                            if(ChildStruct->oldAPICol != GetCurrentColNumber(ChildStruct->hChildCodeMax))
                            {
                                ControlClose(FRMAPIListhWnd);
                            }
                        }
                    }
                    
                    if(APITTip == 1)
                    {
                        if(APIMode == 1)
                        {
                            // Change line
                            if(ChildStruct->oldAPILine != GetCurrentLineNumber(ChildStruct->hChildCodeMax))
                            {
                                ControlClose(FRMAPIhwnd);
                            }
                            else
                            {
                                // Change column
                                if(ChildStruct->oldAPICol != GetCurrentColNumber(ChildStruct->hChildCodeMax))
                                {
                                    if(HighOn == 1)
                                    {
                                        APICurHiglight = CountComma(CMGetCurrentLine(ChildStruct->hChildCodeMax,
                                                                    GetCurrentLineNumber(ChildStruct->hChildCodeMax)).Get_String(),
                                                                    GetCurrentColNumber(ChildStruct->hChildCodeMax));
                                        if(HighPar == 1) APICurHiglight++;
                                    }
                                    else
                                    {
                                        APICurHiglight = -1;
                                    }
                                    if((long) ChildStruct->oldAPIhighLight != (long) APICurHiglight)
                                    {
                                        ChildStruct->oldAPIhighLight = APICurHiglight;
                                        APINoRedraw = 1;
                                        RedrawWindow(APIhText, NULL, 0, RDW_INVALIDATE);
                                    }
                                }
                            }
                        }
                    }
                    
                    // Save current line number
                    ChildStruct->oldCMLine = GetCurrentLineNumber(ChildStruct->hChildCodeMax);
                    break;
                case CMN_VSCROLL:
                    if(CursorFollow == 1)
                    {
                        ChildStruct = LoadStructure(hWnd);
                        ConfineCaret(ChildStruct->hChildCodeMax);
                    }
                    if(APITTip == 1) if(APIMode == 1) MoveFncY(hWnd, FRMAPIhwnd);
                    if(APIListMode == 1) MoveFncY(hWnd, FRMAPIListhWnd);
                    break;
                case CMN_HSCROLL:
                    if(APITTip == 1) if(APIMode == 1) MoveFncX(hWnd, FRMAPIhwnd);
                    if(APIListMode == 1) MoveFncX(hWnd, FRMAPIListhWnd);
                    break;
                case CMN_VIEWCHANGE:
                    if(APITTip == 1) if(APIMode == 1) MoveFncXY(hWnd, FRMAPIhwnd);
                    if(APIListMode == 1) MoveFncXY(hWnd, FRMAPIListhWnd);
                    break;
                case CMN_SHOWPROPS:
                    return(1);
                case CMN_MOUSEUP:
                    // Trap middle mousebutton
                    RtlCopyMemory(&MyMouseData, (void *) lParam, sizeof(MyMouseData));
                    ChildStruct = LoadStructure(hWnd);
                    if((CM_HitTest(ChildStruct->hChildCodeMax, MyMouseData.pt.x, MyMouseData.pt.x) & 0x80000) != 0)
                    {
                        if(MyMouseData.nButton == CM_BTN_MIDDLE)
                        {
                            MCMD_SelectProc();
                            return(1);
                        }
                    }
                    break;
                case CMN_REGISTEREDCMD:
                    CopyMemory(&RegCom, (void *) lParam, sizeof(RegCom));
                    switch(RegCom.wCmd)
                    {
                        case 1000:
                            OpenFileUnderCursor(0, 0);
                            return(1);
                        case 1001:
                            OpenFileUnderCursor(0, 1);
                            return(1);
                        case 1002:
                            hSnippetMenu = CreatePopupMenu();
                            ChildStruct = LoadStructure(hWnd);
                            CreateSnippetMenu(hSnippetMenu, ChildStruct->hChildCodeMax);
                            if(SnipArray.Amount() != 0) DisplaySubContextMenu(hWnd, hSnippetMenu);
                            return(1);
                        case 1003:
                            ChildStruct = LoadStructure(hWnd);
                            CMDecToHex(ChildStruct->hChildCodeMax);
                            return(1);
                        case 1004:
                            ChildStruct = LoadStructure(hWnd);
                            CMDecToBin(ChildStruct->hChildCodeMax);
                            return(1);
                        case 1005:
                            ChildStruct = LoadStructure(hWnd);
                            CMHexToDec(ChildStruct->hChildCodeMax);
                            return(1);
                        case 1006:
                            ChildStruct = LoadStructure(hWnd);
                            CMHexToBin(ChildStruct->hChildCodeMax);
                            return(1);
                        case 1007:
                            ChildStruct = LoadStructure(hWnd);
                            CMBinToDec(ChildStruct->hChildCodeMax);
                            return(1);
                        case 1008:
                            ChildStruct = LoadStructure(hWnd);
                            CMBinToHex(ChildStruct->hChildCodeMax);
                            return(1);
                        case 1009:
                            ChildStruct = LoadStructure(hWnd);
                            CMHexToFlt(ChildStruct->hChildCodeMax);
                            return(1);
                        case 1010:
                            ChildStruct = LoadStructure(hWnd);
                            CMFltToHex(ChildStruct->hChildCodeMax);
                            return(1);
                        case 1011:
                            hSnippetMenu = CreatePopupMenu();
                            if(CreateLangScripts(hSnippetMenu)) DisplaySubContextMenu(hWnd, hSnippetMenu);
                            return(1);
                        case 1012:
                            DisplayAPIList(hWnd);
                            return(1);
                    }
            }
            break;
        case WM_CHILDACTIVATE:
            CurrentForm = hWnd;
            break;
        case WM_WINDOWPOSCHANGED:
            CurrentForm = ClientGetActiveChild(hMDIform.hClient);
            if(ControlIsVisible(CurrentForm) != 0)
            {
                ChildStruct = LoadStructure(CurrentForm);
                SetFocus(ChildStruct->hChildCodeMax);
            }
            break;
        case WM_ERASEBKGND:
            return(0);
        case WM_NCACTIVATE:
            if(APITTip == 1) if(APIMode == 1) ControlClose(FRMAPIhwnd);
            if(APIListMode == 1) ControlClose(FRMAPIListhWnd);
            break;
        case WM_MOVE:
            if(APITTip == 1) if(APIMode == 1) ControlClose(FRMAPIhwnd);
            if(APIListMode == 1) ControlClose(FRMAPIListhWnd);
            break;
        case WM_SIZE:
            if(APITTip == 1) if(APIMode == 1) ControlClose(FRMAPIhwnd);
            if(APIListMode == 1) ControlClose(FRMAPIListhWnd);
            CurrentForm = hWnd;
            break;
        case WM_MOUSEACTIVATE:
            if(ClientGetActiveChild(hMDIform.hClient) != hWnd)
            {
                CurrentForm = hWnd;
                SetFocus(CurrentForm);
                return(MA_ACTIVATE);
            }
            break;
        case WM_PAINT:
            BeginPaint(hWnd, &ChildPs);
            ResizeChildCodeMax(hWnd);
            EndPaint(hWnd, &ChildPs);
            CurrentForm = ClientGetActiveChild(hMDIform.hClient);
            ChildStruct = LoadStructure(CurrentForm);
            LoadCurrentSel(ChildStruct->hChildCodeMax);
            RefreshSBStat = 1;
            WritePositionInStatus(ChildStruct->hChildCodeMax);
            SetFocus(ChildStruct->hChildCodeMax);
            break;
        case WM_CLOSE:
//            if(ForcePrj == 1) {
//NotInProject:
            if(CursorDisabled) DisplayCursor();
            ChildStruct = LoadStructure(hWnd);
            if(ChildStruct->ModifFlag == 1)
            {
                BufString = "File '" + (CStr) CMGetRealFile(ChildStruct->RFile).Get_String() + (CStr) "' modified. Save it now ?";
                switch(MiscMsgBox(hMDIform.hWnd, BufString, MB_QUESTIONCANCEL, Requesters)) {
                    case IDYES:
                        SaveIt(hWnd);
                        break;
                    case IDNO:
                        break;
                    case IDCANCEL:
                        return(0);
                }
            }
            StoreLastPosInChild(hWnd);
            // Delete them manually
            delete ChildStruct->RFile;
            delete ChildStruct->Language;
            delete ChildStruct->oldAPIFnc;
            delete ChildStruct->oldAPILineFnc;
            FreeMem((long) ChildStruct->FileDateOpen);
            // Free the child structure
            FreeMem(GetWindowLong(hWnd, GWL_USERDATA));
            // Put a phony structure for following messages
            SetWindowLong(hWnd, GWL_USERDATA, (long) &ChildStructBack);
//            Else
                // Just hide the window if it is in project
                // And no global close
//                If ProjectOn = False Then GoTo NotInProject
//                LoadStructure hwnd
//                If IsInProject(CMGetRealFile(ChildStruct->RFile), True) = -1 Then GoTo NotInProject
//                SavedHwnd = hwnd
//                MCMD_PreviousWindow
//                ControlVisible SavedHwnd, False
//                SendMessage hMDIform.hClient, WM_MDIREFRESHMENU, 0, ByVal 0
//                DrawMenuBar hMDIform.hwnd
//                MDIChildForm1Proc = 0
//                Exit Function
//           End If
            break;
        case WM_DESTROY:
            NbForms--;
            if(NbForms == 0) ClearStatusBarParts();
            // Notify the parent
            SendMessage(hMDIform.hClient, CHILD_MSG_DESTROYED, 0, (long) hWnd);
            return(0);
    }
    return(DefMDIChildProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Retrieve the child structure
LPCHILDDAT LoadStructure(HWND hWnd)
{
    if(hWnd != NULL)
    {
        //RtlZeroMemory(&ChildStruct, sizeof(ChildStruct));
        return((LPCHILDDAT) GetWindowLong(hWnd, GWL_USERDATA));
        //RtlCopyMemory(&ChildStruct, (void *) GetWindowLong(hWnd, GWL_USERDATA), sizeof(ChildStruct));
    }
    return(NULL);
}
            
// -----------------------------------------------------------------------
// Resize codemax control
void ResizeChildCodeMax(HWND hWnd)
{
    RECT CodeMaxRect;

    GetClientRect(hWnd, &CodeMaxRect);
    ChildStruct = LoadStructure(hWnd);
    SetWindowPos(ChildStruct->hChildCodeMax, 0, CodeMaxRect.left, CodeMaxRect.top, CodeMaxRect.right - CodeMaxRect.left, CodeMaxRect.bottom - CodeMaxRect.top, SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_SHOWWINDOW);
}

// -----------------------------------------------------------------------
// Minimum inits for CodeMax control
void InitMinimumCodeMax(HWND hWnd)
{
    CM_ShowScrollBar(hWnd, 0, 1);
    CM_ShowScrollBar(hWnd, 1, 1);
    CM_EnableSplitter(hWnd, 0, 1);
    CM_EnableGlobalProps(hWnd, 1);
    CM_EnableColumnSel(hWnd, 0);
    CM_EnableHideSel(hWnd, 0);
}

// -----------------------------------------------------------------------
// Init CodeMax control according to properties
void CALLBACK SetCodeMaxProperties(HWND hWnd)
{
    CM_EnableColorSyntax(hWnd, SyntaxH);
    CM_EnableNormalizeCase(hWnd, AutoCorrectKey);
    CM_EnableTabExpand(hWnd, AutoExpand);
    CM_EnableDragDrop(hWnd, EnDragDrop);
    CM_SetTabSize(hWnd, Tabulations);
    CM_EnableSplitter(hWnd, 1, HorizSplit);
    CM_EnableLineToolTips(hWnd, LineTTip);
    if(CM_GetAllBookmarks(hWnd, 0) == 0 && CM_GetAllBreakpoints(hWnd, 0) == 0) CM_EnableLeftMargin(hWnd, LeftM);
    CM_EnableWhitespaceDisplay(hWnd, White);
    CM_EnableSmoothScrolling(hWnd, Smooth);
    SetCodeMaxLineStyle(hWnd, LnStyle);
    RefreshIdentScope(hWnd);
    CM_EnableSelBounds(hWnd, Wrap);
    SetHighLine(hWnd);
    SetCorrectionHook(hWnd);
}

// -----------------------------------------------------------------------
// Refresh auto indent mode according to child's language
void RefreshIdentScope(HWND hWnd)
{
    CStr BufString;
    
    switch(IndentScope)
    {
        case 0:
            CM_SetAutoIndentMode(hWnd, CM_INDENT_OFF);
            break;
        case 1:
            ChildStruct = LoadStructure(GetParent(hWnd));
            BufString = ChildStruct->Language->Left(ChildStruct->Language->Len());
            if(BufString.Len() == 0)
            {
                CM_SetAutoIndentMode(hWnd, CM_INDENT_OFF);
            }
            else
            {
                if(_strcmpi(BufString.Get_String(), "(none)") == 0) CM_SetAutoIndentMode(hWnd, CM_INDENT_OFF);
                else CM_SetAutoIndentMode(hWnd, CM_INDENT_SCOPE);
            }
    }
}

// -----------------------------------------------------------------------
// Store language name to use in this form
void StoreLanguage(CStr LngToStore)
{
    ChildStruct->Language->Set_String(LngToStore.Get_String());
}

// -----------------------------------------------------------------------
// Load a font into codemax
void SetCodeMaxFont(HWND hWnd)
{
    ControlSetFontNoRedraw(hWnd, CurFontHandle);
}

// -----------------------------------------------------------------------
// Set the colors of the codemax control
void SetCodeMaxColors(HWND hWnd)
{
    CM_COLORS ColsToSet;
    CM_FONTSTYLES StyleToSet;

    memset(&ColsToSet, 0, sizeof(ColsToSet));
    memset(&StyleToSet, 0, sizeof(StyleToSet));
    ColsToSet.crTextBk = CurBackColor;
    ColsToSet.crWindow = CurBackColor;
    ColsToSet.crVDividerLines = CurMarginColor;
    ColsToSet.crHDividerLines = CurMarginColor;
    ColsToSet.crBookmark = 0xDD7766;
    ColsToSet.crBookmarkBk = CurBackColor;
    ColsToSet.crBreakpoint = 0x0011DD;
    ColsToSet.crBreakpointBk = CurBackColor;
    ColsToSet.crCommentBk = CurBackColor;
    ColsToSet.crKeywordBk = CurBackColor;
    ColsToSet.crNumberBk = CurBackColor;
    ColsToSet.crOperatorBk = CurBackColor;
    ColsToSet.crScopeKeywordBk = CurBackColor;
    ColsToSet.crStringBk = CurBackColor;
    ColsToSet.crTagAttributeNameBk = CurBackColor;
    ColsToSet.crTagElementNameBk = CurBackColor;
    ColsToSet.crTagEntityBk = CurBackColor;
    ColsToSet.crExtra1Bk = CurBackColor;
    ColsToSet.crExtra2Bk = CurBackColor;
    ColsToSet.crExtra3Bk = CurBackColor;
    ColsToSet.crTagTextBk = CurBackColor;
    ColsToSet.crText = CurNormTextColor;
    ColsToSet.crHighlightedLine = CurHighTextColor;
    ColsToSet.crLeftMargin = CurMarginColor;
    ColsToSet.crLineNumberBk = CurMarginColor;
    ColsToSet.crLineNumber = CurLineColor;
    ColsToSet.crComment = LangCommentsColor;
    ColsToSet.crKeyword = LangColorAColor;
    ColsToSet.crNumber = LangNumbersColor;
    ColsToSet.crString = LangStringsColor;
    ColsToSet.crScopeKeyword = LangKeyWordsColor;
    ColsToSet.crOperator = LangColorBColor;
    ColsToSet.crTagAttributeName = LangColorCColor;
    ColsToSet.crTagElementName = LangColorDColor;
    ColsToSet.crTagEntity = LangColorEColor;
    ColsToSet.crExtra1 = LangColorFColor;
    ColsToSet.crExtra2 = LangColorGColor;
    ColsToSet.crExtra3 = LangColorHColor;
    // Set colors
    CM_SetColors(hWnd, &ColsToSet);
    if(ItalicFont == 1) StyleToSet.byComment = CM_FONT_ITALIC;
    else StyleToSet.byComment = CM_FONT_NORMAL;
    StyleToSet.byKeyword = CM_FONT_NORMAL;
    StyleToSet.byLineNumber = CM_FONT_NORMAL;
    StyleToSet.byNumber = CM_FONT_NORMAL;
    StyleToSet.byOperator = CM_FONT_NORMAL;
    StyleToSet.byScopeKeyword = CM_FONT_NORMAL;
    StyleToSet.byString = CM_FONT_NORMAL;
    StyleToSet.byExtra1 = CM_FONT_NORMAL;
    StyleToSet.byExtra2 = CM_FONT_NORMAL;
    StyleToSet.byExtra3 = CM_FONT_NORMAL;
    StyleToSet.byTagAttributeName = CM_FONT_NORMAL;
    StyleToSet.byTagElementName = CM_FONT_NORMAL;
    StyleToSet.byTagEntity = CM_FONT_NORMAL;
    StyleToSet.byTagText = CM_FONT_NORMAL;
    StyleToSet.byText = CM_FONT_NORMAL;
    CM_SetFontStyles(hWnd, &StyleToSet);
}

// -----------------------------------------------------------------------
// Set the line style of the codemax control
void SetCodeMaxLineStyle(HWND hWnd, long StyleToUse)
{
    CM_LINENUMBERING LnStyleStruct;
    
    memset(&LnStyleStruct, 0, sizeof(LnStyleStruct));
    switch(StyleToUse)
    {
        case 1:
            LnStyleStruct.bEnabled = 1;
            LnStyleStruct.dwStyle = CM_BINARY;
            LnStyleStruct.nStartAt = LnStart;
            LnStyleStruct.bComplementHexa = 0;
            CM_SetLineNumbering(hWnd, &LnStyleStruct);
            break;
        case 2:
            LnStyleStruct.bEnabled = 1;
            LnStyleStruct.dwStyle = CM_DECIMAL;
            LnStyleStruct.nStartAt = LnStart;
            LnStyleStruct.bComplementHexa = 0;
            CM_SetLineNumbering(hWnd, &LnStyleStruct);
            break;
        case 3:
            LnStyleStruct.bEnabled = 1;
            LnStyleStruct.dwStyle = CM_HEXADECIMAL;
            LnStyleStruct.nStartAt = LnStart;
            LnStyleStruct.bComplementHexa = 0;
            CM_SetLineNumbering(hWnd, &LnStyleStruct);
            break;
        case 4:
            LnStyleStruct.bEnabled = 1;
            LnStyleStruct.dwStyle = CM_OCTAL;
            LnStyleStruct.nStartAt = LnStart;
            LnStyleStruct.bComplementHexa = 0;
            CM_SetLineNumbering(hWnd, &LnStyleStruct);
            break;
        default:
            LnStyleStruct.bEnabled = 0;
            LnStyleStruct.dwStyle = 0;
            LnStyleStruct.nStartAt = 0;
            LnStyleStruct.bComplementHexa = 0;
            CM_SetLineNumbering(hWnd, &LnStyleStruct);
            break;
    }
}

// -----------------------------------------------------------------------
// Write cursor position in status bar
void WritePositionInStatus(HWND hWnd)
{
    CStr LnStatText;
    long LnCount = 0;
    long LnPercent = 0;
    long LnSel = 0;
    long ColSel = 0;
    CStr LnSelText;
    CStr ColSelText;
    CStr LnToPrint;
    CStr ColToPrint;
    long RealLine = 0;
    long RealCol = 0;

    if(ControlIsVisible(hStatusBar) != 0)
    {
        LnCount = CM_GetLineCount(hWnd);
        if(LnCount == 0) LnCount = 1;
        LnSel = CodeMaxCurRange.posEnd.nLine - CodeMaxCurRange.posStart.nLine;
        ColSel = CodeMaxCurRange.posEnd.nCol - CodeMaxCurRange.posStart.nCol;
        RealLine = CodeMaxCurRange.posStart.nLine;
        if(LnSel > 0) RealLine = CodeMaxCurRange.posEnd.nLine;
        RealCol = CodeMaxCurRange.posStart.nCol;
        if(ColSel > 0) RealCol = CodeMaxCurRange.posEnd.nCol;
        LnSelText = "";
        if(LnSel != 0) LnSelText = "/" + (CStr) LnSel;
        ColSelText = "";
        ColSel = CM_BufferColToViewCol(hWnd, RealLine, CodeMaxCurRange.posEnd.nCol) - CM_BufferColToViewCol(hWnd, CodeMaxCurRange.posStart.nLine, CodeMaxCurRange.posStart.nCol);
        if(ColSel > 0) ColSelText = "/" + (CStr) ColSel;
        LnPercent = ((RealLine + 1) * 100) / LnCount;
        LnStatText = "L: " + (CStr) (RealLine + 1) + (CStr) LnSelText + (CStr) "/" + (CStr) LnCount + (CStr) " - C: " + (CStr) (CM_BufferColToViewCol(hWnd, RealLine, RealCol) + 1) + (CStr) ColSelText + (CStr) " - " + (CStr) LnPercent + (CStr) "%";
        StatusBarSetText(hStatusBar, 1, STATUSBAR_PARTRAISED, LnStatText, 0);
        if(RefreshSBStat == 1) SetStatusInsert(hWnd);
    }
}

// -----------------------------------------------------------------------
// Write cursor insertion state in status bar
void SetStatusInsert(HWND hWnd)
{
    if(CM_IsOvertypeEnabled(hWnd) == 1) StatusBarSetText(hStatusBar, 2, STATUSBAR_PARTSUNKEN, "O", 0);
    else StatusBarSetText(hStatusBar, 2, STATUSBAR_PARTRAISED, "O", 0);
}

// -----------------------------------------------------------------------
// Retrieve current selection
void LoadCurrentSel(HWND hWnd)
{
    CM_GetSel(hWnd, &CodeMaxCurRange, 1);
}

// -----------------------------------------------------------------------
// Remove current selection
void RemCurrentSel(HWND hWnd)
{
    LoadCurrentSel(hWnd);
    CMSetCaretPos(hWnd, CodeMaxCurRange.posStart.nLine, CodeMaxCurRange.posStart.nCol);
}

// -----------------------------------------------------------------------
// Retrieve the top index of the current view
long GetCurrentTopIndex(HWND hWnd)
{
    return(CM_GetTopIndex(hWnd, CM_GetCurrentView(hWnd)));
}

// -----------------------------------------------------------------------
// Set the top index of the current view
void SetCurrentTopIndex(HWND hWnd, long NewTopIndex)
{
    CM_SetTopIndex(hWnd, CM_GetCurrentView(hWnd), NewTopIndex);
}

// -----------------------------------------------------------------------
// Retrieve current line number
long GetCurrentLineNumber(HWND hWnd)
{
    CM_GetSel(hWnd, &CodeMaxCurRange, 1);
    return(CodeMaxCurRange.posStart.nLine);
}

// -----------------------------------------------------------------------
// Retrieve current column number
long GetCurrentColNumber(HWND hWnd)
{
    CM_GetSel(hWnd, &CodeMaxCurRange, 1);
    return(CodeMaxCurRange.posStart.nCol);
}

// -----------------------------------------------------------------------
// Retrieve a text line
CStr CMGetCurrentLine(HWND hWnd, long LineNumber)
{
    CStr ReturnValue;
    CStr BufString;
    long CurrLineN = 0;
    
    CurrLineN = CM_GetLineLength(hWnd, LineNumber, 0);
    if(CurrLineN <= 0) return(ReturnValue);
    BufString = BufString.String(CurrLineN, 1);
    CM_GetLine(hWnd, LineNumber, BufString.Get_String());
    ReturnValue.Set_String(BufString.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the formated filename associated with codemax control
CStr CMGetRealFile(CStr CMFile)
{
    CStr ReturnValue;
    CStr BufString;
    
    BufString = CMFile.Left(CMFile.Len());
    ReturnValue.Set_String(BufString.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the formated language name associated with codemax control
CStr CMGetRealLanguage(CStr CMFile)
{
    CStr ReturnValue;
    CStr BufString;
    
    BufString = CMFile.Left(CMFile.Len());
    ReturnValue.Set_String(BufString.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the current line
CStr RetrieveCurrentLine(HWND hWnd)
{
    CStr ReturnValue;
    CStr BufString;
    long LineToRetrieve = 0;
    long LnToRetLen = 0;
    
    LineToRetrieve = GetCurrentLineNumber(hWnd);
    LnToRetLen = CM_GetLineLength(hWnd, LineToRetrieve, 0);
    if(LnToRetLen > 0)
    {
        BufString = BufString.String(LnToRetLen, 1);
        CM_GetLine(hWnd, LineToRetrieve, BufString.Get_String());
        ReturnValue.Set_String(BufString.Get_String());
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve logical selection length
long GetCurrentLogicalSelLen(HWND hWnd)
{
    CM_GetSel(hWnd, &CodeMaxCurRange, 1);
    return(CM_GetTextLength(hWnd, &CodeMaxCurRange, 1));
}

// -----------------------------------------------------------------------
// Retrieve logical selection
CStr GetCurrentLogicalSelText(HWND hWnd)
{
    CStr ReturnValue;
    CStr BufString;
    long SelL = 0;

    SelL = GetCurrentLogicalSelLen(hWnd);
    if(SelL == 0) return(ReturnValue);
    BufString = BufString.String(SelL, 1);
    CM_GetText(hWnd, BufString.Get_String(), &CodeMaxCurRange);
    ReturnValue.Set_String(BufString.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Set the caret at specified position
void CMSetCaretPos(HWND hWnd, long nLine, long nCol)
{
    CM_RANGE NewCaretPos;

    NewCaretPos.posStart.nCol = nCol;
    NewCaretPos.posEnd.nCol = nCol;
    NewCaretPos.posStart.nLine = nLine;
    NewCaretPos.posEnd.nLine = nLine;
    CM_SetSel(hWnd, &NewCaretPos, 1);
}

// -----------------------------------------------------------------------
// Select a specified word contained in a line
void CMSelectWordInLine(HWND hWnd, long nLine, CStr KeyWord)
{
    CM_RANGE NewCaretPos;
    CStr LineText;
    long nCol = 0;
    long nECol = 0;

    LineText = CMGetCurrentLine(hWnd, nLine);
    nCol = LineText.In_Str(1, KeyWord, Text_Compare) - 1;
    if(nCol != -1) nECol = nCol + KeyWord.Len();
    if(nCol == -1)
    {
        nCol = 0;
        nECol = 0;
    }
    NewCaretPos.posStart.nCol = nCol;
    NewCaretPos.posEnd.nCol = nECol;
    NewCaretPos.posStart.nLine = nLine;
    NewCaretPos.posEnd.nLine = nLine;
    CM_SetSel(hWnd, &NewCaretPos, 1);
}

// -----------------------------------------------------------------------
// Clear codemax control
void ClearFile(HWND hWnd)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(hWnd);
    ControlSetText(hWnd, ChildStruct->RFile);
    ChildStruct->FileLoaded = 0;
    CM_SetText(ChildStruct->hChildCodeMax, "");
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    SetFocus(ChildStruct->hChildCodeMax);
    RefreshSBStat = 1;
    WritePositionInStatus(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Load a file
// +++ Check return value
long LoadFile(HWND hWnd)
{
    long ReturnValue = 0;
    CStr DirectUnix;
    CStr FileTitle;
    long UnixBytesReaded = 0;
    long UnixFileHandle = 0;
    //long UnixModified = 0;

    ChildStruct = LoadStructure(hWnd);
    if(FileIsUnix(CMGetRealFile(ChildStruct->RFile)) == 1)
    {
        // Read value in ini file
        DirectUnix = IniReadKey("Layout", "AutoConvUNIX", MainIniFile);
        if(DirectUnix.Len() != 0)
        {
            if(_strcmpi(DirectUnix.Get_String(), "1") == 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "File '" + (CStr) CMGetRealFile(ChildStruct->RFile).Get_String() + (CStr) "' converted to DOS format.");
                goto DirectConvertUnix;
            }
        }
        if(MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) CMGetRealFile(ChildStruct->RFile).Get_String() + 
                        (CStr) "' is probably an Unix file.\rWant to convert it ?",
                        MB_QUESTION,
                        Requesters) == IDYES)
        {
DirectConvertUnix:
            UnixFileHandle = (long) MLoadFile(CMGetRealFile(ChildStruct->RFile).Get_String(), &UnixBytesReaded);
            CM_SetTextLng(ChildStruct->hChildCodeMax, UnixFileHandle);
            FreeMem(UnixFileHandle);
            //UnixModified = 1;
        }
        else
        {
            CM_OpenFile(ChildStruct->hChildCodeMax, CMGetRealFile(ChildStruct->RFile).Get_String());
        }
    }
    else
    {
        CM_OpenFile(ChildStruct->hChildCodeMax, CMGetRealFile(ChildStruct->RFile).Get_String());
    }
    // Set title
    FileTitle = CMGetRealFile(ChildStruct->RFile);
    //if(UnixModified == 1) {
        //ControlSetText(hWnd, FileTitle + (CStr) " *");
        //ChildStruct->ModifFlag = 1;
    //} else {
        ControlSetText(hWnd, FileTitle);
        ChildStruct->ModifFlag = 0;
    //}
    // Fill with last write time
    FileGetWriteTime(FileTitle, ChildStruct->FileDateOpen);
    ChildStruct->FileLoaded = 1;
    if(FileIsReadOnly(FileTitle))
    {
        SetChildReadOnly(hWnd, TRUE);
    }
    else
    {
        SetChildReadOnly(hWnd, FALSE);
    }
    RefreshSBStat = 1;
    WritePositionInStatus(ChildStruct->hChildCodeMax);
    SetHighLine(ChildStruct->hChildCodeMax);
    ReturnValue = 1;
    RefreshChildTitle(hWnd);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Include a file
// +++ Check return value
long IncludeFile(HWND hWnd, CStr FName)
{
    long ReturnValue = 0;
    CStr DirectUnix;
    CStr FileTitle;
    long UnixBytesReaded = 0;
    long UnixFileHandle = 0;

    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(FileIsUnix(FName) == 1)
    {
        // Read value in ini file
        DirectUnix = IniReadKey("Layout", "AutoConvUNIX", MainIniFile);
        if(DirectUnix.Len() != 0)
        {
            if(_strcmpi(DirectUnix.Get_String(), "1") == 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "File '" + (CStr) FName.Get_String() + (CStr) "' converted to DOS format.");
                goto DirectConvertUnix;
            }
        }
        if(MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) FName.Get_String() + (CStr) "' is probably an Unix file.\rWant to convert it ?", 
                        MB_QUESTION, Requesters) == IDYES)
        {
DirectConvertUnix:
            UnixFileHandle = (long) MLoadFile(FName.Get_String(), &UnixBytesReaded);
            CM_InsertTextLng(ChildStruct->hChildCodeMax, UnixFileHandle, &CodeMaxCurRange);
            LoadBookmarks(hWnd, FName, CodeMaxCurRange.posStart.nLine);
            FreeMem(UnixFileHandle);
        }
        else
        {
            CM_InsertFile(ChildStruct->hChildCodeMax, FName.Get_String(), &CodeMaxCurRange);
            LoadBookmarks(hWnd, FName, CodeMaxCurRange.posStart.nLine);
        }
    }
    else
    {
        CM_InsertFile(ChildStruct->hChildCodeMax, FName.Get_String(), &CodeMaxCurRange);
        LoadBookmarks(hWnd, FName, CodeMaxCurRange.posStart.nLine);
    }
    WritePositionInStatus(ChildStruct->hChildCodeMax);
    ReturnValue = 1;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Save a file
long SaveCodeMax(HWND hWnd, CStr FName)
{
    CStr BakDirToSave;
    CStr NameDeco;
    long ReturnValue = 0;

    ChildStruct = LoadStructure(hWnd);
    if(CM_SaveFile(ChildStruct->hChildCodeMax, FName.Get_String(), 0) != CME_SUCCESS) goto ErrSave;
    // Create the backup
    if(CreateBak == 1)
    {
        WriteComment("Creating backup file...");
        // Retrieve the backup dir
        BakDirToSave = IniReadKey("Layout", "BackupDir", MainIniFile);
        if(BakDirToSave.Len() != 0)
        {
            if(strcmp(BakDirToSave.Right(1).Get_String(), "\\") != 0) BakDirToSave = BakDirToSave + "\\";
            BakDirToSave = ChangeRelativePaths(BakDirToSave);
            BakDirToSave = BakDirToSave + (CStr) FileGetFileName(CMGetRealFile(FName)).Get_String();
        }
        else
        {
            BakDirToSave = CMGetRealFile(FName).Get_String();
        }
        // Add decoration
        if(DecorateBak == 1)
        {
            NameDeco = DateGetDay() + (CStr) DateGetMonth() + (CStr) DateGetYear() + (CStr) DateGetHour();
            NameDeco = StringReplace(NameDeco, ":", "", 1, -1, Binary_Compare);
            NameDeco = StringReplace(NameDeco, " ", "", 1, -1, Binary_Compare);
            BakDirToSave = BakDirToSave + "." + (CStr) NameDeco;
        }
        BakDirToSave = BakDirToSave + ".BAK";
        DeleteFile(BakDirToSave.Get_String());
        CM_SaveFile(ChildStruct->hChildCodeMax, BakDirToSave.Get_String(), 0);
        WriteComment("");
    }
    SaveBookmarks(hWnd, FName, 0, -1);
    ChildStruct->RFile->Set_String(FName.Get_String());
    ControlSetText(hWnd, CMGetRealFile(ChildStruct->RFile));
    // Store new modified date
    FileGetWriteTime(CMGetRealFile(ChildStruct->RFile), ChildStruct->FileDateOpen);
    ChildStruct->FileLoaded = 1;
    ChildStruct->ModifFlag = 0;
    UpdateLanguage(hWnd, FName);
    RefreshChildTitle(hWnd);
    ReturnValue = 1;
    return(ReturnValue);
ErrSave:
    MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) CMGetRealFile(FName).Get_String() + (CStr) "' seems to be locked: can't save.",
                 MB_ERROR, Requesters);
    ReturnValue = 0;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Save the bookmarks of a file
void SaveBookmarks(HWND hWnd, CStr FileName, long BaseLine, long Lines)
{
    CStr SaveBook;
    CStr BookName;
    HANDLE BookFile = 0;
    long BkNumber = 0;
    long BkBlock = 0;
    long *BkBlockPtr = 0;
    long BkLineNumber = 0;
    long BkRealNumber = 0;
    int i = 0;
    
    if(_strcmpi(FileName.Get_String(), "<untitled document") != 0)
    {
        // Save the bookmarks
        SaveBook = IniReadKey("Layout", "SaveBookmarks", MainIniFile);
        if(SaveBook.Len() != 0)
        {
            if(strcmp(SaveBook.Get_String(), "1") == 0)
            {
                // Retrieve the number of bookmarks
                BookName = FileReplaceExtension(FileName, "mbk");
                ChildStruct = LoadStructure(hWnd);
                BkNumber = CM_GetAllBookmarks(ChildStruct->hChildCodeMax, 0);
                if(BkNumber != 0)
                {
                    // Alloc room to obtain bookmarks lines numbers
                    BkBlock = AllocMem(BkNumber * 4);
                    CM_GetAllBookmarks(ChildStruct->hChildCodeMax, BkBlock);
                    // Create the file
                    BookFile = FileCreateEmpty(BookName, NO_SECURITY_DESCRIPTOR);
                    FileWriteLine(BookFile, "; Chromatic bookmarks file");
                    FileWriteLine(BookFile, "; Do NOT edit this file manually");
                    FileWriteLine(BookFile, "; unless you know what you're doing !!!");
                    FileWriteLine(BookFile, "; (C) 2001-2010 Franck Charlet.");
                    FileWriteLine(BookFile, "");
                    FileWriteLine(BookFile, "[BOOKMARKDAT]");
                    FileClose(BookFile);
                    // Save number of bookmarks
                    BkRealNumber = 0;
                    // Store bookmarks datas
                    BkBlockPtr = (long *) BkBlock;
                    for(i = 0; i <= BkNumber - 1; i++)
                    {
                        BkLineNumber = BkBlockPtr[i] - BaseLine;
                        if(BkLineNumber >= 0)
                        {
                            // Was in a block ?
                            if(Lines != -1) if(BkLineNumber > Lines) goto NoWriteBookmark;
                            IniWriteKey("BOOKMARKDAT", "BOOK" + (CStr) StringNumberComplement(BkRealNumber, 6).Get_String(),
                                          BkLineNumber, BookName);
                            BkRealNumber++;
NoWriteBookmark:;       }
                    }
                    IniWriteKey("BOOKMARKDAT", "BOOKNBR", BkRealNumber, BookName);
                    FreeMem(BkBlock);
                }
                else
                {
                    DeleteFile(BookName.Get_String());
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Load the bookmarks of a file
void LoadBookmarks(HWND hWnd, CStr BkFName, long BaseLine)
{
    CStr LoadBook;
    CStr BkNameToLoad;
    CStr NbBooks;
    long NbBooksLng = 0;
    CStr BookToStore;
    long BookToStoreLng = 0;
    long RealLoadBook = 0;
    FILETIME RootfileTime;
    FILETIME BookfileTime;
    int i = 0;
    
    LoadBook = IniReadKey("Layout", "SaveBookmarks", MainIniFile);
    if(LoadBook.Len() != 0)
    {
        if(strcmp(LoadBook.Get_String(), "1") == 0)
        {
            // Load the associated bookmarks of the file
            BkNameToLoad = FileReplaceExtension(BkFName, "mbk");
            if(FileExist(BkNameToLoad) == 1)
            {
                FileGetWriteTime(BkFName, &RootfileTime);
                FileGetWriteTime(BkNameToLoad, &BookfileTime);
                // Check if bookmark file is obsolete
                if(CompareFileTime(&RootfileTime, &BookfileTime) == 1) return;
                NbBooks = IniReadKey("BOOKMARKDAT", "BOOKNBR", BkNameToLoad);
                if(NbBooks.Len() != 0)
                {
                    if(NbBooks.Is_Numeric())
                    {
                        NbBooksLng = NbBooks.Get_Long();
                        RealLoadBook = 0;
                        ChildStruct = LoadStructure(hWnd);
                        for(i = 0; i <= NbBooksLng - 1; i++)
                        {
                            BookToStore = IniReadKey("BOOKMARKDAT", "BOOK" + (CStr) StringNumberComplement(i, 6).Get_String(), BkNameToLoad);
                            if(BookToStore.Len() != 0)
                            {
                                if(BookToStore.Is_Numeric())
                                {
                                    BookToStoreLng = BookToStore.Get_Long();
                                    CM_SetBookmark(ChildStruct->hChildCodeMax, BookToStoreLng + BaseLine, 1);
                                    RealLoadBook = 1;
                                }
                            }
                        }
                        // Set the margin if required
                        if(RealLoadBook == 1) CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 1);
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Change source language if needed
void UpdateLanguage(HWND hWnd, CStr FileName)
{
    CStr NewRFile;
    CStr NewLanguageToStore;
    
    ChildStruct = LoadStructure(hWnd);
    NewRFile = CMGetRealFile(FileName);
    NewLanguageToStore = GetLanguageToOpen(NewRFile);
    if(_strcmpi(NewLanguageToStore.Get_String(), CMGetRealFile(ChildStruct->Language).Get_String()) != 0)
    {
        StoreLanguage(NewLanguageToStore);
        if(NewLanguageToStore.Len() == 0)
        {
            SetCodeMaxLanguage(ChildStruct->hChildCodeMax);
        }
        else
        {
            SetCodeMaxLanguage(ChildStruct->hChildCodeMax);
        }
    }
    RefreshIdentScope(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Print file content
void PrintFile(HWND hWnd, long Selection)
{
    long PFlags = 0;
    CStr BufString;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(hWnd);
    if(Selection == 1) if(GetCurrentLogicalSelLen(ChildStruct->hChildCodeMax) == 0) goto ErrEmptyPrint;
    else if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) goto ErrEmptyPrint;
    PFlags = CM_PRINT_PAGENUMS | CM_PRINT_DATETIME | CM_PRINT_RICHFONTS | CM_PRINT_PROMPTDLG | CM_PRINT_COLOR;
    if(Selection == 1) PFlags = PFlags | CM_PRINT_SELECTION;
    if(CM_Print(ChildStruct->hChildCodeMax, 0, PFlags) != CME_SUCCESS) goto ErrPrint;
    return;
ErrPrint:
    BufString = "Can't print file '" + (CStr) ChildStruct->RFile->Left(ChildStruct->RFile->Len()).Get_String() + (CStr) "'.";
    MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
    return;
ErrEmptyPrint:
    MiscMsgBox(hMDIform.hWnd, "No text selected.", MB_ERROR, Requesters);
}

// -----------------------------------------------------------------------
// Retrieve the current char of a selected text
CStr GetCurrentCharWord(HWND hWnd)
{
    CStr ReturnValue;
    CM_RANGE CodeMaxCurCharRange;
    CStr BufString;
    
    CM_GetSel(hWnd, &CodeMaxCurCharRange, 1);
    CodeMaxCurCharRange.posEnd.nCol = CodeMaxCurCharRange.posEnd.nCol + 1;
    BufString = BufString.String(CM_GetTextLength(hWnd, &CodeMaxCurCharRange, 1), 1);
    CM_GetText(hWnd, BufString.Get_String(), &CodeMaxCurCharRange);
    ReturnValue.Set_String(BufString.Left(1).Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Select and retrieve the current word
CStr SelectCurrentWord(HWND hWnd)
{
    CStr ReturnValue;
    CStr BufString;

    BufString = GetCurrentWord(hWnd);
    if(BufString.Len() != 0)
    {
        if(GetCurrentLogicalSelLen(hWnd) == 0)
        {
            CM_ExecuteCmd(hWnd, CMD_WORDLEFT, 0);
            CM_ExecuteCmd(hWnd, CMD_WORDENDRIGHTEXTEND, 0);
            BufString = GetCurrentWord(hWnd);
        }
    }
    ReturnValue.Set_String(BufString.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the current word
CStr GetCurrentWord(HWND hWnd)
{
    CStr ReturnValue;
    CStr BufString;
    CM_POSITION CPos;

ReSelectWord:
    LoadCurrentSel(hWnd);
    CPos.nCol = CodeMaxCurRange.posStart.nCol;
    CPos.nLine = CodeMaxCurRange.posStart.nLine;
    if(GetCurrentLogicalSelLen(hWnd) == 0)
    {
        BufString = BufString.String(CM_GetWordLength(hWnd, &CPos, 1), 1);
        CM_GetWord(hWnd, BufString.Get_String(), &CPos);
    }
    else
    {
        BufString = GetCurrentLogicalSelText(hWnd);
    }
    // Pass punctations
    if(BufString == ".")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
ForceItRight:
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == ",")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == "(")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == ":")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == ")")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == "+")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == "*")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == "/")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == "-")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    else if(BufString == "$")
    {
        if(_strcmpi(GetCurrentCharWord(hWnd).Get_String(), BufString.Get_String()) == 0)
        {
            if(CPos.nCol == 0) goto ForceItRight;
            CM_ExecuteCmd(hWnd, CMD_CHARLEFT, 0);
        }
        else
        {
            CM_ExecuteCmd(hWnd, CMD_CHARRIGHT, 0);
        }
        goto ReSelectWord;
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// CodeMax hook
LRESULT CALLBACK CodeMaxProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HMENU hPop;
    HMENU hPopSub;
    HMENU hPopSubSub;
    long NbrDroped = 0;
    long FNameSize = 0;
    CStr DropedFileName;
    POINT DropPosition;
    CM_POSITION DropInPosition;
    int i = 0;
    CStr SnippetVal;
    long wParamSelCM = 0;
    CStr LocCurWord;
    CStr CurrentCMLine;
    CStr CompleteCMLine;
    long PosInvoke = 0;
    long *InvokeArray = 0;
    CStr GetFncWord;
    long CurFncPos = 0;
    CStr CurFncChar;
    long DroppedFile = 0;
    long DroppedRead = 0;
    long LineToDrop = 0;
    long BracketBalance = 0;
    CStr ExprCharToTest;
    long wParamCMD = 0;
    long HaveToDie = 0;
    long *LParamGetSel;
    DLGPROC LocCodeMaxHook;
    POINT New_Mouse_Coords;
    CStr Script_FileName;
    int ScriptFile_Size;
    long ScriptFile_Size_Out;
    HGLOBAL Script_File;
    
    // Execute possible hooks
    for(i = 0; i < CodeMaxHooks.Amount(); i++)
    {
        if(CodeMaxHooks.Get(i)->Content != 0)
        {
            LocCodeMaxHook = CodeMaxHooks.Get(i)->Content;
            if(LocCodeMaxHook(hWnd, uMsg, wParam, lParam) == HOOK_DIE)
            {
                HaveToDie = HaveToDie | HOOK_DIE;
            }
        }
    }
    if((long) HaveToDie == HOOK_DIE) return(0);
    
    switch(uMsg)
    {
        // ----------------------- RichTexBox emulation -----------
        case EM_EXGETSEL:
            // lParam should contains CHARRANGE address
            LParamGetSel = (long *) lParam;
            LParamGetSel[0] = 0;
            LParamGetSel[1] = GetCurrentLogicalSelLen(hWnd);
            return(0);
        case EM_EXSETSEL:
            // Not used yet
            return(0);
        case EM_GETSELTEXT:
            // lParam should contains buffer address to store at
            if(lParam != 0) RtlCopyMemory((void *) lParam, GetCurrentLogicalSelText(hWnd).Get_String(), GetCurrentLogicalSelLen(hWnd));
            return(0);
        case EM_REPLACESEL:
            if(IsChildReadOnly(GetParent(hWnd)) == 1) return(0);
            // wParam is not used (can undo or not)
            // lParam should contains buffer address to retrieve from
            EmulTxtToRep = EmulTxtToRep.String(strlen((char *) lParam), 1);
            CopyMemory(EmulTxtToRep.Get_String(), (void *) lParam, strlen((char *) lParam));
            CM_ReplaceSel(hWnd, EmulTxtToRep.Get_String());
            return(0);
        
        // Create Contextual menu
        case WM_CONTEXTMENU:
            if((HWND) wParam == hWnd)
            {
                if(CursorDisabled) DisplayCursor();
                hPop = CreatePopupMenu();
                MenuAddString(hPop, "Context help\tF1", MENU_CONTEXT_CONTEXTHELP_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPop, "MSDN/Platform SDK help\tCtrl+F1", MENU_CONTEXT_MSDNHELP_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPop, "Windows DDK help\tCtrl+Shift+F1", MENU_CONTEXT_DDKHELP_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPop, "DirectX SDK help\tAlt+Shift+F1", MENU_CONTEXT_DIRECTXSDKHELP_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddSeparator(hPop);
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "Open file under caret\tCtrl+Enter", MENU_CONTEXT_OPENFILEUNDERCURSOR_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Execute file under caret\tCtrl+Shift+Enter", MENU_CONTEXT_EXECFILEUNDERCURSOR_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddSeparator(hPopSub);
                MenuAddString(hPopSub, "Convert dec to hex\tCtrl+1", MENU_CONTEXT_DECTOHEX_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Convert dec to bin\tCtrl+2", MENU_CONTEXT_DECTOBIN_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Convert hex to dec\tCtrl+3", MENU_CONTEXT_HEXTODEC_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Convert hex to bin\tCtrl+4", MENU_CONTEXT_HEXTOBIN_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Convert bin to dec\tCtrl+5", MENU_CONTEXT_BINTODEC_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Convert bin to hex\tCtrl+6", MENU_CONTEXT_BINTOHEX_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Convert hex to float\tCtrl+7", MENU_CONTEXT_HEXTOFLT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Convert float to hex\tCtrl+8", MENU_CONTEXT_FLTTOHEX_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Target");
                hPopSub = CreatePopupMenu();
                CreateSnippetMenu(hPopSub, hWnd);
                if(SnipArray.Amount() != 0) AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Insert snippet (Ctrl+Alt+Space)");
                hPopSub = CreatePopupMenu();
                if(CreateLangScripts(hPopSub)) AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Scripts (Ctrl+Shift+Space)");
                MenuAddSeparator(hPop);
                MenuAddString(hPop, "Undo\tCtrl+Z", MENU_CONTEXT_UNDO_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPop, "Redo\tCtrl+Y", MENU_CONTEXT_REDO_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddSeparator(hPop);
                MenuAddString(hPop, "Cut\tCtrl+X", MENU_CONTEXT_CUT_ID + MENU_CONTEXT_IDBASE, TRUE);
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "Copy selection\tCtrl+C", MENU_CONTEXT_COPY_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                MenuAddString(hPopSub, "Copy file path/name", MENU_CONTEXT_COPYFILEPATHNAME_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Copy entire text", MENU_CONTEXT_COPYENTIRETEXT_ID + MENU_CONTEXT_IDBASE, TRUE);
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Copy");
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "To current window\tCtrl+V", MENU_CONTEXT_PASTE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                MenuAddString(hPopSub, "To new window\tCtrl+Shift+V", MENU_CONTEXT_PASTENEWWINDOW_ID + MENU_CONTEXT_IDBASE, TRUE);
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Paste");
                MenuAddSeparator(hPop);
                MenuAddString(hPop, "Select line\tCtrl+Alt+F8", MENU_CONTEXT_SELECTLINE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPop, "Select all\tCtrl+A", MENU_CONTEXT_SELECTALL_ID + MENU_CONTEXT_IDBASE, TRUE);
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "Select\tCtrl+Shift+A", MENU_CONTEXT_SELECTPROCEDURE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                MenuAddString(hPopSub, "Select and collapse\tCtrl+Shift+Q", MENU_CONTEXT_SELECTPROCEDUREANDCOLLAPSE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Expand\tCtrl+Shift+W", MENU_CONTEXT_UNCOLLAPSEPROCEDURE_ID + MENU_CONTEXT_IDBASE, TRUE);
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Procedure");
                MenuAddSeparator(hPop);
                MenuAddString(hPop, "Find...\tCtrl+F", MENU_CONTEXT_FIND_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPop, "Replace...\tF4", MENU_CONTEXT_REPLACE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddSeparator(hPop);
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "Toggle\tF2", MENU_CONTEXT_BOOKMARKS_TOGGLE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                MenuAddString(hPopSub, "First\tCtrl+F7", MENU_CONTEXT_BOOKMARKS_FIRST_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Previous\tShift+F2", MENU_CONTEXT_BOOKMARKS_PREVIOUS_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Next\tCtrl+F2", MENU_CONTEXT_BOOKMARKS_NEXT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Last\tCtrl+Shift+F7", MENU_CONTEXT_BOOKMARKS_LAST_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Clear all\tCtrl+Shift+F2", MENU_CONTEXT_BOOKMARKS_CLEARALL_ID + MENU_CONTEXT_IDBASE, TRUE);
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Bookmarks");
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "Toggle\tF10", MENU_CONTEXT_BREAKPOINTS_TOGGLE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                MenuAddString(hPopSub, "First\tCtrl+F7", MENU_CONTEXT_BREAKPOINTS_FIRST_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Previous\tShift+F10", MENU_CONTEXT_BREAKPOINTS_PREVIOUS_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Next\tCtrl+F10", MENU_CONTEXT_BREAKPOINTS_NEXT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Last\tCtrl+Shift+F7", MENU_CONTEXT_BREAKPOINTS_LAST_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Clear all\tCtrl+Shift+F10", MENU_CONTEXT_BREAKPOINTS_CLEARALL_ID + MENU_CONTEXT_IDBASE, TRUE);
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Code breakpoints");
                MenuAddSeparator(hPop);
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "Top\tCtrl+Home", MENU_CONTEXT_GOTO_TOP_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                MenuAddString(hPopSub, "Bottom\tCtrl+End", MENU_CONTEXT_GOTO_BOTTOM_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Line\tCtrl+G", MENU_CONTEXT_GOTO_LINE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Matching bracket\tCtrl+[", MENU_CONTEXT_GOTO_MATCHINGBRACKET_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Previous word\tCtrl+Left", MENU_CONTEXT_GOTO_PREVIOUSWORD_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Next word\tCtrl+Right", MENU_CONTEXT_GOTO_NEXTWORD_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Previous procedure\tAlt+Up", MENU_CONTEXT_GOTO_PREVIOUSPROCEDURE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Next procedure\tAlt+Down", MENU_CONTEXT_GOTO_NEXTPROCEDURE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Previous paragraph\tCtrl+Alt+Left", MENU_CONTEXT_GOTO_PREVIOUSPARAGRAPH_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Next paragraph\tCtrl+Alt+Right", MENU_CONTEXT_GOTO_NEXTPARAGRAPH_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddSeparator(hPopSub);
                MenuAddString(hPopSub, "Variable declaration\tCtrl+Alt+S", MENU_CONTEXT_GOTO_VARIABLEDECLARATION_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Variable next use", MENU_CONTEXT_GOTO_VARIABLENEXTUSE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Procedure prototype", MENU_CONTEXT_GOTO_PROCEDUREPROTOTYPE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Procedure entry point\tCtrl+Alt+F", MENU_CONTEXT_GOTO_PROCEDUREENTRYPOINT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Procedure next use", MENU_CONTEXT_GOTO_PROCEDURENEXTUSE_ID + MENU_CONTEXT_IDBASE, TRUE);
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Goto");
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "Indent\tTab", MENU_CONTEXT_BLOCK_INDENT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                MenuAddString(hPopSub, "Unindent\tShift+Tab", MENU_CONTEXT_BLOCK_UNINDENT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Upper case\tCtrl+U", MENU_CONTEXT_BLOCK_UPPERCASE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Lower case\tCtrl+L", MENU_CONTEXT_BLOCK_LOWERCASE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Toggle case\tCtrl+Shift+U", MENU_CONTEXT_BLOCK_TOGGLECASE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Comment\tCtrl+K", MENU_CONTEXT_BLOCK_COMMENT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Uncomment\tCtrl+M", MENU_CONTEXT_BLOCK_UNCOMMENT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Tabify\tCtrl+Shift+T", MENU_CONTEXT_BLOCK_TABIFY_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Untabify\tCtrl+Alt+T", MENU_CONTEXT_BLOCK_UNTABIFY_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Spaces to tabs\tCtrl+Alt+R", MENU_CONTEXT_BLOCK_SPACESTOTABS_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Registers\tCtrl+R", MENU_CONTEXT_BLOCK_REGISTERS_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Comment (multi lines)\tCtrl+Shift+K", MENU_CONTEXT_BLOCK_BLKCOMMENT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Custom comment...\tCtrl+Shift+F8", MENU_CONTEXT_BLOCK_CUSTOMCOMMENT_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Trim trailing spaces\tCtrl+Alt+G", MENU_CONTEXT_BLOCK_TRIMSPACES_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddSeparator(hPopSub);
                MenuAddString(hPopSub, "Convert to include...\tCtrl+Shift+L", MENU_CONTEXT_BLOCK_CONVERTTOINCLUDE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Save as...\tCtrl+W", MENU_CONTEXT_BLOCK_SAVEAS_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPopSub, "Print...\tCtrl+Shift+P", MENU_CONTEXT_BLOCK_PRINT_ID + MENU_CONTEXT_IDBASE, TRUE);
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Block");
                MenuAddSeparator(hPop);
                MenuAddString(hPop, "New\tCtrl+N", MENU_CONTEXT_NEW_ID + MENU_CONTEXT_IDBASE, TRUE);
                
                hPopSub = CreatePopupMenu();
                MenuAddString(hPopSub, "Source file(s)...\tCtrl+I", MENU_CONTEXT_INCLUDEFILE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuSetDefaultItem(hPopSub, 0);
                hPopSubSub = CreatePopupMenu();
                // 0 to avoid global menu registering
                CreateFiltersMenu(hPopSubSub, 0, MENU_CONTEXT_INCLUDEFILTERS_ID + MENU_CONTEXT_IDBASE, "Laboratory...\tCtrl+Shift+I", 1);
                AppendMenu(hPopSub, MF_POPUP, (UINT) hPopSubSub, "Filters");
                AppendMenu(hPop, MF_POPUP, (UINT) hPopSub, "Include");
                
                MenuAddString(hPop, "Duplicate file", MENU_CONTEXT_DUPLICATEFILE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddString(hPop, "Reload file\tCtrl+Shift+N", MENU_CONTEXT_RELOADFILE_ID + MENU_CONTEXT_IDBASE, TRUE);
                MenuAddSeparator(hPop);
                MenuAddString(hPop, "Properties...\tF8", MENU_CONTEXT_PROPERTIES_ID + MENU_CONTEXT_IDBASE, TRUE);
                if(lParam == -1) lParam = (WAMMGetCaretRealYCoordinate(hWnd) * 0x10000) | (WAMMGetCaretRealXCoordinate(hWnd) & 0xFFFF);
                TrackPopupMenu(hPop, TPM_LEFTALIGN | TPM_LEFTBUTTON, lParam & 0xFFFF, ((lParam & 0xFFFF0000) >> 16), 0, hWnd, NULL);
                return(0);
            }
            else
            {
                return(0);
            }
            break;
        // Process dropped file from shell
        case WM_DROPFILES:
            if(AcceptFiles == 1)
            {
                if(wParam != 0)
                {
                    if(IniReadBoolKey("Layout", "NewWindowOnDrop", MainIniFile))
                    {
                        // Retrieve the number of droped files
                        NbrDroped = DragQueryFile((HDROP) wParam, 0xFFFFFFFF, 0, 0);
                        if(NbrDroped != 0)
                        {
                            for(i = 0; i <= (long) NbrDroped - 1; i++)
                            {
                                // Retrieve the size to allocate for filename (+1)
                                FNameSize = DragQueryFile((HDROP) wParam, i, NULL, 0) + 1;
                                if(FNameSize != 0)
                                {
                                    // Allocate space
                                    DropedFileName = DropedFileName.String(FNameSize + 1, ' ');
                                    // Retrieve filename
                                    DragQueryFile((HDROP) wParam, i, DropedFileName.Get_String(), FNameSize);
                                    DropedFileName = DropedFileName.Trim();
                                    OpenUnknownFile(DropedFileName, TRUE);
                                }
                            }
                        }
                        return(0);
                    }
                    else
                    {
                        // Retrieve the number of droped files
                        NbrDroped = DragQueryFile((HDROP) wParam, 0xFFFFFFFF, 0, 0);
                        if(NbrDroped != 0)
                        {
                            // Retrieve cursor position in window
                            DragQueryPoint((HDROP) wParam, &DropPosition);
                            // Set the caret new position (line only)
                            CM_GetSelFromPoint(hWnd, DropPosition.y, DropPosition.x, &DropInPosition);
                            if(DropInPosition.nLine > CM_GetLineCount(hWnd)) DropInPosition.nLine = CM_GetLineCount(hWnd);
                            // Deplace caret at dropping line
                            CM_SetCaretPos(hWnd, DropInPosition.nLine, 0);
                            for(i = 0; i <= NbrDroped - 1; i++)
                            {
                                // Retrieve the size to allocate for filename (+1)
                                FNameSize = DragQueryFile((HDROP) wParam, i, NULL, 0) + 1;
                                if(FNameSize != 0)
                                {
                                    // Allocate space
                                    DropedFileName = DropedFileName.String(FNameSize + 1, ' ');
                                    // Retrieve filename
                                    DragQueryFile((HDROP) wParam, i, DropedFileName.Get_String(), FNameSize);
                                    // Include it in source
                                    DropedFileName = DropedFileName.Trim();
                                    DroppedFile = (long) MLoadFile(DropedFileName.Get_String(), &DroppedRead);
                                    ChildStruct = LoadStructure(CurrentForm);
                                    if(DroppedFile != 0)
                                    {
                                        LoadCurrentSel(ChildStruct->hChildCodeMax);
                                        LineToDrop = CodeMaxCurRange.posStart.nLine;
                                        SmartInsertFile(CurrentForm, DroppedFile);
                                        LoadBookmarks(CurrentForm, CMGetRealFile(DropedFileName), LineToDrop);
                                        FreeMem(DroppedFile);
                                    }
                                    if(_strcmpi(ChildStruct->RFile->Left(18).Get_String(), "<untitled document") == 0)
                                    {
                                        CM_SetModified(hWnd, 0);
                                        ChildStruct->RFile->Set_String(DropedFileName.Get_String());
                                        ControlSetText(CurrentForm, ChildStruct->RFile);
                                        UpdateLanguage(CurrentForm, DropedFileName);
                                    }
                                }
                            }
                        }
                    }
                    SetFocus(hWnd);
                }
            }
            break;

        // --- Contextual menu selections
        case WM_MENUSELECT:
            if(((wParam >> 16) & MF_SYSMENU) == 0)
            {
                if((wParam & 0x8000) == 0)
                {
                    if(CursorDisabled) DisplayCursor();
                    wParamSelCM = (wParam & 0xFFFF);
                    if(wParamSelCM >= MENU_SCRIPTINSERT_IDBASE)
                    {
                        if((wParamSelCM - MENU_SCRIPTINSERT_IDBASE) < LUAArray.Amount())
                        {
                            WriteComment(LUAArray.Get(wParamSelCM - MENU_SCRIPTINSERT_IDBASE)->Content);
                        }
                        else
                        {
                            WriteComment("Modify scripts list for this language");
                        }
                        return(0);
                    }
                    else if(wParamSelCM >= MENU_CODEMAXIMA_IDBASE)
                    {
                        WriteComment("");
                        return(0);
                    }
                    else if(wParamSelCM < MENU_CONTEXT_IDBASE)
                    {
                        WriteComment("");
                        return(0);
                    }
                    else if(wParamSelCM < MENU_SNIPPET_IDBASE)
                    {
                        WriteComment(MenuCodeMaxComments[wParamSelCM - MENU_CONTEXT_IDBASE]);
                        return(0);
                    }
                    else if(wParamSelCM < MENU_CODEMAXIMA_IDBASE)
                    {
                        WriteComment(SnipArray.Get(wParamSelCM - MENU_SNIPPET_IDBASE)->Content);
                        return(0);
                    }
                }
            }
            break;

        case WM_EXITMENULOOP:
            WriteComment("");
            return(0);
        
        // --- Contextual menu commands
        case WM_COMMAND:
            if(CursorDisabled) DisplayCursor();
            if((wParam & 0x8000) == 0)
            {
                wParamCMD = (wParam & 0xFFFF);
                switch(wParamCMD)
                {
                    case MENU_CONTEXT_CONTEXTHELP_ID + MENU_CONTEXT_IDBASE:
                        MCMD_ContextHelp();
                        return(0);
                    case MENU_CONTEXT_MSDNHELP_ID + MENU_CONTEXT_IDBASE:
                        MCMD_MSDNHelp();
                        return(0);
                    case MENU_CONTEXT_DDKHELP_ID + MENU_CONTEXT_IDBASE:
                        MCMD_DDKHelp();
                        return(0);
                    case MENU_CONTEXT_DIRECTXSDKHELP_ID + MENU_CONTEXT_IDBASE:
                        MCMD_DirectXSDKHelp();
                        return(0);
                    case MENU_CONTEXT_OPENFILEUNDERCURSOR_ID + MENU_CONTEXT_IDBASE:
                        OpenFileUnderCursor(0, 0);
                        return(0);
                    case MENU_CONTEXT_EXECFILEUNDERCURSOR_ID + MENU_CONTEXT_IDBASE:
                        OpenFileUnderCursor(0, 1);
                        return(0);
                    case MENU_CONTEXT_DECTOHEX_ID + MENU_CONTEXT_IDBASE:
                        CMDecToHex(hWnd);
                        return(0);
                    case MENU_CONTEXT_DECTOBIN_ID + MENU_CONTEXT_IDBASE:
                        CMDecToBin(hWnd);
                        return(0);
                    case MENU_CONTEXT_HEXTODEC_ID + MENU_CONTEXT_IDBASE:
                        CMHexToDec(hWnd);
                        return(0);
                    case MENU_CONTEXT_HEXTOBIN_ID + MENU_CONTEXT_IDBASE:
                        CMHexToBin(hWnd);
                        return(0);
                    case MENU_CONTEXT_BINTODEC_ID + MENU_CONTEXT_IDBASE:
                        CMBinToDec(hWnd);
                        return(0);
                    case MENU_CONTEXT_BINTOHEX_ID + MENU_CONTEXT_IDBASE:
                        CMBinToHex(hWnd);
                        return(0);
                    case MENU_CONTEXT_HEXTOFLT_ID + MENU_CONTEXT_IDBASE:
                        CMHexToFlt(hWnd);
                        return(0);
                    case MENU_CONTEXT_FLTTOHEX_ID + MENU_CONTEXT_IDBASE:
                        CMFltToHex(hWnd);
                        return(0);
                    case MENU_CONTEXT_UNDO_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Undo();
                        return(0);
                    case MENU_CONTEXT_REDO_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Redo();
                        return(0);
                    case MENU_CONTEXT_CUT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Cut();
                        return(0);
                    case MENU_CONTEXT_COPY_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Copy();
                        return(0);
                    case MENU_CONTEXT_COPYFILEPATHNAME_ID + MENU_CONTEXT_IDBASE:
                        MCMD_CopyFileNamePath();
                        return(0);
                    case MENU_CONTEXT_COPYENTIRETEXT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_CopyEntireText();
                        return(0);
                    case MENU_CONTEXT_PASTE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Paste();
                        return(0);
                    case MENU_CONTEXT_PASTENEWWINDOW_ID + MENU_CONTEXT_IDBASE:
                        MCMD_PasteNewWindow();
                        return(0);
                    case MENU_CONTEXT_SELECTLINE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_SelectLine();
                        return(0);
                    case MENU_CONTEXT_SELECTALL_ID + MENU_CONTEXT_IDBASE:
                        MCMD_SelectAll();
                        return(0);
                    case MENU_CONTEXT_SELECTPROCEDURE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_SelectProc();
                        return(0);
                    case MENU_CONTEXT_SELECTPROCEDUREANDCOLLAPSE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_SelectProcCollapse();
                        return(0);
                    case MENU_CONTEXT_UNCOLLAPSEPROCEDURE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_UnCollapse();
                        return(0);
                    case MENU_CONTEXT_FIND_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Find();
                        return(0);
                    case MENU_CONTEXT_REPLACE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Replace();
                        return(0);
                    case MENU_CONTEXT_BOOKMARKS_TOGGLE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_ToggleBookMark();
                        return(0);
                    case MENU_CONTEXT_BOOKMARKS_FIRST_ID + MENU_CONTEXT_IDBASE:
                        MCMD_FirstBookMark();
                        return(0);
                    case MENU_CONTEXT_BOOKMARKS_PREVIOUS_ID + MENU_CONTEXT_IDBASE:
                        MCMD_PreviousBookMark();
                        return(0);
                    case MENU_CONTEXT_BOOKMARKS_NEXT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_NextBookMark();
                        return(0);
                    case MENU_CONTEXT_BOOKMARKS_LAST_ID + MENU_CONTEXT_IDBASE:
                        MCMD_LastBookMark();
                        return(0);
                    case MENU_CONTEXT_BOOKMARKS_CLEARALL_ID + MENU_CONTEXT_IDBASE:
                        MCMD_ClearAllBookMark();
                        return(0);
                    case MENU_CONTEXT_BREAKPOINTS_TOGGLE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_ToggleBreakpoint();
                        return(0);
                    case MENU_CONTEXT_BREAKPOINTS_FIRST_ID + MENU_CONTEXT_IDBASE:
                        MCMD_FirstBreakpoint();
                        return(0);
                    case MENU_CONTEXT_BREAKPOINTS_PREVIOUS_ID + MENU_CONTEXT_IDBASE:
                        MCMD_PreviousBreakpoint();
                        return(0);
                    case MENU_CONTEXT_BREAKPOINTS_NEXT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_NextBreakpoint();
                        return(0);
                    case MENU_CONTEXT_BREAKPOINTS_LAST_ID + MENU_CONTEXT_IDBASE:
                        MCMD_LastBreakpoint();
                        return(0);
                    case MENU_CONTEXT_BREAKPOINTS_CLEARALL_ID + MENU_CONTEXT_IDBASE:
                        MCMD_ClearAllBreakpoint();
                        return(0);
                    case MENU_CONTEXT_GOTO_TOP_ID + MENU_CONTEXT_IDBASE:
                        MCMD_GotoTop();
                        return(0);
                    case MENU_CONTEXT_GOTO_BOTTOM_ID + MENU_CONTEXT_IDBASE:
                        MCMD_GotoBottom();
                        return(0);
                    case MENU_CONTEXT_GOTO_LINE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_GotoLine();
                        return(0);
                    case MENU_CONTEXT_GOTO_MATCHINGBRACKET_ID + MENU_CONTEXT_IDBASE:
                        MCMD_GotoMatchingBracket();
                        return(0);
                    case MENU_CONTEXT_GOTO_PREVIOUSWORD_ID + MENU_CONTEXT_IDBASE:
                        MCMD_PrevWord();
                        return(0);
                    case MENU_CONTEXT_GOTO_NEXTWORD_ID + MENU_CONTEXT_IDBASE:
                        MCMD_NextWord();
                        return(0);
                    case MENU_CONTEXT_GOTO_PREVIOUSPROCEDURE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_PreviousProc();
                        return(0);
                    case MENU_CONTEXT_GOTO_NEXTPROCEDURE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_NextProc();
                        return(0);
                    case MENU_CONTEXT_GOTO_PREVIOUSPARAGRAPH_ID + MENU_CONTEXT_IDBASE:
                        MCMD_PreviousParagraph();
                        return(0);
                    case MENU_CONTEXT_GOTO_NEXTPARAGRAPH_ID + MENU_CONTEXT_IDBASE:
                        MCMD_NextParagraph();
                        return(0);
                    case MENU_CONTEXT_GOTO_VARIABLEDECLARATION_ID + MENU_CONTEXT_IDBASE:
                        MCMD_VarDeclaration();
                        return(0);
                    case MENU_CONTEXT_GOTO_VARIABLENEXTUSE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_VarNextUse();
                        return(0);
                    case MENU_CONTEXT_GOTO_PROCEDUREPROTOTYPE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_ProcProto();
                        return(0);
                    case MENU_CONTEXT_GOTO_PROCEDUREENTRYPOINT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_ProcDeclare();
                        return(0);
                    case MENU_CONTEXT_GOTO_PROCEDURENEXTUSE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_ProcNextUse();
                        return(0);
                    case MENU_CONTEXT_BLOCK_INDENT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockIndent();
                        return(0);
                    case MENU_CONTEXT_BLOCK_UNINDENT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockUnindent();
                        return(0);
                    case MENU_CONTEXT_BLOCK_UPPERCASE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockUpperCase();
                        return(0);
                    case MENU_CONTEXT_BLOCK_LOWERCASE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockLowerCase();
                        return(0);
                    case MENU_CONTEXT_BLOCK_TOGGLECASE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockToggleCase();
                        return(0);
                    case MENU_CONTEXT_BLOCK_COMMENT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockComment();
                        return(0);
                    case MENU_CONTEXT_BLOCK_UNCOMMENT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockUnComment();
                        return(0);
                    case MENU_CONTEXT_BLOCK_TABIFY_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockTabify();
                        return(0);
                    case MENU_CONTEXT_BLOCK_UNTABIFY_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockUnTabify();
                        return(0);
                    case MENU_CONTEXT_BLOCK_SPACESTOTABS_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockSpacesToTabs();
                        return(0);
                    case MENU_CONTEXT_BLOCK_REGISTERS_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Registers();
                        return(0);
                    case MENU_CONTEXT_BLOCK_BLKCOMMENT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockCommentMultiLine();
                        return(0);
                    case MENU_CONTEXT_BLOCK_CUSTOMCOMMENT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_CustomBlockComment();
                        return(0);
                    case MENU_CONTEXT_BLOCK_TRIMSPACES_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockTrimSpaces();
                        return(0);
                    case MENU_CONTEXT_BLOCK_CONVERTTOINCLUDE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockConvertToInclude();
                        return(0);
                    case MENU_CONTEXT_BLOCK_SAVEAS_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockSave();
                        return(0);
                    case MENU_CONTEXT_BLOCK_PRINT_ID + MENU_CONTEXT_IDBASE:
                        MCMD_BlockPrint();
                        return(0);
                    case MENU_CONTEXT_NEW_ID + MENU_CONTEXT_IDBASE:
                        MCMD_New();
                        return(0);
                    case MENU_CONTEXT_INCLUDEFILE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_IncludeFile();
                        return(0);
                    case MENU_CONTEXT_INCLUDEFILTERS_ID + MENU_CONTEXT_IDBASE:
                        MCMD_IncludeFilters();
                        return(0);
                    case MENU_CONTEXT_DUPLICATEFILE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Duplicate();
                        return(0);
                    case MENU_CONTEXT_RELOADFILE_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Reload();
                        return(0);
                    case MENU_CONTEXT_PROPERTIES_ID + MENU_CONTEXT_IDBASE:
                        MCMD_Properties();
                        return(0);
                }
                if(wParamCMD >= MENU_SNIPPET_IDBASE)
                {
                    if(wParam < (MENU_SNIPPET_IDBASE + 1000))
                    {
                        SnippetVal = SnipArray.Get(wParamCMD - MENU_SNIPPET_IDBASE)->Content;
                        if(SnippetVal.Len() != 0)
                        {
                            SnippetVal = ChangeRelativePaths(SnippetVal);
                            InsertSnippetCode(CurrentForm, SnippetVal);
                        }
                        return(0);
                    }
                    else
                    {
                        if(wParam >= MENU_SCRIPTINSERT_IDBASE)
                        {
                            Script_FileName = LUAArray.Get(wParamCMD - MENU_SCRIPTINSERT_IDBASE)->Content;
                            if(Script_FileName.Extract_FileName().Len() == 0)
                            {
                                WriteToStatus("Empty script filename.");
                                return(0);
                            }
                            // Get the size
                            ScriptFile_Size = FileGetSize(Script_FileName);
                            if(ScriptFile_Size == 0)
                            {
                                WriteToStatus("Empty script file.");
                                return(0);
                            }
                            // Read the file
                            Script_File = FileLoadIntoMem(Script_FileName, &ScriptFile_Size_Out);
                            if(Script_File == NULL)
                            {
                                WriteToStatus("Error while reading script file.");
                                return(0);
                            }
                            if(strlen((char *) Script_File) != 0) InsertScriptCode(Script_FileName.Get_String(), (char *) Script_File);
                            free(Script_File);
                        }
                        return(0);
                    }
                }
                else
                {
                    if((wParam & 0xFFFF) >= MENU_FILTERS_INCLUDE_IDBASE)
                    {
                        // Filters: include mode
                        Run_Filter_Profile((wParam & 0xFFFF) - MENU_FILTERS_INCLUDE_IDBASE, FILTER_INCLUDE_MODE);
                    }
                }
            }
            break;
        case WM_HSCROLL:
            break;
        case WM_VSCROLL:
            break;
        case WM_KEYUP:
            if(wParam == 45)
            {
                RefreshSBStat = 1;
                SetStatusInsert(hWnd);
            }
            if(APITTip == 1)
            {
                switch(wParam)
                {
                    case 188:
                        // Check for ","
                        CurrentCMLine = CMGetCurrentLine(hWnd, GetCurrentLineNumber(hWnd));
                        CompleteCMLine = CurrentCMLine;
                        // Check if it is an invoke line
                        if(CurrentCMLine.In_Str(1, "invoke", Text_Compare) != 0)
                        {
                            CurrentCMLine = CurrentCMLine.Trim();
                            CurrentCMLine = StringReplace(CurrentCMLine, "\t", " ", 1, -1, Binary_Compare);
                            PosInvoke = CurrentCMLine.In_Str(1, "invoke ", Text_Compare);
                            if(PosInvoke != 0)
                            {
                                if((GetCurrentColNumber(hWnd) + 1) > (PosInvoke + 7))
                                {
                                    CurrentCMLine = CurrentCMLine.Trim();
                                    CurrentCMLine = StringReplace(CurrentCMLine, " ", "", 1, -1, Binary_Compare);
                                    CurrentCMLine = RemoveComments(CurrentCMLine).Trim();
                                    PosInvoke = CurrentCMLine.In_Str(1, "invoke", Text_Compare);
                                    CurrentCMLine = CurrentCMLine.Mid(PosInvoke + 6);
                                    InvokeArray = StringSplit(CurrentCMLine, ",");
                                    if(StringGetSplitUBound(InvokeArray) != -1) DispFnc(hWnd, StringGetSplitElement(CurrentCMLine, InvokeArray, 0), CompleteCMLine, 1, 0);
                                    StringReleaseSplit(InvokeArray);
                                }
                                else
                                {
                                    ControlClose(FRMAPIhwnd);
                                }
                            }
                        }
                        else
                        {
                            if(CurrentCMLine.In_Str(1, "call", Text_Compare) != 0)
                            {
                                CurrentCMLine = CurrentCMLine.Trim();
                                CurrentCMLine = StringReplace(CurrentCMLine, "\t", " ", 1, -1, Binary_Compare);
                                PosInvoke = CurrentCMLine.In_Str(1, "call ", Text_Compare);
                                if(PosInvoke != 0)
                                {
                                    if(GetCurrentColNumber(hWnd) + 1 > (PosInvoke + 5))
                                    {
                                        CurrentCMLine = CurrentCMLine.Trim();
                                        CurrentCMLine = StringReplace(CurrentCMLine, " ", "", 1, -1, Binary_Compare);
                                        CurrentCMLine = RemoveComments(CurrentCMLine).Trim();
                                        PosInvoke = CurrentCMLine.In_Str(1, "call", Text_Compare);
                                        CurrentCMLine = CurrentCMLine.Mid(PosInvoke + 4);
                                        InvokeArray = StringSplit(CurrentCMLine, ",");
                                        if(StringGetSplitUBound(InvokeArray) != -1) DispFnc(hWnd, StringGetSplitElement(CurrentCMLine, InvokeArray, 0), CompleteCMLine, 1, 0);
                                        StringReleaseSplit(InvokeArray);
                                    }
                                    else
                                    {
                                        ControlClose(FRMAPIhwnd);
                                    }
                                }
                            }
                            else
                            {
                                CurrentCMLine = CMGetCurrentLine(hWnd, GetCurrentLineNumber(hWnd));
                                CompleteCMLine = CurrentCMLine;
                                CurrentCMLine = StringReplace(CurrentCMLine, "\t", " ", 1, -1, Binary_Compare);
                                CurFncPos = GetCurrentColNumber(hWnd) - 1;
                                GetFncWord = "";
                                BracketBalance = 0;
                                for(i = CurFncPos; i >= 1; i--)
                                {
                                    ExprCharToTest = CurrentCMLine.Mid(i, 1);
                                    if(StringIsOpenBracketChar(ExprCharToTest)) BracketBalance--;
                                    if(StringIsCloseBracketChar(ExprCharToTest)) BracketBalance++;
                                    CurFncPos--;
                                    if(BracketBalance == -1) break;
                                }
                                // Trim it before checking
                                for(i = CurFncPos; i >= 1; i--)
                                {
                                    if(StringIsLabelChar(CurrentCMLine.Mid(i, 1).Get_String()) == 0) CurFncPos--;
                                    else break;
                                }
                                for(i = CurFncPos; i >= 1; i--)
                                {
                                    CurFncChar = CurrentCMLine.Mid(i, 1);
                                    if(StringIsLabelChar(CurFncChar) == 0) break;
                                    GetFncWord = CurFncChar + GetFncWord;
                                }
                                DispFnc(hWnd, GetFncWord, CompleteCMLine, 1, 1);
                            }
                        }
                        break;
                    case 53:
                        // Check for "("
                        CurrentCMLine = CMGetCurrentLine(hWnd, GetCurrentLineNumber(hWnd));
                        CompleteCMLine = CurrentCMLine;
                        CurrentCMLine = StringReplace(CurrentCMLine, "\t", " ", 1, -1, Binary_Compare);
                        CurFncPos = GetCurrentColNumber(hWnd) - 1;
                        GetFncWord = "";
                        // Trim it before checking
                        for(i = CurFncPos; i >= 1; i--)
                        {
                            if(StringIsLabelChar(CurrentCMLine.Mid(i, 1).Get_String()) == 0) CurFncPos--;
                            else break;
                        }
                        for(i = CurFncPos; i >= 1; i--)
                        {
                            CurFncChar = CurrentCMLine.Mid(i, 1);
                            if(StringIsLabelChar(CurFncChar) == 0) break;
                            GetFncWord = CurFncChar + GetFncWord;
                        }
                        DispFnc(hWnd, GetFncWord, CompleteCMLine, 1, 1);
                        break;
                }
            }
            break;
        case WM_KEYDOWN:
            if(APITTip == 1) if(APIMode == 1) if(wParam == 27) ControlClose(FRMAPIhwnd);
            if(APIListMode == 1) if(wParam == 27) ControlClose(FRMAPIListhWnd);
            if(CursorDisabled == 0)
            {
                // Only disable it within codemax region
                if(ControlGetHwndFromPoint() == hWnd)
                {
                    // Store the coordinates
                    GetCursorPos(&Mouse_Coords);
                    HideCursor();
                }
            }
            break;
        case WM_NCMOUSEMOVE:
            if(CursorDisabled) DisplayCursor();
            break;
        case WM_MOUSEMOVE:
            if(CursorDisabled == 1)
            {
                GetCursorPos(&New_Mouse_Coords);
                if(Mouse_Coords.x != New_Mouse_Coords.x || Mouse_Coords.y != New_Mouse_Coords.y)
                {
                    DisplayCursor();
                }
            }
            break;
        case WM_LBUTTONDOWN:
            if(CursorDisabled) DisplayCursor();
            break;
        case WM_LBUTTONUP:
            if(CursorDisabled) DisplayCursor();
            break;
        case WM_RBUTTONDOWN:
            if(CursorDisabled) DisplayCursor();
            break;
        case WM_RBUTTONUP:
            if(CursorDisabled) DisplayCursor();
            break;
    }
    ChildStruct = LoadStructure(GetParent(hWnd));
    if(ChildStruct->OldCodeMaxHook == NULL) return(0);
    return(CallWindowProc((WNDPROC) ChildStruct->OldCodeMaxHook, hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Insert the selected snippet code --- '
void InsertSnippetCode(HWND hWnd, CStr FileName)
{
    if(hWnd == 0) return;
    IncludeFile(hWnd, FileName);
}

// -----------------------------------------------------------------------
// Insert or replace a text in a child window
void SmartInsert(HWND hWnd, CStr Txt)
{
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) != 0)
    {
        if(Txt.Len() != 0) CM_ReplaceSel(ChildStruct->hChildCodeMax, Txt.Get_String());
    }
    else
    {
        if(Txt.Len() != 0) CM_InsertText(ChildStruct->hChildCodeMax, Txt.Get_String(), &CodeMaxCurRange);
    }
    CodeMaxCurRange.posStart.nCol = CodeMaxCurRange.posStart.nCol + Txt.Len();
    CodeMaxCurRange.posEnd.nLine = CodeMaxCurRange.posStart.nLine;
    CodeMaxCurRange.posEnd.nCol = CodeMaxCurRange.posStart.nCol;
    CM_SetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
}

// -----------------------------------------------------------------------
// Insert or replace a text in a child window and jump to next line
void SmartInsertLine(HWND hWnd, CStr Txt)
{
    CStr BufString;

    BufString = Txt;
    BufString = BufString + "\r\n";
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) != 0)
    {
        CM_ReplaceSel(ChildStruct->hChildCodeMax, BufString.Get_String());
    }
    else
    {
        CM_InsertText(ChildStruct->hChildCodeMax, BufString.Get_String(), &CodeMaxCurRange);
    }
    CodeMaxCurRange.posStart.nCol = 0;
    CodeMaxCurRange.posEnd.nCol = 0;
    CodeMaxCurRange.posEnd.nLine = CodeMaxCurRange.posStart.nLine + 1;
    CodeMaxCurRange.posStart.nLine = CodeMaxCurRange.posStart.nLine + 1;
    CM_SetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
}

// -----------------------------------------------------------------------
// Insert or replace a file in a child window
void SmartInsertFile(HWND hWnd, long File)
{
    long LnNumberPasted = 0;
    
    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) != 0)
    {
        CM_ReplaceSelLng(ChildStruct->hChildCodeMax, File);
    }
    else
    {
        CM_InsertTextLng(ChildStruct->hChildCodeMax, File, &CodeMaxCurRange);
    }
    LnNumberPasted = CountFileLines((char *) File, strlen((char *) File));
    CodeMaxCurRange.posStart.nLine = CodeMaxCurRange.posStart.nLine + LnNumberPasted - 1;
    CodeMaxCurRange.posStart.nCol = 0;
    CodeMaxCurRange.posEnd.nLine = CodeMaxCurRange.posStart.nLine;
    CodeMaxCurRange.posEnd.nCol = CodeMaxCurRange.posStart.nCol;
    CM_SetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
}

// -----------------------------------------------------------------------
// Set the text of a codemax control
void SendTextToChild(HWND hWnd, CStr TxtToSet)
{
    ChildStruct = LoadStructure(hWnd);
    CM_SetText(ChildStruct->hChildCodeMax, TxtToSet.Get_String());
}

// -----------------------------------------------------------------------
// Refresh statusbar
void RefreshStatusBar(HWND hWnd)
{
    RefreshSBStat = 1;
    ChildStruct = LoadStructure(hWnd);
    SetStatusBarParts();
    WritePositionInStatus(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Set file flag for a child window
void ForceChildFile(HWND hWnd)
{
    CStr BufString;
    
    ChildStruct = LoadStructure(hWnd);
    ChildStruct->ModifFlag = 1;
    ChildStruct->FileLoaded = 1;
    BufString = ControlGetText(hWnd);
    BufString = BufString + " *";
    ControlSetText(hWnd, BufString);
}

// -----------------------------------------------------------------------
// Set modified flag for a child window
void ForceChildModified(HWND hWnd)
{
    CStr BufString;

    ChildStruct = LoadStructure(hWnd);
    if(ChildStruct->ModifFlag == 1) return;
    ChildStruct->ModifFlag = 1;
    BufString = ControlGetText(hWnd);
    BufString = BufString + " *";
    ControlSetText(hWnd, BufString);
}

// -----------------------------------------------------------------------
// Display the highlight line
void SetHighLine(HWND hWnd)
{
    switch(ShowH)
    {
        case 1:
            LoadCurrentSel(hWnd);
            CM_SetHighlightedLine(hWnd, CodeMaxCurRange.posStart.nLine);
            break;
        case 0:
            CM_SetHighlightedLine(hWnd, -1);
            break;
    }
}

// -----------------------------------------------------------------------
// Make caret always visible
void ConfineCaret(HWND hWnd)
{
    long ViCount = 0;
    long ViTop = 0;

    LoadCurrentSel(hWnd);
    CodeMaxCurRange.posStart.nLine = CodeMaxCurRange.posEnd.nLine;
    CodeMaxCurRange.posStart.nCol = CodeMaxCurRange.posEnd.nCol;
    ViCount = CM_GetVisibleLineCount(hWnd, CM_GetCurrentView(hWnd), 1);
    ViTop = CM_GetTopIndex(hWnd, CM_GetCurrentView(hWnd));
    if(CodeMaxCurRange.posStart.nLine < ViTop)
    {
        CodeMaxCurRange.posStart.nLine = ViTop;
        CodeMaxCurRange.posEnd.nLine = ViTop;
        CM_SetSel(hWnd, &CodeMaxCurRange, 1);
        return;
    }
    if(CodeMaxCurRange.posStart.nLine >= (ViTop + ViCount))
    {
        CodeMaxCurRange.posStart.nLine = ViTop + ViCount - 1;
        CodeMaxCurRange.posEnd.nLine = ViTop + ViCount - 1;
        CM_SetSel(hWnd, &CodeMaxCurRange, 1);
        return;
    }
}

// -----------------------------------------------------------------------
// Define the current language of the codemax control
void SetCodeMaxLanguage(HWND hWnd)
{
    ChildStruct = LoadStructure(GetParent(hWnd));
    // Store assembler language definition
    LanguageToReg = ChildStruct->Language->Left(ChildStruct->Language->Len());
    CM_SetLanguage(hWnd, LanguageToReg.Get_String());
}

// -----------------------------------------------------------------------
// Update the last position arrays()
void StoreLastPosInChild(HWND hWnd)
{
    int i = 0;
    CStr FileToCheckPos;
    
    ChildStruct = LoadStructure(hWnd);
    if(ChildStruct->FileLoaded != 0)
    {
        FileToCheckPos = CMGetRealFile(ChildStruct->RFile);
        for(i = 0; i < LastOpened.Amount(); i++)
        {
            if(_strcmpi(FileToCheckPos.Get_String(), LastOpened.Get(i)->Content) == 0)
            {
                LoadCurrentSel(ChildStruct->hChildCodeMax);
                LastOpenedLine.Set(i, CodeMaxCurRange.posStart.nLine);
                LastOpenedCol.Set(i, CodeMaxCurRange.posStart.nCol);
                LastOpenedFirst.Set(i, GetCurrentTopIndex(ChildStruct->hChildCodeMax));
                break;
            }
        }
    }
}

// -----------------------------------------------------------------------
// Read snippets datas and fill the menu
void CreateSnippetMenu(HMENU hsMenu, HWND hWnd)
{
    int i = 0;
    int k = 0;
    CStr RetVal2;
    CStr TempLang;
    CStr TempLang2;
    CStr BufString;
    CStr CurrentLanguage;
    
    CurrentLanguage = GetCMLangNameByChild(hWnd);

    SnipArray.Erase();
    k = 0;
    for(i = 0; i <= 999; i++)
    {
        ChildRetVal = IniReadKey("Snippets", "SnippetName" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        RetVal2 = IniReadKey("Snippets", "SnippetFile" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(ChildRetVal.Len() == 0) break;
        if(RetVal2.Len() == 0) break;
        RetVal2 = ChangeRelativePaths(RetVal2);
        // Check the file
        if(FileExist(RetVal2) != 0)
        {
            // Get the language
            TempLang = GetLanguageToOpen(RetVal2);
            if(TempLang.Len() == 0)
            {
                // If the language can't be guessed: always display the snippet
                TempLang = "Unknown";
                TempLang2 = CurrentLanguage;
            }
            else
            {
                TempLang2 = TempLang;
            }
            // Only add snippet into menu if guessed language is the same
            if(lstrcmpi(CurrentLanguage.Get_String(), TempLang2.Get_String()) == 0)
            {
                BufString = ChildRetVal;
                MenuAddString(hsMenu, BufString, k + MENU_SNIPPET_IDBASE, TRUE);
                SnipArray.Add(RetVal2.Get_String());
                k++;
            }
        }
    }
}

// -----------------------------------------------------------------------
// Read templates datas and fill the menu
void CreateTemplateMenu(HMENU hsMenu)
{
    int i = 0;
    int k = 0;
    CStr RetVal2;
    long SnippetNumber = 0;
    CStr TempLang;
    CStr BufString;

    TemplatesArray.Erase();
    k = 0;
    for(i = 0; i <= 999; i++)
    {
        ChildRetVal = IniReadKey("Templates", "TemplateName" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        RetVal2 = IniReadKey("Templates", "TemplateFile" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
        if(ChildRetVal.Len() == 0) break;
        if(RetVal2.Len() == 0) break;
        RetVal2 = ChangeRelativePaths(RetVal2);
        // Check the file
        if(FileExist(RetVal2) != 0)
        {
            // Add a separator at first place
            if(SnippetNumber == 0) MenuAddSeparator(hsMenu);
            SnippetNumber = 1;
            // Get the language
            TempLang = GetLanguageToOpen(RetVal2);
            if(TempLang.Len() == 0) TempLang = "Unknown";
            // Add it in menu
            BufString = ChildRetVal + (CStr) " (" + (CStr) TempLang + (CStr) ")";
            MenuAddString(hsMenu, BufString, k + MENU_TEMPLATES_IDBASE, TRUE);
            TemplatesArray.Add(RetVal2.Get_String());
            k++;
        }
    }
}

// -----------------------------------------------------------------------
// Display the generated snippets menu
void DisplaySubContextMenu(HWND hWnd, HMENU hsMenu)
{
    long SMenuTop = 0;
    long SMenuLeft = 0;
    RECT myPopRect;
    RECT ClRect;
    RECT ClRect2;
    POINT SPoint;
    CM_POSITION CurPos;

    ChildStruct = LoadStructure(hWnd);
    GetWindowRect(ChildStruct->hChildCodeMax, &ClRect);
    SMenuTop = ClRect.top;
    SMenuLeft = ClRect.left;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    CurPos.nCol = CodeMaxCurRange.posStart.nCol;
    CurPos.nLine = CodeMaxCurRange.posStart.nLine;
    GetCaretPos(&SPoint);
    CM_PosFromChar(ChildStruct->hChildCodeMax, &CurPos, &ClRect2);
    SMenuTop = SMenuTop + SPoint.y;
    SMenuLeft = SMenuLeft + ClRect2.left;
    TrackPopupMenu(hsMenu, TPM_LEFTALIGN + TPM_LEFTBUTTON, SMenuLeft, SMenuTop, 0, ChildStruct->hChildCodeMax, &myPopRect);
}

// -----------------------------------------------------------------------
// Read scripts and fill the menu
long CreateLangScripts(HMENU hsMenu)
{
    int i = 0;
    int k = 0;
    CStr RetVal2;
    CStr LUAFile;
    CStr LUALanguageToOpen;
    long ReturnValue = FALSE;

    LUAArray.Erase();
    LUALanguageToOpen = GetCMLangFileName(CurrentForm);
    if(LUALanguageToOpen.Len() != 0)
    {
        for(i = 0; i <= 999; i++)
        {
            ChildRetVal = IniReadKey("Scripts", "ScriptName" + (CStr) StringNumberComplement(i, 3).Get_String(), LUALanguageToOpen);
            if(ChildRetVal == "-")
            {
                MenuAddSeparator(hsMenu);
                ReturnValue = FALSE;
            }
            else
            {
                RetVal2 = IniReadKey("Scripts", "ScriptData" + (CStr) StringNumberComplement(i, 3).Get_String(), LUALanguageToOpen);
                // Script filename
                if(RetVal2.Len() == 0) break;
                // Script label
                if(ChildRetVal.Len() == 0) break;
                RetVal2 = ChangeRelativePaths(RetVal2);
                MenuAddString(hsMenu, ChildRetVal, k + MENU_SCRIPTINSERT_IDBASE, TRUE);
                ReturnValue = TRUE;
                LUAArray.Add(RetVal2.Get_String());
                k++;
            }
        }
    }
    // draw the separator bar
    if(ReturnValue) MenuAddSeparator(hsMenu);
    // Add the editor entry
    MenuAddString(hsMenu, "Edit scripts list", k + MENU_SCRIPTINSERT_IDBASE, TRUE);
    if(ReturnValue) i++;
    MenuSetDefaultItem(hsMenu, i);
    MenuEnable(hsMenu, i, FALSE);
    return(TRUE);
}

// -----------------------------------------------------------------------
// Convert current decimal word into hexadecimal
void CMDecToHex(HWND hWnd)
{
    CStr LocCurWord;
    CStr BufString;

    ChildStruct = LoadStructure(GetParent(hWnd));
    LocCurWord = SelectCurrentWord(ChildStruct->hChildCodeMax);
    LocCurWord = DecToHex(hWnd, LocCurWord);
    if (LocCurWord.Len() != 0)
    {
        BufString = GetCMLangHexType(GetParent(hWnd), 1);
        if(BufString.Len() == 0)
        {
            WriteToStatus("Number format not supplied for this language.");
        }
        else
        {
            BufString = StringReplace(BufString, "%1", LocCurWord, 1, -1, Binary_Compare);
            CM_ReplaceText(ChildStruct->hChildCodeMax, BufString.Get_String(), &CodeMaxCurRange);
        }
    }
}

// -----------------------------------------------------------------------
// Convert current decimal word into binary
void CMDecToBin(HWND hWnd)
{
    CStr LocCurWord;
    CStr BufString;

    ChildStruct = LoadStructure(GetParent(hWnd));
    LocCurWord = SelectCurrentWord(ChildStruct->hChildCodeMax);
    LocCurWord = DecToBin(hWnd, LocCurWord);
    if(LocCurWord.Len() != 0)
    {
        BufString = GetCMLangBinType(GetParent(hWnd), 1);
        if(BufString.Len() == 0)
        {
            WriteToStatus("Number format not supplied for this language.");
        }
        else
        {
            BufString = StringReplace(BufString, "%1", LocCurWord, 1, -1, Binary_Compare);
            CM_ReplaceText(ChildStruct->hChildCodeMax, BufString.Get_String(), &CodeMaxCurRange);
        }
    }
}

// -----------------------------------------------------------------------
// Convert current hexadecimal word into decimal
void CMHexToDec(HWND hWnd)
{
    CStr LocCurWord;

    ChildStruct = LoadStructure(GetParent(hWnd));
    LocCurWord = SelectCurrentWord(ChildStruct->hChildCodeMax);
    LocCurWord = HexToDec(hWnd, LocCurWord);
    if (LocCurWord.Len() != 0) CM_ReplaceText(ChildStruct->hChildCodeMax, LocCurWord.Get_String(), &CodeMaxCurRange);
}

// -----------------------------------------------------------------------
// Convert current hexadecimal word into binary
void CMHexToBin(HWND hWnd)
{
    CStr LocCurWord;
    CStr BufString;

    ChildStruct = LoadStructure(GetParent(hWnd));
    LocCurWord = SelectCurrentWord(ChildStruct->hChildCodeMax);
    LocCurWord = HexToBin(hWnd, LocCurWord);
    if(LocCurWord.Len() != 0)
    {
        BufString = GetCMLangBinType(GetParent(hWnd), 1);
        if(BufString.Len() == 0)
        {
            WriteToStatus("Number format not supplied for this language.");
        }
        else
        {
            BufString = StringReplace(BufString, "%1", LocCurWord, 1, -1, Binary_Compare);
            CM_ReplaceText(ChildStruct->hChildCodeMax, BufString.Get_String(), &CodeMaxCurRange);
        }
    }
}

// -----------------------------------------------------------------------
// Convert current binary word into decimal
void CMBinToDec(HWND hWnd)
{
    CStr LocCurWord;

    ChildStruct = LoadStructure(GetParent(hWnd));
    LocCurWord = SelectCurrentWord(ChildStruct->hChildCodeMax);
    LocCurWord = BinToDec(hWnd, LocCurWord);
    if(LocCurWord.Len() != 0) CM_ReplaceText(ChildStruct->hChildCodeMax, LocCurWord.Get_String(), &CodeMaxCurRange);
}

// -----------------------------------------------------------------------
// Convert current binary word into hexadecimal
void CMBinToHex(HWND hWnd)
{
    CStr LocCurWord;
    CStr BufString;

    ChildStruct = LoadStructure(GetParent(hWnd));
    LocCurWord = SelectCurrentWord(ChildStruct->hChildCodeMax);
    LocCurWord = BinToHex(hWnd, LocCurWord);
    if(LocCurWord.Len() != 0)
    {
        BufString = GetCMLangHexType(GetParent(hWnd), 1);
        if(BufString.Len() == 0)
        {
            WriteToStatus("Number format not supplied for this language.");
        }
        else
        {
            BufString = StringReplace(BufString, "%1", LocCurWord, 1, 1, Binary_Compare);
            CM_ReplaceText(ChildStruct->hChildCodeMax, BufString.Get_String(), &CodeMaxCurRange);
        }
    }
}

// -----------------------------------------------------------------------
// Convert current hexadecimal word into a floating point
void CMHexToFlt(HWND hWnd)
{
    CStr LocCurWord;
    CStr BufString;

    ChildStruct = LoadStructure(GetParent(hWnd));
    LocCurWord = SelectCurrentWord(ChildStruct->hChildCodeMax);
    LocCurWord = HexToFlt(hWnd, LocCurWord);
    if (LocCurWord.Len() != 0) CM_ReplaceText(ChildStruct->hChildCodeMax, LocCurWord.Get_String(), &CodeMaxCurRange);
}

// -----------------------------------------------------------------------
// Convert current floating point word into hexadecimal
void CMFltToHex(HWND hWnd)
{
    CStr LocCurWord;
    CStr BufString;

    ChildStruct = LoadStructure(GetParent(hWnd));
    LocCurWord = SelectCurrentWord(ChildStruct->hChildCodeMax);
    LocCurWord = FltToHex(hWnd, LocCurWord);
    if(LocCurWord.Len() != 0)
    {
        BufString = GetCMLangHexType(GetParent(hWnd), 1);
        if(BufString.Len() == 0)
        {
            WriteToStatus("Number format not supplied for this language.");
        }
        else
        {
            BufString = StringReplace(BufString, "%1", LocCurWord, 1, -1, Binary_Compare);
            CM_ReplaceText(ChildStruct->hChildCodeMax, BufString.Get_String(), &CodeMaxCurRange);
        }
    }
}

// -----------------------------------------------------------------------
// Search and display an API function
void DispFnc(HWND hWnd, CStr FnctoShow, CStr CompleteLine, long SetHighLight, long CStyle)
{
    long APILeft = 0;
    long APITop = 0;
    RECT APIRect;
    RECT APIRect2;
    POINT APIPoint;
    CM_POSITION CurPos;
    long OldLn = 0;
    long OldCol = 0;
    CStr TmpAPIName;
    long TmpAPIMode = 0;

    if(FnctoShow.Len() == 0) return;
    ChildStruct = LoadStructure(GetParent(hWnd));
    // Don't display it again
    if(APIMode == 1) if(ChildStruct->oldAPILine == GetCurrentLineNumber(hWnd)) if(_strcmpi(CMGetRealFile(ChildStruct->oldAPIFnc).Get_String(), FnctoShow.Get_String()) == 0) return;
    APICurHiglight = 0;
    if(APIMode == 1) ControlClose(FRMAPIhwnd);
    APIFnc = FnctoShow.Get_String();
    GetWindowRect(hWnd, &APIRect);
    APIOldRealPosition = (GetRealYposition(hWnd));
    APITop = APIRect.top + APIOldRealPosition;
    APILeft = APIRect.left;
    CurPos.nCol = GetCurrentColNumber(hWnd);
    CurPos.nLine = GetCurrentLineNumber(hWnd);
    OldLn = CurPos.nLine;
    OldCol = CurPos.nCol;
    GetCaretPos(&APIPoint);
    CM_PosFromChar(hWnd, &CurPos, &APIRect2);
    APITop = APITop + APIPoint.y;
    APILeft = APILeft + APIRect2.left;
    // Highlight current argument
    APICurHiglight = CountComma(CompleteLine, GetCurrentColNumber(hWnd));
    HighPar = 0;
    if(CStyle == 1)
    {
        APICurHiglight++;
        HighPar = 1;
    }
    ChildStruct->oldAPIhighLight = APICurHiglight;
    ChildStruct->oldAPILine = OldLn;
    ChildStruct->oldAPICol = OldCol;
    // Remove the selection list if it is opened
    if(APIListMode == 1) ControlClose(FRMAPIListhWnd);
    // We need to use a temporary var for APIMode
    // to avoid WM_NCACTIVATE message (after setfocus)
    TmpAPIMode = 0;
    TmpAPIName = DisplayAPI(APILeft, APITop);
    if(TmpAPIName.Len() != 0)
    {
        // Save the correct name
        // (later compared with a lstrcmpi so we don't care about the case)
        SaveAPIName(hWnd, &TmpAPIName);
        TmpAPIMode = 1;
    }
    HighOn = SetHighLight;
    if(ControlIsVisible(hWnd) != 0) SetFocus(hWnd);
    // Pass into API mode now
    APIMode = TmpAPIMode;
}

// -----------------------------------------------------------------------
// Save the name of the API function into the current child structure
void SaveAPIName(HWND hwndCM, CStr FncToSave)
{
    ChildStruct = LoadStructure(GetParent(hwndCM));
    ChildStruct->oldAPIFnc->Set_String(FncToSave.Get_String());
}

// -----------------------------------------------------------------------
// Return the real position in a codemax control
long GetRealYposition(HWND hWnd)
{
    long ReturnValue = 0;
    HFONT PosFont = 0;
    CStr TestString;
    SIZE PosSize;
    HGDIOBJ PosOldObject;
    HDC hDC = 0;

    TestString = "|gHq";
    PosFont = GDIObtainFont(CurFontName, CurFontSize, hWnd, 0, 0);
    hDC = GetDC(hWnd);
    PosOldObject = SelectObject(hDC, PosFont);
    GetTextExtentPoint32(hDC, TestString.Get_String(), TestString.Len(), &PosSize);
    ReturnValue = PosSize.cy;
    SelectObject(hDC, PosOldObject);
    DeleteObject(PosFont);
    ReleaseDC(hWnd, hDC);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the number of commas in a string from a position
long CountComma(CStr Line, long XPos)
{
    long ReturnValue = 0;
    int i = 0;

    if(XPos != 0)
    {
        ReturnValue = 0;
        if(XPos > Line.Len()) XPos = Line.Len();
        for(i = XPos; i >= 1; i--)
        {
            if(strcmp(Line.Mid(i, 1).Get_String(), ",") == 0) ReturnValue++;
        }
    }
    ReturnValue--;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Move fnc window in X axis
void MoveFncX(HWND hWnd, HWND DesthWnd)
{
    long APILeft = 0;
    RECT APIRect;
    RECT APIRect2;
    POINT APIPoint;
    CM_POSITION CurPos;
    long ParLeft = 0;
    long ParTop = 0;
    long ParWidth = 0;
    long ParHeight = 0;

    ChildStruct = LoadStructure(hWnd);
    GetWindowRect(ChildStruct->hChildCodeMax, &APIRect);
    APILeft = APIRect.left;
    CurPos.nCol = GetCurrentColNumber(ChildStruct->hChildCodeMax);
    CurPos.nLine = GetCurrentLineNumber(ChildStruct->hChildCodeMax);
    GetCaretPos(&APIPoint);
    CM_PosFromChar(ChildStruct->hChildCodeMax, &CurPos, &APIRect2);
    APILeft = APILeft + APIRect2.left;
    if(APILeft < APIRect.left)
    {
        ControlClose(DesthWnd);
        return;
    }
    if(APILeft > APIRect.right)
    {
        ControlClose(DesthWnd);
        return;
    }
    if((APILeft + APIXDim) > GetSystemMetrics(SM_CXFULLSCREEN)) APILeft = APILeft - APIXDim;
    if(APILeft < 0) APILeft = 0;
    ParLeft = APILeft;
    ParTop = ControlTop(DesthWnd);
    ParWidth = ControlWidth(DesthWnd);
    ParHeight = ControlHeight(DesthWnd);
    ControlRefresh(DesthWnd);
    MoveWindow(DesthWnd, ParLeft, ParTop, ParWidth, ParHeight, 1);
}

// -----------------------------------------------------------------------
// Move fnc window in Y axis
void MoveFncY(HWND hWnd, HWND DesthWnd)
{
    long APITop = 0;
    RECT APIRect;
    RECT APIRect2;
    POINT APIPoint;
    CM_POSITION CurPos;
    long RealY = 0;
    long ParLeft = 0;
    long ParTop = 0;
    long ParWidth = 0;
    long ParHeight = 0;

    ChildStruct = LoadStructure(hWnd);
    // Min Y
    if(GetCurrentTopIndex(ChildStruct->hChildCodeMax) > GetCurrentLineNumber(ChildStruct->hChildCodeMax))
    {
        ControlClose(DesthWnd);
        return;
    }
    // Max Y
    if(GetCurrentTopIndex(ChildStruct->hChildCodeMax) + CM_GetVisibleLineCount(ChildStruct->hChildCodeMax,
                          CM_GetCurrentView(ChildStruct->hChildCodeMax), 1) <= GetCurrentLineNumber(ChildStruct->hChildCodeMax))
    {
        ControlClose(DesthWnd);
        return;
    }
    GetWindowRect(ChildStruct->hChildCodeMax, &APIRect);
    RealY = GetRealYposition(ChildStruct->hChildCodeMax);
    APITop = APIRect.top + RealY;
    CurPos.nCol = GetCurrentColNumber(ChildStruct->hChildCodeMax);
    CurPos.nLine = GetCurrentLineNumber(ChildStruct->hChildCodeMax);
    GetCaretPos(&APIPoint);
    CM_PosFromChar(ChildStruct->hChildCodeMax, &CurPos, &APIRect2);
    APITop = APITop + APIPoint.y;
    if((APITop + APIYDim) > GetSystemMetrics(SM_CYFULLSCREEN)) APITop = APITop - APIYDim - RealY;
    if(APITop < 0) APITop = 0;
    ParLeft = ControlLeft(DesthWnd);
    ParTop = APITop;
    ParWidth = ControlWidth(DesthWnd);
    ParHeight = ControlHeight(DesthWnd);
    ControlRefresh(DesthWnd);
    MoveWindow(DesthWnd, ParLeft, ParTop, ParWidth, ParHeight, 1);
}

// -----------------------------------------------------------------------
// Move fnc window in both XY axises
void MoveFncXY(HWND hWnd, HWND DesthWnd)
{
    long APILeft = 0;
    long APITop = 0;
    RECT APIRect;
    RECT APIRect2;
    POINT APIPoint;
    CM_POSITION CurPos;
    long RealY = 0;
    long ParLeft = 0;
    long ParTop = 0;
    long ParWidth = 0;
    long ParHeight = 0;

    ChildStruct = LoadStructure(hWnd);
    // Min Y
    if(GetCurrentTopIndex(ChildStruct->hChildCodeMax) > GetCurrentLineNumber(ChildStruct->hChildCodeMax))
    {
        ControlClose(DesthWnd);
        return;
    }
    // Max Y
    if(GetCurrentTopIndex(ChildStruct->hChildCodeMax) + CM_GetVisibleLineCount(ChildStruct->hChildCodeMax,
                          CM_GetCurrentView(ChildStruct->hChildCodeMax), 1) <= GetCurrentLineNumber(ChildStruct->hChildCodeMax))
    {
        ControlClose(DesthWnd);
        return;
    }
    GetWindowRect(ChildStruct->hChildCodeMax, &APIRect);
    APILeft = APIRect.left;
    RealY = GetRealYposition(ChildStruct->hChildCodeMax);
    APITop = APIRect.top + RealY;
    CurPos.nCol = GetCurrentColNumber(ChildStruct->hChildCodeMax);
    CurPos.nLine = GetCurrentLineNumber(ChildStruct->hChildCodeMax);
    GetCaretPos(&APIPoint);
    CM_PosFromChar(ChildStruct->hChildCodeMax, &CurPos, &APIRect2);
    APILeft = APILeft + APIRect2.left;
    APITop = APITop + APIPoint.y;
    if(APILeft < APIRect.left)
    {
        ControlClose(DesthWnd);
        return;
    }
    if(APILeft > APIRect.right)
    {
        ControlClose(DesthWnd);
        return;
    }
    if((APILeft + APIXDim) > GetSystemMetrics(SM_CXFULLSCREEN)) APILeft = APILeft - APIXDim;
    if((APITop + APIYDim) > GetSystemMetrics(SM_CYFULLSCREEN)) APITop = APITop - APIYDim - RealY;
    if(APILeft < 0) APILeft = 0;
    if(APITop < 0) APITop = 0;
    ParLeft = APILeft;
    ParTop = APITop;
    ParWidth = ControlWidth(DesthWnd);
    ParHeight = ControlHeight(DesthWnd);
    ControlRefresh(DesthWnd);
    MoveWindow(DesthWnd, ParLeft, ParTop, ParWidth, ParHeight, 1);
}

// -----------------------------------------------------------------------
// Retrieve the real X position of the caret
long WAMMGetCaretRealXCoordinate(HWND hWnd)
{
    long APILeft = 0;
    RECT APIRect;
    CM_POSITION CurPos;
    POINT APIPoint;
    RECT APIRect2;

    GetWindowRect(hWnd, &APIRect);
    APILeft = APIRect.left;
    CurPos.nCol = GetCurrentColNumber(hWnd);
    CurPos.nLine = GetCurrentLineNumber(hWnd);
    GetCaretPos(&APIPoint);
    CM_PosFromChar(hWnd, &CurPos, &APIRect2);
    APILeft = APILeft + APIRect2.left;
    if(APILeft > GetSystemMetrics(SM_CXFULLSCREEN)) APILeft = GetSystemMetrics(SM_CXFULLSCREEN);
    if(APILeft < 0) APILeft = 0;
    return(APILeft);
}

// -----------------------------------------------------------------------
// Retrieve the real Y position of the caret
long WAMMGetCaretRealYCoordinate(HWND hWnd)
{
    long RealY = 0;
    long APITop = 0;
    RECT APIRect;
    CM_POSITION CurPos;
    POINT APIPoint;
    RECT APIRect2;

    GetWindowRect(hWnd, &APIRect);
    RealY = GetRealYposition(hWnd);
    APITop = APIRect.top + RealY;
    CurPos.nCol = GetCurrentColNumber(hWnd);
    CurPos.nLine = GetCurrentLineNumber(hWnd);
    GetCaretPos(&APIPoint);
    CM_PosFromChar(hWnd, &CurPos, &APIRect2);
    APITop = APITop + APIPoint.y;
    if(APITop > GetSystemMetrics(SM_CYFULLSCREEN)) APITop = GetSystemMetrics(SM_CYFULLSCREEN);
    if(APITop < 0) APITop = 0;
    return(APITop);
}

// -----------------------------------------------------------------------
// Check if a line can be corrected and return possible function keyword
CStr GetLeftWord(HWND hWnd, CStr LineToCheck)
{
    int i = 0;
    long CurFncPos = 0;
    long OldCurFncPos = 0;
    CStr CurFncChar;
    CStr ReturnValue;
    CStr BufString;

    CurFncPos = GetCurrentColNumber(hWnd);
    OldCurFncPos = CurFncPos;
    // Trim it before checking
    for(i = CurFncPos; i >= 1; i++)
    {
        if(StringIsBlankChar(LineToCheck.Mid(i, 1)) == 0) break;
        CurFncPos--;
    }
    // Fill it to the left
    for(i = CurFncPos; i >= 1; i--)
    {
        CurFncChar = LineToCheck.Mid(i, 1);
        if(StringIsLabelChar(CurFncChar) == 0) break;
        BufString = CurFncChar + BufString;
    }
    // Fill it to the right
    OldCurFncPos++;
    for(i = OldCurFncPos; i <= 65535; i++)
    {
        if(LineToCheck.Len() < i) break;
        CurFncChar = LineToCheck.Mid(i, 1);
        if(StringIsLabelChar(CurFncChar) == 0) break;
        BufString = BufString + CurFncChar;
    }
    ReturnValue.Set_String(BufString.Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return keyword located left of the current caret position
// (Only called after DisplayAPIList and from APIhListBoxHook)
CStr SelectLeftWord(HWND hWnd, CStr LineToCheck)
{
    int i = 0;
    long CurFncPos = 0;
    long OldCurFncPos = 0;
    CStr CurFncChar;
    CStr ReturnValue;

    CurFncPos = GetCurrentColNumber(hWnd);
    OldCurFncPos = CurFncPos;
    // Trim it before checking
    for(i = CurFncPos; i >= 1; i--)
    {
        if(StringIsBlankChar(LineToCheck.Mid(i, 1)) == 0) break;
        CurFncPos--;
    }
    CMSetCaretPos(hWnd, LineNumberToDisplay, CurFncPos);
    ReturnValue.Set_String(SelectCurrentWord(hWnd).Get_String());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Display the list of APIs in the database
void DisplayAPIList(HWND hWnd)
{
    RECT APIRect;
    long APILeft = 0;
    long APITop = 0;
    CM_POSITION CurPos;
    long OldLn = 0;
    long OldCol = 0;
    POINT APIPoint;
    RECT APIRect2;

    ChildStruct = LoadStructure(hWnd);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    LineNumberToDisplay = GetCurrentLineNumber(ChildStruct->hChildCodeMax);
    LineToDisplay = CMGetCurrentLine(ChildStruct->hChildCodeMax, LineNumberToDisplay);
    // Found a suitable line ?
    LineToDisplay = GetLeftWord(ChildStruct->hChildCodeMax, LineToDisplay);
    // Can be complete/incomplete or empty name
    // Don't display it again
    if(APIMode == 1) ControlClose(FRMAPIhwnd);
    if(APIListMode == 1)
    {
        if(ChildStruct->oldAPILineLine = GetCurrentLineNumber(ChildStruct->hChildCodeMax))
        {
            if(_strcmpi(CMGetRealFile(ChildStruct->oldAPILineFnc).Get_String(), LineToDisplay.Get_String()) == 0)
            {
                return;
            }
        }
    }
	if(APIListMode == 1) ControlClose(FRMAPIListhWnd);
    if(CursorDisabled) DisplayCursor();
    APILineFnc = LineToDisplay;
    GetWindowRect(ChildStruct->hChildCodeMax, &APIRect);
    APILineOldRealPosition = (GetRealYposition(ChildStruct->hChildCodeMax));
    APITop = APIRect.top + APILineOldRealPosition;
    APILeft = APIRect.left;
    CurPos.nCol = GetCurrentColNumber(ChildStruct->hChildCodeMax);
    CurPos.nLine = GetCurrentLineNumber(ChildStruct->hChildCodeMax);
    OldLn = CurPos.nLine;
    OldCol = CurPos.nCol;
    GetCaretPos(&APIPoint);
    CM_PosFromChar(ChildStruct->hChildCodeMax, &CurPos, &APIRect2);
    APITop = APITop + APIPoint.y;
    APILeft = APILeft + APIRect2.left;
    ChildStruct->oldAPILineLine = OldLn;
    ChildStruct->oldAPILineCol = OldCol;
    DisplayAPIWindow(APILeft, APITop, hWnd, LineToDisplay);
    APIListMode = 1;
}

// -----------------------------------------------------------------------
// Notify the child's container that the file title has been modified
void RefreshChildTitle(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    if(ChildStruct->ReadOnly != 0)
    {
        // Read-Only
        SendMessage(hMDIform.hClient, CHILD_MSG_SETTITLE, ICON_WINDOWLOCK, (LPARAM) hWnd);
        return;
    }
    if(ChildStruct->ModifFlag != 0)
    {
        // Modified
        SendMessage(hMDIform.hClient, CHILD_MSG_SETTITLE, ICON_WINDOWMODIF, (LPARAM) hWnd);
    }
    else
    {
        // Unchanged
        SendMessage(hMDIform.hClient, CHILD_MSG_SETTITLE, ICON_WINDOWNORM, (LPARAM) hWnd);
    }
}

// -----------------------------------------------------------------------
// Set the keywords corrector
void SetCorrectionHook(HWND hWnd)
{
    ChildStruct = LoadStructure(GetParent(hWnd));
    if(AutoCorrectKey != 0 && (ChildStruct->Language->Len() != 0))
    {
        CM_SetTokenCallBack(hWnd,(long) &CorrectKeyWordsHook);
    }
    else
    {
        // No suitable language
        CM_SetTokenCallBack(hWnd, 0);
    }
}

int CALLBACK CorrectKeyWordsHook(LPTSTR Token, long Pos, long PosBefore)
{
    char *CurToken;
    CStr CurToken2;
    CStr APICorrected;

    CurToken = Token + Pos;
    CurToken2 = Token;
    CurToken2 = CurToken2.Mid(PosBefore + 1,Pos - PosBefore);
    while(isspace(CurToken2.Left(1).Asc()))
    {
        PosBefore++;
        CurToken2 = CurToken2.Mid(2);
    }
    while(isspace(CurToken2.Right(1).Asc()))
    {
        Pos--;
        CurToken2 = CurToken2.Mid(1,CurToken2.Len() - 1);
    }
    if(CurToken2.Len() != 0)
    {
        // Check the word in APIs (fnc+const) databases
        APICorrected = RetrieveAPIName(CurToken2);
        // Found a suitable keyword ?
        if(APICorrected.Len() != 0)
        {
            CopyMem2(APICorrected.Get_String(), Token, APICorrected.Len(), 0, PosBefore);
        }
    }
    // Unchanged
    return(0);
}

// -----------------------------------------------------------------------
// Return 1 if the source window is in read-only mode
long IsChildReadOnly(HWND hWnd)
{
    if(NbForms == 0) return(0);
    if(hWnd == 0) return(0);
    ChildStruct = LoadStructure(hWnd);
    return(ChildStruct->ReadOnly);
}

// -----------------------------------------------------------------------
// Set the source window in read only mode or not
void SetChildReadOnly(HWND hWnd, int State)
{
    if(NbForms == 0) return;
    if(hWnd == 0) return;
    ChildStruct = LoadStructure(hWnd);
    ChildStruct->ReadOnly = State;
    CM_SetReadOnly(ChildStruct->hChildCodeMax, State);
}
