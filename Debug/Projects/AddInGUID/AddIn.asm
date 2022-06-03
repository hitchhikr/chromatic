; ------------------------------------------
; Title: AddInGUID
; 
; Author: Franck Charlet
;
; Notes: Simple AddIn
;
; Version: 1.0
; Lastest revision: 31/05/2010
; ------------------------------------------

; --------------- Procedures prototypes
ReplaceGUIDElement32    proto   :dword,:dword
ReplaceGUIDElement16    proto   :dword,:dword
ReplaceGUIDElement8     proto   :dword,:dword

; --------------- Code section
                        .code

; --------------- DLL's entry point
LibMain                 proc    hInstance:HINSTANCE,reason:dword,reserved1:dword
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
AddInLoad               proc    CLStruct:dword
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
                        LIBINVOKE GetContext,addr AddInContext
                        .if AddInContext.NbrChilds != 0
                                LIBINVOKE GetWindowLangGUID,AddInContext.hCurrentChild
                                mov     GUIDSkew,eax
                                LIBINVOKE StringLen,GUIDSkew
                                .if eax == 0
                                        LIBINVOKE MiscMsgBox,AddInContext.hMDI,addr MsgGUIDError,MB_ERROR
                                        jmp     NoForms
                                .endif
EmptyGUIDDef:                   LIBINVOKE MiscObtainGUID,addr GUIDToInsert
                                INVOKE  ReplaceGUIDElement32,GUIDToInsert.Data1,addr MsgGUIDPart1
                                movzx   eax,GUIDToInsert.Data2
                                INVOKE  ReplaceGUIDElement16,eax,addr MsgGUIDPart2
                                movzx   eax,GUIDToInsert.Data3
                                INVOKE  ReplaceGUIDElement16,eax,addr MsgGUIDPart3
                                movzx   eax,GUIDToInsert.Data4[0]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart4
                                movzx   eax,GUIDToInsert.Data4[1]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart5
                                movzx   eax,GUIDToInsert.Data4[2]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart6
                                movzx   eax,GUIDToInsert.Data4[3]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart7
                                movzx   eax,GUIDToInsert.Data4[4]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart8
                                movzx   eax,GUIDToInsert.Data4[5]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart9
                                movzx   eax,GUIDToInsert.Data4[6]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart10
                                movzx   eax,GUIDToInsert.Data4[7]
                                INVOKE  ReplaceGUIDElement8,eax,addr MsgGUIDPart11
                                LIBINVOKE InsertText,AddInContext.hCurrentChild,GUIDSkew
                        .else
                                LIBINVOKE MiscMsgBox,AddInContext.hMDI,addr MsgChildError,MB_ERROR
                        .endif
NoForms:                popad
                        mov     eax,ADDIN_DIE
                        ret
AddInMenu               endp

; --- ReplaceGUIDElement32()
; Replace a 32 bits GUID element in the GUID skeleton
ReplaceGUIDElement32    proc    GUIDDat:dword,MatchingPattern:dword
                        LIBINVOKE StringHex32,GUIDDat
                        LIBINVOKE StringReplace,GUIDSkew,MatchingPattern,eax,1,-1,CASE_SENSITIVE
                        mov     GUIDSkew,eax
                        ret
ReplaceGUIDElement32    endp


; --- ReplaceGUIDElement16()
; Replace a 16 bits GUID element in the GUID skeleton
ReplaceGUIDElement16    proc    GUIDDat:dword,MatchingPattern:dword
                        LIBINVOKE StringHex16,GUIDDat
                        LIBINVOKE StringReplace,GUIDSkew,MatchingPattern,eax,1,-1,CASE_SENSITIVE
                        mov     GUIDSkew,eax
                        ret
ReplaceGUIDElement16    endp

; --- ReplaceGUIDElement8()
; Replace a 8 bits GUID element in the GUID skeleton
ReplaceGUIDElement8     proc    GUIDDat:dword,MatchingPattern:dword
                        local   GUIDAsciiDat:dword
                        LIBINVOKE StringHex8,GUIDDat
                        LIBINVOKE StringReplace,GUIDSkew,MatchingPattern,eax,1,-1,CASE_SENSITIVE
                        mov     GUIDSkew,eax
                        ret
ReplaceGUIDElement8     endp

end LibMain
