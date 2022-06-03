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
// DockingBox.cpp: Class wrapper for the script engine
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "../Objects_Handler.h"
#include "DockingBox.h"

namespace OBJECT_NAME
{

// -----------------------------------------------------------------------
// Retrieve class values
void Read_Value(lua_State *L, int Index)
{
    int *Struct = (int *) Pop_Class_Object(L);

    switch(Index)
    {
        case 0:
            lua_pushinteger(L, Struct[Index]);
            break;
        case 1:
            lua_pushinteger(L, (long) DockingBoxGetState((HWND) Struct[0]));
            break;
    }
}

// -----------------------------------------------------------------------
int Remove(lua_State *L)
{
    DockingBoxRemove((HWND) Pop_Class_Object(L));
    return(1);
}

// -----------------------------------------------------------------------
int WasVisible(lua_State *L)
{
    Pop_Class_Object(L);
    lua_pushinteger(L, (long) Wrapper_DockingBoxWasVisible((char *) luaL_checkstring(L, 1)));
    return(1);
}

// -----------------------------------------------------------------------
int Show(lua_State *L)
{
    Wrapper_DockingBoxShow((HWND) Pop_Class_Object(L),
                           (long) luaL_checkint(L, 1),
                           (long) luaL_checkint(L, 2),
                           (long) luaL_checkint(L, 3)
                           );
    return(1);
}

// -----------------------------------------------------------------------
// New method
int Allocate(lua_State *L)
{
    int *Value;
    void *Func;

    luaL_checktype(L, 2, LUA_TFUNCTION);
    lua_pushvalue(L, 2);
    Func = (void *) luaL_ref(L, LUA_REGISTRYINDEX);

    Value = (int *) lua_newuserdata(L, sizeof(void *));

    *Value = (int) Wrapper_CreateDockingBox((char *) luaL_checkstring(L, 1), &Object_WindowProc);
    if(*Value)
    {
        luaL_getmetatable(L, CLASS_NAME);
        lua_setmetatable(L, -2);
        // Store our lua function here
        // (the class is register with 12 bytes extra)
        SetWindowLong((HWND) *Value, 8, (long) Func);
        // Store the script context
        SetWindowLong((HWND) *Value, 12, (long) L);
    }

    return(1);
}

// -----------------------------------------------------------------------
// Garbage collector
int Release(lua_State *L)
{
    int *Value = (int *) lua_touserdata(L, 1);
    int Func = GetWindowLong((HWND) *Value, 8);

    // Avoid the mess with WM_CLOSE
    // I know it's ugly but lua was fucking with the windows messages
    SetWindowLong((HWND) *Value, 8, (long) 0);
    DockingBoxRemove((HWND) *Value);
    ControlClose((HWND) *Value);
    DestroyWindow((HWND) *Value);
    luaL_unref(L, LUA_REGISTRYINDEX, Func);

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

    { "Remove", Remove },
    { "Show", Show },
    { "WasVisible", WasVisible },

    { "hWnd", 0 },
    { "State", 0 },
    { "WasVisible", 0 },

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
