#pragma once

#include <cstdint>

enum class PacketCastType : uint8_t
{
	UNICAST,
	BROADCAST,
	MULTICAST,
};