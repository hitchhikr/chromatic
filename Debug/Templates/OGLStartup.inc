SCREEN_WIDTH 		    equ	    800
SCREEN_HEIGHT		    equ	    600
SCREEN_BPP		        equ	    16

; ------------------------------------------------------
; structures

literal_float		    MACRO	lit_float:VARARG
                        LOCAL	local_float
			            .data
local_float		        real4	lit_float
			            .code
			            EXITM	<local_float>
			            ENDM

CFLT			        MACRO	lit_float:VARARG
	            		EXITM	<literal_float(lit_float)>
            			ENDM

; ------------------------------------------------------
; structures

Application		        struct
hInstance		        dword	?
className		        dword	?
Application		        ends

GL_WindowInit		    struct
application		        Application <>
wintitle		        dword	?
winwidth		        dword	?
winheight		        dword	?
bitsPerPixel		    dword	?
isFullScreen		    dword	?
GL_WindowInit		    ends

GL_Window		        struct
hWnd			        dword	?
hDC			            dword	?
hRC			            dword	?
init			        GL_WindowInit <>
isVisible		        dword	?
LastTickCount		    LONGLONG ?
FirstTickCount		    LONGLONG ?
GL_Window		        ends

; ------------------------------------------------------
; Variables
			            .data
			
_FullScreen		        DWORD	0
g_window		        GL_Window <>
g_keys			        db	256 dup (FALSE)
g_ClassName		        db	"OGLWindow",0
g_WindowName		    db	"OpenGL window",0
g_AskFullScreen		    db	"Run in fullscreen ?",0
g_isProgramLooping	    dd	0
g_FullScreen		    dd	0
g_ShowCursor		    dd	0
_Flt0001		        real4	0.001
_Dbl45			        real8	45.0
_Dbl01			        real8	0.1
_Dbl100			        real8	100.0
pfd			            PIXELFORMATDESCRIPTOR <	sizeof PIXELFORMATDESCRIPTOR, \
						                        1, \
                        						04h or 020h or 1, \
                        						0, \
                        						0, \
                        						0, 0, 0, 0, 0, 0, \
                        						0, \
                        						0, \
                        						0, \
                        						0, 0, 0, 0, \
                        						16, \
                        						0, \
                        						0, \
                        						0, \
                        						0, \
                        						0, 0, 0 >
HTimerVal		        LARGE_INTEGER <>
HTimerFreq		        LONGLONG 0

; ------------------------------------------------------
; Public functions
Terminate_Application	proto
Initialize		        proto
Deinitialize		    proto
Update			        proto	milliseconds:real4, TotalTime:real4
Render_On_Screen	    proto
Draw			        proto
Reshape_GL		        proto	w:dword, h:dword
