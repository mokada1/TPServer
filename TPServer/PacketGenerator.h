#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include "ObjUser.h"

class GameRoom;

class PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet* Parse(char* const buffer, DWORD bytesTransferred, Session* const owner);

	Packet* CreateError(Session* const owner, const wchar_t* const message);
	Packet* CreateGameRoomObj(Session* const owner, const GameRoom& gameRoom);
	Packet* CreateEnterGameRoom(const shared_ptr<ObjUser> objUser, vector<Session*> packetCastGroup);
	Packet* CreateExitGameRoom(const shared_ptr<ObjUser> objUser);
	
private:
	void SetHeaderOfBuff(char* const buffer, PROTOCOL header);
	Packet* CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header, Session* const owner);
	Packet* CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header, Session* const owner, PACKET_CAST_TYPE packetCastType);
	Packet* CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header, Session* const owner, PACKET_CAST_TYPE packetCastType, vector<Session*> packetCastGroup);
};