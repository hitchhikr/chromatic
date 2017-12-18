; --------------- Code section
                        .code

; --------------- DLL's entry point
LibMain                 proc    hInstance:HINSTANCE,reason:dword,reserved1:dword
                        .if     reason == DLL_PROCESS_ATTACH
                        .elseif reason == DLL_PROCESS_DETACH
                        .elseif reason == DLL_THREAD_ATTACH
                        .elseif reason == DLL_THREAD_DETACH
                        .endif
                        mov     eax,TRUE
                        ret
LibMain                 endp

; --------------- DLL's Functions

; --- LoadDll()
; You can set mKeep member to FALSE to make mIRC unload the DLL after the call.

LoadDll                 proc    LoadInfos:MIRCLOADINFO
                        ret
LoadDll                 endp

; --- UnloadDll()
; mTimeout
;  0 UnloadDll() is being called due to a DLL being unloaded when mIRC exits,
;  or unloaded with /dll -u.
;
;  1 UnloadDll() is being called due to a DLL not being used for ten minutes.
;
;  The UnloadDll() routine can return 0 to keep the DLL loaded,
;  or 1 to allow it to be unloaded.

UnloadDll               proc    mTimeout:dword
                        mov     eax,1
                        ret
UnloadDll               endp

; --- mIRCPlugIn()
; This is just an example of mIrc typical function (Name of function should be changed)
;
; mWnd
;  handle to the main mIRC window.
;
; aWnd
;  handle of the window in which the command is being issued,
;  this might not be the currently active window if the command is being called
;  by a remote script.
;
; data 
;  the information that you wish to send to the DLL.
;  On return, the DLL can fill this variable with the command it wants mIRC
;  to perform if any.
;
; parms
;  is filled by the DLL on return with parameters that it wants mIRC to use
;  when performing the command that it returns in the data variable.
;
; show
;  FALSE if the . prefix was specified to make the command quiet, or TRUE otherwise.
;
; nopause
;  TRUE if mIRC is in a critical routine and the DLL must not do anything that pauses
;  processing in mIRC, eg. the DLL should not pop up a dialog.
;
; The mIRCPlugIn() routine can return: 
;
; 0 means that mIRC should /halt processing.
; 1 means that mIRC should continue processing.
; 2 means that it has filled the data variable with a command
;   which it wants mIRC to perform, and has filled parms with
;   the parameters to use, if any, when performing the command.
; 3 means that the DLL has filled the data variable with the result
;   that $dll() as an identifier should return.

mIRCPlugIn              proc	mHwnd:dword,aWnd:dword,data:dword,parms:dword,show:dword,nopause:dword
                        xor     eax,eax
                        ret
mIRCPlugIn              endp
