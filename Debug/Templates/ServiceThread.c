/* Note: the produced file will auto-install or remove itself when run.
To start it go to the service manager of Windows NT/2000/XP. */

/* --------------- Perform tasks right before service effective creation
Out: 0 = Stop install process
     1 = Proceed with install
---------------
ServicesDatabase variable is initialized
--------------- */
long ServiceStart(void)
{
    return(1);
}

/* --------------- Perform tasks right before service effective removal
Out: 0 = Stop removal process
     1 = Proceed with removal
---------------
ServicesDatabase variable is initialized
ServiceHandle variable is initialized
--------------- */
long ServiceRemove(void)
{
    return(1);
}

/* --------------- Background Thread (infinite) of the service */
DWORD __stdcall ServiceThread(LPVOID Param)
{
    for(;;)
    {
        Sleep(1);
    }
    return(0);
}
