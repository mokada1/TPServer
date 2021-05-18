#include "PacketGenerator.h"
#include "TPDefine.h"
#include "TPUtil.h"

Packet* PacketGenerator::Parse(char* const buffer, DWORD bytesTransferred, Session* const owner)
{
	unsigned char byte1 = buffer[0];
	unsigned char byte2 = buffer[1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	PROTOCOL header = static_cast<PROTOCOL>(headerInt16);
	return new Packet(buffer, bytesTransferred, header, owner, false, false);
}

Packet* PacketGenerator::CreateError(Session* const owner, const wchar_t* const message)
{
	char hMessage[MAX_DATA_SIZE];
	TPUtil::GetInstance().WCharToChar(hMessage, MAX_DATA_SIZE, message);
	
	auto buffer = new char[MAX_DATA_SIZE];
	memset(buffer, 0, MAX_DATA_SIZE);

	PROTOCOL header = PROTOCOL::TP_ERROR;
	SetHeaderOfBuff(buffer, header);

	flatbuffers::FlatBufferBuilder fbb;
	auto offsetMessage = fbb.CreateString(hMessage);

	fbb.Finish(CreateTB_Error(fbb, offsetMessage));

	return CreatePacket(buffer, header, owner, false, fbb);
}

Packet* PacketGenerator::CreateBCGameRoom(Session* const owner, const GameRoom& gameRoom)
{
	auto buffer = new char[MAX_DATA_SIZE];
	memset(buffer, 0, MAX_DATA_SIZE);

	PROTOCOL header = PROTOCOL::BC_GAME_ROOM;
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

	fbb.Finish(CreateTB_BCGameRoom(fbb, offsetObjUserList));

	return CreatePacket(buffer, header, owner, true, fbb);
}

void PacketGenerator::SetHeaderOfBuff(char* const buffer, PROTOCOL header)
{
	uint16_t protoInt16 = static_cast<uint16_t>(header);
	unsigned char byte1 = protoInt16 & 0xFF;
	unsigned char byte2 = protoInt16 >> 8;
	buffer[0] = byte1;
	buffer[1] = byte2;
}

Packet* PacketGenerator::CreatePacket(char* const buffer, PROTOCOL header, Session* const owner, bool isBcast, flatbuffers::FlatBufferBuilder& _fbb)
{
	auto bp = _fbb.GetBufferPointer();
	auto bSize = _fbb.GetSize();
	memcpy(&buffer[PACKET_HEAD_SIZE], bp, bSize);
	const int DATA_SIZE = PACKET_HEAD_SIZE + bSize;
	return new Packet(buffer, DATA_SIZE, header, owner, isBcast);
}
