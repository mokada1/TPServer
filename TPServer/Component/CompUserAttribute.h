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
	float GetStr() const;

	bool SetHp(float& oldData, const float newData);
	void SetStr(const float _str);

private:
	bool isValid;
	std::atomic<float> hp;
	float str;
};