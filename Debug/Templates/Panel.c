/* --------------- DLL's entry point */

BOOL APIENTRY DllMain(HANDLE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
        case DLL_PROCESS_ATTACH:
            break;
        case DLL_THREAD_ATTACH:
            break;
        case DLL_THREAD_DETACH:
            break;
        case DLL_PROCESS_DETACH:
            break;
    }
    return TRUE;
}

/* --------------- DLL's Functions */

/* --------------- CPlApplet() */

LRESULT __stdcall CPlApplet(HWND hWndCPl,
                            UINT uMsg,
                            LPARAM lParam1,
                            LPARAM lParam2)
{
    LPCPLINFO LocCPLINFO;

    switch(uMsg)
    {
        case CPL_INIT:
            // Global levels allocations
            return(1);
        case CPL_GETCOUNT:
            // Return the number of dialog boxes
            // min=1 so windows will send at least one CPL_INQUIRE
            return(1);
        case CPL_INQUIRE:
            // uAppNum = (UINT) lParam1;lpcpli = (LPCPLINFO) lParam2;
            // (Fill lParam2 here for windows to cache cpl datas)
            LocCPLINFO = (LPCPLINFO) lParam2; 
            LocCPLINFO->lData = 0;
            LocCPLINFO->idIcon = ICON_APP;
            // Name (from stringtable)
            LocCPLINFO->idName = 0;	
            // Comment
            LocCPLINFO->idInfo = 1;
            return(0);
        case CPL_NEWINQUIRE:
            // uAppNum = (UINT) lParam1;lpncpli = (LPNEWCPLINFO) lParam2;
            return(0);
        case CPL_DBLCLK:
            // uAppNum = (UINT) lParam1;lData = (LONG) lParam2;
            return(0);
        case CPL_STOP:
            // uAppNum = (UINT) lParam1;lData = (LONG) lParam2;
            return(0);
        case CPL_STARTWPARMS:
            // uAppNum = (UINT) lParam1;lpszExtra = (LPCTSTR) lParam2;
            return(1);
        case CPL_EXIT:
            // Global levels cleanup
            return(0);
        default:
            return(0);
    }
}
