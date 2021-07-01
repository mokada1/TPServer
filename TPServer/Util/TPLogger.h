#pragma once

#include "TSingleton.h"

#include <stdio.h>

class TPLogger : public TSingleton<TPLogger>
{
public:
	void PrintLog(const char* const format)
	{
		puts(format);
	}
	void PrintLog(const wchar_t* const format)
	{
		_putws(format);
	}

	template<typename ... Args>
	void PrintLog(const char* const format, Args ... args)
	{
		const size_t bufferSize = _snprintf(nullptr, 0, format, args ...) + 1ULL;
		auto buffer = new char[bufferSize];
		sprintf_s(buffer, bufferSize, format, args ...);
		puts(buffer);
		delete[] buffer;
	}

	template<typename ... Args>
	void PrintLog(const wchar_t* const format, Args ... args)
	{
		const size_t bufferSize = _snwprintf(nullptr, 0, format, args ...) + 1ULL;
		auto buffer = new wchar_t[bufferSize];
		swprintf_s(buffer, bufferSize, format, args ...);
		_putws(buffer);
		delete[] buffer;
	}
};