#pragma once

#include "ObjUser.h"
#include <map> 

class GameRoom
{
public:
	GameRoom(int _roomId);
	~GameRoom();
	
	int GetRoomId() const;

	void AddObjUser(shared_ptr<ObjUser> objUser);

private:
	int roomId;
	map<wchar_t*, shared_ptr<ObjUser>> objUserMap;
};