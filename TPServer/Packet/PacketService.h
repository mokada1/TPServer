#pragma once

#include "../Util/TSingleton.h"

class Packet;
class TPResult;

class PacketService : public TSingleton<PacketService>
{
public:
	void Process(const Packet& packet);

private:
	TPResult* ProcReqLogin(const Packet& packet);
	TPResult* ProcReqMove(const Packet& packet);
	TPResult* ProcReqRoundTripTime(const Packet& packet);
	TPResult* ProcReqLocationSync(const Packet& packet);
	TPResult* ProcReqAction(const Packet& packet);
	TPResult* ProcReqDamage(const Packet& packet);
	TPResult* ProcReqRotate(const Packet& packet);
};