; --------------- Data Section
                        .data

ChromaticLib            dd      0
AddInContext            ADDINCONTEXT <>

MsgDescription          db      "Create and paste a GUID",0
MsgAuthor               db      "Franck Charlet",0
MsgChildError           db      "No opened documents.",0
MsgGUIDError            db      "No GUID definitions found for current language.",0
MsgGUIDPart1            db      "%0",0
MsgGUIDPart2            db      "%1",0
MsgGUIDPart3            db      "%2",0
MsgGUIDPart4            db      "%3",0
MsgGUIDPart5            db      "%4",0
MsgGUIDPart6            db      "%5",0
MsgGUIDPart7            db      "%6",0
MsgGUIDPart8            db      "%7",0
MsgGUIDPart9            db      "%8",0
MsgGUIDPart10           db      "%9",0
MsgGUIDPart11           db      "%A",0

                        align   8
GUIDToInsert            GUID <>
GUIDSkew                dd      0
