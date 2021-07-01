#pragma once

#include "../Util/TSingleton.h"
#include <windows.h>

#include <sql.h>
#include <sqlext.h>

class TPResult;

class DBService : public TSingleton<DBService>
{
public:
	TPResult* LoginUser(const wchar_t* const userId, const wchar_t* const password);
	TPResult* LoadUserInfo(const wchar_t* const userId);

private:
	SQLHDBC hDbc;

	bool DBConnect();
	SQLHSTMT DBStart();
	void DBEnd(const SQLHSTMT& hStmt);	
};