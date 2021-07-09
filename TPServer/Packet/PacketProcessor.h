#pragma once

#include "../Util/TSingleton.h"
#include <WinSock2.h>
#include <concurrent_queue.h>

class Packet;
class Session;

using namespace Concurrency;

class PacketProcessor : public TSingleton<PacketProcessor>
{
public:
	void Parse(Session* const owner, char* const buffer, const size_t recvBytes);
	bool ProcRecvPacket();
	void SendPacket(const Packet& packet);
	void SendPacket(const Packet& packet, const Session* const session);
	void SendPacket(const Packet& packet, const SOCKET& clntSock);

private:
	void SendPacketAll(const Packet& packet, bool isIgnoreCastGroup);

	concurrent_queue<Packet*> packetRecvQueue;
};