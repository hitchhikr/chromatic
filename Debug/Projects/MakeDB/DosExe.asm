; ------------------------------------------
; Title:    MakeDB v1.0
; 
; Author:   Franck 'hitchhikr' Charlet.
;
; Purpose:  A little utility to create an indexed file from the big.api file.
;       All indexes uses 2 chars instead of 1 for faster search.
;
; Notes:    This was originally designed to gather several files
;       but i've changed my mind finally (that's why it looks a bit strange)
;       the database format is really simple: it just contains
;       offsets/lengths for every combinations of functions
;       followed by the descriptions themselves.
;       Also the algorithm is a bit "trashy" sorry for that.
;
; ------------------------------------------

; --------------- Functions declarations
Main                proto
InitULTables        proto
AllocMem            proto   :dword
FreeMem             proto   :dword
LoCase              proto   :dword
UpCase              proto   :dword
RemCRLF             proto   :dword,:dword
ClearMem            proto   :dword,:dword
CountFileLines      proto   :dword,:dword
GetfileLineAddress  proto   :dword,:dword,:dword

; --------------- Code section
                    .code

; --------------- Main program
Main                proc    hInstance:DWORD,CommandLine:DWORD
                    invoke  SetConsoleTitle,addr AppTitle
                    invoke  ClearScreen
                    invoke  StdOut,addr AppCreds
        
                    ; Retrieve current directory
                    invoke  GetCurrentDirectory,MAX_PATH-1,addr CurDir
        
                    invoke  szCatStr,addr ApiSearchFileName,addr CurDir
                    invoke  szCatStr,addr ApiSearchFileName,addr SearchPattern
                    invoke  szCatStr,addr DbFileName,addr CurDir
                    invoke  szCatStr,addr DbFileName,addr DestFile
        
                    invoke  InitULTables
        
                    ; Search .api file
                    invoke  FindFirstFile,addr ApiSearchFileName,addr FindResult
                    cmp     eax,INVALID_HANDLE_VALUE
                    je      FinishProcess
                    mov     FindHandle,eax
        
                    ; Delete then create a new .Db file
                    invoke  DeleteFile,addr DbFileName
                    invoke  CreateFile,addr DbFileName,GENERIC_READ or GENERIC_WRITE,0,0,CREATE_NEW,FILE_ATTRIBUTE_ARCHIVE or FILE_FLAG_RANDOM_ACCESS,0
                    cmp     eax,INVALID_HANDLE_VALUE
                    je      ErrorDbFile
                    mov     DbFileHandle,eax
        
                    ; Print some useless messages
                    invoke  ClearMem,addr ApiFileName,MAX_PATH
                    invoke  szCatStr,addr ApiFileName,addr CurDir
                    invoke  szCatStr,addr ApiFileName,addr Slash
                    invoke  szCatStr,addr ApiFileName,addr FindResult.cFileName
                    invoke  StdOut,addr AddingMsg
                    invoke  StdOut,addr ApiFileName
                    invoke  StdOut,addr MsgReturn
        
                    ; Get file size
                    mov     eax,FindResult.nFileSizeLow
                    mov     ApiFileSize,eax
        
                    ; Open .api file
                    invoke  CreateFile,addr ApiFileName,GENERIC_READ,0,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0
                    mov     ApiFileHandle,eax
                    cmp     eax,INVALID_HANDLE_VALUE
                    jne     AllocApiMem
                    invoke  StdOut,addr ErrorOApiFile
                    jmp     NoApiFile
AllocApiMem:
                    ; Alloc room for .api file
                    invoke  AllocMem,ApiFileSize
                    mov     hApiFileMem,eax
                    cmp     eax,0
                    jne     ReadApiFile
                    invoke  StdOut,addr ErrorMemory
                    jmp     NoMemForFile

            ; Read complete File content
ReadApiFile:        invoke  ReadFile,ApiFileHandle,hApiFileMem,ApiFileSize,addr FileRW,0
                    cmp     eax,0
                    jne     MakeIndexes
                    invoke  StdOut,addr ErrorRApiFile
                    jmp     ErrReadingFile
MakeIndexes:    
                    ; Create offsets for file
                    invoke  CountFileLines,hApiFileMem,ApiFileSize
                    mov     FileLines,eax
        
                    ; Alloc room for indexes
                    ; (dwords from AA to __)
                    invoke  AllocMem,((31*31)*8)
                    mov     hIndex,eax
                    cmp     eax,0
                    jne     StoreIndexes
                    invoke  StdOut,addr ErrorMemory
                    jmp     ErrReadingFile
StoreIndexes:       mov     LastIndex,((31*31)*8)
                    mov     LastIndexPos,0
                    mov     ecx,FileLines
                    dec     ecx
IndexLoop:          push    ecx
                    invoke  GetfileLineAddress,hApiFileMem,ApiFileSize,CurrentLine
                    mov     edx,eax
                    sub     edx,hApiFileMem     ; Make offset relative
                    push    edx
                    mov     esi,eax
                    mov     edi,offset FncTwoChars
                    cld
                    movsw               ; Get first 2 chars of the line
                    invoke  UpCase,addr FncTwoChars
                    mov     esi,offset FncTwoChars
                    xor     eax,eax
                    lodsw
                    xchg    al,ah
                    sub     ax,"AA"         ; lower index
                    mov     bx,OldFncTwoChars
                    pop     edx
                    cmp     ax,bx
                    je      NoNewIndex
                    mov     OldFncTwoChars,ax
                    push    edx
                    push    ax
                    push    eax
                    invoke  StdOut,addr CIdx
                    pop     eax
                    mov     al,ah           ; Hi>Lo
                    xor     ah,ah           ; Hi=0
                    mov     bx,(31*8)
                    mul     bx          ; Calc upper part
                    xor     ebx,ebx
                    pop     bx
                    xor     bh,bh
                    and     ax,0ffffh
                    shl     ebx,3           ; Convert lower to table offset
                    add     ebx,eax
                    mov     edi,hIndex
                    pop     edx
                    add     edx,((31*31)*8)     ; Add header
                    mov     [edi+ebx],edx       ; Store line address into index table
                    mov     eax,LastIndex
                    sub     eax,edx
                    neg     eax         ; Get length
                    mov     ecx,LastIndexPos
                    cmp     ecx,0
                    jne     CreateNewOffset
                    mov     ecx,edi
                    add     ecx,ebx
                    add     ecx,4
CreateNewOffset:    mov     esi,ecx
                    mov     [esi],eax       ; Store length
                    mov     ecx,edi
                    add     ecx,ebx
                    add     ecx,4
                    mov     LastIndexPos,ecx    ; Save last position in index table
                    mov     LastIndex,edx       ; Save last index position
NoNewIndex:         inc     CurrentLine
                    pop     ecx
                    dec     ecx     
                    cmp     ecx,0
                    jne     IndexLoop

                    ; Write length of last group
                    mov     eax,LastIndex
                    mov     ebx,ApiFileSize
                    add     ebx,((31*31)*8)
                    sub     eax,ebx
                    neg     eax
                    mov     ecx,LastIndexPos
                    mov     esi,ecx
                    mov     [esi],eax       ; Store length
        
                    ; Write indexes into db file
                    invoke  WriteFile,DbFileHandle,hIndex,((31*31)*8),addr FileRW,0
                    invoke  RemCRLF,hApiFileMem,ApiFileSize
                    ; Write functions
                    invoke  WriteFile,DbFileHandle,hApiFileMem,ApiFileSize,addr FileRW,0
                    invoke  FreeMem,((31*31)*8)
ErrReadingFile:     invoke  FreeMem,ApiFileSize
NoMemForFile:       ; Close Api file
                    invoke  CloseHandle,ApiFileHandle
NoApiFile:          invoke  StdOut,addr MsgReturn
                    invoke  FindClose,FindHandle    ; Close the search
ErrorDbFile:        invoke  CloseHandle,DbFileHandle
FinishProcess:      invoke  StdOut,addr EndMsg
                    ret
Main                endp

; --------------- Clear a memory array
ClearMem            proc    Mem:dword,Bytes:dword
                    mov     edi,Mem
                    mov     ecx,Bytes
                    cld
                    xor     al,al
                    shr     ecx,2
                    rep     stosd
                    mov     ecx,Bytes
                    and     ecx,3
                    rep     stosb
                    ret
ClearMem            endp

; --------------- AllocMem
AllocMem            proc    MemSize:dword
                    mov     eax,MemSize
                    or      eax,eax
                    jz      NoAllocMem
                    invoke  GlobalAlloc,GMEM_FIXED+GMEM_ZEROINIT,eax
NoAllocMem:         ret
AllocMem            endp

; --------------- FreeMem
FreeMem             proc    MemHandle:dword
                    mov     eax,MemHandle
                    or      eax,eax
                    jz      NoFreeMem
                    invoke  GlobalFree,eax
NoFreeMem:          ret
FreeMem             endp

; --------------- Count the file lines
; (result is 0 based)
CountFileLines      proc    Text:dword,MaxSize:dword
                    push    esi
                    push    ebx
                    push    ecx
                    xor     ebx,ebx
                    mov     esi,Text
                    cld
                    mov     ecx,MaxSize
CountLines:         lodsb
                    cmp     al,13
                    je      NextChar
                    cmp     al,10
                    jne     NextChar
                    inc     ebx
NextChar:           loop    CountLines
                    mov     eax,ebx
                    inc     eax
                    pop     ecx
                    pop     ebx
                    pop     esi
                    ret
CountFileLines      endp

; --------------- Retrieve the address of a given line
; (result is 0 based)
GetfileLineAddress  proc    Text:dword,MaxSize:dword,Linenumber:dword
                    push    esi
                    push    ebx
                    push    ecx
                    mov     esi,Text
                    mov     ebx,Linenumber
                    or      ebx,ebx
                    jz      ReturnLine
                    cld
                    mov     ecx,MaxSize
GetLine:            lodsb
                    cmp     al,13
                    je      NextChar
                    cmp     al,10
                    jne     NextChar
                    dec     ebx
                    cmp     ebx,0
                    je      ReturnLine
NextChar:           loop    GetLine
ReturnLine:         mov     eax,esi     ; Return address
                    pop     ecx
                    pop     ebx
                    pop     esi
                    ret
GetfileLineAddress  endp

; --------------- Create ucase/lcase chars table
; (Needed for UpCase/LoCase functions)
InitULTables        proc
                    push    edi
                    push    ebx
                    push    ecx
                    lea     edi,CharTableUpper
                    cld
                    xor     eax,eax
                    mov     ecx,65
CreateCharL1Up:     mov     [edi+eax],al
                    inc     eax
                    loop    CreateCharL1Up
                    mov     ecx,26
CreateCharLowerUp:  mov     bl,al
                    mov     [edi+eax],bl
                    inc     eax
                    loop    CreateCharLowerUp
                    mov     ecx,6
CreateCharL2Up:     mov     [edi+eax],al
                    inc     eax
                    loop    CreateCharL2Up
                    mov     ecx,26
CreateCharUpperUp:  mov     bl,al
                    and     bl,0dfh
                    mov     [edi+eax],bl
                    inc     eax
                    loop    CreateCharUpperUp
                    mov     ecx,132
CreateCharL3Up:     mov     [edi+eax],al
                    inc     eax
                    loop    CreateCharL3Up
                    lea     edi,CharTableLower
                    xor     eax,eax
                    mov     ecx,65
CreateCharL1Lo:     mov     [edi+eax],al
                    inc     eax
                    loop    CreateCharL1Lo
                    mov     ecx,26
CreateCharLowerLo:  mov     bl,al
                    or      bl,020h
                    mov     [edi+eax],bl
                    inc     eax
                    loop    CreateCharLowerLo
                    mov     ecx,6
CreateCharL2Lo:     mov     [edi+eax],al
                    inc     eax
                    loop    CreateCharL2Lo
                    mov     ecx,26
CreateCharUpperLo:  mov     bl,al
                    mov     [edi+eax],bl
                    inc     eax
                    loop    CreateCharUpperLo
                    mov     ecx,132
CreateCharL3Lo:     mov     [edi+eax],al
                    inc     eax
                    loop    CreateCharL3Lo
                    pop     ecx
                    pop     ebx
                    pop     edi
                    ret
InitULTables        endp

; --------------- Convert a string to upper case
UpCase              proc    String:dword
                    push    esi
                    push    edi
                    push    ecx
                    invoke  StrLen,String       ; Get string length
                    mov     ecx,eax
                    or      ecx ,ecx
                    jz      EmptyUCase
                    lea     esi,CharTableUpper
                    mov     edi,String
                    cld
                    xor     eax,eax
DoUCase:            mov     al,[edi]
                    mov     al,[esi+eax]
                    stosb
                    loop    DoUCase         
EmptyUCase:         pop     ecx
                    pop     edi
                    pop     esi
                    ret
UpCase              endp

; --------------- Convert a string to lower case
LoCase              proc    String:dword
                    push    esi
                    push    edi
                    push    ecx
                    invoke  StrLen,String       ; Get string length
                    mov     ecx,eax
                    or      ecx,ecx
                    jz      EmptyLCase
                    lea     esi,CharTableLower
                    mov     edi,String
                    cld
                    xor     eax,eax
DoLCase:            mov     al,[edi]
                    mov     al,[esi+eax]
                    stosb
                    loop    DoLCase     
EmptyLCase:         pop     ecx
                    pop     edi
                    pop     esi
                    ret
LoCase              endp

; --------------- Replace all CR/LF combination by ^^
RemCRLF             proc    Text:dword,MaxSize:dword
                    push    esi
                    push    ebx
                    push    ecx
                    mov     ebx,-2
                    mov     esi,Text
                    cld
                    mov     ecx,MaxSize
CheckLines:         lodsb
                    cmp     al,13
                    je      PNextChar
                    cmp     al,10
                    jne     PNextChar
                    mov     word ptr [esi+ebx],05e5eh
PNextChar:          dec     ecx
                    jnz     CheckLines
                    pop     ecx
                    pop     ebx
                    pop     esi
                    ret
RemCRLF             endp
