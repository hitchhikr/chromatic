                        .data

_Dbl1                   real8   1.0

                        .code

; ------------------------------------------------------
; Name: Initialize()
; Desc: Perform the various initializations
Initialize              proc

                        invoke  glClearDepth, dword ptr [_Dbl1], dword ptr [_Dbl1 + 4]
                        invoke  glHint, GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST
                        invoke  glClearColor, CFLT(0.0), CFLT(0.0), CFLT(0.0), CFLT(1.0)
                        invoke  glEnable, GL_NORMALIZE
                        invoke  glEnable, GL_CULL_FACE
    
                        mov     eax,TRUE
                        ret
Initialize              endp

; ------------------------------------------------------
; Name: Deinitialize()
; Desc: Free allocated resources
Deinitialize            proc

                        ret
Deinitialize            endp

; ------------------------------------------------------
; Name: Update()
; Desc: Update the scene
Update                  proc    milliseconds:real4, TotalTime:real4

                        lea     eax,[g_keys + VK_ESCAPE]
                        mov     al,[eax]
                        test    al,al
                        jz      Esc_Pressed
                        invoke  Terminate_Application
Esc_Pressed:            ret
Update                  endp

; ------------------------------------------------------
; Name: Draw()
; Desc: Draw the scene
Draw                    proc
                        invoke  glClear, GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT

                        ret
Draw                    endp
