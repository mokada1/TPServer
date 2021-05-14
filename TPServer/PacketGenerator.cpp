#include "PacketGenerator.h"
#include "TP_generated.h"
#include "TPDefine.h"
#include "TPUtil.h"

Packet* PacketGenerator::CreateError(const wchar_t* message)
{
	char hMessage[MAX_DATA_SIZE];
	TPUtil::GetInstance().WCharToChar(hMessage, MAX_DATA_SIZE, message);
	
	char datas[MAX_DATA_SIZE];
	PROTOCOL header = PROTOCOL::TP_ERROR;
	SetHeaderOfBuff(datas, header);

	flatbuffers::FlatBufferBuilder builder;
	auto offsetMessage = builder.CreateString(hMessage);
	builder.Finish(CreateTB_Error(builder, offsetMessage));
	auto bp = builder.GetBufferPointer();
	auto bSize = builder.GetSize();
	const int BODY_SIZE = bSize;
	memcpy(&datas[HEAD_SIZE], bp, BODY_SIZE);

	const int DATA_SIZE = HEAD_SIZE + BODY_SIZE;
	auto body = &datas[HEAD_SIZE];

	return new Packet(datas, DATA_SIZE, header, body);
}

Packet* PacketGenerator::CreateResLogin(const ObjUser& objUser)
{
	char datas[MAX_DATA_SIZE];
	PROTOCOL header = PROTOCOL::RES_LOGIN;
	SetHeaderOfBuff(datas, header);

	flatbuffers::FlatBufferBuilder builder;
	builder.Finish(CreateTB_ResLogin(builder, objUser.Serialize(builder)));
	auto bp = builder.GetBufferPointer();
	auto bSize = builder.GetSize();
	const int BODY_SIZE = bSize;
	memcpy(&datas[HEAD_SIZE], bp, BODY_SIZE);

	const int DATA_SIZE = HEAD_SIZE + BODY_SIZE;
	auto body = &datas[HEAD_SIZE];

	return new Packet(datas, DATA_SIZE, header, body);
}

void PacketGenerator::SetHeaderOfBuff(char* buffer, PROTOCOL header)
{
	uint16_t protoInt16 = static_cast<uint16_t>(header);
	buffer[0] = protoInt16 & 0xFF;
	buffer[1] = protoInt16 >> 8;
}
