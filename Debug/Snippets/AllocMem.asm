; --- AllocMem function
AllocMem                proc    MemSize:dword
                        mov     eax,MemSize
                        test    eax,eax
                        jz      NoAllocMem
                        invoke  GlobalAlloc,GMEM_FIXED+GMEM_ZEROINIT,eax
NoAllocMem:             ret
AllocMem                endp