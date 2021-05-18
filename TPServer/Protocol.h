#pragma once

#include <cstdint>

enum class PROTOCOL : uint16_t
{
	TP_ERROR = 0,
	REQ_LOGIN = 100,
	RES_LOGIN = 101,
	REQ_MOVE = 200,
	RES_MOVE = 201,	
	GAME_ROOM_OBJ = 1000,
	ENTER_GAME_ROOM = 1001,
	EXIT_GAME_ROOM = 1002,
};