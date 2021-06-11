#pragma once

#include "TSingleton.h"

class TPError : public TSingleton<TPError>
{
public:
	void PrintError(const wchar_t* const message);	
	void PrintError(const wchar_t* const message, const int code);	

private:
	void PrintError(const wchar_t* const message, bool isDAllocBuf);
	const wchar_t* ConcatMessage(const wchar_t* const message, const int code);
};