#pragma once

#include "TSingleton.h"
#include "ObjUser.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

class SQLServiceUser : public TSingleton<SQLServiceUser>
{
public:
	ObjUser* GetUser(const SQLHSTMT& hStmt, const wchar_t* userId);
	ObjUser* InsertUser(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* userId, const wchar_t* password);
	CompUserLocation* GetUserLocation(const SQLHSTMT& hStmt, const wchar_t* userId);
	CompUserLocation* InsertUserLocation(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* userId, const float x, const float y, const float z);
};