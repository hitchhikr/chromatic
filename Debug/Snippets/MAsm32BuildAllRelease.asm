; buildblock RELEASE
;   CAPT [BINDIR]\ml.exe /c /coff /I"[INCLUDEDIR]" "%2"
;   CAPT [BINDIR]\Link.exe /LIBPATH:"[LIBDIR]" /SUBSYSTEM:WINDOWS "%1.obj"
; buildblockend
