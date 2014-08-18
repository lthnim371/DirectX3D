#pragma once

namespace framework
{
	class cStage_NetworkLoading : public cStage
	{
	public:
		cStage_NetworkLoading();
		~cStage_NetworkLoading();

	//	virtual void Init() override;
	//	virtual void Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
	//	virtual void Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2) override;
		virtual void Init(const int nId, tagIngameInfo* sIngameInfo) override;
		virtual void Input(const float elapseTime) override;
		virtual void Update(const float elapseTime) override;
		virtual void Render(const float elapseTime) override;
		virtual void Release() override;

		bool MessageProc( UINT message, WPARAM wParam, LPARAM lParam);

		bool PacketSend();
		bool PacketReceive(OUT network::AccessProtocol& out, const u_int uDelay = 10);

	private:
		LPD3DXSPRITE m_sprite;
		cTestScene* m_scene;
		cButton* m_button1;
		cButton* m_button2;

		network::AccessProtocol m_acceseInfo;
		float fTick;

//		u_short usCount;
	};
}