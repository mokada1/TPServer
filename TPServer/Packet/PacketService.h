#pragma once

#include "../Util/Unconstructible.h"

class Packet;
class TPResult;

class PacketService : public Unconstructible
{
public:
	static PacketService& GetInstance()
	{
		static PacketService* _instance = new PacketService();
		return *_instance;
	}

	void Process(const Packet& packet);

private:
	PacketService() {}

	TPResult* ProcReqLogin(const Packet& packet);
	TPResult* ProcReqMove(const Packet& packet);
	TPResult* ProcReqRoundTripTime(const Packet& packet);
	TPResult* ProcReqLocationSync(const Packet& packet);
	TPResult* ProcReqAction(const Packet& packet);
	TPResult* ProcReqAbility(const Packet& packet);
	TPResult* ProcReqRotationSync(const Packet& packet);
};