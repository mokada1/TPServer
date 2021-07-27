#pragma once

#include "../Util/Unconstructible.h"
#include <WinSock2.h>

class Packet;
class Session;

class PacketProcessor : public Unconstructible
{
public:
	static PacketProcessor& GetInstance()
	{
		static PacketProcessor* _instance = new PacketProcessor();
		return *_instance;
	}

	void Parse(Session* const owner, char* const buffer, const size_t recvBytes);
	void SendPacket(const Packet& packet);
	void SendPacket(const Packet& packet, const Session* const session);
	void SendPacket(const Packet& packet, const SOCKET& clntSock);

private:
	PacketProcessor() {}

	void SendPacketAll(const Packet& packet, bool isIgnoreCastGroup);
};