; --------------- NT driver macros
WCHAR_                  equ	2		; unicode char size

WSTRING                 macro   _STRING_
                        _npar__	=0
                        _ESC_	=0
                        irpc	_CHAR_,<_STRING_>
                        if	_ESC_
                            _ESC_ =0
                            _npar__ = _npar__ + 1
                            if	"&_CHAR_" eq "/"
                                WORD    "/"
                            elseif  "&_CHAR_" eq "n"
                                WORD    0dh,0ah
                                _npar__ = _npar__ + 1
                            elseif	"&_CHAR_" eq "r"
                                WORD    0dh
                            elseif	"&_CHAR_" eq "l"
                                WORD    0ah
                            elseif	"&_CHAR_" eq "s"
                                WORD    20h
                            elseif	"&_CHAR_" eq "c"
                                WORD    3bh
                            elseif	"&_CHAR_" eq "t"
                                WORD    09h
                            elseif	"&_CHAR_" eq ":"
                                WORD    "!"
                            elseif	"&_CHAR_" eq "-"
                                WORD    "'"
                            elseif	"&_CHAR_" eq "="
                                WORD    '"'
                            elseif	"&_CHAR_" eq "|"
                                WORD    "\"
                            elseif	"&_CHAR_" eq "#"
                                WORD    "%"
                            elseif	"&_CHAR_" eq "+"
                                WORD    "&"
                            elseif	"&_CHAR_" eq "("
                                WORD    "<"
                            elseif	"&_CHAR_" eq ")"
                                WORD    ">"
                            elseif	"&_CHAR_" eq "0"
                                WORD    0
                            elseif	"&_CHAR_" eq "1"
                                WORD    1
                            else
                                WORD    "/", "&_CHAR_"
                                _npar__ = _npar__ + 1
                            endif
                        else
                            if	"&_CHAR_" eq "/"
                                _ESC_ = 1
                            else
                                _ESC_ = 0
                                WORD "&_CHAR_"
                                _npar__ = _npar__ + 1
                            endif
                        endif
                        endm

TEXTW                   macro   __Nomen,__Text
				        align   WCHAR_
__Nomen                 label   WCHAR
s&__Nomen               equ     offset __Nomen
                        WSTRING <__Text>
L&__Nomen               equ     _npar__
                        endm
