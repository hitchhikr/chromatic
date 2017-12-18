CLIENT_ID				STRUCT					
UniqueProcess				DWORD	?
UniqueThread				DWORD	?
CLIENT_ID				ENDS
OBJ_INHERIT				equ	00000002H
OBJ_PERMANENT				equ	00000010H
OBJ_EXCLUSIVE				equ	00000020H
OBJ_CASE_INSENSITIVE			equ	00000040H
OBJ_OPENIF				equ	00000080H
OBJ_OPENLINK				equ	00000100H
OBJ_VALID_ATTRIBUTES			equ	000001F2H
OBJECT_ATTRIBUTES			STRUCT	
Length_					DWORD	?
RootDirectory				DWORD	?
ObjectName				DWORD	?
Attributes				DWORD	?
SecurityDescriptor			DWORD	?
SecurityQualityOfService		DWORD	?
OBJECT_ATTRIBUTES			ENDS
TIME_FIELDS				STRUC	DWORD
Year					WORD	?
Month					WORD	?
Day					WORD	?
Hour					WORD	?
Minute					WORD	?
Second					WORD	?
Milliseconds				WORD	?
Weekday					WORD	?
TIME_FIELDS				ENDS
ELAPSED_TIME_FIELDS			STRUCT	DWORD
Years					WORD	?
Months					WORD	?
Days					WORD	?
Hours					WORD	?
Minutes					WORD	?
Seconds					WORD	?
Milliseconds				WORD	?
ELAPSED_TIME_FIELDS			ENDS
CurrentThread				EQU	-2
NtCurrentProcess			EQU	-1
W9xCurrentProcess			EQU	7FFFFFFFH
LPWSTR					TYPEDEF	DWORD
UNICODE_STRING				STRUCT	
Length_					USHORT	?
MaximumLength				USHORT	?
Buffer					LPWSTR	?
UNICODE_STRING				ENDS
LDR_INITIALIZED				EQU	1
PEB_LDR_DATA				STRUCT
cbsize					DWORD	?
Flags					DWORD	?
Unknown8				DWORD	?
InLoadOrderModuleListHead		DWORD	?
PreviousInLoadOrderLdrEntry		DWORD	?
InMemoryOrderModuleListHead		DWORD	?
PreviousInMemoryOrderLdrEntry		DWORD	?
InInitializationOrderModuleListHead	DWORD	?
PreviousInInitializationOrderLdrEntry	DWORD	?
PEB_LDR_DATA				ENDS
LDRP_STATIC_LINK			equ	00000002H
LDRP_IMAGE_DLL				equ	00000004H
LDRP_LOAD_IN_PROGRESS			equ	00001000H
LDRP_UNLOAD_IN_PROGRESS			equ	00002000H
LDRP_ENTRY_PROCESSED			equ	00004000H
LDRP_ENTRY_INSERTED			equ	00008000H
LDRP_CURRENT_LOAD			equ	00010000H
LDRP_FAILED_BUILTIN_LOAD		equ	00020000H
LDRP_DONT_CALL_FOR_THREADS		equ	00040000H
LDRP_PROCESS_ATTACH_CALLED		equ	00080000H
LDRP_DEBUG_SYMBOLS_LOADED		equ	00100000H
LDRP_IMAGE_NOT_AT_BASE			equ	00200000H
LDRP_WX86_IGNORE_MACHINETYPE		equ	00400000H
LDR_ENTRY				STRUCT
NextInLoadOrderLdrEntry			DWORD	?
PreviousInLoadOrderLdrEntry		DWORD	?
NextInMemoryOrderLdrEntry		DWORD	?
PreviousInMemoryOrderLdrEntry		DWORD	?
NextInInitializationOrderLdrEntry	DWORD	?
PreviousInInitializationOrderLdrEntry	DWORD	?
ModuleBase				DWORD	?
EntryPoint				DWORD	?
ModuleSize				DWORD	?
ModuleFileName				UNICODE_STRING <>
ModuleBaseName				UNICODE_STRING <>
Flags					DWORD	?
LoadCount				WORD	?
TlsIndex				WORD	?
LdrpHashTableEntry0			DWORD	?
LdrpHashTableEntry1			DWORD	?
TimeStamp				DWORD	?
LDR_ENTRY				ENDS
PROCESS_PARAMETERS_NORMALIZED		equ	1
PROCESS_PARAMETERS			STRUCT
Unknown00				DWORD	?
Unknown04				DWORD	?
Flags					DWORD	?
Unknown0C				DWORD	?
CsrConsoleInfo				DWORD	?
Unknown14				DWORD	?
StdInputHandle				DWORD	?
StdOutputHandle				DWORD	?
StdErrorHandle				DWORD	?
CurrentDirectory			UNICODE_STRING <>
DirectoryFlags				DWORD	?
SearchPath				UNICODE_STRING <>
ImagePath				UNICODE_STRING <>
CommandLine				UNICODE_STRING <>
Environment				DWORD	?
Unknown4C				DWORD	?
Unknown50				DWORD	?
Unknown54				DWORD	?
Unknown58				DWORD	?
Unknown5C				DWORD	?
Unknown60				DWORD	?
Unknown64				DWORD	?
Unknown68				DWORD	?
Unknown6C				DWORD	?
WindowTitle				UNICODE_STRING <>
WindowStation				UNICODE_STRING <>
CommandLine2				UNICODE_STRING <>
Unknown88				DWORD	82H DUP (?)
PROCESS_PARAMETERS			ENDS
INHERITED_ADDRESS_SPACE			equ	00000001H
READ_IMAGEFILE_EXEC_OPTIONS		equ	00000100H
BEING_DEBUGGED				equ	00010000H
PEB					STRUCT
dwFlags					DWORD	?
Unknown04				DWORD	?
ImageBaseAddress			DWORD	?
PebLdrData				DWORD	?
ProcessParameters			DWORD	?
SubSystemData				DWORD	?
ProgramHeap				DWORD	?
LockingContext				DWORD	?
LockRoutine				DWORD	?
UnlockRoutine				DWORD	?
DirChange				DWORD	?
Unknown2C				DWORD	?
Unknown30				DWORD	?
Unknown34				DWORD	?
Unknown38				DWORD	?
Unknown3C				DWORD	?
Unknown40				DWORD	?
Unknown44				DWORD	?
Unknown48				DWORD	?
ProgramHeap02				DWORD	?
ProgramHeap02a				DWORD	?
InProgramHeap02				DWORD	?
AnsiCodePage				DWORD	?
OemCodePage				DWORD	?
UnicodeCodePage				DWORD	?
NumberProcessors			DWORD	?
GlobalFlag				DWORD	?
Unknown6C				DWORD	?
CritSectTimeout				DWORD	?
Unknown74				DWORD	?
HeapSegmentReserve			DWORD	?
HeapSegementCommit			DWORD	?
HeapDeCommitTotalFreeTreshold		DWORD	?
HeapDeCommitFreeBlockTreshold		DWORD	?
Unknown88				DWORD	?
Unknown8C				DWORD	?
Unknown90				DWORD	?
Unknown94				DWORD	?
Unknown98				DWORD	?
Unknown9C				DWORD	?
UnknownA0				DWORD	?
dwMajorVersion				DWORD	?
dwMinorVersion				DWORD	?
dwBuildNumber				WORD	?
CSDVersion				WORD	?
dwPlatformId				DWORD	?
Subsystem				DWORD	?
MajorSusbsytemVersion			DWORD	?
MinorSusbsytemVersion			DWORD	?
ProcessAffinityMask			DWORD	?
UnknownC4				DWORD	044H DUP (?)
SessionId				DWORD	?
Unknown1D8				DWORD	?
Unknown1DC				DWORD	?
Unknown1E0				DWORD	?
Unknown1E4				DWORD	?
PEB					ENDS
TEB					STRUCT
ExceptionList				DWORD	?
StackBase				DWORD	?
StackLimit				DWORD	?
SubSystemTib				DWORD	?
UNION
FiberData				DWORD	?
Version					DWORD	?
ENDS
ArbitraryUserPointer			DWORD	?
Self					DWORD	?
EnvironmentPtr				DWORD	?
ClientId				CLIENT_ID <>
RpcHandle				DWORD	?
TlsStorage				DWORD	?
pPEB					DWORD	?
LastErrorValue				DWORD	?
OwnedLocksCount				DWORD	?
Unknown3C				DWORD	?
Win32ThreadInfo				DWORD	?
Unknown44				DWORD	020H DUP (?)
Locale					DWORD	?
UnknownC8				DWORD	17BH DUP (?)
RealClientId				CLIENT_ID <>
Unknown6BC				DWORD	?
GdiClientId				CLIENT_ID <>
GdiThreadLocalInfo			DWORD	?
Win32ClientInfo				DWORD	?
Unknown6D0				DWORD	149H DUP (?)
LastStatusValue				DWORD	?
WindowStation				UNICODE_STRING <>
UnknownC00				DWORD	083H DUP (?)
ThreadStack				DWORD	?
UnknownE10				DWORD	043H DUP (?)
RpcTls					DWORD	?
UnknownF20				DWORD	?
UnknownF24				DWORD	?
HardErrorsMode				DWORD	?
UnknownF2C				DWORD	01BH DUP (?)
LogonLocale				DWORD	?
LogonLocale2				DWORD	?
UnknownFA0				DWORD	?
TEB					ENDS
UP_KUSER_SHARED_DATA			equ	7FFE0000H
KUSER_SHARED_DATA			STRUCT
TickCount				DWORD	?
TickCountMultiplier			DWORD	?
ElapsedTime				QWORD	?
ElapsedTimeCompareHigh			DWORD	?
SystemTime				QWORD	?
SystemTimeCompareHigh			DWORD	?
TimeZoneBias				QWORD	?
TimeZoneBiasCompareHigh			DWORD	?
ImageNumberLow				WORD	?
ImageNumberHigh				WORD	?
SystemRoot				WCHAR	MAX_PATH DUP (?)
Unknown238				DWORD	?
CryptoExponent				DWORD	?
TimeZoneId				DWORD	?
Unknown244				DWORD	25H DUP	(?)
KUSER_SHARED_DATA			ENDS
RPC_TLS					STRUCT
fAsync					DWORD	?
HandleToThread				DWORD	?
HandleToThreadEvent			DWORD	?
Context					DWORD	?
SecurityContext				DWORD	?
BufferCacheArray			DWORD	008H DUP (?)
CancelTimeout				DWORD	?
fCallCanceled				DWORD	?
Unknown3C				DWORD	?
ExtendedStatus				DWORD	?
SavedProcedure				DWORD	?
SavedParameter				DWORD	?
ActiveCall				DWORD	?
Unknown50				DWORD	004H DUP (?)
RPC_TLS					ENDS
SystemBasicInformation			equ	0
SYSTEM_BASIC_INFORMATION		STRUCT	DWORD
Machine					DWORD	?
MaximumIncrement			DWORD	?
PageSize				DWORD	?
PhysicalPages				DWORD	?
LowestPhysicalPage			DWORD	?
HighestPhysicalPage			DWORD	?
AllocationGranularity			DWORD	?
LowestUserAddress			DWORD	?
HighestUserAddress			DWORD	?
ActiveProcessors			DWORD	?
NumberProcessors			BYTE	?
SYSTEM_BASIC_INFORMATION		ENDS
SystemProcessorInformation		equ	1
SYSTEM_PROCESSOR_INFORMATION		STRUCT	DWORD
Architecture				WORD	?
Level					WORD	?
Revision				WORD	?
Reserved06				WORD	?
FeatureBits				DWORD	?
SYSTEM_PROCESSOR_INFORMATION		ENDS
FB_FASTSYSCALL				EQU	1000H
SystemPerformanceInformation		equ	2
SYSTEM_PERFORMANCE_INFORMATION		STRUCT	DWORD
IdleTime				QWORD	?
ReadTransferCount			QWORD	?
WriteTransferCount			QWORD	?
OtherTransferCount			QWORD	?
ReadOperationCount			DWORD	?
WriteOperationCount			DWORD	?
OtherOperationCount			DWORD	?
AvailablePages				DWORD	?
TotalCommittedPages			DWORD	?
TotalCommitLimit			DWORD	?
PeakCommitment				DWORD	?
MemInfoCounter00			DWORD	?
MemInfoCounter01			DWORD	?
MemInfoCounter02			DWORD	?
MemInfoCounter03			DWORD	?
MemInfoCounter04			DWORD	?
MemInfoCounter05			DWORD	?
MemInfoCounter06			DWORD	?
MemInfoCounter07			DWORD	?
MemInfoCounter08			DWORD	?
MemInfoCounter09			DWORD	?
MemInfoCounter10			DWORD	?
MemInfoCounter11			DWORD	?
MemInfoCounter12			DWORD	?
PagedPoolSize				DWORD	?
NonpagedPoolSize			DWORD	?
PoolUsage02				DWORD	?
PoolUsage03				DWORD	?
PoolUsage05				DWORD	?
PoolUsage06				DWORD	?
TotalFreeSystemPtes			DWORD	?
SystemCodePage				DWORD	?
TotalSystemDriverPages			DWORD	?
TotalSystemCodePages			DWORD	?
PoolUsage07				DWORD	?
PoolUsage04				DWORD	?
UnknownA0				DWORD	?
SystemCachePage				DWORD	?
PagedPoolUsage				DWORD	?
SystemDriverPage			DWORD	?
FastReadNoWait				DWORD	?
FastReadWait				DWORD	?
FastReadResourceMiss			DWORD	?
FastReadNotPossible			DWORD	?
FastMdlReadNoWait			DWORD	?
FastMdlReadWait				DWORD	?
FastMdlReadResourceMiss			DWORD	?
FastMdlReadNotPossible			DWORD	?
MapDataNoWait				DWORD	?
MapDataWait				DWORD	?
MapDataNoWaitMiss			DWORD	?
MapDataWaitMiss				DWORD	?
PinMappedDataCount			DWORD	?
PinReadNoWait				DWORD	?
PinReadWait				DWORD	?
PinReadNoWaitMiss			DWORD	?
PinReadWaitMiss				DWORD	?
CopyReadNoWait				DWORD	?
CopyReadWait				DWORD	?
CopyReadNoWaitMiss			DWORD	?
CopyReadWaitMiss			DWORD	?
MdlReadNoWait				DWORD	?
MdlReadWait				DWORD	?
MdlReadNoWaitMiss			DWORD	?
MdlReadWaitMiss				DWORD	?
ReadAheadIos				DWORD	?
LazyWriteIos				DWORD	?
LazyWritePages				DWORD	?
DataFlushes				DWORD	?
DataPages				DWORD	?
ContextSwaps				DWORD	?
PCRB4AC					DWORD	?
PCRB4B8					DWORD	?
SyscallsCalled				DWORD	?
SYSTEM_PERFORMANCE_INFORMATION		ENDS
SystemTimeInformation			equ	3
SYSTEM_TIME_INFORMATION			STRUCT
BootTime				QWORD	?
SystemTime				QWORD	?
TimeZoneBias				QWORD	?
CurrentTimeZoneId			QWORD	?
BootTimeBias				QWORD	?
InterruptTimeBias			QWORD	?
SYSTEM_TIME_INFORMATION			ENDS
SystemPathInformation			equ	4
SystemProcessInformation		equ	5
THREAD_INFO_OFFSET			EQU	024H
THREAD_INFO_SIZE			EQU	040H
NT4_PROCESS_INFO_SIZE			EQU	088H
NT5_PROCESS_INFO_SIZE			EQU	0B8H
TS_INITIALIZED				EQU	0
TS_READY				EQU	1
TS_RUNNING				EQU	2
TS_STANDBY				EQU	3
TS_TERMINATED				EQU	4
TS_WAITING				EQU	5
TS_TRANSITION				EQU	6
TS_MAX					EQU	7
WR_EXECUTIVE				EQU	0
WR_FREEPAGE				EQU	1
WR_PAGEIN				EQU	2
WR_POOLALLOC				EQU	3
WR_DELAYEXEC				EQU	4
WR_SUSPENDED				EQU	5
WR_USERREQUEST				EQU	6
WR_EXECUTIVE_				EQU	7
WR_FREEPAGE_				EQU	8
WR_PAGEIN_				EQU	9
WR_POOLALLOC_				EQU	10
WR_DELAYEXEC_				EQU	11
WR_SUSPENDED_				EQU	12
WR_USERREQUEST_				EQU	13
WR_EVENTPAIRHIGH			EQU	14
WR_EVENTPAIRLOW				EQU	15
WR_LPCRECEIVE				EQU	16
WR_LPCREPLY				EQU	17
WR_VIRTMEMORY				EQU	18
WR_PAGEOUT				EQU	19
WR_SPARE1				EQU	20
WR_SPARE2				EQU	21
WR_SPARE3				EQU	22
WR_SPARE4				EQU	23
WR_SPARE5				EQU	24
WR_SPARE6				EQU	25
WR_SPARE7				EQU	26
WR_MAX					EQU	27
SYSTEM_THREAD_INFORMATION		STRUCT
KernelTime				QWORD	?
UserTime				QWORD	?
CreateTime				QWORD	?
TickCount				DWORD	?
StartAddress				DWORD	?
ClientId				CLIENT_ID <>
DynamicPriority				DWORD	?
BasePriority				DWORD	?
ContextSwitches				DWORD	?
State					DWORD	?
WaitReason				DWORD	?
Reserved3C				DWORD	?
SYSTEM_THREAD_INFORMATION		ENDS
NT4_SYSTEM_PROCESS_INFORMATION		STRUCT
SizeOfBlock				DWORD	?
ThreadCount				DWORD	?
Reserved08				DWORD	06H DUP	(?)
CreateTime				QWORD	?
UserTime				QWORD	?
KernelTime				QWORD	?
ProcessName				UNICODE_STRING <>
Priority				DWORD	?
ProcessId				DWORD	?
ParentProcessId				DWORD	?
HandleCount				DWORD	?
SessionId				DWORD	?
Reserved54				DWORD	?
PeakVirtualSize				DWORD	?
VirtualSize				DWORD	?
PageFaultCount				DWORD	?
PeakWorkingSetSize			DWORD	?
WorkingSetSize				DWORD	?
QuotaPeakPagedPoolUsage			DWORD	?
QuotaPagedPoolUsage			DWORD	?
QuotaPeakNonPagedPoolUsage		DWORD	?
QuotaNonPagedPoolUsage			DWORD	?
PagefileUsage				DWORD	?
PeakPagefileUsage			DWORD	?
PrivatePages				DWORD	?
ThreadInformation			SYSTEM_THREAD_INFORMATION <>
NT4_SYSTEM_PROCESS_INFORMATION		ENDS
NT5_SYSTEM_PROCESS_INFORMATION		STRUCT
SizeOfBlock				DWORD	?
ThreadCount				DWORD	?
Reserved08				DWORD	06H DUP	(?)
CreateTime				QWORD	?
UserTime				QWORD	?
KernelTime				QWORD	?
ProcessName				UNICODE_STRING <>
Priority				DWORD	?
ProcessId				DWORD	?
ParentProcessId				DWORD	?
HandleCount				DWORD	?
SessionId				DWORD	?
Reserved54				DWORD	?
PeakVirtualSize				DWORD	?
VirtualSize				DWORD	?
PageFaultCount				DWORD	?
PeakWorkingSetSize			DWORD	?
WorkingSetSize				DWORD	?
QuotaPeakPagedPoolUsage			DWORD	?
QuotaPagedPoolUsage			DWORD	?
QuotaPeakNonPagedPoolUsage		DWORD	?
QuotaNonPagedPoolUsage			DWORD	?
PagefileUsage				DWORD	?
PeakPagefileUsage			DWORD	?
PrivatePages				DWORD	?
ReadOperationCount			QWORD	?
WriteOperationCount			QWORD	?
OtherOperationCount			QWORD	?
ReadTransferCount			QWORD	?
WriteTransferCount			QWORD	?
OtherTransferCount			QWORD	?
ThreadInformation			SYSTEM_THREAD_INFORMATION <>
NT5_SYSTEM_PROCESS_INFORMATION		ENDS
IFDEF	NT4
	SYSTEM_PROCESS_INFORMATION	TEXTEQU	<NT4_SYSTEM_PROCESS_INFORMATION>
ELSE
	SYSTEM_PROCESS_INFORMATION	TEXTEQU	<NT5_SYSTEM_PROCESS_INFORMATION>
ENDIF
SystemSyscallInformation		equ	9
SYSTEM_SYSCALL_INFORMATION		STRUCT
SizeOfBlock				DWORD	?
SyscallTables				DWORD	?
SizeOfTable00				DWORD	?
SYSTEM_SYSCALL_INFORMATION		ENDS
SystemGlobalFlagInformation		equ	9
GF_SOE					equ	00000001H
GF_SLS					equ	00000002H
GF_DIC					equ	00000004H
GF_SHG					equ	00000008H
GF_HTC					equ	00000010H
GF_HFC					equ	00000020H
GF_HPC					equ	00000040H
GF_HVC					equ	00000080H
GF_PTC					equ	00000100H
GF_PFC					equ	00000200H
GF_PTG					equ	00000400H
GF_HTG					equ	00000800H
GF_UST					equ	00001000H
GF_KST					equ	00002000H
GF_OTL					equ	00004000H
GF_HTD					equ	00008000H
GF_IDP					equ	00010000H
GF_D32					equ	00020000H
GF_KSL					equ	00040000H
GF_DPS					equ	00080000H
GF_HAT					equ	00100000H
GF_DHC					equ	00200000H
GF_ECE					equ	00400000H
GF_EEL					equ	00800000H
GF_EOT					equ	01000000H
GF_HPA					equ	02000000H
GF_DWL					equ	04000000H
GF_DDP					equ	08000000H
GF_ECC					equ	10000000H
GF_DPD					equ	80000000H
FLG_STOP_ON_EXCEPTION			equ	GF_SOE
FLG_SHOW_LDR_SNAPS			equ	GF_SLS
FLG_DEBUG_INITIAL_COMMAND		equ	GF_DIC
FLG_STOP_ON_HUNG_GUI			equ	GF_SHG
FLG_HEAP_ENABLE_TAIL_CHECK		equ	GF_HTC
FLG_HEAP_ENABLE_FREE_CHECK		equ	GF_HFC
FLG_HEAP_VALIDATE_PARAMETERS		equ	GF_HPC
FLG_HEAP_VALIDATE_ALL			equ	GF_HVC
FLG_POOL_ENABLE_TAIL_CHECK		equ	GF_PTC
FLG_POOL_ENABLE_FREE_CHECK		equ	GF_PFC
FLG_POOL_ENABLE_TAGGING			equ	GF_PTG
FLG_USER_STACK_TRACE_DB			equ	GF_UST
FLG_KERNEL_STACK_TRACE_DB		equ	GF_KST
FLG_MAINTAIN_OBJECT_TYPELIST		equ	GF_OTL
FLG_WIN95_MODE				equ	GF_HTD
FLG_IGNORE_DEBUG_PRIV			equ	GF_IDP
FLG_ENABLE_CSRDEBUG			equ	GF_D32
FLG_ENABLE_KDEBUG_SYMBOL_LOAD		equ	GF_KSL
FLG_DISABLE_PAGE_KERNEL_STACKS		equ	GF_DPS
FLG_VALID_BITS				equ	000FF7FFH
SYSTEM_GLOBALFLAG_INFORMATION		STRUCT
GlobalFlag				DWORD	?
SYSTEM_GLOBALFLAG_INFORMATION		ENDS
SystemLoadedModuleInformation		equ	11
SYSTEM_LOADEDMODULE			STRUCT
Unknown00				DWORD	?
Unknown04				DWORD	?
ModuleBase				DWORD	?
ModuleSize				DWORD	?
Unknown10				DWORD	?
Unknown14				DWORD	?
Unknown18				WORD	?
BaseNameOffset				WORD	?
ModuleFileName				CHAR	100H DUP (?)
SYSTEM_LOADEDMODULE			ENDS
SYSTEM_LOADEDMODULE_INFORMATION		STRUCT
ModuleCount				DWORD	?
ModuleInformation			SYSTEM_LOADEDMODULE <>
SYSTEM_LOADEDMODULE_INFORMATION		ENDS
SystemHandleInformation			equ	16
OB_TYPE_TYPE				equ	1
OB_TYPE_DIRECTORY			equ	2
OB_TYPE_SYMBOLIC_LINK			equ	3
OB_TYPE_TOKEN				equ	4
OB_TYPE_PROCESS				equ	5
OB_TYPE_THREAD				equ	6
OB_TYPE_EVENT				equ	7
OB_TYPE_EVENT_PAIR			equ	8
OB_TYPE_MUTANT				equ	9
OB_TYPE_SEMAPHORE			equ	10
OB_TYPE_TIMER				equ	11
OB_TYPE_PROFILE				equ	12
OB_TYPE_WINDOW_STATION			equ	13
OB_TYPE_DESKTOP				equ	14
OB_TYPE_SECTION				equ	15
OB_TYPE_KEY				equ	16
OB_TYPE_PORT				equ	17
OB_TYPE_ADAPTER				equ	18
OB_TYPE_CONTROLLER			equ	19
OB_TYPE_DEVICE				equ	20
OB_TYPE_DRIVER				equ	21
OB_TYPE_IO_COMPLETION			equ	22
OB_TYPE_FILE				equ	23
SYSTEM_HANDLE				STRUCT
ProcessId				DWORD	?
ObjectType				BYTE	?
Flags					BYTE	?
Handle					WORD	?
ObjectPointer				DWORD	?
Access					DWORD	?
SYSTEM_HANDLE				ENDS
SYSTEM_HANDLE_INFORMATION		STRUCT
HandleCount				DWORD	?
HandleInformation			SYSTEM_HANDLE <>
SYSTEM_HANDLE_INFORMATION		ENDS
SystemObjectInformation			equ	17
SYSTEM_OBJECT_INFORMATION		STRUCT
SYSTEM_OBJECT_INFORMATION		ENDS
SystemPageFileInformation		equ	18
SYSTEM_PAGEFILE_INFORMATION		STRUCT
SizeOfBlock				DWORD	?
CurrentSize				DWORD	?
TotalUsedPages				DWORD	?
PeakUsedPages				DWORD	?
PagefileName				UNICODE_STRING <>
SYSTEM_PAGEFILE_INFORMATION		ENDS
SystemDebuggerInformation		equ	35
SYSTEM_DEBUGGER_INFORMATION		STRUCT
KernelDebuggerEnabled			BYTE	?
KernelDebuggerNotPresent		BYTE	?
SYSTEM_DEBUGGER_INFORMATION		ENDS
SystemSingleProcessInformation		equ	53
SYSTEM_SESSIONPROCESS_INFORMATION	STRUCT
SessionId				DWORD	?
BufferSize				DWORD	?
Buffer					DWORD	?
SYSTEM_SESSIONPROCESS_INFORMATION	ENDS
ProcessBasicInformation			equ	0
PROCESS_BASIC_INFORMATION		STRUCT
ExitStatus				DWORD	?
PebBaseAddress				DWORD	?
AffinityMask				DWORD	?
BasePriority				DWORD	?
UniqueProcessId				DWORD	?
InheritedFromUniqueProcessId		DWORD	?
PROCESS_BASIC_INFORMATION		ENDS
ProcessQuotaLimits			equ	1
ProcessIoCounters			equ	2
IO_COUNTERS				STRUCT
ReadOperationCount			QWORD	?
WriteOperationCount			QWORD	?
OtherOperationCount			QWORD	?
ReadTransferCount			QWORD	?
WriteTransferCount			QWORD	?
OtherTransferCount			QWORD	?
IO_COUNTERS				ENDS
ProcessVmCounters			equ	3
VM_COUNTERS				STRUCT
PeakVirtualSize				DWORD	?
VirtualSize				DWORD	?
PageFaultCount				DWORD	?
PeakWorkingSetSize			DWORD	?
WorkingSetSize				DWORD	?
QuotaPeakPagedPoolUsage			DWORD	?
QuotaPagedPoolUsage			DWORD	?
QuotaPeakNonPagedPoolUsage		DWORD	?
QuotaNonPagedPoolUsage			DWORD	?
PagefileUsage				DWORD	?
PeakPagefileUsage			DWORD	?
VM_COUNTERS				ENDS
ProcessTimes				equ	4
KERNEL_USER_TIMES			STRUCT
CreateTime				QWORD	?
ExitTime				QWORD	?
KernelTime				QWORD	?
UserTime				QWORD	?
KERNEL_USER_TIMES			ENDS
ProcessPooledUsageAndLimits		equ	14
POOLED_USAGE_AND_LIMITS			STRUCT
PeakPagedPoolUsage			DWORD	?
PagedPoolUsage				DWORD	?
PagedPoolLimit				DWORD	?
PeakNonPagedPoolUsage			DWORD	?
NonPagedPoolUsage			DWORD	?
NonPagedPoolLimit			DWORD	?
PeakPagefileUsage			DWORD	?
PagefileUsage				DWORD	?
PagefileLimit				DWORD	?
POOLED_USAGE_AND_LIMITS			ENDS
ProcessWorkingSetWatch			equ	15
PROCESS_WS_WATCH_INFORMATION		STRUCT
FaultingPc				DWORD	?
FaultingVa				DWORD	?
PROCESS_WS_WATCH_INFORMATION		ENDS
ProcessDeviceMap			equ	23
PROCESS_DEVICEMAP_INFORMATION		STRUCT
DriveMap				DWORD	?
DriveType				WCHAR	32 DUP (?)
PROCESS_DEVICEMAP_INFORMATION		ENDS
ThreadBasicInformation			equ	0
THREAD_BASIC_INFORMATION		STRUCT
ExitStatus				DWORD	?
TebBaseAddress				DWORD	?
ClientId				CLIENT_ID <>
AffinityMask				DWORD	?
BasePriority				DWORD	?
DynamicPriority				DWORD	?
THREAD_BASIC_INFORMATION		ENDS
ABOVE2GB				EQU	08000000H
SE_MIN_WELL_KNOWN_PRIVILEGE		equ	2
SE_CREATE_TOKEN_PRIVILEGE		equ	2
SE_ASSIGNPRIMARYTOKEN_PRIVILEGE		equ	3
SE_LOCK_MEMORY_PRIVILEGE		equ	4
SE_INCREASE_QUOTA_PRIVILEGE		equ	5
SE_UNSOLICITED_INPUT_PRIVILEGE		equ	6
SE_MACHINE_ACCOUNT_PRIVILEGE		equ	6
SE_TCB_PRIVILEGE			equ	7
SE_SECURITY_PRIVILEGE			equ	8
SE_TAKE_OWNERSHIP_PRIVILEGE		equ	9
SE_LOAD_DRIVER_PRIVILEGE		equ	10
SE_SYSTEM_PROFILE_PRIVILEGE		equ	11
SE_SYSTEMTIME_PRIVILEGE			equ	12
SE_PROF_SINGLE_PROCESS_PRIVILEGE	equ	13
SE_INC_BASE_PRIORITY_PRIVILEGE		equ	14
SE_CREATE_PAGEFILE_PRIVILEGE		equ	15
SE_CREATE_PERMANENT_PRIVILEGE		equ	16
SE_BACKUP_PRIVILEGE			equ	17
SE_RESTORE_PRIVILEGE			equ	18
SE_SHUTDOWN_PRIVILEGE			equ	19
SE_DEBUG_PRIVILEGE			equ	20
SE_AUDIT_PRIVILEGE			equ	21
SE_SYSTEM_ENVIRONMENT_PRIVILEGE		equ	22
SE_CHANGE_NOTIFY_PRIVILEGE		equ	23
SE_REMOTE_SHUTDOWN_PRIVILEGE		equ	24
SE_UNDOCK_PRIVILEGE			equ	25
SE_SYNC_AGENT_PRIVILEGE			equ	26
SE_ENABLE_DELEGATION_PRIVILEGE		equ	27
SE_MAX_WELL_KNOWN_PRIVILEGE		equ	SE_ENABLE_DELEGATION_PRIVILEGE
ADJUST_CURRENT_PROCESS			equ	0
ADJUST_CURRENT_THREAD			equ	1
