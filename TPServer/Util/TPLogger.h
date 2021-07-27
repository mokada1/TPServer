#pragma once

#include "Unconstructible.h"

#include <stdio.h>

class TPLogger : public Unconstructible
{
public:
	static TPLogger& GetInstance()
	{
		static TPLogger* _instance = new TPLogger();
		return *_instance;
	}

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

private:
	TPLogger() {}
};