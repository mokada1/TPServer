#pragma once

#include "TPComponent.h"

class CompUserLocation : public TPComponent
{
public:
	virtual bool IsValid() const override;

	CompUserLocation();
	CompUserLocation(const float _x, const float _y, const float _z);
	CompUserLocation(const double _x, const double _y, const double _z);

	float GetX() const;
	float GetY() const;
	float GetZ() const;

private:
	bool isValid;

	float x;
	float y;
	float z;
};