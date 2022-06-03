; --- Clear a memory array
ClearMem                proc    Mem:dword,Bytes:dword
                        push    ecx
                        push    edi
                        mov     edi,Mem
                        mov     ecx,Bytes
                        cld
                        xor     eax,eax
                        shr     ecx,2
                        rep     stosd
                        mov     ecx,Bytes
                        and     ecx,3
                        rep     stosb
                        pop     edi
                        pop     ecx
                        ret
ClearMem                endp
