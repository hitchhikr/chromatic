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
// Now.cpp: Class wrapper for the script engine
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "../Objects_Handler.h"
#include "Now.h"

namespace OBJECT_NAME
{

// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Retrieve class values
void Read_Value(lua_State *L, int Index)
{
    int *Struct = (int *) Pop_Class_Object(L);

    switch(Index)
    {
        case 0:
            lua_pushstring(L, Wrapper_DateGetYear());
            break;
        case 1:
            lua_pushstring(L, Wrapper_DateGetMonth());
            break;
        case 2:
            lua_pushstring(L, Wrapper_DateGetDay());
            break;
        case 3:
            lua_pushstring(L, Wrapper_DateGetHour());
            break;
        case 4:
            lua_pushstring(L, Wrapper_DateGetMinutes());
            break;
        case 5:
            lua_pushstring(L, Wrapper_DateGetSeconds());
            break;
    }
}

// -----------------------------------------------------------------------
int GetNow(lua_State *L)
{
    Pop_Class_Object(L);

    lua_pushstring(L, Wrapper_DateGetNow(luaL_checkinteger(L, 1)));
    return(1);
}

// -----------------------------------------------------------------------
int GetDateFormat(lua_State *L)
{
    Pop_Class_Object(L);

    lua_pushstring(L, Wrapper_DateGetFormat((char *) luaL_checkstring(L, 1)));
    return(1);
}

// -----------------------------------------------------------------------
int GetTimeFormat(lua_State *L)
{
    Pop_Class_Object(L);

    lua_pushstring(L, Wrapper_DateGetTimeFormat((char *) luaL_checkstring(L, 1)));
    return(1);
}

// -----------------------------------------------------------------------
// New method
int Allocate(lua_State *L)
{
    int *Value;

    luaL_checktype(L, 1, LUA_TNONE);
    Value = (int *) lua_newuserdata(L, sizeof(void *));

    luaL_getmetatable(L, CLASS_NAME);
    lua_setmetatable(L, -2);
    return(1);
}

// -----------------------------------------------------------------------
// Garbage collector
int Release(lua_State *L)
{
    int *Value = (int *) lua_touserdata(L, 1);

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

    { "GetNow", GetNow },
    { "GetDateFormat", GetDateFormat },
    { "GetTimeFormat", GetTimeFormat },

    { "Now", 0 },
    { "Year", 0 },
    { "Month", 0 },
    { "Day", 0 },
    { "Hour", 0 },
    { "Minutes", 0 },
    { "Seconds", 0 },

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
