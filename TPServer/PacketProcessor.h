#pragma once

#include "TSingleton.h"
#include <WinSock2.h>

class Packet;
class Session;

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void Process(Session* const owner, char* const buffer, const size_t bytesTransferred);
	void SendPacket(const Packet& packet);
	void SendPacket(const Packet& packet, const Session* const session);
	void SendPacket(const Packet& packet, const SOCKET& clntSock);

private:
	void SendPacketAll(const Packet& packet, bool isIgnoreCastGroup);
};