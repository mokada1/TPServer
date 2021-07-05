#pragma once

#include "../Util/TSingleton.h"
#include "../Object/ObjUser.h"

#include <vector>

using namespace std;

class BattleService : public TSingleton<BattleService>
{
public:
	vector<shared_ptr<ObjUser>> GetObjUserListAround(shared_ptr<ObjUser> target);

private:
	const float DISTANCE_AROUND = 100.f;
};