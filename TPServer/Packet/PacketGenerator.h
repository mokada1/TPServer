#pragma once

#include "../Util/TSingleton.h"
#include "Packet.h"
#include "../Object/ObjUser.h"

class GameRoom;

class PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet Parse(Session* const owner, char* const buffer, const size_t recvBytes);

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
	
private:	
	Packet CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner, PACKET_CAST_TYPE packetCastType);
	Packet CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner);
	Packet CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner, PACKET_CAST_TYPE packetCastType, vector<Session*> packetCastGroup);
	PROTOCOL GetHeaderByBuff(char* const buffer);
	PROTOCOL GetEndOfPacket(char* const buffer, const size_t packetSize);
	void SetHeaderOfBuff(char* const buffer, PROTOCOL header);
	void SetEndOfBuff(char* const buffer, const size_t buffSize);
	bool IsValidHeader(const PROTOCOL protocol);
	bool IsValidEndOfPacket(const PROTOCOL protocol);
};