#pragma once

namespace framework
{
	class cStage_CharacterSelect : public cStage
	{
	public:
		cStage_CharacterSelect();
		~cStage_CharacterSelect();

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

	private:
		LPD3DXSPRITE m_sprite;
		cTestScene* m_scene;
		cButton* m_button1;

		u_short usCount;
		bool m_restart;
	};
}