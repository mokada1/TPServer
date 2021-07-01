#include "SessionPool.h"
#include "../GameRoom/GameRoomService.h"
#include "../Packet/PacketGenerator.h"
#include "../Packet/PacketProcessor.h"
#include "../Util/TPDefine.h"

Session* SessionPool::GetSession(const SOCKET clntSock) const
{
	auto it = sessionMap.find(clntSock);
	if (it == sessionMap.end())
	{
		return nullptr;
	}	
	return it->second;
}

Session* SessionPool::GetSession(const wchar_t* _userId) const
{
	for (auto it = sessionMap.begin(); it != sessionMap.end(); ++it)
	{
		if (wcscmp(it->second->GetUserId(), _userId) == 0)
		{
			return it->second;
		}
	}
	return nullptr;
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
	auto it = sessionMap.find(clntSock);	
	if (it != sessionMap.end())
	{
		std::shared_ptr<ObjUser> objUser = nullptr;
		auto session = it->second;
		auto userId = session->GetUserId();
		if (userId)
		{
			objUser = GameRoomService::GetInstance().GetObjUser(userId);
			GameRoomService::GetInstance().DeleteObjUser(objUser);
		}		
		delete session;
		sessionMap.erase(it);

		if (objUser)
		{
			auto packetBcastExitGameRoom = PacketGenerator::GetInstance().CreateBcastExitGameRoom(objUser);
			PacketProcessor::GetInstance().SendPacket(packetBcastExitGameRoom);
		}
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
