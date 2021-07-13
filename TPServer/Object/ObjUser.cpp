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
	auto offsetUserId = _fbb.CreateString(cUserId);
	auto offsetCompTransform = compTransform ? compTransform->Serialize(_fbb) : 0;
	auto offsetCompCondition = compCondition ? compCondition->Serialize(_fbb) : 0;
	auto offsetCompAttribute = compAttribute ? compAttribute->Serialize(_fbb) : 0;

	TB_ObjUserBuilder builder(_fbb);	
	builder.add_UserId(offsetUserId);
	builder.add_Transform(offsetCompTransform);
	builder.add_Condition(offsetCompCondition);
	builder.add_Attribute(offsetCompAttribute);
	return builder.Finish();
}

ObjUser::ObjUser()
{
	this->compTransform = nullptr;
	this->compCondition = nullptr;
	this->compAttribute = nullptr;
	this->userId = nullptr;
	this->cUserId = nullptr;
	this->password = nullptr;
	totalRttMs = 0;
	rttCount = 0;
	avgRttMs = 0;
	roomId = 0;
}

ObjUser::ObjUser(wchar_t* const _userId, wchar_t* const _password)
{
	this->compTransform = nullptr;
	this->compCondition = nullptr;
	this->compAttribute = nullptr;

	this->userId = new wchar_t[SIZE_USER_USER_ID];	
	wcscpy_s(this->userId, SIZE_USER_USER_ID, _userId);
	
	this->cUserId = new char[SIZE_USER_USER_ID];
	TPUtil::GetInstance().WCharToChar(this->cUserId, SIZE_USER_USER_ID, this->userId);

	this->password = new wchar_t[SIZE_USER_PASSWORD];
	wcscpy_s(this->password, SIZE_USER_PASSWORD, _password);
	
	totalRttMs = 0;
	rttCount = 0;
	avgRttMs = 0;
	roomId = 0;
}

ObjUser::~ObjUser()
{
	if (userId)	delete[] userId;
	if (cUserId) delete[] cUserId;	
	if (password) delete[] password;
	if (compTransform) delete compTransform;
	if (compCondition) delete compCondition;
	if (compAttribute) delete compAttribute;
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

CompUserTransform* ObjUser::GetCompTransform() const
{
	return compTransform;
}

CompUserCondition* ObjUser::GetCompCondition() const
{
	return compCondition;
}

CompUserAttribute* ObjUser::GetCompAttribute() const
{
	return compAttribute;
}

void ObjUser::SetCompTransform(CompUserTransform * const _compTransform)
{
	this->compTransform = _compTransform;
}

void ObjUser::SetCompCondition(CompUserCondition* const _compCondition)
{
	this->compCondition = _compCondition;
}

void ObjUser::SetCompAttribute(CompUserAttribute* const _compAttribute)
{
	this->compAttribute = _compAttribute;
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