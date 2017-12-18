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
// ImageLists.cpp: Handle the image lists
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "ConstRes.h"

// -----------------------------------------------------------------------
// Variables
HIMAGELIST GlobalImageList1;
HIMAGELIST GlobalImageList2;
HIMAGELIST GlobalImageList3;
HIMAGELIST GlobalImageList4;
HIMAGELIST GlobalImageList5;

// -----------------------------------------------------------------------
// Create main imagelist
void InitImageList1(void)
{
    GlobalImageList3 = ImageList_Create(1, 18, ILC_MASK + ILC_COLOR16, 0, 0);
    if(MiscGetOSClass() & 8)
    {
        // Needs exact size
        GlobalImageList1 = ImageList_Create(16, 16, ILC_MASK + ILC_COLOR16, ICON_NBR, 0);
    }
    else
    {
        GlobalImageList1 = ImageList_Create(17, 17, ILC_MASK + ILC_COLOR16, ICON_NBR, 0);
    }
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ASKDIR);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ASM);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ASSEMBLE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CASCADE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CLOSE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CLOSEWIN);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_COLORS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_COMMENT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_COMPONENT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CONSOLE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_COPY);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CUBES);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CUT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DEBUG);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DELETE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DELSTATUS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DIRPERMS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DIRUP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DIRVIEW);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DOWNLOAD);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_EARTH);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_EXEC);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FILEASSOC);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FILES);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FIND);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FINDNEXT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FINDPREV);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FTPUSER);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_GETREG);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_HELPFILES);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_INDENT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_INSERT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_INTERNET);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_MAIL);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_MAINICON);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_KEYBOARD);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_MENUED);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_MSDOS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_NEW);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_NEWDIR);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_NEWFTP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_OPEN);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_OUTDENT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PASTE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PAUSE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STACK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PLUG);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PRINT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PROPERTIES);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REDO);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REFRESH);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_RENAMEDIR);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REPLACE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_RES);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_RUNPROG);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_SAVE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_SAVELITTLE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_SETBOOK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_SNAP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_SPLIT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STATIC);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STEP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STOP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STOREREG);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_TILEHORZ);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_TILEVERT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_TOOLBARS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_TOWIN);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_UNCOMMENT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_UNDO);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_UPLOAD);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_USER);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_WINDOWS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_YIN);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_LAB);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_WINDOW);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_KEY);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_KEYREPLACE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_SERV);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_POPUP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ITEM);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_SEPARATOR);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_UP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DOWN);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DIRECTX);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CUSTTOOLBARS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDINR);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDINS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PANEL);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_EDITDOC);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STAR);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_RELOAD);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STEPBACK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_WINDOWNORM);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_WINDOWMODIF);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FULLSCREEN);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_RUNPROJECT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DEBUGPROJECT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_BUILDPROJECT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDACTIVE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDALLACTIVE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDDIRECTORY);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PROCESS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PAUSELITTLE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STOPLITTLE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_THREAD);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_NEXTWINDOW);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PREVWINDOW);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FIRSTBOOK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PREVBOOK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_NEXTBOOK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_LASTBOOK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CLEARBOOK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDFILES);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_WINDOWLOCK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_DIRSEARCH);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDLIBRARY);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDRESOURCE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_MENUDROPDOWN);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_LIGHT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_HEARTOFGOLD);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_HEARTOFGOLDPLUS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDFILTER);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_TOWINDOW);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_SETBREAK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_CLEARBREAK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_NEXTBREAK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_PREVBREAK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_FIRSTBREAK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_LASTBREAK);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STEPINTO);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STEPOVER);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_STEPOUT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDCONST);
    ImageListAddIcon(GlobalImageList1, ICON_RES_BASE + ICON_RES_STATUSBAR);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_LOWERCASE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_UPPERCASE);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REPLACECONST);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_AUDIO);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_OBJECT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_ADDOBJECT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_OPENGL);

    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGEAX);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGEBX);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGECX);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGEDX);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGESI);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGEDI);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGEIP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGESP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGEBP);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGEFL);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGCS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGDS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGES);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGFS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGGS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGSS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGST0);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGST1);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGST2);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGST3);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGST4);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGST5);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGST6);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGST7);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGFPUCTRL);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGFPUSTAT);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGFPUTAGS);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGFPUEDO);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMM0);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMM1);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMM2);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMM3);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMM4);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMM5);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMM6);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMM7);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGXMM0);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGXMM1);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGXMM2);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGXMM3);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGXMM4);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGXMM5);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGXMM6);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGXMM7);
    ImageListAddIcon(GlobalImageList1, ICON_BASE + ICON_REGMXCSR);
}

// -----------------------------------------------------------------------
// Create little imagelist
void InitImageList2(void)
{
    // Init the second imagelist
    if(MiscGetOSClass() & 8)
    {
        GlobalImageList2 = ImageList_Create(10, 9, ILC_MASK + ILC_COLOR16, ICON_NBR, 0);
    }
    else
    {
        GlobalImageList2 = ImageList_Create(10, 9, ILC_MASK + ILC_COLOR16, ICON_NBR, 0);
    }
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_ASSEMBLE);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_SAVELITTLE);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_EXEC);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_DEBUG);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_CLOSEWIN);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_TOWIN);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_DELSTATUS);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_RELOAD);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_STEPBACK);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_PAUSELITTLE);
    ImageListAddIcon(GlobalImageList2, ICON_BASE + ICON_STOPLITTLE);
}

// -----------------------------------------------------------------------
// Create dialog editor controls imagelist
void InitImageList4(void)
{
    if(MiscGetOSClass() & 8)
    {
        GlobalImageList4 = ImageList_Create(24, 24, ILC_MASK + ILC_COLOR16, ICON_NBR, 0);
    }
    else
    {
        GlobalImageList4 = ImageList_Create(23, 23, ILC_MASK + ILC_COLOR16, ICON_NBR, 0);
    }
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_ANIMATION);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_BUTTON);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_CALENDAR);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_CHECKBOX);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_COMBOBOX);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_CURSOR);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_DATEPICKER);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_FRAME);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_HOTKEY);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_HSCROLLBAR);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_LABEL);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_LISTBOX);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_LISTVIEW);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_PICTURE);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_PROGRESSBAR);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_RADIOBUTTON);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_SLIDER);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_SPIN);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_SYSTAB);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_TEXT);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_TEXTIP);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_TREEVIEW);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_VSCROLLBAR);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_XCOMBOBOX);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_VERTSLIDER);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_TOOLBAR);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_STATUSBAR);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_RICHTEXT);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_PAGER);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_HEADER);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_REBAR);
    ImageListAddIcon(GlobalImageList4, ICON_RES_BASE + ICON_RES_TOOLTIP);
}

// -----------------------------------------------------------------------
// Create dialog editor tools imagelist
void InitImageList5(void)
{
    if(MiscGetOSClass() & 8)
    {
        GlobalImageList5 = ImageList_Create(16, 16, ILC_MASK + ILC_COLOR16, ICON_NBR, 0);
    }
    else
    {
        GlobalImageList5 = ImageList_Create(17, 17, ILC_MASK + ILC_COLOR16, ICON_NBR, 0);
    }
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_LOCKCTRL);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_ALIGNBOTTOM);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_ALIGNLEFT);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_ALIGNRIGHT);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_ALIGNTOP);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_CENTERHORZ);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_CENTERVERT);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_GRID);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_SAMEHEIGHT);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_SAMESIZE);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_SAMEWIDTH);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_SPACEACROSS);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_SPACEDOWN);
    ImageListAddIcon(GlobalImageList5, ICON_RES_TOOL_BASE + ICON_RES_TOOL_ALIGNGRID);
    ImageListAddIcon(GlobalImageList5, ICON_BASE + ICON_MAINICON);
    ImageListAddIcon(GlobalImageList5, ICON_BASE + ICON_TOOLBARS);
    ImageListAddIcon(GlobalImageList5, ICON_BASE + ICON_PROPERTIES);
}
