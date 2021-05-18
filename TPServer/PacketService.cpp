#include "PacketService.h"
#include "PacketProcessor.h"
#include "TP_generated.h"
#include "DBService.h"
#include "TPDefine.h"
#include "PacketGenerator.h"
#include "TPUtil.h"
#include "ObjUser.h"
#include "GameRoomService.h"

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
	default:
		break;
	}

	if (result)
	{
		auto packetResult = result->GetPacket();
		if (packetResult->IsValid())
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
	packet.GetOwner()->SetUserId(wUserId);

	// ���� �� ����
	if (!GameRoomService::GetInstance().AddObjUser(objUser))
	{
		auto packetError = PacketGenerator::GetInstance().CreateError(owner, FAIL_ADD_OBJ_USER_GAME_ROOM);
		resultLoadUserInfo->SetPacket(packetError);
		return resultLoadUserInfo;
	}

	// �� ���� ���޿� ��Ŷ ����
	auto gameRoom = GameRoomService::GetInstance().GetGameRoom();
	auto packetBCGameRoom = PacketGenerator::GetInstance().CreateBCGameRoom(owner, *gameRoom);	

	resultLoadUserInfo->SetPacket(packetBCGameRoom);
	return resultLoadUserInfo;
}

TPResult* PacketService::ProcReqMove(const Packet& packet)
{
	return nullptr;
}