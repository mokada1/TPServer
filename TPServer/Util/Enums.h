#pragma once

#include <cstdint>

enum class PACKET_CAST_TYPE : uint8_t
{
	UNICAST,
	BROADCAST,
	MULTICAST,
};