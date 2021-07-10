#pragma once

#include "TPComponent.h"

class CompUserCondition : public TPComponent
{
public:
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserCondition> Serialize(flatbuffers::FlatBufferBuilder& _fbb) const;

	CompUserCondition();
	~CompUserCondition();
	CompUserCondition(const bool _isCombatPosture);

	bool GetIsCombatPosture() const;
	void SetIsCombatPosture(const bool _isCombatPosture);

private:
	bool isValid;
	bool isCombatPosture;
};