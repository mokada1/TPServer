#include "DBService.h"
#include "DBServer.h"
#include <iostream>
#include <string>
#include "SQLServiceUser.h"
#include "TPDefine.h"

using namespace std;

bool DBService::CheckDBC()
{
	if (!DBServer::GetInstance().GetIsConnected())
	{
		if (!DBServer::GetInstance().DBConnect())
		{
			return false;
		}
	}	
	if (hDbc == nullptr)
	{
		hDbc = DBServer::GetInstance().GetHDBC();
	}
	return true;
}

SQLHSTMT DBService::StartStmt()
{
	if (!CheckDBC())
	{
		return nullptr;
	}

	SQLHSTMT hStmt;
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}
	return hStmt;
}

void DBService::ENDStmt(const SQLHSTMT& hStmt)
{
	if (hStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
}

TPResult* DBService::LoginUser(const wchar_t* userId, const wchar_t* password)
{
	SQLHSTMT hStmt = StartStmt();
	if (hStmt == nullptr)
	{
		return new TPResult(FAIL_DBCONNECT_OR_ALLOC_HANDLE);
	}

	auto result = new TPResult();
	auto objUser = SQLServiceUser::GetInstance().GetUser(hStmt, userId);
	if (objUser)
	{
		if (wcscmp(password, objUser->GetPassword()) == 0)
		{
			wcout << CORRECT_PASSWORD << endl;
			result->AddObject(objUser);
			result->SetFlag(true);			
		}
		else
		{
			wcout << INCORRECT_PASSWORD << endl;
			result->SetMsg(INCORRECT_PASSWORD);
		}
	}
	else
	{
		objUser = SQLServiceUser::GetInstance().InsertUser(hDbc, hStmt, userId, password);
		if (objUser)
		{			
			wcout << SUCCESS_REGISTER_USER << endl;
			result->AddObject(objUser);
			result->SetFlag(true);
		}
		else
		{
			wcout << FAIL_REGISTER_USER << endl;
			result->SetMsg(FAIL_REGISTER_USER);
		}
	}
	
	ENDStmt(hStmt);
	return result;
}

TPResult* DBService::LoadUserInfo(const wchar_t* userId)
{
	SQLHSTMT hStmt = StartStmt();
	if (hStmt == nullptr)
	{
		return new TPResult(FAIL_DBCONNECT_OR_ALLOC_HANDLE);
	}

	auto result = new TPResult();
	auto compUserLocation = SQLServiceUser::GetInstance().GetUserLocation(hStmt, userId);
	if (compUserLocation)
	{
		wcout << SUCCESS_GET_DATA << endl;
		result->AddComp(compUserLocation);
		result->SetFlag(true);
	}
	else
	{
		compUserLocation = SQLServiceUser::GetInstance().InsertUserLocation(hDbc, hStmt, userId, 0.f, 0.f, 0.f);
		if (compUserLocation)
		{
			wcout << SUCCESS_INSERT_DATA << endl;
			result->AddComp(compUserLocation);
			result->SetFlag(true);
		}
		else
		{
			wcout << FAIL_INSERT_DATA << endl;
			result->SetMsg(FAIL_INSERT_DATA);
		}	
	}

	ENDStmt(hStmt);
	return result;
}
