#pragma once

#include "TPObject.h"
#include "CompUserLocation.h"

class ObjUser : public TPObject
{
public:
	virtual wchar_t* GetObjectId() const override;
	virtual bool IsValid() const override;
	flatbuffers::Offset<TB_ObjUser> Serialize(flatbuffers::FlatBufferBuilder& _fbb) const;

	ObjUser();
	ObjUser(wchar_t* const _userId, wchar_t* const _password);
	~ObjUser();
	wchar_t* GetUserId() const;
	wchar_t* GetPassword() const;

	shared_ptr<CompUserLocation> GetCompUserLocation() const;
	void SetCompUserLocation(shared_ptr<CompUserLocation> _userLocation);

private:
	wchar_t* userId;
	wchar_t* password;
	shared_ptr<CompUserLocation> userLocation;
};