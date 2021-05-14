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
public:
	Vector3()
	{
		x = 0.f;
		y = 0.f;
		z = 0.f;
	}
	Vector3(const float _x, const float _y, const float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	Vector3(const double _x, const double _y, const double _z)
	{
		x = static_cast<float>(_x);
		y = static_cast<float>(_y);
		z = static_cast<float>(_z);
	}
	void Set(const float _x, const float _y, const float _z)
	{
		x = _x;
		y = _y;
		z = _z;
	}
	void Set(const double _x, const double _y, const double _z)
	{
		x = static_cast<float>(_x);
		y = static_cast<float>(_y);
		z = static_cast<float>(_z);
	}
	void SetX(const float _x) { x = _x; }
	void SetX(const double _x) { x = static_cast<float>(_x); }
	void SetY(const float _y) { y = _y; }
	void SetY(const double _y) { y = static_cast<float>(_y); }
	void SetZ(const float _z) { z = _z; }
	void SetZ(const double _z) { z = static_cast<float>(_z); }
	float GetX() const { return x; }
	float GetY() const { return y; }
	float GetZ() const { return z; }

private:
	float x;
	float y;
	float z;
};