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
// IniFiles.cpp: Read ini files
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "MiscFunctions.h"
#include "APIDb.h"
#include "IniFiles.h"

// -----------------------------------------------------------------------
// Variables
CStr InitRetVal;

// -----------------------------------------------------------------------
// Read the Main.ini file
long ReadIniFile(void)
{
    int i = 0;
    long ReturnValue = 0;
    CStr TmpSysDir;
    CStr TmpExt;
    CStr BufString;
    
    RegisteredExts.Erase();
    // Read registered extensions
    for(i = 0; i <= 999; i++)
    {
        TmpExt = "Ext" + (CStr) StringNumberComplement(i, 3).Get_String();
        InitRetVal = IniReadKey("Extensions", TmpExt, MainIniFile);
        if(InitRetVal.Len() == 0) break;
        RegisteredExts.Add(InitRetVal.Get_String());
    }
    
    // Read working dir
    NotifiedCurrentdir = "";
    CurrentDir = AppPath;
    InitRetVal = IniReadKey("Dirs", "WORKDIR", MainIniFile);
    // Should be empty at first install: take application directory
    if(InitRetVal.Len() == 0) InitRetVal = AppPath;
    if(InitRetVal.Len() != 0)
    {
        CurrentDir = InitRetVal;
        ChDrive(CurrentDir);
        if(ChDir(CurrentDir) == 0)
        {
            NotifiedCurrentdir = CurrentDir;
            ReturnValue = 1;
            CurrentDir = "c:\\";
            IniWriteKey("Dirs", "WORKDIR", CurrentDir, MainIniFile);
            ChDrive(CurrentDir);
            ChDir(CurrentDir);
        }
    } 

    // Fill others directories + directories names replacements
    DirsRepl[DIR_ROOT] = "ROOTDIR";
    Dirs[DIR_ROOT] = _strdup(IniReadKey("Dirs", DirsRepl[DIR_ROOT], MainIniFile).Get_String());
    DirsRepl[DIR_BIN] = "BINDIR";
    Dirs[DIR_BIN] = _strdup(IniReadKey("Dirs", DirsRepl[DIR_BIN], MainIniFile).Get_String());
    DirsRepl[DIR_INCLUDE] = "INCLUDEDIR";
    Dirs[DIR_INCLUDE] = _strdup(IniReadKey("Dirs", DirsRepl[DIR_INCLUDE], MainIniFile).Get_String());
    DirsRepl[DIR_LIB] = "LIBDIR";
    Dirs[DIR_LIB] = _strdup(IniReadKey("Dirs", DirsRepl[DIR_LIB], MainIniFile).Get_String());
    DirsRepl[DIR_HELP] = "HELPDIR";
    Dirs[DIR_HELP] = _strdup(IniReadKey("Dirs", DirsRepl[DIR_HELP], MainIniFile).Get_String());
    DirsRepl[DIR_PROJECTS] = "PROJECTSDIR";
    Dirs[DIR_PROJECTS] = _strdup(IniReadKey("Dirs", DirsRepl[DIR_PROJECTS], MainIniFile).Get_String());
    // Perform a sanity check on the retrieved directory
    if(strlen(Dirs[DIR_PROJECTS]) == 0 || FileIsDirectory(Dirs[DIR_PROJECTS]) == 0)
    {
        free(Dirs[DIR_PROJECTS]);
        // Set default projects directory
        BufString = AppPath + (CStr) "\\Projects";
        Dirs[DIR_PROJECTS] = _strdup(BufString.Get_String());
        // Update project dir
        // Note: at first install this directory must be empty.
        // (So it points to the correct one)
        IniWriteKey("Dirs", "PROJECTSDIR", BufString, MainIniFile);
    }
    
    // Application directory
    Dirs[DIR_MAIN] = _strdup(AppPath.Get_String());
    DirsRepl[DIR_MAIN] = "MAINDIR";
    IniWriteKey("Dirs", "MAINDIR", Dirs[DIR_MAIN], MainIniFile);
    
    BufString = AppPath + (CStr) "\\Tools";
    Dirs[DIR_TOOLS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_TOOLS] = "TOOLSDIR";
    IniWriteKey("Dirs", "TOOLSDIR", Dirs[DIR_TOOLS], MainIniFile);

    BufString = AppPath + (CStr) "\\Templates";
    Dirs[DIR_TEMPLATES] = _strdup(BufString.Get_String());
    DirsRepl[DIR_TEMPLATES] = "TEMPLATESDIR";
    IniWriteKey("Dirs", "TEMPLATESDIR", Dirs[DIR_TEMPLATES], MainIniFile);

    BufString = AppPath + (CStr) "\\Help";
    Dirs[DIR_MAINHELP] = _strdup(BufString.Get_String());
    DirsRepl[DIR_MAINHELP] = "MAINHELPDIR";
    IniWriteKey("Dirs", "MAINHELPDIR", Dirs[DIR_MAINHELP], MainIniFile);

    BufString = AppPath + (CStr) "\\Snippets";
    Dirs[DIR_SNIPPETS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_SNIPPETS] = "SNIPPETSDIR";
    IniWriteKey("Dirs", "SNIPPETSDIR", Dirs[DIR_SNIPPETS], MainIniFile);

    BufString = AppPath + (CStr) "\\Databases";
    Dirs[DIR_DATABASES] = _strdup(BufString.Get_String());
    DirsRepl[DIR_DATABASES] = "DATABASESDIR";
    IniWriteKey("Dirs", "DATABASESDIR", Dirs[DIR_DATABASES], MainIniFile);

    BufString = AppPath + (CStr) "\\Skins";
    Dirs[DIR_SKINS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_SKINS] = "SKINSDIR";
    IniWriteKey("Dirs", "SKINSDIR", Dirs[DIR_SKINS], MainIniFile);

    BufString = AppPath + (CStr) "\\Languages";
    Dirs[DIR_LANGUAGES] = _strdup(BufString.Get_String());
    DirsRepl[DIR_LANGUAGES] = "LANGUAGESDIR";
    IniWriteKey("Dirs", "LANGUAGESDIR", Dirs[DIR_LANGUAGES], MainIniFile);
    
    BufString = AppPath + (CStr) "\\Languages\\Scripts";
    Dirs[DIR_LANGSCRIPTS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_LANGSCRIPTS] = "LANGSCRIPTSDIR";
    IniWriteKey("Dirs", "LANGSCRIPTSDIR", Dirs[DIR_LANGSCRIPTS], MainIniFile);

    BufString = AppPath + (CStr) "\\Downloads";
    Dirs[DIR_DOWNLOADS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_DOWNLOADS] = "DOWNLOADSDIR";
    IniWriteKey("Dirs", "DOWNLOADSDIR", Dirs[DIR_DOWNLOADS], MainIniFile);
    
    BufString = AppPath + (CStr) "\\AddIns";
    Dirs[DIR_ADDINS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_ADDINS] = "ADDINSDIR";
    IniWriteKey("Dirs", "ADDINSDIR", Dirs[DIR_ADDINS], MainIniFile);
    
    BufString = AppPath + (CStr) "\\Config";
    Dirs[DIR_CONFIG] = _strdup(BufString.Get_String());
    DirsRepl[DIR_CONFIG] = "CONFIGDIR";
    IniWriteKey("Dirs", "CONFIGDIR", Dirs[DIR_CONFIG], MainIniFile);
    
    BufString = Dirs[DIR_CONFIG] + (CStr) "\\UserMenus";
    Dirs[DIR_MENUS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_MENUS] = "MENUSDIR";
    IniWriteKey("Dirs", "MENUSDIR", Dirs[DIR_MENUS], MainIniFile);

    BufString = AppPath + (CStr) "\\Wizards";
    Dirs[DIR_WIZARDS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_WIZARDS] = "WIZARDSDIR";
    IniWriteKey("Dirs", "WIZARDSDIR", Dirs[DIR_WIZARDS], MainIniFile);

    BufString = AppPath + (CStr) "\\Scripts";
    Dirs[DIR_SCRIPTS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_SCRIPTS] = "SCRIPTSDIR";
    IniWriteKey("Dirs", "SCRIPTSDIR", Dirs[DIR_SCRIPTS], MainIniFile);

    BufString = AppPath + (CStr) "\\Filters";
    Dirs[DIR_FILTERS] = _strdup(BufString.Get_String());
    DirsRepl[DIR_FILTERS] = "FILTERSDIR";
    IniWriteKey("Dirs", "FILTERSDIR", Dirs[DIR_FILTERS], MainIniFile);

    BufString = Dirs[DIR_SKINS] + (CStr) "\\ExtraCode";
    Dirs[DIR_EXTRACODE] = _strdup(BufString.Get_String());
    DirsRepl[DIR_EXTRACODE] = "EXTRACODEDIR";
    IniWriteKey("Dirs", "EXTRACODEDIR", Dirs[DIR_EXTRACODE], MainIniFile);

    /*BufString = Dirs[DIR_TOOLS] + (CStr) "\\Tools";
    Dirs[DIR_TOOLS] = strdup(BufString.Get_String());
    DirsRepl[DIR_TOOLS] = "TOOLSDIR";
    IniWriteKey("Dirs", "TOOLSDIR", Dirs[DIR_TOOLS], MainIniFile);*/

    // Handle databases
    APIFile = Dirs[DIR_DATABASES] + (CStr) "\\WinDetails.db";
    InitRetVal = IniReadKey("Databases", "WinDetail", MainIniFile);
    if(InitRetVal.Len() != 0) APIFile = InitRetVal;
    IniWriteKey("Databases", "WinDetail", APIFile, MainIniFile);
    APIFile = ChangeRelativePaths(APIFile);

    APIFncFile = Dirs[DIR_DATABASES] + (CStr) "\\WinFnc.db";
    InitRetVal = IniReadKey("Databases", "WinFnc", MainIniFile);
    if(InitRetVal.Len() != 0) APIFncFile = InitRetVal;
    IniWriteKey("Databases", "WinFnc", APIFncFile, MainIniFile);
    APIFncFile = ChangeRelativePaths(APIFncFile);
    
    APIConstFile = Dirs[DIR_DATABASES] + (CStr) "\\WinConst.db";
    InitRetVal = IniReadKey("Databases", "WinConst", MainIniFile);
    if(InitRetVal.Len() != 0) APIConstFile = InitRetVal;
    IniWriteKey("Databases", "WinConst", APIConstFile, MainIniFile);
    APIConstFile = ChangeRelativePaths(APIConstFile);

    // Set default value
    TipFileName = Dirs[DIR_MAIN] + (CStr) "\\Tips.txt";
    // Or take the one found ini main.ini
    InitRetVal = IniReadKey("Layout", "TipsFile", MainIniFile);
    if(InitRetVal.Len() != 0) TipFileName = InitRetVal;
    TipFileName = ChangeRelativePaths(TipFileName);
    
    // Addins directory by default
    LastAddInDir = Dirs[DIR_PROJECTS];
    
    // Retrieve windows directory
    TmpSysDir = TmpSysDir.String(MAX_PATH, 1);
    GetWindowsDirectory(TmpSysDir.Get_String(), MAX_PATH);
    DirsRepl[DIR_WIN] = "WINDIR";
    Dirs[DIR_WIN] = "";
    if(TmpSysDir.Len() != 0)
    {
        Dirs[DIR_WIN] = _strdup(TmpSysDir.Get_String());
    }
    
    // Retrieve system directory
    TmpSysDir = TmpSysDir.String(MAX_PATH, 1);
    GetSystemDirectory(TmpSysDir.Get_String(), MAX_PATH);
    DirsRepl[DIR_WINSYS] = "WINSYSDIR";
    Dirs[DIR_WINSYS] = "";
    if(TmpSysDir.Len() != 0)
    {
        Dirs[DIR_WINSYS] = _strdup(TmpSysDir.Get_String());
    }
    
    // Get temporary directory
    TmpSysDir = TmpSysDir.String(MAX_PATH, 1);
    GetTempPath(MAX_PATH, TmpSysDir.Get_String());
    DirsRepl[DIR_TEMP] = "TEMPDIR";
    Dirs[DIR_TEMP] = "";
    if(TmpSysDir.Len() != 0)
    {
        if(strcmp(TmpSysDir.Right(1).Get_String(), "\\") == 0) Dirs[DIR_TEMP] = _strdup(TmpSysDir.Left(TmpSysDir.Len() - 1).Get_String());
        else Dirs[DIR_TEMP] = _strdup(TmpSysDir.Get_String());
        Dirs[DIR_TEMP] = _strdup(TmpSysDir.Get_String());
    }
    
    // Read various layouts
    ShowStatusbar = 1;
    InitRetVal = IniReadKey("Layout", "StatusBar", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "0") == 0) ShowStatusbar = 0;
    
    ShowProjetDockWin = 1;
    InitRetVal = IniReadKey("Layout", "DockProjectVisible", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "0") == 0) ShowProjetDockWin = 0;

    ShowWindowsDockWin = 1;
    InitRetVal = IniReadKey("Layout", "DockWindowsVisible", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "0") == 0) ShowWindowsDockWin = 0;

    ShowOutputDockWin = 1;
    InitRetVal = IniReadKey("Layout", "DockOutputVisible", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "0") == 0) ShowOutputDockWin = 0;

    ShowDebugDockWin = 1;
    InitRetVal = IniReadKey("Layout", "DockDebugVisible", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "0") == 0) ShowDebugDockWin = 0;
    
    ShowRegistersDockWin = 1;
    InitRetVal = IniReadKey("Layout", "DockRegistersVisible", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "0") == 0) ShowRegistersDockWin = 0;
    
    LeftM = 0;
    InitRetVal = IniReadKey("Layout", "Margin", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) LeftM = 1;
    White = 0;
    InitRetVal = IniReadKey("Layout", "WhiteSpace", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) White = 1;
    ShowH = 0;
    InitRetVal = IniReadKey("Layout", "Highlight", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) ShowH = 1;
    SyntaxH = 0;
    InitRetVal = IniReadKey("Layout", "Syntax", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) SyntaxH = 1;
    OutputDates = 0;
    InitRetVal = IniReadKey("Layout", "OutputDates", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) OutputDates = 1;
    IndentScope = 0;
    InitRetVal = IniReadKey("Layout", "IndentScope", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) IndentScope = 1;
    EnDragDrop = 1;
    InitRetVal = IniReadKey("Layout", "EnDragDrop", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "0") == 0) EnDragDrop = 0;
    Remember = 0;
    InitRetVal = IniReadKey("Layout", "Remember", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) Remember = 1;
    LineTTip = 0;
    InitRetVal = IniReadKey("Layout", "LineToolTip", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) LineTTip = 1;
    APITTip = 0;
    InitRetVal = IniReadKey("Layout", "APIToolTip", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) APITTip = 1;
    Smooth = 0;
    InitRetVal = IniReadKey("Layout", "SmoothScrolling", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) Smooth = 1;
    CursorFollow = 0;
    InitRetVal = IniReadKey("Layout", "FollowScrolling", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) CursorFollow = 1;
    AcceptFiles = 0;
    InitRetVal = IniReadKey("Layout", "AcceptFiles", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) AcceptFiles = 1;
    ItalicFont = 0;
    InitRetVal = IniReadKey("Layout", "CommentItalic", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) ItalicFont = 1;
    Wrap = 0;
    InitRetVal = IniReadKey("Layout", "WordWrap", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) Wrap = 1;
    AutoSaveDelay = 0;
    InitRetVal = IniReadKey("Layout", "AutoSaveDelay", MainIniFile);
    if(InitRetVal.Len() != 0) AutoSaveDelay = InitRetVal.Get_Long();
    VerboseSave = 0;
    InitRetVal = IniReadKey("Layout", "VerboseSave", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) VerboseSave = 1;
    AutoSave = 0;
    InitRetVal = IniReadKey("Layout", "AutoSave", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) AutoSave = 1;
    AutoExpand = 0;
    InitRetVal = IniReadKey("Layout", "ExpandTabs", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) AutoExpand = 1;
    CreateBak = 0;
    InitRetVal = IniReadKey("Layout", "CreateBak", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) CreateBak = 1;
    DecorateBak = 0;
    InitRetVal = IniReadKey("Layout", "DecorateBak", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) DecorateBak = 1;
    HorizSplit = 0;
    InitRetVal = IniReadKey("Layout", "HorizSplit", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) HorizSplit = 1;
    DetectChange = 0;
    InitRetVal = IniReadKey("Layout", "DetectChange", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) DetectChange = 1;
    AutoCorrectKey = 0;
    InitRetVal = IniReadKey("Layout", "AutoCorrect", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) AutoCorrectKey = 1;
    MaxRecent = 10;
    InitRetVal = IniReadKey("Layout", "Recent", MainIniFile);
    if(InitRetVal.Len() != 0) MaxRecent = InitRetVal.Get_Long();
    MaxRecentPrj = 5;
    InitRetVal = IniReadKey("Layout", "RecentPrj", MainIniFile);
    if(InitRetVal.Len() != 0) MaxRecentPrj = InitRetVal.Get_Long();
    LnStyle = 0;
    InitRetVal = IniReadKey("Layout", "LinesStyle", MainIniFile);
    if(InitRetVal.Len() != 0) LnStyle = InitRetVal.Get_Long();
    LnStart = 0;
    InitRetVal = IniReadKey("Layout", "LinesStart", MainIniFile);
    if(InitRetVal.Len() != 0) LnStart = InitRetVal.Get_Long();
    AutoMaximizeChilds = 0;
    InitRetVal = IniReadKey("Layout", "AutoMax", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) AutoMaximizeChilds = 1;
    AutoClearOut = 0;
    InitRetVal = IniReadKey("Layout", "AutoClear", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) AutoClearOut = 1;
    Tabulations = 8;
    InitRetVal = IniReadKey("Layout", "Tabs", MainIniFile);
    if(InitRetVal.Len() != 0) Tabulations = InitRetVal.Get_Long();
    CurFontName = IniReadKey("Layout", "FontName", MainIniFile);
    InitRetVal = IniReadKey("Layout", "FontSize", MainIniFile);
    CurFontSize = 9;
    if(CurFontName.Len() == 0) CurFontName = "Courier New";
    if(InitRetVal.Len() != 0) CurFontSize = InitRetVal.Get_Long();
    CurFontBold = 0;
    InitRetVal = IniReadKey("Layout", "FontBold", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) CurFontBold = 1;
    if(CurFontName.Len() == 0) CurFontName = "Lucida console";
    if(CurFontSize == 0) CurFontSize = 8;
    UseWinRecent = 0;
    InitRetVal = IniReadKey("Layout", "WinRecent", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) UseWinRecent = 1;
    FullScreenMode = 0;
    InitRetVal = IniReadKey("Layout", "FullScreen", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) FullScreenMode = 1;
    // Load cgrep file
    cGrepIniFile = IniReadKey("Layout", "cGrepFile", MainIniFile);
    cGrepIniFile = ChangeRelativePaths(cGrepIniFile);
    ReadColorsDefinition();
    UseDbFnc = 0;
    InitRetVal = IniReadKey("Layout", "UseDbFnc", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) UseDbFnc = 1;
    UseDbConst = 0;
    InitRetVal = IniReadKey("Layout", "UseDbConst", MainIniFile);
    if(InitRetVal.Len() != 0) if(_strcmpi(InitRetVal.Get_String(), "1") == 0) UseDbConst = 1;
    DbLinesNumbers = 6;
    InitRetVal = IniReadKey("Layout", "DbLinesNumbers", MainIniFile);
    if(InitRetVal.Len() != 0) DbLinesNumbers = InitRetVal.Get_Long();
    if(DbLinesNumbers < 1) DbLinesNumbers = 1;
    // User menus by default
    UserMenusIniFile = AppPath + "\\Config\\UserMenus\\Default.ume";
    InitRetVal = IniReadKey("Layout", "CurrentMenus", MainIniFile);
    InitRetVal = ChangeRelativePaths(InitRetVal);
    if(InitRetVal.Len() != 0) UserMenusIniFile = InitRetVal;
    if(FileExist(UserMenusIniFile) == 0) UserMenusIniFile = AppPath + "\\Config\\UserMenus\\Default.ume";
    UpdateResFiltersIni();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve stand colors from ini file
void ReadColorsDefinition(void)
{
    CurBackColor = 0x543243;
    CurNormTextColor = 0xAAAAAA;
    CurHighTextColor = 0xFFFF;
    CurMarginColor = 0x433254;
    CurLineColor = 0xFFAAAA;
    // Defaults cols
    // (they sucks i know)
    LangNumbersColor = 0xFFAAAA;
    LangStringsColor = 0xAAFFAA;
    LangCommentsColor = 0x55FFAA;
    LangKeyWordsColor = 0xAAFF55;
    LangColorAColor = 0xFF55AA;
    LangColorBColor = 0xFF55FF;
    LangColorCColor = 0xFFAA55;
    LangColorDColor = 0xAA55FF;
    LangColorEColor = 0x55AAFF;
    APIColor = 0xFFFFFF;
    LangColorFColor = 0xFFFFAA;
    LangColorGColor = 0xAAFFFF;
    LangColorHColor = 0xFFFFFF;
    InitRetVal = IniReadKey("Layout", "BackColor", MainIniFile);
    if(InitRetVal.Len() != 0) CurBackColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "NormText", MainIniFile);
    if(InitRetVal.Len() != 0) CurNormTextColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "HighText", MainIniFile);
    if(InitRetVal.Len() != 0) CurHighTextColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "MarginColor", MainIniFile);
    if(InitRetVal.Len() != 0) CurMarginColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "LinesColor", MainIniFile);
    if(InitRetVal.Len() != 0) CurLineColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "NumbersColorValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangNumbersColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "StringsColorValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangStringsColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "CommentsColorValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangCommentsColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "KeyWordsColorValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangKeyWordsColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "ColorAValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorAColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "ColorBValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorBColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "ColorCValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorCColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "ColorDValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorDColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "ColorEValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorEColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "APIHighColor", MainIniFile);
    if(InitRetVal.Len() != 0) APIColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "ColorFValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorFColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "ColorGValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorGColor = InitRetVal.Get_Hex();
    InitRetVal = IniReadKey("Layout", "ColorHValue", MainIniFile);
    if(InitRetVal.Len() != 0) LangColorHColor = InitRetVal.Get_Hex();
}

// -----------------------------------------------------------------------
// Refresh stock resources files dialogs filters if necessary
// (Weren't present in version < 0.35)
void UpdateResFiltersIni(void)
{
    InitRetVal = IniReadKey("ExtrasExt", "AddInclude", MainIniFile);
    if(InitRetVal.Len() == 0) IniWriteKey("ExtrasExt", "AddInclude", "Source files|*.inc;*.a;*.asm;*.ash;*.asi;*.s;*.a86;*.c;*.cc;*.cpp;*.i;*.h;*.mac;*.hpp;*.pp;*.pas;*.bas|All files|*.*", MainIniFile);
    InitRetVal = IniReadKey("ExtrasExt", "AddLibrary", MainIniFile);
    if(InitRetVal.Len() == 0) IniWriteKey("ExtrasExt", "AddLibrary", "Library files|*.lib;*.a;*.ppw;*.ow|All files|*.*", MainIniFile);
    InitRetVal = IniReadKey("ExtrasExt", "AddModule", MainIniFile);
    if(InitRetVal.Len() == 0) IniWriteKey("ExtrasExt", "AddModule", "Source files|*.a;*.asm;*.s;*.a86;*.c;*.cc;*.cpp;*.pas;*.pp;*.bas|All files|*.*", MainIniFile);
    InitRetVal = IniReadKey("ExtrasExt", "AddObject", MainIniFile);
    if(InitRetVal.Len() == 0) IniWriteKey("ExtrasExt", "AddObject", "Object files|*.obj;*.o|All files|*.*", MainIniFile);
    InitRetVal = IniReadKey("ExtrasExt", "AddResource", MainIniFile);
    if(InitRetVal.Len() == 0) IniWriteKey("ExtrasExt", "AddResource", "Resources files|*.rc|All files|*.*", MainIniFile);
    InitRetVal = IniReadKey("ExtrasExt", "AddText", MainIniFile);
    if(InitRetVal.Len() == 0) IniWriteKey("ExtrasExt", "AddText", "Text files|*.txt|Ini files|*.ini|Batch files|*.bat|Log files|*.log|Registry files|*.reg|All files|*.*", MainIniFile);
    InitRetVal = IniReadKey("ExtrasExt", "AddRawDatas", MainIniFile);
    if(InitRetVal.Len() == 0) IniWriteKey("ExtrasExt", "AddRawDatas", "All files|*.*", MainIniFile);
}
