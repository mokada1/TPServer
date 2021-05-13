#include "Packet.h"

Packet::Packet()
{
	this->header = static_cast<PROTOCOL>(0);
	this->body = nullptr;
	this->buffer = nullptr;
	this->packetSize = 0;
	this->owner = nullptr;
}

Packet::Packet(char* buffer, ULONG _packetSize, PROTOCOL header, char* body)
{
	this->header = header;
	this->body = body;
	this->buffer = buffer;
	this->packetSize = _packetSize;
	this->owner = nullptr;	
}

PROTOCOL Packet::GetHeader() const
{
	return this->header;
}

char* Packet::GetBody() const
{
	return this->body;
}

char* Packet::GetBuffer() const
{
	return this->buffer;
}

ULONG Packet::GetPacketSize() const
{
	return this->packetSize;
}

const Session& Packet::GetOwner() const
{
	return *(this->owner);
}

void Packet::SetOwner(Session& owner)
{
	this->owner = &owner;
}

bool Packet::IsValid()
{
	return packetSize > 0;
}