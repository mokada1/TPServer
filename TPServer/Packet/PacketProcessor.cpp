#include "../Core/TPServer.h"
#include "PacketProcessor.h"
#include "PacketService.h"
#include "PacketGenerator.h"
#include "../Util/TPError.h"
#include "../Session/SessionPool.h"

#include <iostream>

using namespace std;

void PacketProcessor::Process(Session* const owner, char* const buffer, const size_t recvBytes)
{
	const auto& packet = PacketGenerator::GetInstance().Parse(owner, buffer, recvBytes);
	if (!packet.IsValid())
	{
		return;
	}

	auto clntSock = packet.GetOwner()->GetClntSock();
	auto header = packet.GetHeader();
	auto strHeader = TPUtil::GetInstance().EnumToString(header);

	cout << "[" << clntSock << "]" << "Recv Packet:" << strHeader << endl;

	PacketService::GetInstance().Process(packet);
}

void PacketProcessor::SendPacket(const Packet& packet)
{		
	switch (packet.GetPacketCastType())
	{
	case PACKET_CAST_TYPE::UNICAST:
		SendPacket(packet, packet.GetOwner());
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
}

void PacketProcessor::SendPacket(const Packet& packet, const Session* const session)
{
	SendPacket(packet, session->GetClntSock());
}

void PacketProcessor::SendPacket(const Packet& packet, const SOCKET& clntSock)
{
	LPPER_IO_DATA perIoData = new PER_IO_DATA;
	DWORD sendBytes = 0;

	memset(&(perIoData->overlapped), 0, sizeof(OVERLAPPED));
	perIoData->wsaBuf.len = static_cast<ULONG>(packet.GetPacketSize());
	perIoData->wsaBuf.buf = packet.GetBuffer();
	perIoData->operation = OP_ServerToClient;
	if (WSASend(clntSock, &(perIoData->wsaBuf), 1, &sendBytes, 0, &(perIoData->overlapped), NULL) == SOCKET_ERROR)
	{		
		auto e = WSAGetLastError();
		if (e != WSA_IO_PENDING)
		{
			TPError::GetInstance().PrintError(L"WSASend() Error", e);
		}
	}

	auto header = packet.GetHeader();
	auto strHeader = TPUtil::GetInstance().EnumToString(header);

	cout << "[" << clntSock << "]" << "Send Packet:" << strHeader << endl;
}

void PacketProcessor::SendPacketAll(const Packet& packet, bool isIgnoreCastGroup)
{	
	auto packetCastGroup = packet.GetPacketCastGroup();
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