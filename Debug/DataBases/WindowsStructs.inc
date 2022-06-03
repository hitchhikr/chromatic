POINT|x|y
POINTS|x|y
POINTL|x|y
RECT|left|top|right|bottom|
INITCOMMONCONTROLSEX|dwSize|dwICC
REBARINFO|cbSize|fMask|himl
REBARBANDINFOA|cbSize|fMask|fStyle|clrFore|clrBack|lpText|cch|iImage|hwndChild|cxMinChild|cyMinChild|lx|hbmBack|wID|cyChild|cyMaxChild|cyIntegral|cxIdeal|lParam|cxHeader
REBARBANDINFO|=REBARBANDINFOA
NMHDR|hwndFrom|idFrom|code
NMREBARCHILDSIZE|hdr<NMHDR>|uBand|wID|rcChild<RECT>|rcBand<RECT>
NMREBAR|hdr<NMHDR>|dwMask|uBand|fStyle|wID|lParam
NMRBAUTOSIZE|hdr<NMHDR>|fChanged|rcTarget<RECT>|rcActual<RECT>
RBHITTESTINFO|pt<POINT>|flags|iBand
IMAGEINFO|hbmImage<HBITMAP>|hbmMask<HBITMAP>|Unused1|Unused2|rcImage<RECT>
HD_ITEM|imask|lxy|pszText|hbm|cchTextMax|fmt|lParam|iImage|iOrder|itype|pvFilter
HD_LAYOUT|prc|pwpos
HD_HITTESTINFO|pt<POINT>|flags|iItem
NMHEADER|hdr<NMHDR>|iItem|iButton|pitem
HD_NOTIFY|hdr<NMHDR>|iItem|iButton|pitem
TBBUTTON|iBitmap|idCommand|fsState|fsStyle|_wPad1|dwData|iString
COLORMAP|From|To
TBADDBITMAP|hInst|nID
TBREPLACEBITMAP|hInstOld|nIdOld|hInstNew|nIdNew|nButtons
TBSAVEPARAMSA|hkr|pszSubKey|pszValueName
TBSAVEPARAMS|=TBSAVEPARAMSA
NMTOOLBAR|hdr<NMHDR>|iItem|tbButton<TBBUTTON>|cchText|pszText
TBNOTIFY|hdr<NMHDR>|iItem|tbButton<TBBUTTON>|cchText|pszText
TOOLINFO|cbSize|uFlags|hWnd|uId|rect<RECT>|hInst|lpszText|lParam
TT_HITTESTINFO|hWnd|pt<POINT>|ti<TOOLINFO>
NMTTDISPINFO|hdr<NMHDR>|lpszText|szText|hInst|hinst|uFlags|lParam
TOOLTIPTEXT|hdr<NMHDR>|lpszText|szText|hInst|hinst|uFlags|lParam
DRAGLISTINFO|uNotification|hWnd|ptCursor<POINT>
UDACCEL|nSec|nInc
NM_UPDOWN|hdr<NMHDR>|iPos|iDelta
NMUPDOWN|hdr<NMHDR>|iPos|iDelta
LV_ITEM|imask|iItem|iSubItem|state|stateMask|pszText|cchTextMax|iImage|lParam|iIndent
LVITEM|imask|iItem|iSubItem|state|stateMask|pszText|cchTextMax|iImage|lParam|iIndent
LV_FINDINFO|flags|psz|lParampt<POINT>|vkDirection
LVFINDINFO|flags|psz|lParampt<POINT>|vkDirection
LV_HITTESTINFO|pt<POINT>|flags|iItem|iSubItem
LVHITTESTINFO|pt<POINT>|flags|iItem|iSubItem
LV_COLUMN|imask|fmt|lx|pszText|cchTextMax|iSubItem|iImage|iOrder
LVCOLUMN|imask|fmt|lx|pszText|cchTextMax|iSubItem|iImage|iOrder
NM_LISTVIEW|hdr<NMHDR>|iItem|iSubItem|uNewState|uOldState|uChanged|ptAction<POINT>|lParam
NMLISTVIEW|hdr<NMHDR>|iItem|iSubItem|uNewState|uOldState|uChanged|ptAction<POINT>|lParam
LV_DISPINFO|hdr<NMHDR>|item<LVITEM>
LV_KEYDOWN|hdr<NMHDR>|wVKey|flags
NMLVKEYDOWN|hdr<NMHDR>|wVKey|flags
TVITEMA|_mask|hItem|state|stateMask|pszText|cchTextMax|iImage|iSelectedImage|cChildren|lParam
TVITEM|=TVITEMA
TV_ITEMEX|imask|hItem|state|stateMask|pszText|cchTextMax|iImage|iSelectedImage|cChildren|lParam|iIntegral
TVITEMEX|imask|hItem|state|stateMask|pszText|cchTextMax|iImage|iSelectedImage|cChildren|lParam|iIntegral
ITEMTYPE|itemex<TVITEMEX>|item<TVITEM>
TV_INSERTSTRUCT|hParent|hInsertAfter|InsetItem<ITEMTYPE>
TVINSERTSTRUCT|hParent|hInsertAfter|InsetItem<ITEMTYPE>
TV_HITTESTINFO|pt<POINT>|flags|hItem
TVHITTESTINFO|pt<POINT>|flags|hItem
TV_SORTCB|hParent|lpfnCompare|lParam
TVSORTCB|hParent|lpfnCompare|lParam
NM_TREEVIEW|hdr<NMHDR>|action|itemOld<TVITEM>|itemNew<TVITEM>|ptDrag<POINT>
NMTREEVIEW|hdr<NMHDR>|action|itemOld<TVITEM>|itemNew<TVITEM>|ptDrag<POINT>
TV_DISPINFO|hdr<NMHDR>|item<TVITEM>
NMTVDISPINFO|hdr<NMHDR>|item<TVITEM>
TV_KEYDOWN|hdr<NMHDR>|wVKey|flags
NMTVKEYDOWN|hdr<NMHDR>|wVKey|flags
TC_ITEMHEADER|imask|lpReserved1|lpReserved2|pszText|cchTextMax|iImage
TCITEMHEADER|imask|lpReserved1|lpReserved2|pszText|cchTextMax|iImage
TC_ITEM|imask|lpReserved1|lpReserved2|pszText|cchTextMax|iImage|lParam
TCITEM|imask|lpReserved1|lpReserved2|pszText|cchTextMax|iImage|lParam
TC_HITTESTINFO|pt<POINT>|flags
TCHITTESTINFO|pt<POINT>|flags
TC_KEYDOWN|hdr<NMHDR>|wVKey|flags
NMTCKEYDOWN|hdr<NMHDR>|wVKey|flags
CHOOSECOLORA|lStructSize|hwndOwner|hInstance|rgbResult|lpCustColors|Flags|lCustData|lpfnHook|lpTemplateName
CHOOSECOLOR|=CHOOSECOLORA
FINDREPLACEA|lStructSize|hwndOwner|hInstance|Flags|lpstrFindWhat|lpstrReplaceWith|wFindWhatLen|wReplaceWithLen|lCustData|lpfnHook|lpTemplateName
FINDREPLACE|=FINDREPLACEA
CHOOSEFONTA|lStructSize|hwndOwner|hDC|lpLogFont|iPointSize|Flags|rgbColors|lCustData|lpfnHook|lpTemplateName|hInstance|lpszStyle|nFontType|Alignment|nSizeMin|nSizeMax
CHOOSEFONT|=CHOOSEFONTA
DEVNAMES|wDriverOffset|wDeviceOffset|wOutputOffset|wDefault
PRINTDLGA|lStructSize|hwndOwner|hDevMode|hDevNames|hDC|Flags|nFromPage|nToPage|nMinPage|nMaxPage|nCopies|hInstance|lCustData|lpfnPrintHook|lpfnSetupHook|lpPrintTemplateName|lpPrintSetupTemplateName|hPrintTemplate|hSetupTemplate
PRINTDLG|=PRINTDLGA
OPENFILENAMEA|lStructSize|hwndOwner|hInstance|lpstrFilter|lpstrCustomFilter|nMaxCustFilter|nFilterIndex|lpstrFile|nMaxFile|lpstrFileTitle|nMaxFileTitle|lpstrInitialDir|lpstrTitle|Flags|nFileOffset|nFileExtension|lpstrDefExt|lCustData|lpfnHook|lpTemplateName
OPENFILENAME|=OPENFILENAMEA
CHARFORMATA|cbSize|dwMask|dwEffects|yHeight|yOffset|crTextColor|bCharSet|bPitchAndFamily|szFaceName|_wPad2
CHARFORMAT|=CHARFORMATA
CHARRANGE|cpMin|cpMax
TEXTRANGEA|chrg<CHARRANGE>|lpstrText
TEXTRANGE|=TEXTRANGEA
EDITSTREAM|dwCookie|dwError|pfnCallback
FINDTEXTA|chrg<CHARRANGE>|lpstrText
FINDTEXT|=FINDTEXTA
FINDTEXTEXA|chrg<CHARRANGE>|lpstrText|chrgText<CHARRANGE>
FINDTEXTEX|=FINDTEXTEXA
FORMATRANGE|hdc|hdcTarget|rc<RECT>|rcPage<RECT>|chrg<CHARRANGE>
PARAFORMAT|cbSize|dwMask|wNumbering|wEffects|dxStartIndent|dxRightIndent|dxOffset|wAlignment|cTabCount|rgxTabs
PARAFORMAT2|cbSize|dwMask|wNumbering|wEffects|dxStartIndent|dxRightIndent|dxOffset|wAlignment|cTabCount|rgxTabs|dySpaceBefore|dySpaceAfter|dyLineSpacing|sStyle|bLineSpacingRule|bOutlineLevel|wShadingWeight|wShadingStyle|wNumberingStart|wNumberingStyle|wNumberingTab|wBorderSpace|wBorderWidth|wBorders
MSGFILTER|nmhdr<NMHDR>|msg|wParam|lParam
REQRESIZE|nmhdr<NMHDR>|rc<RECT>
SELCHANGE|nmhdr<NMHDR>|chrg<CHARRANGE>|seltyp
ENDROPFILES|nmhdr<NMHDR>|hDrop|cp|fProtected
ENPROTECTED|nmhdr<NMHDR>|msg|wPad1|wParam|wPad2|lParam|chrg<CHARRANGE>
ENSAVECLIPBOARD|nmhdr<NMHDR>|cObjectCount|cch
ENOLEOPFAILED|nmhdr<NMHDR>|iob|lOper|hr
ENCORRECTTEXT|nmhdr<NMHDR>|chrg<CHARRANGE>|seltyp
PUNCTUATION|iSize|szPunctuation
COMPCOLOR|crText|crBackground|dwEffects
REPASTESPECIAL|dwAspect|dwParam
SIZEL|x|y
SIZE|=SIZEL
MSG|hwnd|message|wParam|lParam|time|pt<POINT>
SID_AND_ATTRIBUTES|Sid|Attributes
SID_IDENTIFIER_AUTHORITY|Value
OVERLAPPED|Internal|InternalHigh|loffset|OffsetHigh|hEvent
SECURITY_ATTRIBUTES|nLength|lpSecurityDescriptor|bInheritHandle
PROCESS_INFORMATION|hProcess|hThread|dwProcessId|dwThreadId
FILETIME|dwLowDateTime|dwHighDateTime
SYSTEMTIME|wYear|wMonth|wDayOfWeek|wDay|wHour|wMinute|wSecond|wMilliseconds
COMMPROP|wPacketiLength|wPacketVersion|dwServiceMask|dwReserved1|dwMaxTxQueue|dwMaxRxQueue|dwMaxBaud|dwProvSubType|dwProvCapabilities|dwSettableParams|dwSettableBaud|wSettableData|wSettableStopParity|dwCurrentTxQueue|dwCurrentRxQueue|dwProvSpec1|dwProvSpec2|wcProvChar
COMSTAT|fCtsHold|fDsrHold|fRlsdHold|fXoffHold|fXoffSent|fEof|fTxim|fReserved|cbInQue|cbOutQue
DCB|DCBlength|BaudRate|fBinary|fParity|fOutxCtsFlow|fOutxDsrFlow|fDtrControl|fDsrSensitivity|fTXContinueOnXoff|fOutX|fInX|fErrorChar|fNull|fRtsControl|fAbortOnError|fDummy2|wReserved|XonLim|XoffLim|ByteSize|Parity|StopBits|XonChar|XoffChar|ErrorChar|EofChar|EvtChar|wReserved1
COMMTIMEOUTS|ReadIntervalTimeout|ReadTotalTimeoutMultiplier|ReadTotalTimeoutConstant|WriteTotalTimeoutMultiplier|WriteTotalTimeoutConstant
SYSTEM_INFO|wProcessorArchitecture|wReserved|dwPageSize|lpMinimumApplicationAddress|lpMaximumApplicationAddress|dwActiveProcessorMask|dwNumberOfProcessors|dwProcessorType|dwAllocationGranularity|wProcessorLevel|wProcessorRevision
MEMORYSTATUS|dwLength|dwMemoryLoad|dwTotalPhys|dwAvailPhys|dwTotalPageFile|dwAvailPageFile|dwTotalVirtual|dwAvailVirtual
TPMPARAMS|cbSize|rcExclude<RECT>
GENERIC_MAPPING|GenericRead|GenericWrite|GenericExecute|GenericAll
LUID|LowPart|HighPart
ACL|AclRevision|Sbz1|AclSize|AceCount|Sbz2
ACE_HEADER|AceType|AceFlags|AceSize
ACCESS_ALLOWED_ACE|Header<ACE_HEADER>|imask|SidStart
ACCESS_DENIED_ACE|Header<ACE_HEADER>|imask|SidStart
SYSTEM_AUDIT_ACE|Header<ACE_HEADER>|imask|SidStart
SYSTEM_ALARM_ACE|Header<ACE_HEADER>|imask|SidStart
ACL_REVISION_INFORMATION|AclRevision
ACL_SIZE_INFORMATION|AceCount|AclBytesInUse|AclBytesFree
SECURITY_DESCRIPTOR|Revision|Sbz1|Control|Owner|lGroup|Sacl|Dacl
LUID_AND_ATTRIBUTES|Luid<LUID>|Attributes
PRIVILEGE_SET|PrivilegeCount|Control|Privilege<LUID_AND_ATTRIBUTES>
EXCEPTION_RECORD|ExceptionCode|ExceptionFlags|pExceptionRecord|ExceptionAddress|NumberParameters|ExceptionInformation
EXCEPTION_DEBUG_INFO|pExceptionRecord<EXCEPTION_RECORD>|dwFirstChance
CREATE_THREAD_DEBUG_INFO|hThread|lpThreadLocalBase|lpStartAddress
CREATE_PROCESS_DEBUG_INFO|hFile|hProcess|hThread|lpBaseOfImage|dwDebugInfoFileOffset|nDebugInfoSize|lpThreadLocalBase|lpStartAddress|lpImageName|fUnicode
EXIT_THREAD_DEBUG_INFO|dwExitCode
EXIT_PROCESS_DEBUG_INFO|dwExitCode
LOAD_DLL_DEBUG_INFO|hFile|lpBaseOfDll|dwDebugInfoFileOffset|nDebugInfoSize|lpImageName|fUnicode
UNLOAD_DLL_DEBUG_INFO|lpBaseOfDll
OUTPUT_DEBUG_STRING_INFO|lpDebugStringData|fUnicode|nDebugStringiLength
RIP_INFO|dwError|dwType
OFSTRUCT|cBytes|fFixedDisk|nErrCode|Reserved1|Reserved2|szPathName
WNDCLASSEX|cbSize|style|lpfnWndProc|cbClsExtra|cbWndExtra|hInstance|hIcon|hCursor|hbrBackground|lpszMenuName|lpszClassName|hIconSm
WNDCLASSA|style|lpfnWndProc|cbClsExtra|cbWndExtra|hInstance|hIcon|hCursor|hbrBackground|lpszMenuName|lpszClassName
WNDCLASS|=WNDCLASSA
RTL_CRITICAL_SECTION|DebugInfo|LockCount|RecursionCount|OwningThread|LockSemaphore|SpinCount
CRITICAL_SECTION|=RTL_CRITICAL_SECTION
BY_HANDLE_FILE_INFORMATION|dwFileAttributes|ftCreationTime<FILETIME>|ftLastAccessTime<FILETIME>|ftLastWriteTime<FILETIME>|dwVolumeSerialNumber|nFileSizeHigh|nFileSizeLow|nNumberOfLinks|nFileIndexHigh|nFileIndexLow
MEMORY_BASIC_INFORMATION|BaseAddress|AllocationBase|AllocationProtect|RegionSize|State|Protect|lType
EVENTLOGRECORD|iLength|Reserved|RecordNumber|TimeGenerated|TimeWritten|EventID|EventType|NumStrings|EventCategory|ReservedFlags|ClosingRecordNumber|StringOffset|UserSidiLength|UserSidOffset|DataiLength|DataOffset
TOKEN_GROUPS|GroupCount|Groups<SID_AND_ATTRIBUTES>
TOKEN_PRIVILEGES|PrivilegeCount|Privileges<LUID_AND_ATTRIBUTES>
FLOATING_SAVE_AREA|ControlWord|StatusWord|TagWord|ErrorOffset|ErrorSelector|DataOffset|DataSelector|RegisterArea|Cr0NpxState
CONTEXT|ContextFlags|iDr0|iDr1|iDr2|iDr3|iDr6|iDr7|FloatSave<FLOATING_SAVE_AREA>|regGs|regFs|regEs|regDs|regEdi|regEsi|regEbx|regEdx|regEcx|regEax|regEbp|regEip|regCs|regFlag|regEsp|regSs|ExtendedRegisters
EXCEPTION_POINTERS|pExceptionRecord|ContextRecord
LDT_BYTES|BaseMid|Flags1|Flags2|BaseHi
TIME_ZONE_INFORMATION|Bias|StandardName|StandardDate<SYSTEMTIME>|StandardBias|DaylightName|DaylightDate<SYSTEMTIME>|DaylightBias
LARGE_INTEGER|QuadPart
WIN32_STREAM_ID|dwStreamID|dwStreamAttributes|dwSize<LARGE_INTEGER>|dwStreamNameSize|cStreamName
STARTUPINFOA|cb|lpReserved|lpDesktop|lpTitle|dwX|dwY|dwXSize|dwYSize|dwXCountChars|dwYCountChars|dwFillAttribute|dwFlags|wShowWindow|cbReserved2|lpReserved2|hStdInput|hStdOutput|hStdError
STARTUPINFO|=STARTUPINFOA
WIN32_FIND_DATA|dwFileAttributes|ftCreationTime<FILETIME>|ftLastAccessTime<FILETIME>|ftLastWriteTime<FILETIME>|nFileSizeHigh|nFileSizeLow|dwReserved0|dwReserved1|cFileName|cAlternate
CPINFO|MaxCharSize|DefaultChar|LeadByte
NUMBERFMTA|NumDigits|LeadingZero|Grouping|lpDecimalSep|lpThousandSep|NegativeOrder
NUMBERFMT|=NUMBERFMTA
CURRENCYFMTA|NumDigits|LeadingZero|Grouping|lpDecimalSep|lpThousandSep|NegativeOrder|PositiveOrder|lpCurrencySymbol
CURRENCYFMT|=CURRENCYFMTA
COORD|x|y
SMALL_RECT|Left|Top|Right|Bottom
CHARTYPE|UnicodeChar|AsciiChar
KEY_EVENT_RECORD|bKeyDown|wRepeatCount|wVirtualKeyCode|wVirtualScanCode|uChar<CHARTYPE>|dwControlKeyState
MOUSE_EVENT_RECORD|dwMousePosition<COORD>|dwButtonState|dwControlKeyState|dwEventFlags
WINDOW_BUFFER_SIZE_RECORD|dwSize<COORD>
MENU_EVENT_RECORD|dwCommandId
FOCUS_EVENT_RECORD|bSetFocus
CHAR_INFO|Char<CHARTYPE>|Attributes
CONSOLE_SCREEN_BUFFER_INFO|dwSize<COORD>|dwCursorPosition<COORD>|wAttributes|srWindow<SMALL_RECT>|dwMaximumWindowSize<COORD>
CONSOLE_CURSOR_INFO|dwSize|bVisible
XFORM|eM11|eM12|eM21|eM22|ex|ey
BITMAP|bmType|bmWidth|bmHeight|bmWidthBytes|bmPlanes|bmBitsPixel|bmBits
RGBTRIPLE|rgbtBlue|rgbtGreen|rgbtRed
RGBQUAD|rgbBlue|rgbGreen|rgbRed|rgbReserved
BITMAPCOREHEADER|bcSize|bcWidth|bcHeight|bcPlanes|bcBitCount
BITMAPINFOHEADER|biSize|biWidth|biHeight|biPlanes|biBitCount|biCompression|biSizeImage|biXPelsPerMeter|biYPelsPerMeter|biClrUsed|biClrImportant
BITMAPINFO|bmiHeader<BITMAPINFOHEADER>|bmiColors<RGBQUAD>
BITMAPCOREINFO|bmciHeader<BITMAPCOREHEADER>|bmciColors<RGBTRIPLE>
BITMAPFILEHEADER|bfType|bfSize|bfReserved1|bfReserved2|bfOffBits
HANDLETABLE|objectHandle
METARECORD|rdSize|rdFunction|rdParm1
METAFILEPICT|imm|xExt|yExt|hMF
METAHEADER|mtType|mtHeaderSize|mtVersion|mtSize|mtNoObjects|mtMaxRecord|mtNoParameters
ENHMETARECORD|iType|nSize|dParm1
RECTL|left|top|right|bottom
ENHMETAHEADER|iType|nSize|rclBounds<RECTL>|rclFrame<RECTL>|dSignature|nVersion|nBytes|nRecords|nHandles|sReserved|nDescription|offDescription|nPalEntries|szlDevice<SIZEL>|szlMillimeters<SIZEL>|cbPixelFormat|offPixelFormat|bOpenGL
TEXTMETRICA|tmHeight|tmAscent|tmDescent|tmInternalLeading|tmExternalLeading|tmAveCharWidth|tmMaxCharWidth|tmWeight|tmOverhang|tmDigitizedAspectX|tmDigitizedAspectY|tmFirstChar|tmLastChar|tmDefaultChar|tmBreakChar|tmItalic|tmUnderlined|tmStruckOut|tmPitchAndFamily|tmCharSet
TEXTMETRIC|=TEXTMETRICA
NEWTEXTMETRICA|tmHeight|tmAscent|tmDescent|tmInternalLeading|tmExternalLeading|tmAveCharWidth|tmMaxCharWidth|tmWeight|tmOverhang|tmDigitizedAspectX|tmDigitizedAspectY|tmFirstChar|tmLastChar|tmDefaultChar|tmBreakChar|tmItalic|tmUnderlined|tmStruckOut|tmPitchAndFamily|tmCharSet|ntmFlags|ntmSizeEM|ntmCellHeight|ntmAveWidth
NEWTEXTMETRIC|=NEWTEXTMETRICA
PELARRAY|paXCount|paYCount|paXExt|paYExt|paRGBs
LOGBRUSH|lbStyle|lbColor|lbHatch
LOGPEN|lopnStyle|lopnWidth<POINT>|lopnColor
EXTLOGPEN|elpPenStyle|elpWidth|elpBrushStyle|elpColor|elpHatch|elpNumEntries|elpStyleEntry
PALETTEENTRY|peRed|peGreen|peBlue|peFlags
LOGPALETTE|palVersion|palNumEntries|palPalEntry<PALETTEENTRY>
LOGFONTA|lfHeight|lfWidth|lfEscapement|lfOrientation|lfWeight|lfItalic|lfUnderline|lfStrikeOut|lfCharSet|lfOutPrecision|lfClipPrecision|lfQuality|lfPitchAndFamily|lfFaceName
LOGFONT|=LOGFONTA
NONCLIENTMETRICSA|cbSize|iBorderWidth|iScrollWidth|iScrollHeight|iCaptionWidth|iCaptionHeight|lfCaptionFont|iSMCaptionWidth|iSMCaptionHeight|lfSMCaptionFont<LOGFONTA>|iMenuWidth|iMenuHeight|lfMenuFont<LOGFONTA>|lfStatusFont<LOGFONTA>|lfMessageFont<LOGFONTA>
NONCLIENTMETRICS|=NONCLIENTMETRICSA
ENUMLOGFONTA|elfLogFont<LOGFONTA>|elfFullName|elfStyle
ENUMLOGFONT|=ENUMLOGFONTA
PANOSE|bFamilyType|bSerifStyle|bWeight|bProportion|bContrast|bStrokeVariation|bArmStyle|bLetterform|bMidline|bXHeight
EXTLOGFONTA|elfLogFont<LOGFONTA>|elfFullName|elfStyle|elfVersion|elfStyleSize|elfMatch|elfReserved|elfVendorId|elfCulture|elfPanose<PANOSE>
EXTLOGFONT|=EXTLOGFONTA
DEVMODE|dmDeviceName|dmSpecVersion|dmDriverVersion|dmSize|dmDriverExtra|dmFields|dmOrientation|dmPaperSize|dmPaperLength|dmPaperWidth|dmPosition<POINTL>|dmScale|dmCopies|dmDefaultSource|dmPrintQuality|dmColor|dmDuplex|dmYResolution|dmTTOption|dmCollate|dmFormName|dmLogPixels|dmBitsPerPel|dmPelsWidth|dmPelsHeight|dmDisplayFlags|dmDisplayFrequency|dmICMMethod|dmICMIntent|dmMediaType|dmDitherType|dmReserved1|dmReserved2
RGNDATAHEADER|dwSize|iType|nCount|nRgnSize|rcBound<RECT>
RGNDATA|rdh<RGNDATAHEADER>|Buffer
ABC|abcA|abcB|abcC
ABCFLOAT|abcfA|abcfB|abcfC
OUTLINETEXTMETRICA|otmSize|otmTextMetrics<TEXTMETRICA>|otmFiller|otmPanoseNumber<PANOSE>|otmfsSelection|otmfsType|otmsCharSlopeRise|otmsCharSlopeRun|otmItalicAngle|otmEMSquare|otmAscent|otmDescent|otmLineGap|otmsCapEmHeight|otmsXHeight|otmrcFontBox<RECT>|otmMacAscent|otmMacDescent|otmMacLineGap|otmusMinimumPPEM|otmptSubscriptSize|otmptSubscriptOffset|otmptSuperscriptSize|otmptSuperscriptOffset<POINT>|otmsStrikeoutSize|otmsStrikeoutPosition|otmsUnderscoreSize|otmsUnderscorePosition|otmpFamilyName|otmpFaceName|otmpStyleName|otmpFullName
OUTLINETEXTMETRIC|=OUTLINETEXTMETRICA
POLYTEXTA|x|y|n|lpStr|uiFlags|icl<RECT>|pdx
POLYTEXT|=POLYTEXTA
FIXED|fract|Value
MAT2|eM11<FIXED>|eM12<FIXED>|eM21<FIXED>|eM22<FIXED>
GLYPHMETRICS|gmBlackBoxX|gmBlackBoxY|gmptGlyphOrigin<POINT>|gmCellIncX|gmCellIncY
POINTFX|x<FIXED>|y<FIXED>
TTPOLYCURVE|wType|cpfx|apfx<POINTFX>
TTPOLYGONHEADER|cb|dwType|pfxStart<POINTFX>
RASTERIZER_STATUS|nSize|wFlags|nLanguageID
COLORADJUSTMENT|caSize|caFlags|caIlluminantIndex|caRedGamma|caGreenGamma|caBlueGamma|caReferenceBlack|caReferenceWhite|caContrast|caBrightness|caColorfulness|caRedGreenTint
DOCINFOA|cbSize|lpszDocName|lpszOutput|lpszDatatype|fwType
DOCINFO|=DOCINFOA
KERNINGPAIR|wFirst|wSecond|iKernAmount
EMR|iType|nSize
EMRTEXT|ptlReference<POINTL>|nchars|offString|fOptions|ircl<RECTL>|offDx
EMRABORTPATH|emr<EMR>
EMRBEGINPATH|=EMRABORTPATH
EMRENDPATH|=EMRABORTPATH
EMRCLOSEFIGURE|=EMRABORTPATH
EMRFLATTENPATH|=EMRABORTPATH
EMRWIDENPATH|=EMRABORTPATH
EMRSETMETARGN|=EMRABORTPATH
EMREMRSAVEDC|=EMRABORTPATH
EMRREALIZEPALETTE|=EMRABORTPATH
EMRSELECTCLIPPATH|emr<EMR>|iMode
EMRSETBKMODE|=EMRSELECTCLIPPATH
EMRSETMAPMODE|=EMRSELECTCLIPPATH
EMRSETPOLYFILLMODE|=EMRSELECTCLIPPATH
EMRSETROP2|=EMRSELECTCLIPPATH
EMRSETSTRETCHBLTMODE|=EMRSELECTCLIPPATH
EMRSETTEXTALIGN|=EMRSELECTCLIPPATH
EMRSETMITERLIMIT|emr<EMR>|eMiterLimit
EMRRESTOREDC|emr<EMR>|iRelative
EMRSETARCDIRECTION|emr<EMR>|iArcDirection
EMRSETMAPPERFLAGS|emr<EMR>|dwFlags
EMRSETTEXTCOLOR|emr<EMR>|crColor<COLORREF>
EMRSETBKCOLOR|=EMRSETTEXTCOLOR
EMRSELECTOBJECT|emr<EMR>|ihObject
EMRDELETEOBJECT|=EMRSELECTOBJECT
EMRSELECTCOLORSPACE|emr<EMR>|ihCS
EMRDELETECOLORSPACE|=EMRSELECTCOLORSPACE
EMRSELECTPALETTE|emr<EMR>|ihPal
EMRRESIZEPALETTE|emr<EMR>|ihPal|cEntries
EMRSETPALETTEENTRIES|emr<EMR>|ihPal|iStart|cEntries|aPalEntries<PALETTEENTRY>
EMRSETCOLORADJUSTMENT|emr<EMR>|ColorAdjustment<COLORADJUSTMENT>
EMRGDICOMMENT|emr<EMR>|cbData|xData1
EMREOF|emr<EMR>|nPalEntries|offPalEntries|nSizeLast
EMRLINETO|emr<EMR>|ptl<POINTL>
EMRMOVETOEX|=EMRLINETO
EMROFFSETCLIPRGN|emr<EMR>|ptlOffset<POINTL>
EMRFILLPATH|emr<EMR>|rclBounds<RECTL>
EMRSTROKEANDFILLPATH|=EMRFILLPATH
EMRSTROKEPATH|=EMRFILLPATH
EMREXCLUDECLIPRECT|emr<EMR>|rclClip<RECTL>
EMRINTERSECTCLIPRECT|=EMREXCLUDECLIPRECT
EMRSETVIEWPORTORGEX|emr<EMR>|ptlOrigin<POINTL>
EMRSETWINDOWORGEX|=EMRSETVIEWPORTORGEX
EMRSETBRUSHORGEX|=EMRSETVIEWPORTORGEX
EMRSETVIEWPORTEXTEX|emr<EMR>|szlExtent<SIZEL>
EMRSETWINDOWEXTEX|=EMRSETVIEWPORTEXTEX
EMRSCALEVIEWPORTEXTEX|emr<EMR>|xNum|xDenom|yNum|yDemon
EMRSCALEWINDOWEXTEX|=EMRSCALEVIEWPORTEXTEX
EMRSETWORLDTRANSFORM|emr<EMR>|xform<XFORM>
EMRMODIFYWORLDTRANSFORM|emr<EMR>|xform<XFORM>|iMode
EMRSETPIXELV|emr<EMR>|ptlPixel<POINTL>|crColor<COLORREF>
EMREXTFLOODFILL|emr<EMR>|ptlStart<POINTL>|crColor<COLORREF>|iMode
EMRELLIPSE|emr<EMR>|rclBox<RECTL>
EMRRECTANGLE|=EMRELLIPSE
EMRROUNDRECT|emr<EMR>|rclBox<RECTL>|szlCorner<SIZEL>
EMRARC|emr<EMR>|rclBox<RECTL>|ptlStart<POINTL>|ptlEnd<POINTL>
EMRARCTO|=EMRARC
EMRCHORD|=EMRARC
EMRPIE|=EMRARC
EMRANGLEARC|emr<EMR>|ptlCenter<POINTL>|nRadius|eStartAngle|eSweepAngle
EMRPOLYLINE|emr<EMR>|rclBounds<RECTL>|cptl|aptl<POINTL>
EMRPOLYBEZIER|=EMRPOLYLINE
EMRPOLYGON|=EMRPOLYLINE
EMRPOLYBEZIERTO|=EMRPOLYLINE
EMRPOLYLINE16|emr<EMR>|rclBounds<RECTL>|cpts|apts<POINTS>
EMRPOLYBEZIER16|=EMRPOLYLINE16
EMRPOLYGON16|=EMRPOLYLINE16
EMRPLOYBEZIERTO16|=EMRPOLYLINE16
EMRPOLYLINETO16|=EMRPOLYLINE16
EMRPOLYDRAW|emr<EMR>|rclBounds<RECTL>|cptl|aptl<POINTL>|abTypes1
EMRPOLYDRAW16|emr<EMR>|rclBounds<RECTL>|cpts|apts<POINTS>|abTypes
EMRPOLYPOLYLINE|emr<EMR>|rclBounds<RECTL>|nPolys|cptl|aPolyCounts|aptl<POINTL>
EMRPOLYPOLYGON|=EMRPOLYPOLYLINE
EMRPOLYPOLYLINE16|emr<EMR>|rclBounds<RECTL>|nPolys|cpts|aPolyCounts|apts<POINTS>
EMRPOLYPOLYGON16|=EMRPOLYPOLYLINE16
EMRINVERTRGN|emr<EMR>|rclBounds<RECTL>|cbRgnData|RgnData
EMRPAINTRGN|=EMRINVERTRGN
EMRFILLRGN|emr<EMR>|rclBounds<RECTL>|cbRgnData|ihBrush|RgnData
EMRFRAMERGN|emr<EMR>|rclBounds<RECTL>|cbRgnData|ihBrush|szlStroke<SIZEL>|RgnData
EMREXTSELECTCLIPRGN|emr<EMR>|cbRgnData|iMode|RgnData
EMREXTTEXTOUTA|emr<EMR>|rclBounds<RECTL>|iGraphicsMode|exScale|eyScale|emrtext<EMRTEXT>
EMREXTTEXTOUT|=EMREXTTEXTOUTA
EMRBITBLT|emr<EMR>|rclBounds<RECTL>|xDest|yDest|cxDest|cyDest|dwRop|xSrc|ySrc|xformSrc<XFORM>|crBkColorSrc<COLORREF>|iUsageSrc|offBmiSrc|cbBmiSrc|offBitsSrc|cbBitsSrc
EMRSTRETCHBLT|emr<EMR>|rclBounds<RECTL>|xDest|yDest|cxDest|cyDest|dwRop|xSrc|ySrc|xformSrc<XFORM>|crBkColorSrc<COLORREF>|iUsageSrc|offBmiSrc|cbBmiSrc|offBitsSrc|cbBitsSrc|cxSrc|cySrc
EMRMASKBLT|emr<EMR>|rclBounds<RECTL>|xDest|yDest|cxDest|cyDest|dwRop|xSrc2|cyDest2|dwRop2|xSrc|ySrc|xformSrc<XFORM>|crBkColorSrc<COLORREF>|iUsageSrc|offBmiSrc|cbBmiSrc|offBitsSrc|cbBitsSrc|xMask|yMask|iUsageMask|offBmiMask|cbBmiMask|offBitsMask|cbBitsMask
EMRPLGBLT|emr<EMR>|rclBounds<RECTL>|aptlDest<POINTL>|xSrc|ySrc|cxSrc|cySrc|xformSrc<XFORM>|crBkColorSrc<COLORREF>|iUsageSrc|offBmiSrc|cbBmiSrc|offBitsSrc|cbBitsSrc|xMask|yMask|iUsageMask|offBmiMask|cbBmiMask|offBitsMask|cbBitsMask
EMRSETDIBITSTODEVICE|emr<EMR>|rclBounds<RECTL>|xDest|yDest|xSrc|ySrc|cxSrc|cySrc|offBmiSrc|cbBmiSrc|offBitsSrc|cbBitsSrc|iUsageSrc|iStartScan|cScans
EMRSTRETCHDIBITS|emr<EMR>|rclBounds<RECTL>|xDest|yDest|xSrc|ySrc|cxSrc|cySrc|offBmiSrc|cbBmiSrc|offBitsSrc|cbBitsSrc|iUsageSrc|dwRop|cxDest|cyDest
EMRCREATEPALETTE|emr<EMR>|ihPal|lgpl<LOGPALETTE>
EMRCREATEPEN|emr<EMR>|ihPen|lopn<LOGPEN>
EMREXTCREATEPEN|emr<EMR>|ihPen|offBmi|cbBmi|offBits|cbBits|elp<EXTLOGPEN>
EMRCREATEBRUSHINDIRECT|emr<EMR>|ihBrush|lb<LOGBRUSH>
EMRCREATEMONOBRUSH|emr<EMR>|ihBrush|iUsage|offBmi|cbBmi|offBits|cbBits
EMRCREATEDIBPATTERNBRUSHPT|emr<EMR>|ihBursh|iUsage|offBmi|cbBmi|offBits|cbBits
CIEXYZ|ciexyzX|ciexyzY|ciexyzZ
CIEXYZTRIPLE|ciexyzRed<CIEXYZ>|ciexyzGreen<CIEXYZ>|ciexyzBlue<CIEXYZ>
BITMAPV4HEADER|bV4Size|bV4Width|bV4Height|bV4Planes|bV4BitCount|bV4V4Compression|bV4SizeImage|bV4XPelsPerMeter|bV4YPelsPerMeter|bV4ClrUsed|bV4ClrImportant|bV4RedMask|bV4GreenMask|bV4BlueMask|bV4AlphaMask|bV4CSType|bV4Endpoints<CIEXYZTRIPLE>|bV4GammaRed|bV4GammaGreen|bV4GammaBlue
FONTSIGNATURE|fsUsb4|fsCsb2
CHARSETINFO|ciCharset|ciACP|xlfs<FONTSIGNATURE>
LOCALESIGNATURE|lsUsb4|lsCsbDefault|lsCsbSupported
NEWTEXTMETRICEXA|ntmTm<NEWTEXTMETRICA>|ntmFontSig<FONTSIGNATURE>
NEWTEXTMETRICEX|=NEWTEXTMETRICEXA
ENUMLOGFONTEXA|elfLogFont<LOGFONTA>|elfFullName|elfStyle|elfScript
ENUMLOGFONTEX|=ENUMLOGFONTEXA
GCP_RESULTSA|lStructSize|lpOutString|lpOrder|lpDX|lpCaretPos|lpClass|lpGlyphs|nGlyphs|nMaxFit
GCP_RESULTS|=GCP_RESULTSA
LOGCOLORSPACEA|lcsSignature|lcsVersion|lcsSize|lcsCSType|lcsIntent<LCSGAMUTMATCH>|lcsEndPoints<CIEXYZTRIPLE>|lcsGammaRed|lcsGammaGreen|lcsGammaBlue|lcsFileName
LOGCOLORSPACEW|lcsSignature|lcsVersion|lcsSize|lcsCSType|lcsIntent|lcsEndpoints<CIEXYZTRIPLE>|lcsGammaRed|lcsGammaGreen|lcsGammaBlue|lcsFilename
EMRCREATECOLORSPACE|emr<EMR>|ihCS|lcs<LOGCOLORSPACEW>
CBTACTIVATESTRUCT|fMouse|hWndActive
EVENTMSG|message|paramL|paramH|time|hwnd
CWPSTRUCT|lParam|wParam|message|hwnd
DEBUGHOOKINFO|hModuleHook|Reserved|lParam|wParam|code
MOUSEHOOKSTRUCT|pt<POINT>|hwnd|wHitTestCode|dwExtraInfo
MINMAXINFO|ptReserved<POINT>|ptMaxSize<POINT>|ptMaxPosition<POINT>|ptMinTrackSize<POINT>|ptMaxTrackSize<POINT>
COPYDATASTRUCT|dwData|cbData|lpData
WINDOWPOS|hwnd|hWndInsertAfter|x|y|lx|cy|flags
ACCEL|fVirt|key|cmd
PAINTSTRUCT|hdc|fErase|rcPaint<RECT>|fRestore|fIncUpdate|rgbReserved
CREATESTRUCTA|lpCreateParams|hInstance|hMenu|hWndParent|ly|lx|y|x|style|lpszName|lpszClass|ExStyle
CREATESTRUCT|=CREATESTRUCTA
CBT_CREATEWNDA|lpcs|hWndInsertAfter
CBT_CREATEWND|=CBT_CREATEWNDA
WINDOWPLACEMENT|iLength|flags|showCmd|ptMinPosition<POINT>|ptMaxPosition<POINT>|rcNormalPosition<RECT>
MEASUREITEMSTRUCT|CtlType|CtlID|itemID|itemWidth|itemHeight|itemData
DRAWITEMSTRUCT|CtlType|CtlID|itemID|itemAction|itemState|hwndItem|hdc|rcItem<RECT>|itemData
DELETEITEMSTRUCT|CtlType|CtlID|itemID|hwndItem|itemData
COMPAREITEMSTRUCT|CtlType|CtlID|hwndItem|itemID1|itemData1|itemID2|itemData2|dwLocaleId
DLGTEMPLATE|style|dwExtendedStyle|cdit|x|y|lx|ly
DLGITEMTEMPLATE|style|dwExtendedStyle|x|y|lx|cy|id
MENUITEMTEMPLATEHEADER|versionNumber|loffset
MENUITEMTEMPLATE|mtOption|mtID|mtString
ICONINFO|fIcon|xHotspot|yHotspot|hbmMask|hbmColor
MDICREATESTRUCTA|szClass|szTitle`|hOwner|x|y|lx|ly|style|lParam
MDICREATESTRUCT|=MDICREATESTRUCTA
CLIENTCREATESTRUCT|hWindowMenu|idFirstChild
MULTIKEYHELPA|mkSize|mkKeylist|szKeyphrase
MULTIKEYHELP|=MULTIKEYHELPA
HELPWININFOA|wStructSize|x|y|lx|ly|wMax|rgchMember
HELPWININFO|=HELPWININFOA
DDEACK|usFlags
DDEADVISE|usFlags|cfFormat
DDEDATA|usFlags|cfFormat|Value
DDEPOKE|usFlags|cfFormat|Value
DDELN|usFlags|cfFormat
DDEUP|usFlags|cfFormat|rgb
HSZPAIR|hszSvc|hszTopic
SECURITY_QUALITY_OF_SERVICE|iLength|Impersonationlevel|ContextTrackingMode|EffectiveOnly
CONVCONTEXT|cb|wFlags|wCountryID|iCodePage|dwLangID|dwSecurity|qos<SECURITY_QUALITY_OF_SERVICE>
CONVINFO|cb|hUser|hConvPartner|hszSvcPartner|hszServiceReq|hszTopic|hszItem|wFmt|wType|wStatus|wConvst|wLastError|hConvList|ConvCtxt<CONVCONTEXT>|hwnd|hwndPartner
DDEML_MSG_HOOK_DATA|uiLo|uiHi|cbData|xData
MONMSGSTRUCT|cb|hwndTo|dwTime|htask|wMsg|wParam|lParam|dmhd<DDEML_MSG_HOOK_DATA>
MONCBSTRUCT|cb|dwTime|htask|dwRet|wType|wFmt|hConv|hsz1|hsz2|hData|dwData1|dwData2|cc<CONVCONTEXT>|cbData|xData8
MONHSZSTRUCTA|cb|fsAction|dwTime|hsz|htask|xstr
MONHSZSTRUCT|=MONHSZSTRUCTA
MONERRSTRUCT|cb|wLastError|dwTime|htask
MONLINKSTRUCT|cb|dwTime|htask|fEstablished|fNoData|hszSvc|hszTopic|hszItem|wFmt|fServer|hConvServer|hConvClient
MONCONVSTRUCT|cb|fConnect|dwTime|htask|hszSvc|hszTopic|hConvClient|hConvServer
smpte|hour|minute|sec|frame|fps|dummy|pad
midi|songptrpos
MMUNION|ms|sample|cb|ticks|smp<smpte>|midi1<midi>
MMTIME|wType|u<MMUNION>
MIDIEVENT|dwDeltaTime|dwStreamID|dwEvent|dwParms
MIDISTRMBUFFVER|dwVersion|dwMid|dwOEMVersion
MIDIPROPTIMEDIV|cbStruct|dwTimeDiv
MIDIPROPTEMPO|cbStruct|dwTempo
MIXERCAPSA|wMid|wPid|vDriverVersion<MMVERSION>|szPname|fdwSupport|cDestinations
MIXERCAPS|=MIXERCAPSA
Target|dwType|dwDeviceID|wMid|wPid|vDriverVersion<MMVERSION>|szPname
MIXERLINECONTROLSA|cbStruct|dwLineID|dwControlID|dwControlType|cControls|cbmxctrl|pamxctrl
MIXERLINECONTROLS|=MIXERLINECONTROLSA
MIXERCONTROLDETAILS|cbStruct|dwControlID|cChannels|hwndOwner|cMultipleItems|cbDetails|paDetails
MIXERCONTROLDETAILS_BOOLEAN|fValue
MIXERCONTROLDETAILS_SIGNED|lValue
MIXERCONTROLDETAILS_UNSIGNED|dwValue
JOYINFOEX|dwSize|dwFlags|dwXpos|dwYpos|dwZpos|dwRpos|dwUpos|dwVpos|dwButtons|dwButtonNumber|dwPOV|dwReserved1|dwReserved2
DRVCONFIGINFOEX|dwDCISize|lpszDCISectionName|lpszDCIAliasName|dnDevNode
DRVCONFIGINFO|dwDCISize|lpszDCISectionName|lpszDCIAliasName
WAVEHDR|lpData|dwBufferLength|dwBytesRecorded|dwUser|dwFlags|dwLoops|lpNext|Reserved
WAVEOUTCAPSA|wMid|wPid|vDriverVersion<MMVERSION>|szPname|dwFormats|wChannels|wReserved1|dwSupport
WAVEOUTCAPS|=WAVEOUTCAPSA
WAVEINCAPSA|wMid|wPid|vDriverVersion<MMVERSION>|szPname|dwFormats|wChannels|wReserved1
WAVEINCAPS|=WAVEINCAPSA
WAVEFORMAT|wFormatTag|nChannels|nSamplesPerSec|nAvgBytesPerSec|nBlockAlign
PCMWAVEFORMAT|wf<WAVEFORMAT>|wBitsPerSample
MIDIOUTCAPSA|wMid|wPid|vDriverVersion<MMVERSION>|szPname|wTechnology|wVoices|wNotes|wChannelMask|dwSupport
MIDIOUTCAPS|=MIDIOUTCAPSA
MIDIINCAPSA|wMid|wPid|vDriverVersion<MMVERSION>|szPname|dwSupport
MIDIINCAPS|=MIDIINCAPSA
MIDIHDR|lpData|dwBufferiLength|dwBytesRecorded|dwUser|dwFlags|lpNext|Reserved|dwOffset|dwReserved
AUXCAPSA|wMid|wPid|vDriverVersion<MMVERSION>|szPname|wTechnology|dwSupport
AUXCAPS|=AUXCAPSA
TIMECAPS|wPeriodMin|wPeriodMax
JOYCAPSA|wMid|wPid|szPname|wXmin|wXmax|wYmin|wYmax|wZmin|wZmax|wNumButtons|wPeriodMin|wPeriodMax|wRmin|wRmax|wUmin|wUmax|wVmin|wVmax|wCaps|wMaxAxes|wNumAxes|wMaxButtons|szRegKey|szOEMVxD
JOYCAPS|=JOYCAPSA
JOYINFO|wXpos|wYpos|wZpos|wButtons
MMIOINFO|dwFlags|fccIOProc|pIOProc|wErrorRet|htask|cchBuffer|pchBuffer|pchNext|pchEndRead|pchEndWrite|lBufOffset|lDiskOffset|adwInfo|dwReserved1|dwReserved2|hmmio
MMCKINFO|ckid|ckSize|fccType|dwDataOffset|dwFlags
MCI_GENERIC_PARMS|dwCallback
MCI_OPEN_PARMS|dwCallback|wDeviceID|lpstrDeviceType|lpstrElementName|lpstrAlias
MCI_PLAY_PARMS|dwCallback|dwFrom|dwTo
MCI_SEEK_PARMS|dwCallback|dwTo
MCI_STATUS_PARMS|dwCallback|dwReturn|dwItem|dwTrack
MCI_INFO_PARMSA|dwCallback|lpstrReturn|dwRetSize
MCI_INFO_PARMS|=MCI_INFO_PARMSA
MCI_GETDEVCAPS_PARMS|dwCallback|dwReturn|dwIten
MCI_SYSINFO_PARMSA|dwCallback|lpstrReturn|dwRetSize|dwNumber|wDeviceType
MCI_SYSINFO_PARMS|=MCI_SYSINFO_PARMSA
MCI_SET_PARMS|dwCallback|dwTimeFormat|dwAudio
MCI_BREAK_PARMS|dwCallback|nVirtKey|hwndBreak
MCI_SOUND_PARMS|dwCallback|lpstrSoundName
MCI_SAVE_PARMSA|dwCallback|lpFileName
MCI_SAVE_PARMS|=MCI_SAVE_PARMSA
MCI_LOAD_PARMSA|dwCallback|lpFileName
MCI_LOAD_PARMS=MCI_LOAD_PARMSA
MCI_RECORD_PARMS|dwCallback|dwFrom|dwTo
MCI_VD_PLAY_PARMS|dwCallback|dwFrom|dwTo|dwSpeed
MCI_VD_STEP_PARMS|dwCallback|dwFrames
MCI_VD_ESCAPE_PARMSA|dwCallback|lpstrCommand
MCI_VD_ESCAPE_PARMS|=MCI_VD_ESCAPE_PARMSA
MCI_WAVE_OPEN_PARMSA|dwCallback|wDeviceID|lpstrDeviceType|lpstrElementName|lpstrAlias|dwBufferSeconds
MCI_WAVE_OPEN_PARMS|=MCI_WAVE_OPEN_PARMSA
MCI_WAVE_DELETE_PARMS|dwCallback|dwFrom|dwTo
MCI_WAVE_SET_PARMS|dwCallback|dwTimeFormat|dwAudio|wInput|wOutput|wFormatTag|wReserved2|nChannels|wReserved3|nSamplesPerSec|nAvgBytesPerSec|nBlockAlign|wReserved4|wBitsPerSample|wReserved5
MCI_SEQ_SET_PARMS|dwCallback|dwTimeFormat|dwAudio|dwTempo|dwPort|dwSlave|dwMaster|dwOffset
MCI_ANIM_OPEN_PARMSA|dwCallback|wDeviceID|lpstrDeviceType|lpstrElementName|lpstrAlias|dwStyle|hWndParent
MCI_ANIM_OPEN_PARMS|=MCI_ANIM_OPEN_PARMSA
MCI_ANIM_PLAY_PARMS|dwCallback|dwFrom|dwTo|dwSpeed
MCI_ANIM_STEP_PARMS|dwCallback|dwFrames
MCI_ANIM_WINDOW_PARMS|dwCallback|hwnd|nCmdShow|lpstrText
MCI_ANIM_RECT_PARMS|dwCallback|rc<RECT>
MCI_ANIM_UPDATE_PARMS|dwCallback|rc<RECT>|hdc
MCI_OVLY_OPEN_PARMSA|dwCallback|wDeviceID|lpstrDeviceType|lpstrElementName|lpstrAlias|dwStyle|hWndParent
MCI_OVLY_OPEN_PARMS|=MCI_OVLY_OPEN_PARMSA

MCI_OVLY_WINDOW_PARMSA STRUCT
  dwCallback    DWORD       ?
  hwnd          DWORD       ?
  nCmdShow      DWORD       ?
  lpstrText     DWORD       ?
MCI_OVLY_WINDOW_PARMSA ENDS

MCI_OVLY_WINDOW_PARMS  equ  <MCI_OVLY_WINDOW_PARMSA>

MCI_OVLY_RECT_PARMS STRUCT
  dwCallback    DWORD       ?
  rc            RECT        <>
MCI_OVLY_RECT_PARMS ENDS

MCI_OVLY_SAVE_PARMSA STRUCT
  dwCallback    DWORD      ?
  lpFileName    DWORD      ?
  rc            RECT       <>
MCI_OVLY_SAVE_PARMSA ENDS

MCI_OVLY_SAVE_PARMS  equ  <MCI_OVLY_SAVE_PARMSA>

MCI_OVLY_LOAD_PARMSA STRUCT
  dwCallback    DWORD   ?
  lpFileName    DWORD   ?
  rc            RECT    <>
MCI_OVLY_LOAD_PARMSA ENDS

MCI_OVLY_LOAD_PARMS  equ  <MCI_OVLY_LOAD_PARMSA>

PRINTER_INFO_1A STRUCT
  flags         DWORD      ?
  pDescription  DWORD      ?
  pName         DWORD      ?
  pComment      DWORD      ?
PRINTER_INFO_1A ENDS

PRINTER_INFO_1  equ  <PRINTER_INFO_1A>

PRINTER_INFO_2A STRUCT
  pServerName           DWORD      ?
  pPrinterName          DWORD      ?
  pShareName            DWORD      ?
  pPortName             DWORD      ?
  pDriverName           DWORD      ?
  pComment              DWORD      ?
  pLocation             DWORD      ?
  pDevMode              DWORD      ?
  pSepFile              DWORD      ?
  pPrintProcessor       DWORD      ?
  pDatatype             DWORD      ?
  pParameters           DWORD      ?
  pSecurityDescriptor   DWORD      ?
  Attributes            DWORD      ?
  Priority              DWORD      ?
  DefaultPriority       DWORD      ?
  StartTime             DWORD      ?
  UntilTime             DWORD      ?
  Status                DWORD      ?
  cJobs                 DWORD      ?
  AveragePPM            DWORD      ?
PRINTER_INFO_2A ENDS

PRINTER_INFO_2  equ  <PRINTER_INFO_2A>

PRINTER_INFO_3 STRUCT
  pSecurityDescriptor  DWORD      ?
PRINTER_INFO_3 ENDS

PRINTER_INFO_4A STRUCT
  pPrinterName  DWORD ?
  pServerName   DWORD ?
  Attributes    DWORD ?
PRINTER_INFO_4A ENDS

PRINTER_INFO_4  equ  <PRINTER_INFO_4A>

JOB_INFO_1A STRUCT
  JobId         DWORD      ?
  pPrinterName  DWORD      ?
  pMachineName  DWORD      ?
  pUserName     DWORD      ?
  pDocument     DWORD      ?
  pDatatype     DWORD      ?
  pStatus       DWORD      ?
  Status        DWORD      ?
  Priority      DWORD      ?
  Position      DWORD      ?
  TotalPages    DWORD      ?
  PagesPrinted  DWORD      ?
  Submitted     SYSTEMTIME <>
JOB_INFO_1A ENDS

JOB_INFO_1  equ  <JOB_INFO_1A>

JOB_INFO_2A STRUCT
  JobId                 DWORD      ?
  pPrinterName          DWORD      ?
  pMachineName          DWORD      ?
  pUserName             DWORD      ?
  pDocument             DWORD      ?
  pNotifyName           DWORD      ?
  pDatatype             DWORD      ?
  pPrintProcessor       DWORD      ?
  pParameters           DWORD      ?
  pDriverName           DWORD      ?
  pDevMode              DWORD      ?
  pStatus               DWORD      ?
  pSecurityDescriptor   DWORD      ?
  Status                DWORD      ?
  Priority              DWORD      ?
  Position              DWORD      ?
  StartTime             DWORD      ?
  UntilTime             DWORD      ?
  TotalPages            DWORD      ?
  isize                 DWORD      ?
  Submitted             SYSTEMTIME <>
  time                  DWORD      ?
  PagesPrinted          DWORD      ?
JOB_INFO_2A ENDS

JOB_INFO_2  equ  <JOB_INFO_2A>

ADDJOB_INFO_1A STRUCT
  Path      DWORD      ?
  JobId     DWORD      ?
ADDJOB_INFO_1A ENDS

ADDJOB_INFO_1  equ  <ADDJOB_INFO_1A>

DRIVER_INFO_1A STRUCT
  pName     DWORD      ?
DRIVER_INFO_1A ENDS

DRIVER_INFO_1  equ  <DRIVER_INFO_1A>

DRIVER_INFO_2A STRUCT
  cVersion      DWORD      ?
  pName         DWORD      ?
  pEnvironment  DWORD      ?
  pDriverPath   DWORD      ?
  pDataFile     DWORD      ?
  pConfigFile   DWORD      ?
DRIVER_INFO_2A ENDS

DRIVER_INFO_2  equ  <DRIVER_INFO_2A>

DOC_INFO_1A STRUCT
  pDocName      DWORD      ?
  pOutputFile   DWORD      ?
  pDatatype     DWORD      ?
DOC_INFO_1A ENDS

DOC_INFO_1  equ  <DOC_INFO_1A>

FORM_INFO_1A STRUCT
  Flags         DWORD       ?
  pName         DWORD       ?
  iSize         SIZEL       <>
  ImageableArea RECTL       <>
FORM_INFO_1A ENDS

FORM_INFO_1  equ  <FORM_INFO_1A>

PRINTPROCESSOR_INFO_1A STRUCT
  pName  DWORD      ?
PRINTPROCESSOR_INFO_1A ENDS

PRINTPROCESSOR_INFO_1  equ  <PRINTPROCESSOR_INFO_1A>

PORT_INFO_1A STRUCT
  pName  DWORD      ?
PORT_INFO_1A ENDS

PORT_INFO_1  equ  <PORT_INFO_1A>

MONITOR_INFO_1A STRUCT
  pName  DWORD      ?
MONITOR_INFO_1A ENDS

MONITOR_INFO_1  equ  <MONITOR_INFO_1A>

MONITOR_INFO_2A STRUCT
  pName         DWORD      ?
  pEnvironment  DWORD      ?
  pDLLName      DWORD      ?
MONITOR_INFO_2A ENDS

MONITOR_INFO_2  equ  <MONITOR_INFO_2A>

DATATYPES_INFO_1A STRUCT
  pName  DWORD      ?
DATATYPES_INFO_1A ENDS

DATATYPES_INFO_1  equ  <DATATYPES_INFO_1A>

PRINTER_DEFAULTSA STRUCT
  pDatatype         DWORD       ?
  pDevMode          DWORD       ?
  DesiredAccess     ACCESS_MASK ?
PRINTER_DEFAULTSA ENDS

PRINTER_DEFAULTS  equ  <PRINTER_DEFAULTSA>



PRINTER_INFO_5A STRUCT
  pPrinterName              DWORD      ?
  pPortName                 DWORD      ?
  Attributes                DWORD      ?
  DeviceNotSelectedTimeout  DWORD      ?
  TransmissionRetryTimeout  DWORD      ?
PRINTER_INFO_5A ENDS

PRINTER_INFO_5  equ  <PRINTER_INFO_5A>

DRIVER_INFO_3A STRUCT
  cVersion          DWORD      ?
  pName             DWORD      ?
  pEnvironment      DWORD      ?
  pDriverPath       DWORD      ?
  pDataFile         DWORD      ?
  pConfigFile       DWORD      ?
  pHelpFile         DWORD      ?
  pDependentFiles   DWORD      ?
  pMonitorName      DWORD      ?
  pDefaultDataType  DWORD      ?
DRIVER_INFO_3A ENDS

DRIVER_INFO_3  equ  <DRIVER_INFO_3A>

DOC_INFO_2A STRUCT
  pDocName      DWORD      ?
  pOutputFile   DWORD      ?
  pDatatype     DWORD      ?
  dwMode        DWORD      ?
  JobId         DWORD      ?
DOC_INFO_2A ENDS

DOC_INFO_2  equ  <DOC_INFO_2A>

PORT_INFO_2A STRUCT
  pPortName     DWORD      ?
  pMonitorName  DWORD      ?
  pDescription  DWORD      ?
  fPortType     DWORD      ?
  Reserved      DWORD      ?
PORT_INFO_2A ENDS

PORT_INFO_2  equ  <PORT_INFO_2A>

PROVIDOR_INFO_1A STRUCT
  pName         DWORD      ?
  pEnvironment  DWORD      ?
  pDLLName      DWORD      ?
PROVIDOR_INFO_1A ENDS

PROVIDOR_INFO_1  equ  <PROVIDOR_INFO_1A>

NETRESOURCEA STRUCT
  dwScope           DWORD      ?
  dwType            DWORD      ?
  dwDisplayType     DWORD      ?
  dwUsage           DWORD      ?
  lpLocalName       DWORD      ?
  lpRemoteName      DWORD      ?
  lpComment         DWORD      ?
  lpProvider        DWORD      ?
NETRESOURCEA ENDS

NETRESOURCE  equ  <NETRESOURCEA>

NCB STRUCT
  ncb_command       BYTE    ?
  ncb_retcode       BYTE    ?
  ncb_lsn           BYTE    ?
  ncb_num           BYTE    ?
  ncb_buffer        DWORD   ?
  ncb_length        WORD    ?
  ncb_callname      BYTE NCBNAMSZ dup (?)
  ncb_name          BYTE NCBNAMSZ dup (?)
  ncb_rto           BYTE    ?
  ncb_sto           BYTE    ?
  ncb_post          DWORD   ?
  ncb_lana_num      BYTE    ?
  ncb_cmd_cplt      BYTE    ?
  ncb_reserve       BYTE    10 DUP(?)
  ncb_event         DWORD   ?
NCB ENDS

ADAPTER_STATUS STRUCT
  adapter_address   BYTE 6 dup (?)
  rev_major         BYTE      ?
  reserved0         BYTE      ?
  adapter_type      BYTE      ?
  rev_minor         BYTE      ?
  duration          WORD      ?
  frmr_recv         WORD      ?
  frmr_xmit         WORD      ?
  iframe_recv_err   WORD      ?
  xmit_aborts       WORD      ?
  xmit_success      DWORD     ?
  recv_success      DWORD     ?
  iframe_xmit_err   WORD      ?
  recv_buff_unavail WORD      ?
  t1_timeouts       WORD      ?
  ti_timeouts       WORD      ?
  rserved1          DWORD     ?
  free_ncbs         WORD      ?
  max_cfg_ncbs      WORD      ?
  max_ncbs          WORD      ?
  xmit_buf_unavail  WORD      ?
  max_dgram_isize   WORD      ?
  pending_sess      WORD      ?
  max_cfg_sess      WORD      ?
  max_sess          WORD      ?
  max_sess_pkt_isize WORD     ?
  name_count        WORD      ?
ADAPTER_STATUS ENDS

NAME_BUFFER STRUCT
  xname         BYTE NCBNAMSZ dup (?)
  name_num      BYTE      ?
  name_flags    BYTE      ?
NAME_BUFFER ENDS

SESSION_HEADER STRUCT
  sess_name             BYTE      ?
  num_sess              BYTE      ?
  rcv_dg_outstanding    BYTE      ?
  rcv_any_outstanding   BYTE      ?
SESSION_HEADER ENDS

SESSION_BUFFER STRUCT
  lsn                   BYTE      ?
  State                 BYTE      ?
  local_name            BYTE NCBNAMSZ dup (?)
  remote_name           BYTE NCBNAMSZ dup (?)
  rcvs_outstanding      BYTE      ?
  sends_outstanding     BYTE      ?
SESSION_BUFFER ENDS

LANA_ENUM STRUCT
  iLength   BYTE      ?
  lana      BYTE    MAX_LANA+1 DUP(?)
LANA_ENUM ENDS

FIND_NAME_HEADER STRUCT
  node_count    WORD      ?
  Reserved      BYTE      ?
  unique_group  BYTE      ?
FIND_NAME_HEADER ENDS

FIND_NAME_BUFFER STRUCT
  iLength           BYTE        ?
  access_control    BYTE        ?
  frame_control     BYTE        ?
  destination_addr  BYTE  6 DUP(?)
  source_addr       BYTE  6 DUP(?)
  routing_info      BYTE 18 DUP(?)
FIND_NAME_BUFFER ENDS

ACTION_HEADER STRUCT
  transport_id  DWORD      ?
  action_code   WORD      ?
  Reserved      WORD      ?
ACTION_HEADER ENDS

CRGB STRUCT
  bRed      BYTE      ?
  bGreen    BYTE      ?
  bBlue     BYTE      ?
  bExtra    BYTE      ?
CRGB ENDS

SERVICE_STATUS STRUCT
  dwServiceType             DWORD      ?
  dwCurrentState            DWORD      ?
  dwControlsAccepted        DWORD      ?
  dwWin32ExitCode           DWORD      ?
  dwServiceSpecificExitCode DWORD      ?
  dwCheckPoint              DWORD      ?
  dwWaitHint                DWORD      ?
SERVICE_STATUS ENDS

ENUM_SERVICE_STATUSA STRUCT
  lpServiceName  DWORD      ?
  lpDisplayName  DWORD      ?
  ServiceStatus  SERVICE_STATUS <>
ENUM_SERVICE_STATUSA ENDS

ENUM_SERVICE_STATUS  equ  <ENUM_SERVICE_STATUSA>

QUERY_SERVICE_LOCK_STATUSA STRUCT
  fIsLocked         DWORD      ?
  lpLockOwner       DWORD      ?
  dwLockDuration    DWORD      ?
QUERY_SERVICE_LOCK_STATUSA ENDS

QUERY_SERVICE_LOCK_STATUS  equ  <QUERY_SERVICE_LOCK_STATUSA>

QUERY_SERVICE_CONFIGA STRUCT
  dwServiceType         DWORD      ?
  dwStartType           DWORD      ?
  dwErrorControl        DWORD      ?
  lpBinaryPathName      DWORD      ?
  lpLoadOrderGroup      DWORD      ?
  dwTagId               DWORD      ?
  lpDependencies        DWORD      ?
  lpServiceStartName    DWORD      ?
  lpDisplayName         DWORD      ?
QUERY_SERVICE_CONFIGA ENDS

QUERY_SERVICE_CONFIG  equ  <QUERY_SERVICE_CONFIGA>

SERVICE_TABLE_ENTRYA STRUCT
  lpServiceName  DWORD      ?
  lpServiceProc  DWORD      ?
SERVICE_TABLE_ENTRYA ENDS

SERVICE_TABLE_ENTRY  equ  <SERVICE_TABLE_ENTRYA>

PERF_DATA_BLOCK STRUCT
  Signature             WORD 4 dup (?)
  LittleEndian          DWORD      ?
  Version               DWORD      ?
  Revision              DWORD      ?
  TotalByteiLength      DWORD      ?
  HeaderiLength         DWORD      ?
  NumObjectTypes        DWORD      ?
  DefaultObject         DWORD      ?
  SystemTime            SYSTEMTIME <>
  Padding               DWORD ?
  PerfTime              LARGE_INTEGER <>
  PerfFreq              LARGE_INTEGER <>
  PerTime100nSec        LARGE_INTEGER <>
  SystemNameiLength     DWORD      ?
  SystemNameOffset      DWORD      ?
PERF_DATA_BLOCK ENDS

PERF_OBJECT_TYPE STRUCT
  TotalByteiLength      DWORD      ?
  DefinitioniLength     DWORD      ?
  HeaderiLength         DWORD      ?
  ObjectNameTitleIndex  DWORD      ?
  ObjectNameTitle       DWORD      ?
  ObjectHelpTitleIndex  DWORD      ?
  ObjectHelpTitle       DWORD      ?
  DetailLevel           DWORD      ?
  NumCounters           DWORD      ?
  DefaultCounter        DWORD      ?
  NumInstances          DWORD      ?
  CodePage              DWORD      ?
  PerfTime              LARGE_INTEGER <>
  PerfFreq              LARGE_INTEGER <>
PERF_OBJECT_TYPE ENDS

PERF_COUNTER_DEFINITION STRUCT
  ByteiLength               DWORD      ?
  CounterNameTitleIndex     DWORD      ?
  CounterNameTitle          DWORD      ?
  CounterHelpTitleIndex     DWORD      ?
  CounterHelpTitle          DWORD      ?
  DefaultScale              DWORD      ?
  DetailLevel               DWORD      ?
  CounterType               DWORD      ?
  CounterSize               DWORD      ?
  CounterOffset             DWORD      ?
PERF_COUNTER_DEFINITION ENDS

PERF_INSTANCE_DEFINITION STRUCT
  ByteiLength               DWORD      ?
  ParentObjectTitleIndex    DWORD      ?
  ParentObjectInstance      DWORD      ?
  UniqueID                  DWORD      ?
  NameOffset                DWORD      ?
  NameiLength               DWORD      ?
PERF_INSTANCE_DEFINITION ENDS

PERF_COUNTER_BLOCK STRUCT
  ByteiLength  DWORD      ?
PERF_COUNTER_BLOCK ENDS

COMPOSITIONFORM STRUCT
  dwStyle       DWORD      ?
  ptCurrentPos  POINT     <>
  rcArea        RECT      <>
COMPOSITIONFORM ENDS

CANDIDATEFORM STRUCT
  dwIndex       DWORD      ?
  dwStyle       DWORD      ?
  ptCurrentPos  POINT     <>
  rcArea        RECT      <>
CANDIDATEFORM ENDS

CANDIDATELIST STRUCT
  dwSize        DWORD      ?
  dwStyle       DWORD      ?
  dwCount       DWORD      ?
  dwSelection   DWORD      ?
  dwPageStart   DWORD      ?
  dwPageSize    DWORD      ?
  dwOffset1     DWORD      ?
CANDIDATELIST ENDS

STYLEBUFA STRUCT
  dwStyle           DWORD      ?
  szDescription     BYTE STYLE_DESCRIPTION_SIZE dup (?)
STYLEBUFA ENDS

STYLEBUF  equ  <STYLEBUFA>

MODEMDEVCAPS STRUCT
  dwActualSize                  DWORD      ?
  dwRequiredSize                DWORD      ?
  dwDevSpecificOffset           DWORD      ?
  dwDevSpecificSize             DWORD      ?
  dwModemProviderVersion        DWORD      ?
  dwModemManufacturerOffset     DWORD      ?
  dwModemManufacturerSize       DWORD      ?
  dwModemModelOffset            DWORD      ?
  dwModemModelSize              DWORD      ?
  dwModemVersionOffset          DWORD      ?
  dwModemVersionSize            DWORD      ?
  dwDialOptions                 DWORD      ?
  dwCallSetupFailTimer          DWORD      ?
  dwInactivityTimeout           DWORD      ?
  dwSpeakerVolume               DWORD      ?
  dwSpeakerMode                 DWORD      ?
  dwModemOptions                DWORD      ?
  dwMaxDTERate                  DWORD      ?
  dwMaxDCERate                  DWORD      ?
  abVariablePortion             BYTE       ?
MODEMDEVCAPS ENDS

MODEMSETTINGS STRUCT
  dwActualSize              DWORD      ?
  dwRequiredSize            DWORD      ?
  dwDevSpecificOffset       DWORD      ?
  dwDevSpecificSize         DWORD      ?
  dwCallSetupFailTimer      DWORD      ?
  dwInactivityTimeout       DWORD      ?
  dwSpeakerVolume           DWORD      ?
  dwSpeakerMode             DWORD      ?
  dwPreferredModemOptions   DWORD      ?
  dwNegotiatedModemOptions  DWORD      ?
  dwNegotiatedDCERate       DWORD      ?
  abVariablePortion         BYTE       ?
MODEMSETTINGS ENDS

DRAGINFOA STRUCT
  uSize         DWORD      ?
  pt            POINT      <>
  fNC           DWORD      ?
  lpFileList    DWORD      ?
  grfKeyState   DWORD      ?
DRAGINFOA ENDS

DRAGINFO  equ  <DRAGINFOA>

APPBARDATA STRUCT
  cbSize            DWORD      ?
  hwnd              DWORD      ?
  uCallbackMessage  DWORD      ?
  uEdge             DWORD      ?
  rc                RECT       <>
  lParam            DWORD      ?
APPBARDATA ENDS

SHFILEOPSTRUCTA STRUCT
  hwnd                  DWORD      ?
  wFunc                 DWORD      ?
  pFrom                 DWORD      ?
  pTo                   DWORD      ?
  fFlags                FILEOP_FLAGS ?
  fAnyOperationsAborted DWORD      ?
  hNameMappings         DWORD      ?
  lpszProgressTitle     DWORD      ?
SHFILEOPSTRUCTA ENDS

SHFILEOPSTRUCT  equ  <SHFILEOPSTRUCTA>

SHNAMEMAPPINGA STRUCT
  pszOldPath  DWORD      ?
  pszNewPath  DWORD      ?
  cchOldPath  DWORD      ?
  cchNewPath  DWORD      ?
SHNAMEMAPPINGA ENDS

SHNAMEMAPPING  equ  <SHNAMEMAPPINGA>

SHELLEXECUTEINFOA STRUCT
  cbSize        DWORD       ?
  fMask         DWORD       ?
  hwnd          DWORD       ?
  lpVerb        DWORD       ?
  lpFile        DWORD       ?
  lpParameters  DWORD       ?
  lpDirectory   DWORD       ?
  nShow         DWORD       ?
  hInstApp      DWORD       ?
  lpIDList      DWORD       ?
  lpClass       DWORD       ?
  hkeyClass     DWORD       ?
  dwHotKey      DWORD       ?
  union
      hIcon     DWORD       ?
      hMonitor  DWORD       ?
  ends
  hProcess      DWORD       ?
SHELLEXECUTEINFOA ENDS

SHELLEXECUTEINFO  equ  <SHELLEXECUTEINFOA>

NOTIFYICONDATAA STRUCT
  cbSize            DWORD      ?
  hwnd              DWORD      ?
  uID               DWORD      ?
  uFlags            DWORD      ?
  uCallbackMessage  DWORD      ?
  hIcon             DWORD      ?
  szTip             BYTE 64 dup (?)
NOTIFYICONDATAA ENDS

NOTIFYICONDATA  equ  <NOTIFYICONDATAA>

SHFILEINFOA STRUCT
  hIcon             DWORD       ?
  iIcon             DWORD       ?
  dwAttributes      DWORD       ?
  szDisplayName     BYTE        MAX_PATH DUP(?)
  szTypeName        BYTE        80 dup(?)
SHFILEINFOA ENDS

SHFILEINFO  equ  <SHFILEINFOA>

VS_FIXEDFILEINFO STRUCT
  dwSignature           DWORD      ?
  dwStrucVersion        DWORD      ?
  dwFileVersionMS       DWORD      ?
  dwFileVersionLS       DWORD      ?
  dwProductVersionMS    DWORD      ?
  dwProductVersionLS    DWORD      ?
  dwFileFlagsMask       DWORD      ?
  dwFileFlags           DWORD      ?
  dwFileOS              DWORD      ?
  dwFileType            DWORD      ?
  dwFileSubtype         DWORD      ?
  dwFileDateMS          DWORD      ?
  dwFileDateLS          DWORD      ?
VS_FIXEDFILEINFO ENDS

ICONMETRICSA STRUCT
  cbSize        DWORD      ?
  iHorzSpacing  DWORD      ?
  iVertSpacing  DWORD      ?
  iTitleWrap    DWORD      ?
  lfFont        LOGFONTA   <>
ICONMETRICSA ENDS

ICONMETRICS  equ  <ICONMETRICSA>

HELPINFO STRUCT
  cbSize        DWORD      ?
  iContextType  DWORD      ?
  iCtrlId       DWORD      ?
  hItemHandle   DWORD      ?
  dwContextId   DWORD      ?
  MousePos      POINT     <>
HELPINFO ENDS

ANIMATIONINFO STRUCT
  cbSize        DWORD      ?
  iMinAnimate   DWORD      ?
ANIMATIONINFO ENDS

MINIMIZEDMETRICS STRUCT
  cbSize    DWORD      ?
  iWidth    DWORD      ?
  iHorzGap  DWORD      ?
  iVertGap  DWORD      ?
  iArrange  DWORD      ?
MINIMIZEDMETRICS ENDS

OSVERSIONINFOA STRUCT
  dwOSVersionInfoSize   DWORD      ?
  dwMajorVersion        DWORD      ?
  dwMinorVersion        DWORD      ?
  dwBuildNumber         DWORD      ?
  dwPlatformId          DWORD      ?
  szCSDVersion          BYTE 128 dup (?)
OSVERSIONINFOA ENDS

OSVERSIONINFO  equ  <OSVERSIONINFOA>

SYSTEM_POWER_STATUS STRUCT
  ACLineStatus          BYTE      ?
  BatteryFlag           BYTE      ?
  BatteryLifePercent    BYTE      ?
  Reserved1             BYTE      ?
  BatteryLifeTime       DWORD      ?
  BatteryFullLifeTime   DWORD      ?
SYSTEM_POWER_STATUS ENDS



PAGESETUPDLGA STRUCT
  lStructSize               DWORD      ?
  hwndOwner                 DWORD      ?
  hDevMode                  DWORD      ?
  hDevNames                 DWORD      ?
  union
  	Flags                     DWORD      ?
  	flags dd ?
  ends
  ptPaperSize               POINT     <>
  rtMinMargin               RECT      <>
  rtMargin                  RECT      <>
  hInstance                 DWORD      ?
  lCustData                 DWORD      ?
  lpfnPageSetupHook         DWORD      ?
  lpfnPagePaintHook         DWORD      ?
  lpPageSetupTemplateName   DWORD      ?
  hPageSetupTemplate        DWORD      ?
PAGESETUPDLGA ENDS

PAGESETUPDLG  equ  <PAGESETUPDLGA>

COMMCONFIG STRUCT
  dwSize                DWORD      ?
  wVersion              WORD       ?
  wReserved             WORD       ?
  dcbx                  DCB        <>
  dwProviderSubType     DWORD      ?
  dwProviderOffset      DWORD      ?
  dwProviderSize        DWORD      ?
  wcProviderData        WORD       ?
COMMCONFIG ENDS

PIXELFORMATDESCRIPTOR STRUCT
  nSize             WORD      ?
  nVersion          WORD      ?
  dwFlags           DWORD      ?
  iPixelType        BYTE      ?
  cColorBits        BYTE      ?
  cRedBits          BYTE      ?
  cRedShift         BYTE      ?
  cGreenBits        BYTE      ?
  cGreenShift       BYTE      ?
  cBlueBits         BYTE      ?
  cBlueShift        BYTE      ?
  cAlphaBits        BYTE      ?
  cAlphaShift       BYTE      ?
  cAccumBits        BYTE      ?
  cAccumRedBits     BYTE      ?
  cAccumGreenBits   BYTE      ?
  cAccumBlueBits    BYTE      ?
  cAccumAlphaBits   BYTE      ?
  cDepthBits        BYTE      ?
  cStencilBits      BYTE      ?
  cAuxBuffers       BYTE      ?
  iLayerType        BYTE      ?
  bReserved         BYTE      ?
  dwLayerMask       DWORD      ?
  dwVisibleMask     DWORD      ?
  dwDamageMask      DWORD      ?
PIXELFORMATDESCRIPTOR ENDS

DRAWTEXTPARAMS STRUCT
  cbSize            DWORD      ?
  iTabiLength       DWORD      ?
  iLeftMargin       DWORD      ?
  iRightMargin      DWORD      ?
  uiiLengthDrawn    DWORD      ?
DRAWTEXTPARAMS ENDS

MENUITEMINFOA STRUCT
  cbSize        DWORD      ?
  fMask         DWORD      ?
  fType         DWORD      ?
  fState        DWORD      ?
  wID           DWORD      ?
  hSubMenu      DWORD      ?
  hbmpChecked   DWORD      ?
  hbmpUnchecked DWORD      ?
  dwItemData    DWORD      ?
  dwTypeData    DWORD      ?
  cch           DWORD      ?
MENUITEMINFOA ENDS

MENUITEMINFO  equ  <MENUITEMINFOA>

SCROLLINFO STRUCT
  cbSize        DWORD      ?
  fMask         DWORD      ?
  nMin          DWORD      ?
  nMax          DWORD      ?
  nPage         DWORD      ?
  nPos          DWORD      ?
  nTrackPos     DWORD      ?
SCROLLINFO ENDS

MSGBOXPARAMSA STRUCT
  cbSize                DWORD      ?
  hwndOwner             DWORD      ?
  hInstance             DWORD      ?
  lpszText              DWORD      ?
  lpszCaption           DWORD      ?
  dwStyle               DWORD      ?
  lpszIcon              DWORD      ?
  dwContextHelpId       DWORD      ?
  lpfnMsgBoxCallback    DWORD      ?
  dwLanguageId          DWORD      ?
MSGBOXPARAMSA ENDS

MSGBOXPARAMS EQU <MSGBOXPARAMSA>






DEBUGSTRUCT UNION
  Exception         EXCEPTION_DEBUG_INFO <{?,?,?,?,?,EXCEPTION_MAXIMUM_PARAMETERS dup(?)},?>
  CreateThread      CREATE_THREAD_DEBUG_INFO <?,?,?>
  CreateProcessInfo CREATE_PROCESS_DEBUG_INFO <?,?,?,?,?,?,?,?,?,?>
  ExitThread        EXIT_THREAD_DEBUG_INFO <?>
  ExitProcess       EXIT_PROCESS_DEBUG_INFO <?>
  LoadDll           LOAD_DLL_DEBUG_INFO <?,?,?,?,?,?>
  UnloadDll         UNLOAD_DLL_DEBUG_INFO <?>
  DebugString       OUTPUT_DEBUG_STRING_INFO <?,?,?>
  RipInfo           RIP_INFO <?,?>
DEBUGSTRUCT ENDS

DEBUG_EVENT STRUCT
  dwDebugEventCode  DWORD       ?
  dwProcessId       DWORD       ?
  dwThreadId        DWORD       ?
  u                 DEBUGSTRUCT <>
DEBUG_EVENT ENDS

IMAGE_DATA_DIRECTORY STRUCT
  VirtualAddress    DWORD      ?
  isize             DWORD      ?
IMAGE_DATA_DIRECTORY ENDS

IMAGE_OPTIONAL_HEADER32 STRUCT
  Magic                         WORD       ?
  MajorLinkerVersion            BYTE       ?
  MinorLinkerVersion            BYTE       ?
  SizeOfCode                    DWORD      ?
  SizeOfInitializedData         DWORD      ?
  SizeOfUninitializedData       DWORD      ?
  AddressOfEntryPoint           DWORD      ?
  BaseOfCode                    DWORD      ?
  BaseOfData                    DWORD      ?
  ImageBase                     DWORD      ?
  SectionAlignment              DWORD      ?
  FileAlignment                 DWORD      ?
  MajorOperatingSystemVersion   WORD       ?
  MinorOperatingSystemVersion   WORD       ?
  MajorImageVersion             WORD       ?
  MinorImageVersion             WORD       ?
  MajorSubsystemVersion         WORD       ?
  MinorSubsystemVersion         WORD       ?
  Win32VersionValue             DWORD      ?
  SizeOfImage                   DWORD      ?
  SizeOfHeaders                 DWORD      ?
  CheckSum                      DWORD      ?
  Subsystem                     WORD       ?
  DllCharacteristics            WORD       ?
  SizeOfStackReserve            DWORD      ?
  SizeOfStackCommit             DWORD      ?
  SizeOfHeapReserve             DWORD      ?
  SizeOfHeapCommit              DWORD      ?
  LoaderFlags                   DWORD      ?
  NumberOfRvaAndSizes           DWORD      ?
  DataDirectory                 IMAGE_DATA_DIRECTORY IMAGE_NUMBEROF_DIRECTORY_ENTRIES dup(<>)
IMAGE_OPTIONAL_HEADER32 ENDS

IMAGE_OPTIONAL_HEADER  equ  <IMAGE_OPTIONAL_HEADER32>

IMAGE_FILE_HEADER STRUCT
  Machine               WORD    ?
  NumberOfSections      WORD    ?
  TimeDateStamp         DWORD   ?
  PointerToSymbolTable  DWORD   ?
  NumberOfSymbols       DWORD   ?
  SizeOfOptionalHeader  WORD    ?
  Characteristics       WORD    ?
IMAGE_FILE_HEADER ENDS

IMAGE_NT_HEADERS STRUCT
  Signature         DWORD                   ?
  FileHeader        IMAGE_FILE_HEADER       <>
  OptionalHeader    IMAGE_OPTIONAL_HEADER32 <>
IMAGE_NT_HEADERS ENDS

IMAGE_EXPORT_DIRECTORY STRUCT
  Characteristics           DWORD      ?
  TimeDateStamp             DWORD      ?
  MajorVersion              WORD       ?
  MinorVersion              WORD       ?
  nName                     DWORD      ?
  nBase                     DWORD      ?
  NumberOfFunctions         DWORD      ?
  NumberOfNames             DWORD      ?
  AddressOfFunctions        DWORD      ?
  AddressOfNames            DWORD      ?
  AddressOfNameOrdinals     DWORD      ?
IMAGE_EXPORT_DIRECTORY ENDS

IMAGE_DOS_HEADER STRUCT
  e_magic           WORD      ?
  e_cblp            WORD      ?
  e_cp              WORD      ?
  e_crlc            WORD      ?
  e_cparhdr         WORD      ?
  e_minalloc        WORD      ?
  e_maxalloc        WORD      ?
  e_ss              WORD      ?
  e_sp              WORD      ?
  e_csum            WORD      ?
  e_ip              WORD      ?
  e_cs              WORD      ?
  e_lfarlc          WORD      ?
  e_ovno            WORD      ?
  e_res             WORD   4 dup(?)
  e_oemid           WORD      ?
  e_oeminfo         WORD      ?
  e_res2            WORD  10 dup(?)
  e_lfanew          DWORD      ?
IMAGE_DOS_HEADER ENDS

; ------------------------ wsock32 structures -----------------------------

fd_set STRUCT
  fd_count  DWORD      ?
  fd_array  SOCKET FD_SETSIZE dup(?)
fd_set ENDS

timeval STRUCT
  tv_sec   DWORD      ?
  tv_usec  DWORD      ?
timeval ENDS

S_UN_B STRUCT
  s_b1 BYTE ?
  s_b2 BYTE ?
  s_b3 BYTE ?
  s_b4 BYTE ?
S_UN_B ENDS

S_UN_W STRUCT 
  s_w1 WORD ?
  s_w2 WORD ?
S_UN_W ENDS

ADDRESS_UNION UNION 
   S_un_b S_UN_B <>
   S_un_w S_UN_W <>
   S_addr DWORD ?
ADDRESS_UNION ENDS

in_addr STRUCT
  S_un ADDRESS_UNION <>
in_addr ENDS

sockaddr_in STRUCT
  sin_family    WORD      ?
  sin_port      WORD      ?
  sin_addr      in_addr <>
  sin_zero      BYTE 8 dup (?)
sockaddr_in ENDS

sockaddr STRUCT
  sa_family     WORD      ?
  sa_data       BYTE 14 dup(?)
sockaddr ENDS

WSADATA STRUCT
  wVersion      WORD      ?
  wHighVersion  WORD      ?
  szDescription BYTE WSADESCRIPTION_LEN + 1 dup (?)
  szSystemStatus  BYTE WSASYS_STATUS_LEN + 1 dup (?)
  iMaxSockets   WORD      ?
  iMaxUdpDg     WORD      ?
  lpVendorInfo  DWORD      ?
WSADATA ENDS

sockproto STRUCT
  sp_family     WORD      ?
  sp_protocol   WORD      ?
sockproto ENDS

linger STRUCT
  l_onoff   WORD      ?
  l_linger  WORD      ?
linger ENDS

hostent STRUCT
  h_name      DWORD      ?
  h_alias     DWORD      ?
  h_addr      WORD       ?
  h_len       WORD       ?
  h_list      DWORD      ?
hostent ENDS

netent STRUCT
  n_name      DWORD      ?
  n_aliases   DWORD      ?
  n_addrtype  WORD       ?
  n_net       DWORD      ?
netent ENDS

servent STRUCT
  s_name     DWORD      ?
  s_aliases  DWORD      ?
  s_port     WORD       ?
  s_proto    DWORD      ?
servent ENDS

icmp_hdr STRUCT
  icmp_type   BYTE      ?
  icmp_code   BYTE      ?
  icmp_cksum  WORD      ?
  icmp_id     WORD      ?
  icmp_seq    WORD      ?
  icmp_data   BYTE      ?
icmp_hdr ENDS

ip_hdr STRUCT
  ip_hlv    BYTE      ?
  ip_tos    BYTE      ?
  ip_len    WORD      ?
  ip_id     WORD      ?
  ip_off    WORD      ?
  ip_ttl    BYTE      ?
  ip_p      BYTE      ?
  ip_cksum  WORD      ?
  ip_src    DWORD      ?
  ip_dest   DWORD      ?
ip_hdr ENDS

ICMP_OPTIONS STRUCT
  Ttl              BYTE      ?
  Tos              BYTE      ?
  Flags            BYTE      ?
  OptionsSize      BYTE      ?
  OptionsData      DWORD      ?
ICMP_OPTIONS ENDS

ICMP_ECHO_REPLY STRUCT
  Address          DWORD      ?
  Status           DWORD      ?
  RoundTripTime    DWORD      ?
  DataSize         WORD      ?
  Reserved         WORD      ?
  DataPointer      DWORD      ?
  Options          DWORD      ?
  zData            BYTE 250 dup (?)
ICMP_ECHO_REPLY ENDS

HEAPLIST32 STRUCT
    dwSize              DWORD ?
    th32ProcessID       DWORD ?
    th32HeapID          DWORD ?
    dwFlags             DWORD ?
HEAPLIST32 ENDS

HEAPENTRY32 STRUCT
    dwSize          DWORD ?
    hHandle         DWORD ?
    dwAddress       DWORD ?
    dwBlockSize     DWORD ?
    dwFlags         DWORD ?
    dwLockCount     DWORD ?
    dwResvd         DWORD ?
    th32ProcessID   DWORD ?
    th32HeapID      DWORD ?
HEAPENTRY32 ENDS

PROCESSENTRY32 STRUCT
    dwSize              DWORD ?
    cntUsage            DWORD ?
    th32ProcessID       DWORD ?
    th32DefaultHeapID   DWORD ?
    th32ModuleID        DWORD ?
    cntThreads          DWORD ?
    th32ParentProcessID DWORD ?
    pcPriClassBase      DWORD ?
    dwFlags             DWORD ?
    szExeFile           db MAX_PATH dup(?)
PROCESSENTRY32 ENDS

THREADENTRY32 STRUCT
    dwSize              DWORD ?
    cntUsage            DWORD ?
    th32ThreadID        DWORD ?
    th32OwnerProcessID  DWORD ?
    tpBasePri           DWORD ?
    tpDeltaPri          DWORD ?
    dwFlags             DWORD ?
THREADENTRY32 ENDS

MODULEENTRY32 STRUCT
    dwSize          DWORD ?
    th32ModuleID    DWORD ?
    th32ProcessID   DWORD ?
    GlblcntUsage    DWORD ?
    ProccntUsage    DWORD ?
    modBaseAddr     DWORD ?
    modBaseSize     DWORD ?
    hModule         DWORD ?
    szModule        db MAX_MODULE_NAME32+1 dup(?)
    szExePath       db MAX_PATH dup(?)
MODULEENTRY32 ENDS

PROPSHEETPAGEA STRUCT
        dwSize              DWORD ?
        dwFlags             DWORD ?
        hInstance           DWORD ?
          union 
            pszTemplate     DWORD ?
            pResource       DWORD ?
          ends
          union 
            hIcon           DWORD ?
            pszIcon         DWORD ?
          ends
        pszTitle            DWORD ?
        pfnDlgProc          DWORD ?        
        lParam              DWORD ?         
        pfnCallback         DWORD ?
        pcRefParent         DWORD ?
        pszHeaderTitle      DWORD ?
        pszHeaderSubTitle   DWORD ?
PROPSHEETPAGEA ENDS

PROPSHEETHEADERA STRUCT
        dwSize              DWORD ?
        dwFlags             DWORD ?          
        hwndParent          DWORD ?           
        hInstance           DWORD ?      
        union 
            hIcon           DWORD ?
            pszIcon         DWORD ?
        ends
        pszCaption          DWORD ? 
        nPages              DWORD ? 
          union 
            nStartPage      DWORD ?
            pStartPage      DWORD ?    
          ends
          union 
            ppsp            DWORD ?
            phpage          DWORD ?
          ends
        pfnCallback         DWORD ?
          union 
            hbmWatermark    DWORD ?
            pszbmWatermark  DWORD ?
          ends
        hplWatermark        DWORD ?
        union 
            hbmHeader       DWORD ?
            pszbmHeader     DWORD ?
        ends

PROPSHEETHEADERA ENDS

PROPSHEETHEADER         equ   <PROPSHEETHEADERA>
LPPROPSHEETHEADER       equ   <LPPROPSHEETHEADERA>

LPCPROPSHEETHEADER      equ   <LPCPROPSHEETHEADERA>
PROPSHEETHEADER_V1_SIZE equ   <PROPSHEETHEADERA_V1_SIZE>

CreatePropertySheetPage equ   <CreatePropertySheetPageA>
PropertySheet           equ   <PropertySheetA>

PSHNOTIFY STRUCT
    hdr         NMHDR <>
    lParam      DWORD ?
PSHNOTIFY ENDS

LIST_ENTRY STRUCT
    Flink DWORD ?
    Blink DWORD ?
LIST_ENTRY ENDS

SINGLE_LIST_ENTRY STRUCT
	Next DWORD ?
SINGLE_LIST_ENTRY ENDS

GUID	STRUCT
    Data1 	dd ?
    Data2 	dw ?
    Data3 	dw ?
    Data4 	db 8 dup(?)
GUID ENDS

OBJECTID STRUCT
    Lineage         GUID <>
    Uniquifier      dd ?
OBJECTID ENDS

LDTBits RECORD  rBaseHi:8, Granularity:1,Default_Big : 1,Reserved_0:1,Sys:1,LimitHi:4,Pres:1, Dpl:2, S_bit:1, Type1:4,rBaseMid:8

LDT_ENTRY STRUCT
    LimitLow	dw ?
    BaseLow		dw ?
    union HighWord1
        Bytes LDT_BYTES <>
        Bits LDTBits <>
    ends
LDT_ENTRY ENDS


SID STRUCT
   Revision db ?
   SubAuthorityCount db ?
   IdentifierAuthority SID_IDENTIFIER_AUTHORITY <>
   SubAuthority	dd ANYSIZE_ARRAY dup(?)
SID ENDS


ACCESS_ALLOWED_OBJECT_ACE STRUCT
    Header ACE_HEADER   <>
    Mask1 dd            ?
    Flags dd            ?
    ObjectType GUID     <>
    InheritedObjectType GUID <>
    SidStart dd         ?
ACCESS_ALLOWED_OBJECT_ACE ENDS

ACCESS_DENIED_OBJECT_ACE STRUCT
    Header ACE_HEADER   <>
    Mask1 dd            ?
    Flags dd            ?
    ObjectType GUID     <>
    InheritedObjectType GUID <>
    SidStart dd         ?
ACCESS_DENIED_OBJECT_ACE ENDS

SYSTEM_AUDIT_OBJECT_ACE STRUCT
    Header ACE_HEADER   <>
    Mask1 dd            ?
    Flags dd            ?
    ObjectType GUID     <>
    InheritedObjectType GUID <>
    SidStart dd         ?
SYSTEM_AUDIT_OBJECT_ACE ENDS

SYSTEM_ALARM_OBJECT_ACE STRUCT
    Header ACE_HEADER <>
    Mask1 dd        ?
    Flags dd        ?
    ObjectType GUID <>
    InheritedObjectType GUID <>
    SidStart dd     ?
SYSTEM_ALARM_OBJECT_ACE ENDS

SECURITY_DESCRIPTOR_RELATIVE STRUCT
    Revision db     ?
    Sbz1 db         ?
    Control1 dw     ?
    Owner dd        ?
    Group1 dd       ?
    Sacl dd         ?
    Dacl dd         ?
SECURITY_DESCRIPTOR_RELATIVE ENDS

OBJECT_TYPE_LIST STRUCT
    Level dw        ?
    Sbz dw          ?
    ObjectType dd   ?
OBJECT_TYPE_LIST ENDS

TOKEN_USER STRUCT
    User SID_AND_ATTRIBUTES <>
TOKEN_USER ENDS

TOKEN_OWNER STRUCT
    Owner dd ?
TOKEN_OWNER ENDS

TOKEN_PRIMARY_GROUP STRUCT
    PrimaryGroup dd ?
TOKEN_PRIMARY_GROUP ENDS

TOKEN_DEFAULT_DACL STRUCT
    DefaultDacl dd ?
TOKEN_DEFAULT_DACL ENDS

TOKEN_SOURCE STRUCT
    SourceName db TOKEN_SOURCE_LENGTH dup(?)
    SourceIdentifier LUID <>
TOKEN_SOURCE ends

TOKEN_CONTROL STRUCT
    TokenId LUID                <>
    AuthenticationId LUID       <>
    ModifiedId LUID             <>
    TokenSource1 TOKEN_SOURCE   <>
TOKEN_CONTROL ENDS

NT_TIB STRUCT
    ExceptionList dd    ?
    StackBase dd        ?
    StackLimit dd       ?
    SubSystemTib dd     ?
    union
        FiberData dd    ?
        Version dd      ?
    ends
    ArbitraryUserPointer dd ?
    Self dd ?
NT_TIB ENDS

FILE_NOTIFY_INFORMATION STRUCT
    NextEntryOffset dd  ?
    Action dd           ?
    FileNameLength dd   ?
    FileName dw         ?
FILE_NOTIFY_INFORMATION ENDS


REPARSE_DATA_BUFFER STRUCT
    ReparseTag dd           ?
    ReparseDataLength dw    ?
    Reserved dw             ?
    union
        struct SymbolicLinkReparseBuffer
            SubstituteNameOffset dw ?
            SubstituteNameLength dw ?
            PrintNameOffset dw      ?
            PrintNameLength dw      ?
            PathBuffer dw           ?
		  ends
        struct MountPointReparseBuffer
            SubstituteNameOffset dw ?
            SubstituteNameLength dw ?
            PrintNameOffset dw      ?
            PrintNameLength dw      ?
            PathBuffer dw           ?
        ends
        struct GenericReparseBuffer
            DataBuffer db ?
        ends
    ends
REPARSE_DATA_BUFFER ENDS

REPARSE_GUID_DATA_BUFFER STRUCT
    ReparseTag dd           ?
    ReparseDataLength dw    ?
    Reserved dw             ?
    ReparseGuid GUID        <>
    struct GenericReparseBuffer
        DataBuffer db ?
    ends
REPARSE_GUID_DATA_BUFFER ENDS


REPARSE_POINT_INFORMATION STRUCT
    ReparseDataLength dw    ?
    UnparsedNameLength dw   ?
REPARSE_POINT_INFORMATION ENDS

POWER_DEVICE_TIMEOUTS STRUCT
    ConservationIdleTime DWORD ?
    PerformanceIdleTime DWORD  ?
POWER_DEVICE_TIMEOUTS ENDS

IMAGE_VXD_HEADER STRUCT
    e32_magic dw        ?
    e32_border db       ?
    e32_worder db       ?
    e32_level dd        ?
    e32_cpu dw          ?
    e32_os dw           ?
    e32_ver dd          ?
    e32_mflags dd       ?
    e32_mpages dd       ?
    e32_startobj dd     ?
    e32_eip dd          ?
    e32_stackobj dd     ?
    e32_esp dd          ?
    e32_pagesize dd     ?
    e32_lastpagesize dd ?
    e32_fixupsize dd    ?
    e32_fixupsum dd     ?
    e32_ldrsize dd      ?
    e32_ldrsum dd       ?
    e32_objtab dd       ?
    e32_objcnt dd       ?
    e32_objmap dd       ?
    e32_itermap dd      ?
    e32_rsrctab dd      ?
    e32_rsrccnt dd      ?
    e32_restab dd       ?
    e32_enttab dd       ?
    e32_dirtab dd       ?
    e32_dircnt dd       ?
    e32_fpagetab dd     ?
    e32_frectab dd      ?
    e32_impmod dd       ?
    e32_impmodcnt dd    ?
    e32_impproc dd      ?
    e32_pagesum dd      ?
    e32_datapage dd     ?
    e32_preload dd      ?
    e32_nrestab dd      ?
    e32_cbnrestab dd    ?
    e32_nressum dd      ?
    e32_autodata dd     ?
    e32_debuginfo dd    ?
    e32_debuglen dd     ?
    e32_instpreload dd  ?
    e32_instdemand dd   ?
    e32_heapsize dd     ?
    e32_res3 db 12    dup(?)
    e32_winresoff dd    ?
    e32_winreslen dd    ?
    e32_devid dw        ?
    e32_ddkver dw       ?
IMAGE_VXD_HEADER ENDS

IMAGE_ROM_OPTIONAL_HEADER STRUCT
    Magic dw                    ?
    MajorLinkerVersion db       ?
    MinorLinkerVersion db       ?
    SizeOfCode dd               ?
    SizeOfInitializedData dd    ?
    SizeOfUninitializedData dd  ?
    AddressOfEntryPoint dd      ?
    BaseOfCode dd               ?
    BaseOfData dd               ?
    BaseOfBss dd                ?
    GprMask dd                  ?
    CprMask dd              4 dup(?)
    GpValue dd                  ?
IMAGE_ROM_OPTIONAL_HEADER ENDS

IMAGE_ROM_HEADERS STRUCT
    FileHeader IMAGE_FILE_HEADER <>
    OptionalHeader IMAGE_ROM_OPTIONAL_HEADER <>
IMAGE_ROM_HEADERS ENDS

IMAGE_SECTION_HEADER STRUCT
    Name1 db IMAGE_SIZEOF_SHORT_NAME dup(?)
    union Misc
        PhysicalAddress dd  ?
        VirtualSize dd      ?
    ends
    VirtualAddress dd       ?
    SizeOfRawData dd        ?
    PointerToRawData dd     ?
    PointerToRelocations dd ?
    PointerToLinenumbers dd ?
    NumberOfRelocations dw  ?
    NumberOfLinenumbers dw  ?
    Characteristics dd      ?
IMAGE_SECTION_HEADER ENDS

IMAGE_SYMBOL STRUCT
    union N
        ShortName db 8 dup(?)
        struct Name1
            Short1 dd       ?
            Long1 dd        ?
        ends
        LongName dd 2 dup(?)
    ends
    Value dd                ?
    SectionNumber dw        ?
    Type1 dw                ?
    StorageClass db         ?
    NumberOfAuxSymbols db   ?
IMAGE_SYMBOL ENDS

IMAGE_AUX_SYMBOL STRUCT
    struct Sym
        TagIndex dd ?
        union Misc
            struct LnSz
                Linenumber dw ?
                Size1 dw ?
			   ends
           TotalSize dd ?
        ends
        union FcnAry
            struct Function
                PointerToLinenumber dd ?
                PointerToNextFunction dd ?
            ends
            struct Array
                Dimension dw 4 dup(?)
            ends
        ends
        TvIndex dw ?
	 ends
    struct File
        Name1 db IMAGE_SIZEOF_SYMBOL dup(?)
	 ends
    struct Section
        Length1 dd              ?
        NumberOfRelocations dw  ?
        NumberOfLinenumbers dw  ?
        CheckSum dd             ?
        Number dw               ?
        Selection db            ?
    ends
IMAGE_AUX_SYMBOL ENDS

IMAGE_RELOCATION STRUCT
    union 
        VirtualAddress dd   ?
        RelocCount dd       ?
    ends
    SymbolTableIndex dd     ?
    Type1 dw ?
IMAGE_RELOCATION ENDS

IMAGE_LINENUMBER STRUCT
    union Type1
        SymbolTableIndex dd ?
        VirtualAddress dd   ?
	 ends
    Linenumber dw ?
IMAGE_LINENUMBER ENDS


IMAGE_BASE_RELOCATION STRUCT
    VirtualAddress dd   ?
    SizeOfBlock dd      ?
IMAGE_BASE_RELOCATION ENDS

IMAGE_ARCHIVE_MEMBER_HEADER STRUCT
    Name1 db        16 dup(?)
    Date db         12 dup(?)
    UserID db       6 dup(?)
    GroupID db      6 dup(?)
    Mode db         8 dup(?)
    Size1 db        10 dup(?)
    EndHeader db    2 dup(?)
IMAGE_ARCHIVE_MEMBER_HEADER ENDS


IMAGE_IMPORT_BY_NAME STRUCT
    Hint dw     ?
    Name1 db    ?
IMAGE_IMPORT_BY_NAME ENDS

IMAGE_THUNK_DATA32 STRUCT
    union u1
        ForwarderString dd  ?
        Function dd         ?
        Ordinal dd          ?
        AddressOfData dd    ?
    ends
IMAGE_THUNK_DATA32 ENDS


IMAGE_TLS_DIRECTORY32 STRUCT
    StartAddressOfRawData dd    ?
    EndAddressOfRawData dd      ?
    AddressOfIndex dd           ?
    AddressOfCallBacks dd       ?
    SizeOfZeroFill dd           ?
    Characteristics dd          ?
IMAGE_TLS_DIRECTORY32 ENDS

IMAGE_THUNK_DATA EQU <IMAGE_THUNK_DATA32>
IMAGE_TLS_DIRECTORY EQU <IMAGE_TLS_DIRECTORY32>

IMAGE_IMPORT_DESCRIPTOR STRUCT
    union
        Characteristics dd      ?
        OriginalFirstThunk dd   ?
	 ends
    TimeDateStamp dd    ?
    ForwarderChain dd   ?
    Name1 dd            ?
    FirstThunk dd       ?
IMAGE_IMPORT_DESCRIPTOR ENDS

IMAGE_BOUND_IMPORT_DESCRIPTOR STRUCT
    TimeDateStamp dd    ?
    OffsetModuleName dw ?
    NumberOfModuleForwarderRefs dw ?
IMAGE_BOUND_IMPORT_DESCRIPTOR ENDS

IMAGE_BOUND_FORWARDER_REF STRUCT
    TimeDateStamp dd    ?
    OffsetModuleName dw ?
    Reserved dw         ?
IMAGE_BOUND_FORWARDER_REF ENDS

IMAGE_RESOURCE_DIRECTORY STRUCT
    Characteristics dd      ?
    TimeDateStamp dd        ?
    MajorVersion dw         ?
    MinorVersion dw         ?
    NumberOfNamedEntries dw ?
    NumberOfIdEntries dw    ?
IMAGE_RESOURCE_DIRECTORY ENDS

IMAGE_RESOURCE_DIRECTORY_ENTRY STRUCT
    union
        rName	RECORD NameIsString:1,NameOffset:31
        Name1 dd ?
        Id dw ?
    ends
    union
        OffsetToData dd ?
		  rDirectory	RECORD DataIsDirectory:1,OffsetToDirectory:31
    ends
IMAGE_RESOURCE_DIRECTORY_ENTRY ENDS

IMAGE_RESOURCE_DIRECTORY_STRING STRUCT
    Length1 dw      ?
    NameString db   ?
IMAGE_RESOURCE_DIRECTORY_STRING ENDS

IMAGE_RESOURCE_DIR_STRING_U STRUCT
    Length1 dw      ?
    NameString dw   ?
IMAGE_RESOURCE_DIR_STRING_U ENDS

IMAGE_RESOURCE_DATA_ENTRY STRUCT
    OffsetToData dd ?
    Size1 dd        ?
    CodePage dd     ?
    Reserved dd     ?
IMAGE_RESOURCE_DATA_ENTRY ENDS

IMAGE_LOAD_CONFIG_DIRECTORY STRUCT
    Characteristics dd                  ?
    TimeDateStamp dd                    ?
    MajorVersion dw                     ?
    MinorVersion dw                     ?
    GlobalFlagsClear dd                 ?
    GlobalFlagsSet dd                   ?
    CriticalSectionDefaultTimeout dd    ?
    DeCommitFreeBlockThreshold dd       ?
    DeCommitTotalFreeThreshold dd       ?
    LockPrefixTable dd                  ?
    MaximumAllocationSize dd            ?
    VirtualMemoryThreshold dd           ?
    ProcessHeapFlags dd                 ?
    ProcessAffinityMask dd              ?
    CSDVersion dw                       ?
    Reserved1 dw                        ?
    EditList dd                         ?
    Reserved dd                         ?
IMAGE_LOAD_CONFIG_DIRECTORY ENDS

IMAGE_DEBUG_DIRECTORY STRUCT
    Characteristics dd  ?
    TimeDateStamp dd    ?
    MajorVersion dw     ?
    MinorVersion dw     ?
    Type1 dd            ?
    SizeOfData dd       ?
    AddressOfRawData dd ?
    PointerToRawData dd ?
IMAGE_DEBUG_DIRECTORY ENDS

IMAGE_COFF_SYMBOLS_HEADER STRUCT
    NumberOfSymbols dd      ?
    LvaToFirstSymbol dd     ?
    NumberOfLinenumbers dd  ?
    LvaToFirstLinenumber dd ?
    RvaToFirstByteOfCode dd ?
    RvaToLastByteOfCode dd  ?
    RvaToFirstByteOfData dd ?
    RvaToLastByteOfData dd  ?
IMAGE_COFF_SYMBOLS_HEADER ENDS

FPOProlog	RECORD cbFrame  : 2, reserved : 1, fUseBP   : 1, fHasSEH  : 1, cbRegs   : 3, cbProlog : 8
FPO_DATA STRUCT
    ulOffStart dd   ?
    cbProcSize dd   ?
    cdwLocals dd    ?
    cdwParams dw    ?
	 rProlog FPOProlog <>
FPO_DATA ENDS

IMAGE_DEBUG_MISC STRUCT
    DataType dd     ?
    Length1 dd      ?
    Unicode BOOLEAN ?
    Reserved db 3 dup(?)
    Data db         ?
IMAGE_DEBUG_MISC ENDS


IMAGE_FUNCTION_ENTRY STRUCT
    StartingAddress dd  ?
    EndingAddress dd    ?
    EndOfPrologue dd    ?
IMAGE_FUNCTION_ENTRY ENDS

IMAGE_SEPARATE_DEBUG_HEADER STRUCT
    Signature dw            ?
    Flags dw                ?
    Machine dw              ?
    Characteristics dw      ?
    TimeDateStamp dd        ?
    CheckSum dd             ?
    ImageBase dd            ?
    SizeOfImage dd          ?
    NumberOfSections dd     ?
    ExportedNamesSize dd    ?
    DebugDirectorySize dd   ?
    SectionAlignment dd     ?
    Reserved dd 2 dup(?)
IMAGE_SEPARATE_DEBUG_HEADER ENDS

ImportRec RECORD Reserved : 11,NameType : 3,Type2 : 2

IMPORT_OBJECT_HEADER STRUCT
    Sig1 dw     ?
    Sig2 dw     ?
    Version dw  ?
    Machine dw  ?
    TimeDateStamp dd ?
    SizeOfData dd   ?
    union 
        Ordinal dw  ?
        Hint dw     ?
    ends
    rImport ImportRec <>
IMPORT_OBJECT_HEADER ENDS

MESSAGE_RESOURCE_ENTRY STRUCT
    Length1 dw  ?
    Flags dw    ?
    Text db     ?
MESSAGE_RESOURCE_ENTRY ENDS

MESSAGE_RESOURCE_BLOCK STRUCT
    LowId dd    ?
    HighId dd   ?
    OffsetToEntries dd ?
MESSAGE_RESOURCE_BLOCK ENDS

MESSAGE_RESOURCE_DATA STRUCT
    NumberOfBlocks dd ?
    Blocks MESSAGE_RESOURCE_BLOCK <>
MESSAGE_RESOURCE_DATA ENDS

RTL_CRITICAL_SECTION_DEBUG STRUCT
    Type1 dw            ?
    CreatorBackTraceIndex dw ?
    CriticalSection dd  ?
    ProcessLocksList LIST_ENTRY <>
    EntryCount dd       ?
    ContentionCount dd  ?
    Spare dd 2 dup(?)
RTL_CRITICAL_SECTION_DEBUG ENDS

EVENTSFORLOGFILE STRUCT
    ulSize dd       ?
    szLogicalLogFile dw MAXLOGICALLOGNAMESIZE dup(?)
    ulNumRecords dd ?
	 pEventLogRecords EVENTLOGRECORD <>
EVENTSFORLOGFILE ENDS

PACKEDEVENTINFO STRUCT
    ulSize dd                   ?
    ulNumEventsForLogFile dd    ?
    ulOffsets dd                ?
PACKEDEVENTINFO ENDS

TAPE_ERASE STRUCT
    Type1 dd ?
    Immediate BOOLEAN ?
TAPE_ERASE ENDS

TAPE_PREPARE STRUCT
    Operation dd        ?
    Immediate BOOLEAN   ?
TAPE_PREPARE ENDS

TAPE_WRITE_MARKS STRUCT
    Type1 dd    ?
    Count dd    ?
    Immediate BOOLEAN ?
TAPE_WRITE_MARKS ENDS

TAPE_GET_POSITION STRUCT
    Type1 dd        ?
    Partition dd    ?
    Offset1 QWORD   ?
TAPE_GET_POSITION ENDS

TAPE_SET_POSITION STRUCT
    Method dd           ?
    Partition dd        ?
    Offset1 QWORD       ?
    Immediate BOOLEAN   ?
TAPE_SET_POSITION ENDS

TAPE_GET_DRIVE_PARAMETERS STRUCT
    ECC BOOLEAN             ?
    Compression BOOLEAN     ?
    DataPadding BOOLEAN     ?
    ReportSetmarks BOOLEAN  ?
    DefaultBlockSize dd     ?
    MaximumBlockSize dd     ?
    MinimumBlockSize dd     ?
    MaximumPartitionCount dd ?
    FeaturesLow dd          ?
    FeaturesHigh dd         ?
    EOTWarningZoneSize dd   ?
TAPE_GET_DRIVE_PARAMETERS ENDS

TAPE_SET_DRIVE_PARAMETERS STRUCT
    ECC BOOLEAN             ?
    Compression BOOLEAN     ?
    DataPadding BOOLEAN     ?
    ReportSetmarks BOOLEAN  ?
    EOTWarningZoneSize dd   ?
TAPE_SET_DRIVE_PARAMETERS ENDS

TAPE_GET_MEDIA_PARAMETERS STRUCT
    Capacity QWORD          ?
    Remaining QWORD         ?
    BlockSize dd            ?
    PartitionCount dd       ?
    WriteProtected BOOLEAN  ?
TAPE_GET_MEDIA_PARAMETERS ENDS

TAPE_SET_MEDIA_PARAMETERS STRUCT
    BlockSize dd ?
TAPE_SET_MEDIA_PARAMETERS ENDS

TAPE_CREATE_PARTITION STRUCT
    Method dd   ?
    Count dd    ?
    Size1 dd    ?
TAPE_CREATE_PARTITION ENDS

LOADED_IMAGE STRUCT
    ModuleName                      DWORD   ?
    hFile                           DWORD   ?
    MappedAddress                   DWORD   ?
    FileHeader                      DWORD	  ?
    LastRvaSection                  DWORD	  ?
    NumberOfSections                DWORD   ?
    Sections                        DWORD	  ?
    Characteristics                 DWORD   ?
    fSystemImage                    BYTE    ?
    align dword
    fDOSImage                       BYTE    ?
    Links                           DWORD	  ?
    SizeOfImage                     DWORD   ?
LOADED_IMAGE ENDS

IMAGE_DEBUG_INFORMATION STRUCT
    List                            LIST_ENTRY <>
    Size1                           DWORD    ?
    MappedBase                      DWORD    ?
    Machine                         WORD     ?
    Characteristics                 WORD     ?
    CheckSum                        DWORD    ?
    ImageBase                       DWORD    ?
    SizeOfImage                     DWORD    ?
    NumberOfSections                DWORD    ?
    Sections                        DWORD	   ?
    ExportedNamesSize               DWORD    ?
    ExportedNames                   DWORD    ?
    NumberOfFunctionTableEntries    DWORD    ?
    FunctionTableEntries            DWORD    ?
    LowestFunctionStartingAddress   DWORD    ?
    HighestFunctionEndingAddress    DWORD    ?
    NumberOfFpoTableEntries         DWORD    ?
    FpoTableEntries                 DWORD	   ?
    SizeOfCoffSymbols               DWORD    ?
    CoffSymbols                     DWORD	   ?
    SizeOfCodeViewSymbols           DWORD    ?
    CodeViewSymbols                 DWORD    ?
    ImageFilePath                   DWORD    ?
    ImageFileName                   DWORD    ?
    DebugFilePath                   DWORD    ?
    TimeDateStamp                   DWORD    ?
    RomImage                        BYTE     ?
    DebugDirectory                  DWORD	   ?
    NumberOfDebugDirectories        DWORD    ?
    Reserved                        DWORD  3 dup (?)
IMAGE_DEBUG_INFORMATION ENDS

ADDRESS STRUCT
    Offset1  DWORD      ?
    Segment1 WORD       ?
    Mode     DWORD	?
ADDRESS ENDS

KDHELP STRUCT
    Thread                      DWORD    ?
    ThCallbackStack             DWORD    ?
    NextCallback                DWORD    ?
    FramePointer                DWORD    ?
    KiCallUserMode              DWORD    ?
    KeUserCallbackDispatcher    DWORD    ?
    SystemRangeStart            DWORD    ?
KDHELP ENDS

STACKFRAME STRUCT
    AddrPC          ADDRESS	<>
    AddrReturn      ADDRESS	<>
    AddrFrame       ADDRESS	<>
    AddrStack       ADDRESS	<>
    FuncTableEntry  DWORD ?
    Params          DWORD 4 dup (?)
    Far1            BOOL	?
    Virtual         BOOL	?
    Reserved        DWORD 3 dup (?)
    KdHelp          KDHELP	<>
STACKFRAME ENDS

API_VERSION STRUCT
    MajorVersion    WORD     ?
    MinorVersion    WORD     ?
    Revision        WORD     ?
    Reserved        WORD     ?
API_VERSION ENDS

IMAGEHLP_SYMBOL STRUCT
    SizeOfStruct    DWORD   ?
    Address         DWORD   ?
    Size1           DWORD   ?
    Flags           DWORD   ?
    MaxNameLength   DWORD   ?
    Name1           db      ?
IMAGEHLP_SYMBOL ENDS

IMAGEHLP_MODULE STRUCT
    SizeOfStruct    DWORD   ?
    BaseOfImage     DWORD   ?
    ImageSize       DWORD   ?
    TimeDateStamp   DWORD   ?
    CheckSum        DWORD   ?
    NumSyms         DWORD   ?
    SymType1        DWORD   ?
    ModuleName      BYTE  32 dup (?)
    ImageName       BYTE 256 dup (?)
    LoadedImageName BYTE 256 dup (?)
IMAGEHLP_MODULE ENDS

IMAGEHLP_LINE STRUCT
    SizeOfStruct    DWORD    ?
    Key             DWORD    ?
    LineNumber      DWORD    ?
    FileName        DWORD    ?
    Address         DWORD    ?
IMAGEHLP_LINE ENDS

IMAGEHLP_DEFERRED_SYMBOL_LOAD STRUCT
    SizeOfStruct    DWORD    ?
    BaseOfImage     DWORD    ?
    CheckSum        DWORD    ?
    TimeDateStamp   DWORD    ?
    FileName        BYTE MAX_PATH dup (?)
    Reparse         BYTE     ?
IMAGEHLP_DEFERRED_SYMBOL_LOAD ENDS

IMAGEHLP_DUPLICATE_SYMBOL STRUCT
    SizeOfStruct    DWORD   ?
    NumberOfDups    DWORD   ?
    Symbol          DWORD   ?
    SelectedSymbol  DWORD   ?
IMAGEHLP_DUPLICATE_SYMBOL ENDS

CWPRETSTRUCT STRUCT
    lResult DWORD   ?
    lParam DWORD    ?
    wParam DWORD    ?
    message DWORD   ?
    hwnd DWORD      ?
CWPRETSTRUCT ENDS

HARDWAREHOOKSTRUCT STRUCT
    hwnd DWORD      ?
    message DWORD   ?
    wParam DWORD    ?
    lParam DWORD    ?
HARDWAREHOOKSTRUCT ENDS

USEROBJECTFLAGS STRUCT
    fInherit DWORD  ?
    fReserved DWORD ?
    dwFlags DWORD   ?
USEROBJECTFLAGS ENDS

MDINEXTMENU STRUCT
    hmenuIn DWORD   ?
    hmenuNext DWORD ?
    hwndNext DWORD  ?
MDINEXTMENU ENDS

NCCALCSIZE_PARAMS STRUCT
    rgrc RECT 3 dup(<>)
    lppos DWORD     ?
NCCALCSIZE_PARAMS ENDS

STYLESTRUCT STRUCT
    styleOld DWORD ?
    styleNew DWORD ?
STYLESTRUCT ENDS

DROPSTRUCT STRUCT
    hwndSource DWORD    ?
    hwndSink DWORD      ?
    wFmt DWORD          ?
    dwData DWORD        ?
    ptDrop POINT        <>
    dwControlData DWORD ?
DROPSTRUCT ENDS

CURSORSHAPE STRUCT
    xHotSpot DWORD  ?
    yHotSpot DWORD  ?
    regCx DWORD     ?
    regCy DWORD     ?
    cbWidth DWORD   ?
    Planes db       ?
    BitsPixel db    ?
CURSORSHAPE ENDS

SERIALKEYSA STRUCT
    cbSize DWORD            ?
    dwFlags DWORD           ?
    lpszActivePort DWORD    ?
    lpszPort DWORD          ?
    iBaudRate DWORD         ?
    iPortState DWORD        ?
    iActive DWORD           ?
SERIALKEYSA ENDS

SERIALKEYS EQU <SERIALKEYSA>

HIGHCONTRASTA STRUCT
    cbSize DWORD            ?
    dwFlags DWORD           ?
    lpszDefaultScheme DWORD ?
HIGHCONTRASTA ENDS

HIGHCONTRAST EQU <HIGHCONTRASTA>

FILTERKEYS STRUCT
    cbSize DWORD        ?
    dwFlags DWORD       ?
    iWaitMSec DWORD     ?
    iDelayMSec DWORD    ?
    iRepeatMSec DWORD   ?
    iBounceMSec DWORD   ?
FILTERKEYS ENDS

STICKYKEYS STRUCT
    cbSize DWORD    ?
    dwFlags DWORD   ?
STICKYKEYS ENDS

MOUSEKEYS STRUCT
    cbSize DWORD            ?
    dwFlags DWORD           ?
    iMaxSpeed DWORD         ?
    iTimeToMaxSpeed DWORD   ?
    iCtrlSpeed DWORD        ?
    dwReserved1 DWORD       ?
    dwReserved2 DWORD       ?
MOUSEKEYS ENDS

ACCESSTIMEOUT STRUCT
    cbSize DWORD        ?
    dwFlags DWORD       ?
    iTimeOutMSec DWORD  ?
ACCESSTIMEOUT ENDS

SOUNDSENTRYA STRUCT
    cbSize DWORD                    ?
    dwFlags DWORD                   ?
    iFSTextEffect DWORD             ?
    iFSTextEffectMSec DWORD         ?
    iFSTextEffectColorBits DWORD    ?
    iFSGrafEffect DWORD             ?
    iFSGrafEffectMSec DWORD         ?
    iFSGrafEffectColor DWORD        ?
    iWindowsEffect DWORD            ?
    iWindowsEffectMSec DWORD        ?
    lpszWindowsEffectDLL DWORD      ?
    iWindowsEffectOrdinal DWORD     ?
SOUNDSENTRYA ENDS

SOUNDSENTRY EQU <SOUNDSENTRYA>

TOGGLEKEYS STRUCT
    cbSize DWORD ?
    dwFlags DWORD ?
TOGGLEKEYS ENDS


RemHGLOBAL STRUCT
    fNullHGlobal DWORD  ?
    cbData DWORD        ?
    data db             ?
RemHGLOBAL ENDS

RemHMETAFILEPICT STRUCT
    imm DWORD       ?
    xExt DWORD      ?
    yExt DWORD      ?
    cbData DWORD    ?
    data db         ?
RemHMETAFILEPICT ENDS

RemHENHMETAFILE STRUCT
    cbData DWORD    ?
    data db         ?
RemHENHMETAFILE ENDS

RemHBITMAP STRUCT
    cbData DWORD    ?
    data db         ?
RemHBITMAP ENDS

RemHPALETTE STRUCT
    cbData DWORD    ?
    data db         ?
RemHPALETTE ENDS

RemHBRUSH STRUCT
    cbData DWORD    ?
    data db         ?
RemHBRUSH ENDS

COAUTHIDENTITY STRUCT
    User DWORD              ?
    UserLength DWORD        ?
    Domain DWORD            ?
    DomainLength DWORD      ?
    Password DWORD          ?
    PasswordLength DWORD    ?
    Flags DWORD             ?
COAUTHIDENTITY ENDS

COAUTHINFO STRUCT
    dwAuthnSvc DWORD            ?
    dwAuthzSvc DWORD            ?
    pwszServerPrincName DWORD   ?
    dwAuthnLevel DWORD          ?
    dwImpersonationLevel DWORD  ?
    pAuthIdentityData DWORD     ?
    dwCapabilities DWORD        ?
COAUTHINFO ENDS

BYTE_BLOB STRUCT
    clSize DWORD    ?
    abData db       ?
BYTE_BLOB ENDS

WORD_BLOB STRUCT
    clSize DWORD    ?
    asData dw       ?
WORD_BLOB ENDS

DWORD_BLOB STRUCT
    clSize DWORD    ?
    alData DWORD    ?
DWORD_BLOB ENDS

FLAGGED_BYTE_BLOB STRUCT
    fFlags DWORD    ?
    clSize DWORD    ?
    abData db       ?
FLAGGED_BYTE_BLOB ENDS

FLAGGED_WORD_BLOB STRUCT
    fFlags DWORD ?
    clSize DWORD ?
    asData dw ?
FLAGGED_WORD_BLOB ENDS

BYTE_SIZEDARR STRUCT
    clSize DWORD    ?
    pData DWORD     ?
BYTE_SIZEDARR ENDS

WORD_SIZEDARR STRUCT
    clSize DWORD    ?
    pData DWORD     ?
WORD_SIZEDARR ENDS

DWORD_SIZEDARR STRUCT
    clSize DWORD        ?
    pData DWORD         ?
DWORD_SIZEDARR ENDS

HYPER_SIZEDARR STRUCT
    clSize DWORD        ?
    pData DWORD         ?
HYPER_SIZEDARR ENDS

userCLIPFORMAT STRUCT
    fContext DWORD      ?
    union u
        dwValue DWORD   ?
        pwszName DWORD  ?
    ends
userCLIPFORMAT ENDS

GDI_NONREMOTE STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
    ends
GDI_NONREMOTE ENDS

userHGLOBAL STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
        hGlobal DWORD   ?
    ends
userHGLOBAL ENDS

userHMETAFILE STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
        hGlobal DWORD   ?
    ends
userHMETAFILE ENDS

remoteMETAFILEPICT STRUCT
    imm DWORD   ?
    xExt DWORD  ?
    yExt DWORD  ?
    hMF DWORD   ?
remoteMETAFILEPICT ENDS

userHMETAFILEPICT STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
        hGlobal DWORD   ?
    ends
userHMETAFILEPICT ENDS

userHENHMETAFILE STRUCT
    fContext DWORD      ?
    union u
        hInproc DWORD   ?
        hRemote DWORD   ?
        hGlobal DWORD   ?
    ends
userHENHMETAFILE ENDS

userBITMAP STRUCT
    bmType DWORD        ?
    bmWidth DWORD       ?
    bmHeight DWORD      ?
    bmWidthBytes DWORD  ?
    bmPlanes dw         ?
    bmBitsPixel dw      ?
    cbSize DWORD        ?
    pBuffer db          ?
userBITMAP ENDS

userHBITMAP STRUCT
    fContext DWORD ?
    union u
        hInproc dd ?
        hRemote dd ?
        hGlobal dd ?
    ends
userHBITMAP ENDS

rpcLOGPALETTE STRUCT
    palVersion dw       ?
    palNumEntries dw    ?
    palPalEntry PALETTEENTRY <>
rpcLOGPALETTE ENDS

userHPALETTE STRUCT
    fContext dd ?
    union u
        hInproc dd ?
        hRemote dd ?
        hGlobal dd ?
    ends
userHPALETTE ENDS

RemotableHandle STRUCT
    fContext dd ?
    union u
        hInproc dd ?
        hRemote dd ?
    ends
RemotableHandle ENDS

BSTRBLOB STRUCT
    cbSize dd   ?
    pData dd    ?
BSTRBLOB ENDS

BLOB STRUCT
    cbSize dd       ?
    pBlobData dd    ?
BLOB ENDS

CLIPDATA STRUCT
    cbSize dd       ?
    ulClipFmt dd    ?
    pClipData dd    ?
CLIPDATA ENDS

CSPLATFORM STRUCT
    dwPlatformId dd     ?
    dwVersionHi dd      ?
    dwVersionLo dd      ?
    dwProcessorArch dd  ?
CSPLATFORM ENDS

QUERYCONTEXT STRUCT
    dwContext dd    ?
    Platform CSPLATFORM <>
    Locale dd       ?
    dwVersionHi dd  ?
    dwVersionLo dd  ?
QUERYCONTEXT ENDS

uCLSSPEC STRUCT
    tyspec dd ?
    union tagged_union
        clsid CLSID         <>
        iid IID             <>
        typelibID GUID      <>
        pFileExt dd         ?
        pMimeType dd        ?
        pProgId dd          ?
        pFileName dd        ?
        pJavaClassName dd   ?
        pPackageName dd     ?
    ends
uCLSSPEC ENDS

PUBLISHEDAPPINFO STRUCT
    pwszFileExtension dd    ?
    pwszDisplayName dd      ?
    pwszPackagePath dd      ?
PUBLISHEDAPPINFO ENDS

APPDETAIL STRUCT
    AppID GUID          <>
    cClasses dd         ?
    prgClsIdList dd     ?
    cTypeLibIds dd      ?
    prgTypeLibIdList dd ?
    cServers dd         ?
    prgServerNames dd   ?
APPDETAIL ENDS

MIXERLINEA STRUCT            DWORD
    cbStruct                DWORD       ?
    dwDestination           DWORD       ?
    dwSource                DWORD       ?
    dwLineID                DWORD       ?
    fdwLine                 DWORD       ?
    dwUser                  DWORD       ?
    dwComponentType         DWORD       ?
    cChannels               DWORD       ?
    cConnections            DWORD       ?
    cControls               DWORD       ?
    szShortName             db    MIXER_SHORT_NAME_CHARS dup(?)
    szName                  db    MIXER_LONG_NAME_CHARS dup(?)
    struct Target
	dwType dd ?
	dwDeviceID dd ?
	wMid dw ?
	wPid dw ?
	vDriverVersion MMVERSION ?
	szPname db MAXPNAMELEN dup(?)
    ends
MIXERLINEA ENDS

MIXERLINE EQU <MIXERLINEA>

MIXERCONTROLA STRUCT         DWORD
     cbStruct               DWORD       ?
     dwControlID            DWORD       ?
     dwControlType          DWORD       ?
     fdwControl             DWORD       ?
     cMultipleItems         DWORD       ?
     szShortName            db    MIXER_SHORT_NAME_CHARS dup(?)
     szName                 db    MIXER_LONG_NAME_CHARS dup(?)
    union Bounds
	struct
		lMinimum dd ?
		lMaximum dd ?
	ends
	struct
		dwMinimum dd ?
		dwMaximum dd ?
	ends
	dwReserved dd 6 dup(?)
    ends
    union Metrics
	cSteps dd ?
	cbCustomData dd ?
	dwReserved dd 6 dup(?)
    ends
MIXERCONTROLA ENDS

MIXERCONTROL EQU <MIXERCONTROLA>

WAVEFORMATEX STRUCT
	wFormatTag dw ?
	nChannels dw ?
	nSamplesPerSec dd ?
	nAvgBytesPerSec dd ?
	nBlockAlign dw ?
	wBitsPerSample dw ?
	cbSize dw ?
WAVEFORMATEX ENDS

MIXERCONTROLDETAILS_LISTTEXTA STRUCT
	dwParam1 dd ?
	dwParam2 dd ?
	szName db MIXER_LONG_NAME_CHARS dup(?)
MIXERCONTROLDETAILS_LISTTEXTA ENDS

MIXERCONTROLDETAILS_LISTTEXT EQU <MIXERCONTROLDETAILS_LISTTEXTA>

RASCONNA STRUCT
    dwSize dd ?
    hrasconn dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASCONNA ENDS 

RASCONN EQU <RASCONNA>

RASCONNSTATUSA STRUCT
    dwSize dd ?
    rasconnstate dd ?
    dwError dd ?
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASCONNSTATUSA ENDS

RASCONNSTATUS EQU <RASCONNSTATUSA>

RASDIALPARAMSA STRUCT
    dwSize dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
    szPhoneNumber db RAS_MaxPhoneNumber + 1 dup(?)
    szCallbackNumber db RAS_MaxCallbackNumber + 1 dup(?)
    szUserName db UNLEN + 1 dup(?)
    szPassword db PWLEN + 1 dup(?)
    szDomain db DNLEN + 1 dup(?)
RASDIALPARAMSA ENDS

RASDIALPARAMS EQU <RASDIALPARAMSA>

RASDIALEXTENSIONS STRUCT
    dwSize dd ?
    dwfOptions dd ?
    hwndParent dd ?
    reserved dd ?
RASDIALEXTENSIONS ENDS

RASENTRYNAMEA STRUCT
    dwSize dd ?
    szEntryName db RAS_MaxEntryName + 1 dup(?)
RASENTRYNAMEA ENDS

RASENTRYNAME EQU <RASENTRYNAMEA>

RASAMBA STRUCT
    dwSize dd ?
    dwError dd ?
    szNetBiosError db NETBIOS_NAME_LEN + 1 dup(?)
    bLana db ?
RASAMBA ENDS

RASAMB EQU <RASAMBA>

RASPPPNBFA STRUCT
    dwSize dd ?
    dwError dd ?
    dwNetBiosError dd ?
    szNetBiosError db NETBIOS_NAME_LEN + 1 dup(?)
    szWorkstationName db NETBIOS_NAME_LEN + 1 dup(?)
    bLana db ?
RASPPPNBFA ENDS

RASPPPNBF EQU <RASPPPNBFA>

RASPPPIPXA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpxAddress db RAS_MaxIpxAddress + 1 dup (?)
RASPPPIPXA ENDS

RASPPPIPX EQU <RASPPPIPXA>

RASPPPIPA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpAddress db RAS_MaxIpAddress + 1 dup(?)
    szServerIpAddress db RAS_MaxIpAddress + 1 dup(?)
RASPPPIPA ENDS

RASPPPIP EQU <RASPPPIPA>

RASPPPLCP STRUCT
    dwSize dd ?
    fBundled BOOL ?
RASPPPLCP ENDS

RASSLIPA STRUCT
    dwSize dd ?
    dwError dd ?
    szIpAddress db RAS_MaxIpAddress + 1 dup(?)
RASSLIPA ENDS

RASSLIP EQU <RASSLIPA>

RASDEVINFOA STRUCT
    dwSize dd ?
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
RASDEVINFOA ENDS

RASDEVINFO EQU <RASDEVINFOA>

RASCTRYINFO STRUCT
    dwSize dd ?
    dwCountryID dd ?
    dwNextCountryID dd ?
    dwCountryCode dd ?
    dwCountryNameOffset dd ?
RASCTRYINFO ENDS

RASIPADDR STRUCT
    _a db ?
    _b db ?
    _c db ?
    _d db ?
RASIPADDR ENDS

RASENTRYA STRUCT
    dwSize dd ?
    dwfOptions dd ?
    dwCountryID dd ?
    dwCountryCode dd ?
    szAreaCode db RAS_MaxAreaCode + 1 dup(?)
    szLocalPhoneNumber db RAS_MaxPhoneNumber + 1 dup(?)
    dwAlternateOffset dd ?
    ipaddr RASIPADDR <>
    ipaddrDns RASIPADDR <>
    ipaddrDnsAlt RASIPADDR <>
    ipaddrWins RASIPADDR <>
    ipaddrWinsAlt RASIPADDR <>
    dwFrameSize dd ?
    dwfNetProtocols dd ?
    dwFramingProtocol dd ?
    szScript db MAX_PATH dup(?)
    szAutodialDll db MAX_PATH dup(?)
    szAutodialFunc db MAX_PATH dup(?)
    szDeviceType db RAS_MaxDeviceType + 1 dup(?)
    szDeviceName db RAS_MaxDeviceName + 1 dup(?)
    szX25PadType db RAS_MaxPadType + 1 dup(?)
    szX25Address db RAS_MaxX25Address + 1 dup(?)
    szX25Facilities db RAS_MaxFacilities + 1 dup(?)
    szX25UserData db RAS_MaxUserData + 1 dup(?)
    dwChannels dd ?
    dwReserved1 dd ?
    dwReserved2 dd ?
RASENTRYA ENDS

RASENTRY EQU <RASENTRYA>

RASPBDLGA STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    dwCallbackId dd ?
    pCallback dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASPBDLGA ENDS

RASPBDLG EQU <RASPBDLGA>

RASENTRYDLGA STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    szEntry db RAS_MaxEntryName + 1 dup(?)
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASENTRYDLGA ENDS

RASENTRYDLG EQU <RASENTRYDLGA>

RASDIALDLG STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    xDlg dd ?
    yDlg dd ?
    dwSubEntry dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASDIALDLG ENDS

RASMONITORDLG STRUCT
    dwSize dd ?
    hwndOwner dd ?
    dwFlags dd ?
    dwStartPage dd ?
    xDlg dd ?
    yDlg dd ?
    dwError dd ?
    reserved dd ?
    reserved2 dd ?
RASMONITORDLG ENDS

RASNOUSERA STRUCT
    dwSize dd ?
    dwFlags dd ?
    dwTimeoutMs dd ?
    szUserName db UNLEN + 1 dup(?)
    szPassword db PWLEN + 1 dup(?)
    szDomain db DNLEN + 1 dup(?)
RASNOUSERA ENDS

RASNOUSER EQU <RASNOUSERA>

RAS_PARAMS_VALUE UNION DWORD
	Number dd ?
	struct String
		iLength dd ?
		Data dd ?
	ends
RAS_PARAMS_VALUE ENDS

RAS_PARAMETERS STRUCT
    P_Key	db RASSAPI_MAX_PARAM_KEY_SIZE dup(?)
    P_Type dd ?
    P_Attributes db ?
    P_Value RAS_PARAMS_VALUE <>
RAS_PARAMETERS ENDS

RAS_USER_0 STRUCT
    bfPrivilege db ?
    szPhoneNumber dw RASSAPI_MAX_PHONENUMBER_SIZE + 1 dup(?)
RAS_USER_0 ENDS

RAS_PORT_0 STRUCT
    wszPortName dw RASSAPI_MAX_PORT_NAME dup(?)
    wszDeviceType dw RASSAPI_MAX_DEVICETYPE_NAME dup(?)
    wszDeviceName dw RASSAPI_MAX_DEVICE_NAME dup(?)
    wszMediaName dw RASSAPI_MAX_MEDIA_NAME dup(?)
    reserved dd ?
    Flags dd ?
    wszUserName dw UNLEN + 1 dup(?)
    wszComputer dw NETBIOS_NAME_LEN dup(?)
    dwStartSessionTime dd ?
    wszLogonDomain dw DNLEN + 1 dup(?)
    fAdvancedServer BOOL ?
RAS_PORT_0 ENDS

RAS_PPP_NBFCP_RESULT STRUCT
    dwError dd ?
    dwNetBiosError dd ?
    szName db NETBIOS_NAME_LEN + 1 dup(?)
    wszWksta dw NETBIOS_NAME_LEN + 1 dup(?)
RAS_PPP_NBFCP_RESULT ENDS

RAS_PPP_IPCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_IPADDRESSLEN + 1 dup(?)
RAS_PPP_IPCP_RESULT ENDS

RAS_PPP_IPXCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_IPXADDRESSLEN + 1 dup(?)
RAS_PPP_IPXCP_RESULT ENDS

RAS_PPP_ATCP_RESULT STRUCT
    dwError dd ?
    wszAddress dw RAS_ATADDRESSLEN + 1 dup(?)
RAS_PPP_ATCP_RESULT ENDS

RAS_PPP_PROJECTION_RESULT STRUCT
    nbf RAS_PPP_NBFCP_RESULT <>
    ip RAS_PPP_IPCP_RESULT <>
    ipx RAS_PPP_IPXCP_RESULT <>
    at RAS_PPP_ATCP_RESULT <>
RAS_PPP_PROJECTION_RESULT ENDS

RAS_PORT_1 STRUCT
    rasport0 RAS_PORT_0 <>
    LineCondition dd ?
    HardwareCondition dd ?
    LineSpeed dd ?
    NumStatistics dw ?
    NumMediaParms dw ?
    SizeMediaParms dd ?
    ProjResult RAS_PPP_PROJECTION_RESULT <>
RAS_PORT_1 ENDS

RAS_PORT_STATISTICS STRUCT
    dwBytesXmited dd ?
    dwBytesRcved dd ?
    dwFramesXmited dd ?
    dwFramesRcved dd ?
    dwCrcErr dd ?
    dwTimeoutErr dd ?
    dwAlignmentErr dd ?
    dwHardwareOverrunErr dd ?
    dwFramingErr dd ?
    dwBufferOverrunErr dd ?
    dwBytesXmitedUncompressed dd ?
    dwBytesRcvedUncompressed dd ?
    dwBytesXmitedCompressed dd ?
    dwBytesRcvedCompressed dd ?
    dwPortBytesXmited dd ?
    dwPortBytesRcved dd ?
    dwPortFramesXmited dd ?
    dwPortFramesRcved dd ?
    dwPortCrcErr dd ?
    dwPortTimeoutErr dd ?
    dwPortAlignmentErr dd ?
    dwPortHardwareOverrunErr dd ?
    dwPortFramingErr dd ?
    dwPortBufferOverrunErr dd ?
    dwPortBytesXmitedUncompressed dd ?
    dwPortBytesRcvedUncompressed dd ?
    dwPortBytesXmitedCompressed dd ?
    dwPortBytesRcvedCompressed dd ?
RAS_PORT_STATISTICS ENDS

RAS_SERVER_0 STRUCT
    TotalPorts dw ?
    PortsInUse dw ?
    RasVersion dd ?
RAS_SERVER_0 ENDS


CPLINFO STRUCT
    idIcon dd ?
    idName dd ?
    idInfo dd ?
    lData dd ?
CPLINFO ENDS

NEWCPLINFOA STRUCT
    dwSize dd ?
    dwFlags dd ?
    dwHelpContext dd ?
    lData dd ?
    hIcon dd ?
    szName db 32 dup(?)
    szInfo db 64 dup(?)
    szHelpFile db 128 dup(?)
NEWCPLINFOA ENDS

NEWCPLINFO EQU <NEWCPLINFOA>


MapiFileDesc STRUCT
    ulReserved dd ?
    flFlags dd ?
    nPosition dd ?
    lpszPathName dd ?
    lpszFileName dd ?
    lpFileType dd ?
MapiFileDesc ENDS

MapiFileTagExt STRUCT
    ulReserved dd ?
    cbTag dd ?
    lpTag dd ?
    cbEncoding dd ?
    lpEncoding dd ?
MapiFileTagExt ENDS

MapiRecipDesc STRUCT
    ulReserved dd ?
    ulRecipClass dd ?
    lpszName dd ?
    lpszAddress dd ?
    ulEIDSize dd ?
    lpEntryID dd ?
MapiRecipDesc ENDS

MapiMessage STRUCT
    ulReserved dd ?
    lpszSubject dd ?
    lpszNoteText dd ?
    lpszMessageType dd ?
    lpszDateReceived dd ?
    lpszConversationID dd ?
    flFlags dd ?
    lpOriginator dd ?
    nRecipCount dd ?
    lpRecips dd ?
    nFileCount dd ?
    lpFiles dd ?
MapiMessage ENDS


CY STRUCT
    struct 
        Lo dd ?
        Hi dd ?
    ENDS
    int64 dq ?
CY ENDS

CURRENCY TEXTEQU <CY>

ENTRYID STRUCT
    abFlags db 4 dup(?)
    ab db MAPI_DIM dup(?)
ENTRYID ENDS

MAPIUID STRUCT
    ab db 16 dup(?)
MAPIUID ENDS

SPropTagArray STRUCT
    cValues dd ?
    aulPropTag dd MAPI_DIM dup(?)
SPropTagArray ENDS

SBinary STRUCT
    cb dd ?
    lpb dd ?
SBinary ENDS

SShortArray STRUCT
    cValues dd ?
    lpi dd ?
SShortArray ENDS

SGuidArray STRUCT
    cValues dd ?
    lpguid dd ?
SGuidArray ENDS

SRealArray STRUCT
    cValues dd ?
    lpflt dd ?
SRealArray ENDS

SLongArray STRUCT
    cValues dd ?
    lpl dd ?
SLongArray ENDS

SLargeIntegerArray STRUCT
    cValues dd ?
    lpli dd ?
SLargeIntegerArray ENDS

SDateTimeArray STRUCT
    cValues dd ?
	 lpft dd ?
SDateTimeArray ENDS

SAppTimeArray STRUCT
    cValues dd ?
    lpat dd ?
SAppTimeArray ENDS

SCurrencyArray STRUCT
    cValues dd ?
    lpcur dd ?
SCurrencyArray ENDS

SBinaryArray STRUCT
    cValues dd ?
    lpbin dd ?
SBinaryArray ENDS

SDoubleArray STRUCT
    cValues dd ?
    lpdbl dd ?
SDoubleArray ENDS

SWStringArray STRUCT
    cValues dd ?
    lppszW dd ?
SWStringArray ENDS

SLPSTRArray STRUCT
    cValues dd ?
    lppszA dd ?
SLPSTRArray ENDS

UPV UNION DWORD
    i dw ?
    l dd ?
    ul dd ?
    flt dd ?
    dbl dq ?
    b dw ?
    cur CURRENCY <>
    at dq ?
    ft FILETIME <>
    lpszA dd ?
    bin SBinary <>
    lpszW dd ?
    lpguid dd ?
    li LARGE_INTEGER <>
    MVi SShortArray <>
    MVl SLongArray <>
    MVflt SRealArray <>
    MVdbl SDoubleArray <>
    MVcur SCurrencyArray <>
    MVat SAppTimeArray <>
    MVft SDateTimeArray <>
    MVbin SBinaryArray <>
    MVszA SLPSTRArray <>
    MVszW SWStringArray <>
    MVguid SGuidArray <>
    MVli SLargeIntegerArray <>
    err SCODE ?
    x LONG ?
UPV ENDS

SPropValue STRUCT
    ulPropTag dd ?
    dwAlignPad dd ?
    Value UPV <>
SPropValue ENDS

SPropProblem STRUCT
    ulIndex dd ?
    ulPropTag dd ?
    scode SCODE ?
SPropProblem ENDS

SPropProblemArray STRUCT
    cProblem dd ?
    aProblem SPropProblem MAPI_DIM dup(<>)
SPropProblemArray ENDS

FLATENTRY STRUCT
    cb dd ?
    abEntry db MAPI_DIM dup(?)
FLATENTRY ENDS

FLATENTRYLIST STRUCT
    cEntries dd ?
    cbEntries dd ?
    abEntries db MAPI_DIM dup(?)
FLATENTRYLIST ENDS

MTSID STRUCT
    cb dd ?
    ab db MAPI_DIM dup(?)
MTSID ENDS

FLATMTSIDLIST STRUCT
    cMTSIDs dd ?
    cbMTSIDs dd ?
    abMTSIDs db MAPI_DIM dup(?)
FLATMTSIDLIST ENDS

ADRENTRY STRUCT
    ulReserved1 dd ?
    cValues dd ?
    rgPropVals dd ?
ADRENTRY ENDS

ADRLIST STRUCT
    cEntries dd ?
    aEntries ADRENTRY MAPI_DIM dup(<>)
ADRLIST ENDS

SRow STRUCT
    ulAdrEntryPad dd ?
    cValues dd ?
    lpProps dd ?
SRow ENDS

SRowSet STRUCT
    cRows dd ?
    aRow SRow MAPI_DIM dup(<>)
SRowSet ENDS

MAPIERROR STRUCT
    ulVersion dd ?
    lpszError dd ?
    lpszComponent dd ?
    ulLowLevelError dd ?
    ulContext dd ?
MAPIERROR ENDS

ERROR_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    scode SCODE ?
    ulFlags dd ?
    lpMAPIError dd ?
ERROR_NOTIFICATION ENDS

NEWMAIL_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    cbParentID dd ?
    lpParentID dd ?
    ulFlags dd ?
    lpszMessageClass dd ?
    ulMessageFlags dd ?
NEWMAIL_NOTIFICATION ENDS

OBJECT_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    ulObjType dd ?
    cbParentID dd ?
    lpParentID dd ?
    cbOldID dd ?
    lpOldID dd ?
    cbOldParentID dd ?
    lpOldParentID dd ?
    lpPropTagArray dd ?
OBJECT_NOTIFICATION ENDS

TABLE_NOTIFICATION STRUCT
    ulTableEvent dd ?
    hResult dd ?
    propIndex SPropValue <>
    propPrior SPropValue <>
    row SRow <>
    ulPad dd ?
TABLE_NOTIFICATION ENDS

EXTENDED_NOTIFICATION STRUCT
    ulEvent dd ?
    cb dd ?
    pbEventParameters dd ?
EXTENDED_NOTIFICATION ENDS

STATUS_OBJECT_NOTIFICATION STRUCT
    cbEntryID dd ?
    lpEntryID dd ?
    cValues dd ?
    lpPropVals dd ?
STATUS_OBJECT_NOTIFICATION ENDS

NOTIFICATION STRUCT
    ulEventType dd ?
    ulAlignPad dd ?
    union info
        err ERROR_NOTIFICATION <>
        newmail NEWMAIL_NOTIFICATION <>
        obj OBJECT_NOTIFICATION <>
        tab TABLE_NOTIFICATION <>
        ext EXTENDED_NOTIFICATION <>
        statobj STATUS_OBJECT_NOTIFICATION <>
    ends
NOTIFICATION ENDS

MAPINAMEID STRUCT
    lpguid dd ?
    ulKind dd ?
    union Kind
        lID dd ?
        lpwstrName dd ?
    ends
MAPINAMEID ENDS

SSortOrder STRUCT
    ulPropTag dd ?
    ulOrder dd ?
SSortOrder ENDS

SSortOrderSet STRUCT
    cSorts dd ?
    cCategories dd ?
    cExpanded dd ?
    aSort SSortOrder MAPI_DIM dup(<>)
SSortOrderSet ENDS

SAndRestriction STRUCT
    cRes dd ?
    lpRes dd ?
SAndRestriction ENDS

SOrRestriction STRUCT
    cRes dd ?
    lpRes dd ?
SOrRestriction ENDS

SNotRestriction STRUCT
    ulReserved dd ?
    lpRes dd ?
SNotRestriction ENDS

SContentRestriction STRUCT
    ulFuzzyLevel dd ?
    ulPropTag dd ?
    lpProp dd ?
SContentRestriction ends

SBitMaskRestriction STRUCT
    relBMR dd ?
    ulPropTag dd ?
    ulMask dd ?
SBitMaskRestriction ENDS

SPropertyRestriction STRUCT
    relop dd ?
    ulPropTag dd ?
    lpProp dd ?
SPropertyRestriction ENDS

SComparePropsRestriction STRUCT
    relop dd ?
    ulPropTag1 dd ?
    ulPropTag2 dd ?
SComparePropsRestriction ENDS

SSizeRestriction STRUCT
    relop dd ?
    ulPropTag dd ?
    cb dd ?
SSizeRestriction ENDS

SExistRestriction STRUCT
    ulReserved1 dd ?
    ulPropTag dd ?
    ulReserved2 dd ?
SExistRestriction ENDS

SSubRestriction STRUCT
    ulSubObject dd ?
    lpRes dd ?
SSubRestriction ENDS

SCommentRestriction STRUCT
    cValues dd ?
    lpRes dd ?
    lpProp dd ?
SCommentRestriction ENDS

SRestriction STRUCT
    rt dd ?
    union res
        resCompareProps SComparePropsRestriction <>
        resAnd SAndRestriction <>
        resOr SOrRestriction <>
        resNot SNotRestriction <>
        resContent SContentRestriction <>
        resProperty SPropertyRestriction <>
        resBitMask SBitMaskRestriction <>
        resSize SSizeRestriction <>
        resExist SExistRestriction <>
        resSub SSubRestriction <>
        resComment SCommentRestriction <>
	ends
SRestriction ENDS

FlagList STRUCT
    cFlags dd ?
    ulFlag dd MAPI_DIM dup(?)
FlagList ENDS

ADRPARM STRUCT
    cbABContEntryID dd ?
    lpABContEntryID dd ?
    ulFlags dd ?
    lpReserved dd ?
    ulHelpContext dd ?
    lpszHelpFileName dd ?
    lpfnABSDI dd ?
    lpfnDismiss dd ?
    lpvDismissContext dd ?
    lpszCaption dd ?
    lpszNewEntryTitle dd ?
    lpszDestWellsTitle dd ?
    cDestFields dd ?
    nDestFieldFocus dd ?
    lppszDestTitles dd ?
    lpulDestComps dd ?
    lpContRestriction dd ?
    lpHierRestriction dd ?
ADRPARM ENDS

DTBLLABEL STRUCT
    ulbLpszLabelName dd ?
    ulFlags dd ?
DTBLLABEL ENDS

DTBLEDIT STRUCT
    ulbLpszCharsAllowed dd ?
    ulFlags dd ?
    ulNumCharsAllowed dd ?
    ulPropTag dd ?
DTBLEDIT ENDS

DTBLLBX STRUCT
    ulFlags dd ?
    ulPRSetProperty dd ?
    ulPRTableName dd ?
DTBLLBX ENDS

DTBLCOMBOBOX STRUCT
    ulbLpszCharsAllowed dd ?
    ulFlags dd ?
    ulNumCharsAllowed dd ?
    ulPRPropertyName dd ?
    ulPRTableName dd ?
DTBLCOMBOBOX ENDS

DTBLDDLBX STRUCT
    ulFlags dd ?
    ulPRDisplayProperty dd ?
    ulPRSetProperty dd ?
    ulPRTableName dd ?
DTBLDDLBX ENDS

DTBLCHECKBOX STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulPRPropertyName dd ?
DTBLCHECKBOX ENDS

DTBLGROUPBOX STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
DTBLGROUPBOX ENDS

DTBLBUTTON STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulPRControl dd ?
DTBLBUTTON ENDS

DTBLPAGE STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulbLpszComponent dd ?
    ulContext dd ?
DTBLPAGE ENDS

DTBLRADIOBUTTON STRUCT
    ulbLpszLabel dd ?
    ulFlags dd ?
    ulcButtons dd ?
    ulPropTag dd ?
    lReturnValue dd ?
DTBLRADIOBUTTON ENDS

DTBLMVLISTBOX STRUCT
    ulFlags dd ?
    ulMVPropTag dd ?
DTBLMVLISTBOX ENDS

DTBLMVDDLBX STRUCT
    ulFlags dd ?
    ulMVPropTag dd ?
DTBLMVDDLBX ENDS

FORMPRINTSETUP STRUCT
    ulFlags dd ?
    hDevMode dd ?
    hDevNames dd ?
    ulFirstPageNumber dd ?
    fPrintAttachments dd ?
FORMPRINTSETUP ENDS

SMAPIFormPropEnumVal STRUCT
    pszDisplayName dd ?
    nVal dd ?
SMAPIFormPropEnumVal ENDS

SMAPIFormProp STRUCT
    ulFlags dd ?
    nPropType dd ?
    nmid MAPINAMEID <>
    pszDisplayName dd ?
    nSpecialType dd ?
    union u
        struct sl
            nmidIdx MAPINAMEID <>
            cfpevAvailable dd ?
            pfpevAvailable dd ?
		  ends
	 ends
SMAPIFormProp ENDS

SMAPIFormPropArray STRUCT
    cProps dd ?
    ulPad dd ?
    aFormProp SMAPIFormProp MAPI_DIM dup(<>)
SMAPIFormPropArray ENDS

SMAPIVerb STRUCT
    lVerb dd ?
    szVerbname dd ?
    fuFlags dd ?
    grfAttribs dd ?
    ulFlags dd ?
SMAPIVerb ENDS

SMAPIVerbArray STRUCT
    cMAPIVerb dd ?
    aMAPIVerb SMAPIVerb MAPI_DIM dup(<>)
SMAPIVerbArray ENDS


SMessageClassArray STRUCT
    cValues dd ?
    aMessageClass dd MAPI_DIM dup(?)
SMessageClassArray ENDS

SMAPIFormInfoArray STRUCT
    cForms dd ?
    aFormInfo dd MAPI_DIM dup(?)
SMAPIFormInfoArray ENDS


NOTIFKEY STRUCT
    cb dd ?
    ab db MAPI_DIM dup(?)
NOTIFKEY ENDS

OPTIONDATA STRUCT
    ulFlags dd ?
    lpRecipGUID dd ?
    lpszAdrType dd ?
    lpszDLLName dd ?
    ulOrdinal dd ?
    cbOptionsData dd ?
    lpbOptionsData dd ?
    cOptionsProps dd ?
    lpOptionsProps dd ?
OPTIONDATA ENDS


DTCTL STRUCT
    ulCtlType dd ?
    ulCtlFlags dd ?
    lpbNotif dd ?
    cbNotif dd ?
    lpszFilter dd ?
    ulItemID dd ?
    union ctl
        lpv dd ?
        lplabel dd ?
        lpedit dd ?
        lplbx dd ?
        lpcombobox dd ?
        lpddlbx dd ?
        lpcheckbox dd ?
        lpgroupbox dd ?
        lpbutton dd ?
        lpradiobutton dd ?
        lpmvlbx dd ?
        lpmvddlbx dd ?
        lppage dd ?
	  ends
DTCTL ENDS

DTPAGE STRUCT
    cctl dd ?
    lpszResourceName dd ?
    union
        lpszComponent dd ?
        ulItemID dd ?
    ends
    lpctl dd ?
DTPAGE ENDS

MAPIINIT_0 STRUCT
    ulVersion dd    ?
    ulFlags dd      ?
MAPIINIT_0 ENDS

DATE_STRUCT struct
         year dw ?
        month dw ?
        day dw ?
DATE_STRUCT ends
SQL_DATE_STRUCT equ DATE_STRUCT
TIME_STRUCT struct
        hour dw ?
        minute dw ?
        second dw ?
TIME_STRUCT ends
SQL_TIME_STRUCT equ TIME_STRUCT
TIMESTAMP_STRUCT struct
        year dw ?
        month dw ?
        day dw ?
        hour dw ?
        minute dw ?
        second dw ?
        fraction dd ?
TIMESTAMP_STRUCT ends

SQL_YEAR_MONTH_STRUCT struct
		year dword ?
		month dword ?
SQL_YEAR_MONTH_STRUCT ends
SQL_DAY_SECOND_STRUCT struct
		day dword ?
		hour dword ?
		minute dword ?
		second dword ?
		fraction dword ?
SQL_DAY_SECOND_STRUCT ends

SQL_INTERVAL_STRUCT struct
	interval_type dword ?
	interval_sign dword ?
	union intval
		year_month SQL_YEAR_MONTH_STRUCT <>
		day_second SQL_DAY_SECOND_STRUCT <>
	ends
SQL_INTERVAL_STRUCT ends

SQL_NUMERIC_STRUCT struct
	precision db ?
	scale db ?
	sign db ?
	val db SQL_MAX_NUMERIC_LEN dup(?)
SQL_NUMERIC_STRUCT ends
SQLGUID struct
    Data1 dd ?
    Data2 dw ?
    Data3 dw ?
    Data4  db 8 dup(?)
SQLGUID ends

SHQUERYRBINFO STRUCT
 cbSize         dd ?
 i64Size        qword ?
 i64NumItems    qword ?
 SHQUERYRBINFO ENDS

INTERNET_ASYNC_RESULT STRUCT
 dwResult   dd ?
 dwError    dd ?
INTERNET_ASYNC_RESULT ENDS

INTERNET_PROXY_INFO STRUCT
 dwAccessType       dd ?
 lpszProxy          dd ?
 lpszProxyBypass    dd ?
INTERNET_PROXY_INFO ENDS

INTERNET_VERSION_INFO STRUCT
 dwMajorVersion dd ?
 dwMinorVersion dd ?
INTERNET_VERSION_INFO ENDS

HTTP_VERSION_INFO STRUCT
 dwMajorVersion dd ?
 dwMinorVersion dd ?
HTTP_VERSION_INFO ENDS

INTERNET_CONNECTED_INFO STRUCT
 dwConnectedState   dd ?
 dwFlags            dd ?
INTERNET_CONNECTED_INFO ENDS

URL_COMPONENTSA STRUCT dword
 dwStructSize       dd ?
 lpszScheme         dd ?
 dwSchemeLength     dd ?
 nScheme            dd ?
 lpszHostName       dd ?
 dwHostNameLength   dd ?
 nPort              dw ?
 lpszUserName       dd ?
 dwUserNameLength   dd ?
 lpszPassword       dd ?
 dwPasswordLength   dd ?
 lpszUrlPath        dd ?
 dwUrlPathLength    dd ?
 lpszExtraInfo      dd ?
 dwExtraInfoLength  dd ?
URL_COMPONENTSA ENDS
URL_COMPONENTS equ <URL_COMPONENTSA>

INTERNET_CERTIFICATE_INFO STRUCT
 ftExpiry               FILETIME <>
 ftStart                FILETIME <>
 lpszSubjectInfo        dd ?
 lpszIssuerInfo         dd ?
 lpszProtocolName       dd ?
 lpszSignatureAlgName   dd ?
 lpszEncryptionAlgName  dd ?
 dwKeySize              dd ?
INTERNET_CERTIFICATE_INFO ENDS

INTERNET_BUFFERSA STRUCT
 dwStructSize   dd ?
 Next           dd ?
 lpcszHeader    dd ?
 dwHeadersLength dd ?
 dwHeadersTotal dd ?
 lpvBuffer      dd ?
 dwBufferLength dd ?
 dwBufferTotal  dd ?
 dwOffsetLow    dd ?
 dwOffsetHigh   dd ?
INTERNET_BUFFERSA ENDS
INTERNET_BUFFERS equ <INTERNET_BUFFERSA>

GOPHER_FIND_DATAA STRUCT
 DisplayString db MAX_GOPHER_DISPLAY_TEXT + 1 dup(?)
 GopherType dd ?
 SizeLow    dd ?
 SizeHigh   dd ?
 LastModificationTime FILETIME <>
 Locator    db MAX_GOPHER_LOCATOR_LENGTH + 1 dup(?)
GOPHER_FIND_DATAA ENDS
GOPHER_FIND_DATA equ <GOPHER_FIND_DATAA>

GOPHER_ADMIN_ATTRIBUTE_TYPE STRUCT
 Comment        dd ?
 EmailAddress   dd ?
GOPHER_ADMIN_ATTRIBUTE_TYPE ENDS

GOPHER_MOD_DATE_ATTRIBUTE_TYPE STRUCT
 DateAndTime FILETIME <>
GOPHER_MOD_DATE_ATTRIBUTE_TYPE ENDS

GOPHER_TTL_ATTRIBUTE_TYPE STRUCT
 Ttl dd ?
GOPHER_TTL_ATTRIBUTE_TYPE ENDS

GOPHER_SCORE_ATTRIBUTE_TYPE STRUCT
 Score dd ?
GOPHER_SCORE_ATTRIBUTE_TYPE ENDS

GOPHER_SCORE_RANGE_ATTRIBUTE_TYPE STRUCT
 LowerBound dd ?
 UpperBound dd ?
GOPHER_SCORE_RANGE_ATTRIBUTE_TYPE ENDS

GOPHER_SITE_ATTRIBUTE_TYPE STRUCT
 Site dd ?
GOPHER_SITE_ATTRIBUTE_TYPE ENDS

GOPHER_ORGANIZATION_ATTRIBUTE_TYPE STRUCT
 Organization dd ?
GOPHER_ORGANIZATION_ATTRIBUTE_TYPE ENDS

GOPHER_LOCATION_ATTRIBUTE_TYPE STRUCT
 Location dd ?
GOPHER_LOCATION_ATTRIBUTE_TYPE ENDS

GOPHER_GEOGRAPHICAL_LOCATION_ATTRIBUTE_TYPE STRUCT
 DegreesNorth   dd ?
 MinutesNorth   dd ?
 SecondsNorth   dd ?
 DegreesEast    dd ?
 MinutesEast    dd ?
 SecondsEast    dd ?
GOPHER_GEOGRAPHICAL_LOCATION_ATTRIBUTE_TYPE ENDS

GOPHER_TIMEZONE_ATTRIBUTE_TYPE STRUCT
 Zone dd ?
GOPHER_TIMEZONE_ATTRIBUTE_TYPE ENDS

GOPHER_PROVIDER_ATTRIBUTE_TYPE STRUCT
 Provider dd ?
GOPHER_PROVIDER_ATTRIBUTE_TYPE ENDS

GOPHER_VERSION_ATTRIBUTE_TYPE STRUCT
 Version dd ?
GOPHER_VERSION_ATTRIBUTE_TYPE ENDS

GOPHER_ABSTRACT_ATTRIBUTE_TYPE STRUCT
 ShortAbstract  dd ?
 AbstractFile   dd ?
GOPHER_ABSTRACT_ATTRIBUTE_TYPE ENDS

GOPHER_VIEW_ATTRIBUTE_TYPE STRUCT
 ContentType    dd ?
 Language       dd ?
 iSize          dd ?
GOPHER_VIEW_ATTRIBUTE_TYPE ENDS

GOPHER_VERONICA_ATTRIBUTE_TYPE STRUCT
 TreeWalk dd ?
GOPHER_VERONICA_ATTRIBUTE_TYPE ENDS

GOPHER_ASK_ATTRIBUTE_TYPE STRUCT
 QuestionType dd ?
 QuestionText dd ?
GOPHER_ASK_ATTRIBUTE_TYPE ENDS

GOPHER_UNKNOWN_ATTRIBUTE_TYPE STRUCT
 Text dd ?
GOPHER_UNKNOWN_ATTRIBUTE_TYPE ENDS

GOPHER_ATTRIBUTE_TYPE STRUCT
 CategoryId     dd ?
 AttributeId    dd ?
 union AttributeType
 Admin      GOPHER_ADMIN_ATTRIBUTE_TYPE <>
 ModDate    GOPHER_MOD_DATE_ATTRIBUTE_TYPE <>
 Ttl        GOPHER_TTL_ATTRIBUTE_TYPE <>
 Score      GOPHER_SCORE_ATTRIBUTE_TYPE <>
 ScoreRange GOPHER_SCORE_RANGE_ATTRIBUTE_TYPE <>
 Site       GOPHER_SITE_ATTRIBUTE_TYPE <>
 Organization GOPHER_ORGANIZATION_ATTRIBUTE_TYPE <>
 Location   GOPHER_LOCATION_ATTRIBUTE_TYPE <>
 GeographicalLocation GOPHER_GEOGRAPHICAL_LOCATION_ATTRIBUTE_TYPE <>
 TimeZone   GOPHER_TIMEZONE_ATTRIBUTE_TYPE <>
 Provider   GOPHER_PROVIDER_ATTRIBUTE_TYPE <>
 Version    GOPHER_VERSION_ATTRIBUTE_TYPE <>
 Abstract   GOPHER_ABSTRACT_ATTRIBUTE_TYPE <>
 View       GOPHER_VIEW_ATTRIBUTE_TYPE <>
 Veronica   GOPHER_VERONICA_ATTRIBUTE_TYPE <>
 Ask        GOPHER_ASK_ATTRIBUTE_TYPE <>
 Unknown    GOPHER_UNKNOWN_ATTRIBUTE_TYPE <>
 ENDS
GOPHER_ATTRIBUTE_TYPE ENDS

INTERNET_AUTH_NOTIFY_DATA STRUCT
 cbStruct   dd ?
 dwOptions  dd ?
 pfnNotify  dd ?
 dwContext  dd ?
INTERNET_AUTH_NOTIFY_DATA ENDS

INTERNET_CACHE_ENTRY_INFOA STRUCT
 dwStructSize       dd ?
 lpszSourceUrlName  dd ?
 lpszLocalFileName  dd ?
 CacheEntryType     dd ?
 dwUseCount         dd ?
 dwHitRate          dd ?
 dwSizeLow          dd ?
 dwSizeHigh         dd ?
 LastModifiedTime   FILETIME <>
 ExpireTime         FILETIME <>
 LastAccessTime     FILETIME <>
 LastSyncTime       FILETIME <>
 lpHeaderInfo       dd ?
 dwHeaderInfoSize   dd ?
 lpszFileExtension  dd ?
 union
 dwReserved         dd ?
 dwExemptDelta      dd ?
 ENDS
INTERNET_CACHE_ENTRY_INFOA ENDS
INTERNET_CACHE_ENTRY_INFO equ <INTERNET_CACHE_ENTRY_INFOA>
SHELLFLAGSTATE record fShowAllObjects:1,fShowExtensions:1, fNoConfirmRecycle:1,fShowSysFiles:1,fShowCompColor:1,fDoubleClickInWebView:1,fDesktopHTML:1,fWin95Classic:1,fDontPrettyPath:1,fShowAttribCol:1,fMapNetDrvBtn:1,fShowInfoTip:1,fHideIcons:1, fRestFlags:3

SHITEMID STRUCT
 cb     dw ?
 abID   db 1 dup(?) 	; variable length
SHITEMID ENDS

ITEMIDLIST STRUCT
 mkid SHITEMID <>
ITEMIDLIST ENDS

CMINVOKECOMMANDINFO STRUCT
 cbSize         dd ?
 fMask          dd ?
 hwnd           dd ?
 lpVerb         dd ?
 lpParameters   dd ?
 lpDirectory    dd ?
 nShow          dd ?
 dwHotKey       dd ?
 hIcon          dd ?
CMINVOKECOMMANDINFO ENDS

CMINVOKECOMMANDINFOEX STRUCT
 cbSize         dd ?
 fMask          dd ?
 hwnd           dd ?
 lpVerb         dd ?
 lpParameters   dd ?
 lpDirectory    dd ?
 nShow          dd ?
 dwHotKey       dd ?
 hIcon          dd ?
 lpTitle        dd ?
 lpVerbW        dd ?
 lpParametersW  dd ?
 lpDirectoryW   dd ?
 lpTitleW       dd ?
 ptInvoke       POINT <>
CMINVOKECOMMANDINFOEX ENDS

FVSHOWINFO STRUCT
 cbSize     dd ?
 hwndOwner  dd ?
 iShow      dd ?
 dwFlags    dd ?
 rect       RECT <>
 punkRel    dd ?
 strNewFile db MAX_PATH dup(?)
FVSHOWINFO ENDS

FOLDERSETTINGS STRUCT
 ViewMode   dd ?
 fFlags     dd ?
FOLDERSETTINGS ENDS

SV2CVW2_PARAMS STRUCT
 cbSize     dd ?
 psvPrev    dd ?
 pfs        dd ?
 psbOwner   dd ?
 prcView    dd ?
 pvid       dd ?
 hwndView   dd ?
SV2CVW2_PARAMS ENDS


BROWSEINFOA STRUCT
 hwndOwner      dd ?
 pidlRoot       dd ?
 pszDisplayName dd ?
 lpszTitle      dd ?
 ulFlags        dd ?
 lpfn           dd ?
 lParam         dd ?
 iImage         dd ?
BROWSEINFOA ENDS
BROWSEINFO equ <BROWSEINFOA>

DESKBANDINFO STRUCT
 dwMask         dd ?
 ptMinSize      POINT <>
 ptMaxSize      POINT <>
 ptIntegral     POINT <>
 ptActual       POINT <>
 wszTitle       WCHAR 256 dup(?)
 dwModeFlags    dd ?
 crBkgnd        COLORREF ?
DESKBANDINFO ENDS

WALLPAPEROPT STRUCT
 dwSize     dd ?
 dwStyle    dd ?
WALLPAPEROPT ENDS

COMPONENTSOPT STRUCT
 dwSize             dd ?
 fEnableComponents  dd ?
 fActiveDesktop     dd ?
COMPONENTSOPT ENDS

COMPPOS STRUCT
 dwSize                 dd ?
 iLeft                  dd ?
 iTop                   dd ?
 dwWidth                dd ?
 dwHeight               dd ?
 izIndex                dd ?
 fCanResize             dd ?
 fCanResizeX            dd ?
 fCanResizeY            dd ?
 iPreferredLeftPercent  dd ?
 iPreferredTopPercent   dd ?
COMPPOS ENDS

COMPONENT STRUCT
 dwSize     dd ?
 dwID       dd ?
 iComponentType dd ?
 fChecked   dd ?
 fDirty     dd ?
 fNoScroll  dd ?
 cpPos      COMPPOS <>
 wszFriendlyName WCHAR MAX_PATH dup(?)
 wszSource WCHAR INTERNET_MAX_URL_LENGTH dup(?)
 wszSubscribedURL WCHAR INTERNET_MAX_URL_LENGTH dup(?)
COMPONENT ENDS

NRESARRAY STRUCT
 cItems dd ?
 nr NETRESOURCE <>
NRESARRAY ENDS

CIDA STRUCT
 cidl dd ?
 aoffset dd 1 dup(?)
CIDA ENDS

FILEDESCRIPTORA STRUCT
 dwFlags        dd ?
 clsid          CLSID    <>
 sizel          SIZEL    <>
 pointl         POINTL  <>
 dwFileAttributes dd ?
 ftCreationTime FILETIME <>
 ftLastAccessTime FILETIME <>
 ftLastWriteTime FILETIME <>
 nFileSizeHigh  dd ?
 nFileSizeLow   dd ?
 cFileName      db MAX_PATH dup(?)
FILEDESCRIPTORA ENDS
FILEDESCRIPTOR equ <FILEDESCRIPTORA>

FILEGROUPDESCRIPTORA STRUCT
 cItems dd ?
 fgd FILEDESCRIPTOR <>
FILEGROUPDESCRIPTORA ENDS
FILEGROUPDESCRIPTOR equ <FILEGROUPDESCRIPTORA>

DROPFILES STRUCT
 pFiles dd ?
 pt     POINT <>
 fNC    dd ?
 fWide  dd ?
DROPFILES ENDS

SHDESCRIPTIONID STRUCT
 dwDescriptionId    dd ?
 clsid              CLSID <>
SHDESCRIPTIONID ENDS


SERVER_INFO_100 STRUCT
    sv100_platform_id dd ?
    sv100_name dd ?
SERVER_INFO_100 ENDS

SERVER_INFO_101 STRUCT
    sv101_platform_id dd ?
    sv101_name dd ?
    sv101_version_major dd ?
    sv101_version_minor dd ?
    sv101_type dd ?
    sv101_comment dd ?
SERVER_INFO_101 ENDS

SERVER_INFO_102 STRUCT
     sv102_platform_id dd ?
     sv102_name dd ?
     sv102_version_major dd ?
     sv102_version_minor dd ?
     sv102_type dd ?
     sv102_comment dd ?
     sv102_users dd ?
     sv102_disc dd ?
     sv102_hidden dd ?
     sv102_announce dd ?
     sv102_anndelta dd ?
     sv102_licenses dd ?
     sv102_userpath dd ?
SERVER_INFO_102 ENDS

SERVER_INFO_402  STRUCT
     sv402_ulist_mtime dd ?
     sv402_glist_mtime dd ?
     sv402_alist_mtime dd ?
     sv402_alerts dd ?
     sv402_security dd ?
     sv402_numadmin dd ?
     sv402_lanmask dd ?
     sv402_guestacct dd ?
     sv402_chdevs dd ?
     sv402_chdevq dd ?
     sv402_chdevjobs dd ?
     sv402_connections dd ?
     sv402_shares dd ?
     sv402_openfiles dd ?
     sv402_sessopens dd ?
     sv402_sessvcs dd ?
     sv402_sessreqs dd ?
     sv402_opensearch dd ?
     sv402_activelocks dd ?
     sv402_numreqbuf dd ?
     sv402_sizreqbuf dd ?
     sv402_numbigbuf dd ?
     sv402_numfiletasks dd ?
     sv402_alertsched dd ?
     sv402_erroralert dd ?
     sv402_logonalert dd ?
     sv402_accessalert dd ?
     sv402_diskalert dd ?
     sv402_netioalert dd ?
     sv402_maxauditsz dd ?
     sv402_srvheuristics dd ?
SERVER_INFO_402 ENDS

SERVER_INFO_403 STRUCT
     sv403_ulist_mtime dd ?
     sv403_glist_mtime dd ?
     sv403_alist_mtime dd ?
     sv403_alerts dd ?
     sv403_security dd ?
     sv403_numadmin dd ?
     sv403_lanmask dd ?
     sv403_guestacct dd ?
     sv403_chdevs dd ?
     sv403_chdevq dd ?
     sv403_chdevjobs dd ?
     sv403_connections dd ?
     sv403_shares dd ?
     sv403_openfiles dd ?
     sv403_sessopens dd ?
     sv403_sessvcs dd ?
     sv403_sessreqs dd ?
     sv403_opensearch dd ?
     sv403_activelocks dd ?
     sv403_numreqbuf dd ?
     sv403_sizreqbuf dd ?
     sv403_numbigbuf dd ?
     sv403_numfiletasks dd ?
     sv403_alertsched dd ?
     sv403_erroralert dd ?
     sv403_logonalert dd ?
     sv403_accessalert dd ?
     sv403_diskalert dd ?
     sv403_netioalert dd ?
     sv403_maxauditsz dd ?
     sv403_srvheuristics dd ?
     sv403_auditedevents dd ?
     sv403_autoprofile dd ?
     sv403_autopath dd ?
SERVER_INFO_403 ENDS

SERVER_INFO_502 STRUCT
    sv502_sessopens dd ?
    sv502_sessvcs dd ?
    sv502_opensearch dd ?
    sv502_sizreqbuf dd ?
    sv502_initworkitems dd ?
    sv502_maxworkitems dd ?
    sv502_rawworkitems dd ?
    sv502_irpstacksize dd ?
    sv502_maxrawbuflen dd ?
    sv502_sessusers dd ?
    sv502_sessconns dd ?
    sv502_maxpagedmemoryusage dd ?
    sv502_maxnonpagedmemoryusage dd ?
    sv502_enablesoftcompat dd ?
    sv502_enableforcedlogoff dd ?
    sv502_timesource dd ?
    sv502_acceptdownlevelapis dd ?
    sv502_lmannounce dd ?
SERVER_INFO_502 ENDS

SERVER_INFO_503 STRUCT
    sv503_sessopens dd ?
    sv503_sessvcs dd ?
    sv503_opensearch dd ?
    sv503_sizreqbuf dd ?
    sv503_initworkitems dd ?
    sv503_maxworkitems dd ?
    sv503_rawworkitems dd ?
    sv503_irpstacksize dd ?
    sv503_maxrawbuflen dd ?
    sv503_sessusers dd ?
    sv503_sessconns dd ?
    sv503_maxpagedmemoryusage dd ?
    sv503_maxnonpagedmemoryusage dd ?
    sv503_enablesoftcompat dd ?
    sv503_enableforcedlogoff dd ?
    sv503_timesource dd ?
    sv503_acceptdownlevelapis dd ?
    sv503_lmannounce dd ?
    sv503_domain dd ?
    sv503_maxcopyreadlen dd ?
    sv503_maxcopywritelen dd ?
    sv503_minkeepsearch dd ?
    sv503_maxkeepsearch dd ?
    sv503_minkeepcomplsearch dd ?
    sv503_maxkeepcomplsearch dd ?
    sv503_threadcountadd dd ?
    sv503_numblockthreads dd ?
    sv503_scavtimeout dd ?
    sv503_minrcvqueue dd ?
    sv503_minfreeworkitems dd ?
    sv503_xactmemsize dd ?
    sv503_threadpriority dd ?
    sv503_maxmpxct dd ?
    sv503_oplockbreakwait dd ?
    sv503_oplockbreakresponsewait dd ?
    sv503_enableoplocks dd ?
    sv503_enableoplockforceclose dd ?
    sv503_enablefcbopens dd ?
    sv503_enableraw dd ?
    sv503_enablesharednetdrives dd ?
    sv503_minfreeconnections dd ?
    sv503_maxfreeconnections dd ?
SERVER_INFO_503 ENDS

SERVER_INFO_599 STRUCT
    sv599_sessopens dd ?
    sv599_sessvcs dd ?
    sv599_opensearch dd ?
    sv599_sizreqbuf dd ?
    sv599_initworkitems dd ?
    sv599_maxworkitems dd ?
    sv599_rawworkitems dd ?
    sv599_irpstacksize dd ?
    sv599_maxrawbuflen dd ?
    sv599_sessusers dd ?
    sv599_sessconns dd ?
    sv599_maxpagedmemoryusage dd ?
    sv599_maxnonpagedmemoryusage dd ?
    sv599_enablesoftcompat dd ?
    sv599_enableforcedlogoff dd ?
    sv599_timesource dd ?
    sv599_acceptdownlevelapis dd ?
    sv599_lmannounce dd ?
    sv599_domain dd ?
    sv599_maxcopyreadlen dd ?
    sv599_maxcopywritelen dd ?
    sv599_minkeepsearch dd ?
    sv599_maxkeepsearch dd ?
    sv599_minkeepcomplsearch dd ?
    sv599_maxkeepcomplsearch dd ?
    sv599_threadcountadd dd ?
    sv599_numblockthreads dd ?
    sv599_scavtimeout dd ?
    sv599_minrcvqueue dd ?
    sv599_minfreeworkitems dd ?
    sv599_xactmemsize dd ?
    sv599_threadpriority dd ?
    sv599_maxmpxct dd ?
    sv599_oplockbreakwait dd ?
    sv599_oplockbreakresponsewait dd ?
    sv599_enableoplocks dd ?
    sv599_enableoplockforceclose dd ?
    sv599_enablefcbopens dd ?
    sv599_enableraw dd ?
    sv599_enablesharednetdrives dd ?
    sv599_minfreeconnections dd ?
    sv599_maxfreeconnections dd ?
    sv599_initsesstable dd ?
    sv599_initconntable dd ?
    sv599_initfiletable dd ?
    sv599_initsearchtable dd ?
    sv599_alertschedule dd ?
    sv599_errorthreshold dd ?
    sv599_networkerrorthreshold dd ?
    sv599_diskspacethreshold dd ?
    sv599_reserved dd ?
    sv599_maxlinkdelay dd ?
    sv599_minlinkthroughput dd ?
    sv599_linkinfovalidtime dd ?
    sv599_scavqosinfoupdatetime dd ?
    sv599_maxworkitemidletime dd ?
SERVER_INFO_599 ENDS

SERVER_INFO_598 STRUCT
    sv598_maxrawworkitems dd ?
    sv598_maxthreadsperqueue dd ?
    sv598_producttype dd ?
    sv598_serversize dd ?
    sv598_connectionlessautodisc dd ?
    sv598_sharingviolationretries dd ?
    sv598_sharingviolationdelay dd ?
    sv598_maxglobalopensearch dd ?
    sv598_removeduplicatesearches dd ?
    sv598_lockviolationoffset dd ?
    sv598_lockviolationdelay dd ?
    sv598_mdlreadswitchover dd ?
    sv598_cachedopenlimit dd ?
    sv598_otherqueueaffinity dd ?
    sv598_restrictnullsessaccess dd ?
    sv598_enablewfw311directipx dd ?
    sv598_queuesamplesecs dd ?
    sv598_balancecount dd ?
    sv598_preferredaffinity dd ?
    sv598_maxfreerfcbs dd ?
    sv598_maxfreemfcbs dd ?
    sv598_maxfreelfcbs dd ?
    sv598_maxfreepagedpoolchunks dd ?
    sv598_minpagedpoolchunksize dd ?
    sv598_maxpagedpoolchunksize dd ?
    sv598_sendsfrompreferredprocessor dd ?
    sv598_cacheddirectorylimit dd ?
    sv598_maxcopylength dd ?
    sv598_enablebulktransfer dd ?
    sv598_enablecompression dd ?
    sv598_autosharewks dd ?
    sv598_autoshareserver dd ?
    sv598_enablesecuritysignature dd ?
    sv598_requiresecuritysignature dd ?
    sv598_minclientbuffersize dd ?
    sv598_serverguid GUID <>
    sv598_ConnectionNoSessionsTimeout dd ?
SERVER_INFO_598 ENDS

SERVER_INFO_1005 STRUCT
    sv1005_comment dd ?
SERVER_INFO_1005 ENDS

SERVER_INFO_1107 STRUCT
    sv1107_users dd ?
SERVER_INFO_1107 ENDS

SERVER_INFO_1010 STRUCT
    sv1010_disc dd ?
SERVER_INFO_1010 ENDS

SERVER_INFO_1016 STRUCT
    sv1016_hidden dd ?
SERVER_INFO_1016 ENDS

SERVER_INFO_1017 STRUCT
    sv1017_announce dd ?
SERVER_INFO_1017 ENDS

SERVER_INFO_1018 STRUCT
    sv1018_anndelta dd ?
SERVER_INFO_1018 ENDS

SERVER_INFO_1501 STRUCT
    sv1501_sessopens dd ?
SERVER_INFO_1501 ENDS

SERVER_INFO_1502 STRUCT
    sv1502_sessvcs dd ?
SERVER_INFO_1502 ENDS

SERVER_INFO_1503 STRUCT
    sv1503_opensearch dd ?
SERVER_INFO_1503 ENDS

SERVER_INFO_1506 STRUCT
    sv1506_maxworkitems dd ?
SERVER_INFO_1506 ENDS

SERVER_INFO_1509 STRUCT
    sv1509_maxrawbuflen dd ?
SERVER_INFO_1509 ENDS

SERVER_INFO_1510 STRUCT
    sv1510_sessusers dd ?
SERVER_INFO_1510 ENDS

SERVER_INFO_1511 STRUCT
    sv1511_sessconns dd ?
SERVER_INFO_1511 ENDS

SERVER_INFO_1512 STRUCT
    sv1512_maxnonpagedmemoryusage dd ?
SERVER_INFO_1512 ENDS

SERVER_INFO_1513 STRUCT
    sv1513_maxpagedmemoryusage dd ?
SERVER_INFO_1513 ENDS

SERVER_INFO_1514 STRUCT
    sv1514_enablesoftcompat dd ?
SERVER_INFO_1514 ENDS

SERVER_INFO_1515 STRUCT
    sv1515_enableforcedlogoff dd ?
SERVER_INFO_1515 ENDS

SERVER_INFO_1516 STRUCT
    sv1516_timesource dd ?
SERVER_INFO_1516 ENDS

SERVER_INFO_1518 STRUCT
    sv1518_lmannounce dd ?
SERVER_INFO_1518 ENDS

SERVER_INFO_1520 STRUCT
    sv1520_maxcopyreadlen dd ?
SERVER_INFO_1520 ENDS

SERVER_INFO_1521 STRUCT
    sv1521_maxcopywritelen dd ?
SERVER_INFO_1521 ENDS

SERVER_INFO_1522 STRUCT
    sv1522_minkeepsearch dd ?
SERVER_INFO_1522 ENDS

SERVER_INFO_1523 STRUCT
    sv1523_maxkeepsearch dd ?
SERVER_INFO_1523 ENDS

SERVER_INFO_1524 STRUCT
    sv1524_minkeepcomplsearch dd ?
SERVER_INFO_1524 ENDS

SERVER_INFO_1525 STRUCT
    sv1525_maxkeepcomplsearch dd ?
SERVER_INFO_1525 ENDS

SERVER_INFO_1528 STRUCT
    sv1528_scavtimeout dd ?
SERVER_INFO_1528 ENDS

SERVER_INFO_1529 STRUCT
    sv1529_minrcvqueue dd ?
SERVER_INFO_1529 ENDS

SERVER_INFO_1530 STRUCT
    sv1530_minfreeworkitems dd ?
SERVER_INFO_1530 ENDS

SERVER_INFO_1533 STRUCT
    sv1533_maxmpxct dd ?
SERVER_INFO_1533 ENDS

SERVER_INFO_1534 STRUCT
    sv1534_oplockbreakwait dd ?
SERVER_INFO_1534 ENDS

SERVER_INFO_1535 STRUCT
    sv1535_oplockbreakresponsewait dd ?
SERVER_INFO_1535 ENDS

SERVER_INFO_1536 STRUCT
    sv1536_enableoplocks dd ?
SERVER_INFO_1536 ENDS

SERVER_INFO_1537 STRUCT
    sv1537_enableoplockforceclose dd ?
SERVER_INFO_1537 ENDS

SERVER_INFO_1538 STRUCT
    sv1538_enablefcbopens dd ?
SERVER_INFO_1538 ENDS

SERVER_INFO_1539 STRUCT
    sv1539_enableraw dd ?
SERVER_INFO_1539 ENDS

SERVER_INFO_1540 STRUCT
    sv1540_enablesharednetdrives dd ?
SERVER_INFO_1540 ENDS

SERVER_INFO_1541 STRUCT
    sv1541_minfreeconnections dd ?
SERVER_INFO_1541 ENDS

SERVER_INFO_1542 STRUCT
    sv1542_maxfreeconnections dd ?
SERVER_INFO_1542 ENDS

SERVER_INFO_1543 STRUCT
    sv1543_initsesstable dd ?
SERVER_INFO_1543 ENDS

SERVER_INFO_1544 STRUCT
    sv1544_initconntable dd ?
SERVER_INFO_1544 ENDS

SERVER_INFO_1545 STRUCT
    sv1545_initfiletable dd ?
SERVER_INFO_1545 ENDS

SERVER_INFO_1546 STRUCT
    sv1546_initsearchtable dd ?
SERVER_INFO_1546 ENDS

SERVER_INFO_1547 STRUCT
    sv1547_alertschedule dd ?
SERVER_INFO_1547 ENDS

SERVER_INFO_1548 STRUCT
    sv1548_errorthreshold dd ?
SERVER_INFO_1548 ENDS

SERVER_INFO_1549 STRUCT
    sv1549_networkerrorthreshold dd ?
SERVER_INFO_1549 ENDS

SERVER_INFO_1550 STRUCT
    sv1550_diskspacethreshold dd ?
SERVER_INFO_1550 ENDS

SERVER_INFO_1552 STRUCT
    sv1552_maxlinkdelay dd ?
SERVER_INFO_1552 ENDS

SERVER_INFO_1553 STRUCT
    sv1553_minlinkthroughput dd ?
SERVER_INFO_1553 ENDS

SERVER_INFO_1554 STRUCT
    sv1554_linkinfovalidtime dd ?
SERVER_INFO_1554 ENDS

SERVER_INFO_1555 STRUCT
    sv1555_scavqosinfoupdatetime dd ?
SERVER_INFO_1555 ENDS

SERVER_INFO_1556 STRUCT
    sv1556_maxworkitemidletime dd ?
SERVER_INFO_1556 ENDS

SERVER_INFO_1557 STRUCT
    sv1557_maxrawworkitems dd ?
SERVER_INFO_1557 ENDS

SERVER_INFO_1560 STRUCT
    sv1560_producttype dd ?
SERVER_INFO_1560 ENDS

SERVER_INFO_1561 STRUCT
    sv1561_serversize dd ?
SERVER_INFO_1561 ENDS

SERVER_INFO_1562 STRUCT
    sv1562_connectionlessautodisc dd ?
SERVER_INFO_1562 ENDS

SERVER_INFO_1563 STRUCT
    sv1563_sharingviolationretries dd ?
SERVER_INFO_1563 ENDS

SERVER_INFO_1564 STRUCT
    sv1564_sharingviolationdelay dd ?
SERVER_INFO_1564 ENDS

SERVER_INFO_1565 STRUCT
    sv1565_maxglobalopensearch dd ?
SERVER_INFO_1565 ENDS

SERVER_INFO_1566 STRUCT
    sv1566_removeduplicatesearches dd ?
SERVER_INFO_1566 ENDS

SERVER_INFO_1567 STRUCT
    sv1567_lockviolationretries dd ?
SERVER_INFO_1567 ENDS

SERVER_INFO_1568 STRUCT
    sv1568_lockviolationoffset dd ?
SERVER_INFO_1568 ENDS

SERVER_INFO_1569 STRUCT
    sv1569_lockviolationdelay dd ?
SERVER_INFO_1569 ENDS

SERVER_INFO_1570 STRUCT
    sv1570_mdlreadswitchover dd ?
SERVER_INFO_1570 ENDS

SERVER_INFO_1571 STRUCT
    sv1571_cachedopenlimit dd ?
SERVER_INFO_1571 ENDS

SERVER_INFO_1572 STRUCT
    sv1572_criticalthreads dd ?
SERVER_INFO_1572 ENDS

SERVER_INFO_1573 STRUCT
    sv1573_restrictnullsessaccess dd ?
SERVER_INFO_1573 ENDS

SERVER_INFO_1574 STRUCT
    sv1574_enablewfw311directipx dd ?
SERVER_INFO_1574 ENDS

SERVER_INFO_1575 STRUCT
    sv1575_otherqueueaffinity dd ?
SERVER_INFO_1575 ENDS

SERVER_INFO_1576 STRUCT
    sv1576_queuesamplesecs dd ?
SERVER_INFO_1576 ENDS

SERVER_INFO_1577 STRUCT
    sv1577_balancecount dd ?
SERVER_INFO_1577 ENDS

SERVER_INFO_1578 STRUCT
    sv1578_preferredaffinity dd ?
SERVER_INFO_1578 ENDS

SERVER_INFO_1579 STRUCT
    sv1579_maxfreerfcbs dd ?
SERVER_INFO_1579 ENDS

SERVER_INFO_1580 STRUCT
    sv1580_maxfreemfcbs dd ?
SERVER_INFO_1580 ENDS

SERVER_INFO_1581 STRUCT
    sv1581_maxfreemlcbs dd ?
SERVER_INFO_1581 ENDS

SERVER_INFO_1582 STRUCT
    sv1582_maxfreepagedpoolchunks dd ?
SERVER_INFO_1582 ENDS

SERVER_INFO_1583 STRUCT
    sv1583_minpagedpoolchunksize dd ?
SERVER_INFO_1583 ENDS

SERVER_INFO_1584 STRUCT
    sv1584_maxpagedpoolchunksize dd ?
SERVER_INFO_1584 ENDS

SERVER_INFO_1585 STRUCT
    sv1585_sendsfrompreferredprocessor dd ?
SERVER_INFO_1585 ENDS

SERVER_INFO_1586 STRUCT
    sv1586_maxthreadsperqueue dd ?
SERVER_INFO_1586 ENDS

SERVER_INFO_1587 STRUCT
    sv1587_cacheddirectorylimit dd ?
SERVER_INFO_1587 ENDS

SERVER_INFO_1588 STRUCT
    sv1588_maxcopylength dd ?
SERVER_INFO_1588 ENDS

SERVER_INFO_1589 STRUCT
    sv1589_enablebulktransfer dd ?
SERVER_INFO_1589 ENDS

SERVER_INFO_1590 STRUCT
    sv1590_enablecompression dd ?
SERVER_INFO_1590 ENDS

SERVER_INFO_1591 STRUCT
    sv1591_autosharewks dd ?
SERVER_INFO_1591 ENDS

SERVER_INFO_1592 STRUCT
    sv1592_autosharewks dd ?
SERVER_INFO_1592 ENDS

SERVER_INFO_1593 STRUCT
    sv1593_enablesecuritysignature dd ?
SERVER_INFO_1593 ENDS

SERVER_INFO_1594 STRUCT
    sv1594_requiresecuritysignature dd ?
SERVER_INFO_1594 ENDS

SERVER_INFO_1595 STRUCT
    sv1595_minclientbuffersize dd ?
SERVER_INFO_1595 ENDS

SERVER_INFO_1596 STRUCT
    sv1596_ConnectionNoSessionsTimeout dd ?
SERVER_INFO_1596 ENDS

SERVER_TRANSPORT_INFO_0 STRUCT
    svti0_numberofvcs dd ?
    svti0_transportname dd ?
    svti0_transportaddress dd ?
    svti0_transportaddresslength dd ?
    svti0_networkaddress dd ?
SERVER_TRANSPORT_INFO_0 ENDS

SERVER_TRANSPORT_INFO_1 STRUCT
    svti1_numberofvcs dd ?
    svti1_transportname dd ?
    svti1_transportaddress dd ?
    svti1_transportaddresslength dd ?
    svti1_networkaddress dd ?
    svti1_domain dd ?
SERVER_TRANSPORT_INFO_1 ENDS

SERVER_TRANSPORT_INFO_2 STRUCT
    svti2_numberofvcs dd ?
    svti2_transportname dd ?
    svti2_transportaddress dd ?
    svti2_transportaddresslength dd ?
    svti2_networkaddress dd ?
    svti2_domain dd ?
    svti2_flags dd ?
SERVER_TRANSPORT_INFO_2 ENDS


WKSTA_INFO_100 struct
    wki100_platform_id dd ?
    wki100_computername dd ?
    wki100_langroup dd ?
    wki100_ver_major dd ?
    wki100_ver_minor dd ?
WKSTA_INFO_100 ends

WKSTA_INFO_101 struct
    wki101_platform_id dd ?
    wki101_computername dd ?
    wki101_langroup dd ?
    wki101_ver_major dd ?
    wki101_ver_minor dd ?
    wki101_lanroot dd ?
WKSTA_INFO_101 ends

WKSTA_INFO_102  struct
    wki102_platform_id dd ?
    wki102_computername dd ?
    wki102_langroup dd ?
    wki102_ver_major dd ?
    wki102_ver_minor dd ?
    wki102_lanroot dd ?
    wki102_logged_on_users dd ?
WKSTA_INFO_102 ends

WKSTA_INFO_302 struct
    wki302_char_wait dd ?
    wki302_collection_time dd ?
    wki302_maximum_collection_count dd ?
    wki302_keep_conn dd ?
    wki302_keep_search dd ?
    wki302_max_cmds dd ?
    wki302_num_work_buf dd ?
    wki302_siz_work_buf dd ?
    wki302_max_wrk_cache dd ?
    wki302_sess_timeout dd ?
    wki302_siz_error dd ?
    wki302_num_alerts dd ?
    wki302_num_services dd ?
    wki302_errlog_sz dd ?
    wki302_print_buf_time dd ?
    wki302_num_char_buf dd ?
    wki302_siz_char_buf dd ?
    wki302_wrk_heuristics dd ?
    wki302_mailslots dd ?
    wki302_num_dgram_buf dd ?
WKSTA_INFO_302 ends

WKSTA_INFO_402 struct
    wki402_char_wait dd ?
    wki402_collection_time dd ?
    wki402_maximum_collection_count dd ?
    wki402_keep_conn dd ?
    wki402_keep_search dd ?
    wki402_max_cmds dd ?
    wki402_num_work_buf dd ?
    wki402_siz_work_buf dd ?
    wki402_max_wrk_cache dd ?
    wki402_sess_timeout dd ?
    wki402_siz_error dd ?
    wki402_num_alerts dd ?
    wki402_num_services dd ?
    wki402_errlog_sz dd ?
    wki402_print_buf_time dd ?
    wki402_num_char_buf dd ?
    wki402_siz_char_buf dd ?
    wki402_wrk_heuristics dd ?
    wki402_mailslots dd ?
    wki402_num_dgram_buf dd ?
    wki402_max_threads dd ?
WKSTA_INFO_402 ends

WKSTA_INFO_502 struct
    wki502_char_wait dd ?
    wki502_collection_time dd ?
    wki502_maximum_collection_count dd ?
    wki502_keep_conn dd ?
    wki502_max_cmds dd ?
    wki502_sess_timeout dd ?
    wki502_siz_char_buf dd ?
    wki502_max_threads dd ?
    wki502_lock_quota dd ?
    wki502_lock_increment dd ?
    wki502_lock_maximum dd ?
    wki502_pipe_increment dd ?
    wki502_pipe_maximum dd ?
    wki502_cache_file_timeout dd ?
    wki502_dormant_file_limit dd ?
    wki502_read_ahead_throughput dd ?
    wki502_num_mailslot_buffers dd ?
    wki502_num_srv_announce_buffers dd ?
    wki502_max_illegal_datagram_events dd ?
    wki502_illegal_datagram_event_reset_frequency dd ?
    wki502_log_election_packets dd ?
    wki502_use_opportunistic_locking dd ?
    wki502_use_unlock_behind dd ?
    wki502_use_close_behind dd ?
    wki502_buf_named_pipes dd ?
    wki502_use_lock_read_unlock dd ?
    wki502_utilize_nt_caching dd ?
    wki502_use_raw_read dd ?
    wki502_use_raw_write dd ?
    wki502_use_write_raw_data dd ?
    wki502_use_encryption dd ?
    wki502_buf_files_deny_write dd ?
    wki502_buf_read_only_files dd ?
    wki502_force_core_create_mode dd ?
    wki502_use_512_byte_max_transfer dd ?
WKSTA_INFO_502 ends

WKSTA_INFO_1010 struct
     wki1010_char_wait dd ?
WKSTA_INFO_1010 ends

WKSTA_INFO_1011 struct
     wki1011_collection_time dd ?
WKSTA_INFO_1011 ends

WKSTA_INFO_1012 struct
     wki1012_maximum_collection_count dd ?
WKSTA_INFO_1012 ends

WKSTA_INFO_1027 struct
     wki1027_errlog_sz dd ?
WKSTA_INFO_1027 ends

WKSTA_INFO_1028 struct
     wki1028_print_buf_time dd ?
WKSTA_INFO_1028 ends

WKSTA_INFO_1032 struct
     wki1032_wrk_heuristics dd ?
WKSTA_INFO_1032 ends

WKSTA_INFO_1013 struct
     wki1013_keep_conn dd ?
WKSTA_INFO_1013 ends

WKSTA_INFO_1018 struct
     wki1018_sess_timeout dd ?
WKSTA_INFO_1018 ends

WKSTA_INFO_1023 struct
     wki1023_siz_char_buf dd ?
WKSTA_INFO_1023 ends

WKSTA_INFO_1033 struct
     wki1033_max_threads dd ?
WKSTA_INFO_1033 ends

WKSTA_INFO_1041 struct
    wki1041_lock_quota dd ?
WKSTA_INFO_1041 ends

WKSTA_INFO_1042 struct
    wki1042_lock_increment dd ?
WKSTA_INFO_1042 ends

WKSTA_INFO_1043 struct
    wki1043_lock_maximum dd ?
WKSTA_INFO_1043 ends

WKSTA_INFO_1044 struct
    wki1044_pipe_increment dd ?
WKSTA_INFO_1044 ends

WKSTA_INFO_1045 struct
    wki1045_pipe_maximum dd ?
WKSTA_INFO_1045 ends

WKSTA_INFO_1046 struct
    wki1046_dormant_file_limit dd ?
WKSTA_INFO_1046 ends

WKSTA_INFO_1047 struct
    wki1047_cache_file_timeout dd ?
WKSTA_INFO_1047 ends

WKSTA_INFO_1048 struct
    wki1048_use_opportunistic_locking dd ?
WKSTA_INFO_1048 ends

WKSTA_INFO_1049 struct
    wki1049_use_unlock_behind dd ?
WKSTA_INFO_1049 ends

WKSTA_INFO_1050 struct
    wki1050_use_close_behind dd ?
WKSTA_INFO_1050 ends

WKSTA_INFO_1051 struct
    wki1051_buf_named_pipes dd ?
WKSTA_INFO_1051 ends

WKSTA_INFO_1052 struct
    wki1052_use_lock_read_unlock dd ?
WKSTA_INFO_1052 ends

WKSTA_INFO_1053 struct
    wki1053_utilize_nt_caching dd ?
WKSTA_INFO_1053 ends

WKSTA_INFO_1054 struct
    wki1054_use_raw_read dd ?
WKSTA_INFO_1054 ends

WKSTA_INFO_1055 struct
    wki1055_use_raw_write dd ?
WKSTA_INFO_1055 ends

WKSTA_INFO_1056 struct
    wki1056_use_write_raw_data dd ?
WKSTA_INFO_1056 ends

WKSTA_INFO_1057 struct
    wki1057_use_encryption dd ?
WKSTA_INFO_1057 ends

WKSTA_INFO_1058 struct
    wki1058_buf_files_deny_write dd ?
WKSTA_INFO_1058 ends

WKSTA_INFO_1059 struct
    wki1059_buf_read_only_files dd ?
WKSTA_INFO_1059 ends

WKSTA_INFO_1060 struct
    wki1060_force_core_create_mode dd ?
WKSTA_INFO_1060 ends

WKSTA_INFO_1061 struct
    wki1061_use_512_byte_max_transfer dd ?
WKSTA_INFO_1061 ends

WKSTA_INFO_1062 struct
    wki1062_read_ahead_throughput dd ?
WKSTA_INFO_1062 ends

WKSTA_USER_INFO_0 struct
    wkui0_username dd ?
WKSTA_USER_INFO_0 ends

WKSTA_USER_INFO_1 struct
    wkui1_username dd ?
    wkui1_logon_domain dd ?
    wkui1_oth_domains dd ?
    wkui1_logon_server dd ?
WKSTA_USER_INFO_1 ends

WKSTA_USER_INFO_1101 struct
     wkui1101_oth_domains dd ?
WKSTA_USER_INFO_1101 ends

WKSTA_TRANSPORT_INFO_0 struct
    wkti0_quality_of_service dd ?
    wkti0_number_of_vcs dd ?
    wkti0_transport_name dd ?
    wkti0_transport_address dd ?
    wkti0_wan_ish dd ?
WKSTA_TRANSPORT_INFO_0 ends


USER_INFO_0 struct
    usri0_name dd ?
USER_INFO_0 ends

USER_INFO_1 struct
    usri1_name dd ?
    usri1_password dd ?
    usri1_password_age dd ?
    usri1_priv dd ?
    usri1_home_dir dd ?
    usri1_comment dd ?
    usri1_flags dd ?
    usri1_script_path dd ?
USER_INFO_1 ends

USER_INFO_2 struct
    usri2_name dd ?
    usri2_password dd ?
    usri2_password_age dd ?
    usri2_priv dd ?
    usri2_home_dir dd ?
    usri2_comment dd ?
    usri2_flags dd ?
    usri2_script_path dd ?
    usri2_auth_flags dd ?
    usri2_full_name dd ?
    usri2_usr_comment dd ?
    usri2_parms dd ?
    usri2_workstations dd ?
    usri2_last_logon dd ?
    usri2_last_logoff dd ?
    usri2_acct_expires dd ?
    usri2_max_storage dd ?
    usri2_units_per_week dd ?
    usri2_logon_hours dd ?
    usri2_bad_pw_count dd ?
    usri2_num_logons dd ?
    usri2_logon_server dd ?
    usri2_country_code dd ?
    usri2_code_page dd ?
USER_INFO_2 ends

USER_INFO_3 struct
    usri3_name dd ?
    usri3_password dd ?
    usri3_password_age dd ?
    usri3_priv dd ?
    usri3_home_dir dd ?
    usri3_comment dd ?
    usri3_flags dd ?
    usri3_script_path dd ?
    usri3_auth_flags dd ?
    usri3_full_name dd ?
    usri3_usr_comment dd ?
    usri3_parms dd ?
    usri3_workstations dd ?
    usri3_last_logon dd ?
    usri3_last_logoff dd ?
    usri3_acct_expires dd ?
    usri3_max_storage dd ?
    usri3_units_per_week dd ?
    usri3_logon_hours dd ?
    usri3_bad_pw_count dd ?
    usri3_num_logons dd ?
    usri3_logon_server dd ?
    usri3_country_code dd ?
    usri3_code_page dd ?
    usri3_user_id dd ?
    usri3_primary_group_id dd ?
    usri3_profile dd ?
    usri3_home_dir_drive dd ?
    usri3_password_expired dd ?
USER_INFO_3 ends

USER_INFO_10 struct
    usri10_name dd ?
    usri10_comment dd ?
    usri10_usr_comment dd ?
    usri10_full_name dd ?
USER_INFO_10 ends

USER_INFO_11 struct
    usri11_name dd ?
    usri11_comment dd ?
    usri11_usr_comment dd ?
    usri11_full_name dd ?
    usri11_priv dd ?
    usri11_auth_flags dd ?
    usri11_password_age dd ?
    usri11_home_dir dd ?
    usri11_parms dd ?
    usri11_last_logon dd ?
    usri11_last_logoff dd ?
    usri11_bad_pw_count dd ?
    usri11_num_logons dd ?
    usri11_logon_server dd ?
    usri11_country_code dd ?
    usri11_workstations dd ?
    usri11_max_storage dd ?
    usri11_units_per_week dd ?
    usri11_logon_hours dd ?
    usri11_code_page dd ?
USER_INFO_11 ends

USER_INFO_20 struct
    usri20_name dd ?
    usri20_full_name dd ?
    usri20_comment dd ?
    usri20_flags dd ?
    usri20_user_id dd ?
USER_INFO_20 ends

USER_INFO_21 struct
    usri21_password db ENCRYPTED_PWLEN dup(?)
USER_INFO_21 ends

USER_INFO_22 struct
    usri22_name dd ?
    usri22_password db ENCRYPTED_PWLEN dup(?)
    usri22_password_age dd ?
    usri22_priv dd ?
    usri22_home_dir dd ?
    usri22_comment dd ?
    usri22_flags dd ?
    usri22_script_path dd ?
    usri22_auth_flags dd ?
    usri22_full_name dd ?
    usri22_usr_comment dd ?
    usri22_parms dd ?
    usri22_workstations dd ?
    usri22_last_logon dd ?
    usri22_last_logoff dd ?
    usri22_acct_expires dd ?
    usri22_max_storage dd ?
    usri22_units_per_week dd ?
    usri22_logon_hours dd ?
    usri22_bad_pw_count dd ?
    usri22_num_logons dd ?
    usri22_logon_server dd ?
    usri22_country_code dd ?
    usri22_code_page dd ?
USER_INFO_22 ends

USER_INFO_1003 struct
     usri1003_password dd ?
USER_INFO_1003 ends

USER_INFO_1005 struct
     usri1005_priv dd ?
USER_INFO_1005 ends

USER_INFO_1006 struct
     usri1006_home_dir dd ?
USER_INFO_1006 ends

USER_INFO_1007 struct
     usri1007_comment dd ?
USER_INFO_1007 ends

USER_INFO_1008 struct
     usri1008_flags dd ?
USER_INFO_1008 ends

USER_INFO_1009 struct
     usri1009_script_path dd ?
USER_INFO_1009 ends

USER_INFO_1010 struct
     usri1010_auth_flags dd ?
USER_INFO_1010 ends

USER_INFO_1011 struct
     usri1011_full_name dd ?
USER_INFO_1011 ends

USER_INFO_1012 struct
     usri1012_usr_comment dd ?
USER_INFO_1012 ends

USER_INFO_1013 struct
     usri1013_parms dd ?
USER_INFO_1013 ends

USER_INFO_1014 struct
     usri1014_workstations dd ?
USER_INFO_1014 ends

USER_INFO_1017 struct
     usri1017_acct_expires dd ?
USER_INFO_1017 ends

USER_INFO_1018 struct
     usri1018_max_storage dd ?
USER_INFO_1018 ends

USER_INFO_1020 struct
    usri1020_units_per_week dd ?
    usri1020_logon_hours dd ?
USER_INFO_1020 ends

USER_INFO_1023 struct
     usri1023_logon_server dd ?
USER_INFO_1023 ends

USER_INFO_1024 struct
     usri1024_country_code dd ?
USER_INFO_1024 ends

USER_INFO_1025 struct
     usri1025_code_page dd ?
USER_INFO_1025 ends

USER_INFO_1051 struct
     usri1051_primary_group_id dd ?
USER_INFO_1051 ends

USER_INFO_1052 struct
     usri1052_profile dd ?
USER_INFO_1052 ends

USER_INFO_1053 struct
     usri1053_home_dir_drive dd ?
USER_INFO_1053 ends

USER_MODALS_INFO_0 struct
    usrmod0_min_passwd_len dd ?
    usrmod0_max_passwd_age dd ?
    usrmod0_min_passwd_age dd ?
    usrmod0_force_logoff dd ?
    usrmod0_password_hist_len dd ?
USER_MODALS_INFO_0 ends

USER_MODALS_INFO_1 struct
    usrmod1_role dd ?
    usrmod1_primary dd ?
USER_MODALS_INFO_1 ends

USER_MODALS_INFO_2 struct
    usrmod2_domain_name dd ?
    usrmod2_domain_id dd ?
USER_MODALS_INFO_2 ends

USER_MODALS_INFO_3 struct
    usrmod3_lockout_duration dd ?
    usrmod3_lockout_observation_window dd ?
    usrmod3_lockout_threshold dd ?
USER_MODALS_INFO_3 ends

USER_MODALS_INFO_1001 struct
     usrmod1001_min_passwd_len dd ?
USER_MODALS_INFO_1001 ends

USER_MODALS_INFO_1002 struct
     usrmod1002_max_passwd_age dd ?
USER_MODALS_INFO_1002 ends

USER_MODALS_INFO_1003 struct
     usrmod1003_min_passwd_age dd ?
USER_MODALS_INFO_1003 ends

USER_MODALS_INFO_1004 struct
     usrmod1004_force_logoff dd ?
USER_MODALS_INFO_1004 ends

USER_MODALS_INFO_1005 struct
    usrmod1005_password_hist_len dd ?
USER_MODALS_INFO_1005 ends

USER_MODALS_INFO_1006 struct
     usrmod1006_role dd ?
USER_MODALS_INFO_1006 ends

USER_MODALS_INFO_1007 struct
     usrmod1007_primary dd ?
USER_MODALS_INFO_1007 ends

GROUP_INFO_0 struct
    grpi0_name dd ?
GROUP_INFO_0 ends

GROUP_INFO_1 struct
    grpi1_name dd ?
    grpi1_comment dd ?
GROUP_INFO_1 ends

GROUP_INFO_2 struct
    grpi2_name dd ?
    grpi2_comment dd ?
    grpi2_group_id dd ?
    grpi2_attributes dd ?
GROUP_INFO_2 ends

GROUP_INFO_1002 struct
     grpi1002_comment dd ?
GROUP_INFO_1002 ends

GROUP_INFO_1005 struct
     grpi1005_attributes dd ?
GROUP_INFO_1005 ends

GROUP_USERS_INFO_0 struct
     grui0_name dd ?
GROUP_USERS_INFO_0 ends

GROUP_USERS_INFO_1 struct
     grui1_name dd ?
     grui1_attributes dd ?
GROUP_USERS_INFO_1 ends

LOCALGROUP_INFO_0 struct
    lgrpi0_name dd ?
LOCALGROUP_INFO_0 ends

LOCALGROUP_INFO_1 struct
    lgrpi1_name dd ?
    lgrpi1_comment dd ?
LOCALGROUP_INFO_1 ends

LOCALGROUP_INFO_1002 struct
     lgrpi1002_comment dd ?
LOCALGROUP_INFO_1002 ends

LOCALGROUP_MEMBERS_INFO_0 struct
     lgrmi0_sid dd ?
LOCALGROUP_MEMBERS_INFO_0 ends

LOCALGROUP_MEMBERS_INFO_1 struct
     lgrmi1_sid dd ?
     lgrmi1_sidusage dd ?
     lgrmi1_name dd ?
LOCALGROUP_MEMBERS_INFO_1 ends

LOCALGROUP_MEMBERS_INFO_2 struct
     lgrmi2_sid dd ?
     lgrmi2_sidusage dd ?
     lgrmi2_domainandname dd ?
LOCALGROUP_MEMBERS_INFO_2 ends

LOCALGROUP_MEMBERS_INFO_3 struct
     lgrmi3_domainandname dd ?
LOCALGROUP_MEMBERS_INFO_3 ends

LOCALGROUP_USERS_INFO_0 struct
     lgrui0_name dd ?
LOCALGROUP_USERS_INFO_0 ends

NET_DISPLAY_USER struct
    usri1_name dd ?
    usri1_comment dd ?
    usri1_flags dd ?
    usri1_full_name dd ?
    usri1_user_id dd ?
    usri1_next_index dd ?
NET_DISPLAY_USER ends

NET_DISPLAY_MACHINE struct
    usri2_name dd ?
    usri2_comment dd ?
    usri2_flags dd ?
    usri2_user_id dd ?
    usri2_next_index dd ?
NET_DISPLAY_MACHINE ends

NET_DISPLAY_GROUP struct
    grpi3_name dd ?
    grpi3_comment dd ?
    grpi3_group_id dd ?
    grpi3_attributes dd ?
    grpi3_next_index dd ?
NET_DISPLAY_GROUP ends

ACCESS_INFO_0 struct
    acc0_resource_name dd ?
ACCESS_INFO_0 ends

ACCESS_INFO_1 struct
    acc1_resource_name dd ?
    acc1_attr dd ?
    acc1_count dd ?
ACCESS_INFO_1 ends

ACCESS_INFO_1002 struct
     acc1002_attr dd ?
ACCESS_INFO_1002 ends


ACCESS_LIST struct
    acl_ugname dd ?
    acl_access dd ?
ACCESS_LIST ends

NETLOGON_INFO_1 struct
        netlog1_flags dd ?
        netlog1_pdc_connection_status NET_API_STATUS ?
NETLOGON_INFO_1 ends

NETLOGON_INFO_2 struct
        netlog2_flags dd ?
        netlog2_pdc_connection_status NET_API_STATUS ?
        netlog2_trusted_dc_name dd ?
        netlog2_tc_connection_status NET_API_STATUS ?
NETLOGON_INFO_2 ends

NETLOGON_INFO_3 struct
        netlog3_flags dd ?
        netlog3_logon_attempts dd ?
        netlog3_reserved1 dd ?
        netlog3_reserved2 dd ?
        netlog3_reserved3 dd ?
        netlog3_reserved4 dd ?
        netlog3_reserved5 dd ?
NETLOGON_INFO_3 ends

NETLOGON_INFO_4 struct
        netlog4_trusted_dc_name dd ?
        netlog4_trusted_domain_name dd ?
NETLOGON_INFO_4 ends


CHARFORMAT2 STRUCT
  cbSize            DWORD      ?
  dwMask            DWORD      ?
  dwEffects         DWORD      ?
  yHeight           DWORD      ?
  yOffset           DWORD      ?
  crTextColor       DWORD      ?
  bCharSet          BYTE       ?
  bPitchAndFamily   BYTE       ?
  szFaceName        BYTE LF_FACESIZE dup(?)
  _wPad2            WORD       ?
  wWeight           WORD       ?
  sSpacing          WORD       ?
  crBackColor       DWORD      ?
  lcid              DWORD      ?
  dwReserved        DWORD      ?
  sStyle            WORD       ?
  wKerning          WORD       ?
  bUnderlineType    BYTE       ?
  bAnimation        BYTE       ? 
  bRevAuthor        BYTE       ?
  bReserved1        BYTE       ?
CHARFORMAT2 ENDS


STD_ALERT struct
    alrt_timestamp dd ?
    alrt_eventname WCHAR  [EVLEN + 1] dup(?)
    alrt_servicename WCHAR [SNLEN + 1] dup(?)
STD_ALERT ends

ADMIN_OTHER_INFO struct
    alrtad_errcode dd ?
    alrtad_numstrings dd ?
ADMIN_OTHER_INFO ends

ERRLOG_OTHER_INFO struct
    alrter_errcode dd ?
    alrter_offset dd ?
ERRLOG_OTHER_INFO ends

PRINT_OTHER_INFO struct
    alrtpr_jobid dd ?
    alrtpr_status dd ?
    alrtpr_submitted dd ?
    alrtpr_size dd ?
PRINT_OTHER_INFO ends

USER_OTHER_INFO struct
    alrtus_errcode dd ?
    alrtus_numstrings dd ?
USER_OTHER_INFO ends

AT_INFO struct
    JobTime dd ?
    DaysOfMonth dd ?
    DaysOfWeek UCHAR ?
    Flags UCHAR ?
    Command dd ?
AT_INFO ends

AT_ENUM struct
    JobId dd ?
    JobTime dd ?
    DaysOfMonth dd ?
    DaysOfWeek UCHAR ?
    Flags UCHAR ?
    Command dd ?
AT_ENUM ends


HLOG struct
     time dd ?
     last_flags dd ?
     offset1 dd ?
     rec_offset dd ?
HLOG ends

AUDIT_ENTRY struct
     ae_len dd ?
     ae_reserved dd ?
     ae_time dd ?
     ae_type dd ?
     ae_data_offset dd ?
     ae_data_size dd ?
AUDIT_ENTRY ends

AE_SRVSTATUS struct
     ae_sv_status dd ?
AE_SRVSTATUS ends

AE_SESSLOGON struct
     ae_so_compname dd ?
     ae_so_username dd ?
     ae_so_privilege dd ?
AE_SESSLOGON ends

AE_SESSLOGOFF struct
     ae_sf_compname dd ?
     ae_sf_username dd ?
     ae_sf_reason dd ?
AE_SESSLOGOFF ends

AE_SESSPWERR struct
     ae_sp_compname dd ?
     ae_sp_username dd ?
AE_SESSPWERR ends

AE_CONNSTART struct
     ae_ct_compname dd ?
     ae_ct_username dd ?
     ae_ct_netname dd ?
     ae_ct_connid dd ?
AE_CONNSTART ends

AE_CONNSTOP struct
     ae_cp_compname dd ?
     ae_cp_username dd ?
     ae_cp_netname dd ?
     ae_cp_connid dd ?
     ae_cp_reason dd ?
AE_CONNSTOP ends

AE_CONNREJ struct
     ae_cr_compname dd ?
     ae_cr_username dd ?
     ae_cr_netname dd ?
     ae_cr_reason dd ?
AE_CONNREJ ends

AE_RESACCESS struct
     ae_ra_compname dd ?
     ae_ra_username dd ?
     ae_ra_resname dd ?
     ae_ra_operation dd ?
     ae_ra_returncode dd ?
     ae_ra_restype dd ?
     ae_ra_fileid dd ?
AE_RESACCESS ends

AE_RESACCESSREJ struct
     ae_rr_compname dd ?
     ae_rr_username dd ?
     ae_rr_resname dd ?
     ae_rr_operation dd ?
AE_RESACCESSREJ ends

AE_CLOSEFILE struct
     ae_cf_compname dd ?
     ae_cf_username dd ?
     ae_cf_resname dd ?
     ae_cf_fileid dd ?
     ae_cf_duration dd ?
     ae_cf_reason dd ?
AE_CLOSEFILE ends

AE_SERVICESTAT struct
     ae_ss_compname dd ?
     ae_ss_username dd ?
     ae_ss_svcname dd ?
     ae_ss_status dd ?
     ae_ss_code dd ?
     ae_ss_text dd ?
     ae_ss_returnval dd ?
AE_SERVICESTAT ends

AE_ACLMOD struct
     ae_am_compname dd ?
     ae_am_username dd ?
     ae_am_resname dd ?
     ae_am_action dd ?
     ae_am_datalen dd ?
AE_ACLMOD ends

AE_UASMOD struct
     ae_um_compname dd ?
     ae_um_username dd ?
     ae_um_resname dd ?
     ae_um_rectype dd ?
     ae_um_action dd ?
     ae_um_datalen dd ?
AE_UASMOD ends

AE_NETLOGON struct
     ae_no_compname dd ?
     ae_no_username dd ?
     ae_no_privilege dd ?
     ae_no_authflags dd ?
AE_NETLOGON ends

AE_NETLOGOFF struct
     ae_nf_compname dd ?
     ae_nf_username dd ?
     ae_nf_reserved1 dd ?
     ae_nf_reserved2 dd ?
AE_NETLOGOFF ends

AE_ACCLIM struct
     ae_al_compname dd ?
     ae_al_username dd ?
     ae_al_resname dd ?
     ae_al_limit dd ?
AE_ACCLIM ends

AE_LOCKOUT struct
    ae_lk_compname dd ?
    ae_lk_username dd ?
    ae_lk_action dd ?
    ae_lk_bad_pw_count dd ?
AE_LOCKOUT ends

AE_GENERIC struct
     ae_ge_msgfile dd ?
     ae_ge_msgnum dd ?
     ae_ge_params dd ?
     ae_ge_param1 dd ?
     ae_ge_param2 dd ?
     ae_ge_param3 dd ?
     ae_ge_param4 dd ?
     ae_ge_param5 dd ?
     ae_ge_param6 dd ?
     ae_ge_param7 dd ?
     ae_ge_param8 dd ?
     ae_ge_param9 dd ?
AE_GENERIC ends


BROWSER_STATISTICS struct
    StatisticsStartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfServerEnumerations dd ?
    NumberOfDomainEnumerations dd ?
    NumberOfOtherEnumerations dd ?
    NumberOfMissedServerAnnouncements dd ?
    NumberOfMissedMailslotDatagrams dd ?
    NumberOfMissedGetBrowserServerListRequests dd ?
    NumberOfFailedServerAnnounceAllocations dd ?
    NumberOfFailedMailslotAllocations dd ?
    NumberOfFailedMailslotReceives dd ?
    NumberOfFailedMailslotWrites dd ?
    NumberOfFailedMailslotOpens dd ?
    NumberOfDuplicateMasterAnnouncements dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
BROWSER_STATISTICS ends

BROWSER_STATISTICS_100 struct
    StartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
BROWSER_STATISTICS_100 ends

BROWSER_STATISTICS_101 struct
    StartTime LARGE_INTEGER <>
    NumberOfServerAnnouncements LARGE_INTEGER <>
    NumberOfDomainAnnouncements LARGE_INTEGER <>
    NumberOfElectionPackets dd ?
    NumberOfMailslotWrites dd ?
    NumberOfGetBrowserServerListRequests dd ?
    NumberOfIllegalDatagrams LARGE_INTEGER <>
    NumberOfMissedServerAnnouncements dd ?
    NumberOfMissedMailslotDatagrams dd ?
    NumberOfMissedGetBrowserServerListRequests dd ?
    NumberOfFailedServerAnnounceAllocations dd ?
    NumberOfFailedMailslotAllocations dd ?
    NumberOfFailedMailslotReceives dd ?
    NumberOfFailedMailslotWrites dd ?
    NumberOfFailedMailslotOpens dd ?
    NumberOfDuplicateMasterAnnouncements dd ?
BROWSER_STATISTICS_101 ends


BROWSER_EMULATED_DOMAIN struct
    DomainName dd ?
    EmulatedServerName dd ?
    Role dd ?
BROWSER_EMULATED_DOMAIN ends

CHARDEV_INFO_0 struct
    ch0_dev dd ?
CHARDEV_INFO_0 ends

CHARDEV_INFO_1 struct
    ch1_dev dd ?
    ch1_status dd ?
    ch1_username dd ?
    ch1_time dd ?
CHARDEV_INFO_1 ends

CHARDEVQ_INFO_0 struct
    cq0_dev dd ?
CHARDEVQ_INFO_0 ends

CHARDEVQ_INFO_1 struct
    cq1_dev dd ?
    cq1_priority dd ?
    cq1_devs dd ?
    cq1_numusers dd ?
    cq1_numahead dd ?
CHARDEVQ_INFO_1 ends

CHARDEVQ_INFO_1002 struct
    cq1002_priority dd ?
CHARDEVQ_INFO_1002 ends

CHARDEVQ_INFO_1003 struct
    cq1003_devs dd ?
CHARDEVQ_INFO_1003 ends

HANDLE_INFO_1 struct
    hdli1_chartime dd ?
    hdli1_charcount dd ?
HANDLE_INFO_1 ends

CONFIG_INFO_0 struct
     cfgi0_key dd ?
     cfgi0_data dd ?
CONFIG_INFO_0 ends

DFS_INFO_1 struct
    EntryPath dd ?
DFS_INFO_1 ends

DFS_INFO_2 struct
    EntryPath dd ?
    Comment dd ?
    State dd ?
    NumberOfStorages dd ?
DFS_INFO_2 ends

DFS_STORAGE_INFO struct
    State dd ?
    ServerName dd ?
    ShareName dd ?
DFS_STORAGE_INFO ends

DFS_INFO_3 struct
    EntryPath dd ?
    Comment dd ?
    State dd ?
    NumberOfStorages dd ?
    Storage dd ?
DFS_INFO_3 ends

DFS_INFO_100 struct
    Comment dd ?
DFS_INFO_100 ends

DFS_INFO_101 struct
    State dd ?
DFS_INFO_101 ends

ERROR_LOG struct
     el_len dd ?
     el_reserved dd ?
     el_time dd ?
     el_error dd ?
     el_name dd ?
     el_text dd ?
     el_data dd ?
     el_data_size dd ?
     el_nstrings dd ?
ERROR_LOG ends

MSG_INFO_0 struct
    msgi0_name dd ?
MSG_INFO_0 ends

MSG_INFO_1 struct
    msgi1_name dd ?
    msgi1_forward_flag dd ?
    msgi1_forward dd ?
MSG_INFO_1 ends

TIME_OF_DAY_INFO struct
    tod_elapsedt dd ?
    tod_msecs dd ?
    tod_hours dd ?
    tod_mins dd ?
    tod_secs dd ?
    tod_hunds dd ?
    tod_timezone dd ?
    tod_tinterval dd ?
    tod_day dd ?
    tod_month dd ?
    tod_year dd ?
    tod_weekday dd ?
TIME_OF_DAY_INFO ends


REPL_INFO_0 struct
    rp0_role dd ?
    rp0_exportpath dd ?
    rp0_exportlist dd ?
    rp0_importpath dd ?
    rp0_importlist dd ?
    rp0_logonusername dd ?
    rp0_interval dd ?
    rp0_pulse dd ?
    rp0_guardtime dd ?
    rp0_random dd ?
REPL_INFO_0 ends

REPL_INFO_1000 struct
    rp1000_interval dd ?
REPL_INFO_1000 ends

REPL_INFO_1001 struct
    rp1001_pulse dd ?
REPL_INFO_1001 ends

REPL_INFO_1002 struct
    rp1002_guardtime dd ?
REPL_INFO_1002 ends

REPL_INFO_1003 struct
    rp1003_random dd ?
REPL_INFO_1003 ends

REPL_EDIR_INFO_0 struct
    rped0_dirname dd ?
REPL_EDIR_INFO_0 ends

REPL_EDIR_INFO_1 struct
    rped1_dirname dd ?
    rped1_integrity dd ?
    rped1_extent dd ?
REPL_EDIR_INFO_1 ends

REPL_EDIR_INFO_2 struct
    rped2_dirname dd ?
    rped2_integrity dd ?
    rped2_extent dd ?
    rped2_lockcount dd ?
    rped2_locktime dd ?
REPL_EDIR_INFO_2 ends

REPL_EDIR_INFO_1000 struct
    rped1000_integrity dd ?
REPL_EDIR_INFO_1000 ends

REPL_EDIR_INFO_1001 struct
    rped1001_extent dd ?
REPL_EDIR_INFO_1001 ends

REPL_IDIR_INFO_0 struct 
    rpid0_dirname dd ?
REPL_IDIR_INFO_0 ends

REPL_IDIR_INFO_1 struct
    rpid1_dirname dd ?
    rpid1_state dd ?
    rpid1_mastername dd ?
    rpid1_last_update_time dd ?
    rpid1_lockcount dd ?
    rpid1_locktime dd ?
REPL_IDIR_INFO_1 ends


SHARE_INFO_0 struct
    shi0_netname dd ?
SHARE_INFO_0 ends

SHARE_INFO_1 struct
    shi1_netname dd ?
    shi1_type dd ?
    shi1_remark dd ?
SHARE_INFO_1 ends

SHARE_INFO_2 struct
    shi2_netname dd ?
    shi2_type dd ?
    shi2_remark dd ?
    shi2_permissions dd ?
    shi2_max_uses dd ?
    shi2_current_uses dd ?
    shi2_path dd ?
    shi2_passwd dd ?
SHARE_INFO_2 ends

SHARE_INFO_501 struct
    shi501_netname dd ?
    shi501_type dd ?
    shi501_remark dd ?
    shi501_flags dd ?
SHARE_INFO_501 ends

SHARE_INFO_502 struct
    shi502_netname dd ?
    shi502_type dd ?
    shi502_remark dd ?
    shi502_permissions dd ?
    shi502_max_uses dd ?
    shi502_current_uses dd ?
    shi502_path dd ?
    shi502_passwd dd ?
    shi502_reserved dd ?
    shi502_security_descriptor dd ?
SHARE_INFO_502 ends

SHARE_INFO_1004 struct 
    shi1004_remark dd ?
SHARE_INFO_1004 ends

SHARE_INFO_1005 struct
    shi1005_flags dd ?
SHARE_INFO_1005 ends

SHARE_INFO_1006 struct
    shi1006_max_uses dd ?
SHARE_INFO_1006 ends

SHARE_INFO_1007 struct
    shi1007_flags dd ?
    shi1007_AlternateDirectoryName dd ?
SHARE_INFO_1007 ends

SHARE_INFO_1501 struct
    shi1501_reserved dd ?
    shi1501_security_descriptor dd ?
SHARE_INFO_1501 ends

SESSION_INFO_0 struct
    sesi0_cname dd ?
SESSION_INFO_0 ends

SESSION_INFO_1 struct
    sesi1_cname dd ?
    sesi1_username dd ?
    sesi1_num_opens dd ?
    sesi1_time dd ?
    sesi1_idle_time dd ?
    sesi1_user_flags dd ?
SESSION_INFO_1 ends

SESSION_INFO_2 struct
    sesi2_cname dd ?
    sesi2_username dd ?
    sesi2_num_opens dd ?
    sesi2_time dd ?
    sesi2_idle_time dd ?
    sesi2_user_flags dd ?
    sesi2_cltype_name dd ?
SESSION_INFO_2 ends

SESSION_INFO_10 struct
    sesi10_cname dd ?
    sesi10_username dd ?
    sesi10_time dd ?
    sesi10_idle_time dd ?
SESSION_INFO_10 ends

SESSION_INFO_502 struct
    sesi502_cname dd ?
    sesi502_username dd ?
    sesi502_num_opens dd ?
    sesi502_time dd ?
    sesi502_idle_time dd ?
    sesi502_user_flags dd ?
    sesi502_cltype_name dd ?
    sesi502_transport dd ?
SESSION_INFO_502 ends

CONNECTION_INFO_0 struct
    coni0_id dd ?
CONNECTION_INFO_0 ends

CONNECTION_INFO_1 struct
    coni1_id dd ?
    coni1_type dd ?
    coni1_num_opens dd ?
    coni1_num_users dd ?
    coni1_time dd ?
    coni1_username dd ?
    coni1_netname dd ?
CONNECTION_INFO_1 ends

FILE_INFO_2 struct
    fi2_id dd ?
FILE_INFO_2 ends

FILE_INFO_3 struct
    fi3_id dd ?
    fi3_permissions dd ?
    fi3_num_locks dd ?
    fi3_pathname dd ?
    fi3_username dd ?
FILE_INFO_3 ends

STAT_WORKSTATION_0 struct
     stw0_start dd ?
     stw0_numNCB_r dd ?
     stw0_numNCB_s dd ?
     stw0_numNCB_a dd ?
     stw0_fiNCB_r dd ?
     stw0_fiNCB_s dd ?
     stw0_fiNCB_a dd ?
     stw0_fcNCB_r dd ?
     stw0_fcNCB_s dd ?
     stw0_fcNCB_a dd ?
     stw0_sesstart dd ?
     stw0_sessfailcon dd ?
     stw0_sessbroke dd ?
     stw0_uses dd ?
     stw0_usefail dd ?
     stw0_autorec dd ?
     stw0_bytessent_r_lo dd ?
     stw0_bytessent_r_hi dd ?
     stw0_bytesrcvd_r_lo dd ?
     stw0_bytesrcvd_r_hi dd ?
     stw0_bytessent_s_lo dd ?
     stw0_bytessent_s_hi dd ?
     stw0_bytesrcvd_s_lo dd ?
     stw0_bytesrcvd_s_hi dd ?
     stw0_bytessent_a_lo dd ?
     stw0_bytessent_a_hi dd ?
     stw0_bytesrcvd_a_lo dd ?
     stw0_bytesrcvd_a_hi dd ?
     stw0_reqbufneed dd ?
     stw0_bigbufneed dd ?
STAT_WORKSTATION_0 ends

STAT_SERVER_0 struct
     sts0_start dd ?
     sts0_fopens dd ?
     sts0_devopens dd ?
     sts0_jobsqueued dd ?
     sts0_sopens dd ?
     sts0_stimedout dd ?
     sts0_serrorout dd ?
     sts0_pwerrors dd ?
     sts0_permerrors dd ?
     sts0_syserrors dd ?
     sts0_bytessent_low dd ?
     sts0_bytessent_high dd ?
     sts0_bytesrcvd_low dd ?
     sts0_bytesrcvd_high dd ?
     sts0_avresponse dd ?
     sts0_reqbufneed dd ?
     sts0_bigbufneed dd ?
STAT_SERVER_0 ends

SERVICE_INFO_0 struct
    svci0_name dd ?
SERVICE_INFO_0 ends

SERVICE_INFO_1 struct
    svci1_name dd ?
    svci1_status dd ?
    svci1_code dd ?
    svci1_pid dd ?
SERVICE_INFO_1 ends

SERVICE_INFO_2 struct
    svci2_name dd ?
    svci2_status dd ?
    svci2_code dd ?
    svci2_pid dd ?
    svci2_text dd ?
    svci2_specific_error dd ?
    svci2_display_name dd ?
SERVICE_INFO_2 ends

USE_INFO_0 struct
    ui0_local dd ?
    ui0_remote dd ?
USE_INFO_0 ends

USE_INFO_1 struct
    ui1_local dd ?
    ui1_remote dd ?
    ui1_password dd ?
    ui1_status dd ?
    ui1_asg_type dd ?
    ui1_refcount dd ?
    ui1_usecount dd ?
USE_INFO_1 ends

USE_INFO_2 struct
    ui2_local dd ?
    ui2_remote dd ?
    ui2_password dd ?
    ui2_status dd ?
    ui2_asg_type dd ?
    ui2_refcount dd ?
    ui2_usecount dd ?
    ui2_username dd ?
    ui2_domainname dd ?
USE_INFO_2 ends

USE_INFO_3 struct
    ui3_ui2 USE_INFO_2 <>
    ui3_flags dd ?
USE_INFO_3 ends

STRRET STRUCT
 uType      dd ?
 union                        
    pOleStr    dd ?
    pStr       dd ?
    uOffset    dd ?
    union 
	    cStr       db MAX_PATH dup(?)
	    cstr       db MAX_PATH dup(?)
    ends
 ends
STRRET ENDS

GETTEXTEX STRUCT
  cb             DWORD      ? ;DWORD  
  flags          DWORD      ? ;DWORD  
  codepage       DWORD      ? ;UINT
  lpDefaultChar  DWORD      ? ;LPCSTR 
  lpUsedDefChar  DWORD      ? ;LPBOOL 
GETTEXTEX ENDS

SETTEXTEX STRUCT
  flags          DWORD      ?
  codepage       DWORD      ?
SETTEXTEX ENDS

GETTEXTLENGTHEX STRUCT
  flags          DWORD      ?
  codepage       DWORD      ?
GETTEXTLENGTHEX ENDS

IID   STRUCT DWORD
    Data1        dd ?
    Data2        dw ?
    Data3        dw ?
    Data4        db 8 dup(?)
IID ENDS

COLORSCHEME struct
   dwSize dd ?
   clrBtnHighlight COLORREF ?
   clrBtnShadow COLORREF ?
COLORSCHEME ends

NMMOUSE struct
    hdr NMHDR <>
    dwItemSpec dd ?
    dwItemData dd ?
    pt POINT <>
    dwHitInfo dd ?
NMMOUSE ends

NMCLICK equ <NMMOUSE>

TRACKMOUSEEVENT struct
    cbSize dd ?
    dwFlags dd ?
    hwndTrack dd ?
    dwHoverTime dd ?
TRACKMOUSEEVENT ends

DLLVERSIONINFO struct
        cbSize dd ?
        dwMajorVersion dd ?
        dwMinorVersion dd ?
        dwBuildNumber dd ?
        dwPlatformID dd ?
DLLVERSIONINFO ends

WSANETWORKEVENTS STRUCT
    lNetworkEvents              dd  ?
    iErrorCode                  dd  FD_MAX_EVENTS dup (?)
WSANETWORKEVENTS ENDS


TBBUTTONINFOA STRUCT
    cbSize      DWORD ?
    dwMask      DWORD ?
    idCommand   DWORD ?
    iImage      DWORD ?
    fsState     BYTE ?
    fsStyle     BYTE ?
    lx          WORD ?
    lParam      DWORD ?
    pszText     DWORD ?
    cchText     DWORD ?
TBBUTTONINFOA ends

TBBUTTONINFO equ <TBBUTTONINFOA>

NMTOOLTIPSCREATED STRUCT
  hdr               NMHDR  <>
  hwndToolTips      DWORD  ?
NMTOOLTIPSCREATED ENDS
        
NMKEY STRUCT
  hdr               NMHDR  <>
  nVKey             DWORD  ?
  uFlags            DWORD  ?
NMKEY ENDS

NMBYTE STRUCT
  hdr               NMHDR  <>
  _ch               DWORD  ?
  dwItemPrev        DWORD  ?
  dwItemNext        DWORD  ?
NMBYTE ENDS

NMCUSTOMDRAW STRUCT
  hdr               NMHDR  <>
  dwDrawStage       DWORD  ?
  hdc               DWORD  ?
  rc                RECT  <>
  dwItemSpec        DWORD  ?
  uItemState        DWORD  ?
  lItemlParam       DWORD  ?
NMCUSTOMDRAW ENDS

NMTTCUSTOMDRAW STRUCT
  nmcd              NMCUSTOMDRAW  <>
  uDrawFlags        DWORD  ?
NMTTCUSTOMDRAW ENDS

IMAGELISTDRADWORDS STRUCT
  cbSize            DWORD  ?
  himl              DWORD  ?
  i                 DWORD  ?
  hdcDst            DWORD  ?
  x                 DWORD  ?
  y                 DWORD  ?
  _cx               DWORD  ?
  cy                DWORD  ?
  xBitmap           DWORD  ?
  yBitmap           DWORD  ?
  rgbBk             DWORD  ?
  rgbFg             DWORD  ?
  fStyle            DWORD  ?
  dwRop             DWORD  ?
IMAGELISTDRADWORDS ENDS

HD_TEXTFILTERA STRUCT
  pszText           DWORD  ?
  cchTextMax        DWORD  ?
HD_TEXTFILTERA ENDS

TEXTFILTER  equ  <TEXTFILTERA>

HDITEMA STRUCT
  _mask             DWORD  ?
  cxy               DWORD  ?
  pszText           DWORD  ?
  hbm               DWORD  ?
  cchTextMax        DWORD  ?
  fmt               DWORD  ?
  lParam            DWORD  ?
  iImage            DWORD  ?
  iOrder            DWORD  ?
  _type             DWORD  ?
  pvFilter          DWORD  ?
HDITEMA ENDS

HDITEM equ <HDITEMA>

HDHITTESTINFO STRUCT
  pt                POINT  <>
  flags             DWORD  ?
  iItem             DWORD  ?
HDHITTESTINFO ENDS

NMHDDISPINFOA STRUCT
  hdr               NMHDR  <>
  iItem             DWORD  ?
  _mask             DWORD  ?
  pszText           DWORD  ?
  cchTextMax        DWORD  ?
  iImage            DWORD  ?
  lParam            DWORD  ?
NMHDDISPINFOA ENDS

NMHDDISPINFO equ <NMHDDISPINFOA>

NMHDFILTERBTNCLICK STRUCT
  hdr               NMHDR  <>
  iItem             DWORD  ?
  rc                RECT   <>
NMHDFILTERBTNCLICK ENDS

TBINSERTMARK STRUCT
  iButton           DWORD  ?
  dwFlags           DWORD  ?
TBINSERTMARK ENDS

NMTBHOTITEM STRUCT
  hdr               NMHDR  <>
  idOld             DWORD  ?
  idNew             DWORD  ?
  dwFlags           DWORD  ?
NMTBHOTITEM ENDS

NMTBSAVE STRUCT
  hdr               NMHDR  <>
  pData             DWORD  ?
  pCurrent          DWORD  ?
  cbData            DWORD  ?
  iItem             DWORD  ?
  cButtons          DWORD  ?
  tbButton          TBBUTTON  <>
NMTBSAVE ENDS

NMTBRESTORE STRUCT
  hdr               NMHDR  <>
  pData             DWORD  ?
  pCurrent          DWORD  ?
  cbData            DWORD  ?
  iItem             DWORD  ?
  cButtons          DWORD  ?
  cbBytesPerRecord  DWORD  ?
  tbButton          TBBUTTON  <>
NMTBRESTORE ENDS

NMTBGETINFOTIPA STRUCT
  hdr               NMHDR  <>
  pszText           DWORD  ?
  cchTextMax        DWORD  ?
  iItem             DWORD  ?
  lParam            DWORD  ?
NMTBGETINFOTIPA ENDS

NMTBGETINFOTIP equ <NMTBGETINFOTIPA>

NMTBDISPINFOA STRUCT
  hdr               NMHDR  <>
  dwMask            DWORD  ?
  idCommand         DWORD  ?
  lParam            DWORD  ?
  iImage            DWORD  ?
  pszText           DWORD  ?
  cchText           DWORD  ?
NMTBDISPINFOA ENDS

NMTBDISPINFO equ <NMTBDISPINFOA>

NMTOOLBARA STRUCT
  hdr               NMHDR  <>
  iItem             DWORD  ?
  tbButton          TBBUTTON  <>
  cchText           DWORD  ?
  pszText           DWORD  ?
  rcButton          RECT  <>
NMTOOLBARA ENDS

;;; NMTOOLBAR equ <NMTOOLBARA>

NMREBARCHEVRON STRUCT
  hdr               NMHDR  <>
  uBand             DWORD  ?
  wID               DWORD  ?
  lParam            DWORD  ?
  rc                RECT   <>
  lParamNM          DWORD  ?
NMREBARCHEVRON ENDS

TTTOOLINFOA STRUCT
  cbSize            DWORD  ?
  uFlags            DWORD  ?
  hwnd              DWORD  ?
  uId               DWORD  ?
  rect              RECT   <>
  hinst             DWORD  ?
  lpszText          DWORD  ?
  lParam            DWORD  ?
TTTOOLINFOA ENDS

TTTOOLINFO equ <TTTOOLINFOA>

TTHITTESTINFOA STRUCT
  hwnd              DWORD  ?
  pt                POINT  <>
  ti                TTTOOLINFOA  <>
TTHITTESTINFOA ENDS

TTHITTESTINFO equ <TTHITTESTINFOA>

NMTTDISPINFOA STRUCT
  hdr               NMHDR  <>
  lpszText          DWORD  ?
  szText            BYTE 80 dup(?)
  hinst             DWORD  ?
  uFlags            DWORD  ?
  lParam            DWORD  ?
NMTTDISPINFOA ENDS

;;; NMTTDISPINFO equ <NMTTDISPINFOA>

 ; NMUPDOWN STRUCT
 ;   hdr               NMHDR  <>
 ;   iPos              DWORD  ?
 ;   iDelta            DWORD  ?
 ; NMUPDOWN ENDS

PBRANGE STRUCT
  iLow              DWORD  ?
  iHigh             DWORD  ?
PBRANGE ENDS

LVBKIMAGEA STRUCT
  ulFlags           DWORD  ?
  hbm               DWORD  ?
  pszImage          DWORD  ?
  cchImageMax       DWORD  ?
  xOffsetPercent    DWORD  ?
  yOffsetPercent    DWORD  ?
LVBKIMAGEA ENDS

LVBKIMAGE equ <LVBKIMAGEA>

NMITEMACTIVATE STRUCT
  hdr               NMHDR  <>
  iItem             DWORD  ?
  iSubItem          DWORD  ?
  uNewState         DWORD  ?
  uOldState         DWORD  ?
  uChanged          DWORD  ?
  ptAction          POINT  <>
  lParam            DWORD  ?
  uKeyFlags         DWORD  ?
NMITEMACTIVATE ENDS

NMLVCUSTOMDRAW STRUCT
  nmcd              NMCUSTOMDRAW  <>
  clrText           DWORD  ?
  clrTextBk         DWORD  ?
  iSubItem          DWORD  ?
NMLVCUSTOMDRAW ENDS

NMLVCACHEHDWORD STRUCT
  hdr               NMHDR  <>
  iFrom             DWORD  ?
  iTo               DWORD  ?
NMLVCACHEHDWORD ENDS

 ; NMLVFINDITEMA STRUCT
 ;   hdr               NMHDR  <>
 ;   iStart            DWORD  ?
 ;   lvfi              LVFINDINFOA  <>
 ; NMLVFINDITEMA ENDS

NMLVFINDITEM equ <NMLVFINDITEMA>

NMLVODSTATECHANGE STRUCT
  hdr               NMHDR  <>
  iFrom             DWORD  ?
  iTo               DWORD  ?
  uNewState         DWORD  ?
  uOldState         DWORD  ?
NMLVODSTATECHANGE ENDS

 ; NMLVDISPINFOA STRUCT
 ;   hdr               NMHDR  <>
 ;   item              LVITEMA  <>
 ; NMLVDISPINFOA ENDS

NMLVDISPINFO equ <NMLVDISPINFOA>

NMLVGETINFOTIPA STRUCT
  hdr               NMHDR  <>
  dwFlags           DWORD  ?
  pszText           DWORD  ?
  cchTextMax        DWORD  ?
  iItem             DWORD  ?
  iSubItem          DWORD  ?
  lParam            DWORD  ?
NMLVGETINFOTIPA ENDS

NMLVGETINFOTIP equ <NMLVGETINFOTIPA>

NMTVCUSTOMDRAW STRUCT
  nmcd              NMCUSTOMDRAW  <>
  clrText           DWORD  ?
  clrTextBk         DWORD  ?
  iLevel            DWORD  ?
NMTVCUSTOMDRAW ENDS

NMTVGETINFOTIPA STRUCT
  hdr               NMHDR  <>
  pszText           DWORD  ?
  cchTextMax        DWORD  ?
  hItem             DWORD  ?
  lParam            DWORD  ?
NMTVGETINFOTIPA ENDS

NMTVGETINFOTIP equ <NMTVGETINFOTIPA>

COMBOBOXEXITEMA STRUCT
  _mask             DWORD  ?
  iItem             DWORD  ?
  pszText           DWORD  ?
  cchTextMax        DWORD  ?
  iImage            DWORD  ?
  iSelectedImage    DWORD  ?
  iOverlay          DWORD  ?
  iIndent           DWORD  ?
  lParam            DWORD  ?
COMBOBOXEXITEMA ENDS


COMBOBOXEXITEM equ <COMBOBOXEXITEMA>

NMCOMBOBOXEXA STRUCT
  hdr               NMHDR  <>
  ceItem            COMBOBOXEXITEMA  <>
NMCOMBOBOXEXA ENDS

NMCOMBOBOXEX equ <NMCOMBOBOXEXA>

MCHITTESTINFO STRUCT
  cbSize            DWORD  ?
  pt                POINT  <>
  uHit              DWORD  ?
  _st               SYSTEMTIME  <>
MCHITTESTINFO ENDS

NMSELCHANGE STRUCT
  nmhdr             NMHDR  <>
  stSelStart        SYSTEMTIME  <>
  stSelEnd          SYSTEMTIME  <>
NMSELCHANGE ENDS

 ; NMDAYSTATE STRUCT
 ;   nmhdr             NMHDR  <>
 ;   stStart           SYSTEMTIME  <>
 ;   cDayState         DWORD  ?
 ;   prgDayState       LPMONTHDAYSTATE  <>
 ; NMDAYSTATE ENDS

NMDATETIMECHANGE STRUCT
  nmhdr             NMHDR  <>
  dwFlags           DWORD  ?
  _st               SYSTEMTIME  <>
NMDATETIMECHANGE ENDS

NMDATETIMESTRINGA STRUCT
  nmhdr             NMHDR  <>
  pszUserString     DWORD  ?
  _st               SYSTEMTIME  <>
  dwFlags           DWORD  ?
NMDATETIMESTRINGA ENDS

NMDATETIMESTRING equ <NMDATETIMESTRINGA>

NMDATETIMEWMKEYDOWNA STRUCT
  nmhdr             NMHDR  <>
  nVirtKey          DWORD  ?
  pszFormat         DWORD  ?
  _st               SYSTEMTIME  <>
NMDATETIMEWMKEYDOWNA ENDS

NMDATETIMEWMKEYDOWN equ <NMDATETIMEWMKEYDOWNA >

NMDATETIMEFORMATA STRUCT
  nmhdr                 NMHDR  <>
  pszFormat             DWORD  ?
  _st                   SYSTEMTIME  <>
  pszDisplay            DWORD  ?
  szDisplay             BYTE 64 dup(?)
NMDATETIMEFORMATA ENDS

NMDATETIMEFORMAT equ <NMDATETIMEFORMATA>

 ; NMDATETIMEFORMATQUERYA STRUCT
 ;   nmhdr             NMHDR  <>
 ;   pszFormat         DWORD  ?
 ;   szMax             SIZE  <> ; naming conflict
 ; NMDATETIMEFORMATQUERYA ENDS

NMDWORDESS STRUCT
  hdr                   NMHDR  <>
  iField                DWORD  ?
  iValue                DWORD  ?
NMDWORDESS ENDS

NMPGSCROLL STRUCT
  hdr                   NMHDR  <>
  fwKeys                WORD  ?
  rcParent              RECT  <>
  iDir                  DWORD  ?
  iXpos                 DWORD  ?
  iYpos                 DWORD  ?
  iScroll               DWORD  ?
NMPGSCROLL ENDS

NMPGCALCSIZE STRUCT
  hdr               NMHDR  <>
  dwFlag            DWORD  ?
  iWidth            DWORD  ?
  iHeight           DWORD  ?
NMPGCALCSIZE ENDS

_GUID STRUCT
  Data1           DWORD ?
  Data2           WORD ?
  Data3           WORD ?
  Data4           BYTE 8 dup (?)
_GUID ENDS

QUOTA_LIMITS STRUCT
  PagedPoolLimit  DWORD <>
  NonPagedPoolLimit DWORD <>
  MinimumWorkingSetSize DWORD ?
  MaximumWorkingSetSize DWORD ?
  PagefileLimit   DWORD <>
  TimeLimit       LARGE_INTEGER <>
QUOTA_LIMITS ENDS

JOBOBJECT_BASIC_ACCOUNTING_INFORMATION STRUCT
  TotalUserTime   LARGE_INTEGER <>
  TotalKernelTime LARGE_INTEGER <>
  ThisPeriodTotalUserTime LARGE_INTEGER <>
  ThisPeriodTotalKernelTime LARGE_INTEGER <>
  TotalPageFaultCount DWORD ?
  TotalProcesses  DWORD ?
  ActiveProcesses DWORD ?
  TotalTerminatedProcesses DWORD ?
JOBOBJECT_BASIC_ACCOUNTING_INFORMATION ENDS

JOBOBJECT_BASIC_LIMIT_INFORMATION STRUCT
  PerProcessUserTimeLimit LARGE_INTEGER <>
  PerJobUserTimeLimit LARGE_INTEGER <>
  LimitFlags      DWORD ?
  MinimumWorkingSetSize DWORD ?
  MaximumWorkingSetSize DWORD ?
  ActiveProcessLimit DWORD ?
  Affinity        DWORD ?
  PriorityClass   DWORD ?
JOBOBJECT_BASIC_LIMIT_INFORMATION ENDS

JOBOBJECT_BASIC_PROCESS_ID_LIST STRUCT
  NumberOfAssignedProcesses DWORD ?
  NumberOfProcessIdsInList DWORD ?
  ProcessIdList   DWORD 1 dup (?)
JOBOBJECT_BASIC_PROCESS_ID_LIST ENDS

JOBOBJECT_BASIC_UI_RESTRICTIONS STRUCT
  UIRestrictionsClass DWORD ?
JOBOBJECT_BASIC_UI_RESTRICTIONS ENDS

JOBOBJECT_END_OF_JOB_TIME_INFORMATION STRUCT
  EndOfJobTimeAction DWORD ?
JOBOBJECT_END_OF_JOB_TIME_INFORMATION ENDS

JOBOBJECT_ASSOCIATE_COMPLETION_PORT STRUCT
  CompletionKey   DWORD ?
  CompletionPort  DWORD ?
JOBOBJECT_ASSOCIATE_COMPLETION_PORT ENDS

IMAGE_OS2_HEADER STRUCT
  ne_magic        WORD ?
  ne_ver          BYTE ?
  ne_rev          BYTE ?
  ne_enttab       WORD ?
  ne_cbenttab     WORD ?
  ne_crc          DWORD ?
  ne_flags        WORD ?
  ne_autodata     WORD ?
  ne_heap         WORD ?
  ne_stack        WORD ?
  ne_csip         DWORD ?
  ne_sssp         DWORD ?
  ne_cseg         WORD ?
  ne_cmod         WORD ?
  ne_cbnrestab    WORD ?
  ne_segtab       WORD ?
  ne_rsrctab      WORD ?
  ne_restab       WORD ?
  ne_modtab       WORD ?
  ne_imptab       WORD ?
  ne_nrestab      DWORD ?
  ne_cmovent      WORD ?
  ne_align        WORD ?
  ne_cres         WORD ?
  ne_exetyp       BYTE ?
  ne_flagsothers  BYTE ?
  ne_pretthunks   WORD ?
  ne_psegrefbytes WORD ?
  ne_swaparea     WORD ?
  ne_expver       WORD ?
IMAGE_OS2_HEADER ENDS

IMAGE_NT_HEADERS32 STRUCT
  Signature       DWORD ?
  FileHeader      IMAGE_FILE_HEADER <>
  OptionalHeader  IMAGE_OPTIONAL_HEADER32 <>
IMAGE_NT_HEADERS32 ENDS

IMAGE_STUB_DIRECTORY STRUCT
  SecondaryImportAddressTable DWORD ?
  ExpectedISA     WORD 2 dup (?)
  StubAddressTable DWORD 2 dup (?)
IMAGE_STUB_DIRECTORY ENDS

RECOVERY_AGENT_INFORMATIONA STRUCT
  NextEntryOffset DWORD ?
  AgentNameLength DWORD ?
  AgentInformation BYTE 1 dup (?)
RECOVERY_AGENT_INFORMATIONA ENDS

RECOVERY_AGENT_INFORMATION equ <RECOVERY_AGENT_INFORMATIONA>

WIN32_FILE_ATTRIBUTE_DATA STRUCT
  dwFileAttributes DWORD ?
  ftCreationTime  FILETIME <>
  ftLastAccessTime FILETIME <>
  ftLastWriteTime FILETIME <>
  nFileSizeHigh   DWORD ?
  nFileSizeLow    DWORD ?
WIN32_FILE_ATTRIBUTE_DATA ENDS

OSVERSIONINFOEXA STRUCT
  dwOSVersionInfoSize DWORD ?
  dwMajorVersion  DWORD ?
  dwMinorVersion  DWORD ?
  dwBuildNumber   DWORD ?
  dwPlatformId    DWORD ?
  szCSDVersion    BYTE  128  dup (?)
  wServicePackMajor WORD ?
  wServicePackMinor WORD ?
  wReserved       WORD 2 dup (?)
OSVERSIONINFOEXA ENDS

OSVERSIONINFOEX equ <OSVERSIONINFOEXA>

WIN_CERTIFICATE STRUCT
  dwLength        DWORD ?
  wRevision       WORD ?
  wCertificateType WORD ?
  bCertificate    BYTE 1 dup (?)
WIN_CERTIFICATE ENDS

DRAWPATRECT STRUCT
  ptPosition      POINT <>
  ptSize          POINT <>
  wStyle          WORD ?
  wPattern        WORD ?
DRAWPATRECT ENDS

PSINJECTDATA STRUCT
  DataBytes       DWORD ?
  InjectionPoint  DWORD ?
  Flags           DWORD ?
  Reserved        DWORD ?
PSINJECTDATA ENDS

DISPLAY_DEVICEA STRUCT
  cb              DWORD ?
  DeviceName      BYTE 32 dup (?)
  DeviceString    BYTE 128 dup (?)
  StateFlags      DWORD ?
DISPLAY_DEVICEA ENDS

DISPLAY_DEVICE equ <DISPLAY_DEVICEA>

TRIVERTEX STRUCT
  x               DWORD ?
  y               DWORD ?
  Red             WORD ?
  Green           WORD ?
  Blue            WORD ?
  Alpha           WORD ?
TRIVERTEX ENDS

GRADIENT_TRIANGLE STRUCT
  Vertex1         DWORD ?
  Vertex2         DWORD ?
  Vertex3         DWORD ?
GRADIENT_TRIANGLE ENDS

GRADIENT_RECT STRUCT
  UpperLeft       DWORD ?
  LowerRight      DWORD ?
GRADIENT_RECT ENDS

BLENDFUNCTION STRUCT
  BlendOp         BYTE ?
  BlendFlags      BYTE ?
  SourceConstantAlpha BYTE ?
  AlphaFormat     BYTE ?
BLENDFUNCTION ENDS

DIBSECTION STRUCT
  dsBm            BITMAP <>
  dsBmih          BITMAPINFOHEADER <>
  dsBitfields     DWORD 3 dup (?)
  dshSection      DWORD ?
  dsOffset        DWORD ?
DIBSECTION ENDS

EMRFORMAT STRUCT
  dSignature      DWORD ?
  nVersion        DWORD ?
  cbData          DWORD ?
  offData         DWORD ?
EMRFORMAT ENDS

EMRGLSRECORD STRUCT
  emr             EMR <>
  cbData          DWORD ?
  Data            BYTE 1 dup (?)
EMRGLSRECORD ENDS

EMRGLSBOUNDEDRECORD STRUCT
  emr             EMR <>
  rclBounds       RECTL <>
  cbData          DWORD ?
  Data            BYTE 1 dup (?)
EMRGLSBOUNDEDRECORD ENDS

EMRPIXELFORMAT STRUCT
  emr             EMR <>
  pfd             PIXELFORMATDESCRIPTOR <>
EMRPIXELFORMAT ENDS

EMRSETICMPROFILE STRUCT
  emr             EMR <>
  dwFlags         DWORD ?
  cbName          DWORD ?
  cbData          DWORD ?
  Data            BYTE 1 dup (?)
EMRSETICMPROFILE ENDS

POINTFLOAT STRUCT
  x               FLOAT <>
  y               FLOAT <>
POINTFLOAT ENDS

GLYPHMETRICSFLOAT STRUCT
  gmfBlackBoxX    FLOAT <>
  gmfBlackBoxY    FLOAT <>
  gmfptGlyphOrigin POINTFLOAT <>
  gmfCellIncX     FLOAT <>
  gmfCellIncY     FLOAT <>
GLYPHMETRICSFLOAT ENDS

LAYERPLANEDESCRIPTOR STRUCT
  nSize           WORD ?
  nVersion        WORD ?
  dwFlags         DWORD ?
  iPixelType      BYTE ?
  cColorBits      BYTE ?
  cRedBits        BYTE ?
  cRedShift       BYTE ?
  cGreenBits      BYTE ?
  cGreenShift     BYTE ?
  cBlueBits       BYTE ?
  cBlueShift      BYTE ?
  cAlphaBits      BYTE ?
  cAlphaShift     BYTE ?
  cAccumBits      BYTE ?
  cAccumRedBits   BYTE ?
  cAccumGreenBits BYTE ?
  cAccumBlueBits  BYTE ?
  cAccumAlphaBits BYTE ?
  cDepthBits      BYTE ?
  cStencilBits    BYTE ?
  cAuxBuffers     BYTE ?
  iLayerPlane     BYTE ?
  bReserved       BYTE ?
  crTransparent   DWORD ?
LAYERPLANEDESCRIPTOR ENDS

CPINFOEXA STRUCT
  MaxCharSize     DWORD ?
  DefaultChar     BYTE 2 dup (?)
  LeadByte        BYTE 12 dup (?)
  UnicodeDefaultChar WORD ?
  CodePage        DWORD ?
  CodePageName    BYTE 260 dup (?)
CPINFOEXA ENDS

CPINFOEX equ <CPINFOEXA>

PVALUEA STRUCT
  pv_valuename    DWORD ?
  pv_valuelen     DWORD ?
  pv_value_context DWORD ?
  pv_type         DWORD ?
PVALUEA ENDS

PVALUE equ <PVALUEA>

VALENTA STRUCT
  ve_valuename    DWORD ?
  ve_valuelen     DWORD ?
  ve_valueptr     DWORD ?
  ve_type         DWORD ?
VALENTA ENDS

VALENT equ <VALENTA>

DISCDLGSTRUCTA STRUCT
  cbStructure     DWORD ?
  hwndOwner       DWORD ?
  lpLocalName     DWORD ?
  lpRemoteName    DWORD ?
  dwFlags         DWORD ?
DISCDLGSTRUCTA ENDS

DISCDLGSTRUCT equ <DISCDLGSTRUCTA>

UNIVERSAL_NAME_INFOA STRUCT
  lpUniversalName DWORD ?
UNIVERSAL_NAME_INFOA ENDS

UNIVERSAL_NAME_INFO equ <UNIVERSAL_NAME_INFOA>

REMOTE_NAME_INFOA STRUCT
  lpUniversalName DWORD ?
  lpConnectionName DWORD ?
  lpRemainingPath DWORD ?
REMOTE_NAME_INFOA ENDS

REMOTE_NAME_INFO equ <REMOTE_NAME_INFOA>

NETINFOSTRUCT STRUCT
  cbStructure     DWORD ?
  dwProviderVersion DWORD ?
  dwStatus        DWORD ?
  dwCharacteristics DWORD ?
  dwHandle        DWORD ?
  wNetType        WORD ?
  dwPrinters      DWORD ?
  dwDrives        DWORD ?
NETINFOSTRUCT ENDS

NETCONNECTINFOSTRUCT STRUCT
  cbStructure     DWORD ?
  dwFlags         DWORD ?
  dwSpeed         DWORD ?
  dwDelay         DWORD ?
  dwOptDataSize   DWORD ?
NETCONNECTINFOSTRUCT ENDS

LPCPROPSHEETPAGE      equ   <LPCPROPSHEETPAGEA>
LPFNPSPCALLBACK         equ   <LPFNPSPCALLBACKA>
LPPROPSHEETPAGE       equ   <LPPROPSHEETPAGEA>
PROPSHEETPAGE         equ   <PROPSHEETPAGEA>
PROPSHEETPAGE_V1_SIZE equ   <PROPSHEETPAGEA_V1_SIZE>
CLSID EQU <GUID>
