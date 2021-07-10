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

	auto objUser = SQLServiceUser::GetInstance().GetUserObj(hStmt, userId);
	if (objUser)
	{
		if (wcscmp(password, objUser->GetPassword()) == 0)
		{
			result->AddObject(objUser);
			result->SetFlag(true);			
		}
		else
		{
			result->SetMsg(INCORRECT_PASSWORD);
		}
	}
	else
	{
		objUser = SQLServiceUser::GetInstance().InsertUser(hDbc, hStmt, userId, password);
		if (objUser)
		{			
			result->AddObject(objUser);
			result->SetFlag(true);
		}
		else
		{
			result->SetMsg(FAIL_REGISTER_USER);
		}
	}
	
	DBEnd(hStmt);
	return result;
}