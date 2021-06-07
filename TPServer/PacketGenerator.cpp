#include "PacketGenerator.h"
#include "GameRoom.h"
#include "Session.h"
#include <iostream>

using namespace std;

Packet PacketGenerator::Parse(Session* const owner, char* const buffer, size_t bytesTransferred)
{	
	if (bytesTransferred == 0)
	{
		return Packet();
	}

	char* finishedBuffer = nullptr;
	size_t finishedPacketSize = 0;
	bool isDAllocBuf = false;

	auto ownerBuff = owner->GetBuffer();
	if (ownerBuff)
	{
		// 최대 버퍼 크기를 넘는 경우 패킷 처리 안함
		if (owner->GetPacketSize() + bytesTransferred > MAX_BUFF_SIZE)
		{
			cout << "Error:Invalid PacketSize" << endl;
			owner->ClearBuff();
			return Packet();
		}

		owner->AddToBuff(buffer, bytesTransferred);
		ownerBuff = owner->GetBuffer();
		const auto ownerPacketSize = owner->GetPacketSize();

		const auto header = PacketGenerator::GetInstance().GetHeaderByBuff(ownerBuff);
		// 잘못된 Header일 경우 패킷 처리 안함
		if (!IsValidHeader(header))
		{
			cout << "Error:Invalid Header" << endl;
			owner->ClearBuff();
			return Packet();
		}

		const auto endOfPacket = PacketGenerator::GetInstance().GetEndOfPacket(ownerBuff, ownerPacketSize);
		if (!IsValidEndOfPacket(endOfPacket))
		{
			// 잘못된 EndOfPacket일 경우 패킷 처리 안함
			if (ownerPacketSize == MAX_BUFF_SIZE)
			{
				cout << "Error:Invalid EndOfPacket" << endl;
				owner->ClearBuff();
				return Packet();
			}
			cout << "패킷 조립중..." << endl;
		}
		else
		{
			// 패킷 완성
			finishedBuffer = ownerBuff;
			finishedPacketSize = ownerPacketSize;
			owner->ClearBuff(false);
			isDAllocBuf = true;
			cout << "패킷 완성!" << endl;
		}
	}
	else
	{
		if (bytesTransferred < PACKET_HEAD_SIZE)
		{
			owner->AddToBuff(buffer, bytesTransferred);
			cout << "패킷 조립중..." << endl;
		}
		else
		{
			const auto header = PacketGenerator::GetInstance().GetHeaderByBuff(buffer);
			// 잘못된 Header일 경우 패킷 처리 안함
			if (!IsValidHeader(header))
			{
				cout << "Error:Invalid Header" << endl;
				return Packet();
			}

			const auto endOfPacket = PacketGenerator::GetInstance().GetEndOfPacket(buffer, bytesTransferred);
			if (!IsValidEndOfPacket(endOfPacket))
			{
				// 잘못된 EndOfPacket일 경우 패킷 처리 안함
				if (bytesTransferred == MAX_BUFF_SIZE)
				{
					cout << "Error:Invalid EndOfPacket" << endl;
					return Packet();
				}
				owner->AddToBuff(buffer, bytesTransferred);
				cout << "패킷 조립중..." << endl;
			}
			else
			{
				// 패킷 완성
				finishedBuffer = buffer;
				finishedPacketSize = bytesTransferred;
				cout << "패킷 완성!" << endl;
			}
		}
	}

	// 패킷이 완성되지 않았으면 빈 패킷 리턴
	if (!finishedBuffer)
	{
		return Packet();
	}

	// 완성된 패킷 리턴
	PROTOCOL header = GetHeaderByBuff(finishedBuffer);
	auto packetInfo = PacketInfo(finishedBuffer, finishedPacketSize, header);
	auto packetSubInfo = PacketSubInfo(owner, PACKET_CAST_TYPE::UNICAST, vector<Session*>(), isDAllocBuf);
	return Packet(packetInfo, packetSubInfo);
}

Packet PacketGenerator::CreateError(Session* const owner, const wchar_t* const message)
{
	char hMessage[ERR_MSG_SIZE];
	TPUtil::GetInstance().WCharToChar(hMessage, ERR_MSG_SIZE, message);
	
	flatbuffers::FlatBufferBuilder fbb;
	auto offsetMessage = fbb.CreateString(hMessage);

	fbb.Finish(CreateTB_Error(fbb, offsetMessage));

	return CreatePacket(PROTOCOL::TP_ERROR, fbb, owner);
}

Packet PacketGenerator::CreateGameRoomObj(Session* const owner, const GameRoom& gameRoom)
{
	flatbuffers::FlatBufferBuilder fbb;

	flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<TB_ObjUser>>> offsetObjUserList = 0;	
	auto objUserMap = gameRoom.GetObjUserMap();
	if (!objUserMap.empty())
	{
		vector<flatbuffers::Offset<TB_ObjUser>> offsetListObjUser;
		for (auto& data : objUserMap)
		{
			auto obj = data.second;
			auto offset = obj->Serialize(fbb);
			offsetListObjUser.push_back(offset);
		}
		offsetObjUserList = fbb.CreateVector(offsetListObjUser);
	}

	fbb.Finish(CreateTB_GameRoomObj(fbb, offsetObjUserList));

	return CreatePacket(PROTOCOL::GAME_ROOM_OBJ, fbb, owner);
}

Packet PacketGenerator::CreateEnterGameRoom(Session* const owner, const shared_ptr<ObjUser> objUser)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.push_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	fbb.Finish(CreateTB_EnterGameRoom(fbb, objUser->Serialize(fbb)));

	return CreatePacket(PROTOCOL::ENTER_GAME_ROOM, fbb, nullptr, PACKET_CAST_TYPE::BROADCAST, packetCastGroup);
}

Packet PacketGenerator::CreateExitGameRoom(const shared_ptr<ObjUser> objUser)
{
	flatbuffers::FlatBufferBuilder fbb;
	
	fbb.Finish(CreateTB_ExitGameRoom(fbb, objUser->Serialize(fbb)));

	return CreatePacket(PROTOCOL::EXIT_GAME_ROOM, fbb, nullptr, PACKET_CAST_TYPE::BROADCAST);
}

Packet PacketGenerator::CreateMoveLocation(Session* const owner, const flatbuffers::Vector<const ST_Vec3*>& locationList)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.push_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	char hUserId[SIZE_USER_USER_ID];
	TPUtil::GetInstance().WCharToChar(hUserId, SIZE_USER_USER_ID, owner->GetUserId());
	auto offsetUserId = fbb.CreateString(hUserId);

	flatbuffers::Offset<flatbuffers::Vector<const ST_Vec3*>> offsetLocationList = 0;
	if (locationList.size() > 0)
	{
		vector<ST_Vec3> offsetListLocation;
		for (auto it = locationList.begin(); it != locationList.end(); ++it)
		{
			offsetListLocation.push_back(*(*it));
		}
		offsetLocationList = fbb.CreateVectorOfStructs(offsetListLocation);
	}	

	fbb.Finish(CreateTB_MoveLocation(fbb, offsetUserId, offsetLocationList));

	return CreatePacket(PROTOCOL::MOVE_LOCATION, fbb, nullptr, PACKET_CAST_TYPE::BROADCAST, packetCastGroup);
}

Packet PacketGenerator::CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner)
{
	return CreatePacket(header, _fbb, owner, PACKET_CAST_TYPE::UNICAST, vector<Session*>());
}

Packet PacketGenerator::CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner, PACKET_CAST_TYPE packetCastType)
{
	return CreatePacket(header, _fbb, owner, packetCastType, vector<Session*>());
}

Packet PacketGenerator::CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner, PACKET_CAST_TYPE packetCastType, vector<Session*> packetCastGroup)
{
	auto bp = _fbb.GetBufferPointer();
	auto bSize = static_cast<size_t>(_fbb.GetSize());

	const size_t INTER_BUFF_DATA_SIZE = PACKET_HEAD_SIZE + bSize;
	const size_t BUFF_DATA_SIZE = PACKET_HEAD_SIZE + bSize + PACKET_END_SIZE;

	auto buffer = new char[BUFF_DATA_SIZE];
	memset(buffer, 0, BUFF_DATA_SIZE);

	SetHeaderOfBuff(buffer, header);
	memcpy(&buffer[PACKET_HEAD_SIZE], bp, bSize);
	SetEndOfBuff(buffer, INTER_BUFF_DATA_SIZE);

	auto packetInfo = PacketInfo(buffer, BUFF_DATA_SIZE, header);
	auto packetSubInfo = PacketSubInfo(owner, packetCastType, packetCastGroup);
	return Packet(packetInfo, packetSubInfo);
}

PROTOCOL PacketGenerator::GetHeaderByBuff(char* const buffer)
{
	unsigned char byte1 = buffer[0];
	unsigned char byte2 = buffer[1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	return static_cast<PROTOCOL>(headerInt16);
}

PROTOCOL PacketGenerator::GetEndOfPacket(char* const buffer, const size_t packetSize)
{
	unsigned char byte1 = buffer[packetSize - 2];
	unsigned char byte2 = buffer[packetSize - 1];
	uint16_t headerInt16 = byte1 | byte2 << 8;
	return static_cast<PROTOCOL>(headerInt16);
}

void PacketGenerator::SetHeaderOfBuff(char* const buffer, PROTOCOL header)
{
	uint16_t protoInt16 = static_cast<uint16_t>(header);
	unsigned char byte1 = protoInt16 & 0xFF;
	unsigned char byte2 = protoInt16 >> 8;
	buffer[0] = byte1;
	buffer[1] = byte2;
}

void PacketGenerator::SetEndOfBuff(char* const buffer, const size_t buffSize)
{
	uint16_t protoInt16 = static_cast<uint16_t>(PROTOCOL::END_OF_PACKET);
	unsigned char byte1 = protoInt16 & 0xFF;
	unsigned char byte2 = protoInt16 >> 8;
	buffer[buffSize] = byte1;
	buffer[buffSize+1] = byte2;
}

bool PacketGenerator::IsValidHeader(const PROTOCOL protocol)
{
	switch (protocol)
	{
	case PROTOCOL::TP_ERROR:
	case PROTOCOL::REQ_LOGIN:
	case PROTOCOL::REQ_MOVE:
	case PROTOCOL::GAME_ROOM_OBJ:
	case PROTOCOL::ENTER_GAME_ROOM:
	case PROTOCOL::EXIT_GAME_ROOM:
	case PROTOCOL::MOVE_LOCATION:
		return true;
	}
	return false;
}

bool PacketGenerator::IsValidEndOfPacket(const PROTOCOL protocol)
{
	return protocol == PROTOCOL::END_OF_PACKET;
}
