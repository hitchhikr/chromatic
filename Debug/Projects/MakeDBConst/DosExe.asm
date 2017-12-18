; ------------------------------------------
; Title:    MakeLST v1.0
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
AllocMem            proto   :dword
FreeMem             proto   :dword
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
                    align   16
AllocApiMem:        ; Alloc room for .api file
                    invoke  AllocMem,ApiFileSize
                    mov     hApiFileMem,eax
                    test    eax,eax
                    jnz     ReadApiFile
                    invoke  StdOut,addr ErrorMemory
                    jmp     NoMemForFile
                    align   16
                    ; Read complete File content
ReadApiFile:        invoke  ReadFile,ApiFileHandle,hApiFileMem,ApiFileSize,addr FileRW,0
                    test    eax,eax
                    jnz     MakeIndexes
                    invoke  StdOut,addr ErrorRApiFile
                    jmp     ErrReadingFile
                    align   16
MakeIndexes:        ; Create offsets for file
                    invoke  CountFileLines,hApiFileMem,ApiFileSize
                    mov     FileLines,eax
                    ; Alloc room for indexes
                    mov     eax,FileLines       ; hack here (lines+1)
                    inc     eax
                    add     eax,64          ; Letters
                    shl     eax,2
                    mov     HeaderLen,eax
                    invoke  AllocMem,eax
                    mov     hIndex,eax
                    test    eax,eax
                    jnz     StoreIndexes
                    invoke  StdOut,addr ErrorMemory
                    jmp     ErrReadingFile
                    align   16
StoreIndexes:       mov     ecx,FileLines
                    mov     eax,hIndex
                    dec     ecx
                    mov     [eax],ecx       ; Number of lines to write
                    mov     CurrentLine,0
IndexLoop:          push    ecx
                    invoke  GetfileLineAddress,hApiFileMem,ApiFileSize,CurrentLine
                    push    eax
                    sub     eax,hApiFileMem     ; Make offset relative
                    mov     edi,hIndex      ; Dest mem
                    add     eax,HeaderLen       ; Add header length
                    mov     ebx,CurrentLine
                    pop     ecx
                    movzx   ecx,byte ptr [ecx]
                    mov     cl,[CharTableUpper+ecx]
                    sub     ecx,"A"
                    push    eax
                    push    ebx
                    mov     eax,ebx
                    shl     eax,2
                    add     eax,8+(64*4)
                    cmp     dword ptr [edi+ecx*8+8],0
                    jne     IndexDone
                    mov     [edi+ecx*8+8],eax       ; Store line ref.
                    mov     eax,CurrentLine
                    sub     eax,OldLine
                    jz      ZeroIndex
                    mov     ebx,OldIndexPos
                    mov     [ebx],eax           ; Store line len.
ZeroIndex:          lea     eax,[edi+ecx*8+8+4]
                    mov     OldIndexPos,eax
                    push    CurrentLine
                    pop     OldLine
IndexDone:          pop     ebx
                    pop     eax
                    mov     [edi+ebx*4+(8+(64*4))],eax  ; Store line address into index table
                    inc     CurrentLine
                    pop     ecx
                    dec     ecx
                    jnz     IndexLoop
                    ; Put last len
                    mov     eax,CurrentLine
                    sub     eax,OldLine
                    jz      ZeroIndexLast
                    mov     ebx,OldIndexPos
                    mov     [ebx],eax           ; Store line len.
ZeroIndexLast:      ; Remove the eols
                    invoke  RemCRLF,hApiFileMem,ApiFileSize
                    mov     ebx,hIndex
                    mov     [ebx+4],eax     ; Max found length in strings
                    ; Write header and indexes into db file
                    invoke  WriteFile,DbFileHandle,hIndex,HeaderLen,addr FileRW,0
                    ; Write datas
                    invoke  WriteFile,DbFileHandle,hApiFileMem,ApiFileSize,addr FileRW,0
                    invoke  FreeMem,hIndex
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
                    test    eax,eax
                    jz      NoAllocMem
                    invoke  GlobalAlloc,GMEM_FIXED+GMEM_ZEROINIT,eax
NoAllocMem:         ret
AllocMem            endp

; --------------- FreeMem
FreeMem             proc    MemHandle:dword
                    mov     eax,MemHandle
                    test    eax,eax
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
NextChar:           dec     ecx
                    jnz     CountLines
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
                    test    ebx,ebx
                    jz      ReturnLine
                    cld
                    mov     ecx,MaxSize
GetLine:            lodsb
                    cmp     al,13
                    je      NextChar
                    cmp     al,10
                    jne     NextChar
                    dec     ebx
                    jz      ReturnLine
NextChar:           dec     ecx
                    jnz     GetLine
ReturnLine:         mov     eax,esi     ; Return address
                    pop     ecx
                    pop     ebx
                    pop     esi
                    ret
GetfileLineAddress  endp

; --------------- Replace all CR/LF combination by chr(0)+chr(0)
; Return max strlen in eax
RemCRLF             proc    Text:dword,MaxSize:dword
                    push    esi
                    push    ebx
                    push    ecx
                    push    edx
                    mov     esi,Text
                    cld
                    mov     ecx,MaxSize
                    xor     edx,edx
                    xor     ebx,ebx
CheckLines:         lodsb
                    cmp     al,13
                    je      PNextChar
                    inc     edx
                    cmp     al,10
                    jne     PNextChar
                    dec     edx             ; Remove chr(10) from counter
                    cmp     edx,ebx
                    jbe     StoreMaxLen
                    mov     ebx,edx             ; Store len
StoreMaxLen:        xor     edx,edx
                    mov     byte ptr [esi-1],0
                    cmp     byte ptr [esi-2],13
                    jne     PNextChar
                    mov     byte ptr [esi-2],0
PNextChar:          dec     ecx
                    jnz     CheckLines
                    mov     eax,ebx
                    pop     edx
                    pop     ecx
                    pop     ebx
                    pop     esi
                    ret
RemCRLF             endp
