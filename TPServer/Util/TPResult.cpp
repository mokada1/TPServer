#include "TPResult.h"

TPResult::TPResult()
{
	init(nullptr);
}

TPResult::TPResult(const wchar_t* const _message)
{
	init(_message);
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
	if (nextResult)
	{
		delete nextResult;
	}
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

Packet TPResult::GetPacket() const
{
	return packet;
}

TPResult& TPResult::GetNextResult() const
{
	return *nextResult;
}

void TPResult::SetFlag(const bool _flag)
{
	this->flag = _flag;
}

void TPResult::SetMsg(const wchar_t* const _message)
{
	this->message = const_cast<wchar_t*>(_message);
}

void TPResult::SetNumber(const int _number)
{
	this->number = _number;
}

void TPResult::SetPacket(const Packet& _packet)
{
	this->packet = _packet;
}

void TPResult::SetNextResult(TPResult* const _nextResult)
{
	this->nextResult = _nextResult;
}

void TPResult::AddObject(TPObject* const _object)
{
	objectList.emplace_back(shared_ptr<TPObject>(_object));
}

void TPResult::AddComp(TPComponent* const _comp)
{
	compList.emplace_back(shared_ptr<TPComponent>(_comp));
}

void TPResult::init(const wchar_t* const _message)
{
	this->flag = false;
	this->message = _message ? const_cast<wchar_t*>(_message) : nullptr;
	this->number = 0;
	this->nextResult = nullptr;
}
