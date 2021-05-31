#include "PacketService.h"
#include "PacketProcessor.h"
#include "DBService.h"
#include "PacketGenerator.h"
#include "GameRoomService.h"
#include "Session.h"
#include "TPResult.h"

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

	cout << "로그인 요청 - UserId:" << userId << " Password:" << password << endl;

	wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
	TPUtil::GetInstance().CharToWChar(wUserId, SIZE_USER_USER_ID, userId);
	TPUtil::GetInstance().CharToWChar(wPassword, SIZE_USER_PASSWORD, password);

	// 유저 로그인 또는 등록
	auto resultLoginUser = DBService::GetInstance().LoginUser(wUserId, wPassword);
	if (!resultLoginUser->GetFlag())
	{
		auto packetError = PacketGenerator::GetInstance().CreateError(owner, resultLoginUser->GetMsg());
		resultLoginUser->SetPacket(packetError);
		return resultLoginUser;
	}

	// 유저 정보 조회
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
	
	// 유저 컴포넌트 설정
	objUser->SetCompUserLocation(compUserLocation);

	// 세션에 유저 아이디 설정
	owner->SetUserId(wUserId);

	// 유저 방 참가
	if (!GameRoomService::GetInstance().AddObjUser(objUser))
	{
		auto packetError = PacketGenerator::GetInstance().CreateError(owner, FAIL_ADD_OBJ_USER_GAME_ROOM);
		resultLoadUserInfo->SetPacket(packetError);
		return resultLoadUserInfo;
	}

	// 방 정보 전달용 패킷 생성
	auto gameRoom = GameRoomService::GetInstance().GetGameRoom();
	auto packetGameRoomObj = PacketGenerator::GetInstance().CreateGameRoomObj(owner, *gameRoom);	

	// 방 입장 패킷 전송
	auto packetEnterGameRoom = PacketGenerator::GetInstance().CreateEnterGameRoom(owner, objUser);
	PacketProcessor::GetInstance().SendPacket(packetEnterGameRoom);

	resultLoadUserInfo->SetPacket(packetGameRoomObj);
	return resultLoadUserInfo;
}

void PacketService::ProcReqMove(const Packet& packet)
{
	auto body = packet.GetBody();
	auto owner = packet.GetOwner();

	auto req = flatbuffers::GetRoot<TB_ReqMove>(body);
	auto userId = req->UserId()->c_str();
	auto locationList = req->LocationList();

	if (!locationList || locationList->size() == 0)
	{
		return;
	}

	auto packetMoveLocation = PacketGenerator::GetInstance().CreateMoveLocation(owner, *locationList);
	PacketProcessor::GetInstance().SendPacket(packetMoveLocation);
}