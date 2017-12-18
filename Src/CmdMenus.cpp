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
// CmdMenus.cpp: Menu commands
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "MDI_Form.h"
#include "MDI_Childs.h"
#include "MiscFunctions.h"
#include "CmdMenus.h"
#include "AddIns.h"
#include "Splitters.h"
#include "Main.h"
#include "ProjectsBuilder.h"
#include "Menus.h"
#include "ConstRes.h"
#include "ConstCodeMax.h"
#include "Languages.h"
#include "Statusbar.h"
#include "Debugger.h"
#include "Toolbars.h"
#include "Dlg_Filters.h"
#include "Dlg_Properties.h"
#include "Dlg_FTP.h"
#include "Dlg_EnterValue.h"
#include "Dlg_Favorites.h"
#include "Dlg_ProcVarSearch.h"
#include "Dlg_Results.h"
#include "Dlg_SearchInFiles.h"
#include "Dlg_Registers.h"
#include "Dlg_UserMenusEd.h"
#include "Dlg_Toolbars.h"
#include "Dlg_CreateProject.h"
#include "Dlg_PrjProps.h"
#include "Dlg_AddIns.h"
#include "Dlg_About.h"

// -----------------------------------------------------------------------
// Variables
CStr MenusRetVal;
CStr SelToSave;
int Filter_Include;
int Filter_Cancel_Process;
int First_Filter_Include;
CStr Last_Include_File;

// -----------------------------------------------------------------------
// Functions
int Flush_Filter(char *FileName, LPFILTERFILE File_Struct);

// -----------------------------------------------------------------------
// File/New
HWND MCMD_New(void)
{
    CStr OpenedLanguage;

    OpenedLanguage = IniReadKey("RefLanguages", "DefLang", LanguagesIniFile);
    if(OpenedLanguage.Len() == 0) OpenedLanguage = "X86 Assembler";
    StoreLanguageToOpen(OpenedLanguage);
    return(CreateNewFile((CStr) "<Untitled document " + (CStr) (NbForms + 1) + (CStr) ">"));
}

// -----------------------------------------------------------------------
// File/Clear
void MCMD_Clear(void)
{
    CStr BufString;

    if(IsChildReadOnly(CurrentForm) == 1) return;
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(ChildStruct->ModifFlag == 1)
    {
        BufString = "File '" + (CStr) ChildStruct->RFile->Left(ChildStruct->RFile->Len()).Get_String() + (CStr) "' is not saved. Save it now ?";
        switch(MiscMsgBox(hMDIform.hWnd, BufString, MB_QUESTIONCANCEL, Requesters))
        {
            case IDYES:
                SaveIt(CurrentForm);
                break;
            case IDNO:
                break;
            case IDCANCEL:
                return;
        }
    }
    ClearFile(CurrentForm);
    RefreshChildTitle(CurrentForm);
}

// -----------------------------------------------------------------------
// File/Duplicate
void MCMD_Duplicate(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(ChildStruct->FileLoaded == 1) OpenFileNorm(CMGetRealFile(ChildStruct->RFile), 0, 1, 0, 1);
}

// -----------------------------------------------------------------------
// File/Reload file
void MCMD_Reload(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(ChildStruct->FileLoaded == 1)
    {
        ClearFile(CurrentForm);
        LoadFile(CurrentForm);
    }
}

// -----------------------------------------------------------------------
// File/Open file
void MCMD_OpenFile(void)
{
    CStr OpFilters;
    CStr LdFile;
    int i = 0;

    OpFilters = "";
    for(i = 0; i < RegisteredExts.Amount(); i++)
    {
        OpFilters = OpFilters + (CStr) RegisteredExts.Get(i)->Content + (CStr) "|";
    }
    if(OpFilters.Len() == 0) OpFilters = "All files (*.*)|*.*";
    if(GetUseFileDir() == 1)
    {
        if(NbForms != 0)
        {
            ChildStruct = LoadStructure(CurrentForm);
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, FileGetDirectory(ChildStruct->RFile), 1, CurrentDir);
        }
        else
        {
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastLoadDir, 1, CurrentDir);
        }
    }
    else 
    {    
        LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastLoadDir, 1, CurrentDir);
    }
    LastLoadDir = ComDlgParseMultiFilesSelection(LdFile, &EnumOpenFiles, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumOpenFiles(char *FileToAdd, long UserValue)
{
    OpenFileNorm(FileToAdd, 0, 0, 0, 0);
    return(1);
}

// -----------------------------------------------------------------------
// File/Open filters laboratory
void MCMD_OpenFileAsDB(void)
{
    CreateModalDialog(-1, -1, 546, 268, hMDIform.hWnd, &FRMFiltersProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    // Cancel hit
    if(Filter_Cancel) return;
    OpenFileAsDB();
}

void OpenFileAsDB(void)
{
    CStr LdFile;

    // Now let user choose the files
    if(GetUseFileDir() == 1)
    {
        if(NbForms != 0)
        {
            ChildStruct = LoadStructure(CurrentForm);
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, "All files (*.*)|*.*", FileGetDirectory(ChildStruct->RFile), 1, CurrentDir);
        }
        else
        {
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, "All files (*.*)|*.*", LastLoadAsDBDir, 1, CurrentDir);
        }
    }
    else
    {
        LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, "All files (*.*)|*.*", LastLoadAsDBDir, 1, CurrentDir);
    }
    Filter_Include = FILTER_OPEN_MODE;
    Filter_Cancel_Process = 0;
    First_Filter_Include = 0;
    Last_Include_File = "";
    if(LdFile.Len() != 0)
    {
        ComDlgParseMultiFilesSelection(LdFile, &EnumOpenAsDBFiles, MULTIFILESENUM_FORWARD, 0);
    }
    // Clear the list of chosen filters now
    Chosen_Filters.Erase();
}

long CALLBACK EnumOpenAsDBFiles(char *FileToAdd, long UserValue)
{
    FILTERFILE File_Struct;
    FILTERFILE File_Struct_Bak;
    LPFILTERFILE Return_File_Struct;
    HMODULE FilterLib = 0;
    FARPROC FilterDesc = 0;
    FARPROC FilterAuth = 0;
    LPFILTERFILE (__stdcall *FilterProc)(char *FileName, LPFILTERFILE File, LPCHROMATICLIB WALIBStruct);
    int i;
    int Flushed = 0;
    CStr Current_Filter_Name;

    WriteToStatus("Filering file: " + (CStr) FileToAdd + (CStr) "...");
    File_Struct.FileMem = FileLoadIntoMem(FileToAdd, &File_Struct.FileLength);
    if(File_Struct.FileMem == NULL)
    {
        WriteToStatus("Can't load file '" + (CStr) FileToAdd + (CStr) "'.");
        // Abort process
        return(0);
    }
    for(i = 0; i < Chosen_Filters.Amount(); i++)
    {
        MiscDoEvents(hMDIform.hClient, hGlobAccelerators, hMDIform.hWnd);
        Current_Filter_Name = Dirs[DIR_FILTERS] + (CStr) "\\" + (CStr) Chosen_Filters.Get(i)->Content + (CStr) ".dll";
        FilterLib = LoadLibrary(Current_Filter_Name.Get_String());
        if(FilterLib != 0)
        {
            FilterProc = (LPFILTERFILE (__stdcall *)(char *, LPFILTERFILE, LPCHROMATICLIB)) GetProcAddress(FilterLib, "FilterProc");
            if(FilterProc == NULL)
            {
                WriteToStatus("File '" + (CStr) + Chosen_Filters.Get(i)->Content + (CStr) "' is an invalid Chromatic filter");
                if(File_Struct.FileMem != NULL) FreeMem((long) File_Struct.FileMem);
                // Close the filter
                FreeLibrary(FilterLib);
                // Dlg_Filters.cpp for more infos
                #ifndef _DEBUG
                    CloseHandle(FilterLib);
                #endif
                // Abort process
                return(0);
            }
            // Apply filter on file
            File_Struct_Bak.FileMem = File_Struct.FileMem;
            File_Struct_Bak.FileLength = File_Struct.FileLength;
            Return_File_Struct = FilterProc(FileToAdd, &File_Struct, &AddInLib);
            Flushed = 0;
            if(Return_File_Struct == NULL)
            {
                // If Return_File_Struct is empty that means
                // that the filter consider that it's output is definitive
                // and that we must flush it (to a window or a file)
                Flushed = 1;
                if(Flush_Filter(FileToAdd, &File_Struct) == 0)
                {
                    if(File_Struct_Bak.FileMem != NULL) FreeMem((long) File_Struct_Bak.FileMem);
                    FreeLibrary(FilterLib);
                    #ifndef _DEBUG
                        CloseHandle(FilterLib);
                    #endif
                    return(0);
                }
                // Free old file then reload a clean copy
                // for further processing
                if(File_Struct_Bak.FileMem != NULL) FreeMem((long) File_Struct_Bak.FileMem);
                File_Struct.FileMem = FileLoadIntoMem(FileToAdd, &File_Struct.FileLength);
            }
            else
            {
                // Free the older File_Struct if Return_File_Struct
                // is different and use the new one instead
                // If both are identical then proceed with next filter
                if(Return_File_Struct != &File_Struct_Bak)
                {
                    if(File_Struct_Bak.FileMem != NULL) FreeMem((long) File_Struct_Bak.FileMem);
                    File_Struct.FileMem = Return_File_Struct->FileMem;
                    File_Struct.FileLength = Return_File_Struct->FileLength;
                }
            }
            FreeLibrary(FilterLib);
            #ifndef _DEBUG
                CloseHandle(FilterLib);
            #endif
        }
        else
        {
            if(File_Struct.FileMem != NULL) FreeMem((long) File_Struct.FileMem);
            WriteToStatus("Can't open filter '" + (CStr) + Chosen_Filters.Get(i)->Content + (CStr) "'");
            // Abort process
            return(0);
        }       
    }
    // Flush the last used File_Struct if not NULL
    // and if the file hasn't just been flushed previously
    if(File_Struct.FileMem != NULL && Flushed == 0)
    {
        if(Flush_Filter(FileToAdd, &File_Struct) == 0)
        {
            if(File_Struct.FileMem != NULL) FreeMem((long) File_Struct.FileMem);
            return(0);
        }
    }
    return(1);
}

// Flush filtered datas into a window or a file
// (Called / file)
int Flush_Filter(char *FileName, LPFILTERFILE File_Struct)
{
    HWND NewhWnd;
    CStr LdFile;
    HANDLE hFile;

    switch(Filter_Output_Type)
    {
        case FILTER_OUTPUT_WINDOW:
            switch(Filter_Include)
            {
                // Open mode
                case FILTER_OPEN_MODE:
                    // Create a new window
                    NewhWnd = MCMD_New();
                    if(NewhWnd != NULL)
                    {
                        ChildStruct = LoadStructure(NewhWnd);
                        // Set window text
                        CM_SetText(ChildStruct->hChildCodeMax, (char *) File_Struct->FileMem);
                        // Set modified state on
                        ChildStruct->ModifFlag = 1;
                        // Set window referenced file
                        ChildStruct->RFile->Set_String(StringCopyAppendZero(ChildStruct->RFile, FileRemoveExtension(FileName).Get_String()).Get_String());
                        // Set window title
                        ControlSetText(NewhWnd, FileRemoveExtension(FileName).Get_String() + (CStr) " *");
                        LastLoadAsDBDir = FileGetDirectory(FileName);
                        LoadCurrentSel(ChildStruct->hChildCodeMax);
                        RefreshSBStat = 1;
                        WritePositionInStatus(ChildStruct->hChildCodeMax);
                    }
                    else
                    {
                        WriteToStatus("Can't create window.");
                        return(0);
                    }
                    break;
                // Include mode
                case FILTER_INCLUDE_MODE:
                    // Get current form
                    NewhWnd = CurrentForm;
                    if(NewhWnd != NULL)
                    {
                        ChildStruct = LoadStructure(NewhWnd);
                        LoadCurrentSel(ChildStruct->hChildCodeMax);
                        // Insert text
                        CM_InsertText(ChildStruct->hChildCodeMax, (char *) File_Struct->FileMem, &CodeMaxCurRange);
                        LastIncludeDBDir = FileGetDirectory(FileName);
                    }
                    else
                    {
                        WriteToStatus("Can't obtain window.");
                        return(0);
                    }
                    break;
            }
            break;
        case FILTER_OUTPUT_FILE:
            // Select output file
            switch(Filter_Include)
            {
                // Open mode
                case FILTER_OPEN_MODE:
                    LdFile = ComDlgGetSaveFileName(hMDIform.hWnd, "All files (*.*)|*.*", LastLoadAsDBDir, CurrentDir);
                    if(LdFile.Len() == 0)
                    {
                        // Should we cancel operation silently ?
                        if(Filter_Cancel_Process == 0)
                        {
                            switch(MiscMsgBox(hMDIform.hWnd,
                                              "Do you want to cancel the whole process ?",
                                              MB_ICONQUESTION | MB_YESNO, Requesters))
                            {
                                case IDYES:
                                    return(0);
                                case IDNO:
                                    break;
                            }
                            // Ask the user about the messages
                            switch(MiscMsgBox(hMDIform.hWnd,
                                              "Do you want to see any further messages ?",
                                              MB_ICONQUESTION | MB_YESNO, Requesters))
                            {
                                case IDYES:
                                    break;
                                case IDNO:
                                    Filter_Cancel_Process = 1;
                                    break;
                            }
                        }
                        // User cancelled operation
                        break;
                    }
                    // Save directory
                    LastLoadAsDBDir = FileGetDirectory(FileName);
                    // And save the file (file by file)
                    FileSaveFromMem(LdFile.Get_String(), (long) File_Struct->FileMem, File_Struct->FileLength);
                    WriteToStatus("Output written to file '" + (CStr) LdFile + (CStr) "'.");
                    break;
                // Include mode
                case FILTER_INCLUDE_MODE:
                    if(First_Filter_Include == 0)
                    {
                        LdFile = ComDlgGetSaveFileName(hMDIform.hWnd, "All files (*.*)|*.*", LastIncludeDBDir, CurrentDir);
                        // Save first chosen file
                        Last_Include_File = LdFile;
                    }
                    if(Last_Include_File.Len() == 0)
                    {
                        // Ask user if we should cancel operation silently...
                        if(Filter_Cancel_Process == 0)
                        {
                            switch(MiscMsgBox(hMDIform.hWnd,
                                              "Do you want to cancel the whole process ?",
                                              MB_ICONQUESTION | MB_YESNO, Requesters))
                            {
                                case IDYES:
                                    return(0);
                                case IDNO:
                                    break;
                            }
                            // Ask the user about the messages
                            switch(MiscMsgBox(hMDIform.hWnd,
                                              "Do you want to see any further messages ?",
                                              MB_ICONQUESTION | MB_YESNO, Requesters))
                            {
                                case IDYES:
                                    break;
                                case IDNO:
                                    Filter_Cancel_Process = 1;
                                    break;
                            }
                        }
                        // User cancelled operation
                        break;
                    }
                    LastIncludeDBDir = FileGetDirectory(FileName);
                    if(First_Filter_Include == 0)
                    {
                        // Create it
                        hFile = FileCreateEmpty(Last_Include_File, NO_SECURITY_DESCRIPTOR);
                        if(hFile == INVALID_HANDLE_VALUE)
                        {
                            WriteToStatus("Can't open output file.");
                            return(0);
                        }
                        else
                        {
                            // Save it
                            FileWriteBuffer(hFile, File_Struct->FileMem, File_Struct->FileLength);
                            // Close it
                            CloseHandle(hFile);
                        }
                        // Now ask the user if we should use this single file
                        // or create a new one for each input file given
                        if(First_Filter_Include == 0)
                        {
                            switch(MiscMsgBox(hMDIform.hWnd,
                                              "Do you want to gather all output to this file ?",
                                              MB_ICONQUESTION | MB_YESNO, Requesters))
                            {
                                case IDYES:
                                    First_Filter_Include = 1;
                                    break;
                                case IDNO:
                                    break;
                            }
                        }
                    }
                    else
                    {
                        // Open it for append
                        hFile = FileOpenWAppend(Last_Include_File);
                        if(hFile == INVALID_HANDLE_VALUE)
                        {
                            WriteToStatus("Can't open output file.");
                            return(0);
                        }
                        else
                        {
                            // Save it
                            FileWriteBuffer(hFile, File_Struct->FileMem, File_Struct->FileLength);
                            // Close it
                            CloseHandle(hFile);
                        }
                    }
                    WriteToStatus("Output written to file '" + (CStr) Last_Include_File + (CStr) "'.");
                    break;
            }
            break;
        default:
            return(0);
    }
    return(1);
}

// -----------------------------------------------------------------------
// File/Open workspace
void MCMD_OpenWorkSpace(void)
{
    CStr OpFilters;
    CStr LdFile;

    OpFilters = AppTitle + (CStr) " workspace files (*.Mws)|*.Mws";
    if(GetUseFileDir() == 1)
    {
        if(NbForms != 0)
        {
            ChildStruct = LoadStructure(CurrentForm);
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, FileGetDirectory(ChildStruct->RFile), 0, CurrentDir);
        }
        else
        {
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastWorkSpaceDir, 0, CurrentDir);
        }
    }
    else
    {
        LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastWorkSpaceDir, 0, CurrentDir);
    }
    if(LdFile.Len() != 0) OpenWorkSpc(LdFile);
}

// -----------------------------------------------------------------------
// File/Include file
void MCMD_IncludeFile(void)
{
    CStr OpFilters;
    CStr LdFile;
    int i = 0;

    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    OpFilters = "";
    for(i = 0; i < RegisteredExts.Amount(); i++)
    {
        OpFilters = OpFilters + (CStr) RegisteredExts.Get(i)->Content + (CStr) "|";
    }
    if(OpFilters.Len() == 0) OpFilters = "All files (*.*)|*.*";
    if(GetUseFileDir() == 1)
    {
        if(NbForms != 0)
        {
            ChildStruct = LoadStructure(CurrentForm);
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, FileGetDirectory(ChildStruct->RFile), 1, CurrentDir);
        }
        else
        {
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastIncludeDir, 1, CurrentDir);
        }
    }
    else
    {
        LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastIncludeDir, 1, CurrentDir);
    }
    LastIncludeDir = ComDlgParseMultiFilesSelection(LdFile, EnumIncludeFiles, MULTIFILESENUM_BACKWARD, 0);
}

long CALLBACK EnumIncludeFiles(char *FileToAdd, long UserValue)
{
    if(IncludeFile(CurrentForm, FileToAdd) == 0)
    {
        WriteToStatus("Can't include file '" + (CStr) FileToAdd + (CStr) "'.");
    }
    return(1);
}

// -----------------------------------------------------------------------
// File/Include file as Db
void MCMD_IncludeFilters(void)
{
    if(NbForms != 0 && IsChildReadOnly(CurrentForm) == 1) return;
    // Define filtering mode allowed
    CreateModalDialog(-1, -1, 546, 268, hMDIform.hWnd, &FRMFiltersProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    // Cancel hit
    if(Filter_Cancel) return;
    IncludeFileAsDB();
}

void IncludeFileAsDB(void)
{
    CStr LdFile;

    if(GetUseFileDir() == 1)
    {
        if(NbForms != 0)
        {
            ChildStruct = LoadStructure(CurrentForm);
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, "All files (*.*)|*.*", FileGetDirectory(ChildStruct->RFile), 1, CurrentDir);
        }
        else
        {
            LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, "All files (*.*)|*.*", LastIncludeDBDir, 1, CurrentDir);
        }
    }
    else
    {
        LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, "All files (*.*)|*.*", LastIncludeDBDir, 1, CurrentDir);
    }
    Filter_Include = FILTER_INCLUDE_MODE;
    Filter_Cancel_Process = 0;
    First_Filter_Include = 0;
    Last_Include_File = "";
    if(LdFile.Len() != 0)
    {
        // Enum them in INCLUDE mode
        ComDlgParseMultiFilesSelection(LdFile, &EnumOpenAsDBFiles, MULTIFILESENUM_BACKWARD, 0);
    }
    // Clear the list of chosen filters now
    Chosen_Filters.Erase();
}

// -----------------------------------------------------------------------
// File/FTP manager
void MCMD_FTPManager(void)
{
    int i = 0;
    long FoundFtpAccounts = 0;
    CStr BufString;

    for(i = 0; i <= 999; i++)
    {
        MenusRetVal = IniReadKey("FTPAccounts", "FTPAccount" + (CStr) StringNumberComplement(i, 3).Get_String(), FtpAccountsIniFile);
        if(MenusRetVal.Len() == 0) break;
        FoundFtpAccounts++;
    }
    if(FoundFtpAccounts == 0)
    {
        switch(MiscMsgBox(hMDIform.hWnd, "Can't find any FTP accounts.\rDo you want to create some now ?", MB_QUESTION, Requesters))
        {
            case IDNO:
                return;
            case IDYES:
                // Launch properties with ftp accounts displayed at first
                FRMPropertiesFirstTab = 3;
                DisplayProperties();
                return;
        }
    }
    CreateModalDialog(-1, -1, 560, 362, hMDIform.hWnd, &FRMFTPProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// File/Save workspace
void MCMD_SaveWorkSpace(void)
{
    if(NbForms == 0) if(ProjectOn == 0) return;
    CStr LdFile;
    CStr OpFilters;

    OpFilters = AppTitle + (CStr) " workspace files (*.Mws)|*.Mws";
    LdFile = ComDlgGetSaveFileName(hMDIform.hWnd, OpFilters, LastWorkSpaceDir, CurrentDir);
    if(LdFile.Len() != 0)
    {
        WorkSpaceFileName = LdFile;
        // Delete the file first
        DeleteFile(WorkSpaceFileName.Get_String());
        IniWriteKey(AppTitle.Upper_Case().Get_String() + (CStr) "WORKSPC", "Version", AppVersion + (CStr) AppRevMaj + (CStr) AppRevMin, WorkSpaceFileName);
        SaveAllChildsInWorkSpace(hMDIform.hClient);
        // Include project file into the list
        if(ProjectOn == 1)
        {
            IniWriteKey("Files",
                        "File" + (CStr) StringNumberComplement(WorkSpaceNumber, 3).Get_String(),
                        ProjectFName, WorkSpaceFileName);
        }
        LastWorkSpaceDir = FileGetDirectory(WorkSpaceFileName);
        AddRecentFile(WorkSpaceFileName, 1, 0);
    }
}

// -----------------------------------------------------------------------
// File/Close file
void MCMD_CloseFile(void)
{
    if(NbForms != 0) ControlClose(CurrentForm);
    if(NbForms != 0) ClientSetPreviousChild(hMDIform.hClient);
}

// -----------------------------------------------------------------------
// File/Close all files
void MCMD_CloseAll(void)
{
    if(NbForms != 0) CloseAllChilds(hMDIform.hClient);
}

// -----------------------------------------------------------------------
// File/Save file
void MCMD_SaveFile(void)
{
    if(NbForms != 0) SaveIt(CurrentForm);
}

// -----------------------------------------------------------------------
// File/Save file as
void MCMD_SaveFileAs(void)
{
    if(NbForms != 0) SaveItAs(CurrentForm, 0, "", "");
}

// -----------------------------------------------------------------------
// File/Register as snippet
void MCMD_RegSnippet(void)
{
    CStr FileToSnippet;
    CStr FileToSnippet2;
    CStr RetVal2;
    long FoundOcc = 0;
    long FoundOcc2 = 0;
    CStr TempSnippetname;
    int i = 0;
    int j = 0;
    CStr BufString;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Text is empty.", MB_ERROR, Requesters);
        return;
    }
    if(_strcmpi(ChildStruct->RFile->Left(18).Get_String(), "<untitled document") == 0) if(SaveItAs(CurrentForm, 0, "", "").Len() == 0) return;
    FileToSnippet = CMGetRealFile(ChildStruct->RFile);
    TempSnippetname = MiscInputBox(hMDIform.hWnd, "Enter snippet name", ICON_CUBES, PassValueSnippet, 0, INPUTBOX_SIMPLETEXT, "");
    if(TempSnippetname.Len() != 0)
    {
        PassValueSnippet = TempSnippetname;
        // Get first available slot
        // Check for occurences
        FoundOcc = 0;
        FoundOcc2 = 0;
        // Search the key
        for(i = 0; i <= 999; i++)
        {
            MenusRetVal = IniReadKey("Snippets", "SnippetName" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            if(MenusRetVal.Len() == 0) break;
            if(_strcmpi(MenusRetVal.Get_String(), PassValueSnippet.Get_String()) == 0)
            {
                FoundOcc = 1;
                break;
            }
        }
        // Search the file
        FoundOcc2 = 0;
        for(j = 0; j <= 999; j++)
        {
            MenusRetVal = IniReadKey("Snippets", "SnippetFile" + (CStr) StringNumberComplement(j, 3).Get_String(), MainIniFile);
            if(MenusRetVal.Len() == 0) break;
            MenusRetVal = ChangeRelativePaths(MenusRetVal);
            if(_strcmpi(MenusRetVal.Get_String(), FileToSnippet.Get_String()) == 0)
            {
                FoundOcc2 = 1;
                break;
            }
        }
        // Found a name / no file
        if(FoundOcc == 1) if(FoundOcc2 == 0) goto RegSlot;
        // No name / found a file
        if(FoundOcc == 0)
        {
            if(FoundOcc2 == 1)
            {
                // Place at found file
                i = j;
                goto RegSlot;
            }
        }
        // Found a name / Found a file : conflict
        if(FoundOcc == 1) if(FoundOcc2 == 1) goto NoSlot;
        // No name / no file: get the first free slot
        if(FoundOcc == 0)
        {
            if(FoundOcc2 == 0)
            {
                for(i = 0; i <= 999; i++)
                {
                    MenusRetVal = IniReadKey("Snippets", "SnippetName" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                    if(MenusRetVal.Len() == 0) break;
                }
            }
        }
RegSlot:
        // Save the bookmark of the snippet
        SaveBookmarks(CurrentForm, FileToSnippet, 0, -1);
        // Write the datas in the slot
        IniWriteKey("Snippets", "SnippetName" + (CStr) StringNumberComplement(i, 3).Get_String(), PassValueSnippet, MainIniFile);
        FileToSnippet2 = FileToSnippet;
        FileToSnippet2 = ChangeAbsolutePaths(FileToSnippet2);
        IniWriteKey("Snippets", "SnippetFile" + (CStr) StringNumberComplement(i, 3).Get_String(), FileToSnippet2, MainIniFile);
    }
    return;
NoSlot:
    BufString = "Snippet key/file conflict.\rThis file is aleady registered under key '" + (CStr) IniReadKey("Snippets", "SnippetName" + (CStr) StringNumberComplement(j, 3).Get_String(), MainIniFile).Get_String() + (CStr) "'";
    MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
}

// -----------------------------------------------------------------------
// File/Register as template
void MCMD_RegTemplate(void)
{
    CStr FileToSnippet;
    CStr FileToSnippet2;
    CStr RetVal2;
    long FoundOcc = 0;
    long FoundOcc2 = 0;
    CStr TempTemplateName;
    int i = 0;
    int j = 0;
    CStr BufString;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Text is empty.", MB_ERROR, Requesters);
        return;
    }
    if(_strcmpi(ChildStruct->RFile->Left(18).Get_String(), "<untitled document") == 0) if(SaveItAs(CurrentForm, 0, "", "").Len() == 0) return;
    FileToSnippet = CMGetRealFile(ChildStruct->RFile);
    // Save it before
    TempTemplateName = MiscInputBox(hMDIform.hWnd, "Enter source template name", ICON_CUBES, PassValueTemplate, 0, INPUTBOX_SIMPLETEXT, "");
    if(TempTemplateName.Len() != 0)
    {
        PassValueTemplate = TempTemplateName;
        // Get first available slot
        // Check for occurences
        FoundOcc = 0;
        FoundOcc2 = 0;
        // Search the key
        for(i = 0; i <= 999; i++)
        {
            MenusRetVal = IniReadKey("Templates", "TemplateName" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
            if(MenusRetVal.Len() == 0) break;
            if(_strcmpi(MenusRetVal.Get_String(), PassValueTemplate.Get_String()) == 0)
            {
                FoundOcc = 1;
                break;
            }
        }
        // Search the file
        FoundOcc2 = 0;
        for(j = 0; j <= 999; j++)
        {
            MenusRetVal = IniReadKey("Templates", "TemplateFile" + (CStr) StringNumberComplement(j, 3).Get_String(), MainIniFile);
            if(MenusRetVal.Len() == 0) break;
            MenusRetVal = ChangeRelativePaths(MenusRetVal);
            if(_strcmpi(MenusRetVal.Get_String(), FileToSnippet.Get_String()) == 0)
            {
                FoundOcc2 = 1;
                break;
            }
        }
        // Found a name / no file
        if(FoundOcc == 1) if(FoundOcc2 == 0) goto RegSlot;
        // No name / found a file
        if(FoundOcc == 0)
        {
            if(FoundOcc2 == 1)
            {
                // Place at found file
                i = j;
                goto RegSlot;
            }
        }
        // Found a name / Found a file : conflict
        if(FoundOcc == 1) if(FoundOcc2 == 1) goto NoSlot;
        // No name / no file: get the first free slot
        if(FoundOcc == 0)
        {
            if(FoundOcc2 == 0)
            {
                for(i = 0; i <= 999; i++)
                {
                    MenusRetVal = IniReadKey("Templates", "TemplateName" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                    if(MenusRetVal.Len() == 0) break;
                }
            }
        }
RegSlot:
        // Save the bookmark of the snippet
        SaveBookmarks(CurrentForm, FileToSnippet, 0, -1);
        // Write the datas in the slot
        IniWriteKey("Templates", "TemplateName" + (CStr) StringNumberComplement(i, 3).Get_String(), PassValueTemplate, MainIniFile);
        FileToSnippet2 = FileToSnippet;
        FileToSnippet2 = ChangeAbsolutePaths(FileToSnippet2);
        IniWriteKey("Templates", "TemplateFile" + (CStr) StringNumberComplement(i, 3).Get_String(), FileToSnippet2, MainIniFile);
        RefreshNewMenu();
    }
    return;
NoSlot:
    BufString = "Template key/file conflict.\rThis file is aleady registered under key '" + (CStr) IniReadKey("Templates", "TemplateName" + (CStr) StringNumberComplement(j, 3).Get_String(), MainIniFile).Get_String() + (CStr) "'";
    MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
}

// -----------------------------------------------------------------------
// File/Save all
void MCMD_SaveAll(void)
{
    if(NbForms != 0) SaveAllChilds(hMDIform.hClient);
}

// -----------------------------------------------------------------------
// File/Print file
void MCMD_PrintFile(void)
{
    if(NbForms != 0) PrintFile(CurrentForm, 0);
}

// -----------------------------------------------------------------------
// File/Console/Run
void MCMD_RunConsole(void)
{
    ShellExecute(0, "open", IniReadKey("Layout", "MSDOS", MainIniFile).Get_String(), "", "", SW_SHOW);
}

// -----------------------------------------------------------------------
// File/Console/Change
void MCMD_ChangeConsole(void)
{
    CStr PassValueDOS;
    
    PassValueDOS = IniReadKey("Layout", "MSDOS", MainIniFile);
    PassValueDOS = MiscInputBox(hMDIform.hWnd, "Enter console prompt command", ICON_MSDOS, PassValueDOS, 0, INPUTBOX_COMBO, "Shells");
    if(PassValueDOS.Len() == 0) PassValueDOS = "command.com";
    IniWriteKey("Layout", "MSDOS", PassValueDOS, MainIniFile);
}

// -----------------------------------------------------------------------
// File/Run external program
void MCMD_RunExtProg(void)
{
    CStr OpFilters;
    CStr CmdToOpen;

    OpFilters = "All files (*.*)|*.*";
    if(GetUseFileDir() == 1)
    {
        if(NbForms != 0)
        {
            ChildStruct = LoadStructure(CurrentForm);
            CmdToOpen = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, FileGetDirectory(ChildStruct->RFile), 0, CurrentDir);
        }
        else
        {
            CmdToOpen = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastExeDir, 0, CurrentDir);
        }
    }
    else
    {
        CmdToOpen = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastExeDir, 0, CurrentDir);
    }
    RunExtProg(CmdToOpen);
}

// -----------------------------------------------------------------------
// File/Run programs/Clear run programs files list
void MCMD_PurgeRunProgAll(void)
{
    int i = 0;

    for(i = 0; i < RunProgArray.Amount(); i++)
    {
        RunProgArray.Set(i, "");
    }
    AddRecentRunProg("", 0, 1);
    IniDeleteKey("RunProg", "", MainIniFile);
}

// -----------------------------------------------------------------------
// File/Recent files/Remove obsolete
void MCMD_PurgeRecent(void)
{
    int i = 0;

    for(i = 0; i < Recents.Amount(); i++)
    {
        if(strlen(Recents.Get(i)->Content) != 0) if(FileExist(Recents.Get(i)->Content) == 0) Recents.Set(i, "");
    }
    AddRecentFile("", 0, 1);
}

// -----------------------------------------------------------------------
// File/Recent files/Clear recent files list
void MCMD_PurgeRecentAll(void)
{
    int i = 0;

    for(i = 0; i < Recents.Amount(); i++)
    {
        Recents.Set(i, "");
    }
    AddRecentFile("", 0, 1);
}

// -----------------------------------------------------------------------
// File/Recent projects/Remove obsolete
void MCMD_PurgeRecentPrj(void)
{
    int i = 0;

    for(i = 0; i < RecentsPrj.Amount(); i++)
    {
        if(strlen(RecentsPrj.Get(i)->Content) != 0) if(FileExist(RecentsPrj.Get(i)->Content) == 0) RecentsPrj.Set(i, "");
    }
    AddRecentPrj("", 0, 1);
}

// -----------------------------------------------------------------------
// File/Recent projects/Erase recent projects list
void MCMD_PurgeRecentPrjAll(void)
{
    int i = 0;

    for(i = 0; i < RecentsPrj.Amount(); i++)
    {
        RecentsPrj.Set(i, "");
    }
    AddRecentPrj("", 0, 1);
}

// -----------------------------------------------------------------------
// File/Organize favorites
void MCMD_Favorites(void)
{
    CreateModalDialog(-1, -1, 486, 318, hMDIform.hWnd, &FRMFavoritesProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// File/Add to favorites
void MCMD_AddToFavorites(void)
{
    CStr NameToAdd;
    CStr NameFromList;
    int i = 0;

    if(NbForms != 0)
    {
        ChildStruct = LoadStructure(CurrentForm);
        if(ChildStruct->FileLoaded == 1)
        {
            NameToAdd = CMGetRealFile(ChildStruct->RFile);
            // Look for file in list
            for(i = 0; i <= 999; i++)
            {
                NameFromList = IniReadKey("Favorites", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), MainIniFile);
                if(NameFromList.Len() == 0) break;
                if(_strcmpi(NameToAdd.Get_String(), NameFromList.Get_String()) == 0)
                {
                    return;
                }
            }
            // Write it in the list
            IniWriteKey("Favorites", "File" + (CStr) StringNumberComplement(i, 3).Get_String(), NameToAdd, MainIniFile);
            // Reconstruct the menu
            CreateFavoritesMenu();
        }
    }
}

// -----------------------------------------------------------------------
// File/Set working dir
void MCMD_SetWorkingDir(void)
{
    SetCurDir(hMDIform.hWnd);
}

// -----------------------------------------------------------------------
// File/Exit
void MCMD_Exit(void)
{
    PostMessage(hMDIform.hWnd, WM_CLOSE, 0, 0);
}

// -----------------------------------------------------------------------
// Edit/Undo
void MCMD_Undo(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_Undo(ChildStruct->hChildCodeMax);
    if(CM_GetAllBookmarks(ChildStruct->hChildCodeMax, 0) || CM_GetAllBreakpoints(ChildStruct->hChildCodeMax, 0))
    {
        CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 1);
    }
    else
    {
        if(LeftM == 0) CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 0);
    }
}

// -----------------------------------------------------------------------
// Edit/Redo
void MCMD_Redo(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_Redo(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Edit/Cut
void MCMD_Cut(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_Cut(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Edit/Copy/Copy
void MCMD_Copy(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_Copy(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Edit/Copy/Copy file name/path
void MCMD_CopyFileNamePath(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(ChildStruct->FileLoaded == 0) return;
    MiscClipBoardCopyText(CMGetRealFile(ChildStruct->RFile));
}

// -----------------------------------------------------------------------
// Edit/Copy/Copy entire text
void MCMD_CopyEntireText(void)
{
    CStr EntText;
    
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    EntText = EntText.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
    CM_GetTextAll(ChildStruct->hChildCodeMax, EntText.Get_String());
    MiscClipBoardCopyText(EntText);
}

// -----------------------------------------------------------------------
// Edit/Paste
void MCMD_Paste(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_Paste(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Edit/Paste to new window
void MCMD_PasteNewWindow(void)
{
    CStr OpenedLanguage;
    HWND PasteForm = 0;

    if(MiscClipBoardIsEmpty() == 0)
    {
        OpenedLanguage = IniReadKey("RefLanguages", "DefLang", LanguagesIniFile);
        if(OpenedLanguage.Len() == 0) OpenedLanguage = "x86 Assembler";
        StoreLanguageToOpen(OpenedLanguage);
        PasteForm = CreateNewFile("<Untitled document " + (CStr) (NbForms + 1) + (CStr) ">");
        if(PasteForm == 0) return;
        ChildStruct = LoadStructure(PasteForm);
        CM_Paste(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Edit/Delete/Delete selection
void MCMD_DeleteSelection(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_DeleteSel(ChildStruct->hChildCodeMax);
}

// -----------------------------------------------------------------------
// Edit/Delete/Delete line
void MCMD_DeleteLine(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    CM_DeleteLine(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine);
}

// -----------------------------------------------------------------------
// Edit/Delete/Delete to start of line
void MCMD_DeleteToStart(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDELETETOSTART, -1);
}

// -----------------------------------------------------------------------
// Edit/Delete/Delete to end of line
void MCMD_DeleteToEnd(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LINEDELETETOEND, 0);
}

// -----------------------------------------------------------------------
// Edit/Delete/Delete paragraph
void MCMD_DeleteParagraph(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SENTENCECUT, 0);
}

// -----------------------------------------------------------------------
// Edit/Select line
void MCMD_SelectLine(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SELECTLINE, 0);
}

// -----------------------------------------------------------------------
// Edit/Select all
void MCMD_SelectAll(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SELECTALL, 0);
}

// -----------------------------------------------------------------------
// Edit/Select procedure
void MCMD_SelectProc(void)
{
    if(NbForms == 0) return;
    SelectProcedure(CurrentForm);
}

// -----------------------------------------------------------------------
// Edit/Select procedure and collapse
void MCMD_SelectProcCollapse(void)
{
    CM_RANGE SelProcRange;
    CStr ProcToSave;
    CStr CurDirC;
    CStr SelToSave;
    CStr CollapsedExtension;
    CStr CollapsedInclude;
    CStr CollapsedComment;
    CStr CollapsedFile;
    CStr BufString;
                        
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    if(SelectProcedure(CurrentForm) == 1)
    {
        ChildStruct = LoadStructure(CurrentForm);
        ProcToSave = GetProcName(CMGetCurrentLine(ChildStruct->hChildCodeMax, GetCurrentLineNumber(ChildStruct->hChildCodeMax)), LastProcTypeKeyword);
        if(ProcToSave.Len() == 0)
        {
            MiscMsgBox(hMDIform.hWnd, "Error in procedure structure.\rCan't collapse.", MB_ERROR, Requesters);
            if(ControlIsVisible(ChildStruct->hChildCodeMax) != 0) SetFocus(ChildStruct->hChildCodeMax);
            return;
        }
        CurDirC = FileGetDirectory(ChildStruct->RFile->Left(ChildStruct->RFile->Len()));
        if(strcmp(CurDirC.Right(1).Get_String(), "\\") == 0) CurDirC = CurDirC.Left(CurDirC.Len() - 1);
        CollapsedExtension = FileGetExtension(CMGetRealFile(ChildStruct->RFile));
        BufString = CurDirC + (CStr) "\\" + (CStr) ProcToSave + (CStr) "." + (CStr) CollapsedExtension;
        DeleteFile(BufString.Get_String());
        // Save the procedure into a file
        LoadCurrentSel(ChildStruct->hChildCodeMax);
        SelToSave = SelToSave.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
        CM_GetText(ChildStruct->hChildCodeMax, SelToSave.Get_String(), &CodeMaxCurRange);
        CollapsedInclude = GetCMLangInclude(CurrentForm);
        if(CollapsedInclude.Len() == 0)
        {
            MiscMsgBox(hMDIform.hWnd, "No include definition found for current file.", MB_ERROR, Requesters);
            return;
        }
        CollapsedInclude = StringReplace(CollapsedInclude, "%1", ProcToSave + (CStr) "." + (CStr) CollapsedExtension, 1, -1, Binary_Compare);
        CollapsedComment = CollapsedComment + (CStr) " " + (CStr) GetCMLangComment(CurrentForm).Get_String() + (CStr) " --- Collapsed procedure: " + (CStr) ProcToSave;
        CollapsedFile = CurDirC + (CStr) "\\" + (CStr) ProcToSave + (CStr) "." + (CStr) CollapsedExtension;
        if(MSaveFile(CollapsedFile.Get_String(), (long) SelToSave.Get_String(), SelToSave.Len()) == 0)
        {
            MiscMsgBox(hMDIform.hWnd, "Error while saving procedure\rCan't collapse.", MB_ERROR, Requesters);
            return;
        }
        MCMD_DeleteSelection();
        BufString = "\t\t" + (CStr) CollapsedInclude + (CStr) CollapsedComment;
        CM_InsertLine(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine, BufString.Get_String());
        SelProcRange.posStart.nCol = 0;
        SelProcRange.posStart.nLine = CodeMaxCurRange.posStart.nLine;
        SelProcRange.posEnd.nCol = 0;
        SelProcRange.posEnd.nLine = SelProcRange.posStart.nLine;
        CM_SetSel(ChildStruct->hChildCodeMax, &SelProcRange, 1);
    }
}

// -----------------------------------------------------------------------
// Edit/Uncollapse a procedure
void MCMD_UnCollapse(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    OpenFileUnderCursor(1, 0);
}

// -----------------------------------------------------------------------
// Edit/Find
void MCMD_Find(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FIND, 0);
}

// -----------------------------------------------------------------------
// Edit/Find next
void MCMD_FindNext(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDNEXT, 0);
}

// -----------------------------------------------------------------------
// Edit/Find previous
void MCMD_FindPrevious(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDPREV, 0);
}

// -----------------------------------------------------------------------
// Edit/Find from selection (Find fast)
void MCMD_FindSelection(void)
{
    CStr TxtToFind;
    
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(GetCurrentLogicalSelLen(ChildStruct->hChildCodeMax) == 0)
    {
        TxtToFind = GetCurrentWord(ChildStruct->hChildCodeMax);
    }
    else
    {
        TxtToFind = GetCurrentLogicalSelText(ChildStruct->hChildCodeMax);
    }
    CM_SetFindText(ChildStruct->hChildCodeMax, TxtToFind.Get_String());
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDNEXT, 0);
}

// -----------------------------------------------------------------------
// Edit/Replace
void MCMD_Replace(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_FINDREPLACE, 0);
}

// -----------------------------------------------------------------------
// Procedures/Variables search
void MCMD_ProcVarSearch(void)
{
    CStr BufString;

    if(NbForms == 0) if(ProjectOn == 0) return;
    CreateModalDialog(-1, -1, 347, 139, hMDIform.hWnd, &FRMProcVarSearchProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    if(OkSearch == 1)
    {
        if(ListViewItemCount(FrmResultsListView) == 0)
        {
            if(hFRMResults != 0) ControlClose(hFRMResults);
            if(FRMProcVarSearchChkRawValue == 1)
            {
                BufString = "0 entry found.\rKeyword was: '" + (CStr) LastSearch + (CStr) "'\r";
            }
            else
            {
                BufString = "0 " + (CStr) ProcType + (CStr) " found\rKeyword was: '" + (CStr) LastSearch + (CStr) "'\r";
            }
            BufString = BufString + (CStr) "Passed through: " + (CStr) LastSearchFiles + (CStr) " file(s) ";
            BufString = BufString + (CStr) "and " + (CStr) LastSearchLines + (CStr) " line(s).";
            MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
            return;
        }
        if(ControlIsVisible(hFRMResults) == 0) ShowWindow(hFRMResults, SW_SHOW);
        ListViewSetItemSel(FrmResultsListView, 0);
    }
}

// -----------------------------------------------------------------------
// Edit/Search in files
void MCMD_SearchInFiles(void)
{
    CStr BufString;

    CreateModalDialog(-1, -1, 347, 213, hMDIform.hWnd, &FRMFileSearchProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    if(OkSearch == 1)
    {
        if(ListViewItemCount(FrmResultsListView) == 0)
        {
            if(hFRMResults != 0) ControlClose(hFRMResults);
            if(FRMFileSearchChkRawValue == 1)
            {
                BufString = "0 entry found.\rKeyword was: '" + (CStr) LastSearch + (CStr) "'\rWildcard was: '" + (CStr) LastWild + (CStr) "'\r";
            }
            else
            {
                BufString = "0 " + (CStr) ProcType + (CStr) " found\rKeyword was: '" + (CStr) LastSearch + (CStr) "'\rWildcard was: '" + (CStr) LastWild + (CStr) "'\r";
            }
            BufString = BufString + (CStr) "Passed through: " + (CStr) LastSearchFiles + (CStr) " file(s) ";
            BufString = BufString + (CStr) "and " + (CStr) LastSearchLines + (CStr) " line(s).";
            MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
            return;
        }
        if(ControlIsVisible(hFRMResults) == 0) ShowWindow(hFRMResults, SW_SHOW);
        ListViewSetItemSel(FrmResultsListView, 0);
    }
}

// -----------------------------------------------------------------------
// Edit/Bookmark/Toggle
void MCMD_ToggleBookMark(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BOOKMARKTOGGLE, 0);
    if(CM_GetAllBookmarks(ChildStruct->hChildCodeMax, 0) || CM_GetAllBreakpoints(ChildStruct->hChildCodeMax, 0))
    {
        CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 1);
    }
    else
    {
        if(LeftM == 0) CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 0);
    }
}

// -----------------------------------------------------------------------
// Edit/Bookmark/First
void MCMD_FirstBookMark(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BOOKMARKJUMPTOFIRST, 0);
}

// -----------------------------------------------------------------------
// Edit/Bookmark/Previous
void MCMD_PreviousBookMark(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BOOKMARKPREV, 0);
}

// -----------------------------------------------------------------------
// Edit/Bookmark/Next
void MCMD_NextBookMark(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BOOKMARKNEXT, 0);
}

// -----------------------------------------------------------------------
// Edit/Bookmark/Last
void MCMD_LastBookMark(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BOOKMARKJUMPTOLAST, 0);
}

// -----------------------------------------------------------------------
// Edit/Bookmark/Clear All
void MCMD_ClearAllBookMark(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BOOKMARKCLEARALL, 0);
    if(LeftM == 0)
    {
        // No breakpoints left ?
        if(!CM_GetAllBreakpoints(ChildStruct->hChildCodeMax, 0))
        {
            // Remove margin
            CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Edit/Breakpoint/Toggle
void MCMD_ToggleBreakpoint(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BREAKPOINTTOGGLE, 0);
    if(CM_GetAllBookmarks(ChildStruct->hChildCodeMax, 0) || CM_GetAllBreakpoints(ChildStruct->hChildCodeMax, 0))
    {
        CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 1);
    }
    else
    {
        if(LeftM == 0) CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 0);
    }
}

// -----------------------------------------------------------------------
// Edit/Breakpoint/First
void MCMD_FirstBreakpoint(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BREAKPOINTJUMPTOFIRST, 0);
}

// -----------------------------------------------------------------------
// Edit/Breakpoint/Previous
void MCMD_PreviousBreakpoint(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BREAKPOINTPREV, 0);
}

// -----------------------------------------------------------------------
// Edit/Breakpoint/Next
void MCMD_NextBreakpoint(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BREAKPOINTNEXT, 0);
}

// -----------------------------------------------------------------------
// Edit/Breakpoint/Last
void MCMD_LastBreakpoint(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BREAKPOINTJUMPTOLAST, 0);
}

// -----------------------------------------------------------------------
// Edit/Breakpoint/Clear All
void MCMD_ClearAllBreakpoint(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_BREAKPOINTCLEARALL, 0);
    if(LeftM == 0)
    {
        // No bookmarks left ?
        if(!CM_GetAllBookmarks(ChildStruct->hChildCodeMax, 0))
        {
            // Remove margin
            CM_EnableLeftMargin(ChildStruct->hChildCodeMax, 0);
        }
    }
}

// -----------------------------------------------------------------------
// Edit/Goto/Top
void MCMD_GotoTop(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, 0);
}

// -----------------------------------------------------------------------
// Edit/Goto/Bottom
void MCMD_GotoBottom(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, CM_GetLineCount(ChildStruct->hChildCodeMax) - 1);
}

// -----------------------------------------------------------------------
// Edit/Goto/Line
void MCMD_GotoLine(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOLINE, -1);
}

// -----------------------------------------------------------------------
// Edit/Goto/Matching bracket
void MCMD_GotoMatchingBracket(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_GOTOMATCHBRACE, 0);
}

// -----------------------------------------------------------------------
// Edit/Goto/Previous word
void MCMD_PrevWord(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDLEFT, 0);
}

// -----------------------------------------------------------------------
// Edit/Goto/Next word
void MCMD_NextWord(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_WORDRIGHT, 0);
}

// -----------------------------------------------------------------------
// Edit/Goto/Previous procedure
void MCMD_PreviousProc(void)
{
    int i = 0;
    int j = 0;
    long PosCommentLine = 0;
    CStr CurLine;
    long CurLinePos = 0;
    long EndpPos = 0;
    CM_RANGE ProcRange;
    CStr CurLangComment;
    CStr CurLangProc;
    long *CurLangProcArray = 0;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CurLinePos = GetCurrentLineNumber(ChildStruct->hChildCodeMax) - 1;
    CurLangComment = GetCMLangComment(CurrentForm);
    CurLangProc = GetCMLangProc(CurrentForm);
    CurLangProcArray = StringSplit(CurLangProc, " ");
    EndpPos = -1;
    if(CurLinePos > 0)
    {
        // Check text to the end
        for(i = CurLinePos; i >= 0; i--)
        {
            CurLinePos = i + 1;
            CurLine = CMGetCurrentLine(ChildStruct->hChildCodeMax, i).Trim();
            PosCommentLine = CurLine.In_Str(1, CurLangComment);
            if(PosCommentLine != 0)
            {
                if(PosCommentLine != 1) CurLine = CurLine.Left(PosCommentLine - 1);
                else CurLine = "";
            }
            if(CurLine.Len() != 0)
            {
                CurLine = RemoveQuotesVB(StringReplace(CurLine, "\t", " ", 1, -1, Binary_Compare)).Trim();
                for(j = 0; j <= StringGetSplitUBound(CurLangProcArray); j++)
                {
                    // xxx proc xxx
                    if(CurLine.In_Str(1, " " + (CStr) StringGetSplitElement(CurLangProc, CurLangProcArray, j).Get_String() + (CStr) " ", Text_Compare) != 0)
                    {
                        EndpPos = i;
                        goto FoundPrevProc;
                    }
                    // xxx proc
                    if((CurLine.Len() - StringGetSplitElement(CurLangProc, CurLangProcArray, j).Len()) != 0)
                    {
                        if((CurLine.In_Str(1, " " + (CStr) StringGetSplitElement(CurLangProc, CurLangProcArray, j).Get_String(), Text_Compare)) == (CurLine.Len() - StringGetSplitElement(CurLangProc, CurLangProcArray, j).Len()))
                        {
                            EndpPos = i;
                            goto FoundPrevProc;
                        }
                    }
                    // proc xxx
                    if(CurLine.In_Str(1, StringGetSplitElement(CurLangProc, CurLangProcArray, j).Get_String() + (CStr) " ", Text_Compare) == 1)
                    {
                        EndpPos = i;
                        goto FoundPrevProc;
                    }
                }
            }
        }
FoundPrevProc:
        if(EndpPos == -1)
        {
            MCMD_GotoTop();
        }
        else
        {
            ProcRange.posStart.nCol = 0;
            ProcRange.posEnd.nCol = 0;
            ProcRange.posEnd.nLine = EndpPos;
            ProcRange.posStart.nLine = EndpPos;
            CM_SetTopIndex(ChildStruct->hChildCodeMax, CM_GetCurrentView(ChildStruct->hChildCodeMax), EndpPos);
            CM_SetSel(ChildStruct->hChildCodeMax, &ProcRange, 1);
        }
    }
    StringReleaseSplit(CurLangProcArray);
}

// -----------------------------------------------------------------------
// Edit/Goto/Next procedure
void MCMD_NextProc(void)
{
    int i = 0;
    int j = 0;
    long PosCommentLine = 0;
    CStr CurLine;
    long CurLinePos = 0;
    long EndpPos = 0;
    CM_RANGE ProcRange;
    CStr CurLangComment;
    CStr CurLangProc;
    long *CurLangProcArray = 0;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CurLinePos = GetCurrentLineNumber(ChildStruct->hChildCodeMax) + 1;
    CurLangComment = GetCMLangComment(CurrentForm);
    CurLangProc = GetCMLangProc(CurrentForm);
    CurLangProcArray = StringSplit(CurLangProc, " ");
    EndpPos = -1;
    if(StringGetSplitUBound(CurLangProcArray) != -1)
    {
        // Check text to the end
        for(i = CurLinePos; i <= (long) (CM_GetLineCount(ChildStruct->hChildCodeMax) - 1); i++)
        {
            CurLinePos = i + 1;
            CurLine = CMGetCurrentLine(ChildStruct->hChildCodeMax, i).Trim();
            PosCommentLine = CurLine.In_Str(1, CurLangComment);
            if(PosCommentLine != 0)
            {
                if(PosCommentLine != 1)
                {
                    CurLine = CurLine.Left(PosCommentLine - 1);
                }
                else
                {
                    CurLine = "";
                }
            }
            if(CurLine.Len() != 0)
            {
                CurLine = RemoveQuotesVB(StringReplace(CurLine, "\t", " ", 1, -1, Binary_Compare)).Trim();
                for(j = 0; j <= StringGetSplitUBound(CurLangProcArray); j++)
                {
                    // xxx proc xxx
                    if(CurLine.In_Str(1, " " + (CStr) StringGetSplitElement(CurLangProc, CurLangProcArray, j).Get_String() + (CStr) " ", Text_Compare) != 0)
                    {
                        EndpPos = i;
                        goto FoundNextProc;
                    }
                    // xxx proc
                    if((CurLine.Len() - StringGetSplitElement(CurLangProc, CurLangProcArray, j).Len()) != 0)
                    {
                        if(CurLine.In_Str(1, " " + (CStr) StringGetSplitElement(CurLangProc, CurLangProcArray, j).Get_String(), Text_Compare) == (CurLine.Len() - StringGetSplitElement(CurLangProc, CurLangProcArray, j).Len()))
                        {
                            EndpPos = i;
                            goto FoundNextProc;
                        }
                    }
                    // proc xxx
                    if(CurLine.In_Str(1, StringGetSplitElement(CurLangProc, CurLangProcArray, j).Get_String() + (CStr) " ", Text_Compare) == 1)
                    {
                        EndpPos = i;
                        goto FoundNextProc;
                    }
                }
            }
        }
FoundNextProc:
        if(EndpPos == -1)
        {
            MCMD_GotoBottom();
        }
        else
        {
            ProcRange.posStart.nCol = 0;
            ProcRange.posEnd.nCol = 0;
            ProcRange.posEnd.nLine = EndpPos;
            ProcRange.posStart.nLine = EndpPos;
            CM_SetTopIndex(ChildStruct->hChildCodeMax, CM_GetCurrentView(ChildStruct->hChildCodeMax), EndpPos);
            CM_SetSel(ChildStruct->hChildCodeMax, &ProcRange, 1);
        }
    }
    StringReleaseSplit(CurLangProcArray);
}

// -----------------------------------------------------------------------
// Edit/Goto/Previous paragraph
void MCMD_PreviousParagraph(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SENTENCELEFT, 0);
}

// -----------------------------------------------------------------------
// Edit/Goto/Next paragraph
void MCMD_NextParagraph(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_SENTENCERIGHT, 0);
}

// -----------------------------------------------------------------------
// Edit/Goto/Variable declaration
void MCMD_VarDeclaration(void)
{
    CStr BufString;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    if(SearchVarEntry(hMDIform.hClient, GetCurrentWord(ChildStruct->hChildCodeMax)) == 0)
    {
        BufString = "No declaration found for variable '" + (CStr) VarProcToSearchReal + (CStr) "'.";
        MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
    }
    else
    {
        LoadStructure(VarProcFoundhwnd);
        CMSetCaretPos(ChildStruct->hChildCodeMax, VarProcFoundLine - 1, 0);
        SetFocus(VarProcFoundhwnd);
        SetFocus(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Edit/Goto/Variable next use
void MCMD_VarNextUse(void)
{
    CStr BufString;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    if(SearchVarNextUse(hMDIform.hClient, GetCurrentWord(ChildStruct->hChildCodeMax), GetCurrentLineNumber(ChildStruct->hChildCodeMax)) == 0)
    {
        BufString = "No other use found for variable '" + (CStr) VarProcToSearchReal + (CStr) "'.";
        MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
    }
    else
    {
        LoadStructure(VarProcFoundhwnd);
        CMSelectWordInLine(ChildStruct->hChildCodeMax, VarProcFoundLine - 1, VarProcToSearchReal);
        SetFocus(VarProcFoundhwnd);
        SetFocus(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Edit/Goto/Procedure prototypes
void MCMD_ProcProto(void)
{
    CStr BufString;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    if(SearchProcProto(hMDIform.hClient, GetCurrentWord(ChildStruct->hChildCodeMax)) == 0)
    {
        BufString = "No declaration found for procedure '" + (CStr) VarProcToSearchReal + (CStr) "'.";
        MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
    }
    else
    {
        LoadStructure(VarProcFoundhwnd);
        CMSetCaretPos(ChildStruct->hChildCodeMax, VarProcFoundLine - 1, 0);
        SetFocus(VarProcFoundhwnd);
        SetFocus(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Edit/Goto/Procedure entry point
void MCMD_ProcDeclare(void)
{
    CStr BufString;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    if(SearchProcDeclare(hMDIform.hClient, GetCurrentWord(ChildStruct->hChildCodeMax)) == 0)
    {
        BufString = "No entry point found for procedure '" + (CStr) VarProcToSearchReal + (CStr) "'.";
        MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
    }
    else
    {
        LoadStructure(VarProcFoundhwnd);
        CMSetCaretPos(ChildStruct->hChildCodeMax, VarProcFoundLine - 1, 0);
        SetFocus(VarProcFoundhwnd);
        SetFocus(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Edit/Goto/Procedure next use
void MCMD_ProcNextUse(void)
{
    CStr BufString;
    
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    if(SearchProcNextUse(hMDIform.hClient, GetCurrentWord(ChildStruct->hChildCodeMax), GetCurrentLineNumber(ChildStruct->hChildCodeMax)) == 0)
    {
        BufString = "No other use found for procedure '" + (CStr) VarProcToSearchReal + (CStr) "'.";
        MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
    }
    else
    {
        LoadStructure(VarProcFoundhwnd);
        CMSelectWordInLine(ChildStruct->hChildCodeMax, VarProcFoundLine - 1, VarProcToSearchReal);
        SetFocus(VarProcFoundhwnd);
        SetFocus(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Edit/Block/Indent
void MCMD_BlockIndent(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    // Single line selected
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetLineCount(ChildStruct->hChildCodeMax) == 1)
    {
        CMSetCaretPos(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine, 0);
    }
    else
    {
        if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) != 0) if(CodeMaxCurRange.posEnd.nLine == CodeMaxCurRange.posStart.nLine) CMSetCaretPos(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine, 0);
    }
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_INDENTSELECTION, 0);
    if(CM_GetLineCount(ChildStruct->hChildCodeMax) == 1) CMSetCaretPos(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine, 0);
}

// -----------------------------------------------------------------------
// Edit/Block/UnIndent
void MCMD_BlockUnindent(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetLineCount(ChildStruct->hChildCodeMax) == 1)
    {
        CMSetCaretPos(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine, 0);
    }
    else
    {
        if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) != 0) if(CodeMaxCurRange.posEnd.nLine == CodeMaxCurRange.posStart.nLine) CMSetCaretPos(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine, 0);
    }
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_UNINDENTSELECTION, 0);
    if(CM_GetLineCount(ChildStruct->hChildCodeMax) == 1) CMSetCaretPos(ChildStruct->hChildCodeMax, CodeMaxCurRange.posStart.nLine, 0);
}

// -----------------------------------------------------------------------
// Edit/Block/Upper case
void MCMD_BlockUpperCase(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_UPPERCASESELECTION, 0);
}

// -----------------------------------------------------------------------
// Edit/Block/Lower case
void MCMD_BlockLowerCase(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_LOWERCASESELECTION, 0);
}

// -----------------------------------------------------------------------
// Edit/Block/Toggle case
void MCMD_BlockToggleCase(void)
{
    long SelToggleText = 0;
    long SelToggleTextLen = 0;
    
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_GetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
    SelToggleTextLen = CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
    if(SelToggleTextLen == 0) return;
    SelToggleText = AllocMem(SelToggleTextLen + 1);
    if(SelToggleText == 0) return;
    CM_GetTextLng(ChildStruct->hChildCodeMax, SelToggleText, &CodeMaxCurRange);
    ToggleCase((char *) SelToggleText, SelToggleTextLen);
    CM_ReplaceSelLng(ChildStruct->hChildCodeMax, SelToggleText);
    FreeMem(SelToggleText);
}

// -----------------------------------------------------------------------
// Edit/Block/Comment
void MCMD_BlockComment(void)
{
    CStr TxtComment;
    CStr TxtCommentDest;
    long *CommentArray = 0;
    int i = 0;
    long AddedEnter = 0;
    CStr CurLangComment;

    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) == 0) return;
    CurLangComment = GetCMLangComment(CurrentForm);
    if(CurLangComment.Len() == 0) return;
    if(CM_GetLineCount(ChildStruct->hChildCodeMax) == 1)
    {
        CodeMaxCurRange.posStart.nCol = 0;
        CodeMaxCurRange.posEnd.nCol = 0;
        CM_SetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
        CM_InsertText(ChildStruct->hChildCodeMax, CurLangComment.Get_String(), &CodeMaxCurRange);
        return;
    }
    TxtComment = TxtComment.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
    CM_GetText(ChildStruct->hChildCodeMax, TxtComment.Get_String(), &CodeMaxCurRange);
    AddedEnter = 0;
    if(strcmp(TxtComment.Right(2).Get_String(), "\r\n") != 0)
    {
        TxtComment = TxtComment + (CStr) "\r\n";
        AddedEnter = 1;
    }
    CommentArray = StringSplit(TxtComment, "\r\n");
    for(i = 0; i <= StringGetSplitUBound(CommentArray) - 1; i++)
    {
        TxtCommentDest = TxtCommentDest + (CStr) CurLangComment + (CStr) StringGetSplitElement(TxtComment, CommentArray, i).Get_String() + (CStr) "\r\n";
    }
    StringReleaseSplit(CommentArray);
    if(AddedEnter == 1) TxtCommentDest = (CStr) TxtCommentDest.Mid(1, TxtCommentDest.Len() - 2).Get_String();
    CM_ReplaceSel(ChildStruct->hChildCodeMax, TxtCommentDest.Get_String());
}

// -----------------------------------------------------------------------
// Edit/Block/UnComment
void MCMD_BlockUnComment(void)
{
    CStr TxtUnComment;
    CStr TxtUnCommentDest;
    long *CommentArray = 0;
    int i = 0;
    long AddedEnter = 0;
    CStr CurLangComment;
    CStr BufString;

    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) == 0) return;
    CurLangComment = GetCMLangComment(CurrentForm);
    if(CurLangComment.Len() == 0) return;
    TxtUnComment = TxtUnComment.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
    CM_GetText(ChildStruct->hChildCodeMax, TxtUnComment.Get_String(), &CodeMaxCurRange);
    AddedEnter = 0;
    if(strcmp(TxtUnComment.Right(2).Get_String(), "\r\n") != 0)
    {
        TxtUnComment = TxtUnComment + (CStr) "\r\n";
        AddedEnter = 1;
    }
    CommentArray = StringSplit(TxtUnComment, "\r\n");
    for(i = 0; i <= StringGetSplitUBound(CommentArray) - 1; i++)
    {
        BufString = StringReplace(StringGetSplitElement(TxtUnComment, CommentArray, i).Get_String(), "\t", " ", 1, -1, Binary_Compare).Trim();
        BufString = BufString.Left(CurLangComment.Len());
        if(BufString == CurLangComment)
        {
            TxtUnCommentDest = TxtUnCommentDest + (CStr) StringReplace(StringGetSplitElement(TxtUnComment, CommentArray, i), CurLangComment, "", 1, 1, Binary_Compare).Get_String() + (CStr) "\r\n";
        }
        else
        {
            TxtUnCommentDest = TxtUnCommentDest + (CStr) StringGetSplitElement(TxtUnComment, CommentArray, i).Get_String() + (CStr) "\r\n";
        }
    }
    StringReleaseSplit(CommentArray);
    if(AddedEnter == 1) TxtUnCommentDest = (CStr) TxtUnCommentDest.Mid(1, TxtUnCommentDest.Len() - 2).Get_String();
    CM_ReplaceSel(ChildStruct->hChildCodeMax, TxtUnCommentDest.Get_String());
}

// -----------------------------------------------------------------------
// Edit/Block/Tabify
void MCMD_BlockTabify(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_TABIFYSELECTION, 0);
}

// -----------------------------------------------------------------------
// Edit/Block/UnTabify
void MCMD_BlockUnTabify(void)
{
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_ExecuteCmd(ChildStruct->hChildCodeMax, CMD_UNTABIFYSELECTION, 0);
}

// -----------------------------------------------------------------------
// Edit/Block/Spaces to tabs
void MCMD_BlockSpacesToTabs(void)
{
    long SelToggleText = 0;
    long SelToggleTextLen = 0;

    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    CM_GetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
    SelToggleTextLen = CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
    if(SelToggleTextLen == 0) return;
    SelToggleText = AllocMem(SelToggleTextLen + 1);
    if(SelToggleText == 0) return;
    CM_GetTextLng(ChildStruct->hChildCodeMax, SelToggleText, &CodeMaxCurRange);
    SpaceToTab((char *) SelToggleText, SelToggleTextLen);
    CM_ReplaceSelLng(ChildStruct->hChildCodeMax, SelToggleText);
    FreeMem(SelToggleText);
}

// -----------------------------------------------------------------------
// Edit/Block/Registers
void MCMD_Registers(void)
{
    CStr LangRegisters;
    long LnToStart = 0;
    CStr CurComment = 0;
    int i = 0;
    int j = 0;
    long FoundAny = 0;
    CStr LineToCheck;
    CStr CheckInLine;
    long ValidReg = 0;
    long *RegsArray = 0;
    long *SelLines = 0;
    char *LeftValidChars[5 + 1];
    char *RightValidChars[7 + 1];
    long PosRegister = 0;
    CStr LineToCheckTrim;
    CM_RANGE NormRange;
    long FoundPosI;
    long FoundPosJ;

    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    WriteComment("Searching...");
    CursorSetWait();
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) != 0)
    {
        if(FrmRegsOn == 1) ControlClose(FrmRegshWnd);
        FrmRegsFoundFile = CMGetRealFile(ChildStruct->RFile);
        LangRegisters = GetCMLangC(CurrentForm);
        if(LangRegisters.Len() == 0)
        {
            CursorSetNormal();
            WriteComment("");
            MiscMsgBox(hMDIform.hWnd, "Can't find registers datas for current document.", MB_INFORMATION, Requesters);
            return;
        }
        // --- 1.Remove comments
        // Get registers from syntax highlighting
        RegsArray = StringSplit(LangRegisters, "\n");
        // Parse selected text into lines
        LoadCurrentSel(ChildStruct->hChildCodeMax);
        if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) != 0)
        {
            FrmRegsCmTextReg = FrmRegsCmTextReg.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
            CM_GetText(ChildStruct->hChildCodeMax, FrmRegsCmTextReg.Get_String(), &CodeMaxCurRange);
            // Text to display
            FrmRegsPrimLinesTxt = FrmRegsCmTextReg;
            FrmRegsFoundLinesTxt = StringSplit(FrmRegsPrimLinesTxt, "\n");
            // Text to search
            RemoveCommentBlocksFromFileByChildStr(FrmRegsCmTextReg, FrmRegsCmTextReg.Len(), ChildStruct->hChildCodeMax);
            RemoveSingleLineComment(FrmRegsCmTextReg.Get_String(), FrmRegsCmTextReg.Len(), GetCMLangComment(CurrentForm).Get_String());
            RemoveQuotes(FrmRegsCmTextReg.Get_String(), FrmRegsCmTextReg.Len());
            TabToSpace(FrmRegsCmTextReg.Get_String(), FrmRegsCmTextReg.Len());
            SelLines = StringSplit(FrmRegsCmTextReg, "\n");
            FrmRegsSelFirstLine = CodeMaxCurRange.posStart.nLine;
        }
        else
        {
            FrmRegsCmTextReg = FrmRegsCmTextReg.String(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1), 1);
            CM_GetTextAll(ChildStruct->hChildCodeMax, FrmRegsCmTextReg.Get_String());
            // Text to display
            FrmRegsPrimLinesTxt = FrmRegsCmTextReg;
            FrmRegsFoundLinesTxt = StringSplit(FrmRegsPrimLinesTxt, "\n");
            // Text to search
            RemoveCommentBlocksFromFileByChildStr(FrmRegsCmTextReg, FrmRegsCmTextReg.Len(), ChildStruct->hChildCodeMax);
            RemoveSingleLineComment(FrmRegsCmTextReg.Get_String(), FrmRegsCmTextReg.Len(), GetCMLangComment(CurrentForm).Get_String());
            RemoveQuotes(FrmRegsCmTextReg.Get_String(), FrmRegsCmTextReg.Len());
            TabToSpace(FrmRegsCmTextReg.Get_String(), FrmRegsCmTextReg.Len());
            SelLines = StringSplit(FrmRegsCmTextReg, "\n");
            FrmRegsSelFirstLine = 0;
        }
        
        // --- 2.Search registers
        FoundPos.Erase();
        FoundLines.Erase();
        FoundReg.Erase();
        MiscDoEvents(hMDIform.hClient, hGlobAccelerators, hMDIform.hWnd);
        
        CurComment = GetCMLangComment(CurrentForm);
        LnToStart = 0;
        for(j = 0; j <= StringGetSplitUBound(SelLines); j++)
        {
            LineToCheck = StringGetSplitElement(FrmRegsCmTextReg, SelLines, j);
            if(_strcmpi(LineToCheck.Trim().Left(CurComment.Len()).Get_String(), CurComment.Get_String()) == 0) LnToStart++;
        }
        MiscDoEvents(hMDIform.hClient, hGlobAccelerators, hMDIform.hWnd);
        for(i = 0; i <= StringGetSplitUBound(RegsArray); i++)
        {
            for(j = LnToStart; j <= StringGetSplitUBound(SelLines); j++)
            {
                LineToCheck = StringGetSplitElement(FrmRegsCmTextReg, SelLines, j);
                if(LineToCheck.Len() != 0)
                {
                    LineToCheckTrim = StringGetSplitElement(LangRegisters, RegsArray, i).Trim();
                    PosRegister = LineToCheck.In_Str(1, LineToCheckTrim, Text_Compare);
                    if(PosRegister != 0)
                    {
                        FoundPos.Add(PosRegister);
                        // Store related code
                        FoundLines.Add(j + FrmRegsSelFirstLine);
                        // Store ref. register
                        FoundReg.Add(LineToCheckTrim.Get_String());
                    }
                }
            }
        }

        // --- 3.Remove entries with 1 char of difference
        // (Just to avoid things like eax/ax entries)
        for(i = 0; i < FoundPos.Amount(); i++)
        {
            FoundPosI = FoundPos.Get(i)->Content;
            if(FoundPosI != -1)
            {
                for(j = 0; j < FoundPos.Amount(); j++)
                {
                    FoundPosJ = FoundPos.Get(j)->Content;
                    if(FoundPosJ != -1)
                    {
                        if((FoundPosJ - 1) == FoundPosI)
                        {
                            // Same line ?
                            if(FoundLines.Get(j)->Content == FoundLines.Get(i)->Content)
                            {
                                if(strcmp(FoundReg.Get(j)->Content,
                                          FoundReg.Get(i)->Content) == 0)
                                {
                                    FoundLines.Set(j, -1L);
                                    FoundPos.Set(j, -1L);
                                    FoundReg.Set(j, "");
                                }
                            }
                        }
                    }
                }
            }
        }

        // Array of valid chars
        LeftValidChars[0] = " ";
        LeftValidChars[1] = ",";
        LeftValidChars[2] = "[";
        LeftValidChars[3] = ":";
        LeftValidChars[4] = "(";
        LeftValidChars[5] = "+";
        RightValidChars[0] = "\r";
        RightValidChars[1] = " ";
        RightValidChars[2] = ",";
        RightValidChars[3] = "]";
        RightValidChars[4] = ":";
        RightValidChars[5] = ")";
        RightValidChars[6] = "=";
        RightValidChars[7] = "+";

        // --- 4.Check bounds for valid chars in found entries
        for(i = 0; i < FoundPos.Amount(); i++)
        {
            if(FoundPos.Get(i)->Content != -1)
            {
                CheckInLine = StringGetSplitElement(FrmRegsCmTextReg, SelLines, FoundLines.Get(i)->Content - FrmRegsSelFirstLine);
                // Check Left chars
                ValidReg = 1;
                if(FoundPos.Get(i)->Content > 1)
                {
                    ValidReg = 0;
                    for(j = 0; j <= 5; j++)
                    {
                        if(_strcmpi(CheckInLine.Mid(FoundPos.Get(i)->Content - 1, 1).Get_String(), LeftValidChars[j]) == 0)
                        {
                            ValidReg = 1;
                            break;
                        }
                    }
                }
                for(j = 0; j <= 7; j++)
                {
                    if(_strcmpi(CheckInLine.Mid(FoundPos.Get(i)->Content + strlen(FoundReg.Get(i)->Content), 1).Get_String(),
                               RightValidChars[j]) == 0)
                    {
                        ValidReg = ValidReg | 2;
                        break;
                    }
                }
                // Remove the ref. now
                if(ValidReg != 3)
                {
                    FoundLines.Set(i, -1L);
                    FoundPos.Set(i, -1L);
                    FoundReg.Set(i, "");
                }
            }
        }
        FoundAny = 0;
        for(i = 0; i < FoundPos.Amount(); i++)
        {
            if(FoundPos.Get(i)->Content != -1)
            {
                FoundAny = 1;
                break;
            }
        }
        StringReleaseSplit(SelLines);
        StringReleaseSplit(RegsArray);
        if(FoundAny == 0)
        {
            MiscMsgBox(hMDIform.hWnd, "No registers found in selection.", MB_INFORMATION, Requesters);
            return;
        }
        else
        {
            // Clear selection
            NormRange.posEnd.nCol = 0;
            NormRange.posStart.nCol = 0;
            NormRange.posStart.nLine = CodeMaxCurRange.posStart.nLine;
            NormRange.posEnd.nLine = CodeMaxCurRange.posStart.nLine;
            CM_SetSel(ChildStruct->hChildCodeMax, &NormRange, 1);
            FrmRegshWnd = CreateNonModalDialog(-1, -1, 552, 215, hMDIform.hWnd, 0, LoadIcon(ApphInstance, MAKEINTRESOURCE(ICON_FIND + ICON_BASE)),
                                               "Used registers", &FrmRegsInitProc, &FrmRegsWinHook, 0, WS_SYSMENU | WS_SIZEBOX | WS_MINIMIZEBOX, SW_SHOW);
        }
    }
    CursorSetNormal();
    WriteComment("");
}

// -----------------------------------------------------------------------
// Edit/Block/Comment (multiline)
void MCMD_BlockCommentMultiLine(void)
{
    CStr TxtComment;
    CStr CurLangCommentSingle;
    CStr CurLangComment;
    CStr CurLangCommentEnd;

    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) == 0) return;
    CurLangCommentSingle = GetCMLangComment(CurrentForm);
    CurLangComment = GetCMLangMultiComment(CurrentForm);
    CurLangCommentEnd = GetCMLangMultiCommentEnd(CurrentForm);
    if(strlen(CurLangComment.Get_String()) == 0) return;
    if(strlen(CurLangCommentEnd.Get_String()) == 0) return;
    if(CM_GetLineCount(ChildStruct->hChildCodeMax) == 1)
    {
        CodeMaxCurRange.posStart.nCol = 0;
        CodeMaxCurRange.posEnd.nCol = 0;
        CM_SetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
        CM_InsertText(ChildStruct->hChildCodeMax, CurLangCommentSingle.Get_String(), &CodeMaxCurRange);
        return;
    }
    TxtComment = TxtComment.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
    CM_GetText(ChildStruct->hChildCodeMax, TxtComment.Get_String(), &CodeMaxCurRange);
    TxtComment = CurLangComment + (CStr) TxtComment;
    if(strcmp(TxtComment.Right(2).Get_String(), "\r\n") != 0) TxtComment = TxtComment + (CStr) "\r\n";
    TxtComment = TxtComment + (CStr) CurLangCommentEnd;
    CM_ReplaceSel(ChildStruct->hChildCodeMax, TxtComment.Get_String());
}

// -----------------------------------------------------------------------
// Edit/Block/Custom Comment
void MCMD_CustomBlockComment(void)
{
    CStr TxtComment;
    CStr TxtCommentDest;
    long *CommentArray = 0;
    int i = 0;
    long AddedEnter = 0;
    CStr CurLangComment;

    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) == 0) return;
    EnteredCustComment = MiscInputBox(hMDIform.hWnd, "Enter custom comment text", ICON_KEYBOARD, EnteredCustComment, 0, INPUTBOX_COMBO, "CustomComments");
    if(EnteredCustComment.Len() == 0) return;
    CurLangComment = StringReplace(EnteredCustComment, "{TAB}", "\t", 1, -1, Text_Compare);
    CurLangComment = StringReplace(CurLangComment, "{NL}", "\r\n", 1, -1, Text_Compare);
    if(CurLangComment.Len() == 0) return;
    if(CM_GetLineCount(ChildStruct->hChildCodeMax) == 1)
    {
        CodeMaxCurRange.posStart.nCol = 0;
        CodeMaxCurRange.posEnd.nCol = 0;
        CM_SetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
        CM_InsertText(ChildStruct->hChildCodeMax, CurLangComment.Get_String(), &CodeMaxCurRange);
        return;
    }
    TxtComment = TxtComment.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
    CM_GetText(ChildStruct->hChildCodeMax, TxtComment.Get_String(), &CodeMaxCurRange);
    AddedEnter = 0;
    if(strcmp(TxtComment.Right(2).Get_String(), "\r\n") != 0)
    {
        TxtComment = TxtComment + (CStr) "\r\n";
        AddedEnter = 1;
    }
    CommentArray = StringSplit(TxtComment, "\r\n");
    for(i = 0; i <= (long) StringGetSplitUBound(CommentArray) - 1; i++)
    {
        TxtCommentDest = TxtCommentDest + (CStr) CurLangComment + (CStr) StringGetSplitElement(TxtComment, CommentArray, i).Get_String() + (CStr) "\r\n";
    }
    StringReleaseSplit(CommentArray);
    if(AddedEnter == 1) TxtCommentDest = (CStr) TxtCommentDest.Mid(1, TxtCommentDest.Len() - 2).Get_String();
    CM_ReplaceSel(ChildStruct->hChildCodeMax, TxtCommentDest.Get_String());
}

// -----------------------------------------------------------------------
// Edit/Block/trim spaces
void MCMD_BlockTrimSpaces(void)
{
    CStr TxtComment;
    CStr TxtCommentDest;
    long *CommentArray = 0;
    int i = 0;
    long AddedEnter = 0;
    
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) == 0) return;
    TxtComment = TxtComment.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
    if(CM_GetLineCount(ChildStruct->hChildCodeMax) == 1)
    {
        CM_SetSel(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1);
        CM_GetText(ChildStruct->hChildCodeMax, TxtComment.Get_String(), &CodeMaxCurRange);
        TxtComment = TxtComment.R_Trim();
        CM_ReplaceSel(ChildStruct->hChildCodeMax, TxtComment.Get_String());
        return;
    }
    CM_GetText(ChildStruct->hChildCodeMax, TxtComment.Get_String(), &CodeMaxCurRange);
    AddedEnter = 0;
    if(strcmp(TxtComment.Right(2).Get_String(), "\r\n") != 0)
    {
        TxtComment = TxtComment + (CStr) "\r\n";
        AddedEnter = 1;
    }
    CommentArray = StringSplit(TxtComment, "\r\n");
    for(i = 0; i <= StringGetSplitUBound(CommentArray) - 1; i++)
    {
        TxtCommentDest = TxtCommentDest + (CStr) StringGetSplitElement(TxtComment, CommentArray, i).R_Trim() + (CStr) "\r\n";
    }
    StringReleaseSplit(CommentArray);
    if(AddedEnter == 1) TxtCommentDest = (CStr) TxtCommentDest.Mid(1, TxtCommentDest.Len() - 2).Get_String();
    CM_ReplaceSel(ChildStruct->hChildCodeMax, TxtCommentDest.Get_String());
}

// -----------------------------------------------------------------------
// Edit/Block/Convert to include
void MCMD_BlockConvertToInclude(void)
{
    CStr FNameBlock;
    CStr FName;
    CStr Filters;
    CStr IncludeSkew;
    CStr FileNametoCompare;
    CStr FileNametoCompare2;
    CStr BufString;
    int i;
    
    if(NbForms == 0) return;
    if(IsChildReadOnly(CurrentForm) == 1) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) == 0) return;
    IncludeSkew = GetCMLangInclude(CurrentForm);
    if(IncludeSkew.Len() == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "No Include definitions found for current language.", MB_ERROR, Requesters);
        return;
    }
    Filters = "";
    for(i = 0; i < RegisteredExts.Amount(); i++)
    {
        Filters = Filters + (CStr) RegisteredExts.Get(i)->Content + (CStr) "|";
    }
    if(Filters.Len() == 0) Filters = "All files (*.*)|*.*";
    if(GetUseFileDir() == 1)
    {
        FNameBlock = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, FileGetDirectory(ChildStruct->RFile), CurrentDir);
    }
    else
    {
        FNameBlock = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, LastConvertBlockDir, CurrentDir);
    }
    if(FNameBlock.Len() == 0) return;
    LastConvertBlockDir = FileGetDirectory(FNameBlock);
    DeleteFile(FNameBlock.Get_String());
    if(_strcmpi(GetCMLangDoubleSlash(CurrentForm).Get_String(), "1") == 0) FNameBlock = StringReplace(FNameBlock, "\\", "\\\\", 1, -1, Binary_Compare);
    IncludeSkew = StringReplace(IncludeSkew, "%1", FNameBlock, 1, -1, Binary_Compare);
    SelToSave = SelToSave.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
    CM_GetText(ChildStruct->hChildCodeMax, SelToSave.Get_String(), &CodeMaxCurRange);
    if(MSaveFile(FNameBlock.Get_String(), (long) SelToSave.Get_String(), (long) SelToSave.Len()) == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Can't save selection.", MB_ERROR, Requesters);
    }
    else
    {
        FileNametoCompare = FileGetDirectory(CMGetRealFile(ChildStruct->RFile));
        FileNametoCompare2 = FileGetDirectory(FNameBlock);
        // Remove directory if equal
        if(_strcmpi(FileNametoCompare.Get_String(), FileNametoCompare2.Get_String()) == 0)
        {
            IncludeSkew = StringReplace(IncludeSkew, FileNametoCompare, "", 1, -1, Binary_Compare);
        }
        BufString = IncludeSkew + (CStr) "\r\n";
        CM_ReplaceSel(ChildStruct->hChildCodeMax, BufString.Get_String());
        RemCurrentSel(ChildStruct->hChildCodeMax);
    }
}

// -----------------------------------------------------------------------
// Edit/Block/Save as
void MCMD_BlockSave(void)
{
    CStr FNameBlock;
    CStr FName;
    CStr Filters;
    int i;
    
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(CurrentForm);
    if(CM_GetTextLengthAll(ChildStruct->hChildCodeMax, 1) == 0) return;
    LoadCurrentSel(ChildStruct->hChildCodeMax);
    if(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1) == 0) return;
    Filters = "";
    for(i = 0; i < RegisteredExts.Amount(); i++)
    {
        Filters = Filters + (CStr) RegisteredExts.Get(i)->Content + (CStr) "|";
    }
    if(Filters.Len() == 0) Filters = "All files (*.*)|*.*";
    if(GetUseFileDir() == 1)
    {
        FNameBlock = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, FileGetDirectory(ChildStruct->RFile), CurrentDir);
    }
    else
    {
        FNameBlock = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, LastSaveBlockDir, CurrentDir);
    }
    if(FNameBlock.Len() == 0) return;
    LastSaveBlockDir = FileGetDirectory(FNameBlock);
    DeleteFile(FNameBlock.Get_String());
    SelToSave = SelToSave.String(CM_GetTextLength(ChildStruct->hChildCodeMax, &CodeMaxCurRange, 1), 1);
    CM_GetText(ChildStruct->hChildCodeMax, SelToSave.Get_String(), &CodeMaxCurRange);
    if(MSaveFile(FNameBlock.Get_String(), (long) SelToSave.Get_String(), SelToSave.Len()) == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Can't save selection.", MB_ERROR, Requesters);
    }
    else
    {
        SaveBookmarks(CurrentForm, FNameBlock, CodeMaxCurRange.posStart.nLine, CodeMaxCurRange.posEnd.nLine - CodeMaxCurRange.posStart.nLine);
    }
}

// -----------------------------------------------------------------------
// Edit/Block/Print
void MCMD_BlockPrint(void)
{
    if(NbForms != 0) PrintFile(CurrentForm, 1);
}

// -----------------------------------------------------------------------
// View/Statusbar
void MCMD_Statusbar(void)
{
    ShowStatusbar = ShowStatusbar ^ 1;
    ShowHideStatusBar(ShowStatusbar);
}

// -----------------------------------------------------------------------
// View/Display/Project
void MCMD_ProjectDock(void)
{
    ShowProjetDockWin = ShowProjetDockWin ^ 1;
    SetProjectDockState(ShowProjetDockWin);
}

// -----------------------------------------------------------------------
// View/Display/Windows
void MCMD_WindowsDock(void)
{
    ShowWindowsDockWin = ShowWindowsDockWin ^ 1;
    SetWindowsDockState(ShowWindowsDockWin);
}

// -----------------------------------------------------------------------
// View/Display/Output
void MCMD_OutputDock(void)
{
    ShowOutputDockWin = ShowOutputDockWin ^ 1;
    SetOutputDockState(ShowOutputDockWin);
}

// -----------------------------------------------------------------------
// View/Display/Debug
void MCMD_DebugDock(void)
{
    ShowDebugDockWin = ShowDebugDockWin ^ 1;
    SetDebugDockState(ShowDebugDockWin);
}

// -----------------------------------------------------------------------
// View/Display/Registers
void MCMD_RegistersDock(void)
{
    ShowRegistersDockWin = ShowRegistersDockWin ^ 1;
    SetRegistersDockState(ShowRegistersDockWin);
}

// -----------------------------------------------------------------------
// View/Full screen
void MCMD_FullScreen(void)
{
    FullScreenMode = FullScreenMode ^ 1;
    SetFullScreenMode();
}

void SetFullScreenMode(void)
{
    long CurrentWindowStyle = 0;
    
    if(FullScreenMode == 0)
    {
        IniWriteKey("Layout", "FullScreen", "0", MainIniFile);
        CheckMenuItem(hViewMenu, 2, MF_BYPOSITION | MF_UNCHECKED);
        CurrentWindowStyle = GetWindowLong(hMDIform.hWnd, GWL_STYLE);
        CurrentWindowStyle = CurrentWindowStyle | WS_CAPTION | WS_SYSMENU | WS_SIZEBOX | WS_BORDER;
        SetWindowLong(hMDIform.hWnd, GWL_STYLE, CurrentWindowStyle);
        MenuEnable(hViewMenu, MENU_VIEW_STATUSBAR_ID, 1);
        MenuEnable(hViewMenu, 7, 1);
        ControlVisible(hRebar, 1);
        RebarResize(hRebar);
        ShowHideStatusBar(ShowStatusbar);
        MoveWindow(hMDIform.hWnd, XPosToRestore, YPosToRestore, XToRestore, YToRestore, 1);
    }
    else
    {
        if((GetWindowLong(hMDIform.hWnd, GWL_STYLE) & WS_MAXIMIZE) != 0)
        {
            XPosToRestore = 0;
            YPosToRestore = 0;
            XToRestore = GetSystemMetrics(SM_CXMAXIMIZED);
            YToRestore = GetSystemMetrics(SM_CYMAXIMIZED);
        }
        else
        {
            XPosToRestore = ControlLeft(hMDIform.hWnd);
            YPosToRestore = ControlTop(hMDIform.hWnd);
            XToRestore = ControlWidth(hMDIform.hWnd);
            YToRestore = ControlHeight(hMDIform.hWnd);
        }
        IniWriteKey("Layout", "FullScreen", "1", MainIniFile);
        CheckMenuItem(hViewMenu, 2, MF_BYPOSITION | MF_CHECKED);
        CurrentWindowStyle = GetWindowLong(hMDIform.hWnd, GWL_STYLE);
        CurrentWindowStyle = CurrentWindowStyle & ~(WS_CAPTION);
        CurrentWindowStyle = CurrentWindowStyle & ~(WS_SYSMENU);
        CurrentWindowStyle = CurrentWindowStyle & ~(WS_SIZEBOX);
        CurrentWindowStyle = CurrentWindowStyle & ~(WS_BORDER);
        SetWindowLong(hMDIform.hWnd, GWL_STYLE, CurrentWindowStyle);
        MenuEnable(hViewMenu, MENU_VIEW_STATUSBAR_ID, 0);
        MenuEnable(hViewMenu, 7, 0);
        ControlVisible(hRebar, 0);
        RebarResize(hRebar);
        ControlVisible(hStatusBar, 0);
        ResizeMDIform();
        MoveWindow(hMDIform.hWnd, 0, 0, ScreenWidth(), ScreenHeight(), 1);
    }
}

// -----------------------------------------------------------------------
// View/Properties
void MCMD_Properties(void)
{
    // Open with first tab
    FRMPropertiesFirstTab = 0;
    DisplayProperties();
}

// -----------------------------------------------------------------------
// View/User menus editor
void MCMD_UserMenusEd(void)
{
    CreateModalDialog(-1, -1, 530, 366, hMDIform.hWnd, &FRMUserMenusProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// View/Tolbars manager
void MCMD_ToolbarsManager(void)
{
    CreateModalDialog(-1, -1, 359, 301, hMDIform.hWnd, &FRMToolbarsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Project/Create new project
void MCMD_CreateProject(void)
{
    CreateModalDialog(-1, -1, 490, 326, hMDIform.hWnd, &FRMCreateProjectProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    switch(ProjectCreate)
    {
        case 1:
            CreateProject(ProjectGather);
            break;
        case 2:
            OpenUnknownFile(&FRMCreateProjectSelRecent, TRUE);
            break;
    }
}

// -----------------------------------------------------------------------
// Project/Open project
void MCMD_OpenProject(void)
{
    CStr OpFilters;
    CStr LdFile;
    
    OpFilters = AppTitle + (CStr) " project files (*.Med)|*.Med";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, Dirs[DIR_PROJECTS], 0, CurrentDir);
    if(LdFile.Len() != 0) OpenProjectAuto(LdFile);
}

// -----------------------------------------------------------------------
// Project/Save project
void MCMD_SaveProject(void)
{
    if(ProjectOn == 0) return;
    SaveProject();
}

// -----------------------------------------------------------------------
// Project/Add include
void MCMD_AddInclude(void)
{
    CStr LdFile;
    CStr OpFilters;
    
    if(ProjectOn == 0) return;
    OpFilters = IniReadKey("ExtrasExt", "AddInclude", MainIniFile);
    if(OpFilters.Len() == 0)
    {
        OpFilters = "Source files|*.inc;*.a;*.asm;*.ash;*.asi;*.s;*.a86;*.c;*.cc;*.cpp;*.i;*.h;*.mac;*.hpp;*.pp;*.pas;*.bas|All files|*.*";
    }
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, Dirs[DIR_INCLUDE], 1, CurrentDir);
    if(LdFile.Len() != 0)
    {
        if(MiscMsgBox(hMDIform.hWnd, "Add the corresponding libraries file ?", MB_QUESTION, Requesters) == IDNO)
        {
            ComDlgParseMultiFilesSelection(LdFile, &EnumAddInclude, MULTIFILESENUM_FORWARD, 0);
        }
        else
        {
            ComDlgParseMultiFilesSelection(LdFile, &EnumAddInclude, MULTIFILESENUM_FORWARD, 1);
        }
    }
}

long CALLBACK EnumAddInclude(char *FileToAdd, long UserValue)
{
    CStr LbFile;
    CStr LbFileExt;

    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewIncludes, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    if(UserValue == 0)
    {
        TreeViewAddItem(hTreeView, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewIncludes, 0, ICON_NEW, ICON_NEW, 0, 1);
    }
    else
    {
        TreeViewAddItem(hTreeView, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewIncludes, 0, ICON_NEW, ICON_NEW, 0, 1);
        LbFile = Dirs[DIR_LIB] + (CStr) "\\" + (CStr) FileGetFileName(FileToAdd).Get_String();
        // Test with a .lib
        LbFileExt = FileReplaceExtension(LbFile, "lib");
        if(FileExist(LbFileExt) == 0)
        {
            // Test with a *32.lib
            LbFileExt = FileRemoveExtension(LbFile) + "32.lib";
            if(FileExist(LbFileExt) == 0)
            {
                // Test with a lib*.a
                LbFileExt = "lib" + FileReplaceExtension(LbFile, "a");
                // Test with a lib*32.a
                if(FileExist(LbFileExt) == 0)
                {
                    LbFileExt = "lib" + FileRemoveExtension(LbFile) + "32.a";
                }
            }
        }
        if(FileExist(LbFileExt) != 0)
        {
            if(TreeViewSearchItemText(hTreeView, hTreeViewLibs, FileGetFileName(LbFileExt).Get_String() + (CStr) " (" + (CStr) LbFileExt + (CStr) ")") != -1)
            {
                WriteToStatus("File '" + (CStr) LbFileExt + (CStr) "' already referenced in project.");
                return(1);
            }
            TreeViewAddItem(hTreeView, FileGetFileName(LbFileExt).Get_String() + (CStr) " (" + (CStr) LbFileExt + (CStr) ")", hTreeViewLibs, 0, ICON_STATIC, ICON_STATIC, 0, 1);
        }
        else
        {
            WriteToStatus("Library for file '" + (CStr) FileGetFileName(FileToAdd).Get_String() + (CStr) "' not found.");
        }
    }
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add Current file to includes
void MCMD_AddCurrentInclude(void)
{
    CStr LdFile;

    if(ProjectOn == 0) return;
    if(NbForms == 0) return;
    LdFile = SaveIt(CurrentForm);
    if(LdFile.Len() == 0) return;
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewIncludes, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")") != -1)
    {
        MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) LdFile + (CStr) "' already referenced in project.", MB_ERROR, Requesters);
        return;
    }
    if(TreeViewSearchItemText(hTreeView, hTreeViewModules, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")") != -1)
    {
        MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) LdFile + (CStr) "' already referenced in project.", MB_ERROR, Requesters);
        return;
    }
    TreeViewAddItem(hTreeView, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")", hTreeViewIncludes, 0, ICON_NEW, ICON_NEW, 0, 1);
    ProjectModified = TRUE;
}

// -----------------------------------------------------------------------
// Project/Add library
void MCMD_AddLibrary(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = IniReadKey("ExtrasExt", "AddLibrary", MainIniFile);
    if(OpFilters.Len() == 0) OpFilters = "Library files|*.lib;*.a;*.ppw;*.ow|All files|*.*";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, Dirs[DIR_LIB], 1, CurrentDir);
    ComDlgParseMultiFilesSelection(LdFile, &EnumAddLibrary, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddLibrary(char *FileToAdd, long UserValue)
{
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewLibs, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    TreeViewAddItem(hTreeView, FileGetFileName(FileToAdd) + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewLibs, 0, ICON_STATIC, ICON_STATIC, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add module
void MCMD_AddModule(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = IniReadKey("ExtrasExt", "AddModule", MainIniFile);
    if(OpFilters.Len() == 0) OpFilters = "Source files|*.a;*.asm;*.s;*.a86;*.c;*.cc;*.cpp;*.pas;*.pp;*.bas|All files|*.*";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, CurrentDir, 1, CurrentDir);
    ComDlgParseMultiFilesSelection(LdFile, &EnumAddModule, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddModule(char *FileToAdd, long UserValue)
{
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewModules, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    TreeViewAddItem(hTreeView, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewModules, 0, ICON_NEW, ICON_NEW, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add Current file to modules
void MCMD_AddCurrentModule(void)
{
    CStr LdFile;

    if(ProjectOn == 0) return;
    if(NbForms == 0) return;
    LdFile = SaveIt(CurrentForm);
    if(LdFile.Len() == 0) return;
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewModules, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")") != -1)
    {
        MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) LdFile + (CStr) "' already referenced in project.", MB_ERROR, Requesters);
        return;
    }
    if(TreeViewSearchItemText(hTreeView, hTreeViewIncludes, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")") != -1)
    {
        MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) LdFile + (CStr) "' already referenced in project.", MB_ERROR, Requesters);
        return;
    }
    TreeViewAddItem(hTreeView, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")", hTreeViewModules, 0, ICON_NEW, ICON_NEW, 0, 1);
    ProjectModified = TRUE;
}

// -----------------------------------------------------------------------
// Project/Add object
void MCMD_AddObject(void)
{
    CStr LdFile;
    CStr OpFilters;
    
    if(ProjectOn == 0) return;
    OpFilters = IniReadKey("ExtrasExt", "AddObject", MainIniFile);
    if(OpFilters.Len() == 0) OpFilters = "Object files|*.obj;*.o|All files|*.*";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, Dirs[DIR_LIB], 1, CurrentDir);
    ComDlgParseMultiFilesSelection(LdFile, &EnumAddObject, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddObject(char *FileToAdd, long UserValue)
{
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewObjects, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    TreeViewAddItem(hTreeView, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewObjects, 0, ICON_OBJECT, ICON_OBJECT, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add resource
void MCMD_AddResource(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = IniReadKey("ExtrasExt", "AddResource", MainIniFile);
    if(OpFilters.Len() == 0) OpFilters = "Resources files|*.rc|All files|*.*";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, CurrentDir, 1, CurrentDir);
    ComDlgParseMultiFilesSelection(LdFile, &EnumAddResource, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddResource(char *FileToAdd, long UserValue)
{
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewResources, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    TreeViewAddItem(hTreeView, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewResources, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add Current file to resources
void MCMD_AddCurrentRc(void)
{
    CStr LdFile;

    if(ProjectOn == 0) return;
    if(NbForms == 0) return;
    LdFile = SaveIt(CurrentForm);
    if(LdFile.Len() == 0) return;
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewResources, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")") != -1)
    {
        MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) LdFile + (CStr) "' already referenced in project.", MB_ERROR, Requesters);
        return;
    }
    TreeViewAddItem(hTreeView, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")", hTreeViewResources, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
}

// -----------------------------------------------------------------------
// Project/Add text
void MCMD_AddText(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = IniReadKey("ExtrasExt", "AddText", MainIniFile);
    if(OpFilters.Len() == 0) OpFilters = "Text files|*.txt|Ini files|*.ini|Batch files|*.bat|Log files|*.log|Registry files|*.reg|All files|*.*";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, CurrentDir, 1, CurrentDir);
    ComDlgParseMultiFilesSelection(LdFile, &EnumAddText, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddText(char *FileToAdd, long UserValue)
{
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewTexts, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    TreeViewAddItem(hTreeView, FileGetFileName(FileToAdd).Get_String() + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewTexts, 0, ICON_NEW, ICON_NEW, 0, 1);
    ProjectModified = TRUE;
    return(1);
}


// -----------------------------------------------------------------------
// Project/Add Current file to documents
void MCMD_AddCurrentText(void)
{
    CStr LdFile;

    if(ProjectOn == 0) return;
    if(NbForms == 0) return;
    LdFile = SaveIt(CurrentForm);
    if(LdFile.Len() == 0) return;
    // Add it to the treeview
    if(TreeViewSearchItemText(hTreeView, hTreeViewTexts, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")") != -1)
    {
        MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) LdFile + (CStr) "' already referenced in project.", MB_ERROR, Requesters);
        return;
    }
    TreeViewAddItem(hTreeView, FileGetFileName(LdFile).Get_String() + (CStr) " (" + (CStr) LdFile + (CStr) ")", hTreeViewTexts, 0, ICON_NEW, ICON_NEW, 0, 1);
    ProjectModified = TRUE;
}

// -----------------------------------------------------------------------
// Project/Add icon
void MCMD_AddIcon(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = "Icon files|*.ico";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastIconsDir, 1, CurrentDir);
    LastIconsDir = ComDlgParseMultiFilesSelection(LdFile, &EnumAddIcon, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddIcon(char *FileToAdd, long UserValue)
{
    long EntryCount = 0;

    // Add it to the treeview
    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewIcons, "(" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    EntryCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewIcons);
    AddIconInArray(15, 0);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewIcons, "ICON_" + (CStr) EntryCount) != -1)
    {
        EntryCount++;
    }
    TreeViewAddItem(hTreeView, "ICON_" + (CStr) EntryCount + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewIcons, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add cursor
void MCMD_AddCursor(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = "Cursor files|*.cur";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastCursorsDir, 1, CurrentDir);
    LastCursorsDir = ComDlgParseMultiFilesSelection(LdFile, &EnumAddCursor, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddCursor(char *FileToAdd, long UserValue)
{
    long EntryCount = 0;

    // Add it to the treeview
    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewCursors, "(" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    EntryCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewCursors);
    AddCursorInArray(15, 0);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewCursors, "CURSOR_" + (CStr) EntryCount) != -1)
    {
        EntryCount++;
    }
    TreeViewAddItem(hTreeView, "CURSOR_" + (CStr) EntryCount + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewCursors, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add bitmap
void MCMD_AddBitmap(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = "Bitmap files|*.bmp";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastBitmapsDir, 1, CurrentDir);
    LastBitmapsDir = ComDlgParseMultiFilesSelection(LdFile, &EnumAddBitmap, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddBitmap(char *FileToAdd, long UserValue)
{
    long EntryCount = 0;

    // Add it to the treeview
    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewBitmaps, "(" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    EntryCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewBitmaps);
    AddBitmapInArray(15, 0);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewBitmaps, "BITMAP_" + (CStr) EntryCount) != -1)
    {
        EntryCount++;
    }
    TreeViewAddItem(hTreeView, "BITMAP_" + (CStr) EntryCount + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewBitmaps, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add strings
void MCMD_AddStrings(void)
{
    CStr LdFile;
    CStr OpFilters;
    long EntryCount = 0;

    if(ProjectOn == 0) return;
    OpFilters = "String resource files|*.str";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastStringsDir, 1, CurrentDir);
    LastStringsDir = ComDlgParseMultiFilesSelection(LdFile, &EnumAddStrings, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddStrings(char *FileToAdd, long UserValue)
{
    long EntryCount = 0;

    // Add it to the treeview
    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewStrings, "(" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    EntryCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewStrings);
    AddStringInArray(15, 0);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewStrings, "STRINGS_" + (CStr) EntryCount) != -1)
    {
        EntryCount++;
    }
    TreeViewAddItem(hTreeView, "STRINGS_" + (CStr) EntryCount + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewStrings, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add accelerators
void MCMD_AddAccelerators(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = "Accelerators resource files|*.acr";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastAcceleratorsDir, 1, CurrentDir);
    LastAcceleratorsDir = ComDlgParseMultiFilesSelection(LdFile, &EnumAddAccelerators, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddAccelerators(char *FileToAdd, long UserValue)
{
    long EntryCount = 0;

    // Add it to the treeview
    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewAccelerators, "(" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    EntryCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewAccelerators);
    AddAcceleratorInArray(15, 0);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewAccelerators, "ACCELERATORS_" + (CStr) EntryCount) != -1)
    {
        EntryCount++;
    }
    TreeViewAddItem(hTreeView, "ACCELERATORS_" + (CStr) EntryCount + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewAccelerators, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add menus
void MCMD_AddMenus(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = "Menus resource files|*.mnr";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastMenusDir, 1, CurrentDir);
    LastMenusDir = ComDlgParseMultiFilesSelection(LdFile, &EnumAddMenus, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddMenus(char *FileToAdd, long UserValue)
{
    long EntryCount = 0;

    // Add it to the treeview
    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewMenus, "(" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    EntryCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewMenus);
    AddMenuInArray(RESPROPS_DEFAULT, 0);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewMenus, "MENUS_" + (CStr) EntryCount) != -1)
    {
        EntryCount++;
    }
    TreeViewAddItem(hTreeView, "MENUS_" + (CStr) EntryCount + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewMenus, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add dialog
void MCMD_AddDialog(void)
{
    CStr LdFile;
    CStr OpFilters;

    if(ProjectOn == 0) return;
    OpFilters = "Dialog resource files|*.dia";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastDialogsDir, 1, CurrentDir);
    LastDialogsDir = ComDlgParseMultiFilesSelection(LdFile, &EnumAddDialog, MULTIFILESENUM_FORWARD, 0);
}

long CALLBACK EnumAddDialog(char *FileToAdd, long UserValue)
{
    long EntryCount = 0;

    // Add it to the treeview
    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewDialogs, "(" + (CStr) FileToAdd + (CStr) ")") != -1)
    {
        WriteToStatus("File '" + (CStr) FileToAdd + (CStr) "' already referenced in project.");
        return(1);
    }
    EntryCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewDialogs);
    AddDialogInArray(RESPROPS_DEFAULT, 0);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewDialogs, "DIALOG_" + (CStr) EntryCount) != -1)
    {
        EntryCount++;
    }
    TreeViewAddItem(hTreeView, "DIALOG_" + (CStr) EntryCount + (CStr) " (" + (CStr) FileToAdd + (CStr) ")", hTreeViewDialogs, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
    return(1);
}

// -----------------------------------------------------------------------
// Project/Add raw datas
void MCMD_AddRawDatas(void)
{
    CStr LdFile;
    CStr OpFilters;
    long EntryCount = 0;

    if(ProjectOn == 0) return;
    OpFilters = IniReadKey("ExtrasExt", "AddRawDatas", MainIniFile);
    if(OpFilters.Len() == 0) OpFilters = "All files|*.*";
    LdFile = ComDlgGetOpenFileName(hMDIform.hWnd, OpFilters, LastRawsDir, 0, CurrentDir);
    if(LdFile.Len() == 0) return;
    LastRawsDir = FileGetDirectory(LdFile);
    // Add it to the treeview
    if(TreeViewSearchChildPartialText(hTreeView, hTreeViewRawdatas, "(" + (CStr) LdFile + (CStr) ")") != -1)
    {
        MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) LdFile + (CStr) "' already referenced in project.", MB_ERROR, Requesters);
        return;
    }
    EntryCount = TreeViewGetChildItemsCount(hTreeView, hTreeViewRawdatas);
    AddRawDatasInArray(RESPROPS_DEFAULT, 0);
    while(TreeViewSearchChildPartialText(hTreeView, hTreeViewRawdatas, "RAWDATAS_" + (CStr) EntryCount) != -1)
    {
        EntryCount++;
    }
    TreeViewAddItem(hTreeView, "RAWDATAS_" + (CStr) EntryCount + (CStr) " (" + (CStr) LdFile + (CStr) ")", hTreeViewRawdatas, 0, ICON_RES, ICON_RES, 0, 1);
    ProjectModified = TRUE;
}

// -----------------------------------------------------------------------
// Project/Save project
void MCMD_SaveProjectAs(void)
{
    CStr FName;
    CStr Filters;

    if(ProjectOn == 0) return;
    Filters = AppTitle + (CStr) " project files (*.Med)|*.Med";
    FName = ComDlgGetSaveFileName(hMDIform.hWnd, Filters, Dirs[DIR_PROJECTS], CurrentDir);
    if(FName.Len() != 0)
    {
        if(FName.Len() < 4) FName = FName + (CStr) ".med";
        if(_strcmpi(FName.Right(4).Get_String(), ".med") != 0) FName = FName + (CStr) ".med";
        ProjectFName = FName;
        ProjectTitle = FileGetFileName(ProjectFName);
        ProjectTitle = ProjectTitle.Left(ProjectTitle.Len() - 4);
        TreeViewSetItemText(hTreeView, hTreeViewRoot, ProjectTypeName + (CStr) " (" + (CStr) ProjectFName + (CStr) ")");
        SaveProject();
    }
}

// -----------------------------------------------------------------------
// Project/Project properties
void MCMD_ProjectProperties(void)
{
    if(ProjectOn == 0) return;
    CreateModalDialog(-1, -1, 402, 240, hMDIform.hWnd, &FRMPrjPropsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Project/Assemble Project
void MCMD_AssembleProject(void)
{
    if(ProjectOn == 0) return;
    BuildProject(BUILD_ASSEMBLE);
}

// -----------------------------------------------------------------------
// Project/Run Project
void MCMD_RunProject(void)
{
    if(ProjectOn == 0) return;
    BuildProject(BUILD_RUN);
}

// -----------------------------------------------------------------------
// Project/Debug Project
void MCMD_DebugProject(void)
{
    if(ProjectOn == 0) return;
    BuildProject(BUILD_DEBUG);
}

// -----------------------------------------------------------------------
// AddIns/AddIns manager
void MCMD_AddInsManager(void)
{
    CreateModalDialog(-1, -1, 486, 318, hMDIform.hWnd, &FRMAddInsProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Window/Context help
void MCMD_ContextHelp(void)
{
    CStr TmpHelp;
    CStr CurWord = "";
    long UserCmdNbr = 0;
    CStr HelpLanguage;
    long HelpOffset = 0;
    CStr BufString;

    UserCmdNbr = -1;
    if(NbForms)
    {
        ChildStruct = LoadStructure(CurrentForm);
        // Search keyword in current language
        CurWord = GetCurrentWord(ChildStruct->hChildCodeMax);
        HelpLanguage = CMGetRealLanguage(ChildStruct->Language);
        HelpOffset = GetLanguageOffset(HelpLanguage);
        if(HelpOffset == -1)
        {
            MiscMsgBox(hMDIform.hWnd, "Unknown language.", MB_ERROR, Requesters);
            return;
        }
        
        BufString = (char *) GetLangDat(HelpOffset)->pszKeywords;
        if(BufString.In_StrWord(1, CurWord, Text_Compare) != 0)
        {
            BufString = (char *) GetLangDat(HelpOffset)->pszHelpFileA;
            if(BufString.Len() != 0)
            {
                UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
            }
            else
            {
                MiscMsgBox(hMDIform.hWnd, "No help file associated with '" + (CStr) CurWord + (CStr) "'.", MB_ERROR, Requesters);
                return;
            }
            goto HelpResult;
        }
        
        BufString = (char *) GetLangDat(HelpOffset)->pszTagAttributeNames;
        if(BufString.In_StrWord(1, CurWord, Text_Compare) != 0)
        {
            BufString = (char *) GetLangDat(HelpOffset)->pszHelpFileB;
            if(BufString.Len() != 0)
            {
                UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
            }
            else
            {
                MiscMsgBox(hMDIform.hWnd, "No help file associated with '" + (CStr) CurWord + (CStr) "'.", MB_ERROR, Requesters);
                return;
            }
            goto HelpResult;
        }
        
        BufString = (char *) GetLangDat(HelpOffset)->pszTagElementNames;
        if(BufString.In_StrWord(1, CurWord, Text_Compare) != 0)
        {
            BufString = (char *) GetLangDat(HelpOffset)->pszHelpFileC;
            if(BufString.Len() != 0)
            {
                UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
            }
            else
            {
                MiscMsgBox(hMDIform.hWnd, "No help file associated with '" + (CStr) CurWord + (CStr) "'.", MB_ERROR, Requesters);
                return;
            }
            goto HelpResult;
        }
        
        BufString = (char *) GetLangDat(HelpOffset)->pszTagEntities;
        if(BufString.In_StrWord(1, CurWord, Text_Compare) != 0)
        {
            BufString = (char *) GetLangDat(HelpOffset)->pszHelpFileD;
            if(BufString.Len() != 0)
            {
                UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
            }
            else
            {
                MiscMsgBox(hMDIform.hWnd, "No help file associated with '" + (CStr) CurWord + (CStr) "'.", MB_ERROR, Requesters);
                return;
            }
            goto HelpResult;
        }
        
        BufString = (char *) GetLangDat(HelpOffset)->pszOperators;
        if(BufString.In_StrWord(1, CurWord, Text_Compare) != 0)
        {
            BufString = (char *) GetLangDat(HelpOffset)->pszHelpFileE;
            if(BufString.Len() != 0)
            {
                UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
            }
            else
            {
                MiscMsgBox(hMDIform.hWnd, "No help file associated with '" + (CStr) CurWord + (CStr) "'.", MB_ERROR, Requesters);
                return;
            }
            goto HelpResult;
        }
        
        BufString = (char *) GetLangDat(HelpOffset)->pszExtra1;
        if(BufString.In_StrWord(1, CurWord, Text_Compare) != 0)
        {
            BufString = (char *) GetLangDat(HelpOffset)->pszHelpFileF;
            if(BufString.Len() != 0)
            {
                UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
            }
            else
            {
                MiscMsgBox(hMDIform.hWnd, "No help file associated with '" + (CStr) CurWord + (CStr) "'.", MB_ERROR, Requesters);
                return;
            }
            goto HelpResult;
        }

        BufString = (char *) GetLangDat(HelpOffset)->pszExtra2;
        if(BufString.In_StrWord(1, CurWord, Text_Compare) != 0)
        {
            BufString = (char *) GetLangDat(HelpOffset)->pszHelpFileG;
            if(BufString.Len() != 0)
            {
                UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
            }
            else
            {
                MiscMsgBox(hMDIform.hWnd, "No help file associated with '" + (CStr) CurWord + (CStr) "'.", MB_ERROR, Requesters);
                return;
            }
            goto HelpResult;
        }
        
        BufString = (char *) GetLangDat(HelpOffset)->pszExtra3;
        if(BufString.In_StrWord(1, CurWord, Text_Compare) != 0)
        {
            BufString = (char *) GetLangDat(HelpOffset)->pszHelpFileH;
            if(BufString.Len() != 0)
            {
                UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
            }
            else
            {
                MiscMsgBox(hMDIform.hWnd, "No help file associated with '" + (CStr) CurWord + (CStr) "'.", MB_ERROR, Requesters);
                return;
            }
            goto HelpResult;
        }
    }
    // Launch win32api help file if nothing can be found
    BufString = IniReadKey("Layout", "WinHelp", MainIniFile);
    if(BufString.Len() == 0)
    {
        switch(MiscMsgBox(hMDIform.hWnd, "Windows API help directory not specified.\rDo you want to set it now ?", MB_ICONQUESTION | MB_YESNO, Requesters)) {
            case IDYES:
                FRMPropertiesFirstTab = 1;
                DisplayProperties();
                return;
            case IDNO:
                if(NbForms != 0) if(ControlIsVisible(CurrentForm) != 0) SetFocus(CurrentForm);
                return;
        }
    }
    UserCmdNbr = OpenContextHelpFile(BufString, CurWord);
HelpResult:
    if(UserCmdNbr == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Error while opening help file.", MB_ERROR, Requesters);
        if(NbForms != 0) if(ControlIsVisible(CurrentForm) != 0) SetFocus(CurrentForm);
    }
}

// -----------------------------------------------------------------------
// Window/MSDN Help
void MCMD_MSDNHelp(void)
{
    MSDNDir = IniReadKey("Layout", "MSDNHelp", MainIniFile);
    MSDNWord = "";
    if(MSDNDir.Len() == 0)
    {
        switch(MiscMsgBox(hMDIform.hWnd, "MSDN/Platform SDK directory not specified.\rDo you want to set it now ?", MB_ICONQUESTION | MB_YESNO, Requesters))
        {
            case IDYES:
                FRMPropertiesFirstTab = 1;
                DisplayProperties();
                return;
            case IDNO:
                break;
        }
        if(NbForms != 0) if(ControlIsVisible(CurrentForm) != 0) SetFocus(CurrentForm);
        return;
    }
    if(NbForms == 0)
    {
        DisplayChmVB(MSDNDir, MSDNWord);
    }
    else
    {
        ChildStruct = LoadStructure(CurrentForm);
        MSDNWord = GetCurrentWord(ChildStruct->hChildCodeMax);
        DisplayChmVB(MSDNDir, MSDNWord);
    }
}

// -----------------------------------------------------------------------
// Window/DDK Help
void MCMD_DDKHelp(void)
{
    MSDNDir = IniReadKey("Layout", "DDKHelp", MainIniFile);
    MSDNWord = "";
    if(MSDNDir.Len() == 0)
    {
        switch(MiscMsgBox(hMDIform.hWnd, "Windows DDK directory not specified.\rDo you want to set it now ?", MB_ICONQUESTION | MB_YESNO, Requesters))
        {
            case IDYES:
                FRMPropertiesFirstTab = 1;
                DisplayProperties();
                return;
            case IDNO:
                break;
        }
        if(NbForms != 0) if(ControlIsVisible(CurrentForm) != 0) SetFocus(CurrentForm);
        return;
    }
    if(NbForms == 0)
    {
        DisplayChmVB(MSDNDir, MSDNWord);
    }
    else
    {
        ChildStruct = LoadStructure(CurrentForm);
        MSDNWord = GetCurrentWord(ChildStruct->hChildCodeMax);
        DisplayChmVB(MSDNDir, MSDNWord);
    }
}

// -----------------------------------------------------------------------
// Window/DirectX SDK Help
void MCMD_DirectXSDKHelp(void)
{
    MSDNDir = IniReadKey("Layout", "DirectXSDKHelp", MainIniFile);
    MSDNWord = "";
    if(MSDNDir.Len() == 0)
    {
        switch(MiscMsgBox(hMDIform.hWnd, "DirectX SDK directory not specified.\rDo you want to set it now ?", MB_ICONQUESTION | MB_YESNO, Requesters))
        {
            case IDYES:
                FRMPropertiesFirstTab = 1;
                DisplayProperties();
                return;
            case IDNO:
                break;
        }
        if(NbForms != 0) if(ControlIsVisible(CurrentForm) != 0) SetFocus(CurrentForm);
        return;
    }
    if(NbForms == 0)
    {
        DisplayChmVB(MSDNDir, MSDNWord);
    }
    else
    {
        ChildStruct = LoadStructure(CurrentForm);
        MSDNWord = GetCurrentWord(ChildStruct->hChildCodeMax);
        DisplayChmVB(MSDNDir, MSDNWord);
    }
}

// -----------------------------------------------------------------------
// Window/About
void MCMD_About(void)
{
    CreateModalDialog(-1, -1, 500, 318, hMDIform.hWnd, &FRMAboutProc, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
}

// -----------------------------------------------------------------------
// Window/Split
void MCMD_Split(void)
{
    if(NbForms == 0) return;
    ChildStruct = LoadStructure(ClientGetActiveChild(hMDIform.hClient));
    if(CM_GetSplitterPos(ChildStruct->hChildCodeMax, 0) == 0)
    {
        CM_SetSplitterPos(ChildStruct->hChildCodeMax, 0, ControlHeight(ChildStruct->hChildCodeMax) / 2);
    }
    else
    {
        CM_SetSplitterPos(ChildStruct->hChildCodeMax, 0, 0);
    }
}

// -----------------------------------------------------------------------
// Window/Tile horizontally
void MCMD_TileHorizontally(void)
{
    ClientTileHorizontal(hMDIform.hClient);
    FocusOnActiveChild();
}

// -----------------------------------------------------------------------
// Window/Tile vertically
void MCMD_TileVertically(void)
{
    ClientTileVertical(hMDIform.hClient);
    FocusOnActiveChild();
}

// -----------------------------------------------------------------------
// Window/Cascade
void MCMD_Cascade(void)
{
    ClientTileCascade(hMDIform.hClient);
    FocusOnActiveChild();
}

// -----------------------------------------------------------------------
// Window/Arrange icons
void MCMD_ArrangeIcons(void)
{
    ClientTileArrangeIconic(hMDIform.hClient);
    FocusOnActiveChild();
}

// -----------------------------------------------------------------------
// Window/Next window
void MCMD_NextWindow(void)
{
    ClientSetNextChild(hMDIform.hClient);
}

// -----------------------------------------------------------------------
// Window/Previous window
void MCMD_PreviousWindow(void)
{
    ClientSetPreviousChild(hMDIform.hClient);
}
