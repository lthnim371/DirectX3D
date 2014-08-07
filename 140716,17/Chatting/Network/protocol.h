//resive : 받는 것
//send : 보내는 것


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
//		int size;  //패킷 길이가 어는정도가 되는지 파악하는 용도(필드)
	};


	struct sLoginProtocol
	{
		sPacketHeader header;
		char name[ 32];
		//char pass[ 8];
	};


	struct sChatProtocol
	{
		sPacketHeader header;
		char msg[ 64];
	};
#pragma pack()  //끝날때 다시 기본으로 되돌리기

}