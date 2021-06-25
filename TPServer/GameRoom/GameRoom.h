#pragma once

#include "../Object/ObjUser.h"
#include <map> 
#include <cstdint>

class GameRoom
{
public:
	GameRoom(int _roomId);
	~GameRoom();
	
	int GetRoomId() const;
	map<wstring, shared_ptr<ObjUser>> GetObjUserMap() const;
	int64_t GetAvgRttMs() const;

	bool AddObjUser(shared_ptr<ObjUser> objUser);
	bool DeleteObjUser(wchar_t* const userId);

	shared_ptr<ObjUser> GetObjUser(const wchar_t* const userId) const;
	void UpdateRtt();

private:
	int roomId;
	map<wstring, shared_ptr<ObjUser>> objUserMap;

	int64_t avgRttMs;
};