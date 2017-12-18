; ------------------------------------------
; Standard starter file for static libraries
; ------------------------------------------
; buildblock RELEASE
;   CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\lib.exe "%1.obj" /LIBPATH:"[LIBDIR]" /OUT:"%1.lib"
; buildblockend
; buildblock DEBUG
;   CAPT [BINDIR]\ml.exe /Zd /Zi /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\lib.exe "%1.obj" /LIBPATH:"[LIBDIR]" /DEBUGTYPE:CV /INCREMENTAL:NO /OUT:"%1.lib"
; buildblockend

; --------------- File model
                        .386
                        .model  flat,stdcall
                        option  casemap:none

; --------------- Includes
                        include \masm32\include\windows.inc
                        include \masm32\include\kernel32.inc
                        include \masm32\include\user32.inc
                        include \masm32\include\masm32.inc

; --------------- Libraries
                        includelib \masm32\lib\kernel32.lib
                        includelib \masm32\lib\user32.lib
                        includelib \masm32\lib\masm32.lib

; --------------- Datas section
                        .data

; --------------- Code section
                        .code

end
