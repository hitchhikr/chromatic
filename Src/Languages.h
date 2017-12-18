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
// Languages.h: declaration of used variables / structures for Languages.cpp
// -----------------------------------------------------------------------

#ifndef _LANGUAGES_H_
#define _LANGUAGES_H_

// -----------------------------------------------------------------------
// Includes
#include "MsgCodeMax.h"

// -----------------------------------------------------------------------
// Variables
extern CList <CM_LANGUAGE *> LanguageToRegister;

extern CStr LangName;
extern CStr LangCopyRight;
extern CStr LangComment;
extern long LangCaseSensitive;
extern CStr LangSingleComment;
extern CStr LangMultiPrefix;
extern CStr LangMultiLineComment1;
extern CStr LangMultiLineComment2;
extern CStr LangScopeKeyWord1;
extern CStr LangScopeKeyWord2;
extern CStr LangStringsDelimiters;
extern CStr LangEscapeChar;
extern CStr LangTerminatorChar;
extern long LangStyle;

extern CStr LangColorA;
extern CStr LangColorB;
extern CStr LangColorC;
extern CStr LangColorD;
extern CStr LangColorE;
extern CStr LangColorF;
extern CStr LangColorG;
extern CStr LangColorH;

extern CStr LangHelpA;
extern CStr LangHelpB;
extern CStr LangHelpC;
extern CStr LangHelpD;
extern CStr LangHelpE;
extern CStr LangHelpF;
extern CStr LangHelpG;
extern CStr LangHelpH;

extern CStr LangExtensions;
extern CStr LangProc;
extern CStr LangEndp;
extern CStr LangGUID;
extern CStr LangInclude;
extern CStr LangIncludeName;
extern CStr LangIncludeNameFoot;
extern CStr LangInterlaceProc;
extern CStr LangOpenBlockProc;
extern CStr LangCloseBlockProc;
extern CStr LangOphanBlock;
extern CStr LangDoubleSlash;
extern CStr LangFileName;
extern CStr LangHighLightNbr;

extern CStr LangFileToRead;

// -----------------------------------------------------------------------
// Functions declarations
void ReadLangSpec(void);
void Check_Update_Language(void);
void Update_All_Languages(void);
void Update_Language(CStr Language_FileName);
void LoadLanguageDef(void);
void RegisterCodeMaxLanguage(int Update_Idx);
CM_LANGUAGE *GetLangDat(long LangIndex);
CStr GetLanguageToOpen(CStr LFileName);
long GetLanguageOffset(CStr LanguageName);
CStr GetCMLangC(HWND hwnd);
CStr GetCMLangGUID(HWND hwnd);
CStr GetCMLangInclude(HWND hwnd);
CStr GetCMLangDoubleSlash(HWND hwnd);
CStr GetCMLangComment(HWND hwnd);
CStr GetCMLangHexType(HWND hwnd, int Amount);
CStr GetCMLangBinType(HWND hwnd, int Amount);
CStr GetCMLangMultiComment(HWND hwnd);
CStr GetCMLangMultiCommentByChild(HWND hwnd);
CStr GetCMLangMultiCommentByFile(CStr FileName);
CStr GetCMLangMultiCommentEnd(HWND hwnd);
CStr GetCMLangMultiCommentEndByChild(HWND hwnd);
CStr GetCMLangMultiCommentEndByFile(CStr FileName);
CStr GetCMLangProc(HWND hwnd);
CStr GetCMLangNameByChild(HWND hwnd);
CStr GetCMChildLangName(HWND hwnd);
CStr GetCMLangEndp(HWND hwnd);
long GetCMLangProcInterlace(HWND hwnd);
long GetCMLangProcOrphan(HWND hwnd);
CStr GetCMLangProcOpenBlock(HWND hwnd);
CStr GetCMLangProcCloseBlock(HWND hwnd);
CStr GetCMLangFileName(HWND hwnd);
CStr GetCMLangProcByFile(CStr FPName);
CStr GetCMLangProcCloseByFile(CStr FPName);
CStr GetCMLangIncludeNameByFile(CStr FPName);
CStr GetCMLangLangNameByFile(CStr FPName);
CStr GetCMLangIncludeNameFootByFile(CStr FPName);
CStr GetCMLangDoubleslashByFile(CStr FPName);
CStr GetCMLangIncludeNameByhWnd(HWND hwnd);
CStr GetCMLangIncludeNameFootByhWnd(HWND hwnd);
CStr GetCMLangCommentByFile(CStr FPName);

#endif
