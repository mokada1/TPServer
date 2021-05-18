#pragma once

#include "Protocol.h"
#include "Uncopyable.h"
#include "TPDefine.h"
#include "Enums.h"

#include <vector>

using namespace std;

class Session;
class PacketInfo;
class PacketSubInfo;

class Packet : public Uncopyable
{
public:
	Packet(const PacketInfo& packetInfo, const PacketSubInfo& packetSubInfo);
	~Packet();	
	
	PROTOCOL GetHeader() const;
	char* GetBody() const;
	char* GetBuffer() const;
	ULONG GetPacketSize() const;
	Session* GetOwner() const;	
	PACKET_CAST_TYPE GetPacketCastType() const;
	vector<Session*> GetPacketCastGroup() const;
	bool IsValid() const;
private:	
	PROTOCOL header;
	char* body;
	char* buffer;
	ULONG packetSize;
	Session* owner;
	PACKET_CAST_TYPE packetCastType;
	vector<Session*> packetCastGroup;
	bool isDAllocBuf;
};

class PacketInfo
{
public:
	PacketInfo(char* const _buffer, ULONG _packetSize, PROTOCOL _header)
	{
		buffer = _buffer;
		packetSize = _packetSize;
		header = _header;
	}
	
	char* GetBuffer() const { return buffer; }
	ULONG GetPacketSize() const { return packetSize; }
	PROTOCOL GetHeader() const { return header; }

private:
	char* buffer;
	ULONG packetSize;
	PROTOCOL header;
};

class PacketSubInfo
{
public:
	PacketSubInfo(Session* const _owner)
	{
		this->PacketSubInfo::PacketSubInfo(_owner, PACKET_CAST_TYPE::UNICAST, vector<Session*>(), true);
	}
	PacketSubInfo(Session* const _owner, PACKET_CAST_TYPE _packetCastType)
	{
		this->PacketSubInfo::PacketSubInfo(_owner, _packetCastType, vector<Session*>(), true);
	}
	PacketSubInfo(Session* const _owner, PACKET_CAST_TYPE _packetCastType, const vector<Session*>& _packetCastGroup)
	{
		this->PacketSubInfo::PacketSubInfo(_owner, _packetCastType, _packetCastGroup, true);
	}
	PacketSubInfo(Session* const _owner, PACKET_CAST_TYPE _packetCastType, const vector<Session*>& _packetCastGroup, bool _isDAllocBuf)
	{
		owner = _owner;
		packetCastType = _packetCastType;
		packetCastGroup = _packetCastGroup;
		isDAllocBuf = _isDAllocBuf;
	}

	Session* GetOwner() const { return owner; }
	PACKET_CAST_TYPE GetPacketCastType() const { return packetCastType; }
	bool GetIsDAllocBuf() const { return isDAllocBuf; }
	vector<Session*> GetPacketCastGroup() const { return packetCastGroup; }

private:
	Session* owner;
	PACKET_CAST_TYPE packetCastType;
	bool isDAllocBuf;
	vector<Session*> packetCastGroup;
};