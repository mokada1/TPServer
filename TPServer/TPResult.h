#pragma once

#include "TPObject.h"
#include "TPComponent.h"
#include <vector>
#include "Packet.h"

using namespace std;

class TPResult
{
public:
	TPResult();
	~TPResult();
	TPResult(const wchar_t* const _message);
		
	bool GetFlag() const;
	wchar_t* GetMsg() const;
	int GetNumber() const;
	vector<shared_ptr<TPObject>> GetObjectList() const;
	vector<shared_ptr<TPComponent>> GetCompList() const;
	Packet* GetPacket() const;
	TPResult& GetNextResult() const;

	void SetFlag(const bool _flag);
	void SetMsg(const wchar_t* const _message);
	void SetNumber(const int _number);
	void SetPacket(Packet* const _packet);
	void SetNextResult(TPResult* const _nextResult);
	void AddObject(TPObject* const _object);
	void AddComp(TPComponent* const _comp);

private:
	bool flag;
	wchar_t* message;
	int number;
	vector<shared_ptr<TPObject>> objectList;
	vector<shared_ptr<TPComponent>> compList;
	Packet* packet;
	TPResult* nextResult;
};