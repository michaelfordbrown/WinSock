#include "Client.h"

bool Client::SendInt(int _int)
{
	int RetnCheck = send(Connection, (char*)&_int, sizeof(_int), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::GetInt(int & _int)
{
	int RetnCheck = recv(Connection, (char*)& _int, sizeof(_int), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::SendPacketType(Packet _packettype)
{
	std::cout << __LINE__ << "Send PacketType\n";

	int RetnCheck = send(Connection, (char*)&_packettype, sizeof(_packettype), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::GetPacketType(Packet & _packettype)
{
	int RetnCheck = recv(Connection, (char*)& _packettype, sizeof(_packettype), NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;
	return true;
}

bool Client::SendString(std::string & _string)
{
	std::cout << __LINE__ << "Send String\n";

	if (!SendPacketType(P_ChatMessage))
		return false;

	std::cout << __LINE__ << " Packet Type Sent\n";

	int bufferlength = _string.size();
	if (!SendInt(bufferlength))
		return false;

	std::cout << __LINE__ << " Buffer Length Sent\n";

	int RetnCheck = send(Connection, _string.c_str(), bufferlength, NULL);
	if (RetnCheck == SOCKET_ERROR)
		return false;

	std::cout << __LINE__ << " String Sent\n";

	return true;
}

bool Client::GetString(std::string & _string)
{
	std::cout << __LINE__ << " GetString\n";
	int bufferlength;
	if (!GetInt(bufferlength))
		return false;

	std::cout << __LINE__ << " GetString: bufferlength>  " << bufferlength << "\n";

	char * buffer = new char[bufferlength + 1];
	buffer[bufferlength] = '\0';

	std::cout << __LINE__ << " GetString\n";

	int RetnCheck = recv(Connection, buffer, bufferlength, NULL);
	_string = buffer;
	delete[] buffer;

	if (RetnCheck == SOCKET_ERROR)
	{
		std::cout << __LINE__ << " GetString\n";
		return false;
	}
	return true;
}
