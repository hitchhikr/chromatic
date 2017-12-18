; ------------------------------------------
; Title: AddinASCII
; 
; Author: Franck Charlet
;
; Notes: Simple AddIn
;
; Version: 1.1
; Lastest revision: 31/05/2010
; ------------------------------------------

; --------------- Procedures prototypes
DisplayWindow           proto
DateWinHook             proto   :dword,:dword,:dword,:dword
DateInitProc            proto   :dword
PasteDate               proto
GetDateFrm              proto   :dword
GetTimeFrm              proto   :dword

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
                        LIBINVOKE GetContext,addr AddInContext
                        mov     eax,AddInContext.ColdStart
                        test    eax,eax
                        jz      DisplayOnDemand
                        ; It's a coldstart so check if the window was displayed
                        ; in the previous session.
                        LIBINVOKE DockingBoxWasVisible,addr MsgTitle
                        test    eax,eax
                        jz      EndLoadSequence
DisplayOnDemand:        invoke  DisplayWindow
                        jz      EndLoadSequence
                        popad
                        ; Request for an augmented lifespan
                        mov     eax,ADDIN_PERSISTANT
                        ret
EndLoadSequence:        popad
                        mov     eax,ADDIN_FINISHED
                        ret
AddInLoad               endp

; --- AddInUnLoad()
; In: N/A
; Out: N/A
AddInUnLoad             proc
                        pushad
                        .if FlagActive != FALSE
                                LIBINVOKE DockingBoxRemove,hWndDateSel
                        .endif
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
                        invoke  DisplayWindow
                        jz      EndMenuSequence
                        popad
                        mov     eax,ADDIN_ZOMBIE
                        ret
EndMenuSequence:        popad
                        mov     eax,ADDIN_DIE
                        ret
AddInMenu               endp

; --- Create the window and display it
; In: N/A
; Out: return -1 on success
DisplayWindow           proc
                        LIBINVOKE GetContext,addr AddInContext
                        .if FlagActive == FALSE
                                LIBINVOKE CreateDockingBox,addr MsgTitle,addr DateWinHook
                                mov     hWndDateSel,eax
                                test    eax,eax
                                jz      ErrorWindow
                                invoke  DateInitProc,eax
                        .endif
                        LIBINVOKE DockingBoxShow,hWndDateSel,216,230,DOCKINGBOX_RIGHT
                        mov     FlagActive,TRUE
                        or      eax,-1
ErrorWindow:            ret
DisplayWindow           endp

; --- Initialize window
; Return -1 on success
DateInitProc            proc    hWnd:dword
                        pushad
                        LIBINVOKE GDIGetSerif
                        mov SerifFont,eax
                        LIBINVOKE CreateListBox,0,1,217,178,hWnd,0,0,0,WS_TABSTOP Or WS_HSCROLL,WS_EX_STATICEDGE
                        mov     hPropertiesLbFileExt,eax
                        LIBINVOKE GetIniFilePathName,INIFILE_DATES
                        mov     DatesIniFile,eax
                        xor     ecx,ecx
RetrieveFormats:        push    ecx
                        invoke  GetDateFrm,ecx
                        mov     ecx,[esp]
                        invoke  GetTimeFrm,ecx
                        invoke  lstrlen,UserDateFormat
                        push    eax
                        invoke  lstrlen,UserTimeFormat
                        pop     ebx
                        add     ebx,eax
                        jnz     ContinueFormatReading
                        pop     ecx
                        jmp     StopFormatReading
ContinueFormatReading:  LIBINVOKE StringReplace,UserTimeFormat,addr MsgFormatSep,addr MsgSpace,1,-1,CASE_SENSITIVE
                        push    eax
                        LIBINVOKE StringReplace,UserDateFormat,addr MsgFormatSep,addr MsgSpace,1,-1,CASE_SENSITIVE
                        LIBINVOKE DateGetFormat,eax
                        pop     ecx
                        push    eax
                        LIBINVOKE DateGetTimeFormat,ecx
                        pop     ecx
                        LIBINVOKE StringCat,ecx,eax
                        push    eax
                        LIBINVOKE GDIGetTextWidth,hPropertiesLbFileExt,SerifFont,eax
                        .if     MaxText < eax
                                mov     MaxText,eax
                        .endif
                        pop     eax
                        LIBINVOKE ListBoxAddItem,hPropertiesLbFileExt,eax,-1
                        pop     ecx
                        inc     ecx
                        cmp     ecx,1000
                        jne     RetrieveFormats
StopFormatReading:      LIBINVOKE ListBoxCount,hPropertiesLbFileExt
                        test    eax,eax
                        jz      ListIsEmpty
                        LIBINVOKE ListBoxSetIndex,hPropertiesLbFileExt,0
ListIsEmpty:            popad
                        ret
DateInitProc            endp

; --- Window proc
DateWinHook             proc    hWnd:dword,uMsg:dword,wParam:dword,lParam:dword
                        local   PaintS:PAINTSTRUCT
                        pushad
                        .if uMsg == WM_COMMAND
                                mov     eax,lParam
                                .if eax == hPropertiesLbFileExt
                                        LIBINVOKE ControlGetNotifiedCommand,wParam
                                        .if eax == LBN_DBLCLK
                                                invoke  PasteDate
                                        .endif
                                        popad
                                        xor     eax,eax
                                        ret
                                .endif
                        .elseif uMsg == WM_PAINT
                                invoke BeginPaint,hWnd,addr PaintS
                                LIBINVOKE ControlWidth,hWnd
                                mov ebx,eax
                                LIBINVOKE ControlHeight,hWnd
                                invoke MoveWindow,hPropertiesLbFileExt,0,0,ebx,eax,1
                                LIBINVOKE ListBoxSetHorzScrollWidth,hPropertiesLbFileExt,MaxText
                                invoke EndPaint,hWnd,addr PaintS
                        ; Handle dockwindow messages
                        .elseif uMsg == DOCKINGBOX_MSG_QUERY_STATE
                                popad
                                mov     eax,FlagActive
                                ret
                        .elseif uMsg == WM_DESTROY || uMsg == DOCKINGBOX_MSG_CLOSE
                                mov     FlagActive,FALSE
                                LIBINVOKE AddInKillZombie,addr AddInUnLoad
                                popad
                                xor     eax,eax
                                ret
                        .endif
                        popad
                        LIBINVOKE ControlNextHook,hWnd,uMsg,wParam,lParam
                        ret
DateWinHook             endp

; --- Paste the date in current text window
PasteDate               proc
                        local   CurrentLstIndex:dword
                        LIBINVOKE GetContext,addr AddInContext
                        .if AddInContext.NbrChilds != 0 
                                LIBINVOKE ListBoxGetSelItemIndex,hPropertiesLbFileExt
                                mov     CurrentLstIndex,eax
                                invoke  GetDateFrm,CurrentLstIndex
                                invoke  GetTimeFrm,CurrentLstIndex
                                invoke  lstrlen,UserDateFormat
                                push    eax
                                invoke  lstrlen,UserTimeFormat
                                pop     ebx
                                add     ebx,eax
                                jz      StopFormatPasting
StopFormatPasting:              LIBINVOKE StringReplace,UserTimeFormat,addr MsgFormatSep,addr MsgSpace,1,-1,CASE_SENSITIVE
                                push    eax
                                LIBINVOKE StringReplace,UserDateFormat,addr MsgFormatSep,addr MsgSpace,1,-1,CASE_SENSITIVE
                                LIBINVOKE DateGetFormat,eax
                                pop     ecx
                                push    eax
                                LIBINVOKE DateGetTimeFormat,ecx
                                pop     ecx
                                LIBINVOKE StringCat,ecx,eax
                                LIBINVOKE InsertText,AddInContext.hCurrentChild,eax
                                LIBINVOKE SetFocusInsideWindow,AddInContext.hCurrentChild
                        .else
                                LIBINVOKE MiscMsgBox,AddInContext.hMDI,addr MsgChildError,MB_ERROR
                        .endif
                        ret
PasteDate               endp

; --- Retrieve date format from dates.ini file
GetDateFrm              proc    FormatNumber:dword
                        LIBINVOKE StringNumberComplement,FormatNumber,3
                        LIBINVOKE StringCat,addr MsgDateFormat,eax
                        LIBINVOKE IniReadKey,addr MsgFormats,eax,DatesIniFile
                        mov     UserDateFormat,eax
                        ret
GetDateFrm              endp

; --- Retrieve time format from dates.ini file
GetTimeFrm              proc    FormatNumber:dword
                        LIBINVOKE StringNumberComplement,FormatNumber,3
                        LIBINVOKE StringCat,addr MsgTimeFormat,eax
                        LIBINVOKE IniReadKey,addr MsgFormats,eax,DatesIniFile
                        mov     UserTimeFormat,eax
                        ret
GetTimeFrm              endp

end LibMain
