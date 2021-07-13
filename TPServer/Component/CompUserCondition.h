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
	bool GetIsDied() const;
	void SetIsCombatPosture(const bool _isCombatPosture);
	void SetIsDied(const bool _isDied);

private:
	bool isValid;
	bool isCombatPosture;
	bool isDied;
};