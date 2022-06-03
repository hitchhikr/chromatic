; --- Draw a line
DrawLine        proc    WinHdc:dword,X1:dword,Y1:dword,X2:dword,Y2:dword,LineColor:dword
                local   Pt:POINT
                local   hPen:dword
                local   OldhPen:dword
                invoke  CreatePen,PS_SOLID,0,LineColor
                mov     hPen,eax
                invoke  MoveToEx,MainHdc,X1,Y1,ADDR Pt
                invoke  SelectObject,WinHdc,hPen
                mov     OldhPen,eax
                invoke  LineTo,WinHdc,X2,Y2
                invoke  SelectObject,WinHdc,OldhPen
                invoke  DeleteObject,hPen
                ret
DrawLine        endp
