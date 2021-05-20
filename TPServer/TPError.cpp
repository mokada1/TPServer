#include "TPError.h"
#include <iostream>

using namespace std;

void TPError::PrintError(const wchar_t* const message)
{
	PrintError(message, false);
}

void TPError::PrintError(const wchar_t* const message, const int code)
{
	auto cated = ConcatMessage(message, code);
	PrintError(cated, true);
}

void TPError::PrintError(const wchar_t* const message, bool isDAllocBuf)
{
	wcout << message << endl;
	if(isDAllocBuf) delete message;
	exit(1);
}

const wchar_t* TPError::ConcatMessage(const wchar_t* const message, const int code)
{
	wchar_t errorCode[10];
	auto codeBufSize = sizeof(errorCode) / sizeof(wchar_t);
	auto errMsgBufSize = wcslen(message) + codeBufSize;
	wchar_t* errorMsg = new wchar_t[errMsgBufSize];
	wcscpy_s(errorMsg, errMsgBufSize, message);
	swprintf_s(errorCode, codeBufSize, L":%d", code);
	wcscat_s(errorMsg, errMsgBufSize, errorCode);
	return errorMsg;
}