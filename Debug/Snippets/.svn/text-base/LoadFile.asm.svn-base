; --- Open a file and load it into a memory block
; Returns the allocated memory block or 0
; (Free memory block with GlobalFree)
LoadFile                proc    FName:dword,ReadBytes:dword
                        local   FHandle:dword
                        local   FSize:dword
                        local   FPosition:dword

                        mov     FPosition,0
                        invoke  CreateFile,FName,GENERIC_READ,0,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0
                        inc     eax
                        je      Error_OpenFile
                        dec     eax
                        mov     FHandle,eax
                        invoke  GetFileSize,eax,0
                        inc     eax
                        je      Error_ReadFile
                        dec     eax
                        mov     FSize,eax
                        inc     eax                             ; Append a 0 char
                        invoke  GlobalAlloc,GMEM_FIXED or GMEM_ZEROINIT,eax
                        test    eax,eax
                        jz      Error_ReadFile
                        mov     FPosition,eax
                        invoke  ReadFile,FHandle,FPosition,FSize,ReadBytes,0
Error_ReadFile:         invoke  CloseHandle,FHandle
Error_OpenFile:         mov     eax,FPosition                   ; Return the memory block address
                        ret
LoadFile                endp
