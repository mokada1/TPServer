#include "DBServer.h"
#include "TPError.h"
#include <iostream>

using namespace std;

bool DBServer::GetIsConnected()
{
    return this->isConnected;
}

SQLHDBC DBServer::GetHDBC()
{
	return this->hDbc;
}

bool DBServer::DBConnect()
{
    if (isConnected)
    {
        return false;
    }

    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
    {
        TPError::GetInstance().PrintError(L"SQLAllocHandle Error");
        return false;
    }
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER*)SQL_OV_ODBC3, 0) != SQL_SUCCESS)
    {
        TPError::GetInstance().PrintError(L"SQLSetEnvAttr Error");
        return false;
    }
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
    {
        TPError::GetInstance().PrintError(L"SQLAllocHandle Error");
        return false;
    }
    if (SQLSetConnectAttr(hDbc, SQL_AUTOCOMMIT, (SQLPOINTER)FALSE, 0) != SQL_SUCCESS)
    {
        TPError::GetInstance().PrintError(L"SQLSetConnectAttr Error");
        return false;
    }
    if (SQLConnect(hDbc, ODBC_Name, SQL_NTS, ODBC_ID, SQL_NTS, ODBC_PW, SQL_NTS) != SQL_SUCCESS)
    {
        TPError::GetInstance().PrintError(L"SQLConnect Error");
        return false;
    }

    isConnected = true;
    return true;
}

void DBServer::DBDisConnect()
{
    if (!isConnected)
    {
        return;
    }
    
    if (hDbc) SQLDisconnect(hDbc);
    if (hDbc) SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    if (hEnv) SQLFreeHandle(SQL_HANDLE_ENV, hEnv);

    isConnected = false;
}