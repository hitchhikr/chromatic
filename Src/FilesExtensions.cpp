// -----------------------------------------------------------------------
// Chromatic
// Integrated Development Environment
//
// Copyright (C) 2001-2010 Franck Charlet.
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
//  1. Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//  2. Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL FRANCK CHARLET OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
// OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGE.
// -----------------------------------------------------------------------
// FilesExtensions.cpp: Registered extensions handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"

// Typical key to register a file extension:
// HKEY_CLASSES_ROOT\.asm
//   (Default) Chromatic xxx file (String)
// HKEY_CLASSES_ROOT\Chromatic file
//   EditFlags &h00000040& (Binary)
// HKEY_CLASSES_ROOT\Chromatic file\DefaultIcon
//c:\Chromatic\Chromatic.exe,39 (String)
// HKEY_CLASSES_ROOT\Chromatic file\Shell
//   HKEY_CLASSES_ROOT\Chromatic file\Shell\Open
//       HKEY_CLASSES_ROOT\Chromatic file\Shell\Open\Command
//           (Default) c:\Chromatic\Chromatic.exe %1
//       HKEY_CLASSES_ROOT\Chromatic file\Shell\Open with Chromatic\Command
//           (Default) c:\Chromatic\Chromatic.exe %1

// -----------------------------------------------------------------------
// Register a file extension
void RegisterExt(CStr Extension, long IconNumber)
{
    CStr RegFileName;
    long RegFileNameLen = 0;

    if(Extension.Len() == 0) return;
    RegFileName = RegFileName.String(MAX_PATH, 1);
    RegFileNameLen = GetModuleFileName(0, RegFileName.Get_String(), MAX_PATH);
    if(RegFileNameLen == 0) return;
    RegFileName = RegFileName.Left(RegFileNameLen);
    if(RegistryUpdateKey(HKEY_CLASSES_ROOT, "." + (CStr) Extension, "", "Chromatic " + (CStr) Extension + (CStr) " file", REG_SZ) == 1)
    {
        if(RegistryUpdateKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file", "EditFlags", "00000040", REG_BINARY) == 1)
        {
            if(RegistryUpdateKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file\\DefaultIcon", "", RegFileName + (CStr) "," + (CStr) IconNumber, REG_SZ) == 1)
            {
                if(RegistryUpdateKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file\\Shell\\Open\\Command", "", RegFileName + (CStr) " %1", REG_SZ) == 1)
                {
                    RegistryUpdateKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file\\Shell\\Open with Chromatic\\Command", "", RegFileName + (CStr) " %1", REG_SZ);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------
// Unregister a file extension
void UnRegisterExt(CStr Extension)
{
    CStr RegFileName;
    long RegFileNameLen = 0;

    if(Extension.Len() == 0) return;
    RegFileName = RegFileName.String(MAX_PATH, 1);
    RegFileNameLen = GetModuleFileName(0, RegFileName.Get_String(), MAX_PATH);
    if(RegFileNameLen == 0) return;
    RegFileName = RegFileName.Left(RegFileNameLen);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file\\Shell\\Open with Chromatic", "Command", REG_SZ);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file\\Shell\\Open", "Command", REG_SZ);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file\\Shell\\Open", "", REG_SZ);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file\\Shell", "Open with Chromatic", REG_SZ);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file\\Shell", "Open", REG_SZ);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file", "DefaultIcon", REG_SZ);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file", "Shell", REG_SZ);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "Chromatic " + (CStr) Extension + (CStr) " file", "", REG_SZ);
    RegistryDeleteKey(HKEY_CLASSES_ROOT, "." + (CStr) Extension, "", REG_SZ);
}
