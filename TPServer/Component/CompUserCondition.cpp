#include "CompUserCondition.h"

bool CompUserCondition::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserCondition> CompUserCondition::Serialize(flatbuffers::FlatBufferBuilder& _fbb) const
{
	TB_CompUserConditionBuilder builder(_fbb);
	builder.add_IsCombatPosture(isCombatPosture);
	return builder.Finish();
}

CompUserCondition::CompUserCondition()
{
	isValid = false;
	isCombatPosture = false;
}

CompUserCondition::~CompUserCondition()
{
}

CompUserCondition::CompUserCondition(const bool _isCombatPosture)
{
	isValid = true;
	this->isCombatPosture = _isCombatPosture;
}

bool CompUserCondition::GetIsCombatPosture() const
{
	return isCombatPosture;
}

void CompUserCondition::SetIsCombatPosture(const bool _isCombatPosture)
{
	this->isCombatPosture = _isCombatPosture;
}
