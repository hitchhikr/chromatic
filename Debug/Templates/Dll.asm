; --------------- Code section
                        .code

; --------------- DLL's entry point
LibMain                 proc    hInstance:HINSTANCE,reason:dword,reserved1:dword
                        .if     reason == DLL_PROCESS_ATTACH
                        .elseif reason == DLL_PROCESS_DETACH
                        .elseif reason == DLL_THREAD_ATTACH
                        .elseif reason == DLL_THREAD_DETACH
                        .endif
                        mov     eax,TRUE
                        ret
LibMain                 endp

; --------------- DLL's Functions
