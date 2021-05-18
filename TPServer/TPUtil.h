#pragma once

#include "TSingleton.h"

class TPUtil : public TSingleton<TPUtil>
{
public:
	void CharToWChar(wchar_t* buffer, size_t bufferSize, const char* source);
	void WCharToChar(char* buffer, size_t bufferSize, const wchar_t* source);
};

struct Vector3
{
	float x;
	float y;
	float z;
};