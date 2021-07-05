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
	void ProcReqMove(const Packet& packet);
	TPResult* ProcReqRoundTripTime(const Packet& packet);
	void ProcReqLocationSync(const Packet& packet);
	void ProcReqAction(const Packet& packet);
	void ProcReqDamage(const Packet& packet);
	void ProcReqRotate(const Packet& packet);
};