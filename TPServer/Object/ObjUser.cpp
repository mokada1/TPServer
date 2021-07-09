#include "ObjUser.h"
#include "../Util/TPDefine.h"
						 
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
	char hUserId[SIZE_USER_USER_ID];
	TPUtil::GetInstance().WCharToChar(hUserId, SIZE_USER_USER_ID, userId);
	auto offsetUserId = _fbb.CreateString(hUserId);
	auto offsetUserTransform = userTransform ? userTransform->Serialize(_fbb) : 0;

	TB_ObjUserBuilder builder(_fbb);	
	builder.add_UserId(offsetUserId);
	if (!offsetUserTransform.IsNull())
	{
		builder.add_UserTransform(offsetUserTransform);
	}	
	return builder.Finish();
}

ObjUser::ObjUser()
{
	this->userId = nullptr;
	this->cUserId = nullptr;
	this->password = nullptr;
	this->userTransform = nullptr;
	totalRttMs = 0;
	rttCount = 0;
	avgRttMs = 0;
}

ObjUser::ObjUser(wchar_t* const _userId, wchar_t* const _password)
{
	this->userId = new wchar_t[SIZE_USER_USER_ID];	
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
	
	this->cUserId = new char[SIZE_USER_USER_ID];
	TPUtil::GetInstance().WCharToChar(this->cUserId, SIZE_USER_USER_ID, this->userId);

	this->password = new wchar_t[SIZE_USER_PASSWORD];
	wcscpy_s(this->password, SIZE_USER_PASSWORD, _password);

	this->userTransform = nullptr;
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
	if (cUserId)
	{
		delete[] cUserId;
	}
	if (password)
	{
		delete[] password;
	}
	if (userTransform)
	{
		userTransform.reset();
	}
}

wchar_t* ObjUser::GetUserId() const
{
	return userId;
}

char* ObjUser::GetCUserId() const
{
	return cUserId;
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

shared_ptr<CompUserTransform> ObjUser::GetCompUserTransform() const
{
	return userTransform;
}

void ObjUser::SetCompUserTransform(shared_ptr<CompUserTransform> _userTransform)
{
	this->userTransform = _userTransform;
}

void ObjUser::SetRoomId(const int _roomId)
{
	this->roomId = _roomId;
}

void ObjUser::UpdateRtt(const int64_t _rttMs)
{
	if (rttCount >= MAX_RTT_COUNT)
	{
		totalRttMs = 0;
		rttCount = 0;
	}
	totalRttMs += _rttMs;
	rttCount++;
	avgRttMs = totalRttMs / rttCount;
}