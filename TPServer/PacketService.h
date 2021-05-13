#pragma once

#include "TSingleton.h"
#include "Packet.h"

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

private:
	Packet ReqLogin(const Packet& packet);
	void ReqMove(const Packet& packet);
};