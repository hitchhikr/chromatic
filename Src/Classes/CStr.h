// -----------------------------------------------------------------------
// Chromatic
// Integrated Development Environment
//
// Copyright (C) 2001-2013 Franck Charlet.
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
// CStr.h: interface for the CStr class
// -----------------------------------------------------------------------

#ifndef _CSTR_H_
#define _CSTR_H_

// -----------------------------------------------------------------------
// Includes
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

// -----------------------------------------------------------------------
// Constants
enum CStr_Compare
{
    Binary_Compare = 0,
    Text_Compare = 1
};

// -----------------------------------------------------------------------
// Class definition
class CStr
{

public:
    // Default Constructors/Destructor
    CStr(CStr &Datas_To_Set);
    CStr(CStr *Datas_To_Set);
    CStr(char *Datas_To_Set = (char *) 0);
    CStr(int Datas_To_Set);
    CStr(unsigned int Datas_To_Set);
    CStr(long Datas_To_Set);
    CStr(unsigned long Datas_To_Set);
    CStr(bool Datas_To_Set);
    CStr(char Datas_To_Set);
    CStr(unsigned char Datas_To_Set);
    CStr(float Datas_To_Set);
    CStr(double Datas_To_Set);
    CStr(long double Datas_To_Set);
    virtual ~CStr();
    // Methods
	char *Get_String(int = 0);
    long Get_Long(void);
    long Get_Hex(void);
    int Get_Int(void);
    void Set_String(char *Datas_To_Set);
    int Set_Char(int Char_Index, char Datas_To_Set);
    CStr Mid(int, int = -1);
    CStr R_Trim(void);
    CStr L_Trim(void);
    CStr Trim(void);
    CStr Left(int);
    CStr Right(int);
    CStr Chr(int);
    CStr New_Line(void);
    CStr String(int, int);
    int Str_Comp(CStr, CStr, int);
    int Asc(void);
    CStr Space(int);
    CStr Upper_Case(void);
    CStr Lower_Case(void);
    CStr Str_Reverse(void);
    CStr Dec_To_String(long);
    CStr Hex_To_String(long);
    CStr Hex_Fix_To_String(long, long);
    CStr Oct_To_String(long);
    CStr Extract_Directory(void);
    CStr Extract_FileName(void);
    CStr Extract_FileName_Extension(void);
    CStr Remove_FileName_Extension(void);
    CStr Replace_FileName_Extension(CStr);
    int In_Str(int, CStr, int = Binary_Compare);
    int In_StrWord(int, CStr, int = Binary_Compare);
    int Len(void);
    CStr Replace(CStr, CStr, int = 1, int = -1, int = Binary_Compare);
    int Is_Numeric(void);

    // Operators overload
    friend int operator == (CStr, CStr);
    friend int operator != (CStr, CStr);
    friend int operator > (CStr, CStr);
    friend int operator < (CStr, CStr);
    friend int operator >= (CStr, CStr);
    friend int operator <= (CStr, CStr);
    friend int operator || (CStr, CStr);
    friend int operator && (CStr, CStr);

    friend int operator == (CStr *, CStr);
    friend int operator != (CStr *, CStr);
    friend int operator > (CStr *, CStr);
    friend int operator < (CStr *, CStr);
    friend int operator >= (CStr *, CStr);
    friend int operator <= (CStr *, CStr);
    friend int operator || (CStr *, CStr);
    friend int operator && (CStr *, CStr);

    friend int operator == (CStr, CStr *);
    friend int operator != (CStr, CStr *);
    friend int operator > (CStr, CStr *);
    friend int operator < (CStr, CStr *);
    friend int operator >= (CStr, CStr *);
    friend int operator <= (CStr, CStr *);
    friend int operator || (CStr, CStr *);
    friend int operator && (CStr, CStr *);

    friend int operator == (CStr, char *);
    friend int operator != (CStr, char *);
    friend int operator > (CStr, char *);
    friend int operator < (CStr, char *);
    friend int operator >= (CStr, char *);
    friend int operator <= (CStr, char *);
    friend int operator || (CStr, char *);
    friend int operator && (CStr, char *);
    
    friend int operator == (char *, CStr);
    friend int operator != (char *, CStr);
    friend int operator > (char *, CStr);
    friend int operator < (char *, CStr);
    friend int operator >= (char *, CStr);
    friend int operator <= (char *, CStr);
    friend int operator || (char *, CStr);
    friend int operator && (char *, CStr);

    friend CStr operator + (CStr, CStr);
    friend CStr operator + (CStr, CStr *);
    friend CStr operator + (CStr *, CStr);
    friend CStr operator + (CStr, char *);
    friend CStr operator + (CStr, long);
    friend CStr operator + (CStr, unsigned long);
    friend CStr operator + (CStr, int);
    friend CStr operator + (CStr, unsigned int);
    friend CStr operator + (CStr, bool);
    friend CStr operator + (CStr, char);
    friend CStr operator + (CStr, unsigned char);
    friend CStr operator + (CStr, float);
    friend CStr operator + (CStr, double);
    friend CStr operator + (CStr, long double);
    friend CStr operator + (char *, CStr);
    friend CStr operator + (long, CStr);
    friend CStr operator + (unsigned long, CStr);
    friend CStr operator + (int, CStr);
    friend CStr operator + (unsigned int, CStr);
    friend CStr operator + (bool, CStr);
    friend CStr operator + (char, CStr);
    friend CStr operator + (unsigned char, CStr);
    friend CStr operator + (float, CStr);
    friend CStr operator + (double, CStr);
    friend CStr operator + (long double, CStr);

    friend CStr operator & (CStr, CStr);
    friend CStr operator & (CStr, CStr *);
    friend CStr operator & (CStr *, CStr);
    friend CStr operator & (CStr, char *);
    friend CStr operator & (CStr, long);
    friend CStr operator & (CStr, unsigned long);
    friend CStr operator & (CStr, int);
    friend CStr operator & (CStr, unsigned int);
    friend CStr operator & (CStr, bool);
    friend CStr operator & (CStr, char);
    friend CStr operator & (CStr, unsigned char);
    friend CStr operator & (CStr, float);
    friend CStr operator & (CStr, double);
    friend CStr operator & (CStr, long double);
    friend CStr operator & (char *, CStr);
    friend CStr operator & (long, CStr);
    friend CStr operator & (unsigned long, CStr);
    friend CStr operator & (int, CStr);
    friend CStr operator & (unsigned int, CStr);
    friend CStr operator & (bool, CStr);
    friend CStr operator & (char, CStr);
    friend CStr operator & (unsigned char, CStr);
    friend CStr operator & (float, CStr);
    friend CStr operator & (double, CStr);
    friend CStr operator & (long double, CStr);

    operator long()
    {
        return atol(this->Datas);
    }

    operator unsigned long()
    {
        return atol(this->Datas);
    }

    operator int()
    {
        return atol(this->Datas);
    }

    operator unsigned int()
    {
        return atol(this->Datas);
    }

    operator char()
    {
        return (char) atol(this->Datas);
    }

    operator unsigned char()
    {
        return (unsigned char) atol(this->Datas);
    }

    operator char *()
    {
        return this->Datas;
    }

    operator unsigned char *()
    {
        return (unsigned char *) this->Datas;
    }

    operator float()
    {
        char *FloatNumberEnd = "";
        float FloatResult;
        FloatResult = (float) strtod(this->Datas, &FloatNumberEnd);
        return FloatResult;
    }

    operator double()
    {
        char *DoubleNumberEnd = "";
        double DoubleResult;
        DoubleResult = (double) strtod(this->Datas, &DoubleNumberEnd);
        return DoubleResult;
    }

    operator long double()
    {
        char *LongDoubleNumberEnd = "";
        long double LongDoubleResult;
        LongDoubleResult = (long double) strtod(this->Datas, &LongDoubleNumberEnd);
        return LongDoubleResult;
    }

    void operator = (CStr);
    void operator = (CStr *);
    void operator = (char *);
    void operator = (long);
    void operator = (unsigned long);
    void operator = (int);
    void operator = (unsigned int);
    void operator = (bool);
    void operator = (char);
    void operator = (unsigned char);
    void operator = (float);
    void operator = (double);
    void operator = (long double);

    void operator += (CStr);
    void operator += (CStr *);
    void operator += (char *);
    void operator += (long);
    void operator += (unsigned long);
    void operator += (int);
    void operator += (unsigned int);
    void operator += (bool);
    void operator += (char);
    void operator += (unsigned char);
    void operator += (float);
    void operator += (double);
    void operator += (long double);

    char operator [] (long);
    char operator [] (unsigned long);
    char operator [] (int);
    char operator [] (unsigned int);
    char operator [] (unsigned char);

    // Variables
private:
    char *Datas;
    bool Loaded;

};

#endif
