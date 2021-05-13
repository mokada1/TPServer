#include "Session.h"
#include <WS2tcpip.h>

Session::Session()
{
	this->hClntSock = 0;
	this->ip = nullptr;
}

Session::Session(SOCKET clntSock, SOCKADDR_IN clntAddr)
{
	this->hClntSock = clntSock;
	char buf[32] = { 0, };
	this->ip = inet_ntop(AF_INET, &clntAddr.sin_addr, buf, sizeof(buf));
}

SOCKET Session::GetClntSock() const
{
	return this->hClntSock;
}

bool Session::IsValid() const
{
	return this->hClntSock != 0 && this->ip != nullptr;
}
