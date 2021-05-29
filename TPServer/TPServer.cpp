#include "TPServer.h"
#include <iostream>
#include <WS2tcpip.h>
#include "DBServer.h"
#include "PacketProcessor.h"
#include "SessionPool.h"
#include "TPError.h"

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

    SYSTEM_INFO SystemInfo;
    GetSystemInfo(&SystemInfo);
    
    for (unsigned int i = 0; i < SystemInfo.dwNumberOfProcessors; ++i)
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

    LPPER_HANDLE_DATA PerHandleData;

    int RecvBytes;
    int Flags;

    cout << "서버가 시작되었습니다." << endl;

    while (TRUE)
    {
        SOCKADDR_IN clntAddr;
        int addrLen = sizeof(clntAddr);

        SOCKET hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &addrLen);

        PerHandleData = new PER_HANDLE_DATA;
        PerHandleData->hClntSock = hClntSock;
        memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

        char buf[BUFF_SIZE_IP] = { 0, };
        cout << "Connected[" << hClntSock << "]: " << inet_ntop(AF_INET, &clntAddr.sin_addr, buf, sizeof(buf)) << " " << endl;
        SessionPool::GetInstance().CreateSession(hClntSock, clntAddr);

        CreateIoCompletionPort((HANDLE)hClntSock, hCompletionPort, (DWORD)PerHandleData, 0);

        LPPER_IO_DATA PerIoData = new PER_IO_DATA;
        memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
        PerIoData->wsaBuf.len = BUFF_SIZE;
        PerIoData->wsaBuf.buf = PerIoData->buffer;
        PerIoData->operation = OP_ClientToServer;
        Flags = 0;

        WSARecv(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, (LPDWORD)&RecvBytes, (LPDWORD)&Flags, &(PerIoData->overlapped), NULL);
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
    DWORD BytesTransferred;
    LPPER_HANDLE_DATA PerHandleData;
    LPPER_IO_DATA PerIoData;
    DWORD flags;
    int sendBytes = 0;

    while (true) {
        if (!GetQueuedCompletionStatus(hCompletionPort, &BytesTransferred, (LPDWORD)&PerHandleData, (LPOVERLAPPED*)&PerIoData, INFINITE))
        {
            char buf[BUFF_SIZE_IP] = { 0, };
            cout << "Disconnected[" << PerHandleData->hClntSock << "]: " << inet_ntop(AF_INET, &PerHandleData->clntAddr.sin_addr, buf, sizeof(buf)) << " " << endl;
            SessionPool::GetInstance().DeleteSession(PerHandleData->hClntSock);

            closesocket(PerHandleData->hClntSock);
            delete PerHandleData;
            delete PerIoData;
            continue;
        }

        if (PerIoData->operation == OP_ClientToServer)
        {
            auto session = SessionPool::GetInstance().GetSession(PerHandleData->hClntSock);
            if (session != nullptr)
            {
                PacketProcessor::GetInstance().Process(session, PerIoData->wsaBuf.buf, BytesTransferred);
            }
            else
            {
                cout << "Not found session!" << endl;
            }

            memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
            PerIoData->wsaBuf.len = BUFF_SIZE;
            PerIoData->wsaBuf.buf = PerIoData->buffer;
            PerIoData->operation = OP_ClientToServer;
            flags = 0;
            if (WSARecv(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, NULL, &flags, &(PerIoData->overlapped), NULL) == SOCKET_ERROR)
            {
                auto e = WSAGetLastError();
                if (e != WSA_IO_PENDING)
                {
                    TPError::GetInstance().PrintError(L"WSARecv() Error", e);
                }
            }
        }
        else if (PerIoData->operation == OP_ServerToClient)
        {
            delete PerIoData;
        }
    }
}