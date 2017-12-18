; ------------------------------------------
; Standard starter file for windows applications
; ------------------------------------------
; buildblock RELEASE
;   CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /LIBPATH:"[LIBDIR]" /SUBSYSTEM:WINDOWS "%1.obj"
; buildblockend
; buildblock DEBUG
;   CAPT [BINDIR]\ml.exe /Zd /Zi /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /DEBUG /DEBUGTYPE:CV /INCREMENTAL:NO /LIBPATH:"[LIBDIR]" /SUBSYSTEM:WINDOWS "%1.obj"
; buildblockend

; --------------- File model
                        .386
                        .model  flat,stdcall
                        option  casemap:none

; --------------- Includes
                        include \masm32\include\windows.inc
                        include \masm32\include\kernel32.inc
                        include \masm32\include\user32.inc
                        include \masm32\include\gdi32.inc

; --------------- Libraries
                        includelib \masm32\lib\kernel32.lib
                        includelib \masm32\lib\user32.lib
                        includelib \masm32\lib\gdi32.lib

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
