#pragma once

#include "TPObject.h"
#include "../Component/CompUserLocation.h"

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
	int GetRoomId() const;
	int64_t GetAvgRttMs() const;
	shared_ptr<CompUserLocation> GetCompUserLocation() const;

	void SetCompUserLocation(shared_ptr<CompUserLocation> _userLocation);
	void SetRoomId(const int _roomId);

	void AddRttMs(const int64_t _rttMs);

private:
	wchar_t* userId;
	wchar_t* password;
	
	shared_ptr<CompUserLocation> userLocation;

	int roomId;

	int64_t totalRttMs;
	int64_t rttCount;
	int64_t avgRttMs;
};