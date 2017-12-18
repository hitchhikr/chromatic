; --- Write a text
WriteWinText            proc    WinHdc:dword,X:dword,Y:dword,Text:dword,TextColor:dword,hTextFont:dword
                        local   OldhObj:dword
                        local   OldBkMode:dword
                        invoke  SelectObject,WinHdc,hTextFont
                        mov     OldhObj,eax
                        invoke  SetTextColor,WinHdc,TextColor
                        invoke  SetBkMode,WinHdc,TRANSPARENT
                        mov     OldBkMode,eax
                        invoke  StrLen,Text
                        invoke  TextOut,WinHdc,X,Y,Text,eax
                        invoke  SetBkMode,WinHdc,OldBkMode
                        invoke  SelectObject,WinHdc,OldhObj
                        ret
WriteWinText            endp
