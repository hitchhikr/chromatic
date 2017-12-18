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
// Crypt.cpp: Crypt/decrypt FTP passwords
// (Unreliable and lousy)
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "Classes/WALib.h"

// -----------------------------------------------------------------------
// Crypt a password
CStr CryptPassword(CStr PasswordToCrypt)
{
    CList <int> CryptArray;
    CStr ReturnValue;
    long SwapVal = 0;
    int i = 0;
    int k = 0;
    CStr InKey;
    CStr InKey2;
    CStr BufString;

    CryptArray.MAdd(PasswordToCrypt.Len(), 0L);
    InKey = RegistryGetKeyValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName", "ComputerName");
    InKey2 = RegistryGetKeyValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer", "Logon User Name");
    if((InKey.Len() == 0) || (InKey2.Len() == 0))
    {
        InKey = "Build project";
    }
    else
    {
        // Merge machine name and user name
        InKey = InKey + (CStr) InKey2;
    }
    for(i = 1; i <= PasswordToCrypt.Len(); i++)
    {
        CryptArray.Set(i - 1, PasswordToCrypt.Mid(i, 1).Asc());
        CryptArray.Set(i - 1, (CryptArray.Get(i - 1)->Content << 1));
        for(k = 1; k <= InKey.Len(); k++)
        {
            CryptArray.Set(i - 1, CryptArray.Get(i - 1)->Content ^ InKey.Mid(k, 1).Asc());
        }
        CryptArray.Set(i - 1, CryptArray.Get(i - 1)->Content ^ ((i - 1) * 64));
        CryptArray.Set(i - 1, CryptArray.Get(i - 1)->Content ^ 911);
    }
    // Odd number of entries ?
    if((CryptArray.Amount() & 1) != 0) CryptArray.Add(0L);
    for(i = 0; i < CryptArray.Amount(); i = i + 2)
    {
        SwapVal = (CryptArray.Get(i + 1)->Content & 0xFF00) + (CryptArray.Get(i)->Content & 0xFF);
        CryptArray.Set(i + 1, (CryptArray.Get(i)->Content & 0xFF00) + (CryptArray.Get(i + 1)->Content & 0xFF));
        CryptArray.Set(i, SwapVal);
    }
    for(i = 0; i < CryptArray.Amount(); i++)
    {
        BufString = StringNumberComplement(CryptArray.Get(i)->Content, 4) + BufString;
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Decrypt a password
CStr DecryptPassword(CStr PasswordToDeCrypt)
{
    CList <int> CryptArray;
    CStr ReturnValue;
    int i = 0;
    int j = 0;
    int k = 0;
    long SwapVal = 0;
    CStr OutKey;
    CStr OutKey2;
    CStr BufString;

    CryptArray.MAdd(PasswordToDeCrypt.Len(), 0L);
    OutKey = RegistryGetKeyValue(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName", "ComputerName");
    OutKey2 = RegistryGetKeyValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer", "Logon User Name");
    if((OutKey.Len() == 0) || (OutKey2.Len() == 0))
    {
        OutKey = "Build project";
    }
    else
    {
        OutKey = OutKey + (CStr) OutKey2;
    }
    for(i = PasswordToDeCrypt.Len() - 1; i >= 0; i = i - 4)
    {
        CryptArray.Set(j, PasswordToDeCrypt.Mid(i + 1 - 3, 4).Get_Long());
        j++;
    }
    for(i = 0; i < CryptArray.Amount(); i = i + 2)
    {
        SwapVal = (CryptArray.Get(i + 1)->Content & 0xFF00) + (CryptArray.Get(i)->Content & 0xFF);
        CryptArray.Set(i + 1, (CryptArray.Get(i)->Content & 0xFF00) + (CryptArray.Get(i + 1)->Content & 0xFF));
        CryptArray.Set(i, SwapVal);
    }
    j = 0;
    for(i = 0; i <= PasswordToDeCrypt.Len() - 1; i = i + 4)
    {
        if(CryptArray.Get(j)->Content != 0)
        {
            CryptArray.Set(j, CryptArray.Get(j)->Content ^ 911);
            CryptArray.Set(j, CryptArray.Get(j)->Content ^ (j * 64));
            for(k = OutKey.Len(); k >= 1; k--)
            {
                CryptArray.Set(j, CryptArray.Get(j)->Content ^ OutKey.Mid(k, 1).Asc());
            }
            BufString = BufString + (CStr) BufString.Chr((CryptArray.Get(j)->Content >> 1)).Get_String();
        }
        j++;
    }
    ReturnValue = BufString;
    return(ReturnValue);
}
