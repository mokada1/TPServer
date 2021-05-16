#include "TPResult.h"

TPResult::TPResult()
{
	this->flag = false;
	this->message = nullptr;
	this->number = 0;
	this->packet = nullptr;
	this->nextResult = nullptr;
}

TPResult::~TPResult()
{
	if (!objectList.empty())
	{
		objectList.clear();
	}	
	if (!compList.empty())
	{
		compList.clear();
	}
	if (packet)
	{
		delete packet;
	}
	if (nextResult)
	{
		delete nextResult;
	}
}

TPResult::TPResult(const bool _flag)
{
	this->flag = _flag;
	this->message = nullptr;
	this->number = 0;
	this->packet = nullptr;
	this->nextResult = nullptr;
}

TPResult::TPResult(const wchar_t* _message)
{
	this->flag = false;
	this->message = const_cast<wchar_t*>(_message);
	this->number = 0;
	this->packet = nullptr;
	this->nextResult = nullptr;
}

TPResult::TPResult(const int _number)
{
	this->flag = false;
	this->message = nullptr;
	this->number = _number;
	this->packet = nullptr;
	this->nextResult = nullptr;
}

TPResult::TPResult(TPObject* _object)
{
	this->flag = true;
	this->message = nullptr;
	this->number = 0;
	this->objectList.push_back(shared_ptr<TPObject>(_object));
	this->packet = nullptr;
	this->nextResult = nullptr;
}

TPResult::TPResult(TPComponent* _comp)
{
	this->flag = true;
	this->message = nullptr;
	this->number = 0;
	this->compList.push_back(shared_ptr<TPComponent>(_comp));
	this->packet = nullptr;
	this->nextResult = nullptr;
}

bool TPResult::GetFlag() const
{
	return flag;
}

wchar_t* TPResult::GetMsg() const
{
	return message;
}

int TPResult::GetNumber() const
{
	return number;
}

vector<shared_ptr<TPObject>> TPResult::GetObjectList() const
{
	return objectList;
}

vector<shared_ptr<TPComponent>> TPResult::GetCompList() const
{
	return compList;
}

Packet& TPResult::GetPacket() const
{
	return *packet;
}

TPResult& TPResult::GetNextResult() const
{
	return *nextResult;
}

void TPResult::SetFlag(const bool _flag)
{
	this->flag = _flag;
}

void TPResult::SetMsg(const wchar_t* _message)
{
	this->message = const_cast<wchar_t*>(_message);
}

void TPResult::SetNumber(const int _number)
{
	this->number = _number;
}

void TPResult::SetPacket(Packet* _packet)
{
	this->packet = _packet;
}

void TPResult::SetNextResult(TPResult* _nextResult)
{
	this->nextResult = _nextResult;
}

void TPResult::AddObject(TPObject* _object)
{
	objectList.push_back(shared_ptr<TPObject>(_object));
}

void TPResult::AddComp(TPComponent* _comp)
{
	compList.push_back(shared_ptr<TPComponent>(_comp));
}
