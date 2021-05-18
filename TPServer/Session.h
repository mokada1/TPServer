#pragma once
#include <WinSock2.h>

class Session
{
public:
	Session();
	Session(SOCKET clntSock, SOCKADDR_IN clntAddr);
	~Session();

	void SetUserId(const wchar_t* const _userId);
	SOCKET GetClntSock() const;	
	wchar_t* GetUserId() const;

	bool IsValid() const;
private:
	SOCKET hClntSock;
	const char* ip;
	wchar_t* userId;
};