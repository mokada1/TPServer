#pragma once

#include "../Util/Unconstructible.h"
#include "../Object/ObjUser.h"

#include <vector>

using namespace std;

class BattleService : public Unconstructible
{
public:
	static BattleService& GetInstance()
	{
		static BattleService* _instance = new BattleService();
		return *_instance;
	}

	void PlayAbility(const shared_ptr<ObjUser>& caster, AbilityType abilityType);

private:
	BattleService() {}

	void PlayMeleeAttack1(const shared_ptr<ObjUser>& caster);
	void PlayMeleeAttack2_1(const shared_ptr<ObjUser>& caster);
	void PlayMeleeAttack2_2(const shared_ptr<ObjUser>& caster);
	void PlayMeleeAttack3(const shared_ptr<ObjUser>& caster);
	void MeleeAttack(const shared_ptr<ObjUser>& caster, const Vector3 collisionDistance, const float collisionSize, const float damage);
	void ApplyDamage(const shared_ptr<ObjUser>& target, const float damage);
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
static const float MELEE_ATTACK_1_DAMAGE = 1.f;
static const float MELEE_ATTACK_2_1_DAMAGE = 1.f;
static const float MELEE_ATTACK_2_2_DAMAGE = 1.f;
static const float MELEE_ATTACK_3_DAMAGE = 1.f;