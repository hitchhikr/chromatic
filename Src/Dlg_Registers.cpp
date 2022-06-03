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
// Dlg_Registers.cpp: Used registers form
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MiscFunctions.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "Dlg_Registers.h"

// -----------------------------------------------------------------------
// Variables
long FrmRegsOn;
HWND FrmRegshWnd;
CStr FrmRegsFoundFile;
CStr FrmRegsPrimLinesTxt;
long *FrmRegsFoundLinesTxt;
long FrmRegsSelFirstLine;

CStr FrmRegsCmTextReg;

HWND FrmRegsListbox1;
HWND FrmRegsListbox2;
HWND FrmRegsCmdClose;
HWND FrmRegsCmdSave;
HWND FrmRegsCmdView;
HWND FrmRegsCmdToDoc;

long FrmRegsWidth;
long FrmRegsHeight;

CList <int> FoundLines;
CList <int> FoundPos;
CList <char *> FoundReg;
CList <char *> TabLines;
long ResultMem;
CStr ResultStr;

// -----------------------------------------------------------------------
// Initialize registers window
void CALLBACK FrmRegsInitProc(HWND hWnd)
{
    int i = 0;
    int j = 0;
    long LstIdx = 0;
    long FoundR = 0;
    CStr RegRef;
    CStr RepLines;
    CStr BufString;

    FrmRegsListbox1 = CreateListBox(0, 0, 50, -1, hWnd, 0, 0, 0, WS_TABSTOP, WS_EX_STATICEDGE);
    FrmRegsListbox2 = CreateListBox(50, 0, 98, -1, hWnd, 1, 0, 0, WS_TABSTOP | LBS_USETABSTOPS | WS_HSCROLL, WS_EX_STATICEDGE);
    FrmRegsCmdClose = CreateButton(150, 1, 77, 23, hWnd, "Close", 2, 0, 0, 0, BS_DEFPUSHBUTTON | WS_TABSTOP | WS_GROUP, Buttons_StaticEdge);
    FrmRegsCmdSave = CreateButton(150, 26, 77, 23, hWnd, "To file", 3, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    FrmRegsCmdView = CreateButton(150, 50, 77, 23, hWnd, "View", 4, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    FrmRegsCmdToDoc = CreateButton(150, 74, 77, 23, hWnd, "To doc", 5, 0, 0, 0, WS_TABSTOP, Buttons_StaticEdge);
    LstIdx = 0;
    // Construct the register listbox
    for(i = 0; i < FoundReg.Amount(); i++)
    {
        if(strlen(FoundReg.Get(i)->Content) != 0)
        {
            FoundR = 0;
            if(ListBoxCount(FrmRegsListbox1) != 0)
            {
                for(j = 0; j <= ListBoxCount(FrmRegsListbox1) - 1; j++)
                {
                    if(_strcmpi(ListBoxGetItem(FrmRegsListbox1, j).Get_String(), FoundReg.Get(i)->Content) == 0)
                    {
                        FoundR = 1;
                        break;
                    }
                }
            }
            if(FoundR == 0)
            {
                BufString = FoundReg.Get(i)->Content;
                BufString = BufString.Upper_Case(); 
                ListBoxAddItem(FrmRegsListbox1, BufString, LstIdx);
                LstIdx++;
            }
        }
    }
    // Construct the lines ref. array for each register
    TabLines.Erase();

    for(i = 0; i <= ListBoxCount(FrmRegsListbox1) - 1; i++)
    {
        RegRef = "";
        for(j = 0; j < FoundReg.Amount(); j++)
        {
            if(_strcmpi(ListBoxGetItem(FrmRegsListbox1, i).Get_String(),
                        FoundReg.Get(j)->Content) == 0) RegRef = RegRef + (CStr) StringNumberComplement(j, 10).Get_String() + (CStr) "|";
        }
        TabLines.Add(RegRef.Get_String());
        ListBoxSetItemData(FrmRegsListbox1, i, TabLines.Amount() - 1);
    }
    if(ListBoxCount(FrmRegsListbox1) > 1)
    {
        ControlSetText(hWnd, "Found " + (CStr) ListBoxCount(FrmRegsListbox1) + (CStr) " used registers in file: " + (CStr) FrmRegsFoundFile);
    }
    else
    {
        ControlSetText(hWnd, "Found " + (CStr) ListBoxCount(FrmRegsListbox1) + (CStr) " used register in file: " + (CStr) FrmRegsFoundFile);
    }
    ListBoxSetIndex(FrmRegsListbox1, 0);
    FrmRegsOn = 1;
    FillRegisterList();
}

// -----------------------------------------------------------------------
// Default window hook
LRESULT CALLBACK FrmRegsWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SIZE:
            FrmRegsWidth = ControlClientWidth(hWnd);
            FrmRegsHeight = ControlClientHeight(hWnd);
            MoveWindow(FrmRegsCmdClose, FrmRegsWidth - ControlWidth(FrmRegsCmdClose), 1, 77, 23, 1);
            MoveWindow(FrmRegsCmdSave, FrmRegsWidth - ControlWidth(FrmRegsCmdSave), 26, 77, 23, 1);
            MoveWindow(FrmRegsCmdView, FrmRegsWidth - ControlWidth(FrmRegsCmdView), 50, 77, 23, 1);
            MoveWindow(FrmRegsCmdToDoc, FrmRegsWidth - ControlWidth(FrmRegsCmdView), 74, 77, 23, 1);
            MoveWindow(FrmRegsListbox1, 1, 1, FrmRegsWidth / 9, FrmRegsHeight - 2, 1);
            MoveWindow(FrmRegsListbox2, ControlWidth(FrmRegsListbox1) + 4, 1, FrmRegsWidth - ControlWidth(FrmRegsCmdClose) - ControlWidth(FrmRegsListbox1) - 2 - 5, FrmRegsHeight - 2, 1);
            ListBoxSetHorzScrollWidth(FrmRegsListbox2, ControlWidth(FrmRegsListbox2) * 2);
            return(DefWindowProc(hWnd, uMsg, wParam, lParam));;
        case WM_DESTROY:
            StringReleaseSplit(FrmRegsFoundLinesTxt);
            FrmRegsOn = 0;
            return(0);
        case WM_COMMAND:
            if((HWND) lParam == FrmRegsCmdClose)
            {
                ControlClose(hWnd);
                return(0);
            }
            else if((HWND) lParam == FrmRegsCmdSave)
            {
                FrmRegsSaveReport();
                return(0);
            }
            else if((HWND) lParam == FrmRegsCmdView)
            {
                if(ListBoxGetSelItemIndex(FrmRegsListbox2) != -1) GoToLine(FrmRegsFoundFile, ListBoxGetItemData(FrmRegsListbox2, ListBoxGetSelItemIndex(FrmRegsListbox2)), 1);
                return(0);
            }
            else if((HWND) lParam == FrmRegsCmdToDoc)
            {
                FrmRegsToDoc();
                return(0);
            }
            else if((HWND) lParam == FrmRegsListbox1)
            {
                if((wParam & 0xFFFF0000) == 0x10000) FillRegisterList();
                return(0);
            }
            else if((HWND) lParam == FrmRegsListbox2)
            {
                if((wParam & 0xFFFF0000) == 0x20000)
                {
                    if(ListBoxGetSelItemIndex(FrmRegsListbox2) != -1) GoToLine(FrmRegsFoundFile, ListBoxGetItemData(FrmRegsListbox2, ListBoxGetSelItemIndex(FrmRegsListbox2)), 1);
                    return(0);
                }
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}
                        
// -----------------------------------------------------------------------
// Fill the listbox with datas (what else ?)
void FillRegisterList(void)
{
    long *RefStr = 0;
    CStr StrCurrentLine;
    int i = 0;
    long Lst2Idx = 0;
    long CurLine = 0;
    CStr StrLine;

    if(ListBoxGetSelItemIndex(FrmRegsListbox1) != -1)
    {
        StrLine = TabLines.Get(ListBoxGetItemData(FrmRegsListbox1, ListBoxGetSelItemIndex(FrmRegsListbox1)))->Content;
        RefStr = StringSplit(StrLine, "|");
        ListBoxReset(FrmRegsListbox2);
        Lst2Idx = 0;
        for(i = 0; i <= StringGetSplitUBound(RefStr); i++)
        {
            StrCurrentLine = StringGetSplitElement(StrLine, RefStr, i);
            if(StrCurrentLine.Len() != 0)
            {
                CurLine = FoundLines.Get(StrCurrentLine.Get_Long())->Content;
                ListBoxAddItem(FrmRegsListbox2, "Line: " + (CStr) StringNumberComplement(CurLine + 1, 10).Get_String() + (CStr) " >> " + (CStr) StringReplace(StringGetSplitElement(FrmRegsPrimLinesTxt, FrmRegsFoundLinesTxt, CurLine - FrmRegsSelFirstLine), "\r", "", 1, -1, Binary_Compare), Lst2Idx);
                ListBoxSetItemData(FrmRegsListbox2, Lst2Idx, CurLine);
                Lst2Idx++;
            }
        }
        StringReleaseSplit(RefStr);
        if(ListBoxCount(FrmRegsListbox2) != 0) ListBoxSetIndex(FrmRegsListbox2, 0);
    }
}

// -----------------------------------------------------------------------
// Save list
void FrmRegsSaveReport(void)
{
    CStr FName;
    CStr Filters;
    long i = 0;
    long j = 0;
    long GreatestType = 0;
    long GreatestFile = 0;
    long GreatestLn = 0;
    CStr ToWRiteVar1;
    CStr ToWRiteVar2;
    CStr ToWRiteVar3;
    long *RefToSave = 0;
    CStr SaveRefToSave;
    CStr SaveCurrentRefToSave;
    long CompLng = 0;
    CStr CompLngStr;
    CStr ToAdd;

    Filters = "All files (*.*)|*.*";
    ControlEnable(FrmRegshWnd, 0);
    FName = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, "", CurrentDir);
    if(FName.Len() != 0)
    {
        // List overview to check for greatest elements
        GreatestType = 8;
        GreatestFile = 10;
        GreatestLn = 12;
        ToAdd = "";
        // Header
        ResultStr = "File: " + (CStr) FrmRegsFoundFile + (CStr) "\r\n";
        ResultStr = ResultStr + (CStr) ResultStr.String(32, '-') + (CStr) "\r\n";
        ResultStr = ResultStr + (CStr) "Register       Line Related code\r\n";
        ResultStr = ResultStr + (CStr) ResultStr.String(32, '-') + (CStr) "\r\n";
        for(i = 0; i <= ListBoxCount(FrmRegsListbox1) - 1; i++)
        {
            ToAdd = "";
            if((GreatestType - ListBoxGetItem(FrmRegsListbox1, i).Len()) > 0) ToAdd = ToAdd.Space(GreatestType - ListBoxGetItem(FrmRegsListbox1, i).Len());
            ToWRiteVar1 = ListBoxGetItem(FrmRegsListbox1, i) + (CStr) ToAdd;
            ResultStr = ResultStr + (CStr) ToWRiteVar1 + (CStr) "\r\n";
            SaveRefToSave = TabLines.Get(ListBoxGetItemData(FrmRegsListbox1, i))->Content;
            RefToSave = StringSplit(SaveRefToSave, "|");
            for(j = 0; j <= StringGetSplitUBound(RefToSave); j++)
            {
                SaveCurrentRefToSave = StringGetSplitElement(SaveRefToSave, RefToSave, j);
                if(SaveCurrentRefToSave.Len() != 0)
                {
                    CompLng = (FoundLines.Get(SaveCurrentRefToSave.Get_Long())->Content + 1);
                    CompLngStr = CompLng;
                    if((10 - CompLngStr.Len()) > 0) CompLngStr = CompLngStr.String(10 - CompLngStr.Len(), ' ') + (CStr) CompLngStr;
                    ResultStr = ResultStr + (CStr) ResultStr.String(GreatestType + 1, ' ').Get_String() + (CStr) CompLngStr + (CStr) " " + (CStr) StringGetSplitElement(FrmRegsPrimLinesTxt, FrmRegsFoundLinesTxt, FoundLines.Get(SaveCurrentRefToSave.Get_Long())->Content - FrmRegsSelFirstLine).Trim() + (CStr) "\r\n";
                }
            }
            StringReleaseSplit(RefToSave);
        }
        if(MSaveFile(FName.Get_String(), (long) ResultStr.Get_String(), ResultStr.Len()) == 0)
        {
            MiscMsgBox(hMDIform.hWnd, "Error in saving file: '" + (CStr) FName + (CStr) "'.", MB_ERROR, Requesters);
        }
        else
        {
            MiscMsgBox(hMDIform.hWnd, "Report saved as '" + (CStr) FName + (CStr) "'.", MB_INFORMATION, Requesters);
        }
    }
    ControlEnable(FrmRegshWnd, 1);
}

// -----------------------------------------------------------------------
// Create a new document with list
void FrmRegsToDoc(void)
{
    CStr FName;
    CStr Filters;
    int i = 0;
    int j = 0;
    long GreatestType = 0;
    long GreatestFile = 0;
    long GreatestLn = 0;
    CStr ToWRiteVar1;
    CStr ToWRiteVar2;
    CStr ToWRiteVar3;
    long *RefToSave = 0;
    CStr SaveRefToSave;
    CStr SaveCurrentRefToSave;
    long CompLng = 0;
    CStr CompLngStr;
    CStr ToAdd;
    HWND NewChildHandle = 0;

    ControlEnable(FrmRegshWnd, 0);
    StoreLanguageToOpen("");
    NewChildHandle = CreateNewFile("Found registers.txt");
    if(NewChildHandle != 0)
    {
        ForceChildFile(NewChildHandle);
        // List overview to check for greatest elements
        GreatestType = 8;
        GreatestFile = 10;
        GreatestLn = 12;
        ToAdd = "";
        // Header
        ResultStr = "File: " + (CStr) FrmRegsFoundFile + (CStr) "\r\n";
        ResultStr = ResultStr + (CStr) ResultStr.String(32, '-').Get_String() + (CStr) "\r\n";
        ResultStr = ResultStr + (CStr) "Register       Line Related code\r\n";
        ResultStr = ResultStr + (CStr) ResultStr.String(32, '-').Get_String() + (CStr) "\r\n";
        for(i = 0; i <= ListBoxCount(FrmRegsListbox1) - 1; i++)
        {
            ToAdd = "";
            if((GreatestType - ListBoxGetItem(FrmRegsListbox1, i).Len()) > 0) ToAdd = ToAdd.Space(GreatestType - ListBoxGetItem(FrmRegsListbox1, i).Len());
            ToWRiteVar1 = ListBoxGetItem(FrmRegsListbox1, i) + (CStr) ToAdd;
            ResultStr = ResultStr + (CStr) ToWRiteVar1 + (CStr) "\r\n";
            SaveRefToSave = TabLines.Get(ListBoxGetItemData(FrmRegsListbox1, i))->Content;
            RefToSave = StringSplit(SaveRefToSave, "|");
            for(j = 0; j <= StringGetSplitUBound(RefToSave); j++)
            {
                SaveCurrentRefToSave = StringGetSplitElement(SaveRefToSave, RefToSave, j);
                if(SaveCurrentRefToSave.Len() != 0)
                {
                    CompLng = (FoundLines.Get(SaveCurrentRefToSave.Get_Long())->Content + 1);
                    CompLngStr = CompLng;
                    if((10 - CompLngStr.Len()) > 0) CompLngStr = CompLngStr.String(10 - CompLngStr.Len(), ' ') + (CStr) CompLngStr;
                    ResultStr = ResultStr + (CStr) ResultStr.String(GreatestType + 1, ' ').Get_String() + (CStr) CompLngStr + (CStr) " " + (CStr) StringGetSplitElement(FrmRegsPrimLinesTxt, FrmRegsFoundLinesTxt, FoundLines.Get(SaveCurrentRefToSave.Get_Long())->Content - FrmRegsSelFirstLine).Trim() + (CStr) "\r\n";
                }
            }
            StringReleaseSplit(RefToSave);
        }
        SendTextToChild(NewChildHandle, ResultStr);
    }
    ControlEnable(FrmRegshWnd, 1);
}
