#pragma once
#include <WinSock2.h>

class Session
{
public:
	Session();
	Session(SOCKET clntSock, SOCKADDR_IN clntAddr);
	SOCKET GetClntSock() const;	
	bool IsValid() const;
private:
	SOCKET hClntSock;
	const char* ip;
};