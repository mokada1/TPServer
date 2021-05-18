#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include "ObjUser.h"
#include "GameRoom.h"

class PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet* Parse(char* const buffer, DWORD bytesTransferred, Session* const owner);

	Packet* CreateError(Session* const owner, const wchar_t* const message);
	Packet* CreateBCGameRoom(Session* const owner, const GameRoom& gameRoom);
	
private:
	void SetHeaderOfBuff(char* const buffer, PROTOCOL header);
	Packet* CreatePacket(char* const buffer, PROTOCOL header, Session* const owner, bool isBcast, flatbuffers::FlatBufferBuilder& _fbb);
};