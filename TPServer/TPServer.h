#pragma once

#include "TSingleton.h"
#include <stdlib.h>
#include <winsock2.h>
#include <thread>
#include <vector>
#include "Packet.h"

using namespace std;

#define BUFSIZE 1024
#define OP_ServerToClient 1
#define OP_ClientToServer 2

class TPServer : public TSingleton<TPServer>
{
public:
    void Play();

private:
    const uint8_t SERVER_PORT = static_cast<uint8_t>(2738);

    SOCKET hServSock;
    vector<thread*> worker_threads;
    HANDLE hCompletionPort;  
    
	void Initialize();
	void Start();
	void Close();
    void CompletionThread();
};

typedef struct
{
    SOCKET hClntSock;
    SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

typedef struct
{
    OVERLAPPED overlapped;
    char buffer[BUFSIZE];
    WSABUF wsaBuf;
    int operation;
} PER_IO_DATA, * LPPER_IO_DATA;