#pragma once

#include "../Util/TSingleton.h"
#include "Session.h"
#include <map>
#include <cstdint>

using namespace std;

class SessionPool : public TSingleton<SessionPool>
{
public:
	Session* GetSession(const SOCKET clntSock) const;
	Session* GetSession(const wchar_t* _userId) const;
	map<SOCKET, Session*> GetSessionMap() const;
	void CreateSession(const SOCKET clntSock, const SOCKADDR_IN& clntAddr);
	void DeleteSession(const SOCKET clntSock);
	void Destroy();

private:
	map<SOCKET, Session*> sessionMap;
};