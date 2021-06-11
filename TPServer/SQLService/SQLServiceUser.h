#pragma once

#include "../Util/TSingleton.h"
#include "../Object/ObjUser.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

class SQLServiceUser : public TSingleton<SQLServiceUser>
{
public:
	ObjUser* GetUser(const SQLHSTMT& hStmt, const wchar_t* const userId);
	ObjUser* InsertUser(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const wchar_t* const password);
	CompUserLocation* GetUserLocation(const SQLHSTMT& hStmt, const wchar_t* const userId);
	CompUserLocation* InsertUserLocation(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const Vector3 location);
};