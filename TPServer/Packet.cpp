#include "Packet.h"
#include "TPDefine.h"

Packet::Packet()
{
	this->Packet::Packet(nullptr, 0, static_cast<PROTOCOL>(0), nullptr, false, false);
}

Packet::Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header)
{
	this->Packet::Packet(_buffer, _packetSize, _header, nullptr, false, true);
}

Packet::Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, Session* const _owner)
{
	this->Packet::Packet(_buffer, _packetSize, _header, _owner, false, true);
}

Packet::Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, Session* const _owner, bool _isBcast)
{	
	this->Packet::Packet(_buffer, _packetSize, _header, _owner, _isBcast, true);
}

Packet::Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, Session* const _owner, bool _isBcast, bool _isDAlloc)
{
	this->header = _header;
	this->body = &_buffer[PACKET_HEAD_SIZE];
	this->buffer = _buffer;
	this->packetSize = _packetSize;
	this->owner = _owner;
	this->isBcast = _isBcast;
	this->isDAlloc = _isDAlloc;
}

Packet::~Packet()
{	
	if (isDAlloc && IsValid())
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

bool Packet::GetIsBcast() const
{
	return isBcast;
}

bool Packet::IsValid()
{
	return packetSize > 0 && buffer != nullptr;
}