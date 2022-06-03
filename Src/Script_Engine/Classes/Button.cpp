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
// Button.cpp: Class wrapper for the script engine
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "../Objects_Handler.h"
#include "Button.h"

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
            lua_pushinteger(L, (long) ButtonGetState((HWND) Struct[0]));
            break;
    }
}

// -----------------------------------------------------------------------
int SetIcon(lua_State *L)
{
    lua_pushinteger(L, ButtonSetIcon((HWND) Pop_Class_Object(L),
                                     (long) luaL_checkint(L, 1),
                                     (HICON) luaL_checkint(L, 2)
                                     ));
    return(1);
}

// -----------------------------------------------------------------------
int GetImage(lua_State *L)
{
    lua_pushinteger(L, (int) ButtonGetImage((HWND) Pop_Class_Object(L),
                                            (long) luaL_checkint(L, 1)
                                            ));
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

/*    HWND CALLBACK Wrapper_CreateButton(long Wrapper_WABLeft, long Wrapper_WABTop,
                                         long Wrapper_WABWidth, long Wrapper_WABHeight,
                                         HWND Wrapper_WAhParent, char *Wrapper_WABText,
                                         long Wrapper_WACtrlID, long Wrapper_WAImgType,
                                         HICON Wrapper_WAhImage, WNDPROC Wrapper_WAWinProc,
                                         long Wrapper_WAExtraStyle, long Wrapper_WAExtraExStyle)
*/
    *Value = (int) Wrapper_CreateButton(luaL_checkint(L, 1), luaL_checkint(L, 2),
                                        luaL_checkint(L, 3), luaL_checkint(L, 4),
                                        (HWND) luaL_checkint(L, 5), (char *) luaL_checkstring(L, 6),
                                        luaL_checkint(L, 7), luaL_checkint(L, 8),
                                        (HICON) luaL_checkint(L, 9), &Object_WindowProc,
                                        luaL_checkint(L, 10), luaL_checkint(L, 11)
                                       );
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

    { "SetIcon", SetIcon },
    { "GetImage", GetImage },

    { "hWnd", 0 },
    { "State", 0 },

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
