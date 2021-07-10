#include "SQLServiceUser.h"
#include "../Util/TPDefine.h"

ObjUser* SQLServiceUser::GetUserObj(const SQLHSTMT& hStmt, const wchar_t* const userId)
{
	auto sql = 
		"SELECT u.user_id, u.password, t.location_x, t.location_y, t.location_z, t.rotation_x, t.rotation_y, t.rotation_z "
		"FROM user AS u "
		"LEFT JOIN user_transform AS t "
		"ON u.user_id = t.user_id "
		"WHERE u.user_id = ?"
	;

	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
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

	SQLWCHAR cUserId[SIZE_USER_USER_ID] = L"", cPassword[SIZE_USER_PASSWORD] = L"";
	SQLLEN cUserIdLen = 0, cPasswordLen = 0;
	SQLDOUBLE cLocationX = 0.0, cLocationY = 0.0, cLocationZ = 0.0, cRotationX = 0.0, cRotationY = 0.0, cRotationZ = 0.0;

	SQLBindCol(hStmt, 1, SQL_WCHAR, cUserId, SIZE_USER_USER_ID, &cUserIdLen);
	SQLBindCol(hStmt, 2, SQL_WCHAR, cPassword, SIZE_USER_PASSWORD, &cPasswordLen);
	SQLBindCol(hStmt, 3, SQL_DOUBLE, &cLocationX, 0, NULL);
	SQLBindCol(hStmt, 4, SQL_DOUBLE, &cLocationY, 0, NULL);
	SQLBindCol(hStmt, 5, SQL_DOUBLE, &cLocationZ, 0, NULL);
	SQLBindCol(hStmt, 6, SQL_DOUBLE, &cRotationX, 0, NULL);
	SQLBindCol(hStmt, 7, SQL_DOUBLE, &cRotationY, 0, NULL);
	SQLBindCol(hStmt, 8, SQL_DOUBLE, &cRotationZ, 0, NULL);

	if (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		auto castedUserId = reinterpret_cast<wchar_t*>(cUserId);
		auto castedPassword = reinterpret_cast<wchar_t*>(cPassword);
		auto objUser = new ObjUser(castedUserId, castedPassword);
		auto compTransform = new CompUserTransform(
			{ static_cast<float>(cLocationX), static_cast<float>(cLocationY), static_cast<float>(cLocationZ) },
			{ static_cast<float>(cRotationX), static_cast<float>(cRotationY), static_cast<float>(cRotationZ) }
		);
		auto compCondition = new CompUserCondition();
		objUser->SetCompTransform(compTransform);
		objUser->SetCompCondition(compCondition);
		return objUser;
	}

	return nullptr;
}

ObjUser* SQLServiceUser::GetUserObjOnly(const SQLHSTMT& hStmt, const wchar_t* const userId)
{
	auto sql = "SELECT user_id, password FROM user WHERE user_id = ?";
	
	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
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

	SQLWCHAR cUserId[SIZE_USER_USER_ID] = L"", cPassword[SIZE_USER_PASSWORD] = L"";
	SQLLEN cUserIdLen = 0, cPasswordLen = 0;

	SQLBindCol(hStmt, 1, SQL_WCHAR, cUserId, SIZE_USER_USER_ID, &cUserIdLen);
	SQLBindCol(hStmt, 2, SQL_WCHAR, cPassword, SIZE_USER_PASSWORD, &cPasswordLen);

	if (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		auto castedUserId = reinterpret_cast<wchar_t*>(cUserId);
		auto castedPassword = reinterpret_cast<wchar_t*>(cPassword);
		return new ObjUser(castedUserId, castedPassword);
	}

	return nullptr;
}

CompUserTransform* SQLServiceUser::GetUserTransform(const SQLHSTMT& hStmt, const wchar_t* const userId)
{
	auto sql = "SELECT location_x, location_y, location_z, rotation_x, rotation_y, rotation_z FROM user_transform WHERE user_id = ?";

	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
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

	SQLDOUBLE cLocationX = 0.0, cLocationY = 0.0, cLocationZ = 0.0, cRotationX = 0.0, cRotationY = 0.0, cRotationZ = 0.0;
	
	SQLBindCol(hStmt, 1, SQL_DOUBLE, &cLocationX, 0, NULL);
	SQLBindCol(hStmt, 2, SQL_DOUBLE, &cLocationY, 0, NULL);
	SQLBindCol(hStmt, 3, SQL_DOUBLE, &cLocationZ, 0, NULL);
	SQLBindCol(hStmt, 4, SQL_DOUBLE, &cRotationX, 0, NULL);
	SQLBindCol(hStmt, 5, SQL_DOUBLE, &cRotationY, 0, NULL);
	SQLBindCol(hStmt, 6, SQL_DOUBLE, &cRotationZ, 0, NULL);

	if (SQLFetch(hStmt) != SQL_NO_DATA)
	{
		return new CompUserTransform(
			{ static_cast<float>(cLocationX), static_cast<float>(cLocationY), static_cast<float>(cLocationZ) },
			{ static_cast<float>(cRotationX), static_cast<float>(cRotationY), static_cast<float>(cRotationZ) }
		);
	}
	return nullptr;
}

ObjUser* SQLServiceUser::InsertUser(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const wchar_t* const password)
{
	auto objUser = InsertUserOnly(hDbc, hStmt, userId, password);
	if (!objUser)
	{
		return nullptr;
	}

	auto compTransform = SQLServiceUser::GetInstance().InsertUserTransform(hDbc, hStmt, userId, DEFAULT_USER_LOCATION, DEFAULT_USER_ROTATION);
	if (!compTransform)
	{
		SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_ROLLBACK);
		delete objUser;
		return nullptr;
	}

	auto compCondition = new CompUserCondition();

	objUser->SetCompTransform(compTransform);
	objUser->SetCompCondition(compCondition);

	if (SQLEndTran(SQL_HANDLE_DBC, hDbc, SQL_COMMIT) != SQL_SUCCESS)
	{
		delete objUser;
		return nullptr;
	}

	return objUser;
}

ObjUser* SQLServiceUser::InsertUserOnly(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const wchar_t* const password)
{
	auto sql = "INSERT INTO user (user_id, password) VALUES (?, ?)";

	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
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

	auto castedUserId = reinterpret_cast<wchar_t*>(pUserId);
	auto castedPassword = reinterpret_cast<wchar_t*>(pPassword);
	return new ObjUser(castedUserId, castedPassword);
}

CompUserTransform* SQLServiceUser::InsertUserTransform(const SQLHDBC& hDbc, const SQLHSTMT& hStmt, const wchar_t* const userId, const Vector3 location, const Vector3 rotation)
{
	auto sql = "INSERT INTO user_transform (user_id, location_x, location_y, location_z, rotation_x, rotation_y, rotation_z) VALUES (?, ?, ?, ?, ?, ?, ?)";

	if (SQLPrepare(hStmt, (SQLCHAR*)sql, SQL_NTS) != SQL_SUCCESS)
	{
		return nullptr;
	}

	SQLWCHAR pUserId[SIZE_USER_USER_ID] = L"";
	SQLDOUBLE pLocationX = 0, pLocationY = 0, pLocationZ = 0, pRotationX = 0, pRotationY = 0, pRotationZ = 0;

	SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_WCHAR, SQL_WCHAR, SIZE_USER_USER_ID, 0, pUserId, 0, NULL);	
	SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &pLocationX, 0, NULL);
	SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &pLocationY, 0, NULL);
	SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &pLocationZ, 0, NULL);
	SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &pRotationX, 0, NULL);
	SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &pRotationY, 0, NULL);
	SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_DOUBLE, SQL_DOUBLE, 0, 0, &pRotationZ, 0, NULL);

	wcscpy_s(reinterpret_cast<wchar_t*>(pUserId), SIZE_USER_USER_ID, userId);
	pLocationX = location.x;
	pLocationY = location.y;
	pLocationZ = location.z;
	pRotationX = rotation.x;
	pRotationY = rotation.y;
	pRotationZ = rotation.z;

	if (SQLExecute(hStmt) != SQL_SUCCESS)
	{
		return nullptr;
	}

	return new CompUserTransform(
		{ static_cast<float>(pLocationX), static_cast<float>(pLocationY), static_cast<float>(pLocationZ) },
		{ static_cast<float>(pRotationX), static_cast<float>(pRotationY), static_cast<float>(pRotationZ) }
	);
}
