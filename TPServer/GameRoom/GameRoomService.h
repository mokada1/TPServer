#pragma once

#include "../Util/TSingleton.h"
#include "GameRoom.h"
#include <map>

class GameRoomService : public TSingleton<GameRoomService>
{
public:
	void CreateGameRoom();
	bool DeleteGameRoom(const int roomId);
	bool AddObjUser(shared_ptr<ObjUser> objUser);
	bool AddObjUser(const int roomId, shared_ptr<ObjUser> objUser);
	bool DeleteObjUser(shared_ptr<ObjUser> objUser);
	GameRoom* GetGameRoom() const;
	GameRoom* GetGameRoom(const int roomId) const;
	GameRoom* GetGameRoom(const wchar_t* const userId) const;
	shared_ptr<ObjUser> GetObjUser(const wchar_t* const userId) const;
	shared_ptr<ObjUser> GetObjUser(const int roomId, const wchar_t* const userId) const;

private:
	int LAST_ROOM_ID = 0;
	map<int, GameRoom*> roomMap;
};