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
		ProcReqMove(packet);
		break;
	case PROTOCOL::REQ_LOCATION_SYNC:
		ProcReqLocationSync(packet);
		break;
	case PROTOCOL::REQ_ROUND_TRIP_TIME:
		result = ProcReqRoundTripTime(packet);
		break;
	case PROTOCOL::REQ_ACTION:
		ProcReqAction(packet);
		break;
	case PROTOCOL::REQ_DAMAGE:
		ProcReqDamage(packet);
		break;
	case PROTOCOL::REQ_ROTATE:
		ProcReqRotate(packet);
		break;
	default:
		break;
	}

	if (result)
	{
		auto packetResult = result->GetPacket();
		if (packetResult.IsValid())
		{
			PacketProcessor::GetInstance().SendPacket(packetResult);
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
	auto dObjUser = GameRoomService::GetInstance().GetObjUser(wUserId);
	if (dObjUser)
	{
		TPLogger::GetInstance().PrintLog(DUPLICATE_LOGIN);
		auto result = new TPResult();
		auto packetError = PacketGeneratorServer::GetInstance().CreateError(owner, DUPLICATE_LOGIN);
		result->SetPacket(packetError);
		return result;
	}

	// 유저 로그인 또는 등록
	auto resultLoginUser = DBService::GetInstance().LoginUser(wUserId, wPassword);
	if (!resultLoginUser->GetFlag())
	{
		auto packetError = PacketGeneratorServer::GetInstance().CreateError(owner, resultLoginUser->GetMsg());
		resultLoginUser->SetPacket(packetError);
		return resultLoginUser;
	}

	// 유저 정보 조회
	auto resultLoadUserInfo = DBService::GetInstance().LoadUserInfo(wUserId);
	resultLoadUserInfo->SetNextResult(resultLoginUser);
	if (!resultLoadUserInfo->GetFlag())
	{
		auto packetError = PacketGeneratorServer::GetInstance().CreateError(owner, resultLoadUserInfo->GetMsg());
		resultLoadUserInfo->SetPacket(packetError);
		return resultLoadUserInfo;
	}

	auto obj = resultLoginUser->GetObjectList().front();
	auto objUser = static_pointer_cast<ObjUser>(obj);

	auto comp = resultLoadUserInfo->GetCompList().front();
	auto compUserTransform = static_pointer_cast<CompUserTransform>(comp);
	
	// 유저 컴포넌트 설정
	objUser->SetCompUserTransform(compUserTransform);

	// 세션에 유저 아이디 설정
	owner->SetUserId(wUserId);

	// 유저 방 참가
	if (!GameRoomService::GetInstance().AddObjUser(objUser))
	{
		auto packetError = PacketGeneratorServer::GetInstance().CreateError(owner, FAIL_ADD_OBJ_USER_GAME_ROOM);
		resultLoadUserInfo->SetPacket(packetError);
		return resultLoadUserInfo;
	}

	// 방 정보 전달용 패킷 생성
	auto gameRoom = GameRoomService::GetInstance().GetGameRoom();
	auto packetResLogin = PacketGeneratorServer::GetInstance().CreateResLogin(owner, *gameRoom);

	// 방 입장 패킷 전송
	auto packetBcastEnterGameRoom = PacketGeneratorServer::GetInstance().CreateBcastEnterGameRoom(owner, objUser);
	PacketProcessor::GetInstance().SendPacket(packetBcastEnterGameRoom);

	resultLoadUserInfo->SetPacket(packetResLogin);
	return resultLoadUserInfo;
}

void PacketService::ProcReqMove(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqMove>(body);
	auto packetBcastMove = PacketGeneratorServer::GetInstance().CreateBcastMove(owner, *req);
	PacketProcessor::GetInstance().SendPacket(packetBcastMove);
}

TPResult* PacketService::ProcReqRoundTripTime(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqRoundTripTime>(body);
	const auto reqCurrentTimeMs = req->CurrentTimeMs();
	const auto currentTimeMs = TPUtil::GetInstance().TimeSinceEpochMs();
	const auto rttMs = TPUTIL_MAX(currentTimeMs - reqCurrentTimeMs, 0);
	
	// 왕복 시간이 너무 큰 값이면 무시(1초 기준)
	if (rttMs > MAX_RTT_MS)
	{
		return nullptr;
	}

	auto result = new TPResult();

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		return nullptr;
	}
	objUser->UpdateRtt(rttMs);

	auto gameRoom = GameRoomService::GetInstance().GetGameRoom(objUser->GetRoomId());
	if (!gameRoom)
	{
		return nullptr;
	}
	gameRoom->UpdateRtt();

	auto packetResRoundTripTime = PacketGeneratorServer::GetInstance().CreateResRoundTripTime(owner, *gameRoom);
	result->SetPacket(packetResRoundTripTime);
	return result;
}

void PacketService::ProcReqLocationSync(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqLocationSync>(body);
	auto location = req->Location();

	// 유저 위치 정보 갱신
	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		return;
	}
	auto compUserTransform = objUser->GetCompUserTransform();
	compUserTransform->SetLocation({ location->x(), location->y(), location->z() });

	auto packetBcastLocationSync = PacketGeneratorServer::GetInstance().CreateBcastLocationSync(owner, *req);
	PacketProcessor::GetInstance().SendPacket(packetBcastLocationSync);
}

void PacketService::ProcReqAction(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqAction>(body);

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		return;
	}
	auto inputAction = req->InputAction();
	auto compUserTransform = objUser->GetCompUserTransform();
	compUserTransform->SetLocation({ inputAction->Location()->x(), inputAction->Location()->y(), inputAction->Location()->z() });
	compUserTransform->SetRotation({ inputAction->Rotation()->x(), inputAction->Rotation()->y(), inputAction->Rotation()->z() });

	auto packetBcastAction = PacketGeneratorServer::GetInstance().CreateBcastAction(owner, *req);
	PacketProcessor::GetInstance().SendPacket(packetBcastAction);
}

void PacketService::ProcReqDamage(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	//auto req = flatbuffers::GetRoot<TB_ReqDamage>(body);

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		return;
	}

	auto hitList = BattleService::GetInstance().GetObjUserListAround(objUser);
	if (hitList.empty())
	{
		return;
	}

	for (auto& hit : hitList)
	{
		auto cUserId = hit->GetCUserId();
		auto packetBcastHit = PacketGeneratorServer::GetInstance().CreateBcastHit(cUserId);
		PacketProcessor::GetInstance().SendPacket(packetBcastHit);
	}
}

void PacketService::ProcReqRotate(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqRotate>(body);

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		return;
	}
	auto rotation = req->Rotation();
	auto compUserTransform = objUser->GetCompUserTransform();
	compUserTransform->SetRotation({ rotation->x(), rotation->y(), rotation->z() });

	auto packetBcastRotate = PacketGeneratorServer::GetInstance().CreateBcastRotate(owner, *req);
	PacketProcessor::GetInstance().SendPacket(packetBcastRotate);
}
