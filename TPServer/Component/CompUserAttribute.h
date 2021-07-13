#pragma once

#include "TPComponent.h"

class CompUserAttribute : public TPComponent
{
public:
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserAttribute> Serialize(flatbuffers::FlatBufferBuilder& _fbb) const;

	CompUserAttribute();
	~CompUserAttribute();
	CompUserAttribute(const float _hp, const float _str);

	float GetHp() const;
	float GetStr() const;

	void SetHp(const float _hp);
	void SetStr(const float _str);

private:
	bool isValid;
	float hp;
	float str;
};