#pragma once

#include "TSingleton.h"
#include <WinSock2.h>
#include "Packet.h"

class Session;

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void Process(Session* const owner, char* const buffer, const DWORD bytesTransferred);	
	void SendPacket(const Packet* packet);
	void SendPacket(const Packet* const packet, const Session* const session);
	void SendPacket(const Packet* const packet, const SOCKET& clntSock);

private:
	void SendPacketAll(const Packet* const packet, bool isIgnoreCastGroup);
};