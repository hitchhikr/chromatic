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

/* --- LoadDll()
You can set mKeep member to FALSE to make mIRC unload the DLL after the call. */

void __stdcall LoadDll(LPMIRCLOADINFO LoadInfos)
{

}

/* --- UnloadDll()
mTimeout
0 UnloadDll() is being called due to a DLL being unloaded when mIRC exits,
or unloaded with /dll -u.

1 UnloadDll() is being called due to a DLL not being used for ten minutes.

The UnloadDll() routine can return 0 to keep the DLL loaded,
or 1 to allow it to be unloaded. */

int __stdcall UnloadDll(int mTimeout)
{
    return(1);
}

/* --- mIRCPlugIn()
This is just an example of mIrc typical function (Name of function should be changed)

mWnd
  handle to the main mIRC window.

aWnd
  handle of the window in which the command is being issued,
  this might not be the currently active window if the command is being called
  by a remote script.

data 
  the information that you wish to send to the DLL.
  On return, the DLL can fill this variable with the command it wants mIRC
  to perform if any.

parms
  is filled by the DLL on return with parameters that it wants mIRC to use
  when performing the command that it returns in the data variable.

show
  FALSE if the . prefix was specified to make the command quiet, or TRUE otherwise.

nopause
  TRUE if mIRC is in a critical routine and the DLL must not do anything that pauses
  processing in mIRC, eg. the DLL should not pop up a dialog.

The mIRCPlugIn() routine can return: 

0 means that mIRC should /halt processing.
1 means that mIRC should continue processing.
2 means that it has filled the data variable with a command
  which it wants mIRC to perform, and has filled parms with
  the parameters to use, if any, when performing the command.
3 means that the DLL has filled the data variable with the result
  that $dll() as an identifier should return. */

int __stdcall mIRCPlugIn(HWND mHwnd,
                         HWND aWnd,
                         char *data,
                         char *parms,
                         BOOL show,
                         BOOL nopause)
{
    return(0);
}
