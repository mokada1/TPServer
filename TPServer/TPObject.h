#pragma once

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
};