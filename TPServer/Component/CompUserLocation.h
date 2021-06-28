#pragma once

#include "TPComponent.h"

class CompUserLocation : public TPComponent
{
public:
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserLocation> Serialize(flatbuffers::FlatBufferBuilder& _fbb) const;

	CompUserLocation();	
	CompUserLocation(const float _x, const float _y, const float _z);
	CompUserLocation(const double _x, const double _y, const double _z);
	CompUserLocation(const Vector3 _location);
	~CompUserLocation();

	Vector3 GetLocation() const;

	void SetLocation(const float _x, const float _y, const float _z);

private:
	bool isValid;
	Vector3 location;
};