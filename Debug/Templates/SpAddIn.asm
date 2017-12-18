; --------------- Code section
                        .code

; --------------- DLL's entry point
LibMain                 proc    hInstance:HINSTANCE,reason:dword,reserved1:dword
                        mov     eax,TRUE
                        ret
LibMain                 endp

; --------------- DLL's Functions

; --- SpAddInDescription()
; In: N/A
; Out: Can return description string in eax or 0
SpAddInDescription      proc
                        mov     eax,offset MsgDescription
                        ret
SpAddInDescription      endp

; --- SpAddInAuthor()
; In: N/A
; Out: Can return author string in eax or 0
SpAddInAuthor           proc
                        mov     eax,offset MsgAuthor
                        ret
SpAddInAuthor           endp

; --- SpAddInLoad()
; In: Pointer to CHROMATICLIB structure
; Out: Must return ADDIN_FINISHED or ADDIN_PERSISTANT in eax
SpAddInLoad             proc    CLStruct:dword
                        pushad
                        mov     eax,CLStruct
                        mov     ChromaticLib,eax
                        popad
                        mov     eax,ADDIN_FINISHED
                        ret
SpAddInLoad             endp

; --- SpAddInUnLoad()
; In: N/A
; Out: N/A
SpAddInUnLoad           proc
                        pushad
                        popad
                        ret
SpAddInUnLoad           endp

; --- SpAddInAskResource()
; In: Code number of asked resource
; Out: Return value in eax (depends on asked resource)
SpAddInAskResource      proc    AskedResourceNumber:dword
                        pushad
                        popad
                        ret
SpAddInAskResource      endp
