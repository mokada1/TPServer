#pragma once

#include "../TP_generated.h"
#include "../Util/TPUtil.h"

class TPComponent
{
public:
	virtual bool IsValid() const
	{
		return false;
	}
	virtual ~TPComponent()
	{
	}
};