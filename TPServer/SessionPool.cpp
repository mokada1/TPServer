#include "SessionPool.h"
#include "GameRoomService.h"
#include "PacketGenerator.h"
#include "PacketProcessor.h"
#include "TPDefine.h"

Session* SessionPool::GetSession(const SOCKET clntSock) const
{
	auto it = sessionMap.find(clntSock);
	if (it == sessionMap.end())
	{
		return nullptr;
	}	
	return it->second;
}

map<SOCKET, Session*> SessionPool::GetSessionMap() const
{
	return sessionMap;
}

void SessionPool::CreateSession(const SOCKET clntSock, const SOCKADDR_IN& clntAddr)
{
	auto newSession = new Session(clntSock, clntAddr);
	sessionMap.insert(pair<SOCKET, Session*>(clntSock, newSession));
}

void SessionPool::DeleteSession(const SOCKET clntSock)
{
	std::shared_ptr<ObjUser> objUser = nullptr;

	auto it = sessionMap.find(clntSock);	
	if (it != sessionMap.end())
	{
		auto session = it->second;
		auto userId = session->GetUserId();
		if (userId)
		{
			objUser = GameRoomService::GetInstance().GetObjUser(userId);
			GameRoomService::GetInstance().DeleteObjUser(userId);
		}		
		delete session;
		sessionMap.erase(it);
	}	

	if (objUser)
	{
		auto packetExitGameRoom = PacketGenerator::GetInstance().CreateExitGameRoom(objUser);
		PacketProcessor::GetInstance().SendPacket(packetExitGameRoom);
	}
}

void SessionPool::Destroy()
{
	for (auto it=sessionMap.begin(); it != sessionMap.end(); ++it)
	{
		delete it->second;
	}
	sessionMap.clear();
}
