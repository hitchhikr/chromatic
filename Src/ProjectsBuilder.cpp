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
// ProjectsBuilder.cpp: Routines related to projects building
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "MDI_Form.h"
#include "Globals.h"
#include "CmdMenus.h"
#include "Splitters.h"
#include "MiscFunctions.h"
#include "ProjectsBuilder.h"
#include "Script_Engine/Scripts.h"
#include "Menus.h"
#include "ConstRes.h"
#include "Debugger.h"
#include "APIDb.h"
#include "MDI_Childs.h"
#include "Dlg_MissingFiles.h"
#include "Dlg_Args.h"
#include "Dlg_ResProps.h"
#include "Dlg_Menus.h"

// -----------------------------------------------------------------------
// Variables
CStr ProjectsRetVal;
CStr ProjectTitle;
CStr ProjectType;
CStr ProjectTypeName;
CStr ProjectTypeExt;
CStr ProjectTypeImg;
CStr ProjectTypeScheduledImg;
CStr ProjectPostRename;
CStr ProjectTypeExtInternalImg;
CStr ProjectWizard;
int ProjectCreate;
int ProjectGather;
long ProjectOn;
long ProjectModified;
CStr ProjectFName;
FILETIME ProjectFDate;
CStr ProjectDir;
CStr PrjBuildIniFile;
CStr PrjDebugLink;

CList <char *> ConstNames;
CList <char *> ConstValues;

long Prj_Major;
long Prj_Minor;
long Prj_Revision;
long Prj_Release;
long Prj_IncludeVersion;
long Prj_IncludeManifest;
long Prj_FileType;
long Prj_OS;
CStr Prj_Comments;
CStr Prj_FileDescription;
CStr Prj_CompanyName;
CStr Prj_LegalCopyRight;
CStr Prj_LegalTrademarks;
CStr Prj_ProductName;

CStr ProjectRetVal;

HTREEITEM hTreeViewRoot;
HTREEITEM hTreeViewIncludes;
HTREEITEM hTreeViewLibs;
HTREEITEM hTreeViewModules;
HTREEITEM hTreeViewObjects;
HTREEITEM hTreeViewResources;

// --- New resources
HTREEITEM hTreeViewIcons;
HTREEITEM hTreeViewCursors;
HTREEITEM hTreeViewBitmaps;
HTREEITEM hTreeViewStrings;
HTREEITEM hTreeViewAccelerators;
HTREEITEM hTreeViewMenus;
HTREEITEM hTreeViewDialogs;
HTREEITEM hTreeViewRawdatas;

HTREEITEM hTreeViewDefs;
HTREEITEM hTreeViewTexts;
CList <char *> SkinArrayVar;
CList <char *> SkinArrayDat;
CStr HeadExt;
CStr PrjIncludeType;
CStr PrjIncludeLibType;
CStr PutExtendedRes;
HANDLE BatchFile;
HANDLE HeaderFile;
HANDLE ObjectsFile;
CList <char *> LinkDOSCom;
CList <char *> CompDOSCom;
CStr DefinedEntryPoint;

// --- Resources properties arrays
CList <long *> PrjIconsArray;
CList <long *> PrjCursorsArray;
CList <long *> PrjBitmapsArray;
CList <long *> PrjStringsArray;
CList <long *> PrjAcceleratorsArray;
CList <long *> PrjMenusArray;
CList <long *> PrjDialogsArray;
CList <long *> PrjRawDatasArray;
CList <char *> PossibleMissingFiles;
CList <HTREEITEM> PossibleMissingFilesTree;
CList <char *> PossibleMissingFilesResName;
CList <int> PossibleMissingFilesResProp;
CList <int> PossibleMissingFilesResLang;
CStr ObjectsList;

int ProjectFileNumber;

// -----------------------------------------------------------------------
// Functions
void Create_Project_IncludeLib(int GatherFiles, CStr IniSection, CStr IniKey, CStr Directory);
void Project_Write_Version_Res(HANDLE ResourcesFile);
CStr Project_Write_XPManifest_Res(void);
HTREEITEM Add_Project_Constant(HANDLE HeaderFile, HTREEITEM TreeViewChildEntry, CStr ConstantType, long ObjIndex);
void Write_Custom_Constants(void);
void SaveAllChildsInProject(HWND hWnd);
int CALLBACK EnumSaveAllChildsInProject(HWND hWnd, long lParam);
int CheckMissingFile(CStr FileName, int ReportMissing, HTREEITEM hOrigTreeItem, CStr ResourceName, int ResourceProp, int ResourceLang);
void ReadResourceFromProject(CStr PrjName, CStr EntryToAdd, CStr ResourceName, int i, CStr ResourceDefaultLabel, HTREEITEM hTreeItem, void (*AddInArray)(long Data, long Langu));

// -----------------------------------------------------------------------
// Create a new project
void CreateProject(int GatherFiles)
{
    CStr AbsPath;
    int i = 0;

    CStr SaveProjectTitle;
    CStr SaveProjectType;
    CStr SaveProjectTypeName;
    CStr SaveProjectTypeExt;
    CStr SaveProjectPostRename;
    CStr SaveProjectTypeExtInternalImg;
    CStr SaveProjectTypeImg;
    CStr SaveProjectWizard;
    CStr BufString;
    CStr BufString2;

    // Save last project infos
    SaveProjectTitle = ProjectTitle;
    SaveProjectType = ProjectType;
    SaveProjectTypeName = ProjectTypeName;
    SaveProjectTypeExt = ProjectTypeExt;
    SaveProjectTypeExtInternalImg = ProjectTypeExtInternalImg;
    SaveProjectTypeImg = ProjectTypeImg;
    SaveProjectPostRename = ProjectPostRename;
    SaveProjectWizard = ProjectWizard;
    if(ProjectOn)
    {
        if(ProjectModified)
        {
            switch(MiscMsgBox(hMDIform.hWnd, "Current project modified. Save it now ?", MB_ICONQUESTION | MB_YESNOCANCEL, Requesters)) {
                case IDYES:
                    MCMD_SaveProject();
                    break;
                case IDNO:
                    ProjectModified = FALSE;
                    break;
                case IDCANCEL:
                    return;
            }
        }
    }
    if(ProjectOn) MCMD_CloseAll();
    if(AutoClearOut) ClearStatus();
    
    // Restore last project infos
    ProjectTitle = SaveProjectTitle;
    ProjectType = SaveProjectType;
    ProjectTypeName = SaveProjectTypeName;
    ProjectTypeExt = SaveProjectTypeExt;
    ProjectPostRename = SaveProjectPostRename;
    ProjectTypeExtInternalImg = SaveProjectTypeExtInternalImg;
    ProjectTypeImg = SaveProjectTypeImg;
    ProjectWizard = SaveProjectWizard;
    if(ProjectPostRename.Len() != 0)
    {
        BufString = PutStatusDatePrefix();
        BufString = BufString + "Creating project: ";
        BufString = BufString + ProjectTitle;
        BufString = BufString + " (";
        BufString = BufString + ProjectPostRename;
        BufString = BufString + ")...";
        WriteToStatus(BufString);
    }
    else
    {
        BufString = PutStatusDatePrefix();
        BufString = BufString + "Creating project: ";
        BufString = BufString + ProjectTitle;
        BufString = BufString + " ("; 
        BufString = BufString + ProjectTypeExt;
        BufString = BufString + ")...";
        WriteToStatus(BufString);
    }
    if(ProjectType.Len() != 0)
    {
        if(FileExist(Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".med") != 0)
        {
            BufString = (CStr) "Project structure already exists." + (CStr) "\r" + (CStr) "Overwrite project and associated files ?";
            if(MiscMsgBox(hMDIform.hWnd, BufString, MB_QUESTION, Requesters) == IDNO)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Project creation aborted.");
                return;
            }
        }
        else
        {
            BufString = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle;
            if(!FileExist(BufString))
            {
                if(CreateDirectory(BufString.Get_String(), 0) == 0)
                {
                    MiscMsgBox(hMDIform.hWnd, "Can't create directory structure.", MB_ERROR, Requesters);
                    return;
                }
            }
        }
        // Save project name (to resave it)
        ProjectFName = Dirs[DIR_PROJECTS] + (CStr) "\\" + ProjectTitle + "\\" + ProjectTitle + ".med";

        DeleteFile(ProjectFName.Get_String());
        
        // Create project file
        IniWriteKey("Project", "Name", ProjectTitle, ProjectFName);
        IniWriteKey("Project", "Title", ProjectTitle, ProjectFName);
        IniWriteKey("Project", "Type", ProjectType, ProjectFName);
        IniWriteKey("Project", "TypeExt", ProjectTypeExt, ProjectFName);
        IniWriteKey("Project", "TypeName", ProjectTypeName, ProjectFName);
        IniWriteKey("Project", "TypeRename", ProjectPostRename, ProjectFName);
        
        // --- (Re)Init the project values
        Prj_Major = 0;
        Prj_Minor = 0;
        Prj_Revision = 0;
        Prj_Release = 0;
        Prj_IncludeVersion = 1;      // Backwards compatility
        Prj_IncludeManifest = 0;     // Backwards compatility
        Prj_FileType = 1;
        Prj_OS = 4;
        Prj_Comments = "";
        Prj_FileDescription = "";
        // Read registered values

        Prj_CompanyName = StringReplace(IniReadKey("UserInfos", "CompanyName", MainIniFile), Prj_CompanyName.Chr(2), Prj_CompanyName.Chr(13) + Prj_CompanyName.Chr(10), 1, -1, Binary_Compare);
        Prj_LegalCopyRight = StringReplace(IniReadKey("UserInfos", "LegalCopyRight", MainIniFile), Prj_LegalCopyRight.Chr(2), Prj_LegalCopyRight.Chr(13) + Prj_LegalCopyRight.Chr(10), 1, -1, Binary_Compare);
        Prj_LegalTrademarks = StringReplace(IniReadKey("UserInfos", "LegalTrademarks", MainIniFile), Prj_LegalTrademarks.Chr(2), Prj_LegalTrademarks.Chr(13) + Prj_LegalTrademarks.Chr(10), 1, -1, Binary_Compare);

        Prj_ProductName = "";
        ProjectDir = (CStr) Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle;
        
        // Set default icon
        if(ProjectTypeImg.Len() == 0) ProjectTypeImg = "Windows.ico";
        
        // Kill old icon (if necessary)
        BufString = Dirs[DIR_PROJECTS];
        BufString = BufString + "\\";
        BufString = BufString + ProjectTitle;
        BufString = BufString + "\\";
        BufString = BufString + ProjectTypeImg;
        DeleteFile(BufString.Get_String());

        BufString = Dirs[DIR_TEMPLATES];
        BufString = BufString + "\\";
        BufString = BufString + ProjectTypeImg;
        BufString2 = Dirs[DIR_PROJECTS];
        BufString2 = BufString2 + "\\" ;
        BufString2 = BufString2 + ProjectTitle;
        BufString2 = BufString2 + "\\";
        BufString2 = BufString2 + ProjectTypeImg;
        CopyFile(BufString.Get_String(), BufString2.Get_String(), 0);

        IniWriteKey("Project", "Image", ProjectTypeImg, ProjectFName);
        
        if(ProjectWizard.Len() != 0)
        {
            // Use LUA to create project
            if(RunScriptFile(Dirs[DIR_WIZARDS] + (CStr) "\\" + (CStr) ProjectWizard, "") > 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Project created.");
                OpenProjectAuto(ProjectFName);
            }
            else
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Process aborted.");
                // Remove the project file
                DeleteFile(ProjectFName.Get_String());
                // Remove the icon
                DeleteFile(BufString2.Get_String());
                BufString = Dirs[DIR_PROJECTS];
                BufString = BufString + "\\";
                BufString = BufString + ProjectTitle;
                RemoveDirectory(BufString.Get_String());
            }
        }
        else
        {
            // Use internal routines
            Create_Project_IncludeLib(GatherFiles, "INCLUDES", "INCLUDE", Dirs[DIR_INCLUDE]);
            Create_Project_IncludeLib(GatherFiles, "LIBS", "LIBRARY", Dirs[DIR_LIB]);

            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "MODULE" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("MODULES", "MODULE" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
            }
        
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "OBJECT" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = (CStr) Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = (CStr) Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(),AbsPath.Get_String(),0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("OBJECTS", "OBJECT" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
            }

            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "ICONS" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = (CStr) Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("ICONS", "ICON" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
                IniWriteKey("ICONS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), "15", ProjectFName);
                IniWriteKey("ICONS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ProjectFName);
            }
        
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "CURSORS" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = (CStr) Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(),AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("CURSORS", "CURSOR" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
                IniWriteKey("CURSORS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), "15", ProjectFName);
                IniWriteKey("CURSORS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ProjectFName);
            }
        
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "BITMAPS" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("BITMAPS", "BITMAP" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
                IniWriteKey("BITMAPS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), "15", ProjectFName);
                IniWriteKey("BITMAPS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ProjectFName);
            }
        
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "STRINGS" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("STRINGS", "STRING" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
                IniWriteKey("STRINGS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), "15", ProjectFName);
                IniWriteKey("STRINGS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ProjectFName);
            }
        
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "ACCELERATORS" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("ACCELERATORS", "ACCELERATOR" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
                IniWriteKey("ACCELERATORS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), "15", ProjectFName);
                IniWriteKey("ACCELERATORS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ProjectFName);
            }
        
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "MENUS" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("MENUS", "MENU" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
                IniWriteKey("MENUS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), "15", ProjectFName);
                IniWriteKey("MENUS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ProjectFName);
            }

            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "DIALOG" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("DIALOGS", "DIALOG" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
                IniWriteKey("DIALOGS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), "15", ProjectFName);
                IniWriteKey("DIALOGS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ProjectFName);
            }
        
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "RAWDATAS" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("RAWDATAS", "RAWDATA" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
                IniWriteKey("RAWDATAS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), "15", ProjectFName);
                IniWriteKey("RAWDATAS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), "0", ProjectFName);
            }
        
            // Old resources
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "RESOURCE" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("RESOURCES", "RESOURCE" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
            }
        
            if(hTreeViewDefs != 0)
            {
                ProjectRetVal = IniReadKey(ProjectType, "DEF", ProjectsIniFile);
                if(ProjectRetVal.Len() != 0)
                {
                    AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                    DeleteFile(AbsPath.Get_String());
                    BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                    CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                    AbsPath = ChangeAbsolutePaths(AbsPath);
                    IniWriteKey("DEFS", "DEF", AbsPath, ProjectFName);
                }
            }
        
            for(i = 0; i <= 999; i++)
            {
                ProjectRetVal = IniReadKey(ProjectType, "TEXT" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
                if(ProjectRetVal.Len() == 0) break;
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectRetVal;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) ProjectRetVal;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey("TEXTS", "TEXT" + (CStr) StringNumberComplement(i, 3).Get_String(), AbsPath, ProjectFName);
            }
            Write_Custom_Constants();
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Project created.");
            OpenProjectAuto(ProjectFName);
        }
    }
    else
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "*** Error: mangled project definition.");
    }
}

// -----------------------------------------------------------------------
// Write the custom constant in the dest project file
void Write_Custom_Constants(void)
{
    // Write custom constants
    for(int i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey(ProjectType, "CONSTANT" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
        if(ProjectRetVal.Len() == 0) break;
        IniWriteKey("CONSTANTS", "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectRetVal, Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".med");
        ProjectRetVal = IniReadKey(ProjectType, "CONSTANTVALUE" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
        IniWriteKey("CONSTANTS", "VALUE" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectRetVal, Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".med");
    }
}

// -----------------------------------------------------------------------
// Add includes from templates into project
void Create_Project_IncludeLib(int GatherFiles, CStr IniSection, CStr IniKey, CStr Directory)
{
    int i;
    CStr CustInc;
    CStr AbsPath;
    CStr BufString;
    int k = 0;

    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey(ProjectType, IniKey + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
        CustInc = IniReadKey(ProjectType, "IMPORT" + IniKey + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectsIniFile);
        if(ProjectRetVal.Len() == 0 && CustInc.Len() == 0) break;
        if(CustInc.Len() != 0)
        {
            if(!GatherFiles)
            {
                AbsPath = Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) CustInc;
                DeleteFile(AbsPath.Get_String());
                BufString = Dirs[DIR_TEMPLATES] + (CStr) "\\" + (CStr) CustInc;
                CopyFile(BufString.Get_String(), AbsPath.Get_String(), 0);
                AbsPath = ChangeAbsolutePaths(AbsPath);
                IniWriteKey(IniSection, IniKey + (CStr) StringNumberComplement(k, 3).Get_String(), AbsPath, Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".med");
                k++;
            }
            else
            {
                // If there's no directory in given file then we know
                // that it's located in [DIR_TEMPLATES] so reconstruct it
                if(FileGetDirectory(CustInc).Len() == 0) CustInc = Dirs[DIR_TEMPLATES] + (CStr) "\\" + CustInc;
                // Just reference it where it is
                IniWriteKey(IniSection, IniKey + (CStr) StringNumberComplement(k, 3).Get_String(), CustInc, Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".med");
                k++;
            }
        }
        if(ProjectRetVal.Len() != 0)
        {
            AbsPath = Directory + (CStr) "\\" + (CStr) ProjectRetVal;
            AbsPath = ChangeAbsolutePaths(AbsPath);
            IniWriteKey(IniSection, IniKey + (CStr) StringNumberComplement(k, 3).Get_String(), AbsPath, Dirs[DIR_PROJECTS] + (CStr) "\\" + (CStr) ProjectTitle + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".med");
            k++;
        }
    }
}

// -----------------------------------------------------------------------
// Open a project file
void OpenProjectAuto(CStr PrjName)
{
    long RetPrjLoad = 0;
    CStr BufString;
    
    if(PrjName.Len() == 0) return;
    if(FileExist(PrjName) == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Project file not found.", MB_ERROR, Requesters);
        AddRecentPrj(PrjName, 0, 0);
        return;
    }
    if(CheckProjectFile(PrjName) == 0)
    {
        BufString = "Not a " + (CStr) AppTitle + (CStr) " project file.";
        MiscMsgBox(hMDIform.hWnd, BufString, MB_ERROR, Requesters);
        AddRecentPrj(PrjName, 0, 0);
        return;
    }
    RetPrjLoad = RealOpenPrj(PrjName);
    if(RetPrjLoad != -1) AddRecentPrj(PrjName, RetPrjLoad, 0);
}

// -----------------------------------------------------------------------
// Save a project file
void SaveProject(void)
{
    HTREEITEM TreeViewChildEntry = 0;
    CStr TVEntryToSave;
    int i = 0;
    CStr CurrentStringFromStrings;
    CStr CurrentStringFromStrings2;
    CStr IconName;
    CStr BufString;

    WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Saving project...");

    // Retrieve eventual constants
    ConstNames.Erase();
    ConstValues.Erase();
    for(i = 0; i < 999; i++)
    {
        CurrentStringFromStrings = IniReadKey("Constants", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectFName);
        if(CurrentStringFromStrings.Len() == 0) break;
        CurrentStringFromStrings2 = IniReadKey("Constants", "Value" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectFName);
        ConstNames.Add(CurrentStringFromStrings.Get_String());
        ConstValues.Add(CurrentStringFromStrings2.Get_String());
    }

    DeleteFile(ProjectFName.Get_String());

    // --- Header
    IniWriteKey("Project", "Name", ProjectTitle, ProjectFName);
    IniWriteKey("Project", "Type", ProjectType, ProjectFName);
    IniWriteKey("Project", "TypeExt", ProjectTypeExt, ProjectFName);
    IniWriteKey("Project", "TypeRename", ProjectPostRename, ProjectFName);
    IniWriteKey("Project", "TypeName", ProjectTypeName, ProjectFName);
    IniWriteKey("Project", "Image", ProjectTypeImg, ProjectFName);

    // --- New header
    IniWriteKey("Project", "Major", Prj_Major, ProjectFName);
    IniWriteKey("Project", "Minor",  Prj_Minor, ProjectFName);
    IniWriteKey( "Project", "Revision",  Prj_Revision, ProjectFName);
    IniWriteKey( "Project", "Release",  Prj_Release, ProjectFName);
    IniWriteKey( "Project", "IncludeVersion",  Prj_IncludeVersion, ProjectFName);
    IniWriteKey( "Project", "IncludeManifest",  Prj_IncludeManifest, ProjectFName);
    IniWriteKey( "Project", "OS",  Prj_OS, ProjectFName);
    IniWriteKey( "Project", "FileType",  Prj_FileType, ProjectFName);
    IniWriteKey( "Project", "Title", ProjectTitle, ProjectFName);

    // Check if icon has been changed
    if(ProjectTypeScheduledImg.Len() != 0)
    {
        IconName = FileGetFileName(ProjectTypeScheduledImg);
        // Check if new icon is same as old one
        BufString = ProjectDir + (CStr) "\\" + (CStr) ProjectTypeImg;
        if(_strcmpi(BufString.Get_String(), ProjectTypeScheduledImg.Get_String()) != 0)
        {
            // Check if new icon actually exists
            if(FileExist(ProjectTypeScheduledImg) != 0)
            {
                // Remove old icon
                BufString = ProjectDir + (CStr) "\\" + (CStr) ProjectTypeImg;
                DeleteFile(BufString.Get_String());
                // Check if new icon to copy is in same directory
                BufString = ProjectDir + (CStr) "\\" + (CStr) IconName;
                if(_strcmpi(BufString.Get_String(), ProjectTypeScheduledImg.Get_String()) != 0)
                {
                    // Copy the new icon file
                    BufString = ProjectDir + (CStr) "\\" + (CStr) IconName;
                    CopyFile(ProjectTypeScheduledImg.Get_String(), BufString.Get_String(), 0);
                }
                // Set the new used global icon
                ProjectTypeImg = IconName;
            } 
        }
    }
    IniWriteKey("Project", "Image", ProjectTypeImg, ProjectFName);
    ProjectTypeScheduledImg = "";

    IniWriteKey( "Project", "Comments", StringReplace(Prj_Comments, Prj_Comments.Chr(13) + Prj_Comments.Chr(10), Prj_Comments.Chr(2), 1, -1, Binary_Compare), ProjectFName);
    IniWriteKey( "Project", "CompanyName", StringReplace(Prj_CompanyName, Prj_CompanyName.Chr(13) + Prj_CompanyName.Chr(10), Prj_CompanyName.Chr(2), 1, -1, Binary_Compare), ProjectFName);
    IniWriteKey( "Project", "FileDescription", StringReplace(Prj_FileDescription, Prj_FileDescription.Chr(13) + Prj_FileDescription.Chr(10), Prj_FileDescription.Chr(2), 1, -1, Binary_Compare), ProjectFName);
    IniWriteKey( "Project", "LegalCopyright", StringReplace(Prj_LegalCopyRight, Prj_LegalCopyRight.Chr(13) + Prj_LegalCopyRight.Chr(10), Prj_LegalCopyRight.Chr(2), 1, -1, Binary_Compare), ProjectFName);
    IniWriteKey( "Project", "LegalTradeMarks", StringReplace(Prj_LegalTrademarks, Prj_LegalTrademarks.Chr(13) + Prj_LegalTrademarks.Chr(10), Prj_LegalTrademarks.Chr(2), 1, -1, Binary_Compare), ProjectFName);
    IniWriteKey( "Project", "ProductName", StringReplace(Prj_ProductName, Prj_ProductName.Chr(13) + Prj_ProductName.Chr(10), Prj_ProductName.Chr(2), 1, -1, Binary_Compare), ProjectFName);

    // Save includes
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewIncludes);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("INCLUDES", "INCLUDE" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }

    // Save libraries
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewLibs);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("LIBS", "LIBRARY" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    
    // Save modules
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewModules);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("MODULES", "MODULE" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
      
    // Save objects
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewObjects);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("OBJECTS", "OBJECT" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
      
    // Save resources
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewResources);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        if(TreeViewChildEntry == hTreeViewIcons) goto SaveNoValidResources;
        else if(TreeViewChildEntry == hTreeViewCursors) goto SaveNoValidResources;
        else if(TreeViewChildEntry == hTreeViewBitmaps) goto SaveNoValidResources;
        else if(TreeViewChildEntry == hTreeViewStrings) goto SaveNoValidResources;
        else if(TreeViewChildEntry == hTreeViewAccelerators) goto SaveNoValidResources;
        else if(TreeViewChildEntry == hTreeViewMenus) goto SaveNoValidResources;
        else if(TreeViewChildEntry == hTreeViewDialogs) goto SaveNoValidResources;
        else if(TreeViewChildEntry == hTreeViewRawdatas) goto SaveNoValidResources;
        else
        {
            TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
            TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
            IniWriteKey("RESOURCES", "RESOURCE" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
            i++;
        }
SaveNoValidResources:
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
    }
    
    // Save icons
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewIcons);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("ICONS", "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), GetLabelFromTreeView(TreeViewChildEntry).Get_String(), ProjectFName);
        IniWriteKey("ICONS", "ICON" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        IniWriteKey("ICONS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), GetIconProps(i), ProjectFName);
        IniWriteKey("ICONS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), GetIconLang(i), ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }

    // Save cursors
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewCursors);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("CURSORS", "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), GetLabelFromTreeView(TreeViewChildEntry).Get_String(), ProjectFName);
        IniWriteKey("CURSORS", "CURSOR" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        IniWriteKey("CURSORS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), GetCursorProps(i), ProjectFName);
        IniWriteKey("CURSORS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), GetCursorLang(i), ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    
    // Save bitmaps
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewBitmaps);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("BITMAPS", "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), GetLabelFromTreeView(TreeViewChildEntry).Get_String(), ProjectFName);
        IniWriteKey("BITMAPS", "BITMAP" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        IniWriteKey("BITMAPS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), GetBitmapProps(i), ProjectFName);
        IniWriteKey("BITMAPS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), GetBitmapLang(i), ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    
    // Save strings
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewStrings);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("STRINGS", "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), GetLabelFromTreeView(TreeViewChildEntry).Get_String(), ProjectFName);
        IniWriteKey("STRINGS", "STRING" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        IniWriteKey("STRINGS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), Get_StringProps(i), ProjectFName);
        IniWriteKey("STRINGS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), Get_StringLang(i), ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    
    // Save accelerators
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewAccelerators);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("ACCELERATORS", "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), GetLabelFromTreeView(TreeViewChildEntry).Get_String(), ProjectFName);
        IniWriteKey("ACCELERATORS", "ACCELERATOR" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        IniWriteKey("ACCELERATORS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), GetAcceleratorProps(i), ProjectFName);
        IniWriteKey("ACCELERATORS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), GetAcceleratorLang(i), ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    
    // Save menus
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewMenus);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("MENUS", "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), GetLabelFromTreeView(TreeViewChildEntry).Get_String(), ProjectFName);
        IniWriteKey("MENUS", "MENU" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        IniWriteKey("MENUS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), GetMenuProps(i), ProjectFName);
        IniWriteKey("MENUS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), GetMenuLang(i), ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    
    // Save dialogs
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewDialogs);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("DIALOGS", "DIALOG" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    
    // Save raw datas
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewRawdatas);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("RAWDATAS", "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), GetLabelFromTreeView(TreeViewChildEntry).Get_String(), ProjectFName);
        IniWriteKey("RAWDATAS", "RAWDATA" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        IniWriteKey("RAWDATAS", "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), GetRawdataProps(i), ProjectFName);
        IniWriteKey("RAWDATAS", "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), GetRawdataLang(i), ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }
    
    // Save exports
    if(hTreeViewDefs != 0)
    {
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewDefs);
        if(TreeViewChildEntry != 0)
        {
            TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
            TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
            IniWriteKey("DEFS", "DEF", TVEntryToSave, ProjectFName);
        }
    }
    
    // Save texts
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewTexts);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TVEntryToSave = ChangeAbsolutePaths(TVEntryToSave, 1);
        IniWriteKey("TEXTS", "TEXT" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave, ProjectFName);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        i++;
    }

    // Restore the constants
    for(i = 0; i < ConstNames.Amount(); i++)
    {
        IniWriteKey("Constants", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), ConstNames.Get(i)->Content, ProjectFName);
        IniWriteKey("Constants", "Value" + (CStr) StringNumberComplement(i, 3).Get_String(), ConstValues.Get(i)->Content, ProjectFName);
    }
    ConstNames.Erase();
    ConstValues.Erase();

    IniDeleteKey("Windows", "", ProjectFName);
    if(IniReadBoolKey("Layout", "SaveProjectState", MainIniFile))
    {
        SaveAllChildsInProject(hMDIform.hClient);
    }
      
    // Store the new writing date
    FileGetWriteTime(ProjectFName, &ProjectFDate);

    ProjectModified = FALSE;
    WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Done.");
    AddRecentPrj(ProjectFName, 1, 0);
}

// -----------------------------------------------------------------------
// Record opened source windows that belong to a project
void SaveAllChildsInProject(HWND hWnd)
{
    ProjectFileNumber = 0;
    if(NbForms != 0) EnumChildWindows(hWnd, &EnumSaveAllChildsInProject, 0);
}
                    
int CALLBACK EnumSaveAllChildsInProject(HWND hWnd, long lParam)
{
    CStr FileName;
    if(NbForms == 0)
    {
        return(FALSE);
    }
    else
    {
        if(ControlIsWindowChild(hWnd) == 1)
        {
            if(GetWindowLong(hWnd, GWL_USERDATA) != 0)
            {
                ChildStruct = LoadStructure(hWnd);
                if(ChildStruct->FileLoaded != 0)
                {
                    if(ChildStruct->RFile->Len() != 0)
                    {
                        FileName = ChildStruct->RFile->Left(ChildStruct->RFile->Len()).Get_String();
                        if(IsFileInProject(FileName.Get_String()))
                        {
                            FileName = ChangeAbsolutePaths(FileName, TRUE);
                            IniWriteKey("Windows", "Window" + (CStr) StringNumberComplement(ProjectFileNumber, 3).Get_String(), FileName, ProjectFName);
                            // Check if this file is the on on focus
                            if(hWnd == CurrentForm)
                            {
                                IniWriteKey("Windows", "Focus", FileName, ProjectFName);
                                IniWriteKey("Windows", "FocusLine", GetCurrentLineNumber(ChildStruct->hChildCodeMax), ProjectFName);
                            }
                            ProjectFileNumber++;
                        }
                    }
                }
            }
        }
        return(TRUE);
    }
}

// -----------------------------------------------------------------------
// Check for project file validity
// Returns 0 if file is not a valid project file.
// Returns 1 if file is not an old MAsmEd project file.
// Returns 2 if file is not a valid project file.
long CheckProjectFile(CStr FileName)
{
    HANDLE PrjFileHandle = 0;
    unsigned long PrjFileRead = 0;
    CStr PrjHeader;
    CStr BufString;
    
    PrjHeader = PrjHeader.String(15,1);
    if(FileExist(FileName) == 1)
    {
        if(FileIsDirectory(FileName) == 0)
        {
            PrjFileHandle = FileOpenR(FileName);
            if(PrjFileHandle != INVALID_HANDLE_VALUE)
            {
                ReadFile(PrjFileHandle, PrjHeader.Get_String(), 15, &PrjFileRead, 0);
                FileClose(PrjFileHandle);
            }
            // Is it an old project ?
            BufString = "[MASMEDPROJECT]";
            if(_strcmpi(PrjHeader.Get_String(),BufString.Get_String()) == 0) return(1);

            PrjHeader = PrjHeader.String(9,1);
            PrjFileHandle = FileOpenR(FileName);
            if(PrjFileHandle != INVALID_HANDLE_VALUE)
            {
                ReadFile(PrjFileHandle, PrjHeader.Get_String(), 9, &PrjFileRead, 0);
                FileClose(PrjFileHandle);
            }
            // New style project ?
            BufString = "[PROJECT]";
            if(_strcmpi(PrjHeader.Get_String(),BufString.Get_String()) == 0) return(2);
        }
    }
    return(0);
}

// -----------------------------------------------------------------------
// Open a project file
long RealOpenPrj(CStr PrjName)
{
    int i = 0;
    CStr EntryToAdd;
    CStr PrjHeaderKey;
    CStr Original_Name;
    CStr Lib_Dir;

    if(ProjectOn)
    {
        if(ProjectModified)
        {
            switch(MiscMsgBox(hMDIform.hWnd, "Current project modified. Save it now ?", MB_ICONQUESTION | MB_YESNOCANCEL, Requesters))
            {
                case IDYES:
                    MCMD_SaveProject();
                    break;
                case IDNO:
                    ProjectModified = FALSE;
                    break;
                case IDCANCEL:
                    return(-1);
            }
        }
    }
    if(ProjectOn) MCMD_CloseAll();
    
    PossibleMissingFiles.Erase();
    PossibleMissingFilesTree.Erase();
    PossibleMissingFilesResName.Erase();
    PossibleMissingFilesResProp.Erase();
    PossibleMissingFilesResLang.Erase();
    
    // Save project name (to resave it)
    ProjectFName = PrjName;
    ProjectDir = FileGetDirectory(PrjName);
    if(strcmp(ProjectDir.Right(1).Get_String(), "\\") == 0) ProjectDir = ProjectDir.Left(ProjectDir.Len() - 1);

    PrjHeaderKey = "Project";
    // Check if we should open it as an old MAsmEd project
    if(CheckProjectFile(PrjName) == 1) PrjHeaderKey = "MAsmEdProject";
    
    // Read project header
    ProjectTypeExt = IniReadKey(PrjHeaderKey, "TypeExt", PrjName);
    if(ProjectTypeExt.Len() == 0) goto MangledOpPrj;
    ProjectTypeName = IniReadKey(PrjHeaderKey, "TypeName", PrjName);
    if(ProjectTypeName.Len() == 0) goto MangledOpPrj;
    ProjectTypeImg = IniReadKey(PrjHeaderKey, "Image", PrjName);
    ProjectType = IniReadKey(PrjHeaderKey, "Type", PrjName);
    if(ProjectType.Len() == 0) goto MangledOpPrj;
    ProjectTitle = IniReadKey(PrjHeaderKey, "Name", PrjName);
    if(ProjectTitle.Len() == 0) goto MangledOpPrj;
    // Can be empty
    ProjectPostRename = IniReadKey(PrjHeaderKey, "TypeRename", PrjName);
    
    // --- New header
    Prj_Major = 0;
    Prj_Minor = 0;
    Prj_Revision = 0;
    Prj_Release = 0;
    Prj_IncludeVersion = 1;
    Prj_IncludeManifest = 0;
    Prj_FileType = 1;
    Prj_OS = 4;
    Prj_Comments = "";
    Prj_CompanyName = "";
    Prj_FileDescription = "";
    Prj_LegalCopyRight = "";
    Prj_LegalTrademarks = "";
    Prj_ProductName = "";
    ProjectTypeScheduledImg = "";
    ProjectRetVal = IniReadKey(PrjHeaderKey, "Major", PrjName);
    if(ProjectRetVal.Len() != 0) Prj_Major = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(PrjHeaderKey, "Minor", PrjName);
    if(ProjectRetVal.Len() != 0) Prj_Minor = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(PrjHeaderKey, "Revision", PrjName);
    if(ProjectRetVal.Len() != 0) Prj_Revision = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(PrjHeaderKey, "Release", PrjName);
    if(ProjectRetVal.Len() != 0) Prj_Release = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(PrjHeaderKey, "IncludeVersion", PrjName);
    if(ProjectRetVal.Len() != 0) Prj_IncludeVersion = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(PrjHeaderKey, "IncludeManifest", PrjName);
    if(ProjectRetVal.Len() != 0) Prj_IncludeManifest = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(PrjHeaderKey, "OS", PrjName);
    if(ProjectRetVal.Len() != 0) Prj_OS = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(PrjHeaderKey, "FileType", PrjName);
    if(ProjectRetVal.Len() != 0) Prj_FileType = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(PrjHeaderKey, "Title", PrjName);
    if(ProjectRetVal.Len() != 0) ProjectTitle = ProjectRetVal;
    
    ProjectRetVal = StringReplace(IniReadKey(PrjHeaderKey, "Comments", PrjName), ProjectRetVal.Chr(2), ProjectRetVal.Chr(13) + ProjectRetVal.Chr(10), 1, -1, Binary_Compare);
    if(ProjectRetVal.Len() != 0) Prj_Comments = ProjectRetVal;
    ProjectRetVal = StringReplace(IniReadKey(PrjHeaderKey, "CompanyName", PrjName), ProjectRetVal.Chr(2), ProjectRetVal.Chr(13) + ProjectRetVal.Chr(10), 1, -1, Binary_Compare);
    if(ProjectRetVal.Len() != 0) Prj_CompanyName = ProjectRetVal;
    ProjectRetVal = StringReplace(IniReadKey(PrjHeaderKey, "FileDescription", PrjName), ProjectRetVal.Chr(2), ProjectRetVal.Chr(13) + ProjectRetVal.Chr(10), 1, -1, Binary_Compare);
    if(ProjectRetVal.Len() != 0) Prj_FileDescription = ProjectRetVal;
    ProjectRetVal = StringReplace(IniReadKey(PrjHeaderKey, "LegalCopyright", PrjName), ProjectRetVal.Chr(2), ProjectRetVal.Chr(13) + ProjectRetVal.Chr(10), 1, -1, Binary_Compare);
    if(ProjectRetVal.Len() != 0) Prj_LegalCopyRight = ProjectRetVal;
    ProjectRetVal = StringReplace(IniReadKey(PrjHeaderKey, "LegalTradeMarks", PrjName), ProjectRetVal.Chr(2), ProjectRetVal.Chr(13) + ProjectRetVal.Chr(10), 1, -1, Binary_Compare);
    if(ProjectRetVal.Len() != 0) Prj_LegalTrademarks = ProjectRetVal;
    ProjectRetVal = StringReplace(IniReadKey(PrjHeaderKey, "ProductName", PrjName), ProjectRetVal.Chr(2), ProjectRetVal.Chr(13) + ProjectRetVal.Chr(10), 1, -1, Binary_Compare);
    if(ProjectRetVal.Len() != 0) Prj_ProductName = ProjectRetVal;
    
    CreateProjectStruct();
    EraseResArrays();

    // Read includes definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("INCLUDES", "INCLUDE" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        if(CheckMissingFile(EntryToAdd, TRUE, hTreeViewIncludes, "", 0, 0))
        {
            EntryToAdd = FileGetFileName(EntryToAdd).Get_String() + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
            TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewIncludes, 0, ICON_NEW, ICON_NEW, 0, 0);
        }
    }

    // Read libs definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("LIBS", "LIBRARY" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        Original_Name = EntryToAdd;
        if(!CheckMissingFile(EntryToAdd, FALSE, hTreeViewLibs, "", 0, 0))
        {
            // File not found: modify it and test again
            if(_strcmpi(EntryToAdd.Extract_FileName_Extension().Get_String(), "a") == 0)
            {
                EntryToAdd = EntryToAdd.Replace_FileName_Extension("lib");
                Lib_Dir = EntryToAdd.Extract_Directory();
                EntryToAdd = EntryToAdd.Extract_FileName();
                // libxxx.a ?
                if(_strcmpi(EntryToAdd.Left(3).Get_String(), "lib") == 0) EntryToAdd = EntryToAdd.Mid(4);
                EntryToAdd = Lib_Dir + EntryToAdd.Extract_FileName();
            }
            else if(_strcmpi(EntryToAdd.Extract_FileName_Extension().Get_String(), "lib") == 0)
            {
                EntryToAdd = EntryToAdd.Replace_FileName_Extension("a");
                Lib_Dir = EntryToAdd.Extract_Directory();
                EntryToAdd = Lib_Dir + "lib" + EntryToAdd.Extract_FileName();
            }
            if(CheckMissingFile(EntryToAdd, FALSE, hTreeViewLibs, "", 0, 0))
            {
                // Modified
                ProjectModified = TRUE;
                // Only report it if modified
                WriteToStatus("File: '" + Original_Name + (CStr) "' changed into: '" + EntryToAdd + (CStr) "'.");
                EntryToAdd = FileGetFileName(EntryToAdd).Get_String() + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
                TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewLibs, 0, ICON_STATIC, ICON_STATIC, 0, 0);
            }
            else
            {
                // Report original filename
                PossibleMissingFiles.Add(Original_Name.Get_String());
                PossibleMissingFilesTree.Add(hTreeViewLibs);
                PossibleMissingFilesResName.Add("");
                PossibleMissingFilesResProp.Add(0L);
                PossibleMissingFilesResLang.Add(0L);
            }
        }
        else
        {
            EntryToAdd = FileGetFileName(EntryToAdd).Get_String() + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
            TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewLibs, 0, ICON_STATIC, ICON_STATIC, 0, 0);
        }
    }

    // Read modules definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("MODULES", "MODULE" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        if(CheckMissingFile(EntryToAdd, TRUE, hTreeViewModules, "", 0, 0))
        {
            EntryToAdd = FileGetFileName(EntryToAdd).Get_String() + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
            TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewModules, 0, ICON_NEW, ICON_NEW, 0, 0);
        }

    }

    // Read objects definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("OBJECTS", "OBJECT" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);

        Original_Name = EntryToAdd;
        if(!CheckMissingFile(EntryToAdd, FALSE, hTreeViewObjects, "", 0, 0))
        {
            // File not found: modify it and test again
            if(_strcmpi(EntryToAdd.Extract_FileName_Extension().Get_String(), "o") == 0)
            {
                EntryToAdd = EntryToAdd.Replace_FileName_Extension("obj");
            }
            else if(_strcmpi(EntryToAdd.Extract_FileName_Extension().Get_String(), "obj") == 0)
            {
                EntryToAdd = EntryToAdd.Replace_FileName_Extension("o");
            }
            if(CheckMissingFile(EntryToAdd, FALSE, hTreeViewObjects, "", 0, 0))
            {
                // Modified
                ProjectModified = TRUE;
                // Only report it if modified
                WriteToStatus("File: '" + Original_Name + (CStr) "' changed into: '" + EntryToAdd + (CStr) "'.");
                EntryToAdd = FileGetFileName(EntryToAdd).Get_String() + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
                TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewObjects, 0, ICON_OBJECT, ICON_OBJECT, 0, 0);
            }
            else
            {
                // Report original filename
                PossibleMissingFiles.Add(Original_Name.Get_String());
                PossibleMissingFilesTree.Add(hTreeViewObjects);
                PossibleMissingFilesResName.Add("");
                PossibleMissingFilesResProp.Add(0L);
                PossibleMissingFilesResLang.Add(0L);
            }
        }
        else
        {
            EntryToAdd = FileGetFileName(EntryToAdd).Get_String() + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
            TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewObjects, 0, ICON_OBJECT, ICON_OBJECT, 0, 0);
        }
    }

    // Read old resources definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("RESOURCES", "RESOURCE" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);

        if(CheckMissingFile(EntryToAdd, TRUE, hTreeViewResources, "", 0, 0))
        {
            EntryToAdd = FileGetFileName(EntryToAdd).Get_String() + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
            TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewResources, 0, ICON_RES, ICON_RES, 0, 0);
        }
    }

    // Read icons definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("ICONS", "ICON" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        ReadResourceFromProject(PrjName, EntryToAdd, "ICONS", i, "ICON_", hTreeViewIcons, &AddIconInArray);
    }

    // Read cursors definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("CURSORS", "CURSOR" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        ReadResourceFromProject(PrjName, EntryToAdd, "CURSORS", i, "CURSOR_", hTreeViewCursors, &AddCursorInArray);
    }

    // Read bitmaps definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("BITMAPS", "BITMAP" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        ReadResourceFromProject(PrjName, EntryToAdd, "BITMAPS", i, "BITMAP_", hTreeViewBitmaps, &AddBitmapInArray);
    }

    // Read strings definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("STRINGS", "STRING" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        ReadResourceFromProject(PrjName, EntryToAdd, "STRINGS", i, "STRINGS_", hTreeViewStrings, &AddStringInArray);
    }

    // Read accelerators definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("ACCELERATORS", "ACCELERATOR" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        ReadResourceFromProject(PrjName, EntryToAdd, "ACCELERATORS", i, "ACCELERATORS_", hTreeViewAccelerators, &AddAcceleratorInArray);
    }

    // Read menus definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("MENUS", "MENU" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        ReadResourceFromProject(PrjName, EntryToAdd, "MENUS", i, "MENUS_", hTreeViewMenus, &AddMenuInArray);
    }

    // Read dialogs definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("DIALOGS", "DIALOG" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        ReadResourceFromProject(PrjName, EntryToAdd, "DIALOGS", i, "DIALOG_", hTreeViewDialogs, &AddDialogInArray);
    }

    // Read rcdatas definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("RAWDATAS", "RAWDATA" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        ReadResourceFromProject(PrjName, EntryToAdd, "RAWDATAS", i, "RAWDATAS_", hTreeViewRawdatas, &AddRawDatasInArray);
    }

    if(hTreeViewDefs != 0)
    {
        // Read exports definitions
        ProjectRetVal = IniReadKey("DEFS", "DEF", PrjName);
        if(ProjectRetVal.Len() != 0)
        {
            EntryToAdd = ProjectRetVal;
            EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
            if(CheckMissingFile(EntryToAdd, TRUE, hTreeViewDefs, "", 0, 0))
            {
                EntryToAdd = FileGetFileName(EntryToAdd).Get_String() + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
                TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewDefs, 0, ICON_NEW, ICON_NEW, 0, 0);
            }
        }
    }

    // Read texts definitions
    for(i = 0; i <= 999; i++)
    {
        ProjectRetVal = IniReadKey("TEXTS", "TEXT" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
        if(ProjectRetVal.Len() == 0) break;
        EntryToAdd = ProjectRetVal;
        EntryToAdd = ChangeRelativePaths(EntryToAdd, ProjectDir);
        if(CheckMissingFile(EntryToAdd, TRUE, hTreeViewTexts, "", 0, 0))
        {
            EntryToAdd = FileGetFileName(EntryToAdd) + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")";
            TreeViewAddItem(hTreeView, EntryToAdd, hTreeViewTexts, 0, ICON_NEW, ICON_NEW, 0, 0);
        }
    }
    SwitchProjectOn();

    if(IniReadBoolKey("Layout", "SaveProjectState", MainIniFile))
    {
        // Try to reload the files that wheres saved within the project
        for(i = 0; i <= 999; i++)
        {
            ProjectRetVal = IniReadKey("Windows", "Window" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
            if(ProjectRetVal.Len() == 0) break;
            ProjectRetVal = ChangeRelativePaths(ProjectRetVal);
            // Open the file without adding it in recent list
            OpenUnknownFile(ProjectRetVal, FALSE);
        }
        ProjectRetVal = IniReadKey("Windows", "Focus", PrjName);
        if(ProjectRetVal.Len() != 0)
        {
            ProjectRetVal = ChangeRelativePaths(ProjectRetVal);
            GoToLine(ProjectRetVal, IniReadKey("Windows", "FocusLine", PrjName).Get_Long(), TRUE);
        }
    } 

    if(PossibleMissingFiles.Amount() != 0)
    {
        CreateModalDialog(-1, -1, 402, 204, hMDIform.hWnd, &FRMMissingFiles, WS_BORDER | WS_CAPTION | WS_SYSMENU, 1);
    }

    // Store the new writing date
    FileGetWriteTime(ProjectFName, &ProjectFDate);
    return(1);

MangledOpPrj:
    // Clear everything
    if(hTreeViewRoot != 0) TreeViewRemoveItem(hTreeView, hTreeViewRoot);
    ProjectModified = FALSE;
    ProjectOn = FALSE;
    MiscMsgBox(hMDIform.hWnd, "Corrupted project file.", MB_ERROR, Requesters);
    return(0);
}

// -----------------------------------------------------------------------
// Set the project ambient
void SwitchProjectOn(void)
{
    ShowProjetDockWin = 1;
    SetProjectDockState(ShowProjetDockWin);
    ProjectModified = FALSE;
    ProjectOn = TRUE;
}


// -----------------------------------------------------------------------
// Read a resource from a project file
void ReadResourceFromProject(CStr PrjName, CStr EntryToAdd, CStr ResourceName, int i, CStr ResourceDefaultLabel,
                             HTREEITEM hTreeItem, void (*AddInArray)(long Data, long Langu))
{
    CStr EntryToAddResName;
    long EntryProp = 0;
    long EntryLang = 0;

    ProjectRetVal = IniReadKey(ResourceName, "NAME" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
    if(ProjectRetVal.Len() == 0) EntryToAddResName = ResourceDefaultLabel + (CStr) i;
    else EntryToAddResName = ProjectRetVal;
    ProjectRetVal = IniReadKey(ResourceName, "PROP" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
    if(ProjectRetVal.Len() == 0) ProjectRetVal = "15";
    EntryProp = ProjectRetVal.Get_Long();
    ProjectRetVal = IniReadKey(ResourceName, "LANG" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjName);
    if(ProjectRetVal.Len() == 0) ProjectRetVal = "0";
    EntryLang = ProjectRetVal.Get_Long();
    if(CheckMissingFile(EntryToAdd, TRUE, hTreeItem, EntryToAddResName, EntryProp, EntryLang))
    {
        AddInArray(EntryProp, EntryLang);
        TreeViewAddItem(hTreeView, EntryToAddResName + (CStr) " (" + (CStr) EntryToAdd + (CStr) ")", hTreeItem, 0, ICON_RES, ICON_RES, 0, 0);
    }
}

// -----------------------------------------------------------------------
// Check if a file is in a project
// Return -1 if not found or file index if found
long IsInProject(CStr FileToSearch, long IncludeTexts)
{
    long ReturnValue = -1;
    CStr RealTVToSearch;
 
    RealTVToSearch = FileGetFileName(FileToSearch);
    RealTVToSearch = RealTVToSearch + (CStr) " (" + (CStr) FileToSearch.Get_String() + (CStr) ")";
    ReturnValue = TreeViewSearchItemText(hTreeView, hTreeViewIncludes, RealTVToSearch);
    if(ReturnValue != -1) return(ReturnValue);
    ReturnValue = TreeViewSearchItemText(hTreeView, hTreeViewModules, RealTVToSearch);
    if(ReturnValue != -1) return(ReturnValue);
    if(IncludeTexts == 1) ReturnValue = TreeViewSearchItemText(hTreeView, hTreeViewTexts, RealTVToSearch);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the number of modules included in a project
long GetModulesNumber(void)
{
    long ReturnValue = 0;
    HTREEITEM TreeViewChildEntry;
 
    // Enumerate treeview childs
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewModules);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        ReturnValue++;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Build a project (use skins engine)
void BuildProject(long BuildType)
{
    CStr ProjectDir;
    CStr ResFileToInclude;
    HANDLE ResourcesFile = 0;
    CStr SkinVal;
    CStr SkinValB;
    CStr SkinValTextBox;
    long SkinOption = 0;
    CStr SkinOptionDat;
    HANDLE DllDefFile = 0;
    CStr DefVal;
    HTREEITEM TreeViewChildEntry = 0;
    CStr TVEntryToSave;
    long *TempArguments = 0;
    CStr TempskinVal;
    CStr LibLinkFound;
    long PosLibName = 0;
    CStr TempInclude;
    CStr UseVInf;
    CStr AddLibToLink;
    CStr ConstantType;
    long ObjIndex = 0;
    CStr ResourceHead;
    CStr StringStr;
    CStr AcceleratorKeys;
    CStr AcceleratorCmd;
    long PosInAccelKey = 0;
    long AccelCtrlState = 0;
    long AccelAltState = 0;
    long AccelShiftState = 0;
    CStr AccelKey;
    CStr CtrlAltShiftStr;
    CStr StrToAddToAccel;
    int DisabledImports;
    int i = 0;
    int j = 0;
    int k = 0;
    int L = 0;
    CStr BuildSteps;
    CStr RespSep;
    CStr ResponseEntry;
    CStr UserObjects;
    CStr LibForeChar;
    CStr LibBackChar;
    CStr LibRetrieveChar;
    CStr AddResLanguage;
    long AddResLanguageLng = 0;
    CStr HeadLib;
    CStr PrefixLib;
    CStr PreInc;
    CStr InInc;
    CStr PostInc;
    CStr EarlyInclude;
    CStr PascalName;
    CStr StopOnError;
    CStr CurrDOSCom;
    long NoIncludePascal = 0;
    long NoLinkCom = 0;
    CStr UseRawComma;
    long BolUseRawComma = 0;
    CStr DirectObjects;
    CStr DllEntryPoint;
    //CStr ResRealExt;
    CStr PostRealName;
    long PosAsm = 0;
    long LineAsm = 0;
    CStr TrapStdErr;
    CStr ObjLstPrefix;
    CStr BufString;
    CStr CodePrologue;
    CStr CodeEpilogue;
    HANDLE ManifestFile;
    CStr ManifestName;
    CStr ManifestDatas;
    long ReadCodeBytes;
    int ExtraCodeBytes;
    CStr EndProjectFileName;

    SkinArrayVar.Erase();
    SkinArrayDat.Erase();
    PrjDebugLink = "";
    // Retrieve current build file
    PrjBuildIniFile = IniReadKey("BuildSkins", "CurrentSkin", SkinsIniFile);
    PrjBuildIniFile = ChangeRelativePaths(PrjBuildIniFile);
    
    if(GetModulesNumber() == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Can't proceed: no module file in project.", MB_ERROR, Requesters);
        return;
    }
    
    if(IniReadKey("LINKER", "Prog" + (CStr) ProjectTypeExt + (CStr) "000", PrjBuildIniFile).Len() == 0)
    {
        MiscMsgBox(hMDIform.hWnd, "Can't proceed: project type is unavailable for selected skin.", MB_ERROR, Requesters);
        return;
    }
    
    ProjectDir = FileGetDirectory(ProjectFName);
    if(strcmp(ProjectDir.Right(1).Get_String(), "\\") == 0) ProjectDir = ProjectDir.Left(ProjectDir.Len() - 1);
    WriteComment("Building project...");
    CursorSetWait();
    MCMD_SaveAll();
    MCMD_SaveProject();
    
    if(AutoClearOut == 1) ClearStatus();
    WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Building skin: " + (CStr) PrjBuildIniFile);
    WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Creating project: " + (CStr) ProjectFName + (CStr) "...");
    

    BufString = ProjectTypeExt.Upper_Case();
    if(BufString == "WINLIB")
    {
        EndProjectFileName = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".lib";
    }
    else if(BufString == "WINDLL")
    {
        EndProjectFileName = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".dll";
    }
    else if(BufString == "WINSYS")
    {
        EndProjectFileName = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".sys";
    }
    else
    {
        EndProjectFileName = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".exe";
    }

    BufString = EndProjectFileName;
    if(ProjectPostRename.Len()) BufString = FileReplaceExtension(PostRealName, ProjectPostRename);
    DeleteFile(BufString.Get_String());

    WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Building resources...");
    
    ResourcesFile = FileCreateEmpty(ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".trc", NO_SECURITY_DESCRIPTOR);
    // Put accelerators header equates
    WriteAcceleratorEquates(ResourcesFile);
    // Include application icon
    FileWriteLine(ResourcesFile, "500 ICON MOVEABLE PURE LOADONCALL DISCARDABLE " + (CStr) ResIncludeFile(ProjectTypeImg).Get_String());
    
    AddResLanguage = IniReadKey("RES", "AddResLanguage", PrjBuildIniFile);
    if(AddResLanguage.Len() == 0) AddResLanguage = "0";
    AddResLanguageLng = AddResLanguage.Get_Long();
    
    PutExtendedRes = IniReadKey("RES", "ExtendedRes", PrjBuildIniFile);
    if(PutExtendedRes.Len() == 0) PutExtendedRes = "0";
    if(PutExtendedRes.Get_Long() == 0) WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "*** Warning: extended menus resources are not available.");
    
    // Include version info
    UseVInf = IniReadKey("RES", "AddVInfo", PrjBuildIniFile);
    if(UseVInf.Len() == 0) UseVInf = "0";
    if(Prj_IncludeVersion != 0)
    {
        if(UseVInf.Get_Long() == 1) Project_Write_Version_Res(ResourcesFile);
    }
            
    // Primary object number
    ObjIndex = 501;
    // TODO : dialogs
    
    // Icons
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewIcons);
    i = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        FileWriteLine(ResourcesFile, ObjIndex + (CStr) " ICON " + (CStr) CalcResourceHeader(GetIconProps(i)).Get_String() + (CStr) " " + (CStr) ResIncludeFile(TVEntryToSave).Get_String());
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        ObjIndex++;
    }
      
    // Cursors
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewCursors);
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        FileWriteLine(ResourcesFile, ObjIndex + (CStr) " CURSOR " + (CStr) CalcResourceHeader(GetCursorProps(i)).Get_String() + (CStr) " " + (CStr) ResIncludeFile(TVEntryToSave).Get_String());
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        ObjIndex++;
    }
      
    // Bitmaps
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewBitmaps);
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        FileWriteLine(ResourcesFile, ObjIndex + (CStr) " BITMAP " + (CStr) CalcResourceHeader(GetBitmapProps(i)).Get_String() + (CStr) " " + (CStr) ResIncludeFile(TVEntryToSave).Get_String());
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        ObjIndex++;
    }

    // Strings
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewStrings);
    i = 0;
    k = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        FileWriteLine(ResourcesFile, WriteLanguageEquate(Get_StringLang(k)).Get_String());
        FileWriteLine(ResourcesFile, "STRINGTABLE");
        FileWriteLine(ResourcesFile, CalcResourceHeader(Get_StringProps(k)).Get_String());
        if(AddResLanguageLng == 1) FileWriteLine(ResourcesFile, CalcResourceLanguage(Get_StringLang(k)).Get_String());
        FileWriteLine(ResourcesFile, "BEGIN");
        for(j = 0; j <= 15; j++)
        {
            StringStr = IniReadKey("STRDAT", "STR" + (CStr) StringNumberComplement(j, 3).Get_String(), TVEntryToSave);
            if(StringStr.Len() != 0) FileWriteLine(ResourcesFile, (CStr) i + (CStr) ",\"" + (CStr) ResIncludeString(StringStr).Get_String() + (CStr) "\"");
            i++;
        }
        FileWriteLine(ResourcesFile, "END");
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        ObjIndex++;
        k++;
    }

    // Accelerators
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewAccelerators);
    i = 0;
    k = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        FileWriteLine(ResourcesFile, WriteLanguageEquate(GetAcceleratorLang(k)).Get_String());
        if(AddResLanguageLng == 1)
        {
            FileWriteLine(ResourcesFile, ObjIndex + (CStr) " ACCELERATORS " + (CStr) CalcResourceHeader(GetAcceleratorProps(k)).Get_String());
            FileWriteLine(ResourcesFile, CalcResourceLanguage(GetAcceleratorLang(k)).Get_String());
        }
        else
        {
            FileWriteLine(ResourcesFile, ObjIndex + (CStr) " ACCELERATORS");
        }
        FileWriteLine(ResourcesFile, "BEGIN");
        for(i = 0; i <= 999; i++)
        {
            AcceleratorKeys = IniReadKey("ACCELDAT", "ACCEL" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave);
            AcceleratorCmd = IniReadKey("ACCELDAT", "COMMAND" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave);
            if(AcceleratorKeys.Len() == 0) break;
            if(AcceleratorCmd.Len() == 0) break;
            AccelCtrlState = 0;
            AccelAltState = 0;
            AccelShiftState = 0;
            PosInAccelKey = 1;
            StrToAddToAccel = "\"";
            AccelKey = "";
GetAccelerator:
            BufString = AcceleratorKeys.Mid(PosInAccelKey, 5);
            if(BufString == "CTRL ")
            {
                AccelCtrlState = 1;
                PosInAccelKey = PosInAccelKey + 7;
                goto GetAccelerator;
            }
            else if(BufString == "ALT +")
            {
                AccelAltState = 1;
                PosInAccelKey = PosInAccelKey + 6;
                goto GetAccelerator;
            }
            else if(BufString == "SHIFT")
            {
                AccelShiftState = 1;
                PosInAccelKey = PosInAccelKey + 8;
                goto GetAccelerator;
            }
            else
            {
                AccelKey = AcceleratorKeys.Mid(PosInAccelKey).Trim();
            }
            CtrlAltShiftStr = "";
            if(AccelCtrlState != 0) CtrlAltShiftStr = "CONTROL";
            if(AccelShiftState != 0)
            {
                if(CtrlAltShiftStr.Len() != 0) CtrlAltShiftStr = CtrlAltShiftStr + ",";
                CtrlAltShiftStr = CtrlAltShiftStr + "SHIFT";
            }
            if(AccelAltState != 0)
            {
                if(CtrlAltShiftStr.Len() != 0) CtrlAltShiftStr = CtrlAltShiftStr + ",";
                CtrlAltShiftStr = CtrlAltShiftStr + "ALT";
            }
            if(CtrlAltShiftStr.Len() != 0) CtrlAltShiftStr = CtrlAltShiftStr + ",";
            for(j = 0; j <= 84; j++)
            {
                if(_strcmpi(TabAccelerators[j], AccelKey.Get_String()) == 0)
                {
                    StrToAddToAccel = "";
                    break;
                }
            }
            AccelKey = StrToAddToAccel + (CStr) AccelKey + (CStr) StrToAddToAccel;
            FileWriteLine(ResourcesFile, AccelKey + (CStr) "," + (CStr) AcceleratorCmd + (CStr) "," + (CStr) CtrlAltShiftStr + (CStr) "VIRTKEY");
        }
        FileWriteLine(ResourcesFile, "END");
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        ObjIndex++;
    }

    // Menus
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewMenus);
    k = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        ResMenusString = ConvertMenusRes(TVEntryToSave, ObjIndex, k);
        FileWriteBufferVB(ResourcesFile, ResMenusString, ResMenusString.Len());
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        ObjIndex++;
        k++;
    }
    
    // Raw datas
    UseRawComma = IniReadKey("RES", "UseRawComma", PrjBuildIniFile);
    if(UseRawComma.Len() == 0) UseRawComma = "0";
    BolUseRawComma = 0;
    if(_strcmpi(UseRawComma.Get_String(), "1") == 0) BolUseRawComma = 1;
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewRawdatas);
    k = 0;
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        if(Convert_Raw(TVEntryToSave, ObjIndex, k, BolUseRawComma, "RCDATA", 1, TRUE) == 0)
        {
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Error while converting file '" + (CStr) TVEntryToSave + (CStr) "' to raw datas resources.");
        }
        else
        {
            FileWriteBufferVB(ResourcesFile, ConversionStringHead, ConversionStringHead.Len());
        }
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        ObjIndex++;
        k++;
    }
    
    // Write user text resources (custom resources as texts)
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewResources);
    while(TreeViewChildEntry != 0)
    {
        if(TreeViewChildEntry == hTreeViewIcons) goto NoWriteUserText;
        else if(TreeViewChildEntry == hTreeViewCursors) goto NoWriteUserText;
        else if(TreeViewChildEntry == hTreeViewBitmaps) goto NoWriteUserText;
        else if(TreeViewChildEntry == hTreeViewStrings) goto NoWriteUserText;
        else if(TreeViewChildEntry == hTreeViewAccelerators) goto NoWriteUserText;
        else if(TreeViewChildEntry == hTreeViewMenus) goto NoWriteUserText;
        else if(TreeViewChildEntry == hTreeViewDialogs) goto NoWriteUserText;
        else if(TreeViewChildEntry == hTreeViewRawdatas) goto NoWriteUserText;
        else
        {
            TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
            FileWriteLine(ResourcesFile, "#include \"" + (CStr) TVEntryToSave + (CStr) "\"");
        }
NoWriteUserText:    
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
    }

    // Include Windows XP manifest if requested
    if(Prj_IncludeManifest != 0)
    { 
        ManifestName = FileReplaceExtension(EndProjectFileName, "Manifest");
        ManifestFile = FileCreateEmpty(ManifestName, NO_SECURITY_DESCRIPTOR);
        if(ManifestFile != INVALID_HANDLE_VALUE)
        {
            ManifestName = StringReplace(ManifestName, "\\", "\\\\", 1, -1, Binary_Compare);
            ManifestDatas = Project_Write_XPManifest_Res();
            FileWriteBufferVB(ManifestFile, ManifestDatas, ManifestDatas.Len());
            FileClose(ManifestFile);
            ManifestName = "1 24 \"" + ManifestName + "\"\r\n";
            FileWriteBufferVB(ResourcesFile, ManifestName, ManifestName.Len());
        }
    }
    
    FileClose(ResourcesFile);
    ResourcesFile = 0;
    
    WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Building make file...");
        
    // Create the exports file
    DefinedEntryPoint = "";
    if(hTreeViewDefs != 0)
    {
        DllDefFile = FileCreateEmpty(ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".def", NO_SECURITY_DESCRIPTOR);
        FileWriteLine(DllDefFile, "LIBRARY " + (CStr) ProjectTitle);
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewDefs);
        if(TreeViewChildEntry != 0)
        {
            FileWriteLine(DllDefFile, "EXPORTS");
            TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
            TVEntryToSave = ChangeRelativePaths(TVEntryToSave);
            // Retrieve wanted entry point
            DefinedEntryPoint = IniReadKey("ENTRYPOINT", "Proc", TVEntryToSave);
            for(i = 0; i <= 999; i++)
            {
                DefVal = IniReadKey("EXPORTS", "Fnc" + (CStr) StringNumberComplement(i, 3).Get_String(), TVEntryToSave);
                if(DefVal.Len() == 0) break;
                FileWriteLine(DllDefFile, DefVal);
            }
        }
        FileClose(DllDefFile);
        DllDefFile = 0;
    }
    
    HeadExt = IniReadKey("EXTENSION", "Ext", PrjBuildIniFile);
    BufString = ProjectTypeExt.Upper_Case();
    if(BufString == "WINEXE")
    {
        PascalName = IniReadKey("PASCALNAME", "WINEXE", PrjBuildIniFile);
    }
    else if(BufString == "DOSEXE")
    {
        PascalName = IniReadKey("PASCALNAME", "DOSEXE", PrjBuildIniFile);
    }
    else if(BufString == "DOSCOM")
    {
        PascalName = IniReadKey("PASCALNAME", "DOSCOM", PrjBuildIniFile);
    }
    else if(BufString == "WINDLL") 
    {
        PascalName = IniReadKey("PASCALNAME", "WINDLL", PrjBuildIniFile);
    }
    else if(BufString == "WINSYS")
    {
        PascalName = IniReadKey("PASCALNAME", "WINSYS", PrjBuildIniFile);
    }
    else if(BufString == "WINLIB")
    {
        PascalName = IniReadKey("PASCALNAME", "WINLIB", PrjBuildIniFile);
    }
    else
    {
        PascalName = IniReadKey("PASCALNAME", "WINEXE", PrjBuildIniFile);
    }
    
    if(PascalName.Len() != 0)
    {
        PascalName = StringReplace(PascalName, "%0", FileRemoveExtension(FileGetFileName(EndProjectFileName).Get_String()).Get_String() , 1, -1, Binary_Compare);
        PascalName = StringReplace(PascalName, "{NL}", "\r\n", 1, -1, Text_Compare);
        PascalName = StringReplace(PascalName, "{SPC}", " ", 1, -1, Text_Compare);
    }
    
    PrjIncludeType = IniReadKey("INCLUDE", "IncType", PrjBuildIniFile);
    PrjIncludeLibType = IniReadKey("INCLUDELIB", "IncType", PrjBuildIniFile);
    // Pascal, je te merde
    PreInc = IniReadKey("INCLUDELIB", "PreInc", PrjBuildIniFile);
    InInc = IniReadKey("INCLUDELIB", "InInc", PrjBuildIniFile);
    PostInc = IniReadKey("INCLUDELIB", "PostInc", PrjBuildIniFile);
        
    ConstantType = IniReadKey("CONSTANTS", "Const", PrjBuildIniFile);
    EarlyInclude = IniReadKey("INCLUDELIB", "EarlyInclude", PrjBuildIniFile);
    if(EarlyInclude.Len() == 0) EarlyInclude = "0";
    
    BatchFile = FileCreateEmpty(ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".bat", NO_SECURITY_DESCRIPTOR);
    FileWriteLine(BatchFile, "@echo off");
    // Switch to drive if required
    if(ProjectDir.Mid(2, 2) == ":\\")
    {
        FileWriteLine(BatchFile, ProjectDir.Mid(1, 2));
    }
    FileWriteLine(BatchFile, "cd \"" + (CStr) CorrectFileName(ProjectDir).Get_String() + (CStr) "\"");
    
    // Add resource compiler to batch file
    BuildSteps = IniReadKey("Layout", "StepRes", MainIniFile);
    if(BuildSteps.Len() != 0) if(_strcmpi(BuildSteps.Get_String(), "1") != 0) goto NoResStep;
    
    StopOnError = IniReadKey("RES", "StopOnError", PrjBuildIniFile);
    
    for(i = 0; i <= 999; i++)
    {
        SkinVal = IniReadKey("RES", "Prog" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        if(SkinVal.Len() == 0) break;
        SkinVal = ChangeRelativePaths(SkinVal);
        SkinVal = SkinCommand(SkinVal);
        if(FileExist(SkinVal) == 0)
        {
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't find specified resource compiler: " + (CStr) SkinVal);
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Process halted.");
            CleanBuildFiles();
            return;
        }
        SkinValB = IniReadKey("RES", "Args" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        SkinValB = ChangeRelativePaths(SkinValB);
        SkinValB = SkinCommand(SkinValB);
        //+ (CStr) CustomRedirector
        FileWriteLine(BatchFile, "\"" + (CStr) SkinVal + (CStr) "\" " + (CStr) SkinValB);
        if(_strcmpi(StopOnError.Get_String(), "1") == 0) FileWriteLine(BatchFile, "if errorlevel 1 goto AsmEnd");
    }
NoResStep:
    
    HeaderFile = FileCreateEmpty(ProjectDir + (CStr) "\\ChromaticHead." + (CStr) HeadExt, NO_SECURITY_DESCRIPTOR);
    for(i = 0; i <= 999; i++)
    {
        SkinVal = IniReadKey("TEMPHEADER", "Opt" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        if(SkinVal.Len() == 0) break;
        if(_strcmpi(SkinVal.Left(5).Get_String(), "LISTC") == 0)
        {
            for(L = 0; L <= 999; L++)
            {
                SkinValB = IniReadKey("CURRENT", SkinVal + (CStr) StringNumberComplement(L, 3).Get_String(), PrjBuildIniFile);
                if(SkinValB.Len() == 0) break;
                SkinOption = SkinValB.Get_Long();
                // Checked ?
                if(SkinOption != 0)
                {
                    SkinValB = IniReadKey(SkinVal, "Dat" + (CStr) StringNumberComplement(L, 3).Get_String(), PrjBuildIniFile);
                    SkinValB = ChangeRelativePaths(SkinValB);
                    SkinValB = SkinCommand(SkinValB);
                    TempArguments = StringSplit(SkinValB, "|");
                    for(j = 0; j <= StringGetSplitUBound(TempArguments); j++)
                    {
                        TempskinVal = StringGetSplitElement(SkinValB, TempArguments, j).Trim();
                        BufString = TempskinVal.Left(1);
                        if(BufString == "<")
                        {
                            // Redirect to file
                            FileWriteLine(HeaderFile, TempskinVal.Mid(2));
                        }
                        else if(BufString == "%")
                        {
                            // Store variable name
                            SkinArrayVar.Add(TempskinVal.Left(2).Get_String());
                            // Store variable datas
                            if(SkinValTextBox.Len() != 0)
                            {
                                // Textbox value
                                SkinArrayDat.Add(SkinValTextBox.Get_String());
                            }
                            else
                            {
                                // Other value (specified in skin)
                                SkinArrayDat.Add(TempskinVal.Mid(3).Get_String());
                            }
                        }
                    }
                    StringReleaseSplit(TempArguments);
                }
            }
        }
        else
        {
            SkinValB = IniReadKey("CURRENT", SkinVal, PrjBuildIniFile);
            SkinValTextBox = "";
            if(SkinValB.Len() == 0)
            {
                SkinOption = 0;
            }else
            {
                BufString = SkinVal.Left(5).Upper_Case();
                if(BufString == "TEXTB")
                {
                    // Save textbox value
                    SkinValTextBox = SkinValB;
                    // Always 0 for textboxes
                    SkinOption = 0;
                }
                else
                {
                    SkinOption = SkinValB.Get_Long();
                }
            }
            SkinValB = IniReadKey(SkinVal, "Dat" + (CStr) StringNumberComplement(SkinOption, 3).Get_String(), PrjBuildIniFile);
            SkinValB = ChangeRelativePaths(SkinValB);
            SkinValB = SkinCommand(SkinValB);
            TempArguments = StringSplit(SkinValB, "|");
            for(j = 0; j <= (long) StringGetSplitUBound(TempArguments); j++)
            {
                TempskinVal = StringGetSplitElement(SkinValB, TempArguments, j).Trim();
                BufString = TempskinVal.Left(1);
                if(BufString == "<")
                {
                    // Redirect to file
                    FileWriteLine(HeaderFile, TempskinVal.Mid(2));
                }
                else if(BufString == "%")
                {
                    // Store variable name
                    SkinArrayVar.Add(TempskinVal.Left(2).Get_String());
                    // Store variable datas
                    if(SkinValTextBox.Len() != 0)
                    {
                        // Textbox value
                        SkinArrayDat.Add(SkinValTextBox.Get_String());
                    }
                    else
                    {
                        // Other value (specified in skin)
                        SkinArrayDat.Add(TempskinVal.Mid(3).Get_String());
                    }
                }
            }
            StringReleaseSplit(TempArguments);
        }
    }

    // Add that (fucking) pascal program name
    if(PascalName.Len() != 0) FileWriteLine(HeaderFile, PascalName);
    
    // --------------------------
    // --- Early including (mainly for Pascal language)
    // --------------------------
    if(_strcmpi(EarlyInclude.Get_String(), "1") == 0)
    {
        // Add libraries to header
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewLibs);
        AddLibToLink = "";
        PrefixLib = IniReadKey("LINKER", "PrefixLib", PrjBuildIniFile);
        if(TreeViewChildEntry != 0) if(PreInc.Len() != 0) FileWriteLine(HeaderFile, PreInc);
        NoIncludePascal = 0;
        while(TreeViewChildEntry != 0)
        {
            NoIncludePascal = 1;
            TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
            // Put library in header file
            if(PrjIncludeLibType.Len() != 0)
            {
                TempInclude = PrjIncludeLibType;
                TempInclude = StringReplace(TempInclude, "%0", TVEntryToSave, 1, -1, Binary_Compare);
                TempInclude = StringReplace(TempInclude, "%1", FileRemoveExtension(FileGetFileName(TVEntryToSave)), 1, -1, Binary_Compare);
                if(PreInc.Len() != 0)
                {
                    // No carriage return
                    FileWriteBufferVB(HeaderFile, TempInclude, TempInclude.Len());
                }
                else
                {
                    FileWriteLine(HeaderFile, TempInclude);
                }
            }
            else
            {
                // Put library in command line
                AddLibToLink = AddLibToLink + PrefixLib;
                AddLibToLink = AddLibToLink + "\"";
                AddLibToLink = AddLibToLink + TVEntryToSave;
                AddLibToLink = AddLibToLink + "\" ";
            }
            TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
            if(TreeViewChildEntry != 0) if(InInc.Len() != 0) FileWriteBufferVB(HeaderFile, InInc, InInc.Len());
        }
        if(NoIncludePascal == 1) if(PostInc.Len() != 0) FileWriteLine(HeaderFile, PostInc);
    }
    
    // --------------------------
    // --- Create constants
    // --------------------------
    ObjIndex = 500;
    TempInclude = ConstantType;
    TempInclude = StringReplace(TempInclude, "%0", "ICON_APP", 1, -1, Binary_Compare);
    TempInclude = StringReplace(TempInclude, "%1", ObjIndex, 1, -1, Binary_Compare);
    FileWriteLine(HeaderFile, TempInclude);

    ObjIndex++;
    
    // Add icons constants to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewIcons);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = Add_Project_Constant(HeaderFile, TreeViewChildEntry, ConstantType, ObjIndex);
        ObjIndex++;
    }
    
    // Add cursors constants to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewCursors);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = Add_Project_Constant(HeaderFile, TreeViewChildEntry, ConstantType, ObjIndex);
        ObjIndex++;
    }
    
    // Add bitmaps constants to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewBitmaps);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = Add_Project_Constant(HeaderFile, TreeViewChildEntry, ConstantType, ObjIndex);
        ObjIndex++;
    }
    
    // Add strings constants to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewStrings);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = Add_Project_Constant(HeaderFile, TreeViewChildEntry, ConstantType, ObjIndex);
        ObjIndex++;
    }
    
    // Add accelerators constants to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewAccelerators);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = Add_Project_Constant(HeaderFile, TreeViewChildEntry, ConstantType, ObjIndex);
        ObjIndex++;
    }
    
    // Add menus constants to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewMenus);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = Add_Project_Constant(HeaderFile, TreeViewChildEntry, ConstantType, ObjIndex);
        ObjIndex++;
    }

    // Add dialogs constants to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewDialogs);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = Add_Project_Constant(HeaderFile, TreeViewChildEntry, ConstantType, ObjIndex);
        ObjIndex++;
    }
    
    // Add raw datas constants to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewRawdatas);
    while(TreeViewChildEntry != 0)
    {
        TreeViewChildEntry = Add_Project_Constant(HeaderFile, TreeViewChildEntry, ConstantType, ObjIndex);
        ObjIndex++;
    }
    
    // ----------------------------
    
    // Add project constants to header
    for(i = 0; i < 999; i++)
    {
        RespSep = IniReadKey("CONSTANTS", "Name" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectFName);
        if(RespSep.Len() == 0) break;
        TempInclude = ConstantType;
        TempInclude = StringReplace(TempInclude, "%0", RespSep, 1, -1, Binary_Compare);
        RespSep = IniReadKey("CONSTANTS", "Value" + (CStr) StringNumberComplement(i, 3).Get_String(), ProjectFName);
        if(RespSep.Len() == 0) RespSep = "1";
        TempInclude = StringReplace(TempInclude, "%1", RespSep, 1, -1, Binary_Compare);
        FileWriteLine(HeaderFile, TempInclude);
    }

    // Add includes to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewIncludes);
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TempInclude = PrjIncludeType;
        TempInclude = StringReplace(TempInclude, "%0", TVEntryToSave, 1, -1, Binary_Compare);
        TempInclude = StringReplace(TempInclude, "%1", FileRemoveExtension(FileGetFileName(TVEntryToSave)), 1, -1, Binary_Compare);
        FileWriteLine(HeaderFile, TempInclude);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
    }
    
    if(_strcmpi(EarlyInclude.Get_String(), "0") == 0)
    {
        // Add libraries to header
        TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewLibs);
        AddLibToLink = "";
        PrefixLib = IniReadKey("LINKER", "PrefixLib", PrjBuildIniFile);
        if(TreeViewChildEntry != 0) if(PreInc.Len() != 0) FileWriteLine(HeaderFile, PreInc);
        NoIncludePascal = 0;
        while(TreeViewChildEntry != 0)
        {
            NoIncludePascal = 1;
            TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
            // Put library in header file
            if(PrjIncludeLibType.Len() != 0)
            {
                TempInclude = PrjIncludeLibType;
                TempInclude = StringReplace(TempInclude, "%0", TVEntryToSave, 1, -1, Binary_Compare);
                TempInclude = StringReplace(TempInclude, "%1", FileRemoveExtension(FileGetFileName(TVEntryToSave)), 1, -1, Binary_Compare);
                if(PreInc.Len() != 0)
                {
                    // No carriage return
                    FileWriteBufferVB(HeaderFile, TempInclude, TempInclude.Len());
                }
                else
                {
                    FileWriteLine(HeaderFile, TempInclude);
                }
            }
            else
            {
                // Put library in command line
                AddLibToLink = AddLibToLink + PrefixLib;
                AddLibToLink = AddLibToLink + "\"";
                AddLibToLink = AddLibToLink + TVEntryToSave;
                AddLibToLink = AddLibToLink + "\" ";
            }
            TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
            if(TreeViewChildEntry != 0) if(InInc.Len() != 0) FileWriteBufferVB(HeaderFile, InInc, InInc.Len());
        }
        if(NoIncludePascal = 1) if(PostInc.Len() != 0) FileWriteLine(HeaderFile, PostInc);
    }
    
    // Create linker files
    ObjectsFile = FileCreateEmpty(ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".ode", NO_SECURITY_DESCRIPTOR);
    
    // Get response file separator
    RespSep = IniReadKey("RESP", "RespSep", PrjBuildIniFile);
    RespSep = StringReplace(RespSep, "{NL}", "\r\n", 1, -1, Text_Compare);
    RespSep = StringReplace(RespSep, "{TAB}", "\t", 1, -1, Text_Compare);
    RespSep = StringReplace(RespSep, "{SPC}", " ", 1, -1, Text_Compare);
    DirectObjects = IniReadKey("RESP", "RespDirect", PrjBuildIniFile);
    LibForeChar = IniReadKey("LIBSPEC", "ForeChar", PrjBuildIniFile);
    LibForeChar = StringReplace(LibForeChar, "{NL}", "\r\n", 1, -1, Text_Compare);
    LibBackChar = IniReadKey("LIBSPEC", "BackChar", PrjBuildIniFile);
    LibBackChar = StringReplace(LibBackChar, "{NL}", "\r\n", 1, -1, Text_Compare);
    LibRetrieveChar = IniReadKey("LIBSPEC", "RetrieveChar", PrjBuildIniFile);
    LibRetrieveChar = StringReplace(LibRetrieveChar, "{NL}", "\r\n", 1, -1, Text_Compare);
    
    ObjLstPrefix = IniReadKey("EXTRAS", "ObjLstPrefix", PrjBuildIniFile);
    
    // Construct user objects list
    UserObjects = "";
    ObjectsList = "";
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewObjects);
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        ResponseEntry = CorrectFileName(TVEntryToSave);
        // Hack for TASM (needs + forechar)
        if(DirectObjects.Len() != 0)
        {
            // Transform object filename for source header construction
            ResponseEntry = StringReplace(DirectObjects, "%1", ResponseEntry, 1, -1, Text_Compare);
        }
        if(_strcmpi(ProjectTypeExt.Get_String(), "WINLIB") == 0)
        {
            UserObjects = UserObjects + (CStr) LibForeChar + (CStr) ResponseEntry + (CStr) LibBackChar;
            ObjectsList = ObjectsList + (CStr) ObjLstPrefix + (CStr) ResponseEntry + (CStr) " ";
        }
        else
        {
            UserObjects = UserObjects + (CStr) ResponseEntry + (CStr) LibBackChar;
            ObjectsList = ObjectsList + (CStr) ObjLstPrefix + (CStr) ResponseEntry + (CStr) " ";
        }
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
        // Don't write last char
        if(TreeViewChildEntry != 0)
        {
            if(LibRetrieveChar.Len() != 0) UserObjects = UserObjects + LibRetrieveChar;
            else UserObjects = UserObjects + RespSep;
        }
    }
    ObjectsList = ObjectsList.Trim();
    
    // Add direct objects to header
    if(DirectObjects.Len() != 0) FileWriteLine(HeaderFile, UserObjects);

    // Write possible code prologue
    if(IniReadBoolKey("EXTRACODE", "UseStartCode", PrjBuildIniFile) == 1)
    {
        CodePrologue = IniReadKey("EXTRACODE", "StartCode" + (CStr) ProjectTypeExt, PrjBuildIniFile);
        if(CodePrologue.Len())
        {
            CodePrologue = Dirs[DIR_EXTRACODE] + (CStr) "\\" + CodePrologue;
            CodePrologue = ChangeRelativePaths(CodePrologue);
            ExtraCodeBytes = (int) MLoadFile(CodePrologue.Get_String(), &ReadCodeBytes);
            if(ExtraCodeBytes)
            {
                CodePrologue = (char *) ExtraCodeBytes;
                CodePrologue = SkinCommand(CodePrologue);
                FileWriteBuffer(HeaderFile, CodePrologue.Get_String(), ReadCodeBytes);
                FileWriteLine(HeaderFile, "\r\n");
                FreeMem(ExtraCodeBytes);
            }
        }
    }

    // Add user modules to header
    TreeViewChildEntry = TreeViewGetFirstItemChild(hTreeView, hTreeViewModules);
    while(TreeViewChildEntry != 0)
    {
        TVEntryToSave = GetFileNameFromTreeView(TreeViewChildEntry);
        TempInclude = PrjIncludeType;
        TempInclude = StringReplace(TempInclude, "%0", TVEntryToSave, 1, -1, Binary_Compare);
        FileWriteLine(HeaderFile, TempInclude);
        TreeViewChildEntry = TreeViewGetNextItem(hTreeView, TreeViewChildEntry);
    }

    // Write possible code epilogue
    if(IniReadBoolKey("EXTRACODE", "UseEndCode", PrjBuildIniFile) == 1)
    {
        CodeEpilogue = IniReadKey("EXTRACODE", "EndCode" + (CStr) ProjectTypeExt, PrjBuildIniFile);
        if(CodeEpilogue.Len())
        {
            CodeEpilogue = Dirs[DIR_EXTRACODE] + (CStr) "\\" + CodeEpilogue;
            CodeEpilogue = ChangeRelativePaths(CodeEpilogue);
            ExtraCodeBytes = (int) MLoadFile(CodeEpilogue.Get_String(), &ReadCodeBytes);
            if(ExtraCodeBytes)
            {
                CodeEpilogue = (char *) ExtraCodeBytes;
                CodeEpilogue = SkinCommand(CodeEpilogue);
                FileWriteBuffer(HeaderFile, CodeEpilogue.Get_String(), ReadCodeBytes);
                FileWriteLine(HeaderFile, "\r\n");
                FreeMem(ExtraCodeBytes);
            }
        }
    }

    for(i = 0; i <= 999; i++)
    {
        SkinVal = IniReadKey("RESP", "Resp" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        if(SkinVal.Len() == 0) break;
        SkinVal = SkinCommand(SkinVal);
        // Put user object if needed
        if(SkinVal.In_Str(1, "{OBJS}", Text_Compare) != 0)
        {
            if(UserObjects.Len() == 0) goto NoUserObjectsFound;
        }
        ResponseEntry = StringReplace(SkinVal, "{OBJS}", UserObjects, 1, -1, Text_Compare);
        // Hack for TASM (needs + forechar)
        if(_strcmpi(ProjectTypeExt.Get_String(), "WINLIB") == 0)
        {
            if(strcmp(ResponseEntry.Left(1).Get_String(),LibForeChar.Get_String()) != 0) ResponseEntry = LibForeChar + ResponseEntry;
            if(strcmp(ResponseEntry.Right(1).Get_String(),LibBackChar.Get_String()) != 0) ResponseEntry = ResponseEntry + LibBackChar;
        }
        if(ResponseEntry.Len() != 0)
        {
            if(_strcmpi(ResponseEntry.Get_String(), "\r\n") != 0)
            {
                FileWriteBufferVB(ObjectsFile, ResponseEntry, ResponseEntry.Len());
                FileWriteBufferVB(ObjectsFile, RespSep, RespSep.Len());
            }
        }
NoUserObjectsFound:;
    }

    FileClose(ObjectsFile);
    ObjectsFile = 0;

    BuildSteps = IniReadKey("Layout", "StepAsm", MainIniFile);
    if(BuildSteps.Len() != 0) if(_strcmpi(BuildSteps.Get_String(), "1") != 0) goto NoAsmStep;
    
    StopOnError = IniReadKey("COMPILER", "StopOnError", PrjBuildIniFile);

    // Get dos commands
    FillCompDOSCommand();
    
    // Write compiler commands to batch file
    for(i = 0; i <= 999; i++)
    {
        SkinVal = IniReadKey("COMPILER", "Prog" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        if(SkinVal.Len() == 0) break;
        SkinVal = ChangeRelativePaths(SkinVal);
        SkinVal = SkinCommand(SkinVal);
        // Hack for DOS commands
        BufString = SkinVal.Left(7).Upper_Case(); 
        if(BufString != "{DOSCOM")
        {
            if(FileExist(SkinVal) == 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't find specified compiler: " + (CStr) SkinVal);
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Process halted.");
                CleanBuildFiles();
                return;
            }
        }
        SkinValB = IniReadKey("COMPILER", "Args" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        SkinValB = ChangeRelativePaths(SkinValB);
        SkinValB = SkinCommand(SkinValB);
        for(k = 1; k <= 26; k++)
        {
            SkinVal = StringReplace(SkinVal, "%" + (CStr) BufString.Chr(0x41 + k).Get_String(), "", 1, -1, Binary_Compare);
            SkinValB = StringReplace(SkinValB, "%" + (CStr) BufString.Chr(0x41 + k).Get_String(), "", 1, -1, Binary_Compare);
        }
        BufString = SkinVal.Left(7).Upper_Case(); 
        if(BufString == "{DOSCOM")
        {
            // Simple DOS command
            CurrDOSCom = GetCompDOSCommand(SkinVal);
            SkinVal = StringReplace(SkinVal, SkinVal, CurrDOSCom, 1, -1, Text_Compare);
            FileWriteLine(BatchFile, SkinVal + (CStr) " " + (CStr) SkinValB);
        }
        else
        {
            FileWriteLine(BatchFile, "\"" + (CStr) SkinVal + (CStr) "\" " + (CStr) SkinValB);
            if(_strcmpi(StopOnError.Get_String(), "1") == 0) FileWriteLine(BatchFile, "if errorlevel 1 goto AsmEnd");
        }
    }
NoAsmStep:
    
    BuildSteps = IniReadKey("Layout", "StepLink", MainIniFile);
    NoLinkCom = 0;
    if(BuildSteps.Len() != 0) if(_strcmpi(BuildSteps.Get_String(), "1") != 0) NoLinkCom = 1;
   
    HeadLib = IniReadKey("LINKER", "HeadLib", PrjBuildIniFile);
    StopOnError = IniReadKey("LINKER", "StopOnError", PrjBuildIniFile);
            
    // If user don't want any entry point then just report it as an empty string
    if(DefinedEntryPoint.Len() != 0)
    {
        if(IniReadKey("LINKER", "DllEntryPoint", PrjBuildIniFile).Len() != 0)
        {
            DefinedEntryPoint = IniReadKey("LINKER", "DllEntryPoint", PrjBuildIniFile).Get_String() + (CStr) DefinedEntryPoint;
        }
        else
        {
            DefinedEntryPoint = "";
        }
    }

    // Get dos commands
    FillLinkDOSCommand();

    // Write linker commands to batch file
    DisabledImports = 1;
    for(i = 0; i <= 999; i++)
    {
        SkinVal = IniReadKey("LINKER", "Prog" + (CStr) ProjectTypeExt + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        if(SkinVal.Len() == 0) break;
        SkinVal = ChangeRelativePaths(SkinVal);
        SkinVal = SkinCommand(SkinVal);
        // Hack for DOS commands
        BufString = SkinVal.Left(7).Upper_Case(); 
        if(BufString != "{DOSCOM")
        {
            BufString = SkinVal.Left(15).Upper_Case(); 
            if(BufString != "{DISABLEIMPORTS" && BufString != "{ENABLEIMPORTS}")
            {
                if(NoLinkCom == 0)
                {
                    if(FileExist(SkinVal) == 0)
                    {
                        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't find specified linker: " + (CStr) SkinVal);
                        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Process halted.");
                        CleanBuildFiles();
                        return;
                    }
                }
            }
        }
        SkinValB = IniReadKey("LINKER", "Args" + (CStr) ProjectTypeExt + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        SkinValB = ChangeRelativePaths(SkinValB);
        SkinValB = SkinCommand(SkinValB);
        for(k = 1; k <= 26; k++)
        {
            SkinVal = StringReplace(SkinVal, "%" + (CStr) BufString.Chr(0x41 + k).Get_String(), "", 1, -1, Binary_Compare);
            SkinValB = StringReplace(SkinValB, "%" + (CStr) BufString.Chr(0x41 + k).Get_String(), "", 1, -1, Binary_Compare);
        }

        BufString = SkinVal.Left(7).Upper_Case(); 
        if(BufString == "{DOSCOM")
        {
            // Simple DOS command
            CurrDOSCom = GetLinkDOSCommand(SkinVal);
            SkinVal = StringReplace(SkinVal, SkinVal, CurrDOSCom, 1, -1, Text_Compare);
            FileWriteLine(BatchFile, SkinVal + (CStr) " " + (CStr) SkinValB);
        }
        else
        {
            BufString = SkinVal.Left(15).Upper_Case(); 
            if(BufString == "{DISABLEIMPORTS")
            {
                DisabledImports = 0;
            }
            else
            {
                if(BufString == "{ENABLEIMPORTS}")
                {
                    DisabledImports = 1;
                }
                else
                {
                    if(NoLinkCom == 0)
                    {
                        if(_strcmpi(HeadLib.Get_String(), "1") == 0)
                        {
                            // Put libraries before
                            if(DisabledImports)
                            {
                                //+ (CStr) " " + (CStr) CustomRedirector
                                FileWriteLine(BatchFile, "\"" + (CStr) SkinVal + (CStr) "\" " + (CStr) AddLibToLink + (CStr) " " + (CStr) SkinValB);
                            }
                            else
                            {
                                //+ (CStr) " " + (CStr) CustomRedirector
                                FileWriteLine(BatchFile, "\"" + (CStr) SkinVal + (CStr) "\" " + (CStr) SkinValB);
                            }
                        }
                        else
                        {
                            if(DisabledImports)
                            {
                                //+ (CStr) CustomRedirector
                                FileWriteLine(BatchFile, "\"" + (CStr) SkinVal + (CStr) "\" " + (CStr) SkinValB + (CStr) " " + (CStr) AddLibToLink);
                            }
                            else
                            {
                                //+ (CStr) " " + (CStr) CustomRedirector
                                FileWriteLine(BatchFile, "\"" + (CStr) SkinVal + (CStr) "\" " + (CStr) SkinValB);
                            }
                        }
                    }
                }
            }
        }
        if(NoLinkCom == 0) if(_strcmpi(StopOnError.Get_String(), "1") == 0) FileWriteLine(BatchFile, "if errorlevel 1 goto AsmEnd");
    }

    FileWriteLine(BatchFile, ":AsmEnd");
    FileWriteLine(BatchFile, "if exist " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".trc").Get_String() + (CStr) " del " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".trc").Get_String());
    FileWriteLine(BatchFile, "if exist " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".ode").Get_String() + (CStr) " del " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".ode").Get_String());
    FileWriteLine(BatchFile, "if exist " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".def").Get_String() + (CStr) " del " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".def").Get_String());
    FileWriteLine(BatchFile, "if exist Chromatichead." + (CStr) HeadExt + (CStr) " del Chromatichead." + (CStr) HeadExt);
    FileWriteLine(BatchFile, "if exist " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".manifest").Get_String() + (CStr) " del " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".manifest").Get_String());
    BuildSteps = IniReadKey("Layout", "StepClean", MainIniFile);
    if(BuildSteps.Len() != 0)
    {
        if(_strcmpi(BuildSteps.Get_String(), "1") == 0)
        {
            FileWriteLine(BatchFile, "if exist " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".res").Get_String() + (CStr) " del " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".res").Get_String());
            FileWriteLine(BatchFile, "if exist " + (CStr) CorrectFileName("resources.obj").Get_String() + (CStr) " del " + (CStr) CorrectFileName("resources.obj").Get_String());
            FileWriteLine(BatchFile, "if exist " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".obj").Get_String() + (CStr) " del " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".obj").Get_String());
            FileWriteLine(BatchFile, "if exist " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".o").Get_String() + (CStr) " del " + (CStr) CorrectFileName(ProjectTitle + (CStr) ".o").Get_String());
        }
    }

    FileWriteLine(BatchFile, "exit");
    FileClose(HeaderFile);
    HeaderFile = 0;
    FileClose(BatchFile);
    BatchFile = 0;
    WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Creating target file...");
    
    // New trap style
    CaptureDOSError(ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".bat", "", ProjectDir);
    BufString = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".bat";
    DeleteFile(BufString.Get_String());

    // Process post-renaming if necessary
    BufString = ProjectTypeExt.Upper_Case();
    if(BufString == "WINLIB")
    {
//        ResRealExt = "lib";
        PostRealName = EndProjectFileName; //FileReplaceExtension(EndProjectFileName, ResRealExt);
        if(ProjectPostRename.Len() != 0)
        {
            BufString = FileReplaceExtension(PostRealName, ProjectPostRename);
            DeleteFile(BufString.Get_String());
            MoveFile(PostRealName.Get_String(), BufString.Get_String());
            PostRealName = BufString;
        }
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "File size: " + (CStr) FileGetSize(PostRealName) + (CStr) " bytes");
    }
    else if(BufString == "WINDLL")
    {
        //ResRealExt = "dll";
        PostRealName = EndProjectFileName; //FileReplaceExtension(EndProjectFileName, ResRealExt);
        if(ProjectPostRename.Len() != 0)
        {
            BufString = FileReplaceExtension(PostRealName, ProjectPostRename);
            DeleteFile(BufString.Get_String());
            MoveFile(PostRealName.Get_String(), BufString.Get_String());
            PostRealName = BufString;
        }
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "File size: " + (CStr) FileGetSize(PostRealName) + (CStr) " bytes");
    }
    else if(BufString == "WINSYS")
    {
        //ResRealExt = "sys";
        PostRealName = EndProjectFileName; //FileReplaceExtension(EndProjectFileName, ResRealExt);
        if(ProjectPostRename.Len() != 0)
        {
            BufString = FileReplaceExtension(PostRealName, ProjectPostRename);
            DeleteFile(BufString.Get_String());
            MoveFile(PostRealName.Get_String(), BufString.Get_String());
            PostRealName = BufString;
        }
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "File size: " + (CStr) FileGetSize(PostRealName) + (CStr) " bytes");
    }
    else
    {
        //ResRealExt = "exe";
        PostRealName = EndProjectFileName; //FileReplaceExtension(EndProjectFileName, ResRealExt);
        if(ProjectPostRename.Len() != 0)
        {
            BufString = FileReplaceExtension(PostRealName, ProjectPostRename);
            DeleteFile(BufString.Get_String());
            MoveFile(PostRealName.Get_String(), BufString.Get_String());
            PostRealName = BufString;
        }
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "File size: " + (CStr) FileGetSize(PostRealName) + (CStr) " bytes");
    }
    switch(BuildType)
    {
        case BUILD_ASSEMBLE:
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Done.");
            break;
        case BUILD_RUN:
            RunProject();
            break;
        case BUILD_DEBUG:
            DebugProject();
            break;
    }
    WriteComment("");
    CursorSetNormal();
}

// -----------------------------------------------------------------------
// Write the version infos into resource output file
void Project_Write_Version_Res(HANDLE ResourcesFile)
{
    CStr ResRealExt;
    CStr BufString;

    FileWriteLine(ResourcesFile, "1 VERSIONINFO");
    FileWriteLine(ResourcesFile, "FILEVERSION " + (CStr) Prj_Major + (CStr) "," + (CStr) Prj_Minor + (CStr) "," + (CStr) Prj_Revision + (CStr) "," + (CStr) Prj_Release);
    FileWriteLine(ResourcesFile, "PRODUCTVERSION " + (CStr) Prj_Major + (CStr) "," + (CStr) Prj_Minor + (CStr) "," + (CStr) Prj_Revision + (CStr) "," + (CStr) Prj_Release);
    FileWriteLine(ResourcesFile, "FILEFLAGSMASK 0x3fL");
    FileWriteLine(ResourcesFile, "FILEFLAGS 0x0L");
    FileWriteLine(ResourcesFile, "FILEOS 0x" + (CStr) Prj_OS + (CStr) "L");
    FileWriteLine(ResourcesFile, "FILETYPE 0x" + (CStr) Prj_FileType + (CStr) "L");
    FileWriteLine(ResourcesFile, "FILESUBTYPE 0x0L");
    FileWriteLine(ResourcesFile, "BEGIN");
    FileWriteLine(ResourcesFile, "BLOCK \"StringFileInfo\"");
    FileWriteLine(ResourcesFile, "BEGIN");
    FileWriteLine(ResourcesFile, "BLOCK \"040904B0\"");
    FileWriteLine(ResourcesFile, "BEGIN");
    if(Prj_CompanyName.Len() != 0) FileWriteLine(ResourcesFile, "VALUE \"CompanyName\", \"" + (CStr) StringReplace(Prj_CompanyName, "\r\n", " ", 1, -1, Binary_Compare) + (CStr) "\\0\"");
    if(Prj_FileDescription.Len() != 0) FileWriteLine(ResourcesFile, "VALUE \"FileDescription\", \"" + (CStr) StringReplace(Prj_FileDescription, "\r\n", " ", 1, -1, Binary_Compare) + (CStr) "\\0\"");
    FileWriteLine(ResourcesFile, "VALUE \"FileVersion\", \"" + (CStr) Prj_Major + (CStr) "." + (CStr) Prj_Minor + (CStr) "." + (CStr) Prj_Revision + (CStr) "." + (CStr) Prj_Release + (CStr) "\\0\"");
    FileWriteLine(ResourcesFile, "VALUE \"InternalName\", \"" + (CStr) ProjectTitle + (CStr) "\\0\"");
    if(Prj_LegalCopyRight.Len() != 0) FileWriteLine(ResourcesFile, "VALUE \"LegalCopyright\", \"" + (CStr) StringReplace(Prj_LegalCopyRight, "\r\n", " ", 1, -1, Binary_Compare) + (CStr) "\\0\"");
    if(Prj_LegalTrademarks.Len() != 0) FileWriteLine(ResourcesFile, "VALUE \"LegalTrademarks\", \"" + (CStr) StringReplace(Prj_LegalTrademarks, "\r\n", " ", 1, -1, Binary_Compare) + (CStr) "\\0\"");
    if(Prj_Comments.Len() != 0) FileWriteLine(ResourcesFile, "VALUE \"Comments\", \"" + (CStr) StringReplace(Prj_Comments, "\r\n", " ", 1, -1, Binary_Compare) + (CStr) "\\0\"");
    BufString = ProjectTypeExt.Upper_Case();
    if(BufString == "WINEXE")
    {
        ResRealExt = "exe";
        if(ProjectPostRename.Len() != 0) ResRealExt = ProjectPostRename;
        FileWriteLine(ResourcesFile, "VALUE \"OriginalFilename\", \"" + (CStr) ProjectTitle + (CStr) "." + (CStr) ResRealExt + (CStr) "\\0\"");
    }
    else if(BufString == "DOSCOM")
    {
        ResRealExt = "com";
        if(ProjectPostRename.Len() != 0) ResRealExt = ProjectPostRename;
        FileWriteLine(ResourcesFile, "VALUE \"OriginalFilename\", \"" + (CStr) ProjectTitle + (CStr) "." + (CStr) ResRealExt + (CStr) "\\0\"");
    }
    else if(BufString == "DOSEXE")
    {
        ResRealExt = "exe";
        if(ProjectPostRename.Len() != 0) ResRealExt = ProjectPostRename;
        FileWriteLine(ResourcesFile, "VALUE \"OriginalFilename\", \"" + (CStr) ProjectTitle + (CStr) "." + (CStr) ResRealExt + (CStr) "\\0\"");
    }
    else if(BufString == "WINDLL")
    {
        ResRealExt = "dll";
        if(ProjectPostRename.Len() != 0) ResRealExt = ProjectPostRename;
        FileWriteLine(ResourcesFile, "VALUE \"OriginalFilename\", \"" + (CStr) ProjectTitle + (CStr) "." + (CStr) ResRealExt + (CStr) "\\0\"");
    }
    else if(BufString == "WINSYS")
    {
        ResRealExt = "sys";
        if(ProjectPostRename.Len() != 0) ResRealExt = ProjectPostRename;
        FileWriteLine(ResourcesFile, "VALUE \"OriginalFilename\", \"" + (CStr) ProjectTitle + (CStr) "." + (CStr) ResRealExt + (CStr) "\\0\"");
    }
    else if(BufString == "WINLIB")
    {
        ResRealExt = "lib";
        if(ProjectPostRename.Len() != 0) ResRealExt = ProjectPostRename;
        FileWriteLine(ResourcesFile, "VALUE \"OriginalFilename\", \"" + (CStr) ProjectTitle + (CStr) "." + (CStr) ResRealExt + (CStr) "\\0\"");
    }
    else
    {
        ResRealExt = "exe";
        if(ProjectPostRename.Len() != 0) ResRealExt = ProjectPostRename;
        FileWriteLine(ResourcesFile, "VALUE \"OriginalFilename\", \"" + (CStr) ProjectTitle + (CStr) "." + (CStr) ResRealExt + (CStr) "\\0\"");
    }

    if(Prj_ProductName.Len() != 0) FileWriteLine(ResourcesFile, "VALUE \"ProductName\", \"" + (CStr) StringReplace(Prj_ProductName, "\r\n", " ", 1, -1, Binary_Compare) + (CStr) "\\0\"");
    FileWriteLine(ResourcesFile, "VALUE \"ProductVersion\", \"" + (CStr) Prj_Major + (CStr) "." + (CStr) Prj_Minor + (CStr) "." + (CStr) Prj_Revision + (CStr) "." + (CStr) Prj_Release + (CStr) "\\0\"");
    FileWriteLine(ResourcesFile, "END");
    FileWriteLine(ResourcesFile, "END");
    FileWriteLine(ResourcesFile, "BLOCK \"VarFileInfo\"");
    FileWriteLine(ResourcesFile, "BEGIN");
    FileWriteLine(ResourcesFile, "VALUE \"Translation\", 0x409, 1200");
    FileWriteLine(ResourcesFile, "END");
    FileWriteLine(ResourcesFile, "END");
}

// -----------------------------------------------------------------------
// Write the windows XP manifest into resource output file
CStr Project_Write_XPManifest_Res(void)
{
    CStr Manifest;

    Manifest = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\r\n";
    Manifest = Manifest + "<assembly xmlns=\"urn:schemas-microsoft-com:asm.v1\" manifestVersion=\"1.0\">\r\n";
    Manifest = Manifest + "<assemblyIdentity\r\n";
    Manifest = Manifest + "    name=\"" + (CStr) Prj_CompanyName + (CStr) ".Windows." + (CStr) ProjectTitle + (CStr) "\"\r\n";
    Manifest = Manifest + "    processorArchitecture = \"x86\"\r\n";
    Manifest = Manifest + "    version=\"" + (CStr) Prj_Major + (CStr) "." + (CStr) Prj_Minor + (CStr) "." + (CStr) Prj_Revision + (CStr) "." + (CStr) Prj_Release + (CStr) "\"\r\n";
    Manifest = Manifest + "    type=\"win32\"/>\r\n";
    Manifest = Manifest + "<description>" + (CStr) Prj_FileDescription + (CStr) "</description>\r\n";
    Manifest = Manifest + "<dependency>\r\n";
    Manifest = Manifest + "    <dependentAssembly>\r\n";
    Manifest = Manifest + "        <assemblyIdentity\r\n";
    Manifest = Manifest + "            type=\"win32\"\r\n";
    Manifest = Manifest + "            name=\"Microsoft.Windows.Common-Controls\"\r\n";
    Manifest = Manifest + "            version=\"6.0.0.0\"\r\n";
    Manifest = Manifest + "            processorArchitecture=\"x86\"\r\n";
    Manifest = Manifest + "            publicKeyToken=\"6595b64144ccf1df\"\r\n";
    Manifest = Manifest + "            language=\"*\"\r\n";
    Manifest = Manifest + "        />\r\n";
    Manifest = Manifest + "    </dependentAssembly>\r\n";
    Manifest = Manifest + "</dependency>\r\n";
    Manifest = Manifest + "</assembly>\r\n";
    return(Manifest);
}

// -----------------------------------------------------------------------
// Create a constant into header file
HTREEITEM Add_Project_Constant(HANDLE HeaderFile, HTREEITEM TreeViewChildEntry, CStr ConstantType, long ObjIndex)
{
    CStr TempInclude;
    CStr TVEntryToSave;

    TVEntryToSave = GetLabelFromTreeView(TreeViewChildEntry);
    TempInclude = ConstantType;
    TempInclude = StringReplace(TempInclude, "%0", TVEntryToSave, 1, -1, Binary_Compare);
    TempInclude = StringReplace(TempInclude, "%1", ObjIndex, 1, -1, Binary_Compare);
    FileWriteLine(HeaderFile, TempInclude);
    return(TreeViewGetNextItem(hTreeView, TreeViewChildEntry));
}

// -----------------------------------------------------------------------
// Clean temporary projects files
void CleanBuildFiles(void)
{
    CStr BufString;
    
    FileClose(ObjectsFile);
    ObjectsFile = 0;
    FileClose(HeaderFile);
    HeaderFile = 0;
    FileClose(BatchFile);
    BatchFile = 0;
    BufString = ProjectTitle;
    BufString = BufString + ".trc";
    DeleteFile(CorrectFileName(BufString).Get_String());
    BufString = ProjectTitle;
    BufString = BufString + ".ode";
    DeleteFile(CorrectFileName(BufString).Get_String());
    BufString = ProjectTitle;
    BufString = BufString + ".def";
    DeleteFile(CorrectFileName(BufString).Get_String());
    BufString = "Chromatichead.";
    BufString = BufString + HeadExt;
    DeleteFile(BufString.Get_String());
}

// -----------------------------------------------------------------------
// Get the list of compiler DOS commands
void FillCompDOSCommand(void)
{
    CStr DOSCom;
    int i = 0;

    CompDOSCom.Erase();
    for(i = 0; i <= 999; i++)
    {
        DOSCom = IniReadKey("COMPILER", "DOSCom" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        if(DOSCom.Len() == 0) break;
        CompDOSCom.Add(DOSCom.Get_String());
    }
}

// -----------------------------------------------------------------------
// Retrieve the value of a linker DOS command
CStr GetCompDOSCommand(CStr DOSCom)
{
    CStr ReturnValue;
    long ComNumber = 0;

    ComNumber = DOSCom.Mid(8, 3).Get_Long();
    ReturnValue = CompDOSCom.Get(ComNumber)->Content;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Get the list of linker DOS command
void FillLinkDOSCommand(void)
{
    CStr DOSCom;
    int i = 0;

    LinkDOSCom.Erase();
    for(i = 0; i <= 999; i++)
    {
        DOSCom = IniReadKey("LINKER", "DOSCom" + (CStr) StringNumberComplement(i, 3).Get_String(), PrjBuildIniFile);
        if(DOSCom.Len() == 0) break;
        LinkDOSCom.Add(DOSCom.Get_String());
    }
}

// -----------------------------------------------------------------------
// Retrieve the value of a linker DOS command
CStr GetLinkDOSCommand(CStr DOSCom)
{
    CStr ReturnValue;
    long ComNumber = 0;

    ComNumber = DOSCom.Mid(8, 3).Get_Long();
    ReturnValue = LinkDOSCom.Get(ComNumber)->Content;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Run a project
void RunProject(void)
{
    HINSTANCE RunError = 0;
    CStr BufString;

    BufString = ProjectTypeExt.Upper_Case();
    if(BufString == "WINLIB") WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Run command disabled.");
    else if(BufString == "WINDLL") WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Run command disabled.");
    else if(BufString == "WINSYS") WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Run command disabled.");
    else
    {
        if(ProjectPostRename.Len() != 0) RunError = ShellExecute(hMDIform.hWnd, "open", FileReplaceExtension(ProjectFName, ProjectPostRename).Get_String(), IniReadKey("Layout", "PrjArg", MainIniFile).Get_String(), FileGetDirectory(ProjectFName).Get_String(), SW_SHOW);
        else RunError = ShellExecute(hMDIform.hWnd, "open", FileReplaceExtension(ProjectFName, "exe").Get_String(), IniReadKey("Layout", "PrjArg", MainIniFile).Get_String(), FileGetDirectory(ProjectFName).Get_String(), SW_SHOW);
        if((long) RunError <= 32)
        {
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't run file (Error " + (CStr) (long) RunError + (CStr) ").");
        }
        else
        {
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Running file.");
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "hInstance: 0" + (CStr) StringHex32((long) RunError).Get_String() + (CStr) "h (" + (CStr) (long) RunError + (CStr) ")");
        }
    }
}

// -----------------------------------------------------------------------
// Debug a project
void DebugProject(void)
{
    CStr RealExt;
    CStr DebuggerToUse;
    CStr DebuggerToUseArgs;
    CStr BufString;
    CStr FileToDebug;

    ProjectDir = FileGetDirectory(ProjectFName);
    BufString = ProjectTypeExt.Upper_Case();
    if(BufString == "WINLIB")
    {
        FileToDebug = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".lib";
    }
    else if(BufString == "WINDLL")
    {
        FileToDebug = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".dll";
    }
    else if(BufString == "WINSYS")
    {
        FileToDebug = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".sys";
    }
    else
    {
        FileToDebug = ProjectDir + (CStr) "\\" + (CStr) ProjectTitle + (CStr) ".exe";
    }

    DebuggerToUse = IniReadKey("Layout", "PrjDebugger", MainIniFile);
    /*if(ProjectPostRename.Len() != 0) {
        RealExt = ProjectPostRename;
    } else {
        BufString = ProjectTypeExt.Upper_Case();
        if(BufString == "WINLIB") RealExt = "lib";
        else if(BufString == "WINDLL") RealExt = "dll";
        else if(BufString == "WINSYS") RealExt = "sys";
        else RealExt = "exe";
    }*/
    BufString = ProjectTypeExt.Upper_Case();
    DebuggerToUse = DebuggerToUse.Trim();
    if(DebuggerToUse.Lower_Case() != "(use internal debugger)")
    {
        DebuggerToUseArgs = DebuggerToUse;
        // Search for the real filename
        while(!FileExist(DebuggerToUse))
        {
            DebuggerToUse = DebuggerToUse.Left(DebuggerToUse.Len() - 1);
            if(DebuggerToUse.Len() == 0)
            {
                WriteToStatus("Can't find external debugger.");
                return;
            }
        }
        // Extract the arguments
        DebuggerToUseArgs = DebuggerToUseArgs.Mid(DebuggerToUse.Len());
        // Replace the filename wildcard
        DebuggerToUseArgs = DebuggerToUseArgs.Replace("%1", FileToDebug, 1, -1, Binary_Compare);
        DebuggerToUse = DebuggerToUse.Trim();
        DebuggerToUseArgs = DebuggerToUseArgs.Trim();
    }
    
    if(BufString == "WINLIB" || BufString == "WINDLL" || BufString == "WINSYS")
    {
        if(DebuggerToUse.Lower_Case() == "(use internal debugger)")
        {
            WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Debug command disabled.");
        }
        else
        {
            WriteToStatus("Starting external debugger...");
            RunDOSCmd(DebuggerToUse, DebuggerToUseArgs, FileGetDirectory(FileToDebug), "");
            WriteToStatus("Done.");
        }
    }
    else
    {
        if(DebuggerToUse.Lower_Case() == "(use internal debugger)")
        {
            if(WaitForDebugOutput(FileToDebug, IniReadKey("Layout", "PrjArg", MainIniFile).Get_String()) == 0)
            {
                WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Can't create process.");
            }
        }
        else
        {
            WriteToStatus("Starting external debugger...");
            RunDOSCmd(DebuggerToUse, DebuggerToUseArgs, FileGetDirectory(FileToDebug), "");
            WriteToStatus("Done.");
        }
    }
}

// -----------------------------------------------------------------------
// Convert a general command
CStr SkinCommand(CStr Comd)
{
    int k = 0;

    // %A = Project title
    Comd = StringReplace(Comd, "%A", ProjectTitle, 1, -1, Binary_Compare);
    // %9 = Our constructed header
    Comd = StringReplace(Comd, "%9", "ChromaticHead." + (CStr) HeadExt, 1, -1, Binary_Compare);
    // %8 = Entry point argument
    Comd = StringReplace(Comd, "%8", DefinedEntryPoint, 1, -1, Binary_Compare);
    // %7 = List of objects
    Comd = StringReplace(Comd, "%7", ObjectsList, 1, -1, Binary_Compare);
    for(k = 0; k < SkinArrayVar.Amount(); k++)
    {
        if(strlen(SkinArrayVar.Get(k)->Content) != 0) Comd = StringReplace(Comd, SkinArrayVar.Get(k)->Content, SkinArrayDat.Get(k)->Content, 1, -1, Binary_Compare);
    }
    return(Comd);
}

// -----------------------------------------------------------------------
// Generate prototypes for a lib file
void GenProto(CStr LibFile)
{
    CStr GenProg;
    CStr GenCmd;
    CStr GenDest;

    WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Generating prototypes...");
    GenProg = IniReadKey("Layout", "GenProto", MainIniFile);
    GenCmd = IniReadKey("Layout", "GenCmd", MainIniFile);
    GenDest = IniReadKey("Layout", "GenDest", MainIniFile);
    GenProg = ChangeRelativePaths(GenProg);
    GenCmd = ChangeRelativePaths(GenCmd);
    GenDest = ChangeRelativePaths(GenDest);
    GenCmd = TranslateCommands(GenCmd, LibFile);
    if(FRMArgsEntered == 1)
    {
        FRMArgsEntered = 0;
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Operation cancelled.");
        return;
    }
    GenDest = TranslateCommands(GenDest, LibFile);
    if(FRMArgsEntered == 1)
    {
        FRMArgsEntered = 0;
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Operation cancelled.");
        return;
    }
    if(strcmp(GenDest.Left(1).Get_String(), "|") == 0)
    {
        RunDOSCmd(GenProg, GenCmd, FileGetDirectory(GenDest.Mid(2)), "");
    }
    else
    {
        RunDOSCmd(GenProg, GenCmd + (CStr) " " + (CStr) GenDest, FileGetDirectory(GenDest).Get_String(), "");
    }
    if(strcmp(GenDest.Left(1).Get_String(), "|") == 0) GenDest = GenDest.Mid(2);
    if(FileExist(GenDest) != 0)
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Done.");
    }
    else
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Error: can't find generated file.");
        return;
    }
    if(MiscMsgBox(hMDIform.hWnd, "Prototypes generated.\rAdd new include file ?", MB_QUESTION, Requesters) == IDYES)
    {
        if(TreeViewSearchItemText(hTreeView, hTreeViewIncludes, FileGetFileName(GenDest).Get_String() + (CStr) " (" + (CStr) GenDest + (CStr) ")") != -1)
        {
            MiscMsgBox(hMDIform.hWnd, "File '" + (CStr) GenDest + (CStr) "' already referenced in project.", MB_ERROR, Requesters);
            return;
        }
        TreeViewAddItem(hTreeView, FileGetFileName(GenDest).Get_String() + (CStr) " (" + (CStr) GenDest + (CStr) ")", hTreeViewIncludes, 0, ICON_NEW, ICON_NEW, 0, 1);
        ProjectModified = TRUE;
    }
}

// -----------------------------------------------------------------------
// Create the treeview structure
void CreateProjectStruct(void)
{
    if(hTreeViewRoot != 0) TreeViewRemoveItem(hTreeView, hTreeViewRoot);
    hTreeViewRoot = TreeViewAddItem(hTreeView, ProjectTypeName + (CStr) " (" + (CStr) ProjectFName + (CStr) ")", TVI_ROOT, 0, ICON_FILES, ICON_FILES, TVIS_BOLD, 0);
    hTreeViewIncludes = TreeViewAddItem(hTreeView, "Includes", hTreeViewRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewLibs = TreeViewAddItem(hTreeView, "Libraries", hTreeViewRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewModules = TreeViewAddItem(hTreeView, "Modules", hTreeViewRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewObjects = TreeViewAddItem(hTreeView, "Objects", hTreeViewRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewResources = TreeViewAddItem(hTreeView, "Resources", hTreeViewRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewIcons = TreeViewAddItem(hTreeView, "Icons", hTreeViewResources, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewCursors = TreeViewAddItem(hTreeView, "Cursors", hTreeViewResources, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewBitmaps = TreeViewAddItem(hTreeView, "Bitmaps", hTreeViewResources, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewStrings = TreeViewAddItem(hTreeView, "Strings", hTreeViewResources, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewAccelerators = TreeViewAddItem(hTreeView, "Accelerators", hTreeViewResources, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewMenus = TreeViewAddItem(hTreeView, "Menus", hTreeViewResources, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewDialogs = TreeViewAddItem(hTreeView, "Dialogs", hTreeViewResources, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewRawdatas = TreeViewAddItem(hTreeView, "Raw datas", hTreeViewResources, 0, ICON_FILES, ICON_FILES, 0, 0);
    hTreeViewDefs = 0;
    if((_strcmpi(ProjectTypeExt.Get_String(), "WINDLL") == 0) || (_strcmpi(ProjectTypeExt.Get_String(), "WINSYS") == 0))
    {
        hTreeViewDefs = TreeViewAddItem(hTreeView, "Exports", hTreeViewRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
    }
	hTreeViewTexts = TreeViewAddItem(hTreeView, "Documents", hTreeViewRoot, 0, ICON_FILES, ICON_FILES, 0, 0);
}

// -----------------------------------------------------------------------
// Convert a filename for resource including
CStr ResIncludeFile(CStr ResFileName)
{
    CStr BufString;
    CStr ReturnValue;

    BufString = ResFileName;
    if(BufString.In_Str(1, " ") != 0) BufString = "\"" + BufString + (CStr) "\"";
    BufString = StringReplace(BufString, "\\", "\\\\", 1, -1, Binary_Compare);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a string for resource including
CStr ResIncludeString(CStr ResString)
{
    CStr ReturnValue;
    CStr BufString;

    BufString = ResString;
    BufString = StringReplace(BufString, "\\", "\\\\", 1, -1, Binary_Compare);
    BufString = StringReplace(BufString, "\"", "\042", 1, -1, Binary_Compare);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Free all memory allocated for resources properties
void EraseResArrays(void)
{
    int i = 0;

    for(i = 0; i < PrjIconsArray.Amount(); i++)
    {
        FreeMem((long) PrjIconsArray.Get(i)->Content);
    }
    for(i = 0; i < PrjCursorsArray.Amount(); i++)
    {
        FreeMem((long) PrjCursorsArray.Get(i)->Content);
    }
    for(i = 0; i < PrjBitmapsArray.Amount(); i++)
    {
        FreeMem((long) PrjBitmapsArray.Get(i)->Content);
    }
    for(i = 0; i < PrjStringsArray.Amount(); i++)
    {
        FreeMem((long) PrjStringsArray.Get(i)->Content);
    }
    for(i = 0; i < PrjAcceleratorsArray.Amount(); i++)
    {
        FreeMem((long) PrjAcceleratorsArray.Get(i)->Content);
    }
    for(i = 0; i < PrjMenusArray.Amount(); i++)
    {
        FreeMem((long) PrjMenusArray.Get(i)->Content);
    }
    for(i = 0; i < PrjDialogsArray.Amount(); i++)
    {
        FreeMem((long) PrjDialogsArray.Get(i)->Content);
    }
    for(i = 0; i < PrjRawDatasArray.Amount(); i++)
    {
        FreeMem((long) PrjRawDatasArray.Get(i)->Content);
    }
    PrjIconsArray.Erase();
    PrjCursorsArray.Erase();
    PrjBitmapsArray.Erase();
    PrjStringsArray.Erase();
    PrjAcceleratorsArray.Erase();
    PrjMenusArray.Erase();
    PrjDialogsArray.Erase();
    PrjRawDatasArray.Erase();
}

// -----------------------------------------------------------------------
// Add icon properties in icons array
void AddIconInArray(long Data, long Langu)
{
    long *NewMem = 0;

    NewMem = (long *) AllocMem(8);
    NewMem[0] = Data;
    NewMem[1] = Langu;
    PrjIconsArray.Add(NewMem);
}

// -----------------------------------------------------------------------
// Add cursor properties in cursors array
void AddCursorInArray(long Data, long Langu)
{
    long *NewMem = 0;

    NewMem = (long *) AllocMem(8);
    NewMem[0] = Data;
    NewMem[1] = Langu;
    PrjCursorsArray.Add(NewMem);
}

// -----------------------------------------------------------------------
// Add bitmap properties in bitmaps array
void AddBitmapInArray(long Data, long Langu)
{
    long *NewMem = 0;

    NewMem = (long *) AllocMem(8);
    NewMem[0] = Data;
    NewMem[1] = Langu;
    PrjBitmapsArray.Add(NewMem);
}

// -----------------------------------------------------------------------
// Add string properties in strings array
void AddStringInArray(long Data, long Langu)
{
    long *NewMem = 0;

    NewMem = (long *) AllocMem(8);
    NewMem[0] = Data;
    NewMem[1] = Langu;
    PrjStringsArray.Add(NewMem);
}

// -----------------------------------------------------------------------
// Add acccelerator properties in accelerators array
void AddAcceleratorInArray(long Data, long Langu)
{
    long *NewMem = 0;

    NewMem = (long *) AllocMem(8);
    NewMem[0] = Data;
    NewMem[1] = Langu;
    PrjAcceleratorsArray.Add(NewMem);
}

// -----------------------------------------------------------------------
// Add menu properties in menus array
void AddMenuInArray(long Data, long Langu)
{
    long *NewMem = 0;

    NewMem = (long *) AllocMem(8);
    NewMem[0] = Data;
    NewMem[1] = Langu;
    PrjMenusArray.Add(NewMem);
}

// -----------------------------------------------------------------------
// Add dialog properties in dialogs array
void AddDialogInArray(long Data, long Langu)
{
    long *NewMem = 0;

    NewMem = (long *) AllocMem(8);
    NewMem[0] = Data;
    NewMem[1] = Langu;
    PrjDialogsArray.Add(NewMem);
}

// -----------------------------------------------------------------------
// Add raw data properties in raw datas array
void AddRawDatasInArray(long Data, long Langu)
{
    long *NewMem = 0;

    NewMem = (long *) AllocMem(8);
    NewMem[0] = Data;
    NewMem[1] = Langu;
    PrjRawDatasArray.Add(NewMem);
}

// -----------------------------------------------------------------------
// Retrieve icon properties in array
long GetIconProps(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjIconsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[0]);
}

// -----------------------------------------------------------------------
// Retrieve icon language in array
long GetIconLang(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjIconsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[1]);
}

// -----------------------------------------------------------------------
// Retrieve cursor properties in array
long GetCursorProps(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjCursorsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[0]);
}

// -----------------------------------------------------------------------
// Retrieve cursor language in array
long GetCursorLang(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjCursorsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[1]);
}

// -----------------------------------------------------------------------
// Retrieve bitmap properties in array
long GetBitmapProps(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjBitmapsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[0]);
}

// -----------------------------------------------------------------------
// Retrieve bitmap language in array
long GetBitmapLang(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjBitmapsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[1]);
}

// -----------------------------------------------------------------------
// Retrieve string properties in array
long Get_StringProps(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjStringsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[0]);
}

// -----------------------------------------------------------------------
// Retrieve string language in array
long Get_StringLang(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjStringsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[1]);
}

// -----------------------------------------------------------------------
// Retrieve accelerator properties in array
long GetAcceleratorProps(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjAcceleratorsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[0]);
}

// -----------------------------------------------------------------------
// Retrieve accelerator language in array
long GetAcceleratorLang(long Index)
{
    long *NewMem = 0;

    NewMem = (long *) PrjAcceleratorsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[1]);
}

// -----------------------------------------------------------------------
// Retrieve menu properties in array
long GetMenuProps(long Index)
{
    long *NewMem = 0;

    NewMem = PrjMenusArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[0]);
}

// -----------------------------------------------------------------------
// Retrieve menu language in array
long GetMenuLang(long Index)
{
    long *NewMem = 0;

    NewMem = PrjMenusArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[1]);
}

// -----------------------------------------------------------------------
// Retrieve dialog properties in array
long GetDialogProps(long Index)
{
    long *NewMem = 0;

    NewMem = PrjDialogsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[0]);
}

// -----------------------------------------------------------------------
// Retrieve dialog language in array
long GetDialogLang(long Index)
{
    long *NewMem = 0;

    NewMem = PrjDialogsArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[1]);
}

// -----------------------------------------------------------------------
// Retrieve raw data properties in array
long GetRawdataProps(long Index)
{
    long *NewMem = 0;

    NewMem = PrjRawDatasArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[0]);
}

// -----------------------------------------------------------------------
// Retrieve raw data language in array
long GetRawdataLang(long Index)
{
    long *NewMem = 0;

    NewMem = PrjRawDatasArray.Get(Index)->Content;
    if(NewMem == 0) return(0);
    return(NewMem[1]);
}

// -----------------------------------------------------------------------
// Set icon properties in array
void SetIconProps(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = (long *) PrjIconsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[0] = Dat;
}

// -----------------------------------------------------------------------
// Set icon language in array
void SetIconLang(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjIconsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[1] = Dat;
}

// -----------------------------------------------------------------------
// Set cursor properties in array
void SetCursorProps(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjCursorsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[0] = Dat;
}

// -----------------------------------------------------------------------
// Set cursor language in array
void SetCursorLang(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjCursorsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[1] = Dat;
}

// -----------------------------------------------------------------------
// Set bitmap properties in array
void SetBitmapProps(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjBitmapsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[0] = Dat;
}

// -----------------------------------------------------------------------
// Set bitmap language in array
void SetBitmapLang(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjBitmapsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[1] = Dat;
}

// -----------------------------------------------------------------------
// Set string properties in array
void Set_StringProps(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjStringsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[0] = Dat;
}

// -----------------------------------------------------------------------
// Set string language in array
void Set_StringLang(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjStringsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[1] = Dat;
}

// -----------------------------------------------------------------------
// Set accelerator properties in array
void SetAcceleratorProps(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjAcceleratorsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[0] = Dat;
}

// -----------------------------------------------------------------------
// Set accelerator language in array
void SetAcceleratorLang(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjAcceleratorsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[1] = Dat;
}

// -----------------------------------------------------------------------
// Set menu properties in array
void SetMenuProps(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjMenusArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[0] = Dat;
}

// -----------------------------------------------------------------------
// Set menu language in array
void SetMenuLang(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjMenusArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[1] = Dat;
}

// -----------------------------------------------------------------------
// Set dialog properties in array
void SetDialogProps(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjDialogsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[0] = Dat;
}

// -----------------------------------------------------------------------
// Set dialog language in array
void SetDialogLang(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjDialogsArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[1] = Dat;
}

// -----------------------------------------------------------------------
// Set raw data properties in array
void SetRawDataProps(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjRawDatasArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[0] = Dat;
}

// -----------------------------------------------------------------------
// Set raw data language in array
void SetRawDataLang(long Index, long Dat)
{
    long *NewMem = 0;

    NewMem = PrjRawDatasArray.Get(Index)->Content;
    if(NewMem == 0) return;
    NewMem[1] = Dat;
}

// -----------------------------------------------------------------------
// Remove icon from icons array
void RemIconFromArray(long Idx)
{
    FreeMem((long) PrjIconsArray.Get(Idx)->Content);
    PrjIconsArray.Del(Idx);
}

// -----------------------------------------------------------------------
// Remove cursor from cursors array
void RemCursorFromArray(long Idx)
{
    FreeMem((long) PrjCursorsArray.Get(Idx)->Content);
    PrjCursorsArray.Del(Idx);
}

// -----------------------------------------------------------------------
// Remove bitmap from bitmaps array
void RemBitmapFromArray(long Idx)
{
    FreeMem((long) PrjBitmapsArray.Get(Idx)->Content);
    PrjBitmapsArray.Del(Idx);
}

// -----------------------------------------------------------------------
// Remove string from strings array
void RemStringFromArray(long Idx)
{
    FreeMem((long) PrjStringsArray.Get(Idx)->Content);
    PrjStringsArray.Del(Idx);
}

// -----------------------------------------------------------------------
// Remove acccelerator from accelerators array
void RemAcceleratorFromArray(long Idx)
{
    FreeMem((long) PrjAcceleratorsArray.Get(Idx)->Content);
    PrjAcceleratorsArray.Del(Idx);
}

// -----------------------------------------------------------------------
// Remove menu from menus array
void RemMenuFromArray(long Idx)
{
    FreeMem((long) PrjMenusArray.Get(Idx)->Content);
    PrjMenusArray.Del(Idx);
}

// -----------------------------------------------------------------------
// Remove menu from menus array
void RemDialogFromArray(long Idx)
{
    FreeMem((long) PrjDialogsArray.Get(Idx)->Content);
    PrjDialogsArray.Del(Idx);
}

// -----------------------------------------------------------------------
// Remove raw data from raw datas array
void RemRawDataFromArray(long Idx)
{
    FreeMem((long) PrjRawDatasArray.Get(Idx)->Content);
    PrjRawDatasArray.Del(Idx);
}

// -----------------------------------------------------------------------
// Check if a referenced file exists and handle missing files array
int CheckMissingFile(CStr FileName, int ReportMissing, HTREEITEM hOrigTreeItem,
                     CStr ResourceName, int ResourceProp, int ResourceLang)
{
    if(FileExist(FileName) == 0)
    {
        if(ReportMissing)
        {
            PossibleMissingFiles.Add(FileName.Get_String());
            PossibleMissingFilesTree.Add(hOrigTreeItem);
            PossibleMissingFilesResName.Add(ResourceName.Get_String());
            PossibleMissingFilesResProp.Add(ResourceProp);
            PossibleMissingFilesResLang.Add(ResourceLang);
        }
        return(FALSE);
    }
    return(TRUE);
}

// -----------------------------------------------------------------------
// Return a string filled correct resource element directives
CStr CalcResourceHeader(long ResourceID)
{
    CStr ReturnValue;
    CStr BufString;

    if((ResourceID & RESPROPDAT_MOVE) != 0) BufString = "MOVEABLE";
    else BufString = "FIXED";
    if((ResourceID & RESPROPDAT_PURE) != 0) BufString = BufString + " PURE";
    else BufString = BufString + " IMPURE";
    if((ResourceID & RESPROPDAT_LOAD) != 0) BufString = BufString + " LOADONCALL";
    else BufString = BufString + " PRELOAD";
    if((ResourceID & RESPROPDAT_DISCARD) != 0) BufString = BufString + " DISCARDABLE";
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Return a string filled correct resource element language
CStr  CalcResourceLanguage(long ResourceID)
{
    CStr ReturnValue;
    CStr BufString;
    CStr BufString2;

    BufString2 = TabLangRc[ResourceID];
    BufString = "LANGUAGE LANG_" +
                (CStr) BufString2.Upper_Case().Get_String() +
                (CStr) ",0";
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Write language numeric definition
CStr  WriteLanguageEquate(long ResourceID)
{
    CStr ReturnValue;
    CStr BufString;
    CStr BufString2;

    BufString2 = TabLangRc[ResourceID];
    BufString = (CStr) "#define LANG_" + (CStr) BufString2.Upper_Case().Get_String() + (CStr) " " + (CStr) ResourceID;
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Put accelerators constants in resource file
void WriteAcceleratorEquates(HANDLE ResFile)
{
    FileWriteLine(ResFile, "#define VK_LBUTTON 0x1");
    FileWriteLine(ResFile, "#define VK_RBUTTON 0x2");
    FileWriteLine(ResFile, "#define VK_CANCEL 0x3");
    FileWriteLine(ResFile, "#define VK_MBUTTON 0x4");
    FileWriteLine(ResFile, "#define VK_BACK 0x8");
    FileWriteLine(ResFile, "#define VK_TAB 0x9");
    FileWriteLine(ResFile, "#define VK_CLEAR 0xc");
    FileWriteLine(ResFile, "#define VK_RETURN 0xd");
    FileWriteLine(ResFile, "#define VK_SHIFT 0x10");
    FileWriteLine(ResFile, "#define VK_CONTROL 0x11");
    FileWriteLine(ResFile, "#define VK_MENU 0x12");
    FileWriteLine(ResFile, "#define VK_PAUSE 0x13");
    FileWriteLine(ResFile, "#define VK_CAPITAL 0x14");
    FileWriteLine(ResFile, "#define VK_ESCAPE 0x1b");
    FileWriteLine(ResFile, "#define VK_SPACE 0x20");
    FileWriteLine(ResFile, "#define VK_PRIOR 0x21");
    FileWriteLine(ResFile, "#define VK_NEXT 0x22");
    FileWriteLine(ResFile, "#define VK_END 0x23");
    FileWriteLine(ResFile, "#define VK_HOME 0x24");
    FileWriteLine(ResFile, "#define VK_LEFT 0x25");
    FileWriteLine(ResFile, "#define VK_UP 0x26");
    FileWriteLine(ResFile, "#define VK_RIGHT 0x27");
    FileWriteLine(ResFile, "#define VK_DOWN 0x28");
    FileWriteLine(ResFile, "#define VK_SELECT 0x29");
    FileWriteLine(ResFile, "#define VK_PRINT 0x2a");
    FileWriteLine(ResFile, "#define VK_EXECUTE 0x2b");
    FileWriteLine(ResFile, "#define VK_SNAPSHOT 0x2c");
    FileWriteLine(ResFile, "#define VK_INSERT 0x2d");
    FileWriteLine(ResFile, "#define VK_DELETE 0x2e");
    FileWriteLine(ResFile, "#define VK_HELP 0x2f");
    FileWriteLine(ResFile, "#define VK_LWIN 0x5b");
    FileWriteLine(ResFile, "#define VK_RWIN 0x5c");
    FileWriteLine(ResFile, "#define VK_APPS 0x5d");
    FileWriteLine(ResFile, "#define VK_NUMPAD0 0x60");
    FileWriteLine(ResFile, "#define VK_NUMPAD1 0x61");
    FileWriteLine(ResFile, "#define VK_NUMPAD2 0x62");
    FileWriteLine(ResFile, "#define VK_NUMPAD3 0x63");
    FileWriteLine(ResFile, "#define VK_NUMPAD4 0x64");
    FileWriteLine(ResFile, "#define VK_NUMPAD5 0x65");
    FileWriteLine(ResFile, "#define VK_NUMPAD6 0x66");
    FileWriteLine(ResFile, "#define VK_NUMPAD7 0x67");
    FileWriteLine(ResFile, "#define VK_NUMPAD8 0x68");
    FileWriteLine(ResFile, "#define VK_NUMPAD9 0x69");
    FileWriteLine(ResFile, "#define VK_MULTIPLY 0x6a");
    FileWriteLine(ResFile, "#define VK_ADD 0x6b");
    FileWriteLine(ResFile, "#define VK_SEPARATOR 0x6c");
    FileWriteLine(ResFile, "#define VK_SUBTRACT 0x6d");
    FileWriteLine(ResFile, "#define VK_DECIMAL 0x6e");
    FileWriteLine(ResFile, "#define VK_DIVIDE 0x6f");
    FileWriteLine(ResFile, "#define VK_F1 0x70");
    FileWriteLine(ResFile, "#define VK_F2 0x71");
    FileWriteLine(ResFile, "#define VK_F3 0x72");
    FileWriteLine(ResFile, "#define VK_F4 0x73");
    FileWriteLine(ResFile, "#define VK_F5 0x74");
    FileWriteLine(ResFile, "#define VK_F6 0x75");
    FileWriteLine(ResFile, "#define VK_F7 0x76");
    FileWriteLine(ResFile, "#define VK_F8 0x77");
    FileWriteLine(ResFile, "#define VK_F9 0x78");
    FileWriteLine(ResFile, "#define VK_F10 0x79");
    FileWriteLine(ResFile, "#define VK_F11 0x7a");
    FileWriteLine(ResFile, "#define VK_F12 0x7b");
    FileWriteLine(ResFile, "#define VK_F13 0x7c");
    FileWriteLine(ResFile, "#define VK_F14 0x7d");
    FileWriteLine(ResFile, "#define VK_F15 0x7e");
    FileWriteLine(ResFile, "#define VK_F16 0x7f");
    FileWriteLine(ResFile, "#define VK_F17 0x80");
    FileWriteLine(ResFile, "#define VK_F18 0x81");
    FileWriteLine(ResFile, "#define VK_F19 0x82");
    FileWriteLine(ResFile, "#define VK_F20 0x83");
    FileWriteLine(ResFile, "#define VK_F21 0x84");
    FileWriteLine(ResFile, "#define VK_F22 0x85");
    FileWriteLine(ResFile, "#define VK_F23 0x86");
    FileWriteLine(ResFile, "#define VK_F24 0x87");
    FileWriteLine(ResFile, "#define VK_NUMLOCK 0x90");
    FileWriteLine(ResFile, "#define VK_SCROLL 0x91");
    FileWriteLine(ResFile, "#define VK_PROCESSKEY 0xe5");
    FileWriteLine(ResFile, "#define VK_ATTN 0xf6");
    FileWriteLine(ResFile, "#define VK_CRSEL 0xf7");
    FileWriteLine(ResFile, "#define VK_EXSEL 0xf8");
    FileWriteLine(ResFile, "#define VK_EREOF 0xf9");
    FileWriteLine(ResFile, "#define VK_PLAY 0xfa");
    FileWriteLine(ResFile, "#define VK_ZOOM 0xfb");
    FileWriteLine(ResFile, "#define VK_NONAME 0xfc");
    FileWriteLine(ResFile, "#define VK_PA1 0xfd");
    FileWriteLine(ResFile, "#define VK_OEM_CLEAR 0xfe");
}

// -----------------------------------------------------------------------
// Convert a file into menus resources
long Convert_Menus(CStr FileName, CStr IndexID, long ResIndex)
{
    long ReturnValue = 0;
    HWND IconhWnd = 0;

    // Force it
    PutExtendedRes = "1";
    IconhWnd = CreateNewResourceFileWin(FileReplaceExtension(FileName, "rc"));
    if(IconhWnd != 0)
    {
        ChildStruct = LoadStructure(IconhWnd);
        CM_SetText(ChildStruct->hChildCodeMax, ConvertMenusRes(FileName, IndexID, ResIndex).Get_String());
        ForceChildFile(IconhWnd);
        ReturnValue = 1;
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a file into menus resources string
CStr ConvertMenusRes(CStr FileName, CStr IndexID, long ResIndex)
{
    CStr ReturnValue;
    CStr MenuEntryName;
    CStr MenuEntryCmd;
    CStr MenuEntryType;
    long MenuEntryTypeLng = 0;
    int i = 0;
    int j = 0;
    CStr MenuEntryProps;
    CStr MenuEntryLevel;
    long MenuEntryLevelLng = 0;
    long MenuEntryOldLevel = 0;
    long CurrentNest = 0;
    long AddFoot = 0;
    long LastItem = 0;
    long PutExtResLng = 0;
    CStr BufString;

    BufString = "#define MFT_STRING 0x00000000\r\n";
    BufString = BufString + "#define MFT_MENUBARBREAK 0x00000020\r\n";
    BufString = BufString + "#define MFT_MENUBREAK 0x00000040\r\n";
    BufString = BufString + "#define MFT_RADIOCHECK 0x00000200\r\n";
    BufString = BufString + "#define MFT_SEPARATOR 0x00000800\r\n";
    BufString = BufString + "#define MFT_RIGHTJUSTIFY 0x00004000\r\n";
    BufString = BufString + "#define MFS_ENABLED 0x00000000\r\n";
    BufString = BufString + "#define MFS_GRAYED 0x00000003\r\n";
    BufString = BufString + "#define MFS_CHECKED 0x00000008\r\n";
    BufString = BufString + "#define MFS_DEFAULT 0x00001000\r\n";
    PutExtResLng = PutExtendedRes.Get_Long();
    if(PutExtResLng == 1) BufString = BufString + IndexID + (CStr) " MENUEX " + (CStr) CalcResourceHeader(GetMenuProps(ResIndex)).Get_String() + (CStr) "\r\n";
    else BufString = BufString + IndexID + (CStr) " MENU " + (CStr) CalcResourceHeader(GetMenuProps(ResIndex)).Get_String() + (CStr) "\r\n";
    BufString = BufString + (CStr) "BEGIN\r\n";
    CurrentNest = 1;
    MenuEntryOldLevel = -1;
    for(i = 0; i <= 999; i++)
    {
        MenuEntryName = IniReadKey("MNUDAT", "MNU" + (CStr) StringNumberComplement(i, 3).Get_String(), FileName);
        if(MenuEntryName.Len() == 0) break;
        MenuEntryCmd = IniReadKey("MNUDAT", "MNUCMD" + (CStr) StringNumberComplement(i, 3).Get_String(), FileName);
        if(MenuEntryCmd.Len() == 0) break;
        MenuEntryType = IniReadKey("MNUDAT", "MNUTYPE" + (CStr) StringNumberComplement(i, 3).Get_String(), FileName);
        if(MenuEntryType.Len() == 0) break;
        MenuEntryProps = IniReadKey("MNUDAT", "MNUPROPS" + (CStr) StringNumberComplement(i, 3).Get_String(), FileName);
        if(MenuEntryProps.Len() == 0) break;
        MenuEntryLevel = IniReadKey("MNUDAT", "MNULEVEL" + (CStr) StringNumberComplement(i, 3).Get_String(), FileName);
        if(MenuEntryLevel.Len() == 0) break;
        MenuEntryTypeLng = MenuEntryType.Get_Long();
        MenuEntryLevelLng = MenuEntryLevel.Get_Long();
        AddFoot = 1;
        switch(MenuEntryTypeLng)
        {
            case MENUTYPE_BAR:
                // Is it a child ?
                if(MenuEntryOldLevel != MenuEntryLevelLng)
                {
                    // Calc delta
                    if((MenuEntryOldLevel - MenuEntryLevelLng) > 0)
                    {
                        // Dig backward
                        if(((long) LastItem == MENUTYPE_POPUP) || ((long) LastItem == MENUTYPE_BAR))
                        {
                            BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
                            BufString = BufString + ",0\r\n";
                            CurrentNest = MenuEntryOldLevel + 1;
                            for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng; j--)
                            {
                                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                                CurrentNest--;
                            }
                        }
                        else
                        {
                            CurrentNest = MenuEntryOldLevel;
                            for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng + 1; j--)
                            {
                                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                                CurrentNest--;
                            }
                        }
                    }
                }
                else
                {
                    BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
                    BufString = BufString + ",0\r\n";
                    CurrentNest = 1;
                    BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                }
                CurrentNest = 1;
                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "POPUP \"" + (CStr) MenuEntryName + (CStr) "\"";
                // No command
                if(PutExtResLng == 1)
                {
                    BufString = BufString + ",,MFT_STRING";
                    if(((MenuEntryProps.Get_Long() & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_RIGHTJUSTIFY) BufString = BufString + "|MFT_RIGHTJUSTIFY";
                    if(((MenuEntryProps.Get_Long() & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_BARBREAK) BufString = BufString + "|MFT_MENUBARBREAK";
                    if(((MenuEntryProps.Get_Long() & MENUPROP_BREAKMASK) / 0x10000) == MENUPROP_BREAK) BufString = BufString + "|MFT_MENUBREAK";
                    if((MenuEntryProps.Get_Long() & MENUPROP_GRAYED) != 0) BufString = BufString + ",MFS_GRAYED";
                    else BufString = BufString + ",MFS_ENABLED";
                    // No help ID
                    BufString = BufString + ",0\r\n";
                }
                else
                {
                    BufString = BufString + "\r\n";
                }
                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "BEGIN\r\n";
                CurrentNest++;
                MenuEntryOldLevel = MenuEntryLevelLng;
                LastItem = MenuEntryTypeLng;
                break;
            case MENUTYPE_POPUP:
                // Is it a child ?
                if(MenuEntryOldLevel != MenuEntryLevelLng)
                {
                    // Calc delta
                    if((MenuEntryOldLevel - MenuEntryLevelLng) > 0)
                    {
                        // Dig backward
                        if(((long) LastItem == MENUTYPE_POPUP) || ((long) LastItem == MENUTYPE_BAR))
                        {
                            BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
                            BufString = BufString + ",0\r\n";
                            CurrentNest = MenuEntryOldLevel + 1;
                            for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng; j--)
                            {
                                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                                CurrentNest--;
                            }
                        }
                        else
                        {
                            CurrentNest = MenuEntryOldLevel;
                            for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng + 1; j--)
                            {
                                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                                CurrentNest--;
                            }
                        }
                    }
                }
                else
                {
                    if(((long) LastItem == MENUTYPE_POPUP) || ((long) LastItem == MENUTYPE_BAR))
                    {
                        BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
                        BufString = BufString + ",0\r\n";
                        CurrentNest = MenuEntryOldLevel + 1;
                        BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                        CurrentNest--;
                    }
                }
                BufString = BufString + AddResNest(MenuEntryLevelLng + 1).Get_String() + (CStr) "POPUP \"" + (CStr) MenuEntryName + (CStr) "\"";
                // No command
                if(PutExtResLng == 1) BufString = BufString + ",,0\r\n";
                else BufString = BufString + "\r\n";
                BufString = BufString + AddResNest(MenuEntryLevelLng + 1).Get_String() + (CStr) "BEGIN\r\n";
                MenuEntryOldLevel = MenuEntryLevelLng;
                LastItem = MenuEntryTypeLng;
                break;
            case MENUTYPE_ITEM:
                // Is it a child ?
                if(MenuEntryOldLevel != MenuEntryLevelLng)
                {
                    // Calc delta
                    if((MenuEntryOldLevel - MenuEntryLevelLng) > 0)
                    {
                        // Dig backward
                        if(((long) LastItem == MENUTYPE_POPUP) || ((long) LastItem == MENUTYPE_BAR))
                        {
                            BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
                            BufString = BufString + ",0\r\n";
                            CurrentNest = MenuEntryOldLevel + 1;
                            for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng; j--)
                            {
                                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                                CurrentNest--;
                            }
                        }
                        else
                        {
                            CurrentNest = MenuEntryOldLevel;
                            for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng + 1; j--)
                            {
                                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                                CurrentNest--;
                            }
                        }
                    }
                }
                else
                {
                    if(((long) LastItem == MENUTYPE_POPUP) || ((long) LastItem == MENUTYPE_BAR))
                    {
                        BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
                        BufString = BufString + ",0\r\n";
                        CurrentNest = MenuEntryOldLevel + 1;
                        BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                        CurrentNest--;
                    }
                }
                BufString = BufString + AddResNest(MenuEntryLevelLng + 1).Get_String() + (CStr) "MENUITEM \"" + (CStr) MenuEntryName + (CStr) "\"";
                BufString = BufString + "," + (CStr) MenuEntryCmd;
                if(PutExtResLng == 1)
                {
                    BufString = BufString + ",MFT_STRING";
                    if((MenuEntryProps.Get_Long() & MENUPROP_RADIO) != 0) BufString = BufString + "|MFT_RADIOCHECK";
                    if((MenuEntryProps.Get_Long() & MENUPROP_GRAYED) != 0) BufString = BufString + ",MFS_GRAYED";
                    else BufString = BufString + ",MFS_ENABLED";
                    if((MenuEntryProps.Get_Long() & MENUPROP_BOLD) != 0) BufString = BufString + "|MFS_DEFAULT";
                    if((MenuEntryProps.Get_Long() & MENUPROP_CHECKED) != 0) BufString = BufString + "|MFS_CHECKED";
                }
                BufString = BufString + "\r\n";
                MenuEntryOldLevel = MenuEntryLevelLng;
                LastItem = MenuEntryTypeLng;
                break;
            case MENUTYPE_SEPS:
                // Is it a child ?
                if(MenuEntryOldLevel != MenuEntryLevelLng)
                {
                    // Calc delta
                    if((MenuEntryOldLevel - MenuEntryLevelLng) > 0)
                    {
                        // Dig backward
                        if(((long) LastItem == MENUTYPE_POPUP) || ((long) LastItem == MENUTYPE_BAR))
                        {
                            BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
                            BufString = BufString + ",0\r\n";
                            CurrentNest = MenuEntryOldLevel + 1;
                            for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng; j--)
                            {
                                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                                CurrentNest--;
                            }
                        }
                        else
                        {
                            CurrentNest = MenuEntryOldLevel;
                            for(j = MenuEntryOldLevel; j >= (long) MenuEntryLevelLng + 1; j--)
                            {
                                BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                                CurrentNest--;
                            }
                        }
                    }
                }
                else
                {
                    if(((long) LastItem == MENUTYPE_POPUP) || ((long) LastItem == MENUTYPE_BAR))
                    {
                        BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
                        BufString = BufString + ",0\r\n";
                        CurrentNest = MenuEntryOldLevel + 1;
                        BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
                        CurrentNest--;
                    }
                }
                if(PutExtResLng == 1) BufString = BufString + AddResNest(MenuEntryLevelLng + 1).Get_String() + (CStr) "MENUITEM \"\",,MFT_SEPARATOR\r\n";
                else BufString = BufString + AddResNest(MenuEntryLevelLng + 1).Get_String() + (CStr) "MENUITEM SEPARATOR\r\n";
                MenuEntryOldLevel = MenuEntryLevelLng;
                LastItem = MenuEntryTypeLng;
                break;
        }   
    }
    // Write primary foot
    if(AddFoot == 1)
    {
        if(((long) LastItem == MENUTYPE_POPUP) || ((long) LastItem == MENUTYPE_BAR))
        {
            BufString = BufString + AddResNest(MenuEntryOldLevel + 2).Get_String() + (CStr) "MENUITEM \"<Empty entry>\"";
            BufString = BufString + ",0\r\n";
            CurrentNest = MenuEntryOldLevel + 1;
        }
        else
        {
            CurrentNest = MenuEntryOldLevel;
        }
        for(j = MenuEntryOldLevel; j >= 0; j--)
        {
            if(CurrentNest == 0) break;
            BufString = BufString + AddResNest(CurrentNest).Get_String() + (CStr) "END\r\n";
            CurrentNest--;
        }
    }
    BufString = BufString + (CStr) "END\r\n";
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert current nest level into spaces
CStr AddResNest(long NestLevel)
{
    CStr ReturnValue;
    CStr BufString;

    if(NestLevel > 0) BufString = BufString.String(NestLevel, 9);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a file into raw datas
long Convert_Raw(CStr FileName, long IndexID, long ResIndex, long UseComma, CStr StrRawDat, long ByFile, int IncludeLang)
{
    long FLen = 0;
    long ReadedDB = 0;
    long DBFileMem = 0;
    char *CurrentEntry = "";
    CStr BufString;
    long ReturnValue = 0;
    
    if(ByFile == 1) FLen = FileGetSize(FileName);
    else FLen = FileName.Len();
    if(FLen != 0)
    {
        if(ByFile == 1)
        {
            // Load the file into memory
            DBFileMem = (long) MLoadFile(FileName.Get_String(), &ReadedDB);
        }
        else
        {
            // Alloc and copy string directly
            DBFileMem = AllocMem(FLen);
            RtlCopyMemory((void *)DBFileMem, FileName.Get_String(), FLen);
        }
        if(DBFileMem == 0)
        {
            return(ReturnValue);
        }
        else
        {
            if(UseComma == 1)
            {
                // Set buffer to max len
                CurrentEntry = TabRcComma[0];
                ConversionStringBody = ConversionStringBody.String(((FLen / 16) * strlen(CurrentEntry)), ' ');
                // Remaining bytes
                ConversionStringBodyEnd = ConversionStringBodyEnd.String(strlen(CurrentEntry), ' ');
                // Fill the buffer
                CopyMem(CurrentEntry, ConversionStringBody.Get_String(), strlen(CurrentEntry), 0, (FLen / 16));
                // Fill the remaining buffer
                if((FLen % 16) != 0) ConversionStringBodyEnd = TabRcCommaEnd[(16 - (FLen % 16)) / 2];
                //Convert main part
                if(FLen > 14) FillHexWords(DBFileMem, ConversionStringBody.Get_String(), 16, 3, 3, (FLen / 16), 3);
                // Convert remaining bytes
                if((FLen % 16) != 0) FillHexWords(DBFileMem + (FLen - (FLen % 16)), ConversionStringBodyEnd.Get_String(), (FLen % 16), 3, 0, 1, 3);
                //Write the header
                if(IncludeLang) ConversionStringHead = WriteLanguageEquate(GetRawdataLang(ResIndex)) + (CStr) "\r\n";
                else ConversionStringHead = "";
                ConversionStringHead = ConversionStringHead + (CStr) IndexID + (CStr) " " + (CStr) StrRawDat.Get_String() + (CStr) " ";
                // Put header
                if(IncludeLang) ConversionStringHead = ConversionStringHead + CalcResourceHeader(GetRawdataProps(ResIndex)).Trim().Get_String() + (CStr) "\r\n";
                // Copy raw datas
                ConversionStringHead = ConversionStringHead + "BEGIN\r\n";
                if(ConversionStringBodyEnd.Len() != 0)
                {
                    ConversionStringBodyEnd = "\t" + ConversionStringBodyEnd.Trim() + "\r\n";
                }
                ConversionStringHead = ConversionStringHead + ConversionStringBody + ConversionStringBodyEnd;
                ConversionStringHead = ConversionStringHead + "END\r\n";
                // Free the allocated memory for the file
                FreeMem(DBFileMem);
                ReturnValue = 1;
            }
            else
            {
                // Set buffer to max len
                CurrentEntry = TabRc[0];
                ConversionStringBody = ConversionStringBody.String(((FLen / 16) * strlen(CurrentEntry)), ' ');
                // Remaining bytes
                ConversionStringBodyEnd = ConversionStringBodyEnd.String(strlen(CurrentEntry), ' ');
                //Fill the buffer
                CopyMem(CurrentEntry, ConversionStringBody.Get_String(), strlen(CurrentEntry), 0, (FLen / 16));
                //Fill the remaining buffer
                if((FLen % 16) != 0) ConversionStringBodyEnd = TabRc[(16 - (FLen % 16)) / 2];
                //Convert main part
                if(FLen > 14) FillHexWords(DBFileMem, ConversionStringBody.Get_String(), 16, 3, 2, (FLen / 16), 3);
                // Convert remaining bytes
                if((FLen % 16) != 0) FillHexWords(DBFileMem + (FLen - (FLen % 16)), ConversionStringBodyEnd.Get_String(), (FLen % 16), 3, 0, 1, 3);
                // Write the header
                if(IncludeLang) ConversionStringHead = WriteLanguageEquate(GetRawdataLang(ResIndex)) + (CStr) "\r\n";
                else ConversionStringHead = "";
                ConversionStringHead = ConversionStringHead + (CStr) IndexID + (CStr) " " + (CStr) StrRawDat.Get_String() + (CStr) " ";
                // Put header
                if(IncludeLang) ConversionStringHead = ConversionStringHead + CalcResourceHeader(GetRawdataProps(ResIndex)).Trim() + (CStr) "\r\n";
                // Copy raw datas
                ConversionStringHead = ConversionStringHead + "BEGIN\r\n";
                if(ConversionStringBodyEnd.Len() != 0)
                {
                    ConversionStringBodyEnd = "\t" + ConversionStringBodyEnd.Trim() + "\r\n";
                }
                ConversionStringHead = ConversionStringHead + ConversionStringBody + ConversionStringBodyEnd;
                ConversionStringHead = ConversionStringHead + "END\r\n";
                // Free the allocated memory for the file
                FreeMem(DBFileMem);
                ReturnValue = 1;
            }
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Convert a file into icon datas
void Convert_Icon(CStr FileName, CStr IndexID, long ResIndex)
{
    HWND IconhWnd = 0;
    CStr BufString;
    
    IconhWnd = CreateNewResourceFileWin(FileReplaceExtension(FileName, "rc"));
    if(IconhWnd != 0)
    {
        ChildStruct = LoadStructure(IconhWnd);
        BufString = IndexID + (CStr) " ICON " + (CStr) CalcResourceHeader(GetIconProps(ResIndex)).Get_String() + (CStr) " " + (CStr) ResIncludeFile(FileName).Get_String();
        CM_SetText(ChildStruct->hChildCodeMax, BufString.Get_String());
        ForceChildFile(IconhWnd);
    }
}

// -----------------------------------------------------------------------
// Convert a file into cursor datas
void Convert_Cursor(CStr FileName, CStr IndexID, long ResIndex)
{
    HWND IconhWnd = 0;
    CStr BufString;
    
    IconhWnd = CreateNewResourceFileWin(FileReplaceExtension(FileName, "rc"));
    if(IconhWnd != 0)
    {
        ChildStruct = LoadStructure(IconhWnd);
        BufString = IndexID + (CStr) " CURSOR " + (CStr) CalcResourceHeader(GetIconProps(ResIndex)).Get_String() + (CStr) " " + (CStr) ResIncludeFile(FileName).Get_String();
        CM_SetText(ChildStruct->hChildCodeMax, BufString.Get_String());
        ForceChildFile(IconhWnd);
    }
}

// -----------------------------------------------------------------------
// Convert a file into bitmap datas
void Convert_Bitmap(CStr FileName, CStr IndexID, long ResIndex)
{
    HWND IconhWnd = 0;
    CStr BufString;

    IconhWnd = CreateNewResourceFileWin(FileReplaceExtension(FileName, "rc"));
    if(IconhWnd != 0)
    {
        ChildStruct = LoadStructure(IconhWnd);
        BufString = IndexID + (CStr) " BITMAP " + (CStr) CalcResourceHeader(GetIconProps(ResIndex)).Get_String() + (CStr) " " + (CStr) ResIncludeFile(FileName).Get_String();
        CM_SetText(ChildStruct->hChildCodeMax, BufString.Get_String());
        ForceChildFile(IconhWnd);
    }
}

// -----------------------------------------------------------------------
// Convert a file into strings datas
void Convert_Strings(CStr FileName, CStr IndexID, long ResIndex)
{
    int i = 0;
    int j = 0;
    CStr StringResToPaste;
    CStr StringStr;
    HWND IconhWnd = 0;
    
    IconhWnd = CreateNewResourceFileWin(FileReplaceExtension(FileName, "rc"));
    if(IconhWnd != 0)
    {
        ChildStruct = LoadStructure(IconhWnd);
        StringResToPaste = WriteLanguageEquate(Get_StringLang(ResIndex)) + (CStr) "\r\n";
        StringResToPaste = StringResToPaste + "STRINGTABLE\r\n";
        StringResToPaste = StringResToPaste + CalcResourceHeader(Get_StringProps(ResIndex)) + (CStr) "\r\n";
        StringResToPaste = StringResToPaste + CalcResourceLanguage(Get_StringLang(ResIndex)) + (CStr) "\r\n";
        StringResToPaste = StringResToPaste + "BEGIN\r\n";
        i = ResIndex * 16;
        for(j = 0; j <= 15; j++)
        {
            StringStr = IniReadKey("STRDAT", "STR" + (CStr) StringNumberComplement(j, 3).Get_String(), FileName);
            if(StringStr.Len() != 0) StringResToPaste = StringResToPaste + (CStr) " " + (CStr) i + (CStr) ",\"" + (CStr) ResIncludeString(StringStr).Get_String() + (CStr) "\"\r\n";
            i++;
        }
        StringResToPaste = StringResToPaste + "END\r\n";
        CM_SetText(ChildStruct->hChildCodeMax, StringResToPaste.Get_String());
        ForceChildFile(IconhWnd);
    }
}

// -----------------------------------------------------------------------
// Convert a file into accelerators datas
void Convert_Accelerators(CStr FileName, CStr IndexID, long ResIndex)
{
    int i = 0;
    int j = 0;
    long PosInAccelKey = 0;
    long AccelCtrlState = 0;
    long AccelAltState = 0;
    long AccelShiftState = 0;
    CStr AccelKey;
    CStr CtrlAltShiftStr;
    CStr StrToAddToAccel;
    CStr AcceleratorKeys;
    CStr AcceleratorCmd;
    CStr StringResToPaste;
    CStr BufString;
    HWND IconhWnd = 0;

    IconhWnd = CreateNewResourceFileWin(FileReplaceExtension(FileName, "rc"));
    if(IconhWnd != 0)
    {
        ChildStruct = LoadStructure(IconhWnd);
        StringResToPaste = WriteLanguageEquate(GetAcceleratorLang(ResIndex)) + (CStr) "\r\n";
        StringResToPaste = StringResToPaste + IndexID + (CStr) " ACCELERATORS " + (CStr) CalcResourceHeader(GetAcceleratorProps(ResIndex)).Get_String() + (CStr) "\r\n";
        StringResToPaste = StringResToPaste + CalcResourceLanguage(GetAcceleratorLang(ResIndex)) + (CStr) "\r\n";
        StringResToPaste = StringResToPaste + "BEGIN\r\n";
        for(i = 0; i <= 999; i++)
        {
            AcceleratorKeys = IniReadKey("ACCELDAT", "ACCEL" + (CStr) StringNumberComplement(i, 3).Get_String(), FileName);
            AcceleratorCmd = IniReadKey("ACCELDAT", "COMMAND" + (CStr) StringNumberComplement(i, 3).Get_String(), FileName);
            if(AcceleratorKeys.Len() == 0) break;
            if(AcceleratorCmd.Len() == 0) break;
            AccelCtrlState = 0;
            AccelAltState = 0;
            AccelShiftState = 0;
            PosInAccelKey = 1;
            StrToAddToAccel = "\"";
            AccelKey = "";
GetAccelerator:
            BufString = AcceleratorKeys.Mid(PosInAccelKey, 5);
            if(BufString == "CTRL ")
            {
                AccelCtrlState = 1;
                PosInAccelKey = PosInAccelKey + 7;
                goto GetAccelerator;
            }
            else if(BufString == "ALT +")
            {
                AccelAltState = 1;
                PosInAccelKey = PosInAccelKey + 6;
                goto GetAccelerator;
            }
            else if(BufString == "SHIFT")
            {
                AccelShiftState = 1;
                PosInAccelKey = PosInAccelKey + 8;
                goto GetAccelerator;
            }
            else
            {
                AccelKey = AcceleratorKeys.Mid(PosInAccelKey).Trim();
            }
            CtrlAltShiftStr = "";
            if(AccelCtrlState != 0) CtrlAltShiftStr = "CONTROL";
            if(AccelShiftState != 0)
            {
                if(CtrlAltShiftStr.Len() != 0) CtrlAltShiftStr = CtrlAltShiftStr + ",";
                CtrlAltShiftStr = CtrlAltShiftStr + "SHIFT";
            }
            if(AccelAltState != 0)
            {
                if(CtrlAltShiftStr.Len() != 0) CtrlAltShiftStr = CtrlAltShiftStr + ",";
                CtrlAltShiftStr = CtrlAltShiftStr + "ALT";
            }
            if(CtrlAltShiftStr.Len() != 0) CtrlAltShiftStr = CtrlAltShiftStr + ",";
            // TOCHECK 84
            for(j = 0; j < 84; j++)
            {
                if(_strcmpi((char *) TabAccelerators[j], AccelKey.Get_String()) == 0)
                {
                    StrToAddToAccel = "";
                    break;
                }
            }
            AccelKey = StrToAddToAccel + AccelKey + (CStr) StrToAddToAccel;
            StringResToPaste = StringResToPaste + " " + (CStr) AccelKey + (CStr) "," + (CStr) AcceleratorCmd + (CStr) "," + (CStr) CtrlAltShiftStr + (CStr) "VIRTKEY\r\n";
        }
        StringResToPaste = StringResToPaste + "END\r\n";
        CM_SetText(ChildStruct->hChildCodeMax, StringResToPaste.Get_String());
        ForceChildFile(IconhWnd);
    }
}

// -----------------------------------------------------------------------
// Create a new window to contains resources
HWND CreateNewResourceFileWin(CStr ResFile)
{
    CStr OpenedLanguage;
    OpenedLanguage = "Resource";
    StoreLanguageToOpen(OpenedLanguage);
    return(CreateNewFile(ResFile));
}

// -----------------------------------------------------------------------
// Run a DOS command and trap the errors
void CaptureDOSError(CStr DOSCmdName, CStr DOSCmdArgs, CStr DOSCmdDir)
{
    CStr StrCapture;
    unsigned long OutSizeRead = 0;
    long *ArrayCapture = 0;
    SECURITY_ATTRIBUTES SecAt;
    CStr StringtoWrite;
    CStr StringtoWriteReport;
    CStr BufString;
    CStr DOSCmdDirBuf;
    int i = 0;

    RtlZeroMemory(&MyProcStartup, sizeof(MyProcStartup));
    RtlZeroMemory(&MyProcInfos, sizeof(MyProcInfos));
    SecAt.nLength = sizeof(SecAt);
    SecAt.lpSecurityDescriptor = 0;
    SecAt.bInheritHandle = -1;
    if(CreatePipe(&PipeIn, &PipeOut, &SecAt, 65535 * 2) == 0)
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Unable to trap command.");
        return;
    }
    MyProcStartup.cb = sizeof(MyProcStartup);
    MyProcStartup.lpDesktop = 0;
    MyProcStartup.lpReserved = 0;
    MyProcStartup.lpTitle = "";
    MyProcStartup.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    MyProcStartup.hStdInput = 0;
    MyProcStartup.hStdOutput = PipeOut;
    MyProcStartup.hStdError = PipeOut;
    MyProcStartup.wShowWindow = SW_HIDE;
    DOSCmdDirBuf = DOSCmdDir;
    if(DOSCmdDirBuf.Len() == 0) DOSCmdDirBuf = FileGetDirectory(DOSCmdName);
    if(DOSCmdDirBuf.Len() == 0) DOSCmdDirBuf = CurDir();
    if(strcmp(DOSCmdDirBuf.Right(1).Get_String(), "\\") == 0) DOSCmdDirBuf = DOSCmdDirBuf.Mid(1, DOSCmdDirBuf.Len() - 1);
    // Create the process
    WriteComment("Executing process...");
    CursorSetWait();
    DOSCmdName = StringReplace(DOSCmdName, "\"", "", 1, -1, Binary_Compare);
    DOSCmdDirBuf = StringReplace(&DOSCmdDirBuf, "\"", "", 1, -1, Binary_Compare);
    BufString = (CStr) CorrectFileName(DOSCmdName).Get_String() + (CStr) " " + (CStr) DOSCmdArgs.Get_String();
    CreateProcess(DOSCmdName.Get_String(), BufString.Get_String(), &SecAt, &SecAt, 1, 0, NULL, DOSCmdDirBuf.Get_String(), &MyProcStartup, &MyProcInfos);
    if(MyProcInfos.hProcess != 0)
    {
        // --- Display captured text in status window
        WaitForSingleObject(MyProcInfos.hProcess, -1);
        CloseHandle(PipeOut);
        StrCapture = StrCapture.String(65535, 1);
        if(ReadFile(PipeIn, StrCapture.Get_String(), 65535, &OutSizeRead, NULL) != 0)
        {
            if(AutoClearOut == 1) ClearStatus();
            ArrayCapture = StringSplit(StrCapture, "\n");
            for(i = 0; i <= (long) StringGetSplitUBound(ArrayCapture); i++)
            {
                StringtoWrite = StringReplace(StringReplace(StringGetSplitElement(StrCapture, ArrayCapture, i), "\r" , "", 1, -1, Binary_Compare), BufString.Chr(0xFF), " ", 1, -1, Binary_Compare);
                StringtoWrite = StringReplace(StringtoWrite, "\r", "", 1, -1, Binary_Compare);
                StringtoWriteReport = IsErrorReported(StringtoWrite);
                if(StringtoWriteReport.Len() != 0) WriteToStatus(StringtoWrite);
            }
        }
        StringReleaseSplit(ArrayCapture);
        CloseHandle(PipeIn);
        CloseHandle(MyProcInfos.hThread);
        CloseHandle(MyProcInfos.hProcess);
        CursorSetNormal();
        WriteComment("");
        return;
    }
    else
    {
        WriteToStatus(PutStatusDatePrefix().Get_String() + (CStr) "Unable to run command : '" + (CStr) DOSCmdName.Get_String() + (CStr) "'");
        CloseHandle(PipeOut);
        CloseHandle(PipeIn);
    }
    CursorSetNormal();
    WriteComment("");
}

// -----------------------------------------------------------------------
// Check if a text line contains an error
CStr IsErrorReported(CStr LineToCheck)
{
    long PosLibName = 0;
    CStr LibToFind;
    long LineAsm = 0;
    long PosAsm = 0;
    CStr LibLinkFound;
    CStr BufString;
    
    if(LineToCheck[0] == 1) LineToCheck = "";
    PosLibName = LineToCheck.In_Str(1, "undefined symbol : ", Text_Compare);
    if(PosLibName != 0)
    {
        PosLibName = PosLibName + strlen("undefined symbol : ");
        LibToFind = LineToCheck.Mid(PosLibName);
        if(LibToFind.Len() != 0)
        {
            LibLinkFound = RetrieveAPI();
            if(LibLinkFound.Len() != 0) LibLinkFound = "    (Found in: " + (CStr) LibLinkFound + (CStr) ")";
            else LibLinkFound = "    <Not found in database>";
            BufString = LineToCheck + LibLinkFound;
            LineToCheck = BufString;
        }
    }
    else
    {
        // Check for unresolved symbols and display corresponding library
        PosLibName = LineToCheck.In_Str(1, "unresolved external symbol", Text_Compare);
        if(PosLibName != 0)
        {
            PosLibName = PosLibName + strlen("unresolved external symbol") + 1;
            LibToFind = LineToCheck.Mid(PosLibName);
            if(LibToFind.Len() != 0)
            {
                LibLinkFound = RetrieveAPI();
                if(LibLinkFound.Len() != 0)
                {
                    LibLinkFound = "    (Found in: " + (CStr) LibLinkFound + (CStr) ")";
                }
                else
                {
                    LibLinkFound = "    <Not found in database>";
                }
                BufString = LineToCheck + LibLinkFound;
                LineToCheck = BufString;
            }
        }
    }
    
    return(LineToCheck);
}
