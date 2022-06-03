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

/* --- QEPlugIn()
hInst = instance handle
hMain = main window handle
hEd = rich edit control handle
hTool = toolbar handle (Chromatic will send NULL)
hStat = status bar handle (parts are slightly differents in Chromatic so beware) */

void __stdcall QePlugIn(HINSTANCE hInst, HWND hMain, HWND hEd, HWND hTool, HWND hStat)
{
	if(!hTool)
    {
		/* Running inside Chromatic */
	}
    else
    {
		/* Running inside QEditor */
	}
}
