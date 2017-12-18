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
// Dlg_UserDialogsObjects.h: declaration of used variables / structures for Dlg_UserDialogsObjects.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_USERDIALOGSOBJECTS_H_
#define _DLG_USERDIALOGSOBJECTS_H_

// -----------------------------------------------------------------------
// Constants
#define OBJECT_TYPE 0
#define OBJECT_DIMROUTINELEFT 1
#define OBJECT_DIMROUTINELEFTPOS 2
#define OBJECT_DIMROUTINETOP 3
#define OBJECT_DIMROUTINETOPPOS 4
#define OBJECT_DIMROUTINEWIDTH 5
#define OBJECT_DIMROUTINEWIDTHPOS 6
#define OBJECT_DIMROUTINEHEIGHT 7
#define OBJECT_DIMROUTINEHEIGHTPOS 8
#define OBJECT_HWND 9
#define OBJECT_HWNDCHILD 10
#define OBJECT_HWNDCHILDCHILD 11
#define OBJECT_FLAGS 12
#define OBJECT_PROPNBR 13
#define OBJECT_LEN 14

#define PROPERTY_NAME 0
#define PROPERTY_TYPE 1
#define PROPERTY_STORAGE 2
#define PROPERTY_FLAGS 3
#define PROPERTY_EXTEND 4
#define PROPERTY_ROUTINE 5
#define PROPERTY_LOCKED 6
#define PROPERTY_NULL 7
#define PROPERTY_LEN 8

#define OBJECT_FLAG_REBAR 1

// -----------------------------------------------------------------------
// Functions declarations
int *AddObjectEntry(long ObjType, long NbrProperties, long LeftR, long LeftRPos, long TopR,
                    long TopRPos, long WidthR, long WidthRPos, long HeightR, long HeightRPos,
                    HWND hWnd, HWND hWndChild, HWND hWndChildChild, int Flags);
void StorePropertyHeader(long PropNumber, int *hMem, CStr PropName, long PropType, long PropFlags,
                         long PropExtend, long PropRoutine, long PropLocked, CStr PropStringStore);
long GetObjectDatas(int *hMem, long Index);
long GetPropertyDatas(int *hMem, long PropNumber, long PropIndex);
void SetPropertyDatas(int *hMem, long PropNumber, long PropIndex, long Datas);
long GetControlIndex(HWND hWnd);
void EraseControl(long ControlIndex);
void EraseControlsList(void);
void HideGripSelection(HWND hWnd);
void ShowGripSelection(HWND hWnd);
void UnSelectControl(HWND hWnd);
void UnSelectControlsList(void);
void SelectControl(HWND hWnd, long Left, long Top, long Width, long Height);
void ResizeControlFromGripBox(LPGRIPBOXDAT GripDats, LPRECT NewSizeRect);
void ResizeGripBoxFromControl(HWND hWnd, LPRECT NewSizeRect);
HWND GetControlFromGripBox(LPGRIPBOXDAT hGrip);
LPGRIPBOXDAT GetGripBoxFromControl(HWND hControl);
LPRECT GetRectSelectionFromControl(HWND hControl);
int *GetCaptureStatusFromControl(HWND hControl);

#endif
