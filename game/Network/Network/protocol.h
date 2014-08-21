//resive : 받는 것
//send : 보내는 것


#pragma once

#include "../../Common/common.h"

namespace network
{

	namespace PROTOCOL
	{
		enum TYPE
		{
			ACCESS, INGAME, INGAMEEND,
		};

		enum STATE
		{
			NONE, NORMAL, ROTATION, LEFTROTATION, RIGHTROTATION,
			FORWARD, BACKWARD, LEFTWARD, RIGHTWARD,
			DASH, GUARD, GUARD_BE_HIT,
			JUMP, FRONTJUMP, BACKJUMP, LEFTJUMP, RIGHTJUMP,
			LATTACK, RATTACK, BEHIT, 
		};
	}

#pragma pack(4)  //가로 안에 넣은 크기로 데이터를 정렬하겠다는 의미
	struct sPacketHeader
	{
		PROTOCOL::TYPE protocol;
//		int size;  //패킷 길이가 어는정도가 되는지 파악하는 용도(필드)
	};

	typedef struct tagInfoProtocol
	{
		sPacketHeader header;
		PROTOCOL::STATE state1;
		PROTOCOL::STATE state2;
		short nId;
		POINT ptMouse;
	//	bool bAccess;
		common::Vector3 camLook;
		common::Vector3 camPos;
		common::Matrix44 character;
		bool bResult;
		short sWinner;
	} InfoProtocol;

	typedef struct tagAccessProtocol
	{
		sPacketHeader header;
		int nId;
		bool bAccess;
	} AccessProtocol;

	typedef struct tagResultProtocol
	{
		sPacketHeader header;
		int nId;
		bool bResult;
	} ResultProtocol;

#pragma pack()  //끝날때 다시 기본으로 되돌리기

}