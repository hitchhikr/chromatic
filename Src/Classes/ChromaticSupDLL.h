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
// ChromaticSupDLL.h: declaration of function for ChromaticSup.dll
// -----------------------------------------------------------------------

#ifndef _CHROMATICSUPDLL_H_
#define _CHROMATICSUPDLL_H_

// -----------------------------------------------------------------------
// Functions declarations
extern "C"
{
//void CALLBACK LoCase(char *StringToConvert);
//void CALLBACK UpCase(char *StringToConvert);
//long CALLBACK CountFileLines(char *StringToCheck, long MaxSize);
//long CALLBACK GetFileLineAddress(char *StringToCheck, long MaxSize, long LineNumber);
//long CALLBACK GetFileLineOffset(char *StringToCheck, long MaxSize, long LineNumber);
char * CALLBACK GetApiDescription(char *DbFileName, char *FunctionName);
//extern "C" long CALLBACK GetStrLen(char *StrToCheck);
//long CALLBACK MLoadFile(char *FileToLoad, long *ByteReaded);
long CALLBACK MSaveFile(char *FileToSave, long MemorytoSave, long BytestoSave);
//long CALLBACK QEPlug(long DllAddr, HINSTANCE hInst, HWND hW, HWND hText, HWND hTB, HWND hSB);
//void CALLBACK MFreeFile(long FileAddr);
long CALLBACK MGetStrLenCR(char *StrToCheck, long SizeOfString);
//char * CALLBACK LongToString(long StrToCheck);
long CALLBACK Str_InString(long StartPos, char *StrToSearch, char *SuCStr);
char * CALLBACK GetCurrentLine(char *StrToGo, long SizeOfString, long PosIn_String);
void CALLBACK RemoveCommentBlocks(char *StrToStrip, long SizeOfString);
//long CALLBACK DisplayChm(char *File, char *KeyWord);
void CALLBACK RemoveQuotes(char *Txt, long TxtLen);
void CALLBACK RemoveSingleLineComment(char *Txt, long TxtLen, char *CommentChar);
//void CALLBACK ReplaceTabs(char *Txt, long TxtLen);
void CALLBACK RemoveSpaces(char *Txt, char *DestText, long TxtLen);
void CALLBACK RemoveCommChar(char *Txt, char *DestText, long TxtLen);
void CALLBACK MarkSpaces(char *Txt, char *DestText, long TxtLen);
void CALLBACK FillHexBytes(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
void CALLBACK FillHexWords(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
void CALLBACK FillHexDWords(long Source, char *Dest, long Length, long Offset, long LineOffset, long NbrLines, long ElementsOffset);
//long CALLBACK AsciiToDw(char *DecString);
void CALLBACK RetrieveIncludes(char *Txt, long TxtLen, char *CommentChar);
long CALLBACK RetrieveKeyWordLine(char *Txt, char *KeyWord, long TxtLen, char *CommentChar);
long CALLBACK RetrieveKeyWordFromInc(char *PrimaryString, char *KeyWord, long TxtLen, char *CommentChar);
//void CALLBACK ToggleCase(char *StringToConvert, long Length);
//void CALLBACK SpaceToTab(char *StringToConvert, long Length);
//void CALLBACK CopyMem(void *MemSource, void *MemDest, long Length, long MemLeftOffset, long RepeatCount);
//void CALLBACK CopyMem2(void *MemSource, void *MemDest, long Length, long SourceOffset, long DestOffset);
void CALLBACK RemoveCommentBlocksEx(char *StrToStrip, long SizeOfString, char *OpeningComment, char *ClosingComment);
long CALLBACK WAAsmStringSplit(char *StrToSplit, char *StrSplitter);
long CALLBACK GetLinesOffsets(char *TextToGet);
long CALLBACK FillAPIListBox(HWND hListBox, long DbFile);
char * CALLBACK GetAPIKeywordToCorrect(long DbFile, char *Keyword);
long CALLBACK HexaToDw(char *HexString);
//void CALLBACK SpaceToNewLine(char *StringToConvert, long Length);
}

#endif
