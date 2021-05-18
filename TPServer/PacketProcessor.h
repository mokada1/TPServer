#pragma once

#include "TSingleton.h"
#include <WinSock2.h>
#include "Packet.h"
#include "SessionPool.h"

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void Process(Session* const owner, char* const buffer, const DWORD bytesTransferred);	
	void SendPacket(const Packet* const packet);
	void SendPacket(const Packet* const packet, const Session* const session);
	void SendPacket(const Packet* const packet, const SOCKET& clntSock);
	void SendPacketAll(const Packet* const packet);
};