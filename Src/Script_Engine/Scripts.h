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
// Scripts.h: declaration of used variables / structures for Scripts.cpp
// -----------------------------------------------------------------------

#ifndef _SCRIPTS_H_
#define _SCRIPTS_H_

// -----------------------------------------------------------------------
// Includes
#include "../Classes/WALib.h"
#include "../Classes/WALib_WinSock.h"
#include "../Classes/WADock.h"
#include "../AddIns.h"
#include "../Globals.h"
#include "../ConstRes.h"
#include "../Splitters.h"
#include "../MDI_Form.h"
#include "../ConstMenus.h"
#include "../Debugger.h"
#include "../ConstCodeMax.h"

extern "C"
{
    #include "Lua514/src/lua.h"
    #include "Lua514/src/lauxlib.h"
    #include "Lua514/src/lualib.h"
    #include "Lua514/src/lstate.h"
}

// -----------------------------------------------------------------------
// Functions declarations
int AllocScriptEngine(void);
void ReleaseScriptEngine(void);
long InsertScriptCode(char *ScriptName, char *SnippetVal);
long RunScriptFile(CStr FileName, CStr Arguments);
void PrintScriptError(lua_State *L);

void Push_Class_Object(lua_State *L, void *Value);
void *Pop_Class_Object(lua_State *L);

#endif
