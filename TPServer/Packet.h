#pragma once

#include "Protocol.h"
#include "Session.h"
#include "Uncopyable.h"

class Packet : public Uncopyable
{
public:
	Packet();	
	Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header);
	Packet(char* const _uffer, ULONG _packetSize, PROTOCOL _header, Session* const _owner);
	Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, Session* const _owner, bool _isBcast);
	Packet(char* const _buffer, ULONG _packetSize, PROTOCOL _header, Session* const _owner, bool _isBcast, bool _isDAlloc);
	~Packet();	
	
	PROTOCOL GetHeader() const;
	char* GetBody() const;
	char* GetBuffer() const;
	ULONG GetPacketSize() const;
	Session* GetOwner() const;	
	bool GetIsBcast() const;

	bool IsValid();
private:	
	PROTOCOL header;
	char* body;
	char* buffer;
	ULONG packetSize;
	Session* owner;
	bool isBcast;
	bool isDAlloc;
};