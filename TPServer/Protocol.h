#pragma once

#include <cstdint>

enum class PROTOCOL : uint16_t
{
	TP_ERROR = 0,
	REQ_LOGIN = 100,
	RES_LOGIN = 101,
	REQ_MOVE = 200,
	RES_MOVE = 201,
};