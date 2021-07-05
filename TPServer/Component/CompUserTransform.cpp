#include "CompUserTransform.h"

bool CompUserTransform::IsValid() const
{
	return isValid;
}

flatbuffers::Offset<TB_CompUserTransform> CompUserTransform::Serialize(flatbuffers::FlatBufferBuilder& _fbb) const
{
	ST_Vec3 stLocation(location.x, location.y, location.z);	
	ST_Vec3 stRotation(rotation.x, rotation.y, rotation.z);

	TB_CompUserTransformBuilder builder(_fbb);
	builder.add_Location(&stLocation);
	builder.add_Rotation(&stRotation);
	return builder.Finish();
}

CompUserTransform::CompUserTransform()
{
	isValid = false;
	location = { 0.f, 0.f, 0.f };
	rotation = { 0.f, 0.f, 0.f };
}

CompUserTransform::~CompUserTransform()
{
}

CompUserTransform::CompUserTransform(const Vector3 _location, const Vector3 _rotation)
{
	isValid = true;
	location = _location;
	rotation = _rotation;
}

Vector3 CompUserTransform::GetLocation() const
{
	return location;
}

Vector3 CompUserTransform::GetRotation() const
{
	return rotation;
}

void CompUserTransform::SetLocation(const Vector3 _location)
{
	location = _location;
}

void CompUserTransform::SetRotation(const Vector3 _rotation)
{
	rotation = _rotation;
}
