#pragma once

#include "../Util/TSingleton.h"
#include "../Object/ObjUser.h"

#include <vector>

using namespace std;

class BattleService : public TSingleton<BattleService>
{
public:
	void PlayAbility(const shared_ptr<ObjUser>& caster, AbilityType abilityType);

private:
	void PlayMeleeAttack1(const shared_ptr<ObjUser>& caster);
	void PlayMeleeAttack2_1(const shared_ptr<ObjUser>& caster);
	void PlayMeleeAttack2_2(const shared_ptr<ObjUser>& caster);
	void PlayMeleeAttack3(const shared_ptr<ObjUser>& caster);
	void MeleeAttack(const shared_ptr<ObjUser>& caster, const Vector3 collisionDistance, const float collisionSize);
	vector<shared_ptr<ObjUser>> Hit(const shared_ptr<ObjUser>& caster, const Vector3 collisionDistance, const float collisionSize);
};

static const Vector3 MELEE_ATTACK_1_COLLISION_DIST = { 150.f, 0.f, 0.f };
static const Vector3 MELEE_ATTACK_2_1_COLLISION_DIST = { 150.f, 0.f, 0.f };
static const Vector3 MELEE_ATTACK_2_2_COLLISION_DIST = { 150.f, 0.f, 0.f };
static const Vector3 MELEE_ATTACK_3_COLLISION_DIST = { 0.f, 0.f, 0.f };
static const float MELEE_ATTACK_1_COLLISION_SIZE = 230.f;
static const float MELEE_ATTACK_2_1_COLLISION_SIZE = 230.f;
static const float MELEE_ATTACK_2_2_COLLISION_SIZE = 300.f;
static const float MELEE_ATTACK_3_COLLISION_SIZE = 350.f;