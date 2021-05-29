#pragma once

#include "Protocol.h"
#include "TPDefine.h"
#include "Enums.h"

#include <vector>

using namespace std;

class Session;
class PacketInfo;
class PacketSubInfo;

class Packet
{
public:
	Packet();
	Packet(const Packet& rhs);
	Packet& operator=(const Packet& rhs);
	Packet(Packet&& rhs) noexcept;
	Packet& operator=(Packet&& rhs) noexcept;
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
	void Clear();	

private:	
	void Alloc(const Packet& rhs);

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
		init(_owner, PACKET_CAST_TYPE::UNICAST, vector<Session*>(), true);
	}
	PacketSubInfo(Session* const _owner, PACKET_CAST_TYPE _packetCastType)
	{
		init(_owner, _packetCastType, vector<Session*>(), true);
	}
	PacketSubInfo(Session* const _owner, PACKET_CAST_TYPE _packetCastType, const vector<Session*>& _packetCastGroup)
	{
		init(_owner, _packetCastType, _packetCastGroup, true);
	}
	PacketSubInfo(Session* const _owner, PACKET_CAST_TYPE _packetCastType, const vector<Session*>& _packetCastGroup, bool _isDAllocBuf)
	{
		init(_owner, _packetCastType, _packetCastGroup, _isDAllocBuf);
	}

	Session* GetOwner() const { return owner; }
	PACKET_CAST_TYPE GetPacketCastType() const { return packetCastType; }
	bool GetIsDAllocBuf() const { return isDAllocBuf; }
	vector<Session*> GetPacketCastGroup() const { return packetCastGroup; }

private:
	void init(Session* const _owner, PACKET_CAST_TYPE _packetCastType, const vector<Session*>& _packetCastGroup, bool _isDAllocBuf)
	{
		owner = _owner;
		packetCastType = _packetCastType;
		packetCastGroup = _packetCastGroup;
		isDAllocBuf = _isDAllocBuf;
	}

	Session* owner;
	PACKET_CAST_TYPE packetCastType;
	bool isDAllocBuf;
	vector<Session*> packetCastGroup;
};