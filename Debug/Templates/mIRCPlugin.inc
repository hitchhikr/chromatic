; --------------- Returned values to mIRC
mIRC_halt		equ	0
mIRC_continue		equ	1
mIRC_filleddataparms 	equ	2
mIRC_filleddata 	equ	3

; --------------- Dll loading structure
MIRCLOADINFO		STRUCT
mVersion		DWORD	?
mHwnd			DWORD	?
mKeep			DWORD	?
MIRCLOADINFO 		ENDS

