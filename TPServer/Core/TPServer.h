#pragma once

#pragma comment(lib, "ws2_32")

#include "../Util/Unconstructible.h"
#include "../Util/TPDefine.h"

#include <WinSock2.h>
#include <thread>
#include <vector>

using namespace std;

typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[MAX_BUFF_SIZE];
	int operation;
} PER_IO_DATA, *LPPER_IO_DATA;

class TPServer : public Unconstructible
{
public:
	static TPServer& GetInstance()
	{
		static TPServer* _instance = new TPServer();
		return *_instance;
	}

	void Play();

private:
	TPServer() {}

	void Initialize();
	void Start();
	void Close();
	void Accept();
	void ReadCompletionStatus();
	void ProcCompletion(const LPPER_HANDLE_DATA perHandleData, const LPPER_IO_DATA perIoData, const size_t recvBytes);

	SOCKET hServSock;
	HANDLE hCompletionPort;
	const int LISTEN_BACKLOG = 5;
	SYSTEM_INFO systemInfo = { 0 };
	vector<thread*> completionThreads;
	thread* acceptThread = nullptr;
};