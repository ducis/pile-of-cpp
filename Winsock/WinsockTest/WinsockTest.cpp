// WinsockTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <winsock2.h>
#include <ws2tcpip.h>
bool g_bIsServer=false;
wchar_t g_sPortString[16]=L"6798";
wchar_t g_sConnectTo[32]=L"";
int _tmain(int argc, _TCHAR* argv[])
{
	wprintf(L"Initializing...\n");
	for(long i=0;i<argc;++i){
		if(0==wcscmp(argv[i],L"-server")){
			g_bIsServer=true;
		}else if(0==wcscmp(argv[i],L"-port")){
			//g_port=_wtoi(argv[i+1]);
			wcscpy(g_sPortString,argv[++i]);
		}else if(0==wcscmp(argv[i],L"-connect_to")){
			wcscpy(g_sConnectTo,argv[++i]);
		}
	}
	if(g_bIsServer){
		wprintf(L"This is a server.\n");
	}else{
		wprintf(L"This is a client.\n");
	}
	WSADATA wsaData;

	int iResult;

	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		wprintf(L"WSAStartup failed: %d\n", iResult);
		return 1;
	}
	if(g_bIsServer){
		struct addrinfoW *result = NULL,
						*ptr = NULL,
						hints;

		ZeroMemory( &hints, sizeof(hints) );
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the local address and port to be used by the server
		iResult = GetAddrInfoW(NULL, g_sPortString, &hints, &result);
		if ( iResult != 0 ) {
			wprintf(L"getaddrinfo() failed: %d\n", iResult);
			WSACleanup();
			return 1;
		}
		SOCKET ListenSocket = INVALID_SOCKET;
		ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);		
		if (ListenSocket == INVALID_SOCKET) {
			wprintf(L"Error at socket(): %ld\n", WSAGetLastError());
			FreeAddrInfoW(result);
			WSACleanup();
			return 1;
		}
		iResult = bind( ListenSocket, 
			result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			wprintf(L"bind failed: %d\n", WSAGetLastError());
			FreeAddrInfoW(result);
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		if ( listen( ListenSocket, SOMAXCONN ) == SOCKET_ERROR ) {
			wprintf(L"Error at bind(): %ld\n", WSAGetLastError() );
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		SOCKET ClientSocket=INVALID_SOCKET;
		// Accept a client socket
		ClientSocket = accept(ListenSocket, NULL, NULL);
		if (ClientSocket == INVALID_SOCKET) {
			wprintf(L"accept failed: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return 1;
		}
		bool go=true;
		while(go){
			#define DEFAULT_BUFLEN 512

			char recvbuf[DEFAULT_BUFLEN];
			int iResult, iSendResult;
			int recvbuflen = DEFAULT_BUFLEN;

			iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
			if (iResult > 0) {
				wprintf(L"Bytes received: %d\n", iResult);
				wprintf(L"%s\n",recvbuf);
				// Echo the buffer back to the sender
				iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
				if (iSendResult == SOCKET_ERROR) {
					wprintf(L"send failed: %d\n", WSAGetLastError());
					closesocket(ClientSocket);
					WSACleanup();
					return 1;
				}
				wprintf(L"Bytes sent: %d\n", iSendResult);
			}else{
				go=false;
			}
			if(!wcscmp((const wchar_t *)recvbuf,L"\\disconnect")){
				go=false;
			}
		}
	}else{
		struct addrinfoW *result = NULL,
                *ptr = NULL,
                hints;

		ZeroMemory( &hints, sizeof(hints) );
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		iResult = GetAddrInfoW(g_sConnectTo, g_sPortString, &hints, &result);
		if ( iResult != 0 ) {
			wprintf(L"getaddrinfo failed: %d\n", iResult);
			WSACleanup();
			return 1;
		}
		SOCKET ConnectSocket = INVALID_SOCKET;
		// Attempt to connect to the first address returned by
		// the call to getaddrinfo
		ptr=result;
	
		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			wprintf(L"Error at socket(): %ld\n", WSAGetLastError());
			FreeAddrInfoW(result);
			WSACleanup();
			return 1;
		}

		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
		}

		// Should really try the next address returned by getaddrinfo
		// if the connect call failed
		// But for this simple example we just free the resources
		// returned by getaddrinfo and print an error message

		FreeAddrInfoW(result);

		if (ConnectSocket == INVALID_SOCKET) {
			wprintf(L"Unable to connect to server!\n");
			WSACleanup();
			return 1;
		}

		bool go=true;
		while(go){
			#define DEFAULT_BUFLEN 512
			char sendbuf[DEFAULT_BUFLEN];
			char recvbuf[DEFAULT_BUFLEN];
			int iResult;
			int recvbuflen = DEFAULT_BUFLEN;

			scanf("%s",sendbuf);
			//wscanf(L"%s",sendbuf);
			//iResult = send( ConnectSocket, sendbuf, wcslen((wchar_t*)sendbuf)*2+2, 0 );


			if('\\'==sendbuf[0]){
				if(!strcmp("recv",sendbuf+1)){
					iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
					if (iResult > 0) {
						wprintf(L"Bytes received: %d\n", iResult);
						wprintf(L"%s\n",recvbuf);
					}else{
						go=false;
					}
				}else{
					int i;
					sscanf(sendbuf+1,"%i",&i);
					sendbuf[0]=i;
					iResult = send( ConnectSocket, sendbuf,1,0);
				}
			}else{
				iResult = send( ConnectSocket, sendbuf,strlen(sendbuf),0);
			}


			if (iResult == SOCKET_ERROR) {
				wprintf(L"send failed: %d\n", WSAGetLastError());
				closesocket(ConnectSocket);
				WSACleanup();
				return 1;
			}
			wprintf(L"Bytes Sent: %ld\n", iResult);

			//iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
			//if (iResult > 0) {
			//	wprintf(L"Bytes received: %d\n", iResult);
			//	wprintf(L"%s\n",recvbuf);
			//}else{
			//	go=false;
			//}
			if(!wcscmp((const wchar_t *)sendbuf,L"\\disconnect")){
				go=false;
			}
		}


	}
	if(g_bIsServer){
		
	}else{
	}


	return 0;
}

