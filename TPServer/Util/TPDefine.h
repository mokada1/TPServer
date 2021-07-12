#pragma once

typedef unsigned long ULONG;
typedef unsigned long DWORD;

#define PACKET_HEAD_SIZE 2
#define PACKET_END_SIZE 2
#define MAX_BUFF_SIZE 1024
#define BUFF_SIZE_IP 32
#define ERR_MSG_SIZE 256
#define SERVER_IP_SIZE 15

#define MAX_OVERLAPPED_ENTRY 32
#define OP_SERVER_TO_CLIENT 1
#define OP_CLIENT_TO_SERVER 2

#define SIZE_USER_USER_ID 45
#define SIZE_USER_PASSWORD 45

#define MAX_RTT_COUNT 10
#define MAX_RTT_MS 1000

static const wchar_t* FAIL_DBCONNECT_OR_ALLOC_HANDLE = L"디비 연결 또는 준비 작업 실패";
static const wchar_t* CORRECT_PASSWORD = L"패스워드 일치";
static const wchar_t* INCORRECT_PASSWORD = L"패스워드 불일치";
static const wchar_t* SUCCESS_REGISTER_USER = L"유저 등록 성공";
static const wchar_t* FAIL_REGISTER_USER = L"유저 등록 실패";
static const wchar_t* SUCCESS_INSERT_DATA = L"데이터 삽입 성공";
static const wchar_t* FAIL_INSERT_DATA = L"데이터 삽입 실패";
static const wchar_t* SUCCESS_GET_DATA = L"데이터 조회 성공";
static const wchar_t* FAIL_GET_DATA = L"데이터 조회 실패";
static const wchar_t* FAIL_ADD_OBJ_USER_GAME_ROOM = L"유저 방 참가 실패";
static const wchar_t* SUCCESS_ADD_OBJ_USER_GAME_ROOM = L"유저 방 참가 성공[%d]";
static const wchar_t* DELETE_OBJ_USER_GAME_ROOM = L"유저 방 이탈[%d]";
static const wchar_t* DUPLICATE_LOGIN = L"중복 로그인";
static const wchar_t* SERVER_STARTED = L"서버가 시작되었습니다.";
static const wchar_t* NOT_FOUND_SESSION = L"세션 조회 실패";
static const wchar_t* REQ_LOGIN = L"로그인 요청: id[%S] pw[%S]";
static const wchar_t* EXCEED_MAX_RTT_MS = L"최대 RTT 왕복 시간 초과";
static const wchar_t* FAIL_GET_OBJ_USER = L"방 내 유저 조회 실패";
static const wchar_t* FAIL_GET_GAME_ROOM = L"방 조회 실패";
static const wchar_t* NOT_FOUND_HIT_LIST = L"피격 대상을 찾지 못함";
static const char* INVALID_PACKET_SIZE = "INVALID_PACKET_SIZE";
static const char* INVALID_HEADER = "INVALID_HEADER";
static const char* INVALID_END_OF_PACKET = "INVALID_END_OF_PACKET";
static const char* INVALID_OPERATION = "INVALID_OPERATION";
static const char* CONNECTED = "Connected[%d]: %s";
static const char* DISCONNECTED = "Disconnected[%d]: %s";
static const char* WSARECV_ERROR = "WSARecv() Error:%d";
static const char* WSASEND_ERROR = "WSASend() Error:%d";
static const char* WSASTARTUP_ERROR = "WSAStartup() Error!";
static const char* RECV_PACKET_1 = "Recv packet:%s";
static const char* SEND_PACKET_1 = "Send packet:%s";
static const char* RECV_PACKET_2 = "[%d]Recv packet:%s";
static const char* SEND_PACKET_2 = "[%d]Send packet:%s";
static const char* TRANSFERRED_BYTES = "Number of bytes transferred:%d";
static const char* RECEIVED_BYTES = "Number of bytes received:%d";