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
// Languages.cpp: Languages handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"
#include "Globals.h"
#include "Languages.h"
#include "MiscFunctions.h"
#include "ConstCodeMax.h"
#include "MDI_Childs.h"
#include "AddIns.h"

// -----------------------------------------------------------------------
// Variables
CList <CM_LANGUAGE *> LanguageToRegister;

CStr LangName;
CStr LangCopyRight;
CStr LangComment;
long LangCaseSensitive;
CStr LangSingleComment;
CStr LangMultiPrefix;
CStr LangMultiLineComment1;
CStr LangMultiLineComment2;
CStr LangScopeKeyWord1;
CStr LangScopeKeyWord2;
CStr LangStringsDelimiters;
CStr LangEscapeChar;
CStr LangTerminatorChar;
long LangStyle;
CStr LangRetVal;

CStr LangColorA;
CStr LangColorB;
CStr LangColorC;
CStr LangColorD;
CStr LangColorE;
CStr LangColorF;
CStr LangColorG;
CStr LangColorH;

CStr LangHelpA;
CStr LangHelpB;
CStr LangHelpC;
CStr LangHelpD;
CStr LangHelpE;
CStr LangHelpF;
CStr LangHelpG;
CStr LangHelpH;

CStr LangExtensions;
CStr LangProc;
CStr LangEndp;
CStr LangGUID;
CStr LangInclude;
CStr LangIncludeName;
CStr LangIncludeNameFoot;
CStr LangInterlaceProc;
CStr LangOpenBlockProc;
CStr LangCloseBlockProc;
CStr LangOphanBlock;
CStr LangDoubleSlash;
CStr LangFileName;
CStr LangHighLightNbr;
CStr LangHexType;
CStr LangBinType;

CStr LangFileToRead;

int Nbr_Languages;
FILETIME *Language_Dates;
CStr New_LangName;
CStr Old_LangName;

// -----------------------------------------------------------------------
// Functions
long CALLBACK Update_Languages_Proc(HWND hWnd);

// -----------------------------------------------------------------------
// Read the languages specifications and register them into codemax
void ReadLangSpec(void)
{
    int i = 0;

    LanguageToRegister.Erase();
    for(i = 0; i <= 999; i++)
    {
        LangFileToRead = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(LangFileToRead.Len() == 0) break;
        LangExtensions = IniReadKey("RefLanguages", "Ext" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        LangFileToRead = ChangeRelativePaths(LangFileToRead);
        LoadLanguageDef();
        RegisterCodeMaxLanguage(-1);
    }

    // Store the last known write times of the language files
    Language_Dates = (FILETIME *) malloc(i * sizeof(FILETIME));
    Nbr_Languages = i;
    for(i = 0; i <= 999; i++)
    {
        LangFileToRead = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(LangFileToRead.Len() == 0) break;
        LangFileToRead = ChangeRelativePaths(LangFileToRead);
        FileGetWriteTime(LangFileToRead, &Language_Dates[i]);
    }
}

// -----------------------------------------------------------------------
// Check if we must update the languages
void Check_Update_Language(void)
{
    int i;
    FILETIME TmpLanguageFDate;
    CStr Language_Name_From_Ini;

    for(i = 0; i <= Nbr_Languages; i++)
    {
        Language_Name_From_Ini = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(Language_Name_From_Ini.Len() == 0) break;
        Language_Name_From_Ini = ChangeRelativePaths(Language_Name_From_Ini);
        if(FileGetWriteTime(Language_Name_From_Ini, &TmpLanguageFDate))
        {
            if(CompareFileTime(&TmpLanguageFDate, &Language_Dates[i]) > 0)
            {
                Update_Language(Language_Name_From_Ini);
            }
        }
    }
}
// -----------------------------------------------------------------------
// Update all registered languages
void Update_All_Languages(void)
{
    int i;
    CStr Language_Name_From_Ini;

    ReadLangSpec();
    for(i = 0; i <= Nbr_Languages; i++)
    {
        Language_Name_From_Ini = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(Language_Name_From_Ini.Len() == 0) break;
        Language_Name_From_Ini = ChangeRelativePaths(Language_Name_From_Ini);
        Update_Language(Language_Name_From_Ini);
    }
}

// -----------------------------------------------------------------------
// Read the languages specifications and register them into codemax
void Update_Language(CStr Language_FileName)
{
    int i = 0;
    CStr Language_Name_From_Ini;
    int Found_Language = FALSE;
    int Old_Entry_Pos = -1;
    CM_LANGUAGE *CurLanguageToRegister;

    for(i = 0; i <= 999; i++)
    {
        Language_Name_From_Ini = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(i, 3).Get_String(), LanguagesIniFile);
        if(Language_Name_From_Ini.Len() == 0) break;
        Language_Name_From_Ini = ChangeRelativePaths(Language_Name_From_Ini);
        if(Language_FileName == Language_Name_From_Ini)
        {
            // Found the file
            Found_Language = TRUE;
            Old_Entry_Pos = i;
            break;
        }
    }
    if(Found_Language)
    {
        New_LangName = IniReadKey("LangSpec", "LangName", Language_FileName);

        // This is not a language file
        if(New_LangName != "")
        {
            // Remove it from the list
            CurLanguageToRegister = LanguageToRegister.Get(Old_Entry_Pos)->Content;
            Old_LangName = (char *) CurLanguageToRegister->pszLanguageName;
            if(CurLanguageToRegister) free((void *) CurLanguageToRegister);
            // Unregister it from codemax
            hCodeMaxCMUnRegisterLanguage(Old_LangName.Get_String());

            // Name
            LangFileToRead = IniReadKey("RefLanguages", "Lang" + (CStr) StringNumberComplement(Old_Entry_Pos, 3).Get_String(), LanguagesIniFile);
            if(LangFileToRead.Len() == 0) return;
            // Extensions
            LangExtensions = IniReadKey("RefLanguages", "Ext" + (CStr) StringNumberComplement(Old_Entry_Pos, 3).Get_String(), LanguagesIniFile);
            LangFileToRead = ChangeRelativePaths(LangFileToRead);
            LoadLanguageDef();
            RegisterCodeMaxLanguage(Old_Entry_Pos);

            FileGetWriteTime(Language_FileName, &Language_Dates[Old_Entry_Pos]);
        
            // Update the languages in the opened windows
            WAMMEnumSourceWindows((long) &Update_Languages_Proc);           
        }
    }
}

// -----------------------------------------------------------------------
// Callback for WAMMEnumChilds
long CALLBACK Update_Languages_Proc(HWND hWnd)
{
    ChildStruct = LoadStructure(hWnd);
    CStr NewRFile;
    CStr Old_CodeMaxLanguage;

    if(ChildStruct)
    {
        NewRFile = CMGetRealFile(ChildStruct->RFile);
        Old_CodeMaxLanguage = CMGetRealFile(ChildStruct->Language);
        if(Old_LangName == Old_CodeMaxLanguage)
        {
            CM_SetLanguage(ChildStruct->hChildCodeMax, New_LangName.Get_String());
            StoreLanguage(New_LangName);
            RefreshIdentScope(ChildStruct->hChildCodeMax);
        }
    }
    return(1);
}

// -----------------------------------------------------------------------
// Load a language definition file
void LoadLanguageDef(void)
{
    int j = 0;
    long *TabMulti = 0;
    long LangLen = 0;
    CStr SaveMultiLineComment1;
    CStr SaveMultiLineComment2;

    if(LangFileToRead.Len() == 0) return;
    LangName = IniReadKey("LangSpec", "LangName", LangFileToRead);
    LangCopyRight = IniReadKey("LangSpec", "LangCopyright", LangFileToRead);
    LangComment = IniReadKey("LangSpec", "LangComment", LangFileToRead);
    LangCaseSensitive = IniReadKey("LangSpec", "CaseSensitive", LangFileToRead).Get_Long();
    LangSingleComment = IniReadKey("LangSpec", "SingleComment", LangFileToRead);
    // Parse single line comments
    LangSingleComment = StringReplace(LangSingleComment.Trim(), " ", "\n", 1, -1, Binary_Compare);
    LangMultiPrefix = IniReadKey("LangSpec", "MultiLinePrefix", LangFileToRead).Trim();
    LangMultiLineComment1 = IniReadKey("LangSpec", "MultiLineComment1", LangFileToRead);
    LangMultiLineComment2 = IniReadKey("LangSpec", "MultiLineComment2", LangFileToRead);
    // Parse asm comments blocks
    LangMultiLineComment1 = StringReplace(LangMultiLineComment1.Trim(), " ", "\n", 1, -1, Binary_Compare);
    LangMultiLineComment2 = StringReplace(LangMultiLineComment2.Trim(), " ", "\n", 1, -1, Binary_Compare);
    SaveMultiLineComment1 = LangMultiLineComment1;
    if(LangMultiPrefix.Len() != 0)
    {
        TabMulti = StringSplit(SaveMultiLineComment1, "\n");
        LangMultiLineComment1 = "";
        for(j = 0; j <= (long) StringGetSplitUBound(TabMulti); j++)
        {
            LangMultiLineComment1 = LangMultiLineComment1 + (CStr) LangMultiPrefix + (CStr) " " + (CStr) StringGetSplitElement(SaveMultiLineComment1, TabMulti, j).Get_String() + (CStr) "\n";
        }
        for(j = 0; j <= (long) StringGetSplitUBound(TabMulti); j++)
        {
            LangMultiLineComment1 = LangMultiLineComment1 + (CStr) LangMultiPrefix + (CStr) "\t" + (CStr) StringGetSplitElement(SaveMultiLineComment1, TabMulti, j).Get_String() + (CStr) "\n";
        }
        for(j = 0; j <= (long) StringGetSplitUBound(TabMulti); j++)
        {
            LangMultiLineComment1 = LangMultiLineComment1 + (CStr) LangMultiPrefix + (CStr) StringGetSplitElement(SaveMultiLineComment1, TabMulti, j).Get_String() + (CStr) "\n";
        }
        if(LangMultiLineComment1.Len() != 0)
        {
            LangMultiLineComment1 = LangMultiLineComment1.Mid(1, LangMultiLineComment1.Len() - 1);
        }
        StringReleaseSplit(TabMulti);
    }
    // Parse matching ones
    if(LangMultiPrefix.Len() != 0)
    {
        SaveMultiLineComment2 = LangMultiLineComment2;
        TabMulti = StringSplit(SaveMultiLineComment2, "\n");
        LangMultiLineComment2 = "";
        for(j = 0; j <= (long) StringGetSplitUBound(TabMulti); j++)
        {
            LangMultiLineComment2 = LangMultiLineComment2 + (CStr) StringGetSplitElement(SaveMultiLineComment2, TabMulti, j).Get_String() + (CStr) "\n";
        }
        for(j = 0; j <= (long) StringGetSplitUBound(TabMulti); j++)
        {
            LangMultiLineComment2 = LangMultiLineComment2 + (CStr) StringGetSplitElement(SaveMultiLineComment2, TabMulti, j).Get_String() + (CStr) "\n";
        }
        for(j = 0; j <= (long) StringGetSplitUBound(TabMulti); j++)
        {
            LangMultiLineComment2 = LangMultiLineComment2 + (CStr) StringGetSplitElement(SaveMultiLineComment2, TabMulti, j).Get_String() + (CStr) "\n";
        }
        if(LangMultiLineComment2.Len() != 0)
        {
            LangMultiLineComment2 = LangMultiLineComment2.Mid(1, LangMultiLineComment2.Len() - 1);
        }
        StringReleaseSplit(TabMulti);
    }
    LangScopeKeyWord1 = IniReadKey("LangSpec", "ScopeKeyWords1", LangFileToRead);
    LangScopeKeyWord2 = IniReadKey("LangSpec", "ScopeKeyWords2", LangFileToRead);
    LangScopeKeyWord1 = StringReplace(LangScopeKeyWord1.Trim(), " ", "\n", 1, -1, Binary_Compare);
    LangScopeKeyWord2 = StringReplace(LangScopeKeyWord2.Trim(), " ", "\n", 1, -1, Binary_Compare);
    LangStringsDelimiters = StringReplace(IniReadKey("LangSpec", "StringsDelimiters", LangFileToRead), " ", "\n", 1, -1, Binary_Compare);
    LangEscapeChar = IniReadKey("LangSpec", "EscapeChar", LangFileToRead);
    LangTerminatorChar = IniReadKey("LangSpec", "TerminatorChar", LangFileToRead);
    LangStyle = IniReadKey("LangSpec", "Style", LangFileToRead).Get_Long();
    LangProc = IniReadKey("LangSpec", "ProcWords", LangFileToRead);
    LangEndp = IniReadKey("LangSpec", "EndpWords", LangFileToRead);
    LangGUID = IniReadKey("LangSpec", "GUIDStyle", LangFileToRead);
    LangInclude = IniReadKey("LangSpec", "IncludeStyle", LangFileToRead);
    LangIncludeName = IniReadKey("LangSpec", "IncludeName", LangFileToRead);
    LangIncludeNameFoot = IniReadKey("LangSpec", "IncludeNameFoot", LangFileToRead);
    LangDoubleSlash = IniReadKey("LangSpec", "DoubleSlash", LangFileToRead);
    LangInterlaceProc = IniReadKey("LangSpec", "InterlacedProc", LangFileToRead);
    LangOpenBlockProc = IniReadKey("LangSpec", "OpenBlockProc", LangFileToRead);
    LangCloseBlockProc = IniReadKey("LangSpec", "CloseBlockProc", LangFileToRead);
    LangOphanBlock = IniReadKey("LangSpec", "AllowOrphanBlock", LangFileToRead);
    LangHighLightNbr = IniReadKey("LangSpec", "HighlightNbrs", LangFileToRead);
    LangHexType = IniReadKey("LangSpec", "HexType", LangFileToRead);
    LangBinType = IniReadKey("LangSpec", "BinType", LangFileToRead);
    
    LangFileName = LangFileToRead;
    LangColorA = "";
    LangHelpA = IniReadKey("LangSpec", "ColorAHelp", LangFileToRead);
    for(j = 0; j <= 999; j++)
    {
        LangRetVal = IniReadKey("LangSpec", "ColorA" + (CStr) StringNumberComplement(j, 3).Get_String(), LangFileToRead);
        LangLen = LangRetVal.Len();
        if(LangLen == 0) break;
        SpaceToNewLine(LangRetVal.Get_String(),LangLen);
        if(LangColorA.Len() != 0) LangColorA = LangColorA + (CStr) "\n";
        LangColorA = LangColorA + (CStr) LangRetVal;
    }
    LangColorB = "";
    LangHelpB = IniReadKey("LangSpec", "ColorBHelp", LangFileToRead);
    for(j = 0; j <= 999; j++)
    {
        LangRetVal = IniReadKey("LangSpec", "ColorB" + (CStr) StringNumberComplement(j, 3).Get_String(), LangFileToRead);
        LangLen = LangRetVal.Len();
        if(LangLen == 0) break;
        SpaceToNewLine(LangRetVal.Get_String(),LangLen);
        if(LangColorB.Len() != 0) LangColorB = LangColorB + (CStr) "\n";
        LangColorB = LangColorB + (CStr) LangRetVal;
    }
    LangColorC = "";
    LangHelpC = IniReadKey("LangSpec", "ColorCHelp", LangFileToRead);
    for(j = 0; j <= 999; j++)
    {
        LangRetVal = IniReadKey("LangSpec", "ColorC" + (CStr) StringNumberComplement(j, 3).Get_String(), LangFileToRead);
        LangLen = LangRetVal.Len();
        if(LangLen == 0) break;
        SpaceToNewLine(LangRetVal.Get_String(),LangLen);
        if(LangColorC.Len() != 0) LangColorC = LangColorC + (CStr) "\n";
        LangColorC = LangColorC + (CStr) LangRetVal;
    }
    LangColorD = "";
    LangHelpD = IniReadKey("LangSpec", "ColorDHelp", LangFileToRead);
    for(j = 0; j <= 999; j++)
    {
        LangRetVal = IniReadKey("LangSpec", "ColorD" + (CStr) StringNumberComplement(j, 3).Get_String(), LangFileToRead);
        LangLen = LangRetVal.Len();
        if(LangLen == 0) break;
        SpaceToNewLine(LangRetVal.Get_String(),LangLen);
        if(LangColorD.Len() != 0) LangColorD = LangColorD + (CStr) "\n";
        LangColorD = LangColorD + (CStr) LangRetVal;
    }
    LangColorE = "";
    LangHelpE = IniReadKey("LangSpec", "ColorEHelp", LangFileToRead);
    for(j = 0; j <= 999; j++)
    {
        LangRetVal = IniReadKey("LangSpec", "ColorE" + (CStr) StringNumberComplement(j, 3).Get_String(), LangFileToRead);
        LangLen = LangRetVal.Len();
        if(LangLen == 0) break;
        SpaceToNewLine(LangRetVal.Get_String(),LangLen);
        if(LangColorE.Len() != 0) LangColorE = LangColorE + (CStr) "\n";
        LangColorE = LangColorE + (CStr) LangRetVal;
    }
    LangColorF = "";
    LangHelpF = IniReadKey("LangSpec", "ColorFHelp", LangFileToRead);
    for(j = 0; j <= 999; j++)
    {
        LangRetVal = IniReadKey("LangSpec", "ColorF" + (CStr) StringNumberComplement(j, 3).Get_String(), LangFileToRead);
        LangLen = LangRetVal.Len();
        if(LangLen == 0) break;
        SpaceToNewLine(LangRetVal.Get_String(),LangLen);
        if(LangColorF.Len() != 0) LangColorF = LangColorF + (CStr) "\n";
        LangColorF = LangColorF + (CStr) LangRetVal;
    }
    LangColorG = "";
    LangHelpG = IniReadKey("LangSpec", "ColorGHelp", LangFileToRead);
    for(j = 0; j <= 999; j++)
    {
        LangRetVal = IniReadKey("LangSpec", "ColorG" + (CStr) StringNumberComplement(j, 3).Get_String(), LangFileToRead);
        LangLen = LangRetVal.Len();
        if(LangLen == 0) break;
        SpaceToNewLine(LangRetVal.Get_String(),LangLen);
        if(LangColorG.Len() != 0) LangColorG = LangColorG + (CStr) "\n";
        LangColorG = LangColorG + (CStr) LangRetVal;
    }
    LangColorH = "";
    LangHelpH = IniReadKey("LangSpec", "ColorHHelp", LangFileToRead);
    for(j = 0; j <= 999; j++)
    {
        LangRetVal = IniReadKey("LangSpec", "ColorH" + (CStr) StringNumberComplement(j, 3).Get_String(), LangFileToRead);
        LangLen = LangRetVal.Len();
        if(LangLen == 0) break;
        SpaceToNewLine(LangRetVal.Get_String(),LangLen);
        if(LangColorH.Len() != 0) LangColorH = LangColorH + (CStr) "\n";
        LangColorH = LangColorH + (CStr) LangRetVal;
    }
}

// -----------------------------------------------------------------------
// Register a language into codemax
void RegisterCodeMaxLanguage(int Update_Idx)
{
    CM_LANGUAGE CurLanguageToRegister;
    CM_LANGUAGE *NewCurLanguageToRegister = NULL;
    
    memset(&CurLanguageToRegister, 0, sizeof(CurLanguageToRegister));
    CurLanguageToRegister.pszKeywords = _strdup(LangColorA.Get_String());
    CurLanguageToRegister.pszTagAttributeNames = _strdup(LangColorB.Get_String());
    CurLanguageToRegister.pszTagElementNames = _strdup(LangColorC.Get_String());
    CurLanguageToRegister.pszTagEntities = _strdup(LangColorD.Get_String());
    CurLanguageToRegister.pszOperators = _strdup(LangColorE.Get_String());
    // New colors
    CurLanguageToRegister.pszExtra1 = _strdup(LangColorF.Get_String());
    CurLanguageToRegister.pszExtra2 = _strdup(LangColorG.Get_String());
    CurLanguageToRegister.pszExtra3 = _strdup(LangColorH.Get_String());
    CurLanguageToRegister.dwHighLightNbrs = LangHighLightNbr.Get_Long();
    CurLanguageToRegister.pszHexType = _strdup(LangHexType.Get_String());
    CurLanguageToRegister.pszBinType = _strdup(LangBinType.Get_String());
    CurLanguageToRegister.pszSingleLineComments = _strdup(LangSingleComment.Get_String());
    CurLanguageToRegister.pszStringDelims = _strdup(LangStringsDelimiters.Get_String());
    CurLanguageToRegister.bIsCaseSensitive = LangCaseSensitive;
    CurLanguageToRegister.pszScopeKeywords1 = _strdup(LangScopeKeyWord1.Get_String());
    CurLanguageToRegister.pszScopeKeywords2 = _strdup(LangScopeKeyWord2.Get_String());
    CurLanguageToRegister.pszMultiLineComments1 = _strdup(LangMultiLineComment1.Get_String());
    CurLanguageToRegister.pszMultiLineComments2 = _strdup(LangMultiLineComment2.Get_String());
    if(LangEscapeChar.Len() != 0) CurLanguageToRegister.chEscape = LangEscapeChar.Get_String()[0];
    else CurLanguageToRegister.chEscape = 0;
    if(LangTerminatorChar.Len() != 0) CurLanguageToRegister.chTerminator = LangTerminatorChar.Get_String()[0];
    else CurLanguageToRegister.chTerminator = 0;
    if(LangStyle == 0) CurLanguageToRegister.dwStyle = CMLS_PROCEDURAL;
    else CurLanguageToRegister.dwStyle = CMLS_SGML;
    // Custom extensions
    CurLanguageToRegister.pszLanguageName = _strdup(LangName.Get_String());
    CurLanguageToRegister.pszHelpFileA = _strdup(LangHelpA.Get_String());
    CurLanguageToRegister.pszHelpFileB = _strdup(LangHelpB.Get_String());
    CurLanguageToRegister.pszHelpFileC = _strdup(LangHelpC.Get_String());
    CurLanguageToRegister.pszHelpFileD = _strdup(LangHelpD.Get_String());
    CurLanguageToRegister.pszHelpFileE = _strdup(LangHelpE.Get_String());
    CurLanguageToRegister.pszHelpFileF = _strdup(LangHelpF.Get_String());
    CurLanguageToRegister.pszHelpFileG = _strdup(LangHelpG.Get_String());
    CurLanguageToRegister.pszHelpFileH = _strdup(LangHelpH.Get_String());
    CurLanguageToRegister.pszExtensions = _strdup(LangExtensions.Get_String());
    CurLanguageToRegister.pszProcEntry = _strdup(LangProc.Get_String());
    CurLanguageToRegister.pszEndpEntry = _strdup(LangEndp.Get_String());
    CurLanguageToRegister.pszGUIDEntry = _strdup(LangGUID.Get_String());
    CurLanguageToRegister.pszIncludeEntry = _strdup(LangInclude.Get_String());
    CurLanguageToRegister.pszIncludeName = _strdup(LangIncludeName.Get_String());
    CurLanguageToRegister.pszIncludeNameFoot = _strdup(LangIncludeNameFoot.Get_String());
    CurLanguageToRegister.pszFileName = _strdup(LangFileName.Get_String());
    if(LangDoubleSlash.Len() != 0)
    {
        if(_strcmpi(LangDoubleSlash.Get_String(), "1") == 0)
        {
            CurLanguageToRegister.pszDoubleSlash = 1;
        }
        else
        {
            CurLanguageToRegister.pszDoubleSlash = 0;
        }
    }
    if(LangInterlaceProc.Len() != 0)
    {
        if(_strcmpi(LangInterlaceProc.Get_String(), "1") == 0)
        {
            CurLanguageToRegister.pszInterlaceProc = 1;
        }
        else
        {
            CurLanguageToRegister.pszInterlaceProc = 0;
        }
    }
    if(LangOphanBlock.Len() != 0)
    {
        if(_strcmpi(LangOphanBlock.Get_String(), "1") == 0)
        {
            CurLanguageToRegister.pszOrphanBlock = 1;
        }
        else
        {
            CurLanguageToRegister.pszOrphanBlock = 0;
        }
    }
    CurLanguageToRegister.pszOpenBlockProc = _strdup(LangOpenBlockProc.Get_String());
    CurLanguageToRegister.pszCloseBlockProc = _strdup(LangCloseBlockProc.Get_String());

    NewCurLanguageToRegister = (CM_LANGUAGE *) malloc(sizeof(CurLanguageToRegister));
    RtlCopyMemory((void *) NewCurLanguageToRegister, &CurLanguageToRegister, sizeof(CurLanguageToRegister));
    if(Update_Idx != -1)
    {
        LanguageToRegister.Set(Update_Idx, NewCurLanguageToRegister);
    }
    else
    {
        LanguageToRegister.Add(NewCurLanguageToRegister);
    }
    hCodeMaxCMRegisterLanguage(LangName.Get_String(), NewCurLanguageToRegister);
}

// -----------------------------------------------------------------------
// Retrieve a language entry as a CM_LANGUAGE structure
CM_LANGUAGE *GetLangDat(long LangIndex)
{
    return(LanguageToRegister.Get(LangIndex)->Content);
}

// -----------------------------------------------------------------------
// Retrieve the name of the language to open according to the filename
CStr GetLanguageToOpen(CStr LFileName)
{
    CStr ReturnValue;
    int o = 0;
    int p = 0;
    CStr ExtToSearch;
    long *ExtensionsArray = 0;
    CStr BufString;
    
    ExtToSearch = FileGetExtension(LFileName);
    if(ExtToSearch.Len() != 0)
    {
        for(o = 0; o < LanguageToRegister.Amount(); o++)
        {
            ExtensionsArray = StringSplit((char *) GetLangDat(o)->pszExtensions, " ");
            for(p = 0; p <= (long) StringGetSplitUBound(ExtensionsArray); p++)
            {
                if(_strcmpi(StringGetSplitElement((char *) GetLangDat(o)->pszExtensions, ExtensionsArray, p).Get_String(),
                                                  ExtToSearch.Get_String()) == 0)
                {
                    BufString = (char *) GetLangDat(o)->pszLanguageName;
                    StringReleaseSplit(ExtensionsArray);
                    goto LangOut;
                }
            }
            StringReleaseSplit(ExtensionsArray);
        }
    }
LangOut:
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the real offset of a language from the languages array
long GetLanguageOffset(CStr LanguageName)
{
    long ReturnValue = -1;
    int i = 0;

    if(LanguageName.Len() != 0)
    {
        for(i = 0; i < LanguageToRegister.Amount(); i++)
        {
            if(strlen(GetLangDat(i)->pszLanguageName) != 0)
            {
                if(_strcmpi(GetLangDat(i)->pszLanguageName, LanguageName.Get_String()) == 0)
                {
                    ReturnValue = i;
                    break;
                }
            }
        }
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the C (registers) datas associated with child's language
CStr GetCMLangC(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    BufString = (char *) GetLangDat(ReturnOffset)->pszTagElementNames;
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the GUID creation style associated with child's language
CStr GetCMLangGUID(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;
    
    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    BufString = (char *) GetLangDat(ReturnOffset)->pszGUIDEntry;
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the Include creation style associated with child's language
CStr GetCMLangInclude(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    BufString = (char *) GetLangDat(ReturnOffset)->pszIncludeEntry;
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the slash doubling flag associated with child's language
CStr GetCMLangDoubleSlash(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    BufString = GetLangDat(ReturnOffset)->pszDoubleSlash;
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the single line comment char(s) associated with child's language
CStr GetCMLangComment(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    BufString = (char *) GetLangDat(ReturnOffset)->pszSingleLineComments;
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the type of hexadecimal number associated with child's language
CStr GetCMLangHexType(HWND hwnd, int Amount)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr BufString;
    CStr Dest;
    int i;
    int j;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return("");
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return("");
    BufString = (char *) GetLangDat(ReturnOffset)->pszHexType;
    if(Amount != -1)
    {
        Dest = "";
        i = 0;
        for(j = 0; j < Amount; j++)
        {
            while(BufString[i] != ' ' && BufString[i])
            {
                Dest += BufString[i];
                i++;
            }
        }
    }
    else
    {
        Dest = BufString;
    }
    return(Dest);
}

// -----------------------------------------------------------------------
// Retrieve the type of hexadecimal number associated with child's language
CStr GetCMLangBinType(HWND hwnd, int Amount)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr BufString;
    CStr Dest;
    int i;
    int j;
    long ReturnOffset = 0;

    BufString = "";
    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return("");
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return("");
    BufString = (char *) GetLangDat(ReturnOffset)->pszBinType;
    if(Amount != -1)
    {
        Dest = "";
        i = 0;
        for(j = 0; j < Amount; j++)
        {
            while(BufString[i] != ' ' && BufString[i])
            {
                Dest += BufString[i];
                i++;
            }
        }
    }
    else
    {
        Dest = BufString;
    }
    return(Dest);
}

// -----------------------------------------------------------------------
// Retrieve the Multi line comment char(s) associated with child's language
// Always the first
CStr GetCMLangMultiComment(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;
    long *MultiArray = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    MultiArray = StringSplit((char *) GetLangDat(ReturnOffset)->pszMultiLineComments1, "\n");
    if(StringGetSplitUBound(MultiArray) != -1) BufString = StringGetSplitElement((char *) GetLangDat(ReturnOffset)->pszMultiLineComments1, MultiArray, 0).Trim().Get_String() + (CStr) " ";
    StringReleaseSplit(MultiArray);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the Multi line comment char(s) associated with child's language
// Always the first
CStr GetCMLangMultiCommentByChild(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;
    long *MultiArray = 0;

    // Get the language name
    CM_GetLanguage(hwnd, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);

    MultiArray = StringSplit((char *) GetLangDat(ReturnOffset)->pszMultiLineComments1, "\n");
    if(StringGetSplitUBound(MultiArray) != -1) BufString = StringGetSplitElement((char *) GetLangDat(ReturnOffset)->pszMultiLineComments1, MultiArray, 0).Trim().Get_String() + (CStr) " ";
    StringReleaseSplit(MultiArray);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the Multi line comment char(s) associated with child's language
// Always the first
CStr GetCMLangMultiCommentByFile(CStr FileName)
{
    long *MultiArray = 0;
    CStr ReturnValue;
    long ReturnOffset = 0;
    CStr MoLanguageName;
    CStr BufString;

    MoLanguageName = GetLanguageToOpen(FileName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    MultiArray = StringSplit((char *) GetLangDat(ReturnOffset)->pszMultiLineComments1, "\n");
    if(StringGetSplitUBound(MultiArray) != -1) BufString = StringGetSplitElement((char *) GetLangDat(ReturnOffset)->pszMultiLineComments1, MultiArray, 0).Trim().Get_String() + (CStr) " ";
    StringReleaseSplit(MultiArray);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the Multi line comment endchar(s) associated with child's language
// (Always the first in the list)
CStr GetCMLangMultiCommentEnd(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;
    long *MultiArray = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);

    MultiArray = StringSplit((char *) GetLangDat(ReturnOffset)->pszMultiLineComments2, "\n");
    if(StringGetSplitUBound(MultiArray) != -1) BufString = StringGetSplitElement((char *) GetLangDat(ReturnOffset)->pszMultiLineComments2, MultiArray, 0).Trim().Get_String();
    StringReleaseSplit(MultiArray);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the Multi line comment endchar(s) associated with child's language
// (Always the first in the list)
CStr GetCMLangMultiCommentEndByChild(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;
    long *MultiArray = 0;

    // Get the language name
    CM_GetLanguage(hwnd, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);

    MultiArray = StringSplit((char *) GetLangDat(ReturnOffset)->pszMultiLineComments2, "\n");
    if(StringGetSplitUBound(MultiArray) != -1) BufString = StringGetSplitElement((char *) GetLangDat(ReturnOffset)->pszMultiLineComments2, MultiArray, 0).Trim().Get_String();
    StringReleaseSplit(MultiArray);
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the Multi line comment char(s) associated with child's language
// Always the first
CStr GetCMLangMultiCommentEndByFile(CStr FileName)
{
    long *MultiArray = 0;
    CStr ReturnValue;
    CStr BufString;
    long ReturnOffset = 0;
    CStr MoLanguageName;

    MoLanguageName = GetLanguageToOpen(FileName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    MultiArray = StringSplit((char *) GetLangDat(ReturnOffset)->pszMultiLineComments2, "\n");
    if(StringGetSplitUBound(MultiArray) != -1) BufString = StringGetSplitElement((char *) GetLangDat(ReturnOffset)->pszMultiLineComments2, MultiArray, 0).Trim().Get_String() + (CStr) " ";
    StringReleaseSplit(MultiArray);
    ReturnValue = BufString.Trim();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the procedure entry points associated with child's language
CStr GetCMLangProc(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszProcEntry;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the language associated with a codemax window
CStr GetCMLangNameByChild(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    
    CM_GetLanguage(hwnd, (char *) &ReturnLang);
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnValue = ReturnRealLang;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the language associated with child's language
CStr GetCMChildLangName(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnValue;
    CStr ReturnRealLang;

    ChildStruct = LoadStructure(hwnd);
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnValue = ReturnRealLang;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the procedure entry points associated with child's language
CStr GetCMLangEndp(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszEndpEntry;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the procedures interlace flag associated with a child's language
long GetCMLangProcInterlace(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(0);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(0);
    return(GetLangDat(ReturnOffset)->pszInterlaceProc);
}

// -----------------------------------------------------------------------
// Retrieve the procedures orphans flag associated with a child's language
long GetCMLangProcOrphan(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(0);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(0);
    return(GetLangDat(ReturnOffset)->pszOrphanBlock);
}

// -----------------------------------------------------------------------
// Retrieve the procedures open block associated with child's language
CStr GetCMLangProcOpenBlock(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    long ReturnOffset = 0;
    
    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszOpenBlockProc;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the procedures close block associated with child's language --- '
CStr GetCMLangProcCloseBlock(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszCloseBlockProc;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the filename associated with a child's language
CStr GetCMLangFileName(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnRealLang;
    CStr ReturnValue;
    long ReturnOffset = 0;

    ChildStruct = LoadStructure(hwnd);
    // Get the language name
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    // Format name
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszFileName;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the procedure entry points associated with filename
CStr GetCMLangProcByFile(CStr FPName)
{
    long ReturnOffset = 0;
    CStr ReturnValue;
    CStr MoLanguageName;

    MoLanguageName = GetLanguageToOpen(FPName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszProcEntry;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the procedure end points associated with filename
CStr GetCMLangProcCloseByFile(CStr FPName)
{
    long ReturnOffset = 0;
    CStr ReturnValue;
    CStr MoLanguageName;

    MoLanguageName = GetLanguageToOpen(FPName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszEndpEntry;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the include search name associated with filename
CStr GetCMLangIncludeNameByFile(CStr FPName)
{
    long ReturnOffset = 0;
    CStr ReturnValue;
    CStr MoLanguageName;
    
    MoLanguageName = GetLanguageToOpen(FPName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszIncludeName;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the language name associated with filename
CStr GetCMLangLangNameByFile(CStr FPName)
{
    long ReturnOffset = 0;
    CStr ReturnValue;
    CStr MoLanguageName;

    MoLanguageName = GetLanguageToOpen(FPName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszLanguageName;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the include search footer name associated with filename
CStr GetCMLangIncludeNameFootByFile(CStr FPName)
{
    long ReturnOffset = 0;
    CStr ReturnValue;
    CStr MoLanguageName;

    MoLanguageName = GetLanguageToOpen(FPName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszIncludeNameFoot;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the slash doubling flag associated with filename
CStr GetCMLangDoubleslashByFile(CStr FPName)
{
    long ReturnOffset = 0;
    CStr ReturnValue;
    CStr MoLanguageName;
    CStr BufString;
    
    MoLanguageName = GetLanguageToOpen(FPName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    BufString = GetLangDat(ReturnOffset)->pszDoubleSlash;
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the include search name associated with child's language
CStr GetCMLangIncludeNameByhWnd(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnValue;
    CStr ReturnRealLang;
    long ReturnOffset = 0;
    
    ChildStruct = LoadStructure(hwnd);
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszIncludeName;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the include search name associated with child's language
CStr GetCMLangIncludeNameFootByhWnd(HWND hwnd)
{
    char ReturnLang[CM_MAX_LANGUAGE_NAME + 1];
    CStr ReturnValue;
    CStr ReturnRealLang;
    long ReturnOffset = 0;
    
    ChildStruct = LoadStructure(hwnd);
    CM_GetLanguage(ChildStruct->hChildCodeMax, (char *) &ReturnLang);
    ReturnRealLang = CMGetRealLanguage((char *) &ReturnLang);
    if(ReturnRealLang.Len() == 0) return(ReturnValue);
    ReturnOffset = GetLanguageOffset(ReturnRealLang);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszIncludeNameFoot;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve the single line comment char(s) associated with filename
CStr GetCMLangCommentByFile(CStr FPName)
{
    long ReturnOffset = 0;
    CStr ReturnValue;
    CStr MoLanguageName;

    MoLanguageName = GetLanguageToOpen(FPName);
    ReturnOffset = GetLanguageOffset(MoLanguageName);
    if(ReturnOffset == -1) return(ReturnValue);
    ReturnValue = (char *) GetLangDat(ReturnOffset)->pszSingleLineComments;
    return(ReturnValue);
}
