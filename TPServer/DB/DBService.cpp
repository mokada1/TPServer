#include "DBService.h"
#include "DBServer.h"
#include "../SQLService/SQLServiceUser.h"
#include "../Util/TPDefine.h"
#include "../Util/TPResult.h"

bool DBService::DBConnect()
{
	if (!DBServer::GetInstance().GetIsConnected())
	{
		if (!DBServer::GetInstance().DBConnect())
		{
			return false;
		}
	}	
	return true;
}

SQLHSTMT DBService::DBStart()
{
	if (!DBConnect())
	{
		return nullptr;
	}

	hDbc = DBServer::GetInstance().GetHDBC();

	SQLHSTMT hStmt;
	if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}
	return hStmt;
}

void DBService::DBEnd(const SQLHSTMT& hStmt)
{
	if (hStmt)
	{
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	DBServer::GetInstance().DBDisconnect();
}

TPResult* DBService::LoginUser(const wchar_t* const userId, const wchar_t* const password)
{
	auto result = new TPResult();

	SQLHSTMT hStmt = DBStart();
	if (hStmt == nullptr)
	{
		result->SetMsg(FAIL_DBCONNECT_OR_ALLOC_HANDLE);
		return result;
	}

	auto objUser = SQLServiceUser::GetInstance().GetUser(hStmt, userId);
	if (objUser)
	{
		if (wcscmp(password, objUser->GetPassword()) == 0)
		{
			//wcout << CORRECT_PASSWORD << endl;
			result->AddObject(objUser);
			result->SetFlag(true);			
		}
		else
		{
			//wcout << INCORRECT_PASSWORD << endl;
			result->SetMsg(INCORRECT_PASSWORD);
		}
	}
	else
	{
		objUser = SQLServiceUser::GetInstance().InsertUser(hDbc, hStmt, userId, password);
		if (objUser)
		{			
			//wcout << SUCCESS_REGISTER_USER << endl;
			result->AddObject(objUser);
			result->SetFlag(true);
		}
		else
		{
			//wcout << FAIL_REGISTER_USER << endl;
			result->SetMsg(FAIL_REGISTER_USER);
		}
	}
	
	DBEnd(hStmt);
	return result;
}

TPResult* DBService::LoadUserInfo(const wchar_t* const userId)
{
	auto result = new TPResult();

	SQLHSTMT hStmt = DBStart();
	if (hStmt == nullptr)
	{
		result->SetMsg(FAIL_DBCONNECT_OR_ALLOC_HANDLE);
		return result;
	}

	auto compUserTransform = SQLServiceUser::GetInstance().GetUserTransform(hStmt, userId);
	if (compUserTransform)
	{
		//wcout << SUCCESS_GET_DATA << endl;
		result->AddComp(compUserTransform);
		result->SetFlag(true);
	}
	else
	{
		static const Vector3 DEFAULT_USER_LOCATION = { -1340.f, 40.f, 230.f };
		static const Vector3 DEFAULT_USER_ROTATION = { 0.f, 0.f, 0.f };
		compUserTransform = SQLServiceUser::GetInstance().InsertUserTransform(hDbc, hStmt, userId, DEFAULT_USER_LOCATION, DEFAULT_USER_ROTATION);
		if (compUserTransform)
		{
			//wcout << SUCCESS_INSERT_DATA << endl;
			result->AddComp(compUserTransform);
			result->SetFlag(true);
		}
		else
		{
			//wcout << FAIL_INSERT_DATA << endl;
			result->SetMsg(FAIL_INSERT_DATA);
		}	
	}

	DBEnd(hStmt);
	return result;
}
