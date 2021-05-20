#include "TPServer.h"
#include "PacketProcessor.h"
#include "PacketService.h"
#include "PacketGenerator.h"
#include "TPError.h"
#include "SessionPool.h"

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

void PacketProcessor::SendPacket(const Packet* packet)
{		
	switch (packet->GetPacketCastType())
	{
	case PACKET_CAST_TYPE::UNICAST:
		SendPacket(packet, packet->GetOwner());
		break;
	case PACKET_CAST_TYPE::BROADCAST:
		SendPacketAll(packet, true);
		break;
	case PACKET_CAST_TYPE::MULTICAST:
		SendPacketAll(packet, false);
		break;
	default:
		break;
	}
	delete packet;
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
		auto e = WSAGetLastError();
		if (e != WSA_IO_PENDING)
		{
			TPError::GetInstance().PrintError(L"WSASend() Error", e);
		}
	}
}

void PacketProcessor::SendPacketAll(const Packet* const packet, bool isIgnoreCastGroup)
{	
	auto packetCastGroup = packet->GetPacketCastGroup();
	auto sessionMap = SessionPool::GetInstance().GetSessionMap();

	bool found = false;

	for (auto p : sessionMap)
	{
		found = false;
		for (auto session : packetCastGroup)
		{
			if (session->GetClntSock() == p.second->GetClntSock())
			{
				found = true;
				break;
			}
		}
		if (isIgnoreCastGroup ? found : !found)
		{
			continue;
		}
		auto clntSock = p.first;
		SendPacket(packet, clntSock);
	}
}