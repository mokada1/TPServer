#include "PacketService.h"
#include "PacketProcessor.h"
#include "../DB/DBService.h"
#include "PacketGenerator.h"
#include "../GameRoom/GameRoomService.h"
#include "../Session/Session.h"
#include "../Util/TPResult.h"

#include <iostream>

using namespace std;

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

	cout << "�α��� ��û - UserId:" << userId << " Password:" << password << endl;

	wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
	TPUtil::GetInstance().CharToWChar(wUserId, SIZE_USER_USER_ID, userId);
	TPUtil::GetInstance().CharToWChar(wPassword, SIZE_USER_PASSWORD, password);

	// ���� �α��� �Ǵ� ���
	auto resultLoginUser = DBService::GetInstance().LoginUser(wUserId, wPassword);
	if (!resultLoginUser->GetFlag())
	{
		auto packetError = PacketGenerator::GetInstance().CreateError(owner, resultLoginUser->GetMsg());
		resultLoginUser->SetPacket(packetError);
		return resultLoginUser;
	}

	// ���� ���� ��ȸ
	auto resultLoadUserInfo = DBService::GetInstance().LoadUserInfo(wUserId);
	resultLoadUserInfo->SetNextResult(resultLoginUser);
	if (!resultLoadUserInfo->GetFlag())
	{
		auto packetError = PacketGenerator::GetInstance().CreateError(owner, resultLoadUserInfo->GetMsg());
		resultLoadUserInfo->SetPacket(packetError);
		return resultLoadUserInfo;
	}

	auto obj = resultLoginUser->GetObjectList().front();
	auto objUser = static_pointer_cast<ObjUser>(obj);

	auto comp = resultLoadUserInfo->GetCompList().front();
	auto compUserLocation = static_pointer_cast<CompUserLocation>(comp);
	
	// ���� ������Ʈ ����
	objUser->SetCompUserLocation(compUserLocation);

	// ���ǿ� ���� ���̵� ����
	owner->SetUserId(wUserId);

	// ���� �� ����
	if (!GameRoomService::GetInstance().AddObjUser(objUser))
	{
		auto packetError = PacketGenerator::GetInstance().CreateError(owner, FAIL_ADD_OBJ_USER_GAME_ROOM);
		resultLoadUserInfo->SetPacket(packetError);
		return resultLoadUserInfo;
	}

	// �� ���� ���޿� ��Ŷ ����
	auto gameRoom = GameRoomService::GetInstance().GetGameRoom();
	auto packetResLogin = PacketGenerator::GetInstance().CreateResLogin(owner, *gameRoom);

	// �� ���� ��Ŷ ����
	auto packetBcastEnterGameRoom = PacketGenerator::GetInstance().CreateBcastEnterGameRoom(owner, objUser);
	PacketProcessor::GetInstance().SendPacket(packetBcastEnterGameRoom);

	resultLoadUserInfo->SetPacket(packetResLogin);
	return resultLoadUserInfo;
}

void PacketService::ProcReqMove(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqMove>(body);
	auto packetBcastMove = PacketGenerator::GetInstance().CreateBcastMove(owner, *req);
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

	auto result = new TPResult();

	auto objUser = GameRoomService::GetInstance().GetObjUser(owner->GetUserId());
	if (!objUser)
	{
		return nullptr;
	}
	cout << "PacketService::ProcReqRoundTripTime: rttMs:" << rttMs << " reqCurrentTimeMs:" << reqCurrentTimeMs << " currentTimeMs:" << currentTimeMs << endl;
	objUser->UpdateRtt(rttMs);

	auto gameRoom = GameRoomService::GetInstance().GetGameRoom(objUser->GetRoomId());
	if (!gameRoom)
	{
		return nullptr;
	}
	gameRoom->UpdateRtt();

	auto packetResRoundTripTime = PacketGenerator::GetInstance().CreateResRoundTripTime(owner, *gameRoom);
	result->SetPacket(packetResRoundTripTime);
	return result;
}

void PacketService::ProcReqLocationSync(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqLocationSync>(body);
	auto packetBcastLocationSync = PacketGenerator::GetInstance().CreateBcastLocationSync(owner, *req);
	PacketProcessor::GetInstance().SendPacket(packetBcastLocationSync);
}
