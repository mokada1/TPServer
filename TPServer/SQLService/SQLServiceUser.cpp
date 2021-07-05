#include "SQLServiceUser.h"
#include "../Util/TPDefine.h"

ObjUser* SQLServiceUser::GetUser(const SQLHSTMT& hStmt, const wchar_t* const userId)
{
	auto sql = "SELECT user_id, password FROM user WHERE user_id = ?";
	
	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR sUserId[SIZE_USER_USER_ID] = L"";
	
	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, sUserId, 0, NULL);
	wcscpy_s(reinterpret_cast<wchar_t*>(sUserId), SIZE_USER_USER_ID, userId);

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
	auto sql = "INSERT INTO user (user_id, password) VALUES (?, ?)";

	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR sUserId[SIZE_USER_USER_ID] = L"", pPassword[SIZE_USER_PASSWORD] = L"";

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, sUserId, 0, NULL);
	wcscpy_s(reinterpret_cast<wchar_t*>(sUserId), SIZE_USER_USER_ID, userId);
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

	auto castedUserId = reinterpret_cast<wchar_t*>(sUserId);
	auto castedPassword = reinterpret_cast<wchar_t*>(pPassword);
	return new ObjUser(castedUserId, castedPassword);
}

CompUserTransform* SQLServiceUser::GetUserTransform(const SQLHSTMT& hStmt, const wchar_t* const userId)
{
	auto sql = "SELECT location_x, location_y, location_z, rotation_x, rotation_y, rotation_z FROM user_transform WHERE user_id = ?";

	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR sUserId[SIZE_USER_USER_ID] = L"";

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, sUserId, 0, NULL);
	wcscpy_s(reinterpret_cast<wchar_t*>(sUserId), SIZE_USER_USER_ID, userId);

	if (SQLExecute(hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLDOUBLE locationX = 0.0, locationY = 0.0, locationZ = 0.0, rotationX = 0.0, rotationY = 0.0, rotationZ = 0.0;
	
	SQLBindCol(hStmt, 1, SQL_DOUBLE, &locationX, 0, NULL);
	SQLBindCol(hStmt, 2, SQL_DOUBLE, &locationY, 0, NULL);
	SQLBindCol(hStmt, 3, SQL_DOUBLE, &locationZ, 0, NULL);
	SQLBindCol(hStmt, 4, SQL_DOUBLE, &rotationX, 0, NULL);
	SQLBindCol(hStmt, 5, SQL_DOUBLE, &rotationY, 0, NULL);
	SQLBindCol(hStmt, 6, SQL_DOUBLE, &rotationZ, 0, NULL);

	if (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		return new CompUserTransform(
			{ static_cast<float>(locationX), static_cast<float>(locationY), static_cast<float>(locationZ) },
			{ static_cast<float>(rotationX), static_cast<float>(rotationY), static_cast<float>(rotationZ) }
		);
	}
	return nullptr;
}

CompUserTransform* SQLServiceUser::InsertUserTransform(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const Vector3 location, const Vector3 rotation)
{
	auto sql = "INSERT INTO user_transform (user_id, location_x, location_y, location_z, rotation_x, rotation_y, rotation_z) VALUES (?, ?, ?, ?, ?, ?, ?)";

	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR sUserId[SIZE_USER_USER_ID] = L"";
	SQLDOUBLE locationX = 0, locationY = 0, locationZ = 0, rotationX = 0, rotationY = 0, rotationZ = 0;

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, sUserId, 0, NULL);	
	SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &locationX, 0, NULL);
	SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &locationY, 0, NULL);
	SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &locationZ, 0, NULL);
	SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &rotationX, 0, NULL);
	SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &rotationY, 0, NULL);
	SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &rotationZ, 0, NULL);

	wcscpy_s(reinterpret_cast<wchar_t*>(sUserId), SIZE_USER_USER_ID, userId);
	locationX = location.x;
	locationY = location.y;
	locationZ = location.z;
	rotationX = rotation.x;
	rotationY = rotation.y;
	rotationZ = rotation.z;

	if (SQLExecute(hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}

	if (SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_COMMIT) != SQL_SUCCESS)
	{
		return nullptr;
	}

	return new CompUserTransform(
		{ static_cast<float>(locationX), static_cast<float>(locationY), static_cast<float>(locationZ) },
		{ static_cast<float>(rotationX), static_cast<float>(rotationY), static_cast<float>(rotationZ) }
	);
}
