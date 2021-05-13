#include "TPUtil.h"
#include <stdio.h>

void TPUtil::CharToWChar(wchar_t* buffer, size_t bufferSize, const char* source)
{
	auto len = bufferSize / sizeof(wchar_t);
	swprintf(buffer, len, L"%hs", source);
}

void TPUtil::WCharToChar(char* buffer, size_t bufferSize, const wchar_t* source)
{
	auto len = bufferSize / sizeof(char);
	sprintf_s(buffer, len, "%ws", source);
}
