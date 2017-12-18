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
// MsgCodeMax.h: declaration of used variables / structures for MsgCodeMax.cpp
// -----------------------------------------------------------------------

#ifndef _MSGCODEMAX_H_
#define _MSGCODEMAX_H_

// -----------------------------------------------------------------------
// Structures
typedef struct _CM_LANGUAGE
{
    long dwStyle;
    BOOL bIsCaseSensitive;
    LPCTSTR pszKeywords;
    LPCTSTR pszOperators;
    LPCTSTR pszSingleLineComments;
    LPCTSTR pszMultiLineComments1;
    LPCTSTR pszMultiLineComments2;
    LPCTSTR pszScopeKeywords1;
    LPCTSTR pszScopeKeywords2;
    LPCTSTR pszStringDelims;
    TCHAR chEscape;
    TCHAR chTerminator;
    LPCTSTR pszTagElementNames;
    LPCTSTR pszTagAttributeNames;
    LPCTSTR pszTagEntities;
    LPCTSTR pszExtra1;
    LPCTSTR pszExtra2;
    LPCTSTR pszExtra3;
    long dwHighLightNbrs;

    // *** Added this to primary struct
    LPCTSTR pszLanguageName;
    LPCTSTR pszHelpFileA;
    LPCTSTR pszHelpFileB;
    LPCTSTR pszHelpFileC;
    LPCTSTR pszHelpFileD;
    LPCTSTR pszHelpFileE;
    LPCTSTR pszExtensions;
    LPCTSTR pszProcEntry;
    LPCTSTR pszEndpEntry;
    LPCTSTR pszGUIDEntry;
    LPCTSTR pszIncludeEntry;
    long pszDoubleSlash;
    long pszInterlaceProc;
    LPCTSTR pszOpenBlockProc;
    LPCTSTR pszCloseBlockProc;
    long pszOrphanBlock;
    LPCTSTR pszFileName;
    LPCTSTR pszHelpFileF;
    LPCTSTR pszHelpFileG;
    LPCTSTR pszHelpFileH;
    LPCTSTR pszIncludeName;
    LPCTSTR pszIncludeNameFoot;
    LPCTSTR pszHexType;
    LPCTSTR pszBinType;
} CM_LANGUAGE;

// Color settings for CMM_GETCOLORS and CMM_SETCOLORS
// Note:  Use CLR_INVALID on background colors to specify transparent
//        (text) or appropriate control panel setting
typedef struct _CM_COLORS
{
    // window background color
    COLORREF crWindow;
    // left margin background color
    COLORREF crLeftMargin;
    // bookmark foreground color
    COLORREF crBookmark;
    // bookmark background color
    COLORREF crBookmarkBk;
    // breakpoint foreground color
    COLORREF crBreakpoint;
    // breakpoint background color
    COLORREF crBreakpointBk;
    // plain text foreground color
    COLORREF crText;
    // plain text background color
    COLORREF crTextBk;
    // numeric literal foreground color
    COLORREF crNumber;
    // numeric literal background color
    COLORREF crNumberBk;
    // keyword foreground color
    COLORREF crKeyword;
    // keyword background color
    COLORREF crKeywordBk;
    // operator foreground color
    COLORREF crOperator;
    // operator background color
    COLORREF crOperatorBk;
    // scope keyword foreground color
    COLORREF crScopeKeyword;
    // scope keyword background color
    COLORREF crScopeKeywordBk;
    // comment foreground color
    COLORREF crComment;
    //comment background color
    COLORREF crCommentBk;
    // string foreground color
    COLORREF crString;
    // string background color
    COLORREF crStringBk;
    // plain tag text foreground color
    COLORREF crTagText;
    // plain tag text background color
    COLORREF crTagTextBk;
    // tag entity foreground color
    COLORREF crTagEntity;
    // tag entity background color
    COLORREF crTagEntityBk;
    // tag element name foreground color
    COLORREF crTagElementName;
    // tag element name background color
    COLORREF crTagElementNameBk;
    // tag attribute name foreground color
    COLORREF crTagAttributeName;
    // tag attribute name background color
    COLORREF crTagAttributeNameBk;
    // line number foreground color
    COLORREF crLineNumber;
    // line number background color
    COLORREF crLineNumberBk;
    // line number separate line color
    COLORREF crHDividerLines;
    // left margin separate line color
    COLORREF crVDividerLines;
    // highlighted line color
    COLORREF crHighlightedLine;
    COLORREF crExtra1;
    COLORREF crExtra1Bk;
    COLORREF crExtra2;
    COLORREF crExtra2Bk;
    COLORREF crExtra3;
    COLORREF crExtra3Bk;
} CM_COLORS;

// Font style settings for CMM_GETFONTSTYLES and CMM_SETFONTSTYLES
// each byte value is one of the CM_FONT_XXX values listed below
// this declaration
typedef struct _CM_FONTSTYLES
{
    // plain text font style
    BYTE byText;
    // numeric literal font style
    BYTE byNumber;
    // keyword font style
    BYTE byKeyword;
    // operator font style
    BYTE byOperator;
    // scope keyword font style
    BYTE byScopeKeyword;
    // comment font style
    BYTE byComment;
    // string font style
    BYTE byString;
    // plain tag text font style
    BYTE byTagText;
    // tag entity font style
    BYTE byTagEntity;
    // tag element name font style
    BYTE byTagElementName;
    // tag attribute name font style
    BYTE byTagAttributeName;
    BYTE byExtra1;
    BYTE byExtra2;
    BYTE byExtra3;
    // line number font style
    BYTE byLineNumber;
} CM_FONTSTYLES;

// Text position indicators
typedef struct _CM_POSITION
{
    // zero-based line number
    int nLine;
    // zero-based *buffer* column number
    int nCol;
} CM_POSITION;

typedef struct _CM_RANGE
{
    // the anchor
    CM_POSITION posStart;
    // the extension (if same as anchor, selection is empty)
    CM_POSITION posEnd;
    // TRUE if is a column selection, FALSE if paragragh selection
    BOOL bColumnSel;
} CM_RANGE;

// Hot key descriptor
typedef struct _CM_HOTKEY
{
    // 1st keystroke's modifiers (combination of HOTKEYF_ALT, HOTKEYF_SHIFT, HOTKEYF_CONTROL)
    BYTE byModifiers1;
    // 1st keystroke's virtkey (e.g. Ctrl + 'A')
    UINT nVirtKey1;
    // 2nd keystroke's modifiers (combination of HOTKEYF_ALT, HOTKEYF_SHIFT, HOTKEYF_CONTROL)
    BYTE byModifiers2;
    // 2nd keystroke's virtkey (e.g. Ctrl + 'A')
    UINT nVirtKey2;
} CM_HOTKEY;

// CMN_DRAWLINE notification data passed to parent window
typedef struct _CM_DRAWLINEDATA
{
    // standard notification data
    NMHDR hdr;
    // device context to draw with, clipped to current paint area
    HDC hDC;
    // bounds of line, excluding left margin region.
    RECT rcLine;
    // line number to draw
    int nLine;
    // leftmost visible col
    int nLeftCol;
    // rightmost visible col
    int nRightCol;
    // itemdata associated with the line (CMM_SETITEMDATA)
    LPARAM lParam;
    // line style bits (CML_XXX)
    long dwStyle;
} CM_DRAWLINEDATA;

// CMN_DELETELINE notification data passed to parent window
typedef struct _CM_DELETELINEDATA
{
    // standard notification data
    NMHDR hdr;
    // line that is about to be deleted
    int nLine;
    // itemdata associated with the line (CMM_SETITEMDATA)
    LPARAM lParam;
} CM_DELETELINEDATA;

// CMN_CMDFAILURE notification data passed to parent window
typedef struct _CM_CMDFAILUREDATA
{
    // standard notification data
    NMHDR hdr;
    // CMD_XXX command that failed
    WORD wCmd;
    // CMDERR_XXX failure code
    long dwErr;
} CM_CMDFAILUREDATA;

// CMN_REGISTEREDCMD notification data passed to parent window
typedef struct _CM_REGISTEREDCMDDATA
{
    // standard notification data
    NMHDR hdr;
    // registered command to be executed
    WORD wCmd;
} CM_REGISTEREDCMDDATA;

typedef struct _CM_KEYDATA
{
    // standard notification data
    NMHDR hdr;
    // virtkey if CMN_KEYUP or CMN_KEYDOWN.  Ascii code if CMN_KEYPRESS
    int nKeyCode;
    // bitfield of: CM_KEY_SHIFT, CM_KEY_CTRL, and/or CM_KEY_ALT
    int nKeyModifier;
} CM_KEYDATA;

typedef struct _CM_MOUSEDATA
{
    // standard notification data
    NMHDR hdr;
    // position of mouse (client coordinates)
    POINT pt;
    // bitfield of: CM_BTN_LEFT, CM_BTN_RIGHT, and/or CM_BTN_MIDDLE
    int nButton;
    // bitfield of: CM_KEY_SHIFT, CM_KEY_CTRL, and/or CM_KEY_ALT
    int nKeyModifier;
} CM_MOUSEDATA;

// CMN_FINDWRAPPED notification data passed to parent window
typedef struct _CM_FINDWRAPPEDDATA
{
    // standard notification data
    NMHDR hdr;
    // the command being executed
    WORD wCmd;
    // TRUE if wrapped while searching forward, FALSE if searching backward
    BOOL bForward;
} CM_FINDWRAPPEDDATA;

typedef struct _CM_LINENUMBERING
{
    // TRUE to enable, FALSE to disable
    BOOL bEnabled;
    // # of line at top of buffer (line 0)
    UINT nStartAt;
    // CM_DECIMAL, CM_HEXADECIMAL, CM_OCTAL, or CM_BINARY
    long dwStyle;
    // Complement Hexadecmal numbers of not
    BOOL bComplementHexa;
} CM_LINENUMBERING;

// -----------------------------------------------------------------------
// Variables
extern HMODULE hCodeMax;
extern void (* hCodeMaxCMRegisterControl) (long, long);
extern void (* hCodeMaxCMUnregisterControl) (void);
extern void (* hCodeMaxCMRegisterLanguage) (char *, CM_LANGUAGE *);
extern void (* hCodeMaxCMUnRegisterLanguage) (char *);
extern void (* hCodeMaxCMRegisterCommand) (long, char *, char *);
extern void (* hCodeMaxCMRegisterHotKey) (long, long);

// -----------------------------------------------------------------------
// Functions declarations
void InitCodeMax(HINSTANCE hInst);
void UnInitCodeMax(void);
HWND CreateCodeMax(long CMLeft, long CMTop, long CMWidth, long CMHeight, HWND hParent, HINSTANCE hInst, long Maximize);
long CM_SetLanguage(HWND hWnd, char *pszName);
long CM_GetLanguage(HWND hWnd, char *pszName);
long CM_EnableColorSyntax(HWND hWnd, long bEnable);
long CM_IsColorSyntaxEnabled(HWND hWnd);
long CM_OpenFile(HWND hWnd, char *pszFileName);
long CM_InsertFile(HWND hWnd, char *pszFileName, CM_RANGE *pPos);
long CM_InsertText(HWND hWnd, char *pszText, CM_RANGE *pPos);
long CM_InsertTextLng(HWND hWnd, long pszText, CM_RANGE *pPos);
long CM_SetText(HWND hWnd, char *pszText);
long CM_SetTextLng(HWND hWnd, long pszText);
long CM_SetColors(HWND hWnd, CM_COLORS *pColors);
long CM_GetColors(HWND hWnd, CM_COLORS *pColors);
long CM_EnableWhitespaceDisplay(HWND hWnd, long bEnable);
long CM_IsWhitespaceDisplayEnabled(HWND hWnd);
long CM_EnableTabExpand(HWND hWnd, long bEnable);
long CM_IsTabExpandEnabled(HWND hWnd);
long CM_EnableSmoothScrolling(HWND hWnd, long bEnable);
long CM_IsSmoothScrollingEnabled(HWND hWnd);
long CM_SetTabSize(HWND hWnd, long nTabSize);
long CM_GetTabSize(HWND hWnd);
long CM_SetReadOnly(HWND hWnd, long bReadOnly);
long CM_IsReadOnly(HWND hWnd);
long CM_EnableLineToolTips(HWND hWnd, long bEnable);
long CM_IsLineToolTipsEnabled(HWND hWnd);
long CM_EnableLeftMargin(HWND hWnd, long bEnable);
long CM_IsLeftMarginEnabled(HWND hWnd);
long CM_SaveFile(HWND hWnd, char *pszFileName, long bClearUndo);
long CM_ReplaceText(HWND hWnd, char *pszText, CM_RANGE *pRange);
long CM_ReplaceLine(HWND hWnd, char *pszText, long nLine);
long CM_ReplaceTextAll(HWND hWnd, char *pszText);
long CM_GetTextLengthAll(HWND hWnd, long bLogical);
long CM_GetTextLength(HWND hWnd, CM_RANGE *pRange, long bLogical);
long CM_GetText(HWND hWnd, char *pszBuff, CM_RANGE *pRange);
long CM_GetTextLng(HWND hWnd, long pszBuff, CM_RANGE *pRange);
long CM_GetTextAll(HWND hWnd, char *pszBuff);
long CM_GetTextAllLng(HWND hWnd, long pszBuff);
long CM_GetLineCount(HWND hWnd);
long CM_GetLine(HWND hWnd, long nLine, char *pszBuff);
long CM_GetLineLength(HWND hWnd, long nLine, long bLogical);
long CM_GetWord(HWND hWnd, char *pszBuff, CM_POSITION *pPos);
long CM_GetWordLength(HWND hWnd, CM_POSITION *pPos, long bLogical);
long CM_AddText(HWND hWnd, char *pszText);
long CM_DeleteLine(HWND hWnd, long nLine);
long CM_InsertLine(HWND hWnd, long nLine, char *pszText);
long CM_GetSel(HWND hWnd, CM_RANGE *pRange, long bNormalized);
long CM_SetSel(HWND hWnd, CM_RANGE *pRange, long bMakeVisible);
long CM_DeleteSel(HWND hWnd);
long CM_ReplaceSel(HWND hWnd, char *pszText);
long CM_ReplaceSelLng(HWND hWnd, long pszText);
long CM_ExecuteCmd(HWND hWnd, long wCmd, long dwCmdData);
long CM_SetSplitterPos(HWND hWnd, long bHorz, long nPos);
long CM_GetSplitterPos(HWND hWnd, long bHorz);
long CM_SetAutoIndentMode(HWND hWnd, long nMode);
long CM_GetAutoIndentMode(HWND hWnd);
long CM_CanUndo(HWND hWnd);
long CM_CanRedo(HWND hWnd);
long CM_Redo(HWND hWnd);
long CM_Undo(HWND hWnd);
long CM_ClearUndoBuffer(HWND hWnd);
long CM_CanCut(HWND hWnd);
long CM_CanCopy(HWND hWnd);
long CM_CanPaste(HWND hWnd);
long CM_Cut(HWND hWnd);
long CM_Copy(HWND hWnd);
long CM_Paste(HWND hWnd);
long CM_SetUndoLimit(HWND hWnd, long nLimit);
long CM_GetUndoLimit(HWND hWnd);
long CM_IsModified(HWND hWnd);
long CM_SetModified(HWND hWnd, long bModified);
long CM_EnableOvertype(HWND hWnd, long bEnable);
long CM_IsOvertypeEnabled(HWND hWnd);
long CM_EnableCaseSensitive(HWND hWnd, long bEnable);
long CM_IsCaseSensitiveEnabled(HWND hWnd);
long CM_EnablePreserveCase(HWND hWnd, long bEnable);
long CM_IsPreserveCaseEnabled(HWND hWnd);
long CM_EnableWholeWord(HWND hWnd, long bEnable);
long CM_IsWholeWordEnabled(HWND hWnd);
long CM_EnableRegExp(HWND hWnd, long bEnable);
long CM_IsRegExpEnabled(HWND hWnd);
long CM_SetTopIndex(HWND hWnd, long nView, long nLine);
long CM_GetTopIndex(HWND hWnd, long nView);
long CM_GetVisibleLineCount(HWND hWnd, long nView, long bFullyVisible);
long CM_EnableCRLF(HWND hWnd, long bEnable);
long CM_IsCRLFEnabled(HWND hWnd);
long CM_SetFontOwnership(HWND hWnd, long bEnable);
long CM_GetFontOwnership(HWND hWnd);
long CM_GetCurrentView(HWND hWnd);
long CM_SetCurrentView(HWND hWnd, long nLine);
long CM_GetViewCount(HWND hWnd);
long CM_ShowScrollBar(HWND hWnd, long bHorz, long bShow);
long CM_HasScrollBar(HWND hWnd, long bHorz);
long CM_GetSelFromPoint(HWND hWnd, long yClient, long xClient, CM_POSITION *pPos);
long CM_SelectLine(HWND hWnd, long nLine, long bMakeVisible);
long CM_HitTest(HWND hWnd, long xClient, long yClient);
long CM_EnableDragDrop(HWND hWnd, long bEnable);
long CM_IsDragDropEnabled(HWND hWnd);
long CM_EnableSplitter(HWND hWnd, long bHorz, long bEnable);
long CM_IsSplitterEnabled(HWND hWnd, long bHorz);
long CM_EnableColumnSel(HWND hWnd, long bEnable);
long CM_IsColumnSelEnabled(HWND hWnd);
long CM_EnableGlobalProps(HWND hWnd, long bEnable);
long CM_IsGlobalPropsEnabled(HWND hWnd);
long CM_SetDlgParent(HWND hWnd, HWND hwndParent);
long CM_EnableSelBounds(HWND hWnd, long bEnable);
long CM_IsSelBoundsEnabled(HWND hWnd);
long CM_SetFontStyles(HWND hWnd, CM_FONTSTYLES *pFontStyles);
long CM_GetFontStyles(HWND hWnd, CM_FONTSTYLES *pFontStyles);
long CM_SetItemData(HWND hWnd, long nLine, long lParam);
long CM_GetItemData(HWND hWnd, long nLine);
long CM_SetLineStyle(HWND hWnd, long nLine, long dwStyle);
long CM_GetLineStyle(HWND hWnd, long nLine);
long CM_SetBookmark(HWND hWnd, long nLine, long bEnable);
long CM_GetBookmark(HWND hWnd, long nLine);
long CM_SetAllBookmarks(HWND hWnd, long nCount, long pdwLines);
long CM_GetAllBookmarks(HWND hWnd, long pdwLines);
long CM_SetBreakpoint(HWND hWnd, long nLine, long bEnable);
long CM_GetBreakpoint(HWND hWnd, long nLine);
long CM_SetAllBreakpoints(HWND hWnd, long nCount, long pdwLines);
long CM_GetAllBreakpoints(HWND hWnd, long pdwLines);
long CM_SetLineNumbering(HWND hWnd, CM_LINENUMBERING *pNumbering);
long CM_GetLineNumbering(HWND hWnd, CM_LINENUMBERING *pNumbering);
long CM_PosFromChar(HWND hWnd, CM_POSITION *pPos, RECT *pRect);
long CM_EnableHideSel(HWND hWnd, long bEnable);
long CM_IsHideSelEnabled(HWND hWnd);
long CM_SetHighlightedLine(HWND hWnd, long nLine);
long CM_GetHighlightedLine(HWND hWnd);
long CM_EnableNormalizeCase(HWND hWnd, long bEnable);
long CM_IsNormalizeCaseEnabled(HWND hWnd);
long CM_SetDivider(HWND hWnd, long nLine, long bEnable);
long CM_GetDivider(HWND hWnd, long nLine);
long CM_EnableOvertypeCaret(HWND hWnd, long bEnable);
long CM_IsOvertypeCaretEnabled(HWND hWnd);
long CM_SetFindText(HWND hWnd, char *pszText);
long CM_GetFindText(HWND hWnd, char *pszText);
long CM_SetReplaceText(HWND hWnd, char *pszText);
long CM_GetReplaceText(HWND hWnd, char *pszText);
long CM_SetImageList(HWND hWnd, HIMAGELIST hImageList);
HIMAGELIST CM_GetImageList(HWND hWnd);
long CM_SetMarginImages(HWND hWnd, long nLine, long byImages);
long CM_GetMarginImages(HWND hWnd, long nLine);
long CM_Print(HWND hWnd, HDC hDC, long dwFlags);
long CM_SetCaretPos(HWND hWnd, long nLine, long nCol);
long CM_ViewColToBufferCol(HWND hWnd, long nLine, long nViewCol);
long CM_BufferColToViewCol(HWND hWnd, long nLine, long nBufferCol);
long CM_SetBorderStyle(HWND hWnd, long dwStyle);
long CM_GetBorderStyle(HWND hWnd);
long CM_SetTokenCallBack(HWND hWnd,long CallBackRoutine);

#endif
