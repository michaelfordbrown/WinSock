#include "Client.h"

Client::Client(std::string IP, int PORT)
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

	std::cout << "WinSock startup successful.\n";

	//SET BINDING ADDRESS
	//====================

	// The inet_addr function converts a string containing an IPv4 dotted-decimal address into a proper address for the IN_ADDR structure.
	addr.sin_addr.s_addr = inet_addr(IP.c_str());

	// The htons function converts a u_short from host to TCP/IP network byte order (which is big-endian).
	addr.sin_port = htons(PORT);

	// The AF_INET (always used) address family is the address family for IPv4.
	addr.sin_family = AF_INET;

	clientptr = this;

}

bool Client::Connect()
{
	//CONNECT SOCKET
	//================

	// The socket function creates a socket that is bound to a specific transport service provider.
	//		AF_INET - The Internet Protocol version 4 (IPv4) address family.
	//		SOCK_STREAM - A socket type that provides sequenced, reliable, two-way, connection-based byte streams with an OOB data transmission mechanism. This socket type uses the Transmission Control Protocol (TCP) for the Internet address family (AF_INET or AF_INET6).
	Connection = socket(AF_INET, SOCK_STREAM, NULL);

	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0)
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
	std::cout << "connected!" << std::endl;
}
bool Client::ProcessPacket(Packet _packettype)
{
	switch (_packettype)
	{
		case P_ChatMessage:
		{
			std::string Message;
			if (!GetString(Message))
				return false;
			std::cout << "\n" << __LINE__ << " Client Sink buffer = " << Message << " of length " << Message.size() << std::endl;
			break;
		}
		default:
		{
			std::cout << __LINE__ << " Unrecognized Packet: " << _packettype << std::endl;
			break;
		}
	}
	return true;
}

bool Client::CloseConnection()
{
	if (closesocket(Connection) == SOCKET_ERROR)
	{
		if (WSAGetLastError() == WSAENOTSOCK)
			return true;

		std::string ErrorMessage = "Failed to close the socket. Winsock Error: " + std::to_string(WSAGetLastError()) + ".";
		MessageBoxA(NULL, ErrorMessage.c_str(), "Error", MB_OK | MB_ICONERROR);
		return false;
	}
	return true;
}


void Client::ClientThread()
{
	Packet packettype;
	while (true)
	{
		if (!clientptr->GetPacketType(packettype)) // receive packet type
			break;

		std::cout << __LINE__ << " Packet: " << packettype << std::endl;

		if (!clientptr->ProcessPacket(packettype)) // If the packet is not properly processed
		{
			break; // break out of client handler loop
		}

	}
	std::cout << __LINE__ << " Lost connection to the server. " << std::endl;
	if (clientptr->CloseConnection())
	{
		std::cout << "Socket to the server was closed successfully." << std::endl;
	}
	else
	{
		std::cout << "Socket was not able to be closed." << std::endl;
	}
}
