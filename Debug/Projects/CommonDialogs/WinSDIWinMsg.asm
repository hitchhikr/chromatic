; --------------- Messages handling
WinProc			proc	hWin:dword,uMsg:dword,wParam:dword,lParam:dword
			.if	uMsg == WM_CREATE
				mov	MyFont.iPointSize,10
				xor	eax,eax
				ret
			.elseif uMsg == WM_COMMAND
				mov	eax,wParam
				.if	ax == 1000		; ChooseCol
					; Select a color
					invoke	ChooseCol,hWin,hInstance,LastCol
					; Display the selected color	
					.if eax != 0
						mov	LastCol,ebx
						invoke	dw2hex,ebx,addr ChosenDatASCII
						invoke	MessageBox,hWin,addr ChosenDatASCII,addr szWindowName,MB_OK or MB_ICONINFORMATION
					.endif
				.elseif ax == 1001	; GetOpenFileName
					; Select a file to open
					invoke	ChooseOpenFile,hWin,hInstance,addr OpDir,addr FileName,addr OpenFilterText
					.if eax != 0
						invoke	MessageBox,hWin,MyOpenFName.lpstrFile,addr szWindowName,MB_OK or MB_ICONINFORMATION
					.endif
				.elseif ax == 1002	; GetSaveFileName
					; Select a file to save
					invoke	ChooseSaveFile,hWin,hInstance,addr SvDir,addr FileName,addr SaveFilterText
					.if eax != 0
						invoke	MessageBox,hWin,MySaveFName.lpstrFile,addr szWindowName,MB_OK or MB_ICONINFORMATION
					.endif
				.elseif ax == 1003	; ChooseFont
					; Select a font
					invoke	ChooseFontName,hWin,addr CurFontName,MyFont.iPointSize
					.if eax != 0
						invoke	dwtoa,MyFont.iPointSize,addr ChosenDatASCII
						invoke	MemCopy,addr MyLogFont.lfFaceName,addr CurFontName,32
						invoke	MessageBox,hWin,addr CurFontName,addr szWindowName,MB_OK or MB_ICONINFORMATION
						invoke	MessageBox,hWin,addr ChosenDatASCII,addr szWindowName,MB_OK or MB_ICONINFORMATION
					.endif
				.elseif ax == 1004 
					invoke	SendMessage,hWin,WM_SYSCOMMAND,SC_CLOSE,NULL
				.endif
			.elseif	uMsg == WM_CLOSE
				invoke	MessageBox,hWin,addr szQuit,addr szWindowName,MB_YESNO or MB_ICONQUESTION
				.if eax == IDNO
					xor	eax,eax
					ret
				.endif
			.elseif uMsg == WM_DESTROY
				invoke	PostQuitMessage,NULL
				xor	eax,eax
				ret 
			.endif
			invoke	DefWindowProc,hWin,uMsg,wParam,lParam
			ret
WinProc			endp
