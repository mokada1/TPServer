#pragma once

#include "../Object/TPObject.h"
#include "../Component/TPComponent.h"
#include "../Packet/Packet.h"

#include <vector>

using namespace std;

class TPResult
{
public:
	TPResult();
	TPResult(const wchar_t* const _message);
	~TPResult();	
		
	bool GetFlag() const;
	wchar_t* GetMsg() const;
	int GetNumber() const;
	vector<shared_ptr<TPObject>> GetObjectList() const;
	vector<shared_ptr<TPComponent>> GetCompList() const;
	Packet GetPacket() const;

	void SetFlag(const bool _flag);
	void SetMsg(const wchar_t* const _message);
	void SetNumber(const int _number);
	void SetPacket(const Packet& _packet);
	void AddObject(TPObject* const _object);
	void AddComp(TPComponent* const _comp);

private:
	void init(const wchar_t* const _message);

	bool flag;
	wchar_t* message;
	int number;
	vector<shared_ptr<TPObject>> objectList;
	vector<shared_ptr<TPComponent>> compList;
	Packet packet;
};