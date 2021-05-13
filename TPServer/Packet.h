#pragma once

#include "Protocol.h"
#include "Session.h"

class Packet
{
public:
	Packet();
	Packet(char* buffer, ULONG _packetSize, PROTOCOL header, char* body);
	PROTOCOL GetHeader() const;
	char* GetBody() const;
	char* GetBuffer() const;
	ULONG GetPacketSize() const;
	const Session& GetOwner() const;	
	void SetOwner(Session& owner);	
	bool IsValid();
private:	
	PROTOCOL header;
	char* body;
	char* buffer;
	ULONG packetSize;
	const Session* owner;	
};