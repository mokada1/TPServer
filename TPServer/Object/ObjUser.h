#pragma once

#include "TPObject.h"
#include "../Component/CompUserTransform.h"

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
	char* GetCUserId() const;
	wchar_t* GetPassword() const;
	int GetRoomId() const;
	int64_t GetAvgRttMs() const;
	shared_ptr<CompUserTransform> GetCompUserTransform() const;

	void SetCompUserTransform(shared_ptr<CompUserTransform> _userTransform);
	void SetRoomId(const int _roomId);

	void UpdateRtt(const int64_t _rttMs);

private:
	wchar_t* userId;
	char* cUserId;
	wchar_t* password;
	
	shared_ptr<CompUserTransform> userTransform;

	int roomId;

	int64_t totalRttMs;
	int64_t rttCount;
	int64_t avgRttMs;
};