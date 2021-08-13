#pragma once

#include "../Util/Unconstructible.h"
#include <windows.h>

#include <sql.h>
#include <sqlext.h>

class DBServer : public Unconstructible
{
public:
	static DBServer& GetInstance()
	{
		static DBServer* _instance = new DBServer();
		return *_instance;
	}

	bool GetIsConnected();
	SQLHDBC GetHDBC();
	bool DBConnect();
	void DBDisconnect();	

private:
	DBServer() {}

	bool isConnected = false;

	SQLHENV hEnv = nullptr;
	SQLHDBC hDbc = nullptr;
};