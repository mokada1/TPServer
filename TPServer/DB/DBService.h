#pragma once

#include "../Util/Unconstructible.h"
#include <windows.h>

#include <sql.h>
#include <sqlext.h>

class TPResult;

class DBService : public Unconstructible
{
public:
	static DBService& GetInstance()
	{
		static DBService* _instance = new DBService();
		return *_instance;
	}

	TPResult* LoginUser(const wchar_t* const userId, const wchar_t* const password);

private:
	DBService() {}

	SQLHDBC hDbc = nullptr;

	bool DBConnect();
	SQLHSTMT DBStart();
	void DBEnd(const SQLHSTMT& hStmt);	
};