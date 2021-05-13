#include "CompUserLocation.h"

bool CompUserLocation::IsValid() const
{
	return this->isValid;
}

CompUserLocation::CompUserLocation()
{
	this->isValid = false;
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

CompUserLocation::CompUserLocation(const float _x, const float _y, const float _z)
{
	this->isValid = true;
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

CompUserLocation::CompUserLocation(const double _x, const double _y, const double _z)
{
	this->isValid = true;
	this->x = static_cast<float>(_x);
	this->y = static_cast<float>(_y);
	this->z = static_cast<float>(_z);
}

float CompUserLocation::GetX() const
{
	return x;
}

float CompUserLocation::GetY() const
{
	return y;
}

float CompUserLocation::GetZ() const
{
	return z;
}
