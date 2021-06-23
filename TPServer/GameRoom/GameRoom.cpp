#include "GameRoom.h"
#include <iostream>

using namespace std;

GameRoom::GameRoom(int _roomId)
{
	this->roomId = _roomId;
	this->avgRttMs = 0;
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

int64_t GameRoom::GetAvgRttMs() const
{
	return avgRttMs;
}

bool GameRoom::AddObjUser(shared_ptr<ObjUser> objUser)
{	
	auto userId = objUser->GetUserId();
	auto it = objUserMap.find(userId);
	if (it == objUserMap.end())
	{
		objUser->SetRoomId(roomId);
		objUserMap.insert(pair<wchar_t*, shared_ptr<ObjUser>>(objUser->GetUserId(), objUser));
		UpdateAvgRttMs();
		return true;
	}
	return false;
}

bool GameRoom::DeleteObjUser(wchar_t* const userId)
{
	auto it = objUserMap.find(userId);
	if (it != objUserMap.end())
	{
		it->second->SetRoomId(0);
		objUserMap.erase(it);
		UpdateAvgRttMs();
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

void GameRoom::UpdateAvgRttMs()
{
	auto userNum = objUserMap.size();
	if (userNum == 0)
	{
		return;
	}

	int64_t totalRttMs = 0;
	for (auto it = objUserMap.begin(); it != objUserMap.end(); ++it)
	{
		totalRttMs += it->second->GetAvgRttMs();
	}
	avgRttMs = totalRttMs / userNum;
}