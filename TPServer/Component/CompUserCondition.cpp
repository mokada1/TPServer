#include "CompUserCondition.h"

bool CompUserCondition::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserCondition> CompUserCondition::Serialize(flatbuffers::FlatBufferBuilder& _fbb) const
{
	TB_CompUserConditionBuilder builder(_fbb);
	builder.add_IsCombatPosture(isCombatPosture);
	builder.add_IsDied(isDied);
	return builder.Finish();
}

CompUserCondition::CompUserCondition()
{
	isValid = false;
	isCombatPosture = false;
	isDied = false;
}

CompUserCondition::~CompUserCondition()
{
}

CompUserCondition::CompUserCondition(const bool _isCombatPosture)
{
	this->isValid = true;
	this->isCombatPosture = _isCombatPosture;
	this->isDied = false;
}

bool CompUserCondition::GetIsCombatPosture() const
{
	return isCombatPosture;
}

bool CompUserCondition::GetIsDied() const
{
	return isDied;
}

void CompUserCondition::SetIsCombatPosture(const bool _isCombatPosture)
{
	this->isCombatPosture = _isCombatPosture;
}

void CompUserCondition::SetIsDied(const bool _isDied)
{
	this->isDied = _isDied;
}
