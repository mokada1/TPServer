#include "GameRoomService.h"

void GameRoomService::CreateGameRoom()
{
	roomList.push_back(new GameRoom(++LAST_ROOM_ID));
}

bool GameRoomService::DeleteGameRoom(const int roomId)
{
	for (auto it = roomList.begin(); it != roomList.end(); ++it)
	{
		auto gr = *it;
		if (gr->GetRoomId() == roomId)
		{
			roomList.erase(it);
			delete gr;
			return true;
		}
	}
	return false;
}

bool GameRoomService::AddObjUser(shared_ptr<ObjUser> objUser)
{
	int roomId = 0;
	if(roomList.empty())
	{
		CreateGameRoom();
		roomId = LAST_ROOM_ID;
	}
	else
	{
		auto room = roomList.front();
		roomId = room->GetRoomId();
	}
	return AddObjUser(roomId, objUser);
}

bool GameRoomService::AddObjUser(const int roomId, shared_ptr<ObjUser> objUser)
{
	for (auto it = roomList.begin(); it != roomList.end(); ++it)
	{
		auto gr = *it;
		if (gr->GetRoomId() == roomId)
		{
			gr->AddObjUser(objUser);
			return true;
		}
	}
	return false;
}
