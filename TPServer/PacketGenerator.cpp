#include "PacketGenerator.h"
#include "GameRoom.h"

Packet* PacketGenerator::Parse(char* const buffer, DWORD bytesTransferred, Session* const owner)
{
	unsigned char byte1 = buffer[0];
	unsigned char byte2 = buffer[1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	PROTOCOL header = static_cast<PROTOCOL>(headerInt16);
	auto packetInfo = PacketInfo(buffer, bytesTransferred, header);
	auto packetSubInfo = PacketSubInfo(owner, PACKET_CAST_TYPE::UNICAST, vector<Session*>(), false);
	return new Packet(packetInfo, packetSubInfo);
}

Packet* PacketGenerator::CreateError(Session* const owner, const wchar_t* const message)
{
	char hMessage[BUFSIZE];
	TPUtil::GetInstance().WCharToChar(hMessage, BUFSIZE, message);
	
	auto buffer = new char[BUFSIZE];
	memset(buffer, 0, BUFSIZE);

	PROTOCOL header = PROTOCOL::TP_ERROR;
	SetHeaderOfBuff(buffer, header);

	flatbuffers::FlatBufferBuilder fbb;
	auto offsetMessage = fbb.CreateString(hMessage);

	fbb.Finish(CreateTB_Error(fbb, offsetMessage));

	return CreatePacket(fbb, buffer, header, owner);
}

Packet* PacketGenerator::CreateGameRoomObj(Session* const owner, const GameRoom& gameRoom)
{
	auto buffer = new char[BUFSIZE];
	memset(buffer, 0, BUFSIZE);

	PROTOCOL header = PROTOCOL::GAME_ROOM_OBJ;
	SetHeaderOfBuff(buffer, header);

	flatbuffers::FlatBufferBuilder fbb;

	vector<flatbuffers::Offset<TB_ObjUser>> offsetListObjUser;
	auto objUserMap = gameRoom.GetObjUserMap();
	for (auto& data : objUserMap)
	{
		auto obj = data.second;
		auto offset = obj->Serialize(fbb);
		offsetListObjUser.push_back(offset);
	}
	auto offsetObjUserList = objUserMap.empty() ? 0 : fbb.CreateVector(offsetListObjUser);

	fbb.Finish(CreateTB_GameRoomObj(fbb, offsetObjUserList));

	return CreatePacket(fbb, buffer, header, owner);
}

Packet* PacketGenerator::CreateEnterGameRoom(const shared_ptr<ObjUser> objUser, vector<Session*> packetCastGroup)
{
	auto buffer = new char[BUFSIZE];
	memset(buffer, 0, BUFSIZE);

	PROTOCOL header = PROTOCOL::ENTER_GAME_ROOM;
	SetHeaderOfBuff(buffer, header);

	flatbuffers::FlatBufferBuilder fbb;

	fbb.Finish(CreateTB_EnterGameRoom(fbb, objUser->Serialize(fbb)));

	return CreatePacket(fbb, buffer, header, nullptr, PACKET_CAST_TYPE::BROADCAST, packetCastGroup);
}

Packet* PacketGenerator::CreateExitGameRoom(const shared_ptr<ObjUser> objUser)
{
	auto buffer = new char[BUFSIZE];
	memset(buffer, 0, BUFSIZE);

	PROTOCOL header = PROTOCOL::EXIT_GAME_ROOM;
	SetHeaderOfBuff(buffer, header);

	flatbuffers::FlatBufferBuilder fbb;
	
	fbb.Finish(CreateTB_ExitGameRoom(fbb, objUser->Serialize(fbb)));

	return CreatePacket(fbb, buffer, header, nullptr, PACKET_CAST_TYPE::BROADCAST);
}

void PacketGenerator::SetHeaderOfBuff(char* const buffer, PROTOCOL header)
{
	uint16_t protoInt16 = static_cast<uint16_t>(header);
	unsigned char byte1 = protoInt16 & 0xFF;
	unsigned char byte2 = protoInt16 >> 8;
	buffer[0] = byte1;
	buffer[1] = byte2;
}

Packet* PacketGenerator::CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header, Session* const owner)
{
	return CreatePacket(_fbb, buffer, header, owner, PACKET_CAST_TYPE::UNICAST, vector<Session*>());
}

Packet* PacketGenerator::CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header, Session* const owner, PACKET_CAST_TYPE packetCastType)
{
	return CreatePacket(_fbb, buffer, header, owner, packetCastType, vector<Session*>());
}

Packet* PacketGenerator::CreatePacket(flatbuffers::FlatBufferBuilder& _fbb, char* const buffer, PROTOCOL header, Session* const owner, PACKET_CAST_TYPE packetCastType, vector<Session*> packetCastGroup)
{
	auto bp = _fbb.GetBufferPointer();
	auto bSize = _fbb.GetSize();
	memcpy(&buffer[PACKET_HEAD_SIZE], bp, bSize);
	const int BUFFER_SIZE = PACKET_HEAD_SIZE + bSize;
	auto packetInfo = PacketInfo(buffer, BUFFER_SIZE, header);
	auto packetSubInfo = PacketSubInfo(owner, packetCastType, packetCastGroup);
	return new Packet(packetInfo, packetSubInfo);
}
