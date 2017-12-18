; ------------------------------------------
; Flat standard starter file for windows applications
; Only one code section will be produced with Exec/Read/Write abilities
; Resulting smaller file and self modifiable code/datas.
; ------------------------------------------
; buildblock RELEASE
;   CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /MERGE:.data=.text /LIBPATH:"[LIBDIR]" /MERGE:.rdata=.text /SECTION:.text,ERW /IGNORE:4078 /SUBSYSTEM:WINDOWS "%1.obj"
; buildblockend
; buildblock DEBUG
;   CAPT [BINDIR]\ml.exe /Zd /Zi /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /DEBUG /DEBUGTYPE:CV /INCREMENTAL:NO /LIBPATH:"[LIBDIR]" /MERGE:.data=.text /MERGE:.rdata=.text /SECTION:.text,ERW /IGNORE:4078 /SUBSYSTEM:WINDOWS "%1.obj"
; buildblockend

; --------------- File model
                        .386
                        .model  flat,stdcall
                        option  casemap:none

; --------------- Includes
                        include windows.inc
                        include kernel32.inc
                        include user32.inc
                        include gdi32.inc

; --------------- Libraries
                        includelib kernel32.lib
                        includelib user32.lib
                        includelib gdi32.lib

; --------------- Procedures declarations
WinMain                 proto   :dword,:dword,:dword,:dword

; --------------- Datas section
                        .data

; --------------- Code section
                        .code

; ---------------------
; --- Program start ---
; ---------------------
start:                  invoke  GetModuleHandle,NULL
                        push    eax
                        invoke  GetCommandLine
                        pop     ebx
                        invoke  WinMain,ebx,NULL,eax,SW_SHOWDEFAULT
                        invoke  ExitProcess,eax

; -----------------
; --- WinMain() ---
; -----------------
WinMain                 proc    hInstance:dword,hPrevInstance:dword,lpCmdLine:dword,nCmdShow:dword
                        xor     eax,eax
                        ret
WinMain                 endp

end start
