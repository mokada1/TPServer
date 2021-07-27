#pragma once

#include "Unconstructible.h"
#include "../Packet/Protocol.h"

struct Vector3
{
	float x;
	float y;
	float z;
};

class TPUtil : public Unconstructible
{
public:
	static TPUtil& GetInstance()
	{
		static TPUtil* _instance = new TPUtil();
		return *_instance;
	}

	void CharToWChar(wchar_t* buffer, size_t bufferSize, const char* source);
	void WCharToChar(char* buffer, size_t bufferSize, const wchar_t* source);
	void WCharToMultiByte(char* buffer, size_t bufferSize, const wchar_t* const source);
	void MultiByteToWChar(wchar_t* buffer, size_t bufferSize, const char* const source);
	long long TimeSinceEpochMs();
	long long TimeSinceEpochMs(const char* const timeZone);
	const char* EnumToString(PROTOCOL protocol);
	float Distance(const Vector3 v1, const Vector3 v2);
	Vector3 RotateByZAxis(const Vector3 location, const float degree);
	float GetRadian(const float degree);

private:
	TPUtil() {}
};

static const double TPUTIL_PI = 3.14159265359;
static const double TPUTIL_PI_RADIAN = 180;
static const char* TIME_ZONE_ASIA_SEOUL = "Asia/Seoul";

#define TPUTIL_MAX(A,B) (A) > (B) ? (A) : (B)
#define TPUTIL_MIN(A,B) (A) < (B) ? (A) : (B)
