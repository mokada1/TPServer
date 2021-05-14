#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include "TPResult.h"

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

private:
	TPResult* ProcReqLogin(const Packet& packet);
	TPResult* ProcReqMove(const Packet& packet);
};