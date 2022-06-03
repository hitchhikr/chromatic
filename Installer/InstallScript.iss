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
; Installation script
; To be used with Inno Setup 4.2.7 (or later)
; -----------------------------------------------------------------------

[Setup]
AppName=Chromatic IDE
AppVerName=Chromatic IDE v1.3.6
AppPublisher=hitchhikr SoftWorks
AppPublisherURL=http://perso.wanadoo.fr/franck.charlet/index.htm
AppSupportURL=http://perso.wanadoo.fr/franck.charlet/index.htm
AppUpdatesURL=http://perso.wanadoo.fr/franck.charlet/index.htm
DefaultDirName=c:\Chromatic\
DefaultGroupName=Chromatic IDE
DisableProgramGroupPage=yes
LicenseFile=..\Debug\LicenseInstall.txt
Compression=lzma
SolidCompression=yes
VersionInfoCompany=hitchhikr SoftWorks
VersionInfoTextVersion=Chromatic IDE v1.3.6
VersionInfoVersion=1.3.6.0
OutputBaseFilename=Chromatic_svn
OutputDir=.\

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked
Name: "quicklaunchicon"; Description: "{cm:CreateQuickLaunchIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "..\Release\Chromatic.exe"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Debug\Addins\AddInDATETIME.dll"; DestDir: "{app}\AddIns"; Flags: ignoreversion
Source: "..\Debug\Addins\AddInASCII.dll"; DestDir: "{app}\AddIns"; Flags: ignoreversion
Source: "..\Debug\Addins\AddInGUID.dll"; DestDir: "{app}\AddIns"; Flags: ignoreversion
Source: "..\Debug\Backup\Readme.txt"; DestDir: "{app}\Backup"; Flags: ignoreversion
Source: "..\Release\Config\CGrep.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\Colors.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\Dates.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\FtpAccounts.ini"; DestDir: "{app}\Config";
Source: "..\Release\Config\Languages.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\Main.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\ProjectsType.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\Skins.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\Toolbars.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\Filters.ini"; DestDir: "{app}\Config"; Flags: ignoreversion
Source: "..\Release\Config\UserMenus\*.ume"; DestDir: "{app}\Config\UserMenus";
Source: "..\Debug\Databases\WinDetails.db"; DestDir: "{app}\Databases"; Flags: ignoreversion
Source: "..\Debug\Databases\WinFnc.db"; DestDir: "{app}\Databases"; Flags: ignoreversion
Source: "..\Debug\Databases\WinConst.db"; DestDir: "{app}\Databases"; Flags: ignoreversion
Source: "..\Debug\Downloads\Readme.txt"; DestDir: "{app}\Downloads"; Flags: ignoreversion
Source: "..\Debug\Examples\PacketsView\PacketsView.asm"; DestDir: "{app}\Examples\PacketsView"; Flags: ignoreversion
Source: "..\Debug\Examples\PacketsView\PacketsView.ini"; DestDir: "{app}\Examples\PacketsView"; Flags: ignoreversion
Source: "..\Debug\Examples\LDIrc\LDIrc.asm"; DestDir: "{app}\Examples\LDIrc"; Flags: ignoreversion
Source: "..\Debug\Filters\*.dll"; DestDir: "{app}\Filters"; Flags: ignoreversion
Source: "..\Debug\Help\Rc.hlp"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "..\Debug\Help\X86Eas.hlp"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "..\Debug\Help\Cpp.chm"; DestDir: "{app}\Help"; Flags: ignoreversion
Source: "..\Debug\Languages\*.mln"; DestDir: "{app}\Languages"; Flags: ignoreversion
Source: "..\Debug\Languages\Scripts\*.lua"; DestDir: "{app}\Languages\Scripts"; Flags: ignoreversion
Source: "..\Debug\Skins\*.skm"; DestDir: "{app}\Skins"; Flags: ignoreversion
Source: "..\Debug\Skins\ExtraCode\*.*"; DestDir: "{app}\Skins\ExtraCode"; Flags: ignoreversion
Source: "..\Debug\Snippets\*.*"; DestDir: "{app}\Snippets"; Flags: ignoreversion
Source: "..\Debug\Templates\*.*"; DestDir: "{app}\Templates"; Flags: ignoreversion
Source: "..\Debug\Wizards\*.pil"; DestDir: "{app}\Wizards"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInASCII\Cubes.ico"; DestDir: "{app}\Projects\AddInASCII"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInASCII\AddIn.mdf"; DestDir: "{app}\Projects\AddInASCII"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInASCII\AddInDatas.asm"; DestDir: "{app}\Projects\AddInASCII"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInASCII\AddIn.asm"; DestDir: "{app}\Projects\AddInASCII"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInASCII\AddInASCII.med"; DestDir: "{app}\Projects\AddInASCII"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInDATETIME\Cubes.ico"; DestDir: "{app}\Projects\AddInDATETIME"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInDATETIME\AddIn.mdf"; DestDir: "{app}\Projects\AddInDATETIME"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInDATETIME\AddInDatas.asm"; DestDir: "{app}\Projects\AddInDATETIME"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInDATETIME\AddIn.asm"; DestDir: "{app}\Projects\AddInDATETIME"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInDATETIME\AddInDATETIME.med"; DestDir: "{app}\Projects\AddInDATETIME"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInGUID\Cubes.ico"; DestDir: "{app}\Projects\AddInGUID"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInGUID\AddIn.mdf"; DestDir: "{app}\Projects\AddInGUID"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInGUID\AddInDatas.asm"; DestDir: "{app}\Projects\AddInGUID"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInGUID\AddIn.asm"; DestDir: "{app}\Projects\AddInGUID"; Flags: ignoreversion
Source: "..\Debug\Projects\AddInGUID\AddInGUID.med"; DestDir: "{app}\Projects\AddInGUID"; Flags: ignoreversion
Source: "..\Debug\Projects\CommonDialogs\Windows.ico"; DestDir: "{app}\Projects\CommonDialogs"; Flags: ignoreversion
Source: "..\Debug\Projects\CommonDialogs\ACCELERATORS_0.acr"; DestDir: "{app}\Projects\CommonDialogs"; Flags: ignoreversion
Source: "..\Debug\Projects\CommonDialogs\MENUS_0.mnu"; DestDir: "{app}\Projects\CommonDialogs"; Flags: ignoreversion
Source: "..\Debug\Projects\CommonDialogs\WinSDIWinMain.asm"; DestDir: "{app}\Projects\CommonDialogs"; Flags: ignoreversion
Source: "..\Debug\Projects\CommonDialogs\WinSDIWinMsg.asm"; DestDir: "{app}\Projects\CommonDialogs"; Flags: ignoreversion
Source: "..\Debug\Projects\CommonDialogs\CommonDialogs.med"; DestDir: "{app}\Projects\CommonDialogs"; Flags: ignoreversion
Source: "..\Debug\Projects\CommonDialogs\WinSDIDatas.asm"; DestDir: "{app}\Projects\CommonDialogs"; Flags: ignoreversion
Source: "..\Debug\Projects\CommonDialogs\WinFunctions.asm"; DestDir: "{app}\Projects\CommonDialogs"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToC\Filter.mdf"; DestDir: "{app}\Projects\FilterRawToC"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToC\Filter.ico"; DestDir: "{app}\Projects\FilterRawToC"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToC\FilterDatas.c"; DestDir: "{app}\Projects\FilterRawToC"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToC\Filter.c"; DestDir: "{app}\Projects\FilterRawToC"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToC\FilterRawToC.med"; DestDir: "{app}\Projects\FilterRawToC"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDb\Filter.mdf"; DestDir: "{app}\Projects\FilterRawToDb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDb\Filter.ico"; DestDir: "{app}\Projects\FilterRawToDb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDb\FilterDatas.c"; DestDir: "{app}\Projects\FilterRawToDb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDb\Filter.c"; DestDir: "{app}\Projects\FilterRawToDb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDb\FilterRawToDb.med"; DestDir: "{app}\Projects\FilterRawToDb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDcb\Filter.mdf"; DestDir: "{app}\Projects\FilterRawToDcb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDcb\Filter.ico"; DestDir: "{app}\Projects\FilterRawToDcb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDcb\FilterDatas.c"; DestDir: "{app}\Projects\FilterRawToDcb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDcb\Filter.c"; DestDir: "{app}\Projects\FilterRawToDcb"; Flags: ignoreversion
Source: "..\Debug\Projects\FilterRawToDcb\FilterRawToDcb.med"; DestDir: "{app}\Projects\FilterRawToDcb"; Flags: ignoreversion
Source: "..\Debug\Projects\TestFreePascal\TestFreePascal.med"; DestDir: "{app}\Projects\TestFreePascal"; Flags: ignoreversion
Source: "..\Debug\Projects\TestFreePascal\Windows.ico"; DestDir: "{app}\Projects\TestFreePascal"; Flags: ignoreversion
Source: "..\Debug\Projects\TestFreePascal\WinHello.pp"; DestDir: "{app}\Projects\TestFreePascal"; Flags: ignoreversion
Source: "..\Debug\Projects\TestOglAsm\TestOglAsm.med"; DestDir: "{app}\Projects\TestOglAsm"; Flags: ignoreversion
Source: "..\Debug\Projects\TestOglAsm\OpenGL.ico"; DestDir: "{app}\Projects\TestOglAsm"; Flags: ignoreversion
Source: "..\Debug\Projects\TestOglAsm\OGLStartup.asm"; DestDir: "{app}\Projects\TestOglAsm"; Flags: ignoreversion
Source: "..\Debug\Projects\TestOglAsm\OGLMain.asm"; DestDir: "{app}\Projects\TestOglAsm"; Flags: ignoreversion
Source: "..\Debug\Projects\TestOglC\TestOglC.med"; DestDir: "{app}\Projects\TestOglC"; Flags: ignoreversion
Source: "..\Debug\Projects\TestOglC\OpenGL.ico"; DestDir: "{app}\Projects\TestOglC"; Flags: ignoreversion
Source: "..\Debug\Projects\TestOglC\OGLStartup.cpp"; DestDir: "{app}\Projects\TestOglC"; Flags: ignoreversion
Source: "..\Debug\Projects\TestOglC\OGLMain.cpp"; DestDir: "{app}\Projects\TestOglC"; Flags: ignoreversion
Source: "..\Debug\Tools\*.*"; DestDir: "{app}\Tools"; Flags: ignoreversion
Source: "..\Debug\Scripts\*.*"; DestDir: "{app}\Scripts"; Flags: ignoreversion
Source: "..\Debug\cmaxXX.dll"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Debug\MakeDll.bat"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Debug\MakeLib.bat"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Debug\History.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Debug\License.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Debug\Readme.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Debug\Tips.txt"; DestDir: "{app}"; Flags: ignoreversion
Source: "..\Debug\DbgHelp.dll"; DestDir: "{sys}"; Flags: uninsneveruninstall

[Icons]
Name: "{group}\Chromatic"; Filename: "{app}\Chromatic.exe"
Name: "{group}\{cm:UninstallProgram,Chromatic}"; Filename: "{uninstallexe}"
Name: "{userdesktop}\Chromatic"; Filename: "{app}\Chromatic.exe"; Tasks: desktopicon
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\Chromatic"; Filename: "{app}\Chromatic.exe"; Tasks: quicklaunchicon

[Run]
Filename: "{app}\Chromatic.exe"; Description: "{cm:LaunchProgram,Chromatic}"; Flags: nowait postinstall skipifsilent
