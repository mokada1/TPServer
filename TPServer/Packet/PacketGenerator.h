#pragma once

#include "../Util/TSingleton.h"
#include "Packet.h"
#include "../Object/ObjUser.h"

class GameRoom;

class PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet Parse(Session* const owner, char* const buffer, const size_t bytesTransferred);

	Packet CreateError(Session* const owner, const wchar_t* const message);
	Packet CreateGameRoomObj(Session* const owner, const GameRoom& gameRoom);
	Packet CreateEnterGameRoom(Session* const owner, const shared_ptr<ObjUser> objUser);
	Packet CreateExitGameRoom(const shared_ptr<ObjUser> objUser);
	Packet CreateMoveLocation(Session* const owner, const flatbuffers::Vector<const ST_Vec3*>& locationList);
	
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