#pragma once

#include "ObjUser.h"
#include <map> 

class GameRoom
{
public:
	GameRoom(int _roomId);
	~GameRoom();
	
	int GetRoomId() const;
	map<wstring, shared_ptr<ObjUser>> GetObjUserMap() const;

	bool AddObjUser(shared_ptr<ObjUser> objUser);
	bool DeleteObjUser(wchar_t* const userId);

	shared_ptr<ObjUser> GetObjUser(const wchar_t* const userId) const;

private:
	int roomId;
	map<wstring, shared_ptr<ObjUser>> objUserMap;
};