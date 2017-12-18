; ------------------------------------
; Title: 
;
; Author: 
;
; Purpose: 
;
; Notes: 
;
; ------------------------------------

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

; --- AddInDescription()
; In: N/A
; Out: Can return description string in eax or 0
FilterDescription       proc
                        mov     eax,offset MsgDescription
                        ret
FilterDescription       endp

; --- AddInAuthor()
; In: N/A
; Out: Can return author string in eax or 0
FilterAuthor            proc
                        mov     eax,offset MsgAuthor
                        ret
FilterAuthor            endp

; --- FilterProc()
; In: Filename: Name of the file
;     File: pointer to a WAFILTERFILE containing file infos
;     CL: CHROMATICLIB support entry point
; Out: Must return File or 0
FilterProc              proc    FileName:dword,File:dword,CL:dword
                        pushad
                        popad
                        mov	    eax,File
                        ret
FilterProc              endp
