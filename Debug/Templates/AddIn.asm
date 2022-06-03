; --------------- Code section
                        .code

; --------------- DLL's entry point
LibMain                 proc	hInstance:HINSTANCE,reason:dword,reserved1:dword
                        mov     eax,TRUE
                        ret
LibMain                 endp

; --------------- DLL's Functions

; --- AddInDescription()
; In: N/A
; Out: Can return description string in eax or 0
AddInDescription        proc
                        mov     eax,offset MsgDescription
                        ret
AddInDescription        endp

; --- AddInAuthor()
; In: N/A
; Out: Can return author string in eax or 0
AddInAuthor             proc
                        mov     eax,offset MsgAuthor
                        ret
AddInAuthor             endp

; --- AddInLoad()
; In: Pointer to CHROMATICLIB structure
; Out: Must return ADDIN_FINISHED or ADDIN_PERSISTANT in eax
AddInLoad               proc	CLStruct:dword
                        pushad
                        mov     eax,CLStruct
                        mov     ChromaticLib,eax
                        popad
                        mov     eax,ADDIN_FINISHED
                        ret
AddInLoad               endp

; --- AddInUnLoad()
; In: N/A
; Out: N/A
AddInUnLoad             proc
                        pushad
                        popad
                        ret
AddInUnLoad             endp

; --- AddInMenu()
; In: Pointer to CHROMATICLIB structure
; Out: Must return ADDIN_DIE or ADDIN_ZOMBIE in eax
AddInMenu               proc    CLStruct:dword
                        pushad
                        mov     eax,CLStruct
                        mov     ChromaticLib,eax
                        popad
                        mov     eax,ADDIN_DIE
                        ret
AddInMenu               endp
