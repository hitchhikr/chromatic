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
// Objects_Handler.cpp: Read objects tables and handle the objects stack
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Objects_Handler.h"

// -----------------------------------------------------------------------
// Handles a method table:
// __gc
// __index
// __newindex
// ...methods...
// ...properties...
// NULL
void Object_Request(lua_State *L, luaL_reg *Table, char *Name,
                    void (* Read_Fnc)(lua_State *, int)
                   )
{
    int i;
    int Idx;
    int found;

    const char *key = luaL_checkstring(L, 2);

    luaL_checktype(L, 1, LUA_TUSERDATA);

    void **Value = (void **) lua_touserdata(L, 1);

    // Check if the method is in the metatable
    lua_getmetatable(L, 1);
    lua_getfield(L, -1, key);
    if(!lua_isnil(L, -1))
    {
        lua_pop(L, 1);

        i = 3;

        // Functions
        while(Table[i].name)
        {
            if(strcmp(Table[i].name, key) == 0
               && Table[i].func)
            {
                // Save ourself and run the function
                Push_Class_Object(L, *Value);
                lua_pushcfunction(L, Table[i].func);
                return;
            }
            i++;
        }

        // Variables
        Idx = 0;
        i = 3;
        found = 0;
        while(Table[i].name)
        {
            if(strcmp(Table[i].name, key) == 0
               && !Table[i].func)
            {
                found = 1;
                break;
            }
            if(!Table[i].func) Idx++;
            i++;
        }
        if(found)
        {
            Push_Class_Object(L, *Value);
            Read_Fnc(L, Idx);
        }
        else
        {
            // Return the object (i don't think that can ever happen)
            lua_pushlightuserdata(L, Value);
        }
    }
    else
    {
        luaL_error(L, "unknown method or property");
    }
}

// -----------------------------------------------------------------------
// Save an element into the stack
void Push_Class_Object(lua_State *L, void *Value)
{
    L->Obj_Stack[L->Obj_Stack_Pos] = Value;
    L->Obj_Stack_Pos++;
    if(L->Obj_Stack_Pos == OBJ_STACK_SIZE)
    {
        luaL_error(L, "object stack overflow");
    }
}

// -----------------------------------------------------------------------
// Save an element into the stack
void *Pop_Class_Object(lua_State *L)
{
    if(L->Obj_Stack_Pos == 0)
    {
        luaL_error(L, "object stack underflow");
        return(NULL);
    }
    L->Obj_Stack_Pos--;
    return(L->Obj_Stack[L->Obj_Stack_Pos]);
}

// -----------------------------------------------------------------------
// Hook
// Can be re-used for all controls i think
LRESULT CALLBACK Object_WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    int Func = GetWindowLong(hWnd, 8);
    lua_State *L = (lua_State *) GetWindowLong(hWnd, 12);

    if(Func && L)
    {
        lua_rawgeti(L, LUA_REGISTRYINDEX, Func);
        lua_pushinteger(L, (int) hWnd);
        lua_pushinteger(L, uMsg);
        lua_pushinteger(L, wParam);
        lua_pushinteger(L, lParam);
        // We need to use pcall here (call isn't safe)
        if(lua_pcall(L, 4, 0, 0))
        {
            PrintScriptError(L);
        }
        return(ControlNextHook(hWnd, uMsg, wParam, lParam));
    }
    return(ControlNextHook(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Retrieve a global or local variable
int Read_Ref_Argument(lua_State *L, int idx, lua_Debug *local, int NewString)
{
    char *Variable_Name = (char *) luaL_checkstring(L, idx);
    char *local_name;
    int i;
    int j;

Read_Again:
    memset(local, 0, sizeof(lua_Debug));
    // See if it's a local variable
    i = 0;
    while(lua_getstack(L, i, local))
    {
        j = 1;
        while(local_name = (char *) lua_getlocal(L, local, j))
        {
            if(strcmp(Variable_Name, local_name) == 0)
            {
                // This is the variable
                return(-j);
            }
            // Nope, remove it from the stack
            lua_pop(L, 1);
            j++;
        }
        i++;
    }
    // See if it's a global one
    lua_getfield(L, LUA_GLOBALSINDEX, Variable_Name);
    local->name = Variable_Name;
    if(lua_isnil(L, -1))
    {
        // Didn't find it: create it as a global variable.
        lua_pop(L, 1);

        if(NewString)
        {
            lua_pushstring(L, "");
            lua_setglobal(L, Variable_Name);
        }
        else
        {
            lua_pushnumber(L, 0);
            lua_setglobal(L, Variable_Name);
        }
        goto Read_Again;
    }
    else
    {
        return(idx);
    }
}

// -----------------------------------------------------------------------
// Write the current stack value into global or local variable
void Write_Ref_Variable(lua_State *L, int idx, lua_Debug *local)
{
    if(idx >= 0)
    {
        // Global variable
        lua_setfield(L, LUA_GLOBALSINDEX, local->name);
    }
    else
    {
        // Local one
        lua_setlocal(L, local, -idx);
    }
}
