/* --------------- Functions Declarations */
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv);

/* --------------- Service datas */
SC_HANDLE ServiceHandle;
SC_HANDLE ServicesDatabase;
OSVERSIONINFO OsVer;
DWORD ServiceCurrentStatus;
HANDLE ServiceEvent;
HANDLE hServiceThread;
SERVICE_DESCRIPTION ServiceDesc;
SERVICE_STATUS_HANDLE ServiceStatus;
SERVICE_TABLE_ENTRY ServiceTable[2];
SERVICE_STATUS ServiceStatusTable;
char FileName[MAX_PATH + 1];
char *ErrStartMsg = "Can't initialize control dispatcher.";
char *ErrServiceDBMsg = "Can't open services database.";
char *ErrCreateServiceMsg = "Can't create service.";
char *ErrOpenServiceMsg = "Can't open service.";
char *ErrRemoveServiceMsg = "Can't remove service.";
char *ServiceInstalledMsg = "Service installed.";
char *ServiceRemovedMsg = "Service removed.";

/* --------------- Display an error and terminate process */
void RaiseError(char *ErrorMsg)
{
    MessageBox(NULL, ErrorMsg, ServiceName, MB_OK | MB_ICONERROR);
    return;
}

/* --------------- Display an informative message */
void RaiseInformation(char *InfoMsg)
{
    MessageBox(NULL, InfoMsg, ServiceName, MB_OK | MB_ICONINFORMATION);
    return;
}

/* --------------- Service entry point
Must run 3 in ways with the same procedure:
1. Install
2. Start
3. Remove */
int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR lpCmdLine,
                     int nCmdShow)
{
    ServicesDatabase = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
    if(!ServicesDatabase)
    {
        RaiseError(ErrServiceDBMsg);
        return(0);
    }
    /* Check if the service is in starting state */
    ServiceHandle = OpenService(ServicesDatabase, ServiceName, SERVICE_ALL_ACCESS);
    if(!ServiceHandle)
    {
        /* Call user procedure */
        if(ServiceStart()) goto InstallProceed;
        CloseServiceHandle(ServicesDatabase);
        return(0);
    }
    if(QueryServiceStatus(ServiceHandle, &ServiceStatusTable))
    {
        /* Feed service manager with our thread if starting state */
        if(ServiceStatusTable.dwCurrentState == SERVICE_START_PENDING)
        {
            CloseServiceHandle(ServiceHandle);
            CloseServiceHandle(ServicesDatabase);
            ServiceTable[0].lpServiceName = ServiceName;
            ServiceTable[0].lpServiceProc = &ServiceMain;
            if(!StartServiceCtrlDispatcher(&ServiceTable[0])) RaiseError(ErrStartMsg);
            return(0);
        }
    }
    CloseServiceHandle(ServiceHandle);
InstallProceed: 
    GetModuleFileName(0, FileName, MAX_PATH);
    /* Try to install */
    ServiceHandle = CreateService(ServicesDatabase, ServiceName, ServiceName, SERVICE_ALL_ACCESS,
                                  ServiceTypeFlag, ServiceStartFlag, SERVICE_ERROR_NORMAL, FileName,
                                  NULL, NULL, NULL, NULL, NULL);
    if(ServiceHandle)
    {
        OsVer.dwOSVersionInfoSize = sizeof(OsVer);
        if(GetVersionEx(&OsVer) != 0)
        {
            if(OsVer.dwMajorVersion >= 5)
            {
                // Add a description if OS >= Win2k
                if(OsVer.dwPlatformId == VER_PLATFORM_WIN32_NT)
                {
                    ServiceDesc.lpDescription = ServiceDescription;
                    ChangeServiceConfig2(ServiceHandle, SERVICE_CONFIG_DESCRIPTION, &ServiceDesc);
                }
            }
        }
        if(ServiceStartRightNow) StartService(ServiceHandle, 0, 0);
        CloseServiceHandle(ServiceHandle);
        CloseServiceHandle(ServicesDatabase);
        RaiseInformation(ServiceInstalledMsg);
        return(0);
    }
    if(GetLastError() != ERROR_SERVICE_EXISTS)
    {
        CloseServiceHandle(ServicesDatabase);
        RaiseError(ErrCreateServiceMsg);
        return(0);
    }
    /* Perform removal */
    ServiceHandle = OpenService(ServicesDatabase, ServiceName, SERVICE_ALL_ACCESS | DELETE);
    if(!ServiceHandle)
    {
        CloseServiceHandle(ServicesDatabase);
        RaiseError(ErrOpenServiceMsg);
        return(0);
    }
    QueryServiceStatus(ServiceHandle, &ServiceStatusTable);
    if(ServiceStatusTable.dwCurrentState != SERVICE_STOPPED)
    {
        ControlService(ServiceHandle, SERVICE_CONTROL_STOP, &ServiceStatusTable);
        Sleep(500);
    }
    /* Call user procedure */
    if(!ServiceRemove())
    {
        CloseServiceHandle(ServiceHandle);
        CloseServiceHandle(ServicesDatabase);
        return(0);
    }
    if(DeleteService(ServiceHandle))
    {
        CloseServiceHandle(ServiceHandle);
        CloseServiceHandle(ServicesDatabase);
        RaiseInformation(ServiceRemovedMsg);
        return(0);
    }
    CloseServiceHandle(ServiceHandle);
    CloseServiceHandle(ServicesDatabase);
    RaiseError(ErrRemoveServiceMsg);
    return(0);
}

/* --------------- Initialize service thread */
long InitServiceThread(void)
{
    DWORD ThreadID;
    hServiceThread = CreateThread(0, 0, &ServiceThread, 0, 0, &ThreadID);
    if(hServiceThread)
    {
        ServiceCurrentStatus |= 1;
        return(1);
    }
    return(0);
}

/* --------------- Resume service */
void ResumeService(void)
{
    ServiceCurrentStatus &= 0xfffffffd;
    ResumeThread(hServiceThread);
    return;
}

/* --------------- Pause service */
void PauseService(void)
{
    ServiceCurrentStatus |= 2;
    SuspendThread(hServiceThread);
    return;
}

/* --------------- Stop service */
void StopService(void)
{
    ServiceCurrentStatus &= 0xfffffffe;
    SetEvent(ServiceEvent);
    return;
}
    
/* --------------- Send message to system */
long SendStatus(DWORD dwCurrentState,
                DWORD dwWin32ExitCode,
                DWORD dwServiceSpecificExitCode,
                DWORD dwCheckPoint,
                DWORD dwWaitHint)
{
    ServiceStatusTable.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatusTable.dwCurrentState = dwCurrentState;
    if(dwCurrentState == SERVICE_START_PENDING) ServiceStatusTable.dwControlsAccepted = 0;
    else ServiceStatusTable.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_PAUSE_CONTINUE | SERVICE_ACCEPT_SHUTDOWN;
    if(dwServiceSpecificExitCode == 0) ServiceStatusTable.dwWin32ExitCode = dwWin32ExitCode;
    else ServiceStatusTable.dwWin32ExitCode = ERROR_SERVICE_SPECIFIC_ERROR;
    ServiceStatusTable.dwServiceSpecificExitCode = dwServiceSpecificExitCode;
    ServiceStatusTable.dwCheckPoint = dwCheckPoint;
    ServiceStatusTable.dwWaitHint = dwWaitHint;
    SetServiceStatus(ServiceStatus, &ServiceStatusTable);
    return(1);
}

/* --------------- Terminate service */
long TerminateService(DWORD ProvidedErr)
{
    if(ServiceEvent) CloseHandle(ServiceEvent);
    if(ServiceStatus) SendStatus(SERVICE_STOPPED, ProvidedErr, 0, 0, 0);
    if(hServiceThread) CloseHandle(hServiceThread);
    return(0);
}

/* --------------- Answer to system messages */
void __stdcall CtrlHandler(DWORD CtrlCode)
{
    DWORD StatetoSend = 0;
    switch(CtrlCode)
    {
        case SERVICE_CONTROL_STOP:
            SendStatus(SERVICE_STOP_PENDING, NO_ERROR, 0, 1, 5000);
            StopService();
            StatetoSend = SERVICE_STOPPED;
            break;
        case SERVICE_CONTROL_PAUSE:
            if(ServiceCurrentStatus == 1)
            {
                SendStatus(SERVICE_PAUSE_PENDING, NO_ERROR, 0, 1, 1000);
                PauseService();
                StatetoSend = SERVICE_PAUSED;
            }
            break;
        case SERVICE_CONTROL_CONTINUE:
            if(ServiceCurrentStatus == 3)
            {
                SendStatus(SERVICE_CONTINUE_PENDING, NO_ERROR, 0, 1, 1000);
                ResumeService();
                StatetoSend = SERVICE_RUNNING;
            }
            break;
        case SERVICE_CONTROL_SHUTDOWN:
            return;
    }
    SendStatus(StatetoSend, NO_ERROR, 0, 0, 0);
}

/* --------------- Service main handler */
void WINAPI ServiceMain(DWORD dwArgc, LPTSTR *lpszArgv)
{
    ServiceStatus = RegisterServiceCtrlHandler(ServiceName, &CtrlHandler);
    if(!ServiceStatus)
    {
        TerminateService(GetLastError());
        return;
    }
    SendStatus(SERVICE_START_PENDING, NO_ERROR, 0, 1, 5000);
    ServiceEvent = CreateEvent(0, TRUE, FALSE, 0);
    if(!ServiceEvent)
    {
        TerminateService(GetLastError());
        return;
    }
    SendStatus(SERVICE_START_PENDING, NO_ERROR, 0, 2, 1000);
    SendStatus(SERVICE_START_PENDING, NO_ERROR, 0, 3, 5000);
    if(!InitServiceThread())
    {
        TerminateService(GetLastError());
        return;
    }
    SendStatus(SERVICE_RUNNING, NO_ERROR, 0, 0, 0);
    /* Endless loop */
    WaitForSingleObject(ServiceEvent, INFINITE);
    TerminateService(0);
    return;
}
