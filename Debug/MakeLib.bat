@echo off
\masm32\bin\ml /c /coff %1.asm
if errorlevel 1 goto errasm
\masm32\bin\lib %1.obj /out:%1.lib
:errasm
exit
