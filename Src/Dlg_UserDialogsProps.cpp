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
// Dlg_UserDialogsProps.cpp: User dialogs properties
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "ConstRes.h"
#include "MiscFunctions.h"
#include "Dlg_UserDialogs.h"
#include "Dlg_UserDialogsObjects.h"
#include "Dlg_UserDialogsProps.h"

// -----------------------------------------------------------------------
// Variables
HWND hPropListbox;
long PropScrollUp;
long MaxPropXSize;
HANDLE hPropComboPoints;

HWND CurrentEditControl;
HWND CurrentEditControlSubEdit;
long CurrentEditControlType;
long CurrentEditControlIndex;
long PropCurrentTop;
long PropCurrentLeft;
long PropCurrentWidth;
long PropCurrentHeight;
HWND hListBoolean;
long PropEditCancel;
HWND PropEdithwnd;

long SelectedInList;
CStr SelectedInListText;
int *hCurrentObject;

// -----------------------------------------------------------------------
// Properties form init
void CALLBACK FrmUserDiagPropsInitProc(HWND hWnd)
{
    HDC ProphDC = 0;
    SIZE PropSize;
    HGDIOBJ OldPropObject = 0;
    char *Property_Name;
    int i = 0;

    hPropListbox = CreateListBox(1, 20, ControlWidth(hWnd) - 1, ControlHeight(hWnd) - 20, hWnd, 0, &PropListboxHook, 0,
                                 LBS_USETABSTOPS | LBS_OWNERDRAWFIXED | WS_VSCROLL, WS_EX_STATICEDGE);
    hPropCombobox = CreateComboBox(0, 0, ControlWidth(hWnd), 150, hWnd, "", 1, 0, CBS_DROPDOWNLIST | WS_TABSTOP);
    ComboBoxAddItem(hPropCombobox, DialogName + (CStr) " - " + (CStr) "Dialog", -1);
    ComboBoxSetIndex(hPropCombobox, 0);
    FillPropertiesListbox(PROPDIAG_NBRPROPS);
    ProphDC = GetDC(hWnd);
    OldPropObject = SelectObject(ProphDC, WASerifFont);
    MaxPropXSize = 0;
    // Get largest property size
    for(i = 0; i < GetObjectDatas(hCurrentObject, OBJECT_PROPNBR); i++)
    {
        Property_Name = (char *) GetPropertyDatas(hCurrentObject, i, PROPERTY_NAME);
        GetTextExtentPoint32(ProphDC, Property_Name, strlen(Property_Name), &PropSize);
        if(PropSize.cx > MaxPropXSize) MaxPropXSize = PropSize.cx;
    }
    MaxPropXSize = MaxPropXSize + 10;
    SelectObject(ProphDC, OldPropObject);
    ReleaseDC(hWnd, ProphDC);
    ListBoxSetColumnsWidth(hPropListbox, MaxPropXSize);
    ListBoxSetIndex(hPropListbox, 0);
    ListBoxSetTopIndex(hPropListbox, 0);
    // Get the icons
    hPropComboPoints = LoadImage(ApphInstance, MAKEINTRESOURCE(ICON_RES_TOOL_BASE + ICON_RES_TOOL_COMBOPOINTS), IMAGE_ICON, 0, 0, 0);
}

// -----------------------------------------------------------------------
// Properties form hook
LRESULT CALLBACK FrmUserDiagPropsProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    long CurrentControlTop = 0;
    long CurrentControlLeft = 0;
    long CurrentControlWidth = 0;
    long CurrentControlHeight = 0;
    DRAWITEMSTRUCT LocalDraw;
    long XLargItem = 0;
    long LinesColor = 0;
    RECT FocusRect;
    RECT DatRect;
    long TextColor = 0;

    switch(uMsg)
    {
        case WM_DRAWITEM:
            CopyMemory(&LocalDraw, (void *) lParam, sizeof(LocalDraw));
            FocusRect.left = LocalDraw.rcItem.left;
            FocusRect.top = LocalDraw.rcItem.top + 1;
            FocusRect.right = MaxPropXSize;
            FocusRect.bottom = LocalDraw.rcItem.bottom;
            DatRect.left = MaxPropXSize + 1;
            DatRect.top = LocalDraw.rcItem.top + 1;
            DatRect.right = LocalDraw.rcItem.right;
            DatRect.bottom = LocalDraw.rcItem.bottom;
            switch(LocalDraw.itemAction)
            {
                // Redraw item
                case ODA_DRAWENTIRE:
                    XLargItem = ControlClientWidth(LocalDraw.hwndItem);
                    LinesColor = 0xC0C0C0C0;
                    TextColor = GetSysColor(COLOR_WINDOWTEXT);
                    if(LocalDraw.itemState == ODS_SELECTED)
                    {
                        CurrentControlHeight = ControlHeight(CurrentEditControl);
                        PropCurrentTop = DatRect.top + (DatRect.bottom - DatRect.top) + 1;
                        PropCurrentLeft = MaxPropXSize + 1;
                        PropCurrentWidth = DatRect.right - MaxPropXSize - 1;
                        PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                        CurrentControlTop = DatRect.top;
                        switch(CurrentEditControlType)
                        {
                            case PROP_STRING:
                                SetWindowPos(CurrentEditControl, 0, 0, 0, PropCurrentWidth, CurrentControlHeight, SWP_NOZORDER | SWP_NOMOVE);
                                break;
                            case PROP_INTEGER:
                                SetWindowPos(CurrentEditControl, 0, 0, 0, PropCurrentWidth, CurrentControlHeight, SWP_NOZORDER | SWP_NOMOVE);
                                break;
                            default:
                                CurrentControlLeft = ControlClientWidth(hPropListbox) - ControlWidth(CurrentEditControl);
                                CurrentControlWidth = CurrentControlLeft - ControlLeft(CurrentEditControlSubEdit) + GetSystemMetrics(SM_CXVSCROLL);
                                if(CurrentControlLeft < PropCurrentLeft) CurrentControlLeft = PropCurrentLeft;
                                CurrentControlWidth = (DatRect.right - DatRect.left) - ControlWidth(CurrentEditControl) - 1;
                                SetWindowPos(CurrentEditControlSubEdit, 0, 0, 0, CurrentControlWidth, CurrentControlHeight - 1, SWP_NOZORDER | SWP_NOMOVE);
                                SetWindowPos(CurrentEditControl, 0, CurrentControlLeft, CurrentControlTop - 1, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
                                break;
                        }
                        if(PropScrollUp == 1)
                        {
                            switch(GetPropertyDatas(hCurrentObject, LocalDraw.itemID, PROPERTY_TYPE))
                            {
                                case PROP_STRING:
                                    CurrentEditControlIndex = LocalDraw.itemID;
                                    CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                    PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                    ControlClose(CurrentEditControlSubEdit);
                                    ControlClose(CurrentEditControl);
                                    if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_LOCKED) == 1)
                                    {
                                        CurrentEditControl = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropEditHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY, 0);
                                    }
                                    else
                                    {
                                        CurrentEditControl = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropEditHook, WS_TABSTOP | ES_AUTOHSCROLL, 0);
                                    }
                                    TextBoxSetMaxLen(CurrentEditControl, 255);
                                    break;
                                case PROP_INTEGER:
                                    CurrentEditControlIndex = LocalDraw.itemID;
                                    CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                    PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                    ControlClose(CurrentEditControlSubEdit);
                                    ControlClose(CurrentEditControl);
                                    if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_LOCKED) == 1)
                                    {
                                        CurrentEditControl = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropEditHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER | ES_READONLY, 0);
                                    }
                                    else
                                    {
                                        CurrentEditControl = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropEditHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER, 0);
                                    }
                                    TextBoxSetMaxLen(CurrentEditControl, 5);
                                    break;
                                case PROP_BOOLEAN:
                                    CurrentEditControlIndex = LocalDraw.itemID;
                                    CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                    PropCurrentTop = DatRect.top + (DatRect.bottom - DatRect.top) + 1;
                                    PropCurrentLeft = MaxPropXSize + 1;
                                    PropCurrentWidth = DatRect.right - MaxPropXSize - 1;
                                    PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                    ControlClose(CurrentEditControlSubEdit);
                                    CurrentEditControlSubEdit = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropSubComboHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY, 0);
                                    TextBoxSetMaxLen(CurrentEditControlSubEdit, 5);
                                    ControlClose(CurrentEditControl);
                                    CurrentEditControl = CreateButton(DatRect.right - (DatRect.bottom - DatRect.top), DatRect.top - 1, (DatRect.bottom - DatRect.top), (DatRect.bottom - DatRect.top), hPropListbox, "6", 0, 0, 0, &PropBooleanHook, WS_TABSTOP, 0);
                                    ControlSetFont(CurrentEditControl, WAMarlettFont9);
                                    break;
                                case PROP_COMBO:
                                    CurrentEditControlIndex = LocalDraw.itemID;
                                    CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                    PropCurrentTop = DatRect.top + (DatRect.bottom - DatRect.top) + 1;
                                    PropCurrentLeft = MaxPropXSize + 1;
                                    PropCurrentWidth = DatRect.right - MaxPropXSize - 1;
                                    PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                    ControlClose(CurrentEditControlSubEdit);
                                    CurrentEditControlSubEdit = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropSubComboHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY, 0);
                                    TextBoxSetMaxLen(CurrentEditControlSubEdit, 255);
                                    ControlClose(CurrentEditControl);
                                    CurrentEditControl = CreateButton(DatRect.right - (DatRect.bottom - DatRect.top), DatRect.top - 1, (DatRect.bottom - DatRect.top), (DatRect.bottom - DatRect.top), hPropListbox, "6", 0, 0, 0, &PropComboHook, WS_TABSTOP, 0);
                                    ControlSetFont(CurrentEditControl, WAMarlettFont9);
                                    break;
                                case PROP_FONTSELECT:
                                    CurrentEditControlIndex = LocalDraw.itemID;
                                    CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                    PropCurrentTop = DatRect.top + (DatRect.bottom - DatRect.top) + 1;
                                    PropCurrentLeft = MaxPropXSize + 1;
                                    PropCurrentWidth = DatRect.right - MaxPropXSize - 1;
                                    PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                    ControlClose(CurrentEditControlSubEdit);
                                    CurrentEditControlSubEdit = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, 0, WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY, 0);
                                    TextBoxSetMaxLen(CurrentEditControlSubEdit, 255);
                                    ControlClose(CurrentEditControl);
                                    CurrentEditControl = CreateButton(DatRect.right - (DatRect.bottom - DatRect.top), DatRect.top - 1, (DatRect.bottom - DatRect.top), (DatRect.bottom - DatRect.top), hPropListbox, "", 0, IMAGE_ICON, (HICON) hPropComboPoints, &PropFontHook, WS_TABSTOP, 0);
                                    break;
                            }
                            PropScrollUp = 0;
                        }
                        TextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
                        FocusRect.top = FocusRect.top - 1;
                        FocusRect.bottom = FocusRect.bottom - 1;
                        FillRect(LocalDraw.hDC, &FocusRect, GetSysColorBrush(COLOR_HIGHLIGHT));
                        FocusRect.top = FocusRect.top + 1;
                        FocusRect.bottom = FocusRect.bottom + 1;
                    }
                    GDIWriteText(LocalDraw.hDC, LocalDraw.rcItem.left + 2, LocalDraw.rcItem.top + 1,
                                 (char *) GetPropertyDatas(hCurrentObject, LocalDraw.itemID, PROPERTY_NAME),
                                 TextColor, WASerifFont, 1, 0);
                    TextColor = GetSysColor(COLOR_WINDOWTEXT);
                    GDIWriteText(LocalDraw.hDC, DatRect.left + 1, LocalDraw.rcItem.top + 1,
                                 (char *) GetPropertyDatas(hCurrentObject, LocalDraw.itemID, PROPERTY_STORAGE),
                                 TextColor, WASerifFont, 1, 0);
                    if((LocalDraw.rcItem.bottom - 1) < ControlClientHeight(hPropListbox))
                    {
                        GDIDrawLine(LocalDraw.hwndItem, LocalDraw.rcItem.left, LocalDraw.rcItem.bottom - 1, XLargItem, LocalDraw.rcItem.bottom - 1, LinesColor);
                    }
                    if(XLargItem > MaxPropXSize) GDIDrawLine(LocalDraw.hwndItem, MaxPropXSize, LocalDraw.rcItem.top, MaxPropXSize, LocalDraw.rcItem.bottom - 1, LinesColor);
                    return(1);
                // Item selection (no scrolling)
                case ODA_SELECT:
                    if(hListBoolean != 0)
                    {
                        ControlClose(hListBoolean);
                        hListBoolean = 0;
                    }
                    if(LocalDraw.itemState == ODS_SELECTED)
                    {
                        TextColor = GetSysColor(COLOR_HIGHLIGHTTEXT);
                        FocusRect.top = FocusRect.top - 1;
                        FocusRect.bottom = FocusRect.bottom - 1;
                        FillRect(LocalDraw.hDC, &FocusRect, GetSysColorBrush(COLOR_HIGHLIGHT));
                        FocusRect.top = FocusRect.top + 1;
                        FocusRect.bottom = FocusRect.bottom + 1;
                        switch(GetPropertyDatas(hCurrentObject, LocalDraw.itemID, PROPERTY_TYPE))
                        {
                            case PROP_STRING:
                                CurrentEditControlIndex = LocalDraw.itemID;
                                CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                ControlClose(CurrentEditControlSubEdit);
                                ControlClose(CurrentEditControl);
                                CurrentEditControlSubEdit = 0;
                                CurrentEditControl = 0;
                                if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_LOCKED) == 1)
                                {
                                    CurrentEditControl = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropEditHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY, 0);
                                }
                                else
                                {
                                    CurrentEditControl = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropEditHook, WS_TABSTOP | ES_AUTOHSCROLL, 0);
                                }
                                break;
                            case PROP_INTEGER:
                                CurrentEditControlIndex = LocalDraw.itemID;
                                CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                ControlClose(CurrentEditControlSubEdit);
                                ControlClose(CurrentEditControl);
                                CurrentEditControlSubEdit = 0;
                                CurrentEditControl = 0;
                                if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_LOCKED) == 1)
                                {
                                    CurrentEditControl = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropEditHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER | ES_READONLY, 0);
                                }
                                else
                                {
                                    CurrentEditControl = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropEditHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_NUMBER, 0);
                                }
                                TextBoxSetMaxLen(CurrentEditControl, 5);
                                break;
                            case PROP_BOOLEAN:
                                CurrentEditControlIndex = LocalDraw.itemID;
                                CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                PropCurrentTop = DatRect.top + (DatRect.bottom - DatRect.top) + 1;
                                PropCurrentLeft = MaxPropXSize + 1;
                                PropCurrentWidth = DatRect.right - MaxPropXSize - 1;
                                PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                ControlClose(CurrentEditControlSubEdit);
                                CurrentEditControlSubEdit = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropSubComboHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY, 0);
                                ControlClose(CurrentEditControl);
                                CurrentEditControl = CreateButton(DatRect.right - (DatRect.bottom - DatRect.top), DatRect.top - 1, (DatRect.bottom - DatRect.top), (DatRect.bottom - DatRect.top), hPropListbox, "6", 0, 0, 0, &PropBooleanHook, WS_TABSTOP, 0);
                                ControlSetFont(CurrentEditControl, WAMarlettFont9);
                                break;
                            case PROP_COMBO:
                                CurrentEditControlIndex = LocalDraw.itemID;
                                CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                PropCurrentTop = DatRect.top + (DatRect.bottom - DatRect.top) + 1;
                                PropCurrentLeft = MaxPropXSize + 1;
                                PropCurrentWidth = DatRect.right - MaxPropXSize - 1;
                                PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                ControlClose(CurrentEditControlSubEdit);
                                CurrentEditControlSubEdit = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, &PropSubComboHook, WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY, 0);
                                ControlClose(CurrentEditControl);
                                CurrentEditControl = CreateButton(DatRect.right - (DatRect.bottom - DatRect.top), DatRect.top - 1, (DatRect.bottom - DatRect.top), (DatRect.bottom - DatRect.top), hPropListbox, "6", 0, 0, 0, &PropComboHook, WS_TABSTOP, 0);
                                ControlSetFont(CurrentEditControl, WAMarlettFont9);
                                break;
                            case PROP_FONTSELECT:
                                CurrentEditControlIndex = LocalDraw.itemID;
                                CurrentEditControlType = GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_TYPE);
                                PropCurrentTop = DatRect.top + (DatRect.bottom - DatRect.top) + 1;
                                PropCurrentLeft = MaxPropXSize + 1;
                                PropCurrentWidth = DatRect.right - MaxPropXSize - 1;
                                PropCurrentHeight = (DatRect.bottom - DatRect.top) - 1;
                                ControlClose(CurrentEditControlSubEdit);
                                CurrentEditControlSubEdit = CreateTextBox(DatRect.left + 1, DatRect.top, (DatRect.right - DatRect.left) - PropCurrentHeight, (DatRect.bottom - DatRect.top) - 1, hPropListbox, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), 0, 0, WS_TABSTOP | ES_AUTOHSCROLL | ES_READONLY, 0);
                                ControlClose(CurrentEditControl);
                                CurrentEditControl = CreateButton(DatRect.right - (DatRect.bottom - DatRect.top), DatRect.top - 1, (DatRect.bottom - DatRect.top), (DatRect.bottom - DatRect.top), hPropListbox, "...", 0, IMAGE_ICON, (HICON) hPropComboPoints, &PropFontHook, WS_TABSTOP, 0);
                                break;
                        }
                    }
                    else
                    {
                        TextColor = GetSysColor(COLOR_WINDOWTEXT);
                        FocusRect.top = FocusRect.top - 1;
                        FocusRect.bottom = FocusRect.bottom - 1;
                        FillRect(LocalDraw.hDC, &FocusRect, GetSysColorBrush(COLOR_WINDOW));
                        FocusRect.top = FocusRect.top + 1;
                        FocusRect.bottom = FocusRect.bottom + 1;
                        PropScrollUp = 1;
                    }
                    GDIWriteText(LocalDraw.hDC, LocalDraw.rcItem.left + 2, LocalDraw.rcItem.top + 1, (char *) GetPropertyDatas(hCurrentObject, LocalDraw.itemID, PROPERTY_NAME), TextColor, WASerifFont, 1, 0);
            }
            return(1);
        case WM_SIZE:
            MoveWindow(hPropListbox, 1, 24, ControlClientWidth(hWnd) - 2, ControlClientHeight(hWnd) - 25, 1);
            MoveWindow(hPropCombobox, 1, 1, ControlClientWidth(hWnd) - 2, 150, 1);
            if(hListBoolean != 0)
            {
                ControlClose(hListBoolean);
                hListBoolean = 0;
            }
            break;
        case WM_CLOSE:
            PropertiesXPos = ControlLeft(hWnd);
            PropertiesYPos = ControlTop(hWnd);
            PropertiesXLarg = ControlWidth(hWnd);
            PropertiesYLong = ControlHeight(hWnd);
            break;
        case WM_DESTROY:
            ToolBarSetButtonChecked(hDiagTools, DIALOGTOOLS_PROPERTIES, 0);
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Fill the properties labels
void FillPropertiesStrings(void)
{
    int *hDialogEntry = 0;
    CStr BufString;

    ControlsList.Erase();

    // Create a primary entry for the main dialog
    hDialogEntry = AddObjectEntry(CONTROL_DIALOG, 22, 0, 0, 0, 0, (long) &Prop_DialogDimWidth, 21, (long) &Prop_DialogDimHeight, 9, FRMDiaghwnd, NULL, NULL, 0);
    hCurrentObject = hDialogEntry;
    // Fill it's properties
    StorePropertyHeader(0, hDialogEntry, "(ID)", PROP_STRING, PROPFLAG_NONE, 0, 0, 1, "DIALOG_0");
    StorePropertyHeader(1, hDialogEntry, "Border", PROP_COMBO, PROPFLAG_BORDER, AllocSubListBox("(None)|Fixed Single|Sizable|Fixed Dialog|Fixed ToolWindow|Sizable ToolWindow"), 0, 0, "Sizable");
    StorePropertyHeader(2, hDialogEntry, "Caption", PROP_STRING, PROPFLAG_CAPTION, 0, (long) &Prop_DialogCaption, 0, ControlGetText(FRMDiaghwnd));
    StorePropertyHeader(3, hDialogEntry, "Child", PROP_BOOLEAN, PROPFLAG_NONE, 0, 0, 0, "False");
    StorePropertyHeader(4, hDialogEntry, "ClassName", PROP_STRING, PROPFLAG_NONE, 0, 0, 0, "");
    StorePropertyHeader(5, hDialogEntry, "Clipping", PROP_BOOLEAN, PROPFLAG_NONE, 0, 0, 0, "False");
    StorePropertyHeader(6, hDialogEntry, "Enabled", PROP_BOOLEAN, PROPFLAG_NONE, 0, 0, 0, "True");
    StorePropertyHeader(7, hDialogEntry, "FontName", PROP_FONTSELECT, PROPFLAG_NONE, 0, (long) &Prop_DialogFontName, 0, "MS Sans Serif");
    StorePropertyHeader(8, hDialogEntry, "FontSize", PROP_INTEGER, PROPFLAG_NONE, 0, (long) &Prop_DialogFontSize, 0, "8");
    StorePropertyHeader(9, hDialogEntry, "Height", PROP_INTEGER, PROPFLAG_HEIGHT, 0, (long) &Prop_DialogHeight, 0, CurrentHeight);
    StorePropertyHeader(10, hDialogEntry, "Left", PROP_INTEGER, PROPFLAG_LEFT, 0, (long) &Prop_DialogLeft, 0, ControlLeft(FRMDiaghwnd));
    StorePropertyHeader(11, hDialogEntry, "MaxButton", PROP_BOOLEAN, PROPFLAG_MAXIMIZE, 0, (long) &Prop_DialogMaxButton, 0, "True");
    StorePropertyHeader(12, hDialogEntry, "MenuID", PROP_COMBO, PROPFLAG_NONE, AllocSubListBox(GetMenusIDs()), 0, 0, "(None)");
    StorePropertyHeader(13, hDialogEntry, "MinButton", PROP_BOOLEAN, PROPFLAG_MINIMIZE, 0, (long) &Prop_DialogMinButton, 0, "True");
    StorePropertyHeader(14, hDialogEntry, "ScrollBars", PROP_COMBO, PROPFLAG_SCROLLBARS, AllocSubListBox("(None)|Horizontal|Vertical|Both"), (long) &Prop_DialogScrollBars, 0, "(None)");
    StorePropertyHeader(15, hDialogEntry, "StartupPos", PROP_COMBO, PROPFLAG_NONE, AllocSubListBox("(Manual)|Centered|On Mouse"), 0, 0, "(Manual)");
    StorePropertyHeader(16, hDialogEntry, "SysMenu", PROP_BOOLEAN, PROPFLAG_SYSMENU, 0, 0, 0, "True");
    StorePropertyHeader(17, hDialogEntry, "Top", PROP_INTEGER, PROPFLAG_TOP, 0, (long) &Prop_DialogTop, 0, ControlTop(FRMDiaghwnd));
    StorePropertyHeader(18, hDialogEntry, "TopMost", PROP_BOOLEAN, PROPFLAG_NONE, 0, 0, 0, "False");
    StorePropertyHeader(19, hDialogEntry, "Visible", PROP_BOOLEAN, PROPFLAG_NONE, 0, 0, 0, "True");
    StorePropertyHeader(20, hDialogEntry, "Width", PROP_INTEGER, PROPFLAG_WIDTH, 0, (long) &Prop_DialogWidth, 0, CurrentWidth);
    StorePropertyHeader(21, hDialogEntry, "WindowState", PROP_COMBO, PROPFLAG_NONE, AllocSubListBox("(Normal)|Minimized|Maximized"), 0, 0, "(Normal)");
}

// -----------------------------------------------------------------------
// Create a memory block for sub listbox entries
long AllocSubListBox(CStr StringToStore)
{
    long ReturnValue = 0;

    ReturnValue = (long) calloc(StringToStore.Len() + 1, 1);
    RtlCopyMemory((void *) ReturnValue, StringToStore.Get_String(), StringToStore.Len());
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Create empty listbox entries for forms
void FillPropertiesListbox(long Entries)
{
    int i = 0;

    ListBoxReset(hPropListbox);
    for(i = 0; i <= Entries; i++)
    {
        ListBoxAddItem(hPropListbox, "", -1);
    }
}

// -----------------------------------------------------------------------
// Properties elements

// -----------------------------------------------------------------------
// Boolean buttons hook in properties
LRESULT CALLBACK PropBooleanHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETFOCUS:
            return(0);
        case WM_KILLFOCUS:
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Combo buttons hook in properties
LRESULT CALLBACK PropComboHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETFOCUS:
            return(0);
        case WM_KILLFOCUS:
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// File buttons hook in properties
LRESULT CALLBACK PropFileHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETFOCUS:
            return(0);
        case WM_KILLFOCUS:
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Font buttons hook in properties
LRESULT CALLBACK PropFontHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETFOCUS:
            return(0);
        case WM_KILLFOCUS:
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Boolean/Combos sub edit hook in properties
LRESULT CALLBACK PropSubComboHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_SETFOCUS:
            return(0);
        case WM_KILLFOCUS:
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Strings/Number edit hook in properties
LRESULT CALLBACK PropEditHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CHAR:
            switch(wParam)
            {
                case 27:
                    PropEditCancel = 1;
                    SetFocus(hPropListbox);
                    break;
                case 13:
                    PropEditCancel = 0;
                    SetFocus(hPropListbox);
                    break;
            }
            break;
        case WM_SETFOCUS:
            PropEditCancel = 0;
            break;
        case WM_KILLFOCUS:
            PropEdithwnd = hWnd;
            if(PropEditCancel == 0)
            {
                SetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE, (long) ControlGetText(hWnd).Get_String());
                SelectedInList = -1;
                SelectedInListText = (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE);
                // Execute property hook
                if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_ROUTINE) != 0)
                {
                    JumpToAddr((FARPROC) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_ROUTINE));
                }
            }
            else
            {
                ControlSetText(hWnd, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE));
            }
            break;
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Font buttons hook in properties
LRESULT CALLBACK PropListboxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    CStr StrSubListBox;
    long *ArraySub = 0;
    int i = 0;
    char *ComboSubList;

    switch(uMsg)
    {
        case WM_VSCROLL:
            if(hListBoolean != 0)
            {
                ControlClose(hListBoolean);
                hListBoolean = 0;
            }
            break;
        case WM_CTLCOLORSTATIC:
            SetBkColor((HDC) wParam, GetSysColor(COLOR_WINDOW));
            return((LRESULT) hSubEditBrush);
        case WM_COMMAND:
            if((HWND) lParam == CurrentEditControl)
            {
                switch(CurrentEditControlType)
                {
                    case PROP_BOOLEAN:
                        if(hListBoolean != 0)
                        {
                            ControlClose(hListBoolean);
                            hListBoolean = 0;
                        }
                        else
                        {
                            PropCurrentTop = ControlTop(CurrentEditControl) - 2 - ControlTop(hPropListbox) + ControlHeight(CurrentEditControl) + 2;
                            // Bound bottom coordinate
                            if(((PropCurrentHeight * 2) + (PropCurrentTop)) > ControlClientHeight(hPropListbox))
                            {
                                PropCurrentTop = PropCurrentTop - (PropCurrentHeight * 2) - ControlHeight(CurrentEditControl);
                            }
                            hListBoolean = CreateListBox(PropCurrentLeft, PropCurrentTop - 2, PropCurrentWidth, PropCurrentHeight * 2, hPropListbox, 0, &PropSubListboxHook, 0, WS_VSCROLL | WS_BORDER, 0);
                            ListBoxAddItem(hListBoolean, "False", -1);
                            ListBoxAddItem(hListBoolean, "True", -1);
                            ListBoxSetIndex(hListBoolean, ListBoxItemExist(hListBoolean, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE)));
                            SelectedInList = -1;
                            SelectedInListText = "";
                            SetFocus(hListBoolean);
                        }
                        break;
                    case PROP_COMBO:
                        if(hListBoolean != 0)
                        {
                            ControlClose(hListBoolean);
                            hListBoolean = 0;
                        }
                        else
                        {
                            // Bound bottom coordinate
                            PropCurrentTop = ControlTop(CurrentEditControl) - 2 - ControlTop(hPropListbox) + ControlHeight(CurrentEditControl) + 2;
                            if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_EXTEND) != 0) {
                                ComboSubList = (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_EXTEND);
                                StrSubListBox = StrSubListBox.String(strlen(ComboSubList), 1);
                                RtlCopyMemory(StrSubListBox.Get_String(), ComboSubList, strlen(ComboSubList));
                                ArraySub = StringSplit(&StrSubListBox, "|");
                                if(StringGetSplitUBound(ArraySub) != -1)
                                {
                                    // Bound bottom coordinate
                                    if(((PropCurrentHeight * (StringGetSplitUBound(ArraySub) + 1)) + (PropCurrentTop)) > ControlClientHeight(hPropListbox))
                                    {
                                        PropCurrentTop = PropCurrentTop - (PropCurrentHeight * (StringGetSplitUBound(ArraySub) + 1)) - ControlHeight(CurrentEditControl);
                                    }
                                    hListBoolean = CreateListBox(PropCurrentLeft, PropCurrentTop - 2, PropCurrentWidth, PropCurrentHeight * (StringGetSplitUBound(ArraySub) + 1), hPropListbox, 0, &PropSubListboxHook, 0, WS_VSCROLL | WS_BORDER, 0);
                                    for(i = 0; i <= StringGetSplitUBound(ArraySub); i++)
                                    {
                                        ListBoxAddItem(hListBoolean, StringGetSplitElement(&StrSubListBox, ArraySub, i), -1);
                                    }
                                }
                                StringReleaseSplit(ArraySub);
                            }
                            ListBoxSetIndex(hListBoolean, ListBoxItemExist(hListBoolean, (char *) GetPropertyDatas(hCurrentObject,
                                                                                                                       CurrentEditControlIndex,
                                                                                                                       PROPERTY_STORAGE)));
                            SelectedInList = -1;
                            SelectedInListText = "";
                            SetFocus(hListBoolean);
                        }
                        break;
                    case PROP_FILESELECT:
                        break;
                    case PROP_FONTSELECT:
                        PropCurrentTop = ControlTop(CurrentEditControl) - ControlTop(hPropListbox) + ControlHeight(CurrentEditControl);
                        PropSelectFont();
                        break;
                }
                return(0);
            }
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Sub listboxes hook
LRESULT CALLBACK PropSubListboxHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch(uMsg)
    {
        case WM_CHAR:
            switch(wParam)
            {
                case 13:
                    SelectedInList = ListBoxGetSelItemIndex(hListBoolean);
                    SelectedInListText = ListBoxGetCurrentItem(hListBoolean);
                    ControlClose(hListBoolean);
                    SetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE, (long) SelectedInListText.Get_String());
                    ControlSetText(CurrentEditControlSubEdit, SelectedInListText);
                    // Jump to associated routine (if any)
                    if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_ROUTINE) != 0) JumpToAddr((FARPROC) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_ROUTINE));
                    hListBoolean = 0;
                    return(0);
                case 27:
                    SelectedInList = -1;
                    SelectedInListText = "";
                    ControlClose(hListBoolean);
                    hListBoolean = 0;
                    return(0);
            }
            break;
        // Close listbox and update the list
        case WM_LBUTTONUP:
            SelectedInList = ListBoxGetSelItemIndex(hListBoolean);
            SelectedInListText = ListBoxGetCurrentItem(hListBoolean);
            ControlClose(hListBoolean);
            SetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE, (long) SelectedInListText.Get_String());
            ControlSetText(CurrentEditControlSubEdit, SelectedInListText);
            // Jump to associated routine (if any)
            if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_ROUTINE) != 0) JumpToAddr((FARPROC) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_ROUTINE));
            hListBoolean = 0;
            return(0);
    }
    return(CallWindowProc((WNDPROC) GetWindowLong(hWnd, GWL_USERDATA), hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Select a font
void PropSelectFont(void)
{
    CStr FName;
    CStr BufString;
    long FSize = 0;
    HDC NextDC = 0;
    int i = 0;

    // Note: Font name must always be followed by font size
    if(strlen((char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex + 1, PROPERTY_STORAGE)) != 0)
    {
        FSize = atol((char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex + 1, PROPERTY_STORAGE));
    }
    ControlEnable(FRMDiagToolhwnd, 0);
    ControlEnable(FRMDiagTBhwnd, 0);
    ControlEnable(FRMDiagPropshwnd, 0);
    if(ComDlgChooseFont(FRMDiaghwnd, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE), FSize, 0) != 0)
    {
        for(i = 0; i <= LF_FACESIZE - 1; i++)
        {
            if(MyLogFont.lfFaceName[i] == 0) break;
            FName = FName + FName.Chr(MyLogFont.lfFaceName[i]);
        }
        if(FName.Len() != 0)
        {
            BufString = MyFont.iPointSize / 10;
            SetPropertyDatas(hCurrentObject, CurrentEditControlIndex + 1, PROPERTY_STORAGE, (long) BufString.Get_String());
            SetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_STORAGE, (long) FName.Get_String());
            ControlSetText(CurrentEditControlSubEdit, FName);
            // Set size
            NextDC = GetDC(hPropListbox);
            GDIWriteClippedText(NextDC, PropCurrentLeft + 1, PropCurrentTop + 1, PropCurrentWidth - 1, PropCurrentHeight, (char *) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex + 1, PROPERTY_STORAGE), GetSysColor(COLOR_WINDOWTEXT), WASerifFont, 0, GetSysColor(COLOR_WINDOW));
            ReleaseDC(hPropListbox, NextDC);
            if(GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_ROUTINE) != 0) JumpToAddr((FARPROC) GetPropertyDatas(hCurrentObject, CurrentEditControlIndex, PROPERTY_ROUTINE));
        }
    }
    ControlEnable(FRMDiagToolhwnd, 1);
    ControlEnable(FRMDiagTBhwnd, 1);
    ControlEnable(FRMDiagPropshwnd, 1);
}

// -----------------------------------------------------------------------
// Refresh properties list
void RefreshProperty(long Position, long Value)
{
    long PropTopIndex = 0;

    PropTopIndex = ListBoxGetTopIndex(hPropListbox);
}


// -----------------------------------------------------------------------
// Common object dimensions routines
// -----------------------------------------------------------------------

void Prop_DialogDimWidth(void)
{
    CStr BufString;
    
    BufString = ControlWidth(FRMDiaghwnd);
//    DiagLabelsFormStorage.Set(DiagLabelsFormDimRoutinePos.Get(2)->Content, BufString.Get_String());
//    RefreshProperty(DiagLabelsFormDimRoutinePos.Get(2)->Content, (atol(DiagLabelsFormStorage.Get(DiagLabelsFormDimRoutinePos.Get(2)->Content)->Content)));
}

void Prop_DialogDimHeight(void)
{
    CStr BufString;

    BufString = ControlHeight(FRMDiaghwnd);
//    DiagLabelsFormStorage.Set(DiagLabelsFormDimRoutinePos.Get(3)->Content, BufString.Get_String());
//    RefreshProperty(DiagLabelsFormDimRoutinePos.Get(3)->Content, atol(DiagLabelsFormStorage.Get(DiagLabelsFormDimRoutinePos.Get(3)->Content)->Content));
}

// -----------------------------------------------------------------------
// Common object routines
// -----------------------------------------------------------------------

void Prop_DialogCaption(void)
{
//    DiagLabelsFormStorage.Set_Entry(CurrentEditControlIndex,ControlGetText(PropEdithwnd).Get_String());
//    ControlSetText(FRMDiaghwnd, DiagLabelsFormStorage.Get(CurrentEditControlIndex)->Content);
}

void Prop_DialogWidth(void)
{
//    ControlResize(FRMDiaghwnd, ControlLeft(FRMDiaghwnd), ControlTop(FRMDiaghwnd), atol(DiagLabelsFormStorage.Get(CurrentEditControlIndex)->Content), ControlHeight(FRMDiaghwnd));
//    CurrentWidth = atol(DiagLabelsFormStorage.Get(CurrentEditControlIndex)->Content);
}

void Prop_DialogHeight(void)
{
//    ControlResize(FRMDiaghwnd, ControlLeft(FRMDiaghwnd), ControlTop(FRMDiaghwnd), ControlWidth(FRMDiaghwnd), atol(DiagLabelsFormStorage.Get(CurrentEditControlIndex)->Content));
//    CurrentHeight = atol(DiagLabelsFormStorage.Get(CurrentEditControlIndex)->Content);
}

void Prop_DialogLeft(void)
{
}

void Prop_DialogTop(void)
{
}

void Prop_DialogMinButton(void)
{
}

void Prop_DialogMaxButton(void)
{
}

// -----------------------------------------------------------------------
// Response for scrollbars attributes modifications
void Prop_DialogScrollBars(void)
{
    switch(SelectedInList)
    {
        case 0:
            ShowScrollBar(FRMDiaghwnd, SB_VERT, 0);
            ShowScrollBar(FRMDiaghwnd, SB_HORZ, 0);
            break;
        case 1:
            ShowScrollBar(FRMDiaghwnd, SB_VERT, 0);
            ShowScrollBar(FRMDiaghwnd, SB_HORZ, 1);
            break;
        case 2:
            ShowScrollBar(FRMDiaghwnd, SB_VERT, 1);
            ShowScrollBar(FRMDiaghwnd, SB_HORZ, 0);
            break;
        case 3:
            ShowScrollBar(FRMDiaghwnd, SB_VERT, 1);
            ShowScrollBar(FRMDiaghwnd, SB_HORZ, 1);
            break;
    }
}

// -----------------------------------------------------------------------
// Response for Font attributes modifications
void Prop_DialogFontName(void)
{
    if(CurrentFont != 0) DeleteObject(CurrentFont);
//    CurrentFont = GDIObtainFont(SelectedInListText, atol(DiagLabelsFormStorage.Get(CurrentEditControlIndex + 1)->Content), FRMDiaghwnd, 0, 0);
    ControlSetFont(FRMDiaghwnd, CurrentFont);
}

// -----------------------------------------------------------------------
// Response for font size modifications
void Prop_DialogFontSize(void)
{
    if(CurrentFont != 0) DeleteObject(CurrentFont);
    if(SelectedInListText.Len() != 0)
    {
//        CurrentFont = GDIObtainFont(DiagLabelsFormStorage.Get(CurrentEditControlIndex - 1)->Content, SelectedInListText.Get_Long(), FRMDiaghwnd, 0, 0);
//        ControlSetFont(FRMDiaghwnd, CurrentFont);
    }
}
