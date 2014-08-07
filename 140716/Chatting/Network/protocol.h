
#pragma once


namespace network
{

	namespace PROTOCOL
	{
		enum TYPE
		{
			LOGIN,
			CHATTING,
		};
	}

#pragma pack(4)  //가로 안에 넣은 크기로 데이터를 정렬하겠다는 의미
	struct sPacketHeader
	{
		PROTOCOL::TYPE protocol;
//		int size;
	};


	struct sLoginProtocol
	{
		sPacketHeader header;
		char name[ 32];
		char pass[ 8];
	};


	struct sChatProtocol
	{
		sPacketHeader header;
		char msg[ 64];
	};
#pragma pack()  //끝날때 다시 기본으로 되돌리기

}