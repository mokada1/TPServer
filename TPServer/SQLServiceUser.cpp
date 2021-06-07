#include "SQLServiceUser.h"
#include "TPDefine.h"

ObjUser* SQLServiceUser::GetUser(const SQLHSTMT& hStmt, const wchar_t* const userId)
{
	auto sql = L"SELECT user_id, password FROM user WHERE user_id = ?";
	
	if (SQLPrepare(hStmt, (SQLWCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR pUserId[SIZE_USER_USER_ID] = L"";
	
	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, pUserId, 0, NULL);
	wcscpy_s(reinterpret_cast<wchar_t*>(pUserId), SIZE_USER_USER_ID, userId);

	if (SQLExecute(hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR bUserId[SIZE_USER_USER_ID] = L"", bPassword[SIZE_USER_PASSWORD] = L"";
	SQLLEN _bUserId = 0, _bPassword = 0;

	SQLBindCol(hStmt, 1, SQL_WCHAR, bUserId, SIZE_USER_USER_ID, &_bUserId);
	SQLBindCol(hStmt, 2, SQL_WCHAR, bPassword, SIZE_USER_PASSWORD, &_bPassword);

	if (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		auto castedUserId = reinterpret_cast<wchar_t*>(bUserId);
		auto castedPassword = reinterpret_cast<wchar_t*>(bPassword);
		return new ObjUser(castedUserId, castedPassword);
	}

	return nullptr;
}

ObjUser* SQLServiceUser::InsertUser(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const wchar_t* const password)
{
	auto sql = L"INSERT INTO user (user_id, password) VALUES (?, ?)";

	if (SQLPrepare(hStmt, (SQLWCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR pUserId[SIZE_USER_USER_ID] = L"", pPassword[SIZE_USER_PASSWORD] = L"";

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, pUserId, 0, NULL);
	wcscpy_s(reinterpret_cast<wchar_t*>(pUserId), SIZE_USER_USER_ID, userId);
	SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_PASSWORD, 0, pPassword, 0, NULL);
	wcscpy_s(reinterpret_cast<wchar_t*>(pPassword), SIZE_USER_PASSWORD, password);

	if (SQLExecute(hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}

	if (SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_COMMIT) != SQL_SUCCESS)
	{
		return nullptr;
	}

	auto castedUserId = reinterpret_cast<wchar_t*>(pUserId);
	auto castedPassword = reinterpret_cast<wchar_t*>(pPassword);
	return new ObjUser(castedUserId, castedPassword);
}

CompUserLocation* SQLServiceUser::GetUserLocation(const SQLHSTMT& hStmt, const wchar_t* const userId)
{
	auto sql = L"SELECT x, y, z FROM user_location WHERE user_id = ?";

	if (SQLPrepare(hStmt, (SQLWCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR pUserId[SIZE_USER_USER_ID] = L"";

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, pUserId, 0, NULL);
	wcscpy_s(reinterpret_cast<wchar_t*>(pUserId), SIZE_USER_USER_ID, userId);

	if (SQLExecute(hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLDOUBLE bX = 0.0, bY = 0.0, bZ = 0.0;
	
	SQLBindCol(hStmt, 1, SQL_DOUBLE, &bX, 0, NULL);
	SQLBindCol(hStmt, 2, SQL_DOUBLE, &bY, 0, NULL);
	SQLBindCol(hStmt, 3, SQL_DOUBLE, &bZ, 0, NULL);

	if (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		return new CompUserLocation(bX, bY, bZ);
	}
	return nullptr;
}

CompUserLocation* SQLServiceUser::InsertUserLocation(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const Vector3 location)
{
	auto sql = L"INSERT INTO user_location (user_id, x, y, z) VALUES (?, ?, ?, ?)";

	if (SQLPrepare(hStmt, (SQLWCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR pUserId[SIZE_USER_USER_ID] = L"";
	SQLFLOAT pX = location.x, pY = location.y, pZ = location.z;

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, pUserId, 0, NULL);
	wcscpy_s(reinterpret_cast<wchar_t*>(pUserId), SIZE_USER_USER_ID, userId);
	SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pX, 0, NULL);
	SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pY, 0, NULL);
	SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_FLOAT, 0, 0, &pZ, 0, NULL);

	if (SQLExecute(hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}

	if (SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_COMMIT) != SQL_SUCCESS)
	{
		return nullptr;
	}

	return new CompUserLocation(pX, pY, pZ);
}
