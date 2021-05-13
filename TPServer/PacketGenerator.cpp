#include "PacketGenerator.h"
#include "TP_generated.h"
#include "TPDefine.h"
#include "TPUtil.h"

Packet PacketGenerator::CreateError(const wchar_t* message)
{
	char hMessage[MAX_DATA_SIZE];
	TPUtil::GetInstance().WCharToChar(hMessage, MAX_DATA_SIZE, message);
	
	char datas[MAX_DATA_SIZE];
	PROTOCOL header = PROTOCOL::TP_ERROR;
	uint16_t headerInt16 = static_cast<uint16_t>(header);
	datas[0] = headerInt16 & 0xFF;
	datas[1] = headerInt16 >> 8;

	flatbuffers::FlatBufferBuilder builder;
	auto offsetMessage = builder.CreateString(hMessage);
	builder.Finish(CreateTP_ERROR(builder, offsetMessage));
	auto bp = builder.GetBufferPointer();
	auto bSize = builder.GetSize();
	const int BODY_SIZE = bSize;
	memcpy(&datas[HEAD_SIZE], bp, BODY_SIZE);

	const int DATA_SIZE = HEAD_SIZE + BODY_SIZE;
	auto body = &datas[HEAD_SIZE];

	return Packet(datas, DATA_SIZE, header, body);
}