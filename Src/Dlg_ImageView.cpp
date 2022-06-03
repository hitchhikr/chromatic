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
// Dlg_ImageView.cpp: Icon/Cursor/Bitmap viewer
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MDI_Form.h"
#include "Globals.h"
#include "ConstRes.h"
#include "MiscFunctions.h"
#include "Dlg_ImageView.h"

// -----------------------------------------------------------------------
// Variables
long DisplayType;
CStr FileToDisplay;
IMGVIEW ViewStruct;
HANDLE ImgHandle;
long NewViewMem;
HIMAGELIST hImgList;
long Patched;

// -----------------------------------------------------------------------
// Display an image, an icon or a cursor
void DisplayImageView(long ImageType, CStr ImageFileName)
{
    HDC hwDC = 0;
    HWND hFRMImageView = 0;
    
    FileToDisplay = ImageFileName;
    if(FileExist(FileToDisplay) == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Can't find file '" + (CStr) FileToDisplay + (CStr) "'.", MB_ERROR, Requesters);
        return;
    }
    DisplayType = ImageType;
    switch(DisplayType)
    {
        case VIEW_BITMAP:
            hFRMImageView = CreateNonModalDialog(-1, -1, 400, 250, hMDIform.hWnd, 0,
                                                 LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_STACK + ICON_BASE)), FileToDisplay,
                                                 &FrmImageViewInitProc, (WNDPROC) -1, 0,
                                                 WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX | WS_HSCROLL | WS_VSCROLL, SW_SHOW);
            break;
        default:
            hFRMImageView = CreateNonModalDialog(-1, -1, 150, 150, hMDIform.hWnd, 0,
                                                 LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_STACK + ICON_BASE)), FileToDisplay,
                                                 &FrmImageViewInitProc, (WNDPROC) -1, 0,
                                                 WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX | WS_HSCROLL | WS_VSCROLL, SW_SHOW);
            break;
    }
    NewViewMem = AllocMem(sizeof(ViewStruct));
    memset(&ViewStruct, 0, sizeof(ViewStruct));
    ViewStruct.Proc = &FrmImageViewWinHook;
    ViewStruct.hBitmap = ImgHandle;
    ViewStruct.CurrentImage = hImgList;
    ViewStruct.ImType = DisplayType;
    RtlCopyMemory((void *) NewViewMem, &ViewStruct, sizeof(ViewStruct));
    hwDC = GetDC(hFRMImageView);
    switch(ViewStruct.ImType)
    {
        case VIEW_ICON:
            DrawIconEx(hwDC, 0, 0, (HICON) ViewStruct.hBitmap, 0, 0, 0, 0, DI_NORMAL);
            break;
        case VIEW_CURSOR:
            DrawIconEx(hwDC, 0, 0, (HICON) ViewStruct.hBitmap, 0, 0, 0, 0, DI_NORMAL);
            break;
    }
    ReleaseDC(hFRMImageView, hwDC);
    SetWindowLong(hFRMImageView, GWL_USERDATA, NewViewMem);
    SetWindowLong(hFRMImageView, GWL_WNDPROC, (long) &FrmImageViewWinHook);
    ResizeImgView(hFRMImageView);
}

// -----------------------------------------------------------------------
// Initialize the image viewer window
void CALLBACK FrmImageViewInitProc(HWND hwnd)
{
    BITMAP BitmapInfos;

    hImgList = 0;
    switch(DisplayType)
    {
        case VIEW_ICON:
            ImgHandle = LoadImage(0, FileToDisplay.Get_String(), IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
            if(ImgHandle != 0) ScrollBarShowHide(hwnd, 64, 64, 1);
            break;
        case VIEW_CURSOR:
            ImgHandle = LoadImage(0, FileToDisplay.Get_String(), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
            if(ImgHandle != 0) ScrollBarShowHide(hwnd, 64, 64, 1);
            break;
        case VIEW_BITMAP:
            ImgHandle = LoadImage(0, FileToDisplay.Get_String(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
            if(ImgHandle != 0)
            {
                GetObject(ImgHandle, sizeof(BitmapInfos), &BitmapInfos);
                CreatePictureBox(0, 0, BitmapInfos.bmWidth, BitmapInfos.bmHeight, hwnd, ImgHandle, IMAGE_BITMAP, 0, 0, SS_CENTERIMAGE);
                ScrollBarShowHide(hwnd, BitmapInfos.bmWidth, BitmapInfos.bmHeight, 1);
            }
            break;
    }
    SetFocus(hwnd);
}

// -----------------------------------------------------------------------
// Default window hook
LRESULT CALLBACK FrmImageViewWinHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    BITMAP BitmapInfos;
    PAINTSTRUCT ViewPs;
    long hStruct = 0;
    int ObjSizeX = 0;
    int ObjSizeY = 0;
    long PagetoScroll = 0;

    switch(uMsg)
    {
        case WM_DESTROY:
            LoadViewStruct(hWnd);
            hStruct = GetWindowLong(hWnd, GWL_USERDATA);
            FreeMem(hStruct);
            SetWindowLong(hWnd, GWL_USERDATA, 0);
            return(0);
        case WM_HSCROLL:
            LoadViewStruct(hWnd);
            switch(ViewStruct.ImType)
            {
                case VIEW_ICON:
                    ImageList_GetIconSize(ViewStruct.CurrentImage, &ObjSizeX, &ObjSizeY);
                    break;
                case VIEW_CURSOR:
                    ImageList_GetIconSize(ViewStruct.CurrentImage, &ObjSizeX, &ObjSizeY);
                    break;
                case VIEW_BITMAP:
                    GetObject(ViewStruct.hBitmap, sizeof(BitmapInfos), &BitmapInfos);
                    ObjSizeX = BitmapInfos.bmWidth;
                    break;
            }
            switch(wParam & 0xFFFF)
            {
                case SB_LINELEFT:
                    if(GetScrollPos(hWnd, SB_HORZ) > 0) ScrollPicHorz(hWnd, 1, 1);
                    break;
                case SB_LINERIGHT:
                    if(GetScrollPos(hWnd, SB_HORZ) < (long) (ObjSizeX - ControlClientWidth(hWnd))) ScrollPicHorz(hWnd, -1, 1);
                    break;
                case SB_PAGELEFT:
                    PagetoScroll = ControlClientWidth(hWnd);
                    if(GetScrollPos(hWnd, SB_HORZ) - PagetoScroll < 0) PagetoScroll = GetScrollPos(hWnd, SB_HORZ);
                    ScrollPicHorz(hWnd, PagetoScroll, 1);
                    break;
                case SB_PAGERIGHT:
                    PagetoScroll = ControlClientWidth(hWnd);
                    if(GetScrollPos(hWnd, SB_HORZ) + PagetoScroll > ScrollBarGetMaxRange(hWnd, SB_HORZ)) PagetoScroll = (ObjSizeX - ControlClientWidth(hWnd)) - GetScrollPos(hWnd, SB_HORZ);
                    ScrollPicHorz(hWnd, -(long) PagetoScroll, 1);
                    break;
                case SB_THUMBTRACK:
                    ScrollPicHorz(hWnd, (short) ((wParam & 0xFFFF0000) / 0x10000), 0);
                    break;
                case SB_THUMBPOSITION:
                    ScrollPicHorz(hWnd, (short) ((wParam & 0xFFFF0000) / 0x10000), 0);
                    break;
            }
            return(0);
        case WM_VSCROLL:
            LoadViewStruct(hWnd);
            switch(ViewStruct.ImType)
            {
                case VIEW_ICON:
                    ImageList_GetIconSize(ViewStruct.CurrentImage, &ObjSizeX, &ObjSizeY);
                    break;
                case VIEW_CURSOR:
                    ImageList_GetIconSize(ViewStruct.CurrentImage, &ObjSizeX, &ObjSizeY);
                    break;
                case VIEW_BITMAP:
                    GetObject(ViewStruct.hBitmap, sizeof(BitmapInfos), &BitmapInfos);
                    ObjSizeY = BitmapInfos.bmHeight;
                    break;
            }
            switch(wParam & 0xFFFF)
            {
                case SB_LINEUP:
                    if(GetScrollPos(hWnd, SB_VERT) > 0) ScrollPicVert(hWnd, 1, 1);
                    break;
                case SB_LINEDOWN:
                    if(GetScrollPos(hWnd, SB_VERT) < (long) (ObjSizeY - ControlClientHeight(hWnd))) ScrollPicVert(hWnd, -1, 1);
                    break;
                case SB_PAGEUP:
                    PagetoScroll = ControlClientHeight(hWnd);
                    if(GetScrollPos(hWnd, SB_VERT) - PagetoScroll < 0) PagetoScroll = GetScrollPos(hWnd, SB_VERT);
                    ScrollPicVert(hWnd, PagetoScroll, 1);
                    break;
                case SB_PAGEDOWN:
                    PagetoScroll = ControlClientHeight(hWnd);
                    if(GetScrollPos(hWnd, SB_VERT) + PagetoScroll > ScrollBarGetMaxRange(hWnd, SB_VERT)) PagetoScroll = (ObjSizeY - ControlClientHeight(hWnd)) - GetScrollPos(hWnd, SB_VERT);
                    ScrollPicVert(hWnd, -(long) PagetoScroll, 1);
                    break;
                case SB_THUMBTRACK:
                    ScrollPicVert(hWnd, (short) ((wParam & 0xFFFF0000) / 0x10000), 0);
                    break;
                case SB_THUMBPOSITION:
                    ScrollPicVert(hWnd, (short) ((wParam & 0xFFFF0000) / 0x10000), 0);
                    break;
            }
            return(0);
        case WM_PAINT:
            BeginPaint(hWnd, &ViewPs);
            LoadViewStruct(hWnd);
            switch(ViewStruct.ImType)
            {
                case VIEW_ICON:
                    DrawIconEx(ViewPs.hdc, 0, 0, (HICON) ViewStruct.hBitmap, 0, 0, 0, 0, DI_NORMAL);
                    break;
                case VIEW_CURSOR:
                    DrawIconEx(ViewPs.hdc, 0, 0, (HICON) ViewStruct.hBitmap, 0, 0, 0, 0, DI_NORMAL);
                    break;
            }
            EndPaint(hWnd, &ViewPs);
            return(0);
        case WM_SIZE:
            ResizeImgView(hWnd);
            break;
    }
    return(DefWindowProc(hWnd, uMsg, wParam, lParam));
}

// -----------------------------------------------------------------------
// Load the current view back structure
void LoadViewStruct(HWND hWnd)
{
    if(hWnd != 0)
    {
        RtlZeroMemory(&ViewStruct, sizeof(ViewStruct));
        RtlCopyMemory(&ViewStruct, (void *) GetWindowLong(hWnd, GWL_USERDATA), sizeof(ViewStruct));
    }
}

// -----------------------------------------------------------------------
// Store the current view back structure
void StoreViewStruct(HWND hWnd)
{
    if(hWnd != 0) RtlCopyMemory((void *) GetWindowLong(hWnd, GWL_USERDATA), &ViewStruct, sizeof(ViewStruct));
}

// -----------------------------------------------------------------------
// Scroll the pic horizontally
void ScrollPicHorz(HWND hWnd, long Amount, long Absolute)
{
    RECT ScrollRect;
    
    LoadViewStruct(hWnd);
    GetClientRect(hWnd, &ScrollRect);
    switch(Absolute)
    {
        case 0:
            ScrollWindow(hWnd, ViewStruct.OldX - Amount, 0, NULL, &ScrollRect);
            ViewStruct.OldX = Amount;
            StoreViewStruct(hWnd);
            SetScrollPos(hWnd, SB_HORZ, Amount, 1);
            break;
        case 1:
            ScrollWindow(hWnd, Amount, 0, NULL, &ScrollRect);
            ViewStruct.OldX = ViewStruct.OldX - Amount;
            StoreViewStruct(hWnd);
            SetScrollPos(hWnd, SB_HORZ, ViewStruct.OldX, 1);
            break;
    }
}

// -----------------------------------------------------------------------
// Scroll the pic vertically
void ScrollPicVert(HWND hWnd, long Amount, long Absolute)
{
    RECT ScrollRect;

    LoadViewStruct(hWnd);
    GetClientRect(hWnd, &ScrollRect);
    switch(Absolute)
    {
        case 0:
            ScrollWindow(hWnd, 0, ViewStruct.OldY - Amount, NULL, &ScrollRect);
            ViewStruct.OldY = Amount;
            StoreViewStruct(hWnd);
            SetScrollPos(hWnd, SB_VERT, Amount, 1);
            break;
        case 1:
            ScrollWindow(hWnd, 0, Amount, NULL, &ScrollRect);
            ViewStruct.OldY = ViewStruct.OldY - Amount;
            StoreViewStruct(hWnd);
            SetScrollPos(hWnd, SB_VERT, ViewStruct.OldY, 1);
            break;
    }
}

// -----------------------------------------------------------------------
// Resize the form
void ResizeImgView(HWND hWnd)
{
    BITMAP BitmapInfos;
    int ObjSizeX = 0;
    int ObjSizeY = 0;

    LoadViewStruct(hWnd);
    switch(ViewStruct.ImType)
    {
        case VIEW_ICON:
            ImageList_GetIconSize(ViewStruct.CurrentImage, &ObjSizeX, &ObjSizeY);
            break;
        case VIEW_CURSOR:
            ImageList_GetIconSize(ViewStruct.CurrentImage, &ObjSizeX, &ObjSizeY);
            break;
        case VIEW_BITMAP:
            GetObject(ViewStruct.hBitmap, sizeof(BitmapInfos), &BitmapInfos);
            ObjSizeX = BitmapInfos.bmWidth;
            ObjSizeY = BitmapInfos.bmHeight;
            break;
    }
    ScrollBarShowHide(hWnd, ObjSizeX, ObjSizeY, 1);
}
