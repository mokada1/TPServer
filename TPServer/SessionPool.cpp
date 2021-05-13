#include "SessionPool.h"

Session* SessionPool::GetSession(const SOCKET clntSock) const
{
	auto it = sessionMap.find(clntSock);
	if (it == sessionMap.end())
	{
		return nullptr;
	}	
	return it->second;
}

vector<Session*> SessionPool::GetSessionAll() const
{
	vector<Session*> all;
	for (auto it = sessionMap.begin(); it != sessionMap.end(); ++it)
	{
		all.push_back(it->second);
	}
	return all;
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
		delete it->second;
		sessionMap.erase(it);		
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
