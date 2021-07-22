#include "BattleService.h"
#include "../GameRoom/GameRoomService.h"
#include "../Util/TPUtil.h"
#include "../Util/TPLogger.h"
#include "../Packet/PacketProcessor.h"
#include "../Packet/PacketGeneratorServer.h"

void BattleService::PlayAbility(const shared_ptr<ObjUser>& caster, AbilityType abilityType)
{
	switch (abilityType)
	{
	case AbilityType::AbilityType_MeleeAttack1:
		PlayMeleeAttack1(caster);
		break;
	case AbilityType::AbilityType_MeleeAttack2_1:
		PlayMeleeAttack2_1(caster);
		break;
	case AbilityType::AbilityType_MeleeAttack2_2:
		PlayMeleeAttack2_2(caster);
		break;
	case AbilityType::AbilityType_MeleeAttack3:
		PlayMeleeAttack3(caster);
		break;
	}
}

void BattleService::PlayMeleeAttack1(const shared_ptr<ObjUser>& caster)
{
	MeleeAttack(caster, MELEE_ATTACK_1_COLLISION_DIST, MELEE_ATTACK_1_COLLISION_SIZE, MELEE_ATTACK_1_DAMAGE);
}

void BattleService::PlayMeleeAttack2_1(const shared_ptr<ObjUser>& caster)
{
	MeleeAttack(caster, MELEE_ATTACK_2_1_COLLISION_DIST, MELEE_ATTACK_2_1_COLLISION_SIZE, MELEE_ATTACK_2_1_DAMAGE);
}

void BattleService::PlayMeleeAttack2_2(const shared_ptr<ObjUser>& caster)
{
	MeleeAttack(caster, MELEE_ATTACK_2_2_COLLISION_DIST, MELEE_ATTACK_2_2_COLLISION_SIZE, MELEE_ATTACK_2_2_DAMAGE);
}

void BattleService::PlayMeleeAttack3(const shared_ptr<ObjUser>& caster)
{
	MeleeAttack(caster, MELEE_ATTACK_3_COLLISION_DIST, MELEE_ATTACK_3_COLLISION_SIZE, MELEE_ATTACK_3_DAMAGE);
}

void BattleService::MeleeAttack(const shared_ptr<ObjUser>& caster, const Vector3 collisionDistance, const float collisionSize, const float damage)
{
	auto hitList = Hit(caster, collisionDistance, collisionSize);
	if (hitList.empty())
	{
		return;
	}

	for (auto& objUser : hitList)
	{		
		ApplyDamage(objUser, damage);
	}

	auto packet = PacketGeneratorServer::GetInstance().CreateBcastHit(hitList);
	PacketProcessor::GetInstance().SendPacket(packet);
}

void BattleService::ApplyDamage(const shared_ptr<ObjUser>& target, const float damage)
{
	auto compCondition = target->GetCompCondition();
	if (compCondition->GetIsDied())
	{
		return;
	}

	auto compAttribute = target->GetCompAttribute();

	float hp = 0.f;
	float hpAfterDamage = 0.f;	
	do
	{
		hp = compAttribute->GetHp();
		hpAfterDamage = hp - damage < 0 ? 0 : hp - damage;
	} while (!compAttribute->SetHp(hp, hpAfterDamage));

	if (hpAfterDamage <= 0.f)
	{
		auto compCondition = target->GetCompCondition();
		if (!compCondition->GetIsDied())
		{
			compCondition->SetIsDied(true);
		}		
	}
}

vector<shared_ptr<ObjUser>> BattleService::Hit(const shared_ptr<ObjUser>& caster, const Vector3 collisionDistance, const float collisionSize)
{
	vector<shared_ptr<ObjUser>> hitList;
	auto gameRoom = GameRoomService::GetInstance().GetGameRoom(caster->GetRoomId());
	if (!gameRoom)
	{
		return hitList;
	}	

	auto compTransformCaster = caster->GetCompTransform();
	const auto locationCaster = compTransformCaster->GetLocation();
	const auto rotationCaster = compTransformCaster->GetRotation();
	const auto transCollisionDist = TPUtil::GetInstance().RotateByZAxis(collisionDistance, rotationCaster.z);
	Vector3 collisionLocation = {
		locationCaster.x + transCollisionDist.x,
		locationCaster.y + transCollisionDist.y,
		locationCaster.z + transCollisionDist.z
	};
	auto userIdCaster = caster->GetUserId();

	auto objUserMap = gameRoom->GetObjUserMap();
	for (auto& p : objUserMap)
	{
		auto objUser = p.second;
		auto compCondition = objUser->GetCompCondition();

		if (compCondition->GetIsDied())
		{
			continue;
		}

		if (wcscmp(userIdCaster, objUser->GetUserId()) == 0)
		{
			continue;
		}

		auto compTransform = objUser->GetCompTransform();
		auto location = compTransform->GetLocation();
		auto distance = TPUtil::GetInstance().Distance(collisionLocation, location);

		if (distance <= collisionSize)
		{
			hitList.emplace_back(objUser);
		}
	}

	return hitList;
}