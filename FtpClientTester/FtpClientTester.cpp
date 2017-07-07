// FtpClientTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <string>
#include <WinInet.h>

#define SW_UPGRADE_FTP_SERVER_PORT  30021
#define SW_UPGRADE_FTP_USER         "test"
#define SW_UPGRADE_FTP_PASS         "pass"

#pragma comment(lib, "wininet.lib")

int _tmain(int argc, _TCHAR* argv[])
{
  std::string remoteFname = "6.3.0.11_6.3(Official).zip";
  std::string localFname = "newfile.zip";
  std::string serverName = "localhost";
  INTERNET_PORT ftpPort = SW_UPGRADE_FTP_SERVER_PORT;
  

  // Open sessions
  HINTERNET sessionHandle = InternetOpen("FtpClientTester", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
  if (sessionHandle)
  {
    printf("sessionHandle opened successfully,\n");
    HINTERNET ftpSessionHandle = InternetConnect(sessionHandle, serverName.c_str(), ftpPort, SW_UPGRADE_FTP_USER, SW_UPGRADE_FTP_PASS, INTERNET_SERVICE_FTP, NULL, NULL);

#if 1
    // Transfer file
    if (ftpSessionHandle)
    {
      printf("ftpSessionHandle opened successfully\n");
      if (!FtpGetFile(ftpSessionHandle, remoteFname.c_str(), localFname.c_str(), FALSE,
        FILE_ATTRIBUTE_NORMAL, FTP_TRANSFER_TYPE_BINARY | INTERNET_FLAG_RELOAD, 0))
      {
        printf("Failed to transfer file successfully: %d\n", GetLastError());
      }
      else
      {
        printf("File transfer was a SUCCESS!\n");
      }
    }
    else
    {
      printf("Failed to open ftpSessionHandle: %d\n", GetLastError());
    }
#endif

    // Close sessions
    if (ftpSessionHandle)
      InternetCloseHandle(ftpSessionHandle);
    InternetCloseHandle(sessionHandle);
  }

	return 0;
}

