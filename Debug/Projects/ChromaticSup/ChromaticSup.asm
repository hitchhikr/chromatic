; -----------------------------------------------------------------------
; Chromatic
; Integrated Development Environment
;
; Copyright (C) 2001-2010 Franck Charlet.
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions
; are met:
;
;  1. Redistributions of source code must retain the above copyright notice,
;     this list of conditions and the following disclaimer.
;
;  2. Redistributions in binary form must reproduce the above copyright notice,
;     this list of conditions and the following disclaimer in the documentation
;     and/or other materials provided with the distribution.
;
; THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
; ARE DISCLAIMED. IN NO EVENT SHALL FRANCK CHARLET OR CONTRIBUTORS BE LIABLE
; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
; OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
; HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
; LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
; OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
; SUCH DAMAGE.
; -----------------------------------------------------------------------
; Notes:        DLL to provide APIs completion to Chromatic.
;               and some other strings manipulations.
;
;               This one requires MASM32 to be built.
;               i made that to obtain some extra speed
;               when i was using an old p133 and when
;               it was still done in Visual Basic.
;               It should be remade in Cpp (and integrated into the app).
; ------------------------------------------

; buildblock RELEASE
; 	CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
; 	CAPT [BINDIR]\Link.exe /LIBPATH:"[LIBDIR]" /DEF:ChromaticSup.def /DLL /OUT:"..\..\Debug\ChromaticSup.dll" /SUBSYSTEM:WINDOWS "%1.obj"
; buildblockend
                        .code

; --------------- Code section
; external
LoCase                  proto   :dword
UpCase                  proto   :dword
GetFileLineAddress      proto   :dword, :dword, :dword
ReplaceTabs             proto   :dword, :dword
AllocMem                proto   :dword
FreeMem                 proto   :dword
CountFileLines          proto   :dword, :dword
GetFileLineOffset       proto   :dword, :dword, :dword
RemCRLF                 proto   :dword, :dword
FillMemDat              proto   :dword, :dword, :dword

; To be converted to c:
SplitArray              proto   :dword,:dword
CountOccurence          proto   :dword,:dword
Str_Occurence           proto   :dword,:dword
Str_GetSplit            proto   :dword,:dword
Str_Blank               proto   :dword,:dword
Str_Cmp                 proto   :dword,:dword                   
GetElement              proto   :dword,:dword
ClearMem                proto   :dword,:dword
FillMem                 proto   :dword,:dword
StripStringInc          proto   :dword,:dword,:byte,:byte
StripStringInc2         proto   :dword,:dword,:byte
StripStringEx           proto   :dword,:dword,:byte,:byte
vbinstr                 proto   :dword,:dword,:dword
Str_InString            proto   :dword,:dword,:dword

                        .code

; --------------- DLL's entry point
LibMain                 proc    hInstance:HINSTANCE,reason:dword,reserved1:dword
                        .if reason == DLL_PROCESS_ATTACH
                        .elseif reason == DLL_PROCESS_DETACH
                        .elseif reason == DLL_THREAD_ATTACH
                        .elseif reason == DLL_THREAD_DETACH
                        .endif
                        mov     eax,TRUE
                        ret
LibMain                 endp

; --------------- DLL's Functions

; --------------- Retrieve description associated with a function name
; (return memory handle containing description string or 0)
GetApiDescription       proc    FileName:dword,FncName:dword
                        local   UpFncName:dword
                        local   PosInBlock:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        push    edx
                        xor     eax,eax
                        mov     hDescriptionMem,eax
                        mov     eax,FileName
                        test    eax,eax
                        jz      EmptyArg
                        ; Make a local copy of the function name
                        invoke  lstrlen,FncName
                        inc     eax
                        push    eax
                        inc     eax                     ; allocated 2 bytes minimum
                        invoke  AllocMem,eax
                        mov     UpFncName,eax
                        test    eax,eax
                        jz      EmptyArg
                        pop     eax
                        ; at least one char
                        invoke  lstrcpyn,UpFncName,FncName,eax
                        invoke  CreateFile,FileName,GENERIC_READ,0,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0
                        mov     hApiFile,eax
                        cmp     eax,INVALID_HANDLE_VALUE
                        je      ReturnFnc
                        invoke  UpCase,UpFncName
                        mov     esi,UpFncName
                        mov     edi,offset FncTwoChars
                        cld
                        movsw                           ; Get first 2 chars
                        mov     esi,offset FncTwoChars
                        xor     eax,eax
                        xor     ebx,ebx
                        lodsb                           ; Hi
                        cmp     al,"A"
                        jb      SeekError
                        cmp     al,"_"
                        ja      SeekError
                        sub     al,"A"
                        xor     edx,edx
                        mov     bx,(31*8)
                        mul     bx                      ; Calc upper part
                        push    ax
                        xor     eax,eax
                        lodsb                           ; Lo
                        xor     ebx,ebx
                        pop     bx                      ; Safety pop before compares
                        cmp     al,"A"
                        jb      SeekError
                        cmp     al,"_"
                        ja      SeekError
                        sub     al,"A"
                        shl     eax,3                   ; *8
                        add     ebx,eax
EmptyLoChar:            ; Move to correct offset position
                        invoke  SetFilePointer,hApiFile,ebx,0,FILE_BEGIN
                        cmp     eax,-1
                        je      SeekError
                        ; Retrieve index and length
                        invoke  ReadFile,hApiFile,addr FilePosition,8,addr FileRW,0
                        ; Move to correct group position
                        invoke  SetFilePointer,hApiFile,FilePosition,0,FILE_BEGIN
                        cmp     eax,-1
                        je      SeekError
                        mov     eax,FileLength
                        inc     eax                     ; Ensure that the string is 0 terminated
                        invoke  AllocMem,eax
                        mov     hDescriptionMem,eax
                        test    eax,eax
                        jz      SeekError
                        ; Read description content into memory
                        invoke  ReadFile,hApiFile,hDescriptionMem,FileLength,addr FileRW,0
                        ; Search the function name in the retrieved block
                        ; (VB is too slow to do this)
                        mov     eax,FileLength
                        inc     eax
                        invoke  AllocMem,eax
                        mov     hDescriptionMem2,eax
                        ; Copy block into back buffer
                        invoke  RtlMoveMemory,hDescriptionMem2,hDescriptionMem,FileLength
                        ; Search the function in the block (non case sensitive)
                        ; The base is expected to have correct cases
                        invoke  UpCase,hDescriptionMem2
                        invoke  Str_InString,1,hDescriptionMem2,UpFncName
                        mov     PosInBlock,eax
                        test    eax,eax
                        jnz     NoFunctionFound
                        invoke  FreeMem,hDescriptionMem2
                        invoke  FreeMem,hDescriptionMem
                        mov     hDescriptionMem2,0
                        mov     hDescriptionMem,0
                        jmp     SeekError                       
NoFunctionFound:        dec     PosInBlock              ; Zero offset
                        ; Fill backbuffer to the end with zeros
                        invoke  RtlZeroMemory,hDescriptionMem2,FileLength
                        ; Copy from found position to end of buffer
                        ; (we don't know the length of the API line)
                        mov     ebx,FileLength
                        sub     ebx,PosInBlock          ; (Shouldn't be larger than the max len)
                        mov     eax,hDescriptionMem
                        add     eax,PosInBlock
                        invoke  RtlMoveMemory,hDescriptionMem2,eax,ebx
                        ; Free main buffer
                        invoke  FreeMem,hDescriptionMem
                        ; Swap buffers
                        mov     eax,hDescriptionMem2
                        mov     hDescriptionMem,eax
SeekError:              invoke  CloseHandle,hApiFile
ReturnFnc:              invoke  FreeMem,UpFncName
EmptyArg:               mov     eax,hDescriptionMem
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
GetApiDescription       endp

; --------------- Retrieve the address of a line element
GetElement              proc    SplitBlock:dword,Element:dword
                        push    ebx
                        mov     eax,SplitBlock          ; Line's offsets
                        test    eax,eax
                        jz      NoElement
                        add     eax,4                   ; Pass header
                        mov     ebx,Element             ; Element number
                        shl     ebx,2                   ; *4
                        add     eax,ebx                 ; Add number
                        mov     eax,[eax]               ; Get offset
                        dec     eax                     ; -1 (1 based)
                        add     eax,PosInGroup          ; Add address
NoElement:              pop     ebx
                        ret
GetElement              endp

; --------------- Search for the first occurence of a substring in a string
; Return: Position of occurence or 0
Str_InString            proc    StartPos:dword,String:dword,SubString:dword
                        local   Firstchar:byte
                        local   SubStringLen:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        mov     eax,String
                        test    eax,eax
                        jz      SearchStrExit
                        mov     ebx,StartPos
                        test    ebx,ebx
                        jnz     StartPosCorrect
                        xor     eax,eax
                        jmp     SearchStrExit
StartPosCorrect:        invoke  lstrlen,SubString
                        mov     SubStringLen,eax        ; No Null strings allowed
                        test    eax,eax
                        jz      SearchStrExit
                        mov     esi,SubString
                        mov     bl,[esi]
                        inc     esi                     ; Get first substring char in bl
                        mov     esi,String
                        add     esi,StartPos
                        dec     esi                     ; Startpos is 1 based
SeekFirstChar:          mov     al,[esi]
                        inc     esi                     ; Load char
                        test    al,al
                        jz      StringEnd
                        cmp     al,bl
                        je      FoundfirstString
                        jmp     SeekFirstChar
StringEnd:              xor     eax,eax
                        jmp     SearchStrExit
FoundfirstString:       mov     ecx,SubStringLen
                        push    esi
                        dec     esi
                        cld
                        mov     edi,SubString
                        repe    cmpsb
                        pop     esi
                        jnz     SeekFirstChar           ; Last char was not equal...
                        jecxz   SeekDone                ; Reached end of string ?
                        jmp     SeekFirstChar
SeekDone:               sub     esi,String              ; Calculate relative position
                        mov     eax,esi
SearchStrExit:          pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
Str_InString            endp

; --------------- Search the number of occurences of a substring in a string
; Return: Number of occurences
Str_Occurence           proc    String:dword,SubString:dword
                        local   StartP:dword
                        local   SubStringLen:dword
                        mov     eax,String
                        test    eax,eax
                        jz      NoOccPossible
                        push    ebx
                        invoke  lstrlen,SubString
                        xor     ebx,ebx
                        test    eax,eax
                        jz      NoMoreOccurence
                        mov     SubStringLen,eax
                        xor     eax,eax
                        inc     eax
                        mov     StartP,eax
CountOccurences:        push    ebx
                        invoke  Str_InString,StartP,String,SubString
                        pop     ebx
                        test    eax,eax
                        jz      NoMoreOccurence                 
                        add     eax,SubStringLen
                        mov     StartP,eax
                        inc     ebx
                        jmp     CountOccurences
NoMoreOccurence:        mov     eax,ebx
                        pop     ebx
NoOccPossible:          ret
Str_Occurence           endp

; --------------- Return position of every substring in a string
; Return: Allocated memory block of pointers array
Str_GetSplit            proc    String:dword,SubString:dword
                        local   SplitMemBlock:dword
                        local   StartP:dword
                        local   SubStringLen:dword
                        mov     eax,String
                        test    eax,eax
                        jz      NoSplitPossible
                        push    ebx
                        push    ecx
                        invoke  lstrlen,SubString
                        mov     SplitMemBlock,0
                        test    eax,eax
                        jz      EndSplit
                        mov     SubStringLen,eax
                        invoke  Str_Occurence,String,SubString
                        test    eax,eax
                        jz      EndSplit
                        inc     eax                     ; Add 1 to joining elements
                        push    eax
                        inc     eax                     ; Add room for header
                        shl     eax,2
                        invoke  AllocMem,eax            ; Alloc room for pointers
                        mov     SplitMemBlock,eax
                        pop     eax
                        mov     ecx,SplitMemBlock
                        mov     [ecx],eax               ; Store number of entries in header
                        add     ecx,4                   ; Point to first entry
                        mov     eax,1
                        mov     StartP,eax              ; First entry is always 1
                        mov     [ecx],eax
                        mov     ebx,2                   ; First offset in memblock
ContSearchOccSplit:     push    ebx
                        invoke  Str_InString,StartP,String,SubString
                        pop     ebx
                        test    eax,eax
                        jz      EndSplit
                        add     eax,SubStringLen
                        mov     StartP,eax
                        mov     eax,ebx                 ; Entry number
                        shl     eax,2                   ; *4
                        mov     ecx,SplitMemBlock       ; MemBlock
                        add     ecx,eax                 ; Right position in memblock
                        mov     eax,StartP              ; Position data to store
                        mov     [ecx],eax               ; Store position in list
                        inc     ebx                     ; Next offset
                        jmp     ContSearchOccSplit
EndSplit:               mov     eax,SplitMemBlock
                        pop     ecx
                        pop     ebx
NoSplitPossible:        ret
Str_GetSplit            endp

; --------------- Replace every occurences of a substring with 0 in a string
Str_Blank               proc    String:dword,SubString:dword
                        local   StartP:dword
                        local   SubStringLen:dword
                        mov     eax,String
                        test    eax,eax
                        jz      NoBlank
                        push    ebx
                        invoke  lstrlen,SubString
                        xor     ebx,ebx
                        test    eax,eax
                        jz      NoMoreOccurenceToblank
                        mov     SubStringLen,eax
                        mov     StartP,1
BlankOccurences:        push    ebx
                        invoke  Str_InString,StartP,String,SubString
                        pop     ebx
                        test    eax,eax
                        jz      NoMoreOccurenceToblank                  
                        push    eax
                        push    ebx
                        mov     ebx,eax
                        dec     ebx
                        mov     eax,String
                        add     eax,ebx
                        invoke  ClearMem,eax,SubStringLen
                        pop     ebx
                        pop     eax
                        add     eax,SubStringLen
                        mov     StartP,eax              
                        inc     ebx
                        jmp     BlankOccurences
NoMoreOccurenceToblank: mov     eax,ebx
                        pop     ebx
NoBlank:                ret
Str_Blank               endp

; --------------- Replace every occurences of a substring with spaces in a string
Str_Space               proc    String:dword,SubString:dword
                        local   StartP:dword
                        local   SubStringLen:dword
                        mov     eax,String
                        test    eax,eax
                        jz      NoSpacePossible
                        push    ebx
                        invoke  lstrlen,SubString
                        xor     ebx,ebx
                        test    eax,eax
                        jz      NoMoreOccurenceToSpace
                        mov     SubStringLen,eax
                        mov     StartP,1
SpaceOccurences:        push    ebx
                        invoke  Str_InString,StartP,String,SubString
                        pop     ebx
                        test    eax,eax
                        jz      NoMoreOccurenceToSpace
                        push    eax
                        push    ebx
                        mov     ebx,eax
                        dec     ebx
                        mov     eax,String
                        add     eax,ebx
                        invoke  FillMemDat,eax,SubStringLen,20h
                        pop     ebx
                        pop     eax
                        add     eax,SubStringLen
                        mov     StartP,eax              
                        inc     ebx
                        jmp     SpaceOccurences
NoMoreOccurenceToSpace: mov     eax,ebx
                        pop     ebx
NoSpacePossible:        ret
Str_Space               endp

; --------------- Clear a memory array
ClearMem                proc    Mem:dword,Bytes:dword
                        push    ecx
                        push    edi
                        mov     eax,Mem
                        test    eax,eax
                        jz      NoClearPossible
                        mov     edi,eax
                        mov     ecx,Bytes
                        cld
                        xor     eax,eax
                        shr     ecx,2
                        rep     stosd
                        mov     ecx,Bytes
                        and     ecx,3
                        rep     stosb
NoClearPossible:        pop     edi
                        pop     ecx
                        ret
ClearMem                endp

; --------------- Fill a memory array (except 1310 chars)
FillMem                 proc    Mem:dword,Bytes:dword
                        push    esi
                        push    ecx
                        push    ebx
                        mov     eax,Mem
                        test    eax,eax
                        jz      StopFill
                        mov     esi,eax
                        mov     ecx,Bytes
                        mov     bl,020h
LpFill:                 mov     al,[esi]
                        inc     esi
                        cmp     al,13
                        je      Noenter
                        cmp     al,10
                        je      Noenter
                        test    al,al           ; End of file
                        jz      StopFill
                        mov     [esi-1],bl      ; Replace by a space
Noenter:                dec     ecx
                        jnz     LpFill
StopFill:               pop     ebx
                        pop     ecx
                        pop     esi
                        ret
FillMem                 endp

; --------------- Save memory block into a file
MSaveFile               proc    FName:dword,MemPos:dword,FSize:dword
                        local   FHandle:dword
                        local   FRealRead:dword
                        local   FValid:dword
                        mov     FValid,0
                        mov     eax,FName
                        test    eax,eax
                        jz      Error_SaveFile
                        invoke  DeleteFile,FName                ; Delete it before
                        invoke  CreateFile,FName,GENERIC_WRITE,0,0,CREATE_NEW,FILE_ATTRIBUTE_ARCHIVE or FILE_FLAG_RANDOM_ACCESS,0
                        cmp     eax,INVALID_HANDLE_VALUE
                        je      Error_SaveFile
                        mov     FHandle,eax
                        invoke  WriteFile,FHandle,MemPos,FSize,addr FRealRead,0
                        invoke  CloseHandle,FHandle
                        mov     FValid,1
Error_SaveFile:         mov     eax,FValid
                        ret
MSaveFile               endp

; --------------- Compare 2 strings
Str_Cmp                 proc    String1:dword,String2:dword                     
                        push    esi
                        push    edi
                        push    ecx
                        invoke  lstrlen,String1
                        mov     ecx,eax
                        push    ecx
                        invoke  lstrlen,String2
                        pop     ecx
                        cmp     eax,ecx
                        jne     Str_NotEqual
                        mov     esi,String1
                        mov     edi,String2
                        cld
                        repe    cmpsb
                        sete    al
                        pop     ecx
                        pop     edi
                        pop     esi
                        ret
Str_NotEqual:           xor     eax,eax
                        pop     ecx
                        pop     edi
                        pop     esi
                        ret
Str_Cmp                 endp

; --------------- Compare 2 strings by a given number of chars
; Return 1 if strings are equals
Str_CmpN                proc    String1:dword,String2:dword,Chars:dword
                        push    esi
                        push    edi
                        push    ecx
                        xor     eax,eax
                        mov     ecx,Chars
                        mov     esi,String1
                        mov     edi,String2
                        cld
                        repe    cmpsb
                        sete    al
                        pop     ecx
                        pop     edi
                        pop     esi
                        ret
Str_CmpN                endp

; --------------- Retrieve a line length ending with 13,10
MGetStrLenCR            proc    String:dword,Len:dword
                        push    ecx
                        push    edx
                        mov     eax,String
                        test    eax,eax
                        jz      NoStrLenCR
                        mov     ecx,Len
                        test    ecx,ecx
                        jnz     CheckStrLenCR
                        xor     eax,eax
                        jmp     NoStrLenCR
CheckStrLenCR:          mov     dl,[eax]        ;  1
                        inc     eax             ;  1
                        test    dl,dl
                        jz      EndOfLine
                        cmp     dl,10
                        je      EndOfLine
                        cmp     dl,13           ;  1 (check for chr(13)
                        je      EndOfLine       ;  3 when jmp taken
                        dec     ecx
                        jnz     CheckStrLenCR
EndOfLine:              sub     eax,String
                        dec     eax             ;  correct count
NoStrLenCR:             pop     edx
                        pop     ecx
                        ret
MGetStrLenCR            endp

; --------------- Retrieve the current line number from a position
GetCurrentLine          proc    Text:dword,MaxSize:dword,Position:dword
                        push    esi
                        push    ebx
                        push    ecx
                        push    edx
                        xor     ebx,ebx
                        xor     eax,eax
                        mov     esi,Text
                        test    esi,esi
                        jz      NoCurLine
                        mov     edx,Position
                        test    edx,edx
                        jz      GetZero
                        add     edx,esi         ; Relative to text
                        mov     ecx,MaxSize
GetCountLines:          mov     al,[esi]
                        inc     esi
                        cmp     al,13
                        je      GetNextChar
                        cmp     al,10
                        jne     GetNextChar
                        ; edi should be on a chr(10) edi
                        cmp     esi,edx
                        jg      GetZero
                        inc     ebx
GetNextChar:            dec     ecx
                        jnz     GetCountLines
GetZero:                mov     eax,ebx
                        inc     eax
NoCurLine:              pop     edx
                        pop     ecx
                        pop     ebx
                        pop     esi
                        ret
GetCurrentLine          endp

; --------------- Remove a commented block from a text
; Text must be in lower case
RemoveCommentBlocks     proc    Text:dword,MaxSize:dword
                        local   ComLineNumber:dword
                        local   ComLineAddr:dword
                        local   ComLineFirstAddr:dword
                        local   ComChar:byte
                        local   ComEndText:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        push    edx
                        ; Calculate end address
                        mov     eax,MaxSize
                        test    eax,eax
                        jz      EndComment
                        mov     eax,Text
                        test    eax,eax
                        jz      EndComment
                        add     eax,MaxSize
                        mov     ComEndText,eax
                        mov     ComPosition,1
DeadCommentCheck:       ; Seek "comment" word globally
                        invoke  Str_InString,ComPosition,Text,addr CheckComment
                        test    eax,eax
                        jz      EndComment
                        mov     ComPosition,eax
                        ; Retrieve the line number
                        invoke  GetCurrentLine,Text,MaxSize,ComPosition
                        inc     ComPosition             ; Position=Position+1
                        dec     eax
                        mov     ComLineNumber,eax
                        ; Retrieve real line start
                        invoke  GetFileLineAddress,Text,MaxSize,ComLineNumber
                        mov     ComLineFirstAddr,eax
                        ; Strip tabs and spaces
                        mov     esi,ComLineFirstAddr
                        mov     ebx,ComEndText
StripTS:                mov     al,[esi]
                        inc     esi
                        cmp     al," "
                        je      StripTS
                        cmp     al,9
                        je      StripTS
                        mov     ComLineAddr,esi
                        dec     ComLineAddr     ; Begin at C not O
                        cmp     al,"c"
                        je      CmChar2
                        jmp     NoCommentInText
CmChar2:                mov     al,[esi]
                        inc     esi
                        cmp     al,"o"
                        je      CmChar3
                        jmp     NoCommentInText
CmChar3:                mov     al,[esi]
                        inc     esi
                        cmp     al,"m"
                        je      CmChar4
                        jmp     NoCommentInText
CmChar4:                mov     al,[esi]
                        inc     esi
                        cmp     al,"m"
                        je      CmChar5
                        jmp     NoCommentInText
CmChar5:                mov     al,[esi]
                        inc     esi
                        cmp     al,"e"
                        je      CmChar6
                        jmp     NoCommentInText
CmChar6:                mov     al,[esi]
                        inc     esi
                        cmp     al,"n"
                        je      CmChar7
                        jmp     NoCommentInText
CmChar7:                mov     al,[esi]
                        inc     esi
                        cmp     al,"t"
                        je      CmChar8
                        jmp     NoCommentInText
                        ; Strip space/tabs
CmChar8:                mov     al,[esi]
                        inc     esi
                        cmp     esi,ebx
                        jg      NoCommentInText
                        cmp     al," "
                        je      CmChar8
                        cmp     al,9
                        je      CmChar8
                        ; Get comment char
                        mov     cl,al
                        ; Calculate comment length
Commentclose:           mov     al,[esi]
                        inc     esi             ; Get char
                        cmp     esi,ebx         ; Check for end of line
                        jg      EndOfCmText
                        cmp     cl,al           ; Same char ?
                        jne     Commentclose
                        ; Retrieve the length from esi
                        invoke  MGetStrLenCR,esi,MaxSize
                        add     esi,eax         ; Add resting amount of chars
EndOfCmText:            mov     eax,esi
                        sub     eax,ComLineAddr
                        ; Replace the comment block with spaces
                        invoke  FillMem,ComLineAddr,eax
                        ; Convert till no more comments
NoCommentInText:        jmp     DeadCommentCheck
EndComment:             xor     eax,eax
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
RemoveCommentBlocks     endp

; --------------- Remove a commented block from a text
; Text must be in lower case
RemoveCommentBlocksEx   proc    Text:dword,MaxSize:dword,OpeningComment:dword,ClosingComment:dword
                        local   OpeningLen:dword
                        local   ClosingLen:dword
                        local   ComAddr:dword
                        local   ComPositionEx:dword
                        local   OldComPositionEx:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        push    edx
                        ; Calculate end address
                        mov     eax,MaxSize
                        test    eax,eax
                        jz      EndCommentEx
                        mov     eax,Text
                        test    eax,eax
                        jz      EndCommentEx
                        ; Get len of opening comment
                        invoke  lstrlen,OpeningComment
                        test    eax,eax
                        jz      EndCommentEx
                        mov     OpeningLen,eax
                        ; Get len of closing comment
                        invoke  lstrlen,ClosingComment
                        test    eax,eax
                        jz      EndCommentEx
                        mov     ClosingLen,eax
                        mov     ComPositionEx,1
DeadCommentCheckEx:     ; Seek comment word globally
                        invoke  Str_InString,ComPositionEx,Text,OpeningComment
                        test    eax,eax
                        jz      EndCommentEx
                        mov     ebx,eax
                        add     ebx,OpeningLen
                        mov     OldComPositionEx,ebx
                        mov     ComPositionEx,ebx
                        dec     eax
                        mov     esi,Text                ; Get found opening comment address
                        add     esi,eax
                        mov     ComAddr,esi             ; Store it
                        invoke  Str_InString,ComPositionEx,Text,ClosingComment
                        test    eax,eax                 ; No closing comment found ?
                        jnz     NoClosingCommentEx
                        mov     ebx,MaxSize
                        sub     ebx,OldComPositionEx
                        add     ebx,ClosingLen
                        inc     ebx
                        invoke  FillMem,ComAddr,ebx
                        jmp     EndCommentEx
NoClosingCommentEx:     mov     ebx,eax
                        add     ebx,ClosingLen
                        mov     ComPositionEx,ebx
                        sub     eax,OldComPositionEx
                        add     eax,ClosingLen
                        add     eax,ClosingLen
                        invoke  FillMem,ComAddr,eax
                        mov     eax,ComPositionEx       ; Text bounds ?
                        cmp     MaxSize,eax
                        jle     EndCommentEx
                        jmp     DeadCommentCheckEx
EndCommentEx:           xor     eax,eax
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
RemoveCommentBlocksEx   endp

; --------------- Remove a quotes from a text
RemoveQuotes            proc    Text:dword,MaxSize:dword
                        local   TextDest:dword
                        push    ebx
                        push    ecx
                        push    edx
                        mov     eax,Text
                        test    eax,eax
                        jz      NoRemQuotes
                        mov     eax,MaxSize
                        inc     eax
                        invoke  AllocMem,eax
                        mov     TextDest,eax
                        mov     al,"'"
                        invoke  StripStringInc,Text,TextDest,al,al
                        mov     al,34
                        invoke  StripStringInc,TextDest,Text,al,al
                        invoke  FreeMem,TextDest
NoRemQuotes:            pop     edx
                        pop     ecx
                        pop     ebx
                        xor     eax,eax
                        ret
RemoveQuotes            endp

; --------------- Remove single lines comments
RemoveSingleLineComment proc    Text:dword,MaxSize:dword,SearchChar:dword
                        local   TextDest:dword
                        push    ebx
                        push    ecx
                        push    edx
                        mov     eax,Text
                        test    eax,eax
                        jz      NoRemSngCom
                        mov     eax,MaxSize
                        inc     eax
                        invoke  AllocMem,eax
                        mov     TextDest,eax
                        xor     eax,eax
                        mov     eax,SearchChar
                        mov     al,[eax]
                        invoke  StripStringInc2,Text,TextDest,al
                        invoke  RtlMoveMemory,Text,TextDest,MaxSize
                        invoke  FreeMem,TextDest
NoRemSngCom:            pop     edx
                        pop     ecx
                        pop     ebx
                        xor     eax,eax
                        ret
RemoveSingleLineComment endp

; --------------- Blank a range of chars (inclusive)
; Lousely based on hutch's masterpiece.
StripStringInc          proc    lpszSource:dword,lpszDest:dword,stByte:byte,enByte:byte
                        push    esi
                        push    edi
                        push    eax
                        push    ebx
                        push    ecx
                        push    edx
                        mov     cl,stByte
                        mov     dl,enByte
                        mov     esi,lpszSource
                        mov     edi,lpszDest
lpSt:                   mov     al,[esi]                ; read BYTE
                        inc     esi
                        test    al,al
                        jz      EndStrip
                        cmp     al,cl                   ; test for start byte
                        je      FoundEntry
                        mov     [edi],al                ; write byte
                        inc     edi
                        jmp     lpSt
FoundEntry:             mov     byte ptr [edi]," "      ; write byte for entry char
                        inc     edi
InStripLoop:            mov     al,[esi]
                        inc     esi
                        test    al,al
                        jz      EndStrip
                        cmp     al,dl                   ; test for end byte
                        je      FoundEndPoint
                        cmp     al,13                   ; test for end of line
                        je      FoundEndOLPoint
                        mov     byte ptr [edi]," "
                        inc     edi
                        jmp     InStripLoop
FoundEndPoint:          mov     byte ptr [edi]," "
                        inc     edi
                        jmp     lpSt
FoundEndOLPoint:        mov     byte ptr [edi],al
                        inc     edi
                        jmp     lpSt
EndStrip:               mov     [edi],al                ; write terminator
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     eax
                        pop     edi
                        pop     esi
                        ret
StripStringInc          endp

; --------------- Blank a range of chars (inclusive)
; Lousely based on hutch's masterpiece.
StripStringInc2         proc    lpszSource:dword,lpszDest:dword,stByte:byte
                        push    esi
                        push    edi
                        push    eax
                        push    ebx
                        push    ecx
                        push    edx
                        mov     cl,stByte
                        mov     esi,lpszSource
                        mov     edi,lpszDest
lpSt:                   mov     al,[esi]                ; read BYTE
                        inc     esi
                        test    al,al
                        jz      EndStrip
                        cmp     al,cl                   ; test for start byte
                        je      FoundEntry
                        mov     [edi],al                ; write byte
                        inc     edi
                        jmp     lpSt
FoundEntry:             mov     byte ptr [edi]," "      ; write byte for entry char
                        inc     edi
InStripLoop:            mov     al,[esi]
                        inc     esi
                        test    al,al
                        jz      EndStrip
                        cmp     al,13                   ; test for end of line
                        je      FoundEndOLPoint
                        mov     byte ptr [edi]," "
                        inc     edi
                        jmp     InStripLoop
FoundEndOLPoint:        mov     byte ptr [edi],al
                        inc     edi
                        jmp     lpSt
EndStrip:               mov     [edi],al                ; write terminator
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     eax
                        pop     edi
                        pop     esi
                        ret
StripStringInc2         endp

; --------------- Blank a range of chars (exclusive)
; Lousely based on hutch's masterpiece.
StripStringEx           proc    lpszSource:dword,lpszDest:dword,stByte:byte,enByte:byte
                        push    esi
                        push    edi
                        push    eax
                        push    ebx
                        push    ecx
                        push    edx
                        mov     cl,stByte
                        mov     dl,enByte
                        mov     esi,lpszSource
                        mov     edi,lpszDest
lpSt:                   mov     al,[esi]   ; read BYTE
                        inc     esi
                        test    al,al      ; test for zero
                        jz      EndStrip
                        cmp     al,cl      ; test for start byte
                        je      FoundEntry
                        mov     [edi],al   ; write byte
                        inc     edi
                        jmp     lpSt
FoundEntry:             mov     byte ptr [edi],al
                        inc     edi
InStripLoop:            mov     al,[esi]
                        inc     esi
                        test    al,al       ; test for zero
                        jz      EndStrip
                        cmp     al,dl      ; test for end byte
                        je      FoundEndPoint
                        mov     byte ptr [edi]," "
                        inc     edi
                        jmp     InStripLoop
FoundEndPoint:          mov     byte ptr [edi],al
                        inc     edi
                        jmp     lpSt
EndStrip:               mov     [edi],al   ; write terminator
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     eax
                        pop     edi
                        pop     esi
                        ret
StripStringEx           endp

; --------------- Replace spaces from a text
; Return new length
RemoveSpaces            proc    String:dword,DestString:dword,MaxSize:dword
                        push    ebx
                        push    ecx
                        push    esi
                        push    edi
                        xor     ebx,ebx
                        mov     esi,String
                        test    esi,esi
                        jz      NoRemSpcs
                        mov     edi,DestString
                        mov     ecx,MaxSize
                        test    ecx,ecx
                        jz      NoRemSpcs
RemSpcs:                mov     al,[esi]
                        cmp     al," "
                        je      NoSpc
                        mov     byte ptr [edi],al
                        inc     edi
                        inc     ebx
NoSpc:                  inc     esi
                        dec     ecx
                        jnz     RemSpcs
NoRemSpcs:              mov     eax,ebx
                        pop     edi
                        pop     esi
                        pop     ecx
                        pop     ebx
                        ret
RemoveSpaces            endp

; --------------- Replace spaces in a text by chr(2)
MarkSpaces              proc    String:dword,DestString:dword,MaxSize:dword
                        push    ecx
                        push    esi
                        push    edi
                        xor     eax,eax
                        mov     esi,String
                        test    esi,esi
                        jz      NoMarkSpcs
                        mov     edi,DestString
                        mov     ecx,MaxSize
                        test    ecx,ecx
                        jz      NoMarkSpcs
RemSpcsMark:            mov     al,[esi]
                        cmp     al," "
                        je      NoSpcMark
                        mov     byte ptr [edi],al
                        jmp     FoundSpcToMark
NoSpcMark:              mov     byte ptr [edi],2
FoundSpcToMark:         inc     edi
                        inc     esi
                        dec     ecx
                        jnz     RemSpcsMark
NoMarkSpcs:             pop     edi
                        pop     esi
                        pop     ecx
                        ret
MarkSpaces              endp

; --------------- Replace " in a text by spaces
ReplaceQuotes           proc    String:dword,MaxSize:dword
                        push    ecx
                        push    esi
                        xor     eax,eax
                        mov     esi,String
                        test    esi,esi
                        jz      NoRepQuotes
                        mov     ecx,MaxSize
                        test    ecx,ecx
                        jz      NoRepQuotes
RemQtsMark:             mov     al,[esi]
                        cmp     al,34
                        jne     FoundQtToMark
                        mov     byte ptr [esi]," "
FoundQtToMark:          inc     esi
                        dec     ecx
                        jnz     RemQtsMark
NoRepQuotes:            pop     esi
                        pop     ecx
                        ret
ReplaceQuotes           endp

; --------------- Replace " in a text by spaces
ReplaceApos             proc    String:dword,MaxSize:dword
                        push    ecx
                        push    esi
                        xor     eax,eax
                        mov     esi,String
                        test    esi,esi
                        jz      NoRepApos
                        mov     ecx,MaxSize
                        test    ecx,ecx
                        jz      NoRepApos
RemAPOsMark:            mov     al,[esi]
                        cmp     al,"'"
                        jne     FoundAPOToMark
                        mov     byte ptr [esi]," "
FoundAPOToMark:         inc     esi
                        dec     ecx
                        jnz     RemAPOsMark
NoRepApos:              pop     esi
                        pop     ecx
                        ret
ReplaceApos             endp

; --------------- Remove ; from a text
RemoveCommChar          proc    String:dword,DestString:dword,MaxSize:dword
                        push    ecx
                        push    esi
                        push    edi
                        xor     eax,eax
                        mov     esi,String
                        test    esi,esi
                        jz      NoRemComm
                        mov     edi,DestString
                        mov     ecx,MaxSize
                        test    ecx,ecx
                        jz      NoRemComm
RemComm:                mov     al,[esi]
                        cmp     al,";"
                        je      NoComm
                        mov     byte ptr [edi],al
                        inc     edi
NoComm:                 inc     esi
                        dec     ecx
                        jnz     RemComm
NoRemComm:              pop     edi
                        pop     esi
                        pop     ecx
                        ret
RemoveCommChar          endp

; --------------- FillHexBytes()
FillHexBytes            proc    Source:dword,Dest:dword,Len:dword,Off:dword,LineOff:dword,NbrLines:dword,ElOffset:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        push    edx
                        cld
                        mov     esi,Source
                        mov     edi,Dest
                        add     edi,Off
                        mov     ebx,offset TabHex
                        mov     ecx,NbrLines    ; Proceed lines
                        mov     edx,ElOffset
                        xor     eax,eax         ; Avoid *2 boundaries
FillLines:              push    ecx             ; Save ecx
                        mov     ecx,Len         ; Proceed bytes
FillBytes:              xor     ax,ax           ; Avoid *2 boundaries
                        mov     al,[esi]        ; Load one byte
                        inc     esi
                        mov     ax,[ebx+eax*2]  ; Get ascii value
                        stosw                   ; Store 2 bytes
                        add     edi,edx         ; +5-2
                        dec     ecx
                        jnz     FillBytes
                        add     edi,LineOff     ; Next line
                        pop     ecx
                        dec     ecx
                        jnz     FillLines
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
FillHexBytes            endp

; --------------- FillHexWords()
FillHexWords            proc    Source:dword,Dest:dword,Len:dword,Off:dword,LineOff:dword,NbrLines:dword,ElOffset:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        push    edx
                        cld
                        mov     esi,Source
                        mov     edi,Dest
                        add     edi,Off
                        mov     ebx,offset TabHex
                        mov     ecx,NbrLines    ; Proceed lines
                        mov     edx,ElOffset
                        xor     eax,eax         ; Avoid *2 boundaries
FillLinesW:             push    ecx             ; Save ecx
                        mov     ecx,Len         ; Proceed bytes
FillWords:              xor     ax,ax           ; Avoid *2 boundaries
                        mov     al,[esi]        ; Load one byte
                        inc     esi
                        mov     ax,[ebx+eax*2]  ; Get ascii value
                        mov     [edi+2],ax      ; Store 2 bytes in lo position
                        add     edi,2
                        dec     ecx
                        jz      StopFillWords
                        xor     ax,ax
                        mov     al,[esi]
                        inc     esi
                        mov     ax,[ebx+eax*2]
                        mov     [edi-2],ax      ; Store 2 bytes in hi position
                        add     edi,2
                        add     edi,edx
                        dec     ecx
                        jnz     FillWords
                        jmp     NoStopWords
StopFillWords:          add     edi,edx
NoStopWords:            add     edi,LineOff     ; Next line
                        pop     ecx
                        dec     ecx
                        jnz     FillLinesW
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
FillHexWords            endp

; --------------- FillHexDWords()
FillHexDWords           proc    Source:dword,Dest:dword,Len:dword,Off:dword,LineOff:dword,NbrLines:dword,ElOffset:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        push    edx
                        cld
                        mov     esi,Source
                        mov     edi,Dest
                        add     edi,Off
                        mov     ebx,offset TabHex
                        mov     ecx,NbrLines    ; Proceed lines
                        mov     edx,ElOffset
                        xor     eax,eax         ; Avoid *2 boundaries
FillLinesD:             push    ecx             ; Save ecx
                        mov     ecx,Len         ; Proceed bytes
FillDWords:             xor     ax,ax           ; Avoid *2 boundaries
                        mov     al,[esi]        ; Load one byte
                        inc     esi
                        mov     ax,[ebx+eax*2]  ; Get ascii value
                        mov     [edi+6],ax
                        add     edi,2
                        dec     ecx
                        jz      StopFillDWords
                        xor     ax,ax
                        mov     al,[esi]
                        inc     esi
                        mov     ax,[ebx+eax*2]
                        mov     [edi+2],ax
                        add     edi,2
                        add     edi,edx
                        dec     ecx
                        jz      StopFillDWords
                        xor     ax,ax
                        mov     al,[esi]
                        inc     esi
                        mov     ax,[ebx+eax*2]
                        mov     [edi-2],ax
                        add     edi,2
                        add     edi,edx
                        dec     ecx
                        jz      StopFillDWords
                        xor     ax,ax
                        mov     al,[esi]
                        inc     esi
                        mov     ax,[ebx+eax*2]
                        mov     [edi-6],ax
                        add     edi,2
                        add     edi,edx
                        dec     ecx
                        jnz     FillDWords
                        jmp     NoStopDWords
StopFillDWords:         add     edi,edx
NoStopDWords:           add     edi,LineOff     ; Next line
                        pop     ecx
                        dec     ecx
                        jnz     FillLinesD
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
FillHexDWords           endp

; --------------- Retrieve includes references from a text
RetrieveIncludes        proc    String:dword,MaxSize:dword,CommentChar:dword
                        local   TxtBlock:dword
                        local   TxtLines:dword
                        local   TxtLineLen:dword
                        local   LineBlock:dword
                        local   DestMem:dword
                        local   CurDestMem:dword
                        mov     eax,String
                        test    eax,eax
                        jz      NoRetrieve
                        push    ebx
                        push    ecx
                        push    esi
                        push    edi
                        mov     eax,MaxSize
                        inc     eax
                        invoke  AllocMem,eax
                        mov     TxtBlock,eax
                        ; Copy the text
                        invoke  RtlMoveMemory,TxtBlock,String,MaxSize
                        ; Remove comments
                        invoke  ReplaceTabs,TxtBlock,MaxSize
                        invoke  ReplaceQuotes,TxtBlock,MaxSize
                        invoke  ReplaceApos,TxtBlock,MaxSize
                        invoke  LoCase,TxtBlock
                        invoke  RemoveCommentBlocks,TxtBlock,MaxSize
                        invoke  RemoveSingleLineComment,TxtBlock,MaxSize,CommentChar
                        invoke  Str_InString,1,TxtBlock,addr IncWord
                        test    eax,eax
                        jz      NoIncludeInFile
                        ; Check whole file
                        invoke  CountFileLines,TxtBlock,MaxSize
                        mov     TxtLines,eax
                        mov     ecx,eax
                        ; Alloc mem to store results
                        invoke  AllocMem,MaxSize
                        mov     DestMem,eax
                        mov     CurDestMem,eax
                        mov     ecx,TxtLines
CheckIncLines:          push    ecx
                                dec     ecx
                                ; Get the line address
                                invoke  GetFileLineAddress,TxtBlock,MaxSize,ecx
                                push    eax
                                        ; Get len of line
                                        invoke  MGetStrLenCR,eax,MaxSize
                                        mov     TxtLineLen,eax
                                        inc     eax
                                        ; Alloc the required memory
                                        invoke  AllocMem,eax
                                        mov     LineBlock,eax
                                pop     ebx
                                ; Copy the text line
                                invoke  RtlMoveMemory,LineBlock,ebx,TxtLineLen
                                ; Search include word in line
                                invoke  Str_InString,1,LineBlock,addr IncWord
                                test    eax,eax
                                jz      NoIncludeInLine
                                ; Remove all include words
                                invoke  Str_Space,LineBlock,addr IncWord
                                ; Copy the line in dest mem and remove spaces
                                invoke  RemoveSpaces,LineBlock,CurDestMem,TxtLineLen
                                ; Next position in mem
                                mov     ebx,CurDestMem
                                add     ebx,eax
                                mov     CurDestMem,ebx
                                ; Add crlf
                                mov     eax,CurDestMem
                                mov     byte ptr [eax],13
                                inc     eax
                                mov     byte ptr [eax],10
                                inc     eax
                                mov     CurDestMem,eax
NoIncludeInLine:                invoke  FreeMem,LineBlock
                        pop     ecx
                        dec     ecx
                        jnz     CheckIncLines
NoIncludeInFile:        mov     eax,CurDestMem
                        sub     eax,DestMem
                        pop     edi
                        pop     esi
                        pop     ecx
                        pop     ebx
NoRetrieve:             ret
RetrieveIncludes        endp

; --------------- Duplicate a string
NewString               proc    String:dword
                        local   StringLen:dword
                        invoke  lstrlen,String
                        mov     StringLen,eax
                        inc     eax
                        invoke  AllocMem,eax
                        test    eax,eax
                        jz      NoNewString
                        push    eax
                        invoke  RtlMoveMemory,eax,String,StringLen
                        pop     eax
NoNewString:            ret
NewString               endp

; --------------- Search and return for a keyword line in a given text
RetrieveKeyWordLine     proc    String:dword,KeyWordName:dword,MaxSize:dword,CommentChar:dword
                        local   LnAddr:dword
                        local   LnLen:dword
                        local   KeyWordSource:dword
                        local   KeyWordToSearch:dword
                        local   KeyWordToSearchLen:dword
                        push    esi
                        push    edi
                        invoke  ReplaceTabs,String,MaxSize
                        invoke  ReplaceQuotes,String,MaxSize
                        invoke  ReplaceApos,String,MaxSize
                        invoke  RemoveCommentBlocks,String,MaxSize
                        invoke  RemoveSingleLineComment,String,MaxSize,CommentChar
                        ; Create a new string
                        invoke  NewString,KeyWordName
                        mov     KeyWordSource,eax
                        ; Remove leading/Trailing spaces
                        invoke  trim,KeyWordSource
                        ; Retrieve new length
                        invoke  lstrlen,KeyWordSource
                        test    eax,eax
                        jz      KeyWordNotFound
                        ; Alloc memory for constant name
                        ; and add a space to the name
                        inc     eax
                        mov     KeyWordToSearchLen,eax
                        ; Append zero
                        inc     eax
                        invoke  AllocMem,eax
                        mov     KeyWordToSearch,eax
                        invoke  FillMemDat,KeyWordToSearch,KeyWordToSearchLen,20h
                        ; Copy the constant name
                        mov     eax,KeyWordToSearchLen
                        dec     eax
                        invoke  RtlMoveMemory,KeyWordToSearch,KeyWordSource,eax
                        ; Free trimed keyword
                        invoke  FreeMem,KeyWordSource
                        invoke  Str_InString,1,String,KeyWordToSearch
                        test    eax,eax
                        jz      KeyWordNotFound
                        ; Get real line number
                        invoke  GetCurrentLine,String,MaxSize,eax
                        ; line Correction
                        dec     eax
                        ; Get line address
                        invoke  GetFileLineAddress,String,MaxSize,eax
                        mov     LnAddr,eax
                        ; Get line len
                        invoke  MGetStrLenCR,eax,MaxSize
                        mov     LnLen,eax
                        invoke  AllocMem,eax
                        push    eax
                        invoke  RtlMoveMemory,eax,LnAddr,LnLen
                        pop     eax
                        pop     edi
                        pop     esi
                        ret
KeyWordNotFound:        invoke  FreeMem,KeyWordSource
                        xor     eax,eax
                        pop     edi
                        pop     esi
                        ret
RetrieveKeyWordLine     endp

; --------------- Search and return for a keyword from a list of include files
RetrieveKeyWordFromInc  proc    PrimaryString:dword,KeyWordName:dword,MaxSize:dword,Commentchar:dword
                        local   IncFiles:dword
                        local   IncludeLen:dword
                        local   LoadedFile:dword
                        local   IncLine:dword
                        local   OutLineMem:dword
                        push    esi
                        push    edi
                        ; Check primary file to retrieve include list
                        invoke  RetrieveIncludes,PrimaryString,MaxSize,Commentchar
                        mov     IncFiles,eax
                        test    eax,eax
                        jz      NoIncludeInFile

                        ; Remove crlf from list
        ;               invoke  Str_Blank,eax,addr CRLF
        ;               mov     esi,IncFiles

;                       invoke  MSaveFile,ADDR tt,IncFiles,MaxSize

;                       push    esi
;                               ; Check filename length
;                               push    esi
;                                       invoke  StrLen,esi
;                                       mov     IncludeLen,eax
;                               pop     esi
;                               ; Load include files
;                               invoke  LoadFile,esi,IncludeBytes
;                               mov     LoadedFile,eax
;                               ; Check the file
;                               invoke  RetrieveKeyWordLine,eax,KeyWordName,IncludeBytes
;                               or      eax,eax
;                               jz      NoKeyWordInFile
;                               mov     IncLine,eax
;                               ; Get found line len
;                               invoke  GetStrLenCR,eax
;                               push    eax
;                                       ; Alloc room for line
;                                       invoke  AllocMem,eax
;                                       mov     OutLineMem,eax
;                               pop     eax
;                               invoke  RtlMoveMemory,OutLineMem,IncLine,eax
;                               invoke  FreeMem,LoadedFile
;                       pop     esi
;                       invoke  FreeMem,IncFiles
;                       pop     edi
;                       pop     esi
;                       ret
;NoKeyWordInFile:               invoke  FreeMem,LoadedFile
;                       pop     esi
                        invoke  FreeMem,IncFiles
NoIncludeInFile:        xor     eax,eax
                        pop     edi
                        pop     esi
                        ret
RetrieveKeyWordFromInc  endp

; --------------- Split a string
WAAsmStringSplit        proc    StrToSplit:dword,StrSplitter:dword
                        local   OldPosSplitter:dword
                        local   PosSplitter:dword
                        local   MemSplitter:dword
                        local   LngSplitter:dword
                        local   LenSplitter:dword
                        local   LenElements:dword
                        local   i:dword
                        push    ebx
                        push    ecx
                        push    edx
                        push    esi
                        push    edi
                        mov     PosSplitter,0
                        mov     MemSplitter,0
                        mov     LngSplitter,0
                        mov     LenSplitter,0
                        mov     LenElements,0
                        mov     i,0
                        mov     OldPosSplitter,1
                        invoke  lstrlen,StrSplitter
                        mov     LenSplitter,eax
                        ; Count occurences
CountSplits:            invoke  Str_InString,OldPosSplitter,StrToSplit,StrSplitter
                        mov     PosSplitter,eax
                        test    eax,eax
                        jz      StopCountSplits
                        add     LngSplitter,8
                        inc     LenElements
                        mov     eax,PosSplitter
                        add     eax,LenSplitter
                        mov     OldPosSplitter,eax
                        jmp     CountSplits
StopCountSplits:        mov     eax,LenElements 
                        test    eax,eax
                        jz      StopSplit
                        mov     eax,LngSplitter
                        add     eax,4
                        invoke  AllocMem,eax    
                        mov     MemSplitter,eax
                        test    eax,eax
                        jz      StopSplit
                        mov     OldPosSplitter,1
                        ; Store number of elements
                        mov     eax,MemSplitter
                        mov     ebx,LenElements
                        mov     [eax],ebx
                        mov     LngSplitter,4
StoreSplits:            invoke  Str_InString,OldPosSplitter,StrToSplit,StrSplitter
                        mov     PosSplitter,eax
                        test    eax,eax
                        jz      StopStoreSplits
                        ; Store position of string and length
                        mov     eax,MemSplitter
                        add     eax,LngSplitter
                        mov     ebx,OldPosSplitter
                        mov     [eax],ebx
                        mov     eax,MemSplitter
                        add     eax,LngSplitter
                        add     eax,4
                        mov     ebx,PosSplitter
                        sub     ebx,OldPosSplitter
                        mov     [eax],ebx
                        add     LngSplitter,8
                        mov     eax,PosSplitter
                        add     eax,LenSplitter
                        mov     OldPosSplitter,eax
                        jmp     StoreSplits
StopStoreSplits:        mov     eax,MemSplitter
StopSplit:              pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        ret
WAAsmStringSplit        endp

; --------------- Retrieve a block containing file lines offsets
; (Returns memory block or 0)
GetLinesOffsets         proc    TextToGet:dword
                        local   TxtLen:dword
                        local   TxtLines:dword
                        local   PointersBlock:dword
                        push    ecx
                        push    edi
                        mov     eax,TextToGet
                        test    eax,eax
                        jz      NothingToSort
                        invoke  lstrlen,eax
                        mov     TxtLen,eax
                        test    eax,eax
                        jz      NothingToSort
                        invoke  CountFileLines,TextToGet,TxtLen
                        mov     TxtLines,eax
                        test    eax,eax
                        jz      NothingToSort
                        inc     eax                     ; +len
                        shl     eax,2                   ; *4
                        ; Alloc a block of memory for pointers
                        invoke  AllocMem,eax
                        mov     PointersBlock,eax
                        test    eax,eax
                        jz      NothingToSort
                        mov     eax,TxtLines
                        ; Fill pointers array
                        mov     edi,PointersBlock
                        cld
                        stosd                           ; Store number of entries
                        mov     ecx,eax
                        xor     eax,eax
SetPointers:            push    eax
                        invoke  GetFileLineOffset,TextToGet,TxtLen,eax
                        stosd                           ; Store address
                        pop     eax
                        inc     eax
                        dec     ecx
                        jnz     SetPointers
                        mov     eax,PointersBlock
NothingToSort:          pop     edi
                        pop     ecx
                        ret
GetLinesOffsets         endp

; --------------- Fill the API listbox of MAsmEd
; (return max length or 0)
FillAPIListBox          proc    hListBox:dword,APIDbMem:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        push    edx
                        mov     eax,APIDbMem
                        test    eax,eax
                        jz      EmptyArg
                        ; Redraw off
                        invoke  SendMessage,hListBox,WM_SETREDRAW,0,0
                        mov     ebx,APIDbMem
                        mov     ecx,[ebx]               ; Number of lines
                        add     ebx,4
                        push    dword ptr [ebx]         ; Save max length
                        add     ebx,(32*8)              ; Pass indexes
                        test    ecx,ecx
                        jz      EmptyList
FillListBox:            add     ebx,4                   ; Next element
                        push    ecx
                        mov     eax,[ebx]               ; Get offset
                        add     eax,APIDbMem            ; Correct it
                        invoke  SendMessage,hListBox,LB_ADDSTRING,0,eax
                        pop     ecx
                        dec     ecx
                        jnz     FillListBox
EmptyList:              ; Redraw on
                        invoke  SendMessage,hListBox,WM_SETREDRAW,1,0
                        pop     eax                     ; Restore max length
EmptyArg:               pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
FillAPIListBox          endp

; --------------- Fill the API listbox of MAsmEd
; (return max length or 0)
GetAPIKeywordToCorrect  proc    APIDbMem:dword,Keyword:dword
                        push    esi
                        push    edi
                        push    ebx
                        push    ecx
                        push    edx
                        mov     ebx,APIDbMem
                        test    ebx,ebx
                        jz      EmptyList
                        ; Use first letter as index
                        mov     eax,Keyword
                        movzx   eax,byte ptr [eax]
                        mov     al,[CharTableUpper+eax] ; To uppercase
                        cmp     eax,"A"
                        jb      EmptyList
                        cmp     eax,"_"
                        ja      EmptyList
                        sub     eax,"A"
                        mov     ecx,[ebx+eax*8+8+4]
                        mov     eax,[ebx+eax*8+8]
                        test    eax,eax
                        jz      EmptyList
                        add     ebx,eax                 ; Add index
                        ; Greater than file length ?
                        test    ecx,ecx
                        jz      EmptyList
FillListBox:            push    ebx
                        push    ecx
                        mov     eax,[ebx]               ; Get offset
                        add     eax,APIDbMem            ; Correct it
                        push    eax
                        invoke  lstrcmpi,eax,Keyword
                        pop     ecx
                        test    eax,eax
                        jz      FoundInList
                        pop     ecx
                        pop     ebx
                        add     ebx,4
                        dec     ecx
                        jnz     FillListBox
                        jmp     EmptyList
FoundInList:            pop     eax                     ; (Garbage from ecx/ebx)
                        pop     eax
                        mov     eax,ecx                 ; Position in list
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
EmptyList:              xor     eax,eax
                        pop     edx
                        pop     ecx
                        pop     ebx
                        pop     edi
                        pop     esi
                        ret
GetAPIKeywordToCorrect  endp

; --------------- Convert a .lst file
ConvertLstFile          proc    SourceApiFileName:dword,DestApiFileName:dword,ConvertHook:dword
                        local   ApiFileHandle:dword
                        local   ApiFileSize:dword
                        local   DbFileHandle:dword
                        local   hApiFileMem:dword
                        local   FileLines:dword
                        local   HeaderLen:dword
                        local   hIndex:dword
                        local   CurrentLine:dword
                        push    ebx
                        push    ecx
                        push    edx
                        push    esi
                        push    edi
                        ; Create a new file
                        invoke  DeleteFile,DestApiFileName
                        invoke  CreateFile,DestApiFileName,GENERIC_READ or GENERIC_WRITE,0,0,CREATE_NEW,FILE_ATTRIBUTE_ARCHIVE or FILE_FLAG_RANDOM_ACCESS,0
                        mov     DbFileHandle,eax
                        cmp     eax,INVALID_HANDLE_VALUE
                        je      ErrorDestFile
                        ; Open file
                        invoke  CreateFile,SourceApiFileName,GENERIC_READ,0,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0
                        mov     ApiFileHandle,eax
                        cmp     eax,INVALID_HANDLE_VALUE
                        je      ErrorSourceFile
                        ; Get the file length
                        invoke  GetFileSize,ApiFileHandle,addr FileRW
                        mov     ApiFileSize,eax
                        ; Alloc the memory
                        add     eax,4
                        invoke  AllocMem,eax
                        mov     hApiFileMem,eax
                        test    eax,eax
                        jz      ErrorSourceMem
                        ; Read the source file
                        invoke  ReadFile,ApiFileHandle,hApiFileMem,ApiFileSize,addr FileRW,0
                        test    eax,eax
                        jz      ErrorSourceLoad
                        invoke  CountFileLines,hApiFileMem,ApiFileSize
                        mov     FileLines,eax
                        test    eax,eax
                        jz      EmptyLstSource
                        ; Alloc room for indexes
                        ; (dwords from AA to __)
                        mov     eax,FileLines           ; hack here (lines+1)
                        inc     eax
                        shl     eax,2
                        mov     HeaderLen,eax
                        invoke  AllocMem,eax
                        mov     hIndex,eax
                        test    eax,eax
                        jz      ErrorSourceLoad
                        mov     ecx,FileLines
                        mov     eax,hIndex
                        dec     ecx
                        mov     [eax],ecx               ; Number of lines to write
                        mov     CurrentLine,0
IndexLstLoop:           mov     edi,ConvertHook
                        test    edi,edi
                        jz      NoConvLstHook
                        push    ebx
                        push    ecx
                        push    edx
                        push    esi
                        push    edi
                        push    FileLines
                        push    CurrentLine
                        call    edi
                        add     esp,8
                        pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        test    eax,eax
                        jnz     NoConvLstHook
                        invoke  FreeMem,hIndex
                        invoke  FreeMem,hApiFileMem
                        invoke  CloseHandle,ApiFileHandle
                        invoke  CloseHandle,DbFileHandle
                        ; Delete faulty file
                        invoke  DeleteFile,DestApiFileName
                        pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        xor     eax,eax
                        inc     eax                     ; CONVERTDB_USERABORT
                        ret
NoConvLstHook:          push    ecx
                        invoke  GetFileLineAddress,hApiFileMem,ApiFileSize,CurrentLine
                        sub     eax,hApiFileMem         ; Make offset relative
                        mov     edi,hIndex              ; Dest mem
                        add     eax,HeaderLen           ; Add header length
                        mov     ebx,CurrentLine
                        mov     [edi+ebx*4+8],eax       ; Store line address into index table
                        inc     CurrentLine
                        pop     ecx
                        dec     ecx             
                        jnz     IndexLstLoop
                        ; Remove the eols
                        invoke  RemCRLF,hApiFileMem,ApiFileSize
                        mov     ebx,hIndex
                        mov     [ebx+4],eax             ; Max found length in strings
                        ; Write header and indexes into db file
                        invoke  WriteFile,DbFileHandle,hIndex,HeaderLen,addr FileRW,0
                        ; Write datas
                        invoke  WriteFile,DbFileHandle,hApiFileMem,ApiFileSize,addr FileRW,0
                        invoke  FreeMem,hIndex
EmptyLstSource:         invoke  FreeMem,hApiFileMem
                        invoke  CloseHandle,ApiFileHandle
                        invoke  CloseHandle,DbFileHandle
                        pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        xor     eax,eax                 ; CONVERTDB_DONE
                        ret
ErrorSourceLoad:        invoke  FreeMem,hApiFileMem
ErrorSourceMem:         invoke  CloseHandle,ApiFileHandle
ErrorSourceFile:        invoke  CloseHandle,DbFileHandle
ErrorDestFile:          pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        or      eax,-1                  ; CONVERTDB_ERROR
                        ret
ConvertLstFile          endp

; --------------- Convert a .db file
ConvertDbFile           proc    SourceApiFileName:dword,DestApiFileName:dword,ConvertHook:dword
                        local   ApiFileHandle:dword
                        local   ApiFileSize:dword
                        local   DbFileHandle:dword
                        local   hApiFileMem:dword
                        local   FileLines:dword
                        local   HeaderLen:dword
                        local   hIndex:dword
                        local   CurrentLine:dword
                        local   LastIndex:dword
                        local   LastIndexPos:dword
                        push    ebx
                        push    ecx
                        push    edx
                        push    esi
                        push    edi
                        mov     OldFncTwoChars,-1
                        ; Create a new file
                        invoke  DeleteFile,DestApiFileName
                        invoke  CreateFile,DestApiFileName,GENERIC_READ or GENERIC_WRITE,0,0,CREATE_NEW,FILE_ATTRIBUTE_ARCHIVE or FILE_FLAG_RANDOM_ACCESS,0
                        mov     DbFileHandle,eax
                        cmp     eax,INVALID_HANDLE_VALUE
                        je      ErrorDestFile
                        ; Open file
                        invoke  CreateFile,SourceApiFileName,GENERIC_READ,0,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0
                        mov     ApiFileHandle,eax
                        cmp     eax,INVALID_HANDLE_VALUE
                        je      ErrorSourceFile
                        ; Get the file length
                        invoke  GetFileSize,ApiFileHandle,addr FileRW
                        mov     ApiFileSize,eax
                        ; Alloc the memory
                        add     eax,4
                        invoke  AllocMem,eax
                        mov     hApiFileMem,eax
                        test    eax,eax
                        jz      ErrorSourceMem
                        ; Read the source file
                        invoke  ReadFile,ApiFileHandle,hApiFileMem,ApiFileSize,addr FileRW,0
                        test    eax,eax
                        jz      ErrorSourceLoad
                        invoke  CountFileLines,hApiFileMem,ApiFileSize
                        mov     FileLines,eax
                        test    eax,eax
                        jz      EmptySource
                        ; Alloc room for indexes
                        ; (dwords from AA to __)
                        invoke  AllocMem,((31*31)*8)
                        mov     hIndex,eax
                        test    eax,eax
                        jz      ErrorSourceLoad
                        mov     LastIndex,((31*31)*8)
                        mov     LastIndexPos,0
                        mov     ecx,FileLines
                        dec     ecx
IndexDbLoop:            mov     esi,ConvertHook
                        test    esi,esi
                        jz      NoConvLstHook
                        push    ebx
                        push    ecx
                        push    edx
                        push    esi
                        push    edi
                        push    FileLines
                        push    CurrentLine
                        call    edi
                        add     esp,8
                        pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        test    eax,eax
                        jnz     NoConvLstHook
                        invoke  FreeMem,hIndex
                        invoke  FreeMem,hApiFileMem
                        invoke  CloseHandle,ApiFileHandle
                        invoke  CloseHandle,DbFileHandle
                        ; Delete faulty file
                        invoke  DeleteFile,DestApiFileName
                        pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        xor     eax,eax
                        inc     eax                     ; CONVERTDB_USERABORT
                        ret
NoConvLstHook:          push    ecx
                        invoke  GetFileLineAddress,hApiFileMem,ApiFileSize,CurrentLine
                        mov     edx,eax
                        sub     edx,hApiFileMem         ; Make offset relative
                        push    edx
                        mov     esi,eax
                        lea     edi,FncTwoChars
                        cld
                        movsw                           ; Get first 2 chars of the line
                        invoke  UpCase,addr FncTwoChars
                        lea     esi,FncTwoChars
                        xor     eax,eax
                        lodsw
                        xchg    al,ah
                        sub     ax,"AA"                 ; lower index
                        mov     bx,OldFncTwoChars
                        pop     edx
                        cmp     ax,bx
                        je      NoNewDbIndex
                        mov     OldFncTwoChars,ax
                        push    edx
                        push    ax
                        mov     al,ah                   ; Hi>Lo
                        xor     ah,ah                   ; Hi=0
                        mov     bx,(31*8)
                        mul     bx                      ; Calc upper part
                        xor     ebx,ebx
                        pop     bx
                        xor     bh,bh
                        and     ax,0ffffh
                        shl     ebx,3                   ; Convert lower to table offset
                        add     ebx,eax
                        mov     edi,hIndex
                        pop     edx
                        add     edx,((31*31)*8)         ; Add header
                        mov     [edi+ebx],edx           ; Store line address into index table
                        mov     eax,LastIndex
                        sub     eax,edx
                        neg     eax                     ; Get length
                        mov     ecx,LastIndexPos
                        test    ecx,ecx
                        jnz     CreateNewDbOffset
                        mov     ecx,edi
                        add     ecx,ebx
                        add     ecx,4
CreateNewDbOffset:      mov     esi,ecx
                        mov     [esi],eax               ; Store length
                        mov     ecx,edi
                        add     ecx,ebx
                        add     ecx,4
                        mov     LastIndexPos,ecx        ; Save last position in index table
                        mov     LastIndex,edx           ; Save last index position
NoNewDbIndex:           inc     CurrentLine
                        pop     ecx
                        dec     ecx             
                        jnz     IndexDbLoop
                        ; Write length of last group
                        mov     eax,LastIndex
                        mov     ebx,ApiFileSize
                        add     ebx,((31*31)*8)
                        sub     eax,ebx
                        neg     eax
                        mov     ecx,LastIndexPos
                        mov     esi,ecx
                        mov     [esi],eax               ; Store length
                        ; Write indexes into db file
                        invoke  WriteFile,DbFileHandle,hIndex,((31*31)*8),addr FileRW,0
                        invoke  RemCRLF,hApiFileMem,ApiFileSize
                        ; Write functions
                        invoke  WriteFile,DbFileHandle,hApiFileMem,ApiFileSize,addr FileRW,0
                        invoke  FreeMem,hIndex
EmptySource:            invoke  FreeMem,hApiFileMem
                        invoke  CloseHandle,ApiFileHandle
                        invoke  CloseHandle,DbFileHandle
                        pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        xor     eax,eax                 ; CONVERTDB_DONE
                        ret
ErrorSourceLoad:        invoke  FreeMem,hApiFileMem
ErrorSourceMem:         invoke  CloseHandle,ApiFileHandle
ErrorSourceFile:        invoke  CloseHandle,DbFileHandle
ErrorDestFile:          pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        or      eax,-1                  ; CONVERTDB_ERROR
                        ret
ConvertDbFile           endp

; --------------- Convert an hexadecimal string into a dword
HexaToDw                proc    String:dword
                        push    ebx
                        push    ecx
                        push    edx
                        push    esi
                        push    edi
                        mov     edi,String
                        mov     esi,edi
again:                  mov     al,[edi]
                        inc     edi
                        test    al,al
                        jnz     again
                        sub     esi,edi
                        xor     ebx,ebx
                        add     edi,esi
                        xor     edx,edx
                        not     esi
                        .while  esi != 0
                                mov     al,[edi]
                                cmp     al,'A'
                                jb      figure
                                sub     al,'a'-10
                                adc     dl,0
                                shl     dl,5
                                add     al,dl
                                jmp     next
figure:                         sub     al,'0'
next:                           lea     ecx,[esi-1]
                                and     eax,0Fh
                                shl     ecx,2
                                shl     eax,cl
                                add     ebx,eax
                                inc     edi
                                dec     esi
                        .endw
                        mov     eax,ebx
                        pop     edi
                        pop     esi
                        pop     edx
                        pop     ecx
                        pop     ebx
                        ret
HexaToDw                endp

                        end