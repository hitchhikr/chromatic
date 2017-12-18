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
// Dlg_FTP.h: declaration of used variables / structures for Dlg_FTP.cpp
// -----------------------------------------------------------------------

#ifndef _DLG_FTP_H_
#define _DLG_FTP_H_

// -----------------------------------------------------------------------
// Constants
#define FTPCONNECT 1
#define FTPREFRESH 2
#define FTPDIRUP 3
#define FTPDIRVIEW 4
#define FTPNEWDIR 5
#define FTPDIRRENAME 6
#define FTPDEL 7
#define FTPDIRPERMS 8
#define FTPDOWNLOAD 9
#define FTPUPLOAD 10

#define FTPOP_NONE 0
#define FTPOP_DELETE 1
#define FTPOP_PERMS 2
#define FTPOP_DOWNLOAD 3
#define FTPOP_UPLOAD 4

#define FTP_IDBASE 4000
#define FTP_DOWNLOAD 0
#define FTP_DELETE 1

// -----------------------------------------------------------------------
// Variables
extern HWND FRMFTPhWnd;
extern CStr FRMFTPTitle;
extern HWND FRMFTPCbAccounts;
extern HWND FRMFTPListview;
extern HWND FRMFTPCmdClose;
extern HWND FRMFTPToolbar;
extern HWND FRMFTPStatLabel;

extern CStr FTPIP;
extern long FTPAnonymous;
extern long FTPGotPass;
extern CStr FTPUserName;
extern CStr FTPUserPass;
extern CStr FTPPath;
extern long FTPPort;
extern long FTPTimeOut;
extern long FTPTransType;
extern long FTPPassive;
    
extern SOCKET FRMFTPhSocketDir;
extern SOCKET FRMFTPhSocketDown;
extern SOCKET FRMFTPhSocketUp;
extern SOCKET FRMFTPhSocketDirIncoming;
extern SOCKET FRMFTPhSocketDownloading;
extern SOCKET FRMFTPhSocketUploading;
extern SOCKET FTMFTPhSocketMain;
extern sockaddr_in FRMFTPSocketInfos;
extern sockaddr_in FTMFTPSocketInfosAccept;
extern long FRMFTPDirEntries;
extern long FTMFTPConnectionState;
extern WSADATA FRMFTPMyWinsock;
extern CStr FTPCurrentIP;
extern long FTPCurrentPort;
extern long FTPNoAnonAllowed;
extern CStr FTPCurrentPath;

extern CStr NewDatToSend;
extern CStr FTPDirectories;
extern long *FTPDirectoriesArray;
extern long *FilesArray;
extern CStr OldRename;
extern CStr NewRename;
extern long CurrentSelected;
extern long CurrentOperation;
extern HANDLE CurrenthFile;
extern long OkUser;
extern long OkConnected;
extern long TriggerDir;
extern CStr UpLdFile;

extern CList <char *> PermFileName;
extern CList <char *> PermFilePerms;

extern long PermOwnerR;
extern long PermOwnerW;
extern long PermOwnerX;
extern long PermGroupR;
extern long PermGroupW;
extern long PermGroupX;
extern long PermOtherR;
extern long PermOtherW;
extern long PermOtherX;
extern long PermNbrFiles;
extern CStr CurrentDownloaded;
extern CStr CurrentUploaded;
extern CStr CurrentSize;
extern long CurrentSizeLng;
extern long CurrentResume;
extern long CurrentUp;
extern CStr CurrentUpReal;
extern long CurrentUphFile;
extern long CurrentUpLen;
extern long CurrentUpActual;
extern long CurrentUpFileNbr;
extern long CurrentUpBreak;
extern long WinSErrFlag;
extern HMENU hFTPMenu;

// -----------------------------------------------------------------------
// Functions declarations
int CALLBACK FRMFTPProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
void FillAccount(long AccNumber);
void WriteFTPStatus(CStr Txt);
void DispatchFTPMessages(CStr Message);
void ConnectToFTP(HWND hWnd);
void MainSocketEvents(long Message, HWND hWnd);
void UploadSocketEvents(long Message, HWND hWnd);
void DownloadSocketEvents(long Message, HWND hWnd);
void DirSocketEvents(long Message, HWND hWnd);
void BreakConnection(HWND hWnd);
void FilCListView(void);
long SelectFilesToUpload(HWND hWnd);
void FillPermissions(void);
LRESULT CALLBACK FRMFTPListviewHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
int CALLBACK FRMFTPListviewSort(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

#endif
