#pragma once

#include "TSingleton.h"
#include <WinSock2.h>
#include "Packet.h"

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void Process(Session& owner, const char* datas, const DWORD bytesTransferred);
	void SendPacketAll(const Packet& packet);
	void SendPacket(const Packet& packet, const SOCKET& clntSock);		

private:	
	Packet Parse(const char* datas, const DWORD bytesTransferred);
};