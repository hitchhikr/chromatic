; -----------------------------------------------------------------------
; Chromatic
; Integrated Development Environment
;
; Copyright (C) 2001-2010 Franck Charlet.
; All rights reserved.
;
; Redistribution and use in source and binary forms, with or without
; modification, are permitted provided that the following conditions
; are met:
;
;  1. Redistributions of source code must retain the above copyright notice,
;     this list of conditions and the following disclaimer.
;
;  2. Redistributions in binary form must reproduce the above copyright notice,
;     this list of conditions and the following disclaimer in the documentation
;     and/or other materials provided with the distribution.
;
; THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
; ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
; ARE DISCLAIMED. IN NO EVENT SHALL FRANCK CHARLET OR CONTRIBUTORS BE LIABLE
; FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
; DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
; OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
; HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
; LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
; OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
; SUCH DAMAGE.
; -----------------------------------------------------------------------

; --------------- Data Section
                        .data

CharTableUpper          db      000h,001h,002h,003h,004h,005h,006h,007h,008h,009h,00ah,00bh,00ch,00dh,00eh,00fh
                        db      010h,011h,012h,013h,014h,015h,016h,017h,018h,019h,01ah,01bh,01ch,01dh,01eh,01fh
                        db      020h,021h,022h,023h,024h,025h,026h,027h,028h,029h,02ah,02bh,02ch,02dh,02eh,02fh
                        db      030h,031h,032h,033h,034h,035h,036h,037h,038h,039h,03ah,03bh,03ch,03dh,03eh,03fh
                        db      040h,041h,042h,043h,044h,045h,046h,047h,048h,049h,04ah,04bh,04ch,04dh,04eh,04fh
                        db      050h,051h,052h,053h,054h,055h,056h,057h,058h,059h,05ah,05bh,05ch,05dh,05eh,05fh
                        db      060h,041h,042h,043h,044h,045h,046h,047h,048h,049h,04ah,04bh,04ch,04dh,04eh,04fh
                        db      050h,051h,052h,053h,054h,055h,056h,057h,058h,059h,05ah,07bh,07ch,07dh,07eh,07fh
                        db      080h,081h,082h,083h,084h,085h,086h,087h,088h,089h,08ah,08bh,08ch,08dh,08eh,08fh
                        db      090h,091h,092h,093h,094h,095h,096h,097h,098h,099h,09ah,09bh,09ch,09dh,09eh,09fh
                        db      0a0h,0a1h,0a2h,0a3h,0a4h,0a5h,0a6h,0a7h,0a8h,0a9h,0aah,0abh,0ach,0adh,0aeh,0afh
                        db      0b0h,0b1h,0b2h,0b3h,0b4h,0b5h,0b6h,0b7h,0b8h,0b9h,0bah,0bbh,0bch,0bdh,0beh,0bfh
                        db      0c0h,0c1h,0c2h,0c3h,0c4h,0c5h,0c6h,0c7h,0c8h,0c9h,0cah,0cbh,0cch,0cdh,0ceh,0cfh
                        db      0d0h,0d1h,0d2h,0d3h,0d4h,0d5h,0d6h,0d7h,0d8h,0d9h,0dah,0dbh,0dch,0ddh,0deh,0dfh
                        db      0e0h,0e1h,0e2h,0e3h,0e4h,0e5h,0e6h,0e7h,0e8h,0e9h,0eah,0ebh,0ech,0edh,0eeh,0efh
                        db      0f0h,0f1h,0f2h,0f3h,0f4h,0f5h,0f6h,0f7h,0f8h,0f9h,0fah,0fbh,0fch,0fdh,0feh,0ffh
CharTableLower          db      000h,001h,002h,003h,004h,005h,006h,007h,008h,009h,00ah,00bh,00ch,00dh,00eh,00fh
                        db      010h,011h,012h,013h,014h,015h,016h,017h,018h,019h,01ah,01bh,01ch,01dh,01eh,01fh
                        db      020h,021h,022h,023h,024h,025h,026h,027h,028h,029h,02ah,02bh,02ch,02dh,02eh,02fh
                        db      030h,031h,032h,033h,034h,035h,036h,037h,038h,039h,03ah,03bh,03ch,03dh,03eh,03fh
                        db      040h,061h,062h,063h,064h,065h,066h,067h,068h,069h,06ah,06bh,06ch,06dh,06eh,06fh
                        db      070h,071h,072h,073h,074h,075h,076h,077h,078h,079h,07ah,05bh,05ch,05dh,05eh,05fh
                        db      060h,061h,062h,063h,064h,065h,066h,067h,068h,069h,06ah,06bh,06ch,06dh,06eh,06fh
                        db      070h,071h,072h,073h,074h,075h,076h,077h,078h,079h,07ah,07bh,07ch,07dh,07eh,07fh
                        db      080h,081h,082h,083h,084h,085h,086h,087h,088h,089h,08ah,08bh,08ch,08dh,08eh,08fh
                        db      090h,091h,092h,093h,094h,095h,096h,097h,098h,099h,09ah,09bh,09ch,09dh,09eh,09fh
                        db      0a0h,0a1h,0a2h,0a3h,0a4h,0a5h,0a6h,0a7h,0a8h,0a9h,0aah,0abh,0ach,0adh,0aeh,0afh
                        db      0b0h,0b1h,0b2h,0b3h,0b4h,0b5h,0b6h,0b7h,0b8h,0b9h,0bah,0bbh,0bch,0bdh,0beh,0bfh
                        db      0c0h,0c1h,0c2h,0c3h,0c4h,0c5h,0c6h,0c7h,0c8h,0c9h,0cah,0cbh,0cch,0cdh,0ceh,0cfh
                        db      0d0h,0d1h,0d2h,0d3h,0d4h,0d5h,0d6h,0d7h,0d8h,0d9h,0dah,0dbh,0dch,0ddh,0deh,0dfh
                        db      0e0h,0e1h,0e2h,0e3h,0e4h,0e5h,0e6h,0e7h,0e8h,0e9h,0eah,0ebh,0ech,0edh,0eeh,0efh
                        db      0f0h,0f1h,0f2h,0f3h,0f4h,0f5h,0f6h,0f7h,0f8h,0f9h,0fah,0fbh,0fch,0fdh,0feh,0ffh

TabHex                  db      '00','01','02','03','04','05','06','07'
                        db      '08','09','0a','0b','0c','0d','0e','0f'
                        db      '10','11','12','13','14','15','16','17'
                        db      '18','19','1a','1b','1c','1d','1e','1f'
                        db      '20','21','22','23','24','25','26','27'
                        db      '28','29','2a','2b','2c','2d','2e','2f'
                        db      '30','31','32','33','34','35','36','37'
                        db      '38','39','3a','3b','3c','3d','3e','3f'
                        db      '40','41','42','43','44','45','46','47'
                        db      '48','49','4a','4b','4c','4d','4e','4f'
                        db      '50','51','52','53','54','55','56','57'
                        db      '58','59','5a','5b','5c','5d','5e','5f'
                        db      '60','61','62','63','64','65','66','67'
                        db      '68','69','6a','6b','6c','6d','6e','6f'
                        db      '70','71','72','73','74','75','76','77'
                        db      '78','79','7a','7b','7c','7d','7e','7f'
                        db      '80','81','82','83','84','85','86','87'
                        db      '88','89','8a','8b','8c','8d','8e','8f'
                        db      '90','91','92','93','94','95','96','97'
                        db      '98','99','9a','9b','9c','9d','9e','9f'
                        db      'a0','a1','a2','a3','a4','a5','a6','a7'
                        db      'a8','a9','aa','ab','ac','ad','ae','af'
                        db      'b0','b1','b2','b3','b4','b5','b6','b7'
                        db      'b8','b9','ba','bb','bc','bd','be','bf'
                        db      'c0','c1','c2','c3','c4','c5','c6','c7'
                        db      'c8','c9','ca','cb','cc','cd','ce','cf'
                        db      'd0','d1','d2','d3','d4','d5','d6','d7'
                        db      'd8','d9','da','db','dc','dd','de','df'
                        db      'e0','e1','e2','e3','e4','e5','e6','e7'
                        db      'e8','e9','ea','eb','ec','ed','ee','ef'
                        db      'f0','f1','f2','f3','f4','f5','f6','f7'
                        db      'f8','f9','fa','fb','fc','fd','fe','ff'

ComPosition             dd      0
hApiFile                dd      0
hStringMem              dd      0
hDescriptionMem         dd      0
hDescriptionMem2        dd      0

                        ; Must stay together
FilePosition            dd      0
FileLength              dd      0

FileRW                  dd      0
APIBlock                dd      0
APIInfos                dd      0
APIArgs                 dd      0
LngSplitChar            dd      0
PosInGroup              dd      0
IncludeBytes            dd      0
OldFncTwoChars          dw      -1                      ; Ensure that we can register the first index
FncTwoChars             db      "  ", 0
APISplitChar            db      "^^", 0
APISplitChar2           db      "|", 0
APISplitCharArgs        db      ",", 0
CRLF                    db      13, 10, 0
Quotes1                 db      "'", 0
Quotes2                 db      34, 0
CheckComment            db      "comment", 0
MyhLink                 HH_AKLINK <>
IncWord                 db      "include ", 0
