; --------------- Data Section
                        .data

ChromaticLib            dd      0
AddInContext            ADDINCONTEXT <>
AddInLogFont            LOGFONT <>
AddInChooseFont         CHOOSEFONT <>

MsgDescription          db      "ASCII table viewer",0
MsgAuthor               db      "Franck Charlet",0
MsgChildError           db      "No opened documents.",0
MsgTitle                db      "ASCII table",0
MsgPaste                db      "Paste",0
MsgPasteDatas           db      "Paste column :",0
MsgFont                 db      "Font",0
MsgClose                db      "Close",0
MsgEmpty                db      0
MsgSpace                db      " ",0
MsgCourierNew           db      "Courier New",0
MsgOpBracket            db      " (",0
MsgClBracket            db      ")",0
MsgColBin               db      "Bin",0
MsgColCtrl              db      "Ctrl",0
MsgColName              db      "Name",0
MsgColHex               db      "Hex",0
MsgColOct               db      "Oct",0
MsgColDec               db      "Dec",0
MsgColChar              db      "Char",0
MsgNUL                  db      "NUL",0
MsgSOH                  db      "SOH",0
MsgSTX                  db      "STX",0
MsgETX                  db      "ETX",0
MsgEOT                  db      "EOT",0
MsgENQ                  db      "ENQ",0
MsgACK                  db      "ACK",0
MsgBEL                  db      "BEL",0
MsgBS                   db      "BS",0
MsgHT                   db      "HT",0
MsgLF                   db      "LF",0
MsgVT                   db      "VT",0
MsgFF                   db      "FF",0
MsgCR                   db      "CR",0
MsgSO                   db      "SO",0
MsgSI                   db      "SI",0
MsgDLE                  db      "DLE",0
MsgDC1                  db      "DC1",0
MsgDC2                  db      "DC2",0
MsgDC3                  db      "DC3",0
MsgDC4                  db      "DC4",0
MsgNAK                  db      "NAK",0
MsgSYN                  db      "SYN",0
MsgETB                  db      "ETB",0
MsgCAN                  db      "CAN",0
MsgEM                   db      "EM",0
MsgSUB                  db      "SUB",0
MsgESC                  db      "ESC",0
MsgFS                   db      "FS",0
MsgGS                   db      "GS",0
MsgRS                   db      "RS",0
MsgUS                   db      "US",0
MsgSp                   db      "Sp",0
MsgCtrl0                db      "^@",0
MsgCtrl1                db      "^A",0
MsgCtrl2                db      "^B",0
MsgCtrl3                db      "^C",0
MsgCtrl4                db      "^D",0
MsgCtrl5                db      "^E",0
MsgCtrl6                db      "^F",0
MsgCtrl7                db      "^G",0
MsgCtrl8                db      "^H",0
MsgCtrl9                db      "^I",0
MsgCtrl10               db      "^J",0
MsgCtrl11               db      "^K",0
MsgCtrl12               db      "^L",0
MsgCtrl13               db      "^M",0
MsgCtrl14               db      "^N",0
MsgCtrl15               db      "^O",0
MsgCtrl16               db      "^P",0
MsgCtrl17               db      "^Q",0
MsgCtrl18               db      "^R",0
MsgCtrl19               db      "^S",0
MsgCtrl20               db      "^T",0
MsgCtrl21               db      "^U",0
MsgCtrl22               db      "^V",0
MsgCtrl23               db      "^W",0
MsgCtrl24               db      "^X",0
MsgCtrl25               db      "^Y",0
MsgCtrl26               db      "^Z",0
MsgCtrl27               db      "^[",0
MsgCtrl28               db      "^\",0
MsgCtrl29               db      "^]",0
MsgCtrl30               db      "^^",0
MsgCtrl31               db      "^_",0

                        align   8
FlagActive              dd      0
hWndASCIISel            dd      0
hASCIIList              dd      0
hASCIICmdPaste          dd      0
hASCIICmdFont           dd      0
hASCIICbPasteDatas      dd      0
hASCIIFontLabel         dd      0
hASCIIColumnLabel       dd      0
hASCIIFont              dd      0
ASCIIFontName           dd      0
ASCIIFontSize           dd      0
ASCIICount              dd      0
ASCIIType               dd      0
