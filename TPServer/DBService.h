#pragma once

#include "TSingleton.h"
#include "TPResult.h"
#include <windows.h>

#include <sql.h>
#include <sqlext.h>

class DBService : public TSingleton<DBService>
{
public:
	TPResult* LoginUser(const wchar_t* userId, const wchar_t* password);
	TPResult* LoadUserInfo(const wchar_t* userId);

private:
	SQLHDBC hDbc;

	bool CheckDBC();
	SQLHSTMT StartStmt();
	void ENDStmt(const SQLHSTMT& hStmt);	
};