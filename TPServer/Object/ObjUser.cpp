#include "ObjUser.h"
#include "../Util/TPDefine.h"
#include <wchar.h>
#include <iostream>

using namespace std;
						 
wchar_t* ObjUser::GetObjectId() const
{
	return GetUserId();
}

bool ObjUser::IsValid() const
{
	return userId != nullptr && password != nullptr;
}

flatbuffers::Offset<TB_ObjUser> ObjUser::Serialize(flatbuffers::FlatBufferBuilder& _fbb) const
{
	char hUserId[SIZE_USER_USER_ID], hPassword[SIZE_USER_PASSWORD];
	TPUtil::GetInstance().WCharToChar(hUserId, SIZE_USER_USER_ID, userId);
	TPUtil::GetInstance().WCharToChar(hPassword, SIZE_USER_PASSWORD, password);
	auto offsetUserId = _fbb.CreateString(hUserId);
	auto offsetPassword = _fbb.CreateString(hPassword);	
	auto offsetUserLocation = userLocation ? userLocation->Serialize(_fbb) : 0;

	TB_ObjUserBuilder builder(_fbb);	
	builder.add_UserId(offsetUserId);
	builder.add_Password(offsetPassword);
	if (!offsetUserLocation.IsNull())
	{
		builder.add_UserLocation(offsetUserLocation);
	}	
	return builder.Finish();
}

ObjUser::ObjUser()
{
	this->userId = nullptr;
	this->password = nullptr;
	this->userLocation = nullptr;
	totalRttMs = 0;
	rttCount = 0;
	avgRttMs = 0;
}

ObjUser::ObjUser(wchar_t* const _userId, wchar_t* const _password)
{
	this->userId = new wchar_t[SIZE_USER_USER_ID];
	this->password = new wchar_t[SIZE_USER_PASSWORD];
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
	wcscpy_s(this->password, SIZE_USER_PASSWORD, _password);
	this->userLocation = nullptr;
	totalRttMs = 0;
	rttCount = 0;
	avgRttMs = 0;
}

ObjUser::~ObjUser()
{
	if (userId)
	{
		delete[] userId;
	}
	if (password)
	{
		delete[] password;
	}
	if (userLocation)
	{
		userLocation.reset();
	}
}

wchar_t* ObjUser::GetUserId() const
{
	return userId;
}

wchar_t* ObjUser::GetPassword() const
{
	return password;
}

int ObjUser::GetRoomId() const
{
	return roomId;
}

int64_t ObjUser::GetAvgRttMs() const
{
	return avgRttMs;
}

shared_ptr<CompUserLocation> ObjUser::GetCompUserLocation() const
{
	return userLocation;
}

void ObjUser::SetCompUserLocation(shared_ptr<CompUserLocation> _userLocation)
{
	this->userLocation = _userLocation;
}

void ObjUser::SetRoomId(const int _roomId)
{
	this->roomId = _roomId;
}

void ObjUser::UpdateRtt(const int64_t _rttMs)
{
	totalRttMs += _rttMs;
	rttCount++;
	avgRttMs = totalRttMs / rttCount;
}