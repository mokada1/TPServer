#include "PacketGeneratorServer.h"
#include "../GameRoom/GameRoom.h"
#include "../GameRoom/GameRoomService.h"
#include "../Session/Session.h"
#include "../Util/TPLogger.h"

Packet PacketGeneratorServer::CreateError(Session* const owner, const wchar_t* const message)
{
	char nMessage[ERR_MSG_SIZE];
	TPUtil::GetInstance().WCharToChar(nMessage, ERR_MSG_SIZE, message);

	flatbuffers::FlatBufferBuilder fbb;
	auto offsetMessage = fbb.CreateString(nMessage);

	fbb.Finish(CreateTB_Error(fbb, offsetMessage));

	return CreatePacket(PROTOCOL::TP_ERROR, fbb, owner);
}

Packet PacketGeneratorServer::CreateResLogin(Session* const owner, const GameRoom& gameRoom)
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
			if (obj->GetCompCondition()->GetIsDied())
			{
				continue;
			}
			auto offset = obj->Serialize(fbb);
			offsetListObjUser.push_back(offset);
		}
		offsetObjUserList = fbb.CreateVector(offsetListObjUser);
	}

	fbb.Finish(CreateTB_ResLogin(fbb, offsetObjUserList));

	return CreatePacket(PROTOCOL::RES_LOGIN, fbb, owner);
}

Packet PacketGeneratorServer::CreateResRoundTripTime(Session* const owner, const GameRoom& gameRoom)
{
	const auto currentTimeMs = TPUtil::GetInstance().TimeSinceEpochMs();
	flatbuffers::FlatBufferBuilder fbb;
	fbb.Finish(CreateTB_ResRoundTripTime(fbb, currentTimeMs, gameRoom.GetAvgRttMs()));
	return CreatePacket(PROTOCOL::RES_ROUND_TRIP_TIME, fbb, owner);
}

Packet PacketGeneratorServer::CreateBcastEnterGameRoom(Session* const owner, const shared_ptr<ObjUser> objUser)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.emplace_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	fbb.Finish(CreateTB_BcastEnterGameRoom(fbb, objUser->Serialize(fbb)));

	return CreatePacket(PROTOCOL::BCAST_ENTER_GAME_ROOM, fbb, nullptr, PacketCastType::BROADCAST, packetCastGroup);
}

Packet PacketGeneratorServer::CreateBcastExitGameRoom(const shared_ptr<ObjUser> objUser)
{
	flatbuffers::FlatBufferBuilder fbb;

	fbb.Finish(CreateTB_BcastExitGameRoom(fbb, objUser->Serialize(fbb)));

	return CreatePacket(PROTOCOL::BCAST_EXIT_GAME_ROOM, fbb, nullptr, PacketCastType::BROADCAST);
}

Packet PacketGeneratorServer::CreateBcastMove(Session* const owner, const TB_ReqMove& reqMove)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.emplace_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(owner->GetCUserId());
	auto offsetOperation = reqMove.Operation();

	auto inputMove = reqMove.InputMove();
	auto offsetInputMove = CreateTB_InputMove(fbb,
		inputMove->ForwardVector(),
		inputMove->RightVector(),
		inputMove->MoveForward(),
		inputMove->MoveRight()
	);

	fbb.Finish(CreateTB_BcastMove(fbb, offsetUserId, offsetOperation, offsetInputMove));

	return CreatePacket(PROTOCOL::BCAST_MOVE, fbb, nullptr, PacketCastType::BROADCAST, packetCastGroup);
}

Packet PacketGeneratorServer::CreateBcastLocationSync(Session* const owner, const TB_ReqLocationSync& reqLocationSync)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.emplace_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(owner->GetCUserId());
	auto offsetLocation = reqLocationSync.Location();

	fbb.Finish(CreateTB_BcastLocationSync(fbb, offsetUserId, offsetLocation));

	return CreatePacket(PROTOCOL::BCAST_LOCATION_SYNC, fbb, nullptr, PacketCastType::BROADCAST, packetCastGroup);
}

Packet PacketGeneratorServer::CreateBcastAction(Session* const owner, const TB_ReqAction& reqAction)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.emplace_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(owner->GetCUserId());
	auto offsetOperation = reqAction.Operation();

	auto inputAction = reqAction.InputAction();
	auto offsetComboSectionName = fbb.CreateString(inputAction->ComboSectionName());
	auto offsetInputAction = CreateTB_InputAction(fbb,
		inputAction->Location(),
		inputAction->Rotation(),
		offsetComboSectionName,
		inputAction->IsCombatPosture()
	);

	fbb.Finish(CreateTB_BcastAction(fbb, offsetUserId, offsetOperation, offsetInputAction));

	return CreatePacket(PROTOCOL::BCAST_ACTION, fbb, nullptr, PacketCastType::BROADCAST, packetCastGroup);
}

Packet PacketGeneratorServer::CreateBcastHit(const vector<shared_ptr<ObjUser>>& hitList)
{
	flatbuffers::FlatBufferBuilder fbb;

	vector<flatbuffers::Offset<TB_ObjUser>> offsetListObjUser;
	for (auto& hit : hitList)
	{
		offsetListObjUser.push_back(hit->Serialize(fbb));
	}
	auto offsetObjUserList = fbb.CreateVector(offsetListObjUser);

	fbb.Finish(CreateTB_BcastHit(fbb, offsetObjUserList));

	return CreatePacket(PROTOCOL::BCAST_HIT, fbb, nullptr, PacketCastType::BROADCAST);
}

Packet PacketGeneratorServer::CreateBcastRotationSync(Session* const owner, const TB_ReqRotationSync& reqRotationSync)
{
	vector<Session*> packetCastGroup;
	packetCastGroup.emplace_back(owner);

	flatbuffers::FlatBufferBuilder fbb;

	auto offsetUserId = fbb.CreateString(owner->GetCUserId());
	auto offsetRotation = reqRotationSync.Rotation();

	fbb.Finish(CreateTB_BcastRotationSync(fbb, offsetUserId, offsetRotation));

	return CreatePacket(PROTOCOL::BCAST_ROTATION_SYNC, fbb, nullptr, PacketCastType::BROADCAST, packetCastGroup);
}