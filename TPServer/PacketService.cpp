#include "PacketService.h"
#include "PacketProcessor.h"
#include "TP_generated.h"
#include "DBService.h"
#include "TPDefine.h"
#include "PacketGenerator.h"
#include "TPUtil.h"
#include "ObjUser.h"

#include <iostream>

using namespace std;

void PacketService::Process(const Packet& packet)
{
	auto header = packet.GetHeader();
	Packet resultPacket;
	switch (header)
	{
	case PROTOCOL::REQ_LOGIN:
		resultPacket = ReqLogin(packet);
		break;
	default:
		break;
	}

	if (resultPacket.IsValid())
	{
		auto clntSock = packet.GetOwner().GetClntSock();
		PacketProcessor::GetInstance().SendPacket(resultPacket, clntSock);
	}	
}

Packet PacketService::ReqLogin(const Packet& packet)
{
	auto body = packet.GetBody();
	auto req = flatbuffers::GetRoot<REQ_LOGIN>(body);
	auto userId = req->UserId()->c_str();
	auto password = req->Password()->c_str();
	cout << "UserId:" << userId << " Password:" << password << endl;

	wchar_t wUserId[SIZE_USER_USER_ID], wPassword[SIZE_USER_PASSWORD];
	TPUtil::GetInstance().CharToWChar(wUserId, SIZE_USER_USER_ID, userId);
	TPUtil::GetInstance().CharToWChar(wPassword, SIZE_USER_PASSWORD, password);

	auto resultLoginUser =  DBService::GetInstance().LoginUser(wUserId, wPassword);
	if (!resultLoginUser->GetFlag())
	{
		auto packet = PacketGenerator::GetInstance().CreateError(resultLoginUser->GetMsg());
		delete resultLoginUser;
		return packet;
	}

	auto obj = resultLoginUser->GetObjectList().front();
	auto objUser = static_cast<ObjUser*>(obj);
	
	delete resultLoginUser;

	auto resultLoadUserInfo = DBService::GetInstance().LoadUserInfo(wUserId);
	if (!resultLoadUserInfo->GetFlag())
	{
		auto packet = PacketGenerator::GetInstance().CreateError(resultLoadUserInfo->GetMsg());
		delete resultLoadUserInfo;
		return packet;
	}

	auto comp = resultLoadUserInfo->GetCompList().front();
	auto compUserLocation = static_cast<CompUserLocation*>(comp);	

	delete resultLoadUserInfo;

	return Packet();
}

void PacketService::ReqMove(const Packet& packet)
{
	/*PacketProcessor::GetInstance().SendPacket(packet);*/
}