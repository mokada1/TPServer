#include "TPResult.h"

TPResult::TPResult()
{
	this->flag = false;
	this->message = nullptr;
	this->number = 0;
}

TPResult::~TPResult()
{
	if (objectList.size() > 0)
	{
		for (auto obj : objectList)
		{
			delete obj;
		}
		objectList.clear();
	}	

	if (compList.size() > 0)
	{
		for (auto comp : compList)
		{
			delete comp;
		}
		compList.clear();
	}	
}

TPResult::TPResult(const bool _flag)
{
	this->flag = _flag;
	this->message = nullptr;
	this->number = 0;
}

TPResult::TPResult(const wchar_t* _message)
{
	this->flag = false;
	this->message = const_cast<wchar_t*>(_message);
	this->number = 0;
}

TPResult::TPResult(const int _number)
{
	this->flag = false;
	this->message = nullptr;
	this->number = _number;
}

TPResult::TPResult(TPObject* _object)
{
	this->flag = true;
	this->message = nullptr;
	this->number = 0;
	this->objectList.push_back(_object);
}

TPResult::TPResult(TPComponent* _comp)
{
	this->flag = true;
	this->message = nullptr;
	this->number = 0;
	this->compList.push_back(_comp);
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

vector<TPObject*> TPResult::GetObjectList() const
{
	return objectList;
}

vector<TPComponent*> TPResult::GetCompList() const
{
	return compList;
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

void TPResult::AddObject(TPObject* _object)
{
	objectList.push_back(_object);
}

void TPResult::AddComp(TPComponent* _comp)
{
	compList.push_back(_comp);
}
