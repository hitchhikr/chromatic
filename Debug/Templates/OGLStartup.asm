                        .code

; ------------------------------------------------------
; Name: Terminate_Application()
; Desc: Initiate termination procedure
Terminate_Application   proc
                        invoke  PostMessage, dword ptr [g_window + GL_Window.hWnd], WM_QUIT, 0, 0
                        mov     g_isProgramLooping,FALSE
                        ret
Terminate_Application   endp

; ------------------------------------------------------
; Name: Reshape_GL()
; Desc: Resize the opengl viewport
Reshape_GL              proc    w:dword, h:dword
                        local   Ratio:real8

                        invoke  glViewport, 0, 0, w, h
                        invoke  glMatrixMode, GL_PROJECTION
                        invoke  glLoadIdentity
                        fild    dword ptr [w]
                        fidiv   dword ptr [h]
                        fstp    qword ptr [Ratio]
                        invoke  gluPerspective, dword ptr [_Dbl45], dword ptr [_Dbl45 + 4], dword ptr [Ratio], dword ptr [Ratio + 4], dword ptr [_Dbl01], dword ptr [_Dbl01 + 4], dword ptr [_Dbl100], dword ptr [_Dbl100 + 4]
                        invoke  glMatrixMode, GL_MODELVIEW
                        invoke  glLoadIdentity
                        ret
Reshape_GL              endp

; ------------------------------------------------------
; Name: Change_Screen_Resolution()
; Desc: Resize the screen with given dimensions and depth
Change_Screen_Resolution proc   w:dword, h:dword, bitsPerPixel:dword
                        local   dmScreenSettings:DEVMODE

                        invoke  RtlZeroMemory,addr dmScreenSettings, sizeof DEVMODE
                        mov     dmScreenSettings.dmSize, sizeof DEVMODE
                        mov     eax,w
                        mov     dmScreenSettings.dmPelsWidth,eax
                        mov     eax,h
                        mov     dmScreenSettings.dmPelsHeight,eax
                        mov     eax,bitsPerPixel
                        mov     dmScreenSettings.dmBitsPerPel,eax
                        mov     dmScreenSettings.dmFields, 040000h or 080000h or 0100000h
                        invoke  ChangeDisplaySettings,addr dmScreenSettings, 4
                        .if eax != DISP_CHANGE_SUCCESSFUL
                            xor     eax,eax
                        .else
                            mov     eax,TRUE
                        .endif
                        ret
Change_Screen_Resolution endp

; -----------------------------------------------------------------------
; Name: Render_On_Screen()
; Desc: Select the main screen as rendering target
Render_On_Screen        proc
                        invoke  wglMakeCurrent, dword ptr [g_window + GL_Window.hDC], dword ptr [g_window + GL_Window.hRC]
                        ret
Render_On_Screen        endp

; -----------------------------------------------------------------------
; Name: Create_Window_GL()
; Desc: Create the main window
Create_Window_GL        proc
                        local   msg:MSG
                        local   windowStyle:DWORD
                        local   windowExtendedStyle:DWORD
                        local   wwidth:dword
                        local   wheight:dword
                        local   wtop:dword
                        local   wleft:dword
                        local   PixelFormat:dword
            
                        invoke  MessageBox, 0, addr g_AskFullScreen, dword ptr [g_window + GL_Window.init.wintitle], MB_YESNOCANCEL or MB_ICONQUESTION
                        .if eax == IDYES
                            mov     g_FullScreen, TRUE
                        .elseif eax == IDNO
                        .elseif eax == IDCANCEL
                            xor     eax,eax
                            ret
                        .endif
            
; Force window mode if constant _DEBUG is defined
ifdef _DEBUG
                        mov     g_FullScreen, FALSE
endif
                        .if g_FullScreen == TRUE
                            mov     windowStyle, WS_POPUP
                            mov     windowExtendedStyle, WS_EX_TOPMOST
                        .else
                            mov     windowStyle, WS_BORDER or WS_CAPTION or WS_SYSMENU or WS_POPUP or WS_MINIMIZEBOX
                            mov     windowExtendedStyle, WS_EX_APPWINDOW
                        .endif

                        mov     eax,dword ptr [g_window + GL_Window.init.bitsPerPixel]
                        mov     [pfd.cColorBits],al

                        mov     eax,dword ptr [g_window + GL_Window.init.winwidth]
                        mov     [wwidth],eax
                        mov     eax,dword ptr [g_window + GL_Window.init.winheight]
                        mov     [wheight],eax
                        mov     dword ptr [wtop], 0
                        mov     dword ptr [wleft], 0
                        invoke  GetSystemMetrics, SM_CXSCREEN
                        .if wwidth > eax
                            mov     wwidth,eax
                        .endif
                        invoke  GetSystemMetrics, SM_CYSCREEN
                        .if wheight > eax
                            mov     wheight,eax
                        .endif
            
                        .if g_FullScreen == 0
                            ; Center the window if necessary
                            invoke  GetSystemMetrics, SM_CXSCREEN
                            .if wwidth < eax
                                sub     eax,wwidth
                                shr     eax,1
                                mov     wleft,eax
                            .endif
            
                            invoke  GetSystemMetrics, SM_CYSCREEN
                            .if wheight < eax
                                sub     eax,wheight
                                shr     eax,1
                                mov     wtop,eax
                            .endif
                            ; Add some pixels around to make the inner part to fit the dimensions requirements
                            invoke  GetSystemMetrics, SM_CXBORDER
                            add     wwidth,eax
                            invoke  GetSystemMetrics, SM_CYBORDER
                            add     wheight,eax
                            invoke  GetSystemMetrics, SM_CYCAPTION
                            add     wheight,eax
                        .endif
            
                        or  windowStyle,WS_VISIBLE
                        invoke  CreateWindowEx, windowExtendedStyle, \
                                                dword ptr [g_window + GL_Window.init.application.className], \
                                                dword ptr [g_window + GL_Window.init.wintitle], \
                                                windowStyle, \
                                                wleft, wtop, \
                                                wwidth, \
                                                wheight, \
                                                HWND_DESKTOP, \
                                                0, \
                                                dword ptr [g_window + GL_Window.init.application.hInstance], \
                                                addr g_window
                        mov     dword ptr [g_window + GL_Window.hWnd],eax
                        .if eax == 0
                            ret
                        .endif
                        invoke  PeekMessage, addr msg, 0, 0, 0, PM_REMOVE
                        .if eax != 0
                            invoke  TranslateMessage, addr msg
                            invoke  DispatchMessage, addr msg
                        .endif
                        invoke  ShowCursor, g_ShowCursor
                        .if g_FullScreen != 0
                            invoke  Change_Screen_Resolution, dword ptr [g_window + GL_Window.init.winwidth], dword ptr [g_window + GL_Window.init.winheight], dword ptr [g_window + GL_Window.init.bitsPerPixel]
                            .if eax == 0
                                ret
                            .endif
                            invoke  MoveWindow, dword ptr [g_window + GL_Window.hWnd], 0, 0, dword ptr [g_window + GL_Window.init.winwidth], dword ptr [g_window + GL_Window.init.winheight], 1
                        .endif
                        invoke  GetDC, dword ptr [g_window + GL_Window.hWnd]
                        mov     dword ptr [g_window + GL_Window.hDC],eax
                        .if eax == 0
                            invoke  DestroyWindow, dword ptr [g_window + GL_Window.hWnd]
                            mov     dword ptr [g_window + GL_Window.hWnd], 0
                            xor     eax,eax
                            ret
                        .endif
                        invoke  ChoosePixelFormat, dword ptr [g_window + GL_Window.hDC], addr pfd
                        mov     PixelFormat,eax
                        .if eax == 0
                            invoke  ReleaseDC, dword ptr [g_window + GL_Window.hWnd], dword ptr [g_window + GL_Window.hDC]
                            mov     dword ptr [g_window + GL_Window.hDC], 0
                            invoke  DestroyWindow, dword ptr [g_window + GL_Window.hWnd]
                            mov     dword ptr [g_window + GL_Window.hWnd], 0
                            xor     eax,eax
                            ret
                        .endif
                        invoke  SetPixelFormat, dword ptr [g_window + GL_Window.hDC], [PixelFormat], addr pfd
                        .if eax == FALSE
                            invoke  ReleaseDC, dword ptr [g_window + GL_Window.hWnd], dword ptr [g_window + GL_Window.hDC]
                            mov     dword ptr [g_window + GL_Window.hDC], 0
                            invoke  DestroyWindow, dword ptr [g_window + GL_Window.hWnd]
                            mov     dword ptr [g_window + GL_Window.hWnd], 0
                            xor     eax,eax
                            ret
                        .endif
                        invoke  wglCreateContext, dword ptr [g_window + GL_Window.hDC]
                        mov     dword ptr [g_window + GL_Window.hRC],eax 
                        .if eax == 0
                            invoke  ReleaseDC, dword ptr [g_window + GL_Window.hWnd], dword ptr [g_window + GL_Window.hDC]
                            mov     dword ptr [g_window + GL_Window.hDC], 0
                            invoke  DestroyWindow, dword ptr [g_window + GL_Window.hWnd]
                            mov     dword ptr [g_window + GL_Window.hWnd], 0
                            xor     eax,eax
                            ret
                        .endif
                        invoke  Render_On_Screen
                        .if eax == FALSE
                            invoke  wglDeleteContext, dword ptr [g_window + GL_Window.hRC]
                            mov     dword ptr [g_window + GL_Window.hRC], 0
                            invoke  ReleaseDC, dword ptr [g_window + GL_Window.hWnd], dword ptr [g_window + GL_Window.hDC]
                            mov     dword ptr [g_window + GL_Window.hDC], 0
                            invoke  DestroyWindow, dword ptr [g_window + GL_Window.hWnd]
                            mov     dword ptr [g_window + GL_Window.hWnd], 0
                            xor     eax,eax
                            ret
                        .endif
                        mov     dword ptr [g_window + GL_Window.isVisible], TRUE
                        invoke  Reshape_GL, dword ptr [g_window + GL_Window.init.winwidth], dword ptr [g_window + GL_Window.init.winheight]
                        invoke  RtlZeroMemory,addr g_keys, 256
                        mov     eax,TRUE
                        ret
Create_Window_GL    endp

; -----------------------------------------------------------------------
; Name: WindowProc()
; Desc: Messages handler for the main window
WindowProc          proc    hWnd:dword, uMsg:UINT, wParam:WPARAM, lParam:LPARAM
                    local   creation:CREATESTRUCT
            
                    .if uMsg == WM_SYSCOMMAND
                        .if wParam == SC_SCREENSAVE || wParam == SC_MONITORPOWER
                            xor     eax,eax
                            ret
                        .endif
                    .elseif uMsg == WM_CREATE
                        xor     eax,eax
                        ret
                    .elseif uMsg == WM_CLOSE
                        invoke  Terminate_Application
                        xor     eax,eax
                        ret
                    .elseif uMsg == WM_ACTIVATE
                        .if g_FullScreen != 0
                            mov     eax,wParam
                            and     eax,0ffffh
                            mov     ecx,eax
                            and     eax,WA_ACTIVE
                            and     ecx,WA_CLICKACTIVE
                            .if eax != 0 || ecx != 0
                                invoke  Change_Screen_Resolution, dword ptr [g_window + GL_Window.init.winwidth], dword ptr [g_window + GL_Window.init.winheight], dword ptr [g_window + GL_Window.init.bitsPerPixel]
                                invoke  GetSystemMetrics, SM_CXSCREEN
                                push    eax
                                invoke  GetSystemMetrics, SM_CYSCREEN
                                mov     ecx,eax
                                pop     eax
                                invoke  SetWindowPos, hWnd, HWND_TOPMOST, 0, 0, eax, ecx, SWP_SHOWWINDOW
                                mov     dword ptr [g_window + GL_Window.isVisible],TRUE
                            .else
                                invoke  ChangeDisplaySettings, NULL, 0
                                invoke  SetWindowPos, hWnd, HWND_BOTTOM, 0, 0, 0, 0, SWP_SHOWWINDOW
                                invoke  ShowWindow,hWnd, SW_MINIMIZE
                                mov     dword ptr [g_window + GL_Window.isVisible],FALSE
                            .endif
                        .endif
                    .elseif uMsg == WM_SIZE
                        .if wParam == SIZE_MINIMIZED
                            mov     dword ptr [g_window + GL_Window.isVisible],FALSE
                        .elseif wParam == SIZE_MAXIMIZED
                            mov     dword ptr [g_window + GL_Window.isVisible],TRUE
                        .elseif wParam == SIZE_RESTORED
                            mov     dword ptr [g_window + GL_Window.isVisible],TRUE
                        .endif
                    .elseif uMsg == WM_KEYDOWN
                        .if wParam >= 0 && wParam <= 255
                            lea     eax,g_keys
                            mov     ecx,wParam
                            mov     byte ptr [eax + ecx], TRUE
                            xor     eax,eax
                            ret
                        .endif
                    .elseif uMsg == WM_MOUSEMOVE
                        .if g_ShowCursor == TRUE
                            mov     g_ShowCursor, FALSE
                            invoke  ShowCursor, g_ShowCursor
                        .endif
                    .elseif uMsg == WM_NCMOUSEMOVE
                        .if g_ShowCursor == FALSE
                            mov     g_ShowCursor, TRUE
                            invoke  ShowCursor, g_ShowCursor
                        .endif
                    .elseif uMsg == WM_KEYUP
                        .if wParam >= 0 && wParam <= 255
                            lea     eax,g_keys
                            mov     ecx,wParam
                            mov     byte ptr [eax + ecx], FALSE
                            xor     eax,eax
                            ret
                        .endif
                    .endif
                    invoke  DefWindowProc, hWnd, uMsg, wParam, lParam
                    ret
WindowProc          endp

; -----------------------------------------------------------------------
;/ Name: Register_Window_Class()
; Desc: Register the class for the main window
Register_Window_Class proc
                    local   windowClass:WNDCLASSEX
                    local   SubEditBrush:LOGBRUSH

                    invoke  RtlZeroMemory,addr windowClass, sizeof WNDCLASSEX
                    lea     edi,windowClass
                    mov     dword ptr [edi + WNDCLASSEX.cbSize], sizeof WNDCLASSEX
                    mov     dword ptr [edi + WNDCLASSEX.style], CS_HREDRAW or CS_VREDRAW or CS_OWNDC
                    push    offset WindowProc
                    pop     dword ptr  [edi + WNDCLASSEX.lpfnWndProc]
                    push    dword ptr [g_window + GL_Window.init.application.hInstance]
                    pop     dword ptr [edi + WNDCLASSEX.hInstance]
                    invoke  LoadIcon, dword ptr [g_window + GL_Window.init.application.hInstance], ICON_APP
                    mov     [edi + WNDCLASSEX.hIcon],eax
                    mov     [edi + WNDCLASSEX.hIconSm],eax
                    mov     dword ptr [SubEditBrush + LOGBRUSH.lbStyle],BS_SOLID
                    mov     dword ptr [SubEditBrush + LOGBRUSH.lbHatch],0
                    mov     dword ptr [SubEditBrush + LOGBRUSH.lbColor],0
                    invoke  CreateBrushIndirect, addr SubEditBrush
                    mov     [edi + WNDCLASSEX.hbrBackground],eax
                    invoke  LoadCursor, NULL, IDC_ARROW
                    mov     [edi + WNDCLASSEX.hCursor],eax
                    push    dword ptr [g_window + GL_Window.init.application.className]
                    pop     dword ptr [edi + WNDCLASSEX.lpszClassName]
                    invoke  RegisterClassEx, addr windowClass
                    .if eax == 0
                        ret
                    .endif
                    mov     eax,TRUE
                    ret
Register_Window_Class endp

; -----------------------------------------------------------------------
; Name: WinMain()
; Desc: Program entry point
WinMain             proc    hInst:dword,hPrevInst:dword,CmdLine:dword,CmdShow:dword
                    local   msg:MSG
                    local   LastT:real4
                    local   FirstT:real4
        
                    push    offset g_ClassName
                    pop     dword ptr [g_window + GL_Window.init.application.className]
        
                    push    offset g_WindowName
                    pop     dword ptr [g_window + GL_Window.init.wintitle]
        
                    push    hInst
                    pop     dword ptr [g_window + GL_Window.init.application.hInstance]
        
                    mov     dword ptr [g_window + GL_Window.init.winwidth], SCREEN_WIDTH
                    mov     dword ptr [g_window + GL_Window.init.winheight], SCREEN_HEIGHT
                    mov     dword ptr [g_window + GL_Window.init.bitsPerPixel], SCREEN_BPP
        
                    invoke  Register_Window_Class
                    .if eax == 0
                        ret
                    .endif
                    
                    invoke  Create_Window_GL
                    .if eax == TRUE
                        invoke  Initialize
                        .if eax == FALSE
                            invoke  Terminate_Application
                        .else
                            ; init counter
                            invoke  QueryPerformanceFrequency, addr HTimerVal
                            fild    qword ptr [HTimerVal.QuadPart]
                            fmul    dword ptr [_Flt0001]
                            fistp   qword ptr [HTimerFreq]
                            invoke  QueryPerformanceCounter, addr HTimerVal
                            fild    qword ptr [HTimerVal.QuadPart]
                            fistp   qword ptr [g_window + GL_Window.LastTickCount]
                            fild    qword ptr [HTimerVal.QuadPart]
                            fistp   qword ptr [g_window + GL_Window.FirstTickCount]
                            mov     msg.message, 0
                            .while  msg.message != WM_QUIT
                                invoke  PeekMessage, addr msg, dword ptr [g_window + GL_Window.hWnd], 0, 0, PM_REMOVE
                                .if eax != 0
                                    invoke  DispatchMessage, addr msg
                                .else
                                    .if dword ptr [g_window + GL_Window.isVisible] == FALSE
                                        invoke  WaitMessage
                                    .else
                                        invoke  QueryPerformanceCounter, addr HTimerVal
        
                                        fild    qword ptr [HTimerVal.QuadPart]
                                        fild    qword ptr [g_window + GL_Window.LastTickCount]
                                        fsubp   st(1),st(0)
                                        fild    qword ptr [HTimerFreq]
                                        fdivp   st(1),st(0)
                                        fmul    dword ptr [_Flt0001]
                                        fstp    dword ptr [LastT]
        
                                        fild    qword ptr [HTimerVal.QuadPart]
                                        fild    qword ptr [g_window + GL_Window.FirstTickCount]
                                        fsubp   st(1),st(0)
                                        fild    qword ptr [HTimerFreq]
                                        fdivp   st(1),st(0)
                                        fmul    dword ptr [_Flt0001]
                                        fstp    dword ptr [FirstT]
                                        
                                        invoke  Update, LastT, FirstT
                                        fild    qword ptr [HTimerVal.QuadPart]
                                        fistp   qword ptr [g_window + GL_Window.LastTickCount]
                                        invoke  Draw
                                        invoke  SwapBuffers, dword ptr [g_window + GL_Window.hDC]
                                    .endif
                                .endif
                            .endw
                        .endif
                        invoke  Deinitialize
                        invoke  ChangeDisplaySettings, NULL, 0
                        .if dword ptr [g_window + GL_Window.hWnd] != 0
                            .if dword ptr [g_window + GL_Window.hDC] != 0
                                invoke  wglMakeCurrent, dword ptr [g_window + GL_Window.hDC], 0
                                .if dword ptr [g_window + GL_Window.hRC] != 0
                                    invoke  wglDeleteContext, dword ptr [g_window + GL_Window.hRC]
                                .endif
                                invoke  ReleaseDC, dword ptr [g_window + GL_Window.hWnd], dword ptr [g_window + GL_Window.hDC]
                            .endif
                            invoke  DestroyWindow, dword ptr [g_window + GL_Window.hWnd]
                            mov     dword ptr [g_window + GL_Window.hWnd], 0
                        .endif
                        invoke  ShowCursor, TRUE
                    .else
                        mov     g_isProgramLooping,FALSE
                    .endif
                    invoke  UnregisterClass, dword ptr [g_window + GL_Window.init.application.className], dword ptr [g_window + GL_Window.init.application.hInstance]
                    xor     eax,eax
                    ret
WinMain             endp
