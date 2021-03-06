#include "CompUserAttribute.h"

bool CompUserAttribute::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserAttribute> CompUserAttribute::Serialize(flatbuffers::FlatBufferBuilder& _fbb) const
{
	TB_CompUserAttributeBuilder builder(_fbb);
	builder.add_Hp(hp);
	builder.add_MaxHp(maxHp);
	builder.add_Str(str);
	return builder.Finish();
}

CompUserAttribute::CompUserAttribute()
{
	isValid = false;
	hp = 0.f;
	maxHp = 0.f;
	str = 0.f;
}

CompUserAttribute::~CompUserAttribute()
{
}

CompUserAttribute::CompUserAttribute(const float _hp, const float _str)
{
	this->isValid = true;
	this->hp = _hp;
	this->maxHp = _hp;
	this->str = _str;
}

float CompUserAttribute::GetHp() const
{
	return hp.load();
}

float CompUserAttribute::GetMaxHp() const
{
	return maxHp;
}

float CompUserAttribute::GetStr() const
{
	return str;
}

void CompUserAttribute::SetMaxHp(const float _maxHp)
{
	this->maxHp = _maxHp;
}

void CompUserAttribute::SetStr(const float _str)
{
	this->str = _str;
}

void CompUserAttribute::ApplyDamage(const float damage)
{
	float oldHp = 0.f;
	float hpAfterDamage = 0.f;
	do
	{
		oldHp = GetHp();
		hpAfterDamage = oldHp - damage < 0 ? 0 : oldHp - damage;
	} while (!hp.compare_exchange_weak(oldHp, hpAfterDamage));
}
