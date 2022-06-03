; Not defined in Windows.inc (as usual)
SERVICE_CONFIG_DESCRIPTION equ  1

SERVICE_DESCRIPTIONA    STRUCT
lpDescription           LPSTR   0
SERVICE_DESCRIPTIONA    ENDS

SERVICE_DESCRIPTION     TEXTEQU <SERVICE_DESCRIPTION>

; --------------- Service datas
                        .data
ServiceHandle           dd  0
ServicesDatabase        dd  0
ServiceCurrentStatus    dd  0
ServiceEvent            dd  0
hServiceThread          dd  0
ServiceStatus           dd  0
OsVer                   OSVERSIONINFO <>
ServiceDesc             dd  0
ServiceTable            SERVICE_TABLE_ENTRY <0,0>
                        SERVICE_TABLE_ENTRY <0,0>
ServiceStatusTable      SERVICE_STATUS <>
FileName                db  MAX_PATH + 1 dup (0)
ErrStartMsg             db  "Can't initialize control dispatcher.",0
ErrServiceDBMsg         db  "Can't open services database.",0
ErrCreateServiceMsg     db  "Can't create service.",0
ErrOpenServiceMsg       db  "Can't open service.",0
ErrRemoveServiceMsg     db  "Can't remove service.",0
ServiceInstalledMsg     db  "Service installed.",0
ServiceRemovedMsg       db  "Service removed.",0

; --------------- Service handling
                        .code

; --------------- Display an error and terminate process
RaiseError              proc    ErrorMsg:dword
                        invoke  MessageBox,0,ErrorMsg,addr ServiceName,MB_OK or MB_ICONERROR
                        ret
RaiseError              endp

; --------------- Display an informative message
RaiseInformation        proc    InfoMsg:dword
                        invoke  MessageBox,0,InfoMsg,addr ServiceName,MB_OK or MB_ICONINFORMATION
                        ret
RaiseInformation        endp

; --------------- Service entry point
; Must run 3 in ways with the same procedure:
; 1. Install
; 2. Start
; 3. Remove
WinMain                 proc    hInst:dword,hPrevInst:dword,CmdLine:dword,CmdShow:dword
                        invoke  OpenSCManager,0,0,SC_MANAGER_CREATE_SERVICE
                        mov     ServicesDatabase,eax
                        test    eax,eax
                        jnz     DatabaseObtained
                        invoke  RaiseError,addr ErrServiceDBMsg
                        xor     eax,eax
                        ret
DatabaseObtained:       ; Check if the service is in starting state
                        invoke  OpenService,ServicesDatabase,addr ServiceName,SERVICE_ALL_ACCESS
                        mov     ServiceHandle,eax
                        test    eax,eax
                        jz      InvalidService
                        invoke  QueryServiceStatus,ServiceHandle,addr ServiceStatusTable
                        test    eax,eax
                        jz      NoServiceState
                        mov     eax,ServiceStatusTable.dwCurrentState
                        ; Feed service manager with our thread if starting state
                        cmp     eax,SERVICE_START_PENDING
                        je      ServiceStartup
NoServiceState:         invoke  CloseServiceHandle,ServiceHandle
                        jmp     InstallProceed
InvalidService:         ; Call user procedure
                        call    ServiceStart
                        test    eax,eax
                        jnz     InstallProceed
                        invoke  CloseServiceHandle,ServicesDatabase
                        xor     eax,eax
                        ret
InstallProceed:         invoke  GetModuleFileName,0,addr FileName,MAX_PATH
                        ; Try to install
                        invoke  CreateService,ServicesDatabase,addr ServiceName,addr ServiceName,SERVICE_ALL_ACCESS,ServiceTypeFlag,ServiceStartFlag,SERVICE_ERROR_NORMAL,addr FileName,0,0,0,0,0
                        mov     ServiceHandle,eax
                        test    eax,eax
                        jnz     ServiceCreated
                        invoke  GetLastError
                        cmp     eax,ERROR_SERVICE_EXISTS
                        jne     ServiceAlreadyExists
                        ; Perform removal
                        invoke  OpenService,ServicesDatabase,addr ServiceName,SERVICE_ALL_ACCESS or DELETE
                        mov     ServiceHandle,eax
                        test    eax,eax
                        jnz     ServiceOpened
                        invoke  CloseServiceHandle,ServicesDatabase
                        invoke  RaiseError,addr ErrOpenServiceMsg
                        xor     eax,eax
                        ret
ServiceOpened:          invoke  QueryServiceStatus,ServiceHandle,addr ServiceStatusTable
                        mov     eax,ServiceStatusTable.dwCurrentState
                        cmp     eax,SERVICE_STOPPED
                        je      ServiceAlreadyStopped
                        invoke  ControlService,ServiceHandle,SERVICE_CONTROL_STOP,addr ServiceStatusTable
                        invoke  Sleep,500
ServiceAlreadyStopped:  ; Call user procedure
                        call    ServiceRemove
                        test    eax,eax
                        jnz     RemoveProceed
                        invoke  CloseServiceHandle,ServiceHandle
                        invoke  CloseServiceHandle,ServicesDatabase
                        xor     eax,eax
                        ret
RemoveProceed:          invoke  DeleteService,ServiceHandle
                        test    eax,eax
                        jnz     ServiceRemoved
                        invoke  CloseServiceHandle,ServiceHandle
                        invoke  CloseServiceHandle,ServicesDatabase
                        invoke  RaiseError,addr ErrRemoveServiceMsg
                        xor     eax,eax
                        ret
ServiceRemoved:         invoke  CloseServiceHandle,ServiceHandle
                        invoke  CloseServiceHandle,ServicesDatabase
                        invoke  RaiseInformation,addr ServiceRemovedMsg
                        xor     eax,eax
                        ret
ServiceAlreadyExists:   invoke  CloseServiceHandle,ServicesDatabase
                        invoke  RaiseError,addr ErrCreateServiceMsg
                        xor     eax,eax
                        ret
ServiceCreated:         mov     [OsVer.dwOSVersionInfoSize],sizeof OsVer;
                        invoke  GetVersionEx,addr OsVer
                        test    eax,eax
                        jz      CantObtainOSVersion
                        .if [OsVer.dwOSVersionInfoSize] >= 5
                            ; Add a description if OS >= Win2k
                            .if [OsVer.dwPlatformId] == VER_PLATFORM_WIN32_NT
                                mov     eax,offset ServiceDescription
                                mov     [ServiceDesc], eax
                                invoke  ChangeServiceConfig2, ServiceHandle, SERVICE_CONFIG_DESCRIPTION, addr ServiceDesc
                            .endif
                        .endif
CantObtainOSVersion:    .if ServiceStartRightNow != FALSE
                            invoke  StartService,ServiceHandle,0,0
                        .endif
                        invoke  CloseServiceHandle,ServiceHandle
                        invoke  CloseServiceHandle,ServicesDatabase
                        invoke  RaiseInformation,addr ServiceInstalledMsg
                        xor     eax,eax
                        ret
ServiceStartup:         invoke  CloseServiceHandle,ServiceHandle
                        invoke  CloseServiceHandle,ServicesDatabase
                        mov     ServiceTable.lpServiceName,offset ServiceName
                        mov     ServiceTable.lpServiceProc,offset ServiceMain
                        invoke  StartServiceCtrlDispatcher,addr ServiceTable
                        test    eax,eax
                        jnz     ServiceDispatch
                        invoke  RaiseError,addr ErrStartMsg
ServiceDispatch:        xor     eax,eax
                        ret
WinMain                 endp

; --------------- Initialize service thread
InitServiceThread       proc
                        local   ThreadID:dword
                        invoke  CreateThread,0,0,addr ServiceThread,0,0,addr ThreadID
                        mov     hServiceThread,eax
                        test    eax,eax
                        jz      Err_InitThread
                        xor     eax,eax
                        inc     eax
                        or      ServiceCurrentStatus,eax
Err_InitThread:         ret
InitServiceThread       endp

; --------------- Resume service
ResumeService:          and     ServiceCurrentStatus,0fffffffdh
                        invoke  ResumeThread,hServiceThread
                        ret

; --------------- Pause service
PauseService:           or  ServiceCurrentStatus,2
                        invoke  SuspendThread,hServiceThread
                        ret

; --------------- Stop service
StopService:            and ServiceCurrentStatus,0fffffffeh
                        invoke  SetEvent,ServiceEvent
                        ret

; --------------- Send message to system
SendStatus              proc    dwCurrentState:dword,dwWin32ExitCode:dword,dwServiceSpecificExitCode:dword,dwCheckPoint:dword,dwWaitHint:dword
                        mov     ServiceStatusTable.dwServiceType,SERVICE_WIN32_OWN_PROCESS
                        push    dwCurrentState
                        pop     ServiceStatusTable.dwCurrentState
                        cmp     dwCurrentState,SERVICE_START_PENDING
                        jne     SStatusStartPending
                        mov     ServiceStatusTable.dwControlsAccepted,0
                        jmp     CheckSStatusPending
SStatusStartPending:    mov     ServiceStatusTable.dwControlsAccepted,SERVICE_ACCEPT_STOP or SERVICE_ACCEPT_PAUSE_CONTINUE or SERVICE_ACCEPT_SHUTDOWN
CheckSStatusPending:    cmp     dwServiceSpecificExitCode,0
                        jne     SStatusSetExitCode
                        push    dwWin32ExitCode
                        pop     ServiceStatusTable.dwWin32ExitCode
                        jmp     CheckSStatusExitCode
SStatusSetExitCode:     mov     ServiceStatusTable.dwWin32ExitCode,ERROR_SERVICE_SPECIFIC_ERROR
CheckSStatusExitCode:   push    dwServiceSpecificExitCode
                        pop     ServiceStatusTable.dwServiceSpecificExitCode
                        push    dwCheckPoint
                        pop     ServiceStatusTable.dwCheckPoint
                        push    dwWaitHint
                        pop     ServiceStatusTable.dwWaitHint
                        invoke  SetServiceStatus,ServiceStatus,addr ServiceStatusTable
                        xor     eax,eax
                        inc     eax
                        ret
SendStatus              endp

; --------------- Terminate service
TerminateService        proc    ProvidedErr:dword
                        mov     eax,ServiceEvent
                        test    eax,eax
                        jz      NoEventToTerminate
                        push    eax
                        call    CloseHandle
NoEventToTerminate:     mov     eax,ServiceStatus
                        test    eax,eax
                        jz      NoWorkingService
                        invoke  SendStatus,SERVICE_STOPPED,ProvidedErr,0,0,0
NoWorkingService:       mov     eax,hServiceThread
                        test    eax,eax
                        jz      NoThreadToTerminate
                        push    eax
                        call    CloseHandle
NoThreadToTerminate:    xor     eax,eax
                        ret
TerminateService        endp

; --------------- Answer to system messages
CtrlHandler             proc    CtrlCode:dword
                        local   StatetoSend:dword
                        mov     StatetoSend,0
                        cmp     CtrlCode,SERVICE_CONTROL_STOP
                        jne     HandleServStop
                        invoke  SendStatus,SERVICE_STOP_PENDING,NO_ERROR,0,1,5000
                        call    StopService
                        mov     StatetoSend,SERVICE_STOPPED
                        jmp     SCHandler
HandleServStop:         cmp     CtrlCode,SERVICE_CONTROL_PAUSE
                        jne     HandleServPause
                        cmp     ServiceCurrentStatus,1
                        jne     HandleServPause         
                        invoke  SendStatus,SERVICE_PAUSE_PENDING,NO_ERROR,0,1,1000
                        call    PauseService
                        mov     StatetoSend,SERVICE_PAUSED
                        jmp     SCHandler
HandleServPause:        cmp     CtrlCode,SERVICE_CONTROL_CONTINUE
                        jne     HandleServResume
                        cmp     ServiceCurrentStatus,3
                        jne     HandleServResume
                        invoke  SendStatus,SERVICE_CONTINUE_PENDING,NO_ERROR,0,1,1000
                        call    ResumeService
                        mov     StatetoSend,SERVICE_RUNNING
                        jmp     SCHandler
HandleServResume:       cmp     CtrlCode,SERVICE_CONTROL_INTERROGATE
                        je      SCHandler
                        cmp     CtrlCode,SERVICE_CONTROL_SHUTDOWN
                        jne     SCHandler
                        ret
SCHandler:              invoke  SendStatus,StatetoSend,NO_ERROR,0,0,0
                        ret
CtrlHandler             endp

; --------------- Service main handler
ServiceMain             proc    ArgC:dword,ArgV:dword
                        invoke  RegisterServiceCtrlHandler,addr ServiceName,addr CtrlHandler
                        mov     ServiceStatus,eax
                        test    eax,eax
                        jnz     RegisteredCtrlHandler
                        call    GetLastError
                        push    eax
                        call    TerminateService
                        ret
RegisteredCtrlHandler:  invoke  SendStatus,SERVICE_START_PENDING,NO_ERROR,0,1,5000
                        invoke  CreateEvent,0,TRUE,FALSE,0
                        mov     ServiceEvent,eax
                        test    eax,eax
                        jnz     RegisteredEvent
                        call    GetLastError
                        push    eax
                        call    TerminateService
                        ret
RegisteredEvent:        invoke  SendStatus,SERVICE_START_PENDING,NO_ERROR,0,2,1000
                        invoke  SendStatus,SERVICE_START_PENDING,NO_ERROR,0,3,5000
                        call    InitServiceThread
                        test    eax,eax
                        jnz     RegisteredThread
                        call    GetLastError
                        push    eax
                        call    TerminateService
                        ret
RegisteredThread:       invoke  SendStatus,SERVICE_RUNNING,NO_ERROR,0,0,0
                        ; Endless loop
                        invoke  WaitForSingleObject,ServiceEvent,INFINITE
                        push    0
                        call    TerminateService
                        ret
ServiceMain             endp
