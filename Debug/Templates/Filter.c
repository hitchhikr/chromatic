/* ------------------------------------
Title: 

Author: 

Purpose: 

Notes: 

------------------------------------ */

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

/* --------------- DLL's functions */

/* --- FilterDescription()
   In: N/A
   Out: Can return filter description or NULL */
char * __stdcall FilterDescription()
{
    return(MsgDescription);
}

/* --- FilterAuthor()
   In: N/A
   Out: Can return author's name or NULL */
char * __stdcall FilterAuthor()
{
    return(MsgAuthor);
}

/* --- FilterProc()
   In: Filename: Name of the file
       File: pointer to a FILTERFILE containing file infos
       CL: CHROMATICLIB support entry point
   Out: Must return File or NULL */
LPFILTERFILE __stdcall FilterProc(char *FileName, LPFILTERFILE File, LPCHROMATICLIB CL)
{
    return(File);
}
