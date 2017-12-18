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
// ConstCodeMax.h: Constants for CMaxXX.dll
// -----------------------------------------------------------------------

#ifndef _CONSTCODEMAX_H_
#define _CONSTCODEMAX_H_

#define CM_FLAG_WINDOWSXP 1

// maximum size (TCHARs) of text to find or replace
#define CM_MAX_FINDREPL_TEXT 100
// maximum MRU size in find and find/replace dialogs
#define CM_FIND_REPLACE_MRU_MAX 10
// required buffer size for pszMRUList argument CMSetFindReplaceMRUList() and CMGetFindReplaceMRUList()
#define CM_FIND_REPLACE_MRU_BUFF_SIZE ((CM_MAX_FINDREPL_TEXT + 1) * CM_FIND_REPLACE_MRU_MAX)
// maximum size of command string returned in pszBuff param of CMGetCommandString() if bDescription is FALSE
#define CM_MAX_CMD_STRING 50
// maximum size of command string returned in pszBuff param of CMGetCommandString() if bDescription is TRUE
#define CM_MAX_CMD_DESCRIPTION 100
// maximum size of a language name set with CMRegisterLanguage
#define CM_MAX_LANGUAGE_NAME 30
// maximum tab size (characters)
#define CM_MAX_TABSIZE 100
// minimum tab size (characters)
#define CM_MIN_TABSIZE 2
// left margin width (pixels)
#define CM_CXLEFTMARGIN 24

// Parent window should receive CMN_DRAWLINE notifications
#define CML_OWNERDRAW 1
// Parent window should receive CMN_LINEDELETED notifications
#define CML_NOTIFY_DEL 2

#define CMM_SETLANGUAGE (WM_USER + 1600)
#define CMM_GETLANGUAGE (WM_USER + 1601)
#define CMM_ENABLECOLORSYNTAX (WM_USER + 1610)
#define CMM_ISCOLORSYNTAXENABLED (WM_USER + 1620)
#define CMM_SETCOLORS (WM_USER + 1630)
#define CMM_GETCOLORS (WM_USER + 1640)
#define CMM_ENABLEWHITESPACEDISPLAY (WM_USER + 1800)
#define CMM_ISWHITESPACEDISPLAYENABLED (WM_USER + 1810)
#define CMM_ENABLETABEXPAND (WM_USER + 1811)
#define CMM_ISTABEXPANDENABLED (WM_USER + 1812)
#define CMM_ENABLESMOOTHSCROLLING (WM_USER + 1820)
#define CMM_SETTABSIZE (WM_USER + 1821)
#define CMM_GETTABSIZE (WM_USER + 1822)
#define CMM_ISSMOOTHSCROLLINGENABLED (WM_USER + 1830)
#define CMM_SETREADONLY (WM_USER + 1840)
#define CMM_ISREADONLY (WM_USER + 1850)
#define CMM_ENABLELINETOOLTIPS (WM_USER + 1860)
#define CMM_ISLINETOOLTIPSENABLED (WM_USER + 1870)
#define CMM_ENABLELEFTMARGIN (WM_USER + 1880)
#define CMM_ISLEFTMARGINENABLED (WM_USER + 1890)
#define CMM_ENABLECOLUMNSEL (WM_USER + 1891)
#define CMM_ISCOLUMNSELENABLED (WM_USER + 1892)
#define CMM_ENABLEDRAGDROP (WM_USER + 1893)
#define CMM_ISDRAGDROPENABLED (WM_USER + 1894)
#define CMM_ENABLEOVERTYPE (WM_USER + 1900)
#define CMM_ISOVERTYPEENABLED (WM_USER + 1910)
#define CMM_ENABLECASESENSITIVE (WM_USER + 1920)
#define CMM_ISCASESENSITIVEENABLED (WM_USER + 1930)
#define CMM_ENABLEPRESERVECASE (WM_USER + 1931)
#define CMM_ISPRESERVECASEENABLED (WM_USER + 1932)
#define CMM_ENABLEWHOLEWORD (WM_USER + 1940)
#define CMM_ISWHOLEWORDENABLED (WM_USER + 1950)
#define CMM_SETTOPINDEX (WM_USER + 1960)
#define CMM_GETTOPINDEX (WM_USER + 1970)
#define CMM_GETVISIBLELINECOUNT (WM_USER + 1980)
#define CMM_HITTEST (WM_USER + 1990)
#define CMM_OPENFILE (WM_USER + 2300)
#define CMM_SAVEFILE (WM_USER + 2310)
#define CMM_INSERTFILE (WM_USER + 2320)
#define CMM_INSERTTEXT (WM_USER + 2330)
#define CMM_SETTEXT (WM_USER + 2335)
#define CMM_REPLACETEXT (WM_USER + 2340)
#define CMM_GETTEXTLENGTH (WM_USER + 2350)
#define CMM_GETTEXT (WM_USER + 2360)
#define CMM_GETLINECOUNT (WM_USER + 2365)
#define CMM_GETLINE (WM_USER + 2370)
#define CMM_GETLINELENGTH (WM_USER + 2380)
#define CMM_GETWORD (WM_USER + 2381)
#define CMM_GETWORDLENGTH (WM_USER + 2382)
#define CMM_ADDTEXT (WM_USER + 2390)
#define CMM_DELETELINE (WM_USER + 2400)
#define CMM_INSERTLINE (WM_USER + 2410)
#define CMM_GETSEL (WM_USER + 2420)
#define CMM_GETSELFROMPOINT (WM_USER + 2425)
#define CMM_SETSEL (WM_USER + 2430)
#define CMM_SELECTLINE (WM_USER + 2435)
#define CMM_DELETESEL (WM_USER + 2440)
#define CMM_REPLACESEL (WM_USER + 2450)
#define CMM_ISMODIFIED (WM_USER + 2460)
#define CMM_SETMODIFIED (WM_USER + 2461)
#define CMM_ENABLECRLF (WM_USER + 2470)
#define CMM_ISCRLFENABLED (WM_USER + 2480)
#define CMM_SETFONTOWNERSHIP (WM_USER + 2485)
#define CMM_GETFONTOWNERSHIP (WM_USER + 2486)
#define CMM_EXECUTECMD (WM_USER + 2700)
#define CMM_SETSPLITTERPOS (WM_USER + 2900)
#define CMM_GETSPLITTERPOS (WM_USER + 2901)
#define CMM_SETAUTOINDENTMODE (WM_USER + 3100)
#define CMM_GETAUTOINDENTMODE (WM_USER + 3110)
#define CMM_CANUNDO (WM_USER + 3300)
#define CMM_CANREDO (WM_USER + 3310)
#define CMM_CANCUT (WM_USER + 3320)
#define CMM_CANCOPY (WM_USER + 3330)
#define CMM_CANPASTE (WM_USER + 3340)
#define CMM_REDO (WM_USER + 3350)
#define CMM_CLEARUNDOBUFFER (WM_USER + 3351)
#define CMM_UNDO (WM_USER + 3360)
#define CMM_CUT (WM_USER + 3370)
#define CMM_COPY (WM_USER + 3380)
#define CMM_PASTE (WM_USER + 3390)
#define CMM_SETUNDOLIMIT (WM_USER + 3400)
#define CMM_GETUNDOLIMIT (WM_USER + 3410)
#define CMM_GETVIEWCOUNT (WM_USER + 3600)
#define CMM_GETCURRENTVIEW (WM_USER + 3610)
#define CMM_SHOWSCROLLBAR (WM_USER + 3700)
#define CMM_HASSCROLLBAR (WM_USER + 3710)
#define CMM_ENABLESPLITTER (WM_USER + 3720)
#define CMM_ISSPLITTERENABLED (WM_USER + 3730)
#define CMM_ENABLEGLOBALPROPS (WM_USER + 3740)
#define CMM_ISGLOBALPROPSENABLED (WM_USER + 3741)
#define CMM_SETDLGPARENT (WM_USER + 3750)
#define CMM_ENABLESELBOUNDS (WM_USER + 3760)
#define CMM_ISSELBOUNDSENABLED (WM_USER + 3770)
#define CMM_SETFONTSTYLES (WM_USER + 3780)
#define CMM_GETFONTSTYLES (WM_USER + 3790)
#define CMM_ENABLEREGEXP (WM_USER + 3800)
#define CMM_ISREGEXPENABLED (WM_USER + 3810)
#define CMM_SETITEMDATA (WM_USER + 3820)
#define CMM_GETITEMDATA (WM_USER + 3830)
#define CMM_SETLINESTYLE (WM_USER + 3840)
#define CMM_GETLINESTYLE (WM_USER + 3850)
#define CMM_SETBOOKMARK (WM_USER + 3860)
#define CMM_GETBOOKMARK (WM_USER + 3870)
#define CMM_SETALLBOOKMARKS (WM_USER + 3880)
#define CMM_GETALLBOOKMARKS (WM_USER + 3890)
#define CMM_SETBREAKPOINT (WM_USER + 3900)
#define CMM_GETBREAKPOINT (WM_USER + 3910)
#define CMM_SETALLBREAKPOINTS (WM_USER + 3920)
#define CMM_GETALLBREAKPOINTS (WM_USER + 3930)
#define CMM_SETLINENUMBERING (WM_USER + 3940)
#define CMM_GETLINENUMBERING (WM_USER + 3950)
#define CMM_POSFROMCHAR (WM_USER + 3960)
#define CMM_ENABLEHIDESEL (WM_USER + 3970)
#define CMM_ISHIDESELENABLED (WM_USER + 3980)
#define CMM_SETHIGHLIGHTEDLINE (WM_USER + 3990)
#define CMM_GETHIGHLIGHTEDLINE (WM_USER + 4000)
#define CMM_ENABLENORMALIZECASE (WM_USER + 4010)
#define CMM_ISNORMALIZECASEENABLED (WM_USER + 4020)
#define CMM_SETDIVIDER (WM_USER + 4030)
#define CMM_GETDIVIDER (WM_USER + 4040)
#define CMM_ENABLEOVERTYPECARET (WM_USER + 4050)
#define CMM_ISOVERTYPECARETENABLED (WM_USER + 4060)
#define CMM_SETFINDTEXT (WM_USER + 4070)
#define CMM_GETFINDTEXT (WM_USER + 4080)
#define CMM_SETREPLACETEXT (WM_USER + 4090)
#define CMM_GETREPLACETEXT (WM_USER + 4100)
#define CMM_SETIMAGELIST (WM_USER + 4110)
#define CMM_GETIMAGELIST (WM_USER + 4120)
#define CMM_SETMARGINIMAGES (WM_USER + 4130)
#define CMM_GETMARGINIMAGES (WM_USER + 4140)
#define CMM_PRINT (WM_USER + 4150)
#define CMM_SETCARETPOS (WM_USER + 4160)
#define CMM_VIEWCOLTOBUFFERCOL (WM_USER + 4170)
#define CMM_BUFFERCOLTOVIEWCOL (WM_USER + 4180)
#define CMM_SETBORDERSTYLE (WM_USER + 4190)
#define CMM_GETBORDERSTYLE (WM_USER + 4200)
#define CMM_SETCURRENTVIEW (WM_USER + 4210)
#define CMM_REPLACELINE (WM_USER + 4220)
#define CMM_SETTOKENCALLBACK (WM_USER + 4230)
#define CMM_ENABLEALLOPENED (WM_USER + 4240)

// Notifications
// buffer contents changed
#define CMN_CHANGE 100
// horizontal scrollbar pos changed
#define CMN_HSCROLL 110
// vertical scrollbar pos changed
#define CMN_VSCROLL 120
// active selection has changed
#define CMN_SELCHANGE 130
// the user switched to a different splitter view
#define CMN_VIEWCHANGE 140
// the modified state of the buffer has changed
#define CMN_MODIFIEDCHANGE 150
// the user is about to show the props window
#define CMN_SHOWPROPS 160
// the user changed properties via the props window
#define CMN_PROPSCHANGE 170
// the control has been created
#define CMN_CREATE 180
// the control is about to be destroyed
#define CMN_DESTROY 190

// 2.0
// owner-draw functionality
#define CMN_DRAWLINE 200
// line is about to be deleted
#define CMN_DELETELINE 210
// a keystroke command failed to successfully complete
#define CMN_CMDFAILURE 220
// a registered command has been invoked by the user
#define CMN_REGISTEREDCMD 230
// a key was pressed (WM_KEYDOWN)
#define CMN_KEYDOWN 240
// a key was released (WM_KEYUP)
#define CMN_KEYUP 250
// a key was pressed and released (WM_CHAR)
#define CMN_KEYPRESS 260
// a mouse button was pressed (WM_xBUTTONDOWN)
#define CMN_MOUSEDOWN 270
// a mouse button was released (WM_xBUTTONUP)
#define CMN_MOUSEUP 280
// the mouse pointer was moved (WM_MOUSEMOVE)
#define CMN_MOUSEMOVE 290
// the overtype mode has changed
#define CMN_OVERTYPECHANGE 300

// 2.1
// a find operation wrapped to start/end of buffer
#define CMN_FINDWRAPPED 310

// XX
#define CMN_MARKALL 320         // Bookmark were set
#define CMN_FOUNDTEXT 330       // Text was found

// Edit commands
#define CMD_FIRST 100
#define CMD_WORDUPPERCASE (CMD_FIRST + 0)
#define CMD_WORDTRANSPOSE (CMD_FIRST + 1)
#define CMD_WORDRIGHTEXTEND (CMD_FIRST + 2)
#define CMD_WORDRIGHT (CMD_FIRST + 3)
#define CMD_WORDENDRIGHT (CMD_FIRST + 4)
#define CMD_WORDENDRIGHTEXTEND (CMD_FIRST + 5)
#define CMD_WORDLOWERCASE (CMD_FIRST + 6)
#define CMD_WORDLEFTEXTEND (CMD_FIRST + 7)
#define CMD_WORDLEFT (CMD_FIRST + 8)
#define CMD_WORDENDLEFT (CMD_FIRST + 9)
#define CMD_WORDENDLEFTEXTEND (CMD_FIRST + 10)
#define CMD_WORDDELETETOSTART (CMD_FIRST + 11)
#define CMD_WORDDELETETOEND (CMD_FIRST + 12)
#define CMD_WORDCAPITALIZE (CMD_FIRST + 13)
#define CMD_WINDOWSTART (CMD_FIRST + 14)
#define CMD_WINDOWSCROLLUP (CMD_FIRST + 15)
#define CMD_WINDOWSCROLLTOTOP (CMD_FIRST + 16)
#define CMD_WINDOWSCROLLTOCENTER (CMD_FIRST + 17)
#define CMD_WINDOWSCROLLTOBOTTOM (CMD_FIRST + 18)
#define CMD_WINDOWSCROLLRIGHT (CMD_FIRST + 19)
#define CMD_WINDOWSCROLLLEFT (CMD_FIRST + 20)
#define CMD_WINDOWSCROLLDOWN (CMD_FIRST + 21)
#define CMD_WINDOWRIGHTEDGE (CMD_FIRST + 22)
#define CMD_WINDOWLEFTEDGE (CMD_FIRST + 23)
#define CMD_WINDOWEND (CMD_FIRST + 24)
#define CMD_UPPERCASESELECTION (CMD_FIRST + 25)
#define CMD_UNTABIFYSELECTION  (CMD_FIRST + 26)
#define CMD_UNINDENTSELECTION (CMD_FIRST + 27)
#define CMD_UNDOCHANGES (CMD_FIRST + 28)
#define CMD_UNDO (CMD_FIRST + 29)
#define CMD_TABIFYSELECTION (CMD_FIRST + 30)
#define CMD_SENTENCERIGHT (CMD_FIRST + 31)
#define CMD_SENTENCELEFT (CMD_FIRST + 32)
#define CMD_SENTENCECUT (CMD_FIRST + 33)
#define CMD_SELECTSWAPANCHOR (CMD_FIRST + 34)
#define CMD_SELECTPARA (CMD_FIRST + 35)
#define CMD_SELECTLINE (CMD_FIRST + 36)
#define CMD_SELECTALL (CMD_FIRST + 37)
#define CMD_REDOCHANGES (CMD_FIRST + 38)
#define CMD_REDO (CMD_FIRST + 39)
#define CMD_PASTE (CMD_FIRST + 40)
#define CMD_PARAUP (CMD_FIRST + 41)
#define CMD_PARADOWN (CMD_FIRST + 42)
#define CMD_PAGEUPEXTEND (CMD_FIRST + 43)
#define CMD_PAGEUP (CMD_FIRST + 44)
#define CMD_PAGEDOWNEXTEND (CMD_FIRST + 45)
#define CMD_PAGEDOWN (CMD_FIRST + 46)
#define CMD_LOWERCASESELECTION (CMD_FIRST + 47)
#define CMD_LINEUPEXTEND (CMD_FIRST + 48)
#define CMD_LINEUP (CMD_FIRST + 49)
#define CMD_LINETRANSPOSE (CMD_FIRST + 50)
#define CMD_LINESTART (CMD_FIRST + 51)
#define CMD_LINEOPENBELOW (CMD_FIRST + 52)
#define CMD_LINEOPENABOVE (CMD_FIRST + 53)
#define CMD_LINEENDEXTEND (CMD_FIRST + 54)
#define CMD_LINEEND (CMD_FIRST + 55)
#define CMD_LINEDOWNEXTEND (CMD_FIRST + 56)
#define CMD_LINEDOWN (CMD_FIRST + 57)
#define CMD_LINEDELETETOSTART (CMD_FIRST + 58)
#define CMD_LINEDELETETOEND (CMD_FIRST + 59)
#define CMD_LINEDELETE (CMD_FIRST + 60)
#define CMD_LINECUT (CMD_FIRST + 61)
#define CMD_INDENTTOPREV (CMD_FIRST + 62)
#define CMD_INDENTSELECTION (CMD_FIRST + 63)
#define CMD_HOMEEXTEND (CMD_FIRST + 64)
#define CMD_HOME (CMD_FIRST + 65)
#define CMD_GOTOMATCHBRACE (CMD_FIRST + 66)
#define CMD_GOTOINDENTATION (CMD_FIRST + 67)
#define CMD_GOTOLINE (CMD_FIRST + 68)
#define CMD_FINDREPLACE (CMD_FIRST + 69)
#define CMD_REPLACE (CMD_FIRST + 70)
#define CMD_REPLACEALLINBUFFER (CMD_FIRST + 71)
#define CMD_REPLACEALLINSELECTION (CMD_FIRST + 72)
#define CMD_FINDPREVWORD (CMD_FIRST + 73)
#define CMD_FINDPREV (CMD_FIRST + 74)
#define CMD_FINDNEXTWORD (CMD_FIRST + 75)
#define CMD_FINDNEXT (CMD_FIRST + 76)
#define CMD_FINDMARKALL (CMD_FIRST + 77)
#define CMD_FIND (CMD_FIRST + 78)
#define CMD_SETFINDTEXT (CMD_FIRST + 79)
#define CMD_SETREPLACETEXT (CMD_FIRST + 80)
#define CMD_TOGGLEPRESERVECASE (CMD_FIRST + 81)
#define CMD_TOGGLEWHOLEWORD (CMD_FIRST + 82)
#define CMD_TOGGLECASESENSITIVE (CMD_FIRST + 83)
#define CMD_END (CMD_FIRST + 84)
#define CMD_TOGGLEWHITESPACEDISPLAY (CMD_FIRST + 85)
#define CMD_TOGGLEOVERTYPE (CMD_FIRST + 86)
#define CMD_SETREPEATCOUNT (CMD_FIRST + 87)
#define CMD_DOCUMENTSTARTEXTEND (CMD_FIRST + 88)
#define CMD_DOCUMENTSTART (CMD_FIRST + 89)
#define CMD_DOCUMENTENDEXTEND (CMD_FIRST + 90)
#define CMD_DOCUMENTEND (CMD_FIRST + 91)
#define CMD_DELETEHORIZONTALSPACE (CMD_FIRST + 92)
#define CMD_DELETEBLANKLINES (CMD_FIRST + 93)
#define CMD_DELETEBACK (CMD_FIRST + 94)
#define CMD_DELETE (CMD_FIRST + 95)
#define CMD_CUTSELECTION (CMD_FIRST + 96)
#define CMD_CUT (CMD_FIRST + 97)
#define CMD_COPY (CMD_FIRST + 98)
#define CMD_CHARTRANSPOSE (CMD_FIRST + 99)
#define CMD_CHARRIGHTEXTEND (CMD_FIRST + 100)
#define CMD_CHARRIGHT (CMD_FIRST + 101)
#define CMD_CHARLEFTEXTEND (CMD_FIRST + 102)
#define CMD_CHARLEFT (CMD_FIRST + 103)
#define CMD_BOOKMARKTOGGLE (CMD_FIRST + 104)
#define CMD_BOOKMARKPREV (CMD_FIRST + 105)
#define CMD_BOOKMARKNEXT (CMD_FIRST + 106)
#define CMD_BOOKMARKCLEARALL (CMD_FIRST + 107)
#define CMD_BOOKMARKJUMPTOFIRST (CMD_FIRST + 108)
#define CMD_BOOKMARKJUMPTOLAST (CMD_FIRST + 109)
#define CMD_BREAKPOINTTOGGLE (CMD_FIRST + 110)
#define CMD_BREAKPOINTPREV (CMD_FIRST + 111)
#define CMD_BREAKPOINTNEXT (CMD_FIRST + 112)
#define CMD_BREAKPOINTCLEARALL (CMD_FIRST + 113)
#define CMD_BREAKPOINTJUMPTOFIRST (CMD_FIRST + 114)
#define CMD_BREAKPOINTJUMPTOLAST (CMD_FIRST + 115)
#define CMD_APPENDNEXTCUT (CMD_FIRST + 116)
#define CMD_INSERTCHAR (CMD_FIRST + 117)
#define CMD_NEWLINE (CMD_FIRST + 118)
#define CMD_PROPERTIES (CMD_FIRST + 119)
#define CMD_BEGINUNDO (CMD_FIRST + 120)
#define CMD_ENDUNDO (CMD_FIRST + 121)
#define CMD_RESERVED3 (CMD_FIRST + 122)
#define CMD_TOGGLEREGEXP (CMD_FIRST + 123)
#define CMD_CLEARSELECTION (CMD_FIRST + 124)
#define CMD_REGEXPON (CMD_FIRST + 125)
#define CMD_REGEXPOFF (CMD_FIRST + 126)
#define CMD_WHOLEWORDON (CMD_FIRST + 127)
#define CMD_WHOLEWORDOFF (CMD_FIRST + 128)
#define CMD_PRESERVECASEON (CMD_FIRST + 129)
#define CMD_PRESERVECASEOFF (CMD_FIRST + 130)
#define CMD_CASESENSITIVEON (CMD_FIRST + 131)
#define CMD_CASESENSITIVEOFF (CMD_FIRST + 132)
#define CMD_WHITESPACEDISPLAYON (CMD_FIRST + 133)
#define CMD_WHITESPACEDISPLAYOFF (CMD_FIRST + 134)
#define CMD_OVERTYPEON (CMD_FIRST + 135)
#define CMD_OVERTYPEOFF (CMD_FIRST + 136)
#define CMD_TOGGLEALLOPENED (CMD_FIRST + 137)
#define CMD_LAST (CMD_FIRST + 138)

// all register commands must be at or higher than CMD_USER_BASE
#define CMD_USER_BASE (CMD_FIRST + 900)

// general failure
#define CMDERR_FAILURE 1
// bad input
#define CMDERR_INPUT 2
// bad selection
#define CMDERR_SELECTION 3
// data not found
#define CMDERR_NOTFOUND 4
// buffer is empty
#define CMDERR_EMPTYBUF 5
// buffer is read-only
#define CMDERR_READONLY 6

#define CMDERR_NOTFOUND_FORWARD 7
#define CMDERR_NOTFOUND_BACKWARD 8
#define CMDERR_NOTFOUND_FORWARD_MARKALL 9
#define CMDERR_NOTFOUND_FORWARD_REPLACEALL 10

// language styles
#define CMLS_PROCEDURAL 0
#define CMLS_SGML 1

// Font style options used in CM_FONTSTYLES
// normal weight
#define CM_FONT_NORMAL 0
// bold weight
#define CM_FONT_BOLD 1
// normal weight, italic
#define CM_FONT_ITALIC 2
// bold weight, italic
#define CM_FONT_BOLDITALIC 3
// normal weight, underline
#define CM_FONT_UNDERLINE 4

// AutoIndent options
// auto-indent off -- new line begins at column 0
#define CM_INDENT_OFF 0
// new line begins at correct language scope indentation level
#define CM_INDENT_SCOPE 1
// new line has identical indentation of previous line
#define CM_INDENT_PREVLINE 2

// Print option flags used with CMM_PRINT
// display the print common dialog
#define CM_PRINT_PROMPTDLG 0x0
// use default printer (no print dialog displayed)
#define CM_PRINT_DEFAULTPRN 0x1
// use HDC provided
#define CM_PRINT_HDC 0x2
// use bold, italics, underline, etc. when appropriate
#define CM_PRINT_RICHFONTS 0x4
// print in color
#define CM_PRINT_COLOR 0x8
// print 'page # of #' at the bottom of the page
#define CM_PRINT_PAGENUMS 0x10
// print date and time at top of the page
#define CM_PRINT_DATETIME 0x20
// surround text with a thin border
#define CM_PRINT_BORDERTHIN 0x40
// surround text with a thick border
#define CM_PRINT_BORDERTHICK 0x80
// surround text with two thin borders
#define CM_PRINT_BORDERDOUBLE 0x100
// print the selection rather than entire edit contents
#define CM_PRINT_SELECTION 0x200

// Border option flags used with CMM_GETBORDERSTYLE and CMM_SETBORDERSTYLE.
// Note: this values may be or'd together to achieve different effects.
// no border
#define CM_BORDER_NONE 0
// 1-pixel border
#define CM_BORDER_THIN 1
// client edge (WS_EX_CLIENTEDGE)
#define CM_BORDER_CLIENT 2
// static edge (WS_EX_STATICEDGE)
#define CM_BORDER_STATIC 4
// modal edge (WS_EX_DLGMODALFRAME)
#define CM_BORDER_MODAL 8
#define CM_BORDER_CORRAL (CM_BORDER_MODAL | CM_BORDER_CLIENT)

// CMN_KEYDOWN, CMN_KEYUP, CMN_KEYPRESS notification
// data passed to parent window
#define CM_KEY_SHIFT 0x1
#define CM_KEY_CTRL 0x2
#define CM_KEY_ALT 0x4

// CMN_MOUSEDOWN, CMN_MOUSEUP, CMN_MOUSEPRESS notification
// data passed to parent window
#define CM_BTN_LEFT 0x1
#define CM_BTN_RIGHT 0x2
#define CM_BTN_MIDDLE 0x4

// data passed to CMM_SETLINENUMBERING
#define CM_BINARY 2
#define CM_OCTAL 8
#define CM_DECIMAL 10
#define CM_HEXADECIMAL 16

// Error codes
// function or method completed successfully
#define CME_SUCCESS 1
// function or method did not complete because of an error
#define CME_FAILURE 0
// function or method did not complete because an invalid argument was passed in
#define CME_BADARGUMENT -1

// CMM_HITTEST return codes
// Not over the CodeMax control
#define CM_NOWHERE 0
// Over the horizontal splitter bar
#define CM_HSPLITTER 1
// Over the vertical splitter bar
#define CM_VSPLITTER 2
// Over the intersection of the horizontal and vertical splitter bar
#define CM_HVSPLITTER 3
// Over the buffer contents (code)
#define CM_EDITSPACE 4
// Over the horizontal scrollbar
#define CM_HSCROLLBAR 5
// Over the vertical scrollbar
#define CM_VSCROLLBAR 6
// Over the sizebox visible when both scrollbars are visible
#define CM_SIZEBOX 7
// Over the left margin area
#define CM_LEFTMARGIN 8

// Exported functions
#define CM_VERSION 0x2100

#endif
