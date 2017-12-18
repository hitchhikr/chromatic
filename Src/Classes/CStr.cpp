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
// CStr.cpp: implementation of the CStr class
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "CStr.h"

// -----------------------------------------------------------------------
// Default constructors
CStr::CStr(CStr &Datas_To_Set)
{
    this->Datas = _strdup(Datas_To_Set.Datas);
    this->Loaded = true;
}
CStr::CStr(CStr *Datas_To_Set)
{
    this->Datas = _strdup(Datas_To_Set->Datas);
    this->Loaded = true;
}
CStr::CStr(char *Datas_To_Set)
{
    if(Datas_To_Set == (char *) 0)
    {
        this->Datas = "";
        this->Loaded = false;
    }
    else
    {
        this->Datas = _strdup(Datas_To_Set);
        this->Loaded = true;
    }
}
CStr::CStr(int Datas_To_Set)
{
    char ASCII_Number[33];

    _itoa(Datas_To_Set, ASCII_Number, 10);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
}
CStr::CStr(unsigned int Datas_To_Set)
{
    char ASCII_Number[33];

    sprintf(ASCII_Number, "%u", Datas_To_Set);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
}
CStr::CStr(long Datas_To_Set)
{
    char ASCII_Number[33];

    _ltoa(Datas_To_Set, ASCII_Number, 10);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
}
CStr::CStr(unsigned long Datas_To_Set)
{
    char ASCII_Number[33];

    sprintf(ASCII_Number, "%u", Datas_To_Set);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
}
CStr::CStr(bool Datas_To_Set)
{
    if(Datas_To_Set == true) this->Datas = _strdup("True");
    else this->Datas = _strdup("False");
    this->Loaded = true;
}
CStr::CStr(char Datas_To_Set)
{
    char ASCIIChar[2];

    ASCIIChar[0] = Datas_To_Set;
    ASCIIChar[1] = 0;
    this->Datas = _strdup(ASCIIChar);
    this->Loaded = true;
}
CStr::CStr(unsigned char Datas_To_Set)
{
    char ASCII_Number[33];

    sprintf(ASCII_Number, "%u", Datas_To_Set);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
}
CStr::CStr(float Datas_To_Set)
{
    char ASCII_Number[33];

    sprintf(ASCII_Number, "%.7g", Datas_To_Set);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
}
CStr::CStr(double Datas_To_Set)
{
    char ASCII_Number[33];

    sprintf(ASCII_Number, "%.15g", Datas_To_Set);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
}
CStr::CStr(long double Datas_To_Set)
{
    char ASCII_Number[65];

    sprintf(ASCII_Number, "%.15Lg", Datas_To_Set);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
}

// -----------------------------------------------------------------------
// Default destructor
CStr::~CStr()
{
    if(this->Loaded == true) free(this->Datas);
    this->Loaded = false;
}

// -----------------------------------------------------------------------
// Return the string attached to the class
char *CStr::Get_String(int Index)
{
    // Return datas
    return(this->Datas + Index);
}

// -----------------------------------------------------------------------
// Return a possible longword number inside current string
long CStr::Get_Long(void)
{
    // Return datas
    if(this->Datas == 0) return(0);
    return(atol(this->Datas));
}

// -----------------------------------------------------------------------
// Return a possible longword number inside current string
long CStr::Get_Hex(void)
{
    char *HexNumberEnd = "";
    long HexResult;
    CStr BufString;

    BufString = (CStr) this->Datas;
    // Return datas
    HexResult = strtoul(BufString.Get_String(), &HexNumberEnd, 16);
    return(HexResult);
}

// -----------------------------------------------------------------------
// Return a possible integer number inside current string
int CStr::Get_Int(void)
{
    // Return datas
    return(atoi(this->Datas));
}

// -----------------------------------------------------------------------
// Set the string attached to the class
void CStr::Set_String(char *Datas_To_Set)
{
    char *TempString = "";

    // Save it before free() because it can be the same string
    TempString = _strdup(Datas_To_Set);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = TempString;
    this->Loaded = true;
}

// -----------------------------------------------------------------------
// Modify a char inside the string
int CStr::Set_Char(int Char_Index, char Datas_To_Set)
{
    if(this->Loaded == false) return(0);
    if(Char_Index > (this->Len() - 1)) return(0);
    this->Datas[Char_Index] = Datas_To_Set;
    return(1);
}

// -----------------------------------------------------------------------
// == operator
int operator == (CStr String_Left, CStr String_Right)
{
    return(strcmp(String_Left.Datas, String_Right.Datas) == 0);
}
int operator == (CStr *String_Left, CStr String_Right)
{
    return(strcmp(String_Left->Datas, String_Right.Datas) == 0);
}
int operator == (CStr String_Left, CStr *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right->Datas) == 0);
}
int operator == (CStr String_Left, char *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right) == 0);
}
int operator == (char *String_Left, CStr String_Right)
{
    return(strcmp(String_Left, String_Right.Datas) == 0);
}

// -----------------------------------------------------------------------
// != operator
int operator != (CStr String_Left, CStr String_Right)
{
    return(strcmp(String_Left.Datas, String_Right.Datas) != 0);
}
int operator != (CStr *String_Left, CStr String_Right)
{
    return(strcmp(String_Left->Datas, String_Right.Datas) != 0);
}
int operator != (CStr String_Left, CStr *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right->Datas) != 0);
}
int operator != (char *String_Left, CStr String_Right)
{
    return(strcmp(String_Left, String_Right.Datas) != 0);
}
int operator != (CStr String_Left, char *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right) != 0);
}

// -----------------------------------------------------------------------
// > operator
int operator > (CStr String_Left, CStr String_Right)
{
    return(strcmp(String_Left.Datas, String_Right.Datas) > 0);
}
int operator > (CStr *String_Left, CStr String_Right)
{
    return(strcmp(String_Left->Datas, String_Right.Datas) > 0);
}
int operator > (CStr String_Left, CStr *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right->Datas) > 0);
}
int operator > (char *String_Left, CStr String_Right)
{
    return(strcmp(String_Left, String_Right.Datas) > 0);
}
int operator > (CStr String_Left, char *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right) > 0);
}

// -----------------------------------------------------------------------
// < operator
int operator < (CStr String_Left, CStr String_Right)
{
    return(strcmp(String_Left.Datas, String_Right.Datas) < 0);
}
int operator < (CStr *String_Left, CStr String_Right)
{
    return(strcmp(String_Left->Datas, String_Right.Datas) < 0);
}
int operator < (CStr String_Left, CStr *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right->Datas) < 0);
}
int operator < (char *String_Left, CStr String_Right)
{
    return(strcmp(String_Left, String_Right.Datas) < 0);
}
int operator < (CStr String_Left, char *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right) < 0);
}

// -----------------------------------------------------------------------
// >= operator
int operator >= (CStr String_Left, CStr String_Right)
{
    return(strcmp(String_Left.Datas, String_Right.Datas) >= 0);
}
int operator >= (CStr *String_Left, CStr String_Right)
{
    return(strcmp(String_Left->Datas, String_Right.Datas) >= 0);
}
int operator >= (CStr String_Left, CStr *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right->Datas) >= 0);
}
int operator >= (char *String_Left, CStr String_Right)
{
    return(strcmp(String_Left, String_Right.Datas) >= 0);
}
int operator >= (CStr String_Left, char *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right) >= 0);
}

// -----------------------------------------------------------------------
// <= operator
int operator <= (CStr String_Left, CStr String_Right)
{
    return(strcmp(String_Left.Datas, String_Right.Datas) <= 0);
}
int operator <= (CStr *String_Left, CStr String_Right)
{
    return(strcmp(String_Left->Datas, String_Right.Datas) <= 0);
}
int operator <= (CStr String_Left, CStr *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right->Datas) <= 0);
}
int operator <= (char *String_Left, CStr String_Right)
{
    return(strcmp(String_Left, String_Right.Datas) <= 0);
}
int operator <= (CStr String_Left, char *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right) <= 0);
}

// -----------------------------------------------------------------------
// || Operator
int operator || (CStr String_Left, CStr String_Right)
{
    return(strcmp(String_Left.Datas, String_Right.Datas) || 0);
}
int operator || (CStr *String_Left, CStr String_Right)
{
    return(strcmp(String_Left->Datas, String_Right.Datas) || 0);
}
int operator || (CStr String_Left, CStr *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right->Datas) || 0);
}
int operator || (char *String_Left, CStr String_Right)
{
    return(strcmp(String_Left, String_Right.Datas) || 0);
}
int operator || (CStr String_Left, char *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right) || 0);
}

// -----------------------------------------------------------------------
// && Operator
int operator && (CStr String_Left, CStr String_Right)
{
    return(strcmp(String_Left.Datas, String_Right.Datas) && 0);
}
int operator && (CStr *String_Left, CStr String_Right)
{
    return(strcmp(String_Left->Datas, String_Right.Datas) && 0);
}
int operator && (CStr String_Left, CStr *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right->Datas) && 0);
}
int operator && (char *String_Left, CStr String_Right)
{
    return(strcmp(String_Left, String_Right.Datas) && 0);
}
int operator && (CStr String_Left, char *String_Right)
{
    return(strcmp(String_Left.Datas, String_Right) && 0);
}

// -----------------------------------------------------------------------
// + Operator
CStr operator + (CStr String_Left, CStr String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (CStr String_Left, CStr *String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(String_Right->Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, String_Right->Datas);
    return(BToReturn);
}
CStr operator + (CStr *String_Left, CStr String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left->Datas) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left->Datas);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (CStr String_Left, char *String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(String_Right) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, String_Right);
    return(BToReturn);
}
CStr operator + (CStr String_Left, long String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator + (CStr String_Left, unsigned long String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator + (CStr String_Left, int String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator + (CStr String_Left, unsigned int String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator + (CStr String_Left, bool String_Right)
{
    CStr BToReturn;
    char * BoolDatas = "";

    if(String_Right == true) BoolDatas = "True";
    else BoolDatas = "False";
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(BoolDatas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, BoolDatas);
    return(BToReturn);
}
CStr operator + (CStr String_Left, char String_Right)
{
    CStr BToReturn;
    char ASCIIChar[2];

    ASCIIChar[0] = String_Right;
    ASCIIChar[1] = 0;
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCIIChar) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCIIChar);
    return(BToReturn);
}
CStr operator + (CStr String_Left, unsigned char String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator + (CStr String_Left, float String_Right)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.7g", String_Right);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator + (CStr String_Left, double String_Right)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.15g", String_Right);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator + (CStr String_Left, long double String_Right)
{
    CStr BToReturn;
    char ASCII_Number[64];

    sprintf(ASCII_Number, "%.15Lg", String_Right);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator + (char *String_Left, CStr String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (long String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (unsigned long String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (int String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (unsigned int String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (bool String_Left, CStr String_Right)
{
    CStr BToReturn;
    char * BoolDatas = "";

    if(String_Left == true) BoolDatas = "True";
    else BoolDatas = "False";
    BToReturn.Datas = (char *) malloc(strlen(BoolDatas) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, BoolDatas);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (char String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCIIChar[2];

    ASCIIChar[0] = String_Left;
    ASCIIChar[1] = 0;
    BToReturn.Datas = (char *) malloc(strlen(ASCIIChar) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCIIChar);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (unsigned char String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (float String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.7g", String_Left);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (double String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.15g", String_Left);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator + (long double String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[64];

    sprintf(ASCII_Number, "%.15Lg", String_Left);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}

// -----------------------------------------------------------------------
// & Operator
CStr operator & (CStr String_Left, CStr String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (CStr String_Left, CStr *String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(String_Right->Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, String_Right->Datas);
    return(BToReturn);
}
CStr operator & (CStr *String_Left, CStr String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left->Datas) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left->Datas);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (CStr String_Left, char *String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(String_Right) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, String_Right);
    return(BToReturn);
}
CStr operator & (CStr String_Left, long String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator & (CStr String_Left, unsigned long String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator & (CStr String_Left, int String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator & (CStr String_Left, unsigned int String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator & (CStr String_Left, bool String_Right)
{
    CStr BToReturn;
    char * BoolDatas = "";

    if(String_Right == true) BoolDatas = "True";
    else BoolDatas = "False";
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(BoolDatas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, BoolDatas);
    return(BToReturn);
}
CStr operator & (CStr String_Left, char String_Right)
{
    CStr BToReturn;
    char ASCIIChar[2];

    ASCIIChar[0] = String_Right;
    ASCIIChar[1] = 0;
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCIIChar) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCIIChar);
    return(BToReturn);
}
CStr operator & (CStr String_Left, unsigned char String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(String_Right, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator & (CStr String_Left, float String_Right)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.7g", String_Right);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator & (CStr String_Left, double String_Right)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.15g", String_Right);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator & (CStr String_Left, long double String_Right)
{
    CStr BToReturn;
    char ASCII_Number[64];

    sprintf(ASCII_Number, "%.15Lg", String_Right);
    BToReturn.Datas = (char *) malloc(strlen(String_Left.Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left.Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    return(BToReturn);
}
CStr operator & (char *String_Left, CStr String_Right)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(String_Left) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, String_Left);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (long String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (unsigned long String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (int String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (unsigned int String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (bool String_Left, CStr String_Right)
{
    CStr BToReturn;
    char * BoolDatas = "";

    if(String_Left == true) BoolDatas = "True";
    else BoolDatas = "False";
    BToReturn.Datas = (char *) malloc(strlen(BoolDatas) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, BoolDatas);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (char String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCIIChar[2];

    ASCIIChar[0] = String_Left;
    ASCIIChar[1] = 0;
    BToReturn.Datas = (char *) malloc(strlen(ASCIIChar) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCIIChar);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (unsigned char String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(String_Left, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (float String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.7g", String_Left);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (double String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.15g", String_Left);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}
CStr operator & (long double String_Left, CStr String_Right)
{
    CStr BToReturn;
    char ASCII_Number[64];

    sprintf(ASCII_Number, "%.15Lg", String_Left);
    BToReturn.Datas = (char *) malloc(strlen(ASCII_Number) + strlen(String_Right.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, ASCII_Number);
    strcat(BToReturn.Datas, String_Right.Datas);
    return(BToReturn);
}

// -----------------------------------------------------------------------
// = operator
void CStr::operator = (CStr DatasToLoad)
{
    char *Temp_String;

    Temp_String = _strdup(DatasToLoad.Datas);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = Temp_String;
    this->Loaded = true;
    return;
}
void CStr::operator = (CStr *DatasToLoad)
{
    char *Temp_String;

    Temp_String = _strdup(DatasToLoad->Datas);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = Temp_String;
    this->Loaded = true;
    return;
}
void CStr::operator = (char *DatasToLoad)
{
    char *Temp_String;

    Temp_String = _strdup(DatasToLoad);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = Temp_String;
    this->Loaded = true;
    return;
}
void CStr::operator = (long DatasToLoad)
{
    char ASCII_Number[16];

    if(this->Loaded == true) free(this->Datas);
    _ltoa(DatasToLoad, ASCII_Number, 10);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
    return;
}
void CStr::operator = (unsigned long DatasToLoad)
{
    char ASCII_Number[16];

    if(this->Loaded == true) free(this->Datas);
    _ltoa(DatasToLoad, ASCII_Number, 10);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
    return;
}
void CStr::operator = (int DatasToLoad)
{
    char ASCII_Number[16];

    if(this->Loaded == true) free(this->Datas);
    _itoa(DatasToLoad, ASCII_Number, 10);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
    return;
}
void CStr::operator = (unsigned int DatasToLoad)
{
    char ASCII_Number[16];

    if(this->Loaded == true) free(this->Datas);
    _ltoa(DatasToLoad, ASCII_Number, 10);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
    return;
}
void CStr::operator = (bool DatasToLoad)
{
    if(this->Loaded == true) free(this->Datas);
    if(DatasToLoad == true) this->Datas = _strdup("True");
    else this->Datas = _strdup("False");
    this->Loaded = true;
    return;
}
void CStr::operator = (char DatasToLoad)
{
    char ASCIIChar[2];

    ASCIIChar[0] = DatasToLoad;
    ASCIIChar[1] = 0;
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(ASCIIChar);
    this->Loaded = true;
    return;
}
void CStr::operator = (unsigned char DatasToLoad)
{
    char ASCII_Number[16];

    if(this->Loaded == true) free(this->Datas);
    _itoa((int) DatasToLoad, ASCII_Number, 10);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
    return;
}
void CStr::operator = (float DatasToLoad)
{
    char ASCII_Number[32];

    if(this->Loaded == true) free(this->Datas);
    sprintf(ASCII_Number, "%.7g", DatasToLoad);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
    return;
}
void CStr::operator = (double DatasToLoad)
{
    char ASCII_Number[32];

    if(this->Loaded == true) free(this->Datas);
    sprintf(ASCII_Number, "%.15g", DatasToLoad);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
    return;
}
void CStr::operator = (long double DatasToLoad)
{
    char ASCII_Number[64];

    if(this->Loaded == true) free(this->Datas);
    sprintf(ASCII_Number, "%.15Lg", DatasToLoad);
    this->Datas = _strdup(ASCII_Number);
    this->Loaded = true;
    return;
}

// -----------------------------------------------------------------------
// [] operator
char CStr::operator [] (long Index_To_Get)
{
    return(this->Datas[Index_To_Get]);
}
char CStr::operator [] (unsigned long Index_To_Get)
{
    return(this->Datas[Index_To_Get]);
}
char CStr::operator [] (int Index_To_Get)
{
    return(this->Datas[Index_To_Get]);
}
char CStr::operator [] (unsigned int Index_To_Get)
{
    return(this->Datas[Index_To_Get]);
}
char CStr::operator [] (unsigned char Index_To_Get)
{
    return(this->Datas[Index_To_Get]);
}

// -----------------------------------------------------------------------
// += Operator
void CStr::operator += (CStr Datas_To_Add)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(Datas_To_Add.Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, Datas_To_Add.Datas);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (CStr *Datas_To_Add)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(Datas_To_Add->Datas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, Datas_To_Add->Datas);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (char *Datas_To_Add)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(Datas_To_Add) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, Datas_To_Add);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (long Datas_To_Add)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(Datas_To_Add, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (unsigned long Datas_To_Add)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(Datas_To_Add, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (int Datas_To_Add)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(Datas_To_Add, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (unsigned int Datas_To_Add)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _ltoa(Datas_To_Add, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (bool Datas_To_Add)
{
    CStr BToReturn;
    char *BoolDatas = "";

    if(Datas_To_Add == true) BoolDatas = "True";
    else BoolDatas = "False";
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(BoolDatas) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, BoolDatas);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (char Datas_To_Add)
{
    CStr BToReturn;
    char ASCIIChar[2];

    ASCIIChar[0] = Datas_To_Add;
    ASCIIChar[1] = 0;
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCIIChar) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCIIChar);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (unsigned char Datas_To_Add)
{
    CStr BToReturn;
    char ASCII_Number[16];

    _itoa(Datas_To_Add, ASCII_Number, 10);
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (float Datas_To_Add)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.7g", Datas_To_Add);
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (double Datas_To_Add)
{
    CStr BToReturn;
    char ASCII_Number[32];

    sprintf(ASCII_Number, "%.15g", Datas_To_Add);
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}
void CStr::operator += (long double Datas_To_Add)
{
    CStr BToReturn;
    char ASCII_Number[64];

    sprintf(ASCII_Number, "%.15Lg", Datas_To_Add);
    BToReturn.Datas = (char *) malloc(strlen(this->Datas) + strlen(ASCII_Number) + 2);
    BToReturn.Loaded = true;
    strcpy(BToReturn.Datas, this->Datas);
    strcat(BToReturn.Datas, ASCII_Number);
    if(this->Loaded == true) free(this->Datas);
    this->Datas = _strdup(BToReturn.Datas);
    this->Loaded = true;
    return;
}

// -----------------------------------------------------------------------
// Mid() method
CStr CStr::Mid(int Start,int Length)
{
    int TmpLen;
    CStr BToReturn;

    TmpLen = strlen(this->Datas);
    if(Start < 1) Start = 1;
    if(Length == -1) Length = TmpLen - Start + 1;
    if(Start > TmpLen || this->Loaded == false)
    {
        BToReturn.Datas = (char *) (malloc(2));
        BToReturn.Loaded = true;
        Length = 0;
    }
    else
    {
        if(((Start - 1) + TmpLen) < Length) Length = TmpLen - (Start - 1);
        if(Length < 0) Length = 0;
        BToReturn.Datas = (char *) (malloc(Length + 1));
        BToReturn.Loaded = true;
        strncpy(BToReturn.Datas, &this->Datas[Start - 1], Length);
    }
    BToReturn.Datas[Length] = 0;
    return(BToReturn);
}

// -----------------------------------------------------------------------
// R_Trim() method
CStr CStr::R_Trim(void)
{
    int i;
    CStr BToReturn;

    if(this->Loaded == false) return(BToReturn);
    BToReturn.Datas = _strdup(this->Datas);
    BToReturn.Loaded = true;
    i = strlen(this->Datas);
    while(i--)
    {
        if(!isspace(BToReturn.Datas[i] & 0xff)) break;
    }
    BToReturn.Datas[++i] = 0;
    return(BToReturn);
}

// -----------------------------------------------------------------------
// L_Trim() method
CStr CStr::L_Trim(void)
{
    char *OutTrimBuffer;
    CStr BToReturn;
    CStr TempString;

    if(this->Loaded == false) return(BToReturn);
    BToReturn.Datas = _strdup(this->Datas);
    BToReturn.Loaded = true;
    if(strlen(BToReturn.Datas))
    {
        TempString = BToReturn;
        free(BToReturn.Datas);
        BToReturn.Loaded = false;
        for(OutTrimBuffer = TempString.Get_String(); *OutTrimBuffer && isspace(OutTrimBuffer[0] & 0xff); ++OutTrimBuffer);
        if(TempString.Datas != OutTrimBuffer)
        {
            TempString = OutTrimBuffer;
        }
        BToReturn = TempString;
    }
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Trim() method
CStr CStr::Trim(void)
{
    int i;
    char *OutTrimBuffer;
    CStr BToReturn;
    CStr TempString;

    if(this->Loaded == false) return(BToReturn);
    BToReturn.Datas = _strdup(this->Datas);
    BToReturn.Loaded = true;
    i = strlen(BToReturn.Datas);
    while(i--)
    {
        if(!isspace(BToReturn.Datas[i] & 0xff)) break;
    }
    BToReturn.Datas[++i] = 0;
    if(strlen(BToReturn.Datas))
    {
        TempString = BToReturn;
        free(BToReturn.Datas);
        BToReturn.Loaded = false;
        for(OutTrimBuffer = TempString.Get_String(); *OutTrimBuffer && isspace(OutTrimBuffer[0] & 0xff); ++OutTrimBuffer);
        if(TempString.Datas != OutTrimBuffer)
        {
            TempString = OutTrimBuffer;
        }
        BToReturn = TempString;
    }
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Left() method
CStr CStr::Left(int Length)
{
    int StrLeftLen;
    CStr BToReturn;
    
    if(this->Loaded == false) return(BToReturn);
    BToReturn.Datas = _strdup(this->Datas);
    BToReturn.Loaded = true;
    StrLeftLen = strlen(this->Datas);
    if(Length > StrLeftLen) Length = StrLeftLen;
    if(Length < 0) Length = 0;
    BToReturn.Datas[Length] = 0;
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Right() method
CStr CStr::Right(int Length)
{
    int Start;
    CStr BToReturn;

    if(this->Loaded == false) return(BToReturn);
    if(Length < 0) Length = 0;
    Start = strlen(this->Datas) - Length;
    if(Start < 0) Start = 0;
    BToReturn.Datas = _strdup(&this->Datas[Start]);
    BToReturn.Loaded = true;
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Chr() method
CStr CStr::Chr(int IntChar)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(2);
    BToReturn.Datas[0] = IntChar;
    BToReturn.Datas[1] = 0;
    BToReturn.Loaded = true;
    return(BToReturn);
}

// -----------------------------------------------------------------------
// New_Line() method
CStr CStr::New_Line(void)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(3);
    BToReturn.Datas[0] = 13;
    BToReturn.Datas[1] = 10;
    BToReturn.Datas[2] = 0;
    BToReturn.Loaded = true;
    return(BToReturn);
}

// -----------------------------------------------------------------------
// String() method
CStr CStr::String(int Count, int IntChar)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(Count + 1);
    memset(BToReturn.Datas, IntChar, Count);
    BToReturn.Datas[Count] = 0;
    BToReturn.Loaded = true;
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Str_Comp() method
int CStr::Str_Comp(CStr String1, CStr String2, int Search)
{
    if(Search == Binary_Compare) return strcmp(String1.Datas,String2.Datas);
    else return _stricmp(String1.Datas,String2.Datas);
}

// -----------------------------------------------------------------------
// Asc() method
int CStr::Asc(void)
{
    return((unsigned char) this->Datas[0] & 0xff);
}

// -----------------------------------------------------------------------
// Space() method
CStr CStr::Space(int Count)
{
    CStr BToReturn;

    if(Count <= 0) return(BToReturn);
    BToReturn.Datas = (char *) malloc(Count + 1);
    memset(BToReturn.Datas, ' ', Count);
    BToReturn.Datas[Count] = 0;
    BToReturn.Loaded = true;
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Upper_Case() method
CStr CStr::Upper_Case(void)
{
    CStr BToReturn;

    if(this->Loaded == false) return(BToReturn);
    BToReturn.Datas = _strdup(this->Datas);
    BToReturn.Loaded = true;
    BToReturn.Datas = _strupr(BToReturn.Datas);
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Lower_Case() method
CStr CStr::Lower_Case(void)
{
    CStr BToReturn;

    if(this->Loaded == false) return(BToReturn);
    BToReturn.Datas = _strdup(this->Datas);
    BToReturn.Loaded = true;
    BToReturn.Datas = _strlwr(BToReturn.Datas);
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Str_Reverse() method
CStr CStr::Str_Reverse(void)
{
    CStr BToReturn;
    int RevStrLen;
    int i;
    unsigned char RevChar;

    if(this->Loaded == false) return(BToReturn);
    BToReturn.Datas = _strdup(this->Datas);
    BToReturn.Loaded = true;
    RevStrLen = strlen(this->Datas);
    if(RevStrLen > 1)
    {
        for(i = 0; i != (RevStrLen / 2); i++)
        {
            RevChar = BToReturn.Datas[ RevStrLen - 1 - i];
            BToReturn.Datas[RevStrLen - 1 - i] = BToReturn.Datas[i];
            BToReturn.Datas[i] = RevChar;
        }
    }
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Dec_To_String() method
CStr CStr::Dec_To_String(long Value)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(16);
    BToReturn.Loaded = true;
    sprintf(BToReturn.Datas, "%d", Value);
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Hex_To_String() method
CStr CStr::Hex_To_String(long Value)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(16);
    BToReturn.Loaded = true;
    sprintf(BToReturn.Datas, "%X", Value);
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Hex_To_String() method
CStr CStr::Hex_Fix_To_String(long Value, long Length)
{
    CStr BToReturn;
    char *Format;
    long FixLength;

    Format = (char *) malloc(16);
    BToReturn.Datas = (char *) malloc(16);
    BToReturn.Loaded = true;
    FixLength = Length;
    if(FixLength > 8) FixLength = 8;
    sprintf(Format, "%%0%dX", FixLength);
    sprintf(BToReturn.Datas, Format, Value);
    free(Format);
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Oct_To_String() method
CStr CStr::Oct_To_String(long Value)
{
    CStr BToReturn;

    BToReturn.Datas = (char *) malloc(16);
    BToReturn.Loaded = true;
    sprintf(BToReturn.Datas, "%o", Value);
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Len() method
int CStr::Len(void)
{
    return(strlen(this->Datas));
}

// -----------------------------------------------------------------------
// Extract_Directory() method
CStr CStr::Extract_Directory(void)
{
    CStr BToReturn;
    int i;
    
    if(this->Loaded == false) return(BToReturn);
    i = strlen(this->Datas);
    if(i == 0) return(BToReturn);
    while(i--)
    {
        if(this->Datas[i] == '\\') return(this->Left(i + 1));
    }
    return(this->Left(i));
}

// -----------------------------------------------------------------------
// Extract_FileName() method
CStr CStr::Extract_FileName(void)
{
    CStr BToReturn;
    int i;
    int j;

    if(this->Loaded == false) return(BToReturn);
    i = strlen(this->Datas);
    if(i == 0) return(BToReturn);
    j = 0;
    while(i--)
    {
        if(this->Datas[i] == '\\') break;
        j++;
    }
    if(i < 0)
    {
        BToReturn = this->Get_String();
        return(BToReturn);
    }
    return(this->Right(j));
}

// -----------------------------------------------------------------------
// Extract_FileName_Extension() method
CStr CStr::Extract_FileName_Extension(void)
{
    CStr BToReturn;
    int i;
    int j;
    
    if(this->Loaded == false) return(BToReturn);
    i = strlen(this->Datas);
    if(i == 0) return(BToReturn);
    j = 0;
    while(i--)
    {
        if(this->Datas[i] == '.') break;
        j++;
    }
    if(i < 0) return(BToReturn);
    return(this->Right(j));
}

// -----------------------------------------------------------------------
// Remove_FileName_Extension() method
CStr CStr::Remove_FileName_Extension(void)
{
    CStr BToReturn;
    int i;
    
    if(this->Loaded == false) return(BToReturn);
    i = strlen(this->Datas);
    if(i == 0) return(BToReturn);
    while(i--)
    {
        if(this->Datas[i] == '.') break;
    }
    if(i < 0)
    {
        BToReturn = this->Get_String();
        return(BToReturn);
    }
    return(this->Left(i));
}

// -----------------------------------------------------------------------
// Replace_FileName_Extension() method
CStr CStr::Replace_FileName_Extension(CStr NewExtension)
{
    CStr BToReturn;
    int i;
    
    if(this->Loaded == false) return(BToReturn);
    i = strlen(this->Datas);
    if(i == 0) return(BToReturn);
    while(i--)
    {
        if(this->Datas[i] == '.') break;
    }
    if(i <= 0)
    {
        BToReturn = this->Get_String();
        BToReturn = BToReturn + ".";
        BToReturn = BToReturn + NewExtension;
        return(BToReturn);
    }
    else
    {
        BToReturn = this->Left(i);
        BToReturn = BToReturn + ".";
        BToReturn = BToReturn + NewExtension;
        return(BToReturn);
    }
}

// -----------------------------------------------------------------------
// In_Str() method
int CStr::In_Str(int SStart, CStr SuCStr, int Search)
{
    CStr In_StrMaIn_String;
    CStr In_StrSuCStr;
    int i = 0;
    int MaIn_StringLen;
    int SuCStrLen = 0;
    int FirstChar = 0;
    bool FoundFirstChar = false;
    int PosInSuCStr = 0;
    int FoundStart = 0;
    int SaveFoundStart = 0;
    int CharChecked = 0;
    
    MaIn_StringLen = strlen(this->Datas);
    if (SuCStr.Get_String() == 0)
    {
        if (MaIn_StringLen == 0) return(1);
        return(0);
    }
    if (SuCStr.Len() == 0)
    {
        if (MaIn_StringLen == 0) return(0);
        return(SStart);
    }
    if (SStart <= 0) return(0);
    if (SStart > MaIn_StringLen) return(0);
    In_StrMaIn_String = this;
    In_StrSuCStr = SuCStr;
    if (Search == Text_Compare)
    {
        In_StrMaIn_String = this->Upper_Case();
        In_StrSuCStr = In_StrSuCStr.Upper_Case();
    }
    FirstChar = In_StrSuCStr.Datas[0];
    MaIn_StringLen = strlen(In_StrMaIn_String.Datas);
    SuCStrLen = strlen(In_StrSuCStr.Datas);
    FoundStart = SStart;
SearchFirstChar:
    FoundFirstChar = false;
    PosInSuCStr = 1;
    for(i = FoundStart; i <= MaIn_StringLen; i++)
    {
        if(In_StrMaIn_String.Datas[i - 1] == FirstChar)
        {
            FoundFirstChar = true;
            break;
        }
    }
    if(FoundFirstChar == false) return(0);
    FoundStart = i;
    SaveFoundStart = FoundStart;
    FoundFirstChar = false;
    CharChecked = 0;
    while(FoundStart <= MaIn_StringLen)
    {
        if(PosInSuCStr > SuCStrLen) break;
        CharChecked++;
        if(In_StrMaIn_String.Datas[ FoundStart - 1] != In_StrSuCStr.Datas[PosInSuCStr - 1])
        {
            FoundFirstChar = true;
            break;
        }
        PosInSuCStr++;
        FoundStart++;
    }
    if(FoundFirstChar == true)
    {
        FoundStart = SaveFoundStart+1;
        if(FoundStart >= MaIn_StringLen) return(0);
        goto SearchFirstChar;
    }
    if(CharChecked < SuCStrLen) SaveFoundStart = 0;
    return(SaveFoundStart);
}

// -----------------------------------------------------------------------
// In_StrWord() method
int CStr::In_StrWord(int SStart, CStr SuCStr, int Search)
{
    int Found_Word = 0;
    int Len_Word = SuCStr.Len();

    int Pos_Word = In_Str(SStart, SuCStr, Search);
    if(SuCStr.Len() == 0) Pos_Word  = 0;
    while(Pos_Word)
    {
        // Start of the string ?
        if(Pos_Word > 1)
        {
            // No: check if the left char is a valid one
            if(this->Datas[Pos_Word - 2] == ' ' ||
               this->Datas[Pos_Word - 2] == 9 ||
               this->Datas[Pos_Word - 2] == 13 ||
               this->Datas[Pos_Word - 2] == 10)
            {
                Found_Word |= 1;
            }
        }
        if(this->Datas[(Pos_Word - 1) + Len_Word] == 0 ||
           this->Datas[(Pos_Word - 1) + Len_Word] == ' ' ||
           this->Datas[(Pos_Word - 1) + Len_Word] == 9 ||
           this->Datas[(Pos_Word - 1) + Len_Word] == 13 ||
           this->Datas[(Pos_Word - 1) + Len_Word] == 10)
        {
            Found_Word |= 2;
        }
        if(Found_Word == 3) return(Pos_Word);
        Pos_Word = In_Str(Pos_Word + Len_Word, SuCStr, Search);
    }
    return(0);
}

// -----------------------------------------------------------------------
// Replace() method
CStr CStr::Replace(CStr StringToFind, CStr StringToReplace,
                   int ReplaceStart, int RepCount, int CompareType)
{
    CStr BToReturn;
    int OldPosIn_String;
    int PosIn_String;
    int LenToFind;
    int LenToReplace;
    CStr TmpString;
    int NbrToReplace;
    CStr SunMaIn_String;

    NbrToReplace = RepCount;
    LenToFind = StringToFind.Len();
    if (LenToFind != 0)
    {
        LenToReplace = StringToReplace.Len();
        if(ReplaceStart <= 0) ReplaceStart = 1;
        SunMaIn_String = this->Mid(ReplaceStart);
        PosIn_String = 1;
        OldPosIn_String = 1;
ReplaceAll:
        if(NbrToReplace == 0) goto StopReplace;
        PosIn_String = SunMaIn_String.In_Str(PosIn_String, StringToFind, CompareType);
        if(PosIn_String != 0)
        {
            TmpString = SunMaIn_String.Left(PosIn_String - 1);
            TmpString = TmpString + StringToReplace;
            TmpString = TmpString + SunMaIn_String.Mid(PosIn_String + LenToFind);
            PosIn_String = PosIn_String + LenToReplace;
            OldPosIn_String = PosIn_String;
            if(NbrToReplace != -1) NbrToReplace = NbrToReplace - 1;
            SunMaIn_String = TmpString;
            goto ReplaceAll;
        }
StopReplace:
        BToReturn = SunMaIn_String;
    }
    else
    {
        BToReturn = this;
    }
    return(BToReturn);
}

// -----------------------------------------------------------------------
// Is_Numeric() method
int CStr::Is_Numeric(void)
{
    int i;
    int j = 0;
    char *NumericStr = this->Get_String();

    i = this->Len();
    while(j < i)
    {
        if(NumericStr[j] != '+' && NumericStr[j] != '-') break;
        j++;
    }
    if(j >= i) return(0);
    while(i > j)
    {
        if(!isdigit(NumericStr[i - 1]))
        {
            if(NumericStr[i - 1] != ',') return(0);
        }
        i--;
    }
    return(1);
}
