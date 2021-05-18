#pragma once

typedef unsigned long ULONG;
typedef unsigned long DWORD;

#define BUFSIZE 1024
#define BUFSIZE_IP 32
#define OP_ServerToClient 1
#define OP_ClientToServer 2

#define PACKET_HEAD_SIZE 2
#define MAX_DATA_SIZE 1024

#define SIZE_USER_USER_ID 45
#define SIZE_USER_PASSWORD 45

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
static const wchar_t* SUCCESS_ADD_OBJ_USER_GAME_ROOM = L"유저 방 참가 성공";
static const wchar_t* DELETE_OBJ_USER_GAME_ROOM = L"유저 방 이탈";