#include "Session.h"
#include <WS2tcpip.h>
#include "../Util/TPDefine.h"
#include "../Util/TPUtil.h"

Session::Session()
{
	this->hClntSock = 0;
	this->ip = nullptr;
	this->userId = nullptr;
	this->buffer = nullptr;
	this->packetSize = 0;
}

Session::Session(SOCKET clntSock, SOCKADDR_IN clntAddr)
{
	this->hClntSock = clntSock;
	char buf[BUFF_SIZE_IP] = { 0, };
	this->ip = inet_ntop(AF_INET, &clntAddr.sin_addr, buf, sizeof(buf));
	this->userId = nullptr;
	this->buffer = nullptr;
	this->packetSize = 0;
}

Session::~Session()
{
	if (userId)
	{
		delete[] userId;
	}
	if (cUserId)
	{
		delete[] cUserId;
	}
	if (buffer)
	{
		delete[] buffer;
	}
}

void Session::ClearBuff()
{
	ClearBuff(true);
}

void Session::ClearBuff(bool isDeleteBuff)
{
	if (buffer)
	{
		if (isDeleteBuff)
		{
			delete[] buffer;
		}		
		buffer = nullptr;
		packetSize = 0;
	}
}

void Session::AddToBuff(char* const _buffer, const size_t _packetSize)
{
	if (!buffer)
	{
		buffer = new char[_packetSize];		
		memcpy(buffer, _buffer, _packetSize);
		packetSize = _packetSize;
	}
	else
	{
		const auto newPacketSize = packetSize + _packetSize;
		auto newBuff = new char[newPacketSize];
		memcpy(newBuff, buffer, packetSize);
		memcpy(&newBuff[packetSize], _buffer, _packetSize);

		delete[] buffer;
		buffer = newBuff;
		packetSize = newPacketSize;
	}
}

void Session::SetUserId(const wchar_t* const _userId)
{
	this->userId = new wchar_t[SIZE_USER_USER_ID];
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
	this->cUserId = new char[SIZE_USER_USER_ID];
	TPUtil::GetInstance().WCharToChar(this->cUserId, SIZE_USER_USER_ID, this->userId);
}

void Session::SetBuffer(char* const _buffer)
{
	this->buffer = _buffer;
}

void Session::SetPacketSize(const size_t _packetSize)
{
	this->packetSize = _packetSize;
}

SOCKET Session::GetClntSock() const
{
	return hClntSock;
}

wchar_t* Session::GetUserId() const
{
	return userId;
}

char* Session::GetCUserId() const
{
	return cUserId;
}

char* Session::GetBuffer() const
{
	return buffer;
}

size_t Session::GetPacketSize() const
{
	return packetSize;
}

bool Session::IsValid() const
{
	return hClntSock != 0 && ip != nullptr;
}
