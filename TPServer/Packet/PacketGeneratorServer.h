#pragma once

#include "../Util/TSingleton.h"
#include "PacketGenerator.h"
#include "../Object/ObjUser.h"

class GameRoom;

class PacketGeneratorServer : public PacketGenerator, public TSingleton<PacketGeneratorServer>
{
public:
	Packet CreateError(Session* const owner, const wchar_t* const message);
	Packet CreateResLogin(Session* const owner, const GameRoom& gameRoom);
	Packet CreateResRoundTripTime(Session* const owner, const GameRoom& gameRoom);
	Packet CreateBcastEnterGameRoom(Session* const owner, const shared_ptr<ObjUser> objUser);
	Packet CreateBcastExitGameRoom(const shared_ptr<ObjUser> objUser);
	Packet CreateBcastMove(Session* const owner, const TB_ReqMove& reqMove);
	Packet CreateBcastLocationSync(Session* const owner, const TB_ReqLocationSync& reqLocationSync);
	Packet CreateBcastAction(Session* const owner, const TB_ReqAction& reqAction);
	Packet CreateBcastHit(const char* const userId);
	Packet CreateBcastRotate(Session* const owner, const TB_ReqRotate& reqRotate);
};