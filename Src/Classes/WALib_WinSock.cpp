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
// ChromaticLib_WinSock.cpp: implementation the Winsock part of the Chromatic Library
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Includes
#include "WALib_WinSock.h"

// -----------------------------------------------------------------------
// Init winsock
long CALLBACK WinsockInit(LPWSADATA WSADats, long Version)
{
    return(WSAStartup((WORD) Version, WSADats));
}

// -----------------------------------------------------------------------
// Release winsock
long CALLBACK WinsockUnInit(void)
{
    return(WSACleanup());
}

// -----------------------------------------------------------------------
// Create a new socket
SOCKET CALLBACK SocketCreate(void)
{
    return(socket(AF_INET, SOCK_STREAM, IPPROTO_IP));
}

// -----------------------------------------------------------------------
// Destroy a socket
long CALLBACK SocketDestroy(SOCKET hSock, HWND hWnd)
{
    if(hWnd != 0) WSAAsyncSelect(hSock, hWnd, 0, 0);
    return(closesocket(hSock));
}

// -----------------------------------------------------------------------
// Connect to server
long CALLBACK SocketConnect(SOCKET hSock, LPSOCKADDR_IN SockStruct, long Message, HWND hWnd, CStr IP, long Port, long TimeOut)
{
    long ReturnValue;
    long IPtostore;
    LPFD_SET SockSet;
    hostent *HostentStruct;
    TIMEVAL SockTimeOut;
    CStr IPResolvedStr;
    int i;
    char *AddrList;

    memset(&SockTimeOut, 0, sizeof(SockTimeOut));
    memset(&SockSet, 0, sizeof(SockSet));
    IPtostore = inet_addr(IP.Get_String());
    if(IPtostore == -1)
    {
        // Resolve host name
        HostentStruct = gethostbyname(IP.Get_String());
        if(HostentStruct != 0)
        {
            AddrList = HostentStruct->h_addr_list[0];
            for(i = 0; i < HostentStruct->h_length; i++)
            {
                IPResolvedStr = IPResolvedStr + IPResolvedStr.Dec_To_String((long) AddrList[i]);
                if(i != (HostentStruct->h_length -1)) IPResolvedStr = IPResolvedStr + ".";
            }
            IPtostore = inet_addr(IPResolvedStr.Get_String());
            if(IPtostore == -1) return(10049);
        }
        else
        {
            return(10049);
        }
    }
    SockStruct->sin_addr.S_un.S_un_b.s_b4 = (BYTE) ((IPtostore & 0xFF000000) >> 24);
    SockStruct->sin_addr.S_un.S_un_b.s_b3 = (BYTE) ((IPtostore & 0xFF0000) >> 16);
    SockStruct->sin_addr.S_un.S_un_b.s_b2 = (BYTE) ((IPtostore & 0xFF00) >> 8);
    SockStruct->sin_addr.S_un.S_un_b.s_b1 = (BYTE) (IPtostore & 0xFF);
    SockStruct->sin_family = AF_INET;
    SockStruct->sin_port = htons((WORD) Port);
    // Select message to be sent by winsock to us
    WSAAsyncSelect(hSock, hWnd, Message, FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT | FD_ACCEPT);
    // Connect to server
    ReturnValue = connect(hSock, (LPSOCKADDR) SockStruct, sizeof(sockaddr_in));
    if(ReturnValue != 0) ReturnValue = WSAGetLastError();
    // Normal non blocking sockets error
    if(ReturnValue == 10035)
    {
        SockSet = (LPFD_SET) AllocMem(4 + (FD_SETSIZE * 4));
        SockSet->fd_count = 1;
        SockSet->fd_array[0] = hSock;
        SockTimeOut.tv_sec = TimeOut / 1000;
        SockTimeOut.tv_usec = TimeOut;
        ReturnValue = select(0, 0, SockSet, 0, &SockTimeOut);
        switch(ReturnValue)
        {
            case -1:
                ReturnValue = WSAGetLastError();
                break;
            case 0:
                ReturnValue = 10060;
                break;
            default:
                ReturnValue = 0;
                break;
        }
        FreeMem((long) SockSet);
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve socket assigned port
long CALLBACK SocketGetPort(SOCKET hSock, LPSOCKADDR_IN SockStruct)
{
    int WAmysocklocalinfoslen;

    // Retrieve local informations
    WAmysocklocalinfoslen = sizeof(sockaddr_in);
    return(getsockname(hSock, (LPSOCKADDR) SockStruct, &WAmysocklocalinfoslen));
}

// -----------------------------------------------------------------------
/// Set a socket into listen state
long CALLBACK SocketListen(SOCKET hSock, HWND hWnd, long Message, long MaxConnections)
{
    long ReturnValue;
    sockaddr WASockAddr;

    memset(&WASockAddr, 0, sizeof(WASockAddr));
    WSAAsyncSelect(hSock, hWnd, Message, FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT | FD_ACCEPT);
    WASockAddr.sa_family = AF_INET;
    ReturnValue = bind(hSock, &WASockAddr, sizeof(WASockAddr));
    if(ReturnValue == 0) ReturnValue = listen(hSock, MaxConnections);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Accept a requested connection
SOCKET CALLBACK SocketAcceptConnection(SOCKET hSock, LPSOCKADDR_IN SockStruct)
{
    int WASockAddrAcceptLen;

    WASockAddrAcceptLen = sizeof(sockaddr_in);
    return(accept(hSock, (LPSOCKADDR) SockStruct, &WASockAddrAcceptLen));
}

// -----------------------------------------------------------------------
// Send ascii datas to server
long CALLBACK SocketSendASCIIDatasCR(SOCKET hSock, CStr Buffer)
{
    CStr BufToSend;

    BufToSend = Buffer;
    BufToSend = BufToSend + "\r\n";
    return(send(hSock, BufToSend.Get_String(), BufToSend.Len(), 0));
}

// -----------------------------------------------------------------------
// Send raw datas to server
long CALLBACK SocketSendUnknownDatas(SOCKET hSock, long Buffer, long BufLen)
{
    if(Buffer == 0) return(-1);
    if(BufLen == 0) return(-1);
    return(send(hSock,(char *) Buffer, BufLen, 0));
}

// -----------------------------------------------------------------------
// Determine the amount of datas received
long CALLBACK SocketGetRevcLen(SOCKET hSock)
{
    u_long ToReceive = 0;

    ioctlsocket(hSock, FIONREAD, &ToReceive);
    return(ToReceive);
}

// -----------------------------------------------------------------------
// Receive ascii datas from server
CStr  CALLBACK SocketReceiveASCIIDatas(SOCKET hSock)
{
    CStr ReturnValue;
    long LenBuffer;

    LenBuffer = SocketGetRevcLen(hSock);
    if(LenBuffer == 0) return(ReturnValue);
    ReturnValue = ReturnValue.String(LenBuffer, 1);
    recv(hSock, ReturnValue.Get_String(), LenBuffer, 0);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Receive binary datas from server
long CALLBACK SocketReceiveUnknownDatas(SOCKET hSock, long *RecvBuffer, long LenBuffer)
{
    if(RecvBuffer == 0) return(-1);
    if(LenBuffer == 0) return(-1);
    return(recv(hSock, (char *) RecvBuffer, LenBuffer, 0));
}

// -----------------------------------------------------------------------
// FTP protocol handling
// -----------------------------------------------------------------------

// -----------------------------------------------------------------------
// Create a new FTP port
SOCKET CALLBACK FTPInitiatePort(SOCKET hSock, LPSOCKADDR_IN SockStruct, HWND hWnd, long Message)
{
    SOCKET ReturnValue;
    sockaddr_in WASockDir;
    hostent *HostentStruct;
    CStr FtpCommand;
    CStr LocIP;
    CStr IPResolvedStr;
    sockaddr_in TmpSockStruct;
    long IPtostore;
    CStr LocHost;
    long lngRetVal;
    int i;
    char *AddrList;

    memset(&TmpSockStruct, 0, sizeof(TmpSockStruct));
    memset(&WASockDir, 0, sizeof(WASockDir));
    LocHost = LocHost.String(256, 1);
    ReturnValue = SocketCreate();
    SocketListen(ReturnValue, hWnd, Message, 1);
    SocketGetPort(ReturnValue, &WASockDir);
    lngRetVal = gethostname(LocHost.Get_String(), 256);
    if(lngRetVal == -1)
    {
        return(0);
    }
    else
    {
        LocIP = LocHost.String(LocHost.Len(), 1);
        RtlCopyMemory(LocIP.Get_String(), LocHost.Get_String(), LocHost.Len());
    }
    IPtostore = inet_addr(LocIP.Get_String());
    if(IPtostore == -1)
    {
        // Resolve host name
        HostentStruct = gethostbyname(LocIP.Get_String());
        if(HostentStruct != 0)
        {
            AddrList = HostentStruct->h_addr_list[0];
            for(i = 0; i < HostentStruct->h_length; i++)
            {
                IPResolvedStr = IPResolvedStr + IPResolvedStr.Dec_To_String((long) AddrList[i]);
                if(i != (HostentStruct->h_length -1)) IPResolvedStr = IPResolvedStr + ".";
            }
            IPtostore = inet_addr(IPResolvedStr.Get_String());
            if(IPtostore == -1) return(0);
        }
        else
        {
            return(0);
        }
    }
    TmpSockStruct.sin_addr.S_un.S_un_b.s_b4 = (BYTE) ((IPtostore & 0xFF000000) >> 24);
    TmpSockStruct.sin_addr.S_un.S_un_b.s_b3 = (BYTE) ((IPtostore & 0xFF0000) >> 16);
    TmpSockStruct.sin_addr.S_un.S_un_b.s_b2 = (BYTE) ((IPtostore & 0xFF00) >> 8);
    TmpSockStruct.sin_addr.S_un.S_un_b.s_b1 = (BYTE) (IPtostore & 0xFF);
    FtpCommand = "PORT ";
    FtpCommand = FtpCommand + TmpSockStruct.sin_addr.S_un.S_un_b.s_b1;
    FtpCommand = FtpCommand + ",";
    FtpCommand = FtpCommand + TmpSockStruct.sin_addr.S_un.S_un_b.s_b2;
    FtpCommand = FtpCommand + ",";
    FtpCommand = FtpCommand + TmpSockStruct.sin_addr.S_un.S_un_b.s_b3;
    FtpCommand = FtpCommand + ",";
    FtpCommand = FtpCommand + TmpSockStruct.sin_addr.S_un.S_un_b.s_b4;
    FtpCommand = FtpCommand + ",";
    FtpCommand = FtpCommand + (WASockDir.sin_port & 0xFF);
    FtpCommand = FtpCommand + ",";
    FtpCommand = FtpCommand + ((WASockDir.sin_port & 0xFF00) >> 8);
    SocketSendASCIIDatasCR(hSock, &FtpCommand);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve FTP entry date
CStr  CALLBACK FTPGetEntryDate(CStr DirEntry)
{
    CStr ReturnValue;
    CStr StrLine;
    CStr BufString;
    CStr BufString2;
    SYSTEMTIME SysFutDateA;
    SYSTEMTIME SysFutDateB;
    FILETIME FileFutDateA;
    FILETIME FileFutDateB;

    memset(&SysFutDateA, 0, sizeof(SysFutDateA));
    memset(&SysFutDateB, 0, sizeof(SysFutDateA));
    StrLine = FTPCutEntry(DirEntry);
    StrLine = StrLine.Mid(43);
    if(StrLine.Len() == 0) return(ReturnValue);
    StrLine = StrLine.Trim();
    BufString = StrLine.Mid(5, 2);
    BufString = BufString.Trim();
    BufString = StringNumberComplement(BufString.Get_Long(), 2);
    BufString = BufString + "/";
    BufString2 = StrLine.Left(3);
    BufString2 = BufString2.Upper_Case();
    if(BufString2 == "JAN")
    {
        BufString = BufString + "01/";
        SysFutDateA.wMonth = 1;
    }
    else if(BufString2 == "FEB")
    {
        BufString = BufString + "02/";
        SysFutDateA.wMonth = 2;
    }
    else if(BufString2 == "MAR")
    {
        BufString = BufString + "03/";
        SysFutDateA.wMonth = 3;
    }
    else if(BufString2 == "APR")
    {
        BufString = BufString + "04/";
        SysFutDateA.wMonth = 4;
    }
    else if(BufString2 == "MAY")
    {
        BufString = BufString + "05/";
        SysFutDateA.wMonth = 5;
    }
    else if(BufString2 == "JUN")
    {
        BufString = BufString + "06/";
        SysFutDateA.wMonth = 6;
    }
    else if(BufString2 == "JUL")
    {
        BufString = BufString + "07/";
        SysFutDateA.wMonth = 7;
    }
    else if(BufString2 == "AUG")
    {
        BufString = BufString + "08/";
        SysFutDateA.wMonth = 8;
    }
    else if(BufString2 == "SEP")
    {
        BufString = BufString + "09/";
        SysFutDateA.wMonth = 9;
    }
    else if(BufString2 == "OCT")
    {
        BufString = BufString + "10/";
        SysFutDateA.wMonth = 10;
    }
    else if(BufString2 == "NOV")
    {
        BufString = BufString + "11/";
        SysFutDateA.wMonth = 11;
    }
    else if(BufString2 == "DEC")
    {
        BufString = BufString + "12/";
        SysFutDateA.wMonth = 12;
    }
    BufString2 = StrLine.Mid(8, 1);
    if(BufString2 == " ")
    {
        // Last field references year
        BufString = BufString + StrLine.Mid(9, 4);
        BufString = BufString + " 12:00";
    }
    else
    {
        SysFutDateA.wDay = StrLine.Mid(5, 2).Get_Int();
        SysFutDateA.wYear = DateGetYear().Get_Int();
        SysFutDateA.wHour = StrLine.Mid(8, 2).Get_Int();
        SysFutDateA.wMinute = StrLine.Mid(11, 2).Get_Int();
        SystemTimeToFileTime(&SysFutDateA, &FileFutDateA);
        GetLocalTime(&SysFutDateB);
        SystemTimeToFileTime(&SysFutDateB, &FileFutDateB);
        if(CompareFileTime(&FileFutDateA, &FileFutDateB) > 0)
        {
            BufString2 = (DateGetYear().Get_Long() - 1);
            BufString = BufString + BufString2;
            BufString = BufString + " ";
            BufString = BufString + StrLine.Mid(8, 5);
        }
        else
        {
            BufString = BufString + DateGetYear();
            BufString = BufString + " ";
            BufString = BufString + StrLine.Mid(8, 5);
        }
    }
    ReturnValue = BufString;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve FTP entry filename
CStr  CALLBACK FTPGetEntryFileName(CStr DirEntry)
{
    CStr ReturnValue;
    CStr StrLine;

    StrLine = FTPCutEntry(DirEntry);
    ReturnValue = StrLine.Mid(56).Trim();
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve FTP entry attributes
CStr  CALLBACK FTPGetEntryFileAttributes(CStr DirEntry)
{
    CStr ReturnValue;
    CStr StrLine;

    StrLine = FTPCutEntry(DirEntry);
    ReturnValue = StrLine.Mid(2, 9);
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Retrieve FTP entry filesize
CStr  CALLBACK FTPGetEntryFileSize(CStr DirEntry)
{
    CStr ReturnValue;
    long i;
    CStr StrLine;

    StrLine = FTPCutEntry(DirEntry);
    if(_strcmpi(StrLine.Left(1).Get_String(), "D") == 0) return(ReturnValue);
    for (i = 41; i >= 1 ; i--)
    {
        if(strcmp(StrLine.Mid(i, 1).Get_String(), " ") == 0) break;
    }
    if((42 - i) > 0)
    {
        ReturnValue = StrLine.Mid(i, 42 - i);
        ReturnValue = ReturnValue.Trim();
    }
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Check if a FTP entry is a directory
long CALLBACK FTPIsEntryDir(CStr DirEntry)
{
    long ReturnValue = 0;

    if(_strcmpi(DirEntry.Left(1).Get_String(), "D") == 0) ReturnValue = 1;
    return(ReturnValue);
}

// -----------------------------------------------------------------------
// Arrange a FTP dir entry (private)
CStr  CALLBACK FTPCutEntry(CStr FTPEntry)
{
    CStr ReturnValue;
    CStr BufString;
    long StrLineLen;

    BufString = "\r\n";
    StrLineLen = FTPEntry.In_Str(1, BufString);
    if(StrLineLen == 0) StrLineLen = FTPEntry.Len();
    else StrLineLen--;
    if(FTPEntry.Len() != 0) ReturnValue = FTPEntry.Mid(1, StrLineLen);
    return(ReturnValue);
}
