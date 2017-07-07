/*
 * CFtpServer :: a FTP Server Class Library
 *
 * Mail :: thebrowser@gmail.com

  Copyright (C) 2007 Julien Poumailloux

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  4. If you like this software, a fee would be appreciated, particularly if
     you use this software in a commercial product, but is not required.

GCC Compile line:
g++ ./CFtpServer/CFtpServer.cpp ./Example/main.cpp -o FtpServer -Wall -lpthread -D_REENTRANT
Under SunOS and mayber some other OS, you may need to link to '-lsocket', or '-lnsl' too.
Add '-D_FILE_OFFSET_BITS=64' for large file support.

*/

#include "getopt.h"

#define CFTPSERVER_CONFIG_H_PATH	"./CFtpServerConfig.h"
#include "../CFtpServer/CFtpServer.h"

#ifdef CFTPSERVER_ENABLE_EVENTS

void OnServerEvent( int Event )
{
	switch( Event )
	{
		case CFtpServer::START_LISTENING:
			printf("* Server is listening !\r\n");
			break;

		case CFtpServer::START_ACCEPTING:
			printf("* Server is accepting incoming connexions !\r\n");
			break;

		case CFtpServer::STOP_LISTENING:
			printf("* Server stopped listening !\r\n");
			break;

		case CFtpServer::STOP_ACCEPTING:
			printf("* Server stopped accepting incoming connexions !\r\n");
			break;
		case CFtpServer::MEM_ERROR:
			printf("* Warning, the CFtpServer class could not allocate memory !\r\n");
			break;
		case CFtpServer::THREAD_ERROR:
			printf("* Warning, the CFtpServer class could not create a thread !\r\n");
			break;
		case CFtpServer::ZLIB_VERSION_ERROR:
			printf("* Warning, the Zlib header version differs from the Zlib library version !\r\n");
			break;
		case CFtpServer::ZLIB_STREAM_ERROR:
			printf("* Warning, error during compressing/decompressing data !\r\n");
			break;
	}
}

void OnUserEvent( int Event, CFtpServer::CUserEntry *pUser, void *pArg )
{
	switch( Event )
	{
		case CFtpServer::NEW_USER:
			printf("* A new user has been created:\r\n"
					"\tLogin: %s\r\n" "\tPassword: %s\r\n" "\tStart directory: %s\r\n",
				pUser->GetLogin(), pUser->GetPassword(), pUser->GetStartDirectory() );
			break;

		case CFtpServer::DELETE_USER:
			printf("* \"%s\"user is being deleted: \r\n", pUser->GetLogin() );
			break;
	}
}

void OnClientEvent( int Event, CFtpServer::CClientEntry *pClient, void *pArg )
{
	switch( Event )
	{
		case CFtpServer::NEW_CLIENT:
			printf( "* A new client has been created:\r\n"
				"\tClient IP: [%s]\r\n\tServer IP: [%s]\r\n",
				inet_ntoa( *pClient->GetIP() ), inet_ntoa( *pClient->GetServerIP() ) );
			break;

		case CFtpServer::DELETE_CLIENT:
			printf( "* A client is being deleted.\r\n" );
			break;

		case CFtpServer::CLIENT_AUTH:
			printf( "* A client has logged-in as \"%s\".\r\n", pClient->GetUser()->GetLogin() );
			break;

		case CFtpServer::CLIENT_DISCONNECT:
			printf( "* A client has disconnected.\r\n" );
			break;

		case CFtpServer::CLIENT_UPLOAD:
			printf( "* A client logged-on as \"%s\" is uploading a file: \"%s\"\r\n",
				pClient->GetUser()->GetLogin(), (char*)pArg );
			break;

		case CFtpServer::CLIENT_DOWNLOAD:
			printf( "* A client logged-on as \"%s\" is downloading a file: \"%s\"\r\n",
				pClient->GetUser()->GetLogin(), (char*)pArg );
			break;

		case CFtpServer::CLIENT_LIST:
			printf( "* A client logged-on as \"%s\" is listing a directory: \"%s\"\r\n",
				pClient->GetUser()->GetLogin(), (char*)pArg );
			break;

		case CFtpServer::CLIENT_CHANGE_DIR:
			printf( "* A client logged-on as \"%s\" has changed its working directory:\r\n"
				"\tFull path: \"%s\"\r\n\tWorking directory: \"%s\"\r\n",
				pClient->GetUser()->GetLogin(), (char*)pArg, pClient->GetWorkingDirectory() );
			break;

		case CFtpServer::RECVD_CMD_LINE:
			printf( "* Received: %s (%s)>  %s\r\n",
				pClient->GetUser() ? pClient->GetUser()->GetLogin() : "(Not logged in)",
				inet_ntoa( *pClient->GetIP() ),
				(char*) pArg );
			break;
		
		case CFtpServer::SEND_REPLY:
			printf( "* Sent: %s (%s)>  %s\r\n",
				pClient->GetUser() ? pClient->GetUser()->GetLogin() : "(Not logged in)",
				inet_ntoa( *pClient->GetIP() ),
				(char*) pArg );
			break;

		case CFtpServer::TOO_MANY_PASS_TRIES:
			printf( "* Too many pass tries for (%s)\r\n",
				inet_ntoa( *pClient->GetIP() ) );
			break;
	}
}

#endif // #ifdef CFTPSERVER_ENABLE_EVENTS

void Usage() {
	printf( "Usage:\r\n"
			" Needed:\r\n"
			"  -u [login]:[password]   Specify the user login and password.\r\n"
			"  -d [directory]   Specify the user start directory.\r\n"
			"\tNote: the password is optional.\r\n"
			" Optional:\r\n"
			"  -p [port]   Specify the TCP/IP port the FTP server will listen on.\r\n"
			"  -r   Give the user the permission to Read files.\r\n"
			"  -w   Give the user the permission to Write files.\r\n"
			"\tNote: By default, the user has the permission to list files.\r\n"
			"  -z   Enable compression for data transfers.\r\n"
			"  -x   Enable server-to-server transfers.\r\n"
			"Example: program.exe -p 21 -u login:pass -d C:/ -rw\r\n"
			);
	return;
}

int main( int argc, char * argv[] )
{
	printf( "\r\n\t[}-------------/* CFtpServer Example 2 */-------------{]\r\n\n" );
	if( argc == 1 ) {
		Usage();
		return 0;
	}

	#ifdef WIN32
		WSADATA WSAData;
		if( WSAStartup( MAKEWORD(1, 0), &WSAData) != 0 ) {
			printf("-WSAStartup failure: WSAGetLastError=%d\r\n", WSAGetLastError() );
			return 0;
		}
	#endif

	CFtpServer FtpServer;

	FtpServer.SetMaxPasswordTries( 3 );
	FtpServer.SetNoLoginTimeout( 45 ); // seconds
	FtpServer.SetNoTransferTimeout( 90 ); // seconds
	FtpServer.SetDataPortRange( 100, 900 ); // data TCP-Port range = [100-999]

	int iPort = 21;
	char *pszUserLogin = NULL,
		*pszUserPass = NULL,
		*pszUserStartDirectory = NULL,
		*psTemp;

	unsigned long ulLocalInterface = INADDR_ANY;
	unsigned char ucUserPriv = CFtpServer::LIST;

	int c;
	while ((c = getopt(argc , argv, ":p:u:d:rwzx")) != -1) {
		switch( c ) {
			case 'p':
				iPort = atoi( optarg );
				if( iPort < 0 || iPort > 65535 ) {
					printf( "Invalid local port\r\n" );
					return 0;
				}
				break;
			case 'u':
				psTemp = strchr( optarg, ':' );
				pszUserLogin = optarg;
				if( psTemp ) {
					*psTemp = 0;
					pszUserPass = psTemp + 1;
				}
				break;
			case 'd':
				pszUserStartDirectory = strdup( optarg );
				break;
			case 'r':
				ucUserPriv |= CFtpServer::READFILE;
				break;
			case 'z':
				FtpServer.EnableModeZ( true );
				break;
			case 'x':
				FtpServer.EnableFXP( true );
				break;
			case 'w':
				ucUserPriv |= CFtpServer::WRITEFILE | CFtpServer::DELETEFILE |
					CFtpServer::CREATEDIR | CFtpServer::DELETEDIR;
				break;
			case '?':
				printf( "/!\\Unrecognised option: -%c\r\n\n", optopt);
				Usage();
				break;
			case ':':
				printf( "/!\\Missing argument for: -%c\r\n\n", optopt);
				Usage();
				return 0;
		}

	}
	if( !pszUserLogin ) {
		printf("/!\\Error: User Login is missing.\r\n\n" );
		Usage();
		return 0;
	}
	if( !pszUserStartDirectory ) {
		printf("/!\\Error: User Start directory is missing.\r\n\n" );
		Usage();
		return 0;
	}

	CFtpServer::CUserEntry *pUser = FtpServer.AddUser( pszUserLogin, pszUserPass, pszUserStartDirectory );

	if( pUser ) {

		pUser->SetMaxNumberOfClient( 0 ); // Unlimited
		pUser->SetPrivileges( ucUserPriv );

		// If you only want to listen on the TCP Loopback interface,
		// replace 'INNADDR_ANY' by 'inet_addr("127.0.0.1")'.
		if( FtpServer.StartListening( ulLocalInterface, (unsigned short) iPort ) ) {

        	printf( "-Server is listening ! :)\r\n" );

			if( FtpServer.StartAccepting() ) {

				printf( "-Server successfuly started ! :)\r\n" );

				for( ;; )
					#ifdef WIN32
						Sleep( 1000 );
					#else
						sleep( 1 );
					#endif

			} else
				printf( "-Unable to accept incoming connections.\r\n" );

			FtpServer.StopListening();

		} else
			printf( "-Unable to listen.\r\n" );

	} else printf( "-Unable to create the User.\r\n" );

	printf( "-Exiting.\r\n" );

	if( pszUserStartDirectory ) delete [] pszUserStartDirectory;

	#ifdef WIN32
		system( "pause" );
	#endif

	return 0;
}
