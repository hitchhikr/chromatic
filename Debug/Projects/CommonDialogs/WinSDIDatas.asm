			.data

; --------------- Data Section

hWnd			dd	0
hInstance		dd	0
WinWidth		dd	500
WinHeight		dd	350
szClassName		db	"SDI_Class",0
szWindowName		db	"Common dialog examples",0
szQuit			db	"Are you sure ?",0
SaveDiagTitle		db	"Select a file to save",0
OpenDiagTitle		db	"Select a file to open",0
OpenFilterText		db	"All Files",0,"*.*",0
			db	"Text Files",0,"*.txt",0
			db	0
SaveFilterText		db	"All Files",0,"*.*",0
			db	"Text Files",0,"*.txt",0
			db	0	
hAccel			dd	0
LastCol			dd	0
SvDir			db	MAX_PATH dup (0)
OpDir			db	MAX_PATH dup (0)
FileName		db	MAX_PATH dup (0)	
ChosenDatASCII		db	20 dup (0)
CurFontName		db	"Courier New"
			db	32 dup (0)

; --------------- CommonDialog datas
CustomColor		dd	16 dup (0)
CurrentDir		db	MAX_PATH dup (0)
OpenDir			db	MAX_PATH dup (0)
MyOpenFName 		OPENFILENAME <> 
MySaveFName 		OPENFILENAME <> 
MyColor			CHOOSECOLOR <>
MyLogFont		LOGFONT <>
MyFont			CHOOSEFONT <>
