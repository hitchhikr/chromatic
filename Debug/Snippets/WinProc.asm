; --- Standard WinProc
WndProc                 proc    hWin:dword,uMsg:dword,wParam:dword,lParam:dword
                        .if     uMsg == WM_CREATE
                        .elseif uMsg == WM_COMMAND
                        .elseif uMsg == WM_CLOSE
                                invoke  DestroyWindow,hWin
                                return  0
                        .elseif uMsg == WM_DESTROY
                                invoke  PostQuitMessage,0
                                return  0
                        .endif
                        invoke  DefWindowProc,hWin,uMsg,wParam,lParam
                        ret
WndProc                 endp
