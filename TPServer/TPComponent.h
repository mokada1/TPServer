#pragma once

#include "TP_generated.h"
#include "TPUtil.h"

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