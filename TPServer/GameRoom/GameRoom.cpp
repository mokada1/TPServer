#include "GameRoom.h"
#include <iostream>

using namespace std;

GameRoom::GameRoom(int _roomId)
{
	this->roomId = _roomId;
}

GameRoom::~GameRoom()
{
	if (!objUserMap.empty())
	{
		objUserMap.clear();
	}	
}

int GameRoom::GetRoomId() const
{
	return roomId;
}

map<wstring, shared_ptr<ObjUser>> GameRoom::GetObjUserMap() const
{
	return objUserMap;
}

bool GameRoom::AddObjUser(shared_ptr<ObjUser> objUser)
{	
	auto userId = objUser->GetUserId();
	auto it = objUserMap.find(userId);
	if (it == objUserMap.end())
	{
		objUserMap.insert(pair<wchar_t*, shared_ptr<ObjUser>>(objUser->GetUserId(), objUser));
		return true;
	}
	return false;
}

bool GameRoom::DeleteObjUser(wchar_t* const userId)
{
	auto it = objUserMap.find(userId);
	if (it != objUserMap.end())
	{
		objUserMap.erase(it);
		return true;
	}
	return false;
}

shared_ptr<ObjUser> GameRoom::GetObjUser(const wchar_t* const userId) const
{
	auto it = objUserMap.find(userId);
	if (it != objUserMap.end())
	{
		return it->second;
	}
	return nullptr;
}
