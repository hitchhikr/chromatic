; --------------- Data Section
                    .data
AppTitle            db  "MakeLST v1.0",0
AppCreds            db  "MakeLST v1.0 - Chromatic's .Lst file converter.",13,10
                    db  "Written by Franck Charlet."
MsgReturn           db  13,10,0
EndMsg              db  "Finished.",0
AddingMsg           db  "Converting file: ",0
DestFile            db  "\WinConst.db",0
CurDir              db  MAX_PATH dup (0)
SearchPattern       db  "\WinConst.lst",0
Slash               db  "\",0
ErrorOApiFile       db  " Can't open lst file !",0
ErrorRApiFile       db  " Can't read lst file !",0
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
HeaderLen           dd  0
FileRW              dq  0
OldLine             dd  0
OldIndexPos         dd  0
                    align   16
CharTableUpper      db  000h,001h,002h,003h,004h,005h,006h,007h,008h,009h,00ah,00bh,00ch,00dh,00eh,00fh
                    db  010h,011h,012h,013h,014h,015h,016h,017h,018h,019h,01ah,01bh,01ch,01dh,01eh,01fh
                    db  020h,021h,022h,023h,024h,025h,026h,027h,028h,029h,02ah,02bh,02ch,02dh,02eh,02fh
                    db  030h,031h,032h,033h,034h,035h,036h,037h,038h,039h,03ah,03bh,03ch,03dh,03eh,03fh
                    db  040h,041h,042h,043h,044h,045h,046h,047h,048h,049h,04ah,04bh,04ch,04dh,04eh,04fh
                    db  050h,051h,052h,053h,054h,055h,056h,057h,058h,059h,05ah,05bh,05ch,05dh,05eh,05fh
                    db  060h,041h,042h,043h,044h,045h,046h,047h,048h,049h,04ah,04bh,04ch,04dh,04eh,04fh
                    db  050h,051h,052h,053h,054h,055h,056h,057h,058h,059h,05ah,07bh,07ch,07dh,07eh,07fh
                    db  080h,081h,082h,083h,084h,085h,086h,087h,088h,089h,08ah,08bh,08ch,08dh,08eh,08fh
                    db  090h,091h,092h,093h,094h,095h,096h,097h,098h,099h,09ah,09bh,09ch,09dh,09eh,09fh
                    db  0a0h,0a1h,0a2h,0a3h,0a4h,0a5h,0a6h,0a7h,0a8h,0a9h,0aah,0abh,0ach,0adh,0aeh,0afh
                    db  0b0h,0b1h,0b2h,0b3h,0b4h,0b5h,0b6h,0b7h,0b8h,0b9h,0bah,0bbh,0bch,0bdh,0beh,0bfh
                    db  0c0h,0c1h,0c2h,0c3h,0c4h,0c5h,0c6h,0c7h,0c8h,0c9h,0cah,0cbh,0cch,0cdh,0ceh,0cfh
                    db  0d0h,0d1h,0d2h,0d3h,0d4h,0d5h,0d6h,0d7h,0d8h,0d9h,0dah,0dbh,0dch,0ddh,0deh,0dfh
                    db  0e0h,0e1h,0e2h,0e3h,0e4h,0e5h,0e6h,0e7h,0e8h,0e9h,0eah,0ebh,0ech,0edh,0eeh,0efh
                    db  0f0h,0f1h,0f2h,0f3h,0f4h,0f5h,0f6h,0f7h,0f8h,0f9h,0fah,0fbh,0fch,0fdh,0feh,0ffh
