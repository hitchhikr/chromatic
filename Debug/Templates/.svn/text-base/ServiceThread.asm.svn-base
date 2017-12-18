; Note: the produced file will auto-install or remove itself when run.
; To start it go to the service manager of Windows NT/2000/XP.

                        .code

; --------------- Perform tasks right before service effective creation
; Out: 0 = Stop install process
;      1 = Proceed with install
; ---------------
; ServicesDatabase variable is initialized
; ---------------
; (Tip: This routine can be used to display dialogs or whatever)
ServiceStart            proc
                        xor eax,eax
                        inc eax
                        ret
ServiceStart            endp

; --------------- Perform tasks right before service effective removal
; Out: 0 = Stop removal process
;      1 = Proceed with removal
; ---------------
; ServicesDatabase variable is initialized
; ServiceHandle variable is initialized
; ---------------
; (Tip: This routine can be used to display dialogs or whatever)
ServiceRemove           proc
                        xor	eax,eax
                        inc	eax
                        ret
ServiceRemove           endp

; --------------- Background Thread (infinite) of the service
ServiceThread           proc    param:dword
ThreadLoop:             invoke	Sleep,1
                        jmp     ThreadLoop
ServiceThread           endp
