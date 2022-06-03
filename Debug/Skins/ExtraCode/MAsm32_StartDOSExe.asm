                .data

__CommandLine   dd      0
__hInstance     dd      0
__EmptyCmdLine  db      0

                .code
Main            proto   :DWORD,:DWORD

start:          invoke  GetModuleHandle,0
                mov     __hInstance,eax
                invoke  GetCommandLine
                test    eax,eax
                jnz     @F
                lea     eax,__EmptyCmdLine
@@:             mov     bl,byte ptr [eax]
                cmp     bl,022h
                jne     __skip_quotes
__skip_quotes_inside:
                inc     eax
                mov     bl,byte ptr [eax]
                test    bl,bl
                jz      @F
                cmp     bl,022h
                jne     __skip_quotes_inside
@@:             mov     bl,byte ptr [eax]
                cmp     bl,022h
                jne     __done_quotes
                inc     eax
                jmp     __done_quotes
__skip_quotes:  mov     bl,byte ptr [eax]
                test    bl,bl
                jz      @F
                cmp     bl,020h
                je      __done_quotes
                inc     eax
                jmp     __skip_quotes
__done_quotes:  mov     bl,byte ptr [eax]
                test    bl,bl
                jz      @F
                cmp     bl,020h
                ja      @F
                inc     eax
                jmp     __done_quotes
@@:             mov     __CommandLine,eax
                invoke  Main,__hInstance,__CommandLine
                invoke  ExitProcess,0
