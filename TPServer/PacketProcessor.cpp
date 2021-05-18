#include "TPServer.h"
#include "PacketProcessor.h"
#include "PacketService.h"
#include "PacketGenerator.h"
#include "TPError.h"
#include "TPDefine.h"

#include <iostream>

using namespace std;

void PacketProcessor::Process(Session* const owner, char* const buffer, const DWORD bytesTransferred)
{
	auto packet = PacketGenerator::GetInstance().Parse(buffer, bytesTransferred, owner);
	auto clntSock = packet->GetOwner()->GetClntSock();
	auto header = packet->GetHeader();
	
	cout << "[" << clntSock << "]" << "Protocol:" << static_cast<uint16_t>(header) << endl;

	PacketService::GetInstance().Process(*packet);
	delete packet;
}

void PacketProcessor::SendPacket(const Packet* const packet)
{		
	if (packet->GetIsBcast())
	{
		SendPacketAll(packet);
	}
	else
	{
		SendPacket(packet, packet->GetOwner());
	}	
}

void PacketProcessor::SendPacket(const Packet* const packet, const Session* const session)
{
	SendPacket(packet, session->GetClntSock());
}

void PacketProcessor::SendPacket(const Packet* const packet, const SOCKET& clntSock)
{
	LPPER_IO_DATA PerIoData = new PER_IO_DATA;
	int sendBytes = 0;

	memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
	PerIoData->wsaBuf.len = packet->GetPacketSize();
	PerIoData->wsaBuf.buf = packet->GetBuffer();
	PerIoData->operation = OP_ServerToClient;
	if (WSASend(clntSock, &(PerIoData->wsaBuf), 1, (LPDWORD)&sendBytes, 0, &(PerIoData->overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			TPError::ErrorHandling("WSASend() error");
	}
}

void PacketProcessor::SendPacketAll(const Packet* const packet)
{	
	auto sessionMap = SessionPool::GetInstance().GetSessionMap();
	for (auto p : sessionMap)
	{
		auto clntSock = p.first;

		SendPacket(packet, clntSock);
	}
}