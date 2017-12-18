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
// MsgCodeMax.cpp: CodeMax handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ConstCodeMax.h"
#include "MsgCodeMax.h"
#include "Globals.h"

// -----------------------------------------------------------------------
// Variables
HMODULE hCodeMax;
void (* hCodeMaxCMRegisterControl) (long, long);
void (* hCodeMaxCMUnregisterControl) (void);
void (* hCodeMaxCMRegisterLanguage) (char *, CM_LANGUAGE *);
void (* hCodeMaxCMUnRegisterLanguage) (char *);
void (* hCodeMaxCMRegisterCommand) (long, char *, char *);
void (* hCodeMaxCMRegisterHotKey) (long, long);

// -----------------------------------------------------------------------
// Init codeMax control
void InitCodeMax(HINSTANCE hInst)
{
    int Cm_Flags = 0;

    hCodeMax = LoadLibrary("CMaxXX.dll");
    if(hCodeMax == 0) return;
    hCodeMaxCMRegisterControl = (void (*) (long, long)) GetProcAddress(hCodeMax, "CMRegisterControl");
    hCodeMaxCMUnregisterControl = (void (*) (void)) GetProcAddress(hCodeMax, "CMUnregisterControl");
    hCodeMaxCMRegisterLanguage = (void (*) (char *, CM_LANGUAGE *)) GetProcAddress(hCodeMax, "CMRegisterLanguage");
    hCodeMaxCMUnRegisterLanguage = (void (*) (char *)) GetProcAddress(hCodeMax, "CMUnregisterLanguage");
    hCodeMaxCMRegisterCommand = (void (*) (long, char *, char *)) GetProcAddress(hCodeMax, "CMRegisterCommand");
    hCodeMaxCMRegisterHotKey = (void (*) (long, long)) GetProcAddress(hCodeMax, "CMRegisterHotKey");
    if(WindowsXP) Cm_Flags |= CM_FLAG_WINDOWSXP;
    hCodeMaxCMRegisterControl(0x2100, Cm_Flags);
}

// -----------------------------------------------------------------------
// Free CodeMax control from memory
void UnInitCodeMax(void)
{
    if(hCodeMaxCMUnregisterControl != 0) hCodeMaxCMUnregisterControl();
    if(hCodeMax != 0) FreeLibrary(hCodeMax);
}

// -----------------------------------------------------------------------
// Create a CodeMax control
HWND CreateCodeMax(long CMLeft, long CMTop, long CMWidth, long CMHeight, HWND hParent, HINSTANCE hInst, long Maximize)
{
    long CodeMaxProps = 0;

    ControlBound(hParent, CMLeft, CMTop, CMWidth, CMHeight);
    CodeMaxProps = WS_TABSTOP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE | WS_CHILD;
    if(Maximize == 1) CodeMaxProps = CodeMaxProps | WS_MAXIMIZE;
    return(CreateWindowEx(WS_EX_STATICEDGE, "CodeMaxXX", "", CodeMaxProps, CMLeft, CMTop, CMWidth, CMHeight, hParent, NULL, hInst, NULL));
}

// -----------------------------------------------------------------------
// Message crackers for all CM_xxxx messages
long CM_SetLanguage(HWND hWnd, char *pszName)
{
    return(SendMessage(hWnd, CMM_SETLANGUAGE, 0, (LPARAM) pszName));
}

long CM_GetLanguage(HWND hWnd, char *pszName)
{
    return(SendMessage(hWnd, CMM_GETLANGUAGE, 0, (LPARAM) pszName));
}

long CM_EnableColorSyntax(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLECOLORSYNTAX, (WPARAM) bEnable, 0));
}

long CM_IsColorSyntaxEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISCOLORSYNTAXENABLED, 0, 0));
}

long CM_OpenFile(HWND hWnd, char *pszFileName)
{
    return(SendMessage(hWnd, CMM_OPENFILE, 0, (LPARAM) pszFileName));
}

long CM_InsertFile(HWND hWnd, char *pszFileName, CM_RANGE *pPos)
{
    return(SendMessage(hWnd, CMM_INSERTFILE, (WPARAM) pPos, (LPARAM) pszFileName));
}

long CM_InsertText(HWND hWnd, char *pszText, CM_RANGE *pPos)
{
    if(strlen(pszText) != 0) return(SendMessage(hWnd, CMM_INSERTTEXT, (WPARAM) pPos, (LPARAM) pszText));
    return(0);
}

long CM_InsertTextLng(HWND hWnd, long pszText, CM_RANGE *pPos)
{
    return(SendMessage(hWnd, CMM_INSERTTEXT, (WPARAM) pPos, (LPARAM) pszText));
}

long CM_SetText(HWND hWnd, char *pszText)
{
    return(SendMessage(hWnd, CMM_SETTEXT, 0, (LPARAM) pszText));
}

long CM_SetTextLng(HWND hWnd, long pszText)
{
    return(SendMessage(hWnd, CMM_SETTEXT, 0, (LPARAM) pszText));
}

long CM_SetColors(HWND hWnd, CM_COLORS *pColors)
{
    return(SendMessage(hWnd, CMM_SETCOLORS, 0, (LPARAM) pColors));
}

long CM_GetColors(HWND hWnd, CM_COLORS *pColors)
{
    return(SendMessage(hWnd, CMM_GETCOLORS, 0, (LPARAM) pColors));
}

long CM_EnableWhitespaceDisplay(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEWHITESPACEDISPLAY, (WPARAM) bEnable, 0));
}

long CM_IsWhitespaceDisplayEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISWHITESPACEDISPLAYENABLED, 0, 0));
}

long CM_EnableTabExpand(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLETABEXPAND, (WPARAM) bEnable, 0));
}

long CM_IsTabExpandEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISTABEXPANDENABLED, 0, 0));
}

long CM_EnableSmoothScrolling(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLESMOOTHSCROLLING, (WPARAM) bEnable, 0));
}

long CM_IsSmoothScrollingEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISSMOOTHSCROLLINGENABLED, 0, 0));
}

long CM_SetTabSize(HWND hWnd, long nTabSize)
{
    return(SendMessage(hWnd, CMM_SETTABSIZE, (WPARAM) nTabSize, 0));
}

long CM_GetTabSize(HWND hWnd) {
    return(SendMessage(hWnd, CMM_GETTABSIZE, 0, 0));
}

long CM_SetReadOnly(HWND hWnd, long bReadOnly)
{
    return(SendMessage(hWnd, CMM_SETREADONLY, (WPARAM) bReadOnly, 0));
}

long CM_IsReadOnly(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISREADONLY, 0, 0));
}

long CM_EnableLineToolTips(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLELINETOOLTIPS, (WPARAM) bEnable, 0));
}

long CM_IsLineToolTipsEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISLINETOOLTIPSENABLED, 0, 0));
}

long CM_EnableLeftMargin(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLELEFTMARGIN, (WPARAM) bEnable, 0));
}

long CM_IsLeftMarginEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISLEFTMARGINENABLED, 0, 0));
}

long CM_SaveFile(HWND hWnd, char *pszFileName, long bClearUndo)
{
    return(SendMessage(hWnd, CMM_SAVEFILE, (WPARAM) bClearUndo, (LPARAM) pszFileName));
}

long CM_ReplaceText(HWND hWnd, char *pszText, CM_RANGE *pRange)
{
    return(SendMessage(hWnd, CMM_REPLACETEXT, (WPARAM) pRange, (LPARAM) pszText));
}

long CM_ReplaceLine(HWND hWnd, char *pszText, long nLine)
{
    return(SendMessage(hWnd, CMM_REPLACELINE, (WPARAM) nLine, (LPARAM) pszText));
}

long CM_ReplaceTextAll(HWND hWnd, char *pszText)
{
    return(SendMessage(hWnd, CMM_REPLACETEXT, 0, (LPARAM) pszText));
}

long CM_GetTextLengthAll(HWND hWnd, long bLogical)
{
    return(SendMessage(hWnd, CMM_GETTEXTLENGTH, 0, (LPARAM) bLogical));
}

long CM_GetTextLength(HWND hWnd, CM_RANGE *pRange, long bLogical)
{
    return(SendMessage(hWnd, CMM_GETTEXTLENGTH, (WPARAM) pRange, (LPARAM) bLogical));
}

long CM_GetText(HWND hWnd, char *pszBuff, CM_RANGE *pRange)
{
    return(SendMessage(hWnd, CMM_GETTEXT, (WPARAM) pRange, (LPARAM) pszBuff));
}

long CM_GetTextLng(HWND hWnd, long pszBuff, CM_RANGE *pRange)
{
    return(SendMessage(hWnd, CMM_GETTEXT, (WPARAM) pRange, (LPARAM) pszBuff));
}

long CM_GetTextAll(HWND hWnd, char *pszBuff)
{
    return(SendMessage(hWnd, CMM_GETTEXT, 0, (LPARAM) pszBuff));
}

long CM_GetTextAllLng(HWND hWnd, long pszBuff)
{
    return(SendMessage(hWnd, CMM_GETTEXT, 0, (LPARAM) pszBuff));
}

long CM_GetLineCount(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_GETLINECOUNT, 0, 0));
}

long CM_GetLine(HWND hWnd, long nLine, char *pszBuff)
{
    return(SendMessage(hWnd, CMM_GETLINE, (WPARAM) nLine, (LPARAM) pszBuff));
}

long CM_GetLineLength(HWND hWnd, long nLine, long bLogical)
{
    return(SendMessage(hWnd, CMM_GETLINELENGTH, (WPARAM) nLine, (LPARAM) bLogical));
}

long CM_GetWord(HWND hWnd, char *pszBuff, CM_POSITION *pPos)
{
    return(SendMessage(hWnd, CMM_GETWORD, (WPARAM) pPos, (LPARAM) pszBuff));
}

long CM_GetWordLength(HWND hWnd, CM_POSITION *pPos, long bLogical)
{
    return(SendMessage(hWnd, CMM_GETWORDLENGTH, (WPARAM) pPos, (LPARAM) bLogical));
}

long CM_AddText(HWND hWnd, char *pszText)
{
    return(SendMessage(hWnd, CMM_ADDTEXT, 0, (LPARAM) pszText));
}

long CM_DeleteLine(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_DELETELINE, (WPARAM) nLine, 0));
}

long CM_InsertLine(HWND hWnd, long nLine, char *pszText)
{
    return(SendMessage(hWnd, CMM_INSERTLINE, (WPARAM) nLine, (LPARAM) pszText));
}

long CM_GetSel(HWND hWnd, CM_RANGE *pRange, long bNormalized)
{
    return(SendMessage(hWnd, CMM_GETSEL, (WPARAM) bNormalized, (LPARAM) pRange));
}

long CM_SetSel(HWND hWnd, CM_RANGE *pRange, long bMakeVisible)
{
    return(SendMessage(hWnd, CMM_SETSEL, (WPARAM) bMakeVisible, (LPARAM) pRange));
}

long CM_DeleteSel(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_DELETESEL, 0, 0));
}

long CM_ReplaceSel(HWND hWnd, char *pszText)
{
    return(SendMessage(hWnd, CMM_REPLACESEL, 0, (LPARAM) pszText));
}

long CM_ReplaceSelLng(HWND hWnd, long pszText)
{
    return(SendMessage(hWnd, CMM_REPLACESEL, 0, (LPARAM) pszText));
}

long CM_ExecuteCmd(HWND hWnd, long wCmd, long dwCmdData)
{
    return(SendMessage(hWnd, CMM_EXECUTECMD, (WPARAM) wCmd, (LPARAM) dwCmdData));
}

long CM_SetSplitterPos(HWND hWnd, long bHorz, long nPos)
{
    return(SendMessage(hWnd, CMM_SETSPLITTERPOS, (WPARAM) bHorz, (LPARAM) nPos));
}

long CM_GetSplitterPos(HWND hWnd, long bHorz)
{
    return(SendMessage(hWnd, CMM_GETSPLITTERPOS, (WPARAM) bHorz, 0));
}

long CM_SetAutoIndentMode(HWND hWnd, long nMode)
{
    return(SendMessage(hWnd, CMM_SETAUTOINDENTMODE, (long) nMode, 0));
}

long CM_GetAutoIndentMode(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_GETAUTOINDENTMODE, 0, 0));
}

long CM_CanUndo(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_CANUNDO, 0, 0));
}

long CM_CanRedo(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_CANREDO, 0, 0));
}

long CM_Redo(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_REDO, 0, 0));
}

long CM_Undo(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_UNDO, 0, 0));
}

long CM_ClearUndoBuffer(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_CLEARUNDOBUFFER, 0, 0));
}

long CM_CanCut(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_CANCUT, 0, 0));
}

long CM_CanCopy(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_CANCOPY, 0, 0));
}

long CM_CanPaste(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_CANPASTE, 0, 0));
}

long CM_Cut(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_CUT, 0, 0));
}

long CM_Copy(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_COPY, 0, 0));
}

long CM_Paste(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_PASTE, 0, 0));
}

long CM_SetUndoLimit(HWND hWnd, long nLimit)
{
    return(SendMessage(hWnd, CMM_SETUNDOLIMIT, (WPARAM) nLimit, 0));
}

long CM_GetUndoLimit(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_GETUNDOLIMIT, 0, 0));
}

long CM_IsModified(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISMODIFIED, 0, 0));
}

long CM_SetModified(HWND hWnd, long bModified)
{
    return(SendMessage(hWnd, CMM_SETMODIFIED, (WPARAM) bModified, 0));
}

long CM_EnableOvertype(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEOVERTYPE, (WPARAM) bEnable, 0));
}

long CM_IsOvertypeEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISOVERTYPEENABLED, 0, 0));
}

long CM_EnableCaseSensitive(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLECASESENSITIVE, (WPARAM) bEnable, 0));
}

long CM_IsCaseSensitiveEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISCASESENSITIVEENABLED, 0, 0));
}

long CM_EnablePreserveCase(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEPRESERVECASE, (WPARAM) bEnable, 0));
}

long CM_IsPreserveCaseEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISPRESERVECASEENABLED, 0, 0));
}

long CM_EnableWholeWord(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEWHOLEWORD, (WPARAM) bEnable, 0));
}

long CM_IsWholeWordEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISWHOLEWORDENABLED, 0, 0));
}

long CM_EnableRegExp(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEREGEXP, (WPARAM) bEnable, 0));
}

long CM_IsRegExpEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISREGEXPENABLED, 0, 0));
}

long CM_SetTopIndex(HWND hWnd, long nView, long nLine)
{
    return(SendMessage(hWnd, CMM_SETTOPINDEX, (WPARAM) nView, (LPARAM) nLine));
}

long CM_GetTopIndex(HWND hWnd, long nView)
{
    return(SendMessage(hWnd, CMM_GETTOPINDEX, (WPARAM) nView, 0));
}

long CM_GetVisibleLineCount(HWND hWnd, long nView, long bFullyVisible)
{
    return(SendMessage(hWnd, CMM_GETVISIBLELINECOUNT, (WPARAM) nView, (LPARAM) bFullyVisible));
}

long CM_EnableCRLF(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLECRLF, (WPARAM) bEnable, 0));
}

long CM_IsCRLFEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISCRLFENABLED, 0, 0));
}

long CM_SetFontOwnership(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_SETFONTOWNERSHIP, (WPARAM) bEnable, 0));
}

long CM_GetFontOwnership(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_GETFONTOWNERSHIP, 0, 0));
}

long CM_GetCurrentView(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_GETCURRENTVIEW, 0, 0));
}

long CM_SetCurrentView(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_SETCURRENTVIEW, (WPARAM) nLine, 0));
}

long CM_GetViewCount(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_GETVIEWCOUNT, 0, 0));
}

long CM_ShowScrollBar(HWND hWnd, long bHorz, long bShow)
{
    return(SendMessage(hWnd, CMM_SHOWSCROLLBAR, (WPARAM) bHorz, (LPARAM) bShow));
}

long CM_HasScrollBar(HWND hWnd, long bHorz)
{
    return(SendMessage(hWnd, CMM_HASSCROLLBAR, (WPARAM) bHorz, 0));
}

long CM_GetSelFromPoint(HWND hWnd, long yClient, long xClient, CM_POSITION *pPos)
{
    return(SendMessage(hWnd, CMM_GETSELFROMPOINT, (WPARAM) (yClient << 16) + xClient, (LPARAM) pPos));
}

long CM_SelectLine(HWND hWnd, long nLine, long bMakeVisible)
{
    return(SendMessage(hWnd, CMM_SELECTLINE, (WPARAM) nLine, (LPARAM) bMakeVisible));
}

long CM_HitTest(HWND hWnd, long xClient, long yClient)
{
    return(SendMessage(hWnd, CMM_HITTEST, (WPARAM) (xClient << 16) + yClient, 0));
}

long CM_EnableDragDrop(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEDRAGDROP, (WPARAM) bEnable, 0));
}

long CM_IsDragDropEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISDRAGDROPENABLED, 0, 0));
}

long CM_EnableSplitter(HWND hWnd, long bHorz, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLESPLITTER, (WPARAM) bHorz, (LPARAM) bEnable));
}

long CM_IsSplitterEnabled(HWND hWnd, long bHorz)
{
    return(SendMessage(hWnd, CMM_ISSPLITTERENABLED, (WPARAM) bHorz, 0));
}

long CM_EnableColumnSel(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLECOLUMNSEL, (WPARAM) bEnable, 0));
}

long CM_IsColumnSelEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISCOLUMNSELENABLED, 0, 0));
}

long CM_EnableGlobalProps(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEGLOBALPROPS, (WPARAM) bEnable, 0));
}

long CM_IsGlobalPropsEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISGLOBALPROPSENABLED, 0, 0));
}

long CM_SetDlgParent(HWND hWnd, HWND hwndParent)
{
    return(SendMessage(hWnd, CMM_SETDLGPARENT, (WPARAM) hwndParent, 0));
}

long CM_EnableSelBounds(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLESELBOUNDS, (WPARAM) bEnable, 0));
}

long CM_IsSelBoundsEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISSELBOUNDSENABLED, 0, 0));
}

long CM_SetFontStyles(HWND hWnd, CM_FONTSTYLES *pFontStyles)
{
    return(SendMessage(hWnd, CMM_SETFONTSTYLES, 0, (LPARAM) pFontStyles));
}

long CM_GetFontStyles(HWND hWnd, CM_FONTSTYLES *pFontStyles)
{
    return(SendMessage(hWnd, CMM_GETFONTSTYLES, 0, (LPARAM) pFontStyles));
}

long CM_SetItemData(HWND hWnd, long nLine, long lParam)
{
    return(SendMessage(hWnd, CMM_SETITEMDATA, (WPARAM) nLine, (LPARAM) lParam));
}

long CM_GetItemData(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_GETITEMDATA, (WPARAM) nLine, 0));
}

long CM_SetLineStyle(HWND hWnd, long nLine, long dwStyle)
{
    return(SendMessage(hWnd, CMM_SETLINESTYLE, (WPARAM) nLine, (LPARAM) dwStyle));
}

long CM_GetLineStyle(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_GETLINESTYLE, (WPARAM) nLine, 0));
}

long CM_SetBookmark(HWND hWnd, long nLine, long bEnable)
{
    return(SendMessage(hWnd, CMM_SETBOOKMARK, (WPARAM) nLine, (LPARAM) bEnable));
}

long CM_GetBookmark(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_GETBOOKMARK, (WPARAM) nLine, 0));
}

long CM_SetAllBookmarks(HWND hWnd, long nCount, long pdwLines)
{
    return(SendMessage(hWnd, CMM_SETALLBOOKMARKS, (WPARAM) nCount, (LPARAM) pdwLines));
}

long CM_GetAllBookmarks(HWND hWnd, long pdwLines)
{
    return(SendMessage(hWnd, CMM_GETALLBOOKMARKS, 0, (LPARAM) pdwLines));
}

long CM_SetBreakpoint(HWND hWnd, long nLine, long bEnable)
{
    return(SendMessage(hWnd, CMM_SETBREAKPOINT, (WPARAM) nLine, (LPARAM) bEnable));
}

long CM_GetBreakpoint(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_GETBREAKPOINT, (WPARAM) nLine, 0));
}

long CM_SetAllBreakpoints(HWND hWnd, long nCount, long pdwLines)
{
    return(SendMessage(hWnd, CMM_SETALLBREAKPOINTS, (WPARAM) nCount, (LPARAM) pdwLines));
}

long CM_GetAllBreakpoints(HWND hWnd, long pdwLines)
{
    return(SendMessage(hWnd, CMM_GETALLBREAKPOINTS, 0, (LPARAM) pdwLines));
}

long CM_SetLineNumbering(HWND hWnd, CM_LINENUMBERING *pNumbering)
{
    return(SendMessage(hWnd, CMM_SETLINENUMBERING, 0, (LPARAM) pNumbering));
}

long CM_GetLineNumbering(HWND hWnd, CM_LINENUMBERING *pNumbering)
{
    return(SendMessage(hWnd, CMM_GETLINENUMBERING, 0, (LPARAM) pNumbering));
}

long CM_PosFromChar(HWND hWnd, CM_POSITION *pPos, RECT *pRect)
{
    return(SendMessage(hWnd, CMM_POSFROMCHAR, (WPARAM) pPos, (LPARAM) pRect));
}

long CM_EnableHideSel(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEHIDESEL, (WPARAM) bEnable, 0));
}

long CM_IsHideSelEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISHIDESELENABLED, 0, 0));
}

long CM_SetHighlightedLine(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_SETHIGHLIGHTEDLINE, (WPARAM) nLine, 0));
}

long CM_GetHighlightedLine(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_GETHIGHLIGHTEDLINE, 0, 0));
}

long CM_EnableNormalizeCase(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLENORMALIZECASE, (WPARAM) bEnable, 0));
}

long CM_IsNormalizeCaseEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISNORMALIZECASEENABLED, 0, 0));
}

long CM_SetDivider(HWND hWnd, long nLine, long bEnable)
{
    return(SendMessage(hWnd, CMM_SETDIVIDER, (WPARAM) nLine, (LPARAM) bEnable));
}

long CM_GetDivider(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_GETDIVIDER, nLine, 0));
}

long CM_EnableOvertypeCaret(HWND hWnd, long bEnable)
{
    return(SendMessage(hWnd, CMM_ENABLEOVERTYPECARET, (WPARAM) bEnable, 0));
}

long CM_IsOvertypeCaretEnabled(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_ISOVERTYPECARETENABLED, 0, 0));
}

long CM_SetFindText(HWND hWnd, char *pszText)
{
    return(SendMessage(hWnd, CMM_SETFINDTEXT, 0, (LPARAM) pszText));
}

long CM_GetFindText(HWND hWnd, char *pszText)
{
    return(SendMessage(hWnd, CMM_GETFINDTEXT, 0, (LPARAM) pszText));
}

long CM_SetReplaceText(HWND hWnd, char *pszText)
{
    return(SendMessage(hWnd, CMM_SETREPLACETEXT, 0, (LPARAM) pszText));
}

long CM_GetReplaceText(HWND hWnd, char *pszText)
{
    return(SendMessage(hWnd, CMM_GETREPLACETEXT, 0, (LPARAM) pszText));
}
                
long CM_SetImageList(HWND hWnd, HIMAGELIST hImageList)
{
    return(SendMessage(hWnd, CMM_SETIMAGELIST, 0, (LPARAM) hImageList));
}
                
HIMAGELIST CM_GetImageList(HWND hWnd)
{
    return((HIMAGELIST) SendMessage(hWnd, CMM_GETIMAGELIST, 0, 0));
}
                
long CM_SetMarginImages(HWND hWnd, long nLine, long byImages)
{
    return(SendMessage(hWnd, CMM_SETMARGINIMAGES, (WPARAM) nLine, (LPARAM) byImages));
}
                
long CM_GetMarginImages(HWND hWnd, long nLine)
{
    return(SendMessage(hWnd, CMM_GETMARGINIMAGES, (WPARAM) nLine, 0));
}
                
long CM_Print(HWND hWnd, HDC hDC, long dwFlags)
{
    return(SendMessage(hWnd, CMM_PRINT, (WPARAM) hDC, (LPARAM) dwFlags));
}
                
long CM_SetCaretPos(HWND hWnd, long nLine, long nCol)
{
    return(SendMessage(hWnd, CMM_SETCARETPOS, (WPARAM) nLine, (LPARAM) nCol));
}
                
long CM_ViewColToBufferCol(HWND hWnd, long nLine, long nViewCol)
{
    return(SendMessage(hWnd, CMM_VIEWCOLTOBUFFERCOL, (WPARAM) nLine, (LPARAM) nViewCol));
}

long CM_BufferColToViewCol(HWND hWnd, long nLine, long nBufferCol)
{
    return(SendMessage(hWnd, CMM_BUFFERCOLTOVIEWCOL, (WPARAM) nLine, (LPARAM) nBufferCol));
}

long CM_SetBorderStyle(HWND hWnd, long dwStyle)
{
    return(SendMessage(hWnd, CMM_SETBORDERSTYLE, (WPARAM) dwStyle, 0));
}

long CM_GetBorderStyle(HWND hWnd)
{
    return(SendMessage(hWnd, CMM_GETBORDERSTYLE, 0, 0));
}

long CM_SetTokenCallBack(HWND hWnd,long CallBackRoutine)
{
    return(SendMessage(hWnd, CMM_SETTOKENCALLBACK, 0, (LPARAM) CallBackRoutine));
}
