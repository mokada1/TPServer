#include "CompUserAttribute.h"

bool CompUserAttribute::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserAttribute> CompUserAttribute::Serialize(flatbuffers::FlatBufferBuilder& _fbb) const
{
	TB_CompUserAttributeBuilder builder(_fbb);
	builder.add_Hp(hp);
	builder.add_Str(str);
	return builder.Finish();
}

CompUserAttribute::CompUserAttribute()
{
	hp = 0.f;
	str = 0.f;
}

CompUserAttribute::~CompUserAttribute()
{
}

CompUserAttribute::CompUserAttribute(const float _hp, const float _str)
{
	this->isValid = true;
	this->hp = _hp;
	this->str = _str;
}

float CompUserAttribute::GetHp() const
{
	return hp;
}

float CompUserAttribute::GetStr() const
{
	return str;
}

void CompUserAttribute::SetHp(const float _hp)
{
	this->hp = _hp;
}

void CompUserAttribute::SetStr(const float _str)
{
	this->str = _str;
}
