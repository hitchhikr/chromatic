; --- Copy a memory array
CopyMem                 proc    Source:dword,Dest:dword,Bytes:dword
                        push    esi
                        push    edi
                        push    ecx
                        mov     esi,Source
                        mov     edi,Dest
                        mov     ecx,Bytes
                        cld
                        shr     ecx,2
                        rep     movsd
                        mov     ecx,Bytes
                        and     ecx,3
                        rep     movsb
                        pop     ecx
                        pop     edi
                        pop     esi
                        ret
CopyMem                 endp
