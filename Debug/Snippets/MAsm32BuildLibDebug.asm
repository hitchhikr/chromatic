; buildblock DEBUG
;   CAPT [BINDIR]\ml.exe /Zd /Zi /c /I"[INCLUDEDIR]" /coff "%2"
;   CAPT [BINDIR]\lib.exe "%1.obj" /DEBUGTYPE:CV /INCREMENTAL:NO /OUT:"%1.lib"
; buildblockend
