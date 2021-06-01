#include "Packet.h"

Packet::Packet()
{	
	Clear();
}

Packet::Packet(const Packet& rhs)
{
	Alloc(rhs);
}

Packet& Packet::operator=(const Packet& rhs)
{
	Alloc(rhs);
	return *this;
}

Packet::Packet(Packet&& rhs) noexcept
{
	Alloc(rhs);
	rhs.Clear();
}

Packet& Packet::operator=(Packet&& rhs) noexcept
{
	Alloc(rhs);
	rhs.Clear();
	return *this;
}

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
	return packetSize > 0 && buffer != nullptr && packetSize <= MAX_BUFF_SIZE;
}

void Packet::Clear()
{
	header = PROTOCOL::TP_ERROR;
	body = nullptr;
	buffer = nullptr;
	packetSize = 0;
	owner = nullptr;
	packetCastType = PACKET_CAST_TYPE::UNICAST;
	if(!packetCastGroup.empty()) packetCastGroup.clear();
	isDAllocBuf = false;
}

void Packet::Alloc(const Packet& rhs)
{
	if (!rhs.IsValid())
	{
		return;
	}
	if (isDAllocBuf && IsValid())
	{
		delete[] buffer;
		buffer = nullptr;
	}	
	if (rhs.isDAllocBuf)
	{
		const auto packetSizeRhs = rhs.GetPacketSize();
		buffer = new char[packetSizeRhs];
		memcpy(buffer, rhs.buffer, packetSizeRhs);
		body = &buffer[PACKET_HEAD_SIZE];
	}
	else
	{
		buffer = rhs.buffer;
		body = rhs.body;
	}
	header = rhs.header;
	packetSize = rhs.packetSize;
	owner = rhs.owner;
	packetCastType = rhs.packetCastType;
	packetCastGroup = rhs.packetCastGroup;
	isDAllocBuf = rhs.isDAllocBuf;
}
