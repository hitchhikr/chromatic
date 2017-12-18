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
// ProjectsBuilder.h: declaration of used variables / structures for ProjectsBuilder.cpp
// -----------------------------------------------------------------------

#ifndef _PROJECTSBUILDER_H_
#define _PROJECTSBUILDER_H_

// -----------------------------------------------------------------------
// Constants
#define BUILD_ASSEMBLE 0
#define BUILD_RUN 1
#define BUILD_DEBUG 2

#define RESPROPS_DEFAULT 15

// -----------------------------------------------------------------------
// Variables
extern CStr ProjectsRetVal;
extern CStr ProjectTitle;
extern CStr ProjectType;
extern CStr ProjectTypeName;
extern CStr ProjectTypeExt;
extern CStr ProjectTypeImg;
extern CStr ProjectTypeScheduledImg;
extern CStr ProjectPostRename;
extern CStr ProjectTypeExtInternalImg;
extern CStr ProjectWizard;
extern int ProjectCreate;
extern int ProjectGather;
extern long ProjectOn;
extern long ProjectModified;
extern CStr ProjectFName;
extern FILETIME ProjectFDate;
extern CStr ProjectDir;
extern CStr PrjBuildIniFile;
extern CStr PrjDebugLink;

extern long Prj_Major;
extern long Prj_Minor;
extern long Prj_Revision;
extern long Prj_Release;
extern long Prj_IncludeVersion;
extern long Prj_IncludeManifest;
extern long Prj_FileType;
extern long Prj_OS;
extern CStr Prj_Comments;
extern CStr Prj_FileDescription;
extern CStr Prj_CompanyName;
extern CStr Prj_LegalCopyRight;
extern CStr Prj_LegalTrademarks;
extern CStr Prj_ProductName;

extern CStr ProjectRetVal;
extern long PrjErrFileRead;

extern HTREEITEM hTreeViewRoot;
extern HTREEITEM hTreeViewIncludes;
extern HTREEITEM hTreeViewLibs;
extern HTREEITEM hTreeViewModules;
extern HTREEITEM hTreeViewObjects;
extern HTREEITEM hTreeViewResources;

// --- New resources
extern HTREEITEM hTreeViewIcons;
extern HTREEITEM hTreeViewCursors;
extern HTREEITEM hTreeViewBitmaps;
extern HTREEITEM hTreeViewStrings;
extern HTREEITEM hTreeViewAccelerators;
extern HTREEITEM hTreeViewMenus;
extern HTREEITEM hTreeViewDialogs;
extern HTREEITEM hTreeViewRawdatas;

extern HTREEITEM hTreeViewDefs;
extern HTREEITEM hTreeViewTexts;
extern CList <char *> SkinArrayVar;
extern CList <char *> SkinArrayDat;
extern CStr HeadExt;
extern CStr PrjIncludeType;
extern CStr PrjIncludeLibType;
extern CStr PutExtendedRes;
extern HANDLE BatchFile;
extern HANDLE HeaderFile;
extern HANDLE ObjectsFile;
extern CList <char *> LinkDOSCom;
extern CList <char *> CompDOSCom;
extern CStr DefinedEntryPoint;

// --- Resources properties arrays
extern CList <long *> PrjIconsArray;
extern CList <long *> PrjCursorsArray;
extern CList <long *> PrjBitmapsArray;
extern CList <long *> PrjStringsArray;
extern CList <long *> PrjAcceleratorsArray;
extern CList <long *> PrjMenusArray;
extern CList <long *> PrjDialogsArray;
extern CList <long *> PrjRawDatasArray;
extern CList <char *> PossibleMissingFiles;
extern CList <HTREEITEM> PossibleMissingFilesTree;
extern CList <char *> PossibleMissingFilesResName;
extern CList <int> PossibleMissingFilesResProp;
extern CList <int> PossibleMissingFilesResLang;
extern CStr ObjectsList;
extern CStr Manifest;

// -----------------------------------------------------------------------
// Functions declarations
void CreateProject(int GatherFiles);
void OpenProjectAuto(CStr PrjName);
void SaveProject(void);
long CheckProjectFile(CStr FileName);
long RealOpenPrj(CStr PrjName);
void SwitchProjectOn(void);
long IsInProject(CStr FileToSearch, long IncludeTexts);
long GetModulesNumber(void);
void BuildProject(long BuildType);
void CleanBuildFiles(void);
void FillCompDOSCommand(void);
CStr GetCompDOSCommand(CStr DOSCom);
void FillLinkDOSCommand(void);
CStr GetLinkDOSCommand(CStr DOSCom);
void RunProject(void);
void DebugProject(void);
CStr SkinCommand(CStr Comd);
void GenProto(CStr LibFile);
void CreateProjectStruct(void);
CStr ResIncludeFile(CStr ResFileName);
CStr ResIncludeString(CStr ResString);
void EraseResArrays(void);
void AddIconInArray(long Data, long Langu);
void AddCursorInArray(long Data, long Langu);
void AddBitmapInArray(long Data, long Langu);
void AddStringInArray(long Data, long Langu);
void AddAcceleratorInArray(long Data, long Langu);
void AddMenuInArray(long Data, long Langu);
void AddDialogInArray(long Data, long Langu);
void AddRawDatasInArray(long Data, long Langu);
long GetIconProps(long Index);
long GetIconLang(long Index);
long GetCursorProps(long Index);
long GetCursorLang(long Index);
long GetBitmapProps(long Index);
long GetBitmapLang(long Index);
long Get_StringProps(long Index);
long Get_StringLang(long Index);
long GetAcceleratorProps(long Index);
long GetAcceleratorLang(long Index);
long GetMenuProps(long Index);
long GetMenuLang(long Index);
long GetDialogProps(long Index);
long GetDialogLang(long Index);
long GetRawdataProps(long Index);
long GetRawdataLang(long Index);
void SetIconProps(long Index, long Dat);
void SetIconLang(long Index, long Dat);
void SetCursorProps(long Index, long Dat);
void SetCursorLang(long Index, long Dat);
void SetBitmapProps(long Index, long Dat);
void SetBitmapLang(long Index, long Dat);
void Set_StringProps(long Index, long Dat);
void Set_StringLang(long Index, long Dat);
void SetAcceleratorProps(long Index, long Dat);
void SetAcceleratorLang(long Index, long Dat);
void SetMenuProps(long Index, long Dat);
void SetMenuLang(long Index, long Dat);
void SetDialogProps(long Index, long Dat);
void SetDialogLang(long Index, long Dat);
void SetRawDataProps(long Index, long Dat);
void SetRawDataLang(long Index, long Dat);
void RemIconFromArray(long Idx);
void RemCursorFromArray(long Idx);
void RemBitmapFromArray(long Idx);
void RemStringFromArray(long Idx);
void RemAcceleratorFromArray(long Idx);
void RemMenuFromArray(long Idx);
void RemDialogFromArray(long Idx);
void RemRawDataFromArray(long Idx);
CStr CalcResourceHeader(long ResourceID);
CStr CalcResourceLanguage(long ResourceID);
CStr WriteLanguageEquate(long ResourceID);
void WriteAcceleratorEquates(HANDLE ResFile);
long Convert_Menus(CStr FileName, CStr IndexID, long ResIndex);
CStr ConvertMenusRes(CStr FileName, CStr IndexID, long ResIndex);
CStr AddResNest(long NestLevel);
long Convert_Raw(CStr FileName, long IndexID, long ResIndex, long UseComma, CStr StrRawDat, long ByFile, int IncludeLang);
void Convert_Icon(CStr FileName, CStr IndexID, long ResIndex);
void Convert_Cursor(CStr FileName, CStr IndexID, long ResIndex);
void Convert_Bitmap(CStr FileName, CStr IndexID, long ResIndex);
void Convert_Strings(CStr FileName, CStr IndexID, long ResIndex);
HWND CreateNewResourceFileWin(CStr ResFile);
void Convert_Accelerators(CStr FileName, CStr IndexID, long ResIndex);
void CaptureDOSError(CStr DOSCmdName, CStr DOSCmdArgs, CStr DOSCmdDir);
CStr IsErrorReported(CStr LineToCheck);

#endif
