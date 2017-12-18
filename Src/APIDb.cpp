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
// APIDb.cpp: API databases handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MDI_Childs.h"
#include "MDI_Form.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "Dlg_API.h"
#include "Dlg_APIList.h"

// -----------------------------------------------------------------------
// Variables
char *APiLib;
char *APIName;
CStr APIFuckStrLib;
CStr APIFuckStrName;
long *APIBlockLib;
long *APIBlockName;
long *APIInfosLib;
long *APIInfosName;
CStr LibToFind;
long APIX;
long APIY;
CStr APIFnc;
CStr APILineFnc;
char *APIDescription;
CStr APIFuckStr;
long *APIBlock;
long APIFound;
long *APIInfos;
long *APIArgs;
long MaxAPIArgs;
CStr APIString;
HWND APIhText;
long *APIArrParse;
HFONT APIhFont;
HFONT APIhFontBold;
long APICurHiglight;
CStr APIFile;
CStr APIFncFile;
CStr APIConstFile;
HWND APIhListBox;
long APIOldRealPosition;
long APILineOldRealPosition;
long APINoRedraw;
long HighOn;
long HighPar;
long DbLinesNumbers;
long UseDbFnc;
long UseDbConst;

// -----------------------------------------------------------------------
// Retrieve the library import of a API function
// (for linker errors report purposes)
CStr RetrieveAPI(void)
{
    CStr ReturnValue;
    CStr FirstSplit;
    int i = 0;
    long FoundLib = 0;
    long FoundLibLen = 0;
    CStr BufString;

    APIInfosLib = 0;
    APiLib = GetApiDescription(APIFile.Get_String(), LibToFind.Get_String());
    if(APiLib != 0)
    {
        FoundLibLen = strlen(APiLib);
        APIFuckStrLib = APIFuckStrLib.String(FoundLibLen + 1, 1);
        RtlCopyMemory(APIFuckStrLib.Get_String(), (void *) APiLib, FoundLibLen);
        APIBlockLib = StringSplit(APIFuckStrLib, "^^");
        if(StringGetSplitUBound(APIBlockLib) != -1)
        {
            FoundLib = 0;
            for(i = 0; i <= StringGetSplitUBound(APIBlockLib); i++)
            {
                FirstSplit = StringGetSplitElement(APIFuckStrLib, APIBlockLib, i);
                APIInfosLib = StringSplit(FirstSplit, "|");
                if(_strcmpi(LibToFind.Get_String(), StringGetSplitElement(FirstSplit, APIInfosLib, 0).Get_String()) == 0)
                {
                    FoundLib = 1;
                    break;
                }
                // Not found: release it
                StringReleaseSplit(APIInfosLib);
                APIInfosLib = 0;
            }
            if(FoundLib == 1) BufString = StringGetSplitElement(FirstSplit, APIInfosLib, 1);
        }
        if(APIInfosLib != 0) StringReleaseSplit(APIInfosLib);
        StringReleaseSplit(APIBlockLib);
        StringReleaseSplit((long *) APiLib);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Display an API function
// (for API name displaying purposes)
CStr DisplayAPI(long dx, long dy)
{
    CStr ReturnValue;
    CStr StArgs;
    CStr FirstSplit;
    CStr SecondSplit;
    int i = 0;
    int j = 0;
    long FoundLibLen = 0;
    CStr BufString;

    APIX = 0;
    APIY = 0;
    APIDescription = GetApiDescription(APIFile.Get_String(), APIFnc.Get_String());
    if(APIDescription != 0)
    {
        FoundLibLen = strlen(APIDescription);
        APIFuckStr = APIFuckStr.String(FoundLibLen + 1, 1);
        RtlCopyMemory(APIFuckStr.Get_String(), (void *) APIDescription, FoundLibLen);
        APIBlock = StringSplit(APIFuckStr, "^^");
        APIFound = 0;
        if(StringGetSplitUBound(APIBlock) != -1)
        {
            for(i = 0; i <= StringGetSplitUBound(APIBlock); i++)
            {
                FirstSplit = StringGetSplitElement(APIFuckStr, APIBlock, i);
                APIInfos = StringSplit(FirstSplit, "|");
                if(_strcmpi(APIFnc.Get_String(), StringGetSplitElement(FirstSplit, APIInfos, 0).Upper_Case().Get_String()) == 0)
                {
                    APIFound = 1;
                    break;
                }
                // Not found: release it
                StringReleaseSplit(APIInfos);
                APIInfos = 0;
            }
            StringReleaseSplit(APIBlock);
            if(APIFound == 1)
            {
                // Firstsplit = last found entry
                SecondSplit = StringGetSplitElement(FirstSplit, APIInfos, 3);
                APIArgs = StringSplit(SecondSplit, ",");
                if(StringGetSplitUBound(APIArgs) == 0) if(strcmp(StringGetSplitElement(SecondSplit, APIArgs, 0).Get_String(), "-") == 0) goto NoArgs;
                for(i = 0; i <= StringGetSplitUBound(APIArgs); i++)
                {
                    if(i == 2) break;
                    StArgs = StArgs + (CStr) StringGetSplitElement(SecondSplit, APIArgs, i).Get_String() + (CStr) ",";
                }
                StArgs = StArgs + "\r\n";
                while(i <= StringGetSplitUBound(APIArgs))
                {
                    for(j = 0; j <= StringGetSplitUBound(APIArgs); j++)
                    {
                        if(j == 2) break;
                        if((i + j) > StringGetSplitUBound(APIArgs)) break;
                        StArgs = StArgs + StringGetSplitElement(SecondSplit, APIArgs, i + j).Get_String() + (CStr) ",";
                    }
                    i = j + i;
                    StArgs = StArgs + "\r\n";
                }
                if(strcmp(StArgs.Right(3).Get_String(), ",\r\n") == 0) StArgs = StArgs.Left(StArgs.Len() - 3).Get_String() + (CStr) "\r\n";
NoArgs:         
                APIString = "Function: " + (CStr) StringGetSplitElement(FirstSplit, APIInfos, 0).Get_String() + (CStr) "\r\n";
                APIString = APIString + "Import lib: " + (CStr) StringGetSplitElement(FirstSplit, APIInfos, 1).Get_String() + (CStr) "\r\n";
                APIString = APIString + "Return: " + (CStr) StringGetSplitElement(FirstSplit, APIInfos, 2).Get_String() + (CStr) "\r\n";
                if(StringGetSplitUBound(APIArgs) == 0) if(strcmp(StringGetSplitElement(SecondSplit, APIArgs, 0).Get_String(), "-") == 0) goto NoStoreArgs;
                APIString = APIString + "Arguments (" + (CStr) (StringGetSplitUBound(APIArgs) + 1) + (CStr) ") : \r\n" + (CStr) StArgs + (CStr) "\r\n";
                goto StoreArgs;
NoStoreArgs:    APIString = APIString + "-\r\n\r\n";
StoreArgs:      MaxAPIArgs = StringGetSplitUBound(APIArgs);
                StringReleaseSplit(APIArgs);
                APIString = APIString + "Note: " + (CStr) StringGetSplitElement(FirstSplit, APIInfos, 4).Get_String() + (CStr) "\r\n";
                APIString = APIString + "WinNT: " + (CStr) StringGetSplitElement(FirstSplit, APIInfos, 5).Get_String() + (CStr) "\r\n";
                APIString = APIString + "Win95: " + (CStr) StringGetSplitElement(FirstSplit, APIInfos, 6).Get_String() + (CStr) "\r\n";
                APIString = APIString + "Win32s: " + (CStr) StringGetSplitElement(FirstSplit, APIInfos, 7).Get_String();
                if(APIhText == 0)
                {
                    ChildStruct = LoadStructure(CurrentForm);
                    CreateSplashDialog(dx, dy, 1, 1, "", ChildStruct->hChildCodeMax, 0, 0, &FRMAPIInitProc, &FRMAPIWinHook, 0, WS_DISABLED, SW_HIDE);
                }
                if(APIhText != 0)
                {
                    APIArrParse = StringSplit(APIString, "\r\n");
                    if(APIhFont != 0) DeleteObject(APIhFont);
                    APIhFont = GDIObtainFont(CurFontName, CurFontSize, hMDIform.hWnd, 0, 0);
                    if(APIhFontBold == 0) DeleteObject(APIhFontBold);
                    APIhFontBold = GDIObtainFont(CurFontName, CurFontSize, hMDIform.hWnd, 1, 0);
                    ControlVisible(APIhText, 1);
                    SendMessage(APIhText, WM_PAINT, 0, 0);
                }
                // Save API name
                BufString = StringGetSplitElement(FirstSplit, APIInfos, 0);
            }
            if(APIInfosLib != 0) StringReleaseSplit(APIInfos);
        }
        FreeMem((long) APIDescription);
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the correct name of a API function
// (for corrections purposes)
CStr RetrieveAPIName(CStr APIFncN)
{
    CStr ReturnValue;
    char *MaxName = 0;
    char *MaxName2 = 0;
    CStr BufString;

    if((UseDbFnc == 0) && (UseDbConst == 0)) return(ReturnValue);
    APIInfosName = 0;
    if(FncBase != 0) if(UseDbFnc == 1) MaxName = GetAPIKeywordToCorrect(FncBase,APIFncN.Get_String());
    if(ConstBase != 0) if(UseDbConst == 1) MaxName2 = GetAPIKeywordToCorrect(ConstBase,APIFncN.Get_String());
    if((MaxName != 0) || (MaxName2 != 0))
    {
        // found in functions database ?
        if(MaxName != 0) BufString = MaxName;
        // found in constants database ?
        if(MaxName2 != 0) BufString = MaxName2;
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Display API functions/constants listbox
void DisplayAPIWindow(long dx, long dy, HWND hWnd, CStr APIFncN)
{
    CStr ReturnValue;
    long EntryIndex = 0;
    long MaxName = 0;
    long MaxName2 = 0;

    if((UseDbFnc == 0) && (UseDbConst == 0)) return;
    // Retrieve all names
    ChildStruct = LoadStructure(CurrentForm);
    APIListWidth = 0;
    APIListHeight = 1;
    CreateSplashDialog(dx, dy, APIListWidth, APIListHeight, "", ChildStruct->hChildCodeMax, NULL, NULL, &FRMAPIListInitProc, &FRMAPIListWinHook, 0, 0, SW_HIDE);
    ListBoxReset(APIhListBox);
    if(FncBase != 0) if(UseDbFnc == 1) MaxName = FillAPIListBox(APIhListBox, FncBase);
    if(ConstBase != 0) if(UseDbConst == 1) MaxName2 = FillAPIListBox(APIhListBox, ConstBase);
    if(MaxName2 > MaxName) MaxName = MaxName2;
    if(MaxName != 0)
    {
        // Set sizes
        APIListWidth = (GDIGetFontWidth(FRMAPIListhWnd, APIListhFont) * MaxName) + 2 + 1 + GetSystemMetrics(SM_CXVSCROLL);
        APIListHeight = (GDIGetTextHeight(FRMAPIListhWnd, APIListhFont, "hg") * DbLinesNumbers) + 2 + 1;
        if(dx < 0) dx = 0;
        if(dy < 0) dy = 0;
        if((dx + APIListWidth) > GetSystemMetrics(SM_CXFULLSCREEN)) dx = GetSystemMetrics(SM_CXFULLSCREEN) - APIListWidth;
        if((dy + APIListHeight) > GetSystemMetrics(SM_CYFULLSCREEN)) dy = GetSystemMetrics(SM_CYFULLSCREEN) - APIListHeight;
        // Resize and display API list now
        ControlResize(FRMAPIListhWnd, dx, dy, APIListWidth, APIListHeight);
        ControlResize(APIhListBox, 1, 1, APIListWidth - 2, APIListHeight - 2);
        APIXDim = APIListWidth;
        APIYDim = APIListHeight;
        // Modify index of the listbox if necessary
        if(APIFncN.Len() != 0)
        {
            EntryIndex = SendMessage(APIhListBox, LB_FINDSTRING, -1, (long) APIFncN.Get_String());
            ListBoxSetIndex(APIhListBox, EntryIndex);
            ListBoxSetTopIndex(APIhListBox, EntryIndex);
        }
        else
        {
            ListBoxSetIndex(APIhListBox, -1);
            ListBoxSetTopIndex(APIhListBox, -1);
        }
        ControlVisible(FRMAPIListhWnd, 1);
        SetFocus(APIhListBox);
    }
    else
    {
        // Remove it
        ControlClose(FRMAPIListhWnd);
    }
}
