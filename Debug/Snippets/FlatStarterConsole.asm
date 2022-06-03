; ------------------------------------------
; Flat starter file for console applications
; Only one code section will be produced with Exec/Read/Write abilities
; Resulting smaller file and self modifiable code/datas.
; ------------------------------------------
; buildblock RELEASE
;   CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /MERGE:.data=.text /LIBPATH:"[LIBDIR]" /MERGE:.rdata=.text /SECTION:.text,ERW /IGNORE:4078 /SUBSYSTEM:CONSOLE "%1.obj"
; buildblockend
; buildblock DEBUG
;   CAPT [BINDIR]\ml.exe /Zd /Zi /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /DEBUG /DEBUGTYPE:CV /INCREMENTAL:NO /LIBPATH:"[LIBDIR]" /MERGE:.data=.text /MERGE:.rdata=.text /SECTION:.text,ERW /IGNORE:4078 /SUBSYSTEM:CONSOLE "%1.obj"
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
