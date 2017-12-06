//Client
#pragma comment(lib, "ws2_32.lib")
/*
Windows Sockets 2 (Winsock) enables programmers to create advanced Internet, intranet, and other network-capable applications to transmit application data across the wire, independent of the network protocol being used. With Winsock, programmers are provided access to advanced Microsoft® Windows® networking capabilities such as multicast and Quality of Service (QoS).

Winsock follows the Windows Open System Architecture (WOSA) model; it defines a standard service provider interface (SPI) between the application programming interface (API), with its exported functions and the protocol stacks. It uses the sockets paradigm that was first popularized by Berkeley Software Distribution (BSD) UNIX. It was later adapted for Windows in Windows Sockets 1.1, with which Windows Sockets 2 applications are backward compatible. Winsock programming previously centered around TCP/IP. Some programming practices that worked with TCP/IP do not work with every protocol. As a result, the Windows Sockets 2 API adds functions where necessary to handle several protocols.
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <string>
#include <iostream>

SOCKET Connection;


enum Packet
{
	P_ChatMessage,
	P_Test
};

bool ProcessPacket(Packet packettype)
{
	switch (packettype)
	{
	case P_ChatMessage:
	{
		int bufferlength = 0;
		recv(Connection, (char*)&bufferlength, sizeof(bufferlength), NULL); // receive buffer length
		char * buffer = new char[bufferlength + 1]; // allocate buffer
		buffer[bufferlength] = '\0'; //Set last character ti NULL terminator 
		recv(Connection, buffer, bufferlength, NULL); //Receive message from server
		std::cout << "\n" << __LINE__ << " Client Sink buffer = " << buffer << " of length " << bufferlength << std::endl;
		delete[] buffer; //Deallocate buffer
		break;
	}
	case P_Test:
	{
		std::cout << "You have received a Test packet\n";
		break;
	}
	default:
	{
		std::cout << __LINE__ << " Unrecognized Packet: " << packettype << std::endl;
		break;
	}
	}
	return true;
}

void ClientThread()
{
	Packet packettype;
	while (true)
	{
		recv(Connection, (char*)&packettype, sizeof(packettype), NULL); // receive packet type
		
		std::cout << __LINE__ << " Packet: " << packettype << std::endl;

		if (!ProcessPacket(packettype)) // If the packet is not properly processed
		{
			break; // break out of client handler loop
		}

	}
	closesocket(Connection); //close the socket that was being used for the client's connection
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
	if (WSAError != 0)
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
	int sizeofaddr = sizeof(addr);

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
	Connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
	}

	std::cout << "connected!" << std::endl;


	//Creates a thread to execute within the virtual address space of the calling process.
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientThread, NULL, NULL, NULL);

	/*CHAR MOTD[256];

	// The recv function receives data from a connected socket or a bound connectionless socket.
	recv(Connection, MOTD, sizeof(MOTD), NULL);
	std::cout << "MOTD:\t" << MOTD << std::endl;
	*/
	 // Fixed Size String
	// char buffer[256];

	std::string buffer; // string buffer to send message
	while (true)
	{
		// Fixed Size String
		//std::cin.getline(buffer, sizeof(buffer));

		std::getline(std::cin, buffer); //Get line if user presses enter and fill buffer
		int bufferlength = buffer.size();

		std::cout << __LINE__ << " Client Source buffer = " << buffer << " of length " << bufferlength << std::endl;

		Packet chatmessagepacket = P_ChatMessage;
		send(Connection, (char*)&chatmessagepacket, sizeof(chatmessagepacket), NULL);

		send(Connection, (char*)&bufferlength, sizeof(bufferlength), NULL); // Send length of buffer
		send(Connection, buffer.c_str(), bufferlength, NULL); //Send buffer string

		Sleep(10);
	}

	system("pause");
	return 0;
}