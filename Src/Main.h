// -----------------------------------------------------------------------
// Chromatic
// Integrated Development Environment
//
// Copyright (C) 2001-2022 Franck Charlet.
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
// Main.h: declaration of used variables / structures for Main.cpp
// -----------------------------------------------------------------------

#ifndef _MAIN_H_
#define _MAIN_H_

// -----------------------------------------------------------------------
// Variables
extern CStr StartupRetVal;
extern HACCEL hGlobAccelerators;
extern LPTOP_LEVEL_EXCEPTION_FILTER OldExceptFilter;

// -----------------------------------------------------------------------
// Functions declarations
void InitAppVars(void);
void InitAppArrays(void);
void InstallExceptFilter(void);
long CALLBACK MainFilter(EXCEPTION_POINTERS *Filter);
void CheckDirectories(void);
void CheckStockDirectory(CStr DirectoryName);

#endif
