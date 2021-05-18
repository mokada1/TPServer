#include "GameRoomService.h"
#include "TPDefine.h"
#include <iostream> 

using namespace std;

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
		auto gameRoom = *it;
		if (gameRoom->GetRoomId() == roomId)
		{
			if (gameRoom->AddObjUser(objUser))
			{
				wcout << SUCCESS_ADD_OBJ_USER_GAME_ROOM << endl;
				return true;
			}			
		}
	}
	return false;
}

bool GameRoomService::DeleteObjUser(wchar_t* const userId)
{
	for (auto& room : roomList)
	{
		if (room->DeleteObjUser(userId))
		{
			wcout << DELETE_OBJ_USER_GAME_ROOM << endl;
			return true;
		}
	}
	return false;
}

GameRoom* GameRoomService::GetGameRoom() const
{
	if (roomList.empty())
	{
		return nullptr;
	}
	auto room = roomList.front();
	auto roomId = room->GetRoomId();
	return GetGameRoom(roomId);
}

GameRoom* GameRoomService::GetGameRoom(const int roomId) const
{
	for (auto it = roomList.begin(); it != roomList.end(); ++it)
	{
		auto gameRoom = *it;
		if (gameRoom->GetRoomId() == roomId)
		{
			return gameRoom;
		}
	}
	return nullptr;
}

shared_ptr<ObjUser> GameRoomService::GetObjUser(const wchar_t* const userId) const
{
	for (auto& room : roomList)
	{
		auto objUser = room->GetObjUser(userId);
		if (objUser)
		{
			return objUser;
		}
	}
	return nullptr;
}
