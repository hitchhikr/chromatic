/* ------------------------------------
Title: RawToDcb filter

Author: Franck Charlet

Purpose: Convert raw binary datas into assembly dc.b statements

Notes: Terminus filter

------------------------------------ */

/* --------------- DLL's entry point */

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
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
       File: pointer to a WAFILTERFILE containing file infos
       CL: CHROMATICLIB support entry point
   Out: Must return File or NULL */
LPFILTERFILE __stdcall FilterProc(char *FileName, LPFILTERFILE File, LPCHROMATICLIB CL)
{
    char *Output_Buffer;
    char *Output_Buffer_End;
    char *Output_Buffer_Final;
    char *FileName_Label;
    int File_Len = File->FileLength;
    int File_Len16 = File_Len / 16;
    int File_Remainder = File_Len % 16;

    /* Set buffer to max len */
    Output_Buffer = CL->StringString(File_Len16 * strlen(TabHex[0]), ' ');
    /* Remaining bytes */
    Output_Buffer_End = CL->StringString(strlen(TabHex[0]), ' ');
    /* Fill the buffer */
    CL->MiscCopyMemLoop(TabHex[0], Output_Buffer, strlen(TabHex[0]), 0, File_Len16);
    /* Fill the remaining buffer (TabHex is read only) */
    if(File_Remainder != 0) Output_Buffer_End = strdup(TabHex[16 - File_Remainder]);
    /* Convert main part */
    if(File_Len > 15) CL->StringFillHexBytes(File->FileMem, Output_Buffer, 16, 7, 7, File_Len16, 2);
    /* Convert remaining bytes */
    if(File_Remainder != 0) CL->StringFillHexBytes(File->FileMem + (File_Len - File_Remainder), Output_Buffer_End, File_Remainder, 7, 7, 1, 2);
    /* Write the header */
    Output_Buffer_Final = CL->StringCat("; Created from file: ", FileName);
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, "\r\n");
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, "; Original length: ");
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, CL->StringDecToString(File_Len));
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, " byte");
    if(File_Len > 1) Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, "s");
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, "\r\n\r\n");
    FileName_Label = CL->StringReplace(CL->FileReplaceExtension(CL->FileGetFileName(FileName), ""), ".", "", 1, -1, NOT_CASE_SENSITIVE);
    CL->StringFileNameToLabel(FileName_Label);
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, FileName_Label);
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, "_Size\tequ\t");
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, CL->StringDecToString(File_Len));
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, "\r\n");
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, FileName_Label);
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, "_Dats");
    /* Write main buffer */
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, Output_Buffer);
    /* Write remaining bytes */
    Output_Buffer_Final = CL->StringCat(Output_Buffer_Final, CL->StringRTrim(Output_Buffer_End));
    /* Set new buffer */
    File->FileMem = Output_Buffer_Final;
    File->FileLength = strlen(Output_Buffer_Final);
    if(File_Remainder != 0) free(Output_Buffer_End);
    return(NULL);
}
