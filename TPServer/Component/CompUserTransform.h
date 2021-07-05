#pragma once

#include "TPComponent.h"

class CompUserTransform : public TPComponent
{
public:
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_CompUserTransform> Serialize(flatbuffers::FlatBufferBuilder& _fbb) const;
	
	CompUserTransform();
	~CompUserTransform();
	CompUserTransform(const Vector3 _location, const Vector3 _rotation);

	Vector3 GetLocation() const;
	Vector3 GetRotation() const;

	void SetLocation(const Vector3 _location);
	void SetRotation(const Vector3 _rotation);

private:
	bool isValid;
	Vector3 location;
	Vector3 rotation;
};