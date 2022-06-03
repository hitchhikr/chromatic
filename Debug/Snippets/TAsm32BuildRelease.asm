; buildblock RELEASE
;   CAPT [BINDIR]\tasm32.exe /s /m /ml /z "%2"
;   CAPT [BINDIR]\tlink32.exe -x -c -Tpe "%1.obj",,,import32.lib
; buildblockend
