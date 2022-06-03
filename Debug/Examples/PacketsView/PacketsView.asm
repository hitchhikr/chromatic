; ------------------------------------------
; Title: Packets view v1.6
; (Requires at least Windows 2000 with administrator rights).
; ------------------------------------------
; (c) 2001-2003 hitchhikr softworks.
; Written by Franck Charlet
; http://perso.wanadoo.fr/franck.charlet/index.htm
; ------------------------------------------
; History:
;         v1.6:  - Added handling for console signals.
;                - Added Sleep(1) to avoid the report of 100% CPU usage.
;                - Slightly cleaned up the sourcecode.
;                - Increased length of arguments buffers.
;         v1.5:  - Line arguments are now optional.
;                - Interface number checked.
;                - Added an option to limit the amount of hex bytes 
;                  displayed / packet in ini file (DisplayBytes).
;         v1.4:  - DecodeHeader key in ini file can now be:
;                       0 : No decoding (raw format).
;                           In this mode, packets are just converted into ascii hexa
;                           without any kind of formatting (except a longword header
;                           containing the packet len, to allow easy redirecting).
;                       1 : General header (separators+receiving dates+packet len).
;                       2 : Full packets decoding.
;         v1.3:  - Corrected a little bug in ICMPTypeTable (was dd instead of dw).
;                - Added switches in ini file to allow/disallow
;                  displaying of arrival date of packet and header decoding.
;         v1.2:  - Added handling for PacketsView.ini
;                  to allow user to select monitored TCP/UDP ports.
;         v1.1:  - Added TCP/UDP/ICMP headers decoding.
; Todo:
;                - A mode selector for:
;                       Hex only.
;                       Hex/ascii.
;                       Ascii only.
; ------------------------------------------
; (Builtin directives for Chrome's PREP command)
; buildblock RELEASE
;       CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
;       CAPT [BINDIR]\Link.exe /LIBPATH:"[LIBDIR]" /SUBSYSTEM:CONSOLE "%1.obj"
; buildblockend
; buildblock DEBUG
;       CAPT [BINDIR]\ml.exe /Zd /Zi /c /coff /I"[INCLUDEDIR]" "%2"
;       CAPT [BINDIR]\Link.exe /DEBUG /LIBPATH:"[LIBDIR]" /DEBUGTYPE:CV /SUBSYSTEM:CONSOLE "%1.obj"
; buildblockend

; --------------- File model
                        .486
                        .model  flat,stdcall
                        option  casemap:none

; --------------- Includes
                        include windows.inc
                        include kernel32.inc
                        include user32.inc
                        include ws2_32.inc
                        include masm32.inc

; --------------- Constants
WSA_FLAG_OVERLAPPED     equ     1
SIO_ADDRESS_LIST_QUERY  equ     048000016h
SIO_RCVALL              equ     098000001h

; (Structure of the main protocol decoding table)
PROTOCOL_NAME_OFFSET    equ     0
DECODER_PROC_OFFSET     equ     2
FILTER_PROC_OFFSET      equ     4
TABLE_PADDING           equ     6

; --------------- Structures
IPHEADER                STRUCT
Version_and_HdrLen      db      ?
ServiceType             db      ?
TotalLen                dw      ?
ID                      dw      ?
Flags_and_FragOffset    dw      ?
TimeToLive              db      ?
Protocol                db      ?
HeaderChecksum          dw      ?
SourceAddr              dd      ?
DestAddr                dd      ?
IPHEADER                ENDS

TCPHEADER               STRUCT
SourcePort              dw      ?
DestinationPort         dw      ?
SequenceNumber          dd      ?
AckNumber               dd      ?
HeaderLen               db      ?
ControlBits             db      ?
Window                  dw      ?
CheckSum                dw      ?
Padding                 dw      ?
TCPHEADER               ENDS

UDPHEADER               STRUCT
SourcePort              dw      ?
DestinationPort         dw      ?
HLength                 dw      ?
CheckSum                dw      ?
UDPHEADER               ENDS

ICMPHEADER              STRUCT
ICMPType                db      ?
ICMPCode                db      ?
ICMPCheckSum            dw      ?
ICMPHEADER              ENDS

; --------------- Libraries
                        includelib kernel32.lib
                        includelib user32.lib
                        includelib ws2_32.lib
                        includelib masm32.lib

; --------------- Procedures declarations
Main                    proto
IsDigit                 proto   :byte

; --------------- Datas section
                        .data
VarBase                 dd      0
hsocket                 dd      0
RecBufLen               dd      0
RecBuf                  dd      0
InterfaceBuf            dd      0
CmdLineArgHost          dd      0
CmdLineArgInterface     dd      0
TCPPortsTable           dd      0
UDPPortsTable           dd      0
PortsString             dd      0
AppPath                 dd      0
CompleteIniFile         dd      0
CurrentParsedPort       dd      0
RawAsciiBuf             dd      0
DisplayLen              dd      0
PrecedenceTable         dw      (offset MsgSPrec000)-(offset VarBase)
                        dw      (offset MsgSPrec001)-(offset VarBase)
                        dw      (offset MsgSPrec010)-(offset VarBase)
                        dw      (offset MsgSPrec011)-(offset VarBase)
                        dw      (offset MsgSPrec100)-(offset VarBase)
                        dw      (offset MsgSPrec101)-(offset VarBase)
                        dw      (offset MsgSPrec110)-(offset VarBase)
                        dw      (offset MsgSPrec111)-(offset VarBase)
ProtocolTable           dw      (offset MsgProtoReserved)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoICMP)-(offset VarBase),(offset DecodeICMP)-(offset CodeBase),0,0
                        dw      (offset MsgProtoUnassigned)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoGGP)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoGMMP)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoST)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoTCP)-(offset VarBase),(offset DecodeTCP)-(offset CodeBase),(offset FilterTCP)-(offset CodeBase),0
                        dw      (offset MsgProtoUCL)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoUnassigned)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoSecure)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoBBN)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoNVP)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoPUP)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoPluribus)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoTelenet)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoXNET)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoChaos)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoUDP)-(offset VarBase),(offset DecodeUDP)-(offset CodeBase),(offset FilterUDP)-(offset CodeBase),0
                        dw      (offset MsgProtoMultiplexing)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoDCN)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoTAC)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoUnassigned)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoIDP)-(offset VarBase),0,0,0
                        repeat  40
                        dw      (offset MsgProtoUnassigned)-(offset VarBase),0,0,0
                        endm
                        dw      (offset MsgProtoLocal)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoSATNET1)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoMIT)-(offset VarBase),0,0,0
                        repeat  3
                        dw      (offset MsgProtoUnassigned)-(offset VarBase),0,0,0
                        endm
                        dw      (offset MsgProtoSATNET2)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoUnassigned)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoIPCU)-(offset VarBase),0,0,0
                        repeat  4
                        dw      (offset MsgProtoUnassigned)-(offset VarBase),0,0,0
                        endm
                        dw      (offset MsgProtoSATNET3)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoND)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoWIDEBAND1)-(offset VarBase),0,0,0
                        dw      (offset MsgProtoWIDEBAND2)-(offset VarBase),0,0,0
                        repeat  175
                        dw      (offset MsgProtoUnassigned)-(offset VarBase),0,0,0
                        endm
                        dw      (offset MsgProtoRAW)-(offset VarBase),0,0,0
CtrlBitsTable           dw      (offset MsgCtrlFIN)-(offset VarBase)
                        dw      (offset MsgCtrlSYN)-(offset VarBase)
                        dw      (offset MsgCtrlRST)-(offset VarBase)
                        dw      (offset MsgCtrlPSH)-(offset VarBase)
                        dw      (offset MsgCtrlACK)-(offset VarBase)
                        dw      (offset MsgCtrlURG)-(offset VarBase)
                        ; --- Offset -4
                        dw      (offset MsgICMPUnknown)-(offset VarBase),(offset ICMPDecodeUnknown)-(offset CodeBase)
ICMPTypeTable           dw      (offset MsgICMPTypeEchoReply)-(offset VarBase),(offset ICMPDecode8)-(offset CodeBase)
                        dw      (offset MsgICMPUnknown)-(offset VarBase),(offset ICMPDecodeUnknown)-(offset CodeBase)
                        dw      (offset MsgICMPUnknown)-(offset VarBase),(offset ICMPDecodeUnknown)-(offset CodeBase)
                        dw      (offset MsgICMPTypeDestUnreach)-(offset VarBase),(offset ICMPDecode3)-(offset CodeBase)
                        dw      (offset MsgICMPTypeSourceQuench)-(offset VarBase),(offset ICMPDecode3)-(offset CodeBase)
                        dw      (offset MsgICMPTypeRedirect)-(offset VarBase),(offset ICMPDecode5)-(offset CodeBase)
                        dw      (offset MsgICMPUnknown)-(offset VarBase),(offset ICMPDecodeUnknown)-(offset CodeBase)
                        dw      (offset MsgICMPUnknown)-(offset VarBase),(offset ICMPDecodeUnknown)-(offset CodeBase)
                        dw      (offset MsgICMPTypeEcho)-(offset VarBase),(offset ICMPDecode8)-(offset CodeBase)
                        dw      (offset MsgICMPUnknown)-(offset VarBase),(offset ICMPDecodeUnknown)-(offset CodeBase)
                        dw      (offset MsgICMPUnknown)-(offset VarBase),(offset ICMPDecodeUnknown)-(offset CodeBase)
                        dw      (offset MsgICMPTypeTimeExceed)-(offset VarBase),(offset ICMPDecode3)-(offset CodeBase)
                        dw      (offset MsgICMPTypeParamProblem)-(offset VarBase),(offset ICMPDecode12)-(offset CodeBase)
                        dw      (offset MsgICMPTypeTimeStamp)-(offset VarBase),(offset ICMPDecode13)-(offset CodeBase)
                        dw      (offset MsgICMPTypeTimeReply)-(offset VarBase),(offset ICMPDecode13)-(offset CodeBase)
                        dw      (offset MsgICMPTypeInfoRequest)-(offset VarBase),(offset ICMPDecode8)-(offset CodeBase)
                        dw      (offset MsgICMPTypeInfoReply)-(offset VarBase),(offset ICMPDecode8)-(offset CodeBase)
ICMPMsgTypeTable        dw      (offset ICMPTypeUnknownTable)-(offset VarBase)
                        dw      (offset ICMPType0Table)-(offset VarBase)
                        dw      (offset ICMPTypeUnknownTable)-(offset VarBase)
                        dw      (offset ICMPTypeUnknownTable)-(offset VarBase)
                        dw      (offset ICMPType3Table)-(offset VarBase)
                        dw      (offset ICMPType4Table)-(offset VarBase)
                        dw      (offset ICMPType5Table)-(offset VarBase)
                        dw      (offset ICMPTypeUnknownTable)-(offset VarBase)
                        dw      (offset ICMPTypeUnknownTable)-(offset VarBase)
                        dw      (offset ICMPType8Table)-(offset VarBase)
                        dw      (offset ICMPTypeUnknownTable)-(offset VarBase)
                        dw      (offset ICMPTypeUnknownTable)-(offset VarBase)
                        dw      (offset ICMPType11Table)-(offset VarBase)
                        dw      (offset ICMPType12Table)-(offset VarBase)
                        dw      (offset ICMPType13Table)-(offset VarBase)
                        dw      (offset ICMPType14Table)-(offset VarBase)
                        dw      (offset ICMPType15Table)-(offset VarBase)
                        dw      (offset ICMPType16Table)-(offset VarBase)
ICMPType0Table          dw      0
ICMPType3Table          dw      6
                        dw      (offset Type3Msg0)-(offset VarBase)
                        dw      (offset Type3Msg1)-(offset VarBase)
                        dw      (offset Type3Msg2)-(offset VarBase)
                        dw      (offset Type3Msg3)-(offset VarBase)
                        dw      (offset Type3Msg4)-(offset VarBase)
                        dw      (offset Type3Msg5)-(offset VarBase)
ICMPType4Table          dw      0
ICMPType5Table          dw      4
                        dw      (offset Type5Msg0)-(offset VarBase)
                        dw      (offset Type5Msg1)-(offset VarBase)
                        dw      (offset Type5Msg2)-(offset VarBase)
                        dw      (offset Type5Msg3)-(offset VarBase)
ICMPType8Table          dw      0
ICMPType11Table         dw      2
                        dw      (offset Type11Msg0)-(offset VarBase)
                        dw      (offset Type11Msg1)-(offset VarBase)
ICMPType12Table         dw      1
                        dw      (offset Type12Msg0)-(offset VarBase)
ICMPType13Table         dw      0
ICMPType14Table         dw      0
ICMPType15Table         dw      0
ICMPType16Table         dw      0
ICMPTypeUnknownTable    dw      0
MsgErr                  db      "Error: ",0
MsgErrMemory            db      "can't allocate required buffers !",0
MsgErrWinSock           db      "winsock 2.2 required !",0
MsgErrSock              db      "can't allocate socket !",0
MsgErrQuery             db      "can't query addresses list !",0
MsgErrInterface         db      "no network interface available !",0
MsgErrInvalidInterface  db      "invalid interface number !",0
MsgErrBytes             db      "invalid number of bytes !",0
MsgErrBind              db      "can't bind socket !",0
MsgErrReceive           db      "can't initiate receiver !",0
MsgErrHost              db      "can't resolve host address !",0
MsgErrPortsTable        db      "can't allocate ports table !",0
MsgErrPortGroup         db      "unexpected minus sign !",0
MsgErrPortsBalance      db      "incomplete ports definition !",0
MsgErrInvalidIniVal     db      "invalid value found in ini file !",0
MsgErrIllegalPort       db      "illegal port number !",0
MsgTitle                db      "Packets view v1.6",0
MsgCreds                db      "Written by Franck Charlet.",13,10
                        db      "(c)2001-2003 hitchhikr softworks.",13,10,13,10,0
MsgUsage                db      "Usage: PacketsView [Host] [Interface number (from 1 to x)]",13,10
                        db      "Example: PacketsView 123.123.123.123 1",0
MsgArgHelp1             db      "/help",0
MsgArgHelp2             db      "/?",0
MsgIniFile              db      "PacketsView.ini",0
MsgLayoutKey            db      "Layout",0
MsgTCPPortKey           db      "TCPPorts",0
MsgUDPPortKey           db      "UDPPorts",0
MsgDisplayDate          db      "DisplayDate",0
MsgDecodeHeader         db      "DecodeHeader",0
MsgDisplayBytes         db      "DisplayBytes",0
MsgMonitorPorts         db      "Monitor",0
MsgTerminated           db      "Process terminated.",13,10,0
MsgListening            db      "Listening...",13,10,0
MsgDate                 db      "Date: ",0
MsgIPSource             db      "Source IP: ",0
MsgIPDest               db      "Destination IP: ",0
MsgHeadChkSum           db      "(Checksum: ",0
MsgLTime                db      "Life time: ",0
MsgHVersion             db      "Version: ",0
MsgHLen                 db      "Header length: ",0
MsgSPrecedence          db      "Type of service precedence: ",0
MsgSPrec111             db      "Network Control",0
MsgSPrec110             db      "Internetwork Control",0
MsgSPrec101             db      "CRITIC/ECP",0
MsgSPrec100             db      "Flash Override",0
MsgSPrec011             db      "Flash",0
MsgSPrec010             db      "Immediate",0
MsgSPrec001             db      "Priority",0
MsgSPrec000             db      "Routine",0
MsgNormDelay            db      "Delay: Normal",0
MsgLowDelay             db      "Delay: Low",0
MsgNormTrough           db      "- Throughput: Normal",0
MsgHighTrough           db      "- Throughput: High",0
MsgNormReliab           db      "- Reliability: Normal",0
MsgHighReliab           db      "- Reliability: High",0
MsgPosFrag              db      "Yes",0
MsgNoFrag               db      "No",0
MsgMoreFrag             db      "More to come",0
MsgLastFrag             db      "Last part",0
MsgFragOffset           db      " (Offset: ",0
MsgPacketType           db      "Packet Type: ",0
MsgPacketID             db      "Packet ID: ",0
MsgPacketFlags          db      "Fragmentation: ",0
MsgPacketDatas          db      "Packet datas:",0
MsgBytes                db      " bytes",0
MsgProtoICMP            db      "ICMP",0
MsgProtoGGP             db      "GGP Gateway to Gateway",0
MsgProtoGMMP            db      "CMCC Gateway Monitoring Message",0
MsgProtoST              db      "ST",0
MsgProtoTCP             db      "TCP",0
MsgProtoUCL             db      "UCL",0
MsgProtoSecure          db      "Secure",0
MsgProtoBBN             db      "BBN RCC Monitoring",0
MsgProtoNVP             db      "NVP",0
MsgProtoPUP             db      "PUP",0
MsgProtoPluribus        db      "Pluribus",0
MsgProtoTelenet         db      "Telenet",0
MsgProtoXNET            db      "XNET",0
MsgProtoChaos           db      "Chaos",0
MsgProtoUDP             db      "UDP",0
MsgProtoMultiplexing    db      "Multiplexing",0
MsgProtoDCN             db      "DCN",0
MsgProtoTAC             db      "TAC Monitoring",0
MsgProtoIDP             db      "IDP",0
MsgProtoLocal           db      "Local network",0
MsgProtoSATNET1         db      "SATNET and Backroom EXPAK",0
MsgProtoMIT             db      "MIT Subnet Support",0
MsgProtoSATNET2         db      "SATNET Monitoring",0
MsgProtoIPCU            db      "Internet Packet Core Utility",0
MsgProtoSATNET3         db      "Backroom SATNET Monitoring",0
MsgProtoND              db      "Net Disk",0
MsgProtoWIDEBAND1       db      "WIDEBAND Monitoring",0
MsgProtoWIDEBAND2       db      "WIDEBAND EXPAK",0
MsgProtoRAW             db      "RAW",0
MsgProtoUnassigned      db      "Unassigned",0
MsgProtoReserved        db      "Reserved",0
MsgICMPHeader           db      "ICMP header: ",0
MsgICMPType             db      "Message type: ",0
MsgICMPTypeEchoReply    db      "Echo reply (",0
MsgICMPTypeDestUnreach  db      "Destination Unreachable (",0
MsgICMPTypeSourceQuench db      "Source Quench (",0
MsgICMPTypeRedirect     db      "Redirect (",0
MsgICMPTypeEcho         db      "Echo (",0
MsgICMPTypeTimeExceed   db      "Time Exceeded (",0
MsgICMPTypeParamProblem db      "Parameter Problem (",0
MsgICMPTypeTimeStamp    db      "Timestamp (",0
MsgICMPTypeTimeReply    db      "Timestamp Reply (",0
MsgICMPTypeInfoRequest  db      "Information Request (",0
MsgICMPTypeInfoReply    db      "Information Reply (",0
MsgICMPUnknown          db      "Unknown (",0
MsgICMPCode             db      "Message code: ",0
Type3Msg0               db      "net unreachable (",0
Type3Msg1               db      "host unreachable (",0
Type3Msg2               db      "protocol unreachable (",0
Type3Msg3               db      "port unreachable (",0
Type3Msg4               db      "fragmentation needed and DF set (",0
Type3Msg5               db      "source route failed (",0
Type5Msg0               db      "redirect datagrams for the Network (",0
Type5Msg1               db      "redirect datagrams for the Host (",0
Type5Msg2               db      "redirect datagrams for the Type of Service and Network (",0
Type5Msg3               db      "redirect datagrams for the Type of Service and Host (",0
Type11Msg0              db      "time to live exceeded in transit (",0
Type11Msg1              db      "fragment reassembly time exceeded (",0
Type12Msg0              db      "pointer indicates the error (",0
MsgEmbedded             db      "(Original header and datagram included in datas)",0
MsgGateWayIP            db      "Gateway IP: ",0
MsgOrgStamp             db      "Originate timestamp: ",0
MsgRecStamp             db      "Receive timestamp: ",0
MsgTransStamp           db      "Transmit timestamp: ",0
MsgSeconds              db      " milliseconds",0
MsgIdentifier           db      "Identifier: ",0
MsgUDPHeader            db      "UDP header: ",0
MsgLength               db      "Length: ",0
MsgTCPHeader            db      "TCP header: ",0
MsgSourcePort           db      "Source port: ",0
MsgDestPort             db      "Destination port: ",0
MsgSeqNumber            db      "Sequence number: ",0
MsgAckNumber            db      "- Ack number: ",0
MsgPointErr             db      "Error at byte: ",0
MsgCtrlBits             db      "Control bits:",0
MsgCtrlURG              db      " URG",0
MsgCtrlACK              db      " ACK",0
MsgCtrlPSH              db      " PSH",0
MsgCtrlRST              db      " RST",0
MsgCtrlSYN              db      " SYN",0
MsgCtrlFIN              db      " FIN",0
MsgWindow               db      "Window: ",0
MsgCheckSum             db      "Checksum: ",0
MsgSpace                db      " ",0
MsgDPoint               db      ":",0
MsgOBracket             db      "(",0
MsgCBracket             db      ")",0
MsgSlash                db      "/",0
MsgHexSign              db      "$",0
HexTbl                  db      "0123456789ABCDEF"
AdrHex2                 db      "00",0
MsgSeparator            db      66 dup ("-")
MsgCRLF                 db      13,10,0
DecBlock                db      20 dup (0)
CurrentByte             db      0,0
HdrL                    db      0
HexLine                 db      66 dup (32)
                        db      13,10,0
ByteOffsetTableHex      db      0*3,1*3,2*3,3*3,4*3,5*3,6*3,7*3,8*3,9*3,10*3
                        db      11*3,12*3,13*3,14*3,15*3
ByteAsciiPosition       db      16*3
CurrentChar             db      0
PortParsingBalance      db      0
BolDisplayDate          db      0
ByteDecodeHeader        db      0

; --------------- Code section
                        .code
CodeBase:

; ------------------------
; --- Routines annexes --- 
; ------------------------

; --- Display an error message and exit
                        align   16
RaiseError              proc    ErrorMessage:dword
                        invoke  StdOut,addr MsgErr
                        invoke  StdOut,ErrorMessage
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgTerminated
                        invoke  ExitProcess,0
RaiseError              endp

; --------------- AllocMem
                        align   16
AllocMem                proc    MemSize:dword
                        mov     eax,MemSize
                        test    eax,eax
                        jz      NoAllocMem
                        invoke  GlobalAlloc,GMEM_FIXED+GMEM_ZEROINIT,eax
NoAllocMem:             ret
AllocMem                endp

; --------------- FreeMem
                        align   16
FreeMem                 proc    MemHandle:dword
                        mov     eax,MemHandle
                        test    eax,eax
                        jz      NoFreeMem
                        invoke  GlobalFree,eax
NoFreeMem:              ret
FreeMem                 endp

; --------------- Fill a memory array
                        align   16
FillMem                 proc    Mem:dword,Bytes:dword,DWordToFill:dword
                        push    ecx
                        push    edi
                        mov     edi,Mem
                        mov     ecx,Bytes
                        cld
                        mov     eax,DWordToFill
                        shr     ecx,2
                        rep     stosd
                        mov     ecx,Bytes
                        and     ecx,3
                        rep     stosb
                        pop     edi
                        pop     ecx
                        ret
FillMem                 endp

; --------------- Display a byte in hexadecimal
                        align   16
PrintHex2               proc
                        push    eax
                        push    ebx
                        movzx   ebx,al
                        and     bl,0fh
                        mov     bl,[HexTbl+ebx]
                        mov     [AdrHex2+1],bl
                        and     eax,0f0h
                        shr     al,4
                        mov     bl,[HexTbl+eax]
                        mov     [AdrHex2],bl
                        invoke  StdOut,addr AdrHex2
                        pop     ebx
                        pop     eax
                        ret
PrintHex2               endp

; --------------- Display a word in hexadecimal
                        align   16
PrintHex4               proc
                        push    eax
                        xchg    ah,al
                        call    PrintHex2
                        xchg    ah,al
                        call    PrintHex2
                        pop     eax
                        ret
PrintHex4               endp

; --------------- Display a dword in hexadecimal
                        align   16
PrintHex8               proc
                        bswap   eax
                        push    eax
                        bswap   eax
                        xchg    al,ah
                        call    PrintHex4
                        pop     eax
                        jmp     PrintHex4
PrintHex8               endp

; --------------- Store a byte in hexadecimal
                        align   16
StoreHex2               proc
                        push    eax
                        push    ebx
                        movzx   ebx,al
                        and     bl,0fh
                        mov     bl,[HexTbl+ebx]
                        mov     [edi+1],bl
                        and     eax,0f0h
                        shr     al,4
                        mov     bl,[HexTbl+eax]
                        mov     [edi],bl
                        pop     ebx
                        pop     eax
                        ret
StoreHex2               endp

; --------------- Store a word in hexadecimal
                        align   16
StoreHex4               proc
                        push    eax
                        xchg    ah,al
                        call    StoreHex2
                        add     edi,2
                        xchg    ah,al
                        call    StoreHex2
                        add     edi,2
                        pop     eax
                        ret
StoreHex4               endp

; --------------- Store a dword in hexadecimal
                        align   16
StoreHex8               proc
                        bswap   eax
                        push    eax
                        bswap   eax
                        xchg    al,ah
                        call    StoreHex4
                        pop     eax
                        jmp     StoreHex4
StoreHex8               endp

; --------------- Print current Date/Time in ASCII
                        align   16
PrintNow                proc
                        local   Now:SYSTEMTIME
                        local   DateLen:dword
                        local   DateMem:dword
                        mov     al,BolDisplayDate
                        test    al,al
                        jz      CantAllocDate
                        invoke  GetLocalTime,addr Now
                        ; --- Retrieve the needed length
                        invoke  GetDateFormat,LOCALE_USER_DEFAULT,DATE_SHORTDATE,addr Now,0,0,0
                        mov     DateLen,eax
                        inc     eax
                        invoke  AllocMem,eax
                        mov     DateMem,eax
                        test    eax,eax
                        jz      CantAllocDate
                        ; --- Let system format it
                        invoke  GetDateFormat,LOCALE_USER_DEFAULT,DATE_SHORTDATE,addr Now,0,eax,DateLen
                        ; --- Print out
                        invoke  StdOut,addr MsgDate
                        invoke  StdOut,DateMem
                        invoke  FreeMem,DateMem
                        invoke  StdOut,addr MsgSpace
                        ; --- Do the same for the time
                        invoke  GetTimeFormat,LOCALE_USER_DEFAULT,TIME_FORCE24HOURFORMAT,addr Now,0,0,0
                        mov     DateLen,eax
                        inc     eax
                        invoke  AllocMem,eax
                        mov     DateMem,eax
                        test    eax,eax
                        jz      CantAllocTime
                        invoke  GetTimeFormat,LOCALE_USER_DEFAULT,TIME_FORCE24HOURFORMAT,addr Now,0,eax,DateLen
                        ; --- Print out
                        invoke  StdOut,DateMem
                        invoke  FreeMem,DateMem
CantAllocTime:          invoke  StdOut,addr MsgCRLF
CantAllocDate:          ret
PrintNow                endp

; --------------- Filter TCP packets
                        align   16
FilterTCP               proc
                        push    eax
                        mov     esi,[esp]
                        movzx   eax,[esi+TCPHEADER.DestinationPort]
                        xchg    ah,al
                        mov     ebx,TCPPortsTable                       
                        add     ebx,eax
                        xor     eax,eax
                        mov     al,[ebx]                ; Retrieve monitoring status
                        add     esp,4                   ; Stack correction
                        ret
FilterTCP               endp

; --------------- Decode TCP headers
                        align   16
DecodeTCP               proc
                        push    eax
                        invoke  StdOut,addr MsgTCPHeader
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgSourcePort
                        mov     esi,[esp]
                        movzx   eax,[esi+TCPHEADER.SourcePort]
                        xchg    ah,al
                        invoke  dw2a,eax,addr DecBlock
                        invoke  StdOut,addr DecBlock
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgDestPort
                        mov     esi,[esp]
                        movzx   eax,[esi+TCPHEADER.DestinationPort]
                        xchg    ah,al
                        invoke  dw2a,eax,addr DecBlock
                        invoke  StdOut,addr DecBlock
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgSeqNumber
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     eax,[esi+TCPHEADER.SequenceNumber]
                        call    PrintHex8
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgAckNumber
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     eax,[esi+TCPHEADER.AckNumber]
                        call    PrintHex8
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgCtrlBits
                        mov     esi,[esp]
                        mov     al,[esi+TCPHEADER.ControlBits]
                        and     eax,03fh
                        xor     ecx,ecx
                        xor     ebx,ebx
                        mov     cl,6
                        mov     bl,5
CheckCtrlBits:          bt      ax,bx
                        jnc     NoCtrlBit
                        push    eax
                        push    ebx
                        push    ecx
                        movzx   ebx,[CtrlBitsTable+ebx*2]
                        add     ebx,offset VarBase
                        invoke  StdOut,ebx
                        pop     ecx
                        pop     ebx
                        pop     eax
NoCtrlBit:              dec     ebx
                        loop    CheckCtrlBits
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgWindow
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi+TCPHEADER.Window]
                        xchg    ah,al
                        call    PrintHex4
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgCheckSum
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi+TCPHEADER.CheckSum]
                        xchg    ah,al
                        call    PrintHex4
                        add     esp,4
                        push    20
                        pop     eax
                        ret
DecodeTCP               endp

; --------------- Filter UDP packets
                        align   16
FilterUDP               proc
                        push    eax
                        mov     esi,[esp]
                        movzx   eax,[esi+TCPHEADER.DestinationPort]
                        xchg    ah,al
                        mov     ebx,UDPPortsTable
                        add     ebx,eax
                        xor     eax,eax
                        mov     al,[ebx]                ; Retrieve monitoring status
                        add     esp,4
                        ret
FilterUDP               endp

; --------------- Decode UDP headers
                        align   16
DecodeUDP               proc
                        push    eax
                        invoke  StdOut,addr MsgUDPHeader
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgSourcePort
                        mov     esi,[esp]
                        movzx   eax,[esi+UDPHEADER.SourcePort]
                        xchg    ah,al
                        invoke  dw2a,eax,addr DecBlock
                        invoke  StdOut,addr DecBlock
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgDestPort
                        mov     esi,[esp]
                        movzx   eax,[esi+UDPHEADER.DestinationPort]
                        xchg    ah,al
                        invoke  dw2a,eax,addr DecBlock
                        invoke  StdOut,addr DecBlock
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgLength
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi+UDPHEADER.HLength]
                        xchg    ah,al
                        call    PrintHex4
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgCheckSum
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi+UDPHEADER.CheckSum]
                        xchg    ah,al
                        call    PrintHex4
                        add     esp,4
                        push    8
                        pop     eax
                        ret
DecodeUDP               endp

; --------------- Decode ICMP headers
                        align   16
DecodeICMP              proc
                        local   ICMPTp:dword
                        local   ICMPCode:byte
                        push    eax
                        invoke  StdOut,addr MsgICMPHeader
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgICMPType
                        mov     esi,[esp]
                        movzx   eax,[esi+ICMPHEADER.ICMPType]
                        test    al,al
                        js      UnknownICMPType
                        cmp     al,16
                        jle     CorrectICMPType
UnknownICMPType:        xor     eax,eax
                        dec     eax
CorrectICMPType:        ; --- Range: from -1(bad type) to 16
                        mov     ICMPTp,eax
                        movzx   eax,[ICMPTypeTable+eax*4]
                        add     eax,offset VarBase
                        invoke  StdOut,eax
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     al,[esi+ICMPHEADER.ICMPType]
                        call    PrintHex2
                        invoke  StdOut,addr MsgCBracket
DisplayICMPType:        mov     eax,ICMPTp
                        inc     eax
                        movzx   ebx,[ICMPMsgTypeTable+eax*2]
                        add     ebx,offset VarBase
                        xor     ecx,ecx
                        mov     cx,[ebx]                ; Take number of entries
                        mov     esi,[esp]
                        movzx   eax,[esi+ICMPHEADER.ICMPCode]
                        mov     ICMPCode,al
                        cmp     ecx,eax                 ; Secure bad codes from table
                        jge     CorrectICMPCode
                        push    ebx
                        push    ecx
                        push    eax
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgICMPCode
                        invoke  StdOut,addr MsgICMPUnknown
                        invoke  StdOut,addr MsgHexSign
                        pop     eax
                        call    PrintHex2
                        invoke  StdOut,addr MsgCBracket
                        pop     ecx
                        pop     ebx
                        add     esp,4
                        push    2
                        pop     eax
                        ret
                        align   16
CorrectICMPCode:        test    eax,eax
                        jz      EmptyCodeTable
                        xor     eax,eax
                        mov     ax,[ebx+2+eax*2]
                        add     eax,offset VarBase
                        push    eax
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgICMPCode
                        pop     eax
                        invoke  StdOut,eax
                        invoke  StdOut,addr MsgHexSign
                        mov     al,ICMPCode
                        call    PrintHex2
                        invoke  StdOut,addr MsgCBracket
EmptyCodeTable:         invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgCheckSum
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi+ICMPHEADER.ICMPCheckSum]
                        xchg    ah,al
                        call    PrintHex4
                        mov     ebx,ICMPTp
                        mov     esi,[esp]
                        add     esi,sizeof ICMPHEADER
                        movzx   eax,[ICMPTypeTable+2+ebx*4]
                        add     eax,offset CodeBase
                        call    eax
                        add     esp,4
                        add     eax,4           ; Add ICMP header to returned value
                        ret
DecodeICMP              endp
                        align   16
ICMPDecodeUnknown:      xor     eax,eax
                        ret
                        align   16
ICMPDecode3:            invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgEmbedded
                        xor     eax,eax
                        ret
                        align   16
ICMPDecode5:            push    esi
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgGateWayIP
                        mov     esi,[esp]
                        mov     eax,[esi]
                        invoke  inet_ntoa,eax
                        invoke  StdOut,eax
                        add     esp,4
                        push    4
                        pop     eax
                        ret
                        align   16
ICMPDecode8:            push    esi
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgIdentifier
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi]
                        call    PrintHex4
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgSeqNumber
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi+2]
                        call    PrintHex4
                        add     esp,4
                        push    4
                        pop     eax
                        ret
                        align   16
ICMPDecode12:           push    esi
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgPointErr
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     al,[esi]
                        call    PrintHex2
                        add     esp,4
                        xor     eax,eax
                        inc     eax
                        ret
                        align   16
ICMPDecode13:           push    esi
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgIdentifier
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi]
                        call    PrintHex4
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgSeqNumber
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     ax,[esi+2]
                        call    PrintHex4
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgOrgStamp
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     eax,[esi+4]
                        call    PrintHex8
                        invoke  StdOut,addr MsgSeconds
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgRecStamp
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     eax,[esi+8]
                        call    PrintHex8
                        invoke  StdOut,addr MsgSeconds
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgTransStamp
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,[esp]
                        mov     eax,[esi+12]
                        call    PrintHex8
                        invoke  StdOut,addr MsgSeconds
                        add     esp,4
                        push    16
                        pop     eax
                        ret

; --------------- Wait for packets and display them
                        align   16
ReceiveThread           proc
                        local   AddToPacket:dword
                        ; Check amount of pending datas
                        invoke  ioctlsocket,hsocket,FIONREAD,addr RecBufLen
                        test    eax,eax
                        jz      SocketReadOk
                        ret
                        align   16
SocketReadOk:           mov     AddToPacket,eax         ; Clear it
                        mov     eax,RecBufLen
                        test    eax,eax
                        jz      NoDatas                 ; No datas received
                        inc     eax
                        mov     RecBufLen,eax
                        ; Alloc required memory
                        invoke  AllocMem,eax
                        test    eax,eax
                        jz      NoDatas
                        mov     RecBuf,eax
                        ; Clear buffer
                        invoke  FillMem,RecBuf,RecBufLen,0
                        ; Retrieve the datas
                        invoke  recv,hsocket,RecBuf,RecBufLen,0
                        test    eax,eax
                        jns     DisplayPacket
                        invoke  FreeMem,RecBuf
                        ret
                        align   16
DisplayPacket:          mov     RecBufLen,eax
                        ; --- Filtered ports
                        mov     esi,RecBuf
                        movzx   eax,[esi+IPHEADER.Protocol]
                        movzx   ebx,[ProtocolTable+FILTER_PROC_OFFSET+eax*8]
                        test    ebx,ebx
                        jz      NoFilterPacket
                        lea     eax,[esi+sizeof IPHEADER]
                        add     ebx,offset CodeBase
                        call    ebx
                        test    eax,eax
                        jz      CompleteLine
NoFilterPacket:         mov     al,ByteDecodeHeader
                        test    al,al
                        jnz     RawDirect
                        mov     eax,RecBufLen
                        add     eax,eax                 ; Buffer*2
                        add     eax,8                   ; Add len
                        inc     eax                     ; 0 ascii padding
                        invoke  AllocMem,eax
                        mov     RawAsciiBuf,eax
                        test    eax,eax
                        jz      CompleteLine
                        mov     edi,RawAsciiBuf
                        mov     eax,RecBufLen
                        call    StoreHex8               ; Length of the packet as hexa
                        mov     esi,RecBuf
                        mov     ecx,RecBufLen
                        xor     eax,eax
ConvertEntirePacket:    mov     al,[esi]
                        movzx   ebx,al
                        and     bl,0fh
                        mov     bl,[HexTbl+ebx]
                        mov     [edi+1],bl
                        and     eax,0f0h
                        shr     al,4
                        mov     bl,[HexTbl+eax]
                        mov     [edi],bl
                        inc     esi                     
                        add     edi,2
                        loop    ConvertEntirePacket
                        invoke  StdOut,RawAsciiBuf      ; Send it to output
                        jmp     CompleteLine
                        align   16
RawDirect:              invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgSeparator
                        invoke  PrintNow
                        mov     al,ByteDecodeHeader
                        cmp     al,1
                        je      DisplayShortHeader
                        invoke  StdOut,addr MsgPacketType
                        mov     esi,RecBuf
                        movzx   eax,[esi+IPHEADER.Protocol]
                        movzx   ebx,[ProtocolTable+PROTOCOL_NAME_OFFSET+eax*8]
                        add     ebx,offset VarBase
                        invoke  StdOut,ebx
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgOBracket
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,RecBuf
                        movzx   eax,[esi+IPHEADER.Protocol]
                        call    PrintHex2
                        invoke  StdOut,addr MsgCBracket
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgIPSource
                        mov     esi,RecBuf
                        mov     eax,[esi+IPHEADER.SourceAddr]
                        invoke  inet_ntoa,eax
                        invoke  StdOut,eax
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgIPDest
                        mov     esi,RecBuf
                        mov     eax,[esi+IPHEADER.DestAddr]
                        invoke  inet_ntoa,eax
                        invoke  StdOut,eax
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgHVersion
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,RecBuf
                        mov     al,[esi+IPHEADER.Version_and_HdrLen]
                        shr     al,4
                        call    PrintHex2
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgHLen
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,RecBuf
                        mov     al,[esi+IPHEADER.Version_and_HdrLen]
                        and     al,0fh
                        shl     al,2
                        mov     HdrL,al
                        call    PrintHex2
                        invoke  StdOut,addr MsgBytes
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgHeadChkSum
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,RecBuf
                        mov     ax,[esi+IPHEADER.HeaderChecksum]
                        xchg    al,ah
                        call    PrintHex4
                        invoke  StdOut,addr MsgCBracket
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgSPrecedence
                        mov     esi,RecBuf
                        movzx   eax,[esi+IPHEADER.ServiceType]
                        movzx   eax,[esi+IPHEADER.ServiceType]
                        shr     al,5
                        movzx   ebx,[PrecedenceTable+eax*2]
                        add     ebx,offset VarBase
                        invoke  StdOut,ebx
                        invoke  StdOut,addr MsgCRLF
                        mov     esi,RecBuf
                        movzx   eax,[esi+IPHEADER.ServiceType]
                        push    eax
                        lea     ebx,MsgNormDelay
                        bt      ax,4
                        jnc     ServiceDelay
                        lea     ebx,MsgLowDelay
ServiceDelay:           invoke  StdOut,ebx
                        invoke  StdOut,addr MsgSpace
                        mov     eax,[esp]
                        lea     ebx,MsgNormTrough
                        bt      ax,3
                        jnc     ServiceThroughput
                        lea     ebx,MsgHighTrough
ServiceThroughput:      invoke  StdOut,ebx
                        invoke  StdOut,addr MsgSpace
                        pop     eax
                        lea     ebx,MsgNormReliab
                        bt      ax,2
                        jnc     ServiceReliability
                        lea     ebx,MsgHighReliab
ServiceReliability:     invoke  StdOut,ebx
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgLTime
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,RecBuf
                        mov     al,[esi+IPHEADER.TimeToLive]
                        call    PrintHex2
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgPacketID
                        invoke  StdOut,addr MsgHexSign
                        mov     esi,RecBuf
                        mov     ax,[esi+IPHEADER.ID]
                        xchg    al,ah
                        call    PrintHex4
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgPacketFlags
                        mov     esi,RecBuf
                        movzx   eax,[esi+IPHEADER.Flags_and_FragOffset]
                        xchg    ah,al
                        lea     ebx,MsgNoFrag
                        bt      ax,14
                        jc      PossibleFragment
                        lea     ebx,MsgPosFrag
PossibleFragment:       push    eax
                        invoke  StdOut,ebx
                        pop     eax
                        bt      ax,14
                        jc      NoFragment
                        push    eax
                        invoke  StdOut,addr MsgSlash
                        pop     eax
                        lea     ebx,MsgMoreFrag
                        bt      ax,13
                        jc      LastFragment
                        lea     ebx,MsgLastFrag
LastFragment:           invoke  StdOut,ebx
                        invoke  StdOut,addr MsgFragOffset
                        invoke  StdOut,addr MsgHexSign
                        mov     ax,[esi+IPHEADER.Flags_and_FragOffset]
                        xchg    al,ah
                        and     ax,01fffh
                        call    PrintHex4
                        invoke  StdOut,addr MsgCBracket
NoFragment:             invoke  StdOut,addr MsgCRLF
                        mov     esi,RecBuf
                        movzx   eax,[esi+IPHEADER.Protocol]
                        movzx   ebx,[ProtocolTable+DECODER_PROC_OFFSET+eax*8]
                        test    ebx,ebx
                        jz      NoHeaderDecode
                        lea     eax,[esi+sizeof IPHEADER]
                        add     ebx,offset CodeBase
                        call    ebx
                        mov     AddToPacket,eax
                        invoke  StdOut,addr MsgCRLF
NoHeaderDecode:         invoke  StdOut,addr MsgCRLF
DisplayShortHeader:     mov     esi,RecBuf
                        movzx   eax,[esi+IPHEADER.TotalLen]
                        xchg    al,ah
                        sub     al,HdrL
                        sub     eax,AddToPacket
                        jz      CompleteLine            ; Empty line ?
                        push    eax
                        invoke  StdOut,addr MsgPacketDatas
                        invoke  StdOut,addr MsgSpace
                        invoke  StdOut,addr MsgOBracket
                        invoke  StdOut,addr MsgHexSign
                        pop     eax
                        call    PrintHex4
                        invoke  StdOut,addr MsgBytes
                        invoke  StdOut,addr MsgCBracket
                        invoke  StdOut,addr MsgCRLF
NoDisplaySize:          invoke  FillMem,addr HexLine,66,020202020h
                        movzx   ebx,HdrL
                        add     ebx,AddToPacket
                        mov     ecx,RecBufLen
                        sub     ecx,ebx
                        mov     esi,RecBuf
                        add     esi,ebx
                        xor     ebx,ebx
                        cmp     DisplayLen,0
                        je      DisplayPDats
                        cmp     DisplayLen,ecx
                        jae     DisplayPDats
                        mov     ecx,DisplayLen
DisplayPDats:           push    esi
                        push    ecx
                        push    ebx
                        movzx   eax,[ByteOffsetTableHex+ebx]
                        lea     edi,[HexLine+eax]
                        mov     al,[esi]
                        mov     CurrentByte,al
                        movzx   ebx,al
                        and     bl,0fh
                        mov     bl,[HexTbl+ebx]
                        mov     [edi+1],bl
                        and     eax,0f0h
                        shr     al,4
                        mov     bl,[HexTbl+eax]
                        mov     [edi],bl                ; Put hex byte
                        mov     ebx,[esp]
                        lea     edi,HexLine
                        movzx   eax,ByteAsciiPosition
                        add     ebx,eax
                        cmp     ebx,eax
                        jne     FirstQuote
                        mov     byte ptr [edi+ebx],'"'
FirstQuote:             inc     ebx
                        lea     eax,CurrentByte
                        mov     al,[eax]
                        mov     cl,"."
                        test    al,al
                        jnz     RemoveChar0
                        mov     al,cl
RemoveChar0:            cmp     al,7
                        jne     RemoveChar1
                        mov     al,cl
RemoveChar1:            cmp     al,8
                        jne     RemoveChar2
                        mov     al,cl
RemoveChar2:            cmp     al,9
                        jne     RemoveChar3
                        mov     al,cl
RemoveChar3:            cmp     al,10
                        jne     RemoveChar4
                        mov     al,cl
RemoveChar4:            cmp     al,13
                        jne     RemoveChar5
                        mov     al,cl
RemoveChar5:            mov     [edi+ebx],al
                        pop     ebx
                        inc     ebx
                        cmp     ebx,16
                        jne     ResetLine
                        mov     byte ptr [edi+65],'"'
                        invoke  StdOut,edi
                        invoke  FillMem,addr HexLine,66,020202020h
                        xor     ebx,ebx
ResetLine:              pop     ecx
                        pop     esi
                        inc     esi
                        dec     ecx
                        jnz     DisplayPDats
                        test    ebx,ebx
                        jz      CompleteLine
                        lea     edi,HexLine
                        mov     byte ptr [edi+65],'"'
                        invoke  StdOut,edi
CompleteLine:           invoke  FreeMem,RecBuf
NoDatas:                ret
ReceiveThread           endp

; --------------- Free the buffers
                        align   16
FreeBuffers             proc
                        invoke  FreeMem,CompleteIniFile
                        invoke  FreeMem,AppPath
                        invoke  FreeMem,UDPPortsTable
                        invoke  FreeMem,TCPPortsTable
                        invoke  FreeMem,CmdLineArgInterface
                        invoke  FreeMem,CmdLineArgHost
                        invoke  FreeMem,InterfaceBuf
                        ret
FreeBuffers             endp

; --------------- Alloc the needed buffers in memory
                        align   16
AllocBuffers            proc
                        invoke  AllocMem,512
                        mov     InterfaceBuf,eax
                        test    eax,eax
                        jnz     InterfaceBufferOk
                        invoke  RaiseError,addr MsgErrMemory
InterfaceBufferOk:      invoke  AllocMem,MAX_PATH
                        mov     CmdLineArgHost,eax
                        test    eax,eax
                        jnz     ArgHostBufferOk
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrMemory
ArgHostBufferOk:        invoke  AllocMem,MAX_PATH
                        mov     CmdLineArgInterface,eax
                        test    eax,eax
                        jnz     ArgInterfaceBufferOk
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrMemory
ArgInterfaceBufferOk:   invoke  AllocMem,65535          ; Table of allowed TCP ports
                        mov     TCPPortsTable,eax
                        test    eax,eax
                        jnz     AllocTCPPortsTable
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrPortsTable
AllocTCPPortsTable:     invoke  FillMem,eax,65535,0
                        invoke  AllocMem,65535          ; Table of allowed UDP ports
                        mov     UDPPortsTable,eax
                        test    eax,eax
                        jnz     AllocUDPPortsTable
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrPortsTable
AllocUDPPortsTable:     invoke  FillMem,eax,65535,0
                        invoke  AllocMem,MAX_PATH
                        mov     AppPath,eax
                        test    eax,eax
                        jnz     AllocAppPath
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrMemory
AllocAppPath:           invoke  AllocMem,MAX_PATH
                        mov     CompleteIniFile,eax
                        test    eax,eax
                        jnz     AllocCompleteIniFile
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrMemory
AllocCompleteIniFile:   ret
AllocBuffers            endp

; --------------- Close opened socket and cleanup winsock
                        align   16
CloseWinsock            proc
                        invoke  closesocket,hsocket
                        invoke  WSACleanup
                        invoke  FreeBuffers
                        ret
CloseWinsock            endp

; --------------- Try to resolve a host name
                        align   16
ResolveIP               proc    HostString:dword
                        ; --- Fill socket informations
                        invoke  inet_addr,HostString
                        inc     eax
                        jnz     AlreadyIP
                        ; --- Try to resolve hostname
                        invoke  gethostbyname,HostString
                        test    eax,eax                 ; Eax=hostent structure
                        jz      HostentError
                        mov     eax,[eax+12]            ; Get ip list address
                        mov     eax,[eax]               ; Get first pointer
                        mov     eax,[eax]               ; Get contained ip address
                        ret
                        align   16
HostentError:           invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrHost
AlreadyIP:              dec     eax
                        ret
ResolveIP               endp

; -------------------------
; --- Ini file handling ---
; -------------------------

; --------------- Read a value from the ini file
                        align   16
ReadIniValue            proc    KeySection:dword,KeyValue:dword,File:dword
                        invoke  AllocMem,4096
                        test    eax,eax
                        jz      CantReadIniValue
                        push    eax
                        invoke  GetPrivateProfileString,KeySection,KeyValue,0,eax,4096,File
                        pop     eax
CantReadIniValue:       ret
ReadIniValue            endp

; --------------- Read a boolean value from the ini file
                        align   16
ReadBoolIniValue        proc    KeySection:dword,KeyValue:dword
                        local   IniValue:dword
                        invoke  ReadIniValue,KeySection,KeyValue,CompleteIniFile
                        test    eax,eax
                        jz      EmptyBoolIniValue
                        mov     IniValue,eax
                        invoke  lstrlen,eax             ; Should be one char only
                        dec     eax
                        jnz     InvalidBoolLen
                        mov     eax,IniValue
                        mov     bl,[eax]                        
                        sub     bl,"0"                  ; Must be 0 or 1
                        movzx   eax,bl                  ; Save calculated value
                        jz      ValidBoolValue          ; (Z flag is sub's result)
                        dec     bl
                        jz      ValidBoolValue
InvalidBoolLen:         invoke  FreeMem,IniValue
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrInvalidIniVal
ValidBoolValue:         push    eax
                        invoke  FreeMem,IniValue
                        pop     eax
EmptyBoolIniValue:      ret
ReadBoolIniValue        endp

; --------------- Read a digit value from the ini file
                        align   16
ReadDigitIniValue       proc    KeySection:dword,KeyValue:dword
                        local   IniValue:dword
                        invoke  ReadIniValue,KeySection,KeyValue,CompleteIniFile
                        test    eax,eax
                        jz      EmptyDigitIniValue
                        mov     IniValue,eax
                        invoke  lstrlen,eax             ; Should be one char only
                        dec     eax
                        jnz     InvalidDigitLen
                        mov     eax,IniValue
                        mov     bl,[eax]                        
                        sub     bl,"0"                  ; Must be a single digit
                        movzx   eax,bl                  ; Save calculated value
                        jz      ValidDigitValue         ; (Z flag is sub's result)
                        sub     bl,9                    ; 0 to 9
                        jle     ValidDigitValue
InvalidDigitLen:        invoke  FreeMem,IniValue
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrInvalidIniVal
ValidDigitValue:        push    eax
                        invoke  FreeMem,IniValue
                        pop     eax
EmptyDigitIniValue:     ret
ReadDigitIniValue       endp

; --------------- Read a number value from the ini file
                        align   16
ReadNumberIniValue      proc    KeySection:dword,KeyValue:dword
                        local   IniValue:dword
                        invoke  ReadIniValue,KeySection,KeyValue,CompleteIniFile
                        test    eax,eax
                        jz      EmptyNumberIniValue
                        mov     IniValue,eax
                        invoke  lstrlen,eax             ; Should be at least one char
                        test    eax,eax
                        jz      InvalidNumberLen
                        mov     ecx,eax
                        mov     esi,IniValue
                        ; Check that each char is a digit
CheckNumberSanity:      invoke  IsDigit,byte ptr [esi]
                        test    al,al
                        jz      InvalidNumberLen
                        inc     esi
                        dec     ecx
                        jnz     CheckNumberSanity
                        ; Convert it now
                        invoke  atodw,IniValue
                        push    eax
                        invoke  FreeMem,IniValue
                        pop     eax
EmptyNumberIniValue:    ret
                        align   16
InvalidNumberLen:       invoke  FreeMem,IniValue
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrInvalidIniVal
ReadNumberIniValue      endp

; --------------- Handle console signals
                        align   16
ConSigHandler           proc    dwCtrlType:dword
                        invoke  CloseWinsock
                        ret
ConSigHandler           endp

; ---------------------------------
; --- Ini file parsing routines ---
; ---------------------------------

; --------------- Check if char is a space or a tab
                        align   16
IsBlank                 proc    Letter:byte
                        push    bx
                        mov     al,Letter
                        mov     bl,al
                        sub     al," "          ; Space char
                        setz    al
                        sub     bl,9            ; Tab char
                        setz    bl
                        sub     al,bl
                        setnz   al
                        pop     bx
                        ret
IsBlank                 endp

; --------------- Check if char is a minus sign
                        align   16
IsMinus                 proc    Letter:byte
                        push    bx
                        mov     al,Letter
                        sub     al,"-"
                        setz    al
                        pop     bx
                        ret
IsMinus                 endp

; --------------- Check if char is a digit
                        align   16
IsDigit                 proc    Letter:byte
                        push    bx
                        mov     al,Letter
                        mov     bl,al
                        sub     al,"0"
                        setge   al
                        sub     bl,"9"
                        setle   bl
                        sub     al,bl           ; Operation should be: -1+1=0
                        setz    al              ; Sets only if both conditions are true
                        pop     bx
                        ret
IsDigit                 endp

; --------------- Read a char from global line buffer
                        align   16
ReadChar                proc
                        mov     eax,PortsString
                        mov     al,[eax]
                        mov     CurrentChar,al
                        inc     PortsString
                        ret
ReadChar                endp

; --------------- Strip blank chars
                        align   16
StripBlanks             proc
                        mov     al,CurrentChar
PassBlanks:             invoke  IsBlank,al
                        test    al,al
                        jz      NoBlankChar
                        invoke  ReadChar
                        jmp     PassBlanks
NoBlankChar:            mov     al,CurrentChar
                        ret
StripBlanks             endp

; --------------- Retrieve a port number
                        align   16
GetEntity               proc
                        push    ebx
                        invoke  FillMem,addr DecBlock,20,0
                        xor     ebx,ebx
                        mov     al,CurrentChar
BlanksLoop:             invoke  IsDigit,al
                        test    al,al
                        jz      DoneEntity
                        mov     al,CurrentChar
                        mov     [DecBlock+ebx],al
                        inc     ebx
                        invoke  ReadChar
                        cmp     ebx,6                   ; Get only a word
                        jne     BlanksLoop
DoneEntity:             mov     al,CurrentChar
                        pop     ebx
                        ret
GetEntity               endp

; --------------- Parse ini file entries
                        align   16
ParsePorts              proc    IniKey:dword,PortsTable:dword
                        invoke  ReadIniValue,IniKey,addr MsgMonitorPorts,CompleteIniFile
                        test    eax,eax
                        jz      EmptyPortList
                        mov     PortsString,eax
                        invoke  lstrlen,eax
                        test    eax,eax
                        jz      EmptyPortList
                        mov     PortParsingBalance,0
                        invoke  ReadChar
StripPart:              invoke  StripBlanks
                        test    al,al                   ; End of line ?
                        jz      FilledPortsList
                        invoke  IsMinus,CurrentChar
                        test    al,al
                        jz      FirstMinus
                        invoke  ReadChar
                        mov     al,PortParsingBalance
                        test    al,al
                        jnz     FirstMinus
                        invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrPortGroup
FirstMinus:             invoke  GetEntity
                        test    al,al
                        jz      CorrectEntity
                        invoke  IsBlank,CurrentChar
                        test    al,al
                        jnz     CorrectEntity
                        invoke  IsMinus,CurrentChar
                        test    al,al
                        jnz     CorrectEntity
IllegalPort:            invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrIllegalPort
CorrectEntity:          invoke  lstrlen,addr DecBlock
                        test    eax,eax
                        jz      NoStorePort
                        invoke  atodw,addr DecBlock
                        cmp     eax,65535               ; Max port number
                        jg      IllegalPort
                        mov     bl,PortParsingBalance
                        test    bl,bl
                        jz      NotInPortsCouple
                        mov     ecx,CurrentParsedPort
                        cmp     ecx,eax
                        jg      SwapPorts
                        xchg    ecx,eax
SwapPorts:              mov     edi,PortsTable
                        add     edi,eax
                        sub     ecx,eax
                        inc     ecx
                        xor     eax,eax                 ; Allowed flag (1)
                        inc     eax
                        cld
MarkPorts:              stosb
                        loop    MarkPorts
                        mov     PortParsingBalance,0
                        jmp     StripPart
                        align   16
NotInPortsCouple:       mov     CurrentParsedPort,eax
                        invoke  StripBlanks
                        test    al,al                   ; End of line ?
                        jz      ActSinglePort
                        invoke  IsMinus,CurrentChar     ; Pass first minus sign
                        test    al,al
                        jz      ActSinglePort
                        mov     PortParsingBalance,1
                        jmp     StripPart
                        align   16
ActSinglePort:          ; --- Register a single port
                        mov     eax,PortsTable
                        add     eax,CurrentParsedPort
                        mov     byte ptr [eax],1        ; Allow it
                        mov     PortParsingBalance,0
NoStorePort:            jmp     StripPart
                        align   16
EmptyPortList:          ; --- Allow every ports if nothing found
                        invoke  FillMem,PortsTable,65535,001010101h
FilledPortsList:        invoke  FreeMem,PortsString
                        mov     al,PortParsingBalance
                        test    al,al
                        jz      LastBalanceState
                        invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrPortsBalance
LastBalanceState:       ret
ParsePorts              endp

; ---------------------
; --- Program start ---
; ---------------------
                        align   16
start:                  invoke  Main
                        invoke  ExitProcess,0

; --------------
; --- Main() ---
; --------------
                        align   16
Main                    proc
                        ; --- Locals structures
                        local   mysock:sockaddr_in
                        local   mywsa:WSADATA
                        local   BytesReceived:dword
                        local   InterfaceNbr:dword
                        local   Msg:MSG

                        ; --- Display Starting message
                        invoke  SetConsoleTitle,addr MsgTitle
                        invoke  StdOut,addr MsgTitle
                        invoke  StdOut,addr MsgCRLF
                        invoke  StdOut,addr MsgCreds

                        invoke  AllocBuffers

                        ; Note: PacketsView might not be executable remotly
                        invoke  GetAppPath,AppPath
                        invoke  szCatStr,CompleteIniFile,AppPath
                        invoke  szCatStr,CompleteIniFile,addr MsgIniFile

                        invoke  ArgClC,1,CmdLineArgHost
                        invoke  ArgClC,2,CmdLineArgInterface

                        ; Check command line arguments
                        invoke  StrLen,CmdLineArgHost
                        test    eax,eax
                        jz      NoHost  
                        ; Check if some help has been asked (/help or /?)
                        ; and print usage message if yes
                        invoke  lstrcmpi,CmdLineArgHost,addr MsgArgHelp1
                        test    eax,eax
                        jz      PrintUsage
                        invoke  lstrcmpi,CmdLineArgHost,addr MsgArgHelp2
                        test    eax,eax
                        jz      PrintUsage
                        jmp     NoHost
                        align   16
PrintUsage:             invoke  StdOut,addr MsgUsage
                        invoke  FreeBuffers
                        ret
                        align   16
NoHost:                 ; --- Set interface number (or first if not given)
                        mov     InterfaceNbr,1
                        invoke  StrLen,CmdLineArgInterface
                        test    eax,eax
                        jz      NoInterface
                        ; Convert it from ascii to dword
                        invoke  atodw,CmdLineArgInterface
                        mov     InterfaceNbr,eax
NoInterface:
                        ; --- Read main specifications from ini file
                        invoke  ReadBoolIniValue,addr MsgLayoutKey,addr MsgDisplayDate
                        mov     BolDisplayDate,al
                        invoke  ReadDigitIniValue,addr MsgLayoutKey,addr MsgDecodeHeader
                        mov     ByteDecodeHeader,al
                        invoke  ReadNumberIniValue,addr MsgLayoutKey,addr MsgDisplayBytes
                        mov     DisplayLen,eax
                        test    eax,eax                 ; (Must be >=0)
                        jns     DispBytesOk
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrBytes
DispBytesOk:
                        ; --- Init winsock (2.2 is needed here)
                        invoke  WSAStartup,0202h,addr mywsa
                        test    eax,eax
                        jz      WinsockInitOk
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrWinSock
WinsockInitOk:
                        ; --- Socket creation
                        invoke  WSASocket,AF_INET,SOCK_RAW,IPPROTO_IP,0,0,WSA_FLAG_OVERLAPPED
                        mov     hsocket,eax
                        test    eax,eax
                        jnz     OkSocket
                        invoke  WSACleanup
                        invoke  FreeBuffers
                        invoke  RaiseError,addr MsgErrSock
OkSocket:
                        ; --- Query the available addresses
                        invoke  WSAIoctl,hsocket,SIO_ADDRESS_LIST_QUERY,0,0,InterfaceBuf,512,addr BytesReceived,0,0
                        test    eax,eax
                        jz      QueryOk
                        invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrQuery
QueryOk:                mov     esi,InterfaceBuf
                        mov     eax,[esi]                       ; Number of interfaces
                        cmp     InterfaceNbr,0                  ; Must begin at 1
                        je      ErrInterfaceNumber
                        cmp     eax,InterfaceNbr
                        jae     InterfaceNumberOk
ErrInterfaceNumber:     invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrInvalidInterface
InterfaceNumberOk:      test    eax,eax
                        jnz     QueryNumberOk
                        invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrInterface
QueryNumberOk:  
                        ; --- Decode the given host
                        push    esi
                        ; (We can't rely on masm32lib to save the registers)
                        invoke  StrLen,CmdLineArgHost
                        pop     esi
                        test    eax,eax
                        jnz     FillHost
                        ; Subsitute the (not given) IP with the first one in the list
                        mov     eax,[esi+4]
                        mov     eax,[eax+4]                     ; sockaddr.sa_data
                        jmp     SubstituteHost
                        align   16
FillHost:               ; (Decode it if given)
                        invoke  ResolveIP,CmdLineArgHost
SubstituteHost:         mov     mysock.sin_addr.in_addr,eax     ; IP
                        mov     mysock.sin_family,AF_INET

                        ; --- Bind the socket
                        invoke  bind,hsocket,addr mysock,sizeof sockaddr_in
                        test    eax,eax
                        jz      BindSocketOk
                        invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrBind
BindSocketOk:
                        ; --- Initiate the socket for datagrams receptions
                        invoke  WSAIoctl,hsocket,SIO_RCVALL,addr InterfaceNbr,4,0,0,addr BytesReceived,0,0
                        test    eax,eax
                        jz      RecieveInitOk
                        invoke  CloseWinsock
                        invoke  RaiseError,addr MsgErrReceive
RecieveInitOk:
                        ; --- Parse given TCP/UDP ports lists
                        invoke  ParsePorts,addr MsgTCPPortKey,TCPPortsTable
                        invoke  ParsePorts,addr MsgUDPPortKey,UDPPortsTable

                        invoke  StdOut,addr MsgListening

                        ; --- Register a handler for signals response
                        invoke  SetConsoleCtrlHandler,addr ConSigHandler,1

                        ; --- Forever loop (Ctrl+C/Ctrl+Break to stop)
MainLoop:               invoke  Sleep,1                 ; Avoid 100% usage of cpu
                        invoke  ReceiveThread
                        jmp     MainLoop
Main                    endp

end start
