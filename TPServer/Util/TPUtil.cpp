#include "TPUtil.h"
#include <stdio.h>
#include <Windows.h>
#include <chrono>
#include <time.h>
#include <ctime>
#include <iostream>
#include <string>

using namespace std;

using chrono::duration_cast;
using chrono::milliseconds;
using chrono::seconds;
using chrono::system_clock;

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

void TPUtil::WCharToMultiByte(char* buffer, size_t bufferSize, const wchar_t* const source)
{
	auto wBuffer = new wchar_t[bufferSize];
	wcscpy_s(wBuffer, bufferSize, source);
	int len = WideCharToMultiByte(CP_ACP, 0, wBuffer, -1, NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wBuffer, -1, buffer, len, NULL, NULL);
	delete[] wBuffer;
}

void TPUtil::MultiByteToWChar(wchar_t* buffer, size_t bufferSize, const char* const source)
{
	auto hBuffer = new char[bufferSize];
	strcpy_s(hBuffer, bufferSize, source);
	int len = MultiByteToWideChar(CP_ACP, 0, hBuffer, bufferSize, NULL, NULL);
	MultiByteToWideChar(CP_ACP, 0, hBuffer, bufferSize, buffer, len);
	delete[] hBuffer;
}

long long TPUtil::TimeSinceEpochMs()
{
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

long long TPUtil::TimeSinceEpochSec()
{
	return duration_cast<seconds>(system_clock::now().time_since_epoch()).count();
}

const char* TPUtil::EnumToString(PROTOCOL protocol)
{
	switch (protocol)
	{
	case PROTOCOL::TP_ERROR: return "TP_ERROR";
	case PROTOCOL::REQ_LOGIN: return "REQ_LOGIN";
	case PROTOCOL::REQ_MOVE: return "REQ_MOVE";
	case PROTOCOL::REQ_LOCATION_SYNC: return "REQ_LOCATION_SYNC";
	case PROTOCOL::REQ_ROUND_TRIP_TIME: return "REQ_ROUND_TRIP_TIME";
	case PROTOCOL::RES_LOGIN: return "RES_LOGIN";
	case PROTOCOL::RES_ROUND_TRIP_TIME: return "RES_ROUND_TRIP_TIME";
	case PROTOCOL::BCAST_ENTER_GAME_ROOM: return "BCAST_ENTER_GAME_ROOM";
	case PROTOCOL::BCAST_EXIT_GAME_ROOM: return "BCAST_EXIT_GAME_ROOM";
	case PROTOCOL::BCAST_MOVE: return "BCAST_MOVE";
	case PROTOCOL::BCAST_LOCATION_SYNC: return "BCAST_LOCATION_SYNC";
	case PROTOCOL::END_OF_PACKET: return "END_OF_PACKET";
	default: return nullptr;
	}
	return nullptr;
}
