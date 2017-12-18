; ------------------------------------------
; Standard starter file for console applications
; ------------------------------------------
; buildblock RELEASE
;   CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /LIBPATH:"[LIBDIR]" /SUBSYSTEM:CONSOLE "%1.obj"
; buildblockend
; buildblock DEBUG
;   CAPT [BINDIR]\ml.exe /Zd /Zi /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /DEBUG /INCREMENTAL:NO /DEBUGTYPE:CV /LIBPATH:"[LIBDIR]" /SUBSYSTEM:CONSOLE "%1.obj"
; buildblockend

; --------------- File model
                        .386
                        .model  flat,stdcall
                        option  casemap:none

; --------------- Includes
                        include windows.inc
                        include kernel32.inc
                        include user32.inc
                        include masm32.inc

; --------------- Libraries
                        includelib kernel32.lib
                        includelib user32.lib
                        includelib masm32.lib

; --------------- Procedures declarations
Main                    proto

; --------------- Datas section
                        .data

; --------------- Code section
                        .code

; ---------------------
; --- Program start ---
; ---------------------
start:                  invoke  Main
                        invoke  ExitProcess,0

; --------------
; --- Main() ---
; --------------
Main                    proc
                        ret
Main                    endp

end start
