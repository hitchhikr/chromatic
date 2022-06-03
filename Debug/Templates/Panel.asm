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

; --------------- CPlApplet()
CPlApplet               proc    hWndCPl:dword,uMsg:dword,lParam1:dword,lParam2:dword
                        .if	uMsg == CPL_INIT
                            ; Global levels allocations
                            mov eax,1
                            .elseif	uMsg == CPL_GETCOUNT
                            ; Return the number of dialog boxes
                            ; min=1 so windows will send at least one CPL_INQUIRE
                            mov eax,1
                        .elseif	uMsg == CPL_INQUIRE
                            ; uAppNum = (UINT) lParam1;lpcpli = (LPCPLINFO) lParam2;
                            ; (Fill lParam2 here for windows to cache cpl datas)
                            mov eax,lParam2
                            mov [eax+CPLINFO.lData],0
                            mov [eax+CPLINFO.idIcon],ICON_APP
                            mov [eax+CPLINFO.idName],0	; Name (from stringtable)
                            mov [eax+CPLINFO.idInfo],1	; Comment
                            xor eax,eax
                        .elseif	uMsg == CPL_NEWINQUIRE
                            ; uAppNum = (UINT) lParam1;lpncpli = (LPNEWCPLINFO) lParam2;
                            xor eax,eax
                        .elseif uMsg == CPL_DBLCLK
                            ; uAppNum = (UINT) lParam1;lData = (LONG) lParam2;
                            xor eax,eax
                        .elseif	uMsg == CPL_STOP
                            ; uAppNum = (UINT) lParam1;lData = (LONG) lParam2;
                            xor eax,eax
                        .elseif	uMsg == CPL_STARTWPARMS
                            ; uAppNum = (UINT) lParam1;lpszExtra = (LPCTSTR) lParam2;
                            mov eax,1
                        .elseif	uMsg == CPL_EXIT
                            ; Global levels cleanup
                            xor eax,eax
                        .endif
                        ret
CPlApplet               endp
