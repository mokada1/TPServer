#include "GameRoomService.h"
#include "../Util/TPDefine.h"
#include <iostream> 

using namespace std;

void GameRoomService::CreateGameRoom()
{
	++LAST_ROOM_ID;
	roomMap.insert(pair<int, GameRoom*>(LAST_ROOM_ID, new GameRoom(LAST_ROOM_ID)));
}

bool GameRoomService::DeleteGameRoom(const int roomId)
{
	auto it = roomMap.find(roomId);
	if (it != roomMap.end())
	{
		delete it->second;
		roomMap.erase(it);
		return true;
	}
	return false;
}

bool GameRoomService::AddObjUser(shared_ptr<ObjUser> objUser)
{
	int roomId = 0;
	if(roomMap.empty())
	{
		CreateGameRoom();
		roomId = LAST_ROOM_ID;
	}
	else
	{
		auto it = roomMap.begin();
		roomId = it->second->GetRoomId();
	}
	return AddObjUser(roomId, objUser);
}

bool GameRoomService::AddObjUser(const int roomId, shared_ptr<ObjUser> objUser)
{
	auto it = roomMap.find(roomId);
	if (it != roomMap.end())
	{
		auto gameRoom = it->second;
		if (gameRoom->AddObjUser(objUser))
		{
			wcout << SUCCESS_ADD_OBJ_USER_GAME_ROOM << "[" << gameRoom->GetUserNum() << "]" << endl;
			return true;
		}
	}
	return false;
}

bool GameRoomService::DeleteObjUser(shared_ptr<ObjUser> objUser)
{
	auto it = roomMap.find(objUser->GetRoomId());
	if (it != roomMap.end())
	{
		auto gameRoom = it->second;
		if (gameRoom->DeleteObjUser(objUser->GetUserId()))
		{
			wcout << DELETE_OBJ_USER_GAME_ROOM << "[" << gameRoom->GetUserNum() << "]" << endl;
			return true;
		}
	}
	return false;
}

GameRoom* GameRoomService::GetGameRoom() const
{
	if (roomMap.empty())
	{
		return nullptr;
	}
	auto it = roomMap.begin();	
	auto roomId = it->second->GetRoomId();
	return GetGameRoom(roomId);
}

GameRoom* GameRoomService::GetGameRoom(const int roomId) const
{
	auto it = roomMap.find(roomId);
	if (it != roomMap.end())
	{
		return it->second;
	}
	return nullptr;
}

GameRoom* GameRoomService::GetGameRoom(const wchar_t* const userId) const
{
	auto objUser = GetObjUser(userId);
	if (!objUser)
	{
		return nullptr;
	}
	return GetGameRoom(objUser->GetRoomId());
}

shared_ptr<ObjUser> GameRoomService::GetObjUser(const wchar_t* const userId) const
{
	for (auto& p : roomMap)
	{
		auto objUser = p.second->GetObjUser(userId);
		if (objUser)
		{
			return objUser;
		}
	}
	return nullptr;
}

shared_ptr<ObjUser> GameRoomService::GetObjUser(const int roomId, const wchar_t* const userId) const
{
	auto it = roomMap.find(roomId);
	if (it != roomMap.end())
	{
		auto objUser = it->second->GetObjUser(userId);
		if (objUser)
		{
			return objUser;
		}
	}
	return nullptr;
}
