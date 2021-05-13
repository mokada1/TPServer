#pragma once

#include "TPObject.h"
#include "TPComponent.h"
#include <vector>

using namespace std;

class TPResult
{
public:
	TPResult();
	~TPResult();
	TPResult(const bool _flag);
	TPResult(const wchar_t* _message);
	TPResult(const int _number);
	TPResult(TPObject* _object);
	TPResult(TPComponent* _comp);
		
	bool GetFlag() const;
	wchar_t* GetMsg() const;
	int GetNumber() const;
	vector<TPObject*> GetObjectList() const;
	vector<TPComponent*> GetCompList() const;

	void SetFlag(const bool _flag);
	void SetMsg(const wchar_t* _message);
	void SetNumber(const int _number);
	void AddObject(TPObject* _object);
	void AddComp(TPComponent* _comp);

private:
	bool flag;
	wchar_t* message;
	int number;
	vector<TPObject*> objectList;
	vector<TPComponent*> compList;
};