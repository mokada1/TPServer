#include "GameRoom.h"

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

void GameRoom::AddObjUser(shared_ptr<ObjUser> objUser)
{	
	auto userId = objUser->GetUserId();
	auto it = objUserMap.find(userId);
	if (it == objUserMap.end())
	{
		objUserMap.insert(pair<wchar_t*, shared_ptr<ObjUser>>(objUser->GetUserId(), objUser));
	}	
}
