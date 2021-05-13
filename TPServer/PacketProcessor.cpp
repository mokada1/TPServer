#include "TPServer.h"
#include "PacketProcessor.h"
#include "PacketService.h"
#include <iostream>
#include "SessionPool.h"
#include "TPError.h"
#include "TPDefine.h"

using namespace std;

void PacketProcessor::Process(Session& owner, const char* datas, const DWORD bytesTransferred)
{
	auto packet = Parse(datas, bytesTransferred);	
	auto header = packet.GetHeader();
	auto body = packet.GetBuffer();
	packet.SetOwner(owner);
	
	cout << "[" << packet.GetOwner().GetClntSock() << "]" << "Protocol:" << static_cast<uint16_t>(header) << endl;

	PacketService::GetInstance().Process(packet);
}

void PacketProcessor::SendPacketAll(const Packet& packet)
{
	auto sessionList = SessionPool::GetInstance().GetSessionAll();
	for (auto session : sessionList)
	{
		if (packet.GetOwner().GetClntSock() == session->GetClntSock())
		{
			continue;
		}
		SendPacket(packet, session->GetClntSock());
	}
}

void PacketProcessor::SendPacket(const Packet& packet, const SOCKET& clntSock)
{
	LPPER_IO_DATA PerIoData = new PER_IO_DATA;
	int sendBytes = 0;

	memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
	PerIoData->wsaBuf.len = packet.GetPacketSize();
	PerIoData->wsaBuf.buf = packet.GetBuffer();
	PerIoData->operation = OP_ServerToClient;
	if (WSASend(clntSock, &(PerIoData->wsaBuf), 1, (LPDWORD)&sendBytes, 0, &(PerIoData->overlapped), NULL) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
			TPError::ErrorHandling("WSASend() error");
	}
}

Packet PacketProcessor::Parse(const char* datas, const DWORD bytesTransferred)
{
	uint16_t headerInt16 = datas[0] | static_cast<uint16_t>(datas[1]) << 8;
	PROTOCOL header = static_cast<PROTOCOL>(headerInt16);
	auto body = const_cast<char*>(&datas[HEAD_SIZE]);
	auto buffer = const_cast<char*>(datas);
	return Packet(buffer, bytesTransferred, header, body);
}