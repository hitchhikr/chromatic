; --------------- Data Section
			.data

WAStructPtr		dd	0
AddInContext		MASMEDCONTEXT <>

MsgDescription		db	"Edit binary files",0
MsgAuthor		db	"Franck Charlet",0
MsgDialogTitle		db	"HexEdit",0
MsgFontName		db	"Courier New",0
MsgPoint		db	":",0
HexTbl			db	"0123456789ABCDEF",0
MsgEmpty		db	0
MsgOpenFile		db	"Open new file",0
MsgEditMode		db	"Edit mode on/off",0
MsgGotoAddr		db	"Goto given address",0
MsgViewType		db	"Set view type",0
MsgByteView		db	"View as bytes",0
MsgWordView		db	"View as words",0
MsgDWordView		db	"View as doublewords",0
MsgFilters		db	"All files (*.*)|*.*",0
MsgErrOpen		db	"Can't open file",0
FileName		db	MAX_PATH+1 dup (0)

			align	8
CaretXPosBytes		db	10,11, 13,14, 16,17, 19,20, 22,23, 25,26, 28,29, 31,32
			db	34,35, 37,38, 40,41, 43,44, 46,47, 49,50, 52,53, 55,56
			db	58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73

			align	8
HexPs			PAINTSTRUCT <>
CurrentDisplayMode	dd	0
hFileHandle		dd	0
hWndHex			dd	0
hHexDump		dd	0
hHexFont		dd	0
hHexStatus		dd	0
hHexToolbar		dd	0
hHexViewMenu		dd	0
OldCurrentY		dd	0
CaretPosX		dd	0
CaretPosY		dd	0
CaretFilePosY		dd	0
CurrentY		dd	0
CharWidth		dd	0
CharHeight		dd	0
RawOffset		dd	0
LinesLen		dd	0
FileMem			dd	0
FileLen			dd	0
FileLenHigh		dd	0
MaxLines		dd	0
LinesRemainder		dd	0
CurrentLineText		db	76 dup (0)
HexBackColor		dd	0
HexFrontColor		dd	0
FlagActive		dd	0
HexDumpY		dd	0
HexDumpHeight		dd	0
BackDC			WABACKDC <>
BackRECT		RECT <>
CaretOn			dd	0
