; --------------- Data Section
                        .data

ChromaticLib            dd      0
AddInContext            ADDINCONTEXT <>

MsgDescription          db      "Paste current DATE/TIME",0
MsgAuthor               db      "Franck Charlet",0
MsgChildError           db      "No opened documents.",0
MsgTitle                db      "Date/time",0
MsgPaste                db      "Paste",0
MsgClose                db      "Close",0
MsgFormats              db      "Formats",0
MsgDateFormat           db      "DateFormat",0
MsgTimeFormat           db      "TimeFormat",0
MsgFormatSep            db      "|",0
MsgSpace                db      " ",0

                        align   8
hWndDateSel             dd      0
hPropertiesLbFileExt    dd      0
DatesIniFile            dd      0
UserDateFormat          dd      0
UserTimeFormat          dd      0
FlagActive              dd      0
MaxText                 dd      0
SerifFont               dd      0
