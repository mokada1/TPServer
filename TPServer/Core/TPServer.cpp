#include "TPServer.h"
#include "../DB/DBServer.h"
#include "../Packet/PacketProcessor.h"
#include "../Session/SessionPool.h"
#include "../Util/TPLogger.h"

#include <WS2tcpip.h>

void TPServer::Play()
{
    Initialize();
    Start();
    Close();
}

void TPServer::Initialize()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        TPLogger::GetInstance().PrintLog(WSASTARTUP_ERROR);
    }

    hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    GetSystemInfo(&systemInfo);
    
    for (unsigned int i = 0; i < systemInfo.dwNumberOfProcessors; ++i)
    {
        completionThreads.push_back(new thread([&](){ ReadCompletionStatus(); }));
    }
}

void TPServer::Start()
{
    hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

    SOCKADDR_IN servAddr;
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(SERVER_PORT);

    bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
    listen(hServSock, LISTEN_BACKLOG);

    acceptThread = new thread([&](){ Accept(); });
    recvThread = new thread([&]() { ProcRecvPacket(); });
}

void TPServer::Close()
{
    if (acceptThread)
    {
        acceptThread->join();
        delete acceptThread;
    }    
    for (auto t : completionThreads)
    {
        t->join();
        delete t;
    }
    if (recvThread)
    {
        recvThread->join();
        delete recvThread;
    }

    SessionPool::GetInstance().Destroy();
    WSACleanup();    
}

void TPServer::Accept()
{
    LPPER_HANDLE_DATA perHandleData;

    DWORD recvBytes;
    DWORD flags;

    TPLogger::GetInstance().PrintLog(SERVER_STARTED);

    while (TRUE)
    {
        SOCKADDR_IN clntAddr;
        auto addrLen = sizeof(clntAddr);
        auto addrLenInt = static_cast<int>(addrLen);

        SOCKET hClntSock = WSAAccept(hServSock, (SOCKADDR*)&clntAddr, &addrLenInt, nullptr, 0);
        if (hClntSock == SOCKET_ERROR)
        {
            auto e = WSAGetLastError();
            if (e != WSA_IO_PENDING)
            {
                TPLogger::GetInstance().PrintLog(WSARECV_ERROR, e);
            }
            break;
        }

        perHandleData = new PER_HANDLE_DATA;
        perHandleData->hClntSock = hClntSock;
        memcpy(&(perHandleData->clntAddr), &clntAddr, addrLen);

        char buf[BUFF_SIZE_IP] = { 0, };
        auto address = inet_ntop(AF_INET, &clntAddr.sin_addr, buf, sizeof(buf));
        TPLogger::GetInstance().PrintLog(CONNECTED, hClntSock, address);

        SessionPool::GetInstance().CreateSession(hClntSock, clntAddr);

        CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (ULONG_PTR)perHandleData, 0);

        LPPER_IO_DATA perIoData = new PER_IO_DATA;
        memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
        perIoData->wsaBuf.len = MAX_BUFF_SIZE;
        perIoData->wsaBuf.buf = perIoData->buffer;
        perIoData->operation = OP_ClientToServer;
        flags = 0;

        if (WSARecv(perHandleData->hClntSock, &(perIoData->wsaBuf), 1, &recvBytes, &flags, &(perIoData->overlapped), NULL) == SOCKET_ERROR)
        {
            auto e = WSAGetLastError();
            if (e != WSA_IO_PENDING)
            {
                TPLogger::GetInstance().PrintLog(WSARECV_ERROR, e);
            }
        }
    }
}

void TPServer::ReadCompletionStatus()
{    
    OVERLAPPED_ENTRY overlappeds[MAX_OVERLAPPED_ENTRY];
    ULONG count;

    while (true) {
        if (GetQueuedCompletionStatusEx(hCompletionPort, overlappeds, MAX_OVERLAPPED_ENTRY, &count, INFINITE, false))
        {
            TPLogger::GetInstance().PrintLog("GetQueuedCompletionStatusEx:%d", count);
            for (int i = 0; i < count; ++i)
            {
                auto perHandleData = (LPPER_HANDLE_DATA)overlappeds[i].lpCompletionKey;
                auto perIoData = (LPPER_IO_DATA)overlappeds[i].lpOverlapped;
                ProcCompletion(perHandleData, perIoData, static_cast<size_t>(overlappeds[i].dwNumberOfBytesTransferred));
            }
        }
        else
        {
            TPLogger::GetInstance().PrintLog("GetQueuedCompletionStatusEx:false");
        }
    }
}

void TPServer::ProcCompletion(const LPPER_HANDLE_DATA perHandleData, const LPPER_IO_DATA perIoData, const size_t recvBytes)
{
    if (perIoData->operation == OP_ClientToServer)
    {
        if (recvBytes == 0)
        {
            char buf[BUFF_SIZE_IP] = { 0, };
            auto address = inet_ntop(AF_INET, &perHandleData->clntAddr.sin_addr, buf, sizeof(buf));
            TPLogger::GetInstance().PrintLog("Disconnected[%d]: %s", perHandleData->hClntSock, address);

            SessionPool::GetInstance().DeleteSession(perHandleData->hClntSock);

            closesocket(perHandleData->hClntSock);
            delete perHandleData;
            delete perIoData;
            return;
        }

        auto session = SessionPool::GetInstance().GetSession(perHandleData->hClntSock);
        if (session != nullptr)
        {
            PacketProcessor::GetInstance().Parse(session, perIoData->wsaBuf.buf, recvBytes);
        }
        else
        {
            TPLogger::GetInstance().PrintLog(NOT_FOUND_SESSION);
        }

        memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
        perIoData->wsaBuf.len = MAX_BUFF_SIZE;
        perIoData->wsaBuf.buf = perIoData->buffer;
        perIoData->operation = OP_ClientToServer;
        DWORD flags = 0;

        if (WSARecv(perHandleData->hClntSock, &(perIoData->wsaBuf), 1, NULL, &flags, &(perIoData->overlapped), NULL) == SOCKET_ERROR)
        {
            auto e = WSAGetLastError();
            if (e != WSA_IO_PENDING)
            {
                TPLogger::GetInstance().PrintLog(WSARECV_ERROR, e);
            }
        }
    }
    else if (perIoData->operation == OP_ServerToClient)
    {
        delete perIoData;
    }
    else
    {
        TPLogger::GetInstance().PrintLog(INVALID_OPERATION);
        delete perIoData;
    }
}

void TPServer::ProcRecvPacket()
{
    while (true)
    {
        PacketProcessor::GetInstance().ProcRecvPacket();
        this_thread::sleep_for(std::chrono::milliseconds(10));
    }    
}
