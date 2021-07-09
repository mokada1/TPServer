#include "../Core/TPServer.h"
#include "PacketProcessor.h"
#include "PacketService.h"
#include "PacketGenerator.h"
#include "../Util/TPLogger.h"
#include "../Session/SessionPool.h"

void PacketProcessor::Parse(Session* const owner, char* const buffer, const size_t recvBytes)
{
	auto packet = PacketGenerator::GetInstance().Parse(owner, buffer, recvBytes);
	if (!packet)
	{
		return;
	}
	if (!packet->IsValid())
	{
		delete packet;
		return;
	}

	auto clntSock = packet->GetOwner()->GetClntSock();
	auto header = packet->GetHeader();
	auto strHeader = TPUtil::GetInstance().EnumToString(header);

	TPLogger::GetInstance().PrintLog("[%d]Recv packet:%s", clntSock, strHeader);

	packetRecvQueue.push(packet);
}

bool PacketProcessor::ProcRecvPacket()
{
	if (packetRecvQueue.empty())
	{
		return false;
	}
	Packet* packet = nullptr;
	if (packetRecvQueue.try_pop(packet))
	{
		PacketService::GetInstance().Process(*packet);
		delete packet;
		return true;
	}
	return false;
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
			TPLogger::GetInstance().PrintLog("WSASend() Error", e);
		}
	}

	auto header = packet.GetHeader();
	auto strHeader = TPUtil::GetInstance().EnumToString(header);

	TPLogger::GetInstance().PrintLog("[%d]Send packet:%s", clntSock, strHeader);
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