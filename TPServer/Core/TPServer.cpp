#include "TPServer.h"
#include <iostream>
#include <WS2tcpip.h>
#include "../DB/DBServer.h"
#include "../Packet/PacketProcessor.h"
#include "../Session/SessionPool.h"
#include "../Util/TPError.h"

void TPServer::Play()
{
    // DB 시작 처리
    if (!DBServer::GetInstance().DBConnect())
    {
        TPError::GetInstance().PrintError(L"DB연결에 실패했습니다.");
    }
    cout << "DB연결에 성공했습니다." << endl;

    // 서버 시작 처리
    Initialize();
    Start();

    // 서버 종료 처리
    Close();

    // DB 종료 처리
    DBServer::GetInstance().DBDisConnect();
}

void TPServer::Initialize()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        TPError::GetInstance().PrintError(L"WSAStartup() error!");

    hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

    SYSTEM_INFO systemInfo;
    GetSystemInfo(&systemInfo);
    
    for (unsigned int i = 0; i < systemInfo.dwNumberOfProcessors; ++i)
    {
        worker_threads.push_back(new thread([&](){ CompletionThread(); }));
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

    LPPER_HANDLE_DATA perHandleData;

    DWORD recvBytes;
    DWORD flags;

    cout << "서버가 시작되었습니다." << endl;

    while (TRUE)
    {
        SOCKADDR_IN clntAddr;
        auto addrLen = sizeof(clntAddr);
        auto addrLenInt = static_cast<int>(addrLen);

        SOCKET hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &addrLenInt);

        perHandleData = new PER_HANDLE_DATA;
        perHandleData->hClntSock = hClntSock;
        memcpy(&(perHandleData->clntAddr), &clntAddr, addrLen);

        char buf[BUFF_SIZE_IP] = { 0, };
        cout << "Connected[" << hClntSock << "]: " << inet_ntop(AF_INET, &clntAddr.sin_addr, buf, sizeof(buf)) << " " << endl;
        SessionPool::GetInstance().CreateSession(hClntSock, clntAddr);

        CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (ULONG_PTR)perHandleData, 0);

        LPPER_IO_DATA PerIoData = new PER_IO_DATA;
        memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
        PerIoData->wsaBuf.len = MAX_BUFF_SIZE;
        PerIoData->wsaBuf.buf = PerIoData->buffer;
        PerIoData->operation = OP_ClientToServer;
        flags = 0;

        WSARecv(perHandleData->hClntSock, &(PerIoData->wsaBuf), 1, &recvBytes, &flags, &(PerIoData->overlapped), NULL);
    }
}

void TPServer::Close()
{
    for (auto t : worker_threads)
    {
        t->join();
        delete t;
    }

    SessionPool::GetInstance().Destroy();
    WSACleanup();    
}

void TPServer::CompletionThread()
{
    DWORD bytesTransferred;
    LPPER_HANDLE_DATA perHandleData;
    LPPER_IO_DATA perIoData;
    DWORD flags;

    while (true) {
        if (!GetQueuedCompletionStatus(hCompletionPort, &bytesTransferred, (PULONG_PTR)&perHandleData, (LPOVERLAPPED*)&perIoData, INFINITE))
        {
            char buf[BUFF_SIZE_IP] = { 0, };
            cout << "Disconnected[" << perHandleData->hClntSock << "]: " << inet_ntop(AF_INET, &perHandleData->clntAddr.sin_addr, buf, sizeof(buf)) << " " << endl;
            SessionPool::GetInstance().DeleteSession(perHandleData->hClntSock);

            closesocket(perHandleData->hClntSock);
            delete perHandleData;
            delete perIoData;
            continue;
        }

        if (perIoData->operation == OP_ClientToServer)
        {
            auto session = SessionPool::GetInstance().GetSession(perHandleData->hClntSock);
            if (session != nullptr)
            {
                PacketProcessor::GetInstance().Process(session, perIoData->wsaBuf.buf, static_cast<size_t>(bytesTransferred));
            }
            else
            {
                cout << "Not found session!" << endl;
            }

            memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
            perIoData->wsaBuf.len = MAX_BUFF_SIZE;
            perIoData->wsaBuf.buf = perIoData->buffer;
            perIoData->operation = OP_ClientToServer;
            flags = 0;
            if (WSARecv(perHandleData->hClntSock, &(perIoData->wsaBuf), 1, NULL, &flags, &(perIoData->overlapped), NULL) == SOCKET_ERROR)
            {
                auto e = WSAGetLastError();
                if (e != WSA_IO_PENDING)
                {
                    TPError::GetInstance().PrintError(L"WSARecv() Error", e);
                }
            }
        }
        else if (perIoData->operation == OP_ServerToClient)
        {
            delete perIoData;
        }
    }
}