#pragma once

#include "TSingleton.h"
#include "../Packet/Protocol.h"

class TPUtil : public TSingleton<TPUtil>
{
public:
	void CharToWChar(wchar_t* buffer, size_t bufferSize, const char* source);
	void WCharToChar(char* buffer, size_t bufferSize, const wchar_t* source);
	void WCharToMultiByte(char* buffer, size_t bufferSize, const wchar_t* const source);
	void MultiByteToWChar(wchar_t* buffer, size_t bufferSize, const char* const source);
	long long TimeSinceEpochMs();
	long long TimeSinceEpochSec();
	const char* EnumToString(PROTOCOL protocol);
};

struct Vector3
{
	float x;
	float y;
	float z;
};

#define TPUTIL_MAX(A,B) (A) > (B) ? (A) : (B)
#define TPUTIL_MIN(A,B) (A) < (B) ? (A) : (B)