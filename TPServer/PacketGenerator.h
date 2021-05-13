#pragma once

#include "TSingleton.h"
#include "Packet.h"

class PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet CreateError(const wchar_t* message);
};