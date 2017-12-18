; --------------- Code section
                        .code

; --------------- Messages handling
WinProc                 proc    hWin:dword,uMsg:dword,wParam:dword,lParam:dword
                        .if uMsg == WM_CREATE
                            xor	eax,eax
                            ret
                        .elseif uMsg == WM_COMMAND
                            mov	eax,wParam
                            .if ax == 1000
                                invoke	SendMessage,hWin,WM_SYSCOMMAND,SC_CLOSE,NULL
                            .endif
                        .elseif uMsg == WM_CLOSE
                            invoke	MessageBox,hWin,addr szQuit,addr szWindowName,MB_YESNO or MB_ICONQUESTION
                            .if eax == IDNO
                                xor eax,eax
                                ret
                            .endif
                        .elseif uMsg == WM_DESTROY
                            invoke  PostQuitMessage,NULL
                            xor eax,eax
                            ret
                        .endif
                        invoke  DefWindowProc,hWin,uMsg,wParam,lParam
                        ret
WinProc                 endp
