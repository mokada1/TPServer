#pragma once

typedef unsigned long ULONG;
typedef unsigned long DWORD;

#define PACKET_HEAD_SIZE 2
#define PACKET_END_SIZE 2
#define MAX_BUFF_SIZE 4096
#define BUFF_SIZE_IP 32
#define ERR_MSG_SIZE 256
#define SERVER_IP_SIZE 15

#define OP_ServerToClient 1
#define OP_ClientToServer 2

#define SIZE_USER_USER_ID 45
#define SIZE_USER_PASSWORD 45

static const wchar_t* FAIL_DBCONNECT_OR_ALLOC_HANDLE = L"��� ���� �Ǵ� �غ� �۾� ����";
static const wchar_t* CORRECT_PASSWORD = L"�н����� ��ġ";
static const wchar_t* INCORRECT_PASSWORD = L"�н����� ����ġ";
static const wchar_t* SUCCESS_REGISTER_USER = L"���� ��� ����";
static const wchar_t* FAIL_REGISTER_USER = L"���� ��� ����";
static const wchar_t* SUCCESS_INSERT_DATA = L"������ ���� ����";
static const wchar_t* FAIL_INSERT_DATA = L"������ ���� ����";
static const wchar_t* SUCCESS_GET_DATA = L"������ ��ȸ ����";
static const wchar_t* FAIL_GET_DATA = L"������ ��ȸ ����";
static const wchar_t* FAIL_ADD_OBJ_USER_GAME_ROOM = L"���� �� ���� ����";
static const wchar_t* SUCCESS_ADD_OBJ_USER_GAME_ROOM = L"���� �� ���� ����";
static const wchar_t* DELETE_OBJ_USER_GAME_ROOM = L"���� �� ��Ż";