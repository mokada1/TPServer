#include "BattleService.h"
#include "../GameRoom/GameRoomService.h"
#include "../Util/TPUtil.h"

vector<shared_ptr<ObjUser>> BattleService::GetObjUserListAround(shared_ptr<ObjUser> target)
{
	vector<shared_ptr<ObjUser>> objUserList;
	auto gameRoom = GameRoomService::GetInstance().GetGameRoom(target->GetRoomId());
	if (!gameRoom)
	{
		return objUserList;
	}	

	auto tCompUserLocation = target->GetCompUserTransform();
	auto tLocation = tCompUserLocation->GetLocation();

	auto objUserMap = gameRoom->GetObjUserMap();
	for (auto p : objUserMap)
	{
		auto objUser = p.second;
		if (wcscmp(target->GetUserId(), objUser->GetUserId()) == 0)
		{
			continue;
		}

		auto compUserLocation = objUser->GetCompUserTransform();
		auto location = compUserLocation->GetLocation();
		if (TPUtil::GetInstance().Distance(tLocation, location) <= DISTANCE_AROUND)
		{
			objUserList.emplace_back(objUser);
		}
	}

	return objUserList;
}