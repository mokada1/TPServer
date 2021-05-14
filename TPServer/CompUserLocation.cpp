#include "CompUserLocation.h"

bool CompUserLocation::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserLocation> CompUserLocation::Serialize(flatbuffers::FlatBufferBuilder& _fbb) const
{
	ST_Vec3 st_location(location.GetX(), location.GetY(), location.GetZ());	
	TB_CompUserLocationBuilder builder(_fbb);
	builder.add_Location(&st_location);
	return builder.Finish();
}

CompUserLocation::CompUserLocation()
{
	isValid = false;
}

CompUserLocation::CompUserLocation(const float _x, const float _y, const float _z)
{
	isValid = true;
	location.Set(_x, _y, _z);
}

CompUserLocation::CompUserLocation(const double _x, const double _y, const double _z)
{
	isValid = true;
	location.Set(_x, _y, _z);
}

CompUserLocation::CompUserLocation(const Vector3 _location)
{
	isValid = true;
	location = _location;
}

CompUserLocation::~CompUserLocation()
{
}

Vector3 CompUserLocation::GetLocation() const
{
	return location;
}