@echo off
if exist %1.obj del %1.obj
if exist %1.dll del %1.dll
\masm32\bin\ml /c /coff %1.asm
if errorlevel 1 goto errasm
\masm32\bin\Link /SUBSYSTEM:WINDOWS /DLL /DEF:%1.def %1.obj
:errasm
exit