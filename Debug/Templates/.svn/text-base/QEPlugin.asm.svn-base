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

; --- QEPlugIn()
; hInst = instance handle
; hMain = main window handle
; hEd = rich edit control handle
; hTool = toolbar handle (Chromatic will send NULL)
; hStat = status bar handle (parts are slightly differents in Chromatic so beware)

QePlugIn                proc    hInst:dword,hMain:dword,hEd:dword,hTool:dword,hStat:dword
                        .if hTool == 0
                            ; Running inside Chromatic
                        .else
                            ; Running inside QEditor
                        .endif
                        ret
QePlugIn                endp
