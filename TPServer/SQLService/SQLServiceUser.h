#pragma once

#include "../Util/TSingleton.h"
#include "../Object/ObjUser.h"
#include <windows.h>
#include <sql.h>
#include <sqlext.h>

class SQLServiceUser : public TSingleton<SQLServiceUser>
{
public:
	ObjUser* GetUserObj(const SQLHSTMT& hStmt, const wchar_t* const userId);
	ObjUser* GetUserObjOnly(const SQLHSTMT& hStmt, const wchar_t* const userId);	
	CompUserTransform* GetUserTransform(const SQLHSTMT& hStmt, const wchar_t* const userId);

	ObjUser* InsertUser(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const wchar_t* const password);

private:
	ObjUser* InsertUserOnly(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const wchar_t* const password);
	CompUserTransform* InsertUserTransform(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const Vector3 location, const Vector3 rotation);
};

static const Vector3 DEFAULT_USER_LOCATION = { -1340.f, 40.f, 230.f };
static const Vector3 DEFAULT_USER_ROTATION = { 0.f, 0.f, 0.f };