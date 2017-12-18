/* --------------- DLL's entry point */
BOOL APIENTRY DllMain (HANDLE hModule,
                       unsigned long ul_reason_for_call,
                       LPVOID lpReserved)
{
    return TRUE;
}

/* --------------- DLL's Functions */

/* --- AddInDescription()
   In: N/A
   Out: Can return AddIn description or NULL */
char * __stdcall AddInDescription()
{
    return MsgDescription;
}

/* --- AddInAuthor()
   In: N/A
   Out: Can return author's name or NULL */
char * __stdcall AddInAuthor()
{
    return MsgAuthor;
}

/* --- AddInLoad()
   In: Pointer to WALIB structure
   Out: Must return ADDIN_FINISHED or ADDIN_PERSISTANT in eax */
unsigned long __stdcall AddInLoad(LPCHROMATICLIB CLStruct)
{
    ChromaticLib = CLStruct;
    return ADDIN_FINISHED;
}

/* --- AddInUnLoad()
   In: N/A
   Out: N/A */
void __stdcall AddInUnLoad()
{
    return;
}

/* --- AddInMenu()
   In: Pointer to CHROMATICLIB structure
   Out: Must return ADDIN_DIE or ADDIN_ZOMBIE in eax */
unsigned long __stdcall AddInMenu(LPCHROMATICLIB CLStruct)
{
    ChromaticLib = CLStruct;
    return ADDIN_DIE;
}
