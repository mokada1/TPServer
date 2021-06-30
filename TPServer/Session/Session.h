#pragma once

#include <WinSock2.h>

class Session
{
public:
	Session();
	Session(SOCKET clntSock, SOCKADDR_IN clntAddr);
	~Session();

	void ClearBuff();
	void ClearBuff(bool isDeleteBuff);
	void AddToBuff(char* const _buffer, const size_t _packetSize);
	
	void SetUserId(const wchar_t* const _userId);
	void SetBuffer(char* const _buffer);
	void SetPacketSize(const size_t _packetSize);
	
	SOCKET GetClntSock() const;	
	wchar_t* GetUserId() const;
	char* GetCUserId() const;
	char* GetBuffer() const;
	size_t GetPacketSize() const;
	
	bool IsValid() const;
private:
	SOCKET hClntSock;
	const char* ip;
		
	char* buffer;
	size_t packetSize;

	wchar_t* userId;
	char* cUserId;
};