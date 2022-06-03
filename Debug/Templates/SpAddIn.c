/* --------------- DLL's entry point */
BOOL APIENTRY DllMain(HANDLE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved)
{
    return TRUE;
}

/* --------------- DLL's Functions */

/* --- SpAddInDescription()
   In: N/A
   Out: Can return description string in eax or 0 */
char * __stdcall SpAddInDescription()
{
    return MsgDescription;
}

/* --- SpAddInAuthor()
   In: N/A
   Out: Can return author string in eax or 0 */
char * __stdcall SpAddInAuthor()
{
    return MsgAuthor;
}

/* --- SpAddInLoad()
   In: Pointer to WALIB structure
   Out: Must return ADDIN_FINISHED or ADDIN_PERSISTANT in eax */
DWORD __stdcall SpAddInLoad(LPCHROMATICLIB CLStruct)
{
    ChromaticLib = CLStruct;
    return ADDIN_FINISHED;
}

/* --- SpAddInUnLoad()
   In: N/A
   Out: N/A */
void __stdcall SpAddInUnLoad()
{
    return;
}

/* --- SpAddInAskResource()
   In: Code number of asked resource
   Out: Return value in eax (depends on asked resource) */
DWORD __stdcall SpAddInAskResource(DWORD AskedResourceNumber)
{
    return 0;
}
