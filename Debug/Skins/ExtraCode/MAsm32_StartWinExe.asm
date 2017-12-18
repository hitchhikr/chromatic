                .data

__CommandLine   dd      0
__hInstance     dd      0
__StartupInfo   STARTUPINFO <>
__EmptyCmdLine  db      0
__CommCtrls     INITCOMMONCONTROLSEX <>

                .code
WinMain         PROTO   :DWORD,:DWORD,:DWORD,:DWORD

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
                mov     __CommCtrls.dwSize, sizeof __CommCtrls
                mov     __CommCtrls.dwICC, ICC_LISTVIEW_CLASSES or ICC_TREEVIEW_CLASSES or ICC_WIN95_CLASSES or ICC_BAR_CLASSES or ICC_TAB_CLASSES or ICC_PROGRESS_CLASS or ICC_COOL_CLASSES or ICC_USEREX_CLASSES or ICC_INTERNET_CLASSES or ICC_PAGESCROLLER_CLASS or ICC_DATE_CLASSES or ICC_HOTKEY_CLASS or ICC_UPDOWN_CLASS or ICC_ANIMATE_CLASS
                invoke  InitCommonControlsEx, addr __CommCtrls
                .if eax == 0
                        invoke  InitCommonControls
                .endif
                mov     __StartupInfo.dwFlags,0
                invoke  GetStartupInfo,addr __StartupInfo
                mov     ebx,__StartupInfo.dwFlags
                and     ebx,STARTF_USESHOWWINDOW
                xor     ebx,ebx
                mov     bx,__StartupInfo.wShowWindow
                jnz     @F
                mov     ebx,SW_SHOWDEFAULT
@@:             invoke  WinMain,__hInstance,0,__CommandLine,ebx
                invoke  ExitProcess,eax
