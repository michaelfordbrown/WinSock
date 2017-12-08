#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib") //Required for WinSock
#include <WinSock2.h> //For win sockets
#include <string> //For std:string
#include <iostream> //For std::cout, std::endl, std::cin.getline

enum Packet
{
	P_ChatMessage,
	P_Test
};

class Client
{
public:
	Client(std::string IP, int PORT);
	bool Connect();
	bool CloseConnection();
	bool SendString(std::string & _string);
private:
	bool ProcessPacket(Packet _packettype);
	static void ClientThread();

	//Sending Funcs
	bool SendInt(int _int);
	bool SendPacketType(Packet _packettype);
	
	//Getting Funcs
	bool GetInt(int & _int);
	bool GetPacketType(Packet & _packettype);
	bool GetString(std::string & _string);

private:
	SOCKET Connection;//This client's connection to the server

					  //In the Internet address family, the SOCKADDR_IN structure is used by Windows Sockets to specify a local or remote endpoint address to which to connect a socket.

	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
};

static Client * clientptr;