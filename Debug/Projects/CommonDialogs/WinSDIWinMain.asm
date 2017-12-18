			.code

; --------------- Program main inits
WinMain 		proc	hInst:dword,hPrevInst:dword,CmdLine:dword,CmdShow:dword
			local	wc:WNDCLASSEX
			local	msg:MSG
			local	WinLeft:dword
			local	WinTop:dword
			
			mov	wc.cbSize,sizeof WNDCLASSEX
			mov	wc.style,CS_HREDRAW or CS_VREDRAW or CS_BYTEALIGNWINDOW
			mov	wc.lpfnWndProc,offset WinProc
			mov	wc.cbClsExtra,0
			mov	wc.cbWndExtra,0
			mov	eax,hInst
			mov	hInstance,eax
			push	eax
			pop	wc.hInstance
			mov	wc.hbrBackground,COLOR_BTNFACE+1
			mov	wc.lpszMenuName,0
			mov	wc.lpszClassName,offset szClassName
			invoke	LoadIcon,hInst,ICON_APP
			mov	wc.hIcon,eax
			mov	wc.hIconSm,eax
			invoke	LoadCursor,0,IDC_ARROW
			mov	wc.hCursor,eax
			invoke	RegisterClassEx,addr wc
			mov	WinLeft,0
			mov	WinTop,0
			; Center the window if necessary
			invoke	GetSystemMetrics, SM_CXSCREEN
			.if WinWidth < eax
				sub	eax,WinWidth
				shr	eax,1
				mov	WinLeft,eax
			.endif
			invoke	GetSystemMetrics, SM_CYSCREEN
			.if WinHeight < eax
				sub	eax,WinHeight
				shr	eax,1
				mov	WinTop,eax
			.endif
			; Add some pixels around to make the inner part to fit the dimensions requirements
			invoke	GetSystemMetrics, SM_CXBORDER
			add	WinWidth,eax
			invoke	GetSystemMetrics, SM_CYBORDER
			add	WinHeight,eax
			invoke	GetSystemMetrics, SM_CYCAPTION
			add	WinHeight,eax
		        invoke	CreateWindowEx,0,addr szClassName,addr szWindowName,WS_OVERLAPPEDWINDOW,WinLeft,WinTop,WinWidth,WinHeight,0,0,hInst,0
			mov	hWnd,eax
			invoke	LoadAccelerators,hInst,ACCELERATORS
			mov	hAccel,eax
			invoke	LoadMenu,hInst,MENUS
			invoke	SetMenu,hWnd,eax
			invoke	ShowWindow,hWnd,SW_SHOWNORMAL
			invoke	UpdateWindow,hWnd

StartLoop:		invoke	GetMessage,addr msg,0,0,0
			test	eax,eax
			jz	ExitLoop
			invoke	TranslateAccelerator,hWnd,hAccel,addr msg
			invoke	TranslateMessage,addr msg
			invoke	DispatchMessage,addr msg
			jmp	StartLoop
ExitLoop:		mov	eax,msg.wParam
			ret
WinMain			endp
