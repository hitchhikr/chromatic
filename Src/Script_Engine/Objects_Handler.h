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
// Objects_Handler.h: declaration of used variables / structures for Objects_Handler.cpp
// -----------------------------------------------------------------------

#ifndef _OBJECTS_HANDLER_H_
#define _OBJECTS_HANDLER_H_

// -----------------------------------------------------------------------
// Includes
#include "Scripts.h"

// -----------------------------------------------------------------------
// Constants
#define OBJ_STACK_SIZE 1024

// -----------------------------------------------------------------------
// Functions
void Object_Request(lua_State *L, luaL_reg *Table, char *Name,
                    void (* Read_Fnc)(lua_State *, int));
void Push_Class_Object(lua_State *L, void *Value);
void *Pop_Class_Object(lua_State *L);
LRESULT CALLBACK Object_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int Read_Ref_Argument(lua_State *L, int idx, lua_Debug *local, int NewString);
void Write_Ref_Variable(lua_State *L, int idx, lua_Debug *local);

#endif
