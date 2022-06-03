HtmlHelpA	proto	:dword,:dword,:dword,:dword
HtmlHelp	equ	<HtmlHelpA>

HH_KEYWORD_LOOKUP	equ	0Dh

HH_AKLINK	STRUCT
cbStruct	DWORD	?
fReserved	DWORD	?
pszKeywords	DWORD	?
pszUrl		DWORD	?
pszMsgText	DWORD	?
pszMsgTitle	DWORD	?
pszWindow	DWORD	?
fIndexOnFail	DWORD	?
HH_AKLINK	ENDS
