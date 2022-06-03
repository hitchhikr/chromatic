; --------------- Code section
                        .code

; --------------- Main program
Main                    proc    hInst:dword,CmdLine:dword
                        invoke  ClearScreen
                        invoke  SetConsoleTitle,ADDR AppTitle
                        ret
Main                    endp

