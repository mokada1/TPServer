#pragma once

#include "TPComponent.h"
#include <atomic>

class CompUserAttribute : public TPComponent
{
public:
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserAttribute> Serialize(flatbuffers::FlatBufferBuilder& _fbb) const;

	CompUserAttribute();
	~CompUserAttribute();
	CompUserAttribute(const float _hp, const float _str);

	float GetHp() const;
	float GetMaxHp() const;
	float GetStr() const;

	void SetMaxHp(const float _maxHp);
	void SetStr(const float _str);

	void ApplyDamage(const float damage);

private:
	bool isValid;
	std::atomic<float> hp;
	float maxHp;
	float str;
};