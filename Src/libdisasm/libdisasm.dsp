# Microsoft Developer Studio Project File - Name="libdisasm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=libdisasm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "libdisasm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "libdisasm.mak" CFG="libdisasm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "libdisasm - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "libdisasm - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libdisasm - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\libs\libdisasm.lib"

!ELSEIF  "$(CFG)" == "libdisasm - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"..\..\libs\libdisasmd.lib"

!ENDIF 

# Begin Target

# Name "libdisasm - Win32 Release"
# Name "libdisasm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ia32_implicit.c
# End Source File
# Begin Source File

SOURCE=.\ia32_insn.c
# End Source File
# Begin Source File

SOURCE=.\ia32_invariant.c
# End Source File
# Begin Source File

SOURCE=.\ia32_modrm.c
# End Source File
# Begin Source File

SOURCE=.\ia32_opcode_tables.c
# End Source File
# Begin Source File

SOURCE=.\ia32_operand.c
# End Source File
# Begin Source File

SOURCE=.\ia32_reg.c
# End Source File
# Begin Source File

SOURCE=.\ia32_settings.c
# End Source File
# Begin Source File

SOURCE=.\x86_disasm.c
# End Source File
# Begin Source File

SOURCE=.\x86_format.c
# End Source File
# Begin Source File

SOURCE=.\x86_imm.c
# End Source File
# Begin Source File

SOURCE=.\x86_insn.c
# End Source File
# Begin Source File

SOURCE=.\x86_misc.c
# End Source File
# Begin Source File

SOURCE=.\x86_operand_list.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ia32_implicit.h
# End Source File
# Begin Source File

SOURCE=.\ia32_insn.h
# End Source File
# Begin Source File

SOURCE=.\ia32_invariant.h
# End Source File
# Begin Source File

SOURCE=.\ia32_modrm.h
# End Source File
# Begin Source File

SOURCE=.\ia32_opcode_tables.h
# End Source File
# Begin Source File

SOURCE=.\ia32_operand.h
# End Source File
# Begin Source File

SOURCE=.\ia32_reg.h
# End Source File
# Begin Source File

SOURCE=.\ia32_settings.h
# End Source File
# Begin Source File

SOURCE=.\libdis.h
# End Source File
# Begin Source File

SOURCE=.\qword.h
# End Source File
# Begin Source File

SOURCE=.\x86_imm.h
# End Source File
# Begin Source File

SOURCE=.\x86_operand_list.h
# End Source File
# End Group
# End Target
# End Project
