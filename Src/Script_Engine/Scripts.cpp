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
// Scripts.cpp: Scripts interpreter.
// -----------------------------------------------------------------------
// TODO: Convert everything and try to make addins
//       (will need some modif of the addins manager)
//       and wizards to run.

// -----------------------------------------------------------------------
// Compiler directives
#pragma warning(disable:4530) // Can use exceptions handler without warning

// -----------------------------------------------------------------------
// Includes
#include "Scripts.h"

#include "Objects_Handler.h"
#include "Constants.h"

#include "Classes/ImageList.h"
#include "Classes/BackDC.h"
#include "Classes/GripBoxDat.h"
#include "Classes/SplitterDat.h"
#include "Classes/MiniChildDat.h"
#include "Classes/AddInContextDat.h"
#include "Classes/DockingBox.h"
#include "Classes/Screen.h"
#include "Classes/Rect.h"
#include "Classes/FileTime.h"
#include "Classes/Now.h"
#include "Classes/CommonDialog.h"
#include "Classes/Button.h"

// -----------------------------------------------------------------------
// Variables
extern luaL_reg Chromatic_Methods[];

void *Classes_Stack[1024];
int Stack_Pos;

// -----------------------------------------------------------------------
// Private functions
//void InsertPILLWALib(long CodeContext);
long CALLBACK Script_CreateStockModalDialog(long TemplateNumber, long DLeft, long DTop, long DWidth, long DHeight, HWND hParent, char *DTitle, DLGPROC WinProc, long Centered);
HWND CALLBACK Script_CreateNonModalDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WADTitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType);
HWND CALLBACK Script_CreateMDIChildDialog(HWND WAHParent, char *WACTitle, HICON WAhIcon, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAZeroPos, char *WACustomClass);
HWND CALLBACK Script_CreateMDIDialog(long WAMDILeft, long WAMDITop, long WAMDIWidth, long WAMDIHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WAMDITitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAShowType);
HWND CALLBACK Script_CreateSplashDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, char *WADTitle, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType);
void Script_FreeResources(lua_State *L);
void CALLBACK WAMMEnumKillWindows(void);
int CALLBACK EnumKill(HWND hWnd, long lParam);
HGLOBAL CALLBACK PILL_MiscAllocMem(long Amount);
long CALLBACK PILL_MiscFreeMem(void *MemHandle);

// -----------------------------------------------------------------------
// Scripts debugging function
/*void ScriptDebug(lua_State *C, lua_Debug *ar);
{

}*/

// -----------------------------------------------------------------------
// Init the running context for scripts
int AllocScriptEngine(void)
{

    return(TRUE);
}

// -----------------------------------------------------------------------
// Release the resources of the scripts engine
void ReleaseScriptEngine(void)
{

}

// -----------------------------------------------------------------------
// Run a script as string
long InsertScriptCode(char *ScriptName, char *SnippetVal)
{
    int i;
    lua_State *L;
    CList <HWND> Script_Windows;
    CList <HMODULE> Script_Libraries;

    Script_Windows.Erase();
    Script_Libraries.Erase();

    L = lua_open();
    if(L)
    {
        L->Windows = (void *) &Script_Windows;
        L->Libraries = (void *) &Script_Libraries;
        L->In_Dialog = 0;
        L->Obj_Stack = (void **) malloc(OBJ_STACK_SIZE);
        L->Script_Name = ScriptName;
        memset(L->Obj_Stack, 0, OBJ_STACK_SIZE);
        L->Obj_Stack_Pos = 0;
        luaL_openlibs(L);
        luaL_openlib(L, "ChromaticLib", Chromatic_Methods, 0);

        // Declare the constants
        i = 0;
        while(Script_Constants[i].Name)
        {
            lua_pushstring(L, Script_Constants[i].Name);
            lua_pushinteger(L, Script_Constants[i].Value);
            lua_rawset(L, -3);
            i++;
        }

        ImageList::Install(L);
        BackDC::Install(L);
        GripBoxDat::Install(L);
        SplitterDat::Install(L);
        MiniChildDat::Install(L);
        AddInContextDat::Install(L);
        DockingBox::Install(L);
        Screen::Install(L);
        Rect::Install(L);
        FileTime::Install(L);
        Now::Install(L);
        CommonDialog::Install(L);
        Button::Install(L);
  
        L->In_Dialog = 0;

        // Run the script now
        int Error = luaL_dostring(L, SnippetVal);

        Script_FreeResources(L);

        // Print any error if necessary
        if(Error) PrintScriptError(L);

        lua_close(L);
    }        
    return(1);
}

// -----------------------------------------------------------------------
// Run a script file
long RunScriptFile(CStr FileName, CStr Arguments)
{
//  long ReturnCode;
//  PILL_Error CodeError;
    /*int i = 0;
    int Args_Str_Len = Arguments.Len();
    int Nbr_Args = 1;
    int Cur_Arg = 0;
  //  char *Start_Arg;
    CStr Arg_Nbr_Str;*/

//  ScriptContext = InitLUAContext();
  //  if(ScriptContext) {
    /*  InsertPILLWALib(CodeContext);
    
        char *Args = (char *) malloc(Args_Str_Len + 1);
        if(Args) {
            memset(Args, 0, Args_Str_Len + 1);
            memcpy(Args, Arguments.Get_String(), Args_Str_Len);
            Start_Arg = Args;
            if(Arguments.In_Str(1, Arguments.Chr(2))) {
                while(Args[i]) {
                    if(Args[i] == 2) {
                        Nbr_Args++;
                    }
                    i++;
                }
                i = 0;
                PILL_Add_Variable(CodeContext, "ARGV", PILL_VAR_STRING, (long) "", Nbr_Args },
                while(Args[i]) {
                    if(Args[i] == 2) {
                        Args[i] = 0;
                        PILL_Set_Variable_Value(CodeContext, "ARGV", PILL_VAR_STRING, (long) Start_Arg, Cur_Arg);
                        Start_Arg = &Args[i] + 1;
                        Cur_Arg++;
                    }
                    i++;
                }
                PILL_Set_Variable_Value(CodeContext, "ARGV", PILL_VAR_STRING, (long) Start_Arg, Cur_Arg);
            } else {
                PILL_Add_Variable(CodeContext, "ARGV", PILL_VAR_STRING, (long) Start_Arg, Nbr_Args },
            }
        } else {
            Arg_Nbr_Str = "ARGV" + (CStr) (Nbr_Args - 1);
            PILL_Add_Constant(CodeContext, Arg_Nbr_Str.Get_String(), PILL_VAR_STRING, (long) Arguments.Get_String() },
            WriteToStatus("Couldn't allocate room for additional arguments");
        }
        PILL_Add_Constant(CodeContext, "ARGC", Nbr_Args },

        ReturnCode = PILL_Run_Script(CodeContext, FileName.Get_String());
        if(ReturnCode > 0 && ReturnCode < PILL_ERROR_USER_DEFINED) {
            if(ReturnCode != PILL_ERROR_GHOST) {
                PILL_Ask_Error_Details(CodeContext, &CodeError);
                WriteToStatus(FileName + (CStr) ":" + (CStr) CodeError.Line + (CStr) ": error: " + (CStr) CodeError.Description);
            }
            PILL_FreeResources(CodeContext);
            return(FALSE);
        }*/
//        DestroyLUAContext(ScriptContext);
  //  }
/*
    PILL_FreeResources(CodeContext);
    if(Args) free(Args);*/
    return(1);

//  return(ReturnCode);
}

// -----------------------------------------------------------------------
// Output an error mesage into the output box
void PrintScriptError(lua_State *L)
{
    char *Err_String;
    CStr Output_String;
    int Pos_CBracket;

    Err_String = (char *) lua_tostring(L, -1);
    if(Err_String == NULL) Err_String = "unrecognized LUA error";    
    Output_String = Err_String;
    Pos_CBracket = Output_String.In_Str(1, "]");
    Output_String = &Err_String[Pos_CBracket];
    if(Output_String.Left(1) != (CStr) ":") Output_String = ": " + Output_String;
    Output_String = (CStr) L->Script_Name + Output_String;
    WriteToStatus(Output_String);
}

// -----------------------------------------------------------------------
// Remove the resources allocated by a script
void Script_FreeResources(lua_State *L)
{
    int i;
    HWND Act_Window;
    CStr Act_WinClass;
    CList <HWND> *Win = (CList <HWND> *) L->Windows;
    CList <HMODULE> *Lib = (CList <HMODULE> *) L->Libraries;

    if(L->Obj_Stack) free(L->Obj_Stack);

    // Free dialogs
    for(i = 0; i < Win->Amount(); i++)
    {
        DestroyWindow(Win->Get(i)->Content);
    }

    // Free libraries
    for(i = 0; i < Lib->Amount(); i++)
    {
        FreeLibrary(Lib->Get(i)->Content);
    }

    // Zombie dialog ?
    try
    {
        if(L->In_Dialog == 1)
        {
            Act_Window = GetActiveWindow();
            Act_WinClass = ControlGetClassName(Act_Window);
            if(strcmp(Act_WinClass.Left(2).Upper_Case().Get_String(), "WA") != 0)
            {
                ControlClose(Act_Window);
            }
            else
            {
                // Means something went wrong during WM_INITDIALOG
                EndDialog(LastDialog, 0);
                LastDialog = 0;
            }
        }
    }
    catch(...)
    {
    }
}

// -----------------------------------------------------------------------
// Kill the remaining dialog
// (i can't remember what was the purpose of that one)
void CALLBACK WAMMEnumKillWindows(void)
{
    EnumChildWindows(hMDIform.hWnd, &EnumKill, 0);
}
                    
int CALLBACK EnumKill(HWND hWnd, long lParam)
{
    if(ControlIsEnabled(hWnd))
    {
        DestroyWindow(hWnd);    
        return(0);
    }
    return(1);
}

// -----------------------------------------------------------------------
// Functions available for the scripts
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Special version of these routines for scripts
// (mainly for resources tracking because we can't let a dialog around
//  if the script contains errors or the interpreter crashes)
int Script_CreateStockModalDialog(lua_State *L)
{
    int TemplateNumber = luaL_checkint(L, 1);
    int DLeft = luaL_checkint(L, 2);
    int DTop = luaL_checkint(L, 3);
    int DWidth = luaL_checkint(L, 4);
    int DHeight = luaL_checkint(L, 5);
    HWND hParent = (HWND) luaL_checkint(L, 6);
    char *DTitle = (char *) luaL_checkstring(L, 7);
    DLGPROC WinProc = (DLGPROC) luaL_checkint(L, 8);
    int Centered = luaL_checkint(L, 9);

    long ReturnValue;

    // We need to freeze our internal timer
    FreezeTimer = 1;
    L->In_Dialog = 1;

    switch(TemplateNumber)
    {
        case MODALDLG_STOCK_WIZARD:
            ReturnValue = CreateStockModalDialog(TemplateNumber, DLeft, DTop, DWidth, DHeight, hParent, DTitle, WinProc, Centered, (long) MAKEINTRESOURCE(MBMP_BASE + MBMP_WIZARD));
            break;
        default:
            ReturnValue = CreateStockModalDialog(TemplateNumber, DLeft, DTop, DWidth, DHeight, hParent, DTitle, WinProc, Centered, 0);
            break;
    }
    FreezeTimer = 0;
    L->In_Dialog = 0;

    lua_pushnumber(L, ReturnValue);
    return(1);
}

// -----------------------------------------------------------------------
int Script_MsgBox(lua_State *L)
{
    int ReturnValue;

    HWND hParent = (HWND) luaL_checkint(L, 1);
    char *MBText = (char *) luaL_checkstring(L, 2);
    char *MBTitle = (char *) luaL_checkstring(L, 3);
    int MBType = luaL_checkint(L, 4);

    ReturnValue = MessageBox(hParent, MBText, MBTitle, MBType);
    lua_pushnumber(L, ReturnValue);
    return(1);
}

// -----------------------------------------------------------------------
int Script_ObtainExtendedFunctions(lua_State *L)
{
    luaL_error(L, "reserved for future use");
    return(1);
}

// -----------------------------------------------------------------------
int Script_InsertText(lua_State *L)
{
    HWND hForm;
    if(NbForms == 0) return(1);

    hForm = (HWND) luaL_checkint(L, 1);
    if(hForm != 0) InsertText(hForm, (char *) luaL_checkstring(L, 2));
    return(1);
}

// -----------------------------------------------------------------------
int Script_StringHex32(lua_State *L)
{
    char *Return_Value = Wrapper_StringHex32(luaL_checkint(L, 1));

    lua_pushstring(L, Return_Value);
    return(1);
}

// -----------------------------------------------------------------------
int Script_StringCat(lua_State *L)
{
    char *Return_Value = Wrapper_StringCat((char *) luaL_checkstring(L, 1),
                                             (char *) luaL_checkstring(L, 2));
    lua_pushstring(L, Return_Value);
    return(1);
}

// -----------------------------------------------------------------------
int Script_LoadLibrary(lua_State *L)
{
    HMODULE NewLibrary;
    CList <HMODULE> *Lib = (CList <HMODULE> *) L->Libraries;

    NewLibrary = LoadLibrary(luaL_checkstring(L, 1));
    if(NewLibrary) Lib->Add(NewLibrary);

    lua_pushinteger(L, (int) NewLibrary);
    return(1);
}

// -----------------------------------------------------------------------
int Script_FreeLibrary(lua_State *L)
{
    int i;
    HMODULE Library_Base = (HMODULE) luaL_checkint(L, 1);
    CList <HMODULE> *Lib = (CList <HMODULE> *) L->Libraries;

    // Free the entry in the list
    for(i = 0; i < Lib->Amount(); i++)
    {
        if(Lib->Get(i)->Content == Library_Base)
        {
            Lib->Del(i);
            break;
        }
    }

    lua_pushboolean(L, FreeLibrary(Library_Base));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogXUnitToPixel(lua_State *L)
{
    lua_pushinteger(L, DialogXUnitToPixel(luaL_checkint(L, 1),
                                          luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogYUnitToPixel(lua_State *L)
{
    lua_pushinteger(L, DialogYUnitToPixel(luaL_checkint(L, 1),
                                          luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogXPixelToUnit(lua_State *L)
{
    lua_pushinteger(L, DialogXPixelToUnit(luaL_checkint(L, 1),
                                          luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogYPixelToUnit(lua_State *L)
{
    lua_pushinteger(L, DialogYPixelToUnit(luaL_checkint(L, 1),
                                          luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogGetXUnit(lua_State *L)
{
    lua_pushinteger(L, DialogGetXUnit());
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogGetYUnit(lua_State *L)
{
    lua_pushinteger(L, DialogGetYUnit());
    return(1);
}

// -----------------------------------------------------------------------
int Script_DialogSetIcon(lua_State *L)
{
    lua_pushinteger(L, DialogSetIcon((HWND) luaL_checkint(L, 1),
                                     (HICON) luaL_checkint(L, 2)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_FrameRefreshToolbar(lua_State *L)
{
    lua_pushinteger(L, FrameRefreshToolbar((HWND) luaL_checkint(L, 1)));
    return(1);
}


// -----------------------------------------------------------------------
int Script_WinsockInit(lua_State *L)
{
    lua_pushinteger(L, (long) WinsockInit((LPWSADATA) luaL_checkint(L, 1),
                                          (long) luaL_checkint(L, 2)
                                         ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_WinsockUnInit(lua_State *L)
{
    lua_pushinteger(L, WinsockUnInit());
    return(1);
}

// -----------------------------------------------------------------------
int Script_ProgressBarSetRange(lua_State *L)
{
    lua_pushinteger(L, ProgressBarSetRange((HWND) luaL_checkint(L, 1),
                                           (long) luaL_checkint(L, 2),
                                           (long) luaL_checkint(L, 3)
                                           ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_ProgressBarSetRelPos(lua_State *L)
{
    lua_pushinteger(L, ProgressBarSetRelPos((HWND) luaL_checkint(L, 1),
                                            (long) luaL_checkint(L, 2)
                                            ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_CheckBoxGetState(lua_State *L)
{
    lua_pushinteger(L, CheckBoxGetState((HWND) luaL_checkint(L, 1)));
    return(1);
}

// -----------------------------------------------------------------------
int Script_CheckBoxSetState(lua_State *L)
{
    lua_pushinteger(L, CheckBoxSetState((HWND) luaL_checkint(L, 1),
                                        (long) luaL_checkint(L, 2)
                                        ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_ColorBoxGetColor(lua_State *L)
{
    lua_pushinteger(L, ColorBoxGetColor((HWND) luaL_checkint(L, 1)
                                        ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_ColorBoxSetColor(lua_State *L)
{
    lua_pushinteger(L, ColorBoxSetColor((HWND) luaL_checkint(L, 1),
                                        (long) luaL_checkint(L, 2)
                                        ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_StatusBarSetParts(lua_State *L)
{
    long Array[512];
    int i;
    int Top;
    HWND Handle = (HWND) luaL_checkint(L, 1);
    int Parts = luaL_checkint(L, 2);

    if(Parts > 511) Parts = 511;
    luaL_checktype(L, 3, LUA_TTABLE);
    for(i = 1; i <= Parts; i++)
    {
        lua_rawgeti(L, 3, i);
        Top = lua_gettop(L);
        Array[i - 1] = lua_tointeger(L, Top);
        lua_pop(L, 1);
    }

    lua_pushinteger(L, StatusBarSetParts(Handle, Parts, Array));
    return(1);
}

// -----------------------------------------------------------------------
int Script_StatusBarSetText(lua_State *L)
{
    lua_pushinteger(L, Wrapper_StatusBarSetText((HWND) luaL_checkint(L, 1),
                                                (long) luaL_checkint(L, 2),
                                                (long) luaL_checkint(L, 3),
                                                (char *) luaL_checkstring(L, 4),
                                                (long) luaL_checkint(L, 5)
                                                ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_StatusBarGetYSize(lua_State *L)
{
    lua_pushinteger(L, StatusBarGetYSize((HWND) luaL_checkint(L, 1)
                                        ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_LabelGetImage(lua_State *L)
{
    lua_pushinteger(L, (int) LabelGetImage((HWND) luaL_checkint(L, 1),
                                           (long) luaL_checkint(L, 2)
                                          ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_PictureBoxChangeIcon(lua_State *L)
{
    lua_pushinteger(L, PictureBoxChangeIcon((HWND) luaL_checkint(L, 1),
                                            (HICON) luaL_checkint(L, 2)
                                            ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_PictureBoxChangeImage(lua_State *L)
{
    lua_pushinteger(L, PictureBoxChangeImage((HWND) luaL_checkint(L, 1),
                                             (HANDLE) luaL_checkint(L, 2)
                                             ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_RadioButtonGetState(lua_State *L)
{
    lua_pushinteger(L, RadioButtonGetState((HWND) luaL_checkint(L, 1)
                                          ));
    return(1);
}

// -----------------------------------------------------------------------
int Script_RadioButtonSetState(lua_State *L)
{
    lua_pushinteger(L, RadioButtonSetState((HWND) luaL_checkint(L, 1),
                                           (long) luaL_checkint(L, 2)
                                          ));
    return(1);
}

// -----------------------------------------------------------------------

/*HWND CALLBACK PILL_CreateNonModalDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WADTitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType) {
    HWND NewhWnd;
    
    NewhWnd = CreateNonModalDialog(WADLeft, WADTop, WADWidth, WADHeight, WAHParent, WAhMenu, WAhIcon, StringConvertToCStr(WADTitle), InitProc, WAWinProc, WAWExStyle, WAWStyle, WAShowType);
    PILL_Windows.Add(NewhWnd);
    return(NewhWnd);
}

HWND CALLBACK PILL_CreateMDIChildDialog(HWND WAHParent, char *WACTitle, HICON WAhIcon, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAZeroPos, char *WACustomClass) {
    HWND NewhWnd;

    NewhWnd = CreateMDIChildDialog(WAHParent, StringConvertToCStr(WACTitle), WAhIcon, InitProc, WAWinProc, WAExtraStyle, WAZeroPos, StringConvertToCStr(WACustomClass));
    PILL_Windows.Add(NewhWnd);
    return(NewhWnd);
}

HWND CALLBACK PILL_CreateMDIDialog(long WAMDILeft, long WAMDITop, long WAMDIWidth, long WAMDIHeight, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, char *WAMDITitle, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAExtraStyle, long WAShowType) {
    HWND NewhWnd;

    NewhWnd = CreateMDIDialog(WAMDILeft, WAMDITop, WAMDIWidth, WAMDIHeight, WAHParent, WAhMenu, WAhIcon, StringConvertToCStr(WAMDITitle), InitProc, WAWinProc, WAExtraStyle, WAShowType);
    PILL_Windows.Add(NewhWnd);
    return(NewhWnd);
}

HWND CALLBACK PILL_CreateSplashDialog(long WADLeft, long WADTop, long WADWidth, long WADHeight, char *WADTitle, HWND WAHParent, HMENU WAhMenu, HICON WAhIcon, void (CALLBACK *InitProc)(HWND hwnd), WNDPROC WAWinProc, long WAWExStyle, long WAWStyle, long WAShowType) {
    HWND NewhWnd;

    NewhWnd = CreateSplashDialog(WADLeft, WADTop, WADWidth, WADHeight, StringConvertToCStr(WADTitle), WAHParent, WAhMenu, WAhIcon, InitProc, WAWinProc, WAWExStyle, WAWStyle, WAShowType);
    PILL_Windows.Add(NewhWnd);
    return(NewhWnd);
}
*/

// -----------------------------------------------------------------------
// Static methods definitions for Chromatic table
luaL_reg Chromatic_Methods[] =
{
    { "CreateStockModalDialog", Script_CreateStockModalDialog },

    { "MsgBox", Script_MsgBox },
    { "ObtainExtendedFunctions", Script_ObtainExtendedFunctions },

    // ---------------

    { "InsertText", Script_InsertText },

    { "StringHex32", Script_StringHex32 },
    { "StringCat", Script_StringCat },
    { "LoadLibrary", Script_LoadLibrary },
    { "FreeLibrary", Script_FreeLibrary },
    
    { "DialogXUnitToPixel", Script_DialogXUnitToPixel },
    { "DialogYUnitToPixel", Script_DialogYUnitToPixel },
    { "DialogXPixelToUnit", Script_DialogXPixelToUnit },
    { "DialogYPixelToUnit", Script_DialogYPixelToUnit },
    { "DialogGetXUnit", Script_DialogGetXUnit },
    { "DialogGetYUnit", Script_DialogGetYUnit },
    { "DialogSetIcon", Script_DialogSetIcon },

    { "FrameRefreshToolbar", Script_FrameRefreshToolbar },

    { "WinsockInit", Script_WinsockInit },
    { "WinsockUnInit", Script_WinsockUnInit },
/*  FillPILLConstant(CodeContext, "SocketCreate", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketDestroy", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketConnect", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketGetPort", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketListen", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketAcceptConnection", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketSendASCIIDatasCR", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketSendUnknownDatas", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketGetRevcLen", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketReceiveASCIIDatas", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SocketReceiveUnknownDatas", i, PILL_STORE_ENFORCE, 1);
*/

    { "ProgressBarSetRange", Script_ProgressBarSetRange },
    { "ProgressBarSetRelPos", Script_ProgressBarSetRelPos },

    { "CheckBoxGetState", Script_CheckBoxGetState },
    { "CheckBoxSetState", Script_CheckBoxSetState },

    { "ColorBoxGetColor", Script_ColorBoxGetColor },
    { "ColorBoxSetColor", Script_ColorBoxSetColor },

    { "StatusBarSetParts", Script_StatusBarSetParts },
    { "StatusBarSetText", Script_StatusBarSetText },
    { "StatusBarGetYSize", Script_StatusBarGetYSize },

    { "LabelGetImage", Script_LabelGetImage },

    { "PictureBoxChangeIcon", Script_PictureBoxChangeIcon },
    { "PictureBoxChangeImage", Script_PictureBoxChangeImage },

    { "RadioButtonGetState", Script_RadioButtonGetState },
    { "RadioButtonSetState", Script_RadioButtonSetState },

/*    { "GotoTop", Script_GotoTop },
    { "GotoBottom", Script_GotoBottom },
    { "GotoLine", Script_GotoLine },
    { "GotoMatchingBracket", Script_GotoMatchingBracket },
    { "GotoPrevWord", Script_GotoPrevWord },
    { "GotoNextWord", Script_GotoNextWord },
    { "GotoPrevLetter", Script_GotoPrevLetter },
    { "GotoNextLetter", Script_GotoNextLetter },
    { "GotoPrevLine", Script_GotoPrevLine },
    { "GotoNextLine", Script_GotoNextLine },
    { "GotoPrevPage", Script_GotoPrevPage },
    { "GotoNextPage", Script_GotoNextPage },
    { "GotoFileLine", Script_GotoFileLine },
*/
    { NULL, NULL }
};

// -----------------------------------------------------------------------
// Register the WALib into PILL
/*void InsertPILLWALib(long CodeContext) {
    // Necessary
    PILL_Add_Variable(CodeContext, "EndDialog", (long) &EndDialog, 0 },
    PILL_Add_Variable(CodeContext, "GetDlgItem", (long) &GetDlgItem, 0 },
    PILL_Add_Variable(CodeContext, "GetProcAddress", (long) &GetProcAddress, 0 },

    FillPILLConstant(CodeContext, "MiscClipBoardCopyText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscClipBoardPasteText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscClipBoardIsEmpty", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscShellAddFileToRecents", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscWaitEvents", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscSendBroadCastMsg", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscDoEvents", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscObtainGUID", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscAllocMem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscFreeMem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscCopyMemLoop", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MiscCopyMemWithOffsets", i, PILL_STORE_ENFORCE, 1);


    FillPILLConstant(CodeContext, "StringIsLabel", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsHexaDecimal", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsDecimal", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsLabelChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsDigitChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsHexaChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsAlphaChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsBlankChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsOpenBracketChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsUnderLineChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsDotChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsCommaChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsQuoteChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsArobasChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsSpaceChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsEOL", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringNumberComplement", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringHexNumberComplement", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringOctNumberComplement", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringBinNumberComplement", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringReplace", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringHex16", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringHex8", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringSplit", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringGetSplitUBound", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringGetSplitElement", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringReleaseSplit", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringJoin", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringFree", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringLen", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringDecToString", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringStringToDec", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringCountLines", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringGetLinesOffsets", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringFilterNonPrintableChars", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringLeft", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringRight", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringTrim", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringLTrim", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringRTrim", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringInStr", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringMid", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringUpperCase", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringLowerCase", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringReverse", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringSpace", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringString", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringIsNumeric", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringFillHexBytes", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringFillHexWords", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringFillHexDWords", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "StringFileNameToLabel", i, PILL_STORE_ENFORCE, 1);


    i = 0;
    FillPILLConstant(CodeContext, "CreateSplashDialog", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateModalDialog", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateNonModalDialog", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateSplitter", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateMDIDialog", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateMDIChildDialog", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateToolBar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateImageList", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateSysTab", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateTrackBar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateUpDown", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateProgressBar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateStatusBar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateColorBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateGripBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateLabel", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreatePictureBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateFrame", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateIPBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateRichTextBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateHexBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateCheckBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateRebar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreatePager", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateClient", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateRadioButton", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateListView", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateTreeView", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateComboBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateListBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateScrollBar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateDumpBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CreateTextBox", i, PILL_STORE_ENFORCE, 1);
    
    FillPILLConstant(CodeContext, "ToolBarAddButton", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetButton", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetButtonXSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetButtonYSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetButtonsCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetRowsCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetImagelist", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarAddSeparator", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetNotifiedToolTip", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetNotifiedDropDownItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarDisplayToolTip", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarResize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarSetButtonPressed", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarSetButtonChecked", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarSetButtonEnabled", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarIsButtonPressed", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarIsButtonChecked", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarDisplayPopupMenu", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetXSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetYSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetButtonXPosition", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetXYSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetRealPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetButtonXYPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetButtonIndexXSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetXPadding", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetYPadding", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarGetNotifiedHotItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ToolBarRemoveButton", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "SysTabAddItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabGetCurrentItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabGetImagelist", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabSetCurrentItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabSetFocusItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabHighLightItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabItemsCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabGetRowsCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabSetSeparators", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabSetItemsSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabRemoveItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabSetMinWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "SysTabSetPadding", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "TrackBarSetRange", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TrackBarSetPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TrackBarGetPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TrackBarSetTicks", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TrackBarGetToolTips", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "GripBoxGetColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GripBoxSetColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GripBoxClose", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GripBoxVisible", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GripBoxResize", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "IPBoxClearIP", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IPBoxGetIP", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IPBoxSetIP", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IPBoxSetFocus", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IPBoxSetRange", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IPBoxIsBlank", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "TextBoxGetLen", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxSetMaxLen", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetMaxLen", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetTextHandle", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxAddText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxSelText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxRemoveSel", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxSetCaretPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxLinesCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetMarginWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetFirstVisibleLine", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxIsModified", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetCurrentOptions", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetPasswordChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxSetPasswordChar", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetSelection", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetScrollPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxGetBreakProcAddress", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxCanPaste", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TextBoxCanUndo", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "RebarAddBand", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarRemoveBand", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetXSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetYSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarResize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarSetBackColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarSetTextColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarBandVisible", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetBandsCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetHeight", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetBackGroundColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetRowsCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetToolTips", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetTextColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetPalette", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RebarGetImageList", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "PagerDisplaySetSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerMaximizeX", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerMaximizeY", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerGetNotifiedChild", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerSetBackColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerGetBackColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerGetBorderSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerGetButtonSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerGetScrollPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerGetOrientation", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerSetOrientation", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "PagerChangeOrientation", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "ClientResize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ClientGetActiveChild", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ClientSetNextChild", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ClientSetPreviousChild", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ClientTileHorizontal", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ClientTileVertical", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ClientTileCascade", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ClientTileArrangeIconic", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "ListViewAddCol", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetColWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetColWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetScroll", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewClear", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewDetachImageList", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewAddItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewDeleteItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetSubItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetSubItemImage", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetSubItemImage", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewEditValidated", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetEditResult", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewItemCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSelItemCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewMultiSelFirstItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetItemSelState", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetEditModeOn", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetItemSel", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetItemHighlight", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetItemFocus", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetItemUnderCursor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetFirstFocusItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetSelItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetSelItemText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetSelItemText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewCheckBoxItemDoubleClick", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetItemText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetItemIcon", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetNotifiedColumnIndex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSort", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewReOrder", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetHeaderLabel", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetHeaderPosition", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetHeaderPosition", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetBackColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetBackColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetTextBackColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetTextColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetToolTips", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetTopIndex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetCountPerPage", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetEditControl", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetExStyle", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetSysHeader", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetHotCursor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetImageList", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewSetItemCheckbox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewGetItemCheckbox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewIsCheckboxNotify", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewFindItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewFindSubItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewBeginDrag", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewMoveDrag", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewEndDrag", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListViewPasteAutoEdit", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "TreeViewAddItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSetToolTips", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetToolTips", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSetIndent", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewRemoveItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSetBackColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetExpandingState", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetExpandingItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetDeletedItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSetItemIcon", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSetItemExpandedState", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemExpandedState", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSetSingleExpandState", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetSelectedItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemFromPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSetSelectedItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSetItemText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemParent", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetRoot", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetVisibleCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetFirstItemChild", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetNextItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetPreviousItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSearchItemText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewSearchChildPartialText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetChildItemsCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetSelChildItemPos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemLevel", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetBackColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetEditControl", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetImageList", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetIndent", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetInsertMarkColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetLineColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetTextColor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemHeight", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetScrollTime", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemTextRect", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "TreeViewGetItemRect", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "ComboBoxAddItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxDeleteItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxReset", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxSetMaxLen", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxGetIndex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxSetIndex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxGetItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxGetCurrentItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxItemExist", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxGetItemData", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxSaveInIniFile", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ComboBoxFillFromIniFile", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "ListBoxAddItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxReset", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxSetHorzScrollWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxSetColumnsWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxCount", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxGetTopIndex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxSetTopIndex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxSetItemData", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxGetItemData", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxIsItemSelected", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxGetSelItemIndex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxSetIndex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxGetCurrentItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxGetItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxDeleteItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxReplaceItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxReplaceSelItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxProcessDrag", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxSelItemDown", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxSelItemUp", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxSelItemRemove", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxItemExist", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ListBoxItemUnderCursor", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "ScrollBarGetMinRange", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ScrollBarGetMaxRange", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ScrollBarGetPageRange", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ScrollBarSetPageRange", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ScrollBarShowHide", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ScrollBarSetMinMaxRange", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "DumpBoxResize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "DumpBoxScrollDown", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "DumpBoxScrollUp", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "DumpBoxGetVisibleLines", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "DumpBoxGetVisibleColumns", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "MenuGetString", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MenuEnable", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MenuSetDefaultItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MenuSetItemState", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "MenuSetItemType", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "CursorSetWait", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CursorSetNo", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CursorSetCross", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CursorSetNormal", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CursorDisable", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "CursorEnable", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "IniReadKey", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IniReadBoolKey", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IniWriteSection", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IniDeleteKey", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "IniWriteKey", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "FileGetWriteTime", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileGetCreationTime", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileGetAccessedTime", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileGetExtension", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileRemoveExtension", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileReplaceExtension", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileGetDirectory", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileGetFileName", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileOpenR", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileOpenW", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileOpenWAppend", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileClose", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileCreateEmpty", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileWriteBuffer", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileWriteLine", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileIsDirectory", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileGetSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileExist", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileIsReadOnly", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileIsUnix", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileLoadIntoMem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileSaveFromMem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FileDir", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "RegistryUpdateKey", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RegistryDeleteKey", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "RegistryGetKeyValue", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "GDIFillWindow", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIFillWindowFromBrush", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIFillWindowFromBrushAndhDC", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDICreateColorBrush", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIObtainFont", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIDrawHorzSep", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIDrawVertSep", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIDrawLine", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIDrawLineXOR", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIDrawPixel", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIWriteText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIWriteClippedText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIGetTextWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIGetTextHeight", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIGetFontWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIGetFontHeight", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDICreateBackDC", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIDestroyBackDC", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIBlitBackDC", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIColorCalcLuminosity", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIGetSerif", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIGetSerif10", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "GDIGetCourierNew9", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "ControlSendMessage", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlClientTop", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlClientLeft", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlClientWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlClientHeight", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlBringToTop", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlBringToBottom", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlSetTopMost", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlRemTopMost", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlRefresh", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlRefreshLocal", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlRepaint", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlResize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlFreeze", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlBound", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlEnable", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlVisible", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlIsVisible", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlIsEnabled", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetTextLen", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlSetText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetNotifiedMsg", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetNotifiedhWnd", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetNotifiedID", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlTop", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlLeft", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlWidth", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlHeight", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetHwndFromPoint", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetNotifiedCommand", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetXMousePos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetYMousePos", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetFont", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetIcon", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetHotKey", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlSetFont", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlSetFontNoRedraw", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlIsWindowChild", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetClassName", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlSetClassCursor", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlGetWindowState", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "ControlClose", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "FTPInitiatePort", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FTPGetEntryDate", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FTPGetEntryFileName", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FTPGetEntryFileAttributes", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FTPGetEntryFileSize", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "FTPIsEntryDir", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "WAMMGetWindowLangGUID", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangInclude", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangDoubleSlash", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangComment", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangMultiComment", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangMultiCommentEnd", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangProc", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangEndp", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangProcInterlace", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangProcOrphan", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangProcOpenBlock", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangProcCloseBlock", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangFileName", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangIncludeName", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowLangIncludeNameFoot", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "WAMMDecodeRelativePaths", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMRemoveRelativePaths", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMEncodeRelativePaths", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMRunUserCommand", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMRunCommandScript", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMRunDOSCmd", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMRunDOSCmdCapture", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMDisplayHelpFile", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetWindowPath", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMIsFileWorkSpace", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMIsFileProject", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetCurrentChildState", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMFocusOnFile", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetFileNameFromTreeViewItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetLabelFromTreeViewItem", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMOpenUnknownFile", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMHexToDec", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMDecToHex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMDecToBin", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMHexToBin", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMBinToDec", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMBinToHex", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMHexToFlt", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMFltToHex", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "WAMMGetProceduresList", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetLineFromFile", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCreateNewWindow", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMClearWindow", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMReloadWindow", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCloseWindow", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCloseAllWindows", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSaveAllWindows", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMPrintWindow", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSaveWindow", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "WAMMUndo", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMRedo", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCut", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCopy", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCopyFilePathName", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCopyEntireText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMPaste", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMDeleteSel", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMDeleteLine", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMDeleteToStart", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMDeleteToEnd", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSelectLine", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSelectAll", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSelectProc", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMFindNext", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMFindPrevious", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMFindSelection", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "WAMMGetIniFilePathName", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetQuickDirectory", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetContext", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetcurrentLine", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSelectCurrentWord", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetCurrentWord", i, PILL_STORE_ENFORCE, 1);

    FillPILLConstant(CodeContext, "WAMMInsertLine", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMInsertLines", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSetText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetCaretRealXCoordinate", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetCaretRealYCoordinate", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSetFocusInsideWindow", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMRestoreMDIFromSysTray", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMInstallHookProc", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMUnInstallHookProc", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetCurrentLineNumber", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetCurrentColNumber", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetCurrentLineText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMGetLineText", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMWriteToOutputBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMEnumSourceWindows", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMAddInKillZombie", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSpAddInIsAvailable", i, PILL_STORE_ENFORCE, 1);
    
    FillPILLConstant(CodeContext, "WAMMMiscInputBox", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMIsChildReadOnly", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMSetChildReadOnly", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCreateCustomChildDialog", i, PILL_STORE_ENFORCE, 1);
    FillPILLConstant(CodeContext, "WAMMCreateCodeMax", i, PILL_STORE_ENFORCE, 1);
}
*/
