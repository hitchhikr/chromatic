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
// CommonDialog.cpp: Class wrapper for the script engine
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "../Objects_Handler.h"
#include "CommonDialog.h"

namespace OBJECT_NAME
{

// -----------------------------------------------------------------------
// Variables
long *FilesArray;

// -----------------------------------------------------------------------
// Retrieve class values
void Read_Value(lua_State *L, int Index)
{
    int *Struct = (int *) Pop_Class_Object(L);

    lua_pushinteger(L, Struct[Index]);
}

// -----------------------------------------------------------------------
// ChooseColor
int ChooseColor(lua_State *L)
{
    Pop_Class_Object(L);

    int Return_Value;
    int Color;
    int Var_Result;
    lua_Debug local;

    Var_Result = Read_Ref_Argument(L, 2, &local, FALSE);
    if(Var_Result == 0) return(1);

    // Get the value
    Color = lua_tointeger(L, -1);
    lua_pop(L, 1);

    Return_Value = Wrapper_ComDlgChooseColor((HWND) luaL_checkint(L, 1), (long *) &Color);

    lua_pushinteger(L, Color);
    Write_Ref_Variable(L, Var_Result, &local);

    lua_pushboolean(L, Return_Value);
    return(1);
}

// -----------------------------------------------------------------------
// GetOpenFileName
int GetOpenFileName(lua_State *L)
{
    Pop_Class_Object(L);

    int i;
    CStr Ret;
    CStr TempRet;
    int Multi = luaL_checkboolean(L, 4);

    Ret = Wrapper_ComDlgGetOpenFileName((HWND) luaL_checkint(L, 1),
                                          (char *) luaL_checkstring(L, 2),
                                          (char *) luaL_checkstring(L, 3),
                                          Multi);

    if(Multi)
    {
        if(Ret.Len())
        {
            FilesArray = StringSplit(Ret, Ret.Chr(2));

            // In case of multi selection, we return an array
            lua_newtable(L);
            for(i = 0; i <= StringGetSplitUBound(FilesArray); i++)
            {
                TempRet = StringReplace(StringGetSplitElement(Ret, FilesArray, i), Ret.Chr(2), " ", 1, -1, Binary_Compare);
                
                lua_pushstring(L, StringCStrConvertToString(TempRet));
                lua_rawseti(L, -2, i);
            }
        }
    }
    else
    {
        lua_pushstring(L, StringCStrConvertToString(Ret));
    }
    return(1);
}

// -----------------------------------------------------------------------
// GetSaveFileName
int GetSaveFileName(lua_State *L)
{
    Pop_Class_Object(L);

    lua_pushstring(L, Wrapper_ComDlgGetSaveFileName((HWND) luaL_checkint(L, 1),
                                                      (char *) luaL_checkstring(L, 2),
                                                      (char *) luaL_checkstring(L, 3)
                                                     ));
    return(1);
}

// -----------------------------------------------------------------------
// BrowseForFolder
int BrowseForFolder(lua_State *L)
{
    Pop_Class_Object(L);

    lua_pushstring(L, Wrapper_ComDlgBrowseForFolder((HWND) luaL_checkint(L, 1),
                                                      (char *) luaL_checkstring(L, 2)
                                                     ));
    return(1);
}

// -----------------------------------------------------------------------
// ChooseFont
int ChooseFont(lua_State *L)
{
    Pop_Class_Object(L);
    LOGFONT LogFont;
    CHOOSEFONT Font;
    int result;
    int i;
    CStr FName;
    char *FontName;
    int FontSize;
    int Var_Name_Result;
    int Var_Size_Result;
    lua_Debug local_Name;
    lua_Debug local_Size;

    Var_Name_Result = Read_Ref_Argument(L, 5, &local_Name, TRUE);
    if(Var_Name_Result == 0) return(1);

    // Get the value
    FontName = (char *) lua_tostring(L, -1);
    lua_pop(L, 1);

    Var_Size_Result = Read_Ref_Argument(L, 6, &local_Size, FALSE);
    if(Var_Size_Result == 0) return(1);

    // Get the value
    FontSize = lua_tointeger(L, -1);
    lua_pop(L, 1);

    result = Wrapper_ComDlgChooseFont((HWND) luaL_checkint(L, 1),
                                        (char *) luaL_checkstring(L, 2),
                                        luaL_checkint(L, 3),
                                        luaL_checkboolean(L, 4),
                                        &LogFont, &Font
                                       );

    for(i = 0; i <= LF_FACESIZE - 1; i++)
    {
        if(LogFont.lfFaceName[i] == 0) break;
        FName = FName + FName.Chr(LogFont.lfFaceName[i]);
    }
    
    FontName = StringCStrConvertToString(FName);
    FontSize = Font.iPointSize / 10;

    lua_pushstring(L, FontName);
    Write_Ref_Variable(L, Var_Name_Result, &local_Name);

    lua_pushinteger(L, FontSize);
    Write_Ref_Variable(L, Var_Size_Result, &local_Size);

    lua_pushboolean(L, result);
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

    { "ChooseColor", ChooseColor },
    { "GetOpenFileName", GetOpenFileName },
    { "GetSaveFileName", GetSaveFileName },
    { "BrowseForFolder", BrowseForFolder },
    { "ChooseFont", ChooseFont },

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
