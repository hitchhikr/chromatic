; --------------- Data Section
                    .data

AppTitle            db  "MakeDB v1.0",0
AppCreds            db  "MakeDB v1.0 - Chromatic's .Api file converter.",13,10
                    db  "Written by Franck Charlet."
MsgReturn           db  13,10,0
EndMsg              db  "Finished.",0
AddingMsg           db  "Converting file: ",0
DestFile            db  "\WinDetails.db",0
CIdx                db  ".",0
CurDir              db  MAX_PATH dup (0)
SearchPattern       db  "\WinDetails.lst",0
Slash               db  "\",0
ErrorOApiFile       db  " Can't open api file !",0
ErrorRApiFile       db  " Can't read api file !",0
ErrorMemory         db  " Memory error !",0
ApiSearchFileName   db  MAX_PATH dup (0)
ApiFileName         db  MAX_PATH dup (0)
DbFileName          db  MAX_PATH dup (0)
FindResult          WIN32_FIND_DATA <>
FindHandle          dd  0
DbFileHandle        dd  0
FileLines           dd  0
ApiFileHandle       dd  0
ApiFileSize         dd  0
hApiFileMem         dd  0
hIndex              dd  0
CurrentLine         dd  0
LastIndex           dd  0
VeryLastIndexPos    dd  0
LastIndexPos        dd  0
FileRW              dq  0
OldFncTwoChars      dw  -1          ; Ensure that we can register the first index
CharTableUpper      db  256 dup (0)
CharTableLower      db  256 dup (0)
FncTwoChars         db  "  ",0
