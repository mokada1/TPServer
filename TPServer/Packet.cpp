#include "Packet.h"

Packet::Packet(const PacketInfo& packetInfo, const PacketSubInfo& packetSubInfo)
{
	auto _buffer = packetInfo.GetBuffer();
	this->header = packetInfo.GetHeader();
	this->body = &_buffer[PACKET_HEAD_SIZE];
	this->buffer = _buffer;
	this->packetSize = packetInfo.GetPacketSize();
	this->owner = packetSubInfo.GetOwner();
	this->packetCastType = packetSubInfo.GetPacketCastType();
	this->packetCastGroup = packetSubInfo.GetPacketCastGroup();
	this->isDAllocBuf = packetSubInfo.GetIsDAllocBuf();
}

Packet::~Packet()
{	
	if (isDAllocBuf && IsValid())
	{
		delete[] buffer;
	}
}

PROTOCOL Packet::GetHeader() const
{
	return header;
}

char* Packet::GetBody() const
{
	return body;
}

char* Packet::GetBuffer() const
{
	return buffer;
}

ULONG Packet::GetPacketSize() const
{
	return packetSize;
}

Session* Packet::GetOwner() const
{
	return owner;
}

PACKET_CAST_TYPE Packet::GetPacketCastType() const
{
	return packetCastType;
}

vector<Session*> Packet::GetPacketCastGroup() const
{
	return packetCastGroup;
}

bool Packet::IsValid() const
{
	return packetSize > 0 && buffer != nullptr;
}