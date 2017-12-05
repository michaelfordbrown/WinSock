//Server

#pragma comment(lib, "ws2_32.lib")
/*
Windows Sockets 2 (Winsock) enables programmers to create advanced Internet, intranet, and other network-capable applications to transmit application data across the wire, independent of the network protocol being used. With Winsock, programmers are provided access to advanced Microsoft® Windows® networking capabilities such as multicast and Quality of Service (QoS).

Winsock follows the Windows Open System Architecture (WOSA) model; it defines a standard service provider interface (SPI) between the application programming interface (API), with its exported functions and the protocol stacks. It uses the sockets paradigm that was first popularized by Berkeley Software Distribution (BSD) UNIX. It was later adapted for Windows in Windows Sockets 1.1, with which Windows Sockets 2 applications are backward compatible. Winsock programming previously centered around TCP/IP. Some programming practices that worked with TCP/IP do not work with every protocol. As a result, the Windows Sockets 2 API adds functions where necessary to handle several protocols.
*/


#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>

SOCKET Connections[100];
int ConnectionCounter = 0;

void ClientHandlerThread(int index)
{
	// Fixed Size Buffer
	//char buffer[256];
	
	int bufferlength = 0; // Holds the length of the buffer
		
	while (true)
	{
		recv(Connections[index], (char*)&bufferlength, sizeof(bufferlength), NULL); // Get length of incoming message (buffer).

		char * buffer = new char[bufferlength]; // Create dynamic character array
		recv(Connections[index],buffer, bufferlength, NULL);

		for (int i = 0; i < ConnectionCounter; i++)
		{
			if (i == index)
				continue;
			send(Connections[i], (char*)&bufferlength, sizeof(bufferlength), NULL); // Send length of buffer
			send(Connections[i], buffer, bufferlength, NULL);// send the char message to other clients
		}

		delete[] buffer;
	}
}

int main()
{
	//WINSOCK STARTUP
	//================

	// The WSADATA structure contains information about the Windows Sockets implementation.
	WSAData wsaData;

	//WORD - A 16-bit unsigned integer. The range is 0 through 65535 decimal.
	//MAKEWORD  - Creates a WORD value by concatenating the specified values.
	// Version 2.1.0 - (January 1996) was the first public release of the WinSock 2 specification.
	WORD DllVersion = MAKEWORD(2, 1);
	
	// The WSAStartup function initiates use of the WinSock DLL by a process.
	int WSAError = 0;
	WSAError = WSAStartup(DllVersion, &wsaData);
	if ( WSAError != 0)
	{
		// An LPCSTR is a 32-bit pointer to a constant null-terminated string of 8-bit Windows (ANSI) characters.
		LPCSTR  ErrorMessage = "WinSock startup failed. Error code: " + WSAError;

		// if WSA process fails to start (non zero error code) then exit.
		MessageBoxA(NULL, ErrorMessage, "Error", MB_OK | MB_ICONERROR);

		// Terminates the process (unsuccessful termination).
		exit(EXIT_FAILURE);
	}

	//SET BINDING ADDRESS
	//====================

	//In the Internet address family, the SOCKADDR_IN structure is used by Windows Sockets to specify a local or remote endpoint address to which to connect a socket.
	SOCKADDR_IN addr;
	int addrlen = sizeof(addr);

	// The inet_addr function converts a string containing an IPv4 dotted-decimal address into a proper address for the IN_ADDR structure.
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");

	// The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
	addr.sin_port = htons(1111);

	// The AF_INET (always used) address family is the address family for IPv4.
	addr.sin_family = AF_INET;

	//CONNECT SOCKET
	//================

	// The socket function creates a socket that is bound to a specific transport service provider.
	//		AF_INET - The Internet Protocol version 4 (IPv4) address family.
	//		SOCK_STREAM - A socket type that provides sequenced, reliable, two-way, connection-based byte streams with an OOB data transmission mechanism. This socket type uses the Transmission Control Protocol (TCP) for the Internet address family (AF_INET or AF_INET6).
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);

	// The bind function associates a local address with a socket.
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));

	// The listen function places a socket in a state in which it is listening for an incoming connection.
	// SOMAXCONN, the underlying service provider responsible for socket s will set the backlog to a maximum reasonable value of pending connections in a queue.
	listen(sListen, SOMAXCONN);


	SOCKET newConnection;

	for (int i = 0; i < 100; i++)
	{
			// The accept function permits an incoming connection attempt on a socket.
			newConnection = accept(sListen, (SOCKADDR*)&addr, &addrlen);
			if (newConnection == 0)
			{
				WSAError = WSAGetLastError();
				LPCSTR  ErrorMessage = "WinSock Incoming Connection Failure. Error code: " + WSAError;
				MessageBoxA(NULL, ErrorMessage, "Error", MB_OK | MB_ICONERROR);
			}
			else
			{
				std::cout << "Client Connected!" << std::endl;
				std::string MOTD = "Welcome! This is the Message of the Day.";
				int MOTDLength = MOTD.size();

				send(newConnection, (char*)&MOTDLength, sizeof(MOTDLength), NULL);
				
				// The send function sends data on a connected socket.
				send(newConnection, MOTD.c_str(), MOTDLength, NULL);

				// Handle multiple clients
				Connections[i] = newConnection;
				ConnectionCounter++;

				//Creates a thread to execute within the virtual address space of the calling process.
				CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandlerThread, (LPVOID)(i), NULL, NULL);
			}
	}
	system("pause");
	return 0;
}