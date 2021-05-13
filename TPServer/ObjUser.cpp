#include "ObjUser.h"
#include "TPDefine.h"
#include <wchar.h>

wchar_t* ObjUser::GetObjectId() const
{
	return GetUserId();
}

bool ObjUser::IsValid() const
{
	return userId != nullptr && password != nullptr;
}

ObjUser::ObjUser()
{
	this->userId = nullptr;
	this->password = nullptr;
	this->userLocation = nullptr;
}

ObjUser::ObjUser(wchar_t* _userId, wchar_t* _password)
{
	this->userId = new wchar_t[SIZE_USER_USER_ID];
	this->password = new wchar_t[SIZE_USER_PASSWORD];
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
	wcscpy_s(this->password, SIZE_USER_PASSWORD, _password);
	this->userLocation = nullptr;
}

ObjUser::~ObjUser()
{
	if (userId)
		delete[] userId;
	if (password)
		delete[] password;
	if (userLocation)
		delete userLocation;
}

wchar_t* ObjUser::GetUserId() const
{
	return this->userId;
}

wchar_t* ObjUser::GetPassword() const
{
	return this->password;
}

CompUserLocation& ObjUser::GetCompUserLocation() const
{
	return *this->userLocation;
}

void ObjUser::SetCompUserLocation(CompUserLocation* _userLocation)
{
	this->userLocation = _userLocation;
}
