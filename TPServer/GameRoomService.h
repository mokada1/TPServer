#pragma once

#include "TSingleton.h"
#include "GameRoom.h"
#include <vector>

class GameRoomService : public TSingleton<GameRoomService>
{
public:
	void CreateGameRoom();
	bool DeleteGameRoom(const int roomId);
	bool AddObjUser(shared_ptr<ObjUser> objUser);
	bool AddObjUser(const int roomId, shared_ptr<ObjUser> objUser);
	bool DeleteObjUser(wchar_t* const userId);
	GameRoom* GetGameRoom();
	GameRoom* GetGameRoom(const int roomId);

private:
	int LAST_ROOM_ID = 0;
	vector<GameRoom*> roomList;
};