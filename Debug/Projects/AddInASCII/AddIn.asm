; ------------------------------------------
; Title: AddinASCII
; 
; Author: Franck Charlet
;
; Notes: Simple AddIn
;
; Version: 1.2
; Lastest revision: 31/05/2010
; ------------------------------------------

; --------------- Procedures prototypes
DisplayWindow           proto
ASCIIInitProc           proto   :dword
ASCIIWinHook            proto   :dword,:dword,:dword,:dword
ConstructASCIIList      proto
AddAsciiEntry           proto   :dword,:dword
PasteASCII              proto
SetFontText             proto

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
                        ; Request an augmented lifespan
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
                        .if FlagActive != 0
                                LIBINVOKE DockingBoxRemove,hWndASCIISel
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
                                LIBINVOKE CreateDockingBox,addr MsgTitle,addr ASCIIWinHook
                                mov     hWndASCIISel,eax
                                test    eax,eax
                                jz      ErrorWindow
                                invoke  ASCIIInitProc,eax
                        .endif
                        LIBINVOKE DockingBoxShow,hWndASCIISel,300,230,DOCKINGBOX_RIGHT
                        mov     FlagActive,TRUE
                        or      eax,-1
ErrorWindow:            ret
DisplayWindow           endp

; --- Initialize window
ASCIIInitProc           proc    hWnd:dword
                        pushad
                        LIBINVOKE CreateListView,1,2,325,240,hWnd,0,0,0,LVS_EX_FULLROWSELECT Or LVS_EX_LABELTIP,LVS_REPORT Or LVS_SINGLESEL Or WS_TABSTOP Or LVS_NOSORTHEADER,WS_EX_STATICEDGE
                        mov     hASCIIList,eax
                        LIBINVOKE CreateButton,327,2,77,23,hWnd,addr MsgPaste,0,0,0,0,BS_DEFPUSHBUTTON Or WS_GROUP Or WS_TABSTOP,0
                        mov     hASCIICmdPaste,eax
                        LIBINVOKE CreateButton,327,26,77,23,hWnd,addr MsgFont,2,0,0,0,WS_TABSTOP,0
                        mov     hASCIICmdFont,eax
                        LIBINVOKE CreateLabel,2,244,324,16,hWnd,addr MsgEmpty,0,0,0,0
                        mov     hASCIIFontLabel,eax
                        LIBINVOKE CreateLabel,328,52,77,16,hWnd,addr MsgPasteDatas,0,0,0,0
                        mov     hASCIIColumnLabel,eax
                        LIBINVOKE CreateComboBox,328,66,77,150,hWnd,addr MsgEmpty,4,0,WS_TABSTOP Or CBS_DROPDOWNLIST
                        mov     hASCIICbPasteDatas,eax
                        LIBINVOKE ComboBoxAddItem,hASCIICbPasteDatas,addr MsgColChar,-1
                        LIBINVOKE ComboBoxAddItem,hASCIICbPasteDatas,addr MsgColDec,-1
                        LIBINVOKE ComboBoxAddItem,hASCIICbPasteDatas,addr MsgColHex,-1
                        LIBINVOKE ComboBoxAddItem,hASCIICbPasteDatas,addr MsgColBin,-1
                        LIBINVOKE ComboBoxAddItem,hASCIICbPasteDatas,addr MsgColOct,-1
                        LIBINVOKE ComboBoxAddItem,hASCIICbPasteDatas,addr MsgColName,-1
                        LIBINVOKE ComboBoxAddItem,hASCIICbPasteDatas,addr MsgColCtrl,-1
                        LIBINVOKE ComboBoxSetIndex,hASCIICbPasteDatas,ASCIIType
                        .if ASCIIFontName == 0
                                mov     eax,offset MsgCourierNew
                                mov     ASCIIFontName,eax
                        .endif
                        .if ASCIIFontSize == 0
                                mov     ASCIIFontSize,9
                        .endif
                        LIBINVOKE GDIObtainFont,ASCIIFontName,ASCIIFontSize,hWnd,0,0
                        mov     hASCIIFont,eax
                        invoke  SetFontText
                        LIBINVOKE ListViewAddCol,hASCIIList,addr MsgColCtrl,46,6
                        LIBINVOKE ListViewAddCol,hASCIIList,addr MsgColName,46,5
                        LIBINVOKE ListViewAddCol,hASCIIList,addr MsgColOct,46,4
                        LIBINVOKE ListViewAddCol,hASCIIList,addr MsgColBin,74,3
                        LIBINVOKE ListViewAddCol,hASCIIList,addr MsgColHex,46,2
                        LIBINVOKE ListViewAddCol,hASCIIList,addr MsgColDec,46,1
                        LIBINVOKE ListViewAddCol,hASCIIList,addr MsgColChar,46,0
                        LIBINVOKE ControlSetFont,hASCIIList,hASCIIFont
                        invoke  ConstructASCIIList
                        LIBINVOKE ListViewSetItemSel,hASCIIList,0
                        mov     FlagActive,1
                        popad
                        ret
ASCIIInitProc           endp

; --- Fill the listview with ascii chars infos
ConstructASCIIList      proc
                        mov     ASCIICount,0
                        invoke  AddAsciiEntry,addr MsgNUL,addr MsgCtrl0
                        invoke  AddAsciiEntry,addr MsgSOH,addr MsgCtrl1
                        invoke  AddAsciiEntry,addr MsgSTX,addr MsgCtrl2
                        invoke  AddAsciiEntry,addr MsgETX,addr MsgCtrl3
                        invoke  AddAsciiEntry,addr MsgEOT,addr MsgCtrl4
                        invoke  AddAsciiEntry,addr MsgENQ,addr MsgCtrl5
                        invoke  AddAsciiEntry,addr MsgACK,addr MsgCtrl6
                        invoke  AddAsciiEntry,addr MsgBEL,addr MsgCtrl7
                        invoke  AddAsciiEntry,addr MsgBS,addr MsgCtrl8
                        invoke  AddAsciiEntry,addr MsgHT,addr MsgCtrl9
                        invoke  AddAsciiEntry,addr MsgLF,addr MsgCtrl10
                        invoke  AddAsciiEntry,addr MsgVT,addr MsgCtrl11
                        invoke  AddAsciiEntry,addr MsgFF,addr MsgCtrl12
                        invoke  AddAsciiEntry,addr MsgCR,addr MsgCtrl13
                        invoke  AddAsciiEntry,addr MsgSO,addr MsgCtrl14
                        invoke  AddAsciiEntry,addr MsgSI,addr MsgCtrl15
                        invoke  AddAsciiEntry,addr MsgDLE,addr MsgCtrl16
                        invoke  AddAsciiEntry,addr MsgDC1,addr MsgCtrl17
                        invoke  AddAsciiEntry,addr MsgDC2,addr MsgCtrl18
                        invoke  AddAsciiEntry,addr MsgDC3,addr MsgCtrl19
                        invoke  AddAsciiEntry,addr MsgDC4,addr MsgCtrl20
                        invoke  AddAsciiEntry,addr MsgNAK,addr MsgCtrl21
                        invoke  AddAsciiEntry,addr MsgSYN,addr MsgCtrl22
                        invoke  AddAsciiEntry,addr MsgETB,addr MsgCtrl23
                        invoke  AddAsciiEntry,addr MsgCAN,addr MsgCtrl24
                        invoke  AddAsciiEntry,addr MsgEM,addr MsgCtrl25
                        invoke  AddAsciiEntry,addr MsgSUB,addr MsgCtrl26
                        invoke  AddAsciiEntry,addr MsgESC,addr MsgCtrl27
                        invoke  AddAsciiEntry,addr MsgFS,addr MsgCtrl28
                        invoke  AddAsciiEntry,addr MsgGS,addr MsgCtrl29
                        invoke  AddAsciiEntry,addr MsgRS,addr MsgCtrl30
                        invoke  AddAsciiEntry,addr MsgSp,addr MsgEmpty
                        mov     ecx,33
ConstructTable:         push    ecx
                        invoke  AddAsciiEntry,addr MsgEmpty,addr MsgEmpty
                        pop     ecx
                        inc     ecx
                        cmp     ecx,256+1
                        jne     ConstructTable
                        ret
ConstructASCIIList      endp

; --- Add an ASCII entry into listview
AddAsciiEntry           proc    StrNam:dword,StrControl:dword
                        local   ListCount:dword

                        LIBINVOKE StringNumberComplement,ASCIICount,3
                        push    eax
                        LIBINVOKE StringHexNumberComplement,ASCIICount,2
                        push    eax
                        LIBINVOKE StringBinNumberComplement,ASCIICount,8
                        push    eax
                        LIBINVOKE StringOctNumberComplement,ASCIICount,3
                        push    eax

                        LIBINVOKE ListViewItemCount,hASCIIList
                        mov     ListCount,eax
                        lea     ebx,MsgSpace
                        mov     eax,ASCIICount
                        mov     [ebx],al
                        LIBINVOKE ListViewAddItem,hASCIIList,ebx,ListCount, -1
                        ; oct
                        pop     eax
                        LIBINVOKE ListViewSetSubItem,hASCIIList,eax,ListCount,4
                        ; bin
                        pop     eax
                        LIBINVOKE ListViewSetSubItem,hASCIIList,eax,ListCount,3
                        ; hex
                        pop     eax
                        LIBINVOKE ListViewSetSubItem,hASCIIList,eax,ListCount,2
                        ; dec
                        pop     eax
                        LIBINVOKE ListViewSetSubItem,hASCIIList,eax,ListCount,1
                        LIBINVOKE ListViewSetSubItem,hASCIIList,StrNam,ListCount,5
                        LIBINVOKE ListViewSetSubItem,hASCIIList,StrControl,ListCount,6
                        inc     ASCIICount
                        ret
AddAsciiEntry           endp

; --- Window proc
ASCIIWinHook            proc    hWnd:dword,uMsg:dword,wParam:dword,lParam:dword
                        local   PaintS:PAINTSTRUCT
                        local   WinWidth:dword
                        local   WinHeight:dword
                        pushad
                        .if uMsg == WM_SYSCOLORCHANGE
                                invoke  GetSysColor,COLOR_WINDOW
                                LIBINVOKE ListViewSetBackColor,hASCIIList,eax
                        .elseif uMsg == WM_COMMAND
                                mov     eax,lParam
                                .if eax == hASCIICmdPaste
                                        invoke  PasteASCII
                                        popad
                                        xor     eax,eax
                                        ret
                                .elseif eax == hASCIICbPasteDatas
                                        LIBINVOKE ControlGetNotifiedCommand,wParam
                                        .if eax == CBN_SELCHANGE
                                                LIBINVOKE ComboBoxGetIndex,hASCIICbPasteDatas
                                                mov     ASCIIType,eax
                                        .endif
                                        popad
                                        xor     eax,eax
                                        ret
                                .elseif eax == hASCIICmdFont
                                        LIBINVOKE ComDlgChooseFont,hWnd,ASCIIFontName,ASCIIFontSize,1,addr AddInLogFont,addr AddInChooseFont
                                        .if eax != 0
                                                invoke  lstrlen,offset AddInLogFont.lfFaceName
                                                .if eax != 0
                                                        mov     eax,offset AddInLogFont.lfFaceName
                                                        mov     ASCIIFontName,eax
                                                        mov     eax,AddInChooseFont.iPointSize
                                                        xor     edx,edx
                                                        mov     ebx,10
                                                        idiv    ebx
                                                        mov     ASCIIFontSize,eax
                                                        .if hASCIIFont != 0
                                                                invoke  DeleteObject,hASCIIFont
                                                        .endif
                                                        LIBINVOKE GDIObtainFont,ASCIIFontName,ASCIIFontSize,hWnd,0,0
                                                        mov     hASCIIFont,eax
                                                        invoke  SetFontText
                                                        LIBINVOKE ListViewSetItemSel,hASCIIList,0
                                                        LIBINVOKE ListViewSetScroll,hASCIIList,0,-10000
                                                        LIBINVOKE ControlSetFont,hASCIIList,hASCIIFont
                                                        invoke  SetFocus,hASCIIList
                                                .endif
                                        .endif
                                        popad
                                        xor     eax,eax
                                        ret
                                .elseif eax == hASCIIList
                                        LIBINVOKE ControlGetNotifiedCommand,wParam
                                        .if eax == LBN_DBLCLK
                                                invoke  PasteASCII
                                        .endif
                                        popad
                                        xor     eax,eax
                                        ret
                                .endif
                        .elseif uMsg == WM_NOTIFY
                                LIBINVOKE ControlGetNotifiedhWnd,lParam
                                .if eax == hASCIIList
                                        LIBINVOKE ControlGetNotifiedMsg,lParam
                                        .if eax == NM_DBLCLK
                                                invoke  PasteASCII
                                                popad
                                                xor     eax,eax
                                                ret
                                        .endif
                                        
                                .endif
                        .elseif uMsg == WM_PAINT
                                invoke  BeginPaint,hWnd,addr PaintS
                                LIBINVOKE ControlWidth,hWnd
                                mov     WinWidth,eax
                                LIBINVOKE ControlHeight,hWnd
                                mov     WinHeight,eax
                                mov     eax,WinWidth
                                sub     eax,80
                                mov     ebx,WinHeight
                                sub     ebx,2
                                push    eax
                                sub     ebx,17
                                push    ebx
                                invoke  MoveWindow,hASCIIList,0,2,eax,ebx,1
                                mov     eax,WinWidth
                                sub     eax,78
                                invoke  MoveWindow,hASCIICmdPaste,eax,2,77,23,1
                                mov     eax,WinWidth
                                sub     eax,78
                                invoke  MoveWindow,hASCIICmdFont,eax,26,77,23,1
                                mov     eax,WinWidth
                                sub     eax,78
                                invoke  MoveWindow,hASCIIColumnLabel,eax,51,77,16,1
                                mov     eax,WinWidth
                                sub     eax,78
                                invoke  MoveWindow,hASCIICbPasteDatas,eax,67,77,150,1
                                pop     ebx
                                pop     eax
                                add     ebx,5
                                invoke  MoveWindow,hASCIIFontLabel,2,ebx,eax,16,1

                                invoke EndPaint,hWnd,addr PaintS
                        ; Handle dockwindow messages
                        .elseif uMsg == DOCKINGBOX_MSG_QUERY_STATE
                                popad
                                mov     eax,FlagActive
                                ret
                        .elseif uMsg == WM_DESTROY || uMsg == DOCKINGBOX_MSG_CLOSE
                                .if hASCIIFont != 0
                                        invoke  DeleteObject,hASCIIFont
                                        mov hASCIIFont,0
                                .endif
                                mov     FlagActive,FALSE
                                LIBINVOKE AddInKillZombie,addr AddInUnLoad
                                popad
                                xor     eax,eax
                                ret
                        .endif
                        popad
                        LIBINVOKE ControlNextHook,hWnd,uMsg,wParam,lParam
                        ret
ASCIIWinHook            endp

; --- Paste selected entry into current Chromatic child window at current position
PasteASCII              proc
                        ; (Get up to date context)
                        LIBINVOKE GetContext,addr AddInContext
                        .if AddInContext.NbrChilds != 0 
                                LIBINVOKE ListViewGetFirstFocusItem,hASCIIList
                                LIBINVOKE ListViewGetItemText,hASCIIList,eax,ASCIIType
                                LIBINVOKE InsertText,AddInContext.hCurrentChild,eax
                                LIBINVOKE SetFocusInsideWindow,AddInContext.hCurrentChild
                        .else
                                LIBINVOKE MiscMsgBox,hWndASCIISel,addr MsgChildError,MB_ERROR
                        .endif
                        ret
PasteASCII              endp

; --- Prïnt the selected font name/size
SetFontText             proc
                        LIBINVOKE StringDecToString,ASCIIFontSize
                        LIBINVOKE StringCat,addr MsgOpBracket,eax
                        LIBINVOKE StringCat,eax,addr MsgClBracket
                        LIBINVOKE StringCat,ASCIIFontName,eax
                        LIBINVOKE ControlSetText,hASCIIFontLabel,eax
                        ret
SetFontText             endp

end LibMain
