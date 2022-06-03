; buildblock DEBUG
;   CAPT [BINDIR]\tasm32.exe /s /m /ml /z /Zi "%2"
;   CAPT [BINDIR]\tlink32.exe -x -c -Tpe -v "%1.obj",,,import32.lib
; buildblockend
