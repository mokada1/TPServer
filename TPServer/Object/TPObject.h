#pragma once

#include "../TP_generated.h"
#include "../Util/TPUtil.h"

using namespace std;

class TPObject
{
public:
	virtual wchar_t* GetObjectId() const
	{
		return nullptr;
	}
	virtual bool IsValid() const
	{
		return false;
	}
	virtual ~TPObject()
	{
	}
};