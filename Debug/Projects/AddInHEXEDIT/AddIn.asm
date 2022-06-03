; ------------------------------------------
; Title: AddinHEXEDIT
; 
; Author: Franck Charlet
;
; Notes: Simple AddIn
; Very tiny but can edit/patch large files
; (up to 2 or 4Gbs i think)
;
; Version: 1.0
; Lastest revision: 05/05/2003
; ------------------------------------------

; --------------- Constants
MODE_BYTE		equ	0
MODE_WORD		equ	1
MODE_DWORD		equ	2

HEX_IDBASE		equ	4000
HEX_BYTEVIEW		equ	0
HEX_WORDVIEW		equ	1
HEX_DWORDVIEW		equ	2

HEXTB_OPENFILE		equ	1
HEXTB_EDITMODE		equ	2
HEXTB_GOTOADDR		equ	3
HEXTB_VIEWTYPE		equ	4

; --------------- Procedures prototypes
HexWinHook		proto	:dword,:dword,:dword,:dword
HexDumpHook		proto	:dword,:dword,:dword,:dword
HexInitProc		proto	:dword
WriteSingleHexLine	proto	:dword
WriteHexLine		proto	:dword,:dword
WriteLineToWindow	proto	:dword,:dword
CalcLines		proto
CopyMem			proto	:dword,:dword,:dword
ClearMem		proto	:dword,:dword
StoreHex2		proto
StoreHex4		proto
StoreHex8		proto
LoadFile		proto	:dword
OpenReadFile		proto	:dword
ReadFileBytes		proto	:dword
CloseOpenedFile		proto
ScrollDownComplete	proto	:dword
ScrollUpComplete	proto	:dword
DisplayPage		proto
PosCaret		proto	:dword,:dword
DisplayCaret		proto

; --------------- Code section
			.code

; --------------- DLL's entry point
LibMain			proc	hInstance:HINSTANCE,reason:dword,reserved1:dword
			mov	eax,TRUE
			ret
LibMain			endp

; --------------- DLL's Functions

; --- AddInDescription()
; In: N/A
; Out: Can return description string in eax or 0
AddInDescription	proc
			mov	eax,offset MsgDescription
			ret
AddInDescription	endp

; --- AddInAuthor()
; In: N/A
; Out: Can return author string in eax or 0
AddInAuthor		proc
			mov	eax,offset MsgAuthor
			ret
AddInAuthor		endp

; --- AddInLoad()
; In: Pointer to WALIB structure
; Out: Must return ADDIN_FINISHED or ADDIN_PERSISTANT in eax
AddInLoad		proc	WALIBStruct:dword
			pushad
			mov	eax,WALIBStruct
			mov	WAStructPtr,eax
			popad
			mov	eax,ADDIN_FINISHED
			ret
AddInLoad		endp

; --- AddInUnLoad()
; In: N/A
; Out: N/A
AddInUnLoad		proc
			pushad
			.if FlagActive != 0
				WALIBINVOKE WAControlClose,hWndHex
			.endif
			popad
			ret
AddInUnLoad		endp

; --- AddInMenu()
; In: Pointer to WALIB structure
; Out: Must return ADDIN_DIE or ADDIN_ZOMBIE in eax
AddInMenu		proc	WALIBStruct:dword
			pushad
			mov	eax,WALIBStruct
			mov	WAStructPtr,eax
			WALIBINVOKE WAMMGetContext,addr AddInContext
			.if FlagActive == 0
				invoke	LoadIcon,AddInContext.hInstance,MICO_CUBES + MICO_BASE
				WALIBINVOKE WACreateDialog,-1,-1,550,400,AddInContext.hMDI,0,eax,addr MsgDialogTitle,addr HexInitProc,addr HexWinHook,0,WS_SYSMENU OR WS_MINIMIZEBOX OR WS_MAXIMIZEBOX OR WS_SIZEBOX,SW_SHOWNORMAL
				mov	hWndHex,eax
			.else
				invoke	ShowWindow,hWndHex,SW_SHOWNORMAL
			.endif
			popad
			mov	eax,ADDIN_ZOMBIE
			ret
AddInMenu		endp

; --- Initialize window --- '
HexInitProc		proc	hWnd:dword
			pushad
			mov	LinesLen,16
			xor	eax,eax
			mov	FileLen,eax
			mov	CurrentY,eax
			mov	OldCurrentY,eax
			mov	RawOffset,eax
			invoke	CalcLines
			invoke	GetSysColor,COLOR_WINDOWTEXT
    			mov	HexFrontColor,eax
			invoke	GetSysColor,COLOR_WINDOW
    			mov	HexBackColor,eax
    			WALIBINVOKE WAGDIObtainFont,addr MsgFontName,9,hWnd,0,0
    			mov	hHexFont,eax
    			WALIBINVOKE WAGDIGetFontWidth,hWnd,hHexFont
    			mov	CharWidth,eax
    			WALIBINVOKE WAGDIGetFontHeight,hWnd,hHexFont
    			mov	CharHeight,eax
    			WALIBINVOKE WACreateDumpBox,-1, -1, -1, -1,hWnd,0,hHexFont,0,MaxLines,10,10,addr HexDumpHook,WS_HSCROLL OR WS_VSCROLL,WS_EX_STATICEDGE
			mov	hHexDump,eax
			WALIBINVOKE WACreateStatusBar,addr FileName,WASTATUSBAR_PARTNORMAL,hWnd,1,0,WS_BORDER
			mov	hHexStatus,eax
			WALIBINVOKE WAControlClientWidth,hWnd
			WALIBINVOKE WACreateToolBar,0,0,550,23,hWnd,AddInContext.hMainImageList,4,-1,TBSTYLE_TOOLTIPS OR CCS_NORESIZE OR TBSTYLE_FLAT OR TBS_FIXEDLENGTH,TBSTYLE_EX_DRAWDDARROWS
            		mov	hHexToolbar,eax
            		WALIBINVOKE WAToolBarAddButton,hHexToolbar,addr MsgEmpty,HEXTB_OPENFILE,MICO_OPEN,TBSTYLE_BUTTON,TBSTATE_ENABLED,1
			WALIBINVOKE WAToolBarAddSeparator,hHexToolbar,0
            		WALIBINVOKE WAToolBarAddButton,hHexToolbar,addr MsgEmpty,HEXTB_EDITMODE,MICO_EDITDOC,TBSTYLE_CHECK,TBSTATE_ENABLED,1
            		WALIBINVOKE WAToolBarAddButton,hHexToolbar,addr MsgEmpty,HEXTB_GOTOADDR,MICO_STACK,TBSTYLE_BUTTON,TBSTATE_ENABLED,1
            		WALIBINVOKE WAToolBarAddButton,hHexToolbar,addr MsgEmpty,HEXTB_VIEWTYPE,MICO_FIND,TBSTYLE_DROPDOWN OR TBSTYLE_AUTOSIZE,TBSTATE_ENABLED,1
            		invoke	CreatePopupMenu
			mov	hHexViewMenu,eax
            		invoke	AppendMenu,hHexViewMenu,MF_STRING,HEX_IDBASE OR HEX_BYTEVIEW,addr MsgByteView
            		WALIBINVOKE WAMenuSetDefaultItem,hHexViewMenu,0
            		invoke	AppendMenu,hHexViewMenu,MF_STRING,HEX_IDBASE OR HEX_WORDVIEW, addr MsgWordView
            		invoke	AppendMenu,hHexViewMenu,MF_STRING,HEX_IDBASE OR HEX_DWORDVIEW, addr MsgDWordView
			mov	FlagActive,1
			WALIBINVOKE WAGDIFillWindow,hWnd,HexBackColor
			popad
			ret
HexInitProc		endp

; --- Window proc
HexWinHook		proc	hWnd:dword,uMsg:dword,wParam:dword,lParam:dword
            		LOCAL	WinRect:RECT
            		pushad
            		.if uMsg==WM_KEYDOWN
            			.if wParam==VK_UP
					.if	hFileHandle != 0
						mov	eax,CaretFilePosY
						sub	eax,16
						jl	MinScrollYKey
						sub	CaretFilePosY,16
						dec	CaretPosY
						mov	eax,CaretPosY
						test	eax,eax
						jge	MinCaretUp
						mov	CaretPosY,0
MinCaretUp:					invoke	PosCaret,CaretPosX,CaretPosY
		;				invoke	DisplayCaret
					.endif
MinScrollYKey:			.elseif wParam==VK_DOWN
					.if	hFileHandle != 0
						mov	eax,CaretFilePosY
				;		add	eax,16
				;		cmp	FileLen,eax
				;		jg	MaxScrollYKey
						add	CaretFilePosY,16
						inc	CaretPosY
				;		WALIBINVOKE WADumpBoxGetLines,hHexDump,CharHeight
				;		mov	ebx,CaretPosY
				;		cmp	ebx,eax
				;		jle	MinCaretDown
				;		mov	CaretPosY,ebx
MinCaretDown:					invoke	PosCaret,CaretPosX,CaretPosY
				;		invoke	DisplayCaret
					.endif
MaxScrollYKey: 			.elseif wParam==VK_LEFT
					.if	hFileHandle != 0
						dec	CaretPosX
						mov	eax,CaretPosX
						test	eax,eax
						jge	MinCaretLeft
						mov	CaretPosX,0
						mov	eax,CaretFilePosY
						sub	eax,16
						jl	MinCaretLeft
						; Jump to previous line
						mov	CaretPosX,47
						invoke	SendMessage,hWnd,WM_KEYDOWN,VK_UP,0
						popad
						xor	eax,eax
						ret
MinCaretLeft:					invoke	PosCaret,CaretPosX,CaretPosY
			;			invoke	DisplayCaret
MinScrollXKey:				.endif
            			.elseif wParam==VK_RIGHT
					.if	hFileHandle != 0
						mov	eax,CaretPosX
						inc	eax
						cmp	FileLen,eax
						jl	MaxScrollXKey
						inc	CaretPosX
						cmp	CaretPosX,47
						jle	MaxCaretRight
						; Jump to next line
						mov	CaretPosX,0
						invoke	SendMessage,hWnd,WM_KEYDOWN,VK_DOWN,0
						popad
						xor	eax,eax
						ret
MaxCaretRight:					invoke	PosCaret,CaretPosX,CaretPosY
			;			invoke	DisplayCaret
MaxScrollXKey:				.endif
            			.elseif wParam==VK_PGUP
				.elseif wParam==VK_PGDN
				.endif

			.elseif uMsg==WADUMPBOX_LINEUP
				mov	eax,CurrentY
				test	eax,eax
				jle	MinScrollY
				dec	CurrentY
				mov	eax,CurrentY 
				imul	eax,LinesLen
				mov	RawOffset,eax
				WALIBINVOKE WADumpBoxScrollUp,hHexDump,1,CharHeight,1
				invoke	WriteSingleHexLine,0
MinScrollY:			invoke	DisplayCaret
				mov	eax,CurrentY
				mov	OldCurrentY,eax
				popad
				mov	eax,CurrentY
				ret

			.elseif uMsg==WADUMPBOX_LINEDOWN
				mov	eax,CurrentY
				mov	ebx,MaxLines
				sub	ebx,9
				cmp	eax,ebx
				jge	MaxScrollY
				inc	CurrentY
				mov	eax,CurrentY 
				imul	eax,LinesLen
				mov	RawOffset,eax
				WALIBINVOKE WADumpBoxScrollDown,hHexDump,1,CharHeight,1
				WALIBINVOKE WADumpBoxGetVisibleLines,hHexDump,CharHeight,1
                		dec	eax
				invoke	WriteSingleHexLine,eax			
MaxScrollY:			invoke	DisplayCaret
				mov	eax,CurrentY
				mov	OldCurrentY,eax
				popad
				mov	eax,CurrentY
				ret	

			.elseif uMsg==WADUMPBOX_PAGEUP
				WALIBINVOKE WAScrollBarGetPageRange,hHexDump,SB_VERT
				sub	CurrentY,eax
				cmp	CurrentY,0
				jge	MinPageY
				mov	CurrentY,0
MinPageY:			mov	eax,CurrentY
				imul	eax,LinesLen
				mov	RawOffset,eax
				invoke	ScrollUpComplete,9
				invoke	DisplayCaret
				mov	eax,CurrentY
				mov	OldCurrentY,eax
				popad
				mov	eax,CurrentY
				ret	

			.elseif uMsg==WADUMPBOX_PAGEDOWN
            			WALIBINVOKE WAScrollBarGetPageRange,hHexDump,SB_VERT
            			add	CurrentY,eax
				mov	ebx,MaxLines
				sub	ebx,9
				cmp	CurrentY,ebx
				jle	MaxPageY
				mov	CurrentY,ebx
MaxPageY:       		mov	eax,CurrentY 
				imul	eax,LinesLen
				mov	RawOffset,eax
				invoke	ScrollDownComplete,9
				invoke	DisplayCaret
				mov	eax,CurrentY
				mov	OldCurrentY,eax
				popad
				mov	eax,CurrentY
				ret

			.elseif uMsg==WADUMPBOX_TOP
            			mov	CurrentY,0
				mov	RawOffset,0
				invoke	DisplayPage
				mov	eax,CurrentY
				mov	OldCurrentY,eax
				popad
				mov	eax,CurrentY
				ret

			.elseif uMsg==WADUMPBOX_BOTTOM
				mov	eax,MaxLines
				sub	eax,9
				mov	CurrentY,eax
				mov	eax,CurrentY 
				imul	eax,LinesLen
				mov	RawOffset,eax
				invoke	DisplayPage
				mov	eax,CurrentY
				mov	OldCurrentY,eax
				popad
				mov	eax,CurrentY
				ret

			.elseif uMsg==WADUMPBOX_POSITIONUPDOWN
				mov	eax,lParam
				mov	CurrentY,eax
				imul	eax,LinesLen
				mov	RawOffset,eax
				mov	eax,OldCurrentY
				sub	eax,CurrentY
				jz	NoUpDownPosition
				jg	PositionDown
				invoke	ScrollDownComplete,9
				jmp	NoUpDownPosition
PositionDown:			invoke	ScrollUpComplete,9
NoUpDownPosition:		invoke	DisplayCaret
				mov	eax,CurrentY
				mov	OldCurrentY,eax
				popad
				mov	eax,CurrentY
				ret

			.elseif uMsg==WADUMPBOX_TRACKUPDOWN
				mov	eax,lParam
				mov	CurrentY,eax
				imul	eax,LinesLen
				mov	RawOffset,eax
				mov	eax,OldCurrentY
				sub	eax,CurrentY
				jz	NoUpDownTrack
				jg	TrackDown
				neg	eax
				invoke	ScrollDownComplete,eax
				jmp	NoUpDownTrack
TrackDown:			invoke	ScrollUpComplete,eax
NoUpDownTrack:			invoke	DisplayCaret
				mov	eax,CurrentY
				mov	OldCurrentY,eax
				popad
				mov	eax,CurrentY
				ret

			.elseif uMsg==WM_SIZE
				.if wParam!=SIZE_MINIMIZED
                			invoke	SendMessage,hHexStatus,uMsg,wParam,lParam
					WALIBINVOKE WAControlClientWidth,hWnd
					WALIBINVOKE WAControlResize,hHexToolbar,0,0,eax,23
					WALIBINVOKE WAControlClientHeight,hWnd
					mov	HexDumpHeight,eax
					WALIBINVOKE WAStatusBarGetYSize,hHexStatus
					sub	HexDumpHeight,eax
					WALIBINVOKE WAToolBarGetYSize,hHexToolbar
					sub	HexDumpHeight,eax
					mov	HexDumpY,eax
					WALIBINVOKE WAControlClientWidth,hWnd
					WALIBINVOKE WADumpBoxResize,hHexDump,0,HexDumpY,eax,HexDumpHeight
				.endif

			.elseif uMsg==WM_NOTIFY
				WALIBINVOKE WAControlGetNotifiedMsg,lParam
				.if	eax==TTN_NEEDTEXT
                    			WALIBINVOKE WAControlGetNotifiedID,lParam
					.if	eax==HEXTB_OPENFILE
                            			WALIBINVOKE WAToolBarDisplayToolTip,addr MsgOpenFile,lParam
						popad
						xor	eax,eax
						ret
					.elseif	eax==HEXTB_EDITMODE
                            			WALIBINVOKE WAToolBarDisplayToolTip,addr MsgEditMode,lParam
						popad
						xor	eax,eax
						ret
					.elseif	eax==HEXTB_GOTOADDR
                            			WALIBINVOKE WAToolBarDisplayToolTip,addr MsgGotoAddr,lParam
						popad
						xor	eax,eax
						ret
					.elseif	eax==HEXTB_VIEWTYPE
                            			WALIBINVOKE WAToolBarDisplayToolTip,addr MsgViewType,lParam
						popad
						xor	eax,eax
						ret
                    			.endif
				.elseif eax==TBN_DROPDOWN
                    			WALIBINVOKE WAToolBarGetNotifiedDropDownItem,lParam
                    			WALIBINVOKE WAToolBarDisplayPopupMenu,hHexToolbar,eax,hHexViewMenu,hWnd
					popad
					xor	eax,eax
					ret
				.endif

			.elseif uMsg==WM_COMMAND
            			mov	eax,lParam
            			.if eax==hHexToolbar
					.if wParam==HEXTB_OPENFILE
						WALIBINVOKE WAMMDisplayOpenFileSelector,hWnd,addr MsgFilters,addr MsgEmpty,0
						test	eax,eax
						jz	NoFileToOpen
						invoke	LoadFile,eax
NoFileToOpen:					popad
						xor	eax,eax
						ret
					.endif
					.if wParam==HEXTB_VIEWTYPE
						popad
						xor	eax,eax
						ret
					.endif
				.endif
			.elseif	uMsg==WM_SETFOCUS
				invoke	DisplayCaret
			.elseif	uMsg==WM_KILLFOCUS
				cmp	CaretOn,0
				je	HiddenCaret
				invoke	DestroyCaret
HiddenCaret:			mov	CaretOn,0
			.elseif	uMsg==WM_ERASEBKGND
				WALIBINVOKE WAFrameRefreshToolbar,hHexToolbar
				popad
				xor	eax,eax
				ret
			.elseif	uMsg==WM_PAINT
				invoke	BeginPaint,hWnd,addr HexPs
 				invoke	DisplayPage
            			invoke	EndPaint,hWnd,addr HexPs
				popad
				xor	eax,eax
				ret
			.elseif uMsg==WM_CLOSE
    				invoke	SetForegroundWindow,AddInContext.hMDI
    				invoke	SetActiveWindow,AddInContext.hMDI
			.elseif uMsg==WM_DESTROY
				mov	eax,hHexFont
				test	eax,eax
				jz	NoFreeFont
				invoke	DeleteObject,hHexFont
NoFreeFont:			mov	FlagActive,0
				invoke	CloseOpenedFile
				WALIBINVOKE WAMMAddInKillZombie,addr AddInUnLoad
				popad
				xor	eax,eax
				ret
			.endif
			popad
    			invoke	GetWindowLong,hWnd,GWL_USERDATA
    			invoke	CallWindowProc,eax,hWnd,uMsg,wParam,lParam
			ret
HexWinHook		endp

; --- HexEdit events
HexDumpHook		proc	hWnd:dword,uMsg:dword,wParam:dword,lParam:dword
            		pushad
			popad
    			invoke	GetWindowLong,hWnd,GWL_USERDATA
    			invoke	CallWindowProc,eax,hWnd,uMsg,wParam,lParam
			ret
HexDumpHook		endp

; --- Clear a memory array
ClearMem		proc	Mem:dword,Bytes:dword
			push	ecx
			push	edi
			mov	edi,Mem
			mov	ecx,Bytes
			cld
			mov	eax,020202020h
			shr	ecx,2
			rep	stosd
			mov	ecx,Bytes
			and	ecx,3
			rep	stosb
			pop	edi
			pop	ecx
			ret
ClearMem		endp

; --- Copy a memory array
CopyMem			proc	Source:dword,Dest:dword,Bytes:dword
			push	ecx
			push	esi
			push	edi
			mov	esi,Source
			mov	edi,Dest
			mov	ecx,Bytes
			cld
			shr	ecx,2
			rep	movsd
			mov	ecx,Bytes
			and	ecx,3
			rep	movsb
			pop	edi
			pop	esi
			pop	ecx
			ret
CopyMem			endp

; --- Store a byte in hexadecimal
StoreHex2		proc
			push	eax
			push	ebx
			movzx	ebx,al
			and	bl,0fh
			mov	bl,[HexTbl+ebx]
			mov	[edi+1],bl
			and	eax,0f0h
			shr	al,4
			mov	bl,[HexTbl+eax]
			mov	[edi],bl
			add	edi,2
			pop	ebx
			pop	eax
			ret
StoreHex2		endp

; --- Store a word in hexadecimal
StoreHex4		proc
			push	eax
			xchg	ah,al
			call	StoreHex2
			xchg	ah,al
			call	StoreHex2
			pop	eax
			ret
StoreHex4		endp

; --- Store a dword in hexadecimal
StoreHex8		proc
			bswap	eax
			push	eax
			bswap	eax
			xchg	al,ah
			call	StoreHex4
			pop	eax
			jmp	StoreHex4
StoreHex8		endp

; --- Write a single line in directly in main window
WriteSingleHexLine	proc	LineNumber:dword
			invoke	GetDC,hHexDump
			push	eax
			invoke	WriteHexLine,LineNumber,eax
			pop	eax
			invoke	ReleaseDC,hHexDump,eax
			ret
WriteSingleHexLine	endp

; --- Write a line in given hDC
WriteHexLine		proc	LineNumber:dword,hDC:dword
			local	LineOffset:dword
			mov	eax,LinesLen
    			imul	eax,LineNumber
    			add	eax,RawOffset
			mov	LineOffset,eax
			invoke	ClearMem,addr CurrentLineText,74
			lea	edi,offset CurrentLineText+10
			mov	esi,hFileHandle
			test	esi,esi
			jz	NoFileTodisplay
			invoke	ReadFileBytes,LineOffset
			test	eax,eax
			jz	NoFileTodisplay
			mov	esi,eax
			mov	eax,LineOffset
			mov	ecx,FileLen
			add	eax,16
			xor	ebx,ebx
			cmp	eax,ecx
			jl	MaxBytes
			sub	eax,ecx
			mov	ebx,eax
MaxBytes:		.if	CurrentDisplayMode==MODE_BYTE
				mov	ecx,16
				sub	ecx,ebx
				jle	EmptyLine
				push	ecx
				push	esi
				push	edi
StoreByteHexLine:		mov	al,[esi]
				inc	esi
				invoke	StoreHex2
				inc	edi
				loop	StoreByteHexLine
				pop	edi
				pop	esi
				pop	ecx
				lea	edi,[edi+48]
StoreByteAsciiLine:		movzx	eax,byte ptr [esi]
				inc	esi
				WALIBINVOKE WAStringFilterNonPrintableChars,eax,"."
				mov	[edi],al
				inc	edi
				loop	StoreByteAsciiLine
			.elseif	CurrentDisplayMode==MODE_WORD
				mov	ecx,8
				push	esi
				push	edi
StoreWordHexLine:		mov	ax,[esi]
				add	esi,2
				invoke	StoreHex4
				inc	edi
				loop	StoreByteHexLine
				pop	edi
				pop	esi
				mov	ecx,16
				lea	edi,[edi+48]
StoreWordAsciiLine:		movzx	eax,byte ptr [esi]
				inc	esi
				WALIBINVOKE WAStringFilterNonPrintableChars,eax,"."
				mov	[edi],al
				inc	edi
				loop	StoreWordAsciiLine
			.elseif	CurrentDisplayMode==MODE_DWORD
				mov	ecx,4
				push	esi
				push	edi
StoreDWordHexLine:		mov	eax,[esi]
				add	esi,4
				invoke	StoreHex8
				inc	edi
				loop	StoreDWordHexLine
				pop	edi
				pop	esi
				mov	ecx,16
				lea	edi,[edi+48]
StoreDWordAsciiLine:		movzx	eax,byte ptr [esi]
				inc	esi
				WALIBINVOKE WAStringFilterNonPrintableChars,eax,"."
				mov	[edi],al
				inc	edi
				loop	StoreDWordAsciiLine
			.endif
			; Pad address
			WALIBINVOKE WAStringHexNumberComplement,LineOffset,8
			mov	ebx,eax
    			invoke	CopyMem,ebx,addr CurrentLineText,8
			lea	edi,offset CurrentLineText+8
			mov	byte ptr [edi],":"
EmptyLine:		invoke	WriteLineToWindow,LineNumber,hDC
			WALIBINVOKE WAMiscFreeMem,FileMem
NoFileTodisplay:	ret
WriteHexLine		endp

; --- Write a line in the dumpbox
WriteLineToWindow	proc	LineNumber:dword,hDC:dword
    			mov	eax,LineNumber
    			imul	eax,CharHeight
    			WALIBINVOKE WAGDIWriteText,hDC,1,eax,addr CurrentLineText,HexFrontColor,hHexFont,0,HexBackColor
			ret
WriteLineToWindow	endp

; --- Calculate the number of required lines 
CalcLines		proc
			mov	eax,FileLen
			xor	edx,edx
			mov	ebx,LinesLen
			idiv	ebx
			add	eax,8
			mov	MaxLines,eax
			mov	LinesRemainder,edx
			test	edx,edx
			jz	AddLastLine
			inc	MaxLines
AddLastLine:		ret
CalcLines		endp

; --- Scroll down and fill the gap
ScrollDownComplete	proc	Lines:dword
			mov	eax,Lines
			push	eax
			WALIBINVOKE WADumpBoxScrollDown,hHexDump,eax,CharHeight,1
			WALIBINVOKE WADumpBoxGetVisibleLines,hHexDump,CharHeight,1
                	dec	eax
			pop	ecx
CompletePositionDown:	push	ecx
			push	eax
			invoke	WriteSingleHexLine,eax			
			pop	eax
			pop	ecx
			dec	eax
			dec	ecx
			jnz	CompletePositionDown
PositionDown:		ret
ScrollDownComplete	endp

; --- Scroll up and fill the gap
ScrollUpComplete	proc	Lines:dword
			mov	eax,Lines
			push	eax
			WALIBINVOKE WADumpBoxScrollUp,hHexDump,eax,CharHeight,1
			WALIBINVOKE WADumpBoxGetVisibleLines,hHexDump,CharHeight,1
			pop	ecx
			cmp	eax,ecx
			jg	MaxTopLines
			mov	ecx,eax
MaxTopLines:		xor	eax,eax
CompletePositionUp:	push	ecx
			push	eax
			invoke	WriteSingleHexLine,eax		
			pop	eax
			pop	ecx
			inc	eax
			dec	ecx
			jnz	CompletePositionUp
NoUpDownPosition:	ret
ScrollUpComplete	endp

; --- Display a page
DisplayPage		proc
			local	PagehDC:dword
			mov	eax,hHexDump
			mov	BackDC.hWnd,eax
			mov	eax,HexBackColor
			mov	BackDC.Color,eax
    			WALIBINVOKE WAGDICreateBackDC,addr BackDC
    			mov	eax,BackDC.hDC
    			mov	PagehDC,eax
    			; Get the number of lines to display
    			WALIBINVOKE WADumpBoxGetVisibleLines,hHexDump,CharHeight,1
			mov	ecx,eax
			xor	eax,eax
			test	ecx,ecx
			jz	NoRepaint
DisplayHexLines:	push	eax
			push	ecx
        		invoke	WriteHexLine,eax,PagehDC
			pop	ecx
			pop	eax
			inc	eax
			loop	DisplayHexLines
			invoke	GetClientRect,hHexDump,addr BackRECT
			WALIBINVOKE WAGDIBlitBackDC,addr BackRECT,addr BackDC
    			WALIBINVOKE WAGDIDestroyBackDC,addr BackDC
			invoke	DisplayCaret
			ret
NoRepaint:		invoke	GetClientRect,hHexDump,addr BackRECT
			WALIBINVOKE WAGDIBlitBackDC,addr BackRECT,addr BackDC
    			WALIBINVOKE WAGDIDestroyBackDC,addr BackDC
			ret
DisplayPage		endp

; --- Position the caret
PosCaret		proc	PosX:dword,PosY:dword
			mov	ebx,PosX
			movzx	eax,[CaretXPosBytes+ebx]
			imul	eax,CharWidth
			mov	ebx,PosY
			imul	ebx,CharHeight
			inc	eax
			invoke	SetCaretPos,eax,ebx
			ret
PosCaret		endp

; --- Obtain and display a caret
DisplayCaret		proc
			mov	eax,CaretPosY
			.if	eax >= CurrentY
				;WALIBINVOKE WADumpBoxGetLines,hHexDump,CharHeight
				;mov	ebx,CaretPosY
				.if	hFileHandle != 0
					cmp	CaretOn,0
					je	SetCaretOff
					;invoke	HideCaret,hHexDump
					invoke	DestroyCaret
SetCaretOff:				invoke	CreateCaret,hHexDump,0,CharWidth,CharHeight
					invoke	PosCaret,CaretPosX,CaretPosY
					invoke	ShowCaret,hHexDump
					mov	CaretOn,1
				.endif
			.endif
			ret
DisplayCaret		endp

; ------------------------------------------
; Handling of files
; ------------------------------------------

; --- Open a file for reading
OpenReadFile		proc	FName:dword
			invoke	CreateFile,FName,GENERIC_READ,FILE_SHARE_WRITE,0,OPEN_EXISTING,FILE_FLAG_SEQUENTIAL_SCAN,0
			inc	eax
			jz	Error_LoadFile
			dec	eax
Error_LoadFile:		ret
OpenReadFile		endp

; --- Read bytes from file
ReadFileBytes		proc	PosInFile:dword
			local	BytesRead:dword
			mov	FileMem,0
			mov	eax,hFileHandle
			test	eax,eax
			jz	NoFileToRead
			invoke	SetFilePointer,hFileHandle,PosInFile,0,FILE_BEGIN
			WALIBINVOKE WAMiscAllocMem,LinesLen
			mov	FileMem,eax
			test	eax,eax
			jz	CantAlloc
			mov	ebx,eax
			invoke	ReadFile,hFileHandle,ebx,LinesLen,addr BytesRead,0
CantAlloc:		mov	eax,FileMem
NoFileToRead:		ret
ReadFileBytes		endp

; --- Close the opened file handle
CloseOpenedFile		proc
			mov	eax,hFileHandle
			test	eax,eax
			jz	NoOlderFile
			invoke	CloseHandle,eax
			mov	hFileHandle,0
NoOlderFile:		ret
CloseOpenedFile		endp

; --- Load a file
LoadFile		proc	RFileName:dword
			invoke	CloseOpenedFile
			invoke	OpenReadFile,RFileName
			test	eax,eax
			jnz	FileOpened
			WALIBINVOKE WAMiscMsgBox,hWndHex,addr MsgErrOpen,MB_ERROR
			ret
FileOpened:		mov	hFileHandle,eax
			invoke	GetFileSize,eax,addr FileLenHigh
			mov	FileLen,eax
			; Reset vars
			xor	eax,eax
			mov	CurrentY,eax
			mov	OldCurrentY,eax
			mov	RawOffset,eax
			mov	CaretFilePosY,eax
			mov	CaretPosX,eax
			mov	CaretPosY,eax
			invoke	PosCaret,CaretPosX,CaretPosY
			invoke	CalcLines
			WALIBINVOKE WAScrollBarSetMinMaxRange,hHexDump,SB_VERT,0,MaxLines
			invoke	DisplayPage
			invoke	SetScrollPos,hHexDump,SB_VERT,0,1
			ret
LoadFile		endp

end LibMain
