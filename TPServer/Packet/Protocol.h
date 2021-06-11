#pragma once

#include <cstdint>

enum class PROTOCOL : uint16_t
{
	// Error 0~999
	TP_ERROR = 0,

	// C2S Req 1000~1999
	REQ_LOGIN = 1000,
	REQ_MOVE = 1001,

	// S2C Res 2000~2999
	GAME_ROOM_OBJ = 2000,

	// S2C Broadcast 3000~3999
	ENTER_GAME_ROOM = 3000,
	EXIT_GAME_ROOM = 3001,
	MOVE_LOCATION = 3002,

	END_OF_PACKET = 20000,
};