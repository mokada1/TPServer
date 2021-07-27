#pragma once

#include "../Util/Unconstructible.h"
#include "Session.h"
#include <map>
#include <cstdint>

using namespace std;

class SessionPool : public Unconstructible
{
public:
	static SessionPool& GetInstance()
	{
		static SessionPool* _instance = new SessionPool();
		return *_instance;
	}

	Session* GetSession(const SOCKET clntSock) const;
	Session* GetSession(const wchar_t* _userId) const;
	map<SOCKET, Session*> GetSessionMap() const;
	void CreateSession(const SOCKET clntSock, const SOCKADDR_IN& clntAddr);
	void DeleteSession(const SOCKET clntSock);
	void Destroy();

private:
	SessionPool() {}

	map<SOCKET, Session*> sessionMap;
};