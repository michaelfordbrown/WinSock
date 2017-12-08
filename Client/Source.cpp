//Client
#pragma comment(lib, "ws2_32.lib")
/*
Windows Sockets 2 (Winsock) enables programmers to create advanced Internet, intranet, and other network-capable applications to transmit application data across the wire, independent of the network protocol being used. With Winsock, programmers are provided access to advanced Microsoft® Windows® networking capabilities such as multicast and Quality of Service (QoS).

Winsock follows the Windows Open System Architecture (WOSA) model; it defines a standard service provider interface (SPI) between the application programming interface (API), with its exported functions and the protocol stacks. It uses the sockets paradigm that was first popularized by Berkeley Software Distribution (BSD) UNIX. It was later adapted for Windows in Windows Sockets 1.1, with which Windows Sockets 2 applications are backward compatible. Winsock programming previously centered around TCP/IP. Some programming practices that worked with TCP/IP do not work with every protocol. As a result, the Windows Sockets 2 API adds functions where necessary to handle several protocols.
*/

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Client.h"

int main()
{
	Client myClient("127.0.0.1", 1111);

	if (!myClient.Connect())
	{
		std::cout << "Failed to connect to server..." << std::endl;
		system("pause");
		return 1;
	}

	std::string userinput; // string buffer to send message
	while (true)
	{		
		std::getline(std::cin, userinput); //Get line if user presses enter and fill buffer
		std::cout << __LINE__ << " user input: " << userinput.c_str() << std::endl;
		if (!myClient.SendString(userinput))
			break;
		Sleep(10);
	}
	return 0;

	system("pause");
	return 0;
}