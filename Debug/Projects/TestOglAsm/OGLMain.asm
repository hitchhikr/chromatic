			.data

_Dbl1			real8	1.0

RotX			real4	0.0
RotXSpeed		real4	40.0
RotY			real4	0.0
RotYSpeed		real4	30.0
RotZ			real4	0.0
RotZSpeed		real4	20.0

			.code

; ------------------------------------------------------
; Name: Initialize()
; Desc: Perform the various initializations
Initialize		proc

			invoke	glClearDepth, dword ptr [_Dbl1], dword ptr [_Dbl1 + 4]
			invoke	glHint, GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST
			invoke	glClearColor, CFLT(0.0), CFLT(0.0), CFLT(0.0), CFLT(1.0)
			invoke	glEnable, GL_NORMALIZE
			invoke	glEnable, GL_CULL_FACE
	
			mov	eax,TRUE
			ret
Initialize		endp

; ------------------------------------------------------
; Name: Deinitialize()
; Desc: Free allocated resources
Deinitialize		proc

			ret
Deinitialize		endp

; ------------------------------------------------------
; Name: Update()
; Desc: Update the scene
Update			proc	milliseconds:real4, TotalTime:real4
			fld	dword ptr [RotXSpeed]
			fmul	dword ptr [milliseconds]
			fadd	dword ptr [RotX]
			fstp	dword ptr [RotX]

			fld	dword ptr [RotYSpeed]
			fmul	dword ptr [milliseconds]
			fadd	dword ptr [RotY]
			fstp	dword ptr [RotY]

			fld	dword ptr [RotZSpeed]
			fmul	dword ptr [milliseconds]
			fadd	dword ptr [RotZ]
			fstp	dword ptr [RotZ]

			lea	eax,[g_keys + VK_ESCAPE]
			mov	al,[eax]
			test	al,al
			jz	Esc_Pressed
			invoke	Terminate_Application
Esc_Pressed:		ret
Update			endp

; ------------------------------------------------------
; Name: Draw()
; Desc: Draw the scene
Draw			proc
			invoke	glClear, GL_COLOR_BUFFER_BIT or GL_DEPTH_BUFFER_BIT or GL_STENCIL_BUFFER_BIT

			invoke glPushMatrix
				invoke glTranslatef, CFLT(0.0), CFLT(0.0), CFLT(-7.0)
				invoke glRotatef, RotX, CFLT(1.0), CFLT(0.0), CFLT(0.0)
				invoke glRotatef, RotY, CFLT(0.0), CFLT(1.0), CFLT(0.0)
				invoke glRotatef, RotZ, CFLT(0.0), CFLT(0.0), CFLT(1.0)
				invoke glBegin, GL_QUADS
					invoke glColor3f, CFLT(0.5), CFLT(1.0), CFLT(0.5)
					invoke glVertex3f, CFLT(1.0), CFLT(1.0), CFLT(-1.0)
					invoke glVertex3f,CFLT(-1.0), CFLT(1.0), CFLT(-1.0)
					invoke glVertex3f,CFLT(-1.0), CFLT(1.0), CFLT(1.0)
					invoke glVertex3f, CFLT(1.0), CFLT(1.0), CFLT(1.0)
					invoke glColor3f, CFLT(0.5), CFLT(1.0), CFLT(1.0)
					invoke glVertex3f, CFLT(1.0),CFLT(-1.0), CFLT(1.0)
					invoke glVertex3f,CFLT(-1.0),CFLT(-1.0), CFLT(1.0)
					invoke glVertex3f,CFLT(-1.0),CFLT(-1.0),CFLT(-1.0)
					invoke glVertex3f, CFLT(1.0),CFLT(-1.0),CFLT(-1.0)
					invoke glColor3f, CFLT(1.0), CFLT(0.5), CFLT(0.5)
					invoke glVertex3f, CFLT(1.0), CFLT(1.0), CFLT(1.0)
					invoke glVertex3f,CFLT(-1.0), CFLT(1.0), CFLT(1.0)
					invoke glVertex3f,CFLT(-1.0),CFLT(-1.0), CFLT(1.0)
					invoke glVertex3f, CFLT(1.0),CFLT(-1.0), CFLT(1.0)
					invoke glColor3f, CFLT(1.0), CFLT(1.0), CFLT(0.5)
					invoke glVertex3f, CFLT(1.0),CFLT(-1.0),CFLT(-1.0)
					invoke glVertex3f,CFLT(-1.0),CFLT(-1.0),CFLT(-1.0)
					invoke glVertex3f,CFLT(-1.0), CFLT(1.0),CFLT(-1.0)
					invoke glVertex3f, CFLT(1.0), CFLT(1.0),CFLT(-1.0)
					invoke glColor3f, CFLT(0.5), CFLT(0.5), CFLT(1.0)
					invoke glVertex3f, CFLT(-1.0), CFLT(1.0), CFLT(1.0)
					invoke glVertex3f, CFLT(-1.0), CFLT(1.0),CFLT(-1.0)
					invoke glVertex3f, CFLT(-1.0),CFLT(-1.0),CFLT(-1.0)
					invoke glVertex3f, CFLT(-1.0),CFLT(-1.0), CFLT(1.0)
					invoke glColor3f, CFLT(1.0), CFLT(0.5), CFLT(1.0)
					invoke glVertex3f, CFLT(1.0), CFLT(1.0),CFLT(-1.0)
					invoke glVertex3f, CFLT(1.0),CFLT(1.0), CFLT(1.0)
					invoke glVertex3f, CFLT(1.0),CFLT(-1.0), CFLT(1.0)
					invoke glVertex3f, CFLT(1.0),CFLT(-1.0),CFLT(-1.0)
				invoke glEnd
			invoke glPopMatrix
			ret
Draw			endp
