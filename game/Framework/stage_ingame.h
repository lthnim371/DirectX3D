#pragma once

//#include "../Network/Network/network.h"
//using namespace network;

namespace framework
{
	
	class cStage_Ingame : public cStage
	{
	public:
		cStage_Ingame();
		~cStage_Ingame();

	//	virtual void Init() override;
	//	virtual void Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
		virtual void Init(const int nId) override;
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;

	protected:
		bool PacketSend(const network::PROTOCOL::TYPE nState1, const network::PROTOCOL::TYPE nState2, const POINT ptMouse);
	//	bool PacketSend(const int nState1, const int nState2, const POINT ptMouse);
	//	bool PacketSend(const network::InfoProtocol packetInfo);
		bool PacketReceive(OUT network::InfoProtocol& packetInfo);

	private:
		POINT m_currMouse;
		POINT m_prevMouse;
//		bool m_bMouse;
		float fTick1;
		float fTick2;

		graphic::cCharacter* character1;
		graphic::cCharacter* character2;
		
		bool m_access;
	//	short m_id;
		network::InfoProtocol m_packetInfo;
	};
}