#pragma once

#include "Packet.h"
#include "../TP_generated.h"

class PacketGenerator
{
public:
	vector<Packet*> Parse(Session* const owner, char* const buffer, const size_t recvBytes);
	
protected:
	Packet CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner, PacketCastType packetCastType);
	Packet CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner);
	Packet CreatePacket(PROTOCOL header, flatbuffers::FlatBufferBuilder& _fbb, Session* const owner, PacketCastType packetCastType, vector<Session*> packetCastGroup);
	PROTOCOL GetHeaderByBuff(char* const buffer);
	PROTOCOL GetEndOfPacket(char* const buffer, const size_t buffSize, size_t& packetSize);
	void SetHeaderOfBuff(char* const buffer, PROTOCOL header);
	void SetEndOfBuff(char* const buffer, const size_t buffSize);
	bool IsValidHeader(const PROTOCOL protocol);
	bool IsValidEndOfPacket(const PROTOCOL protocol);
};