; ------------------------------------------
; Standard starter file for console applications
; ------------------------------------------
; buildblock RELEASE
;   CAPT [BINDIR]\goasm.exe "%2"
;   CAPT [BINDIR]\golink.exe /console "%1.obj" user32.dll kernel32.dll gdi32.dll
; buildblockend
; buildblock DEBUG
;   CAPT [BINDIR]\goasm.exe "%2"
;   CAPT [BINDIR]\golink.exe /console /debug coff "%1.obj" user32.dll kernel32.dll gdi32.dll
; buildblockend

                        data section

                        code section

start:                  push    0
                        call    ExitProcess
