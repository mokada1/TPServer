#include "Session.h"
#include <WS2tcpip.h>
#include "TPDefine.h"

Session::Session()
{
	this->hClntSock = 0;
	this->ip = nullptr;
	this->userId = nullptr;
}

Session::Session(SOCKET clntSock, SOCKADDR_IN clntAddr)
{
	this->hClntSock = clntSock;
	char buf[BUFSIZE_IP] = { 0, };
	this->ip = inet_ntop(AF_INET, &clntAddr.sin_addr, buf, sizeof(buf));
	this->userId = nullptr;
}

Session::~Session()
{
	if (userId)
	{
		delete[] userId;
	}
}

void Session::SetUserId(const wchar_t* const _userId)
{
	this->userId = new wchar_t[SIZE_USER_USER_ID];
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
}

SOCKET Session::GetClntSock() const
{
	return hClntSock;
}

wchar_t* Session::GetUserId() const
{
	return userId;
}

bool Session::IsValid() const
{
	return hClntSock != 0 && ip != nullptr;
}
