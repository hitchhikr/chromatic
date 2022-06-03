; --- Try to resolve a hostname
; In: String containing hostname or IP Address
; Out: Dword containing IP address or 0
ResolveIP               proc    HostString:dword
                        ; --- Fill socket informations
                        invoke  inet_addr,HostString
                        inc     eax
                        jnz     AlreadyIP
                        ; --- Try to resolve hostname
                        invoke  gethostbyname,HostString
                        test    eax,eax                 ; Eax=hostent structure
                        jz      HostentError
                        mov     eax,[eax+12]            ; Get ip list address
                        mov     eax,[eax]               ; Get first pointer
                        mov     eax,[eax]               ; Get contained ip address
                        ret
HostentError:           inc     eax
AlreadyIP:              dec     eax
                        ret
ResolveIP               endp
