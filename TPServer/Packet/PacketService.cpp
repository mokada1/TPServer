#include "PacketService.h"
#include "PacketProcessor.h"
#include "../DB/DBService.h"
#include "PacketGeneratorServer.h"
#include "../GameRoom/GameRoomService.h"
#include "../Session/Session.h"
#include "../Util/TPResult.h"
#include "../Util/TPLogger.h"
#include "../Battle/BattleService.h"

void PacketService::Process(const Packet& packet)
{
	auto header = packet.GetHeader();
	TPResult* result = nullptr;
	switch (header)
	{
	case PROTOCOL::REQ_LOGIN:
		result = ProcReqLogin(packet);
		break;
	case PROTOCOL::REQ_MOVE:
		result = ProcReqMove(packet);
		break;
	case PROTOCOL::REQ_LOCATION_SYNC:
		result = ProcReqLocationSync(packet);
		break;
	case PROTOCOL::REQ_ROUND_TRIP_TIME:
		result = ProcReqRoundTripTime(packet);
		break;
	case PROTOCOL::REQ_ACTION:
		result = ProcReqAction(packet);
		break;
	case PROTOCOL::REQ_ABILITY:
		result = ProcReqAbility(packet);
		break;
	case PROTOCOL::REQ_ROTATION_SYNC:
		result = ProcReqRotationSync(packet);
		break;
	default:
		break;
	}

	if (result)
	{
		if (!result->GetFlag())
		{
			auto owner = packet.GetOwner();
			auto msg = result->GetMsg();
			TPLogger::GetInstance().PrintLog(msg);
			auto errorPacket = PacketGeneratorServer::GetInstance().CreateError(owner, msg);
			result->SetPacket(errorPacket);
		}

		auto resultPacket = result->GetPacket();
		if (resultPacket.IsValid())
		{
			PacketProcessor::GetInstance().SendPacket(resultPacket);
			delete result;
		}
	}	
}

TPResult* PacketService::ProcReqLogin(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqLogin>(body);
	auto userId = req->UserId()->c_str();
	auto password = req->Password()->c_str();

	TPLogger::GetInstance().PrintLog(REQ_LOGIN, userId, password);

	wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
	TPUtil::GetInstance().CharToWChar(wUserId, SIZE_USER_USER_ID, userId);
	TPUtil::GetInstance().CharToWChar(wPassword, SIZE_USER_PASSWORD, password);

	// 중복 로그인 방지
	auto objUserInGameRoom = GameRoomService::GetInstance().GetObjUser(wUserId);
	if (objUserInGameRoom)
	{
		auto result = new TPResult();
		result->SetMsg(DUPLICATE_LOGIN);
		return result;
	}

	// 유저 로그인 또는 등록
	auto result = DBService::GetInstance().LoginUser(wUserId, wPassword);
	if (!result->GetFlag())
	{
		return result;
	}

	auto obj = result->GetObjectList().front();
	auto objUser = static_pointer_cast<ObjUser>(obj);

	// 세션에 유저 아이디 설정
	owner->SetUserId(wUserId);

	// 유저 방 참가
	if (!GameRoomService::GetInstance().AddObjUser(objUser))
	{
		result->SetFlag(false);
		result->SetMsg(FAIL_ADD_OBJ_USER_GAME_ROOM);
		return result;
	}

	// 방 정보 전달용 패킷 생성
	auto gameRoom = GameRoomService::GetInstance().GetGameRoom();
	auto resLoginPacket = PacketGeneratorServer::GetInstance().CreateResLogin(owner, *gameRoom);

	// 방 입장 패킷 전송
	auto bcastEnterGameRoomPacket = PacketGeneratorServer::GetInstance().CreateBcastEnterGameRoom(owner, objUser);
	PacketProcessor::GetInstance().SendPacket(bcastEnterGameRoomPacket);

	result->SetPacket(resLoginPacket);
	result->SetFlag(true);
	return result;
}

TPResult* PacketService::ProcReqMove(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqMove>(body);
	auto bcastMovePacket = PacketGeneratorServer::GetInstance().CreateBcastMove(owner, *req);
	PacketProcessor::GetInstance().SendPacket(bcastMovePacket);

	return nullptr;
}

TPResult* PacketService::ProcReqRoundTripTime(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqRoundTripTime>(body);
	const auto reqCurrentTimeMs = req->CurrentTimeMs();
	const auto currentTimeMs = TPUtil::GetInstance().TimeSinceEpochMs();
	const auto rttMs = currentTimeMs - reqCurrentTimeMs;

	auto result = new TPResult();

	if (rttMs > MAX_RTT_MS)
	{
		result->SetFlag(false);
		result->SetMsg(EXCEED_MAX_RTT_MS);
		return result;
	}

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		result->SetFlag(false);
		result->SetMsg(FAIL_GET_OBJ_USER);
		return result;
	}
	objUser->UpdateRtt(rttMs);

	auto gameRoom = GameRoomService::GetInstance().GetGameRoom(objUser->GetRoomId());
	if (!gameRoom)
	{
		result->SetFlag(false);
		result->SetMsg(FAIL_GET_GAME_ROOM);
		return result;
	}
	gameRoom->UpdateRtt();

	auto resRoundTripTimePacket = PacketGeneratorServer::GetInstance().CreateResRoundTripTime(owner, objUser->GetAvgRttMs());
	result->SetPacket(resRoundTripTimePacket);
	result->SetFlag(true);
	return result;
}

TPResult* PacketService::ProcReqLocationSync(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqLocationSync>(body);
	auto location = req->Location();

	// 유저 위치 정보 갱신
	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		auto result = new TPResult();
		result->SetMsg(FAIL_GET_OBJ_USER);
		return result;
	}
	auto compUserTransform = objUser->GetCompTransform();
	compUserTransform->SetLocation({ location->x(), location->y(), location->z() });

	auto bcastLocationSyncPacket = PacketGeneratorServer::GetInstance().CreateBcastLocationSync(owner, *req);
	PacketProcessor::GetInstance().SendPacket(bcastLocationSyncPacket);
	
	return nullptr;
}

TPResult* PacketService::ProcReqAction(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqAction>(body);

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		auto result = new TPResult();
		result->SetMsg(FAIL_GET_OBJ_USER);
		return result;
	}

	auto operation = req->Operation();
	auto inputAction = req->InputAction();

	auto compTransform = objUser->GetCompTransform();
	compTransform->SetLocation({ inputAction->Location()->x(), inputAction->Location()->y(), inputAction->Location()->z() });
	compTransform->SetRotation({ inputAction->Rotation()->x(), inputAction->Rotation()->y(), inputAction->Rotation()->z() });
	
	if (operation == OpAction::OpAction_DrawWeapon)
	{
		auto inputCombatPosture = inputAction->IsCombatPosture();
		auto compCondition = objUser->GetCompCondition();
		if (compCondition->GetIsCombatPosture() != inputCombatPosture)
		{
			compCondition->SetIsCombatPosture(inputCombatPosture);
		}
	}

	auto bcastActionPacket = PacketGeneratorServer::GetInstance().CreateBcastAction(owner, *req);
	PacketProcessor::GetInstance().SendPacket(bcastActionPacket);

	return nullptr;
}

TPResult* PacketService::ProcReqAbility(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqAbility>(body);

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		auto result = new TPResult();
		result->SetMsg(FAIL_GET_OBJ_USER);
		return result;
	}

	auto compTransform = objUser->GetCompTransform();
	compTransform->SetLocation({ req->Location()->x(), req->Location()->y(), req->Location()->z() });
	compTransform->SetRotation({ req->Rotation()->x(), req->Rotation()->y(), req->Rotation()->z() });

	BattleService::GetInstance().PlayAbility(objUser, req->Ability());
	
	return nullptr;
}

TPResult* PacketService::ProcReqRotationSync(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqRotationSync>(body);

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		auto result = new TPResult();
		result->SetMsg(FAIL_GET_OBJ_USER);
		return result;
	}
	auto rotation = req->Rotation();
	auto compUserTransform = objUser->GetCompTransform();
	compUserTransform->SetRotation({ rotation->x(), rotation->y(), rotation->z() });

	auto bcastRotationSyncPacket = PacketGeneratorServer::GetInstance().CreateBcastRotationSync(owner, *req);
	PacketProcessor::GetInstance().SendPacket(bcastRotationSyncPacket);

	return nullptr;
}
