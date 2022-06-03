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
// ChromaticLib_WinSock.h: declaration of used variables / structures for ChromaticLib_WinSock.cpp
// -----------------------------------------------------------------------

#ifndef _CHROMATICLIB_WINSOCK_H_
#define _CHROMATICLIB_WINSOCK_H_

#include "WALib.h"

// -----------------------------------------------------------------------
// Functions declarations
long CALLBACK WinsockInit(LPWSADATA WSADats, long Version);
long CALLBACK WinsockUnInit(void);
SOCKET CALLBACK SocketCreate(void);
long CALLBACK SocketDestroy(SOCKET hSock, HWND hWnd);
long CALLBACK SocketConnect(SOCKET hSock, LPSOCKADDR_IN SockStruct, long Message, HWND hWnd, CStr IP, long Port, long TimeOut);
long CALLBACK SocketGetPort(SOCKET hSock, LPSOCKADDR_IN SockStruct);
long CALLBACK SocketListen(SOCKET hSock, HWND hWnd, long Message, long MaxConnections);
SOCKET CALLBACK SocketAcceptConnection(SOCKET hSock, LPSOCKADDR_IN SockStruct);
long CALLBACK SocketSendASCIIDatasCR(SOCKET hSock, CStr Buffer);
long CALLBACK SocketSendUnknownDatas(SOCKET hSock, long Buffer, long BufLen);
long CALLBACK SocketGetRevcLen(SOCKET hSock);
CStr CALLBACK SocketReceiveASCIIDatas(SOCKET hSock);
long CALLBACK SocketReceiveUnknownDatas(SOCKET hSock, long *RecvBuffer, long LenBuffer);
SOCKET CALLBACK FTPInitiatePort(SOCKET hSock, LPSOCKADDR_IN SockStruct, HWND hWnd, long Message);
CStr CALLBACK FTPGetEntryDate(CStr DirEntry);
CStr CALLBACK FTPGetEntryFileName(CStr DirEntry);
CStr CALLBACK FTPGetEntryFileAttributes(CStr DirEntry);
CStr CALLBACK FTPGetEntryFileSize(CStr DirEntry);
long CALLBACK FTPIsEntryDir(CStr DirEntry);
CStr CALLBACK FTPCutEntry(CStr FTPEntry);

#endif
