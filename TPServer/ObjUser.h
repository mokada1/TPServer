#pragma once

#include "TPObject.h"
#include "CompUserLocation.h"

class ObjUser : public TPObject
{
public:
	virtual wchar_t* GetObjectId() const override;
	virtual bool IsValid() const override;

	ObjUser();
	ObjUser(wchar_t* _userId, wchar_t* _password);
	~ObjUser();
	wchar_t* GetUserId() const;
	wchar_t* GetPassword() const;

	CompUserLocation& GetCompUserLocation() const;
	void SetCompUserLocation(CompUserLocation* _userLocation);

private:
	wchar_t* userId;
	wchar_t* password;
	CompUserLocation* userLocation;
};