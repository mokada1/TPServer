#pragma once

#include "TSingleton.h"
#include "../Packet/Protocol.h"

struct Vector3
{
	float x;
	float y;
	float z;
};

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
	float Distance(const Vector3 v1, const Vector3 v2);
};

#define TPUTIL_MAX(A,B) (A) > (B) ? (A) : (B)
#define TPUTIL_MIN(A,B) (A) < (B) ? (A) : (B)
