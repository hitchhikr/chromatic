; --- FreeMem function
FreeMem                 proc    MemHandle:dword
                        mov     eax,MemHandle
                        test    eax,eax
                        jz      NoFreeMem
                        invoke  GlobalFree,eax
NoFreeMem:              ret
FreeMem                 endp
