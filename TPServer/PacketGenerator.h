#pragma once

#include "TSingleton.h"
#include "Packet.h"
#include "ObjUser.h"

class PacketGenerator : public TSingleton<PacketGenerator>
{
public:
	Packet* CreateError(const wchar_t* message);
	Packet* CreateResLogin(const ObjUser& objUser);

private:
	void SetHeaderOfBuff(char* buffer, PROTOCOL header);
};