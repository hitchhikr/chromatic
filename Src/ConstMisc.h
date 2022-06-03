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
// ConstMisc.h: Misc. constants
// -----------------------------------------------------------------------

#ifndef _CONSTMISC_H_
#define _CONSTMISC_H_

// Used by tray icon
#define WM_SHELLNOTIFY WM_USER + 5

// Internal message to remove API listbox when escape key is pressed
#define WM_URGENTKILLCLIST WM_USER + 911

#define WM_FIND_FORWARD_WRAP WM_USER + 912
#define WM_FIND_BACKWARD_WRAP WM_USER + 913
#define WM_FINDMARKALL_WRAP WM_USER + 914
#define WM_FINDREPLACEALL_WRAP WM_USER + 915
#define WM_FOUND_TEXT WM_USER + 916

// -----------------------------------------------------------------------
// FTP Manager constants
#define WM_MAINSOCKET WM_USER + 500
#define WM_DIRSOCKET WM_USER + 501
#define WM_DOWNLOADSOCKET WM_USER + 502
#define WM_UPLOADSOCKET WM_USER + 503

#endif
