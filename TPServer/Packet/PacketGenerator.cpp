#include "PacketGenerator.h"
#include "../GameRoom/GameRoom.h"
#include "../GameRoom/GameRoomService.h"
#include "../Session/Session.h"
#include <iostream>

using namespace std;

Packet PacketGenerator::Parse(Session* const owner, char* const buffer, const size_t recvBytes)
{	
	if (recvBytes == 0)
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
		if (owner->GetPacketSize() + recvBytes > MAX_BUFF_SIZE)
		{
			cout << "Error:Invalid PacketSize" << endl;
			owner->ClearBuff();
			return Packet();
		}

		owner->AddToBuff(buffer, recvBytes);
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
		}
		else
		{
			// 패킷 완성
			finishedBuffer = ownerBuff;
			finishedPacketSize = ownerPacketSize;
			owner->ClearBuff(false);
			isDAllocBuf = true;
		}
	}
	else
	{
		if (recvBytes < PACKET_HEAD_SIZE)
		{
			owner->AddToBuff(buffer, recvBytes);
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

			const auto endOfPacket = PacketGenerator::GetInstance().GetEndOfPacket(buffer, recvBytes);
			if (!IsValidEndOfPacket(endOfPacket))
			{
				// 잘못된 EndOfPacket일 경우 패킷 처리 안함
				if (recvBytes == MAX_BUFF_SIZE)
				{
					cout << "Error:Invalid EndOfPacket" << endl;
					return Packet();
				}
				owner->AddToBuff(buffer, recvBytes);
			}
			else
			{
				// 패킷 완성
				finishedBuffer = buffer;
				finishedPacketSize = recvBytes;
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
	char nMessage[ERR_MSG_SIZE];
	TPUtil::GetInstance().WCharToChar(nMessage, ERR_MSG_SIZE, message);
	
	flatbuffers::FlatBufferBuilder fbb;
	auto offsetMessage = fbb.CreateString(nMessage);

	fbb.Finish(CreateTB_Error(fbb, offsetMessage));

	return CreatePacket(PROTOCOL::TP_ERROR, fbb, owner);
}

Packet PacketGenerator::CreateResLogin(Session* const owner, const GameRoom& gameRoom)
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

	fbb.Finish(CreateTB_ResLogin(fbb, offsetObjUserList));

	return CreatePacket(PROTOCOL::RES_LOGIN, fbb, owner);
}

Packet PacketGenerator::CreateResRoundTripTime(Session* const owner, const GameRoom& gameRoom)
{
	const auto currentTimeMs = TPUtil::GetInstance().TimeSinceEpochMs();
	flatbuffers::FlatBufferBuilder fbb;
	fbb.Finish(CreateTB_ResRoundTripTime(fbb, currentTimeMs, gameRoom.GetAvgRttMs()));
	return CreatePacket(PROTOCOL::RES_ROUND_TRIP_TIME, fbb, owner);
}

Packet PacketGenerator::CreateBcastEnterGameRoom(Session* const owner, const shared_ptr<ObjUser> objUser)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.push_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	fbb.Finish(CreateTB_BcastEnterGameRoom(fbb, objUser->Serialize(fbb)));

	return CreatePacket(PROTOCOL::BCAST_ENTER_GAME_ROOM, fbb, nullptr, PACKET_CAST_TYPE::BROADCAST, packetCastGroup);
}

Packet PacketGenerator::CreateBcastExitGameRoom(const shared_ptr<ObjUser> objUser)
{
	flatbuffers::FlatBufferBuilder fbb;
	
	fbb.Finish(CreateTB_BcastExitGameRoom(fbb, objUser->Serialize(fbb)));

	return CreatePacket(PROTOCOL::BCAST_EXIT_GAME_ROOM, fbb, nullptr, PACKET_CAST_TYPE::BROADCAST);
}

Packet PacketGenerator::CreateBcastMove(Session* const owner, const TB_ReqMove& reqMove)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.push_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(owner->GetCUserId());
	auto inputMove = reqMove.InputMove();
	auto offsetOperation = reqMove.Operation();
	auto offsetInputMove = CreateTB_InputMove(fbb,
		inputMove->ForwardVector(),
		inputMove->RightVector(),
		inputMove->MoveForward(),
		inputMove->MoveRight()
	);

	fbb.Finish(CreateTB_BcastMove(fbb, offsetUserId, offsetOperation, offsetInputMove));

	return CreatePacket(PROTOCOL::BCAST_MOVE, fbb, nullptr, PACKET_CAST_TYPE::BROADCAST, packetCastGroup);
}

Packet PacketGenerator::CreateBcastLocationSync(Session* const owner, const TB_ReqLocationSync& reqLocationSync)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.push_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(owner->GetCUserId());
	auto offsetLocation = reqLocationSync.Location();
	
	fbb.Finish(CreateTB_BcastLocationSync(fbb, offsetUserId, offsetLocation));

	return CreatePacket(PROTOCOL::BCAST_LOCATION_SYNC, fbb, nullptr, PACKET_CAST_TYPE::BROADCAST, packetCastGroup);
}

Packet PacketGenerator::CreateBcastInputAction(Session* const owner, const TB_ReqInputAction& reqInputAction)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.push_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(owner->GetCUserId());
	auto offsetOperation = reqInputAction.Operation();

	fbb.Finish(CreateTB_BcastInputAction(fbb, offsetUserId, offsetOperation));

	return CreatePacket(PROTOCOL::BCAST_INPUT_ACTION, fbb, nullptr, PACKET_CAST_TYPE::BROADCAST, packetCastGroup);
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
	case PROTOCOL::REQ_ROUND_TRIP_TIME:
	case PROTOCOL::REQ_MOVE:
	case PROTOCOL::REQ_LOCATION_SYNC:
	case PROTOCOL::REQ_INPUT_ACTION:
		return true;
	}
	return false;
}

bool PacketGenerator::IsValidEndOfPacket(const PROTOCOL protocol)
{
	return protocol == PROTOCOL::END_OF_PACKET;
}
