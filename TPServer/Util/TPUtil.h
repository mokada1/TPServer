#pragma once

#include "TSingleton.h"

class TPUtil : public TSingleton<TPUtil>
{
public:
	void CharToWChar(wchar_t* buffer, size_t bufferSize, const char* source);
	void WCharToChar(char* buffer, size_t bufferSize, const wchar_t* source);
	void WCharToMultiByte(char* buffer, size_t bufferSize, const wchar_t* const source);
	void MultiByteToWChar(wchar_t* buffer, size_t bufferSize, const char* const source);
	long long TimeSinceEpochMs();
	long long TimeSinceEpochSec();
};

struct Vector3
{
	float x;
	float y;
	float z;
};