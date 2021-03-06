#pragma once

#include "../Util/Unconstructible.h"
#include "PacketGenerator.h"
#include "../Object/ObjUser.h"

class GameRoom;

class PacketGeneratorServer : public PacketGenerator, public Unconstructible
{
public:
	static PacketGeneratorServer& GetInstance()
	{
		static PacketGeneratorServer* _instance = new PacketGeneratorServer();
		return *_instance;
	}

	Packet CreateError(Session* const owner, const wchar_t* const message);
	Packet CreateResLogin(Session* const owner, const GameRoom& gameRoom);
	Packet CreateResRoundTripTime(Session* const owner, const int64_t avgRttMs);
	Packet CreateBcastEnterGameRoom(Session* const owner, const shared_ptr<ObjUser> objUser);
	Packet CreateBcastExitGameRoom(const shared_ptr<ObjUser> objUser);
	Packet CreateBcastMove(Session* const owner, const TB_ReqMove& reqMove);
	Packet CreateBcastLocationSync(Session* const owner, const TB_ReqLocationSync& reqLocationSync);
	Packet CreateBcastAction(Session* const owner, const TB_ReqAction& reqAction);
	Packet CreateBcastHit(const vector<shared_ptr<ObjUser>>& hitList);
	Packet CreateBcastRotationSync(Session* const owner, const TB_ReqRotationSync& reqRotationSync);

private:
	PacketGeneratorServer() {}
};