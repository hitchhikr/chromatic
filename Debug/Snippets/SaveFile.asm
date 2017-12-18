; --- Save a memory block into a file
; Returns the number of written bytes
SaveFile                proc    FName:dword,MemPos:dword,FSize:dword
                        local   FHandle:dword
                        local   FRealWritten:dword

                        mov     FRealWritten,0
                        invoke  DeleteFile,FName                ; Delete it before overwriting
                        invoke  CreateFile,FName,GENERIC_WRITE,0,0,CREATE_NEW,FILE_ATTRIBUTE_ARCHIVE or FILE_FLAG_RANDOM_ACCESS,0
                        inc     eax
                        je      Error_SaveFile
                        dec     eax
                        mov     FHandle,eax
                        invoke  WriteFile,FHandle,MemPos,FSize,addr FRealWritten,0
                        invoke  CloseHandle,FHandle
Error_SaveFile:         mov     eax,FRealWritten
                        ret
SaveFile                endp
