#pragma once

#include "TSingleton.h"
#include <windows.h>

#include <sql.h>
#include <sqlext.h>

class DBServer : public TSingleton<DBServer>
{
public:
	bool GetIsConnected();
	SQLHDBC GetHDBC();
	bool DBConnect();
	void DBDisConnect();	

private:
	bool isConnected = false;

	SQLHENV hEnv = nullptr;
	SQLHDBC hDbc = nullptr;

	SQLWCHAR* ODBC_Name = (SQLWCHAR*)L"mysql_odbc_64";
	SQLWCHAR* ODBC_ID = (SQLWCHAR*)L"root";
	SQLWCHAR* ODBC_PW = (SQLWCHAR*)L"1234";
};