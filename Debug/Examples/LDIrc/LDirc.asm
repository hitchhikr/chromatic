; ------------------------------------------
; An example of Asynchronous I/O in a DOS box.
; ------------------------------------------
; (c) 2001-2002 hitchhikr softworks.
; Written by Franck Charlet
; http://perso.wanadoo.fr/franck.charlet/index.htm
; ------------------------------------------
; I finally decided to make it to work correctly (Especially on Win 2000)
; I won't do anything else on it.
; ------------------------------------------
; (Builtin directives for Chrome's PREP command)
; buildblock RELEASE
;	CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
;	CAPT [BINDIR]\Link.exe /LIBPATH:"[LIBDIR]" /SUBSYSTEM:CONSOLE %1.obj
; buildblockend
; buildblock DEBUG
;	CAPT [BINDIR]\ml.exe /Zd /Zi /c /coff /I"[INCLUDEDIR]" "%2"
;	CAPT [BINDIR]\Link.exe /DEBUG /LIBPATH:"[LIBDIR]" /DEBUGTYPE:CV /SUBSYSTEM:CONSOLE %1.obj
; buildblockend

; --------------- File model
                	.386
                	.model	flat,stdcall
               		option	casemap:none

; --------------- Includes
                	include	windows.inc
                	include	kernel32.inc
                	include	wsock32.inc
                	include	masm32.inc

; --------------- Libraries
                	includelib kernel32.lib
                	includelib wsock32.lib
                	includelib masm32.lib

; --------------- Procedures declarations
Main			    proto
GetInput		    proto	:dword,:dword
FillMem			    proto	:dword,:dword,:dword
AllocMem		    proto	:dword
FreeMem			    proto	:dword
LocCaret		    proto	:dword,:dword

; --------------- Datas section
			        .data
ConsoleInfos	    CONSOLE_SCREEN_BUFFER_INFO <>
hsocket			    dd	    0
ThreadID		    dd	    0
ThreadHandle	    dd	    0
ConOutHandle	    dd	    0
ConInHandle		    dd	    0
CursorX			    dd	    0
CursorY			    dd	    0
RecBufLen		    dd	    0
RecBuf			    dd	    0
SndBuf			    dd	    0
InputBuf		    dd	    0
CmdLineArgHost	    dd	    0		; Host
CmdLineArgPort	    dd	    0		; Port
CmdLineArgNick	    dd	    0		; Nickname
IpToConnect		    db	    0,0,0,0
MsgErr			    db	    "Error: ",0
MsgErrMemory	    db	    "can't allocate required buffers !",0
MsgErrWinSock	    db	    "can't obtain winsock !",0
MsgErrSock		    db	    "can't allocate socket !",0
MsgErrConnect	    db	    "can't connect to host !",0
MsgErrHost		    db	    "can't resolve host address !",0
MsgErrThread	    db	    "can't create thread !",0
MsgTitle		    db	    "Little DOS IRC Client v0.3",0
MsgCreds		    db	    "Written by Franck Charlet.",13,10
			        db	    "(c) 2001-2002 hitchhikr softworks.",13,10,13,10,0
MsgTerminated	    db	    "Process terminated.",13,10,0
MsgConnecting	    db	    "Connecting to: ",0
MsgConnected	    db	    "Connected.",13,10,0
MsgUsage		    db	    "Usage: LDIrc <Host> <Port> <Nick>",0
MsgOnPort		    db	    " port: ",0
CRLF			    db	    13,10,0
CmdNick			    db	    'NICK ',0
CmdUser			    db	    'USER ',0
Space			    db	    ' ',0
Quote			    db	    '"',0
DPoint			    db	    ':',0
UserBuf			    db	    0,0

; --------------- Code section
	                .code

; ------------------------
; --- Routines annexes --- 
; ------------------------

; --------------- Display an error message and exit
RaiseError	        proc	ErrorMessage:dword
			        invoke	StdOut,addr MsgErr
			        invoke	StdOut,ErrorMessage
			        invoke	StdOut,addr CRLF
			        invoke	StdOut,addr MsgTerminated
			        invoke	ExitProcess,0
RaiseError	        endp

; --------------- Wait datas from server
ReceiveDat	        proc
	    	        invoke	ioctlsocket,hsocket,FIONREAD,addr RecBufLen
			        mov	    eax,RecBufLen
			        test	eax,eax
			        jz	    NoDatas
			        inc	    eax
			        mov	    RecBufLen,eax
			        invoke	AllocMem,eax
			        test	eax,eax
			        jz	    MemError
			        mov	    RecBuf,eax
			        invoke	FillMem,RecBuf,RecBufLen,0
			        invoke	recv,hsocket,RecBuf,RecBufLen,0
			        mov	    eax,CursorY
			        dec	    eax
			        invoke	LocCaret,0,eax
			        invoke	StdOut,RecBuf
			        invoke	FreeMem,RecBuf
MemError:	        invoke	LocCaret,0,CursorY
NoDatas:	        ret
ReceiveDat	        endp

; --------------- Send auto datas to server
SendDat		        proc	Message:dword
        			invoke	szCatStr,Message,addr CRLF
        			invoke	lnstr,Message
        			invoke	send,hsocket,Message,eax,0
        			mov	    eax,CursorY
        			dec	    eax
        			invoke	LocCaret,0,eax
        			invoke	StdOut,Message
        			invoke	LocCaret,0,CursorY
        			invoke	FillMem,SndBuf,512,0
        			ret
SendDat		        endp

; --------------- Send user datas to server
WaitInput	        proc
        			invoke	LocCaret,CursorX,CursorY
        			mov	    UserBuf,0
        			mov	    UserBuf+1,0
        			invoke	GetInput,addr UserBuf,1
        			xor	    eax,eax
        			mov	    al,UserBuf
        			cmp	    al,13
        			je	    SendCommand
        			cmp	    al,8
        			jne	    NoBackSpace
        			mov	    eax,CursorX
        			test	eax,eax
        			jz	    StopBackSpace
        			dec	    CursorX
        			mov	    eax,CursorY
        			dec	    eax
        			invoke	LocCaret,CursorX,eax
        			invoke	StdOut,addr Space
        			mov	    eax,CursorY
        			dec	    eax
        			invoke	LocCaret,CursorX,eax
        			mov	    ebx,InputBuf
        			add	    ebx,CursorX
        			xor	    eax,eax
        			mov	    [ebx],al
StopBackSpace:	    xor	    eax,eax
			        ret
NoBackSpace:	    invoke	StdOut,addr UserBuf
        			mov	    al,UserBuf
        			mov	    ebx,InputBuf
        			add	    ebx,CursorX
        			mov	    [ebx],al
        			inc	    CursorX
        			xor	    eax,eax
        			ret
SendCommand:	    mov	    eax,CursorX
        			test	eax,eax	
        			jz	    EmptyLine
        			mov	    dword ptr CursorX,0
        			invoke	szCatStr,InputBuf,addr CRLF
        			invoke	lnstr,InputBuf
        			invoke	send,hsocket,InputBuf,eax,0
        			mov	    eax,InputBuf
        			mov	    eax,[eax]
        			xor	    ebx,ebx
        			and	    eax,0dfdfdfdfh		; Pass it in uppercase
        			cmp	    eax,"TIUQ"
        			jne	    NoQuit
        			inc	    ebx
NoQuit:			    push	ebx
        			mov	    eax,CursorY
        			dec	    eax
        			invoke	LocCaret,CursorX,eax
        			invoke	StdOut,addr CRLF
        			invoke	FillMem,InputBuf,79,32
        			mov	    eax,CursorY
        			dec	    eax
        			invoke	LocCaret,CursorX,eax
        			invoke	StdOut,InputBuf
        			invoke	FillMem,InputBuf,128,0
        			mov	    eax,CursorY
        			dec	    eax
        			invoke	LocCaret,CursorX,eax
        			pop	    eax
EmptyLine:		    ret
WaitInput		    endp

; --------------- AllocMem
AllocMem		    proc	MemSize:dword
        			mov	    eax,MemSize
        			test	eax,eax
        			jz	    NoAllocMem
        			invoke	GlobalAlloc,GMEM_FIXED+GMEM_ZEROINIT,eax
NoAllocMem:		    ret
AllocMem		    endp

; --------------- FreeMem
FreeMem			    proc	MemHandle:dword
    			    mov	    eax,MemHandle
    			    test	eax,eax
    			    jz	    NoFreeMem
    			    invoke	GlobalFree,eax
NoFreeMem:		    ret
FreeMem			    endp

; --------------- Clear the screen
Cls			        proc
        			local	noc:dword
        			local	cnt:dword
        			local	sbi:CONSOLE_SCREEN_BUFFER_INFO
        			invoke	GetConsoleScreenBufferInfo,ConOutHandle,addr sbi
        			mov	    eax,sbi.dwSize
        			push	ax
        			rol	    eax,16
        			mov	    cx,ax
        			pop	    ax
        			mul	    cx
        			cwde
        			mov	    cnt,eax
        			invoke	FillConsoleOutputAttribute,ConOutHandle, \
        			        FOREGROUND_BLUE or FOREGROUND_GREEN or FOREGROUND_RED,cnt,NULL,addr noc
        			invoke	FillConsoleOutputCharacter,ConOutHandle,32,cnt,NULL,addr noc
        			invoke	LocCaret,0,CursorY
        			ret
Cls			        endp

; --------------- Set the caret position
LocCaret		    proc	xPos:dword,yPos:dword
        			push	ebx
        			mov	    eax,yPos
        			shl	    eax,16
        			mov	    ebx,xPos
        			mov	    ax,bx
        	    	invoke	SetConsoleCursorPosition,ConOutHandle,eax
        			pop	    ebx
        			ret
LocCaret		    endp

; --------------- Wait for user input
GetInput		    proc	lpszBuffer:dword,bLen:dword
        			local	bRead:dword
        			invoke	ReadFile,ConInHandle,lpszBuffer,bLen,addr bRead,NULL
        			mov	    eax,bRead
        			ret
GetInput		    endp

; --------------- Fill a memory array
FillMem			    proc	Mem:dword,Bytes:dword,DWordToFill:dword
        			mov	    edi,Mem
        			mov	    ecx,Bytes
        			cld
        			mov	    eax,DWordToFill
        			shr	    ecx,2
        			rep	    stosd
        			mov	    ecx,Bytes
        			and	    ecx,3
        			rep	    stosb
        			ret
FillMem			    endp

; --------------- Free the buffers
FreeBuffers		    proc
        			invoke	FreeMem,SndBuf
        			invoke	FreeMem,InputBuf
        			invoke	FreeMem,CmdLineArgHost
        			invoke	FreeMem,CmdLineArgPort
        			invoke	FreeMem,CmdLineArgNick
        			ret
FreeBuffers		    endp

; --------------- Alloc the needed buffers in memory
AllocBuffers	    proc
        			xor	    eax,eax
        			mov	    SndBuf,eax
        			mov	    InputBuf,eax
        			mov	    CmdLineArgHost,eax
        			mov	    CmdLineArgPort,eax
        			mov	    CmdLineArgNick,eax
        			invoke	AllocMem,512
        			mov	    SndBuf,eax
        			test	eax,eax
        			jnz	    SendBufferOk
        			invoke	RaiseError,addr MsgErrMemory
SendBufferOk:	    invoke	AllocMem,128
        			mov	    InputBuf,eax
        			test	eax,eax
        			jnz	    InputBufferOk
        			invoke	FreeBuffers
        			invoke	RaiseError,addr MsgErrMemory
InputBufferOk:	    invoke	AllocMem,128
        			mov	    CmdLineArgHost,eax
        			test	eax,eax
        			jnz	    ArgHostBufferOk
        			invoke	FreeBuffers
        			invoke	RaiseError,addr MsgErrMemory
ArgHostBufferOk:    invoke	AllocMem,128
    			    mov	    CmdLineArgPort,eax
    			    test	eax,eax
    			    jnz	    ArgPortBufferOk
    			    invoke	FreeBuffers
    			    invoke	RaiseError,addr MsgErrMemory
ArgPortBufferOk:    invoke	AllocMem,128
    			    mov	    CmdLineArgNick,eax
    			    test	eax,eax
    			    jnz	    ArgNickBufferOk
    			    invoke	FreeBuffers
    			    invoke	RaiseError,addr MsgErrMemory
ArgNickBufferOk:    ret
AllocBuffers	    endp

; --------------- Close opened socket and cleanup winsock
CloseWinsock		proc
        			invoke	closesocket,hsocket
        			invoke	WSACleanup
        			invoke	FreeBuffers
        			ret
CloseWinsock		endp

; --------------- Try to resolve a given IP address
ResolveIP		    proc	IPString:dword
        			; --- Fill socket informations
        			invoke	inet_addr,IPString
        			inc	    eax
        			jnz	    IpError
        			; --- Try to resolve hostname
     	        	invoke	gethostbyname,IPString
        			test	eax,eax			    ; eax=hostent structure
        			jz	    HostentError
        			mov	    eax,[eax+12]		; Get ip list address
        			mov	    eax,[eax]		    ; Get first pointer
        			mov	    eax,[eax]		    ; Get contained ip address
        			ret
HostentError:		invoke	WSACleanup
			        invoke	FreeBuffers
			        invoke	RaiseError,addr MsgErrHost
IpError:		    dec	    eax
			        ret
ResolveIP		    endp

; --------------- WinSock Thread
ReceiveThread		proc	Arg:dword
LoopReceive:		invoke	ReceiveDat
			        jmp	    LoopReceive		; Never ending loop
ReceiveThread		endp

; ---------------------
; --- Program start ---
; ---------------------
start:			    invoke	Main
			        invoke	ExitProcess,0

; --------------
; --- Main() --- 
; --------------
Main			    proc
        			; --- Locals structures
		        	local	mysock:sockaddr_in
			        local	mywsa:WSADATA
	
        			; --- Init console		
        			invoke	GetStdHandle,STD_OUTPUT_HANDLE
        			mov	    ConOutHandle,eax
        			invoke	GetStdHandle,STD_INPUT_HANDLE
        			mov	    ConInHandle,eax
        			invoke	SetConsoleMode,ConInHandle,ENABLE_PROCESSED_INPUT
	
        			invoke	GetConsoleScreenBufferInfo,ConOutHandle,addr ConsoleInfos
        			xor	    eax,eax
        			mov	    ax,ConsoleInfos.dwSize.y
        			mov	    CursorY,eax
			
			        invoke	Cls
	
        			; --- Display Starting message
        			invoke	SetConsoleTitle,addr MsgTitle
        			mov	    eax,CursorY
        			dec	    eax
        			invoke	LocCaret,0,eax
        			invoke	StdOut,addr MsgTitle
        			invoke	StdOut,addr CRLF
        			invoke	StdOut,addr MsgCreds
	
			        invoke	AllocBuffers

        			; --- Retrieve user arguments
        			invoke	ArgClC,1,CmdLineArgHost
        			invoke	ArgClC,2,CmdLineArgPort
        			invoke	ArgClC,3,CmdLineArgNick
        			invoke	StrLen,CmdLineArgHost
        			test	eax,eax
        			jnz	    GetUserArgs
        			invoke	StdOut,addr MsgUsage
        			invoke	FreeBuffers
        			ret
GetUserArgs:
        			invoke	StdOut,addr MsgConnecting
        			invoke	StdOut,CmdLineArgHost
        			invoke	StdOut,addr MsgOnPort
        			invoke	StdOut,CmdLineArgPort
        			invoke	StdOut,addr CRLF
		
        			; --- Init winsock (1.1 should be enough here)
        			invoke	WSAStartup,0101h,addr mywsa
        			test	eax,eax
        			jz	    WinsockInitOk
        			invoke	FreeBuffers
        			invoke	RaiseError,addr MsgErrWinSock
WinsockInitOk:
        			; --- Decode the given host
        			invoke	ResolveIP,CmdLineArgHost
        			mov	    mysock.sin_addr.in_addr,eax
        			mov	    mysock.sin_family,AF_INET
	
        			; --- Alloc local port
        			invoke	atodw,CmdLineArgPort
        			invoke	htons,eax		; Reverse port
        			mov	    mysock.sin_port,ax
        	
        			; --- Socket creation
        			invoke	socket,AF_INET,SOCK_STREAM,IPPROTO_IP
        			mov	    hsocket,eax
        			inc	    eax
        			jne	    OkSocket
        			invoke	WSACleanup
        			invoke	FreeBuffers
        			invoke	RaiseError,addr MsgErrSock
OkSocket:
        			; --- Connection to the server
        			invoke	connect,hsocket,addr mysock,16
        			test	eax,eax
        			jz	    OkConnect
        			invoke	CloseWinsock
        			invoke	RaiseError,addr MsgErrConnect
OkConnect:
        			; --- We are connected
        			invoke	StdOut,addr MsgConnected
        	
        			; --- Send nickname
        			invoke	szCatStr,SndBuf,addr CmdNick
        			invoke	szCatStr,SndBuf,CmdLineArgNick
        			invoke	SendDat,SndBuf
        	
        			; --- Send user infos
        			invoke	szCatStr,SndBuf,addr CmdUser
        			invoke	szCatStr,SndBuf,CmdLineArgNick
        			invoke	szCatStr,SndBuf,addr Space
        			invoke	szCatStr,SndBuf,addr Quote
        			invoke	szCatStr,SndBuf,addr Quote
        			invoke	szCatStr,SndBuf,addr Space
        			invoke	szCatStr,SndBuf,addr Quote
        			invoke	szCatStr,SndBuf,addr Quote
        			invoke	szCatStr,SndBuf,addr Space
        			invoke	szCatStr,SndBuf,addr DPoint
        			invoke	szCatStr,SndBuf,CmdLineArgNick
        			invoke	SendDat,SndBuf
        	
        			; --- Create the receiving thread
        			invoke	CreateThread,0,0,addr ReceiveThread,0,0,addr ThreadID
        			mov	    ThreadHandle,eax
        			test	eax,eax
        			jnz	    ThreadCreated
        			invoke	CloseWinsock
        			invoke	RaiseError,addr MsgErrThread
ThreadCreated:
	
			        ; --- Wait for keyboard input
MainLoop:		    invoke	WaitInput
        			test	eax,eax
        			jnz	    ExitLoop
        			jmp	    MainLoop
ExitLoop:
        			; --- End of line
        			invoke	CloseHandle,ThreadHandle
        			invoke	CloseWinsock
        			ret
Main			    endp

end start
