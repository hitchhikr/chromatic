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
// BackDC.cpp: Class wrapper for the script engine
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "../Objects_Handler.h"
#include "BackDC.h"

namespace OBJECT_NAME
{

// -----------------------------------------------------------------------
// Retrieve class values
void Read_Value(lua_State *L, int Index)
{
    int *Struct = (int *) Pop_Class_Object(L);

    lua_pushinteger(L, Struct[Index]);
}

// -----------------------------------------------------------------------
// New method
int Allocate(lua_State *L)
{
    int *Value;

    luaL_checktype(L, 1, LUA_TNONE);
    Value = (int *) lua_newuserdata(L, sizeof(void *));
    *Value = (int) malloc(sizeof(BACKDC));
    memset((void *) *Value, 0, sizeof(BACKDC));
    luaL_getmetatable(L, CLASS_NAME);
    lua_setmetatable(L, -2);
    return(1);
}

// -----------------------------------------------------------------------
// Garbage collector
int Release(lua_State *L)
{
    int *Value = (int *) lua_touserdata(L, 1);
    free((void *) *Value);
    return(0);
}

// -----------------------------------------------------------------------
// Tables
int Reading(lua_State *L);
int Writing(lua_State *L);

luaL_reg Methods[] =
{
    { "New", Allocate },
    { NULL, NULL }
};

luaL_reg Meta[] =
{
    { "__gc", Release },
    { "__index", Reading },
    { "__newindex", Writing },

    { "hWnd", 0 },
    { "Color", 0 },
    { "hDC", 0 },
    { "Bitmap", 0 },
    { "OldObject", 0 },

    { NULL, NULL }
};

// -----------------------------------------------------------------------
// Members reading from the table
int Reading(lua_State *L)
{
    Object_Request(L, Meta, CLASS_NAME, &Read_Value);
    return(1);
}

// -----------------------------------------------------------------------
// Members writing
int Writing(lua_State *L)
{
    luaL_error(L, "property is read only");
    return(1);
}

// -----------------------------------------------------------------------
// Installation
void Install(lua_State *L)
{
    luaL_newmetatable(L, CLASS_NAME);
    luaL_openlib(L, NULL, Meta, 0);
    luaL_openlib(L, CLASS_NAME, Methods, 0);
}

}  // namespace
