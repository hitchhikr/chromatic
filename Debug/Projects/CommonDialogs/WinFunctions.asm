; --------------- Procedures Declarations
			.code

; --------------- CommonDialog Procedures
ChooseCol		proto	:dword,:dword,:dword
ChooseOpenFile		proto	:dword,:dword,:dword,:dword,:dword
ChooseSaveFile		proto	:dword,:dword,:dword,:dword,:dword
ChooseFontName		proto	:dword,:dword,:dword

; --------------- Display ChooseColor dialog
; Out:
; Eax: Ok/Cancel
; Ebx: Color
ChooseCol		proc	hWin:dword,hInst:dword,DefaultCol:dword
			push	DefaultCol
			pop	MyColor.rgbResult
			mov	MyColor.lStructSize,sizeof CHOOSECOLOR
			push	hWin
			pop	MyColor.hwndOwner
			push	hInst
			pop	MyColor.hInstance
			mov	MyColor.lCustData,0
			mov	MyColor.lpfnHook,0
			mov	MyColor.Flags,CC_FULLOPEN or CC_RGBINIT
			mov	MyColor.lpCustColors,offset CustomColor
			invoke	ChooseColor,addr MyColor
			mov	ebx,MyColor.rgbResult
			ret
ChooseCol		endp

; --------------- Display ChooseOpenFile dialog
; Eax: Ok/Cancel
ChooseOpenFile		proc	hWin:dword,hInst:dword,CurDir:dword,FName:dword,OPFilters:dword
			push	edi
			mov	edi,FName
			xor	al,al
			mov	ecx,MAX_PATH
ClearFNameBuffer:	stosb
			loop	ClearFNameBuffer
			mov	MyOpenFName.lStructSize,sizeof OPENFILENAME
			push	hWin
			pop	MyOpenFName.hWndOwner
			push	hInst
			pop	MyOpenFName.hInstance
			push	OPFilters
			pop	MyOpenFName.lpstrFilter
			push	FName
			pop	MyOpenFName.lpstrFile
			mov	MyOpenFName.nMaxFile,MAX_PATH
			mov	MyOpenFName.Flags,OFN_EXPLORER or OFN_HIDEREADONLY or OFN_SHOWHELP \
				      	or OFN_PATHMUSTEXIST or OFN_FILEMUSTEXIST or OFN_LONGNAMES
			mov	MyOpenFName.lpstrTitle,offset OpenDiagTitle
			mov	MyOpenFName.lpstrDefExt,0
			mov	eax,CurDir
			mov	ebx,eax
			mov	al,[eax]
			test	al,al
			jnz	SpecifiedDir
			mov	ebx,offset CurrentDir
SpecifiedDir:		push	ebx
			pop	MyOpenFName.lpstrInitialDir
			mov	MyOpenFName.nFilterIndex,1
			invoke	GetOpenFileName,addr MyOpenFName
			pop	edi
			ret
ChooseOpenFile		endp

; --------------- Display ChooseSaveFile dialog
; Eax: Ok/Cancel
ChooseSaveFile		proc	hWin:dword,hInst:dword,CurDir:dword,FName:dword,OPFilters:dword
			mov	edi,FName
			xor	al,al
			mov	ecx,MAX_PATH
ClearFNameBuffer:	stosb
			loop	ClearFNameBuffer
			mov	MySaveFName.lStructSize,sizeof OPENFILENAME
			push	hWin
			pop	MySaveFName.hWndOwner
			push	hInst
			pop	MySaveFName.hInstance
			push	OPFilters
			pop  	MySaveFName.lpstrFilter
			push	FName
			pop	MySaveFName.lpstrFile
			mov	MySaveFName.nMaxFile,MAX_PATH
			mov	MySaveFName.Flags,OFN_EXPLORER or OFN_HIDEREADONLY or OFN_SHOWHELP \
					      or OFN_PATHMUSTEXIST or OFN_LONGNAMES or OFN_OVERWRITEPROMPT
			mov	MySaveFName.lpstrTitle,offset SaveDiagTitle
			mov	MySaveFName.lpstrDefExt,0
			mov	eax,CurDir
			mov	ebx,eax
			mov	al,[eax]
			test	al,al
			jnz	SpecifiedDir
			mov	ebx,offset CurrentDir
SpecifiedDir:		push	ebx
			pop	MySaveFName.lpstrInitialDir
			mov	MySaveFName.nFilterIndex,1
			invoke	GetSaveFileName,addr MySaveFName
			ret
ChooseSaveFile		endp

; --------------- Display ChooseSaveFile dialog
; Eax: Ok/Cancel
ChooseFontName		proc	hWin:dword,FontName:dword,FontSize:dword
			local	Font_hDC:HDC

			push	esi
			push	edi
    			invoke	GetDC,hWin
    			mov	Font_hDC,eax
    			; Obtain the real height of the font
			invoke	GetDeviceCaps,Font_hDC,LOGPIXELSY
			imul	eax,FontSize
			mov	ebx,72
			cdq
			idiv	ebx
			neg	eax
			mov	MyLogFont.lfHeight,eax
			mov	MyLogFont.lfWidth,0
			mov	MyLogFont.lfEscapement,0
			mov	MyLogFont.lfOrientation,0
			mov	MyLogFont.lfWeight,FW_NORMAL
			mov	MyLogFont.lfItalic,0
			mov	MyLogFont.lfUnderline,0
			mov	MyLogFont.lfStrikeOut,0
			mov	MyLogFont.lfCharSet,DEFAULT_CHARSET
			mov	MyLogFont.lfOutPrecision,OUT_DEFAULT_PRECIS
			mov	MyLogFont.lfClipPrecision,CLIP_DEFAULT_PRECIS
			mov	MyLogFont.lfQuality,DEFAULT_QUALITY
			mov	MyLogFont.lfPitchAndFamily,DEFAULT_PITCH 
			mov	esi,FontName
			mov	edi,offset MyLogFont.lfFaceName
CopyFontName:		mov	al,[esi]
			inc	esi
			test	al,al
			jz	OkFontName
			mov	[edi],al
			inc	edi
			jmp	CopyFontName
OkFontName:		mov	[edi],al		; Null
			mov	MyFont.lStructSize,sizeof MyFont
			push	hWin
			pop	MyFont.hWndOwner
			push	offset MyLogFont
			pop	MyFont.lpLogFont
			mov	eax,FontSize
			imul	eax,10
			mov	MyFont.iPointSize,eax
			mov	MyFont.Flags,CF_BOTH or CF_FORCEFONTEXIST or CF_INITTOLOGFONTSTRUCT or CF_LIMITSIZE
			mov	MyFont.rgbColors,0
			mov	MyFont.lCustData,0
			mov	MyFont.lpfnHook,0
			mov	MyFont.lpTemplateName,0
			mov	MyFont.hInstance,0
			mov	MyFont.lpszStyle,0
			mov	MyFont.nFontType,REGULAR_FONTTYPE
			mov	MyFont.nSizeMin,8
			mov	MyFont.nSizeMax,72
			mov	MyFont.hDC,0
			invoke	ChooseFont,addr MyFont
    			push	eax
    			invoke	ReleaseDC,hWin,Font_hDC
			mov	eax,MyFont.iPointSize
			mov	ebx,10
			cdq
			idiv	ebx
			mov	MyFont.iPointSize,eax
			pop	eax
			pop	edi
			pop	esi
			ret
ChooseFontName		endp
