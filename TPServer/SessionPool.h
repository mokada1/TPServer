#pragma once

#include "TSingleton.h"
#include <map>
#include "Session.h"
#include <vector>

using namespace std;

class SessionPool : public TSingleton<SessionPool>
{
public:
	Session* GetSession(const SOCKET clntSock) const;
	vector<Session*> GetSessionAll() const;
	void CreateSession(const SOCKET clntSock, const SOCKADDR_IN& clntAddr);
	void DeleteSession(const SOCKET clntSock);
	void Destroy();
private:
	map<SOCKET, Session*> sessionMap;
};