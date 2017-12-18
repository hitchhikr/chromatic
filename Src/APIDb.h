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
// APIDb.h: declaration of used variables / structures for APIDb.cpp
// -----------------------------------------------------------------------

#ifndef _APIDB_H_
#define _APIDB_H_

// -----------------------------------------------------------------------
// Variables
extern char *APiLib;
extern char *APIName;
extern CStr APIFuckStrLib;
extern CStr APIFuckStrName;
extern long *APIBlockLib;
extern long *APIBlockName;
extern long *APIInfosLib;
extern long *APIInfosName;
extern CStr LibToFind;
extern long APIX;
extern long APIY;
extern CStr APIFnc;
extern CStr APILineFnc;
extern char *APIDescription;
extern CStr APIFuckStr;
extern long *APIBlock;
extern long APIFound;
extern long *APIInfos;
extern long *APIArgs;
extern long MaxAPIArgs;
extern CStr APIString;
extern HWND APIhText;
extern long *APIArrParse;
extern HFONT APIhFont;
extern HFONT APIhFontBold;
extern long APICurHiglight;
extern CStr APIFile;
extern CStr APIFncFile;
extern CStr APIConstFile;
extern HWND APIhListBox;
extern long APIOldRealPosition;
extern long APILineOldRealPosition;
extern long APINoRedraw;
extern long HighOn;
extern long HighPar;
extern long DbLinesNumbers;
extern long UseDbFnc;
extern long UseDbConst;

// -----------------------------------------------------------------------
// Functions declarations
CStr RetrieveAPI(void);
CStr DisplayAPI(long dx, long dy);
CStr RetrieveAPIName(CStr APIFnc);
void DisplayAPIWindow(long dx, long dy, HWND hWnd, CStr APIFnc);

#endif
